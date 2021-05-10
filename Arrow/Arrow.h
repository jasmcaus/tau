#ifndef ARROW_H
#define ARROW_H

// ADD ASSERTS LATER
#define ARROW_CHECK(cond, ...)       arrow_check((cond), __FILE__, __LINE__, __VA_ARGS__)

// String Functions
// USE MACROS FOR THIS (UTEST.H)
#define ARROW_EXPECT_STREQ(x, y, ...)     arrow_strcmp_check((x), (y), ==,  __FILE__, __LINE__, __VA_ARGS__)
#define ARROW_EXPECT_STRNEQ(x, y, ...)     arrow_strcmp_check((x), (y), !=,  __FILE__, __LINE__, __VA_ARGS__)
#define ARROW_EXPECT_STRNNEQ(x, y, n, ...)     arrow_strncmp_check((x), (y), (n), !=, __FILE__, __LINE__, __VA_ARGS__)

#define ARROW_EXPECT_EQ(x, y)        ARROW_CHECK((x) == (y))
#define ARROW_EXPECT_NE(x, y)        ARROW_CHECK((x) != (y))
#define ARROW_EXPECT_LT(x, y)        ARROW_CHECK((x) < (y))
#define ARROW_EXPECT_LE(x, y)        ARROW_CHECK((x) <= (y))
#define ARROW_EXPECT_GT(x, y)        ARROW_CHECK((x) > (y))
#define ARROW_EXPECT_GE(x, y)        ARROW_CHECK((x) >= (y))

/**********************
 *** Implementation ***
 **********************/
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

/* Enable the use of the non-standard keyword __attribute__ to silence warnings under some compilers */
#if defined(__GNUC__) || defined(__clang__)
    #define ARROW_ATTRIBUTE_(attr)    __attribute__((attr))
#else
    #define ARROW_ATTRIBUTE_(attr)
#endif

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef _MSC_VER
    /* In the multi-platform code like ours, we cannot use the non-standard
     * "safe" functions from Microsoft C lib like e.g. sprintf_s() instead of
     * standard sprintf(). Hence, lets disable the warning C4996. */
    #pragma warning(push)
    #pragma warning(disable: 4996)
#endif

// Forward Declarations
bool arrow_check(bool cond, const char* file, int line, const char* fmt, ...);

bool ARROW_ATTRIBUTE_(format (printf, 4, 5))
arrow_check(bool cond, const char* file, int line, const char* fmt, ...) {
    const char* result_str;
    int result_colour;

    if(cond) {
        result_str = "PASSED";
        result_colour = ARROW_COLOUR_GREEN;
    } else {
        result_str = "FAILED";
        result_colour = ARROW_COLOUR_RED;
    }

    ARROW_PRINTF("  Expected : "); // LINE 1513 in UTest.hgit aa
}

#ifdef __cplusplus
    }  // extern "C"
#endif

#endif // ARROW_H