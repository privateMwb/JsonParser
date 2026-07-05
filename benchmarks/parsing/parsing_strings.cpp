// JsonPro string parsing benchmarks.
//
// Measures Json::parse() throughput for string values across shapes that
// stress the bulk-copy fast path vs the escape-handling slow path.
//
// Covers:
// - Short vs long plain (unescaped) strings
// - Strings with no escapes vs strings with escapes
// - Unicode \u escapes: BMP codepoint vs surrogate pair
// - A string dominated by many escape sequences

#include <common/framework.h>

#include <string>

using namespace JsonPro;

// Measures parsing a short plain string.
static void bench_parse_short_string() {
    auto expr = [&] {
        Json j = Json::parse("\"hello\"");
        doNotOptimize(j);
    };
    BENCH("Parse short string", LARGE, expr);
}

// Measures parsing a long plain string (bulk-copy fast path scaling).
static void bench_parse_long_string() {
    static const std::string input = "\"" + std::string(1000, 'a') + "\"";

    auto expr = [&] {
        Json j = Json::parse(input);
        doNotOptimize(j);
    };
    BENCH("Parse 1000-char string", MEDIUM, expr);
}

// Measures parsing a string with no escapes vs one with several escapes.
static void bench_parse_escaped_string() {
    auto plain = [&] {
        Json j = Json::parse("\"no escapes here\"");
        doNotOptimize(j);
    };
    BENCH("Parse string, no escapes", LARGE, plain);

    auto escaped = [&] {
        Json j = Json::parse(R"("line1\nline2\ttabbed\\end")");
        doNotOptimize(j);
    };
    BENCH("Parse string, with escapes", LARGE, escaped);
}

// Measures parsing a BMP \u escape vs a surrogate pair (astral codepoint).
static void bench_parse_unicode_string() {
    auto bmp = [&] {
        Json j = Json::parse(R"("\u00e9\u00e8\u00ea")");
        doNotOptimize(j);
    };
    BENCH("Parse BMP unicode escapes", LARGE, bmp);

    auto surrogate = [&] {
        Json j = Json::parse(R"("\uD83D\uDE00")");
        doNotOptimize(j);
    };
    BENCH("Parse surrogate pair", LARGE, surrogate);
}

// Measures parsing a string dominated by many escape sequences,
// stressing the segment-flush path in the string scanner.
static void bench_parse_many_escapes() {
    static const std::string input = [] {
        std::string s = "\"";
        for (int i = 0; i < 200; ++i)
            s += "\\n";
        s += "\"";
        return s;
    }();

    auto expr = [&] {
        Json j = Json::parse(input);
        doNotOptimize(j);
    };
    BENCH("Parse string, 200 escapes", MEDIUM, expr);
}

// Runs all string parsing benchmarks.
static void run_benchmarks() {
    bench_parse_short_string();
    std::cout << "\n";

    bench_parse_long_string();
    std::cout << "\n";

    bench_parse_escaped_string();
    std::cout << "\n";

    bench_parse_unicode_string();
    std::cout << "\n";

    bench_parse_many_escapes();
}

REGISTER_BENCH_SUITE();
