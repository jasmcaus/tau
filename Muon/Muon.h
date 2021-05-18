#ifndef MUON_TEST_H_
#define MUON_TEST_H_

#include <Muon/Types.h>
#include <Misc.h>

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
    _Pragma("clang diagnostic push")                                             
    _Pragma("clang diagnostic ignored \"-Wdisabled-macro-expansion\"") 
    _Pragma("clang diagnostic ignored \"-Wlanguage-extension-token\"")     
    _Pragma("clang diagnostic ignored \"-Wc++98-compat-pedantic\"")    
    _Pragma("clang diagnostic ignored \"-Wfloat-equal\"")  
#endif // __clang

/**********************
 *** Implementation ***
 **********************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
    #define MUON_UNIX_   1
    #include <errno.h>
    #include <libgen.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <signal.h>
    #include <time.h>

    #if defined CLOCK_PROCESS_CPUTIME_ID  &&  defined CLOCK_MONOTONIC
        #define MUON_HAS_POSIX_TIMER_    1
    #endif
#endif

#if defined(_gnu_linux_) || defined(__linux__)
    #define MUON_LINUX_      1
    #include <fcntl.h>
    #include <sys/stat.h>
#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    #define MUON_WIN_        1
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
    #define MUON_C_FUNC extern "C"
#else
    #define MUON_C_FUNC
#endif

// Enable the use of the non-standard keyword __attribute__ to silence warnings under some compilers
#if defined(__GNUC__) || defined(__clang__)
    #define MUON_ATTRIBUTE_(attr)    __attribute__((attr))
#else
    #define MUON_ATTRIBUTE_(attr)
#endif


static MUON_UInt64 muon_stats_total_tests = 0;
static MUON_UInt64 muon_stats_tests_ran = 0;
static MUON_UInt64 muon_stats_tests_failed = 0;
static MUON_UInt64 muon_stats_skipped_tests = 0; 

// Overridden in `muon_main` if the cmdline option `--no-color` is passed
static int muon_should_colourize_output = 1;
static int muon_disable_summary = 0; 

MUON_Ll* muon_stats_failed_testcases = MUON_null;
MUON_Ll muon_stats_failed_testcases_length = 0;

static char* muon_argv0_ = MUON_null;
static const char* filter = MUON_null;


static int muon_timer_ = 1;
// Timing --> use muon's version
#if defined(_MSC_VER)
    // define MUON_USE_OLD_QPC before #include "muon.h" to use old QueryPerformanceCounter
    #ifndef MUON_USE_OLD_QPC
        #pragma warning(push, 0)
        #include <Windows.h>
        #pragma warning(pop)

        typedef LARGE_INTEGER muon_large_integer;
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
        } muon_large_integer;

        MUON_C_FUNC __declspec(dllimport) int __stdcall QueryPerformanceCounter(
            muon_large_integer *);
        MUON_C_FUNC __declspec(dllimport) int __stdcall QueryPerformanceFrequency(
            muon_large_integer *);
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
        #define MUON_USE_CLOCKGETTIME
    #else
        #include <sys/syscall.h>
        #include <unistd.h>
    #endif
#else // Other libc implementations
    #include <time.h>
    #define MUON_USE_CLOCKGETTIME
#endif

#elif defined(__APPLE__)
    #include <mach/mach_time.h>
#endif

// Muon Timer 
// This method is useful in timing the execution of an Muon Test Suite
// To use this, simply call this function before and after the particular code block you want to time, 
// and their difference will give you the time (in seconds). 
// NOTE: This method has been edited to return the time (in nanoseconds). Depending on how large this value
// (e.g: 54890938849ns), we appropriately convert it to milliseconds/seconds before displaying it to stdout.
static inline double muon_ns() {
#ifdef MUON_WIN_
    LARGE_INTEGER counter;
    LARGE_INTEGER frequency;
    QueryPerformanceCounter(&counter);
    QueryPerformanceFrequency(&frequency);
    return MUON_cast(double, (counter.QuadPart * 1000 * 1000 * 1000) / frequency.QuadPart); // in nanoseconds

#elif defined(__linux) && defined(__STRICT_ANSI__)
    return MUON_cast(double, clock()) * 1000000000 / CLOCKS_PER_SEC; // in nanoseconds 

#elif defined(__linux)
    struct timespec ts;
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
        timespec_get(&ts, TIME_UTC);
    #else
        const clockid_t cid = CLOCK_REALTIME;
        #if defined(MUON_USE_CLOCKGETTIME)
            clock_gettime(cid, &ts);
        #else
            syscall(SYS_clock_gettime, cid, &ts);
        #endif
    #endif
    return MUON_cast(double, ts.tv_sec) * 1000 * 1000 * 1000 + ts.tv_nsec; // in nanoseconds

#elif __APPLE__
    return MUON_cast(double, mach_absolute_time());
#endif
}

static void muon_timer_print_duration(double nanoseconds_duration) {
    MUON_UInt64 n; 
    int n_digits = 0; 
    n = (MUON_UInt64)nanoseconds_duration;
    while(n!=0) {
        n/=10;
        ++n_digits;
    }
    
    // Stick with nanoseconds (no need for decimal points here)
    if(n_digits < 3) 
        printf("%.0lfns", nanoseconds_duration);

    else if(n_digits >= 3 && n_digits < 6)
        printf("%.2lfus", nanoseconds_duration/1000);
        
    else if(n_digits >= 6 && n_digits <= 9)
        printf("%.2lfms", nanoseconds_duration/1000000);

    else
        printf("%.2lfs", nanoseconds_duration/1000000000);
}

// #if defined MUON_WIN_
//     typedef LARGE_INTEGER muon_timer_type_;
//     static LARGE_INTEGER muon_timer_freq_;
//     static muon_timer_type_ muon_test_timer_start_;
//     static muon_timer_type_ muon_test_timer_end_;

//     static void muon_timer_init_(void) {
//         QueryPerformanceFrequency(&muon_timer_freq_);
//     }

//     static void muon_timer_get_time_(LARGE_INTEGER* ts) {
//         QueryPerformanceCounter(ts);
//     }

//     static double muon_timer_diff_(LARGE_INTEGER start,  LARGE_INTEGER end) {
//         double duration = (double)(end.QuadPart - start.QuadPart);
//         duration /= (double)muon_timer_freq_.QuadPart;
//         // printf("\nDURATION: %f\n", (double)muon_timer_freq_.QuadPart);
//         duration *= 1000; // convert to milliseconds
//         return duration;
//     }

//     static void muon_timer_print_diff_(void) {
//         printf("%.6lf secs", muon_timer_diff_(muon_test_timer_start_, muon_test_timer_end_));
//     }

// #elif defined MUON_HAS_POSIX_TIMER_
//     static clockid_t muon_timer_id_;
//     typedef struct timespec muon_timer_type_;
//     static muon_timer_type_ muon_test_timer_start_;
//     static muon_timer_type_ muon_test_timer_end_;

//     static void muon_timer_init_() {
//         if(muon_timer_ == 1)
//             muon_timer_id_ = CLOCK_MONOTONIC;
//         else if(muon_timer_ == 2)
//             muon_timer_id_ = CLOCK_PROCESS_CPUTIME_ID;
//     }

//     static void muon_timer_get_time_(struct timespec* ts) {
//         clock_gettime(muon_timer_id_, ts);
//     }

//     static double muon_timer_diff_(struct timespec start, struct timespec end) {
//         double endns;
//         double startns;

//         endns = end.tv_sec;
//         endns *= 1e9;
//         endns += end.tv_nsec;

//         startns = start.tv_sec;
//         startns *= 1e9;
//         startns += start.tv_nsec;

//         return ((endns - startns)/ 1e9);
//     }

//     static void muon_timer_print_diff_(){
//         printf("%.6lf secs",
//             muon_timer_diff_(muon_test_timer_start_, muon_test_timer_end_));
//     }
// #else
//     typedef int muon_timer_type_;
//     static muon_timer_type_ muon_test_timer_start_;
//     static muon_timer_type_ muon_test_timer_end_;

//     void muon_timer_init_(void) {}

//     static void muon_timer_get_time_(int* ts) {
//         (void) ts;
//     }

//     static double muon_timer_diff_(int start, int end) {
//         (void) start;
//         (void) end;
//         return 0.0;
//     }

//     static void muon_timer_print_diff_(void) {}
// #endif


#define MUON_PRId64 "I64d"
#define MUON_PRIu64 "I64u"

// MUON_INITIALIZER
#if defined(_MSC_VER)
    #if defined(_WIN64)
        #define MUON_SYMBOL_PREFIX
    #else
        #define MUON_SYMBOL_PREFIX "_"
    #endif

    #if defined(__clang__)
        #define MUON_INITIALIZER_BEGIN_DISABLE_WARNINGS                               \
        _Pragma("clang diagnostic push")                                             \
            _Pragma("clang diagnostic ignored \"-Wmissing-variable-declarations\"")

        #define MUON_INITIALIZER_END_DISABLE_WARNINGS _Pragma("clang diagnostic pop")
    #else
        #define MUON_INITIALIZER_BEGIN_DISABLE_WARNINGS
        #define MUON_INITIALIZER_END_DISABLE_WARNINGS
    #endif

    #pragma section(".CRT$XCU", read)
    #define MUON_INITIALIZER(f)                                                   \
    static void __cdecl f(void);                                                 \
    MUON_INITIALIZER_BEGIN_DISABLE_WARNINGS                                     \
    __pragma(comment(linker, "/include:" MUON_SYMBOL_PREFIX #f "_"))            \
        MUON_C_FUNC __declspec(allocate(".CRT$XCU")) void(__cdecl *             \
                                                            f##_)(void) = f;      \
    MUON_INITIALIZER_END_DISABLE_WARNINGS                                       \
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

    #define MUON_INITIALIZER(f)                                                   \
    static void f(void) __attribute__((constructor));                            \
    static void f(void)
#endif // _MSC_VER

#if defined(__cplusplus)
    #define MUON_EXTERN    extern "C"
    #define MUON_NULL      NULL
#else
    #define MUON_EXTERN    extern
    #define MUON_NULL      0
#endif

// extern to the global state muon needs to execute
MUON_EXTERN struct muon_state_s muon_state;

static inline void* muon_realloc(void* const ptr, MUON_Ll new_size) {
  void* const new_ptr = realloc(ptr, new_size);

  if (MUON_NULL == new_ptr) {
    free(new_ptr);
  }

  return new_ptr;
}

typedef void (*muon_testcase_t)(int* , MUON_Ll);
struct muon_test_s {
    muon_testcase_t func;
    MUON_Ll index;
    char* name;
};

struct muon_state_s {
    struct muon_test_s* tests;
    MUON_Ll num_tests;
    FILE* foutput;
};

#if defined(_MSC_VER)
    #define MUON_WEAK     inline
#else
    #define MUON_WEAK     __attribute__((weak))
#endif

#if defined(_MSC_VER)
    #define MUON_UNUSED
#else
    #define MUON_UNUSED   __attribute__((unused))
#endif


#define MUON_COLOUR_DEFAULT_              0
#define MUON_COLOUR_GREEN_                1
#define MUON_COLOUR_RED_                  2
#define MUON_COLOUR_DEFAULT_INTENSIVE_    3
#define MUON_COLOUR_GREEN_INTENSIVE_      4
#define MUON_COLOUR_RED_INTENSIVE_        5

static int MUON_ATTRIBUTE_(format (printf, 2, 3))
muon_coloured_printf_(int color, const char* fmt, ...) {
    va_list args;
    char buffer[256];
    int n;

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    buffer[sizeof(buffer)-1] = '\0';

    if(!muon_should_colourize_output) {
        return printf("%s", buffer);
    }

#if defined MUON_UNIX_
    {
        const char* col_str;
        switch(color) {
            case MUON_COLOUR_GREEN_:              col_str = "\033[0;32m"; break;
            case MUON_COLOUR_RED_:                col_str = "\033[0;31m"; break;
            case MUON_COLOUR_GREEN_INTENSIVE_:    col_str = "\033[1;32m"; break;
            case MUON_COLOUR_RED_INTENSIVE_:      col_str = "\033[1;31m"; break;
            case MUON_COLOUR_DEFAULT_INTENSIVE_:  col_str = "\033[1m"; break;
            default:                                col_str = "\033[0m"; break;
        }
        printf("%s", col_str);
        n = printf("%s", buffer);
        printf("\033[0m");
        return n;
    }
#elif defined MUON_WIN_
    {
        HANDLE h;
        CONSOLE_SCREEN_BUFFER_INFO info;
        WORD attr;

        h = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(h, &info);

        switch(color) {
            case MUON_COLOUR_GREEN_:              attr = FOREGROUND_GREEN; break;
            case MUON_COLOUR_RED_:                attr = FOREGROUND_RED; break;
            case MUON_COLOUR_GREEN_INTENSIVE_:    attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case MUON_COLOUR_RED_INTENSIVE_:      attr = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            case MUON_COLOUR_DEFAULT_INTENSIVE_:  attr = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY; break;
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


#define muon_printf(...)                            \
    if (muon_state.foutput)                         \
        fprintf(muon_state.foutput, __VA_ARGS__);   \
    printf(__VA_ARGS__)


#ifdef _MSC_VER
    #define MUON_SNPRINTF(BUFFER, N, ...) _snprintf_s(BUFFER, N, N, __VA_ARGS__)
#else
    #define MUON_SNPRINTF(...) snprintf(__VA_ARGS__)
#endif


#ifdef __cplusplus
    /* if we are using c++ we can use overloaded methods (its in the language) */
    #define MUON_OVERLOADABLE
