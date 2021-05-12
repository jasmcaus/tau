#ifndef SHEREDOM_ARROW_H_INCLUDED
#define SHEREDOM_ARROW_H_INCLUDED

#include <Arrow/Types.h>
#include <Arrow/Misc.h>

#ifdef _MSC_VER
    // Disable warning about not inlining 'inline' functions.
    #pragma warning(disable : 4710)
    
    // Disable warning about inlining functions that are not marked 'inline'.
    #pragma warning(disable : 4711)

    // No function prototype given: converting '()' to '(void)'
    #pragma warning(disable : 4255)

    // '__cplusplus' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
    #pragma warning(disable : 4668)

    // In multi-platform code like ours, we cannot use the non-standard "safe" functions from 
    // Microsoft's C lib like e.g. sprintf_s() instead of standard sprintf().
    #pragma warning(disable: 4996)

    // warning C4127: conditional expression is constant  
    // __pragma(warning(disable : 4127))
    #pragma warning(disable : 4127)

    // io.h contains definitions for some structures with natural padding. This is uninteresting, but for some reason, 
    // MSVC's behaviour is to warn about including this system header. That *is* interesting
    #pragma warning(disable : 4820)

    #pragma warning(push, 1)
#endif

#ifdef __clang__
    _Pragma("clang diagnostic push")                                             \
    _Pragma("clang diagnostic ignored \"-Wdisabled-macro-expansion\"") 
    _Pragma("clang diagnostic ignored \"-Wlanguage-extension-token\"")     \
    _Pragma("clang diagnostic ignored \"-Wc++98-compat-pedantic\"")    \
    _Pragma("clang diagnostic ignored \"-Wfloat-equal\"")  
#endif // __clang

/**********************
 *** Implementation ***
 **********************/

// #include <stddef.h>
#include <stdio.h>
// #include <stdlib.h>
#include <string.h>
// #include <errno.h>

#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
    #define ARROW_UNIX_   1
    #include <errno.h>
    #include <libgen.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <signal.h>
    #include <time.h>

    #if defined CLOCK_PROCESS_CPUTIME_ID  &&  defined CLOCK_MONOTONIC
        #define ARROW_HAS_POSIX_TIMER_    1
    #endif
#endif

#if defined(_gnu_linux_) || defined(__linux__)
    #define ARROW_LINUX_      1
    #include <fcntl.h>
    #include <sys/stat.h>
#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    #define ARROW_WIN_        1
    #include <windows.h>
    #include <io.h>
#endif

#ifdef __cplusplus
    #include <exception>
#endif

#ifdef __has_include
    #if __has_include(<valgrind.h>)
        #include <valgrind.h>
    #endif
#endif

#if defined(__cplusplus)
    #define ARROW_C_FUNC extern "C"
#else
    #define ARROW_C_FUNC
#endif

// Enable the use of the non-standard keyword __attribute__ to silence warnings under some compilers
#if defined(__GNUC__) || defined(__clang__)
    #define ARROW_ATTRIBUTE_(attr)    __attribute__((attr))
#else
    #define ARROW_ATTRIBUTE_(attr)
#endif

// Timing --> use acutest's version
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
        } s;
        struct {
            unsigned long LowPart;
            long HighPart;
        } u;
        Int64 QuadPart;
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

// clock
static inline Int64 arrow_ns(void) {
#ifdef _MSC_VER
  arrow_large_integer counter;
  arrow_large_integer frequency;
  QueryPerformanceCounter(&counter);
  QueryPerformanceFrequency(&frequency);
  return cast(Int64,
                    (counter.QuadPart * 1000000000) / frequency.QuadPart);
#elif defined(__linux) && defined(__STRICT_ANSI__)
  return cast(Int64, clock()) * 1000000000 / CLOCKS_PER_SEC;
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
  return cast(Int64, ts.tv_sec) * 1000 * 1000 * 1000 + ts.tv_nsec;
#elif __APPLE__
  return cast(Int64, mach_absolute_time());
#endif
}

