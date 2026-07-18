// Json Size & Capacity Benchmark Suite
// Measures Json::size()/empty() performance against nlohmann::json.
//
// Each lambda performs exactly one call — BENCH sweeps iteration counts
// itself.
//
// Covers:
// - size() on an array
// - size() on an object
// - empty() on a non-empty object
// - empty() on an empty object

#include <common/framework.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Measures size() on a small array (5 elements).
static void bench_array_size() {
    const Json jpSrc(Json::ArrayType{Json(1), Json(2), Json(3), Json(4), Json(5)});
    const nlohmann::json njSrc = nlohmann::json::array({1, 2, 3, 4, 5});

    auto jp = [&] {
        std::size_t n = jpSrc.size();
        doNotOptimize(n);
    };

    auto nj = [&] {
        std::size_t n = njSrc.size();
        doNotOptimize(n);
    };

    BENCH("Array size()", jp, nj);
}

// Measures size() on a small object (5 members).
static void bench_object_size() {
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
        std::size_t n = jpSrc.size();
        doNotOptimize(n);
    };

    auto nj = [&] {
        std::size_t n = njSrc.size();
        doNotOptimize(n);
    };

    BENCH("Object size()", jp, nj);
}

// Measures empty() on a non-empty object.
static void bench_object_empty_false() {
    Json::ObjectType obj;
    obj.insert_or_assign("a", Json(1));
    const Json jpSrc(std::move(obj));

    const nlohmann::json njSrc = nlohmann::json::object({{"a", 1}});

    auto jp = [&] {
        bool e = jpSrc.asObject().empty();
        doNotOptimize(e);
    };

    auto nj = [&] {
        bool e = njSrc.empty();
        doNotOptimize(e);
    };

    BENCH("Object empty() (non-empty)", jp, nj);
}

// Measures empty() on an empty object.
static void bench_object_empty_true() {
    const Json jpSrc((Json::ObjectType()));
    const nlohmann::json njSrc = nlohmann::json::object();

    auto jp = [&] {
        bool e = jpSrc.asObject().empty();
        doNotOptimize(e);
    };

    auto nj = [&] {
        bool e = njSrc.empty();
        doNotOptimize(e);
    };

    BENCH("Object empty() (empty)", jp, nj);
}

// Executes all size & capacity benchmark cases.
static void run_benchmarks() {
    bench_array_size();
    std::cout << "\n";

    bench_object_size();
    std::cout << "\n";

    bench_object_empty_false();
    std::cout << "\n";

    bench_object_empty_true();
}

REGISTER_BENCH_SUITE();