#elif defined(__clang__)
    /* otherwise, if we are using clang with c - use the overloadable attribute */
    #define MUON_OVERLOADABLE __attribute__((overloadable))
#endif

#ifdef MUON_OVERLOADABLE
    MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(float f);
    MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(double d);
    MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(long double d);
    MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(int i);
    MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(unsigned int i);
    MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(long int i);
    MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(long unsigned int i);
    MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(const void* p);

    MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(float f) {
        muon_printf("%f", MUON_cast(double, f));
    }

    MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(double d) {
        muon_printf("%f", d);
    }

    
    MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(long double d) {
        muon_printf("%Lf", d);
    }

    MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(int i) {
        muon_printf("%d", i);
    }

    MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(unsigned int i) {
        muon_printf("%u", i);
    }

    MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(long int i) {
        muon_printf("%ld", i);
    }

    MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(long unsigned int i) {
        muon_printf("%lu", i);
    }

    MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(const void* p) {
        muon_printf("%p", p);
    }

    /*
    long long is a c++11 extension
    */
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) || defined(__cplusplus) && (__cplusplus >= 201103L)
        MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(long long int i);
        MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(long long unsigned int i);
        
        MUON_WEAK MUON_OVERLOADABLE void muon_type_printer(long long int i) {
            muon_printf("%lld", i);
        }

        MUON_WEAK MUON_OVERLOADABLE void
        muon_type_printer(long long unsigned int i) {
            muon_printf("%llu", i);
        }
    #endif
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    #define muon_type_printer(val)                                    \
        muon_printf(_Generic((val),                                   \
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
                                MUON_Ll : "%p",                       \
                                default : "undef")),                  \
                    (val))
