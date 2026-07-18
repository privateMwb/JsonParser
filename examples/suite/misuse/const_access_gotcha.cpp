// The const/non-const operator[] difference in JsonPro.
//
// Demonstrates:
// - Non-const operator[] auto-vivifying a missing key
// - const operator[] throwing JsonOutOfRange instead, since it can't
//   insert into a value it doesn't own a mutable reference to
// - The same asymmetry for array indices: [] is unchecked, at() is checked
// - Why this asymmetry can silently create unwanted keys if not careful

#include <common/framework.h>

using namespace JsonPro;

static void run_examples() {

    // On a non-const Json, operator[] inserts a null value for a missing
    // key rather than failing — the same behavior as std::map::operator[].
    setTitle("Non-const operator[] Auto-Vivifies");

    Json doc = Json::parse(R"({"name": "Rain"})");

    std::cout << "size before: " << doc.size() << "\n";
    std::cout << "value      : " << doc["typo_field"].dump() << "\n";
    std::cout << "size after : " << doc.size() << " (typo_field was silently created)\n\n";

    // On a const Json, the same syntax can't auto-vivify — there's
    // nowhere to insert into — so it throws instead.
    setTitle("const operator[] Throws Instead");

    const Json constDoc = Json::parse(R"({"name": "Rain"})");

    try {
        (void)constDoc["typo_field"];
    } catch (const JsonOutOfRange& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // Array indices have a similar asymmetry: operator[](index) is
    // unchecked, while at(index) is checked and throws JsonOutOfRange.
    setTitle("Array operator[] Is Unchecked, at() Is Checked");

    Json array(Json::ArrayType{Json(1), Json(2)});

    try {
        (void)array.at(10);
    } catch (const JsonOutOfRange& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    // Practical takeaway: prefer at() (or contains() first) whenever the
    // key/index might be missing, and reserve operator[] for cases where
    // auto-vivification on a non-const value is exactly what's wanted.
    setTitle("Preferring at() to Avoid the Gotcha");

    try {
        std::cout << doc.at("another_typo").asString() << "\n";
    } catch (const JsonOutOfRange&) {
        std::cout << "at() failed loudly instead of silently creating a key\n";
    }

    std::cout << "final size: " << doc.size() << " (at() did not add anything)\n";
}

REGISTER_EXAMPLE_SUITE();
