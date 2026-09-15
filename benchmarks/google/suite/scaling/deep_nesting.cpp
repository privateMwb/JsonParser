// Json Deep Nesting Benchmark Suite
// Measures Json performance on deeply nested structures against
// nlohmann::json — the case that stresses parseValue()'s recursion depth
// rather than element count.
//
// Depth is capped at 400, safely under Parser::kMaxDepth (512), so these
// cases exercise deep-but-valid recursion rather than the depth guard
// itself. The original harness ran these under BENCH_CUSTOM for tuned
// sample counts given the cost per call; Google Benchmark auto-calibrates
// iteration counts per benchmark, so a plain BENCHMARK registration serves
// the same purpose here.
//
// Covers:
// - parsing a deeply nested array
// - serializing a deeply nested array
// - parsing a deeply nested object

#include <benchmark/benchmark.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

namespace {

constexpr int kDepth = 400;

const std::string& deepArrayJson() {
    static const std::string s = [] {
        std::string out;
        out.reserve(static_cast<std::size_t>(kDepth) * 2 + 1);

        for (int i = 0; i < kDepth; ++i)
            out += '[';
        out += '1';
        for (int i = 0; i < kDepth; ++i)
            out += ']';

        return out;
    }();

    return s;
}

const std::string& deepObjectJson() {
    static const std::string s = [] {
        std::string out;

        for (int i = 0; i < kDepth; ++i)
            out += "{\"a\":";
        out += '1';
        for (int i = 0; i < kDepth; ++i)
            out += '}';

        return out;
    }();

    return s;
}

} // namespace

// Measures parsing a deeply nested (400-level) array.
static void ParseDeepArray_JsonPro(benchmark::State& state) {
    const std::string& text = deepArrayJson();

    for (auto _ : state) {
        Json j = Json::parse(text);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ParseDeepArray_JsonPro);

static void ParseDeepArray_Nlohmann(benchmark::State& state) {
    const std::string& text = deepArrayJson();

    for (auto _ : state) {
        nlohmann::json j = nlohmann::json::parse(text);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ParseDeepArray_Nlohmann);

// Measures serializing a deeply nested (400-level) array. JsonPro's dump()
// always pretty-prints with a fixed 2-space step, so nlohmann's side uses
// dump(2) to keep the output shape comparable.
static void DumpDeepArray_JsonPro(benchmark::State& state) {
    const Json jpSrc = Json::parse(deepArrayJson());

    for (auto _ : state) {
        std::string s = jpSrc.dump();
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(DumpDeepArray_JsonPro);

static void DumpDeepArray_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc = nlohmann::json::parse(deepArrayJson());

    for (auto _ : state) {
        std::string s = njSrc.dump(2);
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(DumpDeepArray_Nlohmann);

// Measures parsing a deeply nested (400-level) object.
static void ParseDeepObject_JsonPro(benchmark::State& state) {
    const std::string& text = deepObjectJson();

    for (auto _ : state) {
        Json j = Json::parse(text);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ParseDeepObject_JsonPro);

static void ParseDeepObject_Nlohmann(benchmark::State& state) {
    const std::string& text = deepObjectJson();

    for (auto _ : state) {
        nlohmann::json j = nlohmann::json::parse(text);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ParseDeepObject_Nlohmann);
