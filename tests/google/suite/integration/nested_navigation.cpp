// Json nested navigation integration test suite.
//
// Coverage:
// - Chained operator[] navigation through a parsed array-of-objects document
// - Mutating a deeply nested value through a chained access path
// - Mixing at() and operator[] across a nested navigation chain
// - const-qualified navigation through a parsed document
// - contains() gating conditional navigation to avoid a throw
// - Iterating a two-dimensional array (array of arrays)

#include <gtest/gtest.h>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Verifies chained operator[] navigates a parsed array-of-objects document.
TEST(NestedNavigation, NavigateParsedArrayOfObjects) {
    Json j = Json::parse(R"({
        "users": [
            {"name": "Alice", "age": 30},
            {"name": "Bob",   "age": 25}
        ]
    })");

    EXPECT_EQ(j["users"].size(), 2);
    EXPECT_EQ(j["users"][0]["name"].asString(), "Alice");
    EXPECT_EQ(j["users"][1]["age"].asNumber(), 25.0);
}

// Verifies a value at the end of a chained access path can be mutated in place.
TEST(NestedNavigation, MutateDeepValueThroughChainedAccess) {
    Json j = Json::parse(R"({"a": {"b": {"c": 1}}})");

    j["a"]["b"]["c"] = Json(99);

    EXPECT_EQ(j["a"]["b"]["c"].asNumber(), 99.0);
}

// Verifies at() and operator[] can be mixed within a single navigation chain,
// and that a bounds violation partway through the chain still throws.
TEST(NestedNavigation, ChainedAtBoundsCheckedNavigation) {
    Json j = Json::parse(R"({"matrix": [[1, 2], [3, 4]]})");

    EXPECT_EQ(j.at("matrix").at(1).at(0).asNumber(), 3.0);
    EXPECT_THROW(j.at("matrix").at(5), std::runtime_error);
}

// Verifies navigation through a const-qualified parsed document.
TEST(NestedNavigation, ConstNavigationThroughParsedDocument) {
    const Json j = Json::parse(R"({"config": {"enabled": true, "limits": [1, 2, 3]}})");

    EXPECT_EQ(j["config"]["enabled"].asBool(), true);
    EXPECT_EQ(j["config"]["limits"][2].asNumber(), 3.0);
    EXPECT_THROW(j["config"]["missing"], std::runtime_error);
}

// Verifies contains() can gate conditional navigation so a missing key
// never triggers a throw.
TEST(NestedNavigation, ContainsGuidesConditionalNavigation) {
    Json j = Json::parse(R"({"a": 1})");

    EXPECT_EQ(j.contains("a"), true);
    EXPECT_EQ(j.contains("b"), false);

    if (j.contains("a"))
        EXPECT_EQ(j["a"].asNumber(), 1.0);

    if (j.contains("b"))
        FAIL() << "unreachable: contains() correctly gated access";
}

// Verifies navigating a two-dimensional array (array of arrays).
TEST(NestedNavigation, NavigateArrayOfArraysMatrix) {
    Json j = Json::parse("[[1, 2, 3], [4, 5, 6], [7, 8, 9]]");

    double sum = 0.0;
    for (std::size_t row = 0; row < j.size(); ++row)
        for (std::size_t col = 0; col < j[row].size(); ++col)
            sum += j[row][col].asNumber();

    EXPECT_EQ(sum, 45.0);
    EXPECT_EQ(j[1][2].asNumber(), 6.0);
}
