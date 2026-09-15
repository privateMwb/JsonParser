// Json comparison test suite.
//
// Coverage:
// - operator== / operator!= for each type (Null, Bool, Number, String)
// - Deep equality for Array and Object, including nested structures
// - Cross-type comparisons are always unequal, regardless of value
// - Object equality is order-independent (key-based, not insertion-order)

#include <JsonPro/Json.h>

#include <gtest/gtest.h>

using namespace JsonPro;

// Verifies two Null values compare equal.
TEST(Comparison, null_equals_null) {
    EXPECT_EQ(Json(), Json(nullptr));
    EXPECT_FALSE(Json() != Json(nullptr));
}

// Verifies Bool equality and inequality.
TEST(Comparison, bool_equality) {
    EXPECT_EQ(Json(true), Json(true));
    EXPECT_EQ(Json(false), Json(false));
    EXPECT_NE(Json(true), Json(false));
}

// Verifies Number equality and inequality.
TEST(Comparison, number_equality) {
    EXPECT_EQ(Json(1.5), Json(1.5));
    EXPECT_NE(Json(1.5), Json(2.5));
    EXPECT_EQ(Json(1), Json(1.0)); // int ctor converts to double
}

// Verifies String equality and inequality.
TEST(Comparison, string_equality) {
    EXPECT_EQ(Json("abc"), Json("abc"));
    EXPECT_NE(Json("abc"), Json("xyz"));
}

// Verifies Array equality requires identical size and elementwise equality.
TEST(Comparison, array_equality) {
    Json a(Json::ArrayType{Json(1), Json(2), Json(3)});
    Json b(Json::ArrayType{Json(1), Json(2), Json(3)});
    Json c(Json::ArrayType{Json(1), Json(2)});
    Json d(Json::ArrayType{Json(1), Json(2), Json(9)});

    EXPECT_EQ(a, b);
    EXPECT_NE(a, c); // different size
    EXPECT_NE(a, d); // different element
}

// Verifies nested Array/Object structures compare deeply.
TEST(Comparison, nested_structure_equality) {
    Json::ObjectType obj1;
    obj1.emplace("nums", Json(Json::ArrayType{Json(1), Json(2)}));

    Json::ObjectType obj2;
    obj2.emplace("nums", Json(Json::ArrayType{Json(1), Json(2)}));

    Json::ObjectType obj3;
    obj3.emplace("nums", Json(Json::ArrayType{Json(1), Json(9)}));

    Json a(std::move(obj1));
    Json b(std::move(obj2));
    Json c(std::move(obj3));

    EXPECT_EQ(a, b);
    EXPECT_NE(a, c);
}

// Verifies Object equality is key-based, independent of insertion order.
TEST(Comparison, object_equality_order_independent) {
    Json::ObjectType obj1;
    obj1.emplace("a", Json(1));
    obj1.emplace("b", Json(2));

    Json::ObjectType obj2;
    obj2.emplace("b", Json(2));
    obj2.emplace("a", Json(1));

    Json a(std::move(obj1));
    Json b(std::move(obj2));

    EXPECT_EQ(a, b);
}

// Verifies Object equality requires identical size and values.
TEST(Comparison, object_inequality) {
    Json::ObjectType obj1;
    obj1.emplace("a", Json(1));

    Json::ObjectType obj2;
    obj2.emplace("a", Json(1));
    obj2.emplace("b", Json(2));

    Json a(std::move(obj1));
    Json b(std::move(obj2));

    EXPECT_NE(a, b); // different size
}

// Verifies values of different types are never equal, even with "equivalent" values.
TEST(Comparison, cross_type_comparisons_are_unequal) {
    EXPECT_NE(Json(), Json(false));
    EXPECT_NE(Json(0.0), Json(false));
    EXPECT_NE(Json("1"), Json(1.0));
    EXPECT_NE(Json(Json::ArrayType{}), Json(Json::ObjectType{}));
}
