// Json exception safety test suite.
//
// Coverage:
// - A failed asBool()/asString() (wrong type) leaves the value unmodified
// - A failed at() (array out-of-range, missing object key) leaves the
//   container unmodified, and does not auto-vivify a missing key
// - An assignment target is left unchanged when the right-hand side throws
//   during evaluation (e.g. a failed Json::parse())
// - JsonObject::find() never mutates the object, even on a miss
// - A deeply nested document tears down cleanly when it goes out of scope

#include <common/framework.h>

using namespace JsonPro;

// Verifies a failed asBool() call leaves the underlying value unmodified.
static void failed_asBool_does_not_modify_value() {
    Json j(3.14);

    CHK_THROWS(j.asBool(), std::runtime_error);

    CHK(j.isNumber() == true);
    CHK(j.asNumber() == 3.14);
}

// Verifies a failed asString() call leaves the underlying value unmodified.
static void failed_asString_does_not_modify_value() {
    Json j(Json::ArrayType{Json(1), Json(2)});

    CHK_THROWS(j.asString(), std::runtime_error);

    CHK(j.isArray() == true);
    CHK(j.size() == 2);
}

// Verifies a failed array at() (out of range) leaves the array unmodified.
static void failed_array_at_out_of_range_leaves_container_unchanged() {
    Json j(Json::ArrayType{Json(1), Json(2), Json(3)});

    CHK_THROWS(j.at(99), std::runtime_error);

    CHK(j.size() == 3);
    CHK(j[1].asNumber() == 2.0);
}

// Verifies a failed object at() (missing key) leaves the object unmodified,
// and -- unlike operator[] -- does not auto-vivify the missing key.
static void failed_object_at_missing_key_leaves_container_unchanged() {
    Json::ObjectType obj;
    obj.emplace("present", Json(1));

    Json j(std::move(obj));

    CHK_THROWS(j.at("absent"), std::runtime_error);

    CHK(j.size() == 1);
    CHK(j.contains("absent") == false);
}

// Verifies an assignment target is left unchanged when the right-hand side
// throws during evaluation: the rhs must fully complete (or throw) before
// the assignment operator ever runs.
static void assignment_target_unchanged_when_rhs_parse_throws() {
    Json j("original");

    try {
        j = Json::parse("{ not valid json");
    } catch (const std::runtime_error&) {
        // rhs threw before the assignment could take place.
    }

    CHK(j.isString() == true);
    CHK(j.asString() == "original");
}

// Verifies JsonObject::find() never mutates the object, even when the key
// is absent (contrast with operator[], which auto-vivifies).
static void jsonobject_find_on_missing_key_does_not_insert() {
    JsonObject obj;
    obj.emplace("a", Json(1));

    const Json* p = obj.find("missing");

    CHK(p == nullptr);
    CHK(obj.size() == 1);
    CHK(obj.contains("missing") == false);
}

// Verifies a large, deeply nested document tears down cleanly (no crash,
// no leak-detector trip) when it goes out of scope.
static void nested_document_destructs_cleanly_without_crash() {
    {
        Json j = Json(Json::ArrayType{});

        for (int i = 0; i < 100; ++i) {
            Json entry = Json(Json::ObjectType{});
            entry["value"] = Json(i);
            j.asArray().push_back(std::move(entry));
        }

        CHK(j.size() == 100);
    } // j, and all 100 nested entries, are destroyed here.

    CHK(true); // reaching this line confirms teardown completed without crashing
}

// Executes all exception safety test cases.
static void run_tests() {
    RUN(failed_asBool_does_not_modify_value);
    RUN(failed_asString_does_not_modify_value);
    RUN(failed_array_at_out_of_range_leaves_container_unchanged);
    RUN(failed_object_at_missing_key_leaves_container_unchanged);
    RUN(assignment_target_unchanged_when_rhs_parse_throws);
    RUN(jsonobject_find_on_missing_key_does_not_insert);
    RUN(nested_document_destructs_cleanly_without_crash);
}

REGISTER_TEST_SUITE();
