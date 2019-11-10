# wg21
 [![Travis Build Status](https://travis-ci.org/BobSteagall/wg21.svg?branch=master)](https://travis-ci.org/BobSteagall/wg21)
 [![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/BobSteagall/wg21?svg=true&branch=master)](https://ci.appveyor.com/project/BobSteagall/wg21)
 
# Support

The linear algebra proposal is targeted for C++23.  Current compiler support is as follows but is liable to change as compilers catch up with implementing new language features.

* Windows
  * Visual Studio 2019

* Mac OS
  * macOS 10.14, Xcode 10.3
  * macOS 10.14, Xcode 11.2

* Ubuntu 18
  * GCC 8.x, 9.x
  * Clang 7.x, 8.x, 9.x 

# Building Manually Via CMake

The project can be build via CMake as follows:
```bash
cd <project root>
mkdir build
cd build

cmake -G <generator> <configuration options> ../linear_algrbra/code
cmake --build ../
ctest
```

# Installing Via CMake

Installing the project can be run as follows:
```bash
cd <project root>
mkdir build
cd build

cmake -G <generator> <configuration options> -DCMAKE_INSTALL_PREFIX=<install dir> ../
cmake --build ../linear_algrbra/code --target install
```