#if defined ARROW_WIN_
    typedef LARGE_INTEGER arrow_timer_type_;
    static LARGE_INTEGER arrow_timer_freq_;
    static arrow_timer_type_ arrow_timer_start_;
    static arrow_timer_type_ arrow_timer_end_;

    static void arrow_timer_init_(void) {
        QueryPerformanceFrequency(&arrow_timer_freq_);
    }

    static void arrow_timer_get_time_(LARGE_INTEGER* ts) {
        QueryPerformanceCounter(ts);
    }

    static double arrow_timer_diff_(LARGE_INTEGER start,  LARGE_INTEGER end) {
        double duration = (double)(end.QuadPart - start.QuadPart);
        duration /= (double)arrow_timer_freq_.QuadPart;
        return duration;
    }

    static void arrow_timer_print_diff_(void) {
        printf("%.6lf secs", arrow_timer_diff_(arrow_timer_start_, arrow_timer_end_));
    }

#elif defined ARROW_HAS_POSIX_TIMER_
    static clockid_t arrow_timer_id_;
    typedef struct timespec arrow_timer_type_;
    static arrow_timer_type_ arrow_timer_start_;
    static arrow_timer_type_ arrow_timer_end_;

    static void arrow_timer_init_() {
        if(arrow_timer_ == 1)
            arrow_timer_id_ = CLOCK_MONOTONIC;
        else if(arrow_timer_ == 2)
            arrow_timer_id_ = CLOCK_PROCESS_CPUTIME_ID;
    }

    static void arrow_timer_get_time_(struct timespec* ts) {
        clock_gettime(arrow_timer_id_, ts);
    }

    static double arrow_timer_diff_(struct timespec start, struct timespec end) {
        double endns;
        double startns;

        endns = end.tv_sec;
        endns *= 1e9;
        endns += end.tv_nsec;

        startns = start.tv_sec;
        startns *= 1e9;
        startns += start.tv_nsec;

        return ((endns - startns)/ 1e9);
    }

    static void arrow_timer_print_diff_(){
        printf("%.6lf secs",
            arrow_timer_diff_(arrow_timer_start_, arrow_timer_end_));
    }
#else
    typedef int arrow_timer_type_;
    static arrow_timer_type_ arrow_timer_start_;
    static arrow_timer_type_ arrow_timer_end_;

    void arrow_timer_init_(void) {}

    static void arrow_timer_get_time_(int* ts) {
        (void) ts;
    }

    static double arrow_timer_diff_(int start, int end) {
        (void) start;
        (void) end;
        return 0.0;
    }

    static void arrow_timer_print_diff_(void) {}
#endif


#if defined(_MSC_VER) && (_MSC_VER < 1920)
    #define ARROW_PRId64 "I64d"
    #define ARROW_PRIu64 "I64u"
#else
    #include <inttypes.h>

    #define ARROW_PRId64 PRId64
    #define ARROW_PRIu64 PRIu64
#endif

// ARROW_INITIALIZER
#if defined(_MSC_VER)
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

    #define ARROW_INITIALIZER(f)                                                   \
    static void f(void) __attribute__((constructor));                            \
    static void f(void)
#endif // _MSC_VER

#if defined(__cplusplus)
    #define ARROW_EXTERN    extern "C"
    #define ARROW_NULL      NULL
#else
    #define ARROW_EXTERN    extern
    #define ARROW_NULL      0
#endif

static inline void* arrow_realloc(void* const ptr, Ll new_size) {
  void* const new_ptr = realloc(ptr, new_size);

  if (ARROW_NULL == new_ptr) {
    free(new_ptr);
  }

  return new_ptr;
}

typedef void (*arrow_testcase_t)(int* , Ll);
struct arrow_test_s {
    arrow_testcase_t func;
    Ll index;
    char* name;
};

struct arrow_state_s {
    struct arrow_test_s* tests;
    Ll num_tests;
    FILE* foutput;
};

/* extern to the global state arrow needs to execute */
ARROW_EXTERN struct arrow_state_s arrow_state;
static char* arrow_argv0_ = null;
static const char* filter = null;

#if defined(_MSC_VER)
    #define ARROW_WEAK     force_inline
#else
    #define ARROW_WEAK     __attribute__((weak))
#endif

#if defined(_MSC_VER)
    #define ARROW_UNUSED
#else
    #define ARROW_UNUSED   __attribute__((unused))
#endif

// Overridden in `arrow_main` if the cmdline option `--no-color` is passed
static bool arrow_should_colourize_output = true;
static bool arrow_disable_summary = false; 

#define ARROW_COLOR_DEFAULT_              0
#define ARROW_COLOR_GREEN_                1
#define ARROW_COLOR_RED_                  2
#define ARROW_COLOR_DEFAULT_INTENSIVE_    3
#define ARROW_COLOR_GREEN_INTENSIVE_      4
#define ARROW_COLOR_RED_INTENSIVE_        5

