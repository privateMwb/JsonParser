// Json Comparison Benchmark Suite
// Measures Json::operator== performance against nlohmann::json.
//
// Each lambda performs exactly one comparison — BENCH sweeps iteration
// counts itself.
//
// Covers:
// - equal numbers (trivial scalar compare)
// - equal strings
// - equal arrays
// - equal objects
// - unequal objects (mismatch found early vs. late)

#include <common/framework.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Measures comparing two equal numbers.
static void bench_compare_equal_numbers() {
    const Json jpA(42.0), jpB(42.0);
    const nlohmann::json njA(42.0), njB(42.0);

    auto jp = [&] {
        bool eq = (jpA == jpB);
        doNotOptimize(eq);
    };

    auto nj = [&] {
        bool eq = (njA == njB);
        doNotOptimize(eq);
    };

    BENCH("Compare Equal Numbers", jp, nj);
}

// Measures comparing two equal strings.
static void bench_compare_equal_strings() {
    const Json jpA(std::string("a moderately sized benchmark string"));
    const Json jpB(std::string("a moderately sized benchmark string"));
    const nlohmann::json njA(std::string("a moderately sized benchmark string"));
    const nlohmann::json njB(std::string("a moderately sized benchmark string"));

    auto jp = [&] {
        bool eq = (jpA == jpB);
        doNotOptimize(eq);
    };

    auto nj = [&] {
        bool eq = (njA == njB);
        doNotOptimize(eq);
    };

    BENCH("Compare Equal Strings", jp, nj);
}

// Measures comparing two equal small arrays (5 elements).
static void bench_compare_equal_arrays() {
    const Json jpA(Json::ArrayType{Json(1), Json(2), Json(3), Json(4), Json(5)});
    const Json jpB(Json::ArrayType{Json(1), Json(2), Json(3), Json(4), Json(5)});
    const nlohmann::json njA = nlohmann::json::array({1, 2, 3, 4, 5});
    const nlohmann::json njB = nlohmann::json::array({1, 2, 3, 4, 5});

    auto jp = [&] {
        bool eq = (jpA == jpB);
        doNotOptimize(eq);
    };

    auto nj = [&] {
        bool eq = (njA == njB);
        doNotOptimize(eq);
    };

    BENCH("Compare Equal Arrays", jp, nj);
}

// Measures comparing two equal small objects (5 members).
static void bench_compare_equal_objects() {
    auto buildObj = [] {
        Json::ObjectType obj;
        obj.insert_or_assign("a", Json(1));
        obj.insert_or_assign("b", Json(2));
        obj.insert_or_assign("c", Json(3));
        obj.insert_or_assign("d", Json(4));
        obj.insert_or_assign("e", Json(5));
        return obj;
    };

    const Json jpA(buildObj());
    const Json jpB(buildObj());
    const nlohmann::json njA =
        nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}});
    const nlohmann::json njB =
        nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}});

    auto jp = [&] {
        bool eq = (jpA == jpB);
        doNotOptimize(eq);
    };

    auto nj = [&] {
        bool eq = (njA == njB);
        doNotOptimize(eq);
    };

    BENCH("Compare Equal Objects", jp, nj);
}

// Measures comparing two unequal small objects (5 members, last key
// differs) — the mismatch is only found after walking most of the object.
static void bench_compare_unequal_objects() {
    Json::ObjectType objA;
    objA.insert_or_assign("a", Json(1));
    objA.insert_or_assign("b", Json(2));
    objA.insert_or_assign("c", Json(3));
    objA.insert_or_assign("d", Json(4));
    objA.insert_or_assign("e", Json(5));

    Json::ObjectType objB;
    objB.insert_or_assign("a", Json(1));
    objB.insert_or_assign("b", Json(2));
    objB.insert_or_assign("c", Json(3));
    objB.insert_or_assign("d", Json(4));
    objB.insert_or_assign("e", Json(999));

    const Json jpA(std::move(objA));
    const Json jpB(std::move(objB));

    const nlohmann::json njA =
        nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}});
    const nlohmann::json njB =
        nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 999}});

    auto jp = [&] {
        bool eq = (jpA == jpB);
        doNotOptimize(eq);
    };

    auto nj = [&] {
        bool eq = (njA == njB);
        doNotOptimize(eq);
    };

    BENCH("Compare Unequal Objects", jp, nj);
}

// Executes all comparison benchmark cases.
static void run_benchmarks() {
    bench_compare_equal_numbers();
    std::cout << "\n";

    bench_compare_equal_strings();
    std::cout << "\n";

    bench_compare_equal_arrays();
    std::cout << "\n";

    bench_compare_equal_objects();
    std::cout << "\n";

    bench_compare_unequal_objects();
}

REGISTER_BENCH_SUITE();
