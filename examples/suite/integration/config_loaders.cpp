// Loading application configuration with JsonPro.
//
// Demonstrates:
// - Parsing a configuration file from disk
// - Falling back to defaults for optional settings
// - Failing loudly on a required setting that's missing
// - A small typed Config struct assembled from the parsed document

#include <common/framework.h>

#include <fstream>
#include <sstream>

using namespace JsonPro;

namespace {

struct Config {
    std::string host;
    int port;
    int timeoutSeconds;
    bool verbose;
};

Config loadConfig(const Json& doc) {
    Config config;

    // Required setting: fail loudly if it's missing.
    config.host = doc.at("host").asString();

    // Optional settings: fall back to sensible defaults if absent.
    config.port = doc.contains("port") ? static_cast<int>(doc["port"].asNumber()) : 8080;

    config.timeoutSeconds =
        doc.contains("timeout_seconds") ? static_cast<int>(doc["timeout_seconds"].asNumber()) : 30;

    config.verbose = doc.contains("verbose") && doc["verbose"].asBool();

    return config;
}

} // namespace

static void run_examples() {

    // Writes a partial configuration file to disk, missing several
    // optional settings on purpose.
    setTitle("Writing a Partial Config File");

    const std::string path = "app_config.json";

    std::ofstream out(path);
    out << R"({
        "host": "0.0.0.0",
        "verbose": true
    })";
    out.close();

    std::cout << "Wrote " << path << "\n\n";

    // Loads the file back and parses it.
    setTitle("Loading the Config File");

    std::ifstream in(path);
    std::ostringstream buffer;
    buffer << in.rdbuf();

    Json doc = Json::parse(buffer.str());
    std::cout << doc.dump() << "\n\n";

    // Assembles a typed Config, falling back to defaults for the settings
    // the file didn't specify.
    setTitle("Applying Defaults for Missing Settings");

    Config config = loadConfig(doc);

    std::cout << "host           : " << config.host << "\n";
    std::cout << "port           : " << config.port << " (default)\n";
    std::cout << "timeout_seconds: " << config.timeoutSeconds << " (default)\n";
    std::cout << "verbose        : " << config.verbose << " (from file)\n\n";

    // Shows a required setting failing loudly when it's missing entirely.
    setTitle("Required Setting Missing");

    Json broken = Json::parse(R"({"port": 9090})");

    try {
        Config brokenConfig = loadConfig(broken);
        (void)brokenConfig;
    } catch (const JsonOutOfRange& e) {
        std::cout << "Caught: " << e.what() << "\n";
    }
}

REGISTER_EXAMPLE_SUITE();
