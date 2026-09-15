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

#include <gtest/gtest.h>

#include <JsonPro/Json.h>
#include <JsonPro/JsonException.h>

using namespace JsonPro;

// Verifies JsonParseError reports the exact line and column of the failure.
TEST(ErrorPropagation, ParseErrorCarriesLineAndColumn) {
    const std::string bad = "{\n"
                            "  \"a\": 1,\n"
                            "  \"b\": @\n"
                            "}";

    try {
        (void)Json::parse(bad);
        FAIL() << "unreachable: parse() must throw";
    } catch (const JsonParseError& e) {
        EXPECT_EQ(e.line(), 3);
        EXPECT_EQ(e.column(), 8);
    }
}

// Verifies a parse failure can be caught generically via the JsonException base class.
TEST(ErrorPropagation, CatchParseErrorAsBaseException) {
    bool caught = false;

    try {
        (void)Json::parse("{invalid}");
    } catch (const JsonException& e) {
        caught = true;
        EXPECT_EQ(std::string(e.what()).empty(), false);
    }

    EXPECT_EQ(caught, true);
}

// Verifies a type mismatch partway through a navigation chain throws JsonTypeError.
TEST(ErrorPropagation, TypeErrorThrownMidNavigationChain) {
    Json j = Json::parse(R"({"list": [1, 2, 3]})");

    bool caught = false;
    try {
        (void)j["list"]["not_a_key"]; // "list" is an array, not an object
    } catch (const JsonTypeError&) {
        caught = true;
    }

    EXPECT_EQ(caught, true);
}

// Verifies an out-of-range access partway through a navigation chain throws JsonOutOfRange.
TEST(ErrorPropagation, OutOfRangeThrownMidNavigationChain) {
    Json j = Json::parse(R"({"items": [1, 2]})");

    bool caught = false;
    try {
        (void)j.at("items").at(10);
    } catch (const JsonOutOfRange&) {
        caught = true;
    }

    EXPECT_EQ(caught, true);
}

// Verifies a caller can recover from a parse failure by falling back to a
// known-good document.
TEST(ErrorPropagation, RecoverFromParseFailureWithFallbackDocument) {
    const std::string userInput = "{ this is not valid json";
    Json result;

    try {
        result = Json::parse(userInput);
    } catch (const JsonParseError&) {
        result = Json::parse(R"({"status": "fallback"})");
    }

    EXPECT_EQ(result.isObject(), true);
    EXPECT_EQ(result["status"].asString(), "fallback");
}

// Verifies parse errors, type errors, and out-of-range errors are all
// catchable uniformly via the JsonException base class.
TEST(ErrorPropagation, CatchAllJsonExceptionsViaBaseClass) {
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

    EXPECT_EQ(caughtCount, 3);
}
