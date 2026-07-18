// Leading zero regression test suite.
//
// Guards against a class of number-grammar pitfalls in Parser::parseNumber():
// a bare '0' is only rejected when immediately followed by ANOTHER DIGIT --
// it is perfectly valid before a decimal point or exponent marker. These
// cases sit right at that boundary.
//
// Coverage:
// - "0" and "-0" alone are valid
// - "00", "000", "-00" are rejected
// - "01", "010", "0123" are rejected
// - A single leading zero followed by '.' or 'e'/'E' is valid ("0.5", "0e5")
// - A non-zero leading digit is never affected by the leading-zero rule

#include <common/framework.h>

using namespace JsonPro;

// Verifies a bare "0" and "-0" parse successfully.
static void bare_zero_and_negative_zero_accepted() {
    CHK(Json::parse("0").asNumber() == 0.0);
    CHK(Json::parse("-0").asNumber() == 0.0);
}

// Verifies "00" (zero directly followed by another digit) is rejected.
static void double_zero_rejected() {
    CHK_THROWS(Json::parse("00"), std::runtime_error);
    CHK_THROWS(Json::parse("-00"), std::runtime_error);
}

// Verifies three or more leading zeros are rejected.
static void triple_zero_rejected() {
    CHK_THROWS(Json::parse("000"), std::runtime_error);
}

// Verifies a leading zero followed by non-zero digits is rejected.
static void zero_then_digits_rejected() {
    CHK_THROWS(Json::parse("01"), std::runtime_error);
    CHK_THROWS(Json::parse("010"), std::runtime_error);
    CHK_THROWS(Json::parse("0123"), std::runtime_error);
}

// Verifies a single leading zero immediately followed by a decimal point is valid.
static void zero_before_decimal_point_accepted() {
    CHK(Json::parse("0.5").asNumber() == 0.5);
    CHK(Json::parse("0.0").asNumber() == 0.0);
    CHK(Json::parse("-0.25").asNumber() == -0.25);
}

// Verifies a single leading zero immediately followed by an exponent marker is valid.
static void zero_before_exponent_accepted() {
    CHK(Json::parse("0e5").asNumber() == 0.0);
    CHK(Json::parse("0E5").asNumber() == 0.0);
    CHK(Json::parse("0e-5").asNumber() == 0.0);
}

// Verifies a non-zero leading digit is never mistaken for a leading-zero violation.
static void non_zero_leading_digit_unaffected() {
    CHK(Json::parse("10").asNumber() == 10.0);
    CHK(Json::parse("100").asNumber() == 100.0);
    CHK(Json::parse("-10").asNumber() == -10.0);
}

// Executes all leading zero regression test cases.
static void run_tests() {
    RUN(bare_zero_and_negative_zero_accepted);
    RUN(double_zero_rejected);
    RUN(triple_zero_rejected);
    RUN(zero_then_digits_rejected);
    RUN(zero_before_decimal_point_accepted);
    RUN(zero_before_exponent_accepted);
    RUN(non_zero_leading_digit_unaffected);
}

REGISTER_TEST_SUITE();
