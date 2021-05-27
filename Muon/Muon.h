/*
|  \/  | |  | |/ __ \| \ | |
| \  / | |  | | |  | |  \| |    Muon - The Micro Testing Framework for C/C++
| |\/| | |  | | |  | | . ` |    Languages: C, and C++
| |  | | |__| | |__| | |\  |    https://github.com/jasmcaus/Muon
|_|  |_|\____/ \____/|_| \_|
Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

#ifndef MUON_TEST_H_
#define MUON_TEST_H_

#include <Muon/Types.h>
#include <Muon/Misc.h>

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

    // warning C4090: '=': different 'const' qualifiers
    #pragma warning(disable : 4090)

    // io.h contains definitions for some structures with natural padding. This is uninteresting, but for some reason, 
    // MSVC's behaviour is to warn about including this system header. That *is* interesting
    #pragma warning(disable : 4820)

    #pragma warning(push, 1)
#endif // _MSC_VER

#ifdef __clang__
    _Pragma("clang diagnostic push")                                             
    _Pragma("clang diagnostic ignored \"-Wdisabled-macro-expansion\"") 
    _Pragma("clang diagnostic ignored \"-Wlanguage-extension-token\"")     
    _Pragma("clang diagnostic ignored \"-Wc++98-compat-pedantic\"")    
    _Pragma("clang diagnostic ignored \"-Wfloat-equal\"")  
    _Pragma("clang diagnostic ignored \"-Wmissing-variable-declarations\"")
    _Pragma("clang diagnostic ignored \"-Wreserved-id-macro\"")
#endif // __clang


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

    #if defined(CLOCK_PROCESS_CPUTIME_ID) && defined(CLOCK_MONOTONIC)
        #define MUON_HAS_POSIX_TIMER_    1
    #endif // CLOCK_PROCESS_CPUTIME_ID
#endif // unix

#if defined(_gnu_linux_) || defined(__linux__)
    #define MUON_LINUX_      1
    #include <fcntl.h>
    #include <sys/stat.h>
#endif // _gnu_linux_

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    #define MUON_WIN_        1
    #pragma warning(push, 0)
        #include <Windows.h>
        #include <io.h>
    #pragma warning(pop)
#endif // _WIN32

#ifdef __cplusplus
    #include <exception>
#endif // __cplusplus

#ifdef __has_include
    #if __has_include(<valgrind.h>)
        #include <valgrind.h>
    #endif // __has_include(<valgrind.h>)
#endif // __has_include

#ifdef __cplusplus
    #define MUON_C_FUNC extern "C"
    #define MUON_EXTERN extern "C"
#else
    #define MUON_C_FUNC
    #define MUON_EXTERN    extern
#endif // __cplusplus

// Enable the use of the non-standard keyword __attribute__ to silence warnings under some compilers
#if defined(__GNUC__) || defined(__clang__)
    #define MUON_ATTRIBUTE_(attr)    __attribute__((attr))
#else
    #define MUON_ATTRIBUTE_(attr)
#endif // __GNUC__

#ifdef __cplusplus
    // On C++, default to its polymorphism capabilities
    #define MUON_OVERLOADABLE
#elif defined(__clang__)
    // If we're still in C, use the __attribute__ keyword for Clang
    #define MUON_OVERLOADABLE   __attribute__((overloadable))
#endif // __cplusplus

#if defined(_MSC_VER)
    #define MUON_WEAK     inline
    #define MUON_UNUSED
#else
    #define MUON_WEAK     __attribute__((weak))
    #define MUON_UNUSED   __attribute__((unused))
#endif // _MSC_VER

typedef void (*muon_testsuite_t)();
struct muonTestSuiteStruct {
    muon_testsuite_t func;
    char* name;
};

struct muonTestStateStruct {
    struct muonTestSuiteStruct* tests;
    MUON_Ull numTestSuites;
    FILE* foutput;
};

static MUON_UInt64 muonStatsTotalTestSuites = 0;
static MUON_UInt64 muonStatsTestsRan = 0;
static MUON_UInt64 muonStatsNumTestsFailed = 0;
static MUON_UInt64 muonStatsSkippedTests = 0; 
static MUON_Ull* muonStatsFailedTestSuites = MUON_NULL; 
static MUON_Ull muonStatsNumFailedTestSuites = 0;

// Overridden in `muon_main` if the cmdline option `--no-color` is passed
static int muonShouldColourizeOutput = 1;
static int muonDisableSummary = 0;
static int muonDisplayOnlyFailedOutput = 0; 

static char* muon_argv0_ = MUON_NULL;
static const char* filter = MUON_NULL;

// This helps us determine whether a CHECK or a REQUIRE are being called from within or outside a 
// a Test Suite. Muon supports both - so we need to handle this
// We could have determined this somehow from within a function, but this is cleaner a cleaner approach
// (which is the aim on Muon).
// 
// Inside the TEST() initializer, this is set to true (because we are inside a Test Suite), so the 
// CHECKs and REQUIREs will do their thing and return the appropriate result. 
// If the assertion macro is not within the TEST() scope, it simply does not return anything - it only
// resets it back to false so that this same process occurs for the rest of the checks.
static volatile int checkIsInsideTestSuite;
static volatile int hasCurrentTestFailed;

static void failIfInsideTestSuite();
// This function is called from within a macro in the format {CHECK|REQUIRE)_*
// If we are inside a test suite _and_ a check fails, we need to be able to signal to Muon to handle this
// appropriately - fail the current test suite and carry on with the other checks (or move on to the next
// suite in the case of a REQUIRE)
static void failIfInsideTestSuite() {
    if(checkIsInsideTestSuite== 1) {
        hasCurrentTestFailed = 1;
    }
}

// extern to the global state muon needs to execute
MUON_EXTERN struct muonTestStateStruct muonTestContext;

#if defined(_MSC_VER)
    #ifndef MUON_USE_OLD_QPC
        typedef LARGE_INTEGER MUON_LARGE_INTEGER;
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
        } MUON_LARGE_INTEGER;

        MUON_C_FUNC __declspec(dllimport) int 
        __stdcall QueryPerformanceCounter(MUON_LARGE_INTEGER*);
        MUON_C_FUNC __declspec(dllimport) int 
        __stdcall QueryPerformanceFrequency(MUON_LARGE_INTEGER*);
    #endif // MUON_USE_OLD_QPC

#elif defined(__linux__)
    // We need to include glibc's features.h, but we don't want to just include a header that might not be 
    // defined for other C libraries like musl. 
    // Instead we include limits.h, which I know all glibc distributions include features.h
    #include <limits.h>

    #if defined(__GLIBC__) && defined(__GLIBC_MINOR__)
        #include <time.h>

        #if((__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ >= 17)))
            // glibc is version 2.17 or above, so we can just use clock_gettime
            #define MUON_USE_CLOCKGETTIME
        #else
            #include <sys/syscall.h>
            #include <unistd.h>
        #endif // __GLIBC__

    #else // Other libc implementations
        #include <time.h>
        #define MUON_USE_CLOCKGETTIME
    #endif // __GLIBC__

#elif defined(__APPLE__)
    #include <mach/mach_time.h>
#endif // _MSC_VER

// Muon Timer 
// This method is useful in timing the execution of an Muon Test Suite
// To use this, simply call this function before and after the particular code block you want to time, 
// and their difference will give you the time (in seconds). 
// NOTE: This method has been edited to return the time (in nanoseconds). Depending on how large this value
// (e.g: 54890938849ns), we appropriately convert it to milliseconds/seconds before displaying it to stdout.
static inline double muonClock() {
#ifdef MUON_WIN_
    LARGE_INTEGER counter;
    LARGE_INTEGER frequency;
    QueryPerformanceCounter(&counter);
    QueryPerformanceFrequency(&frequency);
    return MUON_CAST(double, (counter.QuadPart * 1000 * 1000 * 1000) / frequency.QuadPart); // in nanoseconds

#elif defined(__linux) && defined(__STRICT_ANSI__)
    return MUON_CAST(double, clock()) * 1000000000 / CLOCKS_PER_SEC; // in nanoseconds 

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
    return MUON_CAST(double, ts.tv_sec) * 1000 * 1000 * 1000 + ts.tv_nsec; // in nanoseconds

#elif __APPLE__
    return MUON_CAST(double, mach_absolute_time());
#endif // MUON_WIN_
}

static void muonClockPrintDuration(double nanoseconds_duration) {
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

// MUON_TEST_INITIALIZER
#if defined(_MSC_VER)
    #if defined(_WIN64)
        #define MUON_SYMBOL_PREFIX
    #else
        #define MUON_SYMBOL_PREFIX "_"
    #endif // _WIN64

    #pragma section(".CRT$XCU", read)
    #define MUON_TEST_INITIALIZER(f)                                                     \
    static void __cdecl f(void);                                                         \
        __pragma(comment(linker, "/include:" MUON_SYMBOL_PREFIX #f "_"))                 \
        MUON_C_FUNC __declspec(allocate(".CRT$XCU"))    void(__cdecl * f##_)(void) = f;  \
    static void __cdecl f(void)
#else
    #define MUON_TEST_INITIALIZER(f)                            \
        static void f(void)     __attribute__((constructor));   \
        static void f(void)
#endif // _MSC_VER


static inline void* muon_realloc(void* const ptr, MUON_Ull new_size) {
    void* const new_ptr = realloc(ptr, new_size);

    if(new_ptr == MUON_NULL)
        free(new_ptr);

    return new_ptr;
}


#define MUON_COLOUR_DEFAULT_              0
#define MUON_COLOUR_RED_                  1
#define MUON_COLOUR_GREEN_                2
#define MUON_COLOUR_YELLOW_               4
#define MUON_COLOUR_BLUE_                 5
#define MUON_COLOUR_CYAN_                 6
#define MUON_COLOUR_BRIGHTRED_            7
#define MUON_COLOUR_BRIGHTGREEN_          8
#define MUON_COLOUR_BRIGHTYELLOW_         9
#define MUON_COLOUR_BRIGHTBLUE_           10
#define MUON_COLOUR_BRIGHTCYAN_           11
#define MUON_COLOUR_BOLD_                 12

static inline int MUON_ATTRIBUTE_(format (printf, 2, 3))
muonColouredPrintf(int colour, const char* fmt, ...);
static inline int MUON_ATTRIBUTE_(format (printf, 2, 3))
muonColouredPrintf(int colour, const char* fmt, ...) {
    va_list args;
    char buffer[256];
    int n;

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    buffer[sizeof(buffer)-1] = '\0';

    if(!muonShouldColourizeOutput) {
        return printf("%s", buffer);
    }

#ifdef MUON_UNIX_
    {
        const char* str;
        switch(colour) {
            case MUON_COLOUR_RED_:          str = "\033[0;31m"; break;
            case MUON_COLOUR_GREEN_:        str = "\033[0;32m"; break;
            case MUON_COLOUR_YELLOW_:       str = "\033[0;33m"; break;
            case MUON_COLOUR_BLUE_:         str = "\033[0;34m"; break;
            case MUON_COLOUR_CYAN_:         str = "\033[0;36m"; break;
            case MUON_COLOUR_BRIGHTRED_:    str = "\033[1;31m"; break;
            case MUON_COLOUR_BRIGHTGREEN_:  str = "\033[1;32m"; break;
            case MUON_COLOUR_BRIGHTYELLOW_: str = "\033[1;33m"; break;
            case MUON_COLOUR_BRIGHTBLUE_:   str = "\033[1;34m"; break;
            case MUON_COLOUR_BRIGHTCYAN_:   str = "\033[1;36m"; break;
            case MUON_COLOUR_BOLD_:         str = "\033[1m"; break;
            default:                        str = "\033[0m"; break;
        }
        printf("%s", str);
        n = printf("%s", buffer);
        printf("\033[0m"); // Reset the colour
        return n;
    }
#elif defined(MUON_WIN_)
    {
        HANDLE h;
        CONSOLE_SCREEN_BUFFER_INFO info;
        WORD attr;

        h = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(h, &info);

        switch(colour) {
            case MUON_COLOUR_RED_:            attr = FOREGROUND_RED; break;
            case MUON_COLOUR_GREEN_:          attr = FOREGROUND_GREEN; break;
            case MUON_COLOUR_BLUE_:           attr = FOREGROUND_BLUE; break;
            case MUON_COLOUR_CYAN_:           attr = FOREGROUND_BLUE | FOREGROUND_GREEN; break;
            case MUON_COLOUR_YELLOW_:         attr = FOREGROUND_RED | FOREGROUND_GREEN; break;
            case MUON_COLOUR_BRIGHTRED_:      attr = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            case MUON_COLOUR_BRIGHTGREEN_:    attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case MUON_COLOUR_BRIGHTCYAN_:     attr = FOREGROUND_BLUE | FOREGROUND_GREEN | 
                                                     FOREGROUND_INTENSITY; break;
            case MUON_COLOUR_BRIGHTYELLOW_:   attr = FOREGROUND_RED | FOREGROUND_GREEN | 
                                                     FOREGROUND_INTENSITY; break;
            case MUON_COLOUR_BOLD_:           attr = FOREGROUND_BLUE | FOREGROUND_GREEN |
                                                     FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            default:                          attr = 0; break;
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
#endif // MUON_UNIX_
}


#define muonPrintf(...)                                  \
    if(muonTestContext.foutput)                          \
        fprintf(muonTestContext.foutput, __VA_ARGS__);   \
    printf(__VA_ARGS__)


#ifdef _MSC_VER
    #define MUON_SNPRINTF(BUFFER, N, ...)   _snprintf_s(BUFFER, N, N, __VA_ARGS__)
#else
    #define MUON_SNPRINTF(...)              snprintf(__VA_ARGS__)
#endif // _MSC_VER


#ifdef MUON_OVERLOADABLE
    #ifndef MUON_CAN_USE_OVERLOADABLES
        #define MUON_CAN_USE_OVERLOADABLES
    #endif // MUON_CAN_USE_OVERLOADABLES

    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(float f);
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(double d);
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(long double d);
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(int i);
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(unsigned int i);
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(long int i);
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(long unsigned int i);
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(const void* p);

    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(float f) { muonPrintf("%f", MUON_CAST(double, f)); }
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(double d) { muonPrintf("%f", d); }
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(long double d) { muonPrintf("%Lf", d); }
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(int i) { muonPrintf("%d", i); }
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(unsigned int i) { muonPrintf("%u", i); }
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(long int i) { muonPrintf("%ld", i); }
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(long unsigned int i) { muonPrintf("%lu", i); }
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(const void* p) { muonPrintf("%p", p); }

    // long long is in C++ only
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) || defined(__cplusplus) && (__cplusplus >= 201103L)
        MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(long long int i);
        MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(long long unsigned int i);

        MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(long long int i) { muonPrintf("%lld", i); }
        MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(long long unsigned int i) { muonPrintf("%llu", i); }
    #endif // __STDC_VERSION__

#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    #ifndef MUON_CAN_USE_OVERLOADABLES
        #define MUON_CAN_USE_OVERLOADABLES
    #endif // MUON_CAN_USE_OVERLOADABLES
    
    #define MUON_OVERLOAD_PRINTER(val)                                \
        muonPrintf(_Generic((val),                                   \
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
                                MUON_Ull : "%p",                       \
                                default : "undef")),                  \
                    (val))
#else
    // If we're here, this means that the Compiler does not support overloadable methods
    #define MUON_OVERLOAD_PRINTER(...)                                                              \
        muonPrintf("Error: Your compiler does not support overloadable methods.");                 \
        muonPrintf("If you think this was an error, please file an issue on Muon's Github repo.")
#endif // MUON_OVERLOADABLE

// ifCondFailsThenPrint is the string representation of the opposite of the truthy value of `cond`
// For example, if `cond` is "!=", then `ifCondFailsThenPrint` will be `==`
#if defined(MUON_CAN_USE_OVERLOADABLES)
    #define __MUONCHECK__(actual, expected, cond, ifCondFailsThenPrint)       \
        do {                                                                  \
            if(!((actual)cond(expected))) {                                   \
                muonPrintf("%s:%u: ", __FILE__, __LINE__);                   \
                muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");\
                muonPrintf("  Expected : ");                                 \
                MUON_OVERLOAD_PRINTER(actual);                                \
                printf(" %s ", #cond);                                        \
                MUON_OVERLOAD_PRINTER(expected);                              \
                muonPrintf("\n");                                            \
                                                                              \
                muonPrintf("    Actual : ");                                 \
                MUON_OVERLOAD_PRINTER(actual);                                \
                printf(" %s ", #ifCondFailsThenPrint);                        \
                MUON_OVERLOAD_PRINTER(expected);                              \
                muonPrintf("\n");                                            \
                failIfInsideTestSuite();                                      \
            }                                                                 \
        }                                                                     \
        while(0)

// MUON_OVERLOAD_PRINTER does not work on some compilers
#else
    #define __MUONCHECK__(actual, expected, cond, ifCondFailsThenPrint)       \
        do {                                                                  \
            if(!((actual)cond(expected))) {                                   \
                failIfInsideTestSuite();                                      \
                muonPrintf("%s:%u: ", __FILE__, __LINE__);                   \
                muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");\
                muonPrintf("  Expected : ");                                 \
                printf(#actual);                                              \
                printf(" %s ", #cond);                                        \
                printf(#expected);                                            \
                muonPrintf("\n");                                            \
                                                                              \
                muonPrintf("    Actual : ");                                 \
                printf(#actual);                                              \
                printf(" %s ", #ifCondFailsThenPrint);                        \
                printf(#expected);                                            \
                muonPrintf("\n");                                            \
            }                                                                 \
        }                                                                     \
        while(0)
#endif // MUON_CAN_USE_OVERLOADABLES


//
// #########################################
//            Check Macros
// #########################################
//
#define CHECK_EQ(actual, expected)     __MUONCHECK__(actual, expected, ==, !=)
#define CHECK_NE(actual, expected)     __MUONCHECK__(actual, expected, !=, ==)
#define CHECK_LT(actual, expected)     __MUONCHECK__(actual, expected, <,  >)
#define CHECK_LE(actual, expected)     __MUONCHECK__(actual, expected, <=, >=)
#define CHECK_GT(actual, expected)     __MUONCHECK__(actual, expected, >,  <)
#define CHECK_GE(actual, expected)     __MUONCHECK__(actual, expected, >=, <=)

#define CHECK(cond, ...)                                                         \
    do {                                                                         \
        if(!(cond)) {                                                            \
            muonPrintf("%s:%u: ", __FILE__, __LINE__);                          \
            if((sizeof(char[]){__VA_ARGS__}) <= 1)                               \
                muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED");         \
            else                                                                 \
                muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, __VA_ARGS__);      \
            printf("\n");                                                        \
            muonColouredPrintf(MUON_COLOUR_YELLOW_, "The following check failed: \n"); \
            muonColouredPrintf(MUON_COLOUR_BRIGHTCYAN_, "   CHECK( %s )\n", #cond); \
            failIfInsideTestSuite();                                             \
        }                                                                        \
    }                                                                            \
    while(0)

#define CHECK_TRUE(cond)                                                      \
    do {                                                                      \
        if(!(cond)) {                                                         \
            muonPrintf("%s:%u: ", __FILE__, __LINE__);                       \
            muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");    \
            muonPrintf("  Expected : true\n");                               \
            muonPrintf("    Actual : false\n");                              \
            failIfInsideTestSuite();                                          \
        }                                                                     \
    } while(0)


#define CHECK_FALSE(cond)                                                     \
    do {                                                                      \
        if((cond)) {                                                          \
            muonPrintf("%s:%u: ", __FILE__, __LINE__);                       \
            muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");    \
            muonPrintf("  Expected : false\n");                              \
            muonPrintf("    Actual : true\n");                               \
            failIfInsideTestSuite();                                          \
        }                                                                     \
    } while(0)
 

// String Macros
#define CHECK_STREQ(actual, expected)                                         \
    do {                                                                      \
        if(strcmp(actual, expected) != 0) {                                   \
            muonPrintf("%s:%u: ", __FILE__, __LINE__);                       \
            muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");    \
            muonPrintf("  Expected : \"%s\" == \"%s\"\n", actual, expected); \
            muonPrintf("    Actual : not equal\n"); \
            failIfInsideTestSuite();                                          \
        }                                                                     \
    }                                                                         \
    while(0)                                                                    

// Compare two strings for no equality.
// Fails the test if `actual` and `expected` are the same strings.
#define CHECK_STRNEQ(actual, expected)                                        \
    do {                                                                      \
        if(strcmp(actual, expected) == 0) {                                   \
            muonPrintf("%s:%u: ", __FILE__, __LINE__);                       \
            muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");    \
            muonPrintf("  Expected : \"%s\" != \"%s\"\n", actual, expected); \
            muonPrintf("    Actual : equal\n"); \
            failIfInsideTestSuite();                                          \
        }                                                                     \
  }                                                                           \
  while(0)                                                                    


#define CHECK_STRNNEQ(actual, expected, n)                                                   \
    do {                                                                                     \
        if(strncmp(actual, expected, n) == 0) {                                              \
            muonPrintf("%s:%u: ", __FILE__, __LINE__);                                      \
            muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");                   \
            muonPrintf("  Expected : \"%.*s\" != \"%.*s\"\n", MUON_CAST(int, n), actual,    \
                                                               MUON_CAST(int, n), expected); \
            muonPrintf("    Actual : equal subtrings\n");                                   \
            failIfInsideTestSuite();                                                         \
        }                                                                                    \
    }                                                                                        \
    while(0)                                                                    

//
// #########################################
//            Assertion Macros
// #########################################
//

// ifCondFailsThenPrint is the string representation of the opposite of the truthy value of `cond`
// For example, if `cond` is "!=", then `ifCondFailsThenPrint` will be `==`
#if defined(MUON_CAN_USE_OVERLOADABLES)
    #define __MUONREQUIRE__(actual, expected, cond, ifCondFailsThenPrint)     \
        do {                                                                  \
            if(!((actual)cond(expected))) {                                   \
                muonPrintf("%s:%u: ", __FILE__, __LINE__);                   \
                muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");\
                muonPrintf("  Expected : ");                                 \
                MUON_OVERLOAD_PRINTER(actual);                                \
                muonPrintf(" %s ", #cond);                                   \
                MUON_OVERLOAD_PRINTER(expected);                              \
                muonPrintf("\n");                                            \
                                                                              \
                muonPrintf("    Actual : ");                                 \
                MUON_OVERLOAD_PRINTER(actual);                                \
                muonPrintf(" %s ", #ifCondFailsThenPrint);                   \
                MUON_OVERLOAD_PRINTER(expected);                              \
                muonPrintf("\n");                                            \
                failIfInsideTestSuite();    \
                return;                                                       \
            }                                                                 \
        }                                                                     \
        while(0)

#else
    #define __MUONREQUIRE__(actual, expected, cond, ifCondFailsThenPrint)     \
        do {                                                                  \
            if(!((actual)cond(expected))) {                                   \
                muonPrintf("%s:%u: ", __FILE__, __LINE__);                   \
                muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");\
                muonPrintf("  Expected : ");                                 \
                muonPrintf(#actual);                                         \
                muonPrintf(" %s ", #cond);                                   \
                muonPrintf(#expected);                                       \
                muonPrintf("\n");                                            \
                                                                              \
                muonPrintf("    Actual : ");                                 \
                muonPrintf(#actual);                                         \
                muonPrintf(" %s ", #ifCondFailsThenPrint);                   \
                muonPrintf(#expected);                                       \
                muonPrintf("\n");                                            \
                failIfInsideTestSuite();                                      \
                return;                                                       \
            }                                                                 \
        }                                                                     \
        while(0)                                                                    
#endif // MUON_CAN_USE_OVERLOADABLES


#define REQUIRE_EQ(actual, expected)     __MUONREQUIRE__(actual, expected, ==, !=)
#define REQUIRE_NE(actual, expected)     __MUONREQUIRE__(actual, expected, !=, ==)
#define REQUIRE_LT(actual, expected)     __MUONREQUIRE__(actual, expected, <,  >)
#define REQUIRE_LE(actual, expected)     __MUONREQUIRE__(actual, expected, <=, >=)
#define REQUIRE_GT(actual, expected)     __MUONREQUIRE__(actual, expected, >,  <)
#define REQUIRE_GE(actual, expected)     __MUONREQUIRE__(actual, expected, >=, <=)

#define REQUIRE(cond, ...)                                                       \
    do {                                                                         \
        if(!(cond)) {                                                            \
            muonPrintf("%s:%u: ", __FILE__, __LINE__);                          \
            if((sizeof(char[]){__VA_ARGS__}) <= 1)                               \
                muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED");     \
            else                                                                 \
                muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, __VA_ARGS__);  \
            printf("\n");                                                        \
            failIfInsideTestSuite();                                             \
        }                                                                        \
    }                                                                            \
    while(0)

#define REQUIRE_TRUE(cond)                                                    \
    do {                                                                      \
        if(!(cond)) {                                                         \
            muonPrintf("%s:%u: ", __FILE__, __LINE__);                       \
            muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");    \
            muonPrintf("  Expected : true\n");                               \
            muonPrintf("    Actual : false\n");                              \
            failIfInsideTestSuite();                                          \
        }                                                                     \
    } while(0)


#define REQUIRE_FALSE(cond)                                                   \
    do {                                                                      \
        if((cond)) {                                                          \
            muonPrintf("%s:%u: ", __FILE__, __LINE__);                       \
            muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");    \
            muonPrintf("  Expected : false\n");                              \
            muonPrintf("    Actual : true\n");                               \
            failIfInsideTestSuite();                                          \
        }                                                                     \
    } while(0)

#define REQUIRE_STREQ(actual, expected)                                       \
    do {                                                                      \
        if(strcmp(actual, expected) != 0) {                                   \
            muonPrintf("%s:%u: ", __FILE__, __LINE__);                       \
            muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");    \
            muonPrintf("  Expected : \"%s\" == \"%s\"\n", actual, expected); \
            muonPrintf("    Actual : not equal\n");                          \
            failIfInsideTestSuite();                                          \
            return;                                                           \
        }                                                                     \
    }                                                                         \
    while(0)                                                                    

#define REQUIRE_STRNEQ(actual, expected)                                      \
    do {                                                                      \
        if(strcmp(actual, expected) == 0) {                                   \
            muonPrintf("%s:%u: ", __FILE__, __LINE__);                       \
            muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");    \
            muonPrintf("  Expected : \"%s\" != \"%s\"\n", actual, expected); \
            muonPrintf("    Actual : equal\n");                              \
            failIfInsideTestSuite();                                          \
            return;                                                           \
        }                                                                     \
    }                                                                         \
    while(0)                                                                    


#define REQUIRE_STRNNEQ(actual, expected, n)                                                 \
    do {                                                                                     \
        if(strncmp(actual, expected, n) == 0) {                                              \
            muonPrintf("%s:%u: ", __FILE__, __LINE__);                                      \
            muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");                   \
            muonPrintf("  Expected : \"%.*s\" != \"%.*s\"\n", MUON_CAST(int, n), actual,    \
                                                               MUON_CAST(int, n), expected); \
            muonPrintf("    Actual : equal subtrings\n");                                   \
            failIfInsideTestSuite();                                                         \
            return;                                                                          \
        }                                                                                    \
    }                                                                                        \
    while(0)                                                                    
    
//
// #########################################
//              Implementation
// #########################################
//

#define TEST(TESTSUITE, TESTNAME)                                                       \
    MUON_EXTERN struct muonTestStateStruct muonTestContext;                                         \
    static void _MUON_TEST_FUNC_##TESTSUITE##_##TESTNAME();                    \
    MUON_TEST_INITIALIZER(muon_register_##TESTSUITE##_##TESTNAME) {                     \
        const MUON_Ull index = muonTestContext.numTestSuites++;                                   \
        const char* name_part = #TESTSUITE "." #TESTNAME;                               \
        const MUON_Ull name_size = strlen(name_part) + 1;                                \
        char* name = MUON_PTRCAST(char* , malloc(name_size));                           \
        muonTestContext.tests = MUON_PTRCAST(                                                \
                                    struct muonTestSuiteStruct* ,                                                       \
                                    muon_realloc(MUON_PTRCAST(void* , muonTestContext.tests), \
                                                sizeof(struct muonTestSuiteStruct) *             \
                                                    muonTestContext.numTestSuites));       \
        muonTestContext.tests[index].func = &_MUON_TEST_FUNC_##TESTSUITE##_##TESTNAME;                  \
        muonTestContext.tests[index].name = name;                                            \
        MUON_SNPRINTF(name, name_size, "%s", name_part);                                \
    }                                                                                   \
    void _MUON_TEST_FUNC_##TESTSUITE##_##TESTNAME()


#define TEST_F_SETUP(FIXTURE)                                                 \
    static void muon_f_setup_##FIXTURE(struct FIXTURE* muon)

#define TEST_F_TEARDOWN(FIXTURE)                                              \
    static void muon_f_teardown_##FIXTURE(struct FIXTURE* muon)

#define TEST_F(FIXTURE, NAME)                                                 \
    MUON_EXTERN struct muonTestStateStruct muonTestContext;                                 \
    static void muon_f_setup_##FIXTURE(struct FIXTURE*);                \
    static void muon_f_teardown_##FIXTURE(struct FIXTURE*);             \
    static void muon_run_##FIXTURE##_##NAME(struct FIXTURE*);           \
    static void muon_f_##FIXTURE##_##NAME() {                 \
        struct FIXTURE fixture;                                                    \
        memset(&fixture, 0, sizeof(fixture));                                      \
        muon_f_setup_##FIXTURE(&fixture);                           \
        if (1!= 0) { return; }                                              \
\
        muon_run_##FIXTURE##_##NAME(&fixture);                      \
        muon_f_teardown_##FIXTURE(&fixture);                        \
    }                                                                            \
    MUON_TEST_INITIALIZER(muon_register_##FIXTURE##_##NAME) {                       \
        const MUON_Ull index = muonTestContext.numTestSuites++;                           \
        const char* name_part = #FIXTURE "." #NAME;                                \
        const MUON_Ull name_size = strlen(name_part) + 1;                            \
        char* name = MUON_PTRCAST(char* , malloc(name_size));                    \
        muonTestContext.tests = MUON_PTRCAST(                                        \
                                    struct muonTestSuiteStruct*,                                           \
                                    muon_realloc(MUON_PTRCAST(void *, muonTestContext.tests),               \
                                                                sizeof(struct muonTestSuiteStruct) *               \
                                                                        muonTestContext.numTestSuites));   \
        muonTestContext.tests[index].func = &muon_f_##FIXTURE##_##NAME;               \
        muonTestContext.tests[index].name = name;                                      \
        MUON_SNPRINTF(name, name_size, "%s", name_part);                          \
    }                                                                            \
    static void muon_run_##FIXTURE##_##NAME(struct FIXTURE* muon)


MUON_WEAK int muonShouldFilterTest(const char* filter, const char* testcase);
MUON_WEAK int muonShouldFilterTest(const char* filter, const char* testcase) {
    if(filter) {
        const char* filter_curr = filter;
        const char* testcase_curr = testcase;
        const char* filter_wildcard = MUON_NULL;

        while ((*filter_curr != MUON_NULLCHAR) && (*testcase_curr != MUON_NULLCHAR)) {
            if('*' == *filter_curr) {
                // store the position of the wildcard
                filter_wildcard = filter_curr;

                // skip the wildcard character
                filter_curr++;

                while ((*filter_curr != MUON_NULLCHAR) && (*testcase_curr != MUON_NULLCHAR)) {
                    if(*filter_curr == '*') {
                        // Found another wildcard (filter is something like *foo*), so exit the current loop, 
                        // and return to the parent loop to handle the wildcard case
                        break;
                    } else if(*filter_curr != *testcase_curr) {
                        // otherwise our filter didn't match, so reset it
                        filter_curr = filter_wildcard;
                    }

                    // move testcase along
                    testcase_curr++;

                    // move filter along
                    filter_curr++;
                }

                if((*filter_curr == MUON_NULLCHAR) && (*testcase_curr == MUON_NULLCHAR))
                    return 0;

                // if the testcase has been exhausted, we don't have a match!
                if(*testcase_curr == MUON_NULLCHAR)
                    return 1;
            } else {
                if(*testcase_curr != *filter_curr) {
                    // test case doesn't match filter
                    return 1;
                } else {
                    // move our filter and testcase forward
                    testcase_curr++;
                    filter_curr++;
                }
            }
        }

        if((*filter_curr != MUON_NULLCHAR) || ((*testcase_curr == MUON_NULLCHAR) && ((filter == filter_curr) || (filter_curr[-1] != '*')))) {
            // We have a mismatch
            return 1;
        }
    }
    return 0;
}

static inline FILE* muon_fopen(const char* filename, const char* mode) {
#ifdef _MSC_VER
    FILE* file;
    if(fopen_s(&file, filename, mode) == 0)
        return file;
    else
        return MUON_NULL;
#else
    return fopen(filename, mode);
#endif // _MSC_VER
}

static void muon_help_() {
        printf("Usage: %s [options] [test...]\n", muon_argv0_);
        printf("\n");
        printf("Run the specified unit tests; or if the option '--skip' is used, run all\n");
        printf("tests in the suite but those listed. By default, if no tests are specified\n");
        printf("on the command line, all unit tests in the suite are run.\n");
        printf("\n");
        printf("Options:\n");
        printf("  --failed-output-only     Output only failed Test Suites");
        printf("  --filter=<filter>        Filter the test suites to run (e.g: Suite1*.a\n");
        printf("                             would run Suite1Case.a but not Suite1Case.b}\n");
    #if defined(MUON_WIN_)
        printf("  --time                   Measure test duration\n");
    #elif defined(MUON_HAS_POSIX_TIMER_)
        printf("  --time                   Measure test duration (real time)\n");
        printf("  --time=TIMER             Measure test duration, using given timer\n");
        printf("                               (TIMER is one of 'real', 'cpu')\n");
    #endif // MUON_WIN_
        printf("  --no-summary             Suppress printing of test results summary\n");
        printf("  --output=<FILE>          Write an XUnit XML file to Enable XUnit output\n");
        printf("                             to the given file\n");
        printf("  --list                   List unit tests in the suite and exit\n");
        printf("  --no-color               Disable coloured output\n");
        printf("  --help                   Display this help and exit\n");
}

static MUON_bool muonCmdLineRead(int argc, char** argv) {
    // Coloured output
#ifdef MUON_UNIX_
    muonShouldColourizeOutput = isatty(STDOUT_FILENO);
#elif defined MUON_WIN_
    #ifdef _BORLANDC_
        muonShouldColourizeOutput = isatty(_fileno(stdout));
    #else
        muonShouldColourizeOutput = _isatty(_fileno(stdout));
    #endif // _BORLANDC_
#else 
    muonShouldColourizeOutput = isatty(STDOUT_FILENO);
#endif // MUON_UNIX_

    // loop through all arguments looking for our options
    for(MUON_Ull i = 1; i < MUON_CAST(MUON_Ull, argc); i++) {
        /* Informational switches */
        const char* helpStr = "--help";
        const char* listStr = "--list";
        const char* colourStr = "--no-color";
        const char* summaryStr = "--no-summary";
        const char* onlyFailedOutput = "--failed-output-only";
        /* Test config switches */
        const char* filterStr = "--filter=";
        const char* XUnitOutput = "--output=";

        // Help
        if(strncmp(argv[i], helpStr, strlen(helpStr)) == 0) {
            muon_help_();
            return MUON_false;
        } 

        // Only failed output
        else if(strncmp(argv[i], onlyFailedOutput, strlen(onlyFailedOutput)) == 0) {
            muonDisplayOnlyFailedOutput = 1;
        }

        // Filter tests
        else if(strncmp(argv[i], filterStr, strlen(filterStr)) == 0)
            // user wants to filter what test suites run!
            filter = argv[i] + strlen(filterStr);

        // Write XUnit XML file
        else if(strncmp(argv[i], XUnitOutput, strlen(XUnitOutput)) == 0)
            muonTestContext.foutput = muon_fopen(argv[i] + strlen(XUnitOutput), "w+");

        // List tests
        else if(strncmp(argv[i], listStr, strlen(listStr)) == 0) {
            for (i = 0; i < muonTestContext.numTestSuites; i++)
                muonPrintf("%s\n", muonTestContext.tests[i].name);
        }

        // Disable colouring
        else if(strncmp(argv[i], colourStr, strlen(colourStr)) == 0) {            
            muonShouldColourizeOutput = 0;
        }

        // Disable Summary
        else if(strncmp(argv[i], summaryStr, strlen(summaryStr))) {
            muonDisableSummary = 1;
        }

        else {
            printf("ERROR: Unrecognized option: %s", argv[i]);
            return MUON_false;
        }
    }

    return MUON_true;
}

