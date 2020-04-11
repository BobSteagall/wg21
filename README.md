## wg21
 [![Travis Build Status](https://travis-ci.org/BobSteagall/wg21.svg?branch=master)](https://travis-ci.org/BobSteagall/wg21)
 [![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/BobSteagall/wg21?svg=true&branch=master)](https://ci.appveyor.com/project/BobSteagall/wg21)
 
Revision R6 The linear algebra proposal P1385 is currently underway in the `la_r6` branch.

## Support

P1385 is targeted for C++23.  Current compiler support is as follows, but is liable to change as compilers catch up with implementing new language features.

* Windows
  * Visual Studio 2019

* Mac OS
  * macOS 10.14, Xcode 10.3
  * macOS 10.14, Xcode 11.2

* Ubuntu 18
  * GCC 8.x, 9.x
  * Clang 7.x, 8.x, 9.x 

## Building Manually Via CMake

The project can be build via CMake as follows:

```bash
cd <project root>
mkdir build
cd build

cmake -G <generator> <configuration options> ../linear_algebra/code
cmake --build ../
ctest
```

The following configuration options are available:

| Name                | Possible Values | Description                             | Default Value |
|---------------------|-----------------|-----------------------------------------|---------------|
| `BUILD_TESTING`     | `ON`, `OFF`     | Build the test suite                    | `ON`          |

## Installing Via CMake

Installing the project can be run as follows:

```bash
cd <project root>
mkdir build
cd build

cmake -G <generator> <configuration options> -DCMAKE_INSTALL_PREFIX=<install dir> ../
cmake --build ../linear_algebra/code --target install
```

## Packages

The Linear Algebra library is available for integration into your own project via our favorite package manager:

### Conan

To add the linear_algebra library to your project as a dependency, you need to add a remote to Conan to point to the location of the library:

```bash
cd <project root>
pip install conan

conan remote add linear_algebra https://api.bintray.com/conan/twonington/public-conan
```

Once this is set, you can add the linear_algebra dependency to you project via the following signature:

```bash
linear_algebra/0.0.1@public-conan/testing
```

Available versions of the Linear Algebra package can be search via Conan:

```bash
conan search linear_algebra
```

### Building the Conan package

The linear_algebra project and package can be built locally via the Conan as such:

```bash
cd <project root>
pip install conan
conan create <project root> [--test-folder None]
```