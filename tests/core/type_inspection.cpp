// Json type inspection test suite.
//
// Coverage:
// - type() returns the correct Type for each alternative
// - isNull / isBool / isNumber / isString / isArray / isObject
// - Each predicate is false for every other constructed type

#include <common/framework.h>

using namespace JsonPro;

// Verifies Null type() and isNull(), and that no other predicate is true.
static void null_type() {
    Json j;
    CHK(j.type()     == Json::Type::Null);
    CHK(j.isNull()   == true);
    CHK(j.isBool()   == false);
    CHK(j.isNumber() == false);
    CHK(j.isString() == false);
    CHK(j.isArray()  == false);
    CHK(j.isObject() == false);
}

// Verifies Bool type() and isBool(), and that no other predicate is true.
static void bool_type() {
    Json j(true);
    CHK(j.type()     == Json::Type::Bool);
    CHK(j.isBool()   == true);
    CHK(j.isNull()   == false);
    CHK(j.isNumber() == false);
    CHK(j.isString() == false);
    CHK(j.isArray()  == false);
    CHK(j.isObject() == false);
}

// Verifies Number type() and isNumber(), and that no other predicate is true.
static void number_type() {
    Json j(3.14);
    CHK(j.type()     == Json::Type::Number);
    CHK(j.isNumber() == true);
    CHK(j.isNull()   == false);
    CHK(j.isBool()   == false);
    CHK(j.isString() == false);
    CHK(j.isArray()  == false);
    CHK(j.isObject() == false);
}

// Verifies String type() and isString(), and that no other predicate is true.
static void string_type() {
    Json j("text");
    CHK(j.type()     == Json::Type::String);
    CHK(j.isString() == true);
    CHK(j.isNull()   == false);
    CHK(j.isBool()   == false);
    CHK(j.isNumber() == false);
    CHK(j.isArray()  == false);
    CHK(j.isObject() == false);
}

// Verifies Array type() and isArray(), and that no other predicate is true.
static void array_type() {
    Json j(Json::ArrayType{ Json(1), Json(2) });
    CHK(j.type()     == Json::Type::Array);
    CHK(j.isArray()  == true);
    CHK(j.isNull()   == false);
    CHK(j.isBool()   == false);
    CHK(j.isNumber() == false);
    CHK(j.isString() == false);
    CHK(j.isObject() == false);
}

// Verifies Object type() and isObject(), and that no other predicate is true.
static void object_type() {
    Json::ObjectType obj;
    obj.emplace("key", Json(1));

    Json j(std::move(obj));
    CHK(j.type()     == Json::Type::Object);
    CHK(j.isObject() == true);
    CHK(j.isNull()   == false);
    CHK(j.isBool()   == false);
    CHK(j.isNumber() == false);
    CHK(j.isString() == false);
    CHK(j.isArray()  == false);
}

// Verifies type() reflects reassignment across different alternatives.
static void type_changes_on_reassignment() {
    Json j(1.0);
    CHK(j.type() == Json::Type::Number);

    j = Json("now a string");
    CHK(j.type()     == Json::Type::String);
    CHK(j.isNumber() == false);
}

// Executes all type inspection test cases.
static void run_tests() {
    RUN(null_type);
    RUN(bool_type);
    RUN(number_type);
    RUN(string_type);
    RUN(array_type);
    RUN(object_type);
    RUN(type_changes_on_reassignment);
}

REGISTER_TEST_SUITE();
