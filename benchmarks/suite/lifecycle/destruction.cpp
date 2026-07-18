// Json Destruction Benchmark Suite
// Measures Json destruction performance against nlohmann::json.
//
// Each case builds a batch of values into a vector, then times only the
// vector's clear() — the point at which every element's destructor runs.
// Building the batch happens inside the same timed lambda (this harness
// has no separate setup/teardown phase to exclude it from timing), so the
// absolute numbers include some construction cost; treat the *relative*
// gap between JsonPro and nlohmann::json here as the meaningful signal,
// since construction cost is already isolated separately in construction.cpp.
//
// Covers:
// - default (null) destruction
// - bool destruction
// - number destruction
// - string destruction (short / SSO, and long / heap-allocating)
// - array destruction
// - object destruction

#include <common/framework.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

#include <vector>

using namespace JsonPro;

// Measures default (null) destruction performance.
static void bench_default_destruction() {
    auto jp = [&] {
        std::vector<Json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back();

        batch.clear();
    };

    auto nj = [&] {
        std::vector<nlohmann::json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back();

        batch.clear();
    };

    BENCH("default destruction", jp, nj);
}

// Measures bool destruction performance.
static void bench_bool_destruction() {
    auto jp = [&] {
        std::vector<Json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(true);

        batch.clear();
    };

    auto nj = [&] {
        std::vector<nlohmann::json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(true);

        batch.clear();
    };

    BENCH("bool destruction", jp, nj);
}

// Measures number destruction performance.
static void bench_number_destruction() {
    auto jp = [&] {
        std::vector<Json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(42.0);

        batch.clear();
    };

    auto nj = [&] {
        std::vector<nlohmann::json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(42.0);

        batch.clear();
    };

    BENCH("number destruction", jp, nj);
}

// Measures string destruction performance for a short (SSO-sized) string.
static void bench_short_string_destruction() {
    const std::string s = "short";

    auto jp = [&] {
        std::vector<Json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(s);

        batch.clear();
    };

    auto nj = [&] {
        std::vector<nlohmann::json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(s);

        batch.clear();
    };

    BENCH("short string destruction", jp, nj);
}

// Measures string destruction performance for a long string that forced a
// heap allocation on construction.
static void bench_long_string_destruction() {
    const std::string s(256, 'x');

    auto jp = [&] {
        std::vector<Json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(s);

        batch.clear();
    };

    auto nj = [&] {
        std::vector<nlohmann::json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(s);

        batch.clear();
    };

    BENCH("long string destruction", jp, nj);
}

// Measures destruction performance for a small array (5 elements).
static void bench_array_destruction() {
    auto jp = [&] {
        std::vector<Json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(Json::ArrayType{Json(1), Json(2), Json(3), Json(4), Json(5)});

        batch.clear();
    };

    auto nj = [&] {
        std::vector<nlohmann::json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(nlohmann::json::array({1, 2, 3, 4, 5}));

        batch.clear();
    };

    BENCH("array destruction", jp, nj);
}

// Measures destruction performance for a small object (5 members).
static void bench_object_destruction() {
    auto jp = [&] {
        std::vector<Json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i) {
            Json::ObjectType obj;
            obj.insert_or_assign("a", Json(1));
            obj.insert_or_assign("b", Json(2));
            obj.insert_or_assign("c", Json(3));
            obj.insert_or_assign("d", Json(4));
            obj.insert_or_assign("e", Json(5));
            batch.emplace_back(std::move(obj));
        }

        batch.clear();
    };

    auto nj = [&] {
        std::vector<nlohmann::json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i) {
            batch.emplace_back(
                nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}}));
        }

        batch.clear();
    };

    BENCH("object destruction", jp, nj);
}

// Executes all destruction benchmark cases.
static void run_benchmarks() {
    bench_default_destruction();
    std::cout << "\n";

    bench_bool_destruction();
    std::cout << "\n";

    bench_number_destruction();
    std::cout << "\n";

    bench_short_string_destruction();
    std::cout << "\n";

    bench_long_string_destruction();
    std::cout << "\n";

    bench_array_destruction();
    std::cout << "\n";

    bench_object_destruction();
}

REGISTER_BENCH_SUITE();
