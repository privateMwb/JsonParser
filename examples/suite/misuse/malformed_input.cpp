// Malformed JSON input in JsonPro.
//
// Demonstrates:
// - A variety of RFC 8259 violations, each rejected with JsonParseError
// - Trailing commas, unquoted keys, single quotes, unterminated strings
// - Invalid escape sequences and malformed numbers
// - Unexpected trailing content after a complete value

#include <common/framework.h>

using namespace JsonPro;

namespace {

void tryParse(const char* label, std::string_view text) {
    setTitle(label);
    try {
        (void)Json::parse(text);
        std::cout << "unexpectedly parsed successfully\n\n";
    } catch (const JsonParseError& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }
}

} // namespace

static void run_examples() {

    tryParse("Trailing Comma", R"({"a": 1, })");
    tryParse("Unquoted Key", R"({a: 1})");
    tryParse("Single-Quoted String", R"({'a': 1})");
    tryParse("Unterminated String", R"({"a": "unterminated)");
    tryParse("Invalid Escape Sequence", R"({"a": "bad \q escape"})");
    tryParse("Leading Zero in Number", R"({"a": 007})");
    tryParse("Trailing Content After Value", R"({"a": 1} garbage)");
}

REGISTER_EXAMPLE_SUITE();
