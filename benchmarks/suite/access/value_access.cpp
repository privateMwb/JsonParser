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

#include <common/framework.h>

#include <nlohmann/json.hpp>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Measures asBool() access performance.
static void bench_as_bool() {
    const Json jpSrc(true);
    const nlohmann::json njSrc(true);

    auto jp = [&] {
        bool v = jpSrc.asBool();
        doNotOptimize(v);
    };

    auto nj = [&] {
        bool v = njSrc.get<bool>();
        doNotOptimize(v);
    };

    BENCH("asBool()", jp, nj);
}

// Measures asNumber() access performance.
static void bench_as_number() {
    const Json jpSrc(42.0);
    const nlohmann::json njSrc(42.0);

    auto jp = [&] {
        double v = jpSrc.asNumber();
        doNotOptimize(v);
    };

    auto nj = [&] {
        double v = njSrc.get<double>();
        doNotOptimize(v);
    };

    BENCH("asNumber()", jp, nj);
}

// Measures asString() access performance. Both sides return a reference to
// the existing string — no copy on either side.
static void bench_as_string() {
    const Json jpSrc(std::string("a moderately sized benchmark string"));
    const nlohmann::json njSrc(std::string("a moderately sized benchmark string"));

    auto jp = [&] {
        const std::string& v = jpSrc.asString();
        doNotOptimize(v);
    };

    auto nj = [&] {
        const auto& v = njSrc.get_ref<const std::string&>();
        doNotOptimize(v);
    };

    BENCH("asString()", jp, nj);
}

// Measures asArray() access performance. Both sides return a reference —
// no copy of the underlying container on either side.
static void bench_as_array() {
    const Json jpSrc(Json::ArrayType{Json(1), Json(2), Json(3), Json(4), Json(5)});
    const nlohmann::json njSrc = nlohmann::json::array({1, 2, 3, 4, 5});

    auto jp = [&] {
        const Json::ArrayType& v = jpSrc.asArray();
        doNotOptimize(v);
    };

    auto nj = [&] {
        const auto& v = njSrc.get_ref<const nlohmann::json::array_t&>();
        doNotOptimize(v);
    };

    BENCH("asArray()", jp, nj);
}

// Measures asObject() access performance. Both sides return a reference —
// no copy of the underlying container on either side.
static void bench_as_object() {
    Json::ObjectType obj;
    obj.insert_or_assign("a", Json(1));
    obj.insert_or_assign("b", Json(2));
    obj.insert_or_assign("c", Json(3));
    obj.insert_or_assign("d", Json(4));
    obj.insert_or_assign("e", Json(5));
    const Json jpSrc(std::move(obj));

    const nlohmann::json njSrc =
        nlohmann::json::object({{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}, {"e", 5}});

    auto jp = [&] {
        const Json::ObjectType& v = jpSrc.asObject();
        doNotOptimize(v);
    };

    auto nj = [&] {
        const auto& v = njSrc.get_ref<const nlohmann::json::object_t&>();
        doNotOptimize(v);
    };

    BENCH("asObject()", jp, nj);
}

// Executes all value access benchmark cases.
static void run_benchmarks() {
    bench_as_bool();
    std::cout << "\n";

    bench_as_number();
    std::cout << "\n";

    bench_as_string();
    std::cout << "\n";

    bench_as_array();
    std::cout << "\n";

    bench_as_object();
}

REGISTER_BENCH_SUITE();
