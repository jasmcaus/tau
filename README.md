# Arrow
A Micro Unit Testing Framework for C11/C++14 onwards. It's tiny - about 1k lines of code. This framework is a *much* simpler and lighter alternative to Google Test, making it suitable for on-to-go testing. 

Arrow is part of [`CSTL`](https://github.com/jasmcaus/CSTL), a neater rewrite of the C/C++ Standard Library, although active development happens in the [Hazel Programming Language](https://github.com/HazelLang/Hazel) repository and changes are subsequently reflected in the [`CSTL`](https://github.com/jasmcaus/CSTL) repo.

## Installation
None! Arrow is header-only, so simply include it in your project
```c
#include <Arrow/Arrow.h>
```

## Example Usage
To begin, you **must** include the following in *any* (but only one) C/C++ file. This initializes Arrow to set up all your tests:
```c
ARROW_MAIN();
```

### Define a Test Case
To define a test case, simply do the following:
```c
TEST(foo, bar) {
    ARROW_CHECK(1); // fails if false
}
```
The `TEST` macro takes two parameters - the first is the name of the Test Suite, and the second is the name of the test. This allows tests to be grouped for convenience. 

### Testing Macros
Arrow provides two variants of Assertion Macros - `CHECK`s and `ASSERT`s. If an `ASSERT` fails, the test case will cease its execution and continue on to the next test case to run. If a `CHECK` fails, the remainder of the test case will still execute, allowing for further checks to run. 

Below is a slightly contrived example showing a number of possible supported operations: