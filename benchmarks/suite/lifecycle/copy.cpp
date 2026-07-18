// Json Copy Benchmark Suite
// Measures Json copy-construction performance against nlohmann::json.
//
// Each case copies a fixed, pre-built source value repeatedly, so the
// measured cost is copying alone — the source's own construction happens
// once, outside the timed loop.
//
// Covers:
// - default (null) copy
// - bool copy
// - number copy
// - string copy (short / SSO, and long / heap-allocating)
// - array copy
// - object copy

#include <common/framework.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Measures default (null) copy performance.
static void bench_default_copy() {
    const Json jpSrc;
    const nlohmann::json njSrc;

    auto jp = [&] {
        Json j(jpSrc);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j(njSrc);
        doNotOptimize(j);
    };

    BENCH("default copy", jp, nj);
}

// Measures bool copy performance.
static void bench_bool_copy() {
    const Json jpSrc(true);
    const nlohmann::json njSrc(true);

    auto jp = [&] {
        Json j(jpSrc);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j(njSrc);
        doNotOptimize(j);
    };

    BENCH("bool copy", jp, nj);
}

// Measures number copy performance.
static void bench_number_copy() {
    const Json jpSrc(42.0);
    const nlohmann::json njSrc(42.0);

    auto jp = [&] {
        Json j(jpSrc);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j(njSrc);
        doNotOptimize(j);
    };

    BENCH("number copy", jp, nj);
}

// Measures string copy performance for a short (SSO-sized) string.
static void bench_short_string_copy() {
    const Json jpSrc(std::string("short"));
    const nlohmann::json njSrc(std::string("short"));

    auto jp = [&] {
        Json j(jpSrc);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j(njSrc);
        doNotOptimize(j);
    };

    BENCH("short string copy", jp, nj);
}

// Measures string copy performance for a long string that forces a heap
// allocation (past the small-string-optimization threshold).
static void bench_long_string_copy() {
    const std::string s(256, 'x');
    const Json jpSrc(s);
    const nlohmann::json njSrc(s);

    auto jp = [&] {
        Json j(jpSrc);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j(njSrc);
        doNotOptimize(j);
    };

    BENCH("long string copy", jp, nj);
}

// Measures copy performance for a small array (5 elements).
static void bench_array_copy() {
    const Json jpSrc(Json::ArrayType{Json(1), Json(2), Json(3), Json(4), Json(5)});
    const nlohmann::json njSrc = nlohmann::json::array({1, 2, 3, 4, 5});

    auto jp = [&] {
        Json j(jpSrc);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j(njSrc);
        doNotOptimize(j);
    };

    BENCH("array copy", jp, nj);
}

// Measures copy performance for a small object (5 members).
static void bench_object_copy() {
    Json::ObjectType obj;
    obj.insert_or_assign("a", Json(1));
    obj.insert_or_assign("b", Json(2));
    obj.insert_or_assign("c", Json(3));
    obj.insert_or_assign("d", Json(4));
    obj.insert_or_assign("e", Json(5));

    const Json jpSrc(std::move(obj));
    const nlohmann::json njSrc =
        nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}});

    auto jp = [&] {
        Json j(jpSrc);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j(njSrc);
        doNotOptimize(j);
    };

    BENCH("object copy", jp, nj);
}

// Executes all copy benchmark cases.
static void run_benchmarks() {
    bench_default_copy();
    std::cout << "\n";

    bench_bool_copy();
    std::cout << "\n";

    bench_number_copy();
    std::cout << "\n";

    bench_short_string_copy();
    std::cout << "\n";

    bench_long_string_copy();
    std::cout << "\n";

    bench_array_copy();
    std::cout << "\n";

    bench_object_copy();
}

REGISTER_BENCH_SUITE();
