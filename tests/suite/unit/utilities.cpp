// Json utilities test suite.
//
// Coverage:
// - size() on arrays, objects, and non-container types
// - contains() on objects, including absent keys and non-object types

#include <common/framework.h>

using namespace JsonPro;

// Verifies size() returns the element count of an array.
static void size_of_array() {
    Json j(Json::ArrayType{Json(1), Json(2), Json(3)});
    CHK(j.size() == 3);
}

// Verifies size() returns 0 for an empty array.
static void size_of_empty_array() {
    Json j(Json::ArrayType{});
    CHK(j.size() == 0);
}

// Verifies size() returns the key count of an object.
static void size_of_object() {
    Json::ObjectType obj;
    obj.emplace("a", Json(1));
    obj.emplace("b", Json(2));

    Json j(std::move(obj));
    CHK(j.size() == 2);
}

// Verifies size() returns 0 for an empty object.
static void size_of_empty_object() {
    Json j(Json::ObjectType{});
    CHK(j.size() == 0);
}

// Verifies size() returns 0 for non-container types (Null, Bool, Number, String).
static void size_of_non_container_types() {
    CHK(Json().size() == 0);
    CHK(Json(true).size() == 0);
    CHK(Json(1.0).size() == 0);
    CHK(Json("text").size() == 0);
}

// Verifies contains() returns true for a present key.
static void contains_existing_key() {
    Json::ObjectType obj;
    obj.emplace("present", Json(1));

    Json j(std::move(obj));
    CHK(j.contains("present") == true);
}

// Verifies contains() returns false for an absent key.
static void contains_missing_key() {
    Json::ObjectType obj;
    obj.emplace("present", Json(1));

    Json j(std::move(obj));
    CHK(j.contains("absent") == false);
}

// Verifies contains() returns false (not throws) on non-object types.
static void contains_on_non_object_returns_false() {
    CHK(Json().contains("k") == false);
    CHK(Json(1.0).contains("k") == false);
    CHK(Json("str").contains("k") == false);
    CHK(Json(Json::ArrayType{}).contains("k") == false);
}

// Executes all utilities test cases.
static void run_tests() {
    RUN(size_of_array);
    RUN(size_of_empty_array);
    RUN(size_of_object);
    RUN(size_of_empty_object);
    RUN(size_of_non_container_types);
    RUN(contains_existing_key);
    RUN(contains_missing_key);
    RUN(contains_on_non_object_returns_false);
}

REGISTER_TEST_SUITE();
