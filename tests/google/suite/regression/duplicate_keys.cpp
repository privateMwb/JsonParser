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

#include <JsonPro/Json.h>

#include <gtest/gtest.h>

using namespace JsonPro;

// Verifies the last occurrence wins even when its type differs from earlier ones.
TEST(DuplicateKeys, last_value_wins_across_differing_types) {
    Json j = Json::parse(R"({"k": 1, "k": "text", "k": true})");

    EXPECT_EQ(j.size(), 1);
    ASSERT_TRUE(j["k"].isBool());
    EXPECT_TRUE(j["k"].asBool());
}

// Verifies the last of three or more repetitions of the same key wins.
TEST(DuplicateKeys, last_value_wins_across_many_repetitions) {
    Json j = Json::parse(R"({"k": 1, "k": 2, "k": 3, "k": 4, "k": 5})");

    EXPECT_EQ(j.size(), 1);
    EXPECT_EQ(j["k"].asNumber(), 5.0);
}

// Verifies a repeated key retains its original position rather than moving
// to where its last occurrence was written.
TEST(DuplicateKeys, repeated_key_keeps_original_position) {
    Json j = Json::parse(R"({"a": 1, "b": 2, "a": 99, "c": 3})");

    ASSERT_EQ(j.size(), 3);
    EXPECT_EQ(j.asObject().entries()[0].first, "a");
    EXPECT_EQ(j.asObject().entries()[1].first, "b");
    EXPECT_EQ(j.asObject().entries()[2].first, "c");
    EXPECT_EQ(j["a"].asNumber(), 99.0); // value still updated
}

// Verifies duplicate keys interleaved with distinct keys preserve every
// key's first-seen order.
TEST(DuplicateKeys, interleaved_duplicates_preserve_first_seen_order) {
    Json j = Json::parse(R"({"x": 1, "y": 2, "x": 3, "z": 4, "y": 5})");

    const auto& entries = j.asObject().entries();
    ASSERT_EQ(entries.size(), 3);
    EXPECT_EQ(entries[0].first, "x");
    EXPECT_EQ(entries[1].first, "y");
    EXPECT_EQ(entries[2].first, "z");
    EXPECT_EQ(j["x"].asNumber(), 3.0);
    EXPECT_EQ(j["y"].asNumber(), 5.0);
    EXPECT_EQ(j["z"].asNumber(), 4.0);
}
