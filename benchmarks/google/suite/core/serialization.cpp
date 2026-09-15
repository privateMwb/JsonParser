// Json Serialization Benchmark Suite
// Measures Json::dump() performance against nlohmann::json::dump().
//
// Each source value is parsed once, outside the timed loop. JsonPro's
// dump() always pretty-prints with a fixed 2-space step per nesting level
// (there is no separate compact mode), so nlohmann's side uses dump(2) to
// keep the output shape comparable rather than its default single-line
// dump().
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

// Measures serializing a small flat object.
static void DumpSmallObject_JsonPro(benchmark::State& state) {
    const Json jpSrc = Json::parse(kSmallObjectJson);

    for (auto _ : state) {
        std::string s = jpSrc.dump();
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(DumpSmallObject_JsonPro);

static void DumpSmallObject_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc = nlohmann::json::parse(kSmallObjectJson);

    for (auto _ : state) {
        std::string s = njSrc.dump(2);
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(DumpSmallObject_Nlohmann);

// Measures serializing a flat 10-element number array.
static void DumpNumberArray_JsonPro(benchmark::State& state) {
    const Json jpSrc = Json::parse(numberArrayJson());

    for (auto _ : state) {
        std::string s = jpSrc.dump();
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(DumpNumberArray_JsonPro);

static void DumpNumberArray_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc = nlohmann::json::parse(numberArrayJson());

    for (auto _ : state) {
        std::string s = njSrc.dump(2);
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(DumpNumberArray_Nlohmann);

// Measures serializing a moderately nested object (3 levels).
static void DumpNested_JsonPro(benchmark::State& state) {
    const Json jpSrc = Json::parse(kNestedJson);

    for (auto _ : state) {
        std::string s = jpSrc.dump();
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(DumpNested_JsonPro);

static void DumpNested_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc = nlohmann::json::parse(kNestedJson);

    for (auto _ : state) {
        std::string s = njSrc.dump(2);
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(DumpNested_Nlohmann);

// Measures serializing a string-heavy object (escape sequences, unicode).
static void DumpStringHeavy_JsonPro(benchmark::State& state) {
    const Json jpSrc = Json::parse(kStringHeavyJson);

    for (auto _ : state) {
        std::string s = jpSrc.dump();
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(DumpStringHeavy_JsonPro);

static void DumpStringHeavy_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc = nlohmann::json::parse(kStringHeavyJson);

    for (auto _ : state) {
        std::string s = njSrc.dump(2);
        benchmark::DoNotOptimize(s);
    }
}
BENCHMARK(DumpStringHeavy_Nlohmann);