static int muonCleanup() {
    for (MUON_Ull i = 0; i < muonTestContext.numTestSuites; i++)
        free(MUON_PTRCAST(void* , muonTestContext.tests[i].name));

    free(MUON_PTRCAST(void* , muonStatsFailedTestSuites));
    free(MUON_PTRCAST(void* , muonTestContext.tests));

    if(muonTestContext.foutput)
        fclose(muonTestContext.foutput);

    return MUON_CAST(int, muonStatsNumTestsFailed);
}

// Triggers and runs all unit tests
// static void muonRunTests(MUON_Ull* muonStatsFailedTestSuites, MUON_Ull* muonStatsNumFailedTestSuites) {
static void muonRunTests() {
    // Run tests
    for(MUON_Ull i = 0; i < muonTestContext.numTestSuites; i++) {
        checkIsInsideTestSuite = 1; 
        hasCurrentTestFailed = 0;

        if(muonShouldFilterTest(filter, muonTestContext.tests[i].name))
            continue;

        if(!muonDisplayOnlyFailedOutput) {
            muonColouredPrintf(MUON_COLOUR_BRIGHTGREEN_, "[ RUN      ] ");
            muonColouredPrintf(MUON_COLOUR_DEFAULT_, "%s\n", muonTestContext.tests[i].name);
        }

        if(muonTestContext.foutput)
            fprintf(muonTestContext.foutput, "<testcase name=\"%s\">", muonTestContext.tests[i].name);

        // Start the timer
        double start = muonClock();

        // The actual test
        muonTestContext.tests[i].func();

        // Stop the timer
        double duration = muonClock() - start;
        
        if(muonTestContext.foutput)
            fprintf(muonTestContext.foutput, "</testcase>\n");

        if(hasCurrentTestFailed == 1) {
            const MUON_Ull failed_testcase_index = muonStatsNumFailedTestSuites++;
            muonStatsFailedTestSuites = MUON_PTRCAST(MUON_Ull*, 
                                            muon_realloc(MUON_PTRCAST(void* , muonStatsFailedTestSuites), 
                                                          sizeof(MUON_Ull) * muonStatsNumFailedTestSuites));
            muonStatsFailedTestSuites[failed_testcase_index] = i;
            muonStatsNumTestsFailed++;
            muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "[  FAILED  ] ");
            muonColouredPrintf(MUON_COLOUR_DEFAULT_, "%s (", muonTestContext.tests[i].name);
            muonClockPrintDuration(duration);
            printf(")\n");
        } else {
            if(!muonDisplayOnlyFailedOutput) {
                muonColouredPrintf(MUON_COLOUR_BRIGHTGREEN_, "[       OK ] ");
                muonColouredPrintf(MUON_COLOUR_DEFAULT_, "%s (", muonTestContext.tests[i].name);
                muonClockPrintDuration(duration);
                printf(")\n");
            }
        }
    }
    muonColouredPrintf(MUON_COLOUR_BRIGHTGREEN_, "[==========] ");
    muonColouredPrintf(MUON_COLOUR_DEFAULT_, "%" MUON_PRIu64 " test suites ran\n", muonStatsTestsRan);
}

