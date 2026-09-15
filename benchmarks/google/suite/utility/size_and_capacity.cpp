// Json Size & Capacity Benchmark Suite
// Measures Json::size()/empty() performance against nlohmann::json.
//
// Covers:
// - size() on an array
// - size() on an object
// - empty() on a non-empty object
// - empty() on an empty object

#include <benchmark/benchmark.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Measures size() on a small array (5 elements).
static void ArraySize_JsonPro(benchmark::State& state) {
    const Json jpSrc(Json::ArrayType{Json(1), Json(2), Json(3), Json(4), Json(5)});

    for (auto _ : state) {
        std::size_t n = jpSrc.size();
        benchmark::DoNotOptimize(n);
    }
}
BENCHMARK(ArraySize_JsonPro);

static void ArraySize_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc = nlohmann::json::array({1, 2, 3, 4, 5});

    for (auto _ : state) {
        std::size_t n = njSrc.size();
        benchmark::DoNotOptimize(n);
    }
}
BENCHMARK(ArraySize_Nlohmann);

// Measures size() on a small object (5 members).
static void ObjectSize_JsonPro(benchmark::State& state) {
    Json::ObjectType obj;
    obj.insert_or_assign("a", Json(1));
    obj.insert_or_assign("b", Json(2));
    obj.insert_or_assign("c", Json(3));
    obj.insert_or_assign("d", Json(4));
    obj.insert_or_assign("e", Json(5));
    const Json jpSrc(std::move(obj));

    for (auto _ : state) {
        std::size_t n = jpSrc.size();
        benchmark::DoNotOptimize(n);
    }
}
BENCHMARK(ObjectSize_JsonPro);

static void ObjectSize_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc =
        nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}});

    for (auto _ : state) {
        std::size_t n = njSrc.size();
        benchmark::DoNotOptimize(n);
    }
}
BENCHMARK(ObjectSize_Nlohmann);

// Measures empty() on a non-empty object.
static void ObjectEmptyFalse_JsonPro(benchmark::State& state) {
    Json::ObjectType obj;
    obj.insert_or_assign("a", Json(1));
    const Json jpSrc(std::move(obj));

    for (auto _ : state) {
        bool e = jpSrc.asObject().empty();
        benchmark::DoNotOptimize(e);
    }
}
BENCHMARK(ObjectEmptyFalse_JsonPro);

static void ObjectEmptyFalse_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc = nlohmann::json::object({{"a", 1}});

    for (auto _ : state) {
        bool e = njSrc.empty();
        benchmark::DoNotOptimize(e);
    }
}
BENCHMARK(ObjectEmptyFalse_Nlohmann);

// Measures empty() on an empty object.
static void ObjectEmptyTrue_JsonPro(benchmark::State& state) {
    const Json jpSrc((Json::ObjectType()));

    for (auto _ : state) {
        bool e = jpSrc.asObject().empty();
        benchmark::DoNotOptimize(e);
    }
}
BENCHMARK(ObjectEmptyTrue_JsonPro);

static void ObjectEmptyTrue_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc = nlohmann::json::object();

    for (auto _ : state) {
        bool e = njSrc.empty();
        benchmark::DoNotOptimize(e);
    }
}
BENCHMARK(ObjectEmptyTrue_Nlohmann);
