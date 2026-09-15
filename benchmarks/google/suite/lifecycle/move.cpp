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

#include <benchmark/benchmark.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Measures default (null) move performance.
static void DefaultMove_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json src;
        Json j(std::move(src));
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(DefaultMove_JsonPro);

static void DefaultMove_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json src;
        nlohmann::json j(std::move(src));
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(DefaultMove_Nlohmann);

// Measures bool move performance.
static void BoolMove_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json src(true);
        Json j(std::move(src));
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(BoolMove_JsonPro);

static void BoolMove_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json src(true);
        nlohmann::json j(std::move(src));
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(BoolMove_Nlohmann);

// Measures number move performance.
static void NumberMove_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json src(42.0);
        Json j(std::move(src));
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(NumberMove_JsonPro);

static void NumberMove_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json src(42.0);
        nlohmann::json j(std::move(src));
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(NumberMove_Nlohmann);

// Measures string move performance for a short (SSO-sized) string.
static void ShortStringMove_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json src(std::string("short"));
        Json j(std::move(src));
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ShortStringMove_JsonPro);

static void ShortStringMove_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json src(std::string("short"));
        nlohmann::json j(std::move(src));
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ShortStringMove_Nlohmann);

// Measures string move performance for a long string that would force a
// heap allocation if copied — a move should sidestep that entirely.
static void LongStringMove_JsonPro(benchmark::State& state) {
    const std::string s(256, 'x');

    for (auto _ : state) {
        Json src(s);
        Json j(std::move(src));
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(LongStringMove_JsonPro);

static void LongStringMove_Nlohmann(benchmark::State& state) {
    const std::string s(256, 'x');

    for (auto _ : state) {
        nlohmann::json src(s);
        nlohmann::json j(std::move(src));
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(LongStringMove_Nlohmann);

// Measures move performance for a small array (5 elements).
static void ArrayMove_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json src(Json::ArrayType{Json(1), Json(2), Json(3), Json(4), Json(5)});
        Json j(std::move(src));
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ArrayMove_JsonPro);

static void ArrayMove_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json src = nlohmann::json::array({1, 2, 3, 4, 5});
        nlohmann::json j(std::move(src));
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ArrayMove_Nlohmann);

// Measures move performance for a small object (5 members).
static void ObjectMove_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json::ObjectType obj;
        obj.insert_or_assign("a", Json(1));
        obj.insert_or_assign("b", Json(2));
        obj.insert_or_assign("c", Json(3));
        obj.insert_or_assign("d", Json(4));
        obj.insert_or_assign("e", Json(5));

        Json src(std::move(obj));
        Json j(std::move(src));
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ObjectMove_JsonPro);

static void ObjectMove_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json src =
            nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}});
        nlohmann::json j(std::move(src));
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ObjectMove_Nlohmann);
