// JSON scalar parsing test suite (null, bool, number).
//
// Coverage:
// - null / true / false literals
// - Integer, negative, fractional, and exponent number forms
// - Leading-zero rejection per RFC 8259
// - Malformed literals and numbers throw std::runtime_error
// - Leading/trailing whitespace is skipped around the top-level value

#include <common/framework.h>

using namespace JsonPro;

// Verifies "null" parses to a Null value.
static void parses_null() {
    Json j = Json::parse("null");
    CHK(j.isNull() == true);
}

// Verifies "true" and "false" parse to Bool values.
static void parses_bool_literals() {
    CHK(Json::parse("true").asBool()  == true);
    CHK(Json::parse("false").asBool() == false);
}

// Verifies a malformed literal (e.g. "nul", "tru") throws.
static void malformed_literal_throws() {
    CHK_THROWS(Json::parse("nul"),  std::runtime_error);
    CHK_THROWS(Json::parse("tru"),  std::runtime_error);
    CHK_THROWS(Json::parse("flse"), std::runtime_error);
}

// Verifies a plain non-negative integer parses correctly.
static void parses_positive_integer() {
    CHK(Json::parse("42").asNumber() == 42.0);
}

// Verifies a negative integer parses correctly.
static void parses_negative_integer() {
    CHK(Json::parse("-17").asNumber() == -17.0);
}

// Verifies zero parses correctly (single "0" is valid, not a leading zero).
static void parses_zero() {
    CHK(Json::parse("0").asNumber()  == 0.0);
    CHK(Json::parse("-0").asNumber() == 0.0);
}

// Verifies a fractional number parses correctly.
static void parses_fraction() {
    CHK(Json::parse("3.14").asNumber() == 3.14);
}

// Verifies numbers with an exponent (both cases, signed and unsigned) parse correctly.
static void parses_exponent_forms() {
    CHK(Json::parse("1e2").asNumber()   == 100.0);
    CHK(Json::parse("1E2").asNumber()   == 100.0);
    CHK(Json::parse("1e+2").asNumber()  == 100.0);
    CHK(Json::parse("1e-2").asNumber()  == 0.01);
    CHK(Json::parse("1.5e3").asNumber() == 1500.0);
}

// Verifies a leading zero followed by more digits is rejected (e.g. "01").
static void leading_zero_rejected() {
    CHK_THROWS(Json::parse("01"),  std::runtime_error);
    CHK_THROWS(Json::parse("-01"), std::runtime_error);
}

// Verifies a bare '-' with no following digit is rejected.
static void missing_digit_after_minus_rejected() {
    CHK_THROWS(Json::parse("-"), std::runtime_error);
}

// Verifies a trailing decimal point with no following digit is rejected.
static void missing_digit_after_decimal_point_rejected() {
    CHK_THROWS(Json::parse("1."), std::runtime_error);
}

// Verifies an exponent with no digits is rejected.
static void missing_digit_in_exponent_rejected() {
    CHK_THROWS(Json::parse("1e"),  std::runtime_error);
    CHK_THROWS(Json::parse("1e+"), std::runtime_error);
}

// Verifies surrounding whitespace around a top-level scalar is skipped.
static void skips_surrounding_whitespace() {
    CHK(Json::parse("  \t\n 42 \n  ").asNumber() == 42.0);
}

// Verifies unexpected trailing characters after a valid value are rejected.
static void trailing_characters_rejected() {
    CHK_THROWS(Json::parse("42 extra"), std::runtime_error);
    CHK_THROWS(Json::parse("true false"), std::runtime_error);
}

// Verifies an empty input is rejected.
static void empty_input_rejected() {
    CHK_THROWS(Json::parse(""), std::runtime_error);
    CHK_THROWS(Json::parse("   "), std::runtime_error);
}

// Executes all scalar parsing test cases.
static void run_tests() {
    RUN(parses_null);
    RUN(parses_bool_literals);
    RUN(malformed_literal_throws);
    RUN(parses_positive_integer);
    RUN(parses_negative_integer);
    RUN(parses_zero);
    RUN(parses_fraction);
    RUN(parses_exponent_forms);
    RUN(leading_zero_rejected);
    RUN(missing_digit_after_minus_rejected);
    RUN(missing_digit_after_decimal_point_rejected);
    RUN(missing_digit_in_exponent_rejected);
    RUN(skips_surrounding_whitespace);
    RUN(trailing_characters_rejected);
    RUN(empty_input_rejected);
}

REGISTER_TEST_SUITE();
