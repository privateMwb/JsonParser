// Maximum nesting depth regression test suite.
//
// Guards against off-by-one and depth/breadth-confusion pitfalls around
// Parser::kMaxDepth (512). depth_ is incremented once per parseArray() or
// parseObject() call and decremented on return, so it tracks true recursive
// nesting -- not the number of sibling elements at any one level.
//
// Coverage:
// - Array nesting exactly at kMaxDepth is accepted; one level beyond is rejected
// - Object nesting exactly at kMaxDepth is accepted; one level beyond is rejected
// - Mixed alternating array/object nesting is bounded by the same limit
// - Many siblings at a shallow depth do not trigger the depth limit,
//   regardless of how many elements are present

#include <common/framework.h>

using namespace JsonPro;

namespace {
constexpr int kMaxDepth = 512; // must match Parser::kMaxDepth
}

// Verifies array nesting exactly at the maximum depth is accepted.
static void array_nesting_at_max_depth_accepted() {
    std::string json(static_cast<std::size_t>(kMaxDepth), '[');
    json.append(static_cast<std::size_t>(kMaxDepth), ']');

    Json j = Json::parse(json);
    CHK(j.isArray() == true);
}

// Verifies array nesting one level beyond the maximum depth is rejected.
static void array_nesting_beyond_max_depth_rejected() {
    constexpr int depth = kMaxDepth + 1;

    std::string json(static_cast<std::size_t>(depth), '[');
    json.append(static_cast<std::size_t>(depth), ']');

    CHK_THROWS(Json::parse(json), std::runtime_error);
}

// Verifies object nesting exactly at the maximum depth is accepted.
static void object_nesting_at_max_depth_accepted() {
    std::string json;
    for (int i = 0; i < kMaxDepth; ++i)
        json += R"({"k":)";
    json += "null";
    for (int i = 0; i < kMaxDepth; ++i)
        json += '}';

    Json j = Json::parse(json);
    CHK(j.isObject() == true);
}

// Verifies object nesting one level beyond the maximum depth is rejected.
static void object_nesting_beyond_max_depth_rejected() {
    constexpr int depth = kMaxDepth + 1;

    std::string json;
    for (int i = 0; i < depth; ++i)
        json += R"({"k":)";
    json += "null";
    for (int i = 0; i < depth; ++i)
        json += '}';

    CHK_THROWS(Json::parse(json), std::runtime_error);
}

// Verifies mixed alternating array/object nesting is bounded by the same
// depth limit, and not tracked separately per structural kind.
static void mixed_alternating_nesting_bounded() {
    constexpr int depth = kMaxDepth + 1;

    std::string json;
    for (int i = 0; i < depth; ++i)
        json += (i % 2 == 0) ? "[" : R"({"k":)";
    json += "null";
    for (int i = depth - 1; i >= 0; --i)
        json += (i % 2 == 0) ? "]" : "}";

    CHK_THROWS(Json::parse(json), std::runtime_error);
}

// Verifies a wide array with many thousands of siblings at a shallow depth
// parses without tripping the depth limit -- breadth is unbounded, only
// recursive nesting is.
static void many_siblings_at_shallow_depth_not_limited() {
    std::string json = "[";
    for (int i = 0; i < 10000; ++i) {
        if (i != 0)
            json += ',';
        json += std::to_string(i);
    }
    json += "]";

    Json j = Json::parse(json);
    CHK(j.size() == 10000);
    CHK(j[9999].asNumber() == 9999.0);
}

// Executes all maximum nesting depth regression test cases.
static void run_tests() {
    RUN(array_nesting_at_max_depth_accepted);
    RUN(array_nesting_beyond_max_depth_rejected);
    RUN(object_nesting_at_max_depth_accepted);
    RUN(object_nesting_beyond_max_depth_rejected);
    RUN(mixed_alternating_nesting_bounded);
    RUN(many_siblings_at_shallow_depth_not_limited);
}

REGISTER_TEST_SUITE();
