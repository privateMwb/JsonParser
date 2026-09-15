// Json Large Object Benchmark Suite
// Measures Json performance on a large flat object (10,000 members)
// against nlohmann::json.
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
// - parsing a large object
// - serializing a large object

#include <benchmark/benchmark.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

namespace {

constexpr std::size_t kCount = 10'000;

const std::string& largeObjectJson() {
    static const std::string s = [] {
        std::string out = "{";
        out.reserve(kCount * 16);

        for (std::size_t i = 0; i < kCount; ++i) {
            if (i)
                out += ',';
            out += "\"key" + std::to_string(i) + "\":" + std::to_string(static_cast<double>(i));
        }

        out += '}';
        return out;
    }();

    return s;
}

} // namespace

// Measures parsing a large (10,000-member) object.
static void ParseLargeObject_JsonPro(benchmark::State& state) {
    const std::string& text = largeObjectJson();

    for (auto _ : state) {
        Json j = Json::parse(text);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ParseLargeObject_JsonPro);

static void ParseLargeObject_Nlohmann(benchmark::State& state) {
    const std::string& text = largeObjectJson();

    for (auto _ : state) {
        nlohmann::json j = nlohmann::json::parse(text);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ParseLargeObject_Nlohmann);

// Measures serializing a large (10,000-member) object. JsonPro's dump()
// always pretty-prints with a fixed 2-space step, so nlohmann's side uses
// dump(2) to keep the output shape comparable.
static void DumpLargeObject_JsonPro(benchmark::State& state) {
    const Json jpSrc = Json::parse(largeObjectJson());

    for (auto _ : state) {
        std::string s = jpSrc.dump();
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(DumpLargeObject_JsonPro);

static void DumpLargeObject_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc = nlohmann::json::parse(largeObjectJson());

    for (auto _ : state) {
        std::string s = njSrc.dump(2);
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(DumpLargeObject_Nlohmann);