#else
    /*
    we don't have the ability to print the values we got, so we create a macro
    to tell our users we can't do anything fancy
    */
    #define muon_type_printer(...) \
        muon_printf("Error: Your compiler does not support overloadable methods.")                 \
        muon_printf("If you think this was an error, please file an issue on Muon's Github repo.")
#endif // MUON_OVERLOADABLE

#if defined(__cplusplus) && (__cplusplus >= 201103L)
    #define MUON_AUTO(x) auto
#elif !defined(__cplusplus)
    #if defined(__clang__)
        // clang-format off
        // had to disable clang-format here because it malforms the pragmas
        #define MUON_AUTO(x)                                                     \
            _Pragma("clang diagnostic push")                                     \
            _Pragma("clang diagnostic ignored \"-Wgnu-auto-type\"")              \
            __auto_type                                                          \
            _Pragma("clang diagnostic pop")
        // clang-format on
    #else
        #define MUON_AUTO(x)   __typeof__(x + 0)
    #endif
#else
    #define MUON_AUTO(x)       typeof(x + 0)
#endif


#if defined(__clang__) || defined(__GNUC__)
    #define __CHECK(x, y, cond)                                               \
        do {                                                                  \
            MUON_AUTO(x) xEval = (x);                                         \
            MUON_AUTO(y) yEval = (y);                                         \
                                                                              \
            if (!((xEval)cond(yEval))) {                                      \
                muon_printf("%s:%u: Failure\n", __FILE__, __LINE__);          \
                muon_printf("  Expected : ");                                 \
                muon_type_printer(xEval);                                     \
                muon_printf("\n");                                            \
                muon_printf("    Actual : ");                                 \
                muon_type_printer(yEval);                                     \
                muon_printf("\n");                                            \
                *muon_result = 1;                                             \
            }                                                                 \
        }                                                                     \
        while (0)

