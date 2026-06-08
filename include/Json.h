#pragma once

#include <cctype>
#include <cstddef>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>

class Json {
public:

    // =========================
    // Type Definition
    // =========================
    enum class Type {
        Null,
        Bool,
        Number,
        String,
        Array,
        Object
    };

    // =========================
    // Aliases
    // =========================
    using Array = std::vector<Json>;
    using Object = std::unordered_map<std::string, Json>;

private:

    // =========================
    // Core State
    // =========================
    Type type_;

    std::variant<
        std::nullptr_t,
        bool,
        double,
        std::string,
        Array,
        Object
    > value_;

public:

    // =========================
    // Constructors
    // =========================
    Json();
    Json(std::nullptr_t);

    Json(bool value);
    Json(double value);
    Json(int value);

    Json(const std::string& value);
    Json(const char* value);

    Json(const Array& value);
    Json(const Object& value);
    
    Json(const Json& other);
    Json& operator=(const Json& other);
    
    Json(Json&& other) noexcept;
    Json& operator=(Json&& other) noexcept;
    
    // =========================
    // Parsing
    // =========================
    static Json parse(const std::string& text);

    // =========================
    // Type Inspection
    // =========================
    Type type() const noexcept;
    bool isNull() const noexcept;
    bool isBool() const noexcept;
    bool isNumber() const noexcept;
    bool isString() const noexcept;
    bool isArray() const noexcept;
    bool isObject() const noexcept;

    // =========================
    // Value Access
    // =========================
    bool asBool() const;
    double asNumber() const;
    const std::string& asString() const;

    Array& asArray();
    const Array& asArray() const;

    Object& asObject();
    const Object& asObject() const;

    // =========================
    // Navigation
    // =========================
    Json& operator[](const std::string& key);
    const Json& operator[](const std::string& key) const;

    Json& operator[](std::size_t index);
    const Json& operator[](std::size_t index) const;

    // =========================
    // Utilities
    // =========================
    std::size_t size() const noexcept;
    bool contains(const std::string& key) const;

    // =========================
    // Comparison
    // =========================
    bool operator==(const Json& other) const;
    bool operator!=(const Json& other) const;

    // =========================
    // Serialization
    // =========================
    std::string dump(int indent = 0) const;
};