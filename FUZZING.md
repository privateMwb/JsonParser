# Fuzzing

JsonParser is fuzzed via [ClusterFuzzLite](https://google.github.io/clusterfuzzlite/),
running on every pull request that touches the fuzzed files, plus a
longer scheduled batch run every night.

## What's covered

**`fuzz_json.cpp`** is a round-trip fuzzer. There's no independent
reference implementation to differentially compare against for a
parser/serializer (unlike a container, where `std::vector` can serve
as a shadow model), so instead it checks the one property a
parser/serializer pair must always satisfy:
`parse(dump(parse(text))) == parse(text)`.

Specifically exercised:

- **`parse()`'s exception contract.** `Json::parse()` is documented
  to throw only `JsonParseError` for malformed input. Any other
  `JsonPro` exception type escaping it — `JsonTypeError`,
  `JsonOutOfRange`, or the base `JsonException` — is treated as a bug
  in its own right, not just a rejected input.
- **`dump()` always producing text `parse()` itself accepts.** A
  value obtained from `parse()` can never contain a non-finite number
  (JSON numbers are always finite per the grammar), so `dump()`'s
  only documented throw path is unreachable here — reaching it, or
  producing text that fails to re-parse, points at a real
  serialization bug (a missing escape, malformed number formatting,
  etc.).
- **Structural equality surviving the round trip**, via `operator==`,
  across every indent width `dump()` supports and every JSON type
  (null, bool, number, string, array, object), at arbitrary nesting.
- **The recursive-descent parser's depth cap** (`kMaxDepth = 512` in
  `Parser.h`). Fuzzing naturally generates pathologically nested
  input like `[[[[[...` and `{"a":{"a":{"a":...`, which is exactly
  what that cap exists to turn into a clean `JsonParseError` instead
  of a stack overflow.
- **String escaping/unescaping fidelity**, including `\uXXXX` escapes
  and surrogate pairs — the corpus mutates raw bytes that land inside
  string literals just as often as it mutates structural JSON syntax.

Built and run under both AddressSanitizer and UndefinedBehaviorSanitizer.

## What's deliberately NOT covered yet

- **`JsonObject` and `Parser` exercised directly.** Both are only
  fuzzed indirectly, through `Json::parse()`/`dump()`. Constructing a
  `Parser` by hand, or fuzzing `JsonObject::insert()`/`erase()`
  directly against an insertion-order-preserving shadow map, would be
  a natural follow-up harness, not a change to this one.
- **Exception message content.** The harness checks exception *type*
  only (e.g., that `parse()` never throws anything but
  `JsonParseError`), not that a given malformed input produces a
  specific, correct diagnostic message or line/column.
- **Performance / algorithmic-complexity regressions.** This harness
  catches crashes and sanitizer violations, not slowdowns — a
  quadratic-blowup input wouldn't be flagged unless it also ran long
  enough to hit libFuzzer's timeout.

## Running locally

```bash
git clone --recursive https://github.com/google/oss-fuzz.git
cd oss-fuzz
python infra/helper.py build_fuzzers --sanitizer address JsonParser /path/to/JsonParser
python infra/helper.py run_fuzzer JsonParser fuzz_json
```

Or, without OSS-Fuzz's tooling, directly with clang (linking the
library's translation units in by hand, since JsonParser is not
header-only):

```bash
clang++ -std=c++20 -fsanitize=fuzzer,address \
  -Iinclude \
  fuzz/fuzz_json.cpp src/JsonPro/Json.cpp src/JsonPro/Parser.cpp src/JsonPro/JsonObject.cpp \
  -o fuzz_json

./fuzz_json
```

Add `-fsanitize=fuzzer,undefined` instead to run under UBSan.

## Reproducing a crash

ClusterFuzzLite uploads the failing input as a workflow artifact when
a run fails. Download it, then:

```bash
./fuzz_json path/to/crash-<hash>
```

This replays that exact byte sequence through
`LLVMFuzzerTestOneInput()` once, deterministically — no sanitizer flags
needed beyond however the binary was already built.

## Adding a new harness

1. Add `fuzz/fuzz_<target>.cpp` with an `extern "C" int
   LLVMFuzzerTestOneInput(const uint8_t*, size_t)` entry point.
2. Add the matching compile + link block to `.clusterfuzzlite/build.sh`,
   linking against the same `src/JsonPro/*.cpp` object files.
3. No workflow changes needed — `cflite_pr.yml`/`cflite_batch.yml`
   build and run every binary `build.sh` produces in `$OUT`.
