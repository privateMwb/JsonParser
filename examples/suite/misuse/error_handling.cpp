// Handling JsonPro errors.
//
// Demonstrates:
// - Catching JsonParseError for malformed input
// - Catching JsonTypeError for wrong-type access
// - Catching JsonOutOfRange for missing keys / out-of-range indices
// - The parser's maximum nesting depth guard
// - Recovering with a default value instead of letting the exception propagate

#include <common/framework.h>

using namespace JsonPro;

static void run_examples() {

    // Malformed JSON throws JsonParseError.
    setTitle("Malformed JSON");

    try {
        (void)Json::parse(R"({"a": )");
    } catch (const JsonParseError& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // Accessing a value as the wrong type throws JsonTypeError.
    setTitle("Wrong-Type Access");

    Json number(42);

    try {
        (void)number.asString();
    } catch (const JsonTypeError& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // A missing key or out-of-range index throws JsonOutOfRange.
    // Note: this is JsonOutOfRange, not std::out_of_range — JsonPro's
    // exceptions all derive from JsonException, not the standard hierarchy.
    setTitle("Out-of-Range Access");

    Json doc = Json::parse(R"({"name": "Rain"})");

    try {
        (void)doc.at("missing_key");
    } catch (const JsonOutOfRange& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // The parser rejects documents nested beyond its maximum depth.
    setTitle("Maximum Nesting Depth");

    std::string deeplyNested(600, '[');
    deeplyNested.append(600, ']');

    try {
        (void)Json::parse(deeplyNested);
    } catch (const JsonParseError& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // Recovering with a default value instead of propagating the exception.
    setTitle("Recovering with a Default Value");

    std::string fallback;
    try {
        fallback = doc.at("nickname").asString();
    } catch (const JsonOutOfRange&) {
        fallback = "Rain";
    }

    std::cout << "nickname: " << fallback << "\n";
}

REGISTER_EXAMPLE_SUITE();
