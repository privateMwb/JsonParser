# JsonPro

<p align="center">
  <img src="https://img.shields.io/github/v/release/privateMwb/JsonParser?style=for-the-badge&logo=github&color=yellow" alt="Version">
  <img src="https://img.shields.io/badge/License-MIT-orange?style=for-the-badge" alt="License - MIT">
  <img src="https://img.shields.io/badge/C%2B%2B-23-blue?style=for-the-badge&logo=c%2B%2B" alt="C++ - 23">
</p>

<p align="center">
  <a href="https://github.com/privateMwb/JsonParser/actions/workflows/build.yml">
    <img src="https://github.com/privateMwb/JsonParser/actions/workflows/build.yml/badge.svg" alt="Build and Test">
  </a>
  <a href="https://github.com/privateMwb/JsonParser/actions/workflows/benchmark.yml">
    <img src="https://github.com/privateMwb/JsonParser/actions/workflows/benchmark.yml/badge.svg" alt="Benchmarks">
  </a>
  <a href="https://github.com/privateMwb/JsonParser/actions/workflows/coverage.yml">
    <img src="https://github.com/privateMwb/JsonParser/actions/workflows/coverage.yml/badge.svg" alt="Coverage">
  </a>
  <a href="https://github.com/privateMwb/JsonParser/actions/workflows/sanitizers.yml">
    <img src="https://github.com/privateMwb/JsonParser/actions/workflows/sanitizers.yml/badge.svg" alt="Sanitizers">
  </a>
  <a href="https://github.com/privateMwb/JsonParser/actions/workflows/clang-tidy.yml">
    <img src="https://github.com/privateMwb/JsonParser/actions/workflows/clang-tidy.yml/badge.svg" alt="Clang Tidy">
  </a>
  <a href="https://github.com/privateMwb/JsonParser/actions/workflows/clang-format.yml">
    <img src="https://github.com/privateMwb/JsonParser/actions/workflows/clang-format.yml/badge.svg" alt="Clang Format">
  </a>
  <a href="https://github.com/privateMwb/JsonParser/actions/workflows/docs.yml">
    <img src="https://github.com/privateMwb/JsonParser/actions/workflows/docs.yml/badge.svg" alt="Documentation">
  </a>
  <a href="https://github.com/privateMwb/JsonParser/actions/workflows/release.yml">
    <img src="https://github.com/privateMwb/JsonParser/actions/workflows/release.yml/badge.svg" alt="Release">
  </a>
  <a href="https://github.com/privateMwb/JsonParser/actions/workflows/packaging.yml">
    <img src="https://github.com/privateMwb/JsonParser/actions/workflows/packaging.yml/badge.svg" alt="Packaging">
  </a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/GCC-support-B46F1B?style=flat&logo=gnu" alt="GCC - support">
  <img src="https://img.shields.io/badge/Clang-support-045891?style=flat&logo=llvm" alt="Clang - support">
  <img src="https://img.shields.io/badge/MSVC-support-5C2D91?style=flat" alt="MSVC - support">
  <img src="https://img.shields.io/badge/AppleClang-support-000000?style=flat&logo=apple" alt="AppleClang - support">
</p>


High-performance, RFC 8259-compliant JSON value type and recursive-descent parser for modern C++23, with strict string escaping, locale-independent number serialization, and O(1) average object lookup.

## 📑 Table of Contents

- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Project Structure](#project-structure)
- [Development](#development)
- [Benchmarks](#benchmarks)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [Changelog](#changelog)
- [License](#license)

## <a id="features"></a>✨ Features

- **Strict RFC 8259 compliance** — full string escaping on output
  (quotes, backslashes, control characters), UTF-16 surrogate pair
  handling on input, and rejection of malformed input (leading zeros,
  trailing content, invalid escapes) rather than silently accepting it.
- **`std::variant`-backed value type** — `type()` is derived directly
  from the variant's active index; no separate discriminant to keep in
  sync.
- **Insertion-order-preserving object storage** — `JsonObject` is a
  `vector<pair<string, Json>>` plus a transparent-hash index, not a
  PIMPL-wrapped `unordered_map`. O(1) average lookup, and `dump()`
  iterates the vector directly instead of walking a separate key-order
  list with a hash lookup per key.
- **Locale-independent, round-trip-safe number serialization** via
  `std::to_chars` — not `operator<<(double)`, which truncates to 6
  significant digits by default and is locale-sensitive.
- **`std::string_view` throughout** — `operator[]`, `at()`, `contains()`,
  and `find()` all take `string_view`, backed by transparent hashing, so
  lookups never allocate a temporary `std::string`.
- **A real exception hierarchy** — `JsonParseError` (carries line and
  column), `JsonTypeError`, and `JsonOutOfRange`, all catchable via a
  common `JsonException` base.
- **Compiled, not header-only** — a real CMake library target with
  package config export (`find_package(JsonPro CONFIG REQUIRED)`).

## <a id="requirements"></a>📋 Requirements

- A C++23-conformant compiler (tested: Clang, GCC, MSVC)
- CMake 3.20+

## <a id="installation"></a>📦 Installation

**From source:**

```bash
git clone https://github.com/privateMwb/JsonParser.git
cd JsonParser
cmake -B build \
  -DJSONPRO_BUILD_TESTS=OFF \
  -DJSONPRO_BUILD_BENCHMARKS=OFF \
  -DJSONPRO_BUILD_TOOLS=OFF \
  -DJSONPRO_BUILD_EXAMPLES=OFF
cmake --install build
```

Then, in your own `CMakeLists.txt`:

```cmake
find_package(JsonPro CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE JsonPro::JsonPro)
```

> vcpkg and Conan packages are built and verified (recipe in
> `recipes/jsonpro/`, port in `vcpkg/ports/jsonpro/`), but not yet
> published to the public registries. This section will be updated once
> they are.

## <a id="quick-start"></a>🚀 Quick Start

**Basic usage:**

```cpp
#include <JsonPro/Json.h>
#include <iostream>

int main() {
    JsonPro::Json doc = JsonPro::Json::parse(R"({"name": "Alice", "age": 30})");

    std::cout << doc["name"].asString() << "\n"; // Alice
    std::cout << doc.dump() << "\n";
}
```

**Building a document from scratch:**

```cpp
#include <JsonPro/Json.h>

int main() {
    using namespace JsonPro;

    JsonObject obj;
    obj.insert_or_assign("id", Json(1));
    obj.insert_or_assign("active", Json(true));

    Json doc(std::move(obj));
    std::cout << doc.dump() << "\n";
}
```

**Error handling:**

```cpp
#include <JsonPro/Json.h>
#include <JsonPro/JsonException.h>
#include <iostream>

int main() {
    try {
        JsonPro::Json::parse("{ invalid }");
    } catch (const JsonPro::JsonParseError& e) {
        std::cout << "Parse failed at line " << e.line() << ", col "
                  << e.column() << ": " << e.what() << "\n";
    }
}
```

## <a id="project-structure"></a>🗂️ Project Structure

```
JsonParser/
├── include/
│   └── JsonPro/
│       ├── Json.h
│       ├── JsonObject.h
│       ├── JsonException.h
│       └── Parser.h
│
├── src/
│   └── JsonPro/
│       ├── Json.cpp
│       ├── JsonObject.cpp
│       └── Parser.cpp
│
├── tests/
│   ├── common/
│   ├── suite/
│   ├── test_all.cpp
│   └── CMakeLists.txt
│
├── benchmarks/
│   ├── common/
│   ├── suite/
│   ├── baselines/
│   ├── bench_all.cpp
│   └── CMakeLists.txt
│
├── examples/
│   ├── common/
│   ├── suite/
│   ├── example_all.cpp
│   └── CMakeLists.txt
│
├── tools/
│   ├── regression/
│   └── CMakeLists.txt
│
├── recipes/
│   └── jsonpro/             
│
├── vcpkg/
│   └── ports/
│
├── scripts/
│   └── update_package_files.py
│
├── .github/
│   └── workflows/
│
├── cmake/
│   └── JsonProConfig.cmake.in   
│
├── docs/
│   ├── Doxyfile
│   └── PACKAGING.md
│
├── .gitignore
├── CMakeLists.txt
├── README.md
└── LICENSE
```

## <a id="development"></a>🛠️ Development

The from-source install above builds the library only. To work on
JsonPro itself — running tests, benchmarks, or the regression tool —
build with everything enabled (the default):

```bash
cmake -B build
cmake --build build
```

**Run the test suite:**

```bash
ctest --test-dir build
```

**Run benchmarks and check for regressions:**

```bash
./build/benchmarks
./build/regression                  # latest baseline vs. benchmarks/results/benchmark_results.json
./build/regression v1.2.0           # a specific baseline vs. current
./build/regression v1.2.0 v1.4.0    # two baselines against each other
```

`regression` picks the latest baseline by semantic version (`v1.10.0`
correctly outranks `v1.9.0`), not alphabetical filename order, and
auto-names its output (`regression_v1.2.0_vs_current.md`/`.json`, etc.).

See [docs/PACKAGING.md](docs/PACKAGING.md) for notes on verifying the
vcpkg port and Conan recipe locally.

## <a id="benchmarks"></a>📊 Benchmarks

Measured against `nlohmann::json`, same build, at 10K / 100K / 1M
iterations (`benchmarks/baselines/v1.0.0.json` has the full dataset).
Numbers below are the 100K row unless noted — differences are
consistent across scale for most operations.

**At parity or faster:**

| Operation | JsonPro | nlohmann::json | Difference |
|---|---|---|---|
| `Object At()` | 1.07 ms | 2.63 ms | ~145% faster |
| `Contains() Miss` | 1.47 ms | 4.03 ms | ~174% faster |
| `Parse Number Array` | 43.26 ms | 161.24 ms | ~273% faster |
| `Round-trip Number Array` | 137.39 ms | 227.58 ms | ~66% faster |
| `Short String Copy` | 549.56 μs | 1.97 ms | ~259% faster |
| `Array Destruction` | 52.61 ms | 113.86 ms | ~116% faster |

**Slower, and consistent across scale — worth fixing:**

| Operation | JsonPro | nlohmann::json | Difference |
|---|---|---|---|
| `Array At()` | 188.55 μs | 64.67 μs | ~66% slower |
| `AsObject()` | 241.38 μs | 118.48 μs | ~51% slower |
| `Dump Number Array` | 85.90 ms | 55.06 ms | ~36% slower |
| `Type()` | 97.53 μs | 32.37 μs | ~67% slower |
| `Object Size()` | 335.75 μs | 182.18 μs | ~46% slower |
| `Compare Equal Numbers` | 226.19 μs | 138.01 μs | ~39% slower |
| `Object Empty()` (non-empty) | 235.05 μs | 64.68 μs | ~72% slower |

<details>
<summary>Why the gap on these specific operations</summary>

Array access, typed accessors (`asXxx()`), type inspection, and size
queries are all single-branch operations completing in 15–35
nanoseconds per call — at that scale, the benchmark harness's own
per-call overhead is a real fraction of what's being measured, so
these deltas are reported as-is rather than attributed to a specific
cause without further profiling.

`dump()` is the clearer signal: it's slower across every document
shape tested (4–44% depending on shape and scale), the one place the
parsing win doesn't carry over — worth investigating separately from
the nanosecond-scale accessor gaps above. Comparison is mixed rather
than uniformly slower: array comparison is actually faster (+43–64%),
while number, string, and object comparison are slower — see
`benchmarks/baselines/v1.0.0.json` for the full per-scale breakdown if
you want to look yourself.

</details>

## <a id="documentation"></a>📖 Documentation

Full API reference, generated with Doxygen from `docs/Doxyfile`:

**https://privateMwb.github.io/JsonParser/**

## <a id="contributing"></a>🤝 Contributing

Issues and pull requests are welcome. Before submitting a PR:

- Run the test suite (`ctest --test-dir build`)
- If you're changing a hot path, run `./build/regression` and mention
  the results in your PR description

## <a id="changelog"></a>📝 Changelog

See the [Releases](https://github.com/privateMwb/JsonParser/releases)
page for version history and release notes.

## <a id="license"></a>📄 License

MIT — see [LICENSE](LICENSE) for details.
