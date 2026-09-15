// Json move semantics test suite.
//
// Coverage:
// - Move construction transfers contents and leaves the source Null
// - Move assignment overwrites the target and leaves the source Null
// - Self move assignment leaves the value unchanged
// - A moved-from value is safely reassignable afterward
// - Move construction and move assignment are noexcept

#include <type_traits>

#include <gtest/gtest.h>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Verifies that move construction leaves the source as Null.
TEST(MoveSemantics, MoveConstructionLeavesSourceNull) {
    Json source("movable");
    Json moved(std::move(source));

    EXPECT_EQ(moved.isString(), true);
    EXPECT_EQ(moved.asString(), "movable");

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    EXPECT_EQ(source.isNull(), true);
}

// Verifies move construction correctly transfers Object contents.
TEST(MoveSemantics, MoveConstructionTransfersObjectContents) {
    Json::ObjectType obj;
    obj.emplace("a", Json(1));
    obj.emplace("b", Json(2));

    Json source(std::move(obj));
    Json moved(std::move(source));

    EXPECT_EQ(moved.isObject(), true);
    EXPECT_EQ(moved.size(), 2);

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    EXPECT_EQ(source.isNull(), true);
}

// Verifies that move assignment overwrites the target and nulls the source.
TEST(MoveSemantics, MoveAssignmentOverwritesTarget) {
    Json source(Json::ArrayType{Json(1), Json(2), Json(3)});
    Json target("old value");

    target = std::move(source);

    EXPECT_EQ(target.isArray(), true);
    EXPECT_EQ(target.size(), 3);

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    EXPECT_EQ(source.isNull(), true);
}

// Verifies that self move assignment leaves the value unchanged.
TEST(MoveSemantics, SelfMoveAssignment) {
    Json j("stable");
    j = std::move(j);

    EXPECT_EQ(j.isString(), true);
    EXPECT_EQ(j.asString(), "stable");
}

// Verifies a moved-from Json is left in a valid state that can be safely reassigned.
TEST(MoveSemantics, MovedFromValueIsSafelyReassignable) {
    Json source("original");
    Json moved(std::move(source));

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    EXPECT_EQ(source.isNull(), true);

    source = Json("reused");
    EXPECT_EQ(source.isString(), true);
    EXPECT_EQ(source.asString(), "reused");
}

// Verifies move construction and move assignment are declared noexcept, so
// containers of Json (e.g. ArrayType) move rather than copy on reallocation.
TEST(MoveSemantics, MoveOperationsAreNoexcept) {
    static_assert(std::is_nothrow_move_constructible<Json>::value,
                  "Json move construction must be noexcept");
    static_assert(std::is_nothrow_move_assignable<Json>::value,
                  "Json move assignment must be noexcept");

    SUCCEED();
}
