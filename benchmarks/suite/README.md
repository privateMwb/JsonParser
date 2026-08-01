# Benchmark Suite

This document describes the benchmark categories under `suite/` — what
each one measures, and the individual benchmarks it contains.

Every benchmark compares JsonPro against nlohmann::json, the de facto
standard JSON library for C++. A category can support more than one
standard for comparison, but for now each category is benchmarked
against a single standard.

Most `BENCH()` calls, in most categories below, are automatically
repeated at three iteration tiers — SMALL (10K), MEDIUM (100K), and
LARGE (1M) — to smooth out timing noise and show whether relative
performance holds steady as call volume increases. This applies across
the suite by default. The **Scaling** category below is the exception:
its cases run through `BENCH_CUSTOM()` instead, since it measures
something different — how per-operation cost changes as the JSON
structure itself grows (element count, nesting depth, key count) —
rather than sweeping call volume against a fixed-size structure.

---

## Access

Benchmarks read and lookup operations on an already-built value —
element/member access and typed value extraction.

### Benchmarks

- `element_access.cpp` — array `operator[]` (unchecked), array `at()`
  (bounds-checked), object `operator[]` (existing key, no auto-vivify),
  object `at()` (bounds-checked)
- `lookup.cpp` — `contains()` hit/miss, `find()` hit/miss
- `value_access.cpp` — `asBool()`, `asNumber()`, `asString()`,
  `asArray()`, `asObject()`

---

## Core

Benchmarks the fundamental, most frequently exercised operations —
parsing JSON text into a value tree, serializing a value tree back to
text, and the combined round trip most real workloads actually run.

### Benchmarks

- `parsing.cpp` — `Json::parse()` on a small flat object, a number
  array, a moderately nested object (3 levels), and a string-heavy
  object (escapes, unicode)
- `serialization.cpp` — `Json::dump()` on the same four source shapes
- `round_trip.cpp` — `parse()` immediately followed by `dump()` on the
  same four source shapes

---

## Lifecycle

Benchmarks object lifetime operations — construction, copying, moving,
and destruction — across every value type (null, bool, number, short
and long string, array, object).

### Benchmarks

- `construction.cpp` — constructing each value type from scratch
- `copy.cpp` — copy-constructing each value type from a pre-built source
- `move.cpp` — move-constructing each value type from a freshly built
  source
- `destruction.cpp` — destroying a batch of 10 values of each type
  (isolates the relative destruction gap; absolute numbers include some
  construction cost from building the batch)

---

## Scaling

Benchmarks how per-operation cost changes as the JSON structure itself
grows — a separate axis from the SMALL/MEDIUM/LARGE iteration tiers
described above: those repeat the same fixed-size operation more times,
while Scaling grows the structure being operated on and observes the
resulting cost. All cases here run through `BENCH_CUSTOM()`.

### Benchmarks

- `deep_nesting.cpp` — parsing and dumping a 400-level-deep array,
  parsing a 400-level-deep object (depth capped safely under
  `Parser::kMaxDepth` of 512)
- `large_array.cpp` — parsing, dumping, and iterating a flat 10,000
  element number array
- `large_object.cpp` — parsing and dumping a flat 10,000-member object
- `wide_object.cpp` — building a 10,000-member object via repeated
  insertion, then looking up keys in it — complements `large_object.cpp`
  by exercising the object API directly rather than through
  parse()/dump()

---

## Utility

Benchmarks introspection and bookkeeping operations that don't belong
to any of the categories above — type inspection, structural equality,
and size/capacity queries.

### Benchmarks

- `type_inspection.cpp` — `type()`, `isNull()`, `isBool()`, `isNumber()`,
  `isString()`, `isArray()`, `isObject()`
- `comparison.cpp` — `operator==` on equal numbers, equal strings, equal
  arrays, equal objects, and unequal objects (mismatch found late)
- `size_and_capacity.cpp` — `size()` on an array and an object,
  `empty()` on a non-empty and an empty object
