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

#include <common/framework.h>

using namespace JsonPro;

// Verifies two whitespace-separated scalar values are rejected as trailing input.
static void two_scalars_separated_by_whitespace_rejected() {
    CHK_THROWS(Json::parse("42 43"), std::runtime_error);
}

// Verifies two adjacent string literals with no separator are rejected.
static void two_strings_with_no_separator_rejected() {
    CHK_THROWS(Json::parse(R"("a""b")"), std::runtime_error);
}

// Verifies two adjacent structures with no separator are rejected.
static void two_structures_with_no_separator_rejected() {
    CHK_THROWS(Json::parse("{}[]"), std::runtime_error);
    CHK_THROWS(Json::parse("[1,2][3]"), std::runtime_error);
}

// Verifies a stray closing bracket left over after a complete array is rejected.
static void stray_closing_bracket_after_complete_array_rejected() {
    CHK_THROWS(Json::parse("[1,2]]"), std::runtime_error);
}

// Verifies a stray closing brace left over after a complete object is rejected.
static void stray_closing_brace_after_complete_object_rejected() {
    CHK_THROWS(Json::parse("{}}"), std::runtime_error);
}

// Verifies a stray trailing comma after an otherwise-complete value is rejected.
static void stray_trailing_comma_rejected() {
    CHK_THROWS(Json::parse("42,"), std::runtime_error);
}

// Verifies non-whitespace garbage after a complete object is rejected.
static void trailing_garbage_after_complete_object_rejected() {
    CHK_THROWS(Json::parse(R"({"a": 1} garbage)"), std::runtime_error);
}

// Control case: trailing whitespace alone (no extra tokens) remains valid.
static void trailing_whitespace_alone_accepted() {
    Json j = Json::parse("  {\"a\": 1}  \n\t");
    CHK(j["a"].asNumber() == 1.0);
}

// Executes all trailing input regression test cases.
static void run_tests() {
    RUN(two_scalars_separated_by_whitespace_rejected);
    RUN(two_strings_with_no_separator_rejected);
    RUN(two_structures_with_no_separator_rejected);
    RUN(stray_closing_bracket_after_complete_array_rejected);
    RUN(stray_closing_brace_after_complete_object_rejected);
    RUN(stray_trailing_comma_rejected);
    RUN(trailing_garbage_after_complete_object_rejected);
    RUN(trailing_whitespace_alone_accepted);
}

REGISTER_TEST_SUITE();
