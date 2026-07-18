// Json Wide Object Benchmark Suite
// Measures Json's object-building and lookup API directly (not via parsing)
// at scale (10,000 members) against nlohmann::json — complements
// largeObject.cpp, which focuses on parse()/dump() instead.
//
// Note: nlohmann::json's default object type is key-ordered via std::map
// (O(log n) insert/lookup). JsonObject uses a vector + transparent-hash
// index (O(1) average). This suite compares against plain nlohmann::json
// since it's the far more commonly used default — the gap at this scale is
// partly that algorithmic difference, not just constant-factor overhead.
//
// Covers:
// - building a wide object via repeated insertion
// - looking up a key in an already-built wide object

#include <common/framework.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

namespace {
constexpr std::size_t kCount = 10'000;
} // namespace

// Measures building a wide (10,000-member) object via repeated insertion.
static void bench_build_wide_object() {
    auto jp = [&] {
        Json::ObjectType obj;
        obj.reserve(kCount);

        for (std::size_t i = 0; i < kCount; ++i)
            obj.insert_or_assign("key" + std::to_string(i), Json(static_cast<double>(i)));

        Json j(std::move(obj));
        doNotOptimize(j);
    };

    auto nj = [&] {
        nlohmann::json j = nlohmann::json::object();

        for (std::size_t i = 0; i < kCount; ++i)
            j["key" + std::to_string(i)] = static_cast<double>(i);

        doNotOptimize(j);
    };

    BENCH_CUSTOM("build wide object", jp, nj);
}

// Measures looking up a key in an already-built wide (10,000-member)
// object — the source is built once, outside the timed lambda.
static void bench_lookup_in_wide_object() {
    Json::ObjectType obj;
    obj.reserve(kCount);
    for (std::size_t i = 0; i < kCount; ++i)
        obj.insert_or_assign("key" + std::to_string(i), Json(static_cast<double>(i)));
    const Json jpSrc(std::move(obj));

    nlohmann::json njSrc = nlohmann::json::object();
    for (std::size_t i = 0; i < kCount; ++i)
        njSrc["key" + std::to_string(i)] = static_cast<double>(i);

    auto jp = [&] {
        for (std::size_t i = 0; i < 1000; ++i) {
            bool found = jpSrc.contains("key" + std::to_string(i * 7 % kCount));
            doNotOptimize(found);
        }
    };

    auto nj = [&] {
        for (std::size_t i = 0; i < 1000; ++i) {
            bool found = njSrc.contains("key" + std::to_string(i * 7 % kCount));
            doNotOptimize(found);
        }
    };

    BENCH_CUSTOM("lookup in wide object", jp, nj);
}

// Executes all wide-object benchmark cases.
static void run_benchmarks() {
    bench_build_wide_object();
    std::cout << "\n";

    bench_lookup_in_wide_object();
}

REGISTER_BENCH_SUITE();
