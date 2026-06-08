# Json

![C++](https://img.shields.io/badge/C%2B%2B-23-blue)
![Status](https://img.shields.io/badge/status-learning-green)

A lightweight JSON parser and serializer written in modern C++23.

---

## Overview

Json is a custom JSON library built from scratch to explore parser design, recursive data structures, variant-based value storage, serialization, and modern C++ programming techniques.

The project supports parsing JSON text into native C++ data structures, accessing values through a simple API, and serializing data back into valid JSON.

---

## Motivation / Goals

This project was created to deepen understanding of:

- Modern C++23
- Recursive descent parsing
- Variant-based type systems
- Object and array data structures
- String processing
- Unicode handling
- Serialization and deserialization
- API design
- Unit testing
- Benchmarking

The goal is educational rather than replacing production-ready libraries such as `nlohmann/json`.

---

## Features

### Supported JSON Types

- Null
- Boolean
- Number
- String
- Array
- Object

### Parsing

- Parse JSON from strings
- Nested arrays
- Nested objects
- Mixed data types
- Escape sequences
- Unicode escape sequences
- UTF-16 surrogate pair support

### Access

- Object member lookup
- Array indexing
- Nested access chains

### Serialization

- Convert JSON values back to JSON text
- Pretty-print support

### Testing

- Unit tests for all supported types
- Edge case coverage
- Unicode validation

### Benchmarking

- Parse performance benchmarks
- Object access benchmarks
- Array access benchmarks

---

## Design Overview

The parser uses a recursive descent design.

```text
JSON Text
    │
    ▼
 Parser
    │
    ▼
  Json
    │
    ▼
std::variant
    │
    ├──► nullptr_t
    ├──► bool
    ├──► double
    ├──► std::string
    ├──► Array  ─────► std::vector<Json>
    └──► Object ─────► std::unordered_map<std::string, Json>
```

### Object Storage

```cpp
using Object = std::unordered_map<std::string, Json>;
```

### Array Storage

```cpp
using Array = std::vector<Json>;
```

### Value Storage

```cpp
std::variant<
    std::nullptr_t,
    bool,
    double,
    std::string,
    Array,
    Object
>
```

## Complexity

| Operation | Complexity |
|------------|------------|
| Parse | O(n) |
| Object Access | O(1) average |
| Array Access | O(1) |
| String Access | O(1) |
| Serialization | O(n) |

---

## Quick Example

```cpp
#include "Json.h"

int main() {
    Json json = Json::parse(
        "{"
        "\"name\":\"Mark\","
        "\"age\":24"
        "}"
    );

    std::cout << json["name"].asString() << '\n';
    std::cout << json["age"].asNumber() << '\n';
}
```

---

## Core API

### Parsing

```cpp
Json::parse(text);
```

### Type Checks

```cpp
isNull();
isBool();
isNumber();
isString();
isArray();
isObject();
```

### Type Conversion

```cpp
asBool();
asNumber();
asString();
asArray();
asObject();
```

### Access

```cpp
json["key"];
json[index];
```

### Serialization

```cpp
dump();
dump(indent);
```

---

## Benchmark Results

### Parse Benchmark

| Iterations | Time |
|------------|------|
| 10,000 | varies by machine |
| 20,000 | varies by machine |
| 40,000 | varies by machine |
| 80,000 | varies by machine |

### Object Access Benchmark

| Iterations | Time |
|------------|------|
| 10,000 | varies by machine |
| 20,000 | varies by machine |
| 40,000 | varies by machine |
| 80,000 | varies by machine |

### Array Access Benchmark

| Iterations | Time |
|------------|------|
| 10,000 | varies by machine |
| 20,000 | varies by machine |
| 40,000 | varies by machine |
| 80,000 | varies by machine |

## Tests

Current unit tests cover:

- Basic types
- String escapes
- Unicode strings
- Arrays
- Objects
- Nested objects
- Mixed JSON types
- Edge number formats
- Serialization

All tests use `assert()` and are located in `tests/test.cpp`.

---

## Project Structure

```text
Json/
├── include/
│   ├── Json.h
│   └── Parser.h
│
├── src/
│   ├── Json.cpp
│   └── Parser.cpp
│
├── tests/
│   └── test.cpp
│
├── examples/
│   └── examples.cpp
│
├── benchmarks/
│   ├── benchmarks.cpp
│   └── utils/
│       ├── Table.h
│       └── Table.tpp
│
├── README.md
└── LICENSE
```

---

## Build Instructions

### Examples

```bash
g++ -std=c++23 examples/examples.cpp src/*.cpp -Iinclude -o examples
./examples
```

### Tests

```bash
g++ -std=c++23 tests/test.cpp src/*.cpp -Iinclude -o tests
./tests
```

### Benchmarks

```bash
g++ -std=c++23 benchmarks/benchmarks.cpp src/*.cpp -Iinclude -O2 -o benchmarks
./benchmarks
```

---

## Notes

- This project is intended for learning purposes.
- Error reporting is intentionally simple.
- Performance was not prioritized over readability.
- The implementation focuses on correctness and understanding parser internals.
- Object key order depends on the underlying container implementation.

---

## License

MIT License

Feel free to use, modify, and distribute this project.