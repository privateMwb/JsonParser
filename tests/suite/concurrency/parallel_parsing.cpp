// Parallel parsing test suite.
//
// Each call to Json::parse() constructs its own Parser over its own input
// buffer, with no shared or global parser state. These tests validate that
// many threads calling Json::parse() concurrently -- on different input,
// on identical input, and on a mix of valid and invalid input -- never
// interfere with one another.
//
// Coverage:
// - Threads parsing different input concurrently each get their own
//   correct, independent result
// - Threads parsing identical input concurrently all get equal results
// - A thread's parse failure (and the exception it throws) has no effect
//   on other threads parsing valid input at the same time
// - Sustained per-thread parsing volume remains correct throughout

#include <string>
#include <thread>
#include <vector>

#include <common/framework.h>

using namespace JsonPro;

// Verifies threads parsing distinct input concurrently each produce their
// own correct, independent result.
static void parallel_parsing_produces_correct_independent_results() {
    constexpr int kThreads = 8;
    std::vector<double> results(kThreads);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&, t]() {
            std::string text = "[" + std::to_string(t) + ", " + std::to_string(t * 10) + "]";
            Json j = Json::parse(text);
            results[t] = j[0].asNumber() + j[1].asNumber();
        });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreads; ++t)
        CHK(results[t] == static_cast<double>(t + t * 10));
}

// Verifies many threads parsing the exact same input concurrently all
// produce equal results.
static void parallel_parsing_of_identical_input_is_deterministic() {
    const std::string text = R"({"a": [1, 2, 3], "b": {"c": "hello"}, "d": 3.14})";
    Json expected = Json::parse(text);

    constexpr int kThreads = 8;
    std::vector<int> ok(kThreads, 0);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&, t]() {
            Json local = Json::parse(text);
            ok[t] = (local == expected) ? 1 : 0;
        });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreads; ++t)
        CHK(ok[t] == 1);
}

// Verifies a parse failure (and exception) on one thread has no effect on
// other threads parsing valid input concurrently.
static void parallel_parsing_errors_do_not_affect_other_threads() {
    constexpr int kThreads = 8;
    std::vector<int> ok(kThreads, 0);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&, t]() {
            if (t % 2 == 0) {
                bool threw = false;
                try {
                    (void)Json::parse("{ not valid json");
                } catch (const std::runtime_error&) {
                    threw = true;
                }
                ok[t] = threw ? 1 : 0;
            } else {
                Json j = Json::parse(R"({"value": 42})");
                ok[t] = (j["value"].asNumber() == 42.0) ? 1 : 0;
            }
        });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreads; ++t)
        CHK(ok[t] == 1);
}

// Verifies sustained per-thread parsing volume (many sequential parses per
// thread, running concurrently with other threads) remains correct throughout.
static void parallel_parsing_high_volume_per_thread() {
    constexpr int kThreads = 4;
    constexpr int kIterations = 500;

    std::vector<int> ok(kThreads, 0);
    std::vector<std::thread> threads;

    for (int t = 0; t < kThreads; ++t) {
        threads.emplace_back([&, t]() {
            bool pass = true;
            for (int i = 0; i < kIterations; ++i) {
                std::string text = R"({"n": )" + std::to_string(i) + "}";
                Json j = Json::parse(text);
                pass = pass && (j["n"].asNumber() == static_cast<double>(i));
            }
            ok[t] = pass ? 1 : 0;
        });
    }

    for (auto& th : threads)
        th.join();

    for (int t = 0; t < kThreads; ++t)
        CHK(ok[t] == 1);
}

// Executes all parallel parsing test cases.
static void run_tests() {
    RUN(parallel_parsing_produces_correct_independent_results);
    RUN(parallel_parsing_of_identical_input_is_deterministic);
    RUN(parallel_parsing_errors_do_not_affect_other_threads);
    RUN(parallel_parsing_high_volume_per_thread);
}

REGISTER_TEST_SUITE();
