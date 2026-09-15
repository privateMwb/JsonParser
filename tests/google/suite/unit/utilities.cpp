// Json utilities test suite.
//
// Coverage:
// - size() on arrays, objects, and non-container types
// - contains() on objects, including absent keys and non-object types

#include <JsonPro/Json.h>

#include <gtest/gtest.h>

using namespace JsonPro;

// Verifies size() returns the element count of an array.
TEST(Utilities, size_of_array) {
    Json j(Json::ArrayType{Json(1), Json(2), Json(3)});
    EXPECT_EQ(j.size(), 3);
}

// Verifies size() returns 0 for an empty array.
TEST(Utilities, size_of_empty_array) {
    Json j(Json::ArrayType{});
    EXPECT_EQ(j.size(), 0);
}

// Verifies size() returns the key count of an object.
TEST(Utilities, size_of_object) {
    Json::ObjectType obj;
    obj.emplace("a", Json(1));
    obj.emplace("b", Json(2));

    Json j(std::move(obj));
    EXPECT_EQ(j.size(), 2);
}

// Verifies size() returns 0 for an empty object.
TEST(Utilities, size_of_empty_object) {
    Json j(Json::ObjectType{});
    EXPECT_EQ(j.size(), 0);
}

// Verifies size() returns 0 for non-container types (Null, Bool, Number, String).
TEST(Utilities, size_of_non_container_types) {
    EXPECT_EQ(Json().size(), 0);
    EXPECT_EQ(Json(true).size(), 0);
    EXPECT_EQ(Json(1.0).size(), 0);
    EXPECT_EQ(Json("text").size(), 0);
}

// Verifies contains() returns true for a present key.
TEST(Utilities, contains_existing_key) {
    Json::ObjectType obj;
    obj.emplace("present", Json(1));

    Json j(std::move(obj));
    EXPECT_TRUE(j.contains("present"));
}

// Verifies contains() returns false for an absent key.
TEST(Utilities, contains_missing_key) {
    Json::ObjectType obj;
    obj.emplace("present", Json(1));

    Json j(std::move(obj));
    EXPECT_FALSE(j.contains("absent"));
}

// Verifies contains() returns false (not throws) on non-object types.
TEST(Utilities, contains_on_non_object_returns_false) {
    EXPECT_FALSE(Json().contains("k"));
    EXPECT_FALSE(Json(1.0).contains("k"));
    EXPECT_FALSE(Json("str").contains("k"));
    EXPECT_FALSE(Json(Json::ArrayType{}).contains("k"));
}
