// Surrogate pair regression test suite.
//
// Guards against pitfalls in Parser::parseRawString()'s \u-escape handling,
// specifically the surrogate-pair combination logic and the boundary
// between the BMP and the surrogate range (U+D800-U+DFFF).
//
// Coverage:
// - The exact lower and upper bounds of the combinable astral range
//   (U+10000 and U+10FFFF)
// - A surrogate pair embedded mid-string, surrounded by literal text
// - Two consecutive surrogate pairs in the same string
// - Codepoints immediately adjacent to the surrogate range (U+D7FF, U+E000)
//   decode as standalone BMP characters, not surrogates
// - A high surrogate followed by a second high surrogate is rejected
// - A high surrogate followed by a non-\u escape is rejected

#include <common/framework.h>

using namespace JsonPro;

// Verifies the lowest combinable surrogate pair (U+10000) decodes to the
// correct 4-byte UTF-8 sequence: F0 90 80 80.
static void lowest_astral_codepoint_decodes_correctly() {
    Json j = Json::parse(R"("\uD800\uDC00")");
    const std::string& s = j.asString();

    CHK(s.size() == 4);
    CHK(static_cast<unsigned char>(s[0]) == 0xF0);
    CHK(static_cast<unsigned char>(s[1]) == 0x90);
    CHK(static_cast<unsigned char>(s[2]) == 0x80);
    CHK(static_cast<unsigned char>(s[3]) == 0x80);
}

// Verifies the highest combinable surrogate pair (U+10FFFF) decodes to the
// correct 4-byte UTF-8 sequence: F4 8F BF BF.
static void highest_astral_codepoint_decodes_correctly() {
    Json j = Json::parse(R"("\uDBFF\uDFFF")");
    const std::string& s = j.asString();

    CHK(s.size() == 4);
    CHK(static_cast<unsigned char>(s[0]) == 0xF4);
    CHK(static_cast<unsigned char>(s[1]) == 0x8F);
    CHK(static_cast<unsigned char>(s[2]) == 0xBF);
    CHK(static_cast<unsigned char>(s[3]) == 0xBF);
}

// Verifies a surrogate pair embedded between literal text segments decodes
// correctly and the surrounding text survives intact.
static void surrogate_pair_embedded_in_literal_text() {
    Json j = Json::parse(R"("before \uD83D\uDE00 after")");
    const std::string& s = j.asString();

    CHK(s.substr(0, 7) == "before ");
    CHK(s.substr(s.size() - 6) == " after");
    CHK(s.size() == 7 + 4 + 6); // literal + 4-byte emoji + literal
}

// Verifies two consecutive surrogate pairs in the same string both decode correctly.
static void two_consecutive_surrogate_pairs() {
    // U+1F600 (grinning face) followed by U+1F601 (grinning face with smiling eyes)
    Json j = Json::parse(R"("\uD83D\uDE00\uD83D\uDE01")");
    const std::string& s = j.asString();

    CHK(s.size() == 8);
    CHK(static_cast<unsigned char>(s[0]) == 0xF0);
    CHK(static_cast<unsigned char>(s[4]) == 0xF0);
    CHK(static_cast<unsigned char>(s[7]) == 0x81); // last byte differs between the two
}

// Verifies U+D7FF, the codepoint immediately below the surrogate range,
// decodes as a standalone 3-byte BMP character (not treated as a surrogate).
static void codepoint_just_below_surrogate_range() {
    Json j = Json::parse(R"("\uD7FF")");
    const std::string& s = j.asString();

    CHK(s.size() == 3);
    CHK(static_cast<unsigned char>(s[0]) == 0xED);
    CHK(static_cast<unsigned char>(s[1]) == 0x9F);
    CHK(static_cast<unsigned char>(s[2]) == 0xBF);
}

// Verifies U+E000, the codepoint immediately above the surrogate range,
// decodes as a standalone 3-byte BMP character (not treated as a surrogate).
static void codepoint_just_above_surrogate_range() {
    Json j = Json::parse(R"("\uE000")");
    const std::string& s = j.asString();

    CHK(s.size() == 3);
    CHK(static_cast<unsigned char>(s[0]) == 0xEE);
    CHK(static_cast<unsigned char>(s[1]) == 0x80);
    CHK(static_cast<unsigned char>(s[2]) == 0x80);
}

// Verifies a high surrogate immediately followed by a second high surrogate
// (instead of a low surrogate) is rejected.
static void high_surrogate_followed_by_another_high_surrogate_rejected() {
    CHK_THROWS(Json::parse(R"("\uD800\uD800")"), std::runtime_error);
}

// Verifies a high surrogate followed by an escape that isn't \u at all is rejected.
static void high_surrogate_followed_by_non_unicode_escape_rejected() {
    CHK_THROWS(Json::parse(R"("\uD83D\n")"), std::runtime_error);
}

// Executes all surrogate pair regression test cases.
static void run_tests() {
    RUN(lowest_astral_codepoint_decodes_correctly);
    RUN(highest_astral_codepoint_decodes_correctly);
    RUN(surrogate_pair_embedded_in_literal_text);
    RUN(two_consecutive_surrogate_pairs);
    RUN(codepoint_just_below_surrogate_range);
    RUN(codepoint_just_above_surrogate_range);
    RUN(high_surrogate_followed_by_another_high_surrogate_rejected);
    RUN(high_surrogate_followed_by_non_unicode_escape_rejected);
}

REGISTER_TEST_SUITE();
