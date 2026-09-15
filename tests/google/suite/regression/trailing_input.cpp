// Trailing input regression test suite.
//
// Guards against Parser silently accepting extra content after a
// syntactically complete value -- an entire document must be exactly one
// JSON value, optionally surrounded by whitespace, and nothing more.
//
// Coverage:
// - Two back-to-back scalar values with only whitespace between them
// - Two back-to-back strings/structures with no separator at all
// - A stray closing bracket/brace left over after a complete structure
// - A stray comma after an otherwise-complete value
// - Trailing non-whitespace garbage after a complete object
// - (Control) trailing whitespace alone remains valid

#include <JsonPro/Json.h>

#include <gtest/gtest.h>

using namespace JsonPro;

// Verifies two whitespace-separated scalar values are rejected as trailing input.
TEST(TrailingInput, two_scalars_separated_by_whitespace_rejected) {
    EXPECT_THROW(Json::parse("42 43"), std::runtime_error);
}

// Verifies two adjacent string literals with no separator are rejected.
TEST(TrailingInput, two_strings_with_no_separator_rejected) {
    EXPECT_THROW(Json::parse(R"("a""b")"), std::runtime_error);
}

// Verifies two adjacent structures with no separator are rejected.
TEST(TrailingInput, two_structures_with_no_separator_rejected) {
    EXPECT_THROW(Json::parse("{}[]"), std::runtime_error);
    EXPECT_THROW(Json::parse("[1,2][3]"), std::runtime_error);
}

// Verifies a stray closing bracket left over after a complete array is rejected.
TEST(TrailingInput, stray_closing_bracket_after_complete_array_rejected) {
    EXPECT_THROW(Json::parse("[1,2]]"), std::runtime_error);
}

// Verifies a stray closing brace left over after a complete object is rejected.
TEST(TrailingInput, stray_closing_brace_after_complete_object_rejected) {
    EXPECT_THROW(Json::parse("{}}"), std::runtime_error);
}

// Verifies a stray trailing comma after an otherwise-complete value is rejected.
TEST(TrailingInput, stray_trailing_comma_rejected) {
    EXPECT_THROW(Json::parse("42,"), std::runtime_error);
}

// Verifies non-whitespace garbage after a complete object is rejected.
TEST(TrailingInput, trailing_garbage_after_complete_object_rejected) {
    EXPECT_THROW(Json::parse(R"({"a": 1} garbage)"), std::runtime_error);
}

// Control case: trailing whitespace alone (no extra tokens) remains valid.
TEST(TrailingInput, trailing_whitespace_alone_accepted) {
    Json j = Json::parse("  {\"a\": 1}  \n\t");
    EXPECT_EQ(j["a"].asNumber(), 1.0);
}
