// JsonPro construction benchmarks.
//
// Measures the cost of building Json values directly, without going
// through the parser.
//
// Covers:
// - Scalar construction (bool, number, string)
// - String construction: copy vs move
// - Array construction via repeated push_back
// - Object construction: insert_or_assign vs operator[]
// - Copy-construct vs move-construct of a populated array

#include <common/framework.h>

#include <string>

using namespace JsonPro;

// Measures constructing each scalar type.
static void bench_construct_scalar() {
    auto b = [&] {
        Json j(true);
        doNotOptimize(j);
    };
    BENCH("Construct bool", LARGE, b);

    auto n = [&] {
        Json j(3.14);
        doNotOptimize(j);
    };
    BENCH("Construct number", LARGE, n);

    auto s = [&] {
        Json j("hello");
        doNotOptimize(j);
    };
    BENCH("Construct string (c-string)", LARGE, s);
}

// Measures string construction: copying an lvalue vs moving an rvalue.
static void bench_construct_string_copy_vs_move() {
    auto copy = [&] {
        std::string s = "hello world";
        Json j(s);
        doNotOptimize(j);
    };
    BENCH("Construct string, copy", LARGE, copy);

    auto move = [&] {
        std::string s = "hello world";
        Json j(std::move(s));
        doNotOptimize(j);
    };
    BENCH("Construct string, move", LARGE, move);
}

// Measures building an array of 100 elements via repeated push_back.
static void bench_build_array_push_back() {
    auto expr = [&] {
        Json::ArrayType arr;
        for (int i = 0; i < 100; ++i)
            arr.push_back(Json(i));

        Json j(std::move(arr));
        doNotOptimize(j);
    };
    BENCH("Build array, 100 push_back", MEDIUM, expr);
}

// Measures building a 100-key object via insert_or_assign vs operator[].
// insert_or_assign does a single lookup+insert; operator[] does a
// default-construct-then-assign (two operations) on first insertion.
static void bench_build_object_insert_or_assign_vs_bracket() {
    auto viaInsert = [&] {
        Json::ObjectType obj;
        for (int i = 0; i < 100; ++i)
            obj.insert_or_assign("k" + std::to_string(i), Json(i));

        Json j(std::move(obj));
        doNotOptimize(j);
    };
    BENCH("Build object, insert_or_assign", MEDIUM, viaInsert);

    auto viaBracket = [&] {
        Json::ObjectType obj;
        for (int i = 0; i < 100; ++i)
            obj[("k" + std::to_string(i))] = Json(i);

        Json j(std::move(obj));
        doNotOptimize(j);
    };
    BENCH("Build object, operator[]", MEDIUM, viaBracket);
}

// Measures copy-construct vs move-construct of a populated 100-element array.
// The move case also pays for a fresh copy each iteration (to have a
// distinct source to move from), so it reflects "copy + move" vs
// "copy" alone rather than move in isolation.
static void bench_copy_vs_move_populated_array() {
    Json source(Json::ArrayType{});
    for (int i = 0; i < 100; ++i)
        source.asArray().push_back(Json(i));

    auto copy = [&] {
        Json j(source);
        doNotOptimize(j);
    };
    BENCH("Copy-construct array (100 elems)", MEDIUM, copy);

    auto move = [&] {
        Json temp = source;
        Json j(std::move(temp));
        doNotOptimize(j);
    };
    BENCH("Copy then move-construct array", MEDIUM, move);
}

// Runs all construction benchmarks.
static void run_benchmarks() {
    bench_construct_scalar();
    std::cout << "\n";

    bench_construct_string_copy_vs_move();
    std::cout << "\n";

    bench_build_array_push_back();
    std::cout << "\n";

    bench_build_object_insert_or_assign_vs_bracket();
    std::cout << "\n";

    bench_copy_vs_move_populated_array();
}

REGISTER_BENCH_SUITE();
