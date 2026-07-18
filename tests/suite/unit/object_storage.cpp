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

#include <common/framework.h>

using namespace JsonPro;

// Verifies operator[](key) inserts a Null entry when the key is absent.
static void index_operator_inserts_default_null() {
    JsonObject obj;
    Json& v = obj["k"];

    CHK(v.isNull() == true);
    CHK(obj.size() == 1);
}

// Verifies operator[](key) returns the existing entry without duplicating it.
static void index_operator_returns_existing_entry() {
    JsonObject obj;
    obj["k"] = Json(1);

    Json& v = obj["k"];
    CHK(v.asNumber() == 1.0);
    CHK(obj.size() == 1);
}

// Verifies insert_or_assign() inserts a new key with the given value.
static void insert_or_assign_new_key() {
    JsonObject obj;
    Json& v = obj.insert_or_assign("k", Json("value"));

    CHK(v.asString() == "value");
    CHK(obj.size() == 1);
    CHK(obj.contains("k") == true);
}

// Verifies insert_or_assign() overwrites an existing key's value in place,
// preserving its original position in entries().
static void insert_or_assign_existing_key_updates_in_place() {
    JsonObject obj;
    obj.insert_or_assign("a", Json(1));
    obj.insert_or_assign("b", Json(2));
    obj.insert_or_assign("a", Json(99));

    CHK(obj.size() == 2);
    CHK(obj.find("a")->asNumber() == 99.0);
    CHK(obj.entries()[0].first == "a"); // position unchanged
}

// Verifies emplace() inserts a new key and returns true.
static void emplace_inserts_new_key() {
    JsonObject obj;
    bool inserted = obj.emplace("k", Json(42));

    CHK(inserted == true);
    CHK(obj.find("k")->asNumber() == 42.0);
}

// Verifies emplace() on an existing key returns false and leaves the value untouched.
static void emplace_existing_key_is_no_op() {
    JsonObject obj;
    obj.emplace("k", Json(1));
    bool inserted = obj.emplace("k", Json(999));

    CHK(inserted == false);
    CHK(obj.find("k")->asNumber() == 1.0);
}

// Verifies mutable find() returns a pointer to the correct value.
static void find_returns_existing_value() {
    JsonObject obj;
    obj.emplace("k", Json("found"));

    Json* p = obj.find("k");
    CHK(p != nullptr);
    CHK(p->asString() == "found");
}

// Verifies find() returns nullptr for an absent key.
static void find_returns_nullptr_for_missing_key() {
    JsonObject obj;
    obj.emplace("present", Json(1));

    CHK(obj.find("absent") == nullptr);
}

// Verifies the const-qualified find() overload behaves identically.
static void find_const_overload() {
    JsonObject obj;
    obj.emplace("k", Json(7));

    const JsonObject& cref = obj;
    const Json* p = cref.find("k");

    CHK(p != nullptr);
    CHK(p->asNumber() == 7.0);
    CHK(cref.find("missing") == nullptr);
}

// Verifies reserve() does not alter existing contents or lookups.
static void reserve_preserves_contents() {
    JsonObject obj;
    obj.emplace("k", Json(1));

    obj.reserve(64);

    CHK(obj.size() == 1);
    CHK(obj.find("k")->asNumber() == 1.0);
}

// Verifies size() and empty() reflect the entry count.
static void size_and_empty() {
    JsonObject obj;
    CHK(obj.empty() == true);
    CHK(obj.size() == 0);

    obj.emplace("k", Json(1));
    CHK(obj.empty() == false);
    CHK(obj.size() == 1);
}

// Verifies contains() reports presence/absence correctly.
static void contains_key() {
    JsonObject obj;
    obj.emplace("present", Json(1));

    CHK(obj.contains("present") == true);
    CHK(obj.contains("absent") == false);
}

// Verifies erase() removes an existing key and reports 1.
static void erase_existing_key() {
    JsonObject obj;
    obj.emplace("k", Json(1));

    std::size_t removed = obj.erase("k");

    CHK(removed == 1);
    CHK(obj.size() == 0);
    CHK(obj.contains("k") == false);
}

// Verifies erase() on a missing key reports 0 and leaves the object unchanged.
static void erase_missing_key() {
    JsonObject obj;
    obj.emplace("k", Json(1));

    std::size_t removed = obj.erase("absent");

    CHK(removed == 0);
    CHK(obj.size() == 1);
}

// Verifies erasing a middle entry preserves the relative order of the rest.
static void erase_preserves_remaining_order() {
    JsonObject obj;
    obj.emplace("a", Json(1));
    obj.emplace("b", Json(2));
    obj.emplace("c", Json(3));

    obj.erase("b");

    CHK(obj.size() == 2);
    CHK(obj.entries()[0].first == "a");
    CHK(obj.entries()[1].first == "c");
    CHK(obj.find("c")->asNumber() == 3.0); // still reachable after reindex
}

// Verifies entries() reflects insertion order, not key order.
static void entries_reflects_insertion_order() {
    JsonObject obj;
    obj.emplace("z", Json(1));
    obj.emplace("a", Json(2));
    obj.emplace("m", Json(3));

    const auto& entries = obj.entries();
    CHK(entries.size() == 3);
    CHK(entries[0].first == "z");
    CHK(entries[1].first == "a");
    CHK(entries[2].first == "m");
}

// Verifies operator== compares by key/value regardless of insertion order.
static void equality_is_order_independent() {
    JsonObject a;
    a.emplace("x", Json(1));
    a.emplace("y", Json(2));

    JsonObject b;
    b.emplace("y", Json(2));
    b.emplace("x", Json(1));

    CHK(a == b);
    CHK(!(a != b));
}

// Verifies operator!= detects differing sizes and differing values.
static void inequality_detects_differences() {
    JsonObject a;
    a.emplace("x", Json(1));

    JsonObject b;
    b.emplace("x", Json(1));
    b.emplace("y", Json(2));

    JsonObject c;
    c.emplace("x", Json(999));

    CHK(a != b); // different size
    CHK(a != c); // same key, different value
}

// Executes all JsonObject storage test cases.
static void run_tests() {
    RUN(index_operator_inserts_default_null);
    RUN(index_operator_returns_existing_entry);
    RUN(insert_or_assign_new_key);
    RUN(insert_or_assign_existing_key_updates_in_place);
    RUN(emplace_inserts_new_key);
    RUN(emplace_existing_key_is_no_op);
    RUN(find_returns_existing_value);
    RUN(find_returns_nullptr_for_missing_key);
    RUN(find_const_overload);
    RUN(reserve_preserves_contents);
    RUN(size_and_empty);
    RUN(contains_key);
    RUN(erase_existing_key);
    RUN(erase_missing_key);
    RUN(erase_preserves_remaining_order);
    RUN(entries_reflects_insertion_order);
    RUN(equality_is_order_independent);
    RUN(inequality_detects_differences);
}

REGISTER_TEST_SUITE();
