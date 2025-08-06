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

## Defining Tests with setup and teardown functions

First define a test fixture:

```c
struct ExampleTestFixture {
   // Define variables needed for this fixture.
   // This test fixture may be empty.
   // In this example we have one single variable:
   int test_variable;
};
```

Then define the function that should be called before each test:

```c
TEST_F_SETUP(ExampleTestFixture)
{
    // This function is called before each test that is part of
    // `ExampleTestFixture`.
    // A parameter named `tau` which is a pointer to a
    // `ExampleTestFixture` object, is available for us.

    // When this function is called it has already been cleared.
    // We can prove this by adding this test:
    CHECK(tau->test_variable == 0, "Weird, fixture was not cleared before setup");

    // Now we can do whatever is needed to setup each test.
    // An example could be to open a file or configure something.

    // In this example, we just assign a `test_variable` a meaningful number.
    tau->test_variable = 42;
}
```

In a similar fashion we can also define a teardown function:

```c
TEST_F_TEARDOWN(ExampleTestFixture)
{
    // This function is called after each test that is part of
    // `ExampleTestFixture`.
    // A parameter named `tau` which is a const pointer to a
    // `ExampleTestFixture` object, is available also for
    // this function.
    // For example this function can return resources acquired by
    // the setup function.
    CHECK(tau != NULL);
}
```

Now we can define two tests for this fixture.

```c
TEST_F(ExampleTestFixture, EnsureSetupRanBefore)
{
    // Let's check that the framework called our setup function before.
    CHECK(tau->test_variable == 42);

    tau->test_variable += 24;
    CHECK(tau->test_variable == 42 + 24);
}

TEST_F(ExampleTestFixture, EnsureSetupRanBeforeAgain)
{
    // Let's check that the framework called our setup also this time.
    CHECK(tau->test_variable == 42);
    tau->test_variable++;
}
```

## Ignoring Tests

Sometimes you may want to temporarily disable a test without removing it from the codebase. Tau provides `IGNORE_TEST` and `IGNORE_TEST_F` macros for this purpose.

### Ignoring Regular Tests
```c
IGNORE_TEST(TestSuiteName, TestName) {
    // This test will be skipped during execution
    CHECK(1 == 2, "This would normally fail but won't run");
}
```

### Ignoring Fixture Tests
```c
IGNORE_TEST_F(ExampleTestFixture, IgnoredFixtureTest) {
    // This fixture test will be skipped during execution
    CHECK(tau->test_variable == 100, "This won't run");
}
```

Ignored tests will be displayed with an `[ IGNORED  ]` status when the test suite runs, allowing you to see which tests are currently disabled.

## Testing Macros
Tau provides two variants of Assertion Macros - `CHECK`s and `ASSERT`s. These resemble function calls. When these assertions fail, Tau prints the source code location (file + line number) along with a failure message. 

`ASSERT`s generate *fatal* failures - the test case will cease its execution and move on to the next test case to run. 
`CHECK`s generate *non-fatal* failures - the remainder of the test case will still execute, allowing for further checks to run. 

We recommend using `CHECK`s over `ASSERT`s unless it doesn't make sense to continue when the assertion in question fails. 

### Adding Custom Failure Messages
We highly recommend you add a custom failure message for your macros - it makes it easier to track down bugs. `Invalid Type ID:` is much more useful than `FAILED`, which is what Tau prints by default.

The message can be a printf format string. This is pretty useful when you do the test in a loop. You can quickly know which iteration of the loop failed.

To do this, simply do the following:
```C
CHECK(i == 42, "Expected i to be 42");

CHECK(result == expected[i], "i=%d", i);
```


## A List of Available Testing Macros

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

### d. Buffer Comparisons
These macros compare memory buffers byte-by-byte using `memcmp`. They are useful for testing binary data, arrays, or structures.

| Fatal assertion                | Nonfatal assertion             | Checks                                                 |
| --------------------------     | ------------------------------ | -------------------------------------------------------- |
| `REQUIRE_BUF_EQ(buf1,buf2,n);` | `CHECK_BUF_EQ(buf1,buf2,n);`  | the two memory buffers have identical content for n bytes |
| `REQUIRE_BUF_NE(buf1,buf2,n);` | `CHECK_BUF_NE(buf1,buf2,n);`  | the two memory buffers have different content within n bytes |

The `n` parameter specifies how many bytes to compare. When a buffer comparison fails, Tau displays the contents in hexadecimal format, highlighting the differences in yellow for easy identification.

### e. Pointer Comparisons
These macros compare pointer values directly (not the content they point to).

