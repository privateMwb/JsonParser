// ============================================================
// Parser.cpp
// Implementation for JsonPro::Parser.
// ============================================================
//
// Sections:
//   1. Lookup Tables
//   2. Constructor
//   3. Character Utilities
//   4. Validation
//   5. Hex and Unicode Helpers
//   6. Internal Parsers
//   7. Entry Point
//
// ============================================================

#include <array>
#include <charconv>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>

#include <JsonPro/Parser.h>

namespace JsonPro {

namespace {

    // ============================================================
    // Section 1 — Lookup Tables
    // ============================================================
    // Branchless-lookup classification tables (256 entries, built once at
    // compile time). Replaces locale-dependent <cctype> calls on the hot path.
    constexpr std::array<bool, 256> kIsSpace = [] {
        std::array<bool, 256> t{};
        t[static_cast<unsigned char>(' ')]  = true;
        t[static_cast<unsigned char>('\t')] = true;
        t[static_cast<unsigned char>('\n')] = true;
        t[static_cast<unsigned char>('\r')] = true;
        return t;
    }();

    constexpr std::array<bool, 256> kIsDigit = [] {
        std::array<bool, 256> t{};
        for (unsigned char c = '0'; c <= '9'; ++c)
            t[c] = true;
        return t;
    }();

    constexpr std::array<std::int8_t, 256> kHexValue = [] {
        std::array<std::int8_t, 256> t{};
        for (auto& v : t) v = -1;
        for (unsigned char c = '0'; c <= '9'; ++c) t[c] = static_cast<std::int8_t>(c - '0');
        for (unsigned char c = 'a'; c <= 'f'; ++c) t[c] = static_cast<std::int8_t>(c - 'a' + 10);
        for (unsigned char c = 'A'; c <= 'F'; ++c) t[c] = static_cast<std::int8_t>(c - 'A' + 10);
        return t;
    }();

} // namespace

// ============================================================
// Section 2 — Constructor
// ============================================================
Parser::Parser(std::string input) :
    input_(std::move(input)),
    cur_(input_.data()),
    begin_(input_.data()),
    end_(input_.data() + input_.size()) {}


// ============================================================
// Section 3 — Character Utilities
// ============================================================
unsigned char Parser::peek() const noexcept {
    return cur_ < end_ ? static_cast<unsigned char>(*cur_) : '\0';
}

unsigned char Parser::get() noexcept {
    if (cur_ >= end_) [[unlikely]]
        return '\0';

    return static_cast<unsigned char>(*cur_++);
}

void Parser::skipWhitespace() noexcept {
    while (cur_ < end_ && kIsSpace[static_cast<unsigned char>(*cur_)])
        ++cur_;
}

// error() is the only place that pays for line/col tracking — it rescans
// [begin_, cur_) once, on the cold (throwing) path only.
[[noreturn]] void Parser::error(std::string_view msg) const {
    std::size_t line = 1;
    std::size_t col  = 1;

    for (const char* p = begin_; p < cur_; ++p) {
        if (*p == '\n') { ++line; col = 1; }
        else             ++col;
    }

    throw std::runtime_error(
        "JSON parse error at line " + std::to_string(line) +
        ", col " + std::to_string(col) + ": " + std::string(msg));
}


// ============================================================
// Section 4 — Validation
// ============================================================
void Parser::ensureEndOfInput() {
    skipWhitespace();

    if (peek() != '\0') [[unlikely]]
        error("unexpected trailing characters after JSON value");
}


// ============================================================
// Section 5 — Hex and Unicode Helpers
// ============================================================
int Parser::hexDigitToInt(unsigned char c) noexcept {
    return kHexValue[c];
}

unsigned int Parser::readHex4() {
    unsigned int codepoint = 0;

    for (int i = 0; i < 4; ++i) {
        int digit = hexDigitToInt(get());

        if (digit < 0) [[unlikely]]
            error("invalid \\u escape: expected 4 hexadecimal digits");

        codepoint = (codepoint << 4) | static_cast<unsigned int>(digit);
    }

    return codepoint;
}

void Parser::appendUtf8(std::string& result, unsigned int codepoint) {
    if (codepoint <= 0x7F) {
        result += static_cast<char>(codepoint);
    } else if (codepoint <= 0x7FF) {
        result += static_cast<char>(0xC0 | (codepoint >> 6));
        result += static_cast<char>(0x80 | (codepoint & 0x3F));
    } else if (codepoint <= 0xFFFF) {
        result += static_cast<char>(0xE0 | (codepoint >> 12));
        result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (codepoint & 0x3F));
    } else {
        result += static_cast<char>(0xF0 | (codepoint >> 18));
        result += static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F));
        result += static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (codepoint & 0x3F));
    }
}


// ============================================================
// Section 6 — Internal Parsers
// ============================================================
Json Parser::parseValue() {
    skipWhitespace();

    switch (peek()) {
        case 'n': return parseNull();
        case 't':
        case 'f': return parseBool();
        case '"': return parseString();
        case '[': return parseArray();
        case '{': return parseObject();

        default:
            if (kIsDigit[peek()] || peek() == '-') [[likely]]
                return parseNumber();

            error("unexpected character; expected a JSON value");
    }
}

Json Parser::parseNull() {
    if ((end_ - cur_) < 4 || std::memcmp(cur_, "null", 4) != 0) [[unlikely]]
        error("expected 'null'");

    cur_ += 4;
    return Json(nullptr);
}

Json Parser::parseBool() {
    if ((end_ - cur_) >= 4 && std::memcmp(cur_, "true", 4) == 0) {
        cur_ += 4;
        return Json(true);
    }

    if ((end_ - cur_) >= 5 && std::memcmp(cur_, "false", 5) == 0) {
        cur_ += 5;
        return Json(false);
    }

    error("expected 'true' or 'false'");
}

