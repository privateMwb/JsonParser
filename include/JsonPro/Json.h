/**
 * @file            Json.h
 *
 * @date            2026-17-7
 *
 * @version         1.0.0
 *
 * @copyright       Copyright (c) 2026 Your Name
 *                  All rights reserved.
 *                  https://github.com/privateMwb/JsonParser
 *
 * @attention       This source is released under the MIT license
 *                  SPDX-License-Identifier: MIT
 *                  <http://opensource.org/licenses/MIT>
 */

#pragma once

// clang-format off
#include <cstddef>     // std::size_t
#include <iosfwd>      // std::ostream (forward declaration only)
#include <string>      // std::string
#include <string_view> // std::string_view
#include <variant>     // std::variant
#include <vector>      // std::vector

#include <JsonPro/JsonObject.h>
// clang-format on

namespace JsonPro {

/**
 * @brief A dynamically-typed JSON value: null, bool, number, string, array,
 * or object, per RFC 8259.
 * @details Backed by a `std::variant`; `type()` is derived directly from
 * the variant's active index, so there is no separate discriminant to keep
 * in sync. Numbers are always stored as `double` (JSON does not distinguish
 * integer and floating-point types), so integers outside the ~53-bit exact
 * range of `double` will lose precision on round-trip.
 */
class Json {
  public:
    /// @brief The kind of value a `Json` currently holds.
    enum class Type { Null, Bool, Number, String, Array, Object };

    using ArrayType = std::vector<Json>;
    using ObjectType = JsonObject;

  private:
    // Type is derived from value_.index() — no separate discriminant is
    // stored or needs to be kept in sync with the active alternative.
    std::variant<std::nullptr_t, bool, double, std::string, ArrayType, ObjectType> value_;

  public:
    // -------------------------------------------------------------------
    // Lifecycle
    // -------------------------------------------------------------------

    /// @brief Constructs a null value.
    Json() noexcept;
    /// @brief Constructs a null value.
    Json(std::nullptr_t) noexcept;

    /// @brief Constructs a bool value.
    Json(bool value) noexcept;

    /// @brief Constructs a number value.
    Json(double value) noexcept;
    /// @brief Constructs a number value from an int (widened to double).
    Json(int value) noexcept;

    /// @brief Constructs a string value, copying `value`.
    Json(const std::string& value);
    /// @brief Constructs a string value, taking ownership of `value`.
    Json(std::string&& value) noexcept;
    /// @brief Constructs a string value from a NUL-terminated C string.
    Json(const char* value);

    /// @brief Constructs an array value, copying `value`.
    Json(const ArrayType& value);
    /// @brief Constructs an array value, taking ownership of `value`.
    Json(ArrayType&& value) noexcept;

    /// @brief Constructs an object value, copying `value`.
    Json(const ObjectType& value);
    /// @brief Constructs an object value, taking ownership of `value`.
    Json(ObjectType&& value) noexcept;

    Json(const Json& other) = default;
    Json& operator=(const Json& other) = default;

    /// @brief Moves from `other`, leaving it null.
    Json(Json&& other) noexcept;
    /// @brief Moves from `other`, leaving it null.
    Json& operator=(Json&& other) noexcept;

    // -------------------------------------------------------------------
    // Parsing
    // -------------------------------------------------------------------

    /**
     * @brief Parses `text` as a complete JSON document.
     * @param text JSON source. Copied internally, since the parser needs a
     * stable, owned buffer to hold onto for the duration of the parse.
     * @return The parsed value.
     * @throws JsonParseError if `text` is not valid JSON per RFC 8259.
     * @details If you already own a `std::string` and don't need it
     * afterward, prefer `parseOwned()` to avoid this copy.
     */
    [[nodiscard]] static Json parse(std::string_view text);

    /**
     * @brief Parses `text` as a complete JSON document, taking ownership of
     * the buffer instead of copying it.
     * @param text JSON source, moved from.
     * @return The parsed value.
     * @throws JsonParseError if `text` is not valid JSON per RFC 8259.
     */
    [[nodiscard]] static Json parseOwned(std::string text);

    // -------------------------------------------------------------------
    // Type inspection
    // -------------------------------------------------------------------

    /// @brief Returns which alternative this value currently holds.
    [[nodiscard]] Type type() const noexcept;
    [[nodiscard]] bool isNull() const noexcept;
    [[nodiscard]] bool isBool() const noexcept;
    [[nodiscard]] bool isNumber() const noexcept;
    [[nodiscard]] bool isString() const noexcept;
    [[nodiscard]] bool isArray() const noexcept;
    [[nodiscard]] bool isObject() const noexcept;

    // -------------------------------------------------------------------
    // Value access
    // -------------------------------------------------------------------

    /**
     * @brief Returns the held bool.
     * @throws JsonTypeError if this value is not a bool.
     */
    [[nodiscard]] bool asBool() const;

