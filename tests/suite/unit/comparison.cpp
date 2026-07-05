// Json comparison test suite.
//
// Coverage:
// - operator== / operator!= for each type (Null, Bool, Number, String)
// - Deep equality for Array and Object, including nested structures
// - Cross-type comparisons are always unequal, regardless of value
// - Object equality is order-independent (key-based, not insertion-order)

#include <common/framework.h>

using namespace JsonPro;

// Verifies two Null values compare equal.
static void null_equals_null() {
    CHK(Json() == Json(nullptr));
    CHK(!(Json() != Json(nullptr)));
}

// Verifies Bool equality and inequality.
static void bool_equality() {
    CHK(Json(true)  == Json(true));
    CHK(Json(false) == Json(false));
    CHK(Json(true)  != Json(false));
}

// Verifies Number equality and inequality.
static void number_equality() {
    CHK(Json(1.5) == Json(1.5));
    CHK(Json(1.5) != Json(2.5));
    CHK(Json(1)   == Json(1.0)); // int ctor converts to double
}

// Verifies String equality and inequality.
static void string_equality() {
    CHK(Json("abc") == Json("abc"));
    CHK(Json("abc") != Json("xyz"));
}

// Verifies Array equality requires identical size and elementwise equality.
static void array_equality() {
    Json a(Json::ArrayType{ Json(1), Json(2), Json(3) });
    Json b(Json::ArrayType{ Json(1), Json(2), Json(3) });
    Json c(Json::ArrayType{ Json(1), Json(2) });
    Json d(Json::ArrayType{ Json(1), Json(2), Json(9) });

    CHK(a == b);
    CHK(a != c); // different size
    CHK(a != d); // different element
}

// Verifies nested Array/Object structures compare deeply.
static void nested_structure_equality() {
    Json::ObjectType obj1;
    obj1.emplace("nums", Json(Json::ArrayType{ Json(1), Json(2) }));

    Json::ObjectType obj2;
    obj2.emplace("nums", Json(Json::ArrayType{ Json(1), Json(2) }));

    Json::ObjectType obj3;
    obj3.emplace("nums", Json(Json::ArrayType{ Json(1), Json(9) }));

    Json a(std::move(obj1));
    Json b(std::move(obj2));
    Json c(std::move(obj3));

    CHK(a == b);
    CHK(a != c);
}

// Verifies Object equality is key-based, independent of insertion order.
static void object_equality_order_independent() {
    Json::ObjectType obj1;
    obj1.emplace("a", Json(1));
    obj1.emplace("b", Json(2));

    Json::ObjectType obj2;
    obj2.emplace("b", Json(2));
    obj2.emplace("a", Json(1));

    Json a(std::move(obj1));
    Json b(std::move(obj2));

    CHK(a == b);
}

// Verifies Object equality requires identical size and values.
static void object_inequality() {
    Json::ObjectType obj1;
    obj1.emplace("a", Json(1));

    Json::ObjectType obj2;
    obj2.emplace("a", Json(1));
    obj2.emplace("b", Json(2));

    Json a(std::move(obj1));
    Json b(std::move(obj2));

    CHK(a != b); // different size
}

// Verifies values of different types are never equal, even with "equivalent" values.
static void cross_type_comparisons_are_unequal() {
    CHK(Json()       != Json(false));
    CHK(Json(0.0)    != Json(false));
    CHK(Json("1")    != Json(1.0));
    CHK(Json(Json::ArrayType{}) != Json(Json::ObjectType{}));
}

// Executes all comparison test cases.
static void run_tests() {
    RUN(null_equals_null);
    RUN(bool_equality);
    RUN(number_equality);
    RUN(string_equality);
    RUN(array_equality);
    RUN(nested_structure_equality);
    RUN(object_equality_order_independent);
    RUN(object_inequality);
    RUN(cross_type_comparisons_are_unequal);
}

REGISTER_TEST_SUITE();
