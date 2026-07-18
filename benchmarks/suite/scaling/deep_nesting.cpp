// Json Deep Nesting Benchmark Suite
// Measures Json performance on deeply nested structures against
// nlohmann::json — the case that stresses parseValue()'s recursion depth
// rather than element count.
//
// Depth is capped at 400, safely under Parser::kMaxDepth (512), so these
// cases exercise deep-but-valid recursion rather than the depth guard
// itself.
//
// Covers:
// - parsing a deeply nested array
// - serializing a deeply nested array
// - parsing a deeply nested object

#include <common/framework.h>

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
static void bench_parse_deep_array() {
    const std::string& text = deepArrayJson();

    auto jp = [&] {
        Json j = Json::parse(text);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j = nlohmann::json::parse(text);
        doNotOptimize(j);
    };

    BENCH_CUSTOM("parse deep array", jp, nj);
}

// Measures serializing a deeply nested (400-level) array. JsonPro's dump()
// always pretty-prints with a fixed 2-space step, so nlohmann's side uses
// dump(2) to keep the output shape comparable.
static void bench_dump_deep_array() {
    const Json jpSrc = Json::parse(deepArrayJson());
    const nlohmann::json njSrc = nlohmann::json::parse(deepArrayJson());

    auto jp = [&] {
        std::string s = jpSrc.dump();
        doNotOptimize(s);
    };

    auto nj = [&] {
        std::string s = njSrc.dump(2);
        doNotOptimize(s);
    };

    BENCH_CUSTOM("dump deep array", jp, nj);
}

// Measures parsing a deeply nested (400-level) object.
static void bench_parse_deep_object() {
    const std::string& text = deepObjectJson();

    auto jp = [&] {
        Json j = Json::parse(text);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j = nlohmann::json::parse(text);
        doNotOptimize(j);
    };

    BENCH_CUSTOM("parse deep object", jp, nj);
}

// Executes all deep-nesting benchmark cases.
static void run_benchmarks() {
    bench_parse_deep_array();
    std::cout << "\n";

    bench_dump_deep_array();
    std::cout << "\n";

    bench_parse_deep_object();
}

REGISTER_BENCH_SUITE();
