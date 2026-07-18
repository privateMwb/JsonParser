// Parse, modify, and re-serialize a JsonPro document.
//
// Demonstrates:
// - Parsing an existing document
// - Modifying an existing field in place via operator[]
// - Adding a new field that didn't exist before
// - Removing a field via JsonObject::erase()
// - Re-serializing the modified document

#include <common/framework.h>

using namespace JsonPro;

static void run_examples() {

    Json doc = Json::parse(R"({
        "name": "Rain",
        "version": "1.0.0",
        "deprecated_flag": true,
        "active": false
    })");

    std::cout << "Original:\n" << doc.dump() << "\n\n";

    // Modifies existing fields in place — no need to rebuild the document.
    setTitle("Modifying Existing Fields");

    doc["version"] = Json("1.1.0");
    doc["active"] = Json(true);

    std::cout << doc.dump() << "\n\n";

    // Adds a field that wasn't present in the original document.
    // operator[] auto-vivifies missing keys on a non-const Json.
    setTitle("Adding a New Field");

    doc["updated_by"] = Json("build-pipeline");

    std::cout << doc.dump() << "\n\n";

    // Removes a field entirely, working through the underlying JsonObject.
    setTitle("Removing a Field");

    std::size_t removed = doc.asObject().erase("deprecated_flag");
    std::cout << "removed: " << removed << " entr" << (removed == 1 ? "y" : "ies") << "\n\n";

    // The document now reflects every change made above.
    setTitle("Final Document");

    std::cout << doc.dump() << "\n";
}

REGISTER_EXAMPLE_SUITE();
