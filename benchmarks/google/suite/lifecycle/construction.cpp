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

#include <benchmark/benchmark.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Measures default (null) construction performance.
static void DefaultConstruction_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json j;
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(DefaultConstruction_JsonPro);

static void DefaultConstruction_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json j;
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(DefaultConstruction_Nlohmann);

// Measures bool construction performance.
static void BoolConstruction_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json j(true);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(BoolConstruction_JsonPro);

static void BoolConstruction_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json j(true);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(BoolConstruction_Nlohmann);

// Measures number construction performance.
static void NumberConstruction_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json j(42.0);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(NumberConstruction_JsonPro);

static void NumberConstruction_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json j(42.0);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(NumberConstruction_Nlohmann);

// Measures string construction performance for a short (SSO-sized) string.
static void ShortStringConstruction_JsonPro(benchmark::State& state) {
    const std::string s = "short";

    for (auto _ : state) {
        Json j(s);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ShortStringConstruction_JsonPro);

static void ShortStringConstruction_Nlohmann(benchmark::State& state) {
    const std::string s = "short";

    for (auto _ : state) {
        nlohmann::json j(s);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ShortStringConstruction_Nlohmann);

// Measures string construction performance for a long string that forces a
// heap allocation (past the small-string-optimization threshold).
static void LongStringConstruction_JsonPro(benchmark::State& state) {
    const std::string s(256, 'x');

    for (auto _ : state) {
        Json j(s);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(LongStringConstruction_JsonPro);

static void LongStringConstruction_Nlohmann(benchmark::State& state) {
    const std::string s(256, 'x');

    for (auto _ : state) {
        nlohmann::json j(s);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(LongStringConstruction_Nlohmann);

// Measures construction of a small array (5 elements).
static void ArrayConstruction_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json j(Json::ArrayType{Json(1), Json(2), Json(3), Json(4), Json(5)});
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ArrayConstruction_JsonPro);

static void ArrayConstruction_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json j = nlohmann::json::array({1, 2, 3, 4, 5});
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ArrayConstruction_Nlohmann);

// Measures construction of a small object (5 members).
static void ObjectConstruction_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json::ObjectType obj;
        obj.insert_or_assign("a", Json(1));
        obj.insert_or_assign("b", Json(2));
        obj.insert_or_assign("c", Json(3));
        obj.insert_or_assign("d", Json(4));
        obj.insert_or_assign("e", Json(5));
        Json j(std::move(obj));
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ObjectConstruction_JsonPro);

static void ObjectConstruction_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json j =
            nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}});
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ObjectConstruction_Nlohmann);
