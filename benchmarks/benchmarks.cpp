// Json Benchmark Suite
// Measures performance of Json parsing, construction, and serialization:
//
// - parse vs construct (each type)
// - parse small vs large array
// - parse small vs large object
// - parse nested structure
// - dump string vs dump stream
// - operator[] vs at() access
//
// Benchmarks compare parsing against direct construction and
// dump(string) against dump(ostream) to guide usage decisions.

#include <iostream>
#include <chrono>
#include <sstream>
#include <string>
#include <vector>

#include "Json.h"
#include "utils/Table.h"

// returns elapsed microseconds for a callable
template<typename F>
auto duration(F func) {
    auto start = std::chrono::steady_clock::now();
    func();
    auto end   = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

// prevents the compiler from eliminating unused operations
template<typename T>
inline void doNotOptimize(const T& value) {
#if defined(__GNUC__) || defined(__clang__)
    asm volatile("" : : "g"(value) : "memory");
#else
    volatile const T* p = &value;
    (void)p;
#endif
}

// Parse vs Construct
// measures parse() overhead against direct construction for each type
void parseVsConstruct() {
    static constexpr std::size_t ITERS = 100'000;

    std::vector<std::string> types     = { "null", "bool", "number", "string" };
    std::vector<long>        parseTimes;
    std::vector<long>        ctorTimes;

    // null
    {
        auto parseTime = duration([&] {
            for (std::size_t i = 0; i < ITERS; ++i) {
                Json j = Json::parse("null");
                doNotOptimize(j.isNull());
            }
        });

        auto ctorTime = duration([&] {
            for (std::size_t i = 0; i < ITERS; ++i) {
                Json j;
                doNotOptimize(j.isNull());
            }
        });

        parseTimes.push_back(parseTime.count());
        ctorTimes.push_back(ctorTime.count());
    }

    // bool
    {
        auto parseTime = duration([&] {
            for (std::size_t i = 0; i < ITERS; ++i) {
                Json j = Json::parse("true");
                doNotOptimize(j.isBool());
            }
        });

        auto ctorTime = duration([&] {
            for (std::size_t i = 0; i < ITERS; ++i) {
                Json j(true);
                doNotOptimize(j.isBool());
            }
        });

        parseTimes.push_back(parseTime.count());
        ctorTimes.push_back(ctorTime.count());
    }

    // number
    {
        auto parseTime = duration([&] {
            for (std::size_t i = 0; i < ITERS; ++i) {
                Json j = Json::parse("3.14");
                doNotOptimize(j.isNumber());
            }
        });

        auto ctorTime = duration([&] {
            for (std::size_t i = 0; i < ITERS; ++i) {
                Json j(3.14);
                doNotOptimize(j.isNumber());
            }
        });

        parseTimes.push_back(parseTime.count());
        ctorTimes.push_back(ctorTime.count());
    }

    // string
    {
        auto parseTime = duration([&] {
            for (std::size_t i = 0; i < ITERS; ++i) {
                Json j = Json::parse("\"hello world\"");
                doNotOptimize(j.isString());
            }
        });

        auto ctorTime = duration([&] {
            for (std::size_t i = 0; i < ITERS; ++i) {
                Json j("hello world");
                doNotOptimize(j.isString());
            }
        });

        parseTimes.push_back(parseTime.count());
        ctorTimes.push_back(ctorTime.count());
    }

    std::vector<std::vector<std::string>> data{
        types,
        Table::convert(parseTimes, "us"),
        Table::convert(ctorTimes,  "us")
    };

    Table::table(
        "Parse vs Construct  (" + std::to_string(ITERS) + " iters)",
        { "Type", "parse()", "construct" },
        data, 56);
}

// Array — Small vs Large
// measures parse cost as array size grows
void parseArray() {
    std::vector<std::size_t> sizes = { 10, 100, 1'000, 10'000 };
    std::vector<long>        parseTimes;
    std::vector<long>        ctorTimes;

    for (std::size_t size : sizes) {
        // build parse string
        std::string text = "[";
        for (std::size_t i = 0; i < size; ++i) {
            text += std::to_string(i);
            if (i + 1 != size) text += ",";
        }
        text += "]";

        auto parseTime = duration([&] {
            Json j = Json::parse(text);
            doNotOptimize(j.size());
        });

        // build equivalent via construction
        auto ctorTime = duration([&] {
            Json::ArrayType arr;
            arr.reserve(size);
            for (std::size_t i = 0; i < size; ++i)
                arr.push_back(Json(static_cast<int>(i)));
            Json j(std::move(arr));
            doNotOptimize(j.size());
        });

        parseTimes.push_back(parseTime.count());
        ctorTimes.push_back(ctorTime.count());
    }

    std::vector<std::vector<std::string>> data{
        Table::convert(sizes),
        Table::convert(parseTimes, "us"),
        Table::convert(ctorTimes,  "us")
    };

    Table::table(
        "Array - Parse vs Construct",
        { "Size", "parse()", "construct" },
        data, 56);
}

// Object — Small vs Large
// measures parse cost as object size grows
void parseObject() {
    std::vector<std::size_t> sizes = { 10, 100, 1'000, 10'000 };
    std::vector<long>        parseTimes;
    std::vector<long>        ctorTimes;

    for (std::size_t size : sizes) {
        // build parse string
        std::string text = "{";
        for (std::size_t i = 0; i < size; ++i) {
            text += "\"key" + std::to_string(i) + "\":" + std::to_string(i);
            if (i + 1 != size) text += ",";
        }
        text += "}";

        auto parseTime = duration([&] {
            Json j = Json::parse(text);
            doNotOptimize(j.size());
        });

        auto ctorTime = duration([&] {
            Json::ObjectType obj;
            for (std::size_t i = 0; i < size; ++i)
                obj["key" + std::to_string(i)] = Json(static_cast<int>(i));
            Json j(std::move(obj));
            doNotOptimize(j.size());
        });

        parseTimes.push_back(parseTime.count());
        ctorTimes.push_back(ctorTime.count());
    }

    std::vector<std::vector<std::string>> data{
        Table::convert(sizes),
        Table::convert(parseTimes, "us"),
        Table::convert(ctorTimes,  "us")
    };

    Table::table(
        "Object - Parse vs Construct",
        { "Size", "parse()", "construct" },
        data, 56);
}

// Nested Structure
// measures parse and construct cost as nesting depth grows
void parseNested() {
    std::vector<std::size_t> depths = { 2, 4, 8, 16 };
    std::vector<long>        parseTimes;
    std::vector<long>        ctorTimes;

    for (std::size_t depth : depths) {
        // build nested array string: [[[[...]]]]
        std::string text;
        for (std::size_t i = 0; i < depth; ++i) text += "[";
        text += "42";
        for (std::size_t i = 0; i < depth; ++i) text += "]";

        auto parseTime = duration([&] {
            Json j = Json::parse(text);
            doNotOptimize(j.isArray());
        });

        auto ctorTime = duration([&] {
            Json j(42);
            for (std::size_t i = 0; i < depth; ++i)
                j = Json(Json::ArrayType{ std::move(j) });
            doNotOptimize(j.isArray());
        });

        parseTimes.push_back(parseTime.count());
        ctorTimes.push_back(ctorTime.count());
    }

    std::vector<std::vector<std::string>> data{
        Table::convert(depths),
        Table::convert(parseTimes, "us"),
        Table::convert(ctorTimes,  "us")
    };

    Table::table(
        "Nested - Parse vs Construct",
        { "Depth", "parse()", "construct" },
        data, 56);
}

// Dump String vs Stream
// measures dump(string) against dump(ostream) to show allocation cost difference
void dumpStringVsStream() {
    std::vector<std::size_t> sizes = { 10, 100, 1'000, 10'000 };
    std::vector<long>        stringTimes;
    std::vector<long>        streamTimes;

    for (std::size_t size : sizes) {
        Json::ArrayType arr;
        arr.reserve(size);
        for (std::size_t i = 0; i < size; ++i)
            arr.push_back(Json(static_cast<int>(i)));
        Json j(std::move(arr));

        auto stringTime = duration([&] {
            std::string s = j.dump();
            doNotOptimize(s.size());
        });

        auto streamTime = duration([&] {
            std::ostringstream oss;
            j.dump(oss);
            doNotOptimize(oss.str().size());
        });

        stringTimes.push_back(stringTime.count());
        streamTimes.push_back(streamTime.count());
    }

    std::vector<std::vector<std::string>> data{
        Table::convert(sizes),
        Table::convert(stringTimes, "us"),
        Table::convert(streamTimes, "us")
    };

    Table::table(
        "Dump - String vs Stream",
        { "Size", "dump(string)", "dump(stream)" },
        data, 56);
}

// operator[] vs at()
// measures unchecked access against bounds-checked access on arrays and objects
void accessOperators() {
    static constexpr std::size_t SIZE  = 1'000;
    static constexpr std::size_t ITERS = 10'000;

    // array
    {
        Json::ArrayType arr;
        arr.reserve(SIZE);
        for (std::size_t i = 0; i < SIZE; ++i)
            arr.push_back(Json(static_cast<int>(i)));
        Json j(std::move(arr));

        std::vector<long> bracketTimes;
        std::vector<long> atTimes;

        auto bracketTime = duration([&] {
            for (std::size_t i = 0; i < ITERS; ++i) {
                doNotOptimize(j[i % SIZE].asNumber());
            }
        });

        auto atTime = duration([&] {
            for (std::size_t i = 0; i < ITERS; ++i) {
                doNotOptimize(j.at(i % SIZE).asNumber());
            }
        });

        bracketTimes.push_back(bracketTime.count());
        atTimes.push_back(atTime.count());

        std::vector<std::vector<std::string>> data{
            { "Array" },
            Table::convert(bracketTimes, "us"),
            Table::convert(atTimes,      "us")
        };

        Table::table(
            "Array Access - operator[] vs at()  (" + std::to_string(ITERS) + " iters)",
            { "Type", "operator[]", "at()" },
            data, 56);
    }

    // object
    {
        Json::ObjectType obj;
        for (std::size_t i = 0; i < SIZE; ++i)
            obj["key" + std::to_string(i)] = Json(static_cast<int>(i));
        Json j(std::move(obj));

        std::vector<long> bracketTimes;
        std::vector<long> atTimes;

        auto bracketTime = duration([&] {
            for (std::size_t i = 0; i < ITERS; ++i) {
                doNotOptimize(j["key" + std::to_string(i % SIZE)].asNumber());
            }
        });

        auto atTime = duration([&] {
            for (std::size_t i = 0; i < ITERS; ++i) {
                doNotOptimize(j.at("key" + std::to_string(i % SIZE)).asNumber());
            }
        });

        bracketTimes.push_back(bracketTime.count());
        atTimes.push_back(atTime.count());

        std::vector<std::vector<std::string>> data{
            { "Object" },
            Table::convert(bracketTimes, "us"),
            Table::convert(atTimes,      "us")
        };

        Table::table(
            "Object Access - operator[] vs at()  (" + std::to_string(ITERS) + " iters)",
            { "Type", "operator[]", "at()" },
            data, 56);
    }
}

// Entry Point
int main() {
    parseVsConstruct();
    parseArray();
    parseObject();
    parseNested();
    dumpStringVsStream();
    accessOperators();

    return 0;
}

