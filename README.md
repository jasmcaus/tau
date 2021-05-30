<div align="center">

<h1>Muon</h1>

[![Standard](https://img.shields.io/badge/C-11/14/17-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Standard](https://img.shields.io/badge/C%2B%2B-11/14/17/20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B)
[![License](https://img.shields.io/github/license/jasmcaus/Muon?label=license)][license]
[![Twitter](https://img.shields.io/twitter/follow/jasmcaus.svg?style=flatl&label=Follow&logo=twitter&logoColor=white&color=1da1f2)][twitter-badge]
[![Download](https://img.shields.io/badge/download%20%20-link-green.svg)](https://github.com/jasmcaus/Muon/releases)
[![Docs](https://img.shields.io/badge/docs%20%20-online-blue.svg)][docs]

</div>

A Micro Unit Testing Framework for >C11/C++11 projects, with the promise of *always being tiny* - about 1k lines of
code. This framework is a *much* simpler, *much* lighter and *much* faster alternative to heavier frameworks like 
Google Test, & Catch2, making it suitable for on-to-go testing (embedded developers will especially love us!).

I initially wrote Muon to be a unit testing framework for C; however, initial results showed great promise of 
compiling with (and testing) C++ code. While Muon doesn't currently support mocking, or a way to test for exceptions
in C++, its limitations are in fact its biggest strength - you get *negligible* overhead & fast compilation speeds 
for the sacrifice of a few constructs.


## Features
* *Ultra-light* (~1k lines of code)
* Can test both C and C++ code (see [ThirdParty tests](https://github.com/jasmcaus/Muon/blob/dev/test/thirdparty))
* [Blazing Fast Assertions](https://github.com/jasmcaus/Muon/blob/dev/benchmarks)
* Gtest-like Assertion Macros
* Test Fixtures 


# Installation
None! Muon is header-only, so simply include it in your project. 
```c
#include <Muon/Muon.h>
```

To build Muon with CMake, read through the [CMake Quickstart Guide](https://github.com/jasmcaus/Muon/blob/dev/docs/CMake-quickstart.md).


## Basic Concepts
Muon provides several variants of Assertion Macros for us - `CHECK`s which are *non-fatal* asserts, and `REQUIRE`s 
which are *fatal* asserts. The result of an assertion is either *success*, *non-fatal failure* or a *fatal failure*. 
Unless the latter takes place, the program continues normally. 

In Muon, you would normally define a ***Test Suite*** which contains multiple tests. These test suites should 
ideally reflect the structure of your tested code. 


# Prerequistes
To begin, you **must** include the following in *any* (but only one) C/C++ file. This initializes Muon to set up 
all your tests:
```c
MUON_MAIN() // IMPORTANT: No semicolon at the end 
```
This defines a main function, so if you write a `main()` function ***and*** declare `MUON_MAIN()`, your compiler will
throw a `redeclaration of main` error. 

If you must write a `main()` function, use the following instead:
```c
MUON_NO_MAIN()
```
This won't define a main function, but sets up any variables/methods that Muon needs to run properly.


# Getting Started
## Defining a Test Suite
To define a test suite, simply do the following:
```c
TEST(TestSuiteName, TestName) {
    CHECK(1); // does not fail
    ... rest of the test body ...
}
```
The `TEST` macro takes two parameters - the first is the name of the Test Suite, and the second is the name of the 
test. This allows tests to be grouped for convenience. 


## Testing Macros
Muon provides two variants of Assertion Macros - `CHECK`s and `ASSERT`s. These resemble function calls. When these 
assertions fail, Muon prints the source code location (file + line number) along with a failure message. 

`ASSERT`s generate *fatal* failures - the test suite will cease its execution and move on to the next test suite to 
run. 

`CHECK`s generate *non-fatal* failures - the remainder of the test suite will still execute, allowing for further 
checks to run. 

Read [the Primer](https://github.com/jasmcaus/Muon/blob/dev/docs/muon-primer.md) for more details, including the 
other testing macros Muon provides you with.


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


# Supported Platforms
Muon supports codebases and compilers that are compliant with the C11/C++11 standard or newer. Muon's source
code is officially supported on the following platforms. If you notice any problems on your platform, please 
file an issue on the [Muon Github Issue Tracker][issues]. PRs with fixes are welcome! 

Operating Systems          | Compilers       
-------------------------- | -------------------------- 
Linux                      | gcc 5.0+ 
macOS                      | clang 5.0+
Windows                    | MSVC 2017+


# Contributing
We appreciate all contributions, feedback and issues. If you plan to contribute new features, utility functions,
or extensions to the core, please go through our [Contribution Guidelines][contributing].

To contribute, start working through the `Muon` codebase, read the [Documentation][docs], navigate to the 
[Issues][issues] tab and start looking through interesting issues. 


# Asking for help
If you have any questions, please:
1. [Read the docs][docs].
2. [Look it up in our Github Discussions (or add a new question)][discussions].
2. [Search through the issues][issues].


# Who uses Muon?
In addition to several of my personal projects, Muon is also used in the following notable projects:
* The [Hazel](https://github.com/HazelLang/Hazel) Programming Language


# Supporting This Project
If you are able to and would like to sponsor this project, I'm available on [Patreon](https://patreon.com/jasmcaus),
[Ko-Fi](https://ko-fi.com/jasmcaus), and [Paypal](https://paypal.me/jasmcaus). Your support is greatly appreciated 😊.


# License 
This project was written by [Jason Dsouza](https://github.com/jasmcaus) and licensed under the [MIT License](LICENSE).

[contributing]: https://github.com/jasmcaus/Muon/blob/dev/.github/CONTRIBUTING.md
[docs]: https://github.com/jasmcaus/Muon/blob/dev/docs
[discussions]: https://github.com/jasmcaus/Muon/discussions
[issues]: https://github.com/jasmcaus/Muon/issues

[twitter-badge]: https://twitter.com/jasmcaus
[license]: https://github.com/jasmcaus/Muon/blob/dev/LICENSE