static int ARROW_ATTRIBUTE_(format (printf, 2, 3))
arrow_coloured_printf_(int color, const char* fmt, ...) {
    va_list args;
    char buffer[256];
    int n;

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    buffer[sizeof(buffer)-1] = '\0';

    if(!arrow_should_colourize_output) {
        return printf("%s", buffer);
    }

#if defined ARROW_UNIX_
    {
        const char* col_str;
        switch(color) {
            case ARROW_COLOR_GREEN_:              col_str = "\033[0;32m"; break;
            case ARROW_COLOR_RED_:                col_str = "\033[0;31m"; break;
            case ARROW_COLOR_GREEN_INTENSIVE_:    col_str = "\033[1;32m"; break;
            case ARROW_COLOR_RED_INTENSIVE_:      col_str = "\033[1;31m"; break;
            case ARROW_COLOR_DEFAULT_INTENSIVE_:  col_str = "\033[1m"; break;
            default:                                col_str = "\033[0m"; break;
        }
        printf("%s", col_str);
        n = printf("%s", buffer);
        printf("\033[0m");
        return n;
    }
#elif defined ARROW_WIN_
    {
        HANDLE h;
        CONSOLE_SCREEN_BUFFER_INFO info;
        WORD attr;

        h = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(h, &info);

        switch(color) {
            case ARROW_COLOR_GREEN_:              attr = FOREGROUND_GREEN; break;
            case ARROW_COLOR_RED_:                attr = FOREGROUND_RED; break;
            case ARROW_COLOR_GREEN_INTENSIVE_:    attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case ARROW_COLOR_RED_INTENSIVE_:      attr = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            case ARROW_COLOR_DEFAULT_INTENSIVE_:  attr = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            default:                              attr = 0; break;
        }
        if(attr != 0)
            SetConsoleTextAttribute(h, attr);
        n = printf("%s", buffer);
        SetConsoleTextAttribute(h, info.wAttributes);
        return n;
    }
#else
    n = printf("%s", buffer);
    return n;
#endif
}


#define arrow_printf(...)                            \
    if (arrow_state.foutput)                         \
        fprintf(arrow_state.foutput, __VA_ARGS__);   \
    printf(__VA_ARGS__)


#ifdef _MSC_VER
    #define ARROW_SNPRINTF(BUFFER, N, ...) _snprintf_s(BUFFER, N, N, __VA_ARGS__)
#else
    #define ARROW_SNPRINTF(...) snprintf(__VA_ARGS__)
#endif


#ifdef __cplusplus
    /* if we are using c++ we can use overloaded methods (its in the language) */
    #define ARROW_OVERLOADABLE
#elif defined(__clang__)
    /* otherwise, if we are using clang with c - use the overloadable attribute */
    #define ARROW_OVERLOADABLE __attribute__((overloadable))
#endif

#ifdef ARROW_OVERLOADABLE
    ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(float f);
    ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(double d);
    ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long double d);
    ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(int i);
    ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(unsigned int i);
    ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long int i);
    ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long unsigned int i);
    ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(const void* p);

    ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(float f) {
        arrow_printf("%f", cast(double, f));
    }

    ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(double d) {
        arrow_printf("%f", d);
    }

    
    ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long double d) {
        arrow_printf("%Lf", d);
    }

    ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(int i) {
        arrow_printf("%d", i);
    }

    ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(unsigned int i) {
        arrow_printf("%u", i);
    }

    ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long int i) {
        arrow_printf("%ld", i);
    }

    ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long unsigned int i) {
        arrow_printf("%lu", i);
    }

    ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(const void* p) {
        arrow_printf("%p", p);
    }

    /*
    long long is a c++11 extension
    */
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) ||              \
        defined(__cplusplus) && (__cplusplus >= 201103L)
        ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long long int i);
        ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long long unsigned int i);
        
        ARROW_WEAK ARROW_OVERLOADABLE void arrow_type_printer(long long int i) {
            arrow_printf("%lld", i);
        }

        ARROW_WEAK ARROW_OVERLOADABLE void
        arrow_type_printer(long long unsigned int i) {
            arrow_printf("%llu", i);
        }
    #endif
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    #define arrow_type_printer(val)                                   \
        arrow_printf(_Generic((val),                                  \
                                signed char : "%d",                   \
                                unsigned char : "%u",                 \
                                short : "%d",                         \
                                unsigned short : "%u",                \
                                int : "%d",                           \
                                long : "%ld",                         \
                                long long : "%lld",                   \
                                unsigned : "%u",                      \
                                unsigned long : "%lu",                \
                                unsigned long long : "%llu",          \
                                float : "%f",                         \
                                double : "%f",                        \
                                long double : "%Lf",                  \
                                default : _Generic((val - val),       \
                                Ll : "%p",                            \
                                default : "undef")),                  \
                    (val))
