// Json Element Access Benchmark Suite
// Measures Json operator[]/at() performance against nlohmann::json.
//
// Each case builds its source structure once, outside the timed lambda —
// only repeated access into an already-built structure is measured.
//
// Covers:
// - array element access via operator[] (unchecked)
// - array element access via at() (bounds-checked)
// - object member access via operator[] (existing key, no auto-vivify)
// - object member access via at() (bounds-checked)

#include <common/framework.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

namespace {
constexpr std::size_t kElementCount = 100;
} // namespace

// Measures unchecked array element access via operator[].
static void bench_array_operator_index() {
    Json::ArrayType arr;
    arr.reserve(kElementCount);
    for (std::size_t i = 0; i < kElementCount; ++i)
        arr.push_back(Json(static_cast<double>(i)));
    const Json jpSrc(std::move(arr));

    nlohmann::json njSrc = nlohmann::json::array();
    for (std::size_t i = 0; i < kElementCount; ++i)
        njSrc.push_back(static_cast<double>(i));

    auto jp = [&] {
        const Json& v = jpSrc[0];
        doNotOptimize(v);
    };

    auto nj = [&] {
        const auto& v = njSrc[0];
        doNotOptimize(v);
    };

    BENCH("array operator[]", jp, nj);
}

// Measures bounds-checked array element access via at().
static void bench_array_at() {
    Json::ArrayType arr;
    arr.reserve(kElementCount);
    for (std::size_t i = 0; i < kElementCount; ++i)
        arr.push_back(Json(static_cast<double>(i)));
    const Json jpSrc(std::move(arr));

    nlohmann::json njSrc = nlohmann::json::array();
    for (std::size_t i = 0; i < kElementCount; ++i)
        njSrc.push_back(static_cast<double>(i));

    auto jp = [&] {
        const Json& v = jpSrc.at(0);
        doNotOptimize(v);
    };

    auto nj = [&] {
        const auto& v = njSrc.at(0);
        doNotOptimize(v);
    };

    BENCH("array at()", jp, nj);
}

// Measures object member access via operator[] for an existing key (the
// lookup-hit path — no new entry is inserted).
static void bench_object_operator_index() {
    Json::ObjectType obj;
    for (std::size_t i = 0; i < kElementCount; ++i)
        obj.insert_or_assign("key" + std::to_string(i), Json(static_cast<double>(i)));
    Json jpSrc(std::move(obj));

    nlohmann::json njSrc = nlohmann::json::object();
    for (std::size_t i = 0; i < kElementCount; ++i)
        njSrc["key" + std::to_string(i)] = static_cast<double>(i);

    auto jp = [&] {
        Json& v = jpSrc["key0"];
        doNotOptimize(v);
    };

    auto nj = [&] {
        auto& v = njSrc["key0"];
        doNotOptimize(v);
    };

    BENCH("object operator[]", jp, nj);
}

// Measures bounds-checked object member access via at().
static void bench_object_at() {
    Json::ObjectType obj;
    for (std::size_t i = 0; i < kElementCount; ++i)
        obj.insert_or_assign("key" + std::to_string(i), Json(static_cast<double>(i)));
    const Json jpSrc(std::move(obj));

    nlohmann::json njSrc = nlohmann::json::object();
    for (std::size_t i = 0; i < kElementCount; ++i)
        njSrc["key" + std::to_string(i)] = static_cast<double>(i);

    auto jp = [&] {
        const Json& v = jpSrc.at("key0");
        doNotOptimize(v);
    };

    auto nj = [&] {
        const auto& v = njSrc.at("key0");
        doNotOptimize(v);
    };

    BENCH("object at()", jp, nj);
}

// Executes all element access benchmark cases.
static void run_benchmarks() {
    bench_array_operator_index();
    std::cout << "\n";

    bench_array_at();
    std::cout << "\n";

    bench_object_operator_index();
    std::cout << "\n";

    bench_object_at();
}

REGISTER_BENCH_SUITE();
