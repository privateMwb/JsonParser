// Json Large Array Benchmark Suite
// Measures Json performance on a large flat array (10,000 numbers) against
// nlohmann::json, at a scale where allocation and cache behavior dominate
// rather than per-element fixed overhead.
//
// The original harness ran these under BENCH_CUSTOM for tuned sample
// counts given the cost per call; Google Benchmark auto-calibrates
// iteration counts per benchmark, so a plain BENCHMARK registration serves
// the same purpose here.
//
// Covers:
// - parsing a large number array
// - serializing a large number array
// - iterating and summing a large number array

#include <benchmark/benchmark.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

namespace {

constexpr std::size_t kCount = 10'000;

const std::string& largeNumberArrayJson() {
    static const std::string s = [] {
        std::string out = "[";
        out.reserve(kCount * 8);

        for (std::size_t i = 0; i < kCount; ++i) {
            if (i)
                out += ',';
            out += std::to_string(static_cast<double>(i) * 1.5);
        }

        out += ']';
        return out;
    }();

    return s;
}

} // namespace

// Measures parsing a large (10,000-element) number array.
static void ParseLargeArray_JsonPro(benchmark::State& state) {
    const std::string& text = largeNumberArrayJson();

    for (auto _ : state) {
        Json j = Json::parse(text);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ParseLargeArray_JsonPro);

static void ParseLargeArray_Nlohmann(benchmark::State& state) {
    const std::string& text = largeNumberArrayJson();

    for (auto _ : state) {
        nlohmann::json j = nlohmann::json::parse(text);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ParseLargeArray_Nlohmann);

// Measures serializing a large (10,000-element) number array. JsonPro's
// dump() always pretty-prints with a fixed 2-space step, so nlohmann's
// side uses dump(2) to keep the output shape comparable.
static void DumpLargeArray_JsonPro(benchmark::State& state) {
    const Json jpSrc = Json::parse(largeNumberArrayJson());

    for (auto _ : state) {
        std::string s = jpSrc.dump();
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(DumpLargeArray_JsonPro);

static void DumpLargeArray_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc = nlohmann::json::parse(largeNumberArrayJson());

    for (auto _ : state) {
        std::string s = njSrc.dump(2);
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(DumpLargeArray_Nlohmann);

// Measures iterating and summing every element of a large array — exercises
// element access and container cache locality at scale rather than parsing.
static void IterateLargeArray_JsonPro(benchmark::State& state) {
    const Json jpSrc = Json::parse(largeNumberArrayJson());

    for (auto _ : state) {
        double sum = 0.0;

        for (const auto& v : jpSrc.asArray())
            sum += v.asNumber();

        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(IterateLargeArray_JsonPro);

static void IterateLargeArray_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc = nlohmann::json::parse(largeNumberArrayJson());

    for (auto _ : state) {
        double sum = 0.0;

        for (const auto& v : njSrc)
            sum += v.get<double>();

        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(IterateLargeArray_Nlohmann);
