/**
 * @file JsonObject.h
 * @brief Insertion-order-preserving associative container used internally by Json.
 *
 * Contains the internal storage container backing Json's object type: a
 * vector of key/value entries plus a transparent-hash index for O(1)
 * average lookup without disturbing insertion order.
 */

#pragma once

// clang-format off
#include <cstddef>       // std::size_t
#include <functional>    // std::hash, std::equal_to
#include <string>        // std::string
#include <string_view>   // std::string_view
#include <unordered_map> // std::unordered_map
#include <utility>       // std::pair
#include <vector>        // std::vector
// clang-format on

// JsonObject stores entries directly in a std::vector<std::pair<std::string,
// Json>>, in insertion order, plus a side unordered_map<std::string,
// std::size_t> that maps each key to its index in that vector. The vector
// *is* the iteration order, so Json::dump() needs no separate
// order-tracking structure and iterates it with full cache locality.
//
// Json is forward-declared here and only required complete inside
// JsonObject.cpp. This works because std::vector (unlike
// std::unordered_map) is permitted to hold an incomplete value type, as
// long as no member function requiring a complete type is instantiated
// before Json is complete. Accordingly, every special member function
// below is declared here and defined out-of-line in JsonObject.cpp, where
// Json.h has already been included.
//
// The index map's hasher is transparent (StringHash + std::equal_to<>),
// so find()/contains()/erase() can be called with a std::string_view and
// never construct a temporary std::string just to perform the lookup.

namespace JsonPro {

class Json; // forward declaration; see file comment above.

/**
 * @brief Transparent hash functor for heterogeneous string-key lookup.
 * @details Lets an `unordered_map<std::string, ...>` be searched with a
 * `std::string_view` (or `const char*`) directly, avoiding the temporary
 * `std::string` allocation that a non-transparent hasher would force on
 * every lookup.
 */
struct StringHash {
    using is_transparent = void;

    [[nodiscard]] std::size_t operator()(std::string_view sv) const noexcept {
        return std::hash<std::string_view>{}(sv);
    }
};

/**
 * @brief An insertion-order-preserving string-keyed map from Json object members to values.
 * @details Backs `Json`'s object type. Entries are stored contiguously in
 * insertion order; a side index gives O(1) average lookup, insertion, and
 * membership testing without disturbing that order. Iterating the map (via
 * `entries()`) directly yields creation order, which is what `Json::dump()`
 * needs for stable, human-expected object serialization.
 */
class JsonObject {
  public:
    /// @brief A single key/value entry, in the order it was inserted.
    using EntryType = std::pair<std::string, Json>;

  private:
    // Core storage, in insertion order. Doubles as the iteration order for
    // Json::dump() — no separate order-tracking structure is needed.
    std::vector<EntryType> entries_;

    // Maps each key to its index in entries_. Transparent hashing lets
    // lookups take a std::string_view without allocating.
    std::unordered_map<std::string, std::size_t, StringHash, std::equal_to<>> index_;

  public:
    /// @brief Constructs an empty object.
    JsonObject();
    /// @brief Destroys all entries.
    ~JsonObject();

    /**
     * @brief Copy-constructs an object, deep-copying `other`'s entries.
     * @param other Object to copy from.
     */
    JsonObject(const JsonObject& other);

    /**
     * @brief Move-constructs an object, taking ownership of `other`'s storage.
     * @param other Object to move from. Left empty.
     */
    JsonObject(JsonObject&& other) noexcept;

    /**
     * @brief Copy-assigns from `other`, replacing this object's entries.
     * @param other Object to copy from.
     * @return Reference to `*this`.
     */
    JsonObject& operator=(const JsonObject& other);

    /**
     * @brief Move-assigns from `other`, replacing this object's entries.
     * @param other Object to move from. Left empty.
     * @return Reference to `*this`.
     */
    JsonObject& operator=(JsonObject&& other) noexcept;

    /**
     * @brief Returns a reference to the value mapped to `key`, inserting a
     * default-constructed `Json` (recording insertion order) if `key` is new.
     * @param key Key to look up or insert.
     * @return Reference to the (possibly newly inserted) mapped value.
     */
    [[nodiscard]] Json& operator[](std::string_view key);

    /**
     * @brief Single lookup + insert-or-replace.
     * @param key Key to insert or update.
     * @param value Value to store.
     * @return Reference to the stored value.
     * @details Records insertion order only the first time `key` appears;
     * overwriting an existing key preserves its original position.
     */
    Json& insert_or_assign(std::string key, Json value);

    /**
     * @brief Inserts `key`/`value` only if `key` is not already present.
     * @param key Key to insert.
     * @param value Value to store.
     * @return `true` if inserted, `false` if `key` already existed (its
     * existing value is left unchanged).
     */
    bool emplace(std::string key, Json value);

    /**
     * @brief Finds the value mapped to `key`.
     * @param key Key to search for.
     * @return Pointer to the value, or `nullptr` if `key` is absent.
     */
    [[nodiscard]] Json* find(std::string_view key);
    /// @brief Const-qualified overload. See the non-const find().
    [[nodiscard]] const Json* find(std::string_view key) const;

    /**
     * @brief Reserves storage for at least `n` entries, in both the entry
     * vector and the index map, to avoid incremental reallocation while
     * building up a large object (e.g. during parsing).
     * @param n Number of entries to reserve capacity for.
     */
    void reserve(std::size_t n);

    /// @brief Returns the number of entries.
    [[nodiscard]] std::size_t size() const noexcept;
    /// @brief Returns whether the object has no entries.
    [[nodiscard]] bool empty() const noexcept;
    /// @brief Returns whether `key` is present.
    [[nodiscard]] bool contains(std::string_view key) const noexcept;

    /**
     * @brief Removes the entry for `key`, if present.
     * @param key Key to remove.
     * @return `1` if an entry was removed, `0` if `key` was not present
     * (matches `std::unordered_map::erase`'s single-key return convention).
     * @details Not on the hot path: an O(n) vector erase plus an O(n) scan
     * to shift every subsequent entry's recorded index down by one.
     */
    std::size_t erase(std::string_view key);

    /// @brief Entries in insertion order. Used by Json::dump() for stable serialization.
    [[nodiscard]] const std::vector<EntryType>& entries() const noexcept {
        return entries_;
    }

    /**
     * @brief Order-independent equality: same key/value pairs, regardless
     * of insertion order (member order carries no meaning in the JSON spec).
     * @param other Object to compare against.
     */
    [[nodiscard]] bool operator==(const JsonObject& other) const;
    /// @brief See operator==.
    [[nodiscard]] bool operator!=(const JsonObject& other) const {
        return !(*this == other);
    }
};

} // namespace JsonPro

/// @brief Short alias so this library can be used as `rain::JsonObject`,
/// while its true namespace (and all internal diagnostics) remains
/// `JsonPro`. See Json.h, Parser.h, and JsonException.h for the same
/// alias applied to `rain::Json`, `rain::Parser`, and the exception types.
namespace rain = JsonPro;