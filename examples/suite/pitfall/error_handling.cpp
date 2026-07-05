// JsonPro error handling.
//
// Demonstrates:
// - Catching parse errors from malformed JSON
// - Catching wrong-type access errors
// - Catching out-of-range access errors
// - The maximum nesting depth guard rejecting adversarial input
// - Falling back to a default value on failure

#include <common/framework.h>

using namespace JsonPro;

static void run_examples() {

    // Catches a parse error from malformed JSON.
    setTitle("Malformed JSON");

    try {
        (void)Json::parse(R"({"a": 1, })"); // trailing comma
    } catch (const std::runtime_error& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // Catches a wrong-type access error.
    setTitle("Wrong-Type Access");

    Json number(42);

    try {
        std::string s = number.asString();
        (void)s;
    } catch (const std::runtime_error& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // Catches an out-of-range access error.
    setTitle("Out-of-Range Access");

    Json array(Json::ArrayType{ Json(1), Json(2) });

    try {
        Json& element = array.at(10);
        (void)element;
    } catch (const std::out_of_range& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // Demonstrates the maximum nesting depth guard, which rejects
    // adversarially deep input before it can exhaust the call stack.
    setTitle("Maximum Nesting Depth");

    std::string deeplyNested(600, '[');
    deeplyNested.append(600, ']');

    try {
        (void)Json::parse(deeplyNested);
    } catch (const std::runtime_error& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // Falls back to a default value when access might fail.
    setTitle("Recovering with a Default Value");

    Json settings = Json::parse(R"({"volume": 80})");

    int timeout;
    try {
        timeout = static_cast<int>(settings.at("timeout").asNumber());
    } catch (const std::out_of_range&) {
        timeout = 30; // default
    }

    std::cout << "timeout: " << timeout << "\n";
}

REGISTER_EXAMPLE_SUITE();
