// Working with JsonObject directly.
//
// Demonstrates:
// - Using JsonObject's own API without going through a Json wrapper
// - find() returning a pointer instead of throwing
// - Iterating entries() directly for low-level, allocation-free access
// - Removing entries with erase() and observing the returned count

#include <common/framework.h>

using namespace JsonPro;

static void run_examples() {

    // Builds a JsonObject directly, without wrapping it in a Json yet.
    setTitle("Building a JsonObject Directly");

    JsonObject object;
    object.insert_or_assign("alpha", Json(1));
    object.insert_or_assign("beta", Json(2));
    object.insert_or_assign("gamma", Json(3));

    std::cout << "size : " << object.size() << "\n";
    std::cout << "empty: " << object.empty() << "\n\n";

    // find() returns a pointer, so a missing key is nullptr rather than
    // an exception — useful in hot paths where exceptions are unwanted.
    setTitle("find() Returns a Pointer");

    if (const Json* value = object.find("beta"))
        std::cout << "beta: " << value->asNumber() << "\n";

    if (object.find("delta") == nullptr)
        std::cout << "delta: not present\n\n";

    // entries() exposes the underlying vector directly, in insertion
    // order, for allocation-free iteration.
    setTitle("Iterating entries() Directly");

    for (const auto& [key, value] : object.entries())
        std::cout << key << " = " << value.asNumber() << "\n";
    std::cout << "\n";

    // emplace() only inserts if the key is new; it leaves an existing
    // key's value untouched and reports that nothing changed.
    setTitle("emplace() Skips Existing Keys");

    bool insertedNew = object.emplace("delta", Json(4));
    bool insertedExisting = object.emplace("alpha", Json(999));

    std::cout << "inserted 'delta': " << insertedNew << "\n";
    std::cout << "inserted 'alpha': " << insertedExisting << " (already existed)\n";
    std::cout << "alpha is still  : " << object.find("alpha")->asNumber() << "\n\n";

    // erase() removes an entry and reports how many were removed (0 or 1).
    setTitle("erase() Reports the Removed Count");

    std::size_t removed = object.erase("gamma");
    std::size_t removedAgain = object.erase("gamma");

    std::cout << "first erase : " << removed << "\n";
    std::cout << "second erase: " << removedAgain << " (already gone)\n";
    std::cout << "final size  : " << object.size() << "\n";
}

REGISTER_EXAMPLE_SUITE();
