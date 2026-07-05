// Json constructor and move semantics test suite.
//
// Coverage:
// - Default and nullptr construction
// - Bool, double, int construction
// - String construction (lvalue, rvalue, C-string)
// - Array construction (lvalue, rvalue)
// - Object construction (lvalue, rvalue)
// - Copy construction (deep copy independence)
// - Copy assignment
// - Move construction (source becomes Null)
// - Move assignment (source becomes Null, target overwritten)
// - Self move assignment

#include <common/framework.h>

using namespace JsonPro;

// Verifies that the default constructor produces a Null value.
static void default_constructor_is_null() {
    Json j;
    CHK(j.isNull() == true);
    CHK(j.type()   == Json::Type::Null);
}

// Verifies explicit nullptr construction.
static void nullptr_constructor() {
    Json j(nullptr);
    CHK(j.isNull() == true);
}

// Verifies bool construction.
static void bool_constructor() {
    Json j(true);
    CHK(j.isBool()  == true);
    CHK(j.asBool()  == true);
}

// Verifies double construction.
static void double_constructor() {
    Json j(3.14);
    CHK(j.isNumber()  == true);
    CHK(j.asNumber()  == 3.14);
}

// Verifies that int construction converts to Number (double storage).
static void int_constructor_converts_to_number() {
    Json j(42);
    CHK(j.isNumber()  == true);
    CHK(j.asNumber()  == 42.0);
}

// Verifies construction from a std::string lvalue.
static void string_lvalue_constructor() {
    std::string s = "hello";
    Json j(s);
    CHK(j.isString()  == true);
    CHK(j.asString()  == "hello");
    CHK(s              == "hello"); // source untouched
}

// Verifies construction from a std::string rvalue (moved-from).
static void string_rvalue_constructor() {
    std::string s = "world";
    Json j(std::move(s));
    CHK(j.isString()  == true);
    CHK(j.asString()  == "world");
}

// Verifies construction from a C-string literal.
static void c_string_constructor() {
    Json j("literal");
    CHK(j.isString()  == true);
    CHK(j.asString()  == "literal");
}

// Verifies construction from an ArrayType lvalue.
static void array_lvalue_constructor() {
    Json::ArrayType arr;
    arr.emplace_back(1);
    arr.emplace_back(2);

    Json j(arr);
    CHK(j.isArray()   == true);
    CHK(j.size()      == 2);
    CHK(arr.size()    == 2); // source untouched
}

// Verifies construction from an ArrayType rvalue.
static void array_rvalue_constructor() {
    Json::ArrayType arr;
    arr.emplace_back(1);
    arr.emplace_back(2);
    arr.emplace_back(3);

    Json j(std::move(arr));
    CHK(j.isArray()  == true);
    CHK(j.size()     == 3);
}

// Verifies construction from an ObjectType lvalue.
static void object_lvalue_constructor() {
    Json::ObjectType obj;
    obj.emplace("key", Json(1));

    Json j(obj);
    CHK(j.isObject()      == true);
    CHK(j.contains("key") == true);
    CHK(obj.size()        == 1); // source untouched
}

// Verifies construction from an ObjectType rvalue.
static void object_rvalue_constructor() {
    Json::ObjectType obj;
    obj.emplace("a", Json(1));
    obj.emplace("b", Json(2));

    Json j(std::move(obj));
    CHK(j.isObject()  == true);
    CHK(j.size()      == 2);
}

// Verifies that copy construction produces an independent deep copy.
static void copy_constructor_deep_copy() {
    Json original(Json::ArrayType{ Json(1), Json(2) });
    Json copy(original);

    copy.asArray().emplace_back(3);

    CHK(original.size() == 2);
    CHK(copy.size()     == 3);
}

// Verifies copy assignment replaces the target's contents.
static void copy_assignment() {
    Json source("copied");
    Json target(123);

    target = source;

    CHK(target.isString()  == true);
    CHK(target.asString()  == "copied");
    CHK(source.asString()  == "copied"); // source untouched
}

// Verifies that move construction leaves the source as Null.
static void move_construction_leaves_source_null() {
    Json source("movable");
    Json moved(std::move(source));

    CHK(moved.isString()  == true);
    CHK(moved.asString()  == "movable");
    CHK(source.isNull()   == true);
}

// Verifies that move assignment overwrites the target and nulls the source.
static void move_assignment_overwrites_target() {
    Json source(Json::ArrayType{ Json(1), Json(2), Json(3) });
    Json target("old value");

    target = std::move(source);

    CHK(target.isArray()  == true);
    CHK(target.size()     == 3);
    CHK(source.isNull()   == true);
}

// Verifies that self move assignment leaves the value unchanged.
static void self_move_assignment() {
    Json j("stable");
    j = std::move(j);

    CHK(j.isString()  == true);
    CHK(j.asString()  == "stable");
}

// Executes all constructor and move semantics test cases.
static void run_tests() {
    RUN(default_constructor_is_null);
    RUN(nullptr_constructor);
    RUN(bool_constructor);
    RUN(double_constructor);
    RUN(int_constructor_converts_to_number);
    RUN(string_lvalue_constructor);
    RUN(string_rvalue_constructor);
    RUN(c_string_constructor);
    RUN(array_lvalue_constructor);
    RUN(array_rvalue_constructor);
    RUN(object_lvalue_constructor);
    RUN(object_rvalue_constructor);
    RUN(copy_constructor_deep_copy);
    RUN(copy_assignment);
    RUN(move_construction_leaves_source_null);
    RUN(move_assignment_overwrites_target);
    RUN(self_move_assignment);
}

REGISTER_TEST_SUITE();
