// Json value access test suite.
//
// Coverage:
// - asBool / asNumber / asString return correct underlying values
// - asArray / asObject (mutable and const) return correct underlying containers
// - Mutable asArray / asObject allow in-place modification
// - Each accessor throws std::runtime_error when called on the wrong type

#include <common/framework.h>

using namespace JsonPro;

// Verifies asBool() returns the stored value.
static void as_bool_returns_value() {
    Json j(true);
    CHK(j.asBool() == true);
}

// Verifies asNumber() returns the stored value.
static void as_number_returns_value() {
    Json j(2.5);
    CHK(j.asNumber() == 2.5);
}

// Verifies asString() returns the stored value.
static void as_string_returns_value() {
    Json j("payload");
    CHK(j.asString() == "payload");
}

// Verifies mutable asArray() exposes the underlying container by reference.
static void as_array_mutable_reference() {
    Json j(Json::ArrayType{ Json(1) });

    j.asArray().emplace_back(2);

    CHK(j.size() == 2);
}

// Verifies const asArray() returns the correct underlying container.
static void as_array_const_reference() {
    const Json j(Json::ArrayType{ Json(1), Json(2), Json(3) });
    CHK(j.asArray().size() == 3);
}

// Verifies mutable asObject() exposes the underlying container by reference.
static void as_object_mutable_reference() {
    Json::ObjectType obj;
    obj.emplace("a", Json(1));

    Json j(std::move(obj));
    j.asObject().emplace("b", Json(2));

    CHK(j.size() == 2);
}

// Verifies const asObject() returns the correct underlying container.
static void as_object_const_reference() {
    Json::ObjectType obj;
    obj.emplace("k", Json(1));

    const Json j(std::move(obj));
    CHK(j.asObject().size() == 1);
}

// Verifies asBool() throws when the value is not a Bool.
static void as_bool_throws_on_wrong_type() {
    Json j(1.0);
    CHK_THROWS(j.asBool(), std::runtime_error);
}

// Verifies asNumber() throws when the value is not a Number.
static void as_number_throws_on_wrong_type() {
    Json j("not a number");
    CHK_THROWS(j.asNumber(), std::runtime_error);
}

// Verifies asString() throws when the value is not a String.
static void as_string_throws_on_wrong_type() {
    Json j(true);
    CHK_THROWS(j.asString(), std::runtime_error);
}

// Verifies asArray() throws when the value is not an Array.
static void as_array_throws_on_wrong_type() {
    Json j(1.0);
    CHK_THROWS(j.asArray(), std::runtime_error);
}

// Verifies asObject() throws when the value is not an Object.
static void as_object_throws_on_wrong_type() {
    Json j(Json::ArrayType{});
    CHK_THROWS(j.asObject(), std::runtime_error);
}

// Verifies const-qualified accessors also throw on wrong type.
static void const_accessors_throw_on_wrong_type() {
    const Json j(nullptr);
    CHK_THROWS(j.asString(), std::runtime_error);
    CHK_THROWS(j.asArray(),  std::runtime_error);
    CHK_THROWS(j.asObject(), std::runtime_error);
}

// Executes all value access test cases.
static void run_tests() {
    RUN(as_bool_returns_value);
    RUN(as_number_returns_value);
    RUN(as_string_returns_value);
    RUN(as_array_mutable_reference);
    RUN(as_array_const_reference);
    RUN(as_object_mutable_reference);
    RUN(as_object_const_reference);
    RUN(as_bool_throws_on_wrong_type);
    RUN(as_number_throws_on_wrong_type);
    RUN(as_string_throws_on_wrong_type);
    RUN(as_array_throws_on_wrong_type);
    RUN(as_object_throws_on_wrong_type);
    RUN(const_accessors_throw_on_wrong_type);
}

REGISTER_TEST_SUITE();
