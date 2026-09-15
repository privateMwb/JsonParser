// ============================================================
// fuzz/fuzz_json.cpp
//
// Round-trip fuzzer for JsonPro::Json. There's no independent
// reference implementation to differentially compare against here
// (unlike a container with std::vector as a shadow model), so this
// instead checks the one property a parser/serializer pair must
// always satisfy: parse(dump(parse(text))) == parse(text).
//
// Specifically targets:
//   - parse() never throwing anything other than JsonParseError for
//     malformed input -- any other JsonPro exception type escaping
//     it (JsonTypeError, JsonOutOfRange, or the base JsonException)
//     is itself a bug, since parse() only ever documents throwing
//     JsonParseError.
//   - dump() always producing text that parse() itself accepts --
//     a value obtained from parse() can never contain a non-finite
//     number (JSON numbers are always finite per the grammar), so
//     dump()'s only documented throw path is unreachable here, and
//     any parse failure on dump()'s own output is a real
//     serialization bug (missing escape, malformed number
//     formatting, etc.).
//   - operator==() surviving that round trip: the reparsed value
//     must be structurally equal to the value that produced it,
//     across all indent widths, for every JSON type (null, bool,
//     number, string, array, object) and arbitrary nesting.
//   - the recursive-descent parser's depth cap (kMaxDepth = 512 in
//     Parser.h): pathologically nested input is expected to raise
//     JsonParseError, not overflow the stack -- fuzzing naturally
//     generates deeply-nested `[[[[...` and `{"a":{"a":{...` inputs
//     that exercise exactly this limit.
//   - string escaping/unescaping fidelity, including \uXXXX escapes
//     and surrogate pairs, since the corpus mutates raw bytes that
//     land inside string literals as often as it mutates structural
//     JSON syntax.
//
// Deliberately NOT covered yet: JsonObject and Parser exercised only
// indirectly through Json::parse()/dump(), not through their own
// APIs directly (insert/erase, constructing a Parser by hand); the
// text of exception messages (only exception *type* is checked);
// and performance/algorithmic-complexity regressions -- this harness
// catches crashes and sanitizer violations, not slowdowns.
// ============================================================

#include <JsonPro/Json.h>
#include <JsonPro/JsonException.h>

#include <cstdint>
#include <cstdlib>
#include <string>
#include <string_view>

using JsonPro::Json;
using JsonPro::JsonException;
using JsonPro::JsonParseError;

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t* data, std::size_t size) {
    if (size == 0)
        return 0;

    // Last byte selects the indent width for the round-trip dump,
    // rather than consuming it from the front -- taking it from the
    // front would shift every offset in the actual JSON text the
    // corpus/dictionary is mutating.
    static constexpr int kIndents[] = {0, 1, 2, 4, 8};
    const int indent = kIndents[data[size - 1] % 5];
    --size;

    const std::string_view text(reinterpret_cast<const char*>(data), size);

    Json value;
    try {
        value = Json::parse(text);
    } catch (const JsonParseError&) {
        // Expected outcome for the overwhelming majority of random
        // inputs -- most byte strings are not valid JSON.
        return 0;
    } catch (const JsonException&) {
        // parse() is documented to throw only JsonParseError; any
        // other JsonPro exception type escaping it is the bug.
        std::abort();
    }

    // Round-trip: serialize, then re-parse. The dumped text must
    // always be valid JSON, and re-parsing it must reproduce a
    // structurally equal value -- if either fails, dump() and
    // parse() disagree about what one of them just
    // produced/accepted.
    std::string dumped;
    try {
        dumped = value.dump(indent);
    } catch (const JsonException&) {
        // dump() only throws for a non-finite number, which parse()
        // can never produce (JSON numbers are always finite) -- so
        // this is unreachable for a value that came from parse(),
        // and reaching it is a bug.
        std::abort();
    }

    Json reparsed;
    try {
        reparsed = Json::parse(dumped);
    } catch (const JsonException&) {
        std::abort(); // dump() produced text its own parser rejects.
    }

    if (reparsed != value)
        std::abort(); // dump()/parse() round-trip lost or changed data.

    // Cheap self-consistency check on operator==/!= while we're here.
    if (value != value || !(value == value))
        std::abort();

    return 0;
}
