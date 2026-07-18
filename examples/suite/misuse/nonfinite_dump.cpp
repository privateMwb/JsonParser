// Non-finite numbers and JsonPro serialization.
//
// Demonstrates:
// - dump() throwing when a value holds NaN or Infinity
// - The failure surfacing even when the non-finite number is nested deep
// - Why JSON has no representation for non-finite numbers, and how to
//   sanitize a value before serializing it

#include <common/framework.h>

#include <cmath>
#include <limits>

using namespace JsonPro;

static void run_examples() {

    // JSON numbers must be finite per RFC 8259 — dump() throws rather
    // than emit "NaN" or "Infinity", which isn't valid JSON.
    setTitle("dump() Rejects NaN");

    Json nan(std::numeric_limits<double>::quiet_NaN());

    try {
        (void)nan.dump();
    } catch (const JsonException& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // The same applies to positive and negative infinity.
    setTitle("dump() Rejects Infinity");

    Json infinity(std::numeric_limits<double>::infinity());

    try {
        (void)infinity.dump();
    } catch (const JsonException& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // The failure surfaces even if the non-finite number is buried deep
    // inside an otherwise valid document — dump() walks the whole tree.
    setTitle("Failure Surfaces From Nested Values");

    Json::ObjectType document;
    document.insert_or_assign("name", Json("Rain"));
    document.insert_or_assign("ratio", Json(std::numeric_limits<double>::quiet_NaN()));

    Json doc(std::move(document));

    try {
        (void)doc.dump();
    } catch (const JsonException& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // Sanitize non-finite values before serializing, e.g. by substituting
    // a sentinel, rather than letting dump() throw at write time.
    setTitle("Sanitizing Before Serialization");

    double computed = std::numeric_limits<double>::infinity();
    Json safe(std::isfinite(computed) ? computed : 0.0);

    std::cout << safe.dump() << "\n";
}

REGISTER_EXAMPLE_SUITE();
