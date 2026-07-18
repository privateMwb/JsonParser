// Json Serialization Benchmark Suite
// Measures Json::dump() performance against nlohmann::json::dump().
//
// Each source value is parsed once, outside the timed lambda. JsonPro's
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

// Measures serializing a small flat object.
static void bench_dump_small_object() {
    const Json jpSrc = Json::parse(kSmallObjectJson);
    const nlohmann::json njSrc = nlohmann::json::parse(kSmallObjectJson);

    auto jp = [&] {
        std::string s = jpSrc.dump();
        doNotOptimize(s);
    };

    auto nj = [&] {
        std::string s = njSrc.dump(2);
        doNotOptimize(s);
    };

    BENCH("dump small object", jp, nj);
}

// Measures serializing a flat 10-element number array.
static void bench_dump_number_array() {
    const Json jpSrc = Json::parse(numberArrayJson());
    const nlohmann::json njSrc = nlohmann::json::parse(numberArrayJson());

    auto jp = [&] {
        std::string s = jpSrc.dump();
        doNotOptimize(s);
    };

    auto nj = [&] {
        std::string s = njSrc.dump(2);
        doNotOptimize(s);
    };

    BENCH("dump number array", jp, nj);
}

// Measures serializing a moderately nested object (3 levels).
static void bench_dump_nested() {
    const Json jpSrc = Json::parse(kNestedJson);
    const nlohmann::json njSrc = nlohmann::json::parse(kNestedJson);

    auto jp = [&] {
        std::string s = jpSrc.dump();
        doNotOptimize(s);
    };

    auto nj = [&] {
        std::string s = njSrc.dump(2);
        doNotOptimize(s);
    };

    BENCH("dump nested object", jp, nj);
}

// Measures serializing a string-heavy object (escape sequences, unicode).
static void bench_dump_string_heavy() {
    const Json jpSrc = Json::parse(kStringHeavyJson);
    const nlohmann::json njSrc = nlohmann::json::parse(kStringHeavyJson);

    auto jp = [&] {
        std::string s = jpSrc.dump();
        doNotOptimize(s);
    };

    auto nj = [&] {
        std::string s = njSrc.dump(2);
        doNotOptimize(s);
    };

    BENCH("dump string-heavy object", jp, nj);
}

// Executes all serialization benchmark cases.
static void run_benchmarks() {
    bench_dump_small_object();
    std::cout << "\n";

    bench_dump_number_array();
    std::cout << "\n";

    bench_dump_nested();
    std::cout << "\n";

    bench_dump_string_heavy();
}

REGISTER_BENCH_SUITE();