#else
    /*
    we don't have the ability to print the values we got, so we create a macro
    to tell our users we can't do anything fancy
    */
    #define arrow_type_printer(...) \
        arrow_printf("Error: Your compiler does not support overloadable methods.")                 \
        arrow_printf("If you think this was an error, please file an issue on Arrow' Github repo.")
#endif // ARROW_OVERLOADABLE

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
        #define ARROW_AUTO(x)   __typeof__(x + 0)
    #endif
#else
    #define ARROW_AUTO(x)       typeof(x + 0)
#endif

#if defined(__clang__) || defined(__GNUC__)
    #define __ARROW_CHECK(x, y, cond)                                               \
        do {                                            \
            ARROW_AUTO(x) xEval = (x);                                 \
            ARROW_AUTO(y) yEval = (y);                                                 \
                                                                                        \
            if (!((xEval)cond(yEval))) {                                                                                  \
                arrow_printf("%s:%u: Failure\n", __FILE__, __LINE__);                \
                arrow_printf("  Expected : ");                                           \
                arrow_type_printer(xEval);                                               \
                arrow_printf("\n");                                                      \
                arrow_printf("    Actual : ");                                           \
                arrow_type_printer(yEval);                                               \
                arrow_printf("\n");                                                      \
                *arrow_result = 1;                                                       \
            }                                                                          \
        }                                                                            \
        while (0)                                                                    

// arrow_type_printer does not work on other compilers
#else
    #define __ARROW_CHECK(x, y, cond)                                               \
    do {                                            \
        if (!((x)cond(y))) {                                                       \
            arrow_printf("%s:%u: Failure\n", __FILE__, __LINE__);                    \
            arrow_printf("    Expected vs Actual not displayed\n");              \
            arrow_printf("    due to compiler limitations.\n");                    \
            *arrow_result = 1;                                                       \
        }                                                                          \
    }                                                                            \
    while (0)                                                                    
#endif

//
// #########################################
//            Check Macros
// #########################################
//

#define CHECK_TRUE(x)                                                         \
    do {                                            \
        if (!(x)) {                                                                \
            arrow_printf("%s:%u: Failure\n", __FILE__, __LINE__);                    \
            arrow_printf("  Expected : true\n");                                     \
            arrow_printf("    Actual : %s\n", (x) ? "true" : "false");               \
            *arrow_result = 1;                                                       \
        }                                                                          \
    }                                                                            \
    while (0)                                                                    


#define CHECK_EQ(x, y)     __ARROW_CHECK(x, y, ==)
#define CHECK_NE(x, y)     __ARROW_CHECK(x, y, !=)
#define CHECK_LT(x, y)     __ARROW_CHECK(x, y, <)
#define CHECK_LE(x, y)     __ARROW_CHECK(x, y, <=)
#define CHECK_GT(x, y)     __ARROW_CHECK(x, y, >)
#define CHECK_GE(x, y)     __ARROW_CHECK(x, y, >=)


// String Macros
#define CHECK_STREQ(x, y)                                                     \
    do {                                            \
        if (strcmp(x, y) != 0) {                                                   \
            arrow_printf("%s:%u: Failure\n", __FILE__, __LINE__);                    \
            arrow_printf("  Expected : \"%s\"\n", x);                                \
            arrow_printf("    Actual : \"%s\"\n", y);                                \
            *arrow_result = 1;                                                       \
        }                                                                          \
    }                                                                            \
    while (0)                                                                    


#define CHECK_STRNEQ(x, y)                                                     \
    do {                                            \
        if (strcmp(x, y) == 0) {                                                   \
            arrow_printf("%s:%u: Failure\n", __FILE__, __LINE__);                    \
            arrow_printf("  Expected : \"%s\"\n", x);                                \
            arrow_printf("    Actual : \"%s\"\n", y);                                \
            *arrow_result = 1;                                                       \
        }                                                                          \
  }                                                                            \
  while (0)                                                                    


