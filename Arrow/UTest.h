#ifndef SHEREDOM_ARROW_H_INCLUDED
#define SHEREDOM_ARROW_H_INCLUDED

#ifdef _MSC_VER
    // Disable warning about not inlining 'inline' functions.
    #pragma warning(disable : 4710)

    #ifdef _WIN32
        // Fixes the `warning C4668: '__cplusplus' is not defined as a preprocessor macro`
        #pragma warning (disable : 4668)

        // Disable 'conditional expression is constant'
        #pragma warning(disable : 4127)
    #endif // _WIN32

    /*
    Disable warning about inlining functions that are not marked 'inline'.
    */
    #pragma warning(disable : 4711)
    #pragma warning(push, 1)
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1920)
    typedef __int64 arrow_int64_t;
    typedef unsigned __int64 arrow_uint64_t;
#else
    #include <stdint.h>
    typedef int64_t arrow_int64_t;
    typedef uint64_t arrow_uint64_t;
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if defined(_MSC_VER)
    #pragma warning(pop)
#endif

#if defined(__cplusplus)
    #define ARROW_C_FUNC extern "C"
#else
    #define ARROW_C_FUNC
#endif

#if defined(_MSC_VER)
    // define ARROW_USE_OLD_QPC before #include "arrow.h" to use old QueryPerformanceCounter
    #ifndef ARROW_USE_OLD_QPC
        #pragma warning(push, 0)
        #include <Windows.h>
        #pragma warning(pop)

        typedef LARGE_INTEGER arrow_large_integer;
    #else 
    //use old QueryPerformanceCounter definitions (not sure is this needed in some edge cases or not)
    //on Win7 with VS2015 these extern declaration cause "second C linkage of overloaded function not allowed" error
    typedef union {
    struct {
        unsigned long LowPart;
        long HighPart;
    } DUMMYSTRUCTNAME;
    struct {
        unsigned long LowPart;
        long HighPart;
    } u;
    arrow_int64_t QuadPart;
    } arrow_large_integer;

    ARROW_C_FUNC __declspec(dllimport) int __stdcall QueryPerformanceCounter(
        arrow_large_integer *);
    ARROW_C_FUNC __declspec(dllimport) int __stdcall QueryPerformanceFrequency(
        arrow_large_integer *);
    #endif
#elif defined(__linux__)

/*
   slightly obscure include here - we need to include glibc's features.h, but
   we don't want to just include a header that might not be defined for other
   c libraries like musl. Instead we include limits.h, which we know on all
   glibc distributions includes features.h
*/
#include <limits.h>

#if defined(__GLIBC__) && defined(__GLIBC_MINOR__)
#include <time.h>

#if ((2 < __GLIBC__) || ((2 == __GLIBC__) && (17 <= __GLIBC_MINOR__)))
/* glibc is version 2.17 or above, so we can just use clock_gettime */
#define ARROW_USE_CLOCKGETTIME
#else
#include <sys/syscall.h>
#include <unistd.h>
#endif
#else // Other libc implementations
#include <time.h>
#define ARROW_USE_CLOCKGETTIME
#endif

#elif defined(__APPLE__)
#include <mach/mach_time.h>
#endif

#if defined(_MSC_VER) && (_MSC_VER < 1920)
#define ARROW_PRId64 "I64d"
#define ARROW_PRIu64 "I64u"
#else
#include <inttypes.h>

#define ARROW_PRId64 PRId64
#define ARROW_PRIu64 PRIu64
#endif

