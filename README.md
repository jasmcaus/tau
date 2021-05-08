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

We highly recommend you add a custom failure message for your macros - it makes it easier to track down bugs. `Invalid Type ID:` is much more useful than `FAILED`, which is what Arrow prints by default.

Below is a slightly contrived example showing a number of possible supported operations: