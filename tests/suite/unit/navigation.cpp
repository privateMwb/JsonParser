// Json navigation test suite.
//
// Coverage:
// - operator[](size_t) on arrays (mutable and const)
// - operator[](const std::string&) on objects (mutable and const)
// - at(size_t) bounds-checked array access
// - at(const std::string&) bounds-checked object access
// - Wrong-type access throws std::runtime_error
// - Out-of-range / missing-key access throws std::runtime_error (JsonOutOfRange)

#include <common/framework.h>

using namespace JsonPro;

// Verifies mutable operator[](size_t) reads and writes array elements.
static void array_index_operator_mutable() {
    Json j(Json::ArrayType{Json(1), Json(2), Json(3)});

    CHK(j[1].asNumber() == 2.0);

    j[1] = Json(99);
    CHK(j[1].asNumber() == 99.0);
}

// Verifies const operator[](size_t) reads array elements.
static void array_index_operator_const() {
    const Json j(Json::ArrayType{Json(10), Json(20)});
    CHK(j[0].asNumber() == 10.0);
}

// Verifies operator[](size_t) throws on a non-array value.
static void array_index_operator_throws_on_wrong_type() {
    Json j(1.0);
    CHK_THROWS(j[0], std::runtime_error);
}

// Verifies mutable operator[](key) reads and writes object values,
// creating the key if absent.
static void object_index_operator_mutable() {
    Json::ObjectType obj;
    obj.emplace("x", Json(1));

    Json j(std::move(obj));

    CHK(j["x"].asNumber() == 1.0);

    j["y"] = Json(2);
    CHK(j["y"].asNumber() == 2.0);
    CHK(j.contains("y") == true);
}

// Verifies const operator[](key) reads existing object values.
static void object_index_operator_const() {
    Json::ObjectType obj;
    obj.emplace("k", Json("v"));

    const Json j(std::move(obj));
    CHK(j["k"].asString() == "v");
}

// Verifies operator[](key) throws on a non-object value.
static void object_index_operator_throws_on_wrong_type() {
    Json j("not an object");
    CHK_THROWS(j["k"], std::runtime_error);
}

// Verifies at(size_t) returns the correct element within bounds.
static void array_at_in_bounds() {
    Json j(Json::ArrayType{Json(5), Json(6)});
    CHK(j.at(1).asNumber() == 6.0);
}

// Verifies at(size_t) throws std::runtime_error (JsonOutOfRange) past the end.
static void array_at_out_of_range() {
    Json j(Json::ArrayType{Json(1)});
    CHK_THROWS(j.at(5), std::runtime_error);
}

// Verifies at(size_t) throws std::runtime_error on a non-array value.
static void array_at_throws_on_wrong_type() {
    Json j(true);
    CHK_THROWS(j.at(std::size_t{0}), std::runtime_error);
}

// Verifies at(key) returns the correct value for an existing key.
static void object_at_existing_key() {
    Json::ObjectType obj;
    obj.emplace("name", Json("Rain"));

    Json j(std::move(obj));
    CHK(j.at("name").asString() == "Rain");
}

// Verifies at(key) throws std::runtime_error (JsonOutOfRange) for a missing key.
static void object_at_missing_key() {
    Json::ObjectType obj;
    obj.emplace("present", Json(1));

    Json j(std::move(obj));
    CHK_THROWS(j.at("absent"), std::runtime_error);
}

// Verifies at(key) throws std::runtime_error on a non-object value.
static void object_at_throws_on_wrong_type() {
    Json j(Json::ArrayType{});
    CHK_THROWS(j.at(std::string{"k"}), std::runtime_error);
}

// Verifies const-qualified at() overloads behave identically.
static void const_at_overloads() {
    const Json arr(Json::ArrayType{Json(1), Json(2)});
    CHK(arr.at(0).asNumber() == 1.0);
    CHK_THROWS(arr.at(9), std::runtime_error);

    Json::ObjectType obj;
    obj.emplace("k", Json(7));
    const Json object(std::move(obj));
    CHK(object.at("k").asNumber() == 7.0);
    CHK_THROWS(object.at("missing"), std::runtime_error);
}

// Executes all navigation test cases.
static void run_tests() {
    RUN(array_index_operator_mutable);
    RUN(array_index_operator_const);
    RUN(array_index_operator_throws_on_wrong_type);
    RUN(object_index_operator_mutable);
    RUN(object_index_operator_const);
    RUN(object_index_operator_throws_on_wrong_type);
    RUN(array_at_in_bounds);
    RUN(array_at_out_of_range);
    RUN(array_at_throws_on_wrong_type);
    RUN(object_at_existing_key);
    RUN(object_at_missing_key);
    RUN(object_at_throws_on_wrong_type);
    RUN(const_at_overloads);
}

REGISTER_TEST_SUITE();
