// JsonObject storage test suite.
//
// Coverage:
// - operator[](key) auto-vivifies a Null entry for a missing key
// - insert_or_assign() inserts new keys and updates existing ones in place
// - emplace() inserts only if absent, leaving existing values untouched
// - find() (mutable and const) locates values or returns nullptr
// - reserve() does not disturb existing contents
// - size() / empty() / contains()
// - erase() removes an entry and reports success/failure
// - entries() reflects insertion order, preserved across erase
// - operator== / operator!= are order-independent

#include <JsonPro/Json.h>

#include <gtest/gtest.h>

using namespace JsonPro;

// Verifies operator[](key) inserts a Null entry when the key is absent.
TEST(ObjectStorage, index_operator_inserts_default_null) {
    JsonObject obj;
    Json& v = obj["k"];

    EXPECT_TRUE(v.isNull());
    EXPECT_EQ(obj.size(), 1);
}

// Verifies operator[](key) returns the existing entry without duplicating it.
TEST(ObjectStorage, index_operator_returns_existing_entry) {
    JsonObject obj;
    obj["k"] = Json(1);

    Json& v = obj["k"];
    EXPECT_EQ(v.asNumber(), 1.0);
    EXPECT_EQ(obj.size(), 1);
}

// Verifies insert_or_assign() inserts a new key with the given value.
TEST(ObjectStorage, insert_or_assign_new_key) {
    JsonObject obj;
    Json& v = obj.insert_or_assign("k", Json("value"));

    EXPECT_EQ(v.asString(), "value");
    EXPECT_EQ(obj.size(), 1);
    EXPECT_TRUE(obj.contains("k"));
}

// Verifies insert_or_assign() overwrites an existing key's value in place,
// preserving its original position in entries().
TEST(ObjectStorage, insert_or_assign_existing_key_updates_in_place) {
    JsonObject obj;
    obj.insert_or_assign("a", Json(1));
    obj.insert_or_assign("b", Json(2));
    obj.insert_or_assign("a", Json(99));

    EXPECT_EQ(obj.size(), 2);
    EXPECT_EQ(obj.find("a")->asNumber(), 99.0);
    EXPECT_EQ(obj.entries()[0].first, "a"); // position unchanged
}

// Verifies emplace() inserts a new key and returns true.
TEST(ObjectStorage, emplace_inserts_new_key) {
    JsonObject obj;
    bool inserted = obj.emplace("k", Json(42));

    EXPECT_TRUE(inserted);
    EXPECT_EQ(obj.find("k")->asNumber(), 42.0);
}

// Verifies emplace() on an existing key returns false and leaves the value untouched.
TEST(ObjectStorage, emplace_existing_key_is_no_op) {
    JsonObject obj;
    obj.emplace("k", Json(1));
    bool inserted = obj.emplace("k", Json(999));

    EXPECT_FALSE(inserted);
    EXPECT_EQ(obj.find("k")->asNumber(), 1.0);
}

// Verifies mutable find() returns a pointer to the correct value.
TEST(ObjectStorage, find_returns_existing_value) {
    JsonObject obj;
    obj.emplace("k", Json("found"));

    Json* p = obj.find("k");
    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->asString(), "found");
}

// Verifies find() returns nullptr for an absent key.
TEST(ObjectStorage, find_returns_nullptr_for_missing_key) {
    JsonObject obj;
    obj.emplace("present", Json(1));

    EXPECT_EQ(obj.find("absent"), nullptr);
}

// Verifies the const-qualified find() overload behaves identically.
TEST(ObjectStorage, find_const_overload) {
    JsonObject obj;
    obj.emplace("k", Json(7));

    const JsonObject& cref = obj;
    const Json* p = cref.find("k");

    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->asNumber(), 7.0);
    EXPECT_EQ(cref.find("missing"), nullptr);
}

// Verifies reserve() does not alter existing contents or lookups.
TEST(ObjectStorage, reserve_preserves_contents) {
    JsonObject obj;
    obj.emplace("k", Json(1));

    obj.reserve(64);

    EXPECT_EQ(obj.size(), 1);
    EXPECT_EQ(obj.find("k")->asNumber(), 1.0);
}

// Verifies size() and empty() reflect the entry count.
TEST(ObjectStorage, size_and_empty) {
    JsonObject obj;
    EXPECT_TRUE(obj.empty());
    EXPECT_EQ(obj.size(), 0);

    obj.emplace("k", Json(1));
    EXPECT_FALSE(obj.empty());
    EXPECT_EQ(obj.size(), 1);
}

// Verifies contains() reports presence/absence correctly.
TEST(ObjectStorage, contains_key) {
    JsonObject obj;
    obj.emplace("present", Json(1));

    EXPECT_TRUE(obj.contains("present"));
    EXPECT_FALSE(obj.contains("absent"));
}

// Verifies erase() removes an existing key and reports 1.
TEST(ObjectStorage, erase_existing_key) {
    JsonObject obj;
    obj.emplace("k", Json(1));

    std::size_t removed = obj.erase("k");

    EXPECT_EQ(removed, 1);
    EXPECT_EQ(obj.size(), 0);
    EXPECT_FALSE(obj.contains("k"));
}

// Verifies erase() on a missing key reports 0 and leaves the object unchanged.
TEST(ObjectStorage, erase_missing_key) {
    JsonObject obj;
    obj.emplace("k", Json(1));

    std::size_t removed = obj.erase("absent");

    EXPECT_EQ(removed, 0);
    EXPECT_EQ(obj.size(), 1);
}

// Verifies erasing a middle entry preserves the relative order of the rest.
TEST(ObjectStorage, erase_preserves_remaining_order) {
    JsonObject obj;
    obj.emplace("a", Json(1));
    obj.emplace("b", Json(2));
    obj.emplace("c", Json(3));

    obj.erase("b");

    ASSERT_EQ(obj.size(), 2);
    EXPECT_EQ(obj.entries()[0].first, "a");
    EXPECT_EQ(obj.entries()[1].first, "c");
    EXPECT_EQ(obj.find("c")->asNumber(), 3.0); // still reachable after reindex
}

// Verifies entries() reflects insertion order, not key order.
TEST(ObjectStorage, entries_reflects_insertion_order) {
    JsonObject obj;
    obj.emplace("z", Json(1));
    obj.emplace("a", Json(2));
    obj.emplace("m", Json(3));

    const auto& entries = obj.entries();
    ASSERT_EQ(entries.size(), 3);
    EXPECT_EQ(entries[0].first, "z");
    EXPECT_EQ(entries[1].first, "a");
    EXPECT_EQ(entries[2].first, "m");
}

// Verifies operator== compares by key/value regardless of insertion order.
TEST(ObjectStorage, equality_is_order_independent) {
    JsonObject a;
    a.emplace("x", Json(1));
    a.emplace("y", Json(2));

    JsonObject b;
    b.emplace("y", Json(2));
    b.emplace("x", Json(1));

    EXPECT_EQ(a, b);
    EXPECT_FALSE(a != b);
}

// Verifies operator!= detects differing sizes and differing values.
TEST(ObjectStorage, inequality_detects_differences) {
    JsonObject a;
    a.emplace("x", Json(1));

    JsonObject b;
    b.emplace("x", Json(1));
    b.emplace("y", Json(2));

    JsonObject c;
    c.emplace("x", Json(999));

    EXPECT_NE(a, b); // different size
    EXPECT_NE(a, c); // same key, different value
}
