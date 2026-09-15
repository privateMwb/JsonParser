// Json Element Access Benchmark Suite
// Measures Json operator[]/at() performance against nlohmann::json.
//
// Each case builds its source structure once, outside the timed loop —
// only repeated access into an already-built structure is measured.
//
// Covers:
// - array element access via operator[] (unchecked)
// - array element access via at() (bounds-checked)
// - object member access via operator[] (existing key, no auto-vivify)
// - object member access via at() (bounds-checked)

#include <benchmark/benchmark.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

namespace {
constexpr std::size_t kElementCount = 100;
} // namespace

// Measures unchecked array element access via operator[].
static void ArrayOperatorIndex_JsonPro(benchmark::State& state) {
    Json::ArrayType arr;
    arr.reserve(kElementCount);
    for (std::size_t i = 0; i < kElementCount; ++i)
        arr.push_back(Json(static_cast<double>(i)));
    const Json jpSrc(std::move(arr));

    for (auto _ : state) {
        const Json& v = jpSrc[0];
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ArrayOperatorIndex_JsonPro);

static void ArrayOperatorIndex_Nlohmann(benchmark::State& state) {
    nlohmann::json njSrc = nlohmann::json::array();
    for (std::size_t i = 0; i < kElementCount; ++i)
        njSrc.push_back(static_cast<double>(i));

    for (auto _ : state) {
        const auto& v = njSrc[0];
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ArrayOperatorIndex_Nlohmann);

// Measures bounds-checked array element access via at().
static void ArrayAt_JsonPro(benchmark::State& state) {
    Json::ArrayType arr;
    arr.reserve(kElementCount);
    for (std::size_t i = 0; i < kElementCount; ++i)
        arr.push_back(Json(static_cast<double>(i)));
    const Json jpSrc(std::move(arr));

    for (auto _ : state) {
        const Json& v = jpSrc.at(0);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ArrayAt_JsonPro);

static void ArrayAt_Nlohmann(benchmark::State& state) {
    nlohmann::json njSrc = nlohmann::json::array();
    for (std::size_t i = 0; i < kElementCount; ++i)
        njSrc.push_back(static_cast<double>(i));

    for (auto _ : state) {
        const auto& v = njSrc.at(0);
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ArrayAt_Nlohmann);

// Measures object member access via operator[] for an existing key (the
// lookup-hit path — no new entry is inserted).
static void ObjectOperatorIndex_JsonPro(benchmark::State& state) {
    Json::ObjectType obj;
    for (std::size_t i = 0; i < kElementCount; ++i)
        obj.insert_or_assign("key" + std::to_string(i), Json(static_cast<double>(i)));
    Json jpSrc(std::move(obj));

    for (auto _ : state) {
        Json& v = jpSrc["key0"];
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ObjectOperatorIndex_JsonPro);

static void ObjectOperatorIndex_Nlohmann(benchmark::State& state) {
    nlohmann::json njSrc = nlohmann::json::object();
    for (std::size_t i = 0; i < kElementCount; ++i)
        njSrc["key" + std::to_string(i)] = static_cast<double>(i);

    for (auto _ : state) {
        auto& v = njSrc["key0"];
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ObjectOperatorIndex_Nlohmann);

// Measures bounds-checked object member access via at().
static void ObjectAt_JsonPro(benchmark::State& state) {
    Json::ObjectType obj;
    for (std::size_t i = 0; i < kElementCount; ++i)
        obj.insert_or_assign("key" + std::to_string(i), Json(static_cast<double>(i)));
    const Json jpSrc(std::move(obj));

    for (auto _ : state) {
        const Json& v = jpSrc.at("key0");
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ObjectAt_JsonPro);

static void ObjectAt_Nlohmann(benchmark::State& state) {
    nlohmann::json njSrc = nlohmann::json::object();
    for (std::size_t i = 0; i < kElementCount; ++i)
        njSrc["key" + std::to_string(i)] = static_cast<double>(i);

    for (auto _ : state) {
        const auto& v = njSrc.at("key0");
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(ObjectAt_Nlohmann);