// muon_type_printer does not work on other compilers
#else
    #define __CHECK(x, y, cond)                                               \
        do {                                                                  \
            if (!((x)cond(y))) {                                              \
                muon_printf("%s:%u: Failure\n", __FILE__, __LINE__);          \
                muon_printf("    Expected vs Actual not displayed\n");        \
                muon_printf("    due to compiler limitations.\n");            \
                *muon_result = 1;                                             \
            }                                                                 \
        }                                                                     \
        while (0)
#endif

#define CHECK(cond, ...)                                                         \
    do {                                                                         \
        if (!(cond)) {                                                           \
            muon_printf("%s:%u: ", __FILE__, __LINE__);                          \
            if((sizeof(char[]){__VA_ARGS__}) <= 1)                               \
                muon_coloured_printf_(MUON_COLOUR_RED_INTENSIVE_, "Failure");    \
            else                                                                 \
                muon_coloured_printf_(MUON_COLOUR_RED_INTENSIVE_, __VA_ARGS__);  \
            printf("\n");                                                        \
            *muon_result = 1;                                                    \
        }                                                                        \
    }                                                                            \
    while (0)

//
// #########################################
//            Check Macros
// #########################################
//

#define CHECK_TRUE(x)                                                         \
    do {                                                                      \
        if (!(x)) {                                                           \
            muon_printf("%s:%u: Failure\n", __FILE__, __LINE__);              \
            muon_printf("  Expected : true\n");                               \
            muon_printf("    Actual : %s\n", (x) ? "true" : "false");         \
            *muon_result = 1;                                                 \
        }                                                                     \
    }                                                                         \
    while (0)                                                                    


