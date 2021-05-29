# Muon
[![Standard](https://img.shields.io/badge/C%2B%2B-11/14/17/20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![Standard](https://img.shields.io/badge/C-11/14/17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)
[![Download](https://img.shields.io/badge/download%20%20-link-blue.svg)](https://github.com/jasmcaus/Muon/releases)

A Micro Unit Testing Framework for C11/C++14 onwards. It's tiny - about 1k lines of code. This framework is a *much* simpler and lighter alternative to Google Test, making it suitable for on-to-go testing. 

Muon is part of [`CSTL`](https://github.com/jasmcaus/CSTL), a neater rewrite of the C/C++ Standard Library, although active development happens in the [Hazel Programming Language](https://github.com/HazelLang/Hazel) repository and changes are subsequently reflected in the [`CSTL`](https://github.com/jasmcaus/CSTL) repo.

## Features
* *Ultra-light* (~1k lines of code)
* Can test both C and C++ projects with equal performance
* [Blazing Fast Assertions](https://github.com/jasmcaus/Muon/blob/dev/benchmarks)
* Gtest-like Assertion Macros
* Test Fixtures 

## Installation
None! Muon is header-only, so simply include it in your project. 
```c
#include <Muon/Muon.h>
```

To build Muon with CMake, read through the [CMake Quickstart Guide](https://github.com/jasmcaus/Muon/blob/dev/docs/CMake-quickstart.md).


## Basic Concepts
Muon provides several variants of Assertion Macros for us - `CHECK`s which are *non-fatal* asserts, and `REQUIRE`s which are *fatal* asserts. The result of an assertion is either *success*, *non-fatal failure* or a *fatal failure*. Unless the latter takes place, the program continues normally. 

In Muon, you would normally define a ***Test Suite*** which contains multiple tests. These test suites should ideally reflect the structure of your tested code. 


## Prerequistes
To begin, you **must** include the following in *any* (but only one) C/C++ file. This initializes Muon to set up all your tests:
```c
MUON_MAIN() // IMPORTANT: No semicolon at the end 
```
This defines a main function, so if you write a `main()` function ***and*** declare `MUON_MAIN()`, your compiler will throw a `redeclaration of main` error. 

If you must write a `main()` function, add `MUON_NO_MAIN()` instead - this does not define a main function, but sets up any variables/methods that Muon needs to run properly.


## Defining a Test Suite
To define a test suite, simply do the following:
```c
TEST(TestSuiteName, TestName) {
    CHECK(1); // does not fail
    ... rest of the test body ...
}
```
The `TEST` macro takes two parameters - the first is the name of the Test Suite, and the second is the name of the test. This allows tests to be grouped for convenience. 


## Testing Macros
Muon provides two variants of Assertion Macros - `CHECK`s and `ASSERT`s. These resemble function calls. When these assertions fail, Muon prints the source code location (file + line number) along with a failure message. 

`ASSERT`s generate *fatal* failures - the test case will cease its execution and move on to the next test case to run. 
`CHECK`s generate *non-fatal* failures - the remainder of the test case will still execute, allowing for further checks to run. 

Read [the Primer](https://github.com/jasmcaus/Muon/blob/dev/docs/muon-primer.md) for more details, including the other testing macros Muon provides you with.


## Example Usage
Below is a slightly contrived example showing a number of possible supported operations:
```C
#include <Muon/Muon.h>
MUON_MAIN() // sets up Muon (+ main function)

TEST(foo, bar1) {
    int a = 42; 
    int b = 13; 
    CHECK_GE(a, b); // pass :)
    CHECK_LE(b, 8); // fail - Test suite not aborted 
}

TEST(foo, bar2) {
    char* a = "foo";
    char* b = "foobar";
    REQUIRE_STREQ(a, a); // pass :)
    REQUIRE_STREQ(a, b); // fail - Test suite aborted
}
```


## Supported Platforms
Muon supports codebases and compilers that are compliant with the C11/C++11 standard or newer. Muon's source code is officially supported on the following platforms. If you notice any problems on your platform, please file an issue on the [Muon Github Issue Tracker](https://github/jasmcaus/Muon/issues). PRs with fixes are welcome! 

Operating Systems          | Compilers       
-------------------------- | -------------------------- 
Linux                      | gcc 5.0+ 
macOS                      | clang 5.0+
Windows                    | MSVC 2017+


## Who uses Muon?
In addition to several of my personal projects, Muon is also used in the following notable projects:
* The [Hazel](https://github.com/HazelLang/Hazel) Programming Language


## License 
This project was written by [Jason Dsouza](https://github.com/jasmcaus) and licensed under the MIT License.