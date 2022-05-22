## Basic Concepts
When using Tau, you begin by writing `assertions`, which are statements that check if a condition is true. The result of an assertion is either *success*, *non-fatal failure* or a *fatal failure*. Unless the latter takes place, the program continues normally. 

In Tau, you would normally define a ***Test Suite*** which contains multiple tests. These test suites should ideally reflect the structure of your tested code. 


## Prerequistes
To begin, you **must** include the following in *any* (but only one) C/C++ file. This initializes Tau to set up all your tests:
```c
TAU_MAIN() // IMPORTANT: No semicolon at the end 
```
This defines a main function, so if you write a main function ***and*** declare `TAU_MAIN()`, your compiler will throw a `redeclaration of main` error.


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
Tau provides two variants of Assertion Macros - `CHECK`s and `ASSERT`s. These resemble function calls. When these assertions fail, Tau prints the source code location (file + line number) along with a failure message. 

`ASSERT`s generate *fatal* failures - the test case will cease its execution and move on to the next test case to run. 
`CHECK`s generate *non-fatal* failures - the remainder of the test case will still execute, allowing for further checks to run. 

We recommend using `CHECK`s over `ASSERT`s unless it doesn't make sense to continue when the assertion in question fails. 

### Adding Custom Failure Messages
We highly recommend you add a custom failure message for your macros - it makes it easier to track down bugs. `Invalid Type ID:` is much more useful than `FAILED`, which is what Tau prints by default.

To do this, simply do the following:
```C
CHECK(i == 42, "Expected i to be 42");
```


## A List of Avaliable Testing Macros
### a. Basic Assertions
These assertions perform basic true/false condition checking. 

Fatal assertion             | Nonfatal assertion         | Checks
--------------------------  | -------------------------- | --------------------
`REQUIRE(condition);`  | `CHECK(condition);`  | `condition` is true
`REQUIRE(!condition);` | `CHECK(!condition);` | `condition` is false

### b. Binary Comparisons
For a majority of your tests, `REQUIRE` and `CHECK` will suffice. However, Tau provides GTest-like Binary Comparisons. Both achieve the same purpose - we recommend `REQUIRE` and `CHECK` as they provide readable comparison checks. 

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
| `REQUIRE_STREQ(str1,str2);`    | `CHECK_STREQ(str1,str2);`     | the two C strings have the same content   		     |
| `REQUIRE_STRNE(str1,str2);`   | `CHECK_STRNE(str1,str2);`    | the two C strings have different contents 		     |
| `REQUIRE_SUBSTREQ(str1,str2);`    | `CHECK_SUBSTREQ(str1,str2);`     | the two C strings have the same contents, upto the length of str1   |
| `REQUIRE_SUBSTRNE(str1,str2);`   | `CHECK_SUBSTRNE(str1,str2);`    | the two C strings have different content, upto the length of str1   |


## Example Usage
Below is a slightly contrived example showing a number of possible supported operations:
```C
#include <tau/tau.h>
TAU_MAIN() // sets up Tau 

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
