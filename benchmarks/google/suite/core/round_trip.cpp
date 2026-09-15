// Json Round-Trip Benchmark Suite
// Measures combined parse() + dump() performance against nlohmann::json,
// i.e. the full ingest-then-reserialize pipeline most real workloads
// actually run.
//
// Covers:
// - a small flat object (10 members, mixed scalar types)
// - a number array (10 elements)
// - a moderately nested object (3 levels)
// - a string-heavy object (escapes, unicode)

#include <benchmark/benchmark.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

namespace {

const std::string kSmallObjectJson = R"json(
    {
        "id": 1,
        "name": "Alice",
        "age": 30,
        "active": true,
        "score": 98.6,
        "city": "Springfield",
        "verified": false,
        "rank": 7,
        "rating": 4.5,
        "tag": "premium"
    }
    )json";

const std::string& numberArrayJson() {
    static const std::string s = [] {
        std::string out = "[";
        for (int i = 0; i < 10; ++i) {
            if (i)
                out += ',';
            out += std::to_string(i * 1.5);
        }
        out += ']';
        return out;
    }();
    return s;
}

const std::string kNestedJson = R"json(
    {
        "user": {
            "id": 42,
            "profile": {
                "name": "Bob",
                "emails": ["bob@example.com", "b.robert@example.com"],
                "settings": {
                    "theme": "dark",
                    "notifications": true
                }
            }
        },
        "meta": {
            "created": "2026-01-01",
            "tags": ["a", "b", "c"]
        }
    }
    )json";

const std::string kStringHeavyJson = R"json(
    {
        "quote": "She said \"hello\" to me",
        "path": "C:\\Users\\test\\file.txt",
        "newline": "line one\nline two",
        "tab": "col1\tcol2",
        "unicode": "caf\u00e9 na\u00efve",
        "long": "................................................................",
        "mixed": "Testing \"quotes\", \\backslashes\\, and\ttabs together"
    }
    )json";

} // namespace

// Measures round-tripping a small flat object.
static void RoundTripSmallObject_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json j = Json::parse(kSmallObjectJson);
        std::string s = j.dump();
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(RoundTripSmallObject_JsonPro);

static void RoundTripSmallObject_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json j = nlohmann::json::parse(kSmallObjectJson);
        std::string s = j.dump(2);
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(RoundTripSmallObject_Nlohmann);

// Measures round-tripping a flat 10-element number array.
static void RoundTripNumberArray_JsonPro(benchmark::State& state) {
    const std::string& text = numberArrayJson();

    for (auto _ : state) {
        Json j = Json::parse(text);
        std::string s = j.dump();
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(RoundTripNumberArray_JsonPro);

static void RoundTripNumberArray_Nlohmann(benchmark::State& state) {
    const std::string& text = numberArrayJson();

    for (auto _ : state) {
        nlohmann::json j = nlohmann::json::parse(text);
        std::string s = j.dump(2);
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(RoundTripNumberArray_Nlohmann);

// Measures round-tripping a moderately nested object (3 levels).
static void RoundTripNested_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json j = Json::parse(kNestedJson);
        std::string s = j.dump();
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(RoundTripNested_JsonPro);

static void RoundTripNested_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json j = nlohmann::json::parse(kNestedJson);
        std::string s = j.dump(2);
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(RoundTripNested_Nlohmann);

// Measures round-tripping a string-heavy object (escape sequences, unicode).
static void RoundTripStringHeavy_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json j = Json::parse(kStringHeavyJson);
        std::string s = j.dump();
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(RoundTripStringHeavy_JsonPro);

static void RoundTripStringHeavy_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json j = nlohmann::json::parse(kStringHeavyJson);
        std::string s = j.dump(2);
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(RoundTripStringHeavy_Nlohmann);
