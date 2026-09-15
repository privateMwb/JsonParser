// Json Type Inspection Benchmark Suite
// Measures Json::type()/isXxx() performance against nlohmann::json's
// type()/is_xxx() equivalents.
//
// Covers:
// - type()
// - isNull() / isBool() / isNumber() / isString() / isArray() / isObject()

#include <benchmark/benchmark.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Measures type() on a number value.
static void Type_JsonPro(benchmark::State& state) {
    const Json jpSrc(42.0);

    for (auto _ : state) {
        Json::Type t = jpSrc.type();
        benchmark::DoNotOptimize(t);
    }
}
BENCHMARK(Type_JsonPro);

static void Type_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc(42.0);

    for (auto _ : state) {
        auto t = njSrc.type();
        benchmark::DoNotOptimize(t);
    }
}
BENCHMARK(Type_Nlohmann);

// Measures isNull() on a null value.
static void IsNull_JsonPro(benchmark::State& state) {
    const Json jpSrc;

    for (auto _ : state) {
        bool b = jpSrc.isNull();
        benchmark::DoNotOptimize(b);
    }
}
BENCHMARK(IsNull_JsonPro);

static void IsNull_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc;

    for (auto _ : state) {
        bool b = njSrc.is_null();
        benchmark::DoNotOptimize(b);
    }
}
BENCHMARK(IsNull_Nlohmann);

// Measures isBool() on a bool value.
static void IsBool_JsonPro(benchmark::State& state) {
    const Json jpSrc(true);

    for (auto _ : state) {
        bool b = jpSrc.isBool();
        benchmark::DoNotOptimize(b);
    }
}
BENCHMARK(IsBool_JsonPro);

static void IsBool_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc(true);

    for (auto _ : state) {
        bool b = njSrc.is_boolean();
        benchmark::DoNotOptimize(b);
    }
}
BENCHMARK(IsBool_Nlohmann);

// Measures isNumber() on a number value.
static void IsNumber_JsonPro(benchmark::State& state) {
    const Json jpSrc(42.0);

    for (auto _ : state) {
        bool b = jpSrc.isNumber();
        benchmark::DoNotOptimize(b);
    }
}
BENCHMARK(IsNumber_JsonPro);

static void IsNumber_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc(42.0);

    for (auto _ : state) {
        bool b = njSrc.is_number();
        benchmark::DoNotOptimize(b);
    }
}
BENCHMARK(IsNumber_Nlohmann);

// Measures isString() on a string value.
static void IsString_JsonPro(benchmark::State& state) {
    const Json jpSrc(std::string("hello"));

    for (auto _ : state) {
        bool b = jpSrc.isString();
        benchmark::DoNotOptimize(b);
    }
}
BENCHMARK(IsString_JsonPro);

static void IsString_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc(std::string("hello"));

    for (auto _ : state) {
        bool b = njSrc.is_string();
        benchmark::DoNotOptimize(b);
    }
}
BENCHMARK(IsString_Nlohmann);

// Measures isArray() on an array value.
static void IsArray_JsonPro(benchmark::State& state) {
    const Json jpSrc(Json::ArrayType{Json(1), Json(2), Json(3)});

    for (auto _ : state) {
        bool b = jpSrc.isArray();
        benchmark::DoNotOptimize(b);
    }
}
BENCHMARK(IsArray_JsonPro);

static void IsArray_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc = nlohmann::json::array({1, 2, 3});

    for (auto _ : state) {
        bool b = njSrc.is_array();
        benchmark::DoNotOptimize(b);
    }
}
BENCHMARK(IsArray_Nlohmann);

// Measures isObject() on an object value.
static void IsObject_JsonPro(benchmark::State& state) {
    Json::ObjectType obj;
    obj.insert_or_assign("a", Json(1));
    const Json jpSrc(std::move(obj));

    for (auto _ : state) {
        bool b = jpSrc.isObject();
        benchmark::DoNotOptimize(b);
    }
}
BENCHMARK(IsObject_JsonPro);

static void IsObject_Nlohmann(benchmark::State& state) {
    const nlohmann::json njSrc = nlohmann::json::object({{"a", 1}});

    for (auto _ : state) {
        bool b = njSrc.is_object();
        benchmark::DoNotOptimize(b);
    }
}
BENCHMARK(IsObject_Nlohmann);
