// Json Parsing Benchmark Suite
// Measures Json::parse() performance against nlohmann::json::parse().
//
// Each source document is built once, outside the timed lambda. BENCH
// itself sweeps the call count (10K/100K/1M), so each lambda runs a
// single parse per sample rather than an additional inner-loop batch.
//
// Covers:
// - a small flat object (10 members, mixed scalar types)
// - a number array (10 elements)
// - a moderately nested object (3 levels)
// - a string-heavy object (escapes, unicode)

#include <common/framework.h>

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
static void bench_parse_small_object() {
    auto jp = [&] {
        Json j = Json::parse(kSmallObjectJson);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j = nlohmann::json::parse(kSmallObjectJson);
        doNotOptimize(j);
    };

    BENCH("parse small object", jp, nj);
}

// Measures parsing a flat 10-element number array.
static void bench_parse_number_array() {
    const std::string& text = numberArrayJson();

    auto jp = [&] {
        Json j = Json::parse(text);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j = nlohmann::json::parse(text);
        doNotOptimize(j);
    };

    BENCH("parse number array", jp, nj);
}

// Measures parsing a moderately nested object (3 levels).
static void bench_parse_nested() {
    auto jp = [&] {
        Json j = Json::parse(kNestedJson);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j = nlohmann::json::parse(kNestedJson);
        doNotOptimize(j);
    };

    BENCH("parse nested object", jp, nj);
}

// Measures parsing a string-heavy object (escape sequences, unicode).
static void bench_parse_string_heavy() {
    auto jp = [&] {
        Json j = Json::parse(kStringHeavyJson);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j = nlohmann::json::parse(kStringHeavyJson);
        doNotOptimize(j);
    };

    BENCH("parse string-heavy object", jp, nj);
}

// Executes all parsing benchmark cases.
static void run_benchmarks() {
    bench_parse_small_object();
    std::cout << "\n";

    bench_parse_number_array();
    std::cout << "\n";

    bench_parse_nested();
    std::cout << "\n";

    bench_parse_string_heavy();
}

REGISTER_BENCH_SUITE();
