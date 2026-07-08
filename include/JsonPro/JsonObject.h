#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace JsonPro {

class Json; // forward declaration

// JsonObject wraps a hash map for O(1) average lookup, while recording
// each key's first-insertion position in a parallel vector, so
// Json::dump() can serialize in creation order regardless of hash
// bucket layout.
//
// The map is stored behind a pointer (PIMPL), not by value. Json (the
// map's value type) is necessarily incomplete at this point, since
// Json's own std::variant holds a JsonObject by value in turn — and
// unlike std::vector/std::list/std::forward_list, std::unordered_map
// is not guaranteed to support an incomplete value type. A pointer to
// the map has no such requirement; the map itself is only ever fully
// instantiated inside JsonObject.cpp, where Json is complete.
class JsonObject {
private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
    std::vector<std::string> order_;
    
public:
    JsonObject();
    ~JsonObject();

    JsonObject(const JsonObject& other);
    JsonObject(JsonObject&& other) noexcept;
    JsonObject& operator=(const JsonObject& other);
    JsonObject& operator=(JsonObject&& other) noexcept;

    // Unchecked access. Inserts a default-constructed Json (recording
    // insertion order) if the key is new.
    Json& operator[](const std::string& key);

    // Single lookup + insert-or-replace. Records insertion order only
    // the first time a key appears.
    Json& insert_or_assign(std::string key, Json value);

    // Inserts key/value only if the key is not already present.
    // Returns true if inserted, false if the key already existed
    // (existing value left unchanged).
    bool emplace(std::string key, Json value);

    // Returns a pointer to the value for key, or nullptr if absent.
    [[nodiscard]] Json* find(const std::string& key);
    [[nodiscard]] const Json* find(const std::string& key) const;

    [[nodiscard]] std::size_t size()  const noexcept;
    [[nodiscard]] bool empty()        const noexcept;
    [[nodiscard]] bool contains(const std::string& key) const noexcept;

    // Removes the entry for key, if present, from both the lookup map
    // and the insertion-order record. Returns 1 if an entry was
    // removed, 0 if the key was not present (matches
    // std::unordered_map::erase's single-key return convention).
    std::size_t erase(const std::string& key);

    // Keys in first-insertion order. Used by Json::dump() for
    // creation-order serialization.
    [[nodiscard]] const std::vector<std::string>& insertionOrder() const noexcept { return order_; }

    [[nodiscard]] bool operator==(const JsonObject& other) const noexcept;
    [[nodiscard]] bool operator!=(const JsonObject& other) const noexcept { return !(*this == other); }
};

} // namespace JsonPro
