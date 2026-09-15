// Json Lookup Benchmark Suite
// Measures Json find()/contains() performance against nlohmann::json.
//
// The source object is built once, outside the timed loop. Hit and miss
// cases are benchmarked separately, since a miss still walks the full hash
// lookup path but never touches the found entry.
//
// Covers:
// - contains(): key present (hit) and key absent (miss)
// - find(): key present (hit) and key absent (miss)

#include <benchmark/benchmark.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

namespace {
constexpr std::size_t kElementCount = 100;
} // namespace

// Measures contains() for a key that is present.
static void ContainsHit_JsonPro(benchmark::State& state) {
    Json::ObjectType obj;
    for (std::size_t i = 0; i < kElementCount; ++i)
        obj.insert_or_assign("key" + std::to_string(i), Json(static_cast<double>(i)));
    const Json jpSrc(std::move(obj));

    for (auto _ : state) {
        bool found = jpSrc.contains("key0");
        benchmark::DoNotOptimize(found);
    }
}
BENCHMARK(ContainsHit_JsonPro);

static void ContainsHit_Nlohmann(benchmark::State& state) {
    nlohmann::json njSrc = nlohmann::json::object();
    for (std::size_t i = 0; i < kElementCount; ++i)
        njSrc["key" + std::to_string(i)] = static_cast<double>(i);

    for (auto _ : state) {
        bool found = njSrc.contains("key0");
        benchmark::DoNotOptimize(found);
    }
}
BENCHMARK(ContainsHit_Nlohmann);

// Measures contains() for a key that is absent.
static void ContainsMiss_JsonPro(benchmark::State& state) {
    Json::ObjectType obj;
    for (std::size_t i = 0; i < kElementCount; ++i)
        obj.insert_or_assign("key" + std::to_string(i), Json(static_cast<double>(i)));
    const Json jpSrc(std::move(obj));

    for (auto _ : state) {
        bool found = jpSrc.contains("missing0");
        benchmark::DoNotOptimize(found);
    }
}
BENCHMARK(ContainsMiss_JsonPro);

static void ContainsMiss_Nlohmann(benchmark::State& state) {
    nlohmann::json njSrc = nlohmann::json::object();
    for (std::size_t i = 0; i < kElementCount; ++i)
        njSrc["key" + std::to_string(i)] = static_cast<double>(i);

    for (auto _ : state) {
        bool found = njSrc.contains("missing0");
        benchmark::DoNotOptimize(found);
    }
}
BENCHMARK(ContainsMiss_Nlohmann);

// Measures find() for a key that is present.
static void FindHit_JsonPro(benchmark::State& state) {
    Json::ObjectType obj;
    for (std::size_t i = 0; i < kElementCount; ++i)
        obj.insert_or_assign("key" + std::to_string(i), Json(static_cast<double>(i)));
    Json jpSrc(std::move(obj));

    for (auto _ : state) {
        const Json* found = jpSrc.asObject().find("key0");
        benchmark::DoNotOptimize(found);
    }
}
BENCHMARK(FindHit_JsonPro);

static void FindHit_Nlohmann(benchmark::State& state) {
    nlohmann::json njSrc = nlohmann::json::object();
    for (std::size_t i = 0; i < kElementCount; ++i)
        njSrc["key" + std::to_string(i)] = static_cast<double>(i);

    for (auto _ : state) {
        auto it = njSrc.find("key0");
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(FindHit_Nlohmann);

// Measures find() for a key that is absent.
static void FindMiss_JsonPro(benchmark::State& state) {
    Json::ObjectType obj;
    for (std::size_t i = 0; i < kElementCount; ++i)
        obj.insert_or_assign("key" + std::to_string(i), Json(static_cast<double>(i)));
    Json jpSrc(std::move(obj));

    for (auto _ : state) {
        const Json* found = jpSrc.asObject().find("missing0");
        benchmark::DoNotOptimize(found);
    }
}
BENCHMARK(FindMiss_JsonPro);

static void FindMiss_Nlohmann(benchmark::State& state) {
    nlohmann::json njSrc = nlohmann::json::object();
    for (std::size_t i = 0; i < kElementCount; ++i)
        njSrc["key" + std::to_string(i)] = static_cast<double>(i);

    for (auto _ : state) {
        auto it = njSrc.find("missing0");
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(FindMiss_Nlohmann);
