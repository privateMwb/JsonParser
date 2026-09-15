# Changelog

All notable changes to JsonParser are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Nothing yet.

## [1.0.0]

The first stable release of **JsonParser**, an RFC 8259–compliant JSON
library for modern C++.

### Added
- Single-pass recursive-descent parser, strictly conforming to RFC 8259.
- Insertion-order-preserving object type (`JsonObject`) backed by a vector
  plus a transparent-hash index.
- `Json` value type covering Null, Bool, Number, String, Array, and Object,
  backed by `std::variant`.
- `parse()` (copies input) and `parseOwned()` (takes ownership) entry points.
- Lossless, round-trip-exact number serialization via `std::to_chars`.
- Bounds-checked (`at()`) and unchecked (`operator[]`) access for both
  arrays and objects.
- Full `\u` escape support, including surrogate-pair decoding to
  astral-plane codepoints.
- Configurable maximum nesting depth guard against malicious or malformed
  deeply-nested input.
- A dedicated exception hierarchy: `JsonParseError` (with line/column),
  `JsonTypeError`, `JsonOutOfRange`.
- Order-independent structural equality (`operator==`) for arrays and
  objects.
- Thread-safe for concurrent const access to a shared value, with no
  internal synchronization overhead.
- Distributed as a vcpkg port and a Conan recipe, alongside plain CMake
  `find_package()` support.

### Performance
- Single-pass parsing avoids intermediate tokenization — numbers are
  validated and converted directly via `std::from_chars`.
- `JsonObject`'s vector-plus-hash-index layout gives O(1) average lookup
  while keeping `dump()` a straight cache-friendly walk, with no separate
  order-tracking structure.
- String serialization scans and bulk-appends unescaped runs, so only
  actual escapes and control characters pay a per-character cost.
- The trade-off: `std::variant`'s `get_if` access pattern costs more per
  call than a hand-rolled tagged union on trivial scalar operations
  (`type()`, `isX()`, `operator[]`) — where nlohmann::json's simpler
  storage wins outright.
- Benchmarked against `nlohmann::json` at 10K / 100K / 1M iterations;
  largest wins on parsing, copy/move, and miss-heavy lookups
  (`Parse Number Array`, `Default Copy`/`Move`, `Contains() Miss`) at the
  1M scale. Full results in `benchmarks/results/v1_0_0.md`.

### Testing
- Comprehensive test suite covering unit, integration, lifecycle,
  concurrency, and regression tests; move semantics; copy semantics;
  exception safety; parser maximum nesting depth; duplicate object key
  resolution; `JsonObject` erase/reindex consistency; surrogate pair
  decoding; non-finite number rejection on serialization; and trailing
  input rejection.
- 93.5% line coverage and 100.0% function coverage, excluding test
  infrastructure and third-party dependencies.

### CI
- Automated builds and tests across GCC, Clang, MSVC, and AppleClang, each
  in Debug and Release configurations.

[Unreleased]: https://github.com/privateMwb/JsonParser/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/privateMwb/JsonParser/releases/tag/v1.0.0
