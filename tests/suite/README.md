# Test Suite

This document describes the test categories under `suite/` — what each
one verifies, and the individual test files it contains.

Unlike the benchmark suite, tests validate the library's own
correctness directly — there is no reference implementation to compare
against, so results are simply pass or fail.

Every test suite registers itself automatically via
`REGISTER_TEST_SUITE()` at startup, and is assigned a sequential id
within its category (e.g. `U1`, `U2` for Unit; `L1`, `L2` for
Lifecycle) — there's no suite list to maintain by hand. This applies
uniformly across every category below.

---

## Concurrency

Verifies thread-safety — JsonPro has no internal synchronization, so
its guarantee is the standard library one: concurrent const access to
a shared value is safe, and threads with no shared object need no
synchronization at all. These tests validate both edges of that
guarantee, plus that `Json::parse()` itself carries no shared state
across threads.

### Tests

- `concurrent_reads.cpp` — Many threads reading scalar/container values, chained navigation, `dump()`, and `size()`/`contains()` on one shared, const document
- `independent_instances.cpp` — Threads each building, mutating, and destroying their own private document concurrently; per-thread documents backed by genuinely distinct memory
- `parallel_parsing.cpp` — Many threads calling `Json::parse()` concurrently on distinct input, identical input, and a mix of valid/invalid input; sustained per-thread parsing volume

---

## Integration

Verifies multiple components working together end-to-end — for
example, a full parse-modify-serialize round trip — rather than a
single function in isolation.

### Tests

- `document_building.cpp` — Building a flat object, a nested object, an array, and an array of objects incrementally; building via `JsonObject` then wrapping in `Json`; a hand-built document survives `dump()`/`parse()`
- `nested_navigation.cpp` — Chained `operator[]` through a parsed array-of-objects document, mutating a deep value in place, mixing `at()`/`operator[]`, const navigation, `contains()`-gated access, iterating a 2D array
- `parse_roundtrip.cpp` — `parse()` → `dump()` → `parse()` preserves equality for every scalar type, empty containers, a nested document, different indent levels, `parse()` vs `parseOwned()`, idempotent re-dump, Unicode content
- `error_propagation.cpp` — `JsonParseError`'s line/column, catching via the `JsonException` base, `JsonTypeError`/`JsonOutOfRange` propagating out of a navigation chain, recovering with a fallback document

---

## Lifecycle

Verifies object lifetime operations — construction, destruction,
copying, and moving — across the different value kinds a JSON value
can hold (null, bool, number, string, array, object), for both `Json`
and `JsonObject`.

### Tests

- `construction.cpp` — Default/nullptr construction, bool/double/int construction, string (lvalue/rvalue/C-string), array and object (lvalue/rvalue)
- `move_semantics.cpp` — Move construction/assignment leaving the source Null, self-move, moved-from reassignability, noexcept guarantees
- `copy_semantics.cpp` — Copy construction/assignment produce independent deep copies (Array, Object, nested structures), self-copy
- `object_lifecycle.cpp` — `JsonObject`'s own default/copy/move construction and assignment, self-copy, self-move
- `exception_safety.cpp` — A failed `asX()`/`at()` leaves the value/container unmodified, no auto-vivification on a throwing `at()`, assignment target unchanged when the rhs throws, clean teardown of a large nested document

---

## Regression

Verifies that a specific, previously identified parsing or storage
edge case stays correctly handled. One test file per class of edge
case.

### Tests

- `duplicate_keys.cpp` — Last value wins across repeated object keys, but a repeated key keeps its original insertion position
- `erase_reindex.cpp` — `JsonObject::erase()` correctly reindexes the side hash map after removing an entry, across first/last/middle erases and interleaved erase/insert sequences
- `leading_zeros.cpp` — RFC 8259 leading-zero boundary: `"0"`/`"-0"` valid, `"00"`/`"01"` rejected, `"0.5"`/`"0e5"` valid
- `max_depth.cpp` — Array/object nesting exactly at `Parser::kMaxDepth` accepted, one level beyond rejected, mixed nesting bounded the same way, wide sibling counts unaffected
- `nonfinite_numbers.cpp` — `dump()` (all overloads) rejects NaN/Infinity, including when nested; construction and numeric access remain unaffected
- `surrogate_pairs.cpp` — Astral-plane boundary codepoints (U+10000, U+10FFFF), embedded/consecutive surrogate pairs, codepoints adjacent to the surrogate range, malformed surrogate sequences rejected
- `trailing_input.cpp` — Adjacent values with no separator, stray closing brackets/braces, stray trailing commas, and non-whitespace garbage after a complete value are all rejected

---

## Unit

Verifies individual functions or methods in isolation — the smallest
testable unit of behavior, independent of the categories above.

### Tests

- `comparison.cpp` — `operator==`/`!=` for every scalar type, deep equality for Array/Object, cross-type inequality, order-independent Object equality
- `navigation.cpp` — `operator[]`/`at()` for arrays and objects (mutable and const), wrong-type and out-of-range throws
- `object_storage.cpp` — `JsonObject`'s `operator[]`, `insert_or_assign()`, `emplace()`, `find()`, `reserve()`, `size()`/`empty()`/`contains()`, `erase()`, `entries()` order, order-independent equality
- `scalar_parsing.cpp` — null/bool literals, integer/negative/fractional/exponent number forms, leading-zero rejection, malformed literals, surrounding whitespace
- `serialization.cpp` — `dump()` for every scalar type, empty and populated Array/Object, nested structures, starting indent, `dump(ostream&)` matches `dump(int)`
- `strings_parsing.cpp` — Plain strings, all standard escapes, `\u` BMP escapes, surrogate pairs, invalid/unpaired surrogates, invalid escapes, unescaped control characters, unterminated strings
- `structural_parsing.cpp` — Empty/populated arrays and objects, nested and mixed structures, whitespace tolerance, trailing/missing comma and colon rejection, non-string keys, duplicate keys, max depth
- `type_inspection.cpp` — `type()` and every `isX()` predicate for each value kind, confirming all other predicates are false, `type()` after reassignment
- `utilities.cpp` — `size()` on arrays, objects, and non-container types; `contains()` on objects, absent keys, and non-object types
- `value_access.cpp` — `asBool()`/`asNumber()`/`asString()`/`asArray()`/`asObject()` return correct values, mutable accessors allow in-place modification, each throws on the wrong type
