// Json nested navigation integration test suite.
//
// Coverage:
// - Chained operator[] navigation through a parsed array-of-objects document
// - Mutating a deeply nested value through a chained access path
// - Mixing at() and operator[] across a nested navigation chain
// - const-qualified navigation through a parsed document
// - contains() gating conditional navigation to avoid a throw
// - Iterating a two-dimensional array (array of arrays)

#include <common/framework.h>

using namespace JsonPro;

// Verifies chained operator[] navigates a parsed array-of-objects document.
static void navigate_parsed_array_of_objects() {
    Json j = Json::parse(R"({
        "users": [
            {"name": "Alice", "age": 30},
            {"name": "Bob",   "age": 25}
        ]
    })");

    CHK(j["users"].size() == 2);
    CHK(j["users"][0]["name"].asString() == "Alice");
    CHK(j["users"][1]["age"].asNumber() == 25.0);
}

// Verifies a value at the end of a chained access path can be mutated in place.
static void mutate_deep_value_through_chained_access() {
    Json j = Json::parse(R"({"a": {"b": {"c": 1}}})");

    j["a"]["b"]["c"] = Json(99);

    CHK(j["a"]["b"]["c"].asNumber() == 99.0);
}

// Verifies at() and operator[] can be mixed within a single navigation chain,
// and that a bounds violation partway through the chain still throws.
static void chained_at_bounds_checked_navigation() {
    Json j = Json::parse(R"({"matrix": [[1, 2], [3, 4]]})");

    CHK(j.at("matrix").at(1).at(0).asNumber() == 3.0);
    CHK_THROWS(j.at("matrix").at(5), std::runtime_error);
}

// Verifies navigation through a const-qualified parsed document.
static void const_navigation_through_parsed_document() {
    const Json j = Json::parse(R"({"config": {"enabled": true, "limits": [1, 2, 3]}})");

    CHK(j["config"]["enabled"].asBool() == true);
    CHK(j["config"]["limits"][2].asNumber() == 3.0);
    CHK_THROWS(j["config"]["missing"], std::runtime_error);
}

// Verifies contains() can gate conditional navigation so a missing key
// never triggers a throw.
static void contains_guides_conditional_navigation() {
    Json j = Json::parse(R"({"a": 1})");

    CHK(j.contains("a") == true);
    CHK(j.contains("b") == false);

    if (j.contains("a"))
        CHK(j["a"].asNumber() == 1.0);

    if (j.contains("b"))
        CHK(false); // unreachable: contains() correctly gated access
}

// Verifies navigating a two-dimensional array (array of arrays).
static void navigate_array_of_arrays_matrix() {
    Json j = Json::parse("[[1, 2, 3], [4, 5, 6], [7, 8, 9]]");

    double sum = 0.0;
    for (std::size_t row = 0; row < j.size(); ++row)
        for (std::size_t col = 0; col < j[row].size(); ++col)
            sum += j[row][col].asNumber();

    CHK(sum == 45.0);
    CHK(j[1][2].asNumber() == 6.0);
}

// Executes all nested navigation test cases.
static void run_tests() {
    RUN(navigate_parsed_array_of_objects);
    RUN(mutate_deep_value_through_chained_access);
    RUN(chained_at_bounds_checked_navigation);
    RUN(const_navigation_through_parsed_document);
    RUN(contains_guides_conditional_navigation);
    RUN(navigate_array_of_arrays_matrix);
}

REGISTER_TEST_SUITE();
