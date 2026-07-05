// JsonPro scalar parsing benchmarks.
//
// Measures Json::parse() throughput for null/bool/number literals across
// input shapes that stress different code paths in the number grammar
// (digit count, fractional part, exponent sign).
//
// Covers:
// - null / bool literals
// - integers of varying digit width
// - fractional numbers of varying precision
// - exponent forms (positive and negative)

#include <common/framework.h>

using namespace JsonPro;

// Measures parsing the "null" literal.
static void bench_parse_null() {
    auto expr = [&] {
        Json j = Json::parse("null");
        doNotOptimize(j);
    };
    BENCH("Parse null", LARGE, expr);
}

// Measures parsing "true" vs "false" literals.
static void bench_parse_bool() {
    auto t = [&] {
        Json j = Json::parse("true");
        doNotOptimize(j);
    };
    BENCH("Parse bool true", LARGE, t);

    auto f = [&] {
        Json j = Json::parse("false");
        doNotOptimize(j);
    };
    BENCH("Parse bool false", LARGE, f);
}

// Measures parsing integers of small vs large digit width.
static void bench_parse_integer() {
    auto small = [&] {
        Json j = Json::parse("7");
        doNotOptimize(j);
    };
    BENCH("Parse small integer", LARGE, small);

    auto large = [&] {
        Json j = Json::parse("123456789");
        doNotOptimize(j);
    };
    BENCH("Parse large integer", LARGE, large);
}

// Measures parsing negative vs positive integers (extra sign branch).
static void bench_parse_negative_integer() {
    auto pos = [&] {
        Json j = Json::parse("42");
        doNotOptimize(j);
    };
    BENCH("Parse positive integer", LARGE, pos);

    auto neg = [&] {
        Json j = Json::parse("-42");
        doNotOptimize(j);
    };
    BENCH("Parse negative integer", LARGE, neg);
}

// Measures parsing fractional numbers of varying precision.
static void bench_parse_float() {
    auto simple = [&] {
        Json j = Json::parse("3.14");
        doNotOptimize(j);
    };
    BENCH("Parse simple float", LARGE, simple);

    auto precise = [&] {
        Json j = Json::parse("3.14159265358979");
        doNotOptimize(j);
    };
    BENCH("Parse high-precision float", LARGE, precise);
}

// Measures parsing exponent forms (positive vs negative exponent sign).
static void bench_parse_exponent() {
    auto pos = [&] {
        Json j = Json::parse("1e10");
        doNotOptimize(j);
    };
    BENCH("Parse positive exponent", LARGE, pos);

    auto neg = [&] {
        Json j = Json::parse("1e-10");
        doNotOptimize(j);
    };
    BENCH("Parse negative exponent", LARGE, neg);
}

// Runs all scalar parsing benchmarks.
static void run_benchmarks() {
    bench_parse_null();
    std::cout << "\n";

    bench_parse_bool();
    std::cout << "\n";

    bench_parse_integer();
    std::cout << "\n";

    bench_parse_negative_integer();
    std::cout << "\n";

    bench_parse_float();
    std::cout << "\n";

    bench_parse_exponent();
}

REGISTER_BENCH_SUITE();
