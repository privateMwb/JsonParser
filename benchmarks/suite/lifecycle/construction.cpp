// Json Construction Benchmark Suite
// Measures Json construction performance against nlohmann::json.
//
// Covers:
// - default (null) construction
// - bool construction
// - number construction
// - string construction (short / SSO, and long / heap-allocating)
// - array construction
// - object construction

#include <common/framework.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Measures default (null) construction performance.
static void bench_default_construction() {
    auto jp = [&] {
        Json j;
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j;
        doNotOptimize(j);
    };

    BENCH("default construction", jp, nj);
}

// Measures bool construction performance.
static void bench_bool_construction() {
    auto jp = [&] {
        Json j(true);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j(true);
        doNotOptimize(j);
    };

    BENCH("bool construction", jp, nj);
}

// Measures number construction performance.
static void bench_number_construction() {
    auto jp = [&] {
        Json j(42.0);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j(42.0);
        doNotOptimize(j);
    };

    BENCH("number construction", jp, nj);
}

// Measures string construction performance for a short (SSO-sized) string.
static void bench_short_string_construction() {
    const std::string s = "short";

    auto jp = [&] {
        Json j(s);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j(s);
        doNotOptimize(j);
    };

    BENCH("short string construction", jp, nj);
}

// Measures string construction performance for a long string that forces a
// heap allocation (past the small-string-optimization threshold).
static void bench_long_string_construction() {
    const std::string s(256, 'x');

    auto jp = [&] {
        Json j(s);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j(s);
        doNotOptimize(j);
    };

    BENCH("long string construction", jp, nj);
}

// Measures construction of a small array (5 elements).
static void bench_array_construction() {
    auto jp = [&] {
        Json j(Json::ArrayType{Json(1), Json(2), Json(3), Json(4), Json(5)});
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j = nlohmann::json::array({1, 2, 3, 4, 5});
        doNotOptimize(j);
    };

    BENCH("array construction", jp, nj);
}

// Measures construction of a small object (5 members).
static void bench_object_construction() {
    auto jp = [&] {
        Json::ObjectType obj;
        obj.insert_or_assign("a", Json(1));
        obj.insert_or_assign("b", Json(2));
        obj.insert_or_assign("c", Json(3));
        obj.insert_or_assign("d", Json(4));
        obj.insert_or_assign("e", Json(5));
        Json j(std::move(obj));
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j =
            nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}});
        doNotOptimize(j);
    };

    BENCH("object construction", jp, nj);
}

// Executes all construction benchmark cases.
static void run_benchmarks() {
    bench_default_construction();
    std::cout << "\n";

    bench_bool_construction();
    std::cout << "\n";

    bench_number_construction();
    std::cout << "\n";

    bench_short_string_construction();
    std::cout << "\n";

    bench_long_string_construction();
    std::cout << "\n";

    bench_array_construction();
    std::cout << "\n";

    bench_object_construction();
}

REGISTER_BENCH_SUITE();
