vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO privateMwb/JsonPro
    REF v1.0.0
    SHA512 6553965c96f5e0af0783372fa524185e39f7eb2d5edb486bc5cf7b77cf6afafb410b9c8f35f40d7aba926f8b5dae21ec21f76a52e18cd729844538d8a6bcfe3e
)

# JsonPro consumers only need the library itself, not the tests,
# benchmarks, tools, or examples. tools/ also fetches nlohmann_json
# via FetchContent at configure time, which requires network access
# that vcpkg's build sandbox does not allow.
vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DJSONPRO_BUILD_TESTS=OFF
        -DJSONPRO_BUILD_BENCHMARKS=OFF
        -DJSONPRO_BUILD_TOOLS=OFF
        -DJSONPRO_BUILD_EXAMPLES=OFF
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(
    PACKAGE_NAME JsonPro
)

# Unlike HashMapPro, JsonPro is compiled (not header-only), so debug
# binaries are real and must be kept — only the duplicate debug/include
# headers are removed.
file(
    REMOVE_RECURSE
    "${CURRENT_PACKAGES_DIR}/debug/include"
)

vcpkg_install_copyright(
    FILE_LIST "${SOURCE_PATH}/LICENSE"
)