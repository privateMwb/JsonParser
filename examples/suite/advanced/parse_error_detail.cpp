// Inspecting JsonPro parse error details.
//
// Demonstrates:
// - Catching JsonParseError specifically instead of a generic exception
// - Reading the 1-based line and column of the failure
// - Using that position to build a precise diagnostic message
// - Distinguishing JsonParseError from JsonTypeError

#include <common/framework.h>

using namespace JsonPro;

static void run_examples() {

    // Catches JsonParseError specifically to access its line/column.
    setTitle("Reading Line and Column");

    std::string malformed = R"({
        "name": "Rain",
        "score": ,
        "active": true
    })";

    try {
        (void)Json::parse(malformed);
    } catch (const JsonParseError& e) {
        std::cout << "message: " << e.what() << "\n";
        std::cout << "line   : " << e.line() << "\n";
        std::cout << "column : " << e.column() << "\n\n";
    }

    // Builds a caret-style diagnostic pointing at the failing column.
    setTitle("Building a Caret Diagnostic");

    try {
        (void)Json::parse(R"({"a": 1 "b": 2})"); // missing comma
    } catch (const JsonParseError& e) {
        std::cout << "error at line " << e.line() << ", column " << e.column() << "\n";
        std::cout << std::string(e.column() - 1, ' ') << "^\n";
        std::cout << e.what() << "\n\n";
    }

    // Shows that JsonParseError is distinct from the other exception
    // types — catching JsonException would work for all of them, but
    // catching JsonParseError specifically only matches malformed input.
    setTitle("JsonParseError Is Distinct From Type Errors");

    Json number(42);

    try {
        (void)number.asString();
    } catch (const JsonParseError&) {
        std::cout << "unreachable: this is a JsonTypeError, not a parse error\n";
    } catch (const JsonTypeError& e) {
        std::cout << "correctly caught as JsonTypeError: " << e.what() << "\n";
    }
}

REGISTER_EXAMPLE_SUITE();
