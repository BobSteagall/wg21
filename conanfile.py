#!/usr/bin/env python
# -*- coding: utf-8 -*-
from conans import ConanFile, CMake, tools
from conans.tools import load
from conans.model.version import Version

import re, os.path

class LinearAlgebraConan(ConanFile):
    name = "linear_algebra"
    license = "MIT"
    url = "https://github.com/BobSteagall/wg21"
    description = "A linear algebra proposal for the C++ standard library"
    topics = ("conan", "linear algebra", "header-only", "std", "math", "wg21")
    exports_sources = "*.txt", "*.hpp", "*.cpp", "*.cmake", "*.cmake.in", "LICENSE.txt"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake_paths"
    requires = ("mdspan/0.1.0")

    def set_version(self):
        content = load(os.path.join(os.path.dirname(__file__), "CMakeLists.txt"))
        version = re.search(r'project\(wg21_linear_algebra VERSION (\d+\.\d+\.\d+)\)', content).group(1)
        self.version = version.strip()

    def build_requirements(self):
        # Ensure the package is build against a version of CMake from 3.16 onwards.
        if CMake.get_version() < Version("3.16"):
            self.build_requires("cmake/3.16.4")

    _cmake = None
    @property
    def cmake(self):
        if self._cmake is None:
            self._cmake = CMake(self)
            self._cmake.definitions.update({
                "LA_BUILD_PACKAGE": True,
                "LA_ENABLE_TESTS": self.run_tests
            })
            self._cmake.configure()
        return self._cmake

    @property
    def run_tests(self):
        """ By default tests should not be built and run during package creation. It can optionally be enabled,
            for example for on CI by enabling setting the environment variable CONAN_RUN_TESTS=1. """
        return tools.get_env("CONAN_RUN_TESTS", False)

    def build(self):
        self.cmake.build()
        if tools.cross_building(self.settings):
            return
        if self.run_tests:
            self.cmake.test()

    def package(self):
        self.cmake.install()
