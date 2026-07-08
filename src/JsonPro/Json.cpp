// ============================================================
// Json.cpp
// Implementation for JsonPro::Json.
// ============================================================
//
// Sections:
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

#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <JsonPro/Json.h>
#include <JsonPro/Parser.h>

namespace JsonPro {

namespace {
    // returns a string of n spaces for indentation
    std::string pad(std::size_t n) {
        return std::string(n, ' ');
    }
}

// ============================================================
// Section 1 — Constructors & Destructor
// ============================================================
Json::Json() noexcept :
    value_(nullptr) {}

Json::Json(std::nullptr_t) noexcept :
    value_(nullptr) {}

Json::Json(bool value) noexcept :
    value_(value) {}

Json::Json(double value) noexcept :
    value_(value) {}

Json::Json(int value) noexcept :
    value_(static_cast<double>(value)) {}

Json::Json(const std::string& value) :
    value_(value) {}

Json::Json(std::string&& value) noexcept :
    value_(std::move(value)) {}

Json::Json(const char* value) :
    value_(std::string(value)) {}

Json::Json(const ArrayType& value) :
    value_(value) {}

Json::Json(ArrayType&& value) noexcept :
    value_(std::move(value)) {}

Json::Json(const ObjectType& value) :
    value_(value) {}

Json::Json(ObjectType&& value) noexcept :
    value_(std::move(value)) {}


// ============================================================
// Section 2 — Move Semantics
// ============================================================
Json::Json(Json&& other) noexcept :
    value_(std::move(other.value_))
{
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
// Section 3 — Parsing
// ============================================================
Json Json::parse(std::string_view text) {
    Parser parser{ std::string(text) };

    return parser.parse();
}


// ============================================================
// Section 4 — Type Inspection
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
// Section 5 — Value Access
// ============================================================
bool Json::asBool() const {
    const auto* p = std::get_if<bool>(&value_);

    if (!p)
        throw std::runtime_error("Json: not a bool");

    return *p;
}

double Json::asNumber() const {
    const auto* p = std::get_if<double>(&value_);

    if (!p)
        throw std::runtime_error("Json: not a number");

    return *p;
}

const std::string& Json::asString() const {
    const auto* p = std::get_if<std::string>(&value_);

    if (!p)
        throw std::runtime_error("Json: not a string");

    return *p;
}

Json::ArrayType& Json::asArray() {
    auto* p = std::get_if<ArrayType>(&value_);

    if (!p)
        throw std::runtime_error("Json: not an array");

    return *p;
}

const Json::ArrayType& Json::asArray() const {
    const auto* p = std::get_if<ArrayType>(&value_);

    if (!p)
        throw std::runtime_error("Json: not an array");

    return *p;
}

Json::ObjectType& Json::asObject() {
    auto* p = std::get_if<ObjectType>(&value_);

    if (!p)
        throw std::runtime_error("Json: not an object");

    return *p;
}

const Json::ObjectType& Json::asObject() const {
    const auto* p = std::get_if<ObjectType>(&value_);

    if (!p)
        throw std::runtime_error("Json: not an object");

    return *p;
}


// ============================================================
// Section 6 — Navigation
// ============================================================
Json& Json::operator[](std::size_t index) {
    auto* p = std::get_if<ArrayType>(&value_);

    if (!p)
        throw std::runtime_error("Json: not an array");

    return (*p)[index];
}

const Json& Json::operator[](std::size_t index) const {
    const auto* p = std::get_if<ArrayType>(&value_);

    if (!p)
        throw std::runtime_error("Json: not an array");

    return (*p)[index];
}

Json& Json::operator[](const std::string& key) {
    auto* p = std::get_if<ObjectType>(&value_);

    if (!p)
        throw std::runtime_error("Json: not an object");

    return (*p)[key];
}

const Json& Json::operator[](const std::string& key) const {
    const auto* p = std::get_if<ObjectType>(&value_);

    if (!p)
        throw std::runtime_error("Json: not an object");

    // Unlike the non-const overload, this can't auto-vivify a missing key
    // (the object is const), so a missing key throws instead.
    const Json* found = p->find(key);

    if (!found)
        throw std::out_of_range("Json: key not found");

    return *found;
}


// ============================================================
// Section 7 — Element Access
// ============================================================
Json& Json::at(std::size_t index) {
    auto* p = std::get_if<ArrayType>(&value_);

    if (!p)
        throw std::runtime_error("Json: not an array");

    if (index >= p->size())
        throw std::out_of_range("Json::at: index out of range");

    return (*p)[index];
}

const Json& Json::at(std::size_t index) const {
    const auto* p = std::get_if<ArrayType>(&value_);

    if (!p)
        throw std::runtime_error("Json: not an array");

    if (index >= p->size())
        throw std::out_of_range("Json::at: index out of range");

    return (*p)[index];
}

Json& Json::at(const std::string& key) {
    auto* p = std::get_if<ObjectType>(&value_);

    if (!p)
        throw std::runtime_error("Json: not an object");

    Json* found = p->find(key);

    if (!found)
        throw std::out_of_range("Json::at: key not found");

    return *found;
}

const Json& Json::at(const std::string& key) const {
    const auto* p = std::get_if<ObjectType>(&value_);

    if (!p)
        throw std::runtime_error("Json: not an object");

    const Json* found = p->find(key);

    if (!found)
        throw std::out_of_range("Json::at: key not found");

    return *found;
}


// ============================================================
// Section 8 — Utilities
// ============================================================
std::size_t Json::size() const noexcept {
    if (const auto* p = std::get_if<ArrayType>(&value_))
        return p->size();

    if (const auto* p = std::get_if<ObjectType>(&value_))
        return p->size();

    return 0;
}

bool Json::contains(const std::string& key) const noexcept {
    const auto* p = std::get_if<ObjectType>(&value_);

    if (!p)
        return false;

    return p->contains(key);
}


// ============================================================
// Section 9 — Comparison
// ============================================================
bool Json::operator==(const Json& other) const {
    if (value_.index() != other.value_.index())
        return false;

    switch (type()) {
        case Type::Null:   return true;
        case Type::Bool:   return std::get<bool>(value_)        == std::get<bool>(other.value_);
        case Type::Number: return std::get<double>(value_)      == std::get<double>(other.value_);
        case Type::String: return std::get<std::string>(value_) == std::get<std::string>(other.value_);
        case Type::Array:  return std::get<ArrayType>(value_)   == std::get<ArrayType>(other.value_);
        case Type::Object: return std::get<ObjectType>(value_)  == std::get<ObjectType>(other.value_);
    }

    return false;
}

bool Json::operator!=(const Json& other) const {
    return !(*this == other);
}


// ============================================================
// Section 10 — Serialization
// ============================================================
// dump(ostream) writes directly to the stream avoiding per-recursion string allocations
void Json::dump(std::ostream& os, int indent) const {
    switch (type()) {
        case Type::Null:
            os << "null";
            return;

        case Type::Bool:
            os << (std::get<bool>(value_) ? "true" : "false");
            return;

        case Type::Number:
            os << std::get<double>(value_);
            return;

        case Type::String:
            os << '"' << std::get<std::string>(value_) << '"';
            return;

        case Type::Array: {
            const auto& arr = std::get<ArrayType>(value_);

            if (arr.empty()) { os << "[]"; return; }

            os << "[\n";

            for (std::size_t i = 0; i < arr.size(); ++i) {
                os << pad(indent + 2);
                arr[i].dump(os, indent + 2);

                if (i + 1 != arr.size())
                    os << ',';

                os << '\n';
            }

            os << pad(indent) << ']';
            return;
        }

        case Type::Object: {
            const auto& obj = std::get<ObjectType>(value_);

            if (obj.empty()) { os << "{}"; return; }

            os << "{\n";

            // Iterates via insertionOrder() rather than the container's own
            // iteration order, so output keys appear in the order they were
            // inserted rather than in bucket/hash order.
            const auto& order = obj.insertionOrder();
            bool first = true;

            for (const auto& key : order) {
                if (!first)
                    os << ",\n";

                os << pad(indent + 2)
                   << '"' << key << "\": ";

                obj.find(key)->dump(os, indent + 2);

                first = false;
            }

            os << '\n' << pad(indent) << '}';
            return;
        }
    }
}

// dump(int) delegates to dump(ostream) to avoid string concatenation per recursion
std::string Json::dump(int indent) const {
    std::ostringstream oss;
    dump(oss, indent);
    return oss.str();
}

} // namespace JsonPro