// JSON scalar parsing test suite (null, bool, number).
//
// Coverage:
// - null / true / false literals
// - Integer, negative, fractional, and exponent number forms
// - Leading-zero rejection per RFC 8259
// - Malformed literals and numbers throw std::runtime_error
// - Leading/trailing whitespace is skipped around the top-level value

#include <JsonPro/Json.h>

#include <gtest/gtest.h>

using namespace JsonPro;

// Verifies "null" parses to a Null value.
TEST(ScalarParsing, parses_null) {
    Json j = Json::parse("null");
    EXPECT_TRUE(j.isNull());
}

// Verifies "true" and "false" parse to Bool values.
TEST(ScalarParsing, parses_bool_literals) {
    EXPECT_TRUE(Json::parse("true").asBool());
    EXPECT_FALSE(Json::parse("false").asBool());
}

// Verifies a malformed literal (e.g. "nul", "tru") throws.
TEST(ScalarParsing, malformed_literal_throws) {
    EXPECT_THROW(Json::parse("nul"), std::runtime_error);
    EXPECT_THROW(Json::parse("tru"), std::runtime_error);
    EXPECT_THROW(Json::parse("flse"), std::runtime_error);
}

// Verifies a plain non-negative integer parses correctly.
TEST(ScalarParsing, parses_positive_integer) {
    EXPECT_EQ(Json::parse("42").asNumber(), 42.0);
}

// Verifies a negative integer parses correctly.
TEST(ScalarParsing, parses_negative_integer) {
    EXPECT_EQ(Json::parse("-17").asNumber(), -17.0);
}

// Verifies zero parses correctly (single "0" is valid, not a leading zero).
TEST(ScalarParsing, parses_zero) {
    EXPECT_EQ(Json::parse("0").asNumber(), 0.0);
    EXPECT_EQ(Json::parse("-0").asNumber(), 0.0);
}

// Verifies a fractional number parses correctly.
TEST(ScalarParsing, parses_fraction) {
    EXPECT_EQ(Json::parse("3.14").asNumber(), 3.14);
}

// Verifies numbers with an exponent (both cases, signed and unsigned) parse correctly.
TEST(ScalarParsing, parses_exponent_forms) {
    EXPECT_EQ(Json::parse("1e2").asNumber(), 100.0);
    EXPECT_EQ(Json::parse("1E2").asNumber(), 100.0);
    EXPECT_EQ(Json::parse("1e+2").asNumber(), 100.0);
    EXPECT_EQ(Json::parse("1e-2").asNumber(), 0.01);
    EXPECT_EQ(Json::parse("1.5e3").asNumber(), 1500.0);
}

// Verifies a leading zero followed by more digits is rejected (e.g. "01").
TEST(ScalarParsing, leading_zero_rejected) {
    EXPECT_THROW(Json::parse("01"), std::runtime_error);
    EXPECT_THROW(Json::parse("-01"), std::runtime_error);
}

// Verifies a bare '-' with no following digit is rejected.
TEST(ScalarParsing, missing_digit_after_minus_rejected) {
    EXPECT_THROW(Json::parse("-"), std::runtime_error);
}

// Verifies a trailing decimal point with no following digit is rejected.
TEST(ScalarParsing, missing_digit_after_decimal_point_rejected) {
    EXPECT_THROW(Json::parse("1."), std::runtime_error);
}

// Verifies an exponent with no digits is rejected.
TEST(ScalarParsing, missing_digit_in_exponent_rejected) {
    EXPECT_THROW(Json::parse("1e"), std::runtime_error);
    EXPECT_THROW(Json::parse("1e+"), std::runtime_error);
}

// Verifies surrounding whitespace around a top-level scalar is skipped.
TEST(ScalarParsing, skips_surrounding_whitespace) {
    EXPECT_EQ(Json::parse("  \t\n 42 \n  ").asNumber(), 42.0);
}

// Verifies unexpected trailing characters after a valid value are rejected.
TEST(ScalarParsing, trailing_characters_rejected) {
    EXPECT_THROW(Json::parse("42 extra"), std::runtime_error);
    EXPECT_THROW(Json::parse("true false"), std::runtime_error);
}

// Verifies an empty input is rejected.
TEST(ScalarParsing, empty_input_rejected) {
    EXPECT_THROW(Json::parse(""), std::runtime_error);
    EXPECT_THROW(Json::parse("   "), std::runtime_error);
}
