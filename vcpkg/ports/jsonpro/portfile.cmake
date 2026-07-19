vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO privateMwb/JsonParser
    REF v1.0.6
    SHA512 c83ba644897d155621ec61053f029c2c303f2caf22f6ad763bbf49205dfdf0ecc6bcd8b53d7e7f61940b38fc134fcc9f183e19bf73543034b88fe45766e7110c
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