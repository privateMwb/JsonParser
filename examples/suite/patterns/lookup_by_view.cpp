// Looking up JsonPro object members via string_view.
//
// Demonstrates:
// - Looking up members with contains()/find() using string_view directly
// - Avoiding a temporary std::string allocation on lookup
// - Looking up a key straight from a slice of a larger buffer

#include <common/framework.h>

using namespace JsonPro;

static void run_examples() {

    Json doc = Json::parse(R"({
        "name": "Rain",
        "role": "Engineer",
        "team": "Systems"
    })");

    // contains() and find() accept a std::string_view, so a lookup never
    // needs to construct a temporary std::string just to search the index.
    setTitle("Lookup Without Allocating a std::string");

    std::string_view key = "role";

    std::cout << "contains(\"role\"): " << doc.contains(key) << "\n";

    const Json::ObjectType& object = doc.asObject();
    if (const Json* found = object.find(key))
        std::cout << "found: " << found->asString() << "\n\n";

    // Works just as well from a view into a larger buffer — no substring
    // needs to be copied out first.
    setTitle("Lookup From a Slice of a Larger Buffer");

    std::string request = "GET /team HTTP/1.1";
    std::string_view field(request.data() + 5, 4); // "team"

    if (const Json* found = object.find(field))
        std::cout << field << " -> " << found->asString() << "\n\n";

    // Missing keys are handled the same way, with no allocation either.
    setTitle("Missing Key Lookup");

    std::string_view missing = "phone";

    std::cout << "contains(\"phone\"): " << doc.contains(missing) << "\n";
    std::cout << "find(\"phone\")    : " << (object.find(missing) == nullptr ? "nullptr" : "found")
              << "\n";
}

REGISTER_EXAMPLE_SUITE();
