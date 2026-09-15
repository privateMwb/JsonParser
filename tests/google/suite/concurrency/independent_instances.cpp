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
// - Per-thread documents are backed by genuinely distinct, simultaneously-
//   live heap memory, never shared or address-aliased across threads

#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <gtest/gtest.h>

#include <JsonPro/Json.h>

using namespace JsonPro;

// Verifies each thread can build its own private document concurrently
// with no interference from any other thread's document.
TEST(IndependentInstances, DocumentsBuiltConcurrently) {
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
        EXPECT_EQ(ok[t], 1) << "thread " << t;
}

// Verifies each thread can freely insert into and erase from its own
// private JsonObject concurrently with other threads doing the same.
TEST(IndependentInstances, JsonObjectMutationAcrossThreads) {
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
        EXPECT_EQ(ok[t], 1) << "thread " << t;
}

// Verifies sustained, high-volume concurrent construction and destruction
// of unrelated documents across threads completes without incident.
TEST(IndependentInstances, ConcurrentConstructionAndDestructionOfUnrelatedDocuments) {
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
        EXPECT_EQ(ok[t], 1) << "thread " << t;
}

// Verifies each thread's document is backed by genuinely distinct memory --
// never accidentally shared or aliased across threads.
//
// Each document is heap-allocated and kept alive (via the unique_ptr vector
// living outside the thread lambdas) until every thread has finished and
// every address has been compared. Taking the address of a *stack-local*
// Json here would be meaningless: a thread's stack frame is freed the
// moment its lambda returns, so a later thread starting after an earlier
// one has already exited can legitimately be allocated the very same
// stack address -- that's normal stack reuse, not aliasing, and comparing
// those addresses produces false failures unrelated to JsonPro itself.
TEST(IndependentInstances, ThreadLocalDocumentsRemainIsolated) {
    constexpr int kThreads = 8;
    std::vector<std::unique_ptr<Json>> docs(kThreads);
    std::vector<const void*> addresses(kThreads, nullptr);
    std::vector<double> values(kThreads, 0.0);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&, t]() {
            docs[t] = std::make_unique<Json>(static_cast<double>(t));
            addresses[t] = static_cast<const void*>(docs[t].get());
            values[t] = docs[t]->asNumber();
        });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreads; ++t) {
        EXPECT_EQ(values[t], static_cast<double>(t)) << "thread " << t;
        for (int u = t + 1; u < kThreads; ++u)
            EXPECT_NE(addresses[t], addresses[u]) << "threads " << t << " and " << u;
    }
}
