/**
 * @file Json.cpp
 * @brief Json implementation.
 *
 * Contains the implementation of Json member functions and
 * internal implementation details.
 */

// ============================================================
// Implementation for JsonPro::Json.
// ============================================================
//
//  Sections:
//   1. Constructors & Destructor
//   2. Move Semantics
//   3. Parsing
//   4. Type Inspection
//   5. Value Access
//   6. Navigation
//   7. Element Access
//   8. Utilities
//   9. Comparison
//  10. Serialization
//
// ============================================================

// clang-format off
#include <algorithm> // std::fill_n
#include <charconv>  // std::to_chars
#include <cmath>     // std::isfinite
#include <iterator>  // std::ostreambuf_iterator
#include <ostream>   // std::ostream
#include <sstream>   // std::ostringstream
#include <string>    // std::string

#include <JsonPro/Json.h>
#include <JsonPro/JsonException.h>
#include <JsonPro/Parser.h>
// clang-format on

namespace JsonPro {

namespace {

// Writes n spaces of indentation directly to the stream via a
// std::ostreambuf_iterator, instead of materializing a temporary
// std::string(n, ' ') on every recursive call as a pad()-style helper
// would.
void writeIndent(std::ostream& os, std::size_t n) {
    std::fill_n(std::ostreambuf_iterator<char>(os), n, ' ');
}

// Writes value as the shortest decimal representation that round-trips
// exactly back to value, via std::to_chars — locale-independent and,
// unlike operator<<(double) at default stream precision, lossless.
// JSON has no representation for NaN/Infinity, so those are rejected
// rather than silently emitting non-JSON tokens like "nan" or "inf".
void writeNumber(std::ostream& os, double value) {
    if (!std::isfinite(value)) [[unlikely]]
        throw JsonException(
            "Json::dump: cannot serialize a non-finite number (NaN or Infinity) as JSON");

    char buf[32]; // std::to_chars's shortest round-trip double representation fits well within 32
                  // bytes.
    const auto result = std::to_chars(buf, buf + sizeof(buf), value);
    os.write(buf, result.ptr - buf);
}

// Writes s as a JSON string literal (including the surrounding quotes),
// escaping per RFC 8259 §7. Mirrors Parser::parseRawString's approach in
// reverse: scans runs of characters needing no escaping and bulk-writes
// them in one shot, so only actual escapes and control characters pay a
// per-character cost.
void writeEscapedString(std::ostream& os, const std::string& s) {
    static constexpr char kHexDigits[] = "0123456789abcdef";

    os << '"';

    std::size_t segStart = 0;

    for (std::size_t i = 0; i < s.size(); ++i) {
        const unsigned char c = static_cast<unsigned char>(s[i]);
        const char* esc = nullptr;

        switch (c) {
        case '"':
            esc = "\\\"";
            break;
        case '\\':
            esc = "\\\\";
            break;
        case '\b':
            esc = "\\b";
            break;
        case '\f':
            esc = "\\f";
            break;
        case '\n':
            esc = "\\n";
            break;
        case '\r':
            esc = "\\r";
            break;
        case '\t':
            esc = "\\t";
            break;
        default:
            if (c < 0x20) {
                os.write(s.data() + segStart, static_cast<std::streamsize>(i - segStart));
                os << "\\u00" << kHexDigits[(c >> 4) & 0xF] << kHexDigits[c & 0xF];
                segStart = i + 1;
            }
            continue;
        }

        os.write(s.data() + segStart, static_cast<std::streamsize>(i - segStart));
        os << esc;
        segStart = i + 1;
    }

    os.write(s.data() + segStart, static_cast<std::streamsize>(s.size() - segStart));
    os << '"';
}

} // namespace

// ============================================================
//  Section 1 — Constructors & Destructor
// ============================================================

Json::Json() noexcept : value_(nullptr) {}

Json::Json(std::nullptr_t) noexcept : value_(nullptr) {}

Json::Json(bool value) noexcept : value_(value) {}

Json::Json(double value) noexcept : value_(value) {}

Json::Json(int value) noexcept : value_(static_cast<double>(value)) {}

Json::Json(const std::string& value) : value_(value) {}

Json::Json(std::string&& value) noexcept : value_(std::move(value)) {}

Json::Json(const char* value) : value_(std::string(value)) {}

Json::Json(const ArrayType& value) : value_(value) {}

Json::Json(ArrayType&& value) noexcept : value_(std::move(value)) {}

Json::Json(const ObjectType& value) : value_(value) {}

Json::Json(ObjectType&& value) noexcept : value_(std::move(value)) {}

// ============================================================
//  Section 2 — Move Semantics
// ============================================================

Json::Json(Json&& other) noexcept : value_(std::move(other.value_)) {
    other.value_ = nullptr;
}

Json& Json::operator=(Json&& other) noexcept {
    if (this != &other) {
        value_ = std::move(other.value_);
        other.value_ = nullptr;
    }

    return *this;
}

// ============================================================
//  Section 3 — Parsing
// ============================================================

Json Json::parse(std::string_view text) {
    Parser parser{std::string(text)};
    return parser.parse();
}

Json Json::parseOwned(std::string text) {
    Parser parser{std::move(text)};
    return parser.parse();
}

// ============================================================
//  Section 4 — Type Inspection
// ============================================================
// Type is derived directly from the variant's active index — no separate
// discriminant is stored or kept in sync.

Json::Type Json::type() const noexcept {
    return static_cast<Type>(value_.index());
}

bool Json::isNull() const noexcept {
    return value_.index() == static_cast<std::size_t>(Type::Null);
}

bool Json::isBool() const noexcept {
    return value_.index() == static_cast<std::size_t>(Type::Bool);
}

bool Json::isNumber() const noexcept {
    return value_.index() == static_cast<std::size_t>(Type::Number);
}

bool Json::isString() const noexcept {
    return value_.index() == static_cast<std::size_t>(Type::String);
}

bool Json::isArray() const noexcept {
    return value_.index() == static_cast<std::size_t>(Type::Array);
}

bool Json::isObject() const noexcept {
    return value_.index() == static_cast<std::size_t>(Type::Object);
}

// ============================================================
//  Section 5 — Value Access
// ============================================================

bool Json::asBool() const {
    const auto* p = std::get_if<bool>(&value_);

    if (!p)
        throw JsonTypeError("Json: not a bool");

    return *p;
}

double Json::asNumber() const {
    const auto* p = std::get_if<double>(&value_);

    if (!p)
        throw JsonTypeError("Json: not a number");

    return *p;
}

const std::string& Json::asString() const {
    const auto* p = std::get_if<std::string>(&value_);

    if (!p)
        throw JsonTypeError("Json: not a string");

    return *p;
}

Json::ArrayType& Json::asArray() {
    auto* p = std::get_if<ArrayType>(&value_);

    if (!p)
        throw JsonTypeError("Json: not an array");

    return *p;
}

const Json::ArrayType& Json::asArray() const {
    const auto* p = std::get_if<ArrayType>(&value_);

    if (!p)
        throw JsonTypeError("Json: not an array");

    return *p;
}

Json::ObjectType& Json::asObject() {
    auto* p = std::get_if<ObjectType>(&value_);

    if (!p)
        throw JsonTypeError("Json: not an object");

    return *p;
}

const Json::ObjectType& Json::asObject() const {
    const auto* p = std::get_if<ObjectType>(&value_);

    if (!p)
        throw JsonTypeError("Json: not an object");

    return *p;
}

// ============================================================
//  Section 6 — Navigation
// ============================================================

Json& Json::operator[](std::size_t index) {
    auto* p = std::get_if<ArrayType>(&value_);

    if (!p)
        throw JsonTypeError("Json: not an array");

    return (*p)[index];
}

const Json& Json::operator[](std::size_t index) const {
    const auto* p = std::get_if<ArrayType>(&value_);

    if (!p)
        throw JsonTypeError("Json: not an array");

    return (*p)[index];
}

Json& Json::operator[](std::string_view key) {
    auto* p = std::get_if<ObjectType>(&value_);

    if (!p)
        throw JsonTypeError("Json: not an object");

    return (*p)[key];
}

const Json& Json::operator[](std::string_view key) const {
    const auto* p = std::get_if<ObjectType>(&value_);

    if (!p)
        throw JsonTypeError("Json: not an object");

    // Unlike the non-const overload, this can't auto-vivify a missing key
    // (the object is const), so a missing key throws instead.
    const Json* found = p->find(key);

    if (!found)
        throw JsonOutOfRange("Json: key not found");

    return *found;
}

// ============================================================
//  Section 7 — Element Access
// ============================================================

Json& Json::at(std::size_t index) {
    auto* p = std::get_if<ArrayType>(&value_);

    if (!p)
        throw JsonTypeError("Json: not an array");

    if (index >= p->size())
        throw JsonOutOfRange("Json::at: index out of range");

    return (*p)[index];
}

const Json& Json::at(std::size_t index) const {
    const auto* p = std::get_if<ArrayType>(&value_);

    if (!p)
        throw JsonTypeError("Json: not an array");

    if (index >= p->size())
        throw JsonOutOfRange("Json::at: index out of range");

    return (*p)[index];
}

Json& Json::at(std::string_view key) {
    auto* p = std::get_if<ObjectType>(&value_);

    if (!p)
        throw JsonTypeError("Json: not an object");

    Json* found = p->find(key);

    if (!found)
        throw JsonOutOfRange("Json::at: key not found");

    return *found;
}

const Json& Json::at(std::string_view key) const {
    const auto* p = std::get_if<ObjectType>(&value_);

    if (!p)
        throw JsonTypeError("Json: not an object");

    const Json* found = p->find(key);

    if (!found)
        throw JsonOutOfRange("Json::at: key not found");

    return *found;
}

// ============================================================
//  Section 8 — Utilities
// ============================================================

std::size_t Json::size() const noexcept {
    if (const auto* p = std::get_if<ArrayType>(&value_))
        return p->size();

    if (const auto* p = std::get_if<ObjectType>(&value_))
        return p->size();

    return 0;
}

bool Json::contains(std::string_view key) const noexcept {
    const auto* p = std::get_if<ObjectType>(&value_);

    if (!p)
        return false;

    return p->contains(key);
}

// ============================================================
//  Section 9 — Comparison
// ============================================================

bool Json::operator==(const Json& other) const {
    if (value_.index() != other.value_.index())
        return false;

    switch (type()) {
    case Type::Null:
        return true;
    case Type::Bool:
        return std::get<bool>(value_) == std::get<bool>(other.value_);
    case Type::Number:
        return std::get<double>(value_) == std::get<double>(other.value_);
    case Type::String:
        return std::get<std::string>(value_) == std::get<std::string>(other.value_);
    case Type::Array:
        return std::get<ArrayType>(value_) == std::get<ArrayType>(other.value_);
    case Type::Object:
        return std::get<ObjectType>(value_) == std::get<ObjectType>(other.value_);
    }

    return false;
}

bool Json::operator!=(const Json& other) const {
    return !(*this == other);
}

// ============================================================
//  Section 10 — Serialization
// ============================================================
// dump(ostream) writes directly to the stream, avoiding the per-recursion
// string concatenation dump(int) would otherwise require.

void Json::dump(std::ostream& os, int indent) const {
    switch (type()) {
    case Type::Null:
        os << "null";
        return;

    case Type::Bool:
        os << (std::get<bool>(value_) ? "true" : "false");
        return;

    case Type::Number:
        writeNumber(os, std::get<double>(value_));
        return;

    case Type::String:
        writeEscapedString(os, std::get<std::string>(value_));
        return;

    case Type::Array: {
        const auto& arr = std::get<ArrayType>(value_);

        if (arr.empty()) {
            os << "[]";
            return;
        }

        os << "[\n";

        for (std::size_t i = 0; i < arr.size(); ++i) {
            writeIndent(os, static_cast<std::size_t>(indent) + 2);
            arr[i].dump(os, indent + 2);

            if (i + 1 != arr.size())
                os << ',';

            os << '\n';
        }

        writeIndent(os, static_cast<std::size_t>(indent));
        os << ']';
        return;
    }

    case Type::Object: {
        const auto& obj = std::get<ObjectType>(value_);

        if (obj.empty()) {
            os << "{}";
            return;
        }

        os << "{\n";

        // entries() is already in insertion order, so this is a
        // straight cache-friendly vector walk — unlike iterating a
        // separate key-order list and doing a hash lookup per key.
        const auto& entries = obj.entries();

        for (std::size_t i = 0; i < entries.size(); ++i) {
            writeIndent(os, static_cast<std::size_t>(indent) + 2);
            writeEscapedString(os, entries[i].first);
            os << ": ";
            entries[i].second.dump(os, indent + 2);

            if (i + 1 != entries.size())
                os << ',';

            os << '\n';
        }

        writeIndent(os, static_cast<std::size_t>(indent));
        os << '}';
        return;
    }
    }
}

// dump(int) delegates to dump(ostream) to avoid string concatenation per recursion.
std::string Json::dump(int indent) const {
    std::ostringstream oss;
    dump(oss, indent);
    return oss.str();
}

} // namespace JsonPro

/// @brief Short alias so this library can be used as `rain::Json`, while
/// its true namespace (and all internal diagnostics) remains `JsonPro`.
/// See JsonObject.cpp, Parser.cpp, and JsonException.h for the same alias
/// applied to `rain::JsonObject`, `rain::Parser`, and the exception types.
namespace rain = JsonPro;