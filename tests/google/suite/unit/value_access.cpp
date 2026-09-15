// Json value access test suite.
//
// Coverage:
// - asBool / asNumber / asString return correct underlying values
// - asArray / asObject (mutable and const) return correct underlying containers
// - Mutable asArray / asObject allow in-place modification
// - Each accessor throws std::runtime_error when called on the wrong type

#include <JsonPro/Json.h>

#include <gtest/gtest.h>

using namespace JsonPro;

// Verifies asBool() returns the stored value.
TEST(ValueAccess, as_bool_returns_value) {
    Json j(true);
    EXPECT_TRUE(j.asBool());
}

// Verifies asNumber() returns the stored value.
TEST(ValueAccess, as_number_returns_value) {
    Json j(2.5);
    EXPECT_EQ(j.asNumber(), 2.5);
}

// Verifies asString() returns the stored value.
TEST(ValueAccess, as_string_returns_value) {
    Json j("payload");
    EXPECT_EQ(j.asString(), "payload");
}

// Verifies mutable asArray() exposes the underlying container by reference.
TEST(ValueAccess, as_array_mutable_reference) {
    Json j(Json::ArrayType{Json(1)});

    j.asArray().emplace_back(2);

    EXPECT_EQ(j.size(), 2);
}

// Verifies const asArray() returns the correct underlying container.
TEST(ValueAccess, as_array_const_reference) {
    const Json j(Json::ArrayType{Json(1), Json(2), Json(3)});
    EXPECT_EQ(j.asArray().size(), 3);
}

// Verifies mutable asObject() exposes the underlying container by reference.
TEST(ValueAccess, as_object_mutable_reference) {
    Json::ObjectType obj;
    obj.emplace("a", Json(1));

    Json j(std::move(obj));
    j.asObject().emplace("b", Json(2));

    EXPECT_EQ(j.size(), 2);
}

// Verifies const asObject() returns the correct underlying container.
TEST(ValueAccess, as_object_const_reference) {
    Json::ObjectType obj;
    obj.emplace("k", Json(1));

    const Json j(std::move(obj));
    EXPECT_EQ(j.asObject().size(), 1);
}

// Verifies asBool() throws when the value is not a Bool.
TEST(ValueAccess, as_bool_throws_on_wrong_type) {
    Json j(1.0);
    EXPECT_THROW(j.asBool(), std::runtime_error);
}

// Verifies asNumber() throws when the value is not a Number.
TEST(ValueAccess, as_number_throws_on_wrong_type) {
    Json j("not a number");
    EXPECT_THROW(j.asNumber(), std::runtime_error);
}

// Verifies asString() throws when the value is not a String.
TEST(ValueAccess, as_string_throws_on_wrong_type) {
    Json j(true);
    EXPECT_THROW(j.asString(), std::runtime_error);
}

// Verifies asArray() throws when the value is not an Array.
TEST(ValueAccess, as_array_throws_on_wrong_type) {
    Json j(1.0);
    EXPECT_THROW(j.asArray(), std::runtime_error);
}

// Verifies asObject() throws when the value is not an Object.
TEST(ValueAccess, as_object_throws_on_wrong_type) {
    Json j(Json::ArrayType{});
    EXPECT_THROW(j.asObject(), std::runtime_error);
}

// Verifies const-qualified accessors also throw on wrong type.
TEST(ValueAccess, const_accessors_throw_on_wrong_type) {
    const Json j(nullptr);
    EXPECT_THROW(j.asString(), std::runtime_error);
    EXPECT_THROW(j.asArray(), std::runtime_error);
    EXPECT_THROW(j.asObject(), std::runtime_error);
}
