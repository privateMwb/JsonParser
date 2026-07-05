// Json serialization test suite.
//
// Coverage:
// - dump() for each scalar type (Null, Bool, Number, String)
// - dump() for empty Array / Object (compact "[]" / "{}")
// - dump() for populated Array / Object with indentation
// - dump() for nested structures
// - dump(std::ostream&) produces output identical to dump(int)

#include <sstream>

#include <common/framework.h>

using namespace JsonPro;

// Verifies Null serializes to "null".
static void dump_null() {
    CHK(Json().dump() == "null");
}

// Verifies Bool serializes to "true" / "false".
static void dump_bool() {
    CHK(Json(true).dump()  == "true");
    CHK(Json(false).dump() == "false");
}

// Verifies Number serializes via default stream formatting.
static void dump_number() {
    CHK(Json(42.0).dump() == "42");
}

// Verifies String serializes wrapped in double quotes.
static void dump_string() {
    CHK(Json("hello").dump() == "\"hello\"");
}

// Verifies an empty array serializes compactly as "[]".
static void dump_empty_array() {
    Json j(Json::ArrayType{});
    CHK(j.dump(0) == "[]");
}

// Verifies an empty object serializes compactly as "{}".
static void dump_empty_object() {
    Json j(Json::ObjectType{});
    CHK(j.dump(0) == "{}");
}

// Verifies a populated array serializes with one element per line,
// indented two spaces past the array's own indent level.
static void dump_populated_array() {
    Json j(Json::ArrayType{ Json(1), Json(2) });

    std::string expected =
        "[\n"
        "  1,\n"
        "  2\n"
        "]";

    CHK(j.dump(0) == expected);
}

// Verifies a populated object serializes with quoted keys and ": " separator.
static void dump_populated_object_single_key() {
    Json::ObjectType obj;
    obj.emplace("key", Json("value"));

    Json j(std::move(obj));

    std::string expected =
        "{\n"
        "  \"key\": \"value\"\n"
        "}";

    CHK(j.dump(0) == expected);
}

// Verifies nested array-in-object serializes with compounding indentation.
static void dump_nested_structure() {
    Json::ObjectType obj;
    obj.emplace("nums", Json(Json::ArrayType{ Json(1), Json(2) }));

    Json j(std::move(obj));

    std::string expected =
        "{\n"
        "  \"nums\": [\n"
        "    1,\n"
        "    2\n"
        "  ]\n"
        "}";

    CHK(j.dump(0) == expected);
}

// Verifies dump(int) respects a non-zero starting indent.
static void dump_respects_starting_indent() {
    Json j(Json::ArrayType{ Json(1) });

    std::string expected =
        "[\n"
        "    1\n"
        "  ]";

    CHK(j.dump(2) == expected);
}

// Verifies dump(std::ostream&) produces output identical to dump(int).
static void dump_ostream_matches_dump_string() {
    Json j(Json::ArrayType{ Json(1), Json(Json::ObjectType{}) });

    std::ostringstream oss;
    j.dump(oss, 0);

    CHK(oss.str() == j.dump(0));
}

// Executes all serialization test cases.
static void run_tests() {
    RUN(dump_null);
    RUN(dump_bool);
    RUN(dump_number);
    RUN(dump_string);
    RUN(dump_empty_array);
    RUN(dump_empty_object);
    RUN(dump_populated_array);
    RUN(dump_populated_object_single_key);
    RUN(dump_nested_structure);
    RUN(dump_respects_starting_indent);
    RUN(dump_ostream_matches_dump_string);
}

REGISTER_TEST_SUITE();