#define CHECK_EQ(x, y)     __CHECK(x, y, ==)
#define CHECK_NE(x, y)     __CHECK(x, y, !=)
#define CHECK_LT(x, y)     __CHECK(x, y, <)
#define CHECK_LE(x, y)     __CHECK(x, y, <=)
#define CHECK_GT(x, y)     __CHECK(x, y, >)
#define CHECK_GE(x, y)     __CHECK(x, y, >=)


// String Macros
#define CHECK_STREQ(x, y)                                                     \
    do {                                                                      \
        if (strcmp(x, y) != 0) {                                              \
            muon_printf("%s:%u: Failure\n", __FILE__, __LINE__);              \
            muon_printf("  Expected : \"%s\"\n", x);                          \
            muon_printf("    Actual : \"%s\"\n", y);                          \
            *muon_result = 1;                                                 \
        }                                                                     \
    }                                                                         \
    while (0)                                                                    


#define CHECK_STRNEQ(x, y)                                                    \
    do {                                                                      \
        if (strcmp(x, y) == 0) {                                              \
            muon_printf("%s:%u: Failure\n", __FILE__, __LINE__);              \
            muon_printf("  Expected : \"%s\"\n", x);                          \
            muon_printf("    Actual : \"%s\"\n", y);                          \
            *muon_result = 1;                                                 \
        }                                                                     \
  }                                                                           \
  while (0)                                                                    


#define CHECK_STRNNEQ(x, y, n)                                                \
    do {                                                                      \
        if (strncmp(x, y, n) == 0) {                                          \
            muon_printf("%s:%u: Failure\n", __FILE__, __LINE__);              \
            muon_printf("  Expected : \"%.*s\"\n", MUON_cast(int, n), x);     \
            muon_printf("    Actual : \"%.*s\"\n", MUON_cast(int, n), y);     \
            *muon_result = 1;                                                 \
        }                                                                     \
    }                                                                         \
    while (0)                                                                    

//
// #########################################
//            Assertion Macros
// #########################################
//

#if defined (__clang__) || defined(__GNUC__) 
    #define __REQUIRE(x, y, cond)                                             \
        do {                                                                  \
            MUON_AUTO(x) xEval = (x);                                         \
            MUON_AUTO(y) yEval = (y);                                         \
            if (!((xEval)cond(yEval))) {                                      \
                muon_printf("%s:%u: Failure\n", __FILE__, __LINE__);          \
                muon_printf("  Expected : ");                                 \
                muon_type_printer(xEval);                                     \
                muon_printf("\n");                                            \
                muon_printf("    Actual : ");                                 \
                muon_type_printer(yEval);                                     \
                muon_printf("\n");                                            \
                *muon_result = 1;                                             \
                return;                                                       \
            }                                                                 \
        }                                                                     \
        while (0)                                                             \

#else
    #define __REQUIRE(x, y, cond)                                             \
        do {                                                                  \
            if (!((x)cond(y))) {                                              \
                muon_printf("%s:%u: Failure\n", __FILE__, __LINE__);          \
                *muon_result = 1;                                             \
                return;                                                       \
            }                                                                 \
        }                                                                     \
        while (0)                                                                    
#endif


#define REQUIRE_EQ(x, y)     __REQUIRE(x, y, ==)
#define REQUIRE_NE(x, y)     __REQUIRE(x, y, !=)
#define REQUIRE_LT(x, y)     __REQUIRE(x, y, <)
#define REQUIRE_LE(x, y)     __REQUIRE(x, y, <=)
#define REQUIRE_GT(x, y)     __REQUIRE(x, y, >)
#define REQUIRE_GE(x, y)     __REQUIRE(x, y, >=)


#define REQUIRE_STREQ(x, y)                                                   \
    do {                                                                      \
        if (strcmp(x, y) != 0) {                                              \
            muon_printf("%s:%u: Failure\n", __FILE__, __LINE__);              \
            muon_printf("  Expected : \"%s\"\n", x);                          \
            muon_printf("    Actual : \"%s\"\n", y);                          \
            *muon_result = 1;                                                 \
            return;                                                           \
        }                                                                     \
    }                                                                         \
    while (0)                                                                    

#define REQUIRE_STRNEQ(x, y)                                                  \
    do {                                                                      \
        if (strcmp(x, y) == 0) {                                              \
            muon_printf("%s:%u: Failure\n", __FILE__, __LINE__);              \
            muon_printf("  Expected : \"%s\"\n", x);                          \
            muon_printf("    Actual : \"%s\"\n", y);                          \
            *muon_result = 1;                                                 \
            return;                                                           \
        }                                                                     \
    }                                                                         \
    while (0)                                                                    


#define REQUIRE_STRNNEQ(x, y, n)                                              \
    do {                                                                      \
        if (strncmp(x, y, n) == 0) {                                          \
            muon_printf("%s:%u: Failure\n", __FILE__, __LINE__);              \
            muon_printf("  Expected : \"%.*s\"\n", MUON_cast(int, n), x);     \
            muon_printf("    Actual : \"%.*s\"\n", MUON_cast(int, n), y);     \
            *muon_result = 1;                                                 \
            return;                                                           \
        }                                                                     \
    }                                                                         \
    while (0)                                                                    
    
//
// #########################################
//              Implementation
// #########################################
//

#define TEST(TESTSUITE, TESTNAME)                                                       \
    MUON_EXTERN struct muon_state_s muon_state;                                         \
    static void muon_run_##TESTSUITE##_##TESTNAME(int* muon_result);                    \
    static void muon_##TESTSUITE##_##TESTNAME(int* muon_result, MUON_Ll muon_index) {   \
            (void)muon_index;                                                           \
            muon_run_##TESTSUITE##_##TESTNAME(muon_result);                             \
    }                                                                                   \
    MUON_INITIALIZER(muon_register_##TESTSUITE##_##TESTNAME) {                          \
        const MUON_Ll index = muon_state.num_tests++;                                   \
        const char* name_part = #TESTSUITE "." #TESTNAME;                               \
        const MUON_Ll name_size = strlen(name_part) + 1;                                \
        char* name = MUON_ptrcast(char* , malloc(name_size));                           \
        muon_state.tests = MUON_ptrcast(                                                \
            struct muon_test_s* ,                                                       \
            muon_realloc(MUON_ptrcast(void* , muon_state.tests),                        \
                        sizeof(struct muon_test_s) *                                    \
                            muon_state.num_tests));                                     \
        muon_state.tests[index].func = &muon_##TESTSUITE##_##TESTNAME;                  \
        muon_state.tests[index].name = name;                                            \
        muon_state.tests[index].index = 0;                                              \
        MUON_SNPRINTF(name, name_size, "%s", name_part);                                \
    }                                                                                   \
    void muon_run_##TESTSUITE##_##TESTNAME(int* muon_result)


MUON_WEAK int muon_should_filter_test(const char* filter, const char* testcase);
MUON_WEAK int muon_should_filter_test(const char* filter, const char* testcase) {
    if (filter) {
        const char* filter_cur = filter;
        const char* testcase_cur = testcase;
        const char* filter_wildcard = MUON_NULL;

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

                if ((*filter_cur == MUON_nullchar) && (*testcase_cur == MUON_nullchar))
                    return 0;

                /* if the testcase has been exhausted, we don't have a match! */
                if (*testcase_cur == MUON_nullchar)
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

        if ((*filter_cur != MUON_nullchar) || ((*testcase_cur == MUON_nullchar) && ((filter == filter_cur) || (filter_cur[-1] != '*')))) {
            // We have a mismatch
            return 1;
        }
    }
    return 0;
}

static inline FILE* muon_fopen(const char* filename, const char* mode) {
    #ifdef _MSC_VER
        FILE* file;
        if (0 == fopen_s(&file, filename, mode))
            return file;
        else
            return MUON_NULL;
    #else
        return fopen(filename, mode);
    #endif
}

static void muon_help_(void) {
        printf("Usage: %s [options] [test...]\n", muon_argv0_);
        printf("\n");
        printf("Run the specified unit tests; or if the option '--skip' is used, run all\n");
        printf("tests in the suite but those listed.  By default, if no tests are specified\n");
        printf("on the command line, all unit tests in the suite are run.\n");
        printf("\n");
        printf("Options:\n");
        printf("  --filter=<filter>   Filter the test suites to run (e.g: Suite1*.a\n");
        printf("                        would run Suite1Case.a but not Suite1Case.b}\n");
    #if defined MUON_WIN_
        printf("  --time              Measure test duration\n");
    #elif defined MUON_HAS_POSIX_TIMER_
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

static MUON_bool muon_cmdline_read(int argc, const char* const argv[]) {
    // Coloured output
#ifdef MUON_UNIX_
    muon_should_colourize_output = isatty(STDOUT_FILENO);
#elif defined MUON_WIN_
    #ifdef _BORLANDC_
        muon_should_colourize_output = isatty(_fileno(stdout));
    #else
        muon_should_colourize_output = _isatty(_fileno(stdout));
    #endif
#else 
    muon_should_colourize_output = isatty(STDOUT_FILENO);
#endif // MUON_UNIX_

    // loop through all arguments looking for our options
    for(MUON_Ll i = 1; i < MUON_cast(MUON_Ll, argc); i++) {
        /* Informational switches */
        const char* help_str = "--help";
        const char* list_str = "--list";
        const char* color_str = "--no-color";
        const char* summary_str = "--no-summary";
        /* Test config switches */
        const char* filter_str = "--filter=";
        const char* output_str = "--output=";

        if (strncmp(argv[i], help_str, strlen(help_str)) == 0) {
            muon_help_();
            return MUON_false;
        } 

        // Filter tests
        else if(strncmp(argv[i], filter_str, strlen(filter_str)) == 0)
            /* user wants to filter what test suites run! */
            filter = argv[i] + strlen(filter_str);

        // Write XUnit XML file
        else if(strncmp(argv[i], output_str, strlen(output_str)) == 0)
            muon_state.foutput = muon_fopen(argv[i] + strlen(output_str), "w+");

        // List tests
        else if(strncmp(argv[i], list_str, strlen(list_str)) == 0) {
            for (i = 0; i < muon_state.num_tests; i++)
                muon_printf("%s\n", muon_state.tests[i].name);
        }

        // Disable colouring
        else if(strncmp(argv[i], color_str, strlen(color_str))) {
            muon_should_colourize_output = MUON_false;
        }

        // Disable Summary
        else if(strncmp(argv[i], summary_str, strlen(summary_str))) {
            muon_disable_summary = MUON_true;
        }

        else {
            printf("ERROR: Unrecognized option: %s", argv[i]);
            return MUON_false;
        }
    }

    return MUON_true;
}


// Triggers and runs all unit tests
static void muon_run_tests() {
    // Run tests
    for (MUON_Ll i = 0; i < muon_state.num_tests; i++) {
        int result = 0;

        if (muon_should_filter_test(filter, muon_state.tests[i].name))
            continue;

        muon_coloured_printf_(MUON_COLOUR_GREEN_INTENSIVE_, "[ RUN      ] ");
        muon_coloured_printf_(MUON_COLOUR_DEFAULT_, "%s\n", muon_state.tests[i].name);

        if(muon_state.foutput)
            fprintf(muon_state.foutput, "<testcase name=\"%s\">", muon_state.tests[i].name);

        // Start the timer
        double start = muon_ns();

        // The actual test
        muon_state.tests[i].func(&result, muon_state.tests[i].index);

        // Stop the timer
        double duration = muon_ns() - start;
    
        if (muon_state.foutput)
            fprintf(muon_state.foutput, "</testcase>\n");

        if (result != 0) {
            const MUON_Ll failed_testcase_index = muon_stats_failed_testcases_length++;
            muon_stats_failed_testcases = MUON_ptrcast(MUON_Ll*, 
                                            muon_realloc(MUON_ptrcast(void* , muon_stats_failed_testcases), 
                                                          sizeof(MUON_Ll) * muon_stats_failed_testcases_length));
            muon_stats_failed_testcases[failed_testcase_index] = i;
            muon_stats_tests_failed++;
            muon_coloured_printf_(MUON_COLOUR_RED_INTENSIVE_, "[  FAILED  ] ");
            muon_coloured_printf_(MUON_COLOUR_DEFAULT_, "%s (", muon_state.tests[i].name);
            muon_timer_print_duration(duration);
            printf(")\n");
        } else {
            muon_coloured_printf_(MUON_COLOUR_GREEN_INTENSIVE_, "[       OK ] ");
            muon_coloured_printf_(MUON_COLOUR_DEFAULT_, "%s (", muon_state.tests[i].name);
            muon_timer_print_duration(duration);
            printf(")\n");
        }
    }

    muon_coloured_printf_(MUON_COLOUR_GREEN_INTENSIVE_, "[==========] ");
    muon_coloured_printf_(MUON_COLOUR_DEFAULT_, "%" MUON_PRIu64 " test suites ran\n", muon_stats_tests_ran);
}


static inline int muon_main(int argc, const char* const argv[]);
inline int muon_main(int argc, const char* const argv[]) {
    muon_stats_total_tests = muon_state.num_tests;
    muon_argv0_ = argv[0];
    
    enum colours { RESET, GREEN, RED };
    const char* colours[] = {"\033[0m", "\033[32m", "\033[31m"};

    // Start the entire Test Session timer
    double start = muon_ns();

    MUON_bool was_cmdline_read_successful = muon_cmdline_read(argc, argv);
    if(!was_cmdline_read_successful) 
        goto cleanup;

    for (MUON_Ll i = 0; i < muon_state.num_tests; i++) {
        if (muon_should_filter_test(filter, muon_state.tests[i].name))
            muon_stats_skipped_tests++;
    }

    muon_stats_tests_ran = muon_stats_total_tests - muon_stats_skipped_tests;

    // Begin tests
    muon_coloured_printf_(MUON_COLOUR_GREEN_INTENSIVE_, "[==========] ");
    muon_coloured_printf_(MUON_COLOUR_DEFAULT_INTENSIVE_, "Running %" MUON_PRIu64 " test cases.\n", MUON_cast(MUON_UInt64, muon_stats_tests_ran));

    if (muon_state.foutput) {
        fprintf(muon_state.foutput, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        fprintf(muon_state.foutput,
                "<testsuites tests=\"%" MUON_PRIu64 "\" name=\"All\">\n",
                MUON_cast(MUON_UInt64, muon_stats_tests_ran));
        fprintf(muon_state.foutput,
                "<testsuite name=\"Tests\" tests=\"%" MUON_PRIu64 "\">\n",
                MUON_cast(MUON_UInt64, muon_stats_tests_ran));
    }

    // Run tests
    muon_run_tests();

    // End the entire Test Session timer
    double duration = muon_ns() - start;

    // Write a Summary
    muon_coloured_printf_(MUON_COLOUR_GREEN_INTENSIVE_, "[  PASSED  ] %" MUON_PRIu64 " tests\n", muon_stats_tests_ran - muon_stats_tests_failed);
    muon_coloured_printf_(MUON_COLOUR_RED_INTENSIVE_, "[  FAILED  ] %" MUON_PRIu64 " %s\n", muon_stats_tests_failed, muon_stats_tests_failed == 1 ? "test" : "tests");

    if(!muon_disable_summary) {
        muon_coloured_printf_(MUON_COLOUR_DEFAULT_INTENSIVE_, "\nSummary:\n");

        printf("   Total unit tests:      %" MUON_PRIu64 "\n", muon_stats_total_tests);
        printf("   Total tests run:       %" MUON_PRIu64 "\n", muon_stats_tests_ran);
        printf("   Total tests skipped:   %" MUON_PRIu64 "\n", muon_stats_skipped_tests);
        printf("   Total tests failed:    %" MUON_PRIu64 "\n", muon_stats_tests_failed);
    }

    if (muon_stats_tests_failed != 0) {
        muon_coloured_printf_(MUON_COLOUR_RED_INTENSIVE_, "FAILED: ");
        printf("%" MUON_PRIu64 " failed, %" MUON_PRIu64 " passed in ", muon_stats_tests_failed, muon_stats_tests_ran - muon_stats_tests_failed);
        muon_timer_print_duration(duration);
        printf("\n");

        for (MUON_Ll i = 0; i < muon_stats_failed_testcases_length; i++) {
            muon_coloured_printf_(MUON_COLOUR_RED_INTENSIVE_, "  [ FAILED ] %s\n", muon_state.tests[muon_stats_failed_testcases[i]].name);
        }
    } else {
        muon_coloured_printf_(MUON_COLOUR_GREEN_INTENSIVE_, "SUCCESS: ");
        printf("%" MUON_PRIu64 " tests have passed in ", muon_stats_tests_ran - muon_stats_tests_failed);
        muon_timer_print_duration(duration);       
        printf("\n");
    }

    if (muon_state.foutput)
        fprintf(muon_state.foutput, "</testsuite>\n</testsuites>\n");

cleanup:
    for (MUON_Ll i = 0; i < muon_state.num_tests; i++)
        free(MUON_ptrcast(void* , muon_state.tests[i].name));

    free(MUON_ptrcast(void* , muon_stats_failed_testcases));
    free(MUON_ptrcast(void* , muon_state.tests));

    if (muon_state.foutput)
        fclose(muon_state.foutput);

    return MUON_cast(int, muon_stats_tests_failed);
}

// Define a main() function to call into muon.h and start executing tests.
#define MUON_MAIN()                                                             \
    /* Define the global struct that will hold the data we need to run Muon. */ \
    struct muon_state_s muon_state = {0, 0, 0};                                 \
                                                                                \
    int main(int argc, const char* const argv[]) {                              \
        return muon_main(argc, argv);                                           \
    }

#ifdef __clang__
     _Pragma("clang diagnostic pop")
#endif // __clang__

#ifdef _MSC_VER
    #pragma warning(pop)
#endif // _MSC_VER

#endif /* MUON_TEST_H_ */