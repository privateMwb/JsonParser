// Json copy semantics test suite.
//
// Coverage:
// - Copy construction produces an independent deep copy (Array and Object)
// - Copy assignment replaces the target's contents without disturbing the source
// - Self copy assignment leaves the value unchanged
// - Copies of nested (mixed Array/Object) structures are fully independent

#include <common/framework.h>

using namespace JsonPro;

// Verifies that copy construction produces an independent deep copy of an Array.
static void copy_constructor_deep_copy_array() {
    Json original(Json::ArrayType{Json(1), Json(2)});
    Json copy(original);

    copy.asArray().emplace_back(3);

    CHK(original.size() == 2);
    CHK(copy.size() == 3);
}

// Verifies that copy construction produces an independent deep copy of an Object.
static void copy_constructor_deep_copy_object() {
    Json::ObjectType obj;
    obj.emplace("a", Json(1));

    Json original(std::move(obj));
    Json copy(original);

    copy.asObject().emplace("b", Json(2));

    CHK(original.size() == 1);
    CHK(copy.size() == 2);
}

// Verifies copy assignment replaces the target's contents.
static void copy_assignment() {
    Json source("copied");
    Json target(123);

    target = source;

    CHK(target.isString() == true);
    CHK(target.asString() == "copied");
    CHK(source.asString() == "copied"); // source untouched
}

// Verifies self copy assignment leaves the value unchanged.
static void self_copy_assignment_leaves_value_unchanged() {
    Json j("stable");
    j = j;

    CHK(j.isString() == true);
    CHK(j.asString() == "stable");
}

// Verifies a copy of a nested Array-in-Object structure is fully independent
// of the original at every level.
static void copy_of_nested_structure_is_independent() {
    Json::ObjectType obj;
    obj.emplace("nums", Json(Json::ArrayType{Json(1), Json(2)}));

    Json original(std::move(obj));
    Json copy(original);

    copy["nums"].asArray().emplace_back(Json(3));

    CHK(original["nums"].size() == 2);
    CHK(copy["nums"].size() == 3);
}

// Executes all copy semantics test cases.
static void run_tests() {
    RUN(copy_constructor_deep_copy_array);
    RUN(copy_constructor_deep_copy_object);
    RUN(copy_assignment);
    RUN(self_copy_assignment_leaves_value_unchanged);
    RUN(copy_of_nested_structure_is_independent);
}

REGISTER_TEST_SUITE();
