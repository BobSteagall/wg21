## wg21
 [![Travis Build Status](https://travis-ci.org/BobSteagall/wg21.svg?branch=master)](https://travis-ci.org/BobSteagall/wg21)
 [![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/BobSteagall/wg21?svg=true&branch=master)](https://ci.appveyor.com/project/BobSteagall/wg21)
 
Work on revision R7  of linear algebra proposal P1385 is currently underway in the `r7` branch.

## Support

P1385 is targeted for C++23.  Current compiler support is as follows, but will change as new compilers become available and catch up with implementing new language features for C++20.  Builds and unit testing have been successful on the following combinations of operating system and compiler:

* Windows 10
  * Visual Studio 2019, Version 16.5.4 and higher

* Mac OS 10.14
  * Xcode 11.4 and higher

* Ubuntu 18.04
  * GCC 8.x, 9.x, 10.x
  * Clang 8.x, 9.x, 10.x 

## Cloning the Repo

After ensuring that git is in your executable path, open a terminal window on Linux, or a Command Prompt on Windows 10, and then:

```bash
cd <repo_root>
git clone --recurse-submodules https://github.com/BobSteagall/wg21.git <project_root>
```

If you do not specify `<project_root>`, it will default to `wg21`.

## Building with the Visual Studio Solution

Open the `lin_alg_test.sln` solution file in the `<repo_root>/<project_root>` directory.  Click **Build..Build Solution** to build the unit test program.  (Note: This build method uses the NuGet package manager to download and install Google Test in the `<project_root>/packages` directory, so you'll need to ensure NuGet is installed.)


## Building Manually Via CMake on Windows 10

The unit test program can be built via CMake on Windows 10 by proceeding as follows.  First, generate the Visual Studio project files:

```bash
cd <project_root>
mkdir build-win
cd build-win
cmake -G "Visual Studio 16 2019" ..
```

Next, build the `Debug` configuration and run the unit test:

```bash
cmake --build . --config Debug
ctest -C Debug
```

Finally, build the `Release` configuration and run the unit test:

```bash
cmake --build . --config Release
ctest -C Release
```

> Note: CMake uses a multi-configuration generator for Visual Studio, so there is no need to specify a CMake build type;  in fact, CMake will ignore `CMAKE_BUILD_TYPE` when using the Visual Studio generator.  

The following CMake configuration options are available on Windows:

| Name                     | Possible Values                      | Description                          | Default Value |
|--------------------------|--------------------------------------|--------------------------------------|---------------|
| `BUILD_TESTING`          | `ON`, `OFF`                          | Build the test suite                 | `ON`          |
| `LA_BUILD_USING_PCH`     | `ON`, `OFF`                          | Build using precompiled headers      | `OFF`         |


## Building Manually Via CMake on Linux

The project can be built via CMake on Linux as follows:

```bash
cd <project_root>
mkdir build-dir
cd build-dir
cmake [-G <generator>] [configuration options] ..
cmake --build . [--config <build_type>]
ctest [--config <build_type>]
```

For example, to build and test the `Debug` configuration, you could do the following:

```bash
cd <project_root>
mkdir build-debug
cd build-debug
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
ctest
```

Likewise, for the `Release` configuration, you could do this:

```bash
cd <project_root>
mkdir build-release
cd build-release
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
ctest
```

> Note: CMake uses a single-configuration generator when generating makefiles, so there is no need to use the `--config` flag with CMake or the `-C` flag with CTest.  When using CMake on Linux, a good practice is to create a separate build directory for each CMake build type.

The following configuration options are available on Linux:

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

cmake [-G <generator>] [configuration options] -DCMAKE_INSTALL_PREFIX=<install_dir> ..
cmake --build . --target install
```

## Packages

The Linear Algebra library is available for integration into your own project via our favorite package manager: [Conan](https://docs.conan.io/en/latest/).

### Getting the Conan package

To add the *linear_algebra* library to your project as a dependency, you need to add a remote to Conan that points to the location of the package:

```bash
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
cd <project_root>
pip install conan
conan create [conan options] <project_root> [--test-folder None]
```
