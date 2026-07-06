# JsonPro

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue)](https://en.cppreference.com/w/cpp/23)
[![Status](https://img.shields.io/badge/status-learning%20project-green)](https://github.com/privateMwb/JsonPro)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

**JsonPro** is a from-scratch JSON value type and recursive-descent parser written in modern C++23. It was built as a deep dive into variant-based value representation, zero-copy tokenizer design, and the real cost of RFC 8259 correctness — Unicode surrogate pairs, strict number grammar, nesting-depth guards — on parsing throughput.

---

## Table of Contents

- [Overview](#overview)
- [Motivation](#motivation)
- [Features](#features)
- [Quick Start](#quick-start)
- [Core API](#core-api)
- [Design Overview](#design-overview)
- [Complexity](#complexity)
- [Benchmarks](#benchmarks)
- [Project Structure](#project-structure)
- [Building from Source](#building-from-source)
- [Known Limitations](#known-limitations)
- [License](#license)

---

## Overview

`JsonPro::Json` is a JSON value type backed by `std::variant`, paired with a `Parser` that builds it from text via recursive descent. It focuses on correctly and efficiently solving the hard problems a JSON library faces internally:

- Deriving the active type directly from the variant's index — no redundant discriminant
- A zero-copy raw-pointer parsing cursor with line/column tracking deferred to the error path only
- Bulk-copy string scanning that only slows down on actual escape sequences
- Allocation-free number parsing via hand-validated grammar + `std::from_chars`
- A bounded recursion depth guard against adversarially nested input

On top of this foundation, JsonPro adds full `\u` escape and surrogate-pair decoding, deep structural equality, pretty-printed serialization, and a benchmark suite covering every public operation.

---

## Motivation

This project was built to understand, in depth:

- Representing a dynamically-typed value with `std::variant` instead of a hand-rolled tagged union
- Why a separate type discriminant is redundant when `std::variant::index()` already provides one
- Recursive-descent parsing: tokenizing and building a tree in a single pass, with no intermediate token stream
- The cost of correctness — Unicode surrogate pairs, escape decoding, strict number grammar — versus skipping validation for speed
- Why per-character line/column tracking on the hot path is wasted work when it's only needed for error messages
- Bulk-copy string scanning vs. character-by-character appending
- Guarding recursive descent against unbounded input depth (stack-overflow denial of service)

---

## Features

| Feature | Description |
|---|---|
| Recursive-descent parser | `Json::parse()` builds the full value tree in a single forward pass |
| RFC 8259 number grammar | No leading zeros, required digits after `.`/`e`, validated by hand before `std::from_chars` |
| Full string escape support | `\" \\ \/ \n \t \r \b \f`, `\uXXXX`, and surrogate pairs for astral codepoints |
| Variant-derived `type()` | `Type` is `static_cast` from `value_.index()` — no separate discriminant stored |
| Zero-copy parsing cursor | Raw pointer cursor; line/column computed lazily, only on error |
| Bulk-copy string scanning | Unescaped runs copied in one `append(ptr, len)`, not char-by-char |
| Maximum nesting depth guard | `kMaxDepth = 512` rejects adversarial input before stack overflow |
| Deep structural equality | `operator==` compares arrays elementwise and objects order-independently |
| Dual serialization entry points | `dump()` returns a `std::string`; `dump(std::ostream&)` writes directly |
| Checked & unchecked access | `operator[]` (unchecked) and `at()` (throws on miss) for both arrays and objects |
| Move-safe value type | Move construction/assignment leave the source as `Null` |

---

## Quick Start

### Basic usage

```cpp
#include "Json.h"

using namespace JsonPro;

int main() {
    Json doc = Json::parse(R"({
        "name": "Rain",
        "active": true,
        "score": 42.5,
        "tags": ["cpp", "json"]
    })");

    std::cout << doc["name"].asString()    << "\n"; // Rain
    std::cout << doc["active"].asBool()    << "\n"; // 1
    std::cout << doc["tags"][0].asString() << "\n"; // cpp
    std::cout << doc.dump()                << "\n"; // pretty-printed JSON
}
```

### Building documents

```cpp
#include "Json.h"

using namespace JsonPro;

int main() {
    Json::ObjectType profile;
    profile.insert_or_assign("name", Json("Rain"));
    profile.insert_or_assign("skills", Json(Json::ArrayType{ Json("C++"), Json("Systems") }));

    Json profileJson(std::move(profile));
    std::cout << profileJson.dump() << "\n";
}
```

### Navigating documents

```cpp
#include "Json.h"

using namespace JsonPro;

int main() {
    Json doc = Json::parse(R"({"address": {"city": "Metropolis"}})");

    for (const auto& [key, value] : doc["address"].asObject())
        std::cout << key << ": " << value.asString() << "\n";

    if (doc.contains("address") && doc["address"].isObject())
        std::cout << "address is present and is an object\n";
}
```

### Error handling

```cpp
#include "Json.h"

using namespace JsonPro;

int main() {
    try {
        Json::parse(R"({"a": 1, })"); // trailing comma — rejected
    } catch (const std::runtime_error& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }

    try {
        Json array(Json::ArrayType{ Json(1) });
        array.at(10); // out of range
    } catch (const std::out_of_range& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }
}
```

---

## Core API

### Constructors

```cpp
Json j;                                   // Null
Json j(nullptr);

Json j(true);                             // Bool
Json j(3.14);                             // Number
Json j(42);                               // int -> Number (double)

Json j(std::string("text"));              // String, copy
Json j(std::move(str));                   // String, move
Json j("literal");                        // String, from C-string

Json j(arrayType);                        // Array, copy
Json j(std::move(arrayType));             // Array, move

Json j(objectType);                       // Object, copy
Json j(std::move(objectType));            // Object, move

Json b(a);                                 // copy construction (deep copy)
Json b(std::move(a));                      // move construction (a becomes Null)
b = a;                                      // copy assignment
b = std::move(a);                           // move assignment
```

### Type Inspection

```cpp
[[nodiscard]] Type type()     const noexcept;
[[nodiscard]] bool isNull()   const noexcept;
[[nodiscard]] bool isBool()   const noexcept;
[[nodiscard]] bool isNumber() const noexcept;
[[nodiscard]] bool isString() const noexcept;
[[nodiscard]] bool isArray()  const noexcept;
[[nodiscard]] bool isObject() const noexcept;
```

### Value Access

```cpp
// All throw std::runtime_error on type mismatch.
[[nodiscard]] bool               asBool()   const;
[[nodiscard]] double             asNumber() const;
[[nodiscard]] const std::string& asString() const;

[[nodiscard]] ArrayType&         asArray();
[[nodiscard]] const ArrayType&   asArray()  const;

[[nodiscard]] ObjectType&        asObject();
[[nodiscard]] const ObjectType&  asObject() const;
```

### Navigation

```cpp
// Unchecked. Throws std::runtime_error on wrong type,
// std::out_of_range on missing key (const overload).
[[nodiscard]] Json&       operator[](std::size_t index);
[[nodiscard]] const Json& operator[](std::size_t index) const;
[[nodiscard]] Json&       operator[](const std::string& key);
[[nodiscard]] const Json& operator[](const std::string& key) const;

// Bounds/key-checked. Throws std::out_of_range on miss.
[[nodiscard]] Json& at(std::size_t index);
[[nodiscard]] const Json& at(std::size_t index) const;
[[nodiscard]] Json& at(const std::string& key);
[[nodiscard]] const Json& at(const std::string& key) const;
```

### Utilities

```cpp
[[nodiscard]] std::size_t size() const noexcept;                     // 0 for non-containers
[[nodiscard]] bool contains(const std::string& key) const noexcept;  // false for non-objects
```

### Comparison

```cpp
[[nodiscard]] bool operator==(const Json& other) const; // deep, order-independent for objects
[[nodiscard]] bool operator!=(const Json& other) const;
```

### Serialization

```cpp
[[nodiscard]] std::string dump(int indent = 0) const;
void dump(std::ostream& os, int indent = 0) const;
```

### Parsing

```cpp
[[nodiscard]] static Json Json::parse(std::string_view text);
// Throws std::runtime_error on any malformed input,
// including nesting beyond Parser::kMaxDepth (512).
```

---

## Design Overview

`Json` stores a single `std::variant<nullptr_t, bool, double, std::string, ArrayType, ObjectType>` — no separate type tag.

### Type derivation

Earlier drafts stored a `Type type_` field alongside the variant, kept in sync by every constructor and assignment. That's a second source of truth for information the variant already tracks:

```cpp
Json::Type Json::type() const noexcept {
    return static_cast<Type>(value_.index());
}
```

`Type`'s enumerators are declared in the same order as the variant's alternatives, so `index()` casts directly to the correct `Type`. This removes a member, removes a write on every construction, and makes it impossible for the type to drift out of sync with the value actually held.

### Zero-copy parsing cursor

`Parser` walks the input with raw pointers (`cur_`, `begin_`, `end_`) rather than an index into a `std::string`, and does **not** track line/column during the hot loop:

```cpp
unsigned char Parser::get() noexcept {
    if (cur_ >= end_) [[unlikely]] return '\0';
    return static_cast<unsigned char>(*cur_++);
}
```

Line and column are only computed — by rescanning `[begin_, cur_)` once — inside `error()`, on the cold, throwing path. Every successful parse pays zero cost for a feature it never uses.

### Fast-path string scanning

String parsing scans runs of unescaped bytes and bulk-copies them in one `append(ptr, len)` call, only breaking the run on a backslash or closing quote:

```cpp
if (c == '\\') [[unlikely]] {
    result.append(segStart, cur_);
    ++cur_;
    // ...handle the escape...
    segStart = cur_;
    continue;
}
```

A string with no escapes is copied in a single bulk operation instead of one `+=` per character.

### Numeric parsing via std::from_chars

Numbers are validated by hand against the JSON grammar (sign, no leading zeros, required digits after `.`/`e`) in a single forward pass with no allocation, then the already-validated span is converted with `std::from_chars` — no `substr` allocation, no locale-dependent `std::stod`, no exceptions on the success path.

### Maximum nesting depth guard

`parseArray()` and `parseObject()` increment a `depth_` counter on entry and check it against `kMaxDepth = 512`:

```cpp
if (++depth_ > kMaxDepth) [[unlikely]]
    error("maximum nesting depth exceeded");
```

Without this, input like `"[[[[[[..."` repeated a few hundred thousand times would recurse until the call stack overflows — a denial-of-service vector for any parser exposed to untrusted input.

---

## Complexity

### Time complexity

| Operation | Complexity | Notes |
|---|---|---|
| `Json::parse()` | O(n) | Single forward pass over the input |
| `dump()` | O(n) | n = total serialized size of the tree |
| `operator[]` / `at()` (array) | O(1) | Direct index into `std::vector` |
| `operator[]` / `at()` (object) | O(1) avg | Hash map lookup |
| `operator==` (scalar) | O(1) | Direct value comparison |
| `operator==` (array) | O(n) | Elementwise, short-circuits on first mismatch |
| `operator==` (object) | O(n) avg | Key-by-key lookup + compare, order-independent |
| `size()` | O(1) | `vector::size()` / `unordered_map::size()` |
| `contains()` | O(1) avg | Hash map lookup |
| Move construction/assignment | O(1) | Variant move, source reset to `Null` |
| Copy construction/assignment | O(n) | Deep copy of the entire tree |

### Space complexity

- O(n) for the total size of the value tree
- No extra discriminant per node — `type()` is derived from the variant, not stored separately

---

## Benchmarks

Benchmarks measure JsonPro in isolation — there is no baseline library in this codebase to compare against. All times are total elapsed time for the listed iteration count.

> Compiled without `-O2`. Results may vary depending on hardware, compiler, and optimization level.

<details>
<summary>Show benchmark results</summary>

#### Comparison

```
----------------------------------------------------------------------
Comparison                              Time           Iteration
----------------------------------------------------------------------
Compare number, equal                   6.84 ms         1000000
Compare number, unequal                 6.88 ms         1000000
Compare string, equal                   12.75 ms        1000000
Compare string, unequal                 13.28 ms        1000000

Compare cross-type (index mismatch)     5.66 ms         1000000

Compare array, equal (100 elems)        399.71 ms       500000
Compare array, mismatch at first        13.64 ms        1000000
Compare array, mismatch at last         408.48 ms       500000

Compare object, equal (100 keys)        3.28 s          500000
Compare object, unequal (100 keys)      2.02 s          500000
----------------------------------------------------------------------
```

#### Construction

```
----------------------------------------------------------------------
Construction                            Time           Iteration
----------------------------------------------------------------------
Construct bool                          8.42 ms         1000000
Construct number                        8.45 ms         1000000
Construct string (c-string)             20.58 ms        1000000

Construct string, copy                  13.32 ms        1000000
Construct string, move                  12.76 ms        1000000

Build array, 100 push Back              3.24 s          500000

Build object, insert Or Assign          16.90 s         500000
Build object, operator[]                19.29 s         500000

Copy-construct array (100 elems)        1.32 s          500000
Copy then move-construct array          1.20 s          500000
----------------------------------------------------------------------
```

#### Navigation

```
----------------------------------------------------------------------
Navigation                              Time           Iteration
----------------------------------------------------------------------
Array operator[]                        9.27 ms         1000000
Array at()                              12.33 ms        1000000

Object operator[]                       52.95 ms        1000000
Object at()                             55.14 ms        1000000

Deep traversal, 3 levels                109.56 ms       1000000

Array access, 100 elements              9.32 ms         1000000
Object access, 100 keys                 49.27 ms        1000000
----------------------------------------------------------------------
```

#### Parsing Scalars

```
----------------------------------------------------------------------
Parsing Scalars                         Time           Iteration
----------------------------------------------------------------------
Parse null                              39.44 ms        1000000

Parse bool true                         37.76 ms        1000000
Parse bool false                        40.75 ms        1000000

Parse small integer                     78.57 ms        1000000
Parse large integer                     113.38 ms       1000000

Parse positive integer                  86.33 ms        1000000
Parse negative integer                  85.70 ms        1000000

Parse simple float                      100.75 ms       1000000
Parse high-precision float              168.00 ms       1000000

Parse positive exponent                 158.02 ms       1000000
Parse negative exponent                 173.80 ms       1000000
----------------------------------------------------------------------
```

#### Parsing Strings

```
----------------------------------------------------------------------
Parsing Strings                         Time           Iteration
----------------------------------------------------------------------
Parse short string                      71.77 ms        1000000

Parse 1000-char string                  1.40 s          500000

Parse string, no escapes                101.24 ms       1000000
Parse string, with escapes              518.58 ms       1000000

Parse BMP unicode escapes               273.17 ms       1000000
Parse surrogate pair                    170.64 ms       1000000

Parse string, 200 escapes               3.58 s          500000
----------------------------------------------------------------------
```

#### Parsing Structural

```
----------------------------------------------------------------------
Parsing Structural                      Time           Iteration
----------------------------------------------------------------------
Parse array, 10 elements                2.60 s          1000000
Parse array, 100 elements               6.94 s          500000

Parse object, 10 keys                   5.42 s          1000000
Parse object, 100 keys                  30.07 s         500000

Parse nesting, depth 10                 2.02 s          1000000
Parse nesting, depth 100                2.12 s          100000

Parse mixed document                    5.70 s          1000000
----------------------------------------------------------------------
```

#### Serialization

```
----------------------------------------------------------------------
Serialization                           Time           Iteration
----------------------------------------------------------------------
Dump null                               258.48 ms       1000000
Dump bool                               310.64 ms       1000000
Dump number                             892.35 ms       1000000
Dump string                             382.60 ms       1000000

Dump array, 10 elements                 11.58 s         1000000
Dump array, 100 elements                110.85 s        500000

Dump object, 10 keys                    25.68 s         1000000
Dump object, 100 keys                   121.08 s        500000

Dump mixed document                     12.59 s         1000000

Dump via dump(int) -> string            55.47 s         500000
Dump via dump(ostream&)                 39.03 s         500000
----------------------------------------------------------------------
```

#### Summary

**Notable findings:**

- Early-exit dominates comparison cost: array mismatch-at-first (`13.64 ms`) is ~30x faster than mismatch-at-last or equal (`~400 ms`) — `std::vector`'s `operator==` exits on the first differing element instead of scanning the whole container.
- `insert_or_assign` beats `operator[]` by ~14% building a 100-key object (`16.90 s` vs `19.29 s`) — the single lookup+insert avoids the default-construct-then-assign `operator[]` performs on every new key.
- `at()` costs a consistent ~25-30% over the unchecked `operator[]` on both arrays and objects — the price of the bounds/key check and potential throw path.
- Object access is ~5-6x slower than array access at equal size (`49.27 ms` vs `9.32 ms`) — hash computation and bucket lookup vs. a direct index into contiguous memory.
- Every scalar parse pays for a `std::string` copy of the input inside `Json::parse()` — that fixed allocation cost is why `null`/`true`/`false` all land in the same ~38-41 ms band (1M iterations) regardless of content.
- Escapes are the dominant cost in string parsing: a string with a handful of escapes (`518.58 ms`) is ~5x slower than the same-length string with none (`101.24 ms`) — each escape forces a segment flush plus a switch dispatch, breaking the bulk-copy fast path.
- Array and object parsing both scale roughly linearly with element/key count; nesting depth costs less per level than flat element count, since each nesting level is just an open-bracket, a recursive call, and a close-bracket.
- Every `dump()` call constructs a fresh `std::ostringstream` internally — that allocation is why even a bare `null` (`258.48 ms` / 1M) costs far more than parsing the same literal (`39.44 ms` / 1M).

| Area | Key Finding |
|---|---|
| Comparison | Early-exit dominates: mismatch-at-first is ~30x faster than a full scan |
| Construction | `insert_or_assign` beats `operator[]` by ~14% building a 100-key object |
| Navigation | `at()` costs ~25-30% over `operator[]`; objects ~5-6x slower than arrays |
| Parsing Scalars | Fixed per-call `std::string` copy dominates trivial literals |
| Parsing Strings | Escapes break the bulk-copy fast path — 200 escapes cost ~7 µs/call |
| Parsing Structural | Array/object parsing scale near-linearly with element/key count |
| Serialization | `std::ostringstream` construction dominates trivial dumps |

**The recurring theme:** allocation dominates the small cases — a fresh `std::string` copy per `parse()`, a fresh `std::ostringstream` per `dump()` — while the parsing and serialization algorithms themselves scale linearly and cleanly with input size. See [Known Limitations](#known-limitations) for the concrete follow-ups this suggests.

</details>

---


## Project Structure

```
HashMap/
├── include/
│   └── JsonPro/
│       ├── Json.h
│       └── Parser.h
│
├── src/
│   └── JsonPro/
│       ├── Json.cpp
│       └── Parser.cpp
│
├── tests/
├── benchmarks/
├── examples/
│
├── cmake/
│   └── JsonProConfig.cmake.in
│
├── .gitignore
├── CMakeLists.txt
├── README.md
└── LICENSE
```

---

## Building from Source

### Requirements

- GCC 16+ or Clang with C++23 support
- CMake 3.20+

### Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Run tests

```bash
./tests                 # run all test suites
./tests list            # list available suites
./tests 1               # run by number
./tests name            # run by name
```

### Run benchmarks

```bash
./benchmarks            # run all benchmark suites
./benchmarks list       # list available suites
./benchmarks 1          # run by number
./benchmarks name       # run by name
```

### Run examples

```bash
./examples              # run all examples
./examples list         # list available examples
./examples 1            # run by number
./examples name         # run by name
```

---

## Known Limitations

- **`Json::parse()` always copies the input into an owned `std::string`** (`Parser parser{ std::string(text) }`) before parsing. Every scalar-literal benchmark (`~38-41 ms` for 1M iterations of `null`/`true`/`false`) is dominated by this allocation rather than actual tokenizing. Callers who can guarantee buffer lifetime could bypass this via a `std::string_view`-based `Parser` constructor — not currently exposed.
- **`dump()` constructs a fresh `std::ostringstream` on every call**, which is why even a bare `null` (`258.48 ms` / 1M) costs far more than parsing the same literal (`39.44 ms` / 1M). A pooled/reused stream or a direct `std::string`-append path for the common small-value case is a likely follow-up.
- **The maximum nesting depth (`kMaxDepth = 512`) is a compile-time constant**, not currently configurable per-parse.
- **`type()`, `dump()`, and `operator==` rely on `Type`'s six enumerators always matching the variant's six alternatives** — there's no `default` case in their switches, so adding a seventh type to the variant without updating `Type` would silently fall through.
- **Duplicate object keys resolve to "last one wins"** (`insert_or_assign` semantics) — consistent with most JSON parsers, but not mandated by RFC 8259.
- **String parsing escape overhead is disproportionate**: a string with a handful of escapes (`518.58 ms` / 1M) is ~5x slower than the same-length string with none (`101.24 ms` / 1M), since every escape breaks the bulk-copy fast path for at least one character.

### Fixed during development

- `const operator[](const std::string&)` previously dereferenced `unordered_map::end()` directly on a missing key — undefined behavior, not a throw. It now checks for `end()` and throws `std::out_of_range`, matching `at()`'s existing behavior.
- `Json` previously stored a separate `Type type_` member alongside the `std::variant`, manually kept in sync by every constructor and assignment — a redundant, driftable second source of truth. `type()` now derives directly from `value_.index()`, and the member was removed entirely.
- The parser previously tracked line/column on every character parsed, even on the successful path, purely to support error messages. This is now deferred entirely to `error()`, which rescans `[begin_, cur_)` once, only when a parse actually fails.
- Number parsing previously used `substr()` (heap allocation) plus `std::stod` (locale-dependent, throws). It now hand-validates the grammar in place and converts with `std::from_chars` directly on the input buffer.
- String parsing previously appended one character at a time (`result += c`); it now bulk-copies runs of unescaped bytes via `append(ptr, len)`, only breaking the run on an actual escape or the closing quote.
- Recursive descent had no bound on nesting depth, making deeply nested input (`"[[[[[..."`) a stack-overflow denial-of-service vector. A `depth_` counter checked against `kMaxDepth = 512` was added to `parseArray()`/`parseObject()`.

---

## License

Licensed under the [MIT License](LICENSE) — free to use, modify, and distribute for educational and personal purposes.
