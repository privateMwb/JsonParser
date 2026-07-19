from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy, get
import os


class JsonProConan(ConanFile):
    name = "jsonpro"
    version = "1.0.0"

    package_type = "library"

    license = "MIT"
    author = "privateMwb"
    url = "https://github.com/privateMwb/JsonParser"
    description = "High-performance RFC 8259 JSON value type and recursive-descent parser."
    topics = (
        "json",
        "parser",
        "serialization",
        "cpp",
        "data-structure",
    )

    settings = "os", "compiler", "build_type", "arch"

    options = {
        "shared": [True, False],
        "fPIC": [True, False],
    }

    default_options = {
        "shared": False,
        "fPIC": True,
    }

    exports_sources = (
        "CMakeLists.txt",
        "cmake/*",
        "include/*",
        "src/*",
    )

    def config_options(self):
        if self.settings.os == "Windows":
            self.options.rm_safe("fPIC")

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        cmake_layout(self)

    def validate(self):
        check_min_cppstd(self, 23)

    def source(self):
        get(
            self,
            **self.conan_data["sources"][self.version],
            strip_root=True,
        )

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure(
            variables={
                "JSONPRO_BUILD_TESTS": "OFF",
                "JSONPRO_BUILD_BENCHMARKS": "OFF",
                "JSONPRO_BUILD_TOOLS": "OFF",
                "JSONPRO_BUILD_EXAMPLES": "OFF",
            }
        )
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

        copy(
            self,
            "LICENSE",
            src=self.source_folder,
            dst=os.path.join(self.package_folder, "licenses"),
        )

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "JsonPro")
        self.cpp_info.set_property("cmake_target_name", "JsonPro::JsonPro")
        self.cpp_info.libs = ["JsonPro"]