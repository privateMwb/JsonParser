// JSON string parsing test suite.
//
// Coverage:
// - Plain strings with no escapes
// - All standard escape sequences (\" \\ \/ \n \t \r \b \f)
// - \u escapes for BMP codepoints
// - \u surrogate pairs decoding to a single UTF-8 codepoint (astral plane)
// - Invalid / unpaired surrogates rejected
// - Invalid escape character rejected
// - Unescaped control characters rejected
// - Unterminated strings rejected
// - Mixed runs of literal text and escapes decode correctly

#include <common/framework.h>

using namespace JsonPro;

// Verifies a plain string with no escapes parses unchanged.
static void parses_plain_string() {
    CHK(Json::parse("\"hello world\"").asString() == "hello world");
}

// Verifies an empty string parses correctly.
static void parses_empty_string() {
    CHK(Json::parse("\"\"").asString() == "");
}

// Verifies each standard single-character escape sequence decodes correctly.
static void parses_standard_escapes() {
    CHK(Json::parse(R"("\"")").asString() == "\"");
    CHK(Json::parse(R"("\\")").asString() == "\\");
    CHK(Json::parse(R"("\/")").asString() == "/");
    CHK(Json::parse(R"("\n")").asString() == "\n");
    CHK(Json::parse(R"("\t")").asString() == "\t");
    CHK(Json::parse(R"("\r")").asString() == "\r");
    CHK(Json::parse(R"("\b")").asString() == "\b");
    CHK(Json::parse(R"("\f")").asString() == "\f");
}

// Verifies literal text and escapes interleave correctly within one string.
static void parses_mixed_literal_and_escapes() {
    CHK(Json::parse(R"("line1\nline2\ttabbed")").asString() == "line1\nline2\ttabbed");
}

// Verifies a \u escape for a BMP codepoint decodes to the correct UTF-8 bytes.
static void parses_unicode_bmp_escape() {
    // \u00e9 = 'é' (U+00E9), UTF-8: 0xC3 0xA9
    Json j = Json::parse(R"("\u00e9")");
    const std::string& s = j.asString();

    CHK(s.size() == 2);
    CHK(static_cast<unsigned char>(s[0]) == 0xC3);
    CHK(static_cast<unsigned char>(s[1]) == 0xA9);
}

// Verifies a valid surrogate pair decodes to a single 4-byte UTF-8 codepoint.
static void parses_surrogate_pair() {
    // U+1F600 (grinning face) = surrogate pair \uD83D\uDE00, UTF-8: F0 9F 98 80
    Json j = Json::parse(R"("\uD83D\uDE00")");
    const std::string& s = j.asString();

    CHK(s.size() == 4);
    CHK(static_cast<unsigned char>(s[0]) == 0xF0);
    CHK(static_cast<unsigned char>(s[1]) == 0x9F);
    CHK(static_cast<unsigned char>(s[2]) == 0x98);
    CHK(static_cast<unsigned char>(s[3]) == 0x80);
}

// Verifies a high surrogate not followed by \u<low> is rejected.
static void high_surrogate_without_low_rejected() {
    CHK_THROWS(Json::parse(R"("\uD83D")"), std::runtime_error);
    CHK_THROWS(Json::parse(R"("\uD83Dxxxx")"), std::runtime_error);
}

// Verifies a high surrogate followed by an invalid low surrogate is rejected.
static void invalid_low_surrogate_rejected() {
    CHK_THROWS(Json::parse(R"("\uD83D\u0041")"), std::runtime_error);
}

// Verifies an unpaired low surrogate (with no preceding high surrogate) is rejected.
static void unpaired_low_surrogate_rejected() {
    CHK_THROWS(Json::parse(R"("\uDE00")"), std::runtime_error);
}

// Verifies an unrecognized escape character is rejected.
static void invalid_escape_character_rejected() {
    CHK_THROWS(Json::parse(R"("\q")"), std::runtime_error);
}

// Verifies a \u escape with non-hex digits is rejected.
static void invalid_hex_digit_rejected() {
    CHK_THROWS(Json::parse(R"("\u12zz")"), std::runtime_error);
}

// Verifies a raw, unescaped control character inside a string is rejected.
static void unescaped_control_character_rejected() {
    std::string input = "\"a\tb\""; // literal tab byte inside quotes
    CHK_THROWS(Json::parse(input), std::runtime_error);
}

// Verifies an unterminated string (missing closing quote) is rejected.
static void unterminated_string_rejected() {
    CHK_THROWS(Json::parse("\"unterminated"), std::runtime_error);
}

// Verifies an unterminated escape sequence at end of input is rejected.
static void unterminated_escape_rejected() {
    CHK_THROWS(Json::parse("\"abc\\"), std::runtime_error);
}

// Executes all string parsing test cases.
static void run_tests() {
    RUN(parses_plain_string);
    RUN(parses_empty_string);
    RUN(parses_standard_escapes);
    RUN(parses_mixed_literal_and_escapes);
    RUN(parses_unicode_bmp_escape);
    RUN(parses_surrogate_pair);
    RUN(high_surrogate_without_low_rejected);
    RUN(invalid_low_surrogate_rejected);
    RUN(unpaired_low_surrogate_rejected);
    RUN(invalid_escape_character_rejected);
    RUN(invalid_hex_digit_rejected);
    RUN(unescaped_control_character_rejected);
    RUN(unterminated_string_rejected);
    RUN(unterminated_escape_rejected);
}

REGISTER_TEST_SUITE();
