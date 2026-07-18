// Json move semantics test suite.
//
// Coverage:
// - Move construction transfers contents and leaves the source Null
// - Move assignment overwrites the target and leaves the source Null
// - Self move assignment leaves the value unchanged
// - A moved-from value is safely reassignable afterward
// - Move construction and move assignment are noexcept

#include <type_traits>

#include <common/framework.h>

using namespace JsonPro;

// Verifies that move construction leaves the source as Null.
static void move_construction_leaves_source_null() {
    Json source("movable");
    Json moved(std::move(source));

    CHK(moved.isString() == true);
    CHK(moved.asString() == "movable");

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    CHK(source.isNull() == true);
}

// Verifies move construction correctly transfers Object contents.
static void move_construction_transfers_object_contents() {
    Json::ObjectType obj;
    obj.emplace("a", Json(1));
    obj.emplace("b", Json(2));

    Json source(std::move(obj));
    Json moved(std::move(source));

    CHK(moved.isObject() == true);
    CHK(moved.size() == 2);

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    CHK(source.isNull() == true);
}

// Verifies that move assignment overwrites the target and nulls the source.
static void move_assignment_overwrites_target() {
    Json source(Json::ArrayType{Json(1), Json(2), Json(3)});
    Json target("old value");

    target = std::move(source);

    CHK(target.isArray() == true);
    CHK(target.size() == 3);

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    CHK(source.isNull() == true);
}

// Verifies that self move assignment leaves the value unchanged.
static void self_move_assignment() {
    Json j("stable");
    j = std::move(j);

    CHK(j.isString() == true);
    CHK(j.asString() == "stable");
}

// Verifies a moved-from Json is left in a valid state that can be safely reassigned.
static void moved_from_value_is_safely_reassignable() {
    Json source("original");
    Json moved(std::move(source));

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    CHK(source.isNull() == true);

    source = Json("reused");
    CHK(source.isString() == true);
    CHK(source.asString() == "reused");
}

// Verifies move construction and move assignment are declared noexcept, so
// containers of Json (e.g. ArrayType) move rather than copy on reallocation.
static void move_operations_are_noexcept() {
    static_assert(std::is_nothrow_move_constructible<Json>::value,
                  "Json move construction must be noexcept");
    static_assert(std::is_nothrow_move_assignable<Json>::value,
                  "Json move assignment must be noexcept");

    CHK(true);
}

// Executes all move semantics test cases.
static void run_tests() {
    RUN(move_construction_leaves_source_null);
    RUN(move_construction_transfers_object_contents);
    RUN(move_assignment_overwrites_target);
    RUN(self_move_assignment);
    RUN(moved_from_value_is_safely_reassignable);
    RUN(move_operations_are_noexcept);
}

REGISTER_TEST_SUITE();