static inline int muon_main(int argc, char** argv);
inline int muon_main(int argc, char** argv) {
    muonStatsTotalTestSuites = MUON_CAST(MUON_UInt64, muonTestContext.numTestSuites);
    muon_argv0_ = argv[0];

    // Start the entire Test Session timer
    double start = muonClock();

    MUON_bool wasCmdLineReadSuccessful = muonCmdLineRead(argc, argv);
    if(!wasCmdLineReadSuccessful) 
        return muonCleanup();

    for (MUON_Ull i = 0; i < muonTestContext.numTestSuites; i++) {
        if(muonShouldFilterTest(filter, muonTestContext.tests[i].name))
            muonStatsSkippedTests++;
    }

    muonStatsTestsRan = muonStatsTotalTestSuites - muonStatsSkippedTests;

    // Begin tests`
    muonColouredPrintf(MUON_COLOUR_BRIGHTGREEN_, "[==========] ");
    muonColouredPrintf(MUON_COLOUR_BOLD_, "Running %" MUON_PRIu64 " test suites.\n", MUON_CAST(MUON_UInt64, muonStatsTestsRan));

    if(muonTestContext.foutput) {
        fprintf(muonTestContext.foutput, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        fprintf(muonTestContext.foutput,
                "<testsuites tests=\"%" MUON_PRIu64 "\" name=\"All\">\n",
                MUON_CAST(MUON_UInt64, muonStatsTestsRan));
        fprintf(muonTestContext.foutput,
                "<testsuite name=\"Tests\" tests=\"%" MUON_PRIu64 "\">\n",
                MUON_CAST(MUON_UInt64, muonStatsTestsRan));
    }

    // Run tests
    muonRunTests();

    // End the entire Test Session timer
    double duration = muonClock() - start;

    // Write a Summary
    muonColouredPrintf(MUON_COLOUR_BRIGHTGREEN_, "[  PASSED  ] %" MUON_PRIu64 " %s\n", muonStatsTestsRan - muonStatsNumTestsFailed, muonStatsTestsRan - muonStatsNumTestsFailed == 1 ? "suite" : "suites");
    muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "[  FAILED  ] %" MUON_PRIu64 " %s\n", muonStatsNumTestsFailed, muonStatsNumTestsFailed == 1 ? "suite" : "suites");

    if(!muonDisableSummary) {
        muonColouredPrintf(MUON_COLOUR_BOLD_, "\nSummary:\n");

        printf("    Total test suites:      %" MUON_PRIu64 "\n", muonStatsTotalTestSuites);
        printf("    Total suites run:       %" MUON_PRIu64 "\n", muonStatsTestsRan);
        printf("    Total suites skipped:   %" MUON_PRIu64 "\n", muonStatsSkippedTests);
        printf("    Total suites failed:    %" MUON_PRIu64 "\n", muonStatsNumTestsFailed);
    }

    if(muonStatsNumTestsFailed != 0) {
        muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED: ");
        printf("%" MUON_PRIu64 " failed, %" MUON_PRIu64 " passed in ", muonStatsNumTestsFailed, muonStatsTestsRan - muonStatsNumTestsFailed);
        muonClockPrintDuration(duration);
        printf("\n");
        
        for (MUON_Ull i = 0; i < muonStatsNumFailedTestSuites; i++) {
            muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "  [ FAILED ] %s\n", muonTestContext.tests[muonStatsFailedTestSuites[i]].name);
        }
    } else {
        MUON_UInt64 total_tests_passed = muonStatsTestsRan - muonStatsNumTestsFailed;
        muonColouredPrintf(MUON_COLOUR_BRIGHTGREEN_, "SUCCESS: ");
        printf("%" MUON_PRIu64 " test suites passed in ", total_tests_passed);
        muonClockPrintDuration(duration);
        printf("\n");
    }

    if(muonTestContext.foutput)
        fprintf(muonTestContext.foutput, "</testsuite>\n</testsuites>\n");

    return muonCleanup();
}


// If a user wants to define their own `main()` function, this _must_ be at the very end of the functtion
#define MUON_NO_MAIN()                                       \
    struct muonTestStateStruct muonTestContext = {0, 0, 0};

// Define a main() function to call into muon.h and start executing tests.
#define MUON_MAIN()                                                             \
    /* Define the global struct that will hold the data we need to run Muon. */ \
    struct muonTestStateStruct muonTestContext = {0, 0, 0};                     \
                                                                                \
    int main(int argc, char** argv) {                                           \
        return muon_main(argc, argv);                                           \
    }

#ifdef __clang__
     _Pragma("clang diagnostic pop")
#endif // __clang__

#ifdef _MSC_VER
    #pragma warning(pop)
#endif // _MSC_VER

#endif // MUON_TEST_H_