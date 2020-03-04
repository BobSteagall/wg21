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
    generators = "cmake"

    def set_version(self):
        content = load(os.path.join(os.path.dirname(__file__), "linear_algebra", "code", "CMakeLists.txt"))
        version = re.search(r'project\(wg21_linear_algebra VERSION (\d+\.\d+\.\d+)\)', content).group(1)
        self.version = version.strip()

    def build_requirements(self):
        # Ensure the package is build against a version of CMake from 3.14 onwards.
        if CMake.get_version() < Version("3.14"):
            self.build_requires("cmake_installer/3.14.7@conan/stable")

    _cmake = None
    @property
    def cmake(self):
        if self._cmake is None:
            self._cmake = CMake(self)
            self._cmake.definitions.update({
                "BUILD_TESTING": False
            })
            self._cmake.configure(source_folder=os.path.join("linear_algebra", "code"))
        return self._cmake


    def package(self):
        self.cmake.build()
        if self.options.build_testing:
            self.cmake.test()

    def package(self):
        self.cmake.install()
