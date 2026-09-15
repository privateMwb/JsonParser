// Json navigation test suite.
//
// Coverage:
// - operator[](size_t) on arrays (mutable and const)
// - operator[](const std::string&) on objects (mutable and const)
// - at(size_t) bounds-checked array access
// - at(const std::string&) bounds-checked object access
// - Wrong-type access throws std::runtime_error
// - Out-of-range / missing-key access throws std::runtime_error (JsonOutOfRange)

#include <JsonPro/Json.h>

#include <gtest/gtest.h>

using namespace JsonPro;

// Verifies mutable operator[](size_t) reads and writes array elements.
TEST(Navigation, array_index_operator_mutable) {
    Json j(Json::ArrayType{Json(1), Json(2), Json(3)});

    EXPECT_EQ(j[1].asNumber(), 2.0);

    j[1] = Json(99);
    EXPECT_EQ(j[1].asNumber(), 99.0);
}

// Verifies const operator[](size_t) reads array elements.
TEST(Navigation, array_index_operator_const) {
    const Json j(Json::ArrayType{Json(10), Json(20)});
    EXPECT_EQ(j[0].asNumber(), 10.0);
}

// Verifies operator[](size_t) throws on a non-array value.
TEST(Navigation, array_index_operator_throws_on_wrong_type) {
    Json j(1.0);
    EXPECT_THROW(j[0], std::runtime_error);
}

// Verifies mutable operator[](key) reads and writes object values,
// creating the key if absent.
TEST(Navigation, object_index_operator_mutable) {
    Json::ObjectType obj;
    obj.emplace("x", Json(1));

    Json j(std::move(obj));

    EXPECT_EQ(j["x"].asNumber(), 1.0);

    j["y"] = Json(2);
    EXPECT_EQ(j["y"].asNumber(), 2.0);
    EXPECT_TRUE(j.contains("y"));
}

// Verifies const operator[](key) reads existing object values.
TEST(Navigation, object_index_operator_const) {
    Json::ObjectType obj;
    obj.emplace("k", Json("v"));

    const Json j(std::move(obj));
    EXPECT_EQ(j["k"].asString(), "v");
}

// Verifies operator[](key) throws on a non-object value.
TEST(Navigation, object_index_operator_throws_on_wrong_type) {
    Json j("not an object");
    EXPECT_THROW(j["k"], std::runtime_error);
}

// Verifies at(size_t) returns the correct element within bounds.
TEST(Navigation, array_at_in_bounds) {
    Json j(Json::ArrayType{Json(5), Json(6)});
    EXPECT_EQ(j.at(1).asNumber(), 6.0);
}

// Verifies at(size_t) throws std::runtime_error (JsonOutOfRange) past the end.
TEST(Navigation, array_at_out_of_range) {
    Json j(Json::ArrayType{Json(1)});
    EXPECT_THROW(j.at(5), std::runtime_error);
}

// Verifies at(size_t) throws std::runtime_error on a non-array value.
TEST(Navigation, array_at_throws_on_wrong_type) {
    Json j(true);
    EXPECT_THROW(j.at(std::size_t{0}), std::runtime_error);
}

// Verifies at(key) returns the correct value for an existing key.
TEST(Navigation, object_at_existing_key) {
    Json::ObjectType obj;
    obj.emplace("name", Json("Rain"));

    Json j(std::move(obj));
    EXPECT_EQ(j.at("name").asString(), "Rain");
}

// Verifies at(key) throws std::runtime_error (JsonOutOfRange) for a missing key.
TEST(Navigation, object_at_missing_key) {
    Json::ObjectType obj;
    obj.emplace("present", Json(1));

    Json j(std::move(obj));
    EXPECT_THROW(j.at("absent"), std::runtime_error);
}

// Verifies at(key) throws std::runtime_error on a non-object value.
TEST(Navigation, object_at_throws_on_wrong_type) {
    Json j(Json::ArrayType{});
    EXPECT_THROW(j.at(std::string{"k"}), std::runtime_error);
}

// Verifies const-qualified at() overloads behave identically.
TEST(Navigation, const_at_overloads) {
    const Json arr(Json::ArrayType{Json(1), Json(2)});
    EXPECT_EQ(arr.at(0).asNumber(), 1.0);
    EXPECT_THROW(arr.at(9), std::runtime_error);

    Json::ObjectType obj;
    obj.emplace("k", Json(7));
    const Json object(std::move(obj));
    EXPECT_EQ(object.at("k").asNumber(), 7.0);
    EXPECT_THROW(object.at("missing"), std::runtime_error);
}
