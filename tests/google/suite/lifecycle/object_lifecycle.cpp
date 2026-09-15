// JsonObject lifecycle test suite.
//
// Coverage:
// - Default construction produces an empty object
// - Copy construction produces an independent deep copy
// - Move construction transfers contents and leaves the source empty
// - Copy assignment replaces the target's contents without disturbing the source
// - Move assignment replaces the target's contents and empties the source
// - Self copy assignment leaves contents unchanged
// - Self move assignment leaves the object in a valid, reusable state
//
// JsonObject stores entries_ (std::vector) as an incomplete Json type in its
// header, so every special member is declared there but defined out-of-line
// in JsonObject.cpp as `= default`, once Json.h has made Json complete.

#include <gtest/gtest.h>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Verifies default construction produces an empty object.
TEST(ObjectLifecycle, DefaultConstructionIsEmpty) {
    JsonObject obj;
    EXPECT_EQ(obj.empty(), true);
    EXPECT_EQ(obj.size(), 0);
}

// Verifies copy construction produces an independent deep copy.
TEST(ObjectLifecycle, CopyConstructorDeepCopy) {
    JsonObject original;
    original.emplace("a", Json(1));

    JsonObject copy(original);
    copy.emplace("b", Json(2));

    EXPECT_EQ(original.size(), 1);
    EXPECT_EQ(copy.size(), 2);
}

// Verifies move construction transfers contents and leaves the source empty,
// per the documented "left empty" contract.
TEST(ObjectLifecycle, MoveConstructorLeavesSourceEmpty) {
    JsonObject source;
    source.emplace("k", Json(1));

    JsonObject moved(std::move(source));

    EXPECT_EQ(moved.size(), 1);
    EXPECT_EQ(moved.find("k")->asNumber(), 1.0);

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    EXPECT_EQ(source.size(), 0);
}

// Verifies copy assignment replaces the target's contents without disturbing the source.
TEST(ObjectLifecycle, CopyAssignmentReplacesContents) {
    JsonObject source;
    source.emplace("x", Json(1));

    JsonObject target;
    target.emplace("old", Json(0));

    target = source;

    EXPECT_EQ(target.size(), 1);
    EXPECT_EQ(target.find("x")->asNumber(), 1.0);
    EXPECT_EQ(source.size(), 1); // source untouched
}

// Verifies move assignment replaces the target's contents and empties the source.
TEST(ObjectLifecycle, MoveAssignmentReplacesContentsAndEmptiesSource) {
    JsonObject source;
    source.emplace("x", Json(1));

    JsonObject target;
    target.emplace("old", Json(0));

    target = std::move(source);

    EXPECT_EQ(target.size(), 1);
    EXPECT_EQ(target.find("x")->asNumber(), 1.0);

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    EXPECT_EQ(source.size(), 0);
}

// Verifies self copy assignment leaves contents unchanged.
TEST(ObjectLifecycle, SelfCopyAssignmentLeavesContentsUnchanged) {
    JsonObject obj;
    obj.emplace("k", Json(1));

    obj = obj;

    EXPECT_EQ(obj.size(), 1);
    EXPECT_EQ(obj.find("k")->asNumber(), 1.0);
}

// Verifies self move assignment does not crash and leaves the object in a
// valid, reusable state. Unlike Json::operator=(Json&&), JsonObject's move
// assignment is defaulted with no explicit self-check, so only a valid
// post-state is guaranteed -- not that prior contents survive.
TEST(ObjectLifecycle, SelfMoveAssignmentLeavesObjectInValidState) {
    JsonObject obj;
    obj.emplace("k", Json(1));

    obj = std::move(obj);

    obj.emplace("after", Json(2));
    EXPECT_EQ(obj.contains("after"), true);
}
