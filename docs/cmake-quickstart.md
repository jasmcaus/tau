# Quickstart: Building With CMake

This guide will get you up and running with Muon using CMake. 

## Prerequisites
To follow along, you'll need:
* A compatible operating system (Linux, macOS or Windows).
* A compatible C/C++ compiler that supports at least C11/C++11 onwards.
* [CMake](https://cmake.org/) and a compatible build tool ([Make](https://www.gnu.org/software/make/), [Ninja](https://ninja-build.org/) and [others](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html)). 

If you don't already have CMake installed, see the [CMake Installation Guide](https://cmake.org/install). 


## Set up a project
CMake uses the `CMakeLists.txt` file to configure the build system for your project. You'll need this file to set up your project and declare a dependency on Muon. 

First, in your project directory (for the most part, in your ***root*** project directory), create the `CMakeLists.txt` file. 
Next, you'll need to tell CMake that you want Muon as a dependency. You can do this in numerous ways, but we recommend the [`FetchContent` CMake module](https://cmake.org/cmake/help/latest/module/FetchContent.html). 

Inside the CMakeLists.txt, add the following contents:
```cmake
cmake_minimum_required(VERSION 3.20)
project(DemoProject) # name this to whatever you'd like 

# Muon requires at least C11/C++11
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  Muon
  URL https://github.com/jasmcaus/Muon/archive/dev.zip
)

FetchContent_MakeAvailable(Muon)
```

The above configuration declares a dependency on Muon which is automatically downloaded from Github. This example always includes the *latest* version of Muon. 

## Creating & Running a Binary
With Muon as a dependency, you can now use Muon code within your project. 

As an example, create a file named `test_assertions.c` in your project directory with the following contents:
```c
#include "Muon/Muon.h"

MUON_MAIN()

TEST(a, count) { 
    int count = 10000000;
    REQUIRE_EQ(count, 10000000); 
}

TEST(b, require) {
    REQUIRE_LE(1, 1);
    REQUIRE_LE(1, 2);
}
```

You can read more about the assertions that Muon provides [here](https://github.com/jasmcaus/Muon/blob/dev/docs/CMake-quickstart.md). 

To build the code, add the following to the end of your `CMakeLists.txt` file:
```cmake
enable_testing()

add_executable(
    DemoProject
    test_assertions.c
) 

target_link_libraries(
    DemoProject
    Muon
)
```

The above configuration enable testing in CMake, declares the C/C++ (in our case, C) test binary you want to build (`DemoProject`) and links to `Muon`. 

Now you can build and run your test:
<pre>
<strong>DemoProject$ cmake -S . -B build </strong>
-- The C compiler identification is GNU 10.2.1
-- The CXX compiler identification is GNU 10.2.1
...
-- Build files have been written to: .../DemoProject/build

<strong>DemoProject$ cmake --build build </strong>
Scanning dependencies of target gtest
...
[100%] Built target Muon

<strong>DemoProject$ cd build && ./DemoProject </strong>
[==========] Running 2 test cases.
...
Summary:
   Total unit tests:      2
   Total tests run:       2
   Total tests skipped:   0
   Total tests failed:    0
SUCCESS: 2 tests have passed in 8.35ms
</pre>

Congratulations! You've successfully built and run a test binary using Muon!

# Next Steps
* [Check out the Primer](muon-primer.md) in Muon to start writing powerful unit tests.