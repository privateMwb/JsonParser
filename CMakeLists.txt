# ============================================================
# CMakeLists.txt
# Build configuration for the JsonPro library.
# ============================================================
#
# Sections:
#   1. C++ Configuration
#   2. JsonPro Library
#   3. Subdirectories
#   4. Installation
#   5. Packaging
#
# ============================================================

cmake_minimum_required(VERSION 3.20)

project(
    JsonPro
    VERSION 1.0.0
    LANGUAGES CXX
)

# ============================================================================
# C++ Configuration
# ============================================================================

# Configure the C++ language standard.
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Place all executables directly in the build directory, regardless of
# which subdirectory defines them (e.g. build/tests, build/compare).
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# ============================================================================
# JsonPro (Header-only Library)
# ============================================================================

# Collect all library source files.
file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/JsonPro/*.cpp"
)

# Define the header-only JsonPro library.
add_library(JsonPro STATIC ${LIB_SOURCES})

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

# ============================================================================
# Subdirectories
# ============================================================================

# Consumers (e.g. vcpkg) should disable these — only the library itself
# needs to be configured/installed for downstream projects. Defaults to
# ON so local development and existing CI workflows are unaffected.
option(JSONPRO_BUILD_TESTS "Build the JsonPro test suite" ON)
option(JSONPRO_BUILD_BENCHMARKS "Build the JsonPro benchmark suite" ON)
option(JSONPRO_BUILD_TOOLS "Build the compare/regression benchmark tools" ON)
option(JSONPRO_BUILD_EXAMPLES "Build the JsonPro examples" ON)

enable_testing()

if(JSONPRO_BUILD_TESTS)
    add_subdirectory(tests tests_build)
endif()

if(JSONPRO_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks benchmarks_build)
endif()

if(JSONPRO_BUILD_TOOLS)
    add_subdirectory(tools tools_build)
endif()

if(JSONPRO_BUILD_EXAMPLES)
    add_subdirectory(examples examples_build)
endif()

# ============================================================================
# Installation
# ============================================================================

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

# ============================================================================
# Packaging
# ============================================================================

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