// Json Destruction Benchmark Suite
// Measures Json destruction performance against nlohmann::json.
//
// Each case builds a batch of values into a vector, then times only the
// vector's clear() — the point at which every element's destructor runs.
// Building the batch happens inside the same timed loop body (kept as a
// single unpaused loop, matching the original harness's lack of a separate
// setup/teardown phase), so the absolute numbers include some construction
// cost; treat the *relative* gap between JsonPro and nlohmann::json here as
// the meaningful signal, since construction cost is already isolated
// separately in construction.cpp.
//
// Covers:
// - default (null) destruction
// - bool destruction
// - number destruction
// - string destruction (short / SSO, and long / heap-allocating)
// - array destruction
// - object destruction

#include <benchmark/benchmark.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

#include <vector>

using namespace JsonPro;

// Measures default (null) destruction performance.
static void BM_DefaultDestruction_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<Json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back();

        batch.clear();
    }
}
BENCHMARK(BM_DefaultDestruction_JsonPro);

static void BM_DefaultDestruction_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<nlohmann::json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back();

        batch.clear();
    }
}
BENCHMARK(BM_DefaultDestruction_Nlohmann);

// Measures bool destruction performance.
static void BM_BoolDestruction_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<Json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(true);

        batch.clear();
    }
}
BENCHMARK(BM_BoolDestruction_JsonPro);

static void BM_BoolDestruction_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<nlohmann::json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(true);

        batch.clear();
    }
}
BENCHMARK(BM_BoolDestruction_Nlohmann);

// Measures number destruction performance.
static void BM_NumberDestruction_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<Json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(42.0);

        batch.clear();
    }
}
BENCHMARK(BM_NumberDestruction_JsonPro);

static void BM_NumberDestruction_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<nlohmann::json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(42.0);

        batch.clear();
    }
}
BENCHMARK(BM_NumberDestruction_Nlohmann);

// Measures string destruction performance for a short (SSO-sized) string.
static void BM_ShortStringDestruction_JsonPro(benchmark::State& state) {
    const std::string s = "short";

    for (auto _ : state) {
        std::vector<Json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(s);

        batch.clear();
    }
}
BENCHMARK(BM_ShortStringDestruction_JsonPro);

static void BM_ShortStringDestruction_Nlohmann(benchmark::State& state) {
    const std::string s = "short";

    for (auto _ : state) {
        std::vector<nlohmann::json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(s);

        batch.clear();
    }
}
BENCHMARK(BM_ShortStringDestruction_Nlohmann);

// Measures string destruction performance for a long string that forced a
// heap allocation on construction.
static void BM_LongStringDestruction_JsonPro(benchmark::State& state) {
    const std::string s(256, 'x');

    for (auto _ : state) {
        std::vector<Json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(s);

        batch.clear();
    }
}
BENCHMARK(BM_LongStringDestruction_JsonPro);

static void BM_LongStringDestruction_Nlohmann(benchmark::State& state) {
    const std::string s(256, 'x');

    for (auto _ : state) {
        std::vector<nlohmann::json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(s);

        batch.clear();
    }
}
BENCHMARK(BM_LongStringDestruction_Nlohmann);

// Measures destruction performance for a small array (5 elements).
static void BM_ArrayDestruction_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<Json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(Json::ArrayType{Json(1), Json(2), Json(3), Json(4), Json(5)});

        batch.clear();
    }
}
BENCHMARK(BM_ArrayDestruction_JsonPro);

static void BM_ArrayDestruction_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<nlohmann::json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i)
            batch.emplace_back(nlohmann::json::array({1, 2, 3, 4, 5}));

        batch.clear();
    }
}
BENCHMARK(BM_ArrayDestruction_Nlohmann);

// Measures destruction performance for a small object (5 members).
static void BM_ObjectDestruction_JsonPro(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<Json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i) {
            Json::ObjectType obj;
            obj.insert_or_assign("a", Json(1));
            obj.insert_or_assign("b", Json(2));
            obj.insert_or_assign("c", Json(3));
            obj.insert_or_assign("d", Json(4));
            obj.insert_or_assign("e", Json(5));
            batch.emplace_back(std::move(obj));
        }

        batch.clear();
    }
}
BENCHMARK(BM_ObjectDestruction_JsonPro);

static void BM_ObjectDestruction_Nlohmann(benchmark::State& state) {
    for (auto _ : state) {
        std::vector<nlohmann::json> batch;
        batch.reserve(10);

        for (int i = 0; i < 10; ++i) {
            batch.emplace_back(
                nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}}));
        }

        batch.clear();
    }
}
BENCHMARK(BM_ObjectDestruction_Nlohmann);
