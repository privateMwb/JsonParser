// Json copy semantics test suite.
//
// Coverage:
// - Copy construction produces an independent deep copy (Array and Object)
// - Copy assignment replaces the target's contents without disturbing the source
// - Self copy assignment leaves the value unchanged
// - Copies of nested (mixed Array/Object) structures are fully independent

#include <gtest/gtest.h>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Verifies that copy construction produces an independent deep copy of an Array.
TEST(CopySemantics, CopyConstructorDeepCopyArray) {
    Json original(Json::ArrayType{Json(1), Json(2)});
    Json copy(original);

    copy.asArray().emplace_back(3);

    EXPECT_EQ(original.size(), 2);
    EXPECT_EQ(copy.size(), 3);
}

// Verifies that copy construction produces an independent deep copy of an Object.
TEST(CopySemantics, CopyConstructorDeepCopyObject) {
    Json::ObjectType obj;
    obj.emplace("a", Json(1));

    Json original(std::move(obj));
    Json copy(original);

    copy.asObject().emplace("b", Json(2));

    EXPECT_EQ(original.size(), 1);
    EXPECT_EQ(copy.size(), 2);
}

// Verifies copy assignment replaces the target's contents.
TEST(CopySemantics, CopyAssignment) {
    Json source("copied");
    Json target(123);

    target = source;

    EXPECT_EQ(target.isString(), true);
    EXPECT_EQ(target.asString(), "copied");
    EXPECT_EQ(source.asString(), "copied"); // source untouched
}

// Verifies self copy assignment leaves the value unchanged.
TEST(CopySemantics, SelfCopyAssignmentLeavesValueUnchanged) {
    Json j("stable");
    j = j;

    EXPECT_EQ(j.isString(), true);
    EXPECT_EQ(j.asString(), "stable");
}

// Verifies a copy of a nested Array-in-Object structure is fully independent
// of the original at every level.
TEST(CopySemantics, CopyOfNestedStructureIsIndependent) {
    Json::ObjectType obj;
    obj.emplace("nums", Json(Json::ArrayType{Json(1), Json(2)}));

    Json original(std::move(obj));
    Json copy(original);

    copy["nums"].asArray().emplace_back(Json(3));

    EXPECT_EQ(original["nums"].size(), 2);
    EXPECT_EQ(copy["nums"].size(), 3);
}
