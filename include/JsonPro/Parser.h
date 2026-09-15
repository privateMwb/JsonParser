/**
 * @file            Parser.h
 *
 * @date            2026-17-7
 *
 * @version         1.0.0
 *
 * @copyright       Copyright (c) 2026 privateMwb
 *                  All rights reserved.
 *                  https://github.com/privateMwb/JsonParser
 *
 * @attention       This source is released under the MIT license
 *                  SPDX-License-Identifier: MIT
 *                  <http://opensource.org/licenses/MIT>
 */

#pragma once

// clang-format off
#include <array>       // std::array
#include <cstddef>     // std::size_t
#include <string>      // std::string
#include <string_view> // std::string_view

#include "Json.h" // Json
// clang-format on

namespace JsonPro {

/**
 * @brief A single-pass recursive-descent JSON parser, strictly conforming
 * to RFC 8259.
 * @details Owns a copy of the input buffer and walks it with raw pointers
 * (`cur_`/`begin_`/`end_`); line/column position is not tracked during the
 * hot loop, only computed lazily by rescanning the consumed prefix when an
 * error is actually thrown. Not intended to be reused across multiple
 * documents — construct one `Parser` per `parse()` call.
 */
class Parser {
  private:
    // input_ owns the buffer; cur_/begin_/end_ are raw pointers into it.
    // Line/column are NOT tracked during the hot loop — they are computed
    // lazily by rescanning [begin_, cur_) only when error() is called.
    std::string input_;
    const char* cur_;
    const char* begin_;
    const char* end_;
    std::size_t depth_ = 0;

    // Caps nested array/object recursion to avoid stack overflow on
    // malicious or malformed deeply-nested input. parseValue() recurses
    // through parseArray()/parseObject(), so this bounds real call-stack
    // depth, not just a logical counter — tune against the target
    // platform's stack size if this library runs on threads with small
    // stacks (e.g. some mobile/embedded configurations).
    static constexpr std::size_t kMaxDepth = 512;

    [[nodiscard]] unsigned char peek() const noexcept;
    [[nodiscard]] unsigned char get() noexcept;

    void skipWhitespace() noexcept;

    /**
     * @brief Throws a JsonParseError describing the current parse position.
     * @param msg Description of what went wrong.
     * @throws JsonParseError always.
     */
    [[noreturn]] void error(std::string_view msg) const;

    void ensureEndOfInput();

    [[nodiscard]] static int hexDigitToInt(unsigned char c) noexcept;
    [[nodiscard]] unsigned int readHex4();
    void appendUtf8(std::string& result, unsigned int codepoint);

    [[nodiscard]] Json parseValue();
    [[nodiscard]] Json parseNull();
    [[nodiscard]] Json parseBool();
    [[nodiscard]] Json parseNumber();
    [[nodiscard]] Json parseString();
    [[nodiscard]] std::string parseRawString();
    [[nodiscard]] Json parseArray();
    [[nodiscard]] Json parseObject();

  public:
    /**
     * @brief Constructs a parser that owns `input`.
     * @param input JSON source text, moved in.
     */
    explicit Parser(std::string input);

    /**
     * @brief Parses the entire buffer as one JSON document.
     * @return The parsed value.
     * @throws JsonParseError if the buffer is not valid JSON per RFC 8259,
     * including if there is unexpected trailing content after the value.
     */
    [[nodiscard]] Json parse();
};

} // namespace JsonPro