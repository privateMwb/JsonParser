# Example Suite

This document describes the example categories under `suite/` — what
each one demonstrates, and the individual example files it contains.

Unlike the test suite, an example doesn't assert correctness — it
demonstrates real usage of the library, including deliberate misuse
where instructive (see Misuse), so the reader sees both the correct
pattern and the mistake it guards against.

Every example file ends with `REGISTER_EXAMPLE_SUITE()`, which derives
the suite's category from its containing directory and assigns it a
sequential id within that category. This applies uniformly across
every category below.

---

## Advanced

Demonstrates deeper mechanics of the library — move semantics, parse
error internals, capacity management, and working with `JsonObject`
directly instead of through the `Json` wrapper.

### Examples

- `move_semantics.cpp` — `parse()` copying its input vs `parseOwned()` taking ownership, the move constructor/assignment leaving the source null, moving values into containers
- `object_internals.cpp` — using `JsonObject`'s own API directly: `find()` returning a pointer, iterating `entries()` allocation-free, `emplace()` skipping existing keys, `erase()`'s returned count
- `parse_error_detail.cpp` — catching `JsonParseError` specifically to read its 1-based line/column, building a caret-style diagnostic, distinguishing it from `JsonTypeError`
- `reserve_capacity.cpp` — why `reserve()` exists, reserving both the entry vector and index map in one call, building a large object efficiently before serializing it

---

## Integration

Demonstrates interoperability with the rest of a codebase — loading
configuration files, converting to and from user-defined structs and
STL containers, and exporting documents to other destinations.

### Examples

- `config_loaders.cpp` — parsing a config file, falling back to defaults for optional settings, failing loudly on a missing required setting, assembling a typed `Config` struct
- `export.cpp` — exporting a document to a string and to a file, reading it back, and confirming array and object key order survive the round trip
- `struct_conversion.cpp` — writing `toJson()`/`fromJson()` for a user-defined struct, composing conversions for nested types, round-tripping through serialized text
- `stl_containers.cpp` — converting `std::vector<T>` to a Json array and back, `std::map<std::string, T>` to a Json object and back, nesting STL conversions inside one another

---

## Misuse

Demonstrates common mistakes and the exceptions they lead to, alongside
the correct pattern — including examples shown but not executed, so the
reader can see what to avoid without the program actually invoking
undefined behavior.

### Examples

- `const_access_gotcha.cpp` — non-const `operator[]` silently auto-vivifying a missing key vs const `operator[]` throwing `JsonOutOfRange`; the same asymmetry between array `[]` and `at()`
- `error_handling.cpp` — catching `JsonParseError`, `JsonTypeError`, and `JsonOutOfRange` individually, the parser's maximum nesting depth guard, recovering with a default value
- `malformed_input.cpp` — a sweep of RFC 8259 violations (trailing commas, unquoted keys, single quotes, unterminated strings, invalid escapes, leading zeros, trailing content), each rejected with `JsonParseError`
- `nonfinite_dump.cpp` — `dump()` throwing on NaN and Infinity, the failure surfacing from values nested deep in a document, sanitizing a value before serializing it
- `wrong_type_access.cpp` — every `asX()` accessor throwing `JsonTypeError` on a type mismatch, and checking `type()`/`isX()` first to avoid it

---

## Patterns

Demonstrates common usage idioms built on top of the core API — merging
objects, allocation-free lookup, in-place modification, and composing
documents from small builder functions.

### Examples

- `builder_helpers.cpp` — small helper functions that each build one reusable fragment, composed into a larger document, keeping construction code readable as documents grow
- `lookup_by_view.cpp` — looking up members with `contains()`/`find()` via `std::string_view` directly, including from a slice of a larger buffer, with no temporary `std::string` allocation
- `merge_objects.cpp` — merging two objects with override-wins semantics via `insert_or_assign`, defaults-win semantics via `emplace`, and how an overwritten key keeps its original position
- `modify_reserialize.cpp` — parsing a document, modifying an existing field in place, adding a new field, removing a field via `JsonObject::erase()`, then re-serializing

---

## Quickstart

Demonstrates fundamental, everyday usage — constructing and parsing
values, building documents programmatically, and navigating a parsed
document safely.

### Examples

- `basic_usage.cpp` — constructing scalar/array/object values directly, parsing with `Json::parse()`, type inspection with `type()`/`isX()`, value access with `asX()`, serialization with `dump()`
- `building_documents.cpp` — building an array via `push_back`, building an object via `insert_or_assign` vs `operator[]`, nesting arrays in objects and objects in arrays, assembling a full document from scratch
- `navigating_documents.cpp` — accessing values with `operator[]`, safe access with `at()` and exception handling, traversing and iterating nested arrays/objects, checking presence/type before accessing
