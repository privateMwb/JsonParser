// Json Large Array Benchmark Suite
// Measures Json performance on a large flat array (10,000 numbers) against
// nlohmann::json, at a scale where allocation and cache behavior dominate
// rather than per-element fixed overhead.
//
// Covers:
// - parsing a large number array
// - serializing a large number array
// - iterating and summing a large number array

#include <common/framework.h>

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
static void bench_parse_large_array() {
    const std::string& text = largeNumberArrayJson();

    auto jp = [&] {
        Json j = Json::parse(text);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j = nlohmann::json::parse(text);
        doNotOptimize(j);
    };

    BENCH_CUSTOM("parse large array", jp, nj);
}

// Measures serializing a large (10,000-element) number array. JsonPro's
// dump() always pretty-prints with a fixed 2-space step, so nlohmann's
// side uses dump(2) to keep the output shape comparable.
static void bench_dump_large_array() {
    const Json jpSrc = Json::parse(largeNumberArrayJson());
    const nlohmann::json njSrc = nlohmann::json::parse(largeNumberArrayJson());

    auto jp = [&] {
        std::string s = jpSrc.dump();
        doNotOptimize(s);
    };

    auto nj = [&] {
        std::string s = njSrc.dump(2);
        doNotOptimize(s);
    };

    BENCH_CUSTOM("dump large array", jp, nj);
}

// Measures iterating and summing every element of a large array — exercises
// element access and container cache locality at scale rather than parsing.
static void bench_iterate_large_array() {
    const Json jpSrc = Json::parse(largeNumberArrayJson());
    const nlohmann::json njSrc = nlohmann::json::parse(largeNumberArrayJson());

    auto jp = [&] {
        double sum = 0.0;

        for (const auto& v : jpSrc.asArray())
            sum += v.asNumber();

        doNotOptimize(sum);
    };

    auto nj = [&] {
        double sum = 0.0;

        for (const auto& v : njSrc)
            sum += v.get<double>();

        doNotOptimize(sum);
    };

    BENCH_CUSTOM("iterate large array", jp, nj);
}

// Executes all large-array benchmark cases.
static void run_benchmarks() {
    bench_parse_large_array();
    std::cout << "\n";

    bench_dump_large_array();
    std::cout << "\n";

    bench_iterate_large_array();
}

REGISTER_BENCH_SUITE();
