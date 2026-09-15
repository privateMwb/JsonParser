// Json serialization test suite.
//
// Coverage:
// - dump() for each scalar type (Null, Bool, Number, String)
// - dump() for empty Array / Object (compact "[]" / "{}")
// - dump() for populated Array / Object with indentation
// - dump() for nested structures
// - dump(std::ostream&) produces output identical to dump(int)

#include <JsonPro/Json.h>

#include <gtest/gtest.h>

#include <sstream>

using namespace JsonPro;

// Verifies Null serializes to "null".
TEST(Serialization, dump_null) {
    EXPECT_EQ(Json().dump(), "null");
}

// Verifies Bool serializes to "true" / "false".
TEST(Serialization, dump_bool) {
    EXPECT_EQ(Json(true).dump(), "true");
    EXPECT_EQ(Json(false).dump(), "false");
}

// Verifies Number serializes via default stream formatting.
TEST(Serialization, dump_number) {
    EXPECT_EQ(Json(42.0).dump(), "42");
}

// Verifies String serializes wrapped in double quotes.
TEST(Serialization, dump_string) {
    EXPECT_EQ(Json("hello").dump(), "\"hello\"");
}

// Verifies an empty array serializes compactly as "[]".
TEST(Serialization, dump_empty_array) {
    Json j(Json::ArrayType{});
    EXPECT_EQ(j.dump(0), "[]");
}

// Verifies an empty object serializes compactly as "{}".
TEST(Serialization, dump_empty_object) {
    Json j(Json::ObjectType{});
    EXPECT_EQ(j.dump(0), "{}");
}

// Verifies a populated array serializes with one element per line,
// indented two spaces past the array's own indent level.
TEST(Serialization, dump_populated_array) {
    Json j(Json::ArrayType{Json(1), Json(2)});

    std::string expected = "[\n"
                           "  1,\n"
                           "  2\n"
                           "]";

    EXPECT_EQ(j.dump(0), expected);
}

// Verifies a populated object serializes with quoted keys and ": " separator.
TEST(Serialization, dump_populated_object_single_key) {
    Json::ObjectType obj;
    obj.emplace("key", Json("value"));

    Json j(std::move(obj));

    std::string expected = "{\n"
                           "  \"key\": \"value\"\n"
                           "}";

    EXPECT_EQ(j.dump(0), expected);
}

// Verifies nested array-in-object serializes with compounding indentation.
TEST(Serialization, dump_nested_structure) {
    Json::ObjectType obj;
    obj.emplace("nums", Json(Json::ArrayType{Json(1), Json(2)}));

    Json j(std::move(obj));

    std::string expected = "{\n"
                           "  \"nums\": [\n"
                           "    1,\n"
                           "    2\n"
                           "  ]\n"
                           "}";

    EXPECT_EQ(j.dump(0), expected);
}

// Verifies dump(int) respects a non-zero starting indent.
TEST(Serialization, dump_respects_starting_indent) {
    Json j(Json::ArrayType{Json(1)});

    std::string expected = "[\n"
                           "    1\n"
                           "  ]";

    EXPECT_EQ(j.dump(2), expected);
}

// Verifies dump(std::ostream&) produces output identical to dump(int).
TEST(Serialization, dump_ostream_matches_dump_string) {
    Json j(Json::ArrayType{Json(1), Json(Json::ObjectType{})});

    std::ostringstream oss;
    j.dump(oss, 0);

    EXPECT_EQ(oss.str(), j.dump(0));
}
