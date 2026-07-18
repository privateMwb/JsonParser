// Json Move Benchmark Suite
// Measures Json move-construction performance against nlohmann::json.
//
// Each case rebuilds a fresh source value every iteration (moves are
// destructive, so a single source can't be reused across iterations the
// way copy.cpp's shared source can) and immediately moves from it. Both
// libraries pay the same rebuild cost, so the timing delta still isolates
// the move path.
//
// Covers:
// - default (null) move
// - bool move
// - number move
// - string move (short / SSO, and long / heap-allocating)
// - array move
// - object move

#include <common/framework.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Measures default (null) move performance.
static void bench_default_move() {
    auto jp = [&] {
        Json src;
        Json j(std::move(src));
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json src;
        nlohmann::json j(std::move(src));
        doNotOptimize(j);
    };

    BENCH("default move", jp, nj);
}

// Measures bool move performance.
static void bench_bool_move() {
    auto jp = [&] {
        Json src(true);
        Json j(std::move(src));
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json src(true);
        nlohmann::json j(std::move(src));
        doNotOptimize(j);
    };

    BENCH("bool move", jp, nj);
}

// Measures number move performance.
static void bench_number_move() {
    auto jp = [&] {
        Json src(42.0);
        Json j(std::move(src));
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json src(42.0);
        nlohmann::json j(std::move(src));
        doNotOptimize(j);
    };

    BENCH("number move", jp, nj);
}

// Measures string move performance for a short (SSO-sized) string.
static void bench_short_string_move() {
    auto jp = [&] {
        Json src(std::string("short"));
        Json j(std::move(src));
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json src(std::string("short"));
        nlohmann::json j(std::move(src));
        doNotOptimize(j);
    };

    BENCH("short string move", jp, nj);
}

// Measures string move performance for a long string that would force a
// heap allocation if copied — a move should sidestep that entirely.
static void bench_long_string_move() {
    const std::string s(256, 'x');

    auto jp = [&] {
        Json src(s);
        Json j(std::move(src));
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json src(s);
        nlohmann::json j(std::move(src));
        doNotOptimize(j);
    };

    BENCH("long string move", jp, nj);
}

// Measures move performance for a small array (5 elements).
static void bench_array_move() {
    auto jp = [&] {
        Json src(Json::ArrayType{Json(1), Json(2), Json(3), Json(4), Json(5)});
        Json j(std::move(src));
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json src = nlohmann::json::array({1, 2, 3, 4, 5});
        nlohmann::json j(std::move(src));
        doNotOptimize(j);
    };

    BENCH("array move", jp, nj);
}

// Measures move performance for a small object (5 members).
static void bench_object_move() {
    auto jp = [&] {
        Json::ObjectType obj;
        obj.insert_or_assign("a", Json(1));
        obj.insert_or_assign("b", Json(2));
        obj.insert_or_assign("c", Json(3));
        obj.insert_or_assign("d", Json(4));
        obj.insert_or_assign("e", Json(5));

        Json src(std::move(obj));
        Json j(std::move(src));
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json src =
            nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}});
        nlohmann::json j(std::move(src));
        doNotOptimize(j);
    };

    BENCH("object move", jp, nj);
}

// Executes all move benchmark cases.
static void run_benchmarks() {
    bench_default_move();
    std::cout << "\n";

    bench_bool_move();
    std::cout << "\n";

    bench_number_move();
    std::cout << "\n";

    bench_short_string_move();
    std::cout << "\n";

    bench_long_string_move();
    std::cout << "\n";

    bench_array_move();
    std::cout << "\n";

    bench_object_move();
}

REGISTER_BENCH_SUITE();
