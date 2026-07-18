// Json Type Inspection Benchmark Suite
// Measures Json::type()/isXxx() performance against nlohmann::json's
// type()/is_xxx() equivalents.
//
// Each lambda performs exactly one call — BENCH sweeps iteration counts
// itself.
//
// Covers:
// - type()
// - isNull() / isBool() / isNumber() / isString() / isArray() / isObject()

#include <common/framework.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Measures type() on a number value.
static void bench_type() {
    const Json jpSrc(42.0);
    const nlohmann::json njSrc(42.0);

    auto jp = [&] {
        Json::Type t = jpSrc.type();
        doNotOptimize(t);
    };

    auto nj = [&] {
        auto t = njSrc.type();
        doNotOptimize(t);
    };

    BENCH("type()", jp, nj);
}

// Measures isNull() on a null value.
static void bench_is_null() {
    const Json jpSrc;
    const nlohmann::json njSrc;

    auto jp = [&] {
        bool b = jpSrc.isNull();
        doNotOptimize(b);
    };

    auto nj = [&] {
        bool b = njSrc.is_null();
        doNotOptimize(b);
    };

    BENCH("isNull()", jp, nj);
}

// Measures isBool() on a bool value.
static void bench_is_bool() {
    const Json jpSrc(true);
    const nlohmann::json njSrc(true);

    auto jp = [&] {
        bool b = jpSrc.isBool();
        doNotOptimize(b);
    };

    auto nj = [&] {
        bool b = njSrc.is_boolean();
        doNotOptimize(b);
    };

    BENCH("isBool()", jp, nj);
}

// Measures isNumber() on a number value.
static void bench_is_number() {
    const Json jpSrc(42.0);
    const nlohmann::json njSrc(42.0);

    auto jp = [&] {
        bool b = jpSrc.isNumber();
        doNotOptimize(b);
    };

    auto nj = [&] {
        bool b = njSrc.is_number();
        doNotOptimize(b);
    };

    BENCH("isNumber()", jp, nj);
}

// Measures isString() on a string value.
static void bench_is_string() {
    const Json jpSrc(std::string("hello"));
    const nlohmann::json njSrc(std::string("hello"));

    auto jp = [&] {
        bool b = jpSrc.isString();
        doNotOptimize(b);
    };

    auto nj = [&] {
        bool b = njSrc.is_string();
        doNotOptimize(b);
    };

    BENCH("isString()", jp, nj);
}

// Measures isArray() on an array value.
static void bench_is_array() {
    const Json jpSrc(Json::ArrayType{Json(1), Json(2), Json(3)});
    const nlohmann::json njSrc = nlohmann::json::array({1, 2, 3});

    auto jp = [&] {
        bool b = jpSrc.isArray();
        doNotOptimize(b);
    };

    auto nj = [&] {
        bool b = njSrc.is_array();
        doNotOptimize(b);
    };

    BENCH("isArray()", jp, nj);
}

// Measures isObject() on an object value.
static void bench_is_object() {
    Json::ObjectType obj;
    obj.insert_or_assign("a", Json(1));
    const Json jpSrc(std::move(obj));

    const nlohmann::json njSrc = nlohmann::json::object({{"a", 1}});

    auto jp = [&] {
        bool b = jpSrc.isObject();
        doNotOptimize(b);
    };

    auto nj = [&] {
        bool b = njSrc.is_object();
        doNotOptimize(b);
    };

    BENCH("isObject()", jp, nj);
}

// Executes all type inspection benchmark cases.
static void run_benchmarks() {
    bench_type();
    std::cout << "\n";

    bench_is_null();
    std::cout << "\n";

    bench_is_bool();
    std::cout << "\n";

    bench_is_number();
    std::cout << "\n";

    bench_is_string();
    std::cout << "\n";

    bench_is_array();
    std::cout << "\n";

    bench_is_object();
}

REGISTER_BENCH_SUITE();