#define CHECK_STRNNEQ(x, y, n)                                                 \
    do {                                            \
        if (strncmp(x, y, n) == 0) {                                         \
            arrow_printf("%s:%u: Failure\n", __FILE__, __LINE__);                    \
            arrow_printf("  Expected : \"%.*s\"\n", cast(int, n), x);          \
            arrow_printf("    Actual : \"%.*s\"\n", cast(int, n), y);          \
            *arrow_result = 1;                                                       \
        }                                                                          \
    }                                                                            \
    while (0)                                                                    

//
// #########################################
//            Assertion Macros
// #########################################
//

#if defined (__clang__) || defined(__GNUC__) 
    #define __ARROW_ASSERT(x, y, cond)                                               \
        do {                                            \
            ARROW_AUTO(x) xEval = (x);                                                 \
            ARROW_AUTO(y) yEval = (y);                                                 \
            if (!((xEval)cond(yEval))) {                                               \
                arrow_printf("%s:%u: Failure\n", __FILE__, __LINE__);                    \
                arrow_printf("  Expected : ");                                           \
                arrow_type_printer(xEval);                                               \
                arrow_printf("\n");                                                      \
                arrow_printf("    Actual : ");                                           \
                arrow_type_printer(yEval);                                               \
                arrow_printf("\n");                                                      \
                *arrow_result = 1;                                                       \
                return;                                                                  \
            }                                                                          \
        }                                                                            \
        while (0)                                                                    \

#else
    #define __ARROW_ASSERT(x, y, cond)                                               \
        do {                                            \
            if (!((x)cond(y))) {                                                       \
                arrow_printf("%s:%u: Failure\n", __FILE__, __LINE__);                    \
                *arrow_result = 1;                                                       \
                return;                                                                  \
            }                                                                          \
        }                                                                            \
        while (0)                                                                    
#endif


#define ASSERT_EQ(x, y)     __ARROW_ASSERT(x, y, ==)
#define ASSERT_NE(x, y)     __ARROW_ASSERT(x, y, !=)
#define ASSERT_LT(x, y)     __ARROW_ASSERT(x, y, <)
#define ASSERT_LE(x, y)     __ARROW_ASSERT(x, y, <=)
#define ASSERT_GT(x, y)     __ARROW_ASSERT(x, y, >)
#define ASSERT_GE(x, y)     __ARROW_ASSERT(x, y, >=)


#define ASSERT_STREQ(x, y)                                                     \
    do {                                            \
        if (strcmp(x, y) != 0) {                                                   \
            arrow_printf("%s:%u: Failure\n", __FILE__, __LINE__);                    \
            arrow_printf("  Expected : \"%s\"\n", x);                                \
            arrow_printf("    Actual : \"%s\"\n", y);                                \
            *arrow_result = 1;                                                       \
            return;                                                                  \
        }                                                                          \
    }                                                                            \
    while (0)                                                                    

#define ASSERT_STRNEQ(x, y)                                                     \
    do {                                            \
        if (strcmp(x, y) == 0) {                                                   \
            arrow_printf("%s:%u: Failure\n", __FILE__, __LINE__);                    \
            arrow_printf("  Expected : \"%s\"\n", x);                                \
            arrow_printf("    Actual : \"%s\"\n", y);                                \
            *arrow_result = 1;                                                       \
            return;                                                                  \
        }                                                                          \
    }                                                                            \
    while (0)                                                                    


#define ASSERT_STRNNEQ(x, y, n)                                                 \
    do {                                            \
        if (strncmp(x, y, n) == 0) {                                         \
            arrow_printf("%s:%u: Failure\n", __FILE__, __LINE__);                    \
            arrow_printf("  Expected : \"%.*s\"\n", cast(int, n), x);          \
            arrow_printf("    Actual : \"%.*s\"\n", cast(int, n), y);          \
            *arrow_result = 1;                                                       \
            return;                                                                  \
        }                                                                          \
    }                                                                            \
    while (0)                                                                    
    
//
// #########################################
//              Implementation
// #########################################
//

