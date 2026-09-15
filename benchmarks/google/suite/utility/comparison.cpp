// Json Comparison Benchmark Suite
// Measures Json::operator== performance against nlohmann::json.
//
// Covers:
// - equal numbers (trivial scalar compare)
// - equal strings
// - equal arrays
// - equal objects
// - unequal objects (mismatch found early vs. late)

#include <benchmark/benchmark.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Measures comparing two equal numbers.
static void CompareEqualNumbers_JsonPro(benchmark::State& state) {
    const Json jpA(42.0), jpB(42.0);

    for (auto _ : state) {
        bool eq = (jpA == jpB);
        benchmark::DoNotOptimize(eq);
    }
}
BENCHMARK(CompareEqualNumbers_JsonPro);

static void CompareEqualNumbers_Nlohmann(benchmark::State& state) {
    const nlohmann::json njA(42.0), njB(42.0);

    for (auto _ : state) {
        bool eq = (njA == njB);
        benchmark::DoNotOptimize(eq);
    }
}
BENCHMARK(CompareEqualNumbers_Nlohmann);

// Measures comparing two equal strings.
static void CompareEqualStrings_JsonPro(benchmark::State& state) {
    const Json jpA(std::string("a moderately sized benchmark string"));
    const Json jpB(std::string("a moderately sized benchmark string"));

    for (auto _ : state) {
        bool eq = (jpA == jpB);
        benchmark::DoNotOptimize(eq);
    }
}
BENCHMARK(CompareEqualStrings_JsonPro);

static void CompareEqualStrings_Nlohmann(benchmark::State& state) {
    const nlohmann::json njA(std::string("a moderately sized benchmark string"));
    const nlohmann::json njB(std::string("a moderately sized benchmark string"));

    for (auto _ : state) {
        bool eq = (njA == njB);
        benchmark::DoNotOptimize(eq);
    }
}
BENCHMARK(CompareEqualStrings_Nlohmann);

// Measures comparing two equal small arrays (5 elements).
static void CompareEqualArrays_JsonPro(benchmark::State& state) {
    const Json jpA(Json::ArrayType{Json(1), Json(2), Json(3), Json(4), Json(5)});
    const Json jpB(Json::ArrayType{Json(1), Json(2), Json(3), Json(4), Json(5)});

    for (auto _ : state) {
        bool eq = (jpA == jpB);
        benchmark::DoNotOptimize(eq);
    }
}
BENCHMARK(CompareEqualArrays_JsonPro);

static void CompareEqualArrays_Nlohmann(benchmark::State& state) {
    const nlohmann::json njA = nlohmann::json::array({1, 2, 3, 4, 5});
    const nlohmann::json njB = nlohmann::json::array({1, 2, 3, 4, 5});

    for (auto _ : state) {
        bool eq = (njA == njB);
        benchmark::DoNotOptimize(eq);
    }
}
BENCHMARK(CompareEqualArrays_Nlohmann);

// Measures comparing two equal small objects (5 members).
static void CompareEqualObjects_JsonPro(benchmark::State& state) {
    auto buildObj = [] {
        Json::ObjectType obj;
        obj.insert_or_assign("a", Json(1));
        obj.insert_or_assign("b", Json(2));
        obj.insert_or_assign("c", Json(3));
        obj.insert_or_assign("d", Json(4));
        obj.insert_or_assign("e", Json(5));
        return obj;
    };

    const Json jpA(buildObj());
    const Json jpB(buildObj());

    for (auto _ : state) {
        bool eq = (jpA == jpB);
        benchmark::DoNotOptimize(eq);
    }
}
BENCHMARK(CompareEqualObjects_JsonPro);

static void CompareEqualObjects_Nlohmann(benchmark::State& state) {
    const nlohmann::json njA =
        nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}});
    const nlohmann::json njB =
        nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}});

    for (auto _ : state) {
        bool eq = (njA == njB);
        benchmark::DoNotOptimize(eq);
    }
}
BENCHMARK(CompareEqualObjects_Nlohmann);

// Measures comparing two unequal small objects (5 members, last key
// differs) — the mismatch is only found after walking most of the object.
static void CompareUnequalObjects_JsonPro(benchmark::State& state) {
    Json::ObjectType objA;
    objA.insert_or_assign("a", Json(1));
    objA.insert_or_assign("b", Json(2));
    objA.insert_or_assign("c", Json(3));
    objA.insert_or_assign("d", Json(4));
    objA.insert_or_assign("e", Json(5));

    Json::ObjectType objB;
    objB.insert_or_assign("a", Json(1));
    objB.insert_or_assign("b", Json(2));
    objB.insert_or_assign("c", Json(3));
    objB.insert_or_assign("d", Json(4));
    objB.insert_or_assign("e", Json(999));

    const Json jpA(std::move(objA));
    const Json jpB(std::move(objB));

    for (auto _ : state) {
        bool eq = (jpA == jpB);
        benchmark::DoNotOptimize(eq);
    }
}
BENCHMARK(CompareUnequalObjects_JsonPro);

static void CompareUnequalObjects_Nlohmann(benchmark::State& state) {
    const nlohmann::json njA =
        nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}});
    const nlohmann::json njB =
        nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 999}});

    for (auto _ : state) {
        bool eq = (njA == njB);
        benchmark::DoNotOptimize(eq);
    }
}
BENCHMARK(CompareUnequalObjects_Nlohmann);
