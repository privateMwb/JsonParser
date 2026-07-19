#pragma once

// clang-format off
#include <nlohmann/json.hpp>   // nlohmann::json (parses benchmark snapshots in loadResults)

#include <JsonPro/Json.h>      // JsonPro::Json (BenchmarkResult/RegressionRow storage)

#include <chrono>              // pulled in via `using namespace std::chrono` (no direct use here)
#include <fstream>             // std::ifstream (loadResults)
#include <iomanip>             // std::setw, std::setprecision, std::fixed, std::left, std::showpos
#include <iostream>            // std::cout
#include <sstream>             // std::ostringstream (printComparisonRow)
#include <string>              // std::string, std::string_view
#include <vector>              // std::vector
#include <filesystem>          //
#include <algorithm>           //
#include <stdexcept>           //
#include <tuple>               //
// clang-format on

using json = nlohmann::json;
using namespace std::chrono;

// ANSI terminal color codes.
constexpr const char* RESET = "\033[0m";
constexpr const char* GREEN = "\033[92m";
constexpr const char* RED = "\033[91m";
constexpr const char* YELLOW = "\033[93m";
constexpr const char* CYAN = "\033[96m";
constexpr const char* GRAY = "\033[37m";
constexpr const char* BLUE = "\033[94m";

// Shape of one entry loaded from a benchmark JSON snapshot
// (benchmarks/baselines/*.json or benchmark_results.json).
struct BenchmarkResult {
    std::string suite;
    std::string operation;
    std::uint64_t total_ns;
    std::size_t iterations;
    double ns_per_op;
};

// Shape of one row this tool produces: a baseline-vs-current comparison,
// not a raw benchmark result. Kept separate from BenchmarkResult since
// they're different data (input vs. output of this tool).
struct RegressionRow {
    std::string suite;
    std::string operation;
    std::size_t iteration;
    double baseline_ns;
    double current_ns;
    double pct_change;
};

// Process-wide accumulator of comparison rows, populated by
// printComparisonRow() and read back by exportJson().
inline std::vector<RegressionRow>& regression_results() {
    static std::vector<RegressionRow> results;
    return results;
}

// Accumulates a markdown-formatted transcript of the run, built up by
// setHeader()/printComparisonRow() alongside their normal stdout printing.
// Same rationale as the benchmarks tool's markdown_buffer(): section
// grouping only exists at print time, so it's captured there rather than
// reconstructed later from regression_results().
inline std::string& markdown_buffer() {
    static std::string buffer;
    return buffer;
}

// Loads a benchmark JSON snapshot (see BenchmarkResult) from disk into a Json.
inline std::vector<BenchmarkResult> loadResults(const std::string& file) {
    std::ifstream in(file);

    if (!in) {
        throw std::runtime_error("Failed to open: " + file);
    }

    json data;
    in >> data;

    std::vector<BenchmarkResult> results;

    for (const auto& entry : data) {
        results.push_back({entry["suite"].get<std::string>(), entry["operation"].get<std::string>(),
                           entry["total_ns"].get<std::uint64_t>(),
                           entry["iterations"].get<std::size_t>(),
                           entry["ns_per_op"].get<double>()});
    }

    return results;
}

// Looks up the current ns/op for a given operation + iteration count.
inline double getCns(const std::vector<BenchmarkResult>& results, const std::string& op,
                     std::size_t iter) {
    for (const auto& result : results) {
        if (result.operation == op && result.iterations == iter) {
            return result.ns_per_op;
        }
    }

    return 0.0;
}

// Formats an iteration count (10'000/100'000/1'000'000) as a short label ("10K"/"100K"/"1M").
inline std::string convertIter(std::size_t iter) {
    return iter == 10'000 ? "10K" : iter == 100'000 ? "100K" : "1M";
}

// Prints a horizontal separator line.
inline void borderLine() {
    std::cout << GRAY << std::string(90, '-') << RESET << "\n";
}

// Prints a section header row and starts the matching markdown table.
inline void setHeader(std::string_view header) {
    borderLine();
    std::cout << std::left << CYAN << std::setw(30) << header << std::setw(15) << "Iteration"
              << std::setw(15) << "Current" << std::setw(15) << "Baseline" << std::setw(15) << "  Δ"
              << RESET << "\n";
    borderLine();

    markdown_buffer() += "\n## " + std::string(header) + "\n\n";
    markdown_buffer() += "| Test | Iteration | Current | Baseline | Δ |\n";
    markdown_buffer() += "|---|---|---|---|---|\n";
}

// Prints (and records, for export) one baseline-vs-current comparison row.
// NOTE: preserves your original call convention exactly — bNs is
// whatever you pass first (your printRegression() passes baseline there),
// cNs is what you pass second (current). Flagging this because the
// column headers say "Current"/"Baseline" but display bNs/cNs in
// that order — worth double-checking that's the mapping you actually want,
// this function doesn't change it either way.
inline void printComparisonRow(std::string_view suite, std::string_view name, std::size_t iter,
                               std::string_view iteration, double bNs, double cNs) {
    const double pct = cNs == 0 ? 0.0 : (cNs - bNs) / bNs * 100.0;

    // +-15% treated as measurement noise, not a real signal — see the
    // Termux big.LITTLE / thermal-throttling discussion.
    const char* deltaColor = (pct > 0.0) ? GREEN : (pct < 0.0) ? RED : GRAY;

    std::ostringstream deltaStream;
    deltaStream << std::showpos << std::fixed << std::setprecision(1) << pct << "%";

    std::ostringstream cnsStream;
    cnsStream << bNs << " ns";

    std::ostringstream snsStream;
    snsStream << cNs << " ns";
    
    // clang-format off
    std::cout << std::left 
              << std::setw(30) << name 
              << std::setw(15) << iteration 
              << std::setw(15) << cnsStream.str() 
              << std::setw(15) << snsStream.str() 
              << deltaColor << deltaStream.str() 
              << RESET << "\n";
    // clang-format on
    
    markdown_buffer() += "| " + std::string(name) + " | " + std::string(iteration) + " | " +
                         cnsStream.str() + " | " + snsStream.str() + " | " + deltaStream.str() +
                         " |\n";

    regression_results().push_back({std::string(suite), std::string(name), iter, bNs, cNs, pct});
}

// Iterates baseline results, printing/recording a comparison row against
// current results for each entry, grouped and re-headered by suite.
static void printRegression(const std::vector<BenchmarkResult>& baseline,
                            const std::vector<BenchmarkResult>& current) {
    std::string currentSuite = " ";

    for (std::size_t i = 0; i < baseline.size(); ++i) {
        std::string suite = baseline[i].suite;

        if (currentSuite != suite) {
            std::cout << "\n";
            setHeader(suite);
            currentSuite = suite;
        }

        std::string op = baseline[i].operation;
        std::size_t iter = baseline[i].iterations;
        std::string citer = convertIter(iter);
        double bns = baseline[i].ns_per_op;
        double cns = getCns(current, op, iter);

        printComparisonRow(suite, op, iter, citer, bns, cns);

        if (citer == "1M")
            std::cout << "\n";
    }
}

namespace fs = std::filesystem;

struct Baseline {
    int major{};
    int minor{};
    int patch{};
    std::string name;

    auto operator<=>(const Baseline&) const = default;
};

static Baseline parseBaseline(const fs::path& path) {
    std::string name = path.stem().string();

    std::string version = name;
    if (!version.empty() && (version.front() == 'v' || version.front() == 'V'))
        version.erase(0, 1);

    Baseline baseline;
    baseline.name = name;

    char dot1, dot2;
    std::stringstream ss(version);

    if (!(ss >> baseline.major >> dot1 >> baseline.minor >> dot2 >> baseline.patch)
        || dot1 != '.' || dot2 != '.') {
        throw std::runtime_error("Invalid version: " + name);
    }

    return baseline;
}

// Returns the path to the newest baseline snapshot.
static std::string latestBaseline() {
    namespace fs = std::filesystem;

    bool found = false;
    Baseline latest;
    fs::path latestPath;

    for (const auto& entry : fs::directory_iterator("benchmarks/baselines")) {
        if (!entry.is_regular_file() || entry.path().extension() != ".json")
            continue;

        Baseline current = parseBaseline(entry.path());

        if (!found || current > latest) {
            latest = current;
            latestPath = entry.path();
            found = true;
        }
    }

    if (!found)
        throw std::runtime_error("No baseline snapshots found.");

    return latestPath.string();
}


static void printList() {
    std::vector<Baseline> baselines;

    for (const auto& entry : fs::directory_iterator("benchmarks/baselines")) {
        if (!entry.is_regular_file() || entry.path().extension() != ".json")
            continue;

        baselines.push_back(parseBaseline(entry.path()));
    }

    std::sort(baselines.begin(), baselines.end());

    std::cout << "Available baselines:\n\n";

    for (const auto& baseline : baselines)
        std::cout << "  " << baseline.name << '\n';
}