#if defined(_MSC_VER)
  #define ARROW_INLINE __forceinline

  #if defined(_WIN64)
    #define ARROW_SYMBOL_PREFIX
  #else
    #define ARROW_SYMBOL_PREFIX "_"
  #endif

  #if defined(__clang__)
    #define ARROW_INITIALIZER_BEGIN_DISABLE_WARNINGS                               \
      _Pragma("clang diagnostic push")                                             \
          _Pragma("clang diagnostic ignored \"-Wmissing-variable-declarations\"")

    #define ARROW_INITIALIZER_END_DISABLE_WARNINGS _Pragma("clang diagnostic pop")
  #else
    #define ARROW_INITIALIZER_BEGIN_DISABLE_WARNINGS
    #define ARROW_INITIALIZER_END_DISABLE_WARNINGS
  #endif

  #pragma section(".CRT$XCU", read)
  #define ARROW_INITIALIZER(f)                                                   \
    static void __cdecl f(void);                                                 \
    ARROW_INITIALIZER_BEGIN_DISABLE_WARNINGS                                     \
    __pragma(comment(linker, "/include:" ARROW_SYMBOL_PREFIX #f "_"))            \
        ARROW_C_FUNC __declspec(allocate(".CRT$XCU")) void(__cdecl *             \
                                                          f##_)(void) = f;      \
    ARROW_INITIALIZER_END_DISABLE_WARNINGS                                       \
    static void __cdecl f(void)
#else
  #if defined(__linux__)
    #if defined(__clang__)
      #if __has_warning("-Wreserved-id-macro")
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wreserved-id-macro"
      #endif
    #endif

    #define __STDC_FORMAT_MACROS 1

    #if defined(__clang__)
      #if __has_warning("-Wreserved-id-macro")
        #pragma clang diagnostic pop
      #endif
    #endif
  #endif

  #define ARROW_INLINE inline

  #define ARROW_INITIALIZER(f)                                                   \
    static void f(void) __attribute__((constructor));                            \
    static void f(void)
#endif

#if defined(__cplusplus)
#define ARROW_CAST(type, x) static_cast<type>(x)
#define ARROW_PTR_CAST(type, x) reinterpret_cast<type>(x)
#define ARROW_EXTERN extern "C"
#define ARROW_NULL NULL
#else
#define ARROW_CAST(type, x) ((type)x)
#define ARROW_PTR_CAST(type, x) ((type)x)
#define ARROW_EXTERN extern
#define ARROW_NULL 0
#endif

#ifdef _MSC_VER
/*
    io.h contains definitions for some structures with natural padding. This is
    uninteresting, but for some reason MSVC's behaviour is to warn about
    including this system header. That *is* interesting
*/
#pragma warning(disable : 4820)
#pragma warning(push, 1)
#include <io.h>
#pragma warning(pop)
#define ARROW_COLOUR_OUTPUT() (_isatty(_fileno(stdout)))
#else
#include <unistd.h>
#define ARROW_COLOUR_OUTPUT() (isatty(STDOUT_FILENO))
#endif

static ARROW_INLINE void *arrow_realloc(void *const pointer, size_t new_size) {
  void *const new_pointer = realloc(pointer, new_size);

  if (ARROW_NULL == new_pointer) {
    free(new_pointer);
  }

  return new_pointer;
}

static ARROW_INLINE arrow_int64_t arrow_ns(void) {
#ifdef _MSC_VER
  arrow_large_integer counter;
  arrow_large_integer frequency;
  QueryPerformanceCounter(&counter);
  QueryPerformanceFrequency(&frequency);
  return ARROW_CAST(arrow_int64_t,
                    (counter.QuadPart * 1000000000) / frequency.QuadPart);
#elif defined(__linux) && defined(__STRICT_ANSI__)
  return ARROW_CAST(arrow_int64_t, clock()) * 1000000000 / CLOCKS_PER_SEC;
#elif defined(__linux)
  struct timespec ts;
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
  timespec_get(&ts, TIME_UTC);
#else
  const clockid_t cid = CLOCK_REALTIME;
#if defined(ARROW_USE_CLOCKGETTIME)
  clock_gettime(cid, &ts);
#else
  syscall(SYS_clock_gettime, cid, &ts);
#endif
#endif
  return ARROW_CAST(arrow_int64_t, ts.tv_sec) * 1000 * 1000 * 1000 + ts.tv_nsec;
#elif __APPLE__
  return ARROW_CAST(arrow_int64_t, mach_absolute_time());
#endif
}

typedef void (*arrow_testcase_t)(int *, size_t);

struct arrow_test_state_s {
  arrow_testcase_t func;
  size_t index;
  char *name;
};

struct arrow_test_s {
  struct arrow_test_state_s *tests;
  size_t num_tests;
  FILE *output;
};

/* extern to the global state arrow needs to execute */
ARROW_EXTERN struct arrow_test_s arrow_test_;

#if defined(_MSC_VER)
#define ARROW_WEAK __forceinline
#else
#define ARROW_WEAK __attribute__((weak))
#endif

#if defined(_MSC_VER)
#define ARROW_UNUSED
#else
#define ARROW_UNUSED __attribute__((unused))
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wvariadic-macros"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif
#define ARROW_PRINTF(...)                                                      \
  if (arrow_test_.foutput) {                                                    \
    fprintf(arrow_test_.foutput, __VA_ARGS__);                                  \
  }                                                                            \
  printf(__VA_ARGS__)
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wvariadic-macros"
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif

#ifdef _MSC_VER
#define ARROW_SNPRINTF(BUFFER, N, ...) _snprintf_s(BUFFER, N, N, __VA_ARGS__)
#else
#define ARROW_SNPRINTF(...) snprintf(__VA_ARGS__)
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#if defined(__cplusplus)
/* if we are using c++ we can use overloaded methods (its in the language) */
#define ARROW_OVERLOADABLE
#elif defined(__clang__)
/* otherwise, if we are using clang with c - use the overloadable attribute */
#define ARROW_OVERLOADABLE __attribute__((overloadable))
#endif

#if defined(ARROW_OVERLOADABLE)
ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(float f);
ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(float f) {
  ARROW_PRINTF("%f", ARROW_CAST(double, f));
}

ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(double d);
ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(double d) {
  ARROW_PRINTF("%f", d);
}

ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long double d);
ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long double d) {
  ARROW_PRINTF("%Lf", d);
}

ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(int i);
ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(int i) {
  ARROW_PRINTF("%d", i);
}

ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(unsigned int i);
ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(unsigned int i) {
  ARROW_PRINTF("%u", i);
}

ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long int i);
ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long int i) {
  ARROW_PRINTF("%ld", i);
}

ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long unsigned int i);
ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long unsigned int i) {
  ARROW_PRINTF("%lu", i);
}

ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(const void *p);
ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(const void *p) {
  ARROW_PRINTF("%p", p);
}

