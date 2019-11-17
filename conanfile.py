#!/usr/bin/env python
# -*- coding: utf-8 -*-
from conans import ConanFile, CMake, tools
import os.path

class LinearAlgebraConan(ConanFile):
    name = "linear_algebra"
    version = "0.0.1"
    license = "MIT"
    url = "https://github.com/BobSteagall/wg21"
    description = "A linear algebra proposal for the C++ standard library"
    topics = ("conan", "linear algebra", "header-only", "std", "math", "wg21")
    exports_sources = "*.txt", "*.hpp", "*.cpp", "*.cmake", "*.cmake.in", "LICENSE.txt"
    generators = "cmake"

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
