# Arrow
A Micro Unit Testing Framework for C11/C++14 onwards. It's tiny - about 1k lines of code. This framework is a *much* simpler and lighter alternative to Google Test, making it suitable for on-to-go testing. 

Arrow is part of [`CSTL`](https://github.com/jasmcaus/CSTL), a neater rewrite of the C/C++ Standard Library, although active development happens in the [Hazel Programming Language](https://github.com/HazelLang/Hazel) repository and changes are subsequently reflected in the [`CSTL`](https://github.com/jasmcaus/CSTL) repo.

## Installation
None! Arrow is header-only, so simply include it in your project
```c
#include <Arrow/Arrow.h>
```

## Basic Concepts
When using Arrow, you begin by writing `assertions`, which are statements that check if a condition is true. The result of an assertion is either *success*, *non-fatal failure* or a *fatal failure*. Unless the latter takes place, the program continues normally. 

In Arrow, you would normally define a ***Test Suite*** which contains multiple tests. These test suites should ideally reflect the structure of your tested code. 

## Prerequistes
To begin, you **must** include the following in *any* (but only one) C/C++ file. This initializes Arrow to set up all your tests:
```c
ARROW_MAIN();
```

## Defining a Test Suite
To define a test suite, simply do the following:
```c
TEST(TestSuiteName, TestName) {
    ARROW_CHECK(1); // fails if false
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
ARROW_CHECK(i == 42, "Expected i to be 32");
```

## A List of Avaliable Testing Macros
### Basic Assertions
These assertions perform basic true/false condition checking. 

Fatal assertion             | Nonfatal assertion         | Checks
--------------------------  | -------------------------- | --------------------
`ARROW_ASSERT(condition);`  | `ARROW_CHECK(condition);`  | `condition` is true
`ARROW_ASSERT(!condition);` | `ARROW_CHECK(!condition);` | `condition` is false

### Binary Comparisons
For a majority of your tests, `ARROW_ASSERT` and `ARROW_CHECK` will suffice. However, Arrow provides GTest-like Binary Comparisons. Both achieve the same purpose - we recommend `ARROW_ASSERT` and `ARROW_CHECK` as they provide readable comparison checks. 

For user-defined types in a C++ codebase, we recommend using these Binary Comparisons (they don't require you to overload the `==`, `<=`... operators).

Fatal assertion          | Nonfatal assertion       | Checks
------------------------ | ------------------------ | --------------
`ARROW_ASSERT_EQ(x, y);` | `ARROW_CHECK_EQ(x, y);`  | `x == y`
`ARROW_ASSERT_NE(x, y);` | `ARROW_CHECK_NE(x, y);`  | `x != y`
`ARROW_ASSERT_LT(x, y);` | `ARROW_CHECK_LT(x, y);`  | `x < y`
`ARROW_ASSERT_LE(x, y);` | `ARROW_CHECK_LE(x, y);`  | `x <= y`
`ARROW_ASSERT_GT(x, y);` | `ARROW_CHECK_GT(x, y);`  | `x > y`
`ARROW_ASSERT_GE(x, y);` | `ARROW_CHECK_GE(x, y);`  | `x >= y`

## String Comparisons
These macros compare two ***C-strings***. 

| Fatal assertion                | Nonfatal assertion             | Checks                                                 |
| --------------------------     | ------------------------------ | -------------------------------------------------------- |
| `ARROW_ASSERT_STREQ(str1,str2);`     | `ARROW_EXPECT_STREQ(str1,str2);`     | the two C strings have the same content   		     |
| `ARROW_ASSERT_STRNEQ(str1,str2);`     | `ARROW_EXPECT_STRNEQ(str1,str2);`     | the two C strings have different contents 		     |
| `ARROW_ASSERT_STRNNEQ(str1,str2);` | `ARROW_EXPECT_STRNNEQ(str1,str2);` | the two C strings have the same content, upto the length of str1   |



## Example Usage
Below is a slightly contrived example showing a number of possible supported operations:
```C
```

## Supported Platforms
Arrow supports codebases and compilers that are compliant with the C11/C++11 standard or newer. Arrow's source code is officially supported on the following platforms. If you notice any problems on your platform, please file an issue on the [Arrow Github Issue Tracker](https://github/jasmcaus/Arrow/issues). PRs with fixes are welcome! 

Operating Systems          | Compilers       
-------------------------- | -------------------------- 
Linux                      | gcc 5.0+ 
macOS                      | clang 5.0+
Windows                    | MSVC 2017+