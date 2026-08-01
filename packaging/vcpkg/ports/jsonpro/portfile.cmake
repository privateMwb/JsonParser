vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO privateMwb/JsonParser
    REF v1.0.0
    SHA512 6120462e174d7f5ab7cf6e991c6f71d4990d0bf7b4cf3edd4681986a83b7645d9cd15c7f3b6cb251865801f82c39c890d4a6fb64d2f566c730cb0172c9b9b699
)

set(VCPKG_PORT_NAME JsonPro)

# Consumers only need the library itself, not the tests, benchmarks,
# regression tools, or examples. regression/ also fetches a third-party
# dependency via FetchContent at configure time, which requires network
# access that vcpkg's build sandbox does not allow.
vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DBUILD_TESTS=OFF
        -DBUILD_BENCHMARKS=OFF
        -DBUILD_REGRESSION=OFF
        -DBUILD_EXAMPLES=OFF
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(
    PACKAGE_NAME ${VCPKG_PORT_NAME}
    CONFIG_PATH lib/cmake/${VCPKG_PORT_NAME}
)

# This library is compiled (not header-only), so debug binaries are
# real and must be kept — only the duplicate debug/include headers
# are removed.
file(
    REMOVE_RECURSE
    "${CURRENT_PACKAGES_DIR}/debug/include"
)

vcpkg_install_copyright(
    FILE_LIST "${SOURCE_PATH}/LICENSE"
)