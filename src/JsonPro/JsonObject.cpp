// ============================================================
// JsonObject.cpp
// Implementation for JsonPro::JsonObject.
// ============================================================
//
// Sections:
//   1. Lifecycle
//   2. Element Access
//   3. Insertion
//   4. Capacity & Query
//   5. Removal
//   6. Comparison
//
// ============================================================

#include <algorithm>
#include <unordered_map>

#include <JsonPro/Json.h>
#include <JsonPro/JsonObject.h>

namespace JsonPro {

// Impl is only defined here, where Json is complete, so
// std::unordered_map<std::string, Json> can be fully instantiated.
struct JsonObject::Impl {
    std::unordered_map<std::string, Json> map;
};

// ============================================================
// Section 1 — Lifecycle
// ============================================================
JsonObject::JsonObject() :
    impl_(std::make_unique<Impl>()) {}

JsonObject::~JsonObject() = default;

JsonObject::JsonObject(const JsonObject& other) :
    impl_(std::make_unique<Impl>(*other.impl_)),
    order_(other.order_) {}

JsonObject::JsonObject(JsonObject&& other) noexcept = default;

JsonObject& JsonObject::operator=(const JsonObject& other) {
    if (this != &other) {
        impl_  = std::make_unique<Impl>(*other.impl_);
        order_ = other.order_;
    }

    return *this;
}

JsonObject& JsonObject::operator=(JsonObject&& other) noexcept = default;


// ============================================================
// Section 2 — Element Access
// ============================================================
Json& JsonObject::operator[](const std::string& key) {
    auto it = impl_->map.find(key);

    // Auto-vivifies: a missing key is inserted with a default Json and its
    // insertion order recorded, matching std::unordered_map's operator[].
    if (it == impl_->map.end()) {
        order_.push_back(key);
        it = impl_->map.emplace(key, Json()).first;
    }

    return it->second;
}

Json* JsonObject::find(const std::string& key) {
    auto it = impl_->map.find(key);
    return it == impl_->map.end() ? nullptr : &it->second;
}

const Json* JsonObject::find(const std::string& key) const {
    auto it = impl_->map.find(key);
    return it == impl_->map.end() ? nullptr : &it->second;
}


// ============================================================
// Section 3 — Insertion
// ============================================================
Json& JsonObject::insert_or_assign(std::string key, Json value) {
    auto it = impl_->map.find(key);

    // order_ only grows on a genuinely new key; overwriting an existing
    // key preserves its original insertion position.
    if (it == impl_->map.end()) {
        order_.push_back(key);
        auto result = impl_->map.emplace(std::move(key), std::move(value));
        return result.first->second;
    }

    it->second = std::move(value);
    return it->second;
}

bool JsonObject::emplace(std::string key, Json value) {
    auto it = impl_->map.find(key);

    if (it != impl_->map.end())
        return false;

    order_.push_back(key);
    impl_->map.emplace(std::move(key), std::move(value));
    return true;
}


// ============================================================
// Section 4 — Capacity & Query
// ============================================================
std::size_t JsonObject::size() const noexcept {
    return impl_->map.size();
}

bool JsonObject::empty() const noexcept {
    return impl_->map.empty();
}

bool JsonObject::contains(const std::string& key) const noexcept {
    return impl_->map.contains(key);
}


// ============================================================
// Section 5 — Removal
// ============================================================
std::size_t JsonObject::erase(const std::string& key) {
    auto it = impl_->map.find(key);

    if (it == impl_->map.end())
        return 0;

    impl_->map.erase(it);

    // order_ is a plain vector, so removal is an O(n) linear search + erase
    // rather than O(1) -- acceptable since erase() is not on the hot path.
    auto orderIt = std::find(order_.begin(), order_.end(), key);
    if (orderIt != order_.end())
        order_.erase(orderIt);

    return 1;
}


// ============================================================
// Section 6 — Comparison
// ============================================================
bool JsonObject::operator==(const JsonObject& other) const noexcept {
    return impl_->map == other.impl_->map;
}

} // namespace JsonPro