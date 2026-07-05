#pragma once

#include <array>
#include <cstddef>
#include <string>
#include <string_view>

#include "Json.h"

namespace JsonPro {

class Parser {
private:
    // State
    // input_ owns the buffer; cur_/begin_/end_ are raw pointers into it.
    // Line/column are NOT tracked during the hot loop — they are computed
    // lazily by rescanning [begin_, cur_) only when error() is called.
    std::string input_;
    const char* cur_;
    const char* begin_;
    const char* end_;
    std::size_t depth_ = 0;

    // Caps nested array/object recursion to avoid stack overflow on
    // malicious or malformed deeply-nested input.
    static constexpr std::size_t kMaxDepth = 512;

    // Character Utilities
    [[nodiscard]] unsigned char peek() const noexcept;
    [[nodiscard]] unsigned char get() noexcept;

    void skipWhitespace() noexcept;
    [[noreturn]] void error(std::string_view msg) const;

    // Validation
    void ensureEndOfInput();

    // Hex and Unicode Helpers
    [[nodiscard]] static int hexDigitToInt(unsigned char c) noexcept;
    [[nodiscard]] unsigned int readHex4();
    void appendUtf8(std::string& result, unsigned int codepoint);

    // Internal Parsers
    [[nodiscard]] Json parseValue();
    [[nodiscard]] Json parseNull();
    [[nodiscard]] Json parseBool();
    [[nodiscard]] Json parseNumber();
    [[nodiscard]] Json parseString();
    [[nodiscard]] std::string parseRawString();
    [[nodiscard]] Json parseArray();
    [[nodiscard]] Json parseObject();

public:
    // Constructor
    explicit Parser(std::string input);

    // Entry Point
 [[nodiscard]] Json parse();
};

} // namespace JsonPro