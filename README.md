# JsonParser

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

JsonParser is an RFC 8259–compliant JSON library for modern C++ — a single-pass recursive-descent parser, an insertion-order-preserving object type backed by a vector plus a transparent-hash index instead of a key-ordered tree, and lossless, round-trip-exact number serialization via `std::to_chars`.

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

- **Single-pass recursive-descent parser** — numbers are validated and converted directly via `std::from_chars`, with no intermediate tokenization pass. Strictly conforms to RFC 8259, including surrogate-pair decoding, leading-zero rejection, and a configurable maximum nesting depth guard against malicious or malformed deeply-nested input.
- **Insertion-order-preserving object storage** — `JsonObject` stores entries in a `std::vector`, in insertion order, with a side transparent-hash index for O(1) average lookup. `dump()` iterates the vector directly for stable, human-expected serialization, with no separate order-tracking structure to keep in sync.
- **`std::variant`-backed value type** — `Json` covers Null, Bool, Number, String, Array, and Object. `type()` is derived directly from the variant's active index, so there's no separate discriminant to keep in sync.
- **Lossless, round-trip-exact serialization** — `dump()` writes the shortest decimal representation that round-trips exactly, via `std::to_chars`, and rejects non-finite numbers (NaN/Infinity) rather than silently emitting invalid JSON.
- **A dedicated exception hierarchy** — `JsonParseError` carries the 1-based line/column of the failure (computed lazily, only on the throwing path); `JsonTypeError` and `JsonOutOfRange` distinguish caller-side type/access mistakes from malformed input.
- **Bounds-checked and unchecked access, on purpose** — `at()` throws on a missing key or out-of-range index; `operator[]` auto-vivifies a missing object key (matching `std::map`'s convention) or is left unchecked for arrays.
- **Thread-safe for concurrent const access** — no internal synchronization, since no read-only operation touches any mutable state; a single shared value can be read from many threads at once with no external locking required.

## <a id="requirements"></a>📋 Requirements

- A C++23-conformant compiler (tested: GCC, Clang, MSVC, AppleClang)
- CMake 3.20+

## <a id="installation"></a>📦 Installation

**From source:**

```bash
git clone https://github.com/privateMwb/JsonParser.git
cd JsonParser
cmake -B build \
  -DBUILD_TESTS=OFF \
  -DBUILD_BENCHMARKS=OFF \
  -DBUILD_REGRESSION=OFF \
  -DBUILD_EXAMPLES=OFF
cmake --install build
```

Then, in your own `CMakeLists.txt`:

```cmake
find_package(JsonPro CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE JsonPro::JsonPro)
```

> vcpkg and Conan packages are built and verified (recipe in
> `packaging/recipes/jsonpro/`, port in `packaging/vcpkg/ports/jsonpro/`),
> but not yet published to the public registries. This section will be
> updated once they are.

## <a id="quick-start"></a>🚀 Quick Start

```cpp
#include <JsonPro/Json.h>

int main() {
    JsonPro::Json j = JsonPro::Json::parse(R"({"name": "Rain", "active": true})");

    std::cout << j["name"].asString() << "\n";

    j["age"] = JsonPro::Json(30); // auto-vivifies the new key

    std::cout << j.dump() << "\n";
}
```

Building a document programmatically:

```cpp
JsonPro::Json::ObjectType obj;
obj.insert_or_assign("id", JsonPro::Json(1));
obj.insert_or_assign("tags", JsonPro::Json(JsonPro::Json::ArrayType{
    JsonPro::Json("cpp"), JsonPro::Json("json")}));

JsonPro::Json doc(std::move(obj));
std::cout << doc.dump() << "\n";
```

Malformed input throws a real, catchable diagnostic rather than failing silently:

```cpp
try {
    JsonPro::Json bad = JsonPro::Json::parse("{ invalid json");
} catch (const JsonPro::JsonParseError& e) {
    std::cerr << "line " << e.line() << ", col " << e.column() << ": " << e.what() << '\n';
}
```

## <a id="project-structure"></a>🗂️ Project Structure

```
JsonParser/
├── include/
│   └── JsonPro/
│       ├── Json.h
│       ├── JsonObject.h
│       ├── Parser.h
│       └── JsonException.h
│
├── src/
│   └── JsonPro/
│       ├── Json.cpp
│       ├── JsonObject.cpp
│       └── Parser.cpp
│
├── tests/
│   ├── support/
│   ├── suite/
│   ├── test_main.cpp
│   └── CMakeLists.txt
│
├── benchmarks/
│   ├── support/
│   ├── suite/
│   ├── baselines/
│   ├── bench_main.cpp
│   └── CMakeLists.txt
│
├── examples/
│   ├── support/
│   ├── suite/
│   ├── example_main.cpp
│   └── CMakeLists.txt
│
├── regression/
│   ├── support/
│   ├── regression_main.cpp
│   └── CMakeLists.txt
│
├── packaging/
│   ├── README.md
│   ├── recipes/
│   │   └── jsonpro/
│   ├── vcpkg/
│   │   └── ports/
│   │       └── jsonpro/
│   └── vcpkg-smoke-test/
│
├── scripts/
│   └── update_package_files.py
│
├── .github/
│   ├── releases/
│   └── workflows/
│
├── cmake/
│   └── JsonProConfig.cmake.in
│
├── docs/
│   ├── Doxyfile
│   └── README.md
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

See [packaging/README.md](packaging/README.md) for notes on verifying the vcpkg
port and Conan recipe locally.

## <a id="benchmarks"></a>📊 Benchmarks

Measured against `nlohmann::json`, same build, at 10K / 100K / 1M
iterations (`benchmarks/baselines/v1.0.0.json` has the full dataset).

| Operation | JsonPro (1M) | nlohmann::json (1M) | Δ |
|---|---|---|---|
| `Short String Copy` | 6.21 ms | 25.50 ms | +310.4% |
| `Parse Number Array` | 480.96 ms | 1.70 s | +253.7% |
| `Default Copy` | 1.71 ms | 5.91 ms | +246.2% |
| `Contains() Miss` | 15.98 ms | 52.65 ms | +229.5% |
| `Default Move` | 4.71 ms | 12.08 ms | +156.6% |
| `Round-trip String-heavy Object` | 2.68 s | 4.82 s | +80.1% |
| `Compare Equal Arrays` | 8.90 ms | 13.99 ms | +57.2% |
| `Round-trip Small Object` | 2.71 s | 3.77 s | +39.1% |
| `Object Copy` | 154.25 ms | 199.08 ms | +29.1% |
| `Array Operator[]` | 2.21 ms | 628.59 us | -71.5% |
| `Type()` | 1.56 ms | 308.33 us | -80.2% |
| `AsBool()` | 1.86 ms | 317.37 us | -82.9% |

JsonPro's single-pass parser and hash-indexed object storage pay off
most on parsing, copy/move of scalar and container values, and
miss-heavy lookups (`Contains() Miss`) — all cases where
`nlohmann::json`'s tree-of-`std::map` design or heavier per-value
overhead shows up directly.

The trade-off: `std::variant`'s `get_if`-based access costs more per
call than `nlohmann::json`'s storage on trivial scalar operations
(`operator[]`, `type()`, `asBool()`), so those benchmarks favor
`nlohmann::json` instead.

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
