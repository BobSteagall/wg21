## wg21
 [![Travis Build Status](https://travis-ci.org/BobSteagall/wg21.svg?branch=master)](https://travis-ci.org/BobSteagall/wg21)
 [![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/BobSteagall/wg21?svg=true&branch=master)](https://ci.appveyor.com/project/BobSteagall/wg21)
 
Work on revision R7  of linear algebra proposal P1385 is currently underway in the `r7` branch.

## Support

P1385 is targeted for C++23.  Current compiler support is as follows, but will change as compilers catch up with implementing new language features for C++20.

* Windows
  * Visual Studio 2019, Version 16.5.4

* Mac OS
  * macOS 10.14, Xcode 10.3
  * macOS 10.14, Xcode 11.2

* Ubuntu 18
  * GCC 8.x, 9.x, 10.x
  * Clang 8.x, 9.x, 10.x 

## Building Manually Via CMake

The project can be built via CMake as follows:

```bash
cd <project root>
mkdir build
cd build

cmake -G <generator> <configuration options> ..
cmake --build . --config <build_type>
ctest --config <build_type>
```

The following configuration options are available:

| Name                     | Possible Values                      | Description                          | Default Value |
|--------------------------|--------------------------------------|--------------------------------------|---------------|
| `BUILD_TESTING`          | `ON`, `OFF`                          | Build the test suite                 | `ON`          |
| `CMAKE_BUILD_TYPE`       | `Debug`, `Release`, `RelWithDebInfo` | Build type                           | `Debug`       |
| `LA_BUILD_USING_PCH`     | `ON`, `OFF`                          | Build using precompiled headers      | `OFF`         |
| `LA_ENABLE_SANITIZERS`   | `ON`, `OFF`                          | Build with address and UB sanitizers | `OFF`         |
| `LA_VERBOSE_TEST_OUTPUT` | `ON`, `OFF`                          | Write verbose test results           | `OFF`         |

## Installing Via CMake

Installing the project can be performed as follows:

```bash
cd <project root>
mkdir build
cd build

cmake -G <generator> <configuration options> -DCMAKE_INSTALL_PREFIX=<install dir> ..
cmake --build . --target install
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
linear_algebra/0.7.0@public-conan/testing
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
