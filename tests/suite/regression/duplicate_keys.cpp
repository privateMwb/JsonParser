// Duplicate object key regression test suite.
//
// Guards against pitfalls in how the parser folds repeated keys into a
// JsonObject: the *value* always comes from the last occurrence, but the
// key's *position* is fixed at its first occurrence (JsonObject::insert_or_
// assign updates in place rather than moving the entry to the end).
//
// Coverage:
// - Last value wins when the value type changes between occurrences
// - Last value wins across three or more repetitions of the same key
// - A repeated key keeps its original insertion position, not the position
//   of its last occurrence
// - Duplicate keys interleaved with distinct keys preserve everyone's
//   first-seen order

#include <common/framework.h>

using namespace JsonPro;

// Verifies the last occurrence wins even when its type differs from earlier ones.
static void last_value_wins_across_differing_types() {
    Json j = Json::parse(R"({"k": 1, "k": "text", "k": true})");

    CHK(j.size() == 1);
    CHK(j["k"].isBool() == true);
    CHK(j["k"].asBool() == true);
}

// Verifies the last of three or more repetitions of the same key wins.
static void last_value_wins_across_many_repetitions() {
    Json j = Json::parse(R"({"k": 1, "k": 2, "k": 3, "k": 4, "k": 5})");

    CHK(j.size() == 1);
    CHK(j["k"].asNumber() == 5.0);
}

// Verifies a repeated key retains its original position rather than moving
// to where its last occurrence was written.
static void repeated_key_keeps_original_position() {
    Json j = Json::parse(R"({"a": 1, "b": 2, "a": 99, "c": 3})");

    CHK(j.size() == 3);
    CHK(j.asObject().entries()[0].first == "a");
    CHK(j.asObject().entries()[1].first == "b");
    CHK(j.asObject().entries()[2].first == "c");
    CHK(j["a"].asNumber() == 99.0); // value still updated
}

// Verifies duplicate keys interleaved with distinct keys preserve every
// key's first-seen order.
static void interleaved_duplicates_preserve_first_seen_order() {
    Json j = Json::parse(R"({"x": 1, "y": 2, "x": 3, "z": 4, "y": 5})");

    const auto& entries = j.asObject().entries();
    CHK(entries.size() == 3);
    CHK(entries[0].first == "x");
    CHK(entries[1].first == "y");
    CHK(entries[2].first == "z");
    CHK(j["x"].asNumber() == 3.0);
    CHK(j["y"].asNumber() == 5.0);
    CHK(j["z"].asNumber() == 4.0);
}

// Executes all duplicate object key regression test cases.
static void run_tests() {
    RUN(last_value_wins_across_differing_types);
    RUN(last_value_wins_across_many_repetitions);
    RUN(repeated_key_keeps_original_position);
    RUN(interleaved_duplicates_preserve_first_seen_order);
}

REGISTER_TEST_SUITE();
