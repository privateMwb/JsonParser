// Concurrent read test suite.
//
// JsonPro has no internal synchronization -- its thread-safety guarantee is
// the standard library one: concurrent *const* access to a single shared
// object, with no concurrent writer, is safe because no read-only operation
// (value access, navigation, dump(), size()/contains()) touches any mutable
// state. These tests document and validate that guarantee.
//
// Each test spins up several threads that only read from one shared, const
// Json/JsonObject; every thread writes its pass/fail verdict to its own
// vector slot (no shared writes), and assertions run on the joined results
// back on the main thread -- gtest assertion macros are not safe to call
// concurrently from multiple threads, so none run inside the thread bodies.
//
// Coverage:
// - Concurrent scalar and container value access on a shared document
// - Concurrent chained navigation through a shared nested document
// - Concurrent dump() calls all produce byte-identical output
// - Concurrent size()/contains() checks on a shared object

#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Verifies many threads reading scalar and container values from one
// shared document concurrently all observe correct, consistent results.
TEST(ConcurrentReads, ScalarAndContainerAccess) {
    const Json j = Json::parse(R"({"a": 1, "b": "two", "c": [1, 2, 3], "d": {"e": true}})");

    constexpr int kThreads = 8;
    std::vector<int> ok(kThreads, 0);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&, t]() {
            bool pass = true;
            for (int i = 0; i < 1000; ++i) {
                pass = pass && (j["a"].asNumber() == 1.0);
                pass = pass && (j["b"].asString() == "two");
                pass = pass && (j["c"].size() == 3);
                pass = pass && (j["d"]["e"].asBool() == true);
            }
            ok[t] = pass ? 1 : 0;
        });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreads; ++t)
        EXPECT_EQ(ok[t], 1) << "thread " << t;
}

// Verifies concurrent chained navigation (mixing operator[] and at())
// through a shared nested document is safe and consistent.
TEST(ConcurrentReads, NestedNavigationThroughSharedDocument) {
    const Json j = Json::parse(R"({
        "users": [
            {"name": "Alice", "roles": ["admin", "editor"]},
            {"name": "Bob",   "roles": ["viewer"]}
        ]
    })");

    constexpr int kThreads = 8;
    std::vector<int> ok(kThreads, 0);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&, t]() {
            bool pass = true;
            for (int i = 0; i < 500; ++i) {
                pass = pass && (j.at("users").at(0).at("name").asString() == "Alice");
                pass = pass && (j["users"][0]["roles"][1].asString() == "editor");
                pass = pass && (j["users"][1]["roles"].size() == 1);
            }
            ok[t] = pass ? 1 : 0;
        });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreads; ++t)
        EXPECT_EQ(ok[t], 1) << "thread " << t;
}

// Verifies concurrent dump() calls on the same shared document all produce
// byte-identical output.
TEST(ConcurrentReads, DumpProducesIdenticalOutput) {
    const Json j = Json::parse(R"({"id": 1, "tags": ["a", "b", "c"], "nested": {"x": 1.5}})");
    const std::string expected = j.dump();

    constexpr int kThreads = 8;
    std::vector<std::string> results(kThreads);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&, t]() { results[t] = j.dump(); });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreads; ++t)
        EXPECT_EQ(results[t], expected) << "thread " << t;
}

// Verifies concurrent size()/contains() checks on a shared object are safe
// and consistently correct.
TEST(ConcurrentReads, ContainsAndSizeChecks) {
    const Json j = Json::parse(R"({"x": 1, "y": 2, "z": 3})");

    constexpr int kThreads = 8;
    std::vector<int> ok(kThreads, 0);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&, t]() {
            bool pass = true;
            for (int i = 0; i < 500; ++i) {
                pass = pass && (j.size() == 3);
                pass = pass && j.contains("x");
                pass = pass && j.contains("y");
                pass = pass && !j.contains("absent");
            }
            ok[t] = pass ? 1 : 0;
        });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreads; ++t)
        EXPECT_EQ(ok[t], 1) << "thread " << t;
}
