/**
 * @file JsonException.h
 * @brief Exception types thrown by JsonPro.
 *
 * Contains the exception hierarchy JsonPro throws on parse failures and
 * invalid type access, all derived from a common JsonException base.
 */

#pragma once

// clang-format off
#include <cstddef>   // std::size_t
#include <stdexcept> // std::runtime_error
#include <string>    // std::string
// clang-format on

namespace JsonPro {

/**
 * @brief Base class for every exception JsonPro throws.
 * @details Catch this to handle any JsonPro failure uniformly, or catch one
 * of the derived types below to distinguish a malformed-input problem
 * (`JsonParseError`) from a caller-side type-usage mistake (`JsonTypeError`,
 * `JsonOutOfRange`).
 */
class JsonException : public std::runtime_error {
  public:
    /// @brief Constructs the exception with a human-readable message.
    explicit JsonException(const std::string& what) : std::runtime_error(what) {}
};

/**
 * @brief Thrown when `Json::parse()` (or `Parser::parse()`) encounters
 * malformed input.
 * @details Carries the 1-based line and column of the offending character
 * in addition to the formatted message, so callers can point a user at the
 * exact location without re-parsing the message text.
 */
class JsonParseError : public JsonException {
    std::size_t line_;
    std::size_t column_;

  public:
    /**
     * @brief Constructs a parse error.
     * @param what Human-readable message, already including position info.
     * @param line 1-based line number of the error.
     * @param column 1-based column number of the error.
     */
    JsonParseError(const std::string& what, std::size_t line, std::size_t column)
        : JsonException(what), line_(line), column_(column) {}

    /// @brief 1-based line number where parsing failed.
    [[nodiscard]] std::size_t line() const noexcept {
        return line_;
    }
    /// @brief 1-based column number where parsing failed.
    [[nodiscard]] std::size_t column() const noexcept {
        return column_;
    }
};

/**
 * @brief Thrown when a `Json` value is accessed as the wrong type, e.g.
 * calling `asBool()` on a value holding a string.
 * @details This signals a caller-side logic mistake rather than a data
 * problem — check `type()` or `isXxx()` first if the value's shape isn't
 * already known, to avoid triggering this.
 */
class JsonTypeError : public JsonException {
  public:
    /// @brief Constructs the exception with a human-readable message.
    explicit JsonTypeError(const std::string& what) : JsonException(what) {}
};

/**
 * @brief Thrown by `Json::at()` (and the const `operator[]` overloads) when
 * an array index or object key does not exist.
 */
class JsonOutOfRange : public JsonException {
  public:
    /// @brief Constructs the exception with a human-readable message.
    explicit JsonOutOfRange(const std::string& what) : JsonException(what) {}
};

} // namespace JsonPro

/// @brief Short alias so this library can be used as `rain::JsonException`
/// (and its derived types), while the true namespace (and all internal
/// diagnostics) remains `JsonPro`. See Json.h, JsonObject.h, and Parser.h
/// for the same alias applied to `rain::Json`, `rain::JsonObject`, and
/// `rain::Parser`.
namespace rain = JsonPro;