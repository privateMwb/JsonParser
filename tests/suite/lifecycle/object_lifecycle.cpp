// JsonObject lifecycle test suite.
//
// Coverage:
// - Default construction produces an empty object
// - Copy construction produces an independent deep copy
// - Move construction transfers contents and leaves the source empty
// - Copy assignment replaces the target's contents without disturbing the source
// - Move assignment replaces the target's contents and empties the source
// - Self copy assignment leaves contents unchanged
// - Self move assignment leaves the object in a valid, reusable state
//
// JsonObject stores entries_ (std::vector) as an incomplete Json type in its
// header, so every special member is declared there but defined out-of-line
// in JsonObject.cpp as `= default`, once Json.h has made Json complete.

#include <common/framework.h>

using namespace JsonPro;

// Verifies default construction produces an empty object.
static void default_construction_is_empty() {
    JsonObject obj;
    CHK(obj.empty() == true);
    CHK(obj.size() == 0);
}

// Verifies copy construction produces an independent deep copy.
static void copy_constructor_deep_copy() {
    JsonObject original;
    original.emplace("a", Json(1));

    JsonObject copy(original);
    copy.emplace("b", Json(2));

    CHK(original.size() == 1);
    CHK(copy.size() == 2);
}

// Verifies move construction transfers contents and leaves the source empty,
// per the documented "left empty" contract.
static void move_constructor_leaves_source_empty() {
    JsonObject source;
    source.emplace("k", Json(1));

    JsonObject moved(std::move(source));

    CHK(moved.size() == 1);
    CHK(moved.find("k")->asNumber() == 1.0);

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    CHK(source.size() == 0);
}

// Verifies copy assignment replaces the target's contents without disturbing the source.
static void copy_assignment_replaces_contents() {
    JsonObject source;
    source.emplace("x", Json(1));

    JsonObject target;
    target.emplace("old", Json(0));

    target = source;

    CHK(target.size() == 1);
    CHK(target.find("x")->asNumber() == 1.0);
    CHK(source.size() == 1); // source untouched
}

// Verifies move assignment replaces the target's contents and empties the source.
static void move_assignment_replaces_contents_and_empties_source() {
    JsonObject source;
    source.emplace("x", Json(1));

    JsonObject target;
    target.emplace("old", Json(0));

    target = std::move(source);

    CHK(target.size() == 1);
    CHK(target.find("x")->asNumber() == 1.0);

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    CHK(source.size() == 0);
}

// Verifies self copy assignment leaves contents unchanged.
static void self_copy_assignment_leaves_contents_unchanged() {
    JsonObject obj;
    obj.emplace("k", Json(1));

    obj = obj;

    CHK(obj.size() == 1);
    CHK(obj.find("k")->asNumber() == 1.0);
}

// Verifies self move assignment does not crash and leaves the object in a
// valid, reusable state. Unlike Json::operator=(Json&&), JsonObject's move
// assignment is defaulted with no explicit self-check, so only a valid
// post-state is guaranteed -- not that prior contents survive.
static void self_move_assignment_leaves_object_in_valid_state() {
    JsonObject obj;
    obj.emplace("k", Json(1));

    obj = std::move(obj);

    obj.emplace("after", Json(2));
    CHK(obj.contains("after") == true);
}

// Executes all JsonObject lifecycle test cases.
static void run_tests() {
    RUN(default_construction_is_empty);
    RUN(copy_constructor_deep_copy);
    RUN(move_constructor_leaves_source_empty);
    RUN(copy_assignment_replaces_contents);
    RUN(move_assignment_replaces_contents_and_empties_source);
    RUN(self_copy_assignment_leaves_contents_unchanged);
    RUN(self_move_assignment_leaves_object_in_valid_state);
}

REGISTER_TEST_SUITE();
