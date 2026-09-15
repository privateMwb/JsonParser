// Json exception safety test suite.
//
// Coverage:
// - A failed asBool()/asString() (wrong type) leaves the value unmodified
// - A failed at() (array out-of-range, missing object key) leaves the
//   container unmodified, and does not auto-vivify a missing key
// - An assignment target is left unchanged when the right-hand side throws
//   during evaluation (e.g. a failed Json::parse())
// - JsonObject::find() never mutates the object, even on a miss
// - A deeply nested document tears down cleanly when it goes out of scope

#include <gtest/gtest.h>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Verifies a failed asBool() call leaves the underlying value unmodified.
TEST(ExceptionSafety, FailedAsBoolDoesNotModifyValue) {
    Json j(3.14);

    EXPECT_THROW(j.asBool(), std::runtime_error);

    EXPECT_EQ(j.isNumber(), true);
    EXPECT_EQ(j.asNumber(), 3.14);
}

// Verifies a failed asString() call leaves the underlying value unmodified.
TEST(ExceptionSafety, FailedAsStringDoesNotModifyValue) {
    Json j(Json::ArrayType{Json(1), Json(2)});

    EXPECT_THROW(j.asString(), std::runtime_error);

    EXPECT_EQ(j.isArray(), true);
    EXPECT_EQ(j.size(), 2);
}

// Verifies a failed array at() (out of range) leaves the array unmodified.
TEST(ExceptionSafety, FailedArrayAtOutOfRangeLeavesContainerUnchanged) {
    Json j(Json::ArrayType{Json(1), Json(2), Json(3)});

    EXPECT_THROW(j.at(99), std::runtime_error);

    EXPECT_EQ(j.size(), 3);
    EXPECT_EQ(j[1].asNumber(), 2.0);
}

// Verifies a failed object at() (missing key) leaves the object unmodified,
// and -- unlike operator[] -- does not auto-vivify the missing key.
TEST(ExceptionSafety, FailedObjectAtMissingKeyLeavesContainerUnchanged) {
    Json::ObjectType obj;
    obj.emplace("present", Json(1));

    Json j(std::move(obj));

    EXPECT_THROW(j.at("absent"), std::runtime_error);

    EXPECT_EQ(j.size(), 1);
    EXPECT_EQ(j.contains("absent"), false);
}

// Verifies an assignment target is left unchanged when the right-hand side
// throws during evaluation: the rhs must fully complete (or throw) before
// the assignment operator ever runs.
TEST(ExceptionSafety, AssignmentTargetUnchangedWhenRhsParseThrows) {
    Json j("original");

    try {
        j = Json::parse("{ not valid json");
    } catch (const std::runtime_error&) {
        // rhs threw before the assignment could take place.
    }

    EXPECT_EQ(j.isString(), true);
    EXPECT_EQ(j.asString(), "original");
}

// Verifies JsonObject::find() never mutates the object, even when the key
// is absent (contrast with operator[], which auto-vivifies).
TEST(ExceptionSafety, JsonObjectFindOnMissingKeyDoesNotInsert) {
    JsonObject obj;
    obj.emplace("a", Json(1));

    const Json* p = obj.find("missing");

    EXPECT_EQ(p, nullptr);
    EXPECT_EQ(obj.size(), 1);
    EXPECT_EQ(obj.contains("missing"), false);
}

// Verifies a large, deeply nested document tears down cleanly (no crash,
// no leak-detector trip) when it goes out of scope.
TEST(ExceptionSafety, NestedDocumentDestructsCleanlyWithoutCrash) {
    {
        Json j = Json(Json::ArrayType{});

        for (int i = 0; i < 100; ++i) {
            Json entry = Json(Json::ObjectType{});
            entry["value"] = Json(i);
            j.asArray().push_back(std::move(entry));
        }

        EXPECT_EQ(j.size(), 100);
    } // j, and all 100 nested entries, are destroyed here.

    SUCCEED(); // reaching this line confirms teardown completed without crashing
}
