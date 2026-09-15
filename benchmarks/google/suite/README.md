# Google Benchmark Suite

This document describes the benchmark categories under `suite/` — what each
one measures, and the individual benchmarks it contains.

| Category | Focus |
|---|---|
| [Access](#access) | Reads, lookups, and typed value extraction on an already-built value |
| [Core](#core) | Parsing JSON text, serializing back to text, and the combined round trip |
| [Lifecycle](#lifecycle) | Construction, copying, moving, and destruction |
| [Scaling](#scaling) | Cost vs. structure growth (element count, nesting depth, key count), independent of iteration count |
| [Utility](#utility) | Type inspection, structural equality, and size/capacity queries |

Every benchmark compares JsonPro against nlohmann::json, the de facto
standard JSON library for C++. A category can support more than one
standard for comparison, but for now each category is benchmarked against a
single standard.

Every case below is a pair of `BENCHMARK()`-registered functions — one for
JsonPro, one for nlohmann::json — timed with Google Benchmark's own
iteration auto-calibration. This replaces the custom framework's
SMALL/MEDIUM/LARGE (10K/100K/1M) iteration-tier sweep and its separate
`BENCH_CUSTOM()` path for scale-sensitive cases; Google Benchmark handles
both uniformly, so the **Scaling** category below needs no special-cased
registration the way it does in the custom suite.

---

## Access

Benchmarks read and lookup operations on an already-built value — element/
member access and typed value extraction.

### Benchmarks

| File | What it covers |
|---|---|
| `element_access.cpp` | Array `operator[]` (unchecked), array `at()` (bounds-checked), object `operator[]` (existing key, no auto-vivify), object `at()` (bounds-checked) |
| `lookup.cpp` | `contains()` hit/miss, `find()` hit/miss |
| `value_access.cpp` | `asBool()`, `asNumber()`, `asString()`, `asArray()`, `asObject()` |

---

## Core

Benchmarks the fundamental, most frequently exercised operations — parsing
JSON text into a value tree, serializing a value tree back to text, and the
combined round trip most real workloads actually run.

### Benchmarks

| File | What it covers |
|---|---|
| `parsing.cpp` | `Json::parse()` on a small flat object, a number array, a moderately nested object (3 levels), and a string-heavy object (escapes, unicode) |
| `serialization.cpp` | `Json::dump()` on the same four source shapes |
| `round_trip.cpp` | `parse()` immediately followed by `dump()` on the same four source shapes |

---

## Lifecycle

Benchmarks object lifetime operations — construction, copying, moving, and
destruction — across every value type (null, bool, number, short and long
string, array, object).

### Benchmarks

| File | What it covers |
|---|---|
| `construction.cpp` | Constructing each value type from scratch |
| `copy.cpp` | Copy-constructing each value type from a pre-built source |
| `move.cpp` | Move-constructing each value type from a freshly built source |
| `destruction.cpp` | Destroying a batch of 10 values of each type (isolates the relative destruction gap; absolute numbers include some construction cost from building the batch) |

---

## Scaling

Benchmarks how per-operation cost changes as the JSON structure itself
grows, at a scale where allocation and cache behavior dominate rather than
per-call fixed overhead.

### Benchmarks

| File | What it covers |
|---|---|
| `deep_nesting.cpp` | Parsing and dumping a 400-level-deep array, parsing a 400-level-deep object (depth capped safely under `Parser::kMaxDepth` of 512) |
| `large_array.cpp` | Parsing, dumping, and iterating a flat 10,000-element number array |
| `large_object.cpp` | Parsing and dumping a flat 10,000-member object |
| `wide_object.cpp` | Building a 10,000-member object via repeated insertion, then looking up keys in it — complements `large_object.cpp` by exercising the object API directly rather than through parse()/dump() |

---

## Utility

Benchmarks introspection and bookkeeping operations that don't belong to
any of the categories above — type inspection, structural equality, and
size/capacity queries.

### Benchmarks

| File | What it covers |
|---|---|
| `type_inspection.cpp` | `type()`, `isNull()`, `isBool()`, `isNumber()`, `isString()`, `isArray()`, `isObject()` |
| `comparison.cpp` | `operator==` on equal numbers, equal strings, equal arrays, equal objects, and unequal objects (mismatch found late) |
| `size_and_capacity.cpp` | `size()` on an array and an object, `empty()` on a non-empty and an empty object |

---

## Conventions

These apply only to this Google Benchmark suite — the custom-framework
suite has its own conventions and does not follow these.

- **One file per category topic**, matching the custom suite's breakdown —
  a case moved from `BENCH()` to `BENCHMARK()` keeps the same file it lived
  in before.
- **Two functions per case**: each comparison becomes a pair of free
  functions, `BM_<Case>_JsonPro` and `BM_<Case>_Nlohmann`, each registered
  with its own `BENCHMARK(...)` call immediately after its definition —
  never combined into one function.
- **Setup outside the timed loop**: any source value, pre-built container,
  or fixture is constructed before `for (auto _ : state)`, never inside it,
  unless the benchmark is specifically measuring construction/build cost.
- **`benchmark::DoNotOptimize(...)`** wraps the result of every timed
  operation, in place of the custom framework's `doNotOptimize(...)`.
- **No `BENCHMARK_MAIN()`** in any suite file — `main()` lives once in the
  shared harness so every suite file compiles into a single `benchmarks`
  binary without redefinition conflicts.
- **`#include <JsonPro/Json.h>`** and `#include <nlohmann/json.hpp>` in
  every file; `using namespace JsonPro;` so `Json` needs no qualification,
  matching nlohmann::json's unqualified `nlohmann::json` usage on the
  other side of each comparison.
- **No custom iteration-tier sweep.** The custom suite's SMALL/MEDIUM/LARGE
  (10K/100K/1M) repetition and `BENCH_CUSTOM()`'s tuned sample counts are
  both superseded by Google Benchmark's own iteration auto-calibration — a
  plain `BENCHMARK(...)` registration covers both cases here, with no
  equivalent of `BENCH_CUSTOM()` needed.