    /**
     * @brief Returns the held number.
     * @throws JsonTypeError if this value is not a number.
     */
    [[nodiscard]] double asNumber() const;

    /**
     * @brief Returns the held string.
     * @throws JsonTypeError if this value is not a string.
     */
    [[nodiscard]] const std::string& asString() const;

    /**
     * @brief Returns the held array.
     * @throws JsonTypeError if this value is not an array.
     */
    [[nodiscard]] ArrayType& asArray();
    /// @brief Const-qualified overload. See the non-const asArray().
    [[nodiscard]] const ArrayType& asArray() const;

    /**
     * @brief Returns the held object.
     * @throws JsonTypeError if this value is not an object.
     */
    [[nodiscard]] ObjectType& asObject();
    /// @brief Const-qualified overload. See the non-const asObject().
    [[nodiscard]] const ObjectType& asObject() const;

    // -------------------------------------------------------------------
    // Navigation
    // -------------------------------------------------------------------

    /**
     * @brief Returns a reference to the member `key`, inserting a null
     * value if it doesn't already exist (as `std::map::operator[]` does).
     * @param key Key to look up or insert.
     * @throws JsonTypeError if this value is not an object.
     */
    [[nodiscard]] Json& operator[](std::string_view key);

    /**
     * @brief Returns a const reference to the member `key`.
     * @param key Key to look up.
     * @throws JsonTypeError if this value is not an object.
     * @throws JsonOutOfRange if `key` does not exist (a const value can't
     * auto-vivify a missing key the way the non-const overload does).
     */
    [[nodiscard]] const Json& operator[](std::string_view key) const;

    /**
     * @brief Returns a reference to the element at `index`.
     * @param index Array index. Not bounds-checked — see `at()` for a
     * checked equivalent.
     * @throws JsonTypeError if this value is not an array.
     */
    [[nodiscard]] Json& operator[](std::size_t index);
    /// @brief Const-qualified overload. See the non-const operator[](std::size_t).
    [[nodiscard]] const Json& operator[](std::size_t index) const;

    // -------------------------------------------------------------------
    // Element access
    // -------------------------------------------------------------------

    /**
     * @brief Returns a reference to the element at `index`, bounds-checked.
     * @throws JsonTypeError if this value is not an array.
     * @throws JsonOutOfRange if `index >= size()`.
     */
    [[nodiscard]] Json& at(std::size_t index);
    /// @brief Const-qualified overload. See the non-const at(std::size_t).
    [[nodiscard]] const Json& at(std::size_t index) const;

    /**
     * @brief Returns a reference to the member `key`, bounds-checked.
     * @throws JsonTypeError if this value is not an object.
     * @throws JsonOutOfRange if `key` does not exist.
     */
    [[nodiscard]] Json& at(std::string_view key);
    /// @brief Const-qualified overload. See the non-const at(std::string_view).
    [[nodiscard]] const Json& at(std::string_view key) const;

    // -------------------------------------------------------------------
    // Utilities
    // -------------------------------------------------------------------

    /// @brief Element count for an array or object; 0 for any other type.
    [[nodiscard]] std::size_t size() const noexcept;

    /// @brief Whether this is an object containing `key`. `false` (not a
    /// throw) if this value isn't an object at all.
    [[nodiscard]] bool contains(std::string_view key) const noexcept;

    // -------------------------------------------------------------------
    // Comparison
    // -------------------------------------------------------------------

    /// @brief Structural equality: same type and same value. Objects compare
    /// order-independently; arrays compare order-sensitively.
    [[nodiscard]] bool operator==(const Json& other) const;
    [[nodiscard]] bool operator!=(const Json& other) const;

    // -------------------------------------------------------------------
    // Serialization
    // -------------------------------------------------------------------

    /**
     * @brief Serializes this value to a JSON string.
     * @param indent Spaces per nesting level. `0` (the default) still
     * pretty-prints with newlines at the current indent width of 0; there
     * is currently no separate "compact, single-line" mode.
     * @return The serialized JSON text.
     * @throws JsonException if this value (or a nested value) contains a
     * non-finite number (NaN or Infinity), which has no JSON representation.
     */
    [[nodiscard]] std::string dump(int indent = 0) const;

    /**
     * @brief Serializes this value directly to `os`, avoiding the
     * intermediate string `dump(int)` builds.
     * @param os Stream to write to.
     * @param indent Spaces per nesting level so far (used internally for
     * recursion; pass 0 at the top level).
     * @throws JsonException if this value (or a nested value) contains a
     * non-finite number (NaN or Infinity), which has no JSON representation.
     */
    void dump(std::ostream& os, int indent = 0) const;
};

} // namespace JsonPro

/// @brief Short alias so this library can be used as `rain::Json`, while
/// its true namespace (and all internal diagnostics) remains `JsonPro`.
/// See JsonObject.h, Parser.h, and JsonException.h for the same alias
/// applied to `rain::JsonObject`, `rain::Parser`, and the exception types.
namespace rain = JsonPro;