#define TEST(TESTSUITE, TESTNAME)                                                       \
    ARROW_EXTERN struct arrow_state_s arrow_state;                               \
    static void arrow_run_##TESTSUITE##_##TESTNAME(int* arrow_result);                     \
    static void arrow_##TESTSUITE##_##TESTNAME(int* arrow_result, Ll arrow_index) {    \
            (void)arrow_index;                                                         \
            arrow_run_##TESTSUITE##_##TESTNAME(arrow_result);                                    \
    }                                                                            \
    ARROW_INITIALIZER(arrow_register_##TESTSUITE##_##TESTNAME) {                           \
        const Ll index = arrow_state.num_tests++;                           \
        const char* name_part = #TESTSUITE "." #TESTNAME;                                    \
        const Ll name_size = strlen(name_part) + 1;                            \
        char* name = ptrcast(char* , malloc(name_size));                    \
        arrow_state.tests = ptrcast(                                        \
            struct arrow_test_s* ,                                           \
            arrow_realloc(ptrcast(void* , arrow_state.tests),               \
                        sizeof(struct arrow_test_s) *                      \
                            arrow_state.num_tests));                          \
        arrow_state.tests[index].func = &arrow_##TESTSUITE##_##TESTNAME;                     \
        arrow_state.tests[index].name = name;                                      \
        arrow_state.tests[index].index = 0;                                        \
        ARROW_SNPRINTF(name, name_size, "%s", name_part);                          \
    }                                                                            \
    void arrow_run_##TESTSUITE##_##TESTNAME(int* arrow_result)

ARROW_WEAK int arrow_should_filter_test(const char* filter, const char* testcase);
ARROW_WEAK int arrow_should_filter_test(const char* filter, const char* testcase) {
    if (filter) {
        const char* filter_cur = filter;
        const char* testcase_cur = testcase;
        const char* filter_wildcard = ARROW_NULL;

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
                    } else if(*filter_cur != *testcase_cur) {
                        /* otherwise our filter didn't match, so reset it */
                        filter_cur = filter_wildcard;
                    }

                    /* move testcase along */
                    testcase_cur++;

                    /* move filter along */
                    filter_cur++;
                }

                if ((*filter_cur == nullchar) && (*testcase_cur == nullchar))
                    return 0;

                /* if the testcase has been exhausted, we don't have a match! */
                if (*testcase_cur == nullchar)
                    return 1;
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

        if ((*filter_cur != nullchar) || ((*testcase_cur == nullchar) && ((filter == filter_cur) || (filter_cur[-1] != '*')))) {
            // We have a mismatch
            return 1;
        }
    }
    return 0;
}

static inline FILE* arrow_fopen(const char* filename, const char* mode) {
    #ifdef _MSC_VER
        FILE* file;
        if (0 == fopen_s(&file, filename, mode))
            return file;
        else
            return ARROW_NULL;
    #else
        return fopen(filename, mode);
    #endif
}

static void arrow_help_(void) {
        printf("Usage: %s [options] [test...]\n", arrow_argv0_);
        printf("\n");
        printf("Run the specified unit tests; or if the option '--skip' is used, run all\n");
        printf("tests in the suite but those listed.  By default, if no tests are specified\n");
        printf("on the command line, all unit tests in the suite are run.\n");
        printf("\n");
        printf("Options:\n");
        printf("  --filter=<filter>   Filter the test cases to run (e.g: Suite1*.a\n");
        printf("                        would run Suite1Case.a but not Suite1Case.b}\n");
    #if defined ARROW_WIN_
        printf("  --time              Measure test duration\n");
    #elif defined ARROW_HAS_POSIX_TIMER_
        printf("  --time              Measure test duration (real time)\n");
        printf("  --time=TIMER        Measure test duration, using given timer\n");
        printf("                          (TIMER is one of 'real', 'cpu')\n");
    #endif
        printf("  --no-summary        Suppress printing of test results summary\n");
        printf("  --output=<FILE>     Write an XUnit XML file to Enable XUnit output\n");
        printf("                        to the given file\n");
        printf("  --list              List unit tests in the suite and exit\n");
        printf("  --no-color          Disable coloured output\n");
        printf("  --help              Display this help and exit\n");
}

