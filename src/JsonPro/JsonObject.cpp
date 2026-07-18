/**
 * @file JsonObject.cpp
 * @brief JsonObject implementation.
 *
 * Contains the implementation of JsonObject member functions and
 * internal implementation details.
 */

// ============================================================
// Implementation for JsonPro::JsonObject.
// ============================================================
//
//  Sections:
//   1. Lifecycle
//   2. Element Access
//   3. Insertion
//   4. Capacity & Query
//   5. Removal
//   6. Comparison
//
// ============================================================

// clang-format off
#include <JsonPro/JsonObject.h>
#include <JsonPro/Json.h>        // Json must be complete here: entries_ is vector<pair<string, Json>>.
// clang-format on

namespace JsonPro {

// ============================================================
//  Section 1 — Lifecycle
// ============================================================
// Every special member is declared in the header (where Json is only
// forward-declared) and explicitly defaulted here, where Json.h has already
// made Json complete. This is the same incomplete-type technique a PIMPL
// would use, but applied directly to entries_ instead of routing through an
// extra heap-allocated Impl — one fewer allocation per JsonObject, including
// empty ones.

JsonObject::JsonObject() = default;
JsonObject::~JsonObject() = default;

JsonObject::JsonObject(const JsonObject& other) = default;
JsonObject::JsonObject(JsonObject&& other) noexcept = default;

JsonObject& JsonObject::operator=(const JsonObject& other) = default;
JsonObject& JsonObject::operator=(JsonObject&& other) noexcept = default;

// ============================================================
//  Section 2 — Element Access
// ============================================================

Json& JsonObject::operator[](std::string_view key) {
    if (const auto it = index_.find(key); it != index_.end())
        return entries_[it->second].second;

    // New key: two allocations are unavoidable here (one for the index's
    // own copy, one for entries_'s copy) since key is a non-owning view
    // with nothing to move from.
    const std::size_t idx = entries_.size();
    index_.emplace(std::string(key), idx);
    entries_.emplace_back(std::string(key), Json());
    return entries_.back().second;
}

Json* JsonObject::find(std::string_view key) {
    const auto it = index_.find(key);
    return it == index_.end() ? nullptr : &entries_[it->second].second;
}

const Json* JsonObject::find(std::string_view key) const {
    const auto it = index_.find(key);
    return it == index_.end() ? nullptr : &entries_[it->second].second;
}

// ============================================================
//  Section 3 — Insertion
// ============================================================

Json& JsonObject::insert_or_assign(std::string key, Json value) {
    if (const auto it = index_.find(key); it != index_.end()) {
        Json& slot = entries_[it->second].second;
        slot = std::move(value);
        return slot;
    }

    // Existing key not found: copy key into the index first, then move the
    // caller's original into entries_ — one copy total, not two.
    const std::size_t idx = entries_.size();
    index_.emplace(key, idx);
    entries_.emplace_back(std::move(key), std::move(value));
    return entries_.back().second;
}

bool JsonObject::emplace(std::string key, Json value) {
    if (index_.contains(key))
        return false;

    const std::size_t idx = entries_.size();
    index_.emplace(key, idx);
    entries_.emplace_back(std::move(key), std::move(value));
    return true;
}

// ============================================================
//  Section 4 — Capacity & Query
// ============================================================

void JsonObject::reserve(std::size_t n) {
    entries_.reserve(n);
    index_.reserve(n);
}

std::size_t JsonObject::size() const noexcept {
    return entries_.size();
}

bool JsonObject::empty() const noexcept {
    return entries_.empty();
}

bool JsonObject::contains(std::string_view key) const noexcept {
    return index_.contains(key);
}

// ============================================================
//  Section 5 — Removal
// ============================================================

std::size_t JsonObject::erase(std::string_view key) {
    const auto it = index_.find(key);
    if (it == index_.end())
        return 0;

    const std::size_t idx = it->second;
    index_.erase(it);
    entries_.erase(entries_.begin() + static_cast<std::ptrdiff_t>(idx));

    // Every entry after the erased one just shifted down by one position in
    // entries_; the index must follow. erase() is not on the hot path, so
    // this O(n) scan of index_ is an acceptable cost for keeping it
    // consistent with entries_.
    for (auto& [entryKey, entryIdx] : index_) {
        if (entryIdx > idx)
            --entryIdx;
    }

    return 1;
}

// ============================================================
//  Section 6 — Comparison
// ============================================================
// Member order carries no meaning in the JSON spec, so equality is defined
// as "same key/value pairs" regardless of insertion order.

bool JsonObject::operator==(const JsonObject& other) const {
    if (entries_.size() != other.entries_.size())
        return false;

    for (const auto& [key, value] : entries_) {
        const Json* otherValue = other.find(key);
        if (otherValue == nullptr || !(*otherValue == value))
            return false;
    }

    return true;
}

} // namespace JsonPro

/// @brief Short alias so this library can be used as `rain::JsonObject`,
/// while its true namespace (and all internal diagnostics) remains
/// `JsonPro`. See Json.cpp, Parser.cpp, and JsonException.h for the same
/// alias applied to `rain::Json`, `rain::Parser`, and the exception types.
namespace rain = JsonPro;