# JsonPro

<p align="center">
  <img src="https://img.shields.io/github/v/release/privateMwb/JsonParser?style=for-the-badge&logo=github&color=yellow" alt="Version">
  <img src="https://img.shields.io/badge/License-MIT-orange?style=for-the-badge" alt="License - MIT">
  <img src="https://img.shields.io/badge/C%2B%2B-23-blue?style=for-the-badge&logo=c%2B%2B" alt="C++ - 23">
</p>

<p align="center">
  <a href="https://github.com/privateMwb/JsonParser/actions/workflows/packaging.yml">
    <img src="https://github.com/privateMwb/JsonParser/actions/workflows/packaging.yml/badge.svg" alt="Packaging">
  </a>
  <a href="https://github.com/privateMwb/JsonParser/actions/workflows/update-package-recipes.yml">
    <img src="https://github.com/privateMwb/JsonParser/actions/workflows/update-package-recipes.yml/badge.svg" alt="Update Package Recipes">
  </a>
</p>

<!--
  VectorPro's README also carries badges for Build/Test, Benchmarks,
  Coverage, Sanitizers, Clang Tidy, Clang Format, and Docs workflows.
  I only have direct visibility into Packaging and Update Package Recipes
  actually existing for JsonPro — add the rest here once confirmed, using
  the same badge URL pattern.
-->

<p align="center">
  <img src="https://img.shields.io/badge/GCC-support-B46F1B?style=flat&logo=gnu" alt="GCC - support">
  <img src="https://img.shields.io/badge/Clang-support-045891?style=flat&logo=llvm" alt="Clang - support">
  <img src="https://img.shields.io/badge/MSVC-support-5C2D91?style=flat" alt="MSVC - support">
</p>

A C++23, RFC 8259-compliant JSON value type and recursive-descent parser,
benchmarked against nlohmann::json.

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

Measured against `nlohmann::json`, same build, at 10K/100K/1M iterations
across 17 benchmark files (`benchmarks/baselines/v1.0.0.json` has the
full dataset). Being upfront about both sides:

**At parity or faster:**

- **Object member access** — `operator[]`/`at()` on an existing key:
  ~106–152% faster.
- **Object lookup** — `contains()`/`find()`, both hit and miss:
  ~95–198% faster.
- **Parsing**, every document shape tested (flat, array, nested,
  string-heavy): ~134–285% faster.
- **Round-trip (parse + dump)**: ~46–101% faster.
- **Construction, copy, and move**, every value type: ~31–348% faster.
  The one soft spot is object copy, at "only" +31–35%.
- **Large/wide object parsing and lookup**: ~9–97% faster, with the gap
  widening as size increases.
- **String/array/object destruction**: ~50–136% faster. Destroying a
  bare bool/number/null is a wash either way — within noise.

**Slower, and worth knowing about:**

- **Array element access** (`operator[]`, `at()`) and every `asXxx()`
  typed accessor: ~60–83% slower.
- **`type()`/`isXxx()`** and **`size()`/`empty()`**: ~46–80% slower.
- **`dump()`**, every document shape: ~4–44% slower — the one place the
  parsing win doesn't carry over.
- **Comparison** is mixed: array comparison is faster (+49–60%);
  number, string, and object comparison are slower (−6% to −62%).

<details>
<summary>Why the gap on these specific operations</summary>

Array access, typed accessors, type inspection, and size queries are
all single-branch operations completing in 15–35 nanoseconds per call —
at that scale, the benchmark harness's own per-call overhead is a real
fraction of what's being measured, so these deltas are reported as-is
rather than attributed to a specific cause without further profiling.

The clearer, larger-magnitude story: JsonPro wins decisively on parsing,
object mutation, and lifecycle operations, where the `JsonObject`
redesign and `to_chars`-based number handling do the most work, and
loses on `dump()` and raw scalar access, where nlohmann's more mature
code path currently has the edge.

</details>

## <a id="documentation"></a>📖 Documentation

Full API reference, generated with Doxygen from `docs/Doxyfile`:

**https://privateMwb.github.io/JsonParser/**

> ⚠️ URL follows VectorPro's pattern (`github.io/<repo>`) but I don't
> have confirmation there's an actual CI step publishing this to GitHub
> Pages — `docs/Doxyfile` existing means the docs *can* be generated,
> not necessarily that they're deployed live yet. Double check before
> linking people to it.

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
