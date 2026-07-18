// Converting between STL containers and JsonPro values.
//
// Demonstrates:
// - Converting a std::vector<T> to a Json array and back
// - Converting a std::map<std::string, T> to a Json object and back
// - Nesting STL conversions inside one another

#include <common/framework.h>

#include <map>
#include <vector>

using namespace JsonPro;

namespace {

Json vectorToJson(const std::vector<int>& values) {
    Json::ArrayType array;
    array.reserve(values.size());
    for (int value : values)
        array.push_back(Json(value));
    return Json(std::move(array));
}

std::vector<int> jsonToVector(const Json& json) {
    std::vector<int> values;
    values.reserve(json.size());
    for (const Json& element : json.asArray())
        values.push_back(static_cast<int>(element.asNumber()));
    return values;
}

Json mapToJson(const std::map<std::string, int>& scores) {
    Json::ObjectType object;
    for (const auto& [key, value] : scores)
        object.insert_or_assign(key, Json(value));
    return Json(std::move(object));
}

std::map<std::string, int> jsonToMap(const Json& json) {
    std::map<std::string, int> scores;
    for (const auto& [key, value] : json.asObject().entries())
        scores.emplace(key, static_cast<int>(value.asNumber()));
    return scores;
}

} // namespace

static void run_examples() {

    // Converts a std::vector<int> to a Json array and back.
    setTitle("Vector to Array and Back");

    std::vector<int> numbers{4, 8, 15, 16, 23, 42};
    Json numbersJson = vectorToJson(numbers);

    std::cout << numbersJson.dump() << "\n";

    std::vector<int> restored = jsonToVector(numbersJson);
    std::cout << "round-trip matches: " << (restored == numbers) << "\n\n";

    // Converts a std::map<std::string, int> to a Json object and back.
    setTitle("Map to Object and Back");

    std::map<std::string, int> scores{{"alice", 90}, {"bob", 85}, {"carol", 95}};
    Json scoresJson = mapToJson(scores);

    std::cout << scoresJson.dump() << "\n";

    std::map<std::string, int> restoredScores = jsonToMap(scoresJson);
    std::cout << "round-trip matches: " << (restoredScores == scores) << "\n\n";

    // Nests one conversion inside another: a map of named score lists.
    setTitle("Nested STL Conversion");

    std::map<std::string, std::vector<int>> byPlayer{{"alice", {90, 92, 88}},
                                                     {"bob", {85, 80, 83}}};

    Json::ObjectType nested;
    for (const auto& [player, playerScores] : byPlayer)
        nested.insert_or_assign(player, vectorToJson(playerScores));

    std::cout << Json(std::move(nested)).dump() << "\n";
}

REGISTER_EXAMPLE_SUITE();
