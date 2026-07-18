// Independent instances test suite.
//
// When each thread owns and mutates its own, entirely private Json /
// JsonObject instance -- no object shared across threads -- no
// synchronization is required at all, since there is nothing to race on.
// These tests document and validate that ownership boundary: building,
// mutating, and destroying unrelated documents concurrently is always safe
// regardless of what JsonPro itself does or doesn't synchronize internally.
//
// Coverage:
// - Threads independently building their own document concurrently
// - Threads independently mutating (insert/erase) their own JsonObject
// - Sustained concurrent construction/destruction of unrelated documents
// - Per-thread documents are backed by genuinely distinct memory, never shared

#include <string>
#include <thread>
#include <vector>

#include <common/framework.h>

using namespace JsonPro;

// Verifies each thread can build its own private document concurrently
// with no interference from any other thread's document.
static void independent_documents_built_concurrently() {
    constexpr int kThreads = 8;
    std::vector<int> ok(kThreads, 0);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&, t]() {
            Json j = Json(Json::ObjectType{});
            j["owner"] = Json(t);
            j["items"] = Json(Json::ArrayType{});

            for (int i = 0; i < 50; ++i)
                j["items"].asArray().push_back(Json(t * 100 + i));

            bool pass = (j["owner"].asNumber() == static_cast<double>(t)) &&
                        (j["items"].size() == 50) &&
                        (j["items"][49].asNumber() == static_cast<double>(t * 100 + 49));
            ok[t] = pass ? 1 : 0;
        });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreads; ++t)
        CHK(ok[t] == 1);
}

// Verifies each thread can freely insert into and erase from its own
// private JsonObject concurrently with other threads doing the same.
static void independent_jsonobject_mutation_across_threads() {
    constexpr int kThreads = 8;
    std::vector<int> ok(kThreads, 0);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&, t]() {
            JsonObject obj;
            for (int i = 0; i < 20; ++i)
                obj.emplace("k" + std::to_string(i), Json(i));

            for (int i = 0; i < 20; i += 2)
                obj.erase("k" + std::to_string(i));

            bool pass =
                (obj.size() == 10) && (obj.contains("k1") == true) && (obj.contains("k0") == false);
            ok[t] = pass ? 1 : 0;
        });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreads; ++t)
        CHK(ok[t] == 1);
}

// Verifies sustained, high-volume concurrent construction and destruction
// of unrelated documents across threads completes without incident.
static void concurrent_construction_and_destruction_of_unrelated_documents() {
    constexpr int kThreads = 8;
    constexpr int kIterations = 200;

    std::vector<int> ok(kThreads, 0);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&, t]() {
            for (int i = 0; i < kIterations; ++i) {
                Json j = Json::parse(R"({"a": [1, 2, 3], "b": {"c": true}})");
                (void)j; // built and destroyed each iteration
            }
            ok[t] = 1;
        });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreads; ++t)
        CHK(ok[t] == 1);
}

// Verifies each thread's document is backed by genuinely distinct memory --
// never accidentally shared or aliased across threads.
static void thread_local_documents_remain_isolated() {
    constexpr int kThreads = 8;
    std::vector<const void*> addresses(kThreads, nullptr);
    std::vector<double> values(kThreads, 0.0);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&, t]() {
            Json j(static_cast<double>(t));
            addresses[t] = static_cast<const void*>(&j);
            values[t] = j.asNumber();
        });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreads; ++t) {
        CHK(values[t] == static_cast<double>(t));
        for (int u = t + 1; u < kThreads; ++u)
            CHK(addresses[t] != addresses[u]);
    }
}

// Executes all independent instances test cases.
static void run_tests() {
    RUN(independent_documents_built_concurrently);
    RUN(independent_jsonobject_mutation_across_threads);
    RUN(concurrent_construction_and_destruction_of_unrelated_documents);
    RUN(thread_local_documents_remain_isolated);
}

REGISTER_TEST_SUITE();
