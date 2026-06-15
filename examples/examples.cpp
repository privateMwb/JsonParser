// Json Examples
// Demonstrates basic usage of the Json class:
//
// - basic types (null, bool, number, string)
// - array construction and access
// - object construction and access
// - parsing JSON strings
// - serialization (dump to string and ostream)
// - navigation (nested operator[] access)
// - bounds-checked access (at())
// - key existence (contains())
// - comparison (operator== and operator!=)
// - copy and move semantics
// - mixed nested structure
//
// These examples illustrate the core features and intended usage of Json.

#include <iostream>
#include <sstream>

#include "Json.h"

// Basic Types
// shows direct construction of each Json type
void basicTypes() {
    Json null;
    Json b(true);
    Json n(3.14);
    Json i(42);
    Json s("hello");

    (void)null.isNull();   // true
    (void)b.isBool();      // true
    (void)b.asBool();      // true
    (void)n.isNumber();    // true
    (void)n.asNumber();    // 3.14
    (void)i.asNumber();    // 42.0
    (void)s.isString();    // true
    (void)s.asString();    // "hello"
}

// Array Usage
// shows array construction and index access
void arrayUsage() {
    Json::ArrayType arr{
        Json("Coke"),
        Json("Sprite"),
        Json("Pepsi")
    };

    Json j(arr);

    (void)j.isArray();           // true
    (void)j.size();              // 3
    (void)j[0].asString();       // "Coke"
    (void)j[2].asString();       // "Pepsi"
}

// Object Usage
// shows object construction and key access
void objectUsage() {
    Json::ObjectType obj{
        { "name",  Json("Claude") },
        { "age",   Json(2)        },
        { "smart", Json(true)     }
    };

    Json j(obj);

    (void)j.isObject();              // true
    (void)j.size();                  // 3
    (void)j["name"].asString();      // "Claude"
    (void)j["age"].asNumber();       // 2
    (void)j["smart"].asBool();       // true
}

// Parsing
// shows Json::parse() for each type
void parsing() {
    Json null   = Json::parse("null");
    Json b      = Json::parse("true");
    Json n      = Json::parse("3.14");
    Json s      = Json::parse("\"hello\"");
    Json arr    = Json::parse("[1, 2, 3]");
    Json obj    = Json::parse("{\"key\": \"value\"}");

    (void)null.isNull();          // true
    (void)b.asBool();             // true
    (void)n.asNumber();           // 3.14
    (void)s.asString();           // "hello"
    (void)arr.isArray();          // true
    (void)arr.size();             // 3
    (void)obj.isObject();         // true
    (void)obj["key"].asString();  // "value"
}

// Serialization
// shows dump() to string and to ostream
void serialization() {
    Json::ObjectType obj{
        { "name", Json("Claude") },
        { "age",  Json(2)        }
    };

    Json j(obj);

    // dump to string
    std::string s = j.dump(0);
    (void)s;

    // dump to ostream
    std::ostringstream oss;
    j.dump(oss, 0);
    (void)oss.str();
}

// Navigation
// shows nested operator[] traversal
void navigation() {
    Json::ObjectType obj{
        {
            "user",
            Json::ObjectType{
                { "name",   Json("Claude") },
                { "scores", Json::ArrayType{ Json(90), Json(95), Json(100) } }
            }
        }
    };

    Json j(obj);

    (void)j["user"]["name"].asString();       // "Claude"
    (void)j["user"]["scores"][0].asNumber();  // 90
    (void)j["user"]["scores"][2].asNumber();  // 100
}

// Bounds-Checked Access
// shows at() throwing on invalid index and key
void boundsChecked() {
    Json::ArrayType arr{ Json(1), Json(2), Json(3) };
    Json ja(arr);

    (void)ja.at(0).asNumber(); // 1 — valid
    (void)ja.at(2).asNumber(); // 3 — valid

    try {
        (void)ja.at(99);
    } catch (const std::out_of_range&) {
        // index out of range
    }

    Json::ObjectType obj{ { "key", Json("value") } };
    Json jo(obj);

    (void)jo.at("key").asString(); // "value" — valid

    try {
        (void)jo.at("missing");
    } catch (const std::out_of_range&) {
        // key not found
    }
}

// Contains
// shows key existence check on an object
void contains() {
    Json::ObjectType obj{
        { "name", Json("Claude") },
        { "age",  Json(2)        }
    };

    Json j(obj);

    (void)j.contains("name");    // true
    (void)j.contains("missing"); // false
}

// Comparison
// shows operator== and operator!=
void comparison() {
    Json a(42);
    Json b(42);
    Json c(99);

    (void)(a == b); // true
    (void)(a != c); // true
    (void)(a == c); // false

    Json s1("hello");
    Json s2("hello");
    Json s3("world");

    (void)(s1 == s2); // true
    (void)(s1 != s3); // true
}

// Copy and Move
// shows copy construction, copy assignment, move construction, move assignment
void copyMove() {
    Json a("original");

    // copy construction — b is independent
    Json b(a);
    (void)b.asString(); // "original"

    // copy assignment
    Json c;
    c = a;
    (void)c.asString(); // "original"

    // move construction — a is now null
    Json d(std::move(a));
    (void)d.asString(); // "original"
    (void)a.isNull();   // true

    // move assignment
    Json e;
    e = std::move(b);
    (void)e.asString(); // "original"
    (void)b.isNull();   // true
}

// Mixed Types
// shows a real-world nested structure with mixed value types
void mixTypes() {
    Json::ObjectType obj{
        {
            "pokemon",
            Json::ArrayType{
                Json::ObjectType{
                    { "name",   Json("Rayquaza") },
                    { "health", Json(5000)       },
                    { "shiny",  Json(true)       },
                    { "stats",  Json::ArrayType{ Json(100), Json(90), Json(100), Json(95) } }
                },
                Json::ObjectType{
                    { "name",   Json("Groudon") },
                    { "health", Json(6000)      },
                    { "shiny",  Json(false)     },
                    { "stats",  Json::ArrayType{ Json(90), Json(100), Json(100), Json(90) } }
                }
            }
        }
    };

    Json j(obj);

    (void)j["pokemon"].size();                      // 2
    (void)j["pokemon"][0]["name"].asString();        // "Rayquaza"
    (void)j["pokemon"][1]["name"].asString();        // "Groudon"
    (void)j["pokemon"][0]["shiny"].asBool();         // true
    (void)j["pokemon"][0]["stats"][0].asNumber();    // 100
}

// Entry Point
int main() {
    basicTypes();
    arrayUsage();
    objectUsage();
    parsing();
    serialization();
    navigation();
    boundsChecked();
    contains();
    comparison();
    copyMove();
    mixTypes();

    return 0;
}

