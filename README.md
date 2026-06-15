# JsonParser

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue)](https://en.cppreference.com/w/cpp/23)
[![Status](https://img.shields.io/badge/status-learning%20project-green)](https://github.com/privateMwb/JsonParser)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A JSON parser and value library implemented from scratch in **C++23**, built to explore recursive descent parsing, `std::variant`-based type-safe value representation, Unicode handling, serialization design, and the tradeoffs between parsing and direct construction.

---

## Table of Contents

- [Overview](#overview)
- [Motivation](#motivation)
- [Features](#features)
- [Design Overview](#design-overview)
  - [Internal Structure](#internal-structure)
  - [Type System](#type-system)
  - [Parser Architecture](#parser-architecture)
  - [Recursive Descent](#recursive-descent)
  - [String Parsing & Unicode](#string-parsing--unicode)
  - [Serialization Design](#serialization-design)
  - [operator\[\] vs at()](#operator-vs-at)
  - [Custom Backend](#custom-backend)
- [Supported JSON](#supported-json)
- [Complexity](#complexity)
- [Quick Example](#quick-example)
  - [Parsing](#parsing)
  - [Direct Construction](#direct-construction)
  - [Type Inspection](#type-inspection)
  - [Value Access](#value-access)
  - [Navigation](#navigation)
  - [Bounds-Checked Access](#bounds-checked-access)
  - [Serialization](#serialization)
  - [Comparison](#comparison)
  - [Copy & Move](#copy--move)
  - [Mixed Nested Structure](#mixed-nested-structure)
- [Core API](#core-api)
  - [Constructors](#constructors)
  - [Parsing](#parsing-1)
  - [Type Inspection](#type-inspection-1)
  - [Value Access](#value-access-1)
  - [Navigation](#navigation-1)
  - [Element Access](#element-access)
  - [Utilities](#utilities)
  - [Comparison](#comparison-1)
  - [Serialization](#serialization-1)
- [Error Handling](#error-handling)
- [Benchmark Results](#benchmark-results)
  - [Parse vs Construct](#parse-vs-construct)
  - [Array — Parse vs Construct](#array--parse-vs-construct)
  - [Object — Parse vs Construct](#object--parse-vs-construct)
  - [Nested — Parse vs Construct](#nested--parse-vs-construct)
  - [Dump — String vs Stream](#dump--string-vs-stream)
  - [Array Access — operator\[\] vs at()](#array-access--operator-vs-at)
  - [Object Access — operator\[\] vs at()](#object-access--operator-vs-at)
  - [Summary](#summary)
- [Project Structure](#project-structure)
- [Build Instructions](#build-instructions)
- [Notes](#notes)
- [Contributing](#contributing)
- [License](#license)

---

## Overview

JsonParser is a two-component library: a `Json` value class and a `Parser` that produces `Json` values from text. `Json` stores any JSON value — null, bool, number, string, array, or object — in a `std::variant`, with a separate `Type` enum for fast type dispatch. `Parser` is a hand-written recursive descent parser that processes input character-by-character, tracking line and column for error reporting.

The library also supports direct construction of `Json` values in C++ without parsing, full copy/move semantics, `dump()` serialization to string or stream, and an optional custom backend that can swap `std::vector` and `std::unordered_map` for custom containers.

---

## Motivation

This project was built to deeply understand:

- Recursive descent parsing — how to write a parser that mirrors a grammar directly in code
- `std::variant` as a type-safe discriminated union
- Character-level input processing (`peek`, `get`, position tracking)
- Unicode escape decoding and UTF-8 encoding (`\uXXXX`, surrogate pairs)
- Serialization design tradeoffs (string concatenation vs stream writing)
- The cost of parsing vs direct construction
- Error reporting with line and column context

---

## Features

- Full JSON parsing via `Json::parse(std::string_view)`
- All six JSON types: `null`, `bool`, `number`, `string`, `array`, `object`
- Direct C++ construction for all types (no parsing required)
- Type inspection: `isNull()`, `isBool()`, `isNumber()`, `isString()`, `isArray()`, `isObject()`
- Value access: `asBool()`, `asNumber()`, `asString()`, `asArray()`, `asObject()`
- Navigation: `operator[]` by index (array) and by key (object)
- Bounds-checked access: `at(index)` and `at(key)` — throws on invalid access
- `contains(key)` — key existence check on objects
- `size()` — element count for arrays and objects
- Full copy & move semantics (Rule of 5)
- `operator==` / `operator!=` — deep equality comparison
- `dump(int indent)` — serialize to `std::string`
- `dump(std::ostream&, int indent)` — serialize directly to any stream
- Line and column error reporting on parse failure
- Unicode `\uXXXX` escape decoding with surrogate pair support
- Pluggable container backend via `#define JSON_USE_CUSTOM`

---

## Design Overview

### Internal Structure

```
Json
├── type_    (Type enum)
└── value_   (std::variant<
                nullptr_t,   ← Null
                bool,        ← Bool
                double,      ← Number
                std::string, ← String
                ArrayType,   ← Array  (std::vector<Json>)
                ObjectType   ← Object (std::unordered_map<std::string, Json>)
              >)
```

`type_` is stored separately from `value_` for fast type dispatch — `isNull()`, `isBool()`, etc. are simple enum comparisons that never touch the variant. Type-specific access (`asBool()`, `asNumber()`, etc.) uses `std::get_if` for safe extraction.

---

### Type System

| JSON Type | C++ Storage      | `Json` constructor         |
|-----------|------------------|----------------------------|
| null      | `nullptr_t`      | `Json()` or `Json(nullptr)`|
| bool      | `bool`           | `Json(bool)`               |
| number    | `double`         | `Json(double)`, `Json(int)`|
| string    | `std::string`    | `Json(string)`, `Json(const char*)` |
| array     | `ArrayType`      | `Json(ArrayType)`          |
| object    | `ObjectType`     | `Json(ObjectType)`         |

All JSON numbers are stored as `double`. Integer inputs (`Json(42)`) are cast to `double` at construction — consistent with the JSON spec, which defines a single `number` type.

---

### Parser Architecture

`Parser` is a separate class from `Json`. It owns a copy of the input string and a position cursor, and exposes only two public methods:

```
Parser(std::string input)
Json parse()
```

`Json::parse(std::string_view)` is a static factory that constructs a `Parser` and calls `parse()` — keeping the parser implementation hidden from the `Json` interface.

The parser state:

```
Parser
├── input_   — full input string (owned copy)
├── pos_     — current character index
├── line_    — current line number (for error messages)
└── column_  — current column number (for error messages)
```

Two primitives drive all parsing:

- `peek()` — returns the character at `pos_` without advancing
- `get()` — returns the character at `pos_` and advances; tracks `\n` for line counting

---

### Recursive Descent

`parseValue()` is the central dispatch function. It inspects the current character and delegates:

```
parseValue()
  ├── 'n'        → parseNull()
  ├── 't' / 'f' → parseBool()
  ├── '"'        → parseString()
  ├── '['        → parseArray()
  ├── '{'        → parseObject()
  └── digit / '-'→ parseNumber()
```

`parseArray()` and `parseObject()` call `parseValue()` recursively — this is what makes the parser handle arbitrary nesting without explicit depth tracking.

```
parseObject()
  └── for each key-value pair:
        parseString() → key
        parseValue()  → value  (may recurse into parseArray/parseObject)
```

---

### String Parsing & Unicode

String parsing handles all JSON escape sequences:

| Escape | Result    |
|--------|-----------|
| `\"`   | `"`       |
| `\\`   | `\`       |
| `\/`   | `/`       |
| `\n`   | newline   |
| `\t`   | tab       |
| `\r`   | carriage return |
| `\b`   | backspace |
| `\f`   | form feed |
| `\uXXXX` | UTF-8 encoded Unicode codepoint |

**Unicode (`\uXXXX`):** four hex digits are parsed and the codepoint is encoded into UTF-8 via `appendUtf8()`. The encoder handles the full Unicode range:

```
U+0000–U+007F    → 1-byte UTF-8
U+0080–U+07FF    → 2-byte UTF-8
U+0800–U+FFFF    → 3-byte UTF-8
U+10000–U+10FFFF → 4-byte UTF-8 (via surrogate pair decoding)
```

**Surrogate pairs (`\uD800\uDCxx`):** when a high surrogate (`D800–DBFF`) is encountered, the parser immediately reads the following `\uXXXX` as the low surrogate (`DC00–DFFF`) and combines them into a codepoint above U+10000.

---

### Serialization Design

`dump()` has two overloads with different performance characteristics:

```cpp
std::string  dump(int indent = 0) const;           // string version
void         dump(std::ostream& os, int indent = 0) const; // stream version
```

The stream version writes directly to the output stream, avoiding intermediate string allocations per recursive call. The string version delegates to the stream version via `std::ostringstream` — so both ultimately use the same code path.

Indentation is handled by a local `pad(n)` function that returns `n` spaces. Arrays and objects are formatted with trailing commas on all but the last element, consistent with standard JSON output.

---

### operator[] vs at()

| Method          | Out-of-bounds behavior        | Missing key behavior           |
|-----------------|-------------------------------|--------------------------------|
| `operator[](i)` | Returns default `Json()` (null) | —                            |
| `operator[](k)` | —                             | Inserts empty `Json()` (null)  |
| `at(i)`         | Throws `std::out_of_range`    | —                              |
| `at(k)`         | —                             | Throws `std::out_of_range`     |

`operator[]` on a missing key silently inserts a null value — the same behavior as `std::unordered_map::operator[]`. Use `at()` when you want an exception on missing keys, or `contains()` to check first.

---

### Custom Backend

The array and object types are defined via type aliases that can be swapped at compile time:

```cpp
#ifdef JSON_USE_CUSTOM
    // using ArrayType  = VectorPro<Json>;
    // using ObjectType = HashMap<std::string, Json>;
#else
    using ArrayType  = std::vector<Json>;
    using ObjectType = std::unordered_map<std::string, Json>;
#endif
```

Define `JSON_USE_CUSTOM` and uncomment the custom lines to use `VectorPro` and `HashMap` as the backing containers. The rest of the library is unaffected.

---

## Supported JSON

| Feature                        | Supported |
|--------------------------------|:---------:|
| null                           | ✅        |
| true / false                   | ✅        |
| Integer numbers                | ✅        |
| Floating-point numbers         | ✅        |
| Negative numbers               | ✅        |
| Scientific notation (`1e3`)    | ✅        |
| Strings                        | ✅        |
| Escape sequences (`\n`, `\t`, etc.) | ✅   |
| Unicode escapes (`\uXXXX`)     | ✅        |
| Surrogate pairs (`\uD800\uDCxx`) | ✅      |
| Arrays                         | ✅        |
| Nested arrays                  | ✅        |
| Objects                        | ✅        |
| Nested objects                 | ✅        |
| Mixed nested structures        | ✅        |
| Trailing commas                | ❌        |
| Comments                       | ❌        |
| Leading zeros (`007`)          | ❌ (error)|
| Trailing characters after value | ❌ (error)|

---

## Complexity

| Operation              | Complexity | Notes                                               |
|------------------------|:----------:|-----------------------------------------------------|
| `parse()` — primitive  | O(n)       | n = input length; linear scan                       |
| `parse()` — array      | O(n)       | One pass; each element parsed once                  |
| `parse()` — object     | O(n)       | One pass; each key-value pair parsed once           |
| `isNull()` / `isBool()` etc. | O(1) | Enum comparison only                              |
| `asBool()` / `asNumber()` etc. | O(1) | `std::get_if` on variant                        |
| `operator[](index)`    | O(1)       | Direct vector index                                 |
| `operator[](key)`      | O(1) avg   | Hash map lookup                                     |
| `at(index)`            | O(1)       | Bounds check + vector index                         |
| `at(key)`              | O(1) avg   | Bounds check + hash map lookup                      |
| `contains(key)`        | O(1) avg   | Hash map lookup                                     |
| `size()`               | O(1)       | Vector or map size                                  |
| `dump()` — string      | O(n)       | n = total node count; recursive traversal           |
| `dump()` — stream      | O(n)       | Same traversal; avoids per-node string allocation   |
| Copy construction      | O(n)       | Deep copies variant (including nested structures)   |
| Move construction      | O(1)       | Moves variant; source becomes null                  |
| `operator==`           | O(n)       | Recursive deep comparison                           |

---

## Quick Example

### Parsing

```cpp
#include "Json.h"

// Primitives
Json null   = Json::parse("null");
Json b      = Json::parse("true");
Json n      = Json::parse("3.14");
Json s      = Json::parse("\"hello\"");

// Array
Json arr = Json::parse("[1, 2, 3]");

// Object
Json obj = Json::parse("{\"name\": \"Claude\", \"age\": 2}");

// Complex nested
Json data = Json::parse(R"({
    "users": [
        {"name": "Alice", "active": true},
        {"name": "Bob",   "active": false}
    ]
})");
```

---

### Direct Construction

```cpp
// Primitives
Json null;               // null
Json b(true);            // bool
Json n(3.14);            // number (double)
Json i(42);              // number (int → double)
Json s("hello");         // string

// Array
Json::ArrayType arr{ Json(1), Json(2), Json(3) };
Json ja(std::move(arr));

// Object
Json::ObjectType obj{
    { "name", Json("Claude") },
    { "age",  Json(2)        }
};
Json jo(std::move(obj));
```

---

### Type Inspection

```cpp
Json j = Json::parse("42");

j.isNull();    // false
j.isBool();    // false
j.isNumber();  // true
j.isString();  // false
j.isArray();   // false
j.isObject();  // false

// Using type()
switch (j.type()) {
    case Json::Type::Null:   break;
    case Json::Type::Bool:   break;
    case Json::Type::Number: break;
    case Json::Type::String: break;
    case Json::Type::Array:  break;
    case Json::Type::Object: break;
}
```

---

### Value Access

```cpp
Json b = Json::parse("true");
Json n = Json::parse("99.5");
Json s = Json::parse("\"hello\"");

b.asBool();    // true
n.asNumber();  // 99.5
s.asString();  // "hello"

// asArray() and asObject() return references
Json arr = Json::parse("[1, 2, 3]");
auto& vec = arr.asArray(); // std::vector<Json>&

Json obj = Json::parse("{\"x\": 1}");
auto& map = obj.asObject(); // std::unordered_map<std::string, Json>&
```

All accessors throw `std::runtime_error` if the type doesn't match.

---

### Navigation

```cpp
Json data = Json::parse(R"({
    "user": {
        "name": "Alice",
        "scores": [90, 95, 100]
    }
})");

data["user"]["name"].asString();        // "Alice"
data["user"]["scores"][0].asNumber();   // 90
data["user"]["scores"][2].asNumber();   // 100

// Missing key — returns null (operator[] inserts it silently)
data["user"]["missing"].isNull();       // true
```

---

### Bounds-Checked Access

```cpp
Json arr = Json::parse("[10, 20, 30]");

arr.at(0).asNumber();  // 10 — valid
arr.at(2).asNumber();  // 30 — valid

try {
    arr.at(99); // throws std::out_of_range
} catch (const std::out_of_range& e) {
    // "Json::at: index out of range"
}

Json obj = Json::parse("{\"key\": \"value\"}");

obj.at("key").asString(); // "value" — valid

try {
    obj.at("missing"); // throws std::out_of_range
} catch (const std::out_of_range& e) {
    // "Json::at: key not found"
}
```

---

### Serialization

```cpp
Json::ObjectType obj{
    { "name", Json("Claude") },
    { "age",  Json(2)        }
};
Json j(std::move(obj));

// To string (with indent)
std::string s = j.dump(2);
std::cout << s;
// {
//   "name": "Claude",
//   "age": 2
// }

// To stream (more efficient for large structures)
j.dump(std::cout, 2);
```

---

### Comparison

```cpp
Json a(42);
Json b(42);
Json c(99);

a == b;  // true
a != c;  // true
a == c;  // false

// Deep comparison for arrays and objects
Json arr1 = Json::parse("[1, 2, 3]");
Json arr2 = Json::parse("[1, 2, 3]");
Json arr3 = Json::parse("[1, 2, 4]");

arr1 == arr2;  // true
arr1 != arr3;  // true
```

---

### Copy & Move

```cpp
Json a("original");

// Copy — independent from source
Json b(a);
b.asString();  // "original"

// Move — source becomes null
Json c(std::move(a));
c.asString();  // "original"
a.isNull();    // true

// Copy assignment
Json d;
d = b;
d.asString();  // "original"

// Move assignment
Json e;
e = std::move(b);
b.isNull();    // true
```

---

### Mixed Nested Structure

```cpp
Json::ObjectType obj{
    {
        "pokemon",
        Json::ArrayType{
            Json::ObjectType{
                { "name",   Json("Rayquaza") },
                { "health", Json(5000)       },
                { "shiny",  Json(true)       },
                { "stats",  Json::ArrayType{ Json(100), Json(90), Json(100), Json(95) } }
            }
        }
    }
};

Json j(std::move(obj));

j["pokemon"].size();                    // 1
j["pokemon"][0]["name"].asString();     // "Rayquaza"
j["pokemon"][0]["shiny"].asBool();      // true
j["pokemon"][0]["stats"][0].asNumber(); // 100
```

---

## Core API

### Constructors

```cpp
Json();                          // null
Json(std::nullptr_t);            // null
Json(bool value);                // bool
Json(double value);              // number
Json(int value);                 // number (cast to double)
Json(const std::string& value);  // string
Json(std::string&& value);       // string (move)
Json(const char* value);         // string
Json(const ArrayType& value);    // array
Json(ArrayType&& value);         // array (move)
Json(const ObjectType& value);   // object
Json(ObjectType&& value);        // object (move)

Json(const Json& other);         // copy
Json& operator=(const Json& other);

Json(Json&& other) noexcept;     // move — source becomes null
Json& operator=(Json&& other) noexcept;
```

---

### Parsing

```cpp
// Static factory. Throws std::runtime_error with line/column on failure.
[[nodiscard]] static Json parse(std::string_view text);
```

---

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

---

### Value Access

```cpp
// All throw std::runtime_error if the type doesn't match.
[[nodiscard]] bool               asBool()   const;
[[nodiscard]] double             asNumber() const;
[[nodiscard]] const std::string& asString() const;

[[nodiscard]] ArrayType&         asArray();
[[nodiscard]] const ArrayType&   asArray()  const;

[[nodiscard]] ObjectType&        asObject();
[[nodiscard]] const ObjectType&  asObject() const;
```

---

### Navigation

```cpp
// Unchecked. Missing key inserts null. Out-of-bounds index returns null.
[[nodiscard]] Json&       operator[](const std::string& key);
[[nodiscard]] const Json& operator[](const std::string& key) const;

[[nodiscard]] Json&       operator[](std::size_t index);
[[nodiscard]] const Json& operator[](std::size_t index) const;
```

---

### Element Access

```cpp
// Bounds-checked. Throws std::out_of_range on invalid access.
[[nodiscard]] Json&       at(std::size_t index);
[[nodiscard]] const Json& at(std::size_t index) const;

[[nodiscard]] Json&       at(const std::string& key);
[[nodiscard]] const Json& at(const std::string& key) const;
```

---

### Utilities

```cpp
// Returns element count for arrays and objects. Returns 0 for all other types.
[[nodiscard]] std::size_t size() const noexcept;

// Returns true if the Json is an object and the key exists. False otherwise.
[[nodiscard]] bool contains(const std::string& key) const noexcept;
```

---

### Comparison

```cpp
[[nodiscard]] bool operator==(const Json& other) const; // deep equality
[[nodiscard]] bool operator!=(const Json& other) const;
```

---

### Serialization

```cpp
// Serialize to std::string. indent controls spaces per level (0 = compact).
[[nodiscard]] std::string dump(int indent = 0) const;

// Serialize directly to any std::ostream. More efficient for large structures.
void dump(std::ostream& os, int indent = 0) const;
```

---

## Error Handling

All parse errors throw `std::runtime_error` with a message that includes line and column:

```
line 3, col 12: expected ':'
line 1, col 8: invalid escape sequence
line 2, col 5: unterminated string
```

Type mismatch errors (wrong `as*()` call) throw `std::runtime_error`:

```
Json: not a bool
Json: not a number
Json: not a string
Json: not an array
Json: not an object
```

Bounds errors from `at()` throw `std::out_of_range`:

```
Json::at: index out of range
Json::at: key not found
```

---

## Benchmark Results

All benchmarks compiled without `-O2`. Times in microseconds (µs).

---

### Parse vs Construct

100,000 iterations per type — measures overhead of parsing text vs direct C++ construction.

| Type   | parse() (µs) | construct (µs) | parse overhead |
|--------|-------------:|---------------:|:--------------:|
| null   | 77,457       | 16,184         | **4.8×**       |
| bool   | 76,644       | 17,491         | **4.4×**       |
| number | 142,726      | 18,216         | **7.8×**       |
| string | 150,273      | 31,010         | **4.8×**       |

Number parsing is the most expensive — `std::stod` carries significant overhead compared to a direct `double` constructor. String parsing is next, due to escape scanning and string building. Null and bool parsing are cheaper since they just do a fixed-length string comparison (`"null"`, `"true"`, `"false"`).

**Takeaway:** if you're constructing known values in C++ code, prefer direct construction. Reserve `parse()` for actual external input.

---

### Array — Parse vs Construct

Single parse or construction per size.

| Size   | parse() (µs) | construct (µs) | parse overhead |
|--------|-------------:|---------------:|:--------------:|
| 10     | 124          | 9              | **13.8×**      |
| 100    | 295          | 85             | **3.5×**       |
| 1,000  | 2,394        | 689            | **3.5×**       |
| 10,000 | 27,615       | 7,379          | **3.7×**       |

Parsing adds consistent ~3.5× overhead at scale — the extra cost of text scanning, tokenizing, and `std::stod` per element vs direct `Json(int)` construction. The 10-element gap is disproportionally large due to fixed parser setup cost.

---

### Object — Parse vs Construct

Single parse or construction per size.

| Size   | parse() (µs) | construct (µs) | parse overhead |
|--------|-------------:|---------------:|:--------------:|
| 10     | 74           | 22             | **3.4×**       |
| 100    | 420          | 200            | **2.1×**       |
| 1,000  | 4,251        | 2,008          | **2.1×**       |
| 10,000 | 46,919       | 22,261         | **2.1×**       |

Object parsing adds a consistent ~2× overhead vs construction. Objects are relatively cheaper to parse than arrays per element because key string generation (`"key0"`, `"key1"`, ...) in the construction benchmark also involves `std::to_string` overhead, narrowing the gap.

---

### Nested — Parse vs Construct

Deeply nested arrays (`[[[[42]]]]`) at increasing depth.

| Depth | parse() (µs) | construct (µs) | Notes                    |
|-------|-------------:|---------------:|:-------------------------|
| 2     | 26           | 12             | parse faster             |
| 4     | 9            | 14             | parse faster             |
| 8     | 15           | 42             | parse ~3× faster         |
| 16    | 29           | 131            | **parse ~4.5× faster**   |

This is the most surprising result. For deep nesting, `parse()` is significantly faster than manual construction. The reason: parsing builds the structure bottom-up via `std::move` in recursive calls, while the construction benchmark wraps each level in a new `Json::ArrayType{ std::move(j) }` — which involves repeated vector allocation and copy at each depth level. At depth 16, construction is 4.5× slower.

---

### Dump — String vs Stream

| Size   | dump(string) (µs) | dump(stream) (µs) | Notes            |
|--------|------------------:|------------------:|:-----------------|
| 10     | 31                | 14                | stream 2.2× faster |
| 100    | 158               | 133               | stream 1.2× faster |
| 1,000  | 1,316             | 1,304             | ~tie             |
| 10,000 | 13,215            | 13,082            | ~tie             |

Stream is faster for small structures because `dump(string)` constructs an `ostringstream` internally and then calls `.str()` to extract the result — two allocations vs one. At large sizes, both paths are dominated by the recursive traversal cost and the difference vanishes.

**Takeaway:** use `dump(ostream)` when writing to a file or socket. Use `dump(string)` when you need the result as a `std::string`. The difference only matters for small outputs.

---

### Array Access — operator[] vs at()

1,000-element array, 10,000 accesses.

| Type  | operator[] (µs) | at() (µs) | Overhead      |
|-------|----------------:|----------:|:-------------:|
| Array | 1,027           | 1,137     | **~1.1×**     |

The bounds check in `at()` adds ~10% overhead — a size comparison and a throw path that is never taken in the benchmark. In practice this difference is negligible.

---

### Object Access — operator[] vs at()

1,000-key object, 10,000 accesses.

| Type   | operator[] (µs) | at() (µs) | Overhead  |
|--------|----------------:|----------:|:---------:|
| Object | 6,694           | 6,826     | **~1.0×** |

Effectively identical. Object access is dominated by the hash map lookup cost — the additional `find` + `end()` check in `at()` is the same operation as `operator[]`'s internal lookup, so the overhead is immeasurable.

---

### Summary

| Benchmark                  | Faster             | Notes                                              |
|----------------------------|--------------------|----------------------------------------------------|
| Parse vs construct (null)  | Construct          | 4.8× faster                                       |
| Parse vs construct (bool)  | Construct          | 4.4× faster                                       |
| Parse vs construct (number)| Construct          | 7.8× faster — `std::stod` dominates parse cost    |
| Parse vs construct (string)| Construct          | 4.8× faster                                       |
| Array parse vs construct   | Construct          | ~3.5× faster at scale                             |
| Object parse vs construct  | Construct          | ~2× faster at scale                               |
| Nested parse vs construct  | **Parse** (deep)   | Parse up to 4.5× faster at depth 16               |
| dump string vs stream      | Stream (small)     | Tie at large sizes; stream ~2× faster for small   |
| Array operator[] vs at()   | operator[]         | ~10% difference — negligible                      |
| Object operator[] vs at()  | Tie                | Effectively identical                              |

> Construct directly in C++ when values are known at compile time. Use `parse()` for external input — it's your only option there. For deeply nested structures, `parse()` is actually faster than manual construction due to move semantics in the recursive parser.

---

## Project Structure

```
JsonParser/
├── include/
│   ├── Json.h         # Json class declaration (types, API, variant storage)
│   └── Parser.h       # Parser class declaration
│
├── src/
│   ├── Json.cpp       # Json method implementations + dump logic
│   └── Parser.cpp     # Recursive descent parser implementation
│
├── benchmarks/
│   ├── benchmarks.cpp # 7 benchmark suites (parse, construct, dump, access)
│   └── utils/
│       ├── Table.h    # Benchmark result table formatting
│       └── Table.tpp
│
├── tests/
│   └── test.cpp       # Unit tests: lifecycle, types, access, unicode, escapes, nesting
│
├── examples/
│   └── examples.cpp   # Usage examples for all major features
│
├── README.md
└── LICENSE
```

---

## Build Instructions

### Requirements

- C++23-compatible compiler: GCC 13+, Clang 17+, or MSVC 19.38+
- No external dependencies

### Compile & Run Tests

```bash
g++ -std=c++23 tests/test.cpp src/* -Iinclude -o build/test
./build/tests
```

### Compile & Run Examples

```bash
g++ -std=c++23 examples/examples.cpp src/* -Iinclude -o build/examples
./build/examples
```

### Compile & Run Benchmarks

```bash
g++ -std=c++23 benchmarks/benchmarks.cpp src/* -Iinclude -Ibenchmarks/utils -o build/benchmarks
./build/benchmarks
```

> Use `-O2` or `-O3` for production-representative benchmark results.

---

## Notes

- **Not production-ready.** This is an educational project — use a battle-tested library like [nlohmann/json](https://github.com/nlohmann/json) or [simdjson](https://github.com/simdjson/simdjson) for real workloads.
- All JSON numbers are stored as `double`. Very large integers may lose precision beyond 2⁵³.
- `operator[]` on a missing object key silently inserts a null value — the same footgun as `std::unordered_map::operator[]`. Use `contains()` + `at()` when you don't want silent insertion.
- `contains()` only works on objects. Calling it on any other type returns `false`.
- `size()` returns 0 for non-array, non-object types (null, bool, number, string).
- After a move, the source `Json` becomes null (`type_ == Type::Null`).
- Trailing commas and comments are not supported — they are not valid JSON per the spec.
- The `JSON_USE_CUSTOM` backend swap is compile-time only — both containers must satisfy the same interface as `std::vector` and `std::unordered_map` respectively.

---

## Contributing

Learning-focused PRs and improvements are welcome. Some areas worth exploring:

- `std::int64_t` storage for integer values to avoid `double` precision loss
- Streaming parser (parse without loading the full string into memory)
- JSON Pointer support (`/users/0/name`)
- `merge()` — merge two JSON objects
- Schema validation
- CMake build system
- CI pipeline (GitHub Actions)

---

## License

[MIT](LICENSE) — free to use, modify, and distribute for educational and personal purposes.