static bool arrow_cmdline_read(int argc, const char* const argv[]) {
    // Coloured output
#ifdef ACUTEST_UNIX_
    arrow_should_colourize_output = isatty(STDOUT_FILENO);
#elif defined ACUTEST_WIN_
    #ifdef _BORLANDC_
        arrow_should_colourize_output = isatty(_fileno(stdout));
    #else
        arrow_should_colourize_output = _isatty(_fileno(stdout));
    #endif
#else 
    arrow_should_colourize_output = isatty(STDOUT_FILENO);
#endif // ACUTEST_UNIX_

    // loop through all arguments looking for our options
    for(Ll i = 1; i < cast(Ll, argc); i++) {
        /* Informational switches */
        const char* help_str = "--help";
        const char* list_str = "--list";
        const char* color_str = "--no-color";
        const char* summary_str = "--no-summary";
        /* Test config switches */
        const char* filter_str = "--filter=";
        const char* output_str = "--output=";

        if (strncmp(argv[i], help_str, strlen(help_str)) == 0) {
            arrow_help_();
            return false;
        } 

        // Filter tests
        else if(strncmp(argv[i], filter_str, strlen(filter_str)) == 0)
            /* user wants to filter what test cases run! */
            filter = argv[i] + strlen(filter_str);

        // Write XUnit XML file
        else if(strncmp(argv[i], output_str, strlen(output_str)) == 0)
            arrow_state.foutput = arrow_fopen(argv[i] + strlen(output_str), "w+");

        // List tests
        else if(strncmp(argv[i], list_str, strlen(list_str)) == 0) {
            for (i = 0; i < arrow_state.num_tests; i++)
                arrow_printf("%s\n", arrow_state.tests[i].name);
        }

        // Disable colouring
        else if(strncmp(argv[i], color_str, strlen(color_str))) {
            arrow_should_colourize_output = false;
        }

        // Disable Summary
        else if(strncmp(argv[i], summary_str, strlen(summary_str))) {
            arrow_disable_summary = true;
        }

        else {
            printf("ERROR: Unrecognized option: %s", argv[i]);
            return false;
        }
    }

    return true;
}


