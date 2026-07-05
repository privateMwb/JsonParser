// JsonPro structural parsing benchmarks (arrays and objects).
//
// Measures Json::parse() throughput as array/object size and nesting
// depth scale, and for a realistic mixed document shape.
//
// Covers:
// - Array parsing: small vs large element count
// - Object parsing: few vs many keys (insert_or_assign cost)
// - Nesting depth: shallow vs deep
// - A realistic mixed object/array document

#include <common/framework.h>

#include <string>

using namespace JsonPro;

// Builds a flat JSON array of `count` integers, e.g. "[0,1,2,...]".
static std::string makeArray(int count) {
    std::string s = "[";
    for (int i = 0; i < count; ++i) {
        if (i != 0) s += ",";
        s += std::to_string(i);
    }
    s += "]";
    return s;
}

// Builds a flat JSON object of `count` keys, e.g. {"k0":0,"k1":1,...}.
static std::string makeObject(int count) {
    std::string s = "{";
    for (int i = 0; i < count; ++i) {
        if (i != 0) s += ",";
        s += "\"k" + std::to_string(i) + "\":" + std::to_string(i);
    }
    s += "}";
    return s;
}

// Builds an array nested `depth` levels deep, e.g. "[[[...]]]".
static std::string makeNestedArray(int depth) {
    std::string s(static_cast<std::size_t>(depth), '[');
    s.append(static_cast<std::size_t>(depth), ']');
    return s;
}

// Measures parsing arrays of small vs large element count.
static void bench_parse_array_size() {
    static const std::string small = makeArray(10);
    static const std::string large = makeArray(100);

    auto smallExpr = [&] {
        Json j = Json::parse(small);
        doNotOptimize(j);
    };
    BENCH("Parse array, 10 elements", LARGE, smallExpr);

    auto largeExpr = [&] {
        Json j = Json::parse(large);
        doNotOptimize(j);
    };
    BENCH("Parse array, 100 elements", MEDIUM, largeExpr);
}

// Measures parsing objects of few vs many keys.
static void bench_parse_object_size() {
    static const std::string small = makeObject(10);
    static const std::string large = makeObject(100);

    auto smallExpr = [&] {
        Json j = Json::parse(small);
        doNotOptimize(j);
    };
    BENCH("Parse object, 10 keys", LARGE, smallExpr);

    auto largeExpr = [&] {
        Json j = Json::parse(large);
        doNotOptimize(j);
    };
    BENCH("Parse object, 100 keys", MEDIUM, largeExpr);
}

// Measures parsing shallow vs deep array nesting.
static void bench_parse_nesting_depth() {
    static const std::string shallow = makeNestedArray(10);
    static const std::string deep    = makeNestedArray(100);

    auto shallowExpr = [&] {
        Json j = Json::parse(shallow);
        doNotOptimize(j);
    };
    BENCH("Parse nesting, depth 10", LARGE, shallowExpr);

    auto deepExpr = [&] {
        Json j = Json::parse(deep);
        doNotOptimize(j);
    };
    BENCH("Parse nesting, depth 100", SMALL, deepExpr);
}

// Measures parsing a realistic mixed object/array document.
static void bench_parse_mixed_document() {
    static const std::string input =
        R"({"name":"Rain","active":true,"score":42.5,)"
        R"("tags":["cpp","json","perf"],)"
        R"("meta":{"id":1,"nested":{"depth":2}}})";

    auto expr = [&] {
        Json j = Json::parse(input);
        doNotOptimize(j);
    };
    BENCH("Parse mixed document", LARGE, expr);
}

// Runs all structural parsing benchmarks.
static void run_benchmarks() {
    bench_parse_array_size();
    std::cout << "\n";

    bench_parse_object_size();
    std::cout << "\n";

    bench_parse_nesting_depth();
    std::cout << "\n";

    bench_parse_mixed_document();
}

REGISTER_BENCH_SUITE();
