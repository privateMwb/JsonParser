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

#include <JsonPro/Json.h>

#include <gtest/gtest.h>

using namespace JsonPro;

// Verifies a plain string with no escapes parses unchanged.
TEST(StringsParsing, parses_plain_string) {
    EXPECT_EQ(Json::parse("\"hello world\"").asString(), "hello world");
}

// Verifies an empty string parses correctly.
TEST(StringsParsing, parses_empty_string) {
    EXPECT_EQ(Json::parse("\"\"").asString(), "");
}

// Verifies each standard single-character escape sequence decodes correctly.
TEST(StringsParsing, parses_standard_escapes) {
    EXPECT_EQ(Json::parse(R"("\"")").asString(), "\"");
    EXPECT_EQ(Json::parse(R"("\\")").asString(), "\\");
    EXPECT_EQ(Json::parse(R"("\/")").asString(), "/");
    EXPECT_EQ(Json::parse(R"("\n")").asString(), "\n");
    EXPECT_EQ(Json::parse(R"("\t")").asString(), "\t");
    EXPECT_EQ(Json::parse(R"("\r")").asString(), "\r");
    EXPECT_EQ(Json::parse(R"("\b")").asString(), "\b");
    EXPECT_EQ(Json::parse(R"("\f")").asString(), "\f");
}

// Verifies literal text and escapes interleave correctly within one string.
TEST(StringsParsing, parses_mixed_literal_and_escapes) {
    EXPECT_EQ(Json::parse(R"("line1\nline2\ttabbed")").asString(), "line1\nline2\ttabbed");
}

// Verifies a \u escape for a BMP codepoint decodes to the correct UTF-8 bytes.
TEST(StringsParsing, parses_unicode_bmp_escape) {
    // \u00e9 = 'é' (U+00E9), UTF-8: 0xC3 0xA9
    Json j = Json::parse(R"("\u00e9")");
    const std::string& s = j.asString();

    ASSERT_EQ(s.size(), 2);
    EXPECT_EQ(static_cast<unsigned char>(s[0]), 0xC3);
    EXPECT_EQ(static_cast<unsigned char>(s[1]), 0xA9);
}

// Verifies a valid surrogate pair decodes to a single 4-byte UTF-8 codepoint.
TEST(StringsParsing, parses_surrogate_pair) {
    // U+1F600 (grinning face) = surrogate pair \uD83D\uDE00, UTF-8: F0 9F 98 80
    Json j = Json::parse(R"("\uD83D\uDE00")");
    const std::string& s = j.asString();

    ASSERT_EQ(s.size(), 4);
    EXPECT_EQ(static_cast<unsigned char>(s[0]), 0xF0);
    EXPECT_EQ(static_cast<unsigned char>(s[1]), 0x9F);
    EXPECT_EQ(static_cast<unsigned char>(s[2]), 0x98);
    EXPECT_EQ(static_cast<unsigned char>(s[3]), 0x80);
}

// Verifies a high surrogate not followed by \u<low> is rejected.
TEST(StringsParsing, high_surrogate_without_low_rejected) {
    EXPECT_THROW(Json::parse(R"("\uD83D")"), std::runtime_error);
    EXPECT_THROW(Json::parse(R"("\uD83Dxxxx")"), std::runtime_error);
}

// Verifies a high surrogate followed by an invalid low surrogate is rejected.
TEST(StringsParsing, invalid_low_surrogate_rejected) {
    EXPECT_THROW(Json::parse(R"("\uD83D\u0041")"), std::runtime_error);
}

// Verifies an unpaired low surrogate (with no preceding high surrogate) is rejected.
TEST(StringsParsing, unpaired_low_surrogate_rejected) {
    EXPECT_THROW(Json::parse(R"("\uDE00")"), std::runtime_error);
}

// Verifies an unrecognized escape character is rejected.
TEST(StringsParsing, invalid_escape_character_rejected) {
    EXPECT_THROW(Json::parse(R"("\q")"), std::runtime_error);
}

// Verifies a \u escape with non-hex digits is rejected.
TEST(StringsParsing, invalid_hex_digit_rejected) {
    EXPECT_THROW(Json::parse(R"("\u12zz")"), std::runtime_error);
}

// Verifies a raw, unescaped control character inside a string is rejected.
TEST(StringsParsing, unescaped_control_character_rejected) {
    std::string input = "\"a\tb\""; // literal tab byte inside quotes
    EXPECT_THROW(Json::parse(input), std::runtime_error);
}

// Verifies an unterminated string (missing closing quote) is rejected.
TEST(StringsParsing, unterminated_string_rejected) {
    EXPECT_THROW(Json::parse("\"unterminated"), std::runtime_error);
}

// Verifies an unterminated escape sequence at end of input is rejected.
TEST(StringsParsing, unterminated_escape_rejected) {
    EXPECT_THROW(Json::parse("\"abc\\"), std::runtime_error);
}
