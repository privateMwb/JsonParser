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

#include <gtest/gtest.h>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Verifies that the default constructor produces a Null value.
TEST(Construction, DefaultConstructorIsNull) {
    Json j;
    EXPECT_EQ(j.isNull(), true);
    EXPECT_EQ(j.type(), Json::Type::Null);
}

// Verifies explicit nullptr construction.
TEST(Construction, NullptrConstructor) {
    Json j(nullptr);
    EXPECT_EQ(j.isNull(), true);
}

// Verifies bool construction.
TEST(Construction, BoolConstructor) {
    Json j(true);
    EXPECT_EQ(j.isBool(), true);
    EXPECT_EQ(j.asBool(), true);
}

// Verifies double construction.
TEST(Construction, DoubleConstructor) {
    Json j(3.14);

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    EXPECT_EQ(j.isNumber(), true);
    EXPECT_EQ(j.asNumber(), 3.14);
}

// Verifies that int construction converts to Number (double storage).
TEST(Construction, IntConstructorConvertsToNumber) {
    Json j(42);
    EXPECT_EQ(j.isNumber(), true);
    EXPECT_EQ(j.asNumber(), 42.0);
}

// Verifies construction from a std::string lvalue.
TEST(Construction, StringLvalueConstructor) {
    std::string s = "hello";
    Json j(s);
    EXPECT_EQ(j.isString(), true);
    EXPECT_EQ(j.asString(), "hello");
    EXPECT_EQ(s, "hello"); // source untouched
}

// Verifies construction from a std::string rvalue (moved-from).
TEST(Construction, StringRvalueConstructor) {
    std::string s = "world";
    Json j(std::move(s));
    EXPECT_EQ(j.isString(), true);
    EXPECT_EQ(j.asString(), "world");
}

// Verifies construction from a C-string literal.
TEST(Construction, CStringConstructor) {
    Json j("literal");
    EXPECT_EQ(j.isString(), true);
    EXPECT_EQ(j.asString(), "literal");
}

// Verifies construction from an ArrayType lvalue.
TEST(Construction, ArrayLvalueConstructor) {
    Json::ArrayType arr;
    arr.emplace_back(1);
    arr.emplace_back(2);

    Json j(arr);
    EXPECT_EQ(j.isArray(), true);
    EXPECT_EQ(j.size(), 2);
    EXPECT_EQ(arr.size(), 2); // source untouched
}

// Verifies construction from an ArrayType rvalue.
TEST(Construction, ArrayRvalueConstructor) {
    Json::ArrayType arr;
    arr.emplace_back(1);
    arr.emplace_back(2);
    arr.emplace_back(3);

    Json j(std::move(arr));
    EXPECT_EQ(j.isArray(), true);
    EXPECT_EQ(j.size(), 3);
}

// Verifies construction from an ObjectType lvalue.
TEST(Construction, ObjectLvalueConstructor) {
    Json::ObjectType obj;
    obj.emplace("key", Json(1));

    Json j(obj);
    EXPECT_EQ(j.isObject(), true);
    EXPECT_EQ(j.contains("key"), true);
    EXPECT_EQ(obj.size(), 1); // source untouched
}

// Verifies construction from an ObjectType rvalue.
TEST(Construction, ObjectRvalueConstructor) {
    Json::ObjectType obj;
    obj.emplace("a", Json(1));
    obj.emplace("b", Json(2));

    Json j(std::move(obj));
    EXPECT_EQ(j.isObject(), true);
    EXPECT_EQ(j.size(), 2);
}
