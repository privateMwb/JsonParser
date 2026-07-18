// Exporting JsonPro documents.
//
// Demonstrates:
// - Exporting a document to a std::string via dump()
// - Exporting a document to a file, then reading it back
// - Round-tripping a document through dump() -> parse() -> dump()
// - Object key order is preserved end-to-end, since JsonObject stores
//   entries in insertion order (a side hash index gives O(1) lookup
//   without disturbing that order)

#include <common/framework.h>

#include <fstream>
#include <sstream>

using namespace JsonPro;

static void run_examples() {

    // Exports a document to an in-memory string.
    setTitle("Exporting to a String");

    Json::ObjectType profile;
    profile.insert_or_assign("name", Json("Rain"));
    profile.insert_or_assign("role", Json("Engineer"));
    profile.insert_or_assign("tags",
                             Json(Json::ArrayType{Json("cpp"), Json("json"), Json("systems")}));

    Json doc(std::move(profile));
    std::string exported = doc.dump();

    std::cout << exported << "\n\n";

    // Exports the same document to a file on disk.
    setTitle("Exporting to a File");

    const std::string path = "exported_document.json";

    std::ofstream out(path);
    out << exported;
    out.close();

    std::cout << "Wrote document to " << path << "\n\n";

    // Reads the file back and re-parses it.
    setTitle("Reading the File Back");

    std::ifstream in(path);
    std::ostringstream buffer;
    buffer << in.rdbuf();

    Json reloaded = Json::parse(buffer.str());

    std::cout << reloaded.dump() << "\n\n";

    // Checks that array order survives a full export/import round trip.
    setTitle("Array Order After Round Trip");

    std::cout << "original tags order: ";
    for (const Json& tag : doc["tags"].asArray())
        std::cout << tag.asString() << " ";
    std::cout << "\n";

    std::cout << "reloaded tags order: ";
    for (const Json& tag : reloaded["tags"].asArray())
        std::cout << tag.asString() << " ";
    std::cout << "\n\n";

    // Checks that object key order also survives the round trip. JsonObject
    // stores entries in an insertion-ordered vector (with a side hash index
    // for O(1) lookup), so dump() iterates and reproduces the exact order
    // keys were originally inserted in.
    setTitle("Object Key Order Is Preserved");

    Json::ObjectType ordered;
    ordered.insert_or_assign("z_first", Json(1));
    ordered.insert_or_assign("a_second", Json(2));
    ordered.insert_or_assign("m_third", Json(3));
    ordered.insert_or_assign("b_fourth", Json(4));

    Json orderedJson(std::move(ordered));
    Json orderedReloaded = Json::parse(orderedJson.dump());

    std::cout << "insertion order : z_first, a_second, m_third, b_fourth\n";
    std::cout << "dump() order    : " << orderedJson.dump() << "\n";
    std::cout << "reloaded order  : " << orderedReloaded.dump() << "\n";
}

REGISTER_EXAMPLE_SUITE();
