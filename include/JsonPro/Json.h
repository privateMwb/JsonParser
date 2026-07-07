#pragma once

#include <cstddef>
#include <iosfwd>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace JsonPro {

class Json;

// Insertion-order-preserving map used for JSON objects.
//
// A plain std::unordered_map does not preserve the order keys were
// inserted in, so round-tripping an object through parse()/dump() can
// reorder its keys. JsonObject keeps entries in a vector (insertion
// order) alongside a hash index (key -> position) so lookups stay
// O(1) average while iteration/dump order matches insertion order.
class JsonObject {
public:
    using Container      = std::vector<std::pair<std::string, Json>>;
    using iterator       = Container::iterator;
    using const_iterator = Container::const_iterator;

    // Defined out-of-line (Json.cpp): the bodies below index/iterate a
    // vector<pair<string, Json>>, which needs Json to be a complete type.
    // Json is still only forward-declared at this point in the header,
    // so these can't be defined inline here the way the rest of this
    // class's (non-Json-touching) declarations are.
    JsonObject();
    JsonObject(const JsonObject&);
    JsonObject& operator=(const JsonObject&);
    JsonObject(JsonObject&&) noexcept;
    JsonObject& operator=(JsonObject&&) noexcept;
    ~JsonObject();

    [[nodiscard]] iterator       begin()       noexcept;
    [[nodiscard]] iterator       end()         noexcept;
    [[nodiscard]] const_iterator begin() const noexcept;
    [[nodiscard]] const_iterator end()   const noexcept;

    [[nodiscard]] bool        empty() const noexcept;
    [[nodiscard]] std::size_t size()  const noexcept;

    [[nodiscard]] iterator       find(const std::string& key) noexcept;
    [[nodiscard]] const_iterator find(const std::string& key) const noexcept;

    [[nodiscard]] bool contains(const std::string& key) const noexcept;

    // Auto-vivifying access: inserts a Null entry at the end (preserving
    // creation order) if the key doesn't already exist.
    Json& operator[](const std::string& key);

    // Single lookup + insert-or-replace, without disturbing the position
    // of an already-existing key. Defined out-of-line for the same reason
    // as the methods above.
    void insert_or_assign(std::string key, Json value);

    // Matches std::unordered_map::emplace semantics: inserts only if the
    // key is absent (existing entries are left untouched, unlike
    // insert_or_assign). Returns {iterator to element, true if inserted}.
    std::pair<iterator, bool> emplace(std::string key, Json value);

    [[nodiscard]] bool operator==(const JsonObject& other) const;

private:
    Container items_;
    std::unordered_map<std::string, std::size_t> index_;
};

class Json {
public:

    // Type
    enum class Type {
        Null,
        Bool,
        Number,
        String,
        Array,
        Object
    };

    // Aliases
    using ArrayType  = std::vector<Json>;
    using ObjectType = JsonObject;

private:

    // Core State
    // Type is derived from value_.index() — no separate discriminant is stored.
    std::variant<
        std::nullptr_t,
        bool,
        double,
        std::string,
        ArrayType,
        ObjectType
        > value_;

public:

    // Constructors & Destructor
    Json() noexcept;
    Json(std::nullptr_t) noexcept;

    Json(bool value) noexcept;

    Json(double value) noexcept;
    Json(int value) noexcept;

    Json(const std::string& value);
    Json(std::string&& value) noexcept;
    Json(const char* value);

    Json(const ArrayType& value);
    Json(ArrayType&& value) noexcept;

    Json(const ObjectType& value);
    Json(ObjectType&& value) noexcept;

    Json(const Json& other)            = default;
    Json& operator=(const Json& other) = default;

    Json(Json&& other) noexcept;
    Json& operator=(Json&& other) noexcept;

    // Parsing
    [[nodiscard]] static Json parse(std::string_view text);

    // Type Inspection
    [[nodiscard]] Type type()     const noexcept;
    [[nodiscard]] bool isNull()   const noexcept;
    [[nodiscard]] bool isBool()   const noexcept;
    [[nodiscard]] bool isNumber() const noexcept;
    [[nodiscard]] bool isString() const noexcept;
    [[nodiscard]] bool isArray()  const noexcept;
    [[nodiscard]] bool isObject() const noexcept;

    // Value Access
    [[nodiscard]] bool                asBool()   const;
    [[nodiscard]] double              asNumber() const;
    [[nodiscard]] const std::string&  asString() const;

    [[nodiscard]] ArrayType&          asArray();
    [[nodiscard]] const ArrayType&    asArray()  const;

    [[nodiscard]] ObjectType&         asObject();
    [[nodiscard]] const ObjectType&   asObject() const;

    // Navigation
    // Unlike at(), these auto-vivify: a missing key/out-of-range index
    // inserts/extends with a Null Json rather than throwing.
    [[nodiscard]] Json&       operator[](const std::string& key);
    [[nodiscard]] const Json& operator[](const std::string& key) const;

    [[nodiscard]] Json&       operator[](std::size_t index);
    [[nodiscard]] const Json& operator[](std::size_t index) const;

    // Element Access
    // Bounds/key-checked access: throws on out-of-range index or missing key.
    [[nodiscard]] Json& at(std::size_t index);
    [[nodiscard]] const Json& at(std::size_t index) const;

    [[nodiscard]] Json& at(const std::string& key);
    [[nodiscard]] const Json& at(const std::string& key) const;

    // Utilities
    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] bool contains(const std::string& key) const noexcept;

    // Comparison
    [[nodiscard]] bool operator==(const Json& other) const;
    [[nodiscard]] bool operator!=(const Json& other) const;

    // Serialization
    // indent == 0 produces compact output; indent > 0 pretty-prints with
    // that many spaces per nesting level.
    [[nodiscard]] std::string dump(int indent = 0) const;
    void dump(std::ostream& os, int indent = 0) const;
};

} // namespace JsonPro