#include <iostream>
#include <chrono>
#include <cstddef>
#include <string>
#include <vector>

#include "Json.h"
#include "Table.h"

// ============================================================
// Benchmark Configuration
// ============================================================
std::vector<std::size_t> iter = {
	10'000,
	20'000,
	40'000,
	80'000,
};

// ============================================================
// Benchmark Runner
// Executes a callable multiple times, measures execution time,
// and displays the results in a formatted table.
// ============================================================
template<typename F>
void benchmarks(
    const std::string& title,
    const std::vector<std::string>& subTitle,
    std::size_t length,
    F&& func,
    const std::vector<std::size_t>& iterations = iter)
{
	std::vector<std::size_t> durations;

	for(std::size_t run = 0; run < iterations.size(); ++run) {
		auto clock_now = []() {
			return std::chrono::high_resolution_clock::now();
		};

		auto start = clock_now();
        
        volatile double sink = 0;
        
		for(std::size_t i = 0; i < iterations[run]; ++i) {
			sink += func();
		}

		auto end = clock_now();

		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

		durations.push_back(duration);
	}

	Table::TableData data {
		{
			Table::convert(iterations),
			Table::convert(durations, "ms")
		}
	};

	Table::table(
	{title},
	subTitle,
	data,
	length
	);
}

// ============================================================
// Parse Benchmark
// Measures JSON parsing performance.
// ============================================================
void parse() {
	std::string text =
	    "{"
	    "\"a\": 1,"
	    "\"b\": 2,"
	    "\"c\": [3, 4, 5, 6, 7],"
	    "\"d\": {"
	    "\"x\": 8,"
	    "\"y\": 9,"
	    "\"z\": 10"
	    "}"
	    "}";

	std::cout << "\n";
	benchmarks(
	    "Parse Benchmark",
	{ "Iterations", "Time" },
	30,
	[&]() {
		return Json::parse(text).asObject().size();
	});
	
	std::cout << "\n";
}

// ============================================================
// Object Access Benchmark
// Measures object member lookup performance.
// ============================================================
void object_access() {
	Json obj = Json::parse(
	               "{"
	               "\"a\": 10,"
	               "\"b\": 20,"
	               "\"c\": 30,"
	               "\"d\": 40,"
	               "\"e\": 50"
	               "}"
	           );

	benchmarks(
	    "Object Access Benchmark",
	{ "Iterations", "Time" },
	30,
	[&]() {
		return obj["a"].asNumber()
		       + obj["b"].asNumber()
		       + obj["c"].asNumber()
		       + obj["d"].asNumber()
		       + obj["e"].asNumber();
	});
	
	std::cout << "\n";
}

// ============================================================
// Array Access Benchmark
// Measures array indexing performance.
// ============================================================
void array_access() {
    Json arr = Json::parse("[1,2,3,4,5,6,7,8,9,10]");
    
    benchmarks(
        "Array Access Benchmark",
	{ "Iterations", "Time" },
	30,
	[&]() {
	    double sum = 0.0;
	    
	    for(std::size_t i = 0; i < 10; ++i) {
	        sum += arr[i].asNumber();
	    }
	    
	    return sum;
	});
	
	std::cout << "\n";
}

int main() {
	parse();

	object_access();
	
	array_access();
	
	return 0;
}
