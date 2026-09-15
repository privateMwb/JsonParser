// Json Wide Object Benchmark Suite
// Measures Json's object-building and lookup API directly (not via parsing)
// at scale (10,000 members) against nlohmann::json — complements
// large_object.cpp, which focuses on parse()/dump() instead.
//
// Note: nlohmann::json's default object type is key-ordered via std::map
// (O(log n) insert/lookup). JsonObject uses a vector + transparent-hash
// index (O(1) average). This suite compares against plain nlohmann::json
// since it's the far more commonly used default — the gap at this scale is
// partly that algorithmic difference, not just constant-factor overhead.
//
// The original harness ran these under BENCH_CUSTOM for tuned sample
// counts given the cost per call; Google Benchmark auto-calibrates
// iteration counts per benchmark, so a plain BENCHMARK registration serves
// the same purpose here.
//
// Covers:
// - building a wide object via repeated insertion
// - looking up a key in an already-built wide object

#include <benchmark/benchmark.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

namespace {
constexpr std::size_t kCount = 10'000;
} // namespace

// Measures building a wide (10,000-member) object via repeated insertion.
static void BuildWideObject_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json::ObjectType obj;
        obj.reserve(kCount);

        for (std::size_t i = 0; i < kCount; ++i)
            obj.insert_or_assign("key" + std::to_string(i), Json(static_cast<double>(i)));

        Json j(std::move(obj));
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(BuildWideObject_JsonPro);

static void BuildWideObject_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json j = nlohmann::json::object();

        for (std::size_t i = 0; i < kCount; ++i)
            j["key" + std::to_string(i)] = static_cast<double>(i);

        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(BuildWideObject_Nlohmann);

// Measures looking up a key in an already-built wide (10,000-member)
// object — the source is built once, outside the timed loop.
static void LookupInWideObject_JsonPro(benchmark::State& state) {
    Json::ObjectType obj;
    obj.reserve(kCount);
    for (std::size_t i = 0; i < kCount; ++i)
        obj.insert_or_assign("key" + std::to_string(i), Json(static_cast<double>(i)));
    const Json jpSrc(std::move(obj));

    for (auto _ : state) {
        for (std::size_t i = 0; i < 1000; ++i) {
            bool found = jpSrc.contains("key" + std::to_string(i * 7 % kCount));
            benchmark::DoNotOptimize(found);
        }
    }
}
BENCHMARK(LookupInWideObject_JsonPro);

static void LookupInWideObject_Nlohmann(benchmark::State& state) {
    nlohmann::json njSrc = nlohmann::json::object();
    for (std::size_t i = 0; i < kCount; ++i)
        njSrc["key" + std::to_string(i)] = static_cast<double>(i);

    for (auto _ : state) {
        for (std::size_t i = 0; i < 1000; ++i) {
            bool found = njSrc.contains("key" + std::to_string(i * 7 % kCount));
            benchmark::DoNotOptimize(found);
        }
    }
}
BENCHMARK(LookupInWideObject_Nlohmann);
