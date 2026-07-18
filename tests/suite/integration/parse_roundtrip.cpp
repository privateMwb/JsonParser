// Json parse/dump round-trip integration test suite.
//
// Coverage:
// - parse() -> dump() -> parse() preserves value equality for every scalar type
// - Empty array / object round-trip
// - A realistic nested document survives a full round-trip
// - dump() at different indent levels reparses to the same logical value
// - parse() (copies input) and parseOwned() (moves input) agree on the same text
// - dump() output is stable across a second parse/dump cycle (idempotent)
// - Escaped / Unicode string content survives a round-trip

#include <common/framework.h>

using namespace JsonPro;

// Verifies every scalar literal survives a parse -> dump -> parse cycle unchanged.
static void roundtrip_scalar_values() {
    const char* literals[] = {"null", "true", "false", "42",
                              "-17",  "3.14", "1e3",   R"("hello world")"};

    for (const char* lit : literals) {
        Json original = Json::parse(lit);
        Json reparsed = Json::parse(original.dump());
        CHK(original == reparsed);
    }
}

// Verifies empty arrays and objects survive a round-trip.
static void roundtrip_empty_containers() {
    Json arr = Json::parse("[]");
    Json obj = Json::parse("{}");

    CHK(Json::parse(arr.dump()) == arr);
    CHK(Json::parse(obj.dump()) == obj);
}

// Verifies a realistic, deeply nested document survives a full round-trip.
static void roundtrip_nested_document() {
    const std::string text = R"({
        "id": 1,
        "name": "widget",
        "tags": ["a", "b", "c"],
        "meta": {"active": true, "score": 4.5},
        "children": [{"id": 2}, {"id": 3}]
    })";

    Json original = Json::parse(text);
    Json reparsed = Json::parse(original.dump());

    CHK(original == reparsed);
}

// Verifies dump() at different indent widths produces different text but
// reparses to the same logical value.
static void roundtrip_stable_across_indent_levels() {
    Json j = Json::parse(R"({"a": [1, 2], "b": {"c": 3}})");

    std::string compact = j.dump(0);
    std::string indented = j.dump(4);

    CHK(compact != indented);
    CHK(Json::parse(compact) == Json::parse(indented));
}

// Verifies parse() (which copies its input) and parseOwned() (which moves
// its input) produce equal results for the same source text.
static void parse_and_parseOwned_produce_equal_results() {
    const std::string text = R"({"a": [1, 2, 3], "b": "text"})";

    Json viaParse = Json::parse(text);      // text copied internally
    Json viaOwned = Json::parseOwned(text); // parameter copied since text is const

    CHK(viaParse == viaOwned);
}

// Verifies a second parse/dump cycle produces byte-identical output to the first.
static void dump_reparse_is_idempotent() {
    const std::string text = R"({"a": 1, "b": [2, 3], "c": {"d": 4}})";

    Json first = Json::parse(text);
    std::string firstDump = first.dump();

    Json second = Json::parse(firstDump);
    std::string secondDump = second.dump();

    CHK(firstDump == secondDump);
}

// Verifies escaped and Unicode string content survives a round-trip intact.
static void roundtrip_preserves_unicode_content() {
    Json original = Json::parse(R"("caf\u00e9 \ud83d\ude00")");
    Json reparsed = Json::parse(original.dump());

    CHK(reparsed == original);
    CHK(reparsed.asString() == original.asString());
}

// Executes all parse/dump round-trip test cases.
static void run_tests() {
    RUN(roundtrip_scalar_values);
    RUN(roundtrip_empty_containers);
    RUN(roundtrip_nested_document);
    RUN(roundtrip_stable_across_indent_levels);
    RUN(parse_and_parseOwned_produce_equal_results);
    RUN(dump_reparse_is_idempotent);
    RUN(roundtrip_preserves_unicode_content);
}

REGISTER_TEST_SUITE();
