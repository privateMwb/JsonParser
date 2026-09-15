// Json Copy Benchmark Suite
// Measures Json copy-construction performance against nlohmann::json.
//
// Each case copies a fixed, pre-built source value repeatedly, so the
// measured cost is copying alone — the source's own construction happens
// once, outside the timed loop.
//
// Covers:
// - default (null) copy
// - bool copy
// - number copy
// - string copy (short / SSO, and long / heap-allocating)
// - array copy
// - object copy

#include <benchmark/benchmark.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Measures default (null) copy performance.
static void DefaultCopy_JsonPro(benchmark::State& state) {
    const Json jpSrc;

    for (auto _ : state) {
        Json j(jpSrc);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(DefaultCopy_JsonPro);

static void DefaultCopy_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc;

    for (auto _ : state) {
        nlohmann::json j(njSrc);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(DefaultCopy_Nlohmann);

// Measures bool copy performance.
static void BoolCopy_JsonPro(benchmark::State& state) {
    const Json jpSrc(true);

    for (auto _ : state) {
        Json j(jpSrc);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(BoolCopy_JsonPro);

static void BoolCopy_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc(true);

    for (auto _ : state) {
        nlohmann::json j(njSrc);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(BoolCopy_Nlohmann);

// Measures number copy performance.
static void NumberCopy_JsonPro(benchmark::State& state) {
    const Json jpSrc(42.0);

    for (auto _ : state) {
        Json j(jpSrc);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(NumberCopy_JsonPro);

static void NumberCopy_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc(42.0);

    for (auto _ : state) {
        nlohmann::json j(njSrc);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(NumberCopy_Nlohmann);

// Measures string copy performance for a short (SSO-sized) string.
static void ShortStringCopy_JsonPro(benchmark::State& state) {
    const Json jpSrc(std::string("short"));

    for (auto _ : state) {
        Json j(jpSrc);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ShortStringCopy_JsonPro);

static void ShortStringCopy_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc(std::string("short"));

    for (auto _ : state) {
        nlohmann::json j(njSrc);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ShortStringCopy_Nlohmann);

// Measures string copy performance for a long string that forces a heap
// allocation (past the small-string-optimization threshold).
static void LongStringCopy_JsonPro(benchmark::State& state) {
    const std::string s(256, 'x');
    const Json jpSrc(s);

    for (auto _ : state) {
        Json j(jpSrc);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(LongStringCopy_JsonPro);

static void LongStringCopy_Nlohmann(benchmark::State& state) {
    const std::string s(256, 'x');
    const nlohmann::json njSrc(s);

    for (auto _ : state) {
        nlohmann::json j(njSrc);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(LongStringCopy_Nlohmann);

// Measures copy performance for a small array (5 elements).
static void ArrayCopy_JsonPro(benchmark::State& state) {
    const Json jpSrc(Json::ArrayType{Json(1), Json(2), Json(3), Json(4), Json(5)});

    for (auto _ : state) {
        Json j(jpSrc);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ArrayCopy_JsonPro);

static void ArrayCopy_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc = nlohmann::json::array({1, 2, 3, 4, 5});

    for (auto _ : state) {
        nlohmann::json j(njSrc);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ArrayCopy_Nlohmann);

// Measures copy performance for a small object (5 members).
static void ObjectCopy_JsonPro(benchmark::State& state) {
    Json::ObjectType obj;
    obj.insert_or_assign("a", Json(1));
    obj.insert_or_assign("b", Json(2));
    obj.insert_or_assign("c", Json(3));
    obj.insert_or_assign("d", Json(4));
    obj.insert_or_assign("e", Json(5));

    const Json jpSrc(std::move(obj));

    for (auto _ : state) {
        Json j(jpSrc);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ObjectCopy_JsonPro);

static void ObjectCopy_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc =
        nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}});

    for (auto _ : state) {
        nlohmann::json j(njSrc);
        benchmark::DoNotOptimize(j);
    }
}
BENCHMARK(ObjectCopy_Nlohmann);
