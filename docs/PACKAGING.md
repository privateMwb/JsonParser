# Packaging: Local Verification

Notes for re-verifying the vcpkg port and Conan recipe after changes
(e.g. a new release, editing `portfile.cmake`, or editing
`recipes/jsonpro/all/conanfile.py`). Scoped to what isn't obvious
from the files themselves — not a general build tutorial.

## Termux-specific quirks

Both package managers assume a "normal" Linux/CI environment.
Termux's Android-flavored profile detection breaks several of their
default assumptions. None of this applies on real CI (GitHub Actions
Ubuntu/Windows runners, a real Linux box, etc.) — it's purely a
Termux problem.

- **`vcpkg`** — not installed on Termux; bootstrapping it needs a full
  C++ toolchain build and has known portability issues on bionic libc.
  Skip it. `sha512sum` produces the identical hash `vcpkg hash` would,
  and a plain `cmake --install` reproduces what `vcpkg_cmake_configure`
  + `vcpkg_cmake_install` do under the hood — that's enough to verify
  the port without the binary.

- **`conan profile detect`** auto-detects `os=Android`, which makes
  `CMakeToolchain` demand `tools.android:ndk_path` — Termux isn't
  actually cross-compiling, so override: `-s os=Linux`.

- **`compiler.libcxx`** auto-detects as `libstdc++11`, but Termux's
  Clang actually ships `libc++`. Wrong value forces `-stdlib=libstdc++`,
  which doesn't exist here and breaks every standard header include.
  Override: `-s compiler.libcxx=libc++` (or bake it into
  `~/.conan2/profiles/default` so it doesn't need repeating per command).

- **`cmake.configure()` without the `JSONPRO_BUILD_*` overrides
  silently configures the entire top-level project**, not just the
  library. JsonPro's `CMakeLists.txt` also defines `benchmarks/`,
  `tests/`, `tools/`, and `examples/` targets, and `benchmarks/`
  `FetchContent`-clones nlohmann/json at configure time. On Termux's
  mobile connection that clone can sit for a long time with zero CMake
  output, right after `-- Detecting CXX compile features - done` —
  looks exactly like a hang, not a slow clone. Always pass
  `-DJSONPRO_BUILD_TESTS=OFF -DJSONPRO_BUILD_BENCHMARKS=OFF
  -DJSONPRO_BUILD_TOOLS=OFF -DJSONPRO_BUILD_EXAMPLES=OFF` (already
  baked into `conanfile.py`'s `build()` and `portfile.cmake`'s
  `vcpkg_cmake_configure`, but relevant if configuring manually
  outside either of those).

- **The release tarball extracts to `JsonParser-<version>`, not
  `JsonPro-<version>`** — the GitHub repo is `JsonParser`; the CMake
  project/library target inside it is `JsonPro`. `cd` into whatever
  the tarball actually produced, not the library name.

- **Host/build profile mismatch** — overriding `os` on the host profile
  only (`-s os=Linux`) while the build profile still says `os=Android`
  confuses Conan into partial cross-compile behavior (breaks include
  path resolution even for basic headers like `<cassert>`). If sticking
  with `CMakeToolchain`, override both: add `-s:b os=Linux` too.

- **Simpler alternative** — if `CMakeToolchain` keeps fighting the
  Android detection, drop it entirely. Use `[generators]` with just
  `CMakeDeps` in the consumer `conanfile.txt`, then build with a plain
  `cmake -B build -DCMAKE_PREFIX_PATH=$(pwd)` instead of the generated
  preset. Trade-off: `CMakeToolchain` normally sets `CMAKE_CXX_STANDARD`
  automatically from `compiler.cppstd`; without it, the consumer's own
  `CMakeLists.txt` must set the C++ standard explicitly, or every
  C++23 construct JsonPro uses fails to compile.

- **`CMAKE_BUILD_TYPE` matters even without a toolchain file** —
  Conan's `CMakeDeps`-generated `INTERFACE_INCLUDE_DIRECTORIES` are
  gated behind `$<$<CONFIG:Release>:...>` generator expressions. If
  `CMAKE_BUILD_TYPE` isn't set to match (e.g. left empty), the include
  directories silently evaluate to nothing — no error, just a
  "file not found" on every header. Always pass
  `-DCMAKE_BUILD_TYPE=Release` explicitly when testing.

## Verifying the vcpkg port

1. Extract the release tarball and install to a staging prefix with
   the same flags `portfile.cmake` uses:
   ```bash
   tar xzf v<version>.tar.gz && cd JsonParser-<version>
   cmake -B build-install \
     -DCMAKE_INSTALL_PREFIX=$HOME/staging \
     -DJSONPRO_BUILD_TESTS=OFF \
     -DJSONPRO_BUILD_BENCHMARKS=OFF \
     -DJSONPRO_BUILD_TOOLS=OFF \
     -DJSONPRO_BUILD_EXAMPLES=OFF
   cmake --build build-install
   cmake --install build-install
   ```
2. Confirm the CMake package files exist:
   ```bash
   find $HOME/staging -name "*.cmake"
   ```
   Expect `JsonProConfig.cmake`, `JsonProConfigVersion.cmake`,
   `JsonProTargets.cmake` (plus a per-config `JsonProTargets-<config>.cmake`)
   under `lib/cmake/JsonPro/`.
3. In a **separate** directory, with a minimal consumer project
   (`find_package(JsonPro CONFIG REQUIRED)` +
   `target_link_libraries(test PRIVATE JsonPro::JsonPro)`):
   ```bash
   cmake -B build -DCMAKE_PREFIX_PATH=$HOME/staging
   cmake --build build
   ./build/test
   ```

## Verifying the Conan recipe

1. From repo root:
   ```bash
   conan create recipes/jsonpro/all --version <version> \
     -s compiler.cppstd=23 -s os=Linux
   ```
   Watch for: `validate()` passing (C++23 floor check), `LICENSE`
   landing in `package(): Packaged ... LICENSE`, and (unlike a
   header-only recipe) a **distinct `package_id` per settings
   combination** — JsonPro compiles real `.cpp` files, so a Debug
   build and a Release build must not resolve to the same cached
   package.
2. In a **separate** directory, with `conanfile.txt`:
   ```
   [requires]
   jsonpro/<version>

   [generators]
   CMakeDeps
   CMakeToolchain
   ```
   plus the same minimal consumer `CMakeLists.txt`/`main.cpp` as above:
   ```bash
   conan install . -s compiler.cppstd=23 --build=missing
   cmake -B build -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
   cmake --build build
   ./build/test
   ```

Both should build and run with exit code 0 — the minimal consumer
just default-constructs a `JsonPro::Json` and returns, so a clean run
already confirms the header parsed, the library linked, and the
symbols resolved. No output is expected.