/*
   long long is a c++11 extension
*/
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) ||              \
    defined(__cplusplus) && (__cplusplus >= 201103L)

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif

ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long long int i);
ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long long int i) {
  ARROW_PRINTF("%lld", i);
}

ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long long unsigned int i);
ARROW_WEAK ARROW_OVERLOADABLE void
arrow_type_printer(long long unsigned int i) {
  ARROW_PRINTF("%llu", i);
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
#define arrow_type_printer(val)                                                \
  ARROW_PRINTF(_Generic((val), signed char                                     \
                        : "%d", unsigned char                                  \
                        : "%u", short                                          \
                        : "%d", unsigned short                                 \
                        : "%u", int                                            \
                        : "%d", long                                           \
                        : "%ld", long long                                     \
                        : "%lld", unsigned                                     \
                        : "%u", unsigned long                                  \
                        : "%lu", unsigned long long                            \
                        : "%llu", float                                        \
                        : "%f", double                                         \
                        : "%f", long double                                    \
                        : "%Lf", default                                       \
                        : _Generic((val - val), ptrdiff_t                      \
                                   : "%p", default                             \
                                   : "undef")),                                \
               (val))
#else
/*
   we don't have the ability to print the values we got, so we create a macro
   to tell our users we can't do anything fancy
*/
#define arrow_type_printer(...) ARROW_PRINTF("undef")
#endif

#ifdef _MSC_VER
#define ARROW_SURPRESS_WARNING_BEGIN                                           \
  __pragma(warning(push)) __pragma(warning(disable : 4127))
#define ARROW_SURPRESS_WARNING_END __pragma(warning(pop))
#else
#define ARROW_SURPRESS_WARNING_BEGIN
#define ARROW_SURPRESS_WARNING_END
#endif

#if defined(__cplusplus) && (__cplusplus >= 201103L)
#define ARROW_AUTO(x) auto
#elif !defined(__cplusplus)

#if defined(__clang__)
/* clang-format off */
/* had to disable clang-format here because it malforms the pragmas */
#define ARROW_AUTO(x)                                                          \
  _Pragma("clang diagnostic push")                                             \
      _Pragma("clang diagnostic ignored \"-Wgnu-auto-type\"") __auto_type      \
          _Pragma("clang diagnostic pop")
/* clang-format on */
#else
#define ARROW_AUTO(x) __typeof__(x + 0)
#endif

#else
#define ARROW_AUTO(x) typeof(x + 0)
#endif

/* Macros for testing whether an unit test succeeds or fails. These macros
 * can be used arbitrarily in functions implementing the unit test
*/

#if defined(__clang__)
#define ARROW_STRNCMP(x, y, size)                                              \
  _Pragma("clang diagnostic push")                                             \
      _Pragma("clang diagnostic ignored \"-Wdisabled-macro-expansion\"")       \
          strncmp(x, y, size) _Pragma("clang diagnostic pop")
#else
#define ARROW_STRNCMP(x, y, size) strncmp(x, y, size)
#endif

//
#if defined(__clang__)
#define ARROW_EXPECT(x, y, cond)                                               \
  ARROW_SURPRESS_WARNING_BEGIN do {                                            \
    _Pragma("clang diagnostic push")                                           \
        _Pragma("clang diagnostic ignored \"-Wlanguage-extension-token\"")     \
            _Pragma("clang diagnostic ignored \"-Wc++98-compat-pedantic\"")    \
                _Pragma("clang diagnostic ignored \"-Wfloat-equal\"")          \
                    ARROW_AUTO(x) xEval = (x);                                 \
    ARROW_AUTO(y) yEval = (y);                                                 \
    if (!((xEval)cond(yEval))) {                                               \
      _Pragma("clang diagnostic pop")                                          \
          ARROW_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                \
      ARROW_PRINTF("  Expected : ");                                           \
      arrow_type_printer(xEval);                                               \
      ARROW_PRINTF("\n");                                                      \
      ARROW_PRINTF("    Actual : ");                                           \
      arrow_type_printer(yEval);                                               \
      ARROW_PRINTF("\n");                                                      \
      *arrow_result = 1;                                                       \
    }                                                                          \
  }                                                                            \
  while (0)                                                                    \
  ARROW_SURPRESS_WARNING_END

#endif 
// #define EXPECT_TRUE(x)                                                         \
//   ARROW_SURPRESS_WARNING_BEGIN do {                                            \
//     if (!(x)) {                                                                \
//       ARROW_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//       ARROW_PRINTF("  Expected : true\n");                                     \
//       ARROW_PRINTF("    Actual : %s\n", (x) ? "true" : "false");               \
//       *arrow_result = 1;                                                       \
//     }                                                                          \
//   }                                                                            \
//   while (0)                                                                    \
//   ARROW_SURPRESS_WARNING_END

// #define EXPECT_FALSE(x)                                                        \
//   ARROW_SURPRESS_WARNING_BEGIN do {                                            \
//     if (x) {                                                                   \
//       ARROW_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//       ARROW_PRINTF("  Expected : false\n");                                    \
//       ARROW_PRINTF("    Actual : %s\n", (x) ? "true" : "false");               \
//       *arrow_result = 1;                                                       \
//     }                                                                          \
//   }                                                                            \
//   while (0)                                                                    \
//   ARROW_SURPRESS_WARNING_END

// #define EXPECT_EQ(x, y) ARROW_EXPECT(x, y, ==)
// #define EXPECT_NE(x, y) ARROW_EXPECT(x, y, !=)
// #define EXPECT_LT(x, y) ARROW_EXPECT(x, y, <)
// #define EXPECT_LE(x, y) ARROW_EXPECT(x, y, <=)
// #define EXPECT_GT(x, y) ARROW_EXPECT(x, y, >)
// #define EXPECT_GE(x, y) ARROW_EXPECT(x, y, >=)

// #define EXPECT_STREQ(x, y)                                                     \
//   ARROW_SURPRESS_WARNING_BEGIN do {                                            \
//     if (0 != strcmp(x, y)) {                                                   \
//       ARROW_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//       ARROW_PRINTF("  Expected : \"%s\"\n", x);                                \
//       ARROW_PRINTF("    Actual : \"%s\"\n", y);                                \
//       *arrow_result = 1;                                                       \
//     }                                                                          \
//   }                                                                            \
//   while (0)                                                                    \
//   ARROW_SURPRESS_WARNING_END

// #define EXPECT_STRNE(x, y)                                                     \
//   ARROW_SURPRESS_WARNING_BEGIN do {                                            \
//     if (0 == strcmp(x, y)) {                                                   \
//       ARROW_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//       ARROW_PRINTF("  Expected : \"%s\"\n", x);                                \
//       ARROW_PRINTF("    Actual : \"%s\"\n", y);                                \
//       *arrow_result = 1;                                                       \
//     }                                                                          \
//   }                                                                            \
//   while (0)                                                                    \
//   ARROW_SURPRESS_WARNING_END

// #define EXPECT_STRNEQ(x, y, n)                                                 \
//   ARROW_SURPRESS_WARNING_BEGIN do {                                            \
//     if (0 != ARROW_STRNCMP(x, y, n)) {                                         \
//       ARROW_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//       ARROW_PRINTF("  Expected : \"%.*s\"\n", ARROW_CAST(int, n), x);          \
//       ARROW_PRINTF("    Actual : \"%.*s\"\n", ARROW_CAST(int, n), y);          \
//       *arrow_result = 1;                                                       \
//     }                                                                          \
//   }                                                                            \
//   while (0)                                                                    \
//   ARROW_SURPRESS_WARNING_END

// #define EXPECT_STRNNE(x, y, n)                                                 \
//   ARROW_SURPRESS_WARNING_BEGIN do {                                            \
//     if (0 == ARROW_STRNCMP(x, y, n)) {                                         \
//       ARROW_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//       ARROW_PRINTF("  Expected : \"%.*s\"\n", ARROW_CAST(int, n), x);          \
//       ARROW_PRINTF("    Actual : \"%.*s\"\n", ARROW_CAST(int, n), y);          \
//       *arrow_result = 1;                                                       \
//     }                                                                          \
//   }                                                                            \
//   while (0)                                                                    \
//   ARROW_SURPRESS_WARNING_END

// #if defined(__clang__)
// #define ARROW_ASSERT(x, y, cond)                                               \
//   ARROW_SURPRESS_WARNING_BEGIN do {                                            \
//     _Pragma("clang diagnostic push")                                           \
//         _Pragma("clang diagnostic ignored \"-Wlanguage-extension-token\"")     \
//             _Pragma("clang diagnostic ignored \"-Wc++98-compat-pedantic\"")    \
//                 _Pragma("clang diagnostic ignored \"-Wfloat-equal\"")          \
//                     ARROW_AUTO(x) xEval = (x);                                 \
//     ARROW_AUTO(y) yEval = (y);                                                 \
//     if (!((xEval)cond(yEval))) {                                               \
//       _Pragma("clang diagnostic pop")                                          \
//           ARROW_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                \
//       ARROW_PRINTF("  Expected : ");                                           \
//       arrow_type_printer(xEval);                                               \
//       ARROW_PRINTF("\n");                                                      \
//       ARROW_PRINTF("    Actual : ");                                           \
//       arrow_type_printer(yEval);                                               \
//       ARROW_PRINTF("\n");                                                      \
//       *arrow_result = 1;                                                       \
//       return;                                                                  \
//     }                                                                          \
//   }                                                                            \
//   while (0)                                                                    \
//   ARROW_SURPRESS_WARNING_END
// #elif defined(__GNUC__)
// #define ARROW_ASSERT(x, y, cond)                                               \
//   ARROW_SURPRESS_WARNING_BEGIN do {                                            \
//     ARROW_AUTO(x) xEval = (x);                                                 \
//     ARROW_AUTO(y) yEval = (y);                                                 \
//     if (!((xEval)cond(yEval))) {                                               \
//       ARROW_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//       ARROW_PRINTF("  Expected : ");                                           \
//       arrow_type_printer(xEval);                                               \
//       ARROW_PRINTF("\n");                                                      \
//       ARROW_PRINTF("    Actual : ");                                           \
//       arrow_type_printer(yEval);                                               \
//       ARROW_PRINTF("\n");                                                      \
//       *arrow_result = 1;                                                       \
//       return;                                                                  \
//     }                                                                          \
//   }                                                                            \
//   while (0)                                                                    \
//   ARROW_SURPRESS_WARNING_END
// #else
// #define ARROW_ASSERT(x, y, cond)                                               \
//   ARROW_SURPRESS_WARNING_BEGIN do {                                            \
//     if (!((x)cond(y))) {                                                       \
//       ARROW_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//       *arrow_result = 1;                                                       \
//       return;                                                                  \
//     }                                                                          \
//   }                                                                            \
//   while (0)                                                                    \
//   ARROW_SURPRESS_WARNING_END
// #endif

// #define ASSERT_TRUE(x)                                                         \
//   ARROW_SURPRESS_WARNING_BEGIN do {                                            \
//     if (!(x)) {                                                                \
//       ARROW_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//       ARROW_PRINTF("  Expected : true\n");                                     \
//       ARROW_PRINTF("    Actual : %s\n", (x) ? "true" : "false");               \
//       *arrow_result = 1;                                                       \
//       return;                                                                  \
//     }                                                                          \
//   }                                                                            \
//   while (0)                                                                    \
//   ARROW_SURPRESS_WARNING_END

// #define ASSERT_FALSE(x)                                                        \
//   ARROW_SURPRESS_WARNING_BEGIN do {                                            \
//     if (x) {                                                                   \
//       ARROW_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//       ARROW_PRINTF("  Expected : false\n");                                    \
//       ARROW_PRINTF("    Actual : %s\n", (x) ? "true" : "false");               \
//       *arrow_result = 1;                                                       \
//       return;                                                                  \
//     }                                                                          \
//   }                                                                            \
//   while (0)                                                                    \
//   ARROW_SURPRESS_WARNING_END

// #define ASSERT_EQ(x, y) ARROW_ASSERT(x, y, ==)
// #define ASSERT_NE(x, y) ARROW_ASSERT(x, y, !=)
// #define ASSERT_LT(x, y) ARROW_ASSERT(x, y, <)
// #define ASSERT_LE(x, y) ARROW_ASSERT(x, y, <=)
// #define ASSERT_GT(x, y) ARROW_ASSERT(x, y, >)
// #define ASSERT_GE(x, y) ARROW_ASSERT(x, y, >=)

// #define ASSERT_STREQ(x, y)                                                     \
//   ARROW_SURPRESS_WARNING_BEGIN do {                                            \
//     if (0 != strcmp(x, y)) {                                                   \
//       ARROW_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//       ARROW_PRINTF("  Expected : \"%s\"\n", x);                                \
//       ARROW_PRINTF("    Actual : \"%s\"\n", y);                                \
//       *arrow_result = 1;                                                       \
//       return;                                                                  \
//     }                                                                          \
//   }                                                                            \
//   while (0)                                                                    \
//   ARROW_SURPRESS_WARNING_END

// #define ASSERT_STRNE(x, y)                                                     \
//   ARROW_SURPRESS_WARNING_BEGIN do {                                            \
//     if (0 == strcmp(x, y)) {                                                   \
//       ARROW_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//       ARROW_PRINTF("  Expected : \"%s\"\n", x);                                \
//       ARROW_PRINTF("    Actual : \"%s\"\n", y);                                \
//       *arrow_result = 1;                                                       \
//       return;                                                                  \
//     }                                                                          \
//   }                                                                            \
//   while (0)                                                                    \
//   ARROW_SURPRESS_WARNING_END

// #define ASSERT_STRNEQ(x, y, n)                                                 \
//   ARROW_SURPRESS_WARNING_BEGIN do {                                            \
//     if (0 != ARROW_STRNCMP(x, y, n)) {                                         \
//       ARROW_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//       ARROW_PRINTF("  Expected : \"%.*s\"\n", ARROW_CAST(int, n), x);          \
//       ARROW_PRINTF("    Actual : \"%.*s\"\n", ARROW_CAST(int, n), y);          \
//       *arrow_result = 1;                                                       \
//       return;                                                                  \
//     }                                                                          \
//   }                                                                            \
//   while (0)                                                                    \
//   ARROW_SURPRESS_WARNING_END

// #define ASSERT_STRNNE(x, y, n)                                                 \
//   ARROW_SURPRESS_WARNING_BEGIN do {                                            \
//     if (0 == ARROW_STRNCMP(x, y, n)) {                                         \
//       ARROW_PRINTF("%s:%u: Failure\n", __FILE__, __LINE__);                    \
//       ARROW_PRINTF("  Expected : \"%.*s\"\n", ARROW_CAST(int, n), x);          \
//       ARROW_PRINTF("    Actual : \"%.*s\"\n", ARROW_CAST(int, n), y);          \
//       *arrow_result = 1;                                                       \
//       return;                                                                  \
//     }                                                                          \
//   }                                                                            \
//   while (0)                                                                    \
//   ARROW_SURPRESS_WARNING_END

#define TEST(SET, NAME)                                                       \
  ARROW_EXTERN struct arrow_test_s arrow_test_;                               \
  static void arrow_run_##SET##_##NAME(int *arrow_result);                     \
  static void arrow_##SET##_##NAME(int *arrow_result, size_t arrow_index) {    \
    (void)arrow_index;                                                         \
    arrow_run_##SET##_##NAME(arrow_result);                                    \
  }                                                                            \
  ARROW_INITIALIZER(arrow_register_##SET##_##NAME) {                           \
    const size_t index = arrow_test_.num_tests++;                           \
    const char *name_part = #SET "." #NAME;                                    \
    const size_t name_size = strlen(name_part) + 1;                            \
    char *name = ARROW_PTR_CAST(char *, malloc(name_size));                    \
    arrow_test_.tests = ARROW_PTR_CAST(                                        \
        struct arrow_test_state_s *,                                           \
        arrow_realloc(ARROW_PTR_CAST(void *, arrow_test_.tests),               \
                      sizeof(struct arrow_test_state_s) *                      \
                          arrow_test_.num_tests));                          \
    arrow_test_.tests[index].func = &arrow_##SET##_##NAME;                     \
    arrow_test_.tests[index].name = name;                                      \
    arrow_test_.tests[index].index = 0;                                        \
    ARROW_SNPRINTF(name, name_size, "%s", name_part);                          \
  }                                                                            \
  void arrow_run_##SET##_##NAME(int *arrow_result)


