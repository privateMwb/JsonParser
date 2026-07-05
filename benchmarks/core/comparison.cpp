// JsonPro comparison benchmarks.
//
// Measures operator== cost across types and container sizes, including
// best-case (early-exit) and worst-case (full-scan) mismatch positions.
//
// Covers:
// - Scalar comparison: number and string, equal vs unequal
// - Cross-type comparison (index mismatch short-circuit)
// - Array comparison: equal, mismatch at first element, mismatch at last
// - Object comparison: equal vs unequal, at fixed key count

#include <common/framework.h>

#include <string>

using namespace JsonPro;

// Measures scalar equality/inequality for numbers and strings.
static void bench_compare_scalars() {
    Json a(1.0), b(1.0), c(2.0);

    auto numEq = [&] {
        bool r = (a == b);
        doNotOptimize(r);
    };
    BENCH("Compare number, equal", LARGE, numEq);

    auto numNeq = [&] {
        bool r = (a == c);
        doNotOptimize(r);
    };
    BENCH("Compare number, unequal", LARGE, numNeq);

    Json s1("hello"), s2("hello"), s3("world");

    auto strEq = [&] {
        bool r = (s1 == s2);
        doNotOptimize(r);
    };
    BENCH("Compare string, equal", LARGE, strEq);

    auto strNeq = [&] {
        bool r = (s1 == s3);
        doNotOptimize(r);
    };
    BENCH("Compare string, unequal", LARGE, strNeq);
}

// Measures cross-type comparison, which short-circuits on variant index mismatch.
static void bench_compare_cross_type() {
    Json number(1.0);
    Json string("1");

    auto expr = [&] {
        bool r = (number == string);
        doNotOptimize(r);
    };
    BENCH("Compare cross-type (index mismatch)", LARGE, expr);
}

// Measures array comparison: equal, mismatch at the first element
// (best case, early exit), and mismatch at the last element (worst
// case, full scan).
static void bench_compare_arrays() {
    Json::ArrayType base;
    for (int i = 0; i < 100; ++i)
        base.emplace_back(i);

    Json a(base);
    Json b(base);

    Json::ArrayType firstMismatch = base;
    firstMismatch[0] = Json(-1);
    Json c(std::move(firstMismatch));

    Json::ArrayType lastMismatch = base;
    lastMismatch[99] = Json(-1);
    Json d(std::move(lastMismatch));

    auto eq = [&] {
        bool r = (a == b);
        doNotOptimize(r);
    };
    BENCH("Compare array, equal (100 elems)", MEDIUM, eq);

    auto early = [&] {
        bool r = (a == c);
        doNotOptimize(r);
    };
    BENCH("Compare array, mismatch at first", LARGE, early);

    auto late = [&] {
        bool r = (a == d);
        doNotOptimize(r);
    };
    BENCH("Compare array, mismatch at last", MEDIUM, late);
}

// Measures object comparison: equal vs unequal at a fixed key count.
static void bench_compare_objects() {
    Json::ObjectType base;
    for (int i = 0; i < 100; ++i)
        base.emplace("k" + std::to_string(i), Json(i));

    Json a(base);
    Json b(base);

    Json::ObjectType mismatched = base;
    mismatched["k50"] = Json(-1);
    Json c(std::move(mismatched));

    auto eq = [&] {
        bool r = (a == b);
        doNotOptimize(r);
    };
    BENCH("Compare object, equal (100 keys)", MEDIUM, eq);

    auto neq = [&] {
        bool r = (a == c);
        doNotOptimize(r);
    };
    BENCH("Compare object, unequal (100 keys)", MEDIUM, neq);
}

// Runs all comparison benchmarks.
static void run_benchmarks() {
    bench_compare_scalars();
    std::cout << "\n";

    bench_compare_cross_type();
    std::cout << "\n";

    bench_compare_arrays();
    std::cout << "\n";

    bench_compare_objects();
}

REGISTER_BENCH_SUITE();
