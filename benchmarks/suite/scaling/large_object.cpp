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
// Covers:
// - parsing a large object
// - serializing a large object

#include <common/framework.h>

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
static void bench_parse_large_object() {
    const std::string& text = largeObjectJson();

    auto jp = [&] {
        Json j = Json::parse(text);
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j = nlohmann::json::parse(text);
        doNotOptimize(j);
    };

    BENCH_CUSTOM("parse large object", jp, nj);
}

// Measures serializing a large (10,000-member) object. JsonPro's dump()
// always pretty-prints with a fixed 2-space step, so nlohmann's side uses
// dump(2) to keep the output shape comparable.
static void bench_dump_large_object() {
    const Json jpSrc = Json::parse(largeObjectJson());
    const nlohmann::json njSrc = nlohmann::json::parse(largeObjectJson());

    auto jp = [&] {
        std::string s = jpSrc.dump();
        doNotOptimize(s);
    };

    auto nj = [&] {
        std::string s = njSrc.dump(2);
        doNotOptimize(s);
    };

    BENCH_CUSTOM("dump large object", jp, nj);
}

// Executes all large-object benchmark cases.
static void run_benchmarks() {
    bench_parse_large_object();
    std::cout << "\n";

    bench_dump_large_object();
}

REGISTER_BENCH_SUITE();
