// JsonPro serialization benchmarks.
//
// Measures Json::dump() throughput across scalar types, container sizes,
// a realistic nested document, and the two dump() entry points.
//
// Covers:
// - Scalar serialization (null, bool, number, string)
// - Array serialization: small vs large element count
// - Object serialization: few vs many keys
// - A realistic mixed object/array document
// - dump(int) -> string vs dump(ostream&) directly

#include <common/framework.h>

#include <sstream>
#include <string>

using namespace JsonPro;

// Builds a flat array of `count` integers.
static Json buildArray(int count) {
    Json::ArrayType arr;
    for (int i = 0; i < count; ++i)
        arr.push_back(Json(i));

    return Json(std::move(arr));
}

// Builds a flat object of `count` keys.
static Json buildObject(int count) {
    Json::ObjectType obj;
    for (int i = 0; i < count; ++i)
        obj.emplace("k" + std::to_string(i), Json(i));

    return Json(std::move(obj));
}

// Measures dumping each scalar type.
static void bench_dump_scalars() {
    Json null;
    Json boolean(true);
    Json number(3.14);
    Json string("hello");

    auto nullExpr = [&] {
        std::string r = null.dump();
        doNotOptimize(r);
    };
    BENCH("Dump null", LARGE, nullExpr);

    auto boolExpr = [&] {
        std::string r = boolean.dump();
        doNotOptimize(r);
    };
    BENCH("Dump bool", LARGE, boolExpr);

    auto numExpr = [&] {
        std::string r = number.dump();
        doNotOptimize(r);
    };
    BENCH("Dump number", LARGE, numExpr);

    auto strExpr = [&] {
        std::string r = string.dump();
        doNotOptimize(r);
    };
    BENCH("Dump string", LARGE, strExpr);
}

// Measures dumping arrays of small vs large element count.
static void bench_dump_array_size() {
    static const Json small = buildArray(10);
    static const Json large = buildArray(100);

    auto smallExpr = [&] {
        std::string r = small.dump();
        doNotOptimize(r);
    };
    BENCH("Dump array, 10 elements", LARGE, smallExpr);

    auto largeExpr = [&] {
        std::string r = large.dump();
        doNotOptimize(r);
    };
    BENCH("Dump array, 100 elements", MEDIUM, largeExpr);
}

// Measures dumping objects of few vs many keys.
static void bench_dump_object_size() {
    static const Json small = buildObject(10);
    static const Json large = buildObject(100);

    auto smallExpr = [&] {
        std::string r = small.dump();
        doNotOptimize(r);
    };
    BENCH("Dump object, 10 keys", LARGE, smallExpr);

    auto largeExpr = [&] {
        std::string r = large.dump();
        doNotOptimize(r);
    };
    BENCH("Dump object, 100 keys", MEDIUM, largeExpr);
}

// Measures dumping a realistic mixed object/array document.
static void bench_dump_mixed_document() {
    static const Json doc = Json::parse(
        R"({"name":"Rain","active":true,"score":42.5,)"
        R"("tags":["cpp","json","perf"],)"
        R"("meta":{"id":1,"nested":{"depth":2}}})");

    auto expr = [&] {
        std::string r = doc.dump();
        doNotOptimize(r);
    };
    BENCH("Dump mixed document", LARGE, expr);
}

// Measures dump(int) -> std::string vs writing directly to an ostream,
// isolating the cost of the internal ostringstream construction.
static void bench_dump_string_vs_ostream() {
    static const Json arr = buildArray(50);

    auto viaString = [&] {
        std::string r = arr.dump(0);
        doNotOptimize(r);
    };
    BENCH("Dump via dump(int) -> string", MEDIUM, viaString);

    auto viaStream = [&] {
        std::ostringstream oss;
        arr.dump(oss, 0);
        std::string r = oss.str();
        doNotOptimize(r);
    };
    BENCH("Dump via dump(ostream&)", MEDIUM, viaStream);
}

// Runs all serialization benchmarks.
static void run_benchmarks() {
    bench_dump_scalars();
    std::cout << "\n";

    bench_dump_array_size();
    std::cout << "\n";

    bench_dump_object_size();
    std::cout << "\n";

    bench_dump_mixed_document();
    std::cout << "\n";

    bench_dump_string_vs_ostream();
}

REGISTER_BENCH_SUITE();
