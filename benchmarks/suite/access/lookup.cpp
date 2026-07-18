// Json Lookup Benchmark Suite
// Measures Json find()/contains() performance against nlohmann::json.
//
// The source object is built once, outside the timed lambda. Hit and miss
// cases are benchmarked separately, since a miss still walks the full hash
// lookup path but never touches the found entry.
//
// Covers:
// - contains(): key present (hit) and key absent (miss)
// - find(): key present (hit) and key absent (miss)

#include <common/framework.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

namespace {
constexpr std::size_t kElementCount = 100;
} // namespace

// Measures contains() for a key that is present.
static void bench_contains_hit() {
    Json::ObjectType obj;
    for (std::size_t i = 0; i < kElementCount; ++i)
        obj.insert_or_assign("key" + std::to_string(i), Json(static_cast<double>(i)));
    const Json jpSrc(std::move(obj));

    nlohmann::json njSrc = nlohmann::json::object();
    for (std::size_t i = 0; i < kElementCount; ++i)
        njSrc["key" + std::to_string(i)] = static_cast<double>(i);

    auto jp = [&] {
        bool found = jpSrc.contains("key0");
        doNotOptimize(found);
    };

    auto nj = [&] {
        bool found = njSrc.contains("key0");
        doNotOptimize(found);
    };

    BENCH("contains() hit", jp, nj);
}

// Measures contains() for a key that is absent.
static void bench_contains_miss() {
    Json::ObjectType obj;
    for (std::size_t i = 0; i < kElementCount; ++i)
        obj.insert_or_assign("key" + std::to_string(i), Json(static_cast<double>(i)));
    const Json jpSrc(std::move(obj));

    nlohmann::json njSrc = nlohmann::json::object();
    for (std::size_t i = 0; i < kElementCount; ++i)
        njSrc["key" + std::to_string(i)] = static_cast<double>(i);

    auto jp = [&] {
        bool found = jpSrc.contains("missing0");
        doNotOptimize(found);
    };

    auto nj = [&] {
        bool found = njSrc.contains("missing0");
        doNotOptimize(found);
    };

    BENCH("contains() miss", jp, nj);
}

// Measures find() for a key that is present.
static void bench_find_hit() {
    Json::ObjectType obj;
    for (std::size_t i = 0; i < kElementCount; ++i)
        obj.insert_or_assign("key" + std::to_string(i), Json(static_cast<double>(i)));
    Json jpSrc(std::move(obj));

    nlohmann::json njSrc = nlohmann::json::object();
    for (std::size_t i = 0; i < kElementCount; ++i)
        njSrc["key" + std::to_string(i)] = static_cast<double>(i);

    auto jp = [&] {
        const Json* found = jpSrc.asObject().find("key0");
        doNotOptimize(found);
    };

    auto nj = [&] {
        auto it = njSrc.find("key0");
        doNotOptimize(it);
    };

    BENCH("find() hit", jp, nj);
}

// Measures find() for a key that is absent.
static void bench_find_miss() {
    Json::ObjectType obj;
    for (std::size_t i = 0; i < kElementCount; ++i)
        obj.insert_or_assign("key" + std::to_string(i), Json(static_cast<double>(i)));
    Json jpSrc(std::move(obj));

    nlohmann::json njSrc = nlohmann::json::object();
    for (std::size_t i = 0; i < kElementCount; ++i)
        njSrc["key" + std::to_string(i)] = static_cast<double>(i);

    auto jp = [&] {
        const Json* found = jpSrc.asObject().find("missing0");
        doNotOptimize(found);
    };

    auto nj = [&] {
        auto it = njSrc.find("missing0");
        doNotOptimize(it);
    };

    BENCH("find() miss", jp, nj);
}

// Executes all lookup benchmark cases.
static void run_benchmarks() {
    bench_contains_hit();
    std::cout << "\n";

    bench_contains_miss();
    std::cout << "\n";

    bench_find_hit();
    std::cout << "\n";

    bench_find_miss();
}

REGISTER_BENCH_SUITE();
