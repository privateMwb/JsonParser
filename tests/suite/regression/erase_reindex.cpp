// JsonObject erase-reindex regression test suite.
//
// JsonObject stores entries in an insertion-ordered vector plus a side
// unordered_map from key to vector index. erase() must remove the vector
// entry AND fix up every index in the map that pointed past the removed
// slot -- guards against a class of bug where find()/contains() silently
// desyncs from the vector after an erase.
//
// Coverage:
// - Erasing the first entry leaves every remaining key still findable
// - Erasing the last entry requires no reindexing and remains consistent
// - Erasing a middle entry, then inserting a new one, appends correctly
//   rather than colliding with a stale index
// - Erasing every entry one at a time empties the object and leaves it reusable
// - Erasing the same key twice is a no-op the second time
// - A longer interleaved erase/insert/find sequence stays internally consistent

#include <common/framework.h>

using namespace JsonPro;

// Verifies erasing the first entry leaves all remaining keys correctly findable.
static void erase_first_entry_keeps_remainder_findable() {
    JsonObject obj;
    obj.emplace("a", Json(1));
    obj.emplace("b", Json(2));
    obj.emplace("c", Json(3));

    obj.erase("a");

    CHK(obj.size() == 2);
    CHK(obj.find("b")->asNumber() == 2.0);
    CHK(obj.find("c")->asNumber() == 3.0);
    CHK(obj.find("a") == nullptr);
}

// Verifies erasing the last entry (no reindexing required) stays consistent.
static void erase_last_entry_stays_consistent() {
    JsonObject obj;
    obj.emplace("a", Json(1));
    obj.emplace("b", Json(2));
    obj.emplace("c", Json(3));

    obj.erase("c");

    CHK(obj.size() == 2);
    CHK(obj.find("a")->asNumber() == 1.0);
    CHK(obj.find("b")->asNumber() == 2.0);
    CHK(obj.find("c") == nullptr);
}

// Verifies erasing a middle entry, then inserting a new key, appends the
// new entry correctly rather than colliding with a stale index left behind
// by the erase.
static void erase_middle_then_insert_appends_correctly() {
    JsonObject obj;
    obj.emplace("a", Json(1));
    obj.emplace("b", Json(2));
    obj.emplace("c", Json(3));

    obj.erase("b");
    obj.emplace("d", Json(4));

    CHK(obj.size() == 3);
    CHK(obj.entries()[0].first == "a");
    CHK(obj.entries()[1].first == "c");
    CHK(obj.entries()[2].first == "d");
    CHK(obj.find("a")->asNumber() == 1.0);
    CHK(obj.find("c")->asNumber() == 3.0);
    CHK(obj.find("d")->asNumber() == 4.0);
}

// Verifies erasing every entry one at a time empties the object and leaves
// it in a fully reusable state.
static void erase_all_entries_leaves_object_reusable() {
    JsonObject obj;
    obj.emplace("a", Json(1));
    obj.emplace("b", Json(2));
    obj.emplace("c", Json(3));

    obj.erase("a");
    obj.erase("b");
    obj.erase("c");

    CHK(obj.empty() == true);
    CHK(obj.size() == 0);

    obj.emplace("fresh", Json(99));
    CHK(obj.size() == 1);
    CHK(obj.find("fresh")->asNumber() == 99.0);
}

// Verifies erasing the same key a second time is a safe no-op.
static void erase_same_key_twice_is_no_op_second_time() {
    JsonObject obj;
    obj.emplace("a", Json(1));
    obj.emplace("b", Json(2));

    CHK(obj.erase("a") == 1);
    CHK(obj.erase("a") == 0);

    CHK(obj.size() == 1);
    CHK(obj.find("b")->asNumber() == 2.0);
}

// Verifies a longer interleaved sequence of erase/insert/find operations
// leaves the vector and index map mutually consistent throughout.
static void interleaved_erase_insert_find_stays_consistent() {
    JsonObject obj;
    obj.emplace("a", Json(1));
    obj.emplace("b", Json(2));
    obj.emplace("c", Json(3));
    obj.emplace("d", Json(4));

    obj.erase("b");            // a, c, d
    obj.emplace("e", Json(5)); // a, c, d, e
    obj.erase("a");            // c, d, e
    obj.emplace("f", Json(6)); // c, d, e, f
    obj.erase("d");            // c, e, f

    CHK(obj.size() == 3);
    CHK(obj.entries()[0].first == "c");
    CHK(obj.entries()[1].first == "e");
    CHK(obj.entries()[2].first == "f");
    CHK(obj.find("c")->asNumber() == 3.0);
    CHK(obj.find("e")->asNumber() == 5.0);
    CHK(obj.find("f")->asNumber() == 6.0);
    CHK(obj.find("a") == nullptr);
    CHK(obj.find("b") == nullptr);
    CHK(obj.find("d") == nullptr);
}

// Executes all erase-reindex regression test cases.
static void run_tests() {
    RUN(erase_first_entry_keeps_remainder_findable);
    RUN(erase_last_entry_stays_consistent);
    RUN(erase_middle_then_insert_appends_correctly);
    RUN(erase_all_entries_leaves_object_reusable);
    RUN(erase_same_key_twice_is_no_op_second_time);
    RUN(interleaved_erase_insert_find_stays_consistent);
}

REGISTER_TEST_SUITE();