ARROW_WEAK
int arrow_should_filter_test(const char *filter, const char *testcase);
ARROW_WEAK int arrow_should_filter_test(const char *filter,
                                        const char *testcase) {
  if (filter) {
    const char *filter_cur = filter;
    const char *testcase_cur = testcase;
    const char *filter_wildcard = ARROW_NULL;

    while (('\0' != *filter_cur) && ('\0' != *testcase_cur)) {
      if ('*' == *filter_cur) {
        /* store the position of the wildcard */
        filter_wildcard = filter_cur;

        /* skip the wildcard character */
        filter_cur++;

        while (('\0' != *filter_cur) && ('\0' != *testcase_cur)) {
          if ('*' == *filter_cur) {
            /*
               we found another wildcard (filter is something like *foo*) so we
               exit the current loop, and return to the parent loop to handle
               the wildcard case
            */
            break;
          } else if (*filter_cur != *testcase_cur) {
            /* otherwise our filter didn't match, so reset it */
            filter_cur = filter_wildcard;
          }

          /* move testcase along */
          testcase_cur++;

          /* move filter along */
          filter_cur++;
        }

        if (('\0' == *filter_cur) && ('\0' == *testcase_cur)) {
          return 0;
        }

        /* if the testcase has been exhausted, we don't have a match! */
        if ('\0' == *testcase_cur) {
          return 1;
        }
      } else {
        if (*testcase_cur != *filter_cur) {
          /* test case doesn't match filter */
          return 1;
        } else {
          /* move our filter and testcase forward */
          testcase_cur++;
          filter_cur++;
        }
      }
    }

    if (('\0' != *filter_cur) ||
        (('\0' != *testcase_cur) &&
         ((filter == filter_cur) || ('*' != filter_cur[-1])))) {
      /* we have a mismatch! */
      return 1;
    }
  }

  return 0;
}

