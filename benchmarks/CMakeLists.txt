# ============================================================
# benchmarks/CMakeLists.txt
# Build configuration for the JsonPro benchmark suite.
# ============================================================

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Fetch nlohmann::json — the baseline every benchmark case compares
# JsonPro against. FetchContent_MakeAvailable is a no-op if some outer
# scope (e.g. the top-level CMakeLists.txt) has already made it available.
include(FetchContent)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(nlohmann_json)

# Collect all benchmark suite source files.
file(GLOB_RECURSE BENCH_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/suite/*.cpp"
)

# Build the benchmark executable.
add_executable(
    benchmarks
    ${BENCH_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/bench_all.cpp
)

# Allow simple benchmark includes.
target_include_directories(
    benchmarks
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
)

# Link JsonPro and nlohmann::json to the benchmark executable.
# nlohmann_json::nlohmann_json carries its own include directory as an
# INTERFACE property, so no separate target_include_directories() entry
# for it is needed.
target_link_libraries(
    benchmarks
    PRIVATE
        JsonPro
        nlohmann_json::nlohmann_json
)