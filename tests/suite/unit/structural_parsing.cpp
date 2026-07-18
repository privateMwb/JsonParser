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

#include <common/framework.h>

using namespace JsonPro;

// Verifies an empty array parses to a zero-size Array.
static void parses_empty_array() {
    Json j = Json::parse("[]");
    CHK(j.isArray() == true);
    CHK(j.size() == 0);
}

// Verifies an empty object parses to a zero-size Object.
static void parses_empty_object() {
    Json j = Json::parse("{}");
    CHK(j.isObject() == true);
    CHK(j.size() == 0);
}

// Verifies a populated array with mixed element types parses correctly.
static void parses_populated_array() {
    Json j = Json::parse(R"([1, "two", true, null])");
    CHK(j.size() == 4);
    CHK(j[0].asNumber() == 1.0);
    CHK(j[1].asString() == "two");
    CHK(j[2].asBool() == true);
    CHK(j[3].isNull() == true);
}

// Verifies a populated object with multiple keys parses correctly.
static void parses_populated_object() {
    Json j = Json::parse(R"({"a": 1, "b": "two", "c": false})");
    CHK(j.size() == 3);
    CHK(j["a"].asNumber() == 1.0);
    CHK(j["b"].asString() == "two");
    CHK(j["c"].asBool() == false);
}

// Verifies nested arrays parse into the correct shape.
static void parses_nested_arrays() {
    Json j = Json::parse("[[1, 2], [3, 4]]");
    CHK(j.size() == 2);
    CHK(j[0].size() == 2);
    CHK(j[0][1].asNumber() == 2.0);
    CHK(j[1][0].asNumber() == 3.0);
}

// Verifies objects nested inside arrays, and arrays nested inside objects, parse correctly.
static void parses_mixed_nesting() {
    Json j = Json::parse(R"({"list": [1, {"x": 2}]})");
    CHK(j["list"].size() == 2);
    CHK(j["list"][0].asNumber() == 1.0);
    CHK(j["list"][1]["x"].asNumber() == 2.0);
}

// Verifies arbitrary whitespace around structural tokens is tolerated.
static void tolerates_whitespace_around_tokens() {
    Json j = Json::parse(" { \n \"k\" \t : \n [ 1 , 2 ] \n } ");
    CHK(j["k"].size() == 2);
    CHK(j["k"][1].asNumber() == 2.0);
}

// Verifies a trailing comma in an array is rejected.
static void trailing_comma_in_array_rejected() {
    CHK_THROWS(Json::parse("[1, 2, ]"), std::runtime_error);
}

// Verifies a trailing comma in an object is rejected.
static void trailing_comma_in_object_rejected() {
    CHK_THROWS(Json::parse(R"({"a": 1, })"), std::runtime_error);
}

// Verifies a missing comma between array elements is rejected.
static void missing_comma_in_array_rejected() {
    CHK_THROWS(Json::parse("[1 2]"), std::runtime_error);
}

// Verifies a missing comma between object members is rejected.
static void missing_comma_in_object_rejected() {
    CHK_THROWS(Json::parse(R"({"a": 1 "b": 2})"), std::runtime_error);
}

// Verifies a missing colon after an object key is rejected.
static void missing_colon_in_object_rejected() {
    CHK_THROWS(Json::parse(R"({"a" 1})"), std::runtime_error);
}

// Verifies a non-string object key is rejected.
static void non_string_key_rejected() {
    CHK_THROWS(Json::parse("{1: 2}"), std::runtime_error);
}

// Verifies that when a key is repeated, the last value wins.
static void duplicate_keys_last_value_wins() {
    Json j = Json::parse(R"({"k": 1, "k": 2})");
    CHK(j.size() == 1);
    CHK(j["k"].asNumber() == 2.0);
}

// Verifies an unterminated array (missing ']') is rejected.
static void unterminated_array_rejected() {
    CHK_THROWS(Json::parse("[1, 2"), std::runtime_error);
}

// Verifies an unterminated object (missing '}') is rejected.
static void unterminated_object_rejected() {
    CHK_THROWS(Json::parse(R"({"a": 1)"), std::runtime_error);
}

// Verifies nesting exactly at the maximum depth is accepted.
static void nesting_at_max_depth_accepted() {
    constexpr int kDepth = 512; // matches Parser::kMaxDepth

    std::string json(static_cast<std::size_t>(kDepth), '[');
    json.append(static_cast<std::size_t>(kDepth), ']');

    Json j = Json::parse(json);
    CHK(j.isArray() == true);
}

// Verifies nesting one level beyond the maximum depth is rejected.
static void nesting_beyond_max_depth_rejected() {
    constexpr int kDepth = 513; // one past Parser::kMaxDepth

    std::string json(static_cast<std::size_t>(kDepth), '[');
    json.append(static_cast<std::size_t>(kDepth), ']');

    CHK_THROWS(Json::parse(json), std::runtime_error);
}

// Executes all structural parsing test cases.
static void run_tests() {
    RUN(parses_empty_array);
    RUN(parses_empty_object);
    RUN(parses_populated_array);
    RUN(parses_populated_object);
    RUN(parses_nested_arrays);
    RUN(parses_mixed_nesting);
    RUN(tolerates_whitespace_around_tokens);
    RUN(trailing_comma_in_array_rejected);
    RUN(trailing_comma_in_object_rejected);
    RUN(missing_comma_in_array_rejected);
    RUN(missing_comma_in_object_rejected);
    RUN(missing_colon_in_object_rejected);
    RUN(non_string_key_rejected);
    RUN(duplicate_keys_last_value_wins);
    RUN(unterminated_array_rejected);
    RUN(unterminated_object_rejected);
    RUN(nesting_at_max_depth_accepted);
    RUN(nesting_beyond_max_depth_rejected);
}

REGISTER_TEST_SUITE();
