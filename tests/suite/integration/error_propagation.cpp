// Json error propagation integration test suite.
//
// Coverage:
// - JsonParseError carries the correct 1-based line and column
// - A parse failure can be caught via the JsonException base class
// - JsonTypeError propagates out of a multi-step navigation chain
// - JsonOutOfRange propagates out of a multi-step navigation chain
// - A caller can recover from a parse failure with a fallback document
// - Every JsonPro exception type is catchable uniformly via JsonException

#include <string>

#include <JsonPro/JsonException.h>

#include <common/framework.h>

using namespace JsonPro;

// Verifies JsonParseError reports the exact line and column of the failure.
static void parse_error_carries_line_and_column() {
    const std::string bad = "{\n"
                            "  \"a\": 1,\n"
                            "  \"b\": @\n"
                            "}";

    try {
        (void)Json::parse(bad);
        CHK(false); // unreachable: parse() must throw
    } catch (const JsonParseError& e) {
        CHK(e.line() == 3);
        CHK(e.column() == 8);
    }
}

// Verifies a parse failure can be caught generically via the JsonException base class.
static void catch_parse_error_as_base_exception() {
    bool caught = false;

    try {
        (void)Json::parse("{invalid}");
    } catch (const JsonException& e) {
        caught = true;
        CHK(std::string(e.what()).empty() == false);
    }

    CHK(caught == true);
}

// Verifies a type mismatch partway through a navigation chain throws JsonTypeError.
static void type_error_thrown_mid_navigation_chain() {
    Json j = Json::parse(R"({"list": [1, 2, 3]})");

    bool caught = false;
    try {
        (void)j["list"]["not_a_key"]; // "list" is an array, not an object
    } catch (const JsonTypeError&) {
        caught = true;
    }

    CHK(caught == true);
}

// Verifies an out-of-range access partway through a navigation chain throws JsonOutOfRange.
static void out_of_range_thrown_mid_navigation_chain() {
    Json j = Json::parse(R"({"items": [1, 2]})");

    bool caught = false;
    try {
        (void)j.at("items").at(10);
    } catch (const JsonOutOfRange&) {
        caught = true;
    }

    CHK(caught == true);
}

// Verifies a caller can recover from a parse failure by falling back to a
// known-good document.
static void recover_from_parse_failure_with_fallback_document() {
    const std::string userInput = "{ this is not valid json";
    Json result;

    try {
        result = Json::parse(userInput);
    } catch (const JsonParseError&) {
        result = Json::parse(R"({"status": "fallback"})");
    }

    CHK(result.isObject() == true);
    CHK(result["status"].asString() == "fallback");
}

// Verifies parse errors, type errors, and out-of-range errors are all
// catchable uniformly via the JsonException base class.
static void catch_all_json_exceptions_via_base_class() {
    int caughtCount = 0;

    auto tryCatch = [&](auto&& fn) {
        try {
            fn();
        } catch (const JsonException&) {
            ++caughtCount;
        }
    };

    tryCatch([] { (void)Json::parse("not json"); });
    tryCatch([] {
        Json j(1.0);
        (void)j.asString();
    });
    tryCatch([] {
        Json j(Json::ArrayType{});
        (void)j.at(0);
    });

    CHK(caughtCount == 3);
}

// Executes all error propagation test cases.
static void run_tests() {
    RUN(parse_error_carries_line_and_column);
    RUN(catch_parse_error_as_base_exception);
    RUN(type_error_thrown_mid_navigation_chain);
    RUN(out_of_range_thrown_mid_navigation_chain);
    RUN(recover_from_parse_failure_with_fallback_document);
    RUN(catch_all_json_exceptions_via_base_class);
}

REGISTER_TEST_SUITE();