// Number grammar is validated by hand (single pass, no allocation), then
// std::from_chars converts the already-validated span directly — no substr,
// no locale, no exceptions on the happy path.
Json Parser::parseNumber() {
    const char* start = cur_;

    if (peek() == '-')
        (void)get();

    if (!kIsDigit[peek()]) [[unlikely]]
        error("expected digit");

    if (peek() == '0') {
        (void)get();

        if (kIsDigit[peek()]) [[unlikely]]
            error("leading zeros are not allowed");
    } else {
        while (kIsDigit[peek()])
            (void)get();
    }

    if (peek() == '.') {
        (void)get();

        if (!kIsDigit[peek()]) [[unlikely]]
            error("expected digit after decimal point");

        while (kIsDigit[peek()])
            (void)get();
    }

    if (peek() == 'e' || peek() == 'E') {
        (void)get();

        if (peek() == '+' || peek() == '-')
            (void)get();

        if (!kIsDigit[peek()]) [[unlikely]]
            error("expected digit in exponent");

        while (kIsDigit[peek()])
            (void)get();
    }

    double value = 0.0;
    auto result = std::from_chars(start, cur_, value);

    if (result.ec != std::errc{}) [[unlikely]]
        error("invalid number literal");

    return Json(value);
}

// Scans runs of unescaped bytes and bulk-appends them in one shot; only
// escape sequences and the closing quote break the run. Avoids the
// per-character append() that dominated the old implementation.
std::string Parser::parseRawString() {
    if (get() != '"') [[unlikely]]
        error("expected '\"'");

    std::string result;
    const char* segStart = cur_;

    while (true) {
        if (cur_ >= end_) [[unlikely]]
            error("unterminated string literal");

        unsigned char c = static_cast<unsigned char>(*cur_);

        if (c == '"') [[unlikely]] {
            result.append(segStart, cur_);
            ++cur_;
            break;
        }

        if (c == '\\') [[unlikely]] {
            result.append(segStart, cur_);
            ++cur_; // consume backslash

            if (cur_ >= end_) [[unlikely]]
                error("unterminated escape sequence");

            unsigned char esc = static_cast<unsigned char>(*cur_++);

            switch (esc) {
                case '"':  result += '"';  break;
                case '\\': result += '\\'; break;
                case '/':  result += '/';  break;
                case 'n':  result += '\n'; break;
                case 't':  result += '\t'; break;
                case 'r':  result += '\r'; break;
                case 'b':  result += '\b'; break;
                case 'f':  result += '\f'; break;

                case 'u': {
                    unsigned int codepoint = readHex4();

                    // Surrogate pairs: a high surrogate must be immediately
                    // followed by a \u-escaped low surrogate; combine them
                    // into a single codepoint per UTF-16 surrogate pair rules.
                    if (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
                        if (get() != '\\' || get() != 'u') [[unlikely]]
                            error("expected low surrogate in \\u escape pair");

                        unsigned int low = readHex4();

                        if (low < 0xDC00 || low > 0xDFFF) [[unlikely]]
                            error("invalid low surrogate");

                        codepoint = 0x10000 + ((codepoint - 0xD800) << 10) + (low - 0xDC00);
                    } else if (codepoint >= 0xDC00 && codepoint <= 0xDFFF) [[unlikely]] {
                        error("unexpected low surrogate without preceding high surrogate");
                    }

                    appendUtf8(result, codepoint);
                    break;
                }

                default:
                    error("invalid escape sequence");
            }

            segStart = cur_;
            continue;
        }

        if (c < 0x20) [[unlikely]]
            error("invalid unescaped control character in string");

        ++cur_;
    }

    return result;
}

Json Parser::parseString() {
    return Json(parseRawString());
}

Json Parser::parseArray() {
    if (get() != '[') [[unlikely]]
        error("expected '['");

    if (++depth_ > kMaxDepth) [[unlikely]]
        error("maximum nesting depth exceeded");

    Json::ArrayType arr;
    skipWhitespace();

    if (peek() == ']') {
        (void)get();
        --depth_;
        return Json(std::move(arr));
    }

    while (true) {
        arr.push_back(parseValue());
        skipWhitespace();

        if (peek() == ',') {
            (void)get();
            skipWhitespace();
        } else if (peek() == ']') {
            (void)get();
            break;
        } else {
            error("expected ',' or ']'");
        }
    }

    --depth_;
    return Json(std::move(arr));
}

Json Parser::parseObject() {
    if (get() != '{') [[unlikely]]
        error("expected '{'");

    if (++depth_ > kMaxDepth) [[unlikely]]
        error("maximum nesting depth exceeded");

    Json::ObjectType obj;
    skipWhitespace();

    if (peek() == '}') {
        (void)get();
        --depth_;
        return Json(std::move(obj));
    }

    while (true) {
        skipWhitespace();

        if (peek() != '"') [[unlikely]]
            error("expected string key");

        std::string key = parseRawString();

        skipWhitespace();

        if (get() != ':') [[unlikely]]
            error("expected ':' after object key");

        // insert_or_assign: single lookup + insert/replace, vs.
        // operator[]'s default-construct-then-assign (two ops).
        obj.insert_or_assign(std::move(key), parseValue());

        skipWhitespace();

        if (peek() == ',') {
            
(void)get();
        } else if (peek() == '}') {
            (void)get();
            break;
        } else {
            error("expected ',' or '}'");
        }
    }

    --depth_;
    return Json(std::move(obj));
}


// ============================================================
// Section 7 — Entry Point
// ============================================================
Json Parser::parse() {
    skipWhitespace();
    Json value = parseValue();
    ensureEndOfInput();
    return value;
}

} // namespace JsonPro