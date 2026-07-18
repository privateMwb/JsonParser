// Wrong-type access in JsonPro.
//
// Demonstrates:
// - Catching JsonTypeError when calling asX() on the wrong type
// - Every asX() accessor rejecting a mismatched type the same way
// - Checking type()/isX() first as the idiomatic way to avoid the error

#include <common/framework.h>

using namespace JsonPro;

static void run_examples() {

    Json number(42);
    Json text("not a number");
    Json flag(true);
    Json list(Json::ArrayType{Json(1)});
    Json map(Json::ObjectType{});

    // asBool() on a non-bool value.
    setTitle("asBool() on a Number");

    try {
        (void)number.asBool();
    } catch (const JsonTypeError& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // asNumber() on a non-number value.
    setTitle("asNumber() on a String");

    try {
        (void)text.asNumber();
    } catch (const JsonTypeError& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // asString() on a non-string value.
    setTitle("asString() on a Bool");

    try {
        (void)flag.asString();
    } catch (const JsonTypeError& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // asArray() on a non-array value.
    setTitle("asArray() on an Object");

    try {
        (void)map.asArray();
    } catch (const JsonTypeError& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // asObject() on a non-object value.
    setTitle("asObject() on an Array");

    try {
        (void)list.asObject();
    } catch (const JsonTypeError& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // The idiomatic way to avoid all of the above: check the type first.
    setTitle("Avoiding the Error With isX()");

    if (number.isNumber())
        std::cout << "number is a number: " << number.asNumber() << "\n";
    else
        std::cout << "number is not a number, skipping asNumber()\n";
}

REGISTER_EXAMPLE_SUITE();
