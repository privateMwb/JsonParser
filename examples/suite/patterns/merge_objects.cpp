// Merging JsonPro objects.
//
// Demonstrates:
// - Merging two objects with override semantics via insert_or_assign
// - Merging two objects with defaults-win semantics via emplace
// - How overwriting an existing key preserves its original insertion position

#include <common/framework.h>

using namespace JsonPro;

static void run_examples() {

    // Builds two objects to merge: a base configuration and a set of overrides.
    setTitle("Base and Override Objects");

    Json::ObjectType base;
    base.insert_or_assign("host", Json("localhost"));
    base.insert_or_assign("port", Json(8080));
    base.insert_or_assign("timeout", Json(30));

    Json::ObjectType overrides;
    overrides.insert_or_assign("port", Json(9090));
    overrides.insert_or_assign("retries", Json(3));

    std::cout << "base     : " << Json(base).dump() << "\n";
    std::cout << "overrides: " << Json(overrides).dump() << "\n\n";

    // Merges overrides into base, letting overrides win on conflicting keys.
    setTitle("Merging with Override Wins (insert_or_assign)");

    Json::ObjectType merged = base;
    for (const auto& [key, value] : overrides.entries())
        merged.insert_or_assign(key, value);

    std::cout << Json(merged).dump() << "\n\n";

    // Confirms that overwriting "port" replaced its value but kept its
    // original position — it did not move to the end of the object.
    setTitle("Overwritten Key Keeps Its Original Position");

    for (const auto& [key, value] : merged.entries())
        std::cout << key << " ";
    std::cout << "\n\n";

    // Merges the other direction: base's existing keys are left untouched,
    // only genuinely new keys from overrides are added.
    setTitle("Merging with Defaults Wins (emplace)");

    Json::ObjectType defaultsWin = base;
    for (const auto& [key, value] : overrides.entries())
        defaultsWin.emplace(key, value);

    std::cout << Json(defaultsWin).dump() << "\n";
}

REGISTER_EXAMPLE_SUITE();
