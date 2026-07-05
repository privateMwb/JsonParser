# ============================================================
# CMakeLists.txt
# Build configuration for the JsonPro library.
# ============================================================
#
# Sections:
#   1. C++ Configuration
#   2. JsonPro Library
#   3. Tests
#   4. Benchmarks
#   5. Examples
#   6. Installation
#   7. Packaging
#
# ============================================================

cmake_minimum_required(VERSION 3.20)

project(
    JsonPro
    VERSION 1.0.0
    LANGUAGES CXX
)

# ============================================================
# Section 1 — C++ Configuration
# ============================================================

# Configure the C++ language standard.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
# Section 2 — JsonPro Library
# ============================================================

# Collect all library source files.
file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Build the JsonPro library.
add_library(
    JsonPro
    ${LIB_SOURCES}
)

# Expose the public include directory to consumers.
target_include_directories(
    JsonPro
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Require C++23 for all targets that consume JsonPro.
target_compile_features(
    JsonPro
    PUBLIC
    cxx_std_23
)

# ============================================================
# Section 3 — Tests
# ============================================================

# Collect all test source files.
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
)

# Build the test executable.
add_executable(
    tests
    ${TEST_SOURCES}
)

# Allow simple test includes.
target_include_directories(
    tests
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tests
)

# Link JsonPro to the test executable.
target_link_libraries(
    tests
    PRIVATE
    JsonPro
)

# ============================================================
# Section 4 — Benchmarks
# ============================================================

# Collect all benchmark source files.
file(GLOB_RECURSE BENCH_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/benchmarks/*.cpp"
)

# Build the benchmark executable.
add_executable(
    benchmarks
    ${BENCH_SOURCES}
)

# Allow simple benchmark includes.
target_include_directories(
    benchmarks
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks
)

# Link JsonPro to the benchmark executable.
target_link_libraries(
    benchmarks
    PRIVATE
    JsonPro
)

# ============================================================
# Section 5 — Examples
# ============================================================

# Collect all example source files.
file(GLOB_RECURSE EXAMPLE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/examples/*.cpp"
)

# Build the example executable.
add_executable(
    examples
    ${EXAMPLE_SOURCES}
)

# Allow simple example includes.
target_include_directories(
    examples
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/examples
)

# Link JsonPro to the example executable.
target_link_libraries(
    examples
    PRIVATE
    JsonPro
)

# ============================================================
# Section 6 — Installation
# ============================================================

# Install the JsonPro target and export it for downstream projects.
install(
    TARGETS JsonPro
    EXPORT JsonProTargets
)

# Install the exported CMake targets.
install(
    EXPORT JsonProTargets
    FILE JsonProTargets.cmake
    NAMESPACE JsonPro::
    DESTINATION lib/cmake/JsonPro
)

# Install the public header files.
install(
    DIRECTORY include/
    DESTINATION include
)

# ============================================================
# Section 7 — Packaging
# ============================================================

include(CMakePackageConfigHelpers)

# Generate the package configuration file.
configure_package_config_file(
    cmake/JsonProConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/JsonProConfig.cmake
    INSTALL_DESTINATION lib/cmake/JsonPro
)

# Generate the package version file.
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/JsonProConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Install the package configuration files.
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/JsonProConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/JsonProConfigVersion.cmake
    DESTINATION lib/cmake/JsonPro
)