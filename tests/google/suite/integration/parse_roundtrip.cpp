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

#include <gtest/gtest.h>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Verifies every scalar literal survives a parse -> dump -> parse cycle unchanged.
TEST(ParseRoundtrip, ScalarValues) {
    const char* literals[] = {"null", "true", "false", "42",
                              "-17",  "3.14", "1e3",   R"("hello world")"};

    for (const char* lit : literals) {
        SCOPED_TRACE(lit);
        Json original = Json::parse(lit);
        Json reparsed = Json::parse(original.dump());
        EXPECT_EQ(original, reparsed);
    }
}

// Verifies empty arrays and objects survive a round-trip.
TEST(ParseRoundtrip, EmptyContainers) {
    Json arr = Json::parse("[]");
    Json obj = Json::parse("{}");

    EXPECT_EQ(Json::parse(arr.dump()), arr);
    EXPECT_EQ(Json::parse(obj.dump()), obj);
}

// Verifies a realistic, deeply nested document survives a full round-trip.
TEST(ParseRoundtrip, NestedDocument) {
    const std::string text = R"({
        "id": 1,
        "name": "widget",
        "tags": ["a", "b", "c"],
        "meta": {"active": true, "score": 4.5},
        "children": [{"id": 2}, {"id": 3}]
    })";

    Json original = Json::parse(text);
    Json reparsed = Json::parse(original.dump());

    EXPECT_EQ(original, reparsed);
}

// Verifies dump() at different indent widths produces different text but
// reparses to the same logical value.
TEST(ParseRoundtrip, StableAcrossIndentLevels) {
    Json j = Json::parse(R"({"a": [1, 2], "b": {"c": 3}})");

    std::string compact = j.dump(0);
    std::string indented = j.dump(4);

    EXPECT_NE(compact, indented);
    EXPECT_EQ(Json::parse(compact), Json::parse(indented));
}

// Verifies parse() (which copies its input) and parseOwned() (which moves
// its input) produce equal results for the same source text.
TEST(ParseRoundtrip, ParseAndParseOwnedProduceEqualResults) {
    const std::string text = R"({"a": [1, 2, 3], "b": "text"})";

    Json viaParse = Json::parse(text);      // text copied internally
    Json viaOwned = Json::parseOwned(text); // parameter copied since text is const

    EXPECT_EQ(viaParse, viaOwned);
}

// Verifies a second parse/dump cycle produces byte-identical output to the first.
TEST(ParseRoundtrip, DumpReparseIsIdempotent) {
    const std::string text = R"({"a": 1, "b": [2, 3], "c": {"d": 4}})";

    Json first = Json::parse(text);
    std::string firstDump = first.dump();

    Json second = Json::parse(firstDump);
    std::string secondDump = second.dump();

    EXPECT_EQ(firstDump, secondDump);
}

// Verifies escaped and Unicode string content survives a round-trip intact.
TEST(ParseRoundtrip, PreservesUnicodeContent) {
    Json original = Json::parse(R"("caf\u00e9 \ud83d\ude00")");
    Json reparsed = Json::parse(original.dump());

    EXPECT_EQ(reparsed, original);
    EXPECT_EQ(reparsed.asString(), original.asString());
}