static inline int arrow_main(int argc, const char* const argv[]);
inline int arrow_main(int argc, const char* const argv[]) {
    UInt64 total_tests = arrow_state.num_tests;
    UInt64 tests_ran = 0;
    UInt64 failed_tests = 0;
    UInt64 skipped_tests = 0; 

    Ll* failed_testcases = null;
    Ll failed_testcases_length = 0;
    arrow_argv0_ = argv[0];

    // Timers


    enum colours { RESET, GREEN, RED };
    const char* colours[] = {"\033[0m", "\033[32m", "\033[31m"};

    bool was_cmdline_read_successful = arrow_cmdline_read(argc, argv);
    if(!was_cmdline_read_successful) 
        goto cleanup;

    for (Ll i = 0; i < arrow_state.num_tests; i++) {
        if (arrow_should_filter_test(filter, arrow_state.tests[i].name))
            skipped_tests++;
    }

    tests_ran = total_tests - skipped_tests;

    // Begin tests
    // arrow_coloured_printf_(ARROW_COLOR_GREEN_, "[==========]%s Running %" ARROW_PRIu64 " test cases.\n", cast(UInt64, tests_ran));
    arrow_coloured_printf_(ARROW_COLOR_GREEN_, "[==========]%s Running %I64d test cases.\n", cast(UInt64, tests_ran));
    // printf("%s[==========]%s Running %" ARROW_PRIu64 " test cases.\n",
    //         colours[GREEN], colours[RESET], cast(UInt64, tests_ran));

    if (arrow_state.foutput) {
        fprintf(arrow_state.foutput, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        fprintf(arrow_state.foutput,
                "<testsuites tests=\"%" ARROW_PRIu64 "\" name=\"All\">\n",
                cast(UInt64, tests_ran));
        fprintf(arrow_state.foutput,
                "<testsuite name=\"Tests\" tests=\"%" ARROW_PRIu64 "\">\n",
                cast(UInt64, tests_ran));
    }

    for (Ll i = 0; i < arrow_state.num_tests; i++) {
        int result = 0;
        Int64 now = 0;

        if (arrow_should_filter_test(filter, arrow_state.tests[i].name))
            continue;

        // arrow_coloured_printf_(ARROW_COLOR_GREEN_, "[ RUN      ]\n");
        printf("%s[ RUN      ]%s %s\n", colours[GREEN], colours[RESET],
            arrow_state.tests[i].name);

        if(arrow_state.foutput)
            fprintf(arrow_state.foutput, "<testcase name=\"%s\">", arrow_state.tests[i].name);


        now = arrow_ns();
        errno = 0;
        arrow_state.tests[i].func(&result, arrow_state.tests[i].index);
        now = arrow_ns() - now;

        if (arrow_state.foutput)
            fprintf(arrow_state.foutput, "</testcase>\n");

        if (result != 0) {
            const Ll failed_testcase_index = failed_testcases_length++;
            failed_testcases = ptrcast(Ll *, arrow_realloc(ptrcast(void* , failed_testcases),
                                        sizeof(Ll) * failed_testcases_length));
            failed_testcases[failed_testcase_index] = i;
            failed_tests++;
            // arrow_coloured_printf_(ARROW_COLOR_RED_, "[  FAILED  ] %s (%" ARROW_PRId64 "ns)\n", arrow_state.tests[i].name, now);
            printf("%s[  FAILED  ]%s %s (%" ARROW_PRId64 "ns)\n", colours[RED],
                    colours[RESET], arrow_state.tests[i].name, now);
        } else {
            // arrow_coloured_printf_(ARROW_COLOR_RED_, "[       OK ] %s (%" ARROW_PRId64 "ns)\n", arrow_state.tests[i].name, now);
            printf("%s[       OK ]%s %s (%" ARROW_PRId64 "ns)\n", colours[GREEN],
                colours[RESET], arrow_state.tests[i].name, now);
        }
    }

    // arrow_coloured_printf_(ARROW_COLOR_GREEN_, "[==========] %" ARROW_PRIu64 " test cases ran.\n", tests_ran);
    printf("%s[==========]%s %" ARROW_PRIu64 " test cases ran.\n", colours[GREEN],
            colours[RESET], tests_ran);
    
    // Write a Summary
    // arrow_coloured_printf_(ARROW_COLOR_GREEN_, "[  PASSED  ] %" ARROW_PRIu64 " tests.\n", tests_ran - failed_tests);
    // arrow_coloured_printf_(ARROW_COLOR_RED_, "[  FAILED  ] %" ARROW_PRIu64 " %s.\n", failed_tests, failed_tests == 1 ? "test" : "tests");
    printf("%s[  PASSED  ]%s %" ARROW_PRIu64 " tests.\n", colours[GREEN],
            colours[RESET], tests_ran - failed_tests);
    printf("%s[  FAILED  ]%s %" ARROW_PRIu64 " %s.\n", colours[RED],
            colours[RESET], failed_tests, failed_tests == 1 ? "test" : "tests");

    if(!arrow_disable_summary) {
        // arrow_coloured_printf(ARROW_COLOR_DEFAULT_INTENSIVE_, "\nSummary:\n");
        printf("\nSummary:\n");

        printf("   Total unit tests:    %4d\n", cast(int, total_tests));
        printf("   Total tests run:     %4d\n", tests_ran);
        printf("   Total tests skipped: %4d\n", skipped_tests);
        printf("   Total failed tests:  %4d\n", failed_tests);
    }

    if (failed_tests != 0) {
        for (Ll i = 0; i < failed_testcases_length; i++) {
            // printf("  %s[ FAILED ]%s %s\n", colours[RED], colours[RESET],
            //         arrow_state.tests[failed_testcases[i]].name);
            arrow_coloured_printf_(ARROW_COLOR_RED_, "  [ FAILED ] %s\n", arrow_state.tests[failed_testcases[i]].name);
        }
    } else {
        // printf("%sSUCCESS:%s All tests have passed.\n", colours[GREEN], colours[RESET]);
        arrow_coloured_printf_(ARROW_COLOR_GREEN_, "SUCCESS: All tests have passed.\n");
    }

    if (arrow_state.foutput)
        fprintf(arrow_state.foutput, "</testsuite>\n</testsuites>\n");

cleanup:
    for (Ll i = 0; i < arrow_state.num_tests; i++)
        free(ptrcast(void* , arrow_state.tests[i].name));

    free(ptrcast(void* , failed_testcases));
    free(ptrcast(void* , arrow_state.tests));

    if (arrow_state.foutput)
        fclose(arrow_state.foutput);

    return cast(int, failed_tests);
}

/*
   define a main() function to call into arrow.h and start executing tests! A
   user can optionally not use this macro, and instead define their own main()
   function and manually call arrow_main. The user must, in exactly one source
   file, use the ARROW_STATE macro to declare a global struct variable that
   arrow requires.
*/
#define ARROW_MAIN()     \
    /* we need, in exactly one source file, define the global struct that will hold */ \
    /* the data we need to run arrow. This macro allows the user to declare the */ \
    /* data without having to use the ARROW_MAIN macro, thus allowing them to write */ \
    /* their own main() function. */ \
    struct arrow_state_s arrow_state = {0, 0, 0};                                       \
                                                                \
    int main(int argc, const char* const argv[]) {                               \
        return arrow_main(argc, argv);                                             \
    }

#ifdef __clang__
     _Pragma("clang diagnostic pop")
#endif // __clang__

#ifdef _MSC_VER
    #pragma warning(pop)
#endif // _MSC_VER

#endif /* SHEREDOM_ARROW_H_INCLUDED */