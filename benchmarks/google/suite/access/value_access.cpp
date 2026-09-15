// Json Value Access Benchmark Suite
// Measures Json's typed accessors (asBool/asNumber/asString/asArray/
// asObject) against nlohmann::json's equivalents.
//
// asString()/asArray()/asObject() return references with no copy, so the
// nlohmann side uses get_ref<T&>() rather than get<T>() (which would copy)
// to keep the comparison fair.
//
// Covers:
// - asBool()
// - asNumber()
// - asString()
// - asArray()
// - asObject()

#include <benchmark/benchmark.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Measures asBool() access performance.
static void AsBool_JsonPro(benchmark::State& state) {
    const Json jpSrc(true);

    for (auto _ : state) {
        bool v = jpSrc.asBool();
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(AsBool_JsonPro);

static void AsBool_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc(true);

    for (auto _ : state) {
        bool v = njSrc.get<bool>();
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(AsBool_Nlohmann);

// Measures asNumber() access performance.
static void AsNumber_JsonPro(benchmark::State& state) {
    const Json jpSrc(42.0);

    for (auto _ : state) {
        double v = jpSrc.asNumber();
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(AsNumber_JsonPro);

static void AsNumber_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc(42.0);

    for (auto _ : state) {
        double v = njSrc.get<double>();
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(AsNumber_Nlohmann);

// Measures asString() access performance. Both sides return a reference to
// the existing string — no copy on either side.
static void AsString_JsonPro(benchmark::State& state) {
    const Json jpSrc(std::string("a moderately sized benchmark string"));

    for (auto _ : state) {
        const std::string& v = jpSrc.asString();
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(AsString_JsonPro);

static void AsString_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc(std::string("a moderately sized benchmark string"));

    for (auto _ : state) {
        const auto& v = njSrc.get_ref<const std::string&>();
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(AsString_Nlohmann);

// Measures asArray() access performance. Both sides return a reference —
// no copy of the underlying container on either side.
static void AsArray_JsonPro(benchmark::State& state) {
    const Json jpSrc(Json::ArrayType{Json(1), Json(2), Json(3), Json(4), Json(5)});

    for (auto _ : state) {
        const Json::ArrayType& v = jpSrc.asArray();
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(AsArray_JsonPro);

static void AsArray_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc = nlohmann::json::array({1, 2, 3, 4, 5});

    for (auto _ : state) {
        const auto& v = njSrc.get_ref<const nlohmann::json::array_t&>();
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(AsArray_Nlohmann);

// Measures asObject() access performance. Both sides return a reference —
// no copy of the underlying container on either side.
static void AsObject_JsonPro(benchmark::State& state) {
    Json::ObjectType obj;
    obj.insert_or_assign("a", Json(1));
    obj.insert_or_assign("b", Json(2));
    obj.insert_or_assign("c", Json(3));
    obj.insert_or_assign("d", Json(4));
    obj.insert_or_assign("e", Json(5));
    const Json jpSrc(std::move(obj));

    for (auto _ : state) {
        const Json::ObjectType& v = jpSrc.asObject();
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(AsObject_JsonPro);

static void AsObject_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc =
        nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}});

    for (auto _ : state) {
        const auto& v = njSrc.get_ref<const nlohmann::json::object_t&>();
        benchmark::DoNotOptimize(v);
    }
}
BENCHMARK(AsObject_Nlohmann);