static ARROW_INLINE FILE *arrow_fopen(const char *filename, const char *mode) {
#ifdef _MSC_VER
  FILE *file;
  if (0 == fopen_s(&file, filename, mode)) {
    return file;
  } else {
    return ARROW_NULL;
  }
#else
  return fopen(filename, mode);
#endif
}

static ARROW_INLINE int arrow_main(int argc, const char *const argv[]);
int arrow_main(int argc, const char *const argv[]) {
  size_t index = 0;
  size_t *failed_testcases = ARROW_NULL;
  size_t failed_testcases_length = 0;
  const char *filter = ARROW_NULL;
  arrow_uint64_t ran_tests = 0;


  /* loop through all arguments looking for our options */
  for (index = 1; index < ARROW_CAST(size_t, argc); index++) {
    /* Informational switches */
    const char help_str[] = "--help";
    const char list_str[] = "--list-tests";
    /* Test config switches */
    const char filter_str[] = "--filter=";
    const char output_str[] = "--output=";

    if (0 == ARROW_STRNCMP(argv[index], help_str, strlen(help_str))) {
      printf("arrow.h - the single file unit testing solution for C/C++!\n"
             "Command line Options:\n"
             "  --help            Show this message and exit.\n"
             "  --filter=<filter> Filter the test cases to run (EG. MyTest*.a "
             "would run MyTestCase.a but not MyTestCase.b).\n"
             "  --list-tests      List testnames, one per line. Output names "
             "can be passed to --filter.\n"
             "  --output=<output> Output an xunit XML file to the file "
             "specified in <output>.\n");
      goto cleanup;
    } else if (0 ==
               ARROW_STRNCMP(argv[index], filter_str, strlen(filter_str))) {
      /* user wants to filter what test cases run! */
      filter = argv[index] + strlen(filter_str);
    } else if (0 ==
               ARROW_STRNCMP(argv[index], output_str, strlen(output_str))) {
      arrow_test_.foutput = arrow_fopen(argv[index] + strlen(output_str), "w+");
    } else if (0 == ARROW_STRNCMP(argv[index], list_str, strlen(list_str))) {
      for (index = 0; index < arrow_test_.num_tests; index++) {
        ARROW_PRINTF("%s\n", arrow_test_.tests[index].name);
      }
      /* when printing the test list, don't actually run the tests */
      return 0;
    }
  }

  for (index = 0; index < arrow_test_.num_tests; index++) {
    if (arrow_should_filter_test(filter, arrow_test_.tests[index].name)) {
      continue;
    }

    ran_tests++;
  }

  printf("%s[==========]%s Running %" ARROW_PRIu64 " test cases.\n",
         colours[GREEN], colours[RESET], ARROW_CAST(arrow_uint64_t, ran_tests));

  if (arrow_test_.foutput) {
    fprintf(arrow_test_.foutput, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(arrow_test_.foutput,
            "<testsuites tests=\"%" ARROW_PRIu64 "\" name=\"All\">\n",
            ARROW_CAST(arrow_uint64_t, ran_tests));
    fprintf(arrow_test_.foutput,
            "<testsuite name=\"Tests\" tests=\"%" ARROW_PRIu64 "\">\n",
            ARROW_CAST(arrow_uint64_t, ran_tests));
  }

  // Loop through and run each test
  for (index = 0; index < arrow_test_.num_tests; index++) {
    int result = 0;
    arrow_int64_t ns = 0;

    if (arrow_should_filter_test(filter, arrow_test_.tests[index].name)) {
      continue;
    }

    printf("%s[ RUN      ]%s %s\n", colours[GREEN], colours[RESET],
           arrow_test_.tests[index].name);

    if (arrow_test_.foutput) {
      fprintf(arrow_test_.foutput, "<testcase name=\"%s\">",
              arrow_test_.tests[index].name);
    }

    ns = arrow_ns();
    errno = 0;
    arrow_test_.tests[index].func(&result, arrow_test_.tests[index].index);
    ns = arrow_ns() - ns;

    if (arrow_test_.foutput) {
      fprintf(arrow_test_.foutput, "</testcase>\n");
    }

    if (0 != result) {
      const size_t failed_testcase_index = failed_testcases_length++;
      failed_testcases = ARROW_PTR_CAST(
          size_t *, arrow_realloc(ARROW_PTR_CAST(void *, failed_testcases),
                                  sizeof(size_t) * failed_testcases_length));
      failed_testcases[failed_testcase_index] = index;
      failed++;
      printf("%s[  FAILED  ]%s %s (%" ARROW_PRId64 "ns)\n", colours[RED],
             colours[RESET], arrow_test_.tests[index].name, ns);
    } else {
      printf("%s[       OK ]%s %s (%" ARROW_PRId64 "ns)\n", colours[GREEN],
             colours[RESET], arrow_test_.tests[index].name, ns);
    }
  }

  printf("%s[==========]%s %" ARROW_PRIu64 " test cases ran.\n", colours[GREEN],
         colours[RESET], ran_tests);
  printf("%s[  PASSED  ]%s %" ARROW_PRIu64 " tests.\n", colours[GREEN],
         colours[RESET], ran_tests - failed);

  if (0 != failed) {
    printf("%s[  FAILED  ]%s %" ARROW_PRIu64 " tests, listed below:\n",
           colours[RED], colours[RESET], failed);
    for (index = 0; index < failed_testcases_length; index++) {
      printf("%s[  FAILED  ]%s %s\n", colours[RED], colours[RESET],
             arrow_test_.tests[failed_testcases[index]].name);
    }
  }

  if (arrow_test_.foutput) {
    fprintf(arrow_test_.foutput, "</testsuite>\n</testsuites>\n");
  }

cleanup:
  for (index = 0; index < arrow_test_.num_tests; index++) {
    free(ARROW_PTR_CAST(void *, arrow_test_.tests[index].name));
  }

  free(ARROW_PTR_CAST(void *, failed_testcases));
  free(ARROW_PTR_CAST(void *, arrow_test_.tests));

  if (arrow_test_.foutput) {
    fclose(arrow_test_.foutput);
  }

  return ARROW_CAST(int, failed);
}

/*
   we need, in exactly one source file, define the global struct that will hold
   the data we need to run arrow. This macro allows the user to declare the
   data without having to use the ARROW_MAIN macro, thus allowing them to write
   their own main() function.
*/
#define ARROW_STATE() struct arrow_test_s arrow_test_ = {0, 0, 0}

/*
   define a main() function to call into arrow.h and start executing tests! A
   user can optionally not use this macro, and instead define their own main()
   function and manually call arrow_main. The user must, in exactly one source
   file, use the ARROW_STATE macro to declare a global struct variable that
   arrow requires.
*/
#define ARROW_MAIN()                                                           \
  ARROW_STATE();                                                               \
  int main(int argc, const char *const argv[]) {                               \
    return arrow_main(argc, argv);                                             \
  }

#endif /* SHEREDOM_ARROW_H_INCLUDED */