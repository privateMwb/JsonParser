// JsonPro navigation benchmarks.
//
// Measures element access cost on pre-built (non-parsed) Json trees.
//
// Covers:
// - Array access: operator[] vs at() (bounds-check overhead)
// - Object access: operator[] vs at() (hash lookup overhead)
// - Deep nested traversal through multiple levels
// - Array vs object access cost at equal container size

#include <common/framework.h>

#include <string>

using namespace JsonPro;

// Measures array operator[] vs at() on a 1000-element array.
static void bench_array_access() {
    Json arr(Json::ArrayType{});
    for (int i = 0; i < 1000; ++i)
        arr.asArray().push_back(Json(i));

    auto bracket = [&] {
        double v = arr[500].asNumber();
        doNotOptimize(v);
    };
    BENCH("Array operator[]", LARGE, bracket);

    auto atExpr = [&] {
        double v = arr.at(500).asNumber();
        doNotOptimize(v);
    };
    BENCH("Array at()", LARGE, atExpr);
}

// Measures object operator[] vs at() on a 1000-key object.
static void bench_object_access() {
    Json::ObjectType obj;
    for (int i = 0; i < 1000; ++i)
        obj.emplace("k" + std::to_string(i), Json(i));

    Json object(std::move(obj));

    auto bracket = [&] {
        double v = object["k500"].asNumber();
        doNotOptimize(v);
    };
    BENCH("Object operator[]", LARGE, bracket);

    auto atExpr = [&] {
        double v = object.at("k500").asNumber();
        doNotOptimize(v);
    };
    BENCH("Object at()", LARGE, atExpr);
}

// Measures traversal through three nested levels of an object.
static void bench_deep_traversal() {
    Json::ObjectType inner;
    inner.emplace("c", Json(42));

    Json::ObjectType mid;
    mid.emplace("b", Json(std::move(inner)));

    Json::ObjectType outer;
    outer.emplace("a", Json(std::move(mid)));

    Json root(std::move(outer));

    auto expr = [&] {
        double v = root["a"]["b"]["c"].asNumber();
        doNotOptimize(v);
    };
    BENCH("Deep traversal, 3 levels", LARGE, expr);
}

// Measures array index access vs object key lookup at equal container size.
static void bench_array_vs_object_access() {
    Json arr(Json::ArrayType{});
    for (int i = 0; i < 100; ++i)
        arr.asArray().push_back(Json(i));

    Json::ObjectType obj;
    for (int i = 0; i < 100; ++i)
        obj.emplace("k" + std::to_string(i), Json(i));

    Json object(std::move(obj));

    auto arrExpr = [&] {
        double v = arr[99].asNumber();
        doNotOptimize(v);
    };
    BENCH("Array access, 100 elements", LARGE, arrExpr);

    auto objExpr = [&] {
        double v = object["k99"].asNumber();
        doNotOptimize(v);
    };
    BENCH("Object access, 100 keys", LARGE, objExpr);
}

// Runs all navigation benchmarks.
static void run_benchmarks() {
    bench_array_access();
    std::cout << "\n";

    bench_object_access();
    std::cout << "\n";

    bench_deep_traversal();
    std::cout << "\n";

    bench_array_vs_object_access();
}

REGISTER_BENCH_SUITE();
