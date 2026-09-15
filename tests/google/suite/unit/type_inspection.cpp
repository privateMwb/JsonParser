// Json type inspection test suite.
//
// Coverage:
// - type() returns the correct Type for each alternative
// - isNull / isBool / isNumber / isString / isArray / isObject
// - Each predicate is false for every other constructed type

#include <JsonPro/Json.h>

#include <gtest/gtest.h>

using namespace JsonPro;

// Verifies Null type() and isNull(), and that no other predicate is true.
TEST(TypeInspection, null_type) {
    Json j;
    EXPECT_EQ(j.type(), Json::Type::Null);
    EXPECT_TRUE(j.isNull());
    EXPECT_FALSE(j.isBool());
    EXPECT_FALSE(j.isNumber());
    EXPECT_FALSE(j.isString());
    EXPECT_FALSE(j.isArray());
    EXPECT_FALSE(j.isObject());
}

// Verifies Bool type() and isBool(), and that no other predicate is true.
TEST(TypeInspection, bool_type) {
    Json j(true);
    EXPECT_EQ(j.type(), Json::Type::Bool);
    EXPECT_TRUE(j.isBool());
    EXPECT_FALSE(j.isNull());
    EXPECT_FALSE(j.isNumber());
    EXPECT_FALSE(j.isString());
    EXPECT_FALSE(j.isArray());
    EXPECT_FALSE(j.isObject());
}

// Verifies Number type() and isNumber(), and that no other predicate is true.
TEST(TypeInspection, number_type) {
    Json j(3.14);
    EXPECT_EQ(j.type(), Json::Type::Number);
    EXPECT_TRUE(j.isNumber());
    EXPECT_FALSE(j.isNull());
    EXPECT_FALSE(j.isBool());
    EXPECT_FALSE(j.isString());
    EXPECT_FALSE(j.isArray());
    EXPECT_FALSE(j.isObject());
}

// Verifies String type() and isString(), and that no other predicate is true.
TEST(TypeInspection, string_type) {
    Json j("text");
    EXPECT_EQ(j.type(), Json::Type::String);
    EXPECT_TRUE(j.isString());
    EXPECT_FALSE(j.isNull());
    EXPECT_FALSE(j.isBool());
    EXPECT_FALSE(j.isNumber());
    EXPECT_FALSE(j.isArray());
    EXPECT_FALSE(j.isObject());
}

// Verifies Array type() and isArray(), and that no other predicate is true.
TEST(TypeInspection, array_type) {
    Json j(Json::ArrayType{Json(1), Json(2)});
    EXPECT_EQ(j.type(), Json::Type::Array);
    EXPECT_TRUE(j.isArray());
    EXPECT_FALSE(j.isNull());
    EXPECT_FALSE(j.isBool());
    EXPECT_FALSE(j.isNumber());
    EXPECT_FALSE(j.isString());
    EXPECT_FALSE(j.isObject());
}

// Verifies Object type() and isObject(), and that no other predicate is true.
TEST(TypeInspection, object_type) {
    Json::ObjectType obj;
    obj.emplace("key", Json(1));

    Json j(std::move(obj));
    EXPECT_EQ(j.type(), Json::Type::Object);
    EXPECT_TRUE(j.isObject());
    EXPECT_FALSE(j.isNull());
    EXPECT_FALSE(j.isBool());
    EXPECT_FALSE(j.isNumber());
    EXPECT_FALSE(j.isString());
    EXPECT_FALSE(j.isArray());
}

// Verifies type() reflects reassignment across different alternatives.
TEST(TypeInspection, type_changes_on_reassignment) {
    Json j(1.0);
    EXPECT_EQ(j.type(), Json::Type::Number);

    j = Json("now a string");
    EXPECT_EQ(j.type(), Json::Type::String);
    EXPECT_FALSE(j.isNumber());
}