| Fatal assertion                | Nonfatal assertion             | Checks                                                 |
| --------------------------     | ------------------------------ | -------------------------------------------------------- |
| `REQUIRE_PTR_EQ(ptr1,ptr2);`   | `CHECK_PTR_EQ(ptr1,ptr2);`     | the two pointers point to the same memory address     |
| `REQUIRE_PTR_NE(ptr1,ptr2);`   | `CHECK_PTR_NE(ptr1,ptr2);`     | the two pointers point to different memory addresses  |

These are particularly useful for testing pointer assignments, linked data structures, or verifying that functions return the expected pointer values.

### f. NULL Checks
Convenient macros for checking null pointer values.

| Fatal assertion                | Nonfatal assertion             | Checks                                                 |
| --------------------------     | ------------------------------ | -------------------------------------------------------- |
| `REQUIRE_NULL(ptr);`           | `CHECK_NULL(ptr);`             | the pointer is NULL                                    |
| `REQUIRE_NOT_NULL(ptr);`       | `CHECK_NOT_NULL(ptr);`         | the pointer is not NULL                                |


## Example Usage
Below is a comprehensive example showing various supported operations:

```C
#include <tau/tau.h>
#include <string.h>

TAU_MAIN() // sets up Tau 

TEST(BasicTests, ArithmeticOperations) {
    int a = 42; 
    int b = 13; 
    CHECK_GE(a, b); // pass :)
    CHECK_LE(b, 8); // fail - Test suite not aborted 
}

TEST(StringTests, StringComparisons) {
    char* a = "foo";
    char* b = "foobar";
    char* c = "foo";
    
    REQUIRE_STREQ(a, c); // pass :) - same content
    REQUIRE_STREQ(a, b); // fail - Test suite aborted :(
    CHECK_SUBSTREQ(b, a, 3, "First 3 chars should match"); // pass :)
}

TEST(BufferTests, MemoryComparisons) {
    unsigned char buffer1[] = {0x01, 0x02, 0x03, 0x04};
    unsigned char buffer2[] = {0x01, 0x02, 0x03, 0x04};
    unsigned char buffer3[] = {0x01, 0x02, 0xFF, 0x04};
    
    CHECK_BUF_EQ(buffer1, buffer2, 4, "Buffers should be identical");
    CHECK_BUF_NE(buffer1, buffer3, 4, "Buffers should differ at byte 2");
}

TEST(PointerTests, PointerComparisons) {
    int value = 100;
    int* ptr1 = &value;
    int* ptr2 = &value;
    int* ptr3 = NULL;
    
    CHECK_PTR_EQ(ptr1, ptr2, "Both pointers should point to same address");
    CHECK_PTR_NE(ptr1, ptr3, "Pointer should not be NULL");
    CHECK_NULL(ptr3, "This pointer should be NULL");
    CHECK_NOT_NULL(ptr1, "This pointer should not be NULL");
}

// This test will be ignored during execution
IGNORE_TEST(IgnoredTests, TemporarilyDisabled) {
    CHECK(1 == 2, "This would fail but won't run");
}

// Example with test fixture
struct FileTestFixture {
    FILE* test_file;
    char* file_content;
};

TEST_F_SETUP(FileTestFixture) {
    tau->test_file = fopen("test.txt", "w+");
    tau->file_content = malloc(256);
    strcpy(tau->file_content, "Hello, Tau!");
    CHECK_NOT_NULL(tau->test_file, "File should be opened successfully");
}

TEST_F_TEARDOWN(FileTestFixture) {
    if (tau->test_file) {
        fclose(tau->test_file);
    }
    free(tau->file_content);
}

TEST_F(FileTestFixture, FileOperations) {
    // Write to file
    fprintf(tau->test_file, "%s", tau->file_content);
    fflush(tau->test_file);
    
    // Read back and compare
    rewind(tau->test_file);
    char read_buffer[256] = {0};
    fread(read_buffer, 1, strlen(tau->file_content), tau->test_file);
    
    CHECK_STREQ(read_buffer, tau->file_content, "File content should match");
}

// This fixture test will be ignored
IGNORE_TEST_F(FileTestFixture, IgnoredFileTest) {
    // This test won't run but setup/teardown structure is preserved
    CHECK_STREQ(tau->file_content, "Different content", "This won't execute");
}
```

This example demonstrates:
- Basic arithmetic checks with custom failure messages
- String comparison operations (whole strings and substrings)
- Binary buffer comparisons with hexadecimal output on failure
- Pointer comparisons and NULL checks
- Test fixtures with setup and teardown functions
- How to ignore tests temporarily using `IGNORE_TEST` and `IGNORE_TEST_F`
- Mixed use of `CHECK` (non-fatal) and `REQUIRE` (fatal) assertions