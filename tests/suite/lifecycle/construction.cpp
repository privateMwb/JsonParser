// Json construction test suite.
//
// Coverage:
// - Default and nullptr construction (Null)
// - Bool, double, int construction (int widens to double)
// - String construction (lvalue, rvalue, C-string)
// - Array construction (lvalue, rvalue)
// - Object construction (lvalue, rvalue)
//
// Copy/move construction and assignment are covered separately in
// copy_semantics.cpp and move_semantics.cpp.

#include <common/framework.h>

using namespace JsonPro;

// Verifies that the default constructor produces a Null value.
static void default_constructor_is_null() {
    Json j;
    CHK(j.isNull() == true);
    CHK(j.type() == Json::Type::Null);
}

// Verifies explicit nullptr construction.
static void nullptr_constructor() {
    Json j(nullptr);
    CHK(j.isNull() == true);
}

// Verifies bool construction.
static void bool_constructor() {
    Json j(true);
    CHK(j.isBool() == true);
    CHK(j.asBool() == true);
}

// Verifies double construction.
static void double_constructor() {
    Json j(3.14);

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    CHK(j.isNumber() == true);
    CHK(j.asNumber() == 3.14);
}

// Verifies that int construction converts to Number (double storage).
static void int_constructor_converts_to_number() {
    Json j(42);
    CHK(j.isNumber() == true);
    CHK(j.asNumber() == 42.0);
}

// Verifies construction from a std::string lvalue.
static void string_lvalue_constructor() {
    std::string s = "hello";
    Json j(s);
    CHK(j.isString() == true);
    CHK(j.asString() == "hello");
    CHK(s == "hello"); // source untouched
}

// Verifies construction from a std::string rvalue (moved-from).
static void string_rvalue_constructor() {
    std::string s = "world";
    Json j(std::move(s));
    CHK(j.isString() == true);
    CHK(j.asString() == "world");
}

// Verifies construction from a C-string literal.
static void c_string_constructor() {
    Json j("literal");
    CHK(j.isString() == true);
    CHK(j.asString() == "literal");
}

// Verifies construction from an ArrayType lvalue.
static void array_lvalue_constructor() {
    Json::ArrayType arr;
    arr.emplace_back(1);
    arr.emplace_back(2);

    Json j(arr);
    CHK(j.isArray() == true);
    CHK(j.size() == 2);
    CHK(arr.size() == 2); // source untouched
}

// Verifies construction from an ArrayType rvalue.
static void array_rvalue_constructor() {
    Json::ArrayType arr;
    arr.emplace_back(1);
    arr.emplace_back(2);
    arr.emplace_back(3);

    Json j(std::move(arr));
    CHK(j.isArray() == true);
    CHK(j.size() == 3);
}

// Verifies construction from an ObjectType lvalue.
static void object_lvalue_constructor() {
    Json::ObjectType obj;
    obj.emplace("key", Json(1));

    Json j(obj);
    CHK(j.isObject() == true);
    CHK(j.contains("key") == true);
    CHK(obj.size() == 1); // source untouched
}

// Verifies construction from an ObjectType rvalue.
static void object_rvalue_constructor() {
    Json::ObjectType obj;
    obj.emplace("a", Json(1));
    obj.emplace("b", Json(2));

    Json j(std::move(obj));
    CHK(j.isObject() == true);
    CHK(j.size() == 2);
}

// Executes all construction test cases.
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
}

REGISTER_TEST_SUITE();
