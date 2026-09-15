// Json Parsing Benchmark Suite
// Measures Json::parse() performance against nlohmann::json::parse().
//
// Each source document is built once, outside the timed loop. The Google
// Benchmark harness itself handles iteration count and warmup, so each
// timed body runs a single parse per iteration.
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

// Measures parsing a small flat object.
static void ParseSmallObject_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json j = Json::parse(kSmallObjectJson);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ParseSmallObject_JsonPro);

static void ParseSmallObject_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json j = nlohmann::json::parse(kSmallObjectJson);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ParseSmallObject_Nlohmann);

// Measures parsing a flat 10-element number array.
static void ParseNumberArray_JsonPro(benchmark::State& state) {
    const std::string& text = numberArrayJson();

    for (auto _ : state) {
        Json j = Json::parse(text);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ParseNumberArray_JsonPro);

static void ParseNumberArray_Nlohmann(benchmark::State& state) {
    const std::string& text = numberArrayJson();

    for (auto _ : state) {
        nlohmann::json j = nlohmann::json::parse(text);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ParseNumberArray_Nlohmann);

// Measures parsing a moderately nested object (3 levels).
static void ParseNested_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json j = Json::parse(kNestedJson);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ParseNested_JsonPro);

static void ParseNested_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json j = nlohmann::json::parse(kNestedJson);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ParseNested_Nlohmann);

// Measures parsing a string-heavy object (escape sequences, unicode).
static void ParseStringHeavy_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        Json j = Json::parse(kStringHeavyJson);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ParseStringHeavy_JsonPro);

static void ParseStringHeavy_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        nlohmann::json j = nlohmann::json::parse(kStringHeavyJson);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ParseStringHeavy_Nlohmann);
