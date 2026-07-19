vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO privateMwb/JsonParser
    REF v1.0.0
    SHA512 ff332d779350c135be0566f6db5fe8186b799a7cd5f5b3fac3549298e2f422c2604baea2e11ad3203abe76fab9ac8b5ef37174f6bc626d52722d3482b587b2a5
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
    CONFIG_PATH lib/cmake/JsonPro
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