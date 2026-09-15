// JSON structural parsing test suite (arrays and objects).
//
// Coverage:
// - Empty and populated arrays / objects
// - Nested arrays, objects, and mixed structures
// - Whitespace tolerance around structural tokens
// - Trailing comma, missing comma, and missing colon rejection
// - Non-string object keys rejected
// - Duplicate object keys: last value wins
// - Maximum nesting depth enforcement (kMaxDepth)
// - Unterminated array / object rejection

#include <JsonPro/Json.h>

#include <gtest/gtest.h>

using namespace JsonPro;

// Verifies an empty array parses to a zero-size Array.
TEST(StructuralParsing, parses_empty_array) {
    Json j = Json::parse("[]");
    EXPECT_TRUE(j.isArray());
    EXPECT_EQ(j.size(), 0);
}

// Verifies an empty object parses to a zero-size Object.
TEST(StructuralParsing, parses_empty_object) {
    Json j = Json::parse("{}");
    EXPECT_TRUE(j.isObject());
    EXPECT_EQ(j.size(), 0);
}

// Verifies a populated array with mixed element types parses correctly.
TEST(StructuralParsing, parses_populated_array) {
    Json j = Json::parse(R"([1, "two", true, null])");
    ASSERT_EQ(j.size(), 4);
    EXPECT_EQ(j[0].asNumber(), 1.0);
    EXPECT_EQ(j[1].asString(), "two");
    EXPECT_TRUE(j[2].asBool());
    EXPECT_TRUE(j[3].isNull());
}

// Verifies a populated object with multiple keys parses correctly.
TEST(StructuralParsing, parses_populated_object) {
    Json j = Json::parse(R"({"a": 1, "b": "two", "c": false})");
    ASSERT_EQ(j.size(), 3);
    EXPECT_EQ(j["a"].asNumber(), 1.0);
    EXPECT_EQ(j["b"].asString(), "two");
    EXPECT_FALSE(j["c"].asBool());
}

// Verifies nested arrays parse into the correct shape.
TEST(StructuralParsing, parses_nested_arrays) {
    Json j = Json::parse("[[1, 2], [3, 4]]");
    ASSERT_EQ(j.size(), 2);
    ASSERT_EQ(j[0].size(), 2);
    EXPECT_EQ(j[0][1].asNumber(), 2.0);
    EXPECT_EQ(j[1][0].asNumber(), 3.0);
}

// Verifies objects nested inside arrays, and arrays nested inside objects, parse correctly.
TEST(StructuralParsing, parses_mixed_nesting) {
    Json j = Json::parse(R"({"list": [1, {"x": 2}]})");
    ASSERT_EQ(j["list"].size(), 2);
    EXPECT_EQ(j["list"][0].asNumber(), 1.0);
    EXPECT_EQ(j["list"][1]["x"].asNumber(), 2.0);
}

// Verifies arbitrary whitespace around structural tokens is tolerated.
TEST(StructuralParsing, tolerates_whitespace_around_tokens) {
    Json j = Json::parse(" { \n \"k\" \t : \n [ 1 , 2 ] \n } ");
    ASSERT_EQ(j["k"].size(), 2);
    EXPECT_EQ(j["k"][1].asNumber(), 2.0);
}

// Verifies a trailing comma in an array is rejected.
TEST(StructuralParsing, trailing_comma_in_array_rejected) {
    EXPECT_THROW(Json::parse("[1, 2, ]"), std::runtime_error);
}

// Verifies a trailing comma in an object is rejected.
TEST(StructuralParsing, trailing_comma_in_object_rejected) {
    EXPECT_THROW(Json::parse(R"({"a": 1, })"), std::runtime_error);
}

// Verifies a missing comma between array elements is rejected.
TEST(StructuralParsing, missing_comma_in_array_rejected) {
    EXPECT_THROW(Json::parse("[1 2]"), std::runtime_error);
}

// Verifies a missing comma between object members is rejected.
TEST(StructuralParsing, missing_comma_in_object_rejected) {
    EXPECT_THROW(Json::parse(R"({"a": 1 "b": 2})"), std::runtime_error);
}

// Verifies a missing colon after an object key is rejected.
TEST(StructuralParsing, missing_colon_in_object_rejected) {
    EXPECT_THROW(Json::parse(R"({"a" 1})"), std::runtime_error);
}

// Verifies a non-string object key is rejected.
TEST(StructuralParsing, non_string_key_rejected) {
    EXPECT_THROW(Json::parse("{1: 2}"), std::runtime_error);
}

// Verifies that when a key is repeated, the last value wins.
TEST(StructuralParsing, duplicate_keys_last_value_wins) {
    Json j = Json::parse(R"({"k": 1, "k": 2})");
    EXPECT_EQ(j.size(), 1);
    EXPECT_EQ(j["k"].asNumber(), 2.0);
}

// Verifies an unterminated array (missing ']') is rejected.
TEST(StructuralParsing, unterminated_array_rejected) {
    EXPECT_THROW(Json::parse("[1, 2"), std::runtime_error);
}

// Verifies an unterminated object (missing '}') is rejected.
TEST(StructuralParsing, unterminated_object_rejected) {
    EXPECT_THROW(Json::parse(R"({"a": 1)"), std::runtime_error);
}

// Verifies nesting exactly at the maximum depth is accepted.
TEST(StructuralParsing, nesting_at_max_depth_accepted) {
    constexpr int kDepth = 512; // matches Parser::kMaxDepth

    std::string json(static_cast<std::size_t>(kDepth), '[');
    json.append(static_cast<std::size_t>(kDepth), ']');

    Json j = Json::parse(json);
    EXPECT_TRUE(j.isArray());
}

// Verifies nesting one level beyond the maximum depth is rejected.
TEST(StructuralParsing, nesting_beyond_max_depth_rejected) {
    constexpr int kDepth = 513; // one past Parser::kMaxDepth

    std::string json(static_cast<std::size_t>(kDepth), '[');
    json.append(static_cast<std::size_t>(kDepth), ']');

    EXPECT_THROW(Json::parse(json), std::runtime_error);
}
