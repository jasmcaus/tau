# Arrow
A Micro Unit Testing Framework for C11/C++14 onwards. It's tiny - about 1k lines of code. This framework is a *much* simpler and lighter alternative to Google Test, making it suitable for on-to-go testing. 

Arrow is part of [`CSTL`](https://github.com/jasmcaus/CSTL), a neater rewrite of the C/C++ Standard Library, although active development happens in the [Hazel Programming Language](https://github.com/HazelLang/Hazel) repository and changes are subsequently reflected in the [`CSTL`](https://github.com/jasmcaus/CSTL) repo.


## Installation
None! Arrow is header-only, so simply include it in your project. 
```c
#include <Arrow/Arrow.h>
```

To build Arrow with CMake, read through the [CMake Quickstart Guide](https://github.com/jasmcaus/Arrow/blob/dev/docs/cmake-quickstart.md).

## Basic Concepts
When using Arrow, you begin by writing `assertions`, which are statements that check if a condition is true. The result of an assertion is either *success*, *non-fatal failure* or a *fatal failure*. Unless the latter takes place, the program continues normally. 

In Arrow, you would normally define a ***Test Suite*** which contains multiple tests. These test suites should ideally reflect the structure of your tested code. 


## Prerequistes
To begin, you **must** include the following in *any* (but only one) C/C++ file. This initializes Arrow to set up all your tests:
```c
ARROW_MAIN() // IMPORTANT: No semicolon at the end 
```
This defines a main function, so if you write a main function ***and*** declare `ARROW_MAIN()`, your compiler will throw a `redeclaration of main` error.


## Defining a Test Suite
To define a test suite, simply do the following:
```c
TEST(TestSuiteName, TestName) {
    CHECK(1); // fails if false
    ... rest of the test body ...
}
```
The `TEST` macro takes two parameters - the first is the name of the Test Suite, and the second is the name of the test. This allows tests to be grouped for convenience. 


## Testing Macros
Arrow provides two variants of Assertion Macros - `CHECK`s and `ASSERT`s. These resemble function calls. When these assertions fail, Arrow prints the source code location (file + line number) along with a failure message. 

`ASSERT`s generate *fatal* failures - the test case will cease its execution and move on to the next test case to run. 
`CHECK`s generate *non-fatal* failures - the remainder of the test case will still execute, allowing for further checks to run. 

We recommend using `CHECK`s over `ASSERT`s unless it doesn't make sense to continue when the assertion in question fails. 

### Adding Custom Failure Messages
We highly recommend you add a custom failure message for your macros - it makes it easier to track down bugs. `Invalid Type ID:` is much more useful than `FAILED`, which is what Arrow prints by default.

To do this, simply do the following:
```C
CHECK(i == 42, "Expected i to be 32");
```


## A List of Avaliable Testing Macros
### a. Basic Assertions
These assertions perform basic true/false condition checking. 

Fatal assertion             | Nonfatal assertion         | Checks
--------------------------  | -------------------------- | --------------------
`REQUIRE(condition);`  | `CHECK(condition);`  | `condition` is true
`REQUIRE(!condition);` | `CHECK(!condition);` | `condition` is false

### b. Binary Comparisons
For a majority of your tests, `REQUIRE` and `CHECK` will suffice. However, Arrow provides GTest-like Binary Comparisons. Both achieve the same purpose - we recommend `REQUIRE` and `CHECK` as they provide readable comparison checks. 

For user-defined types in a C++ codebase, we recommend using these Binary Comparisons (they don't require you to overload the `==`, `<=`... operators).

Fatal assertion          | Nonfatal assertion       | Checks
------------------------ | ------------------------ | --------------
`REQUIRE_EQ(x, y);` | `CHECK_EQ(x, y);`  | `x == y`
`REQUIRE_NE(x, y);` | `CHECK_NE(x, y);`  | `x != y`
`REQUIRE_LT(x, y);` | `CHECK_LT(x, y);`  | `x < y`
`REQUIRE_LE(x, y);` | `CHECK_LE(x, y);`  | `x <= y`
`REQUIRE_GT(x, y);` | `CHECK_GT(x, y);`  | `x > y`
`REQUIRE_GE(x, y);` | `CHECK_GE(x, y);`  | `x >= y`

### c. String Comparisons
These macros compare two ***C-strings***. 

| Fatal assertion                | Nonfatal assertion             | Checks                                                 |
| --------------------------     | ------------------------------ | -------------------------------------------------------- |
| `REQUIRE_STREQ(str1,str2);`     | `CHECK_STREQ(str1,str2);`     | the two C strings have the same content   		     |
| `REQUIRE_STRNEQ(str1,str2);`     | `CHECK_STRNEQ(str1,str2);`     | the two C strings have different contents 		     |
| `REQUIRE_STRNNEQ(str1,str2);` | `CHECK_STRNNEQ(str1,str2);` | the two C strings have the same content, upto the length of str1   |


## Example Usage
Below is a slightly contrived example showing a number of possible supported operations:
```C
#include <Arrow/Arrow.h>
ARROW_MAIN() // sets up Arrow 

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
    REQUIRE_STREQ(a, b); // fail - Test suite aborted :(
}
```


## Supported Platforms
Arrow supports codebases and compilers that are compliant with the C11/C++11 standard or newer. Arrow's source code is officially supported on the following platforms. If you notice any problems on your platform, please file an issue on the [Arrow Github Issue Tracker](https://github/jasmcaus/Arrow/issues). PRs with fixes are welcome! 

Operating Systems          | Compilers       
-------------------------- | -------------------------- 
Linux                      | gcc 5.0+ 
macOS                      | clang 5.0+
Windows                    | MSVC 2017+

## License 
This project was written by [Jason Dsouza](https://github.com/jasmcaus) and licensed under the MIT License.