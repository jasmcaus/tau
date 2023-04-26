/*
 _______           _    _
|__   __|  /\    | |  | |
   | |    /  \   | |  | |  Tau - The Micro Testing Framework for C/C++
   | |   / /\ \  | |  | |  Language: C
   | |  / ____ \ | |__| |  https://github.com/jasmcaus/tau
   |_| /_/    \_\ \____/
Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

#ifndef TAU_H_
#define TAU_H_

#include <tau/types.h>
#include <tau/misc.h>

TAU_DISABLE_DEBUG_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
    #define TAU_UNIX_   1
    #include <errno.h>
    #include <libgen.h>
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
    #include <signal.h>
    #include <time.h>

    #if defined(CLOCK_PROCESS_CPUTIME_ID) && defined(CLOCK_MONOTONIC)
        #define TAU_HAS_POSIX_TIMER_    1
    #endif // CLOCK_PROCESS_CPUTIME_ID
#endif // unix

#if defined(_gnu_linux_) || defined(__linux__)
    #define TAU_LINUX_      1
    #include <fcntl.h>
    #include <sys/stat.h>
#endif // _gnu_linux_

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    #define TAU_WIN_        1
    #pragma warning(push, 0)
        #include <windows.h>
        #include <io.h>
    #pragma warning(pop)
#endif // _WIN32

#ifdef __has_include
    #if __has_include(<valgrind.h>)
        #include <valgrind.h>
    #endif // __has_include(<valgrind.h>)
#endif // __has_include

#ifdef __cplusplus
    #define TAU_C_FUNC   extern "C"
    #define TAU_EXTERN   extern "C"
#else
    #define TAU_C_FUNC
    #define TAU_EXTERN   extern
#endif // __cplusplus

#if defined(__cplusplus)
    #include <exception>
#endif //__cplusplus

#if defined(__cplusplus)
    #define TAU_ABORT     std::abort()
#else
    #define TAU_ABORT     exit(1)
#endif //__cplusplus

// Enable the use of the non-standard keyword __attribute__ to silence warnings under some compilers
#if defined(__GNUC__) || defined(__clang__)
    #define TAU_ATTRIBUTE_(attr)    __attribute__((attr))
#else
    #define TAU_ATTRIBUTE_(attr)
#endif // __GNUC__

#ifdef __cplusplus
    // On C++, default to its polymorphism capabilities
    #define TAU_OVERLOADABLE
#elif defined(__clang__)
    // If we're still in C, use the __attribute__ keyword for Clang
    #define TAU_OVERLOADABLE   __attribute__((overloadable))
#endif // __cplusplus

#if defined(_MSC_VER) || defined(__cplusplus)
    #define TAU_WEAK     inline
    #define TAU_UNUSED
#else
    #define TAU_WEAK     __attribute__((weak))
    #define TAU_UNUSED   __attribute__((unused))
#endif // _MSC_VER

#ifndef TAU_NO_TESTING

typedef void (*tau_testsuite_t)();
typedef struct tauTestSuiteStruct {
    tau_testsuite_t func;
    char* name;
} tauTestSuiteStruct;

typedef struct tauTestStateStruct {
    tauTestSuiteStruct* tests;
    tau_ull numTestSuites;
    FILE* foutput;
} tauTestStateStruct;

static tau_u64 tauStatsTotalTestSuites = 0;
static tau_u64 tauStatsTestsRan = 0;
static tau_u64 tauStatsNumTestsFailed = 0;
static tau_u64 tauStatsSkippedTests = 0;
static tau_ull* tauStatsFailedTestSuites = TAU_NULL;
static tau_ull tauStatsNumFailedTestSuites = 0;
extern tau_u64 tauStatsNumWarnings;

// Overridden in `tau_main` if the cmdline option `--no-color` is passed
static int tauShouldColourizeOutput = 1;
static int tauDisableSummary = 0;
static int tauDisplayOnlyFailedOutput = 0;
static int tauDisplayTests = 0;

static const char* tau_argv0_ = TAU_NULL;
static const char* cmd_filter = TAU_NULL;
#endif // TAU_NO_TESTING

/**
    This helps us determine whether a CHECK or a REQUIRE are being called from within (or outside)
    a Test Suite. Tau supports both - so we need to handle this.

    We could have somehow determined this from within a function, but this is a cleaner approach
    (which is the Tau's aim).

    Inside the TEST() initializer, this is set to `true` (because we are inside a Test Suite), so the
    `CHECK`s and `REQUIRE`s will do their thing and return the appropriate result.
    If the assertion macro is not within the `TEST()` scope, it simply does not return anything - it only
    resets it back to false so that this same process occurs for the rest of the checks.
*/
extern volatile int checkIsInsideTestSuite;
extern volatile int hasCurrentTestFailed;

#ifndef TAU_NO_TESTING
extern volatile int shouldFailTest;
extern volatile int shouldAbortTest;

/**
    This function is called from within a macro in the format {CHECK|REQUIRE)_*
    If we are inside a test suite _and_ a check fails, we need to be able to signal to Tau to handle this
    appropriately - fail the current test suite and carry on with the other checks (or move on to the next
    suite in the case of a REQUIRE)
*/
static void failIfInsideTestSuite__();
static void abortIfInsideTestSuite__();

static void failIfInsideTestSuite__() {
    if(checkIsInsideTestSuite == 1) {
        hasCurrentTestFailed = 1;
        shouldFailTest = 1;
    }
}

static void abortIfInsideTestSuite__() {
    if(checkIsInsideTestSuite == 1) {
        hasCurrentTestFailed = 1;
        shouldAbortTest = 1;
    }
}

#endif // TAU_NO_TESTING

static void incrementWarnings() {
#ifndef TAU_NO_TESTING
    tauStatsNumWarnings++;
#endif // TAU_NO_TESTING
}

#ifndef TAU_NO_TESTING
// extern to the global state tau needs to execute
TAU_EXTERN tauTestStateStruct tauTestContext;

#if defined(_MSC_VER)
    #ifndef TAU_USE_OLD_QPC
        typedef LARGE_INTEGER TAU_LARGE_INTEGER;
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
        } TAU_LARGE_INTEGER;

        TAU_C_FUNC __declspec(dllimport) int __stdcall QueryPerformanceCounter(TAU_LARGE_INTEGER*);
        TAU_C_FUNC __declspec(dllimport) int __stdcall QueryPerformanceFrequency(TAU_LARGE_INTEGER*);
    #endif // TAU_USE_OLD_QPC

#elif defined(__linux__)
    // We need to include glibc's features.h, but we don't want to just include a header that might not be
    // defined for other C libraries like musl.
    // Instead we include limits.h, which I know all glibc distributions include features.h
    #include <limits.h>

    #if defined(__GLIBC__) && defined(__GLIBC_MINOR__)
        #include <time.h>

        #if((__GLIBC__ > 2) || ((__GLIBC__ == 2) && (__GLIBC_MINOR__ >= 17)))
            // glibc is version 2.17 or above, so we can just use clock_gettime
            #define TAU_USE_CLOCKGETTIME
        #else
            #include <sys/syscall.h>
            #include <unistd.h>
        #endif // __GLIBC__

    #else // Other libc implementations
        #include <time.h>
        #define TAU_USE_CLOCKGETTIME
    #endif // __GLIBC__

#elif defined(__APPLE__)
    #include <mach/mach_time.h>
#endif // _MSC_VER

/**
    Tau Timer
    This method is useful in timing the execution of an Tau Test Suite
    To use this, simply call this function before and after the particular code block you want to time,
    and their difference will give you the time (in seconds).
    NOTE: This method has been edited to return the time (in nanoseconds). Depending on how large this value
    (e.g: 54890938849ns), we appropriately convert it to milliseconds/seconds before displaying it to stdout.
*/
static inline double tauClock() {
#ifdef TAU_WIN_
    LARGE_INTEGER counter;
    LARGE_INTEGER frequency;
    QueryPerformanceCounter(&counter);
    QueryPerformanceFrequency(&frequency);
    return TAU_CAST(double, (counter.QuadPart * 1000 * 1000 * 1000) / frequency.QuadPart); // in nanoseconds

#elif defined(__linux) && defined(__STRICT_ANSI__)
    return TAU_CAST(double, clock()) * 1000000000 / CLOCKS_PER_SEC; // in nanoseconds

#elif defined(__linux)
    struct timespec ts;
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
        timespec_get(&ts, TIME_UTC);
    #else
        const clockid_t cid = CLOCK_REALTIME;
        #if defined(TAU_USE_CLOCKGETTIME)
            clock_gettime(cid, &ts);
        #else
            syscall(SYS_clock_gettime, cid, &ts);
        #endif
    #endif
    return TAU_CAST(double, ts.tv_sec) * 1000 * 1000 * 1000 + TAU_CAST(double, ts.tv_nsec); // in nanoseconds

#elif __APPLE__
    return TAU_CAST(double, mach_absolute_time());
#endif // TAU_WIN_
}

static void tauClockPrintDuration(const double nanoseconds_duration) {
    tau_u64 n;
    int num_digits = 0;
    n = TAU_CAST(tau_u64, nanoseconds_duration);
    while(n!=0) {
        n/=10;
        ++num_digits;
    }

    // Stick with nanoseconds (no need for decimal points here)
    switch(num_digits) {
        case 1: case 2:
            printf("%.0lfns", nanoseconds_duration); break;
        case 3: case 4: case 5:
            printf("%.2lfus", nanoseconds_duration/1000); break;
        case 6: case 7: case 8:
            printf("%.2lfms", nanoseconds_duration/1000000); break;
        default:
            printf("%.2lfs", nanoseconds_duration/1000000000); break;
    }
}

// TAU_TEST_INITIALIZER
#if defined(_MSC_VER)
    #if defined(_WIN64)
        #define TAU_SYMBOL_PREFIX
    #else
        #define TAU_SYMBOL_PREFIX "_"
    #endif // _WIN64

    #pragma section(".CRT$XCU", read)
    #define TAU_TEST_INITIALIZER(f)                                                          \
        static void __cdecl f(void);                                                         \
        __pragma(comment(linker, "/include:" TAU_SYMBOL_PREFIX #f "_"))                      \
        TAU_C_FUNC __declspec(allocate(".CRT$XCU"))    void(__cdecl * f##_)(void) = f;       \
        static void __cdecl f(void)
#else
    #define TAU_TEST_INITIALIZER(f)                             \
        static void f(void)     __attribute__((constructor));   \
        static void f(void)
#endif // _MSC_VER


static inline void* tau_realloc(void* const ptr, const tau_ull new_size) {
    void* const new_ptr = realloc(ptr, new_size);

    if(TAU_NONE(new_ptr))
        free(new_ptr);

    return new_ptr;
}
#endif // TAU_NO_TESTING

#define TAU_COLOUR_DEFAULT_              0
#define TAU_COLOUR_RED_                  1
#define TAU_COLOUR_GREEN_                2
#define TAU_COLOUR_YELLOW_               4
#define TAU_COLOUR_BLUE_                 5
#define TAU_COLOUR_CYAN_                 6
#define TAU_COLOUR_BRIGHTRED_            7
#define TAU_COLOUR_BRIGHTGREEN_          8
#define TAU_COLOUR_BRIGHTYELLOW_         9
#define TAU_COLOUR_BRIGHTBLUE_           10
#define TAU_COLOUR_BRIGHTCYAN_           11
#define TAU_COLOUR_BOLD_                 12

static inline int TAU_ATTRIBUTE_(format (printf, 2, 3))
tauColouredPrintf(const int colour, const char* const fmt, ...);
static inline int TAU_ATTRIBUTE_(format (printf, 2, 3))
tauColouredPrintf(const int colour, const char* const fmt, ...) {
    va_list args;
    char buffer[256];
    int n;

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    buffer[sizeof(buffer)-1] = '\0';

#ifndef TAU_NO_TESTING
    if(!tauShouldColourizeOutput) {
        return printf("%s", buffer);
    }
#endif // TAU_NO_TESTING

#ifdef TAU_UNIX_
    {
        const char* str;
        switch(colour) {
            case TAU_COLOUR_RED_:          str = "\033[0;31m"; break;
            case TAU_COLOUR_GREEN_:        str = "\033[0;32m"; break;
            case TAU_COLOUR_YELLOW_:       str = "\033[0;33m"; break;
            case TAU_COLOUR_BLUE_:         str = "\033[0;34m"; break;
            case TAU_COLOUR_CYAN_:         str = "\033[0;36m"; break;
            case TAU_COLOUR_BRIGHTRED_:    str = "\033[1;31m"; break;
            case TAU_COLOUR_BRIGHTGREEN_:  str = "\033[1;32m"; break;
            case TAU_COLOUR_BRIGHTYELLOW_: str = "\033[1;33m"; break;
            case TAU_COLOUR_BRIGHTBLUE_:   str = "\033[1;34m"; break;
            case TAU_COLOUR_BRIGHTCYAN_:   str = "\033[1;36m"; break;
            case TAU_COLOUR_BOLD_:         str = "\033[1m"; break;
            default:                       str = "\033[0m"; break;
        }
        printf("%s", str);
        n = printf("%s", buffer);
        printf("\033[0m"); // Reset the colour
        return n;
    }
#elif defined(TAU_WIN_)
    {
        HANDLE h;
        CONSOLE_SCREEN_BUFFER_INFO info;
        WORD attr;

        h = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(h, &info);

        switch(colour) {
            case TAU_COLOUR_RED_:            attr = FOREGROUND_RED; break;
            case TAU_COLOUR_GREEN_:          attr = FOREGROUND_GREEN; break;
            case TAU_COLOUR_BLUE_:           attr = FOREGROUND_BLUE; break;
            case TAU_COLOUR_CYAN_:           attr = FOREGROUND_BLUE | FOREGROUND_GREEN; break;
            case TAU_COLOUR_YELLOW_:         attr = FOREGROUND_RED | FOREGROUND_GREEN; break;
            case TAU_COLOUR_BRIGHTRED_:      attr = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            case TAU_COLOUR_BRIGHTGREEN_:    attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case TAU_COLOUR_BRIGHTCYAN_:     attr = FOREGROUND_BLUE | FOREGROUND_GREEN |
                                                    FOREGROUND_INTENSITY; break;
            case TAU_COLOUR_BRIGHTYELLOW_:   attr = FOREGROUND_RED | FOREGROUND_GREEN |
                                                    FOREGROUND_INTENSITY; break;
            case TAU_COLOUR_BOLD_:           attr = FOREGROUND_BLUE | FOREGROUND_GREEN |
                                                    FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            default:                         attr = 0; break;
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
#endif // TAU_UNIX_
}

#ifndef TAU_NO_TESTING
    #define tauPrintf(...) {                                \
        if(tauTestContext.foutput)                          \
            fprintf(tauTestContext.foutput, __VA_ARGS__);   \
        printf(__VA_ARGS__);                                \
    }
#else
    #define tauPrintf(...)   \
        printf(__VA_ARGS__)
#endif // TAU_NO_TESTING


static inline int tauIsDigit(const char c) { return c >= '0' && c <= '9'; }
// If the macro arguments can be decomposed further, we need to print the `In macro ..., so and so failed`.
// This method signals whether this message should be printed.
//
// Note: the arguments are of type `char const*` as opposed to `char*`.
// This helps mitigate the ``warning: ISO C++ forbids converting a string constant to 'char*'`.
// See: https://stackoverflow.com/questions/20944784/why-is-conversion-from-string-constant-to-char-valid-in-c-but-invalid-in-c/20944858
static inline int tauShouldDecomposeMacro(const char* const actual, const char* const expected, const int isStringCmp) {
    // Signal that the macro can be further decomposed if either of the following symbols are present
    int dots = 0;
    int numActualDigits = 0;
    int numExpectedDigits = 0;

    // If not inside a string comparison, we will return `1` only if we determine that `actual` is a variable
    // name/expression (i.e for a value, we search through each character verifying that each is a digit
    // - for floats, we allow a maximum of 1 '.' char)
    if(!isStringCmp) {
        for(int i = 0; i < strlen(actual); i++) {
            if(tauIsDigit(actual[i])) {
                numActualDigits++;
            } else if(actual[i] == '.') {
                dots++;
                if(dots > 1) { return 1; }
            } else {
                return 1;
            }
        }
        // Do the same for `expected`
        dots = 0;
        for(int i=0; i < strlen(expected); i++) {
            if(tauIsDigit(expected[i])) {
                numExpectedDigits++;
            } else if(expected[i] == '.') {
                dots++;
                if(dots > 1) { return 1; }
            } else {
                return 1;
            }
        }
    }
    // Inside a string comparison, we search for common expression tokens like the following:
    // '(', ')', '-'
    else {
        if(strchr(actual, '(') != NULL || strchr(expected, '(') != NULL ||
           actual[0] != '"' || expected[0] != '"' ) {
            return 1;
        }
    }
    return 0;
}


#ifdef TAU_OVERLOADABLE
    #ifndef TAU_CAN_USE_OVERLOADABLES
        #define TAU_CAN_USE_OVERLOADABLES
    #endif // TAU_CAN_USE_OVERLOADABLES

    TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const float f);
    TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const double d);
    TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const long double d);
    TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const int i);
    TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const unsigned int i);
    TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const long int i);
    TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const long unsigned int i);
    TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const void* const p);

    TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const float f) { tauPrintf("%f", TAU_CAST(double, f)); }
    TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const double d) { tauPrintf("%f", d); }
    TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const long double d) { tauPrintf("%Lf", d); }
    TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const int i) { tauPrintf("%d", i); }
    TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const unsigned int i) { tauPrintf("%u", i); }
    TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const long int i) { tauPrintf("%ld", i); }
    TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const long unsigned int i) { tauPrintf("%lu", i); }
    TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const void* const p) { tauPrintf("%p", p); }

    // long long is in C++ only
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) || defined(__cplusplus) && (__cplusplus >= 201103L)
        TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const long long int i);
        TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const long long unsigned int i);

        TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const long long int i) { tauPrintf("%lld", i); }
        TAU_WEAK TAU_OVERLOADABLE void TAU_OVERLOAD_PRINTER(const long long unsigned int i) { tauPrintf("%llu", i); }
    #endif // __STDC_VERSION__

#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    #ifndef TAU_CAN_USE_OVERLOADABLES
        #define TAU_CAN_USE_OVERLOADABLES
    #endif // TAU_CAN_USE_OVERLOADABLES

    #define TAU_OVERLOAD_PRINTER(val)                         \
        tauPrintf(                                            \
            _Generic((val),                                   \
                        char : "'%c'",                        \
                        char* : "%s",                         \
                        unsigned char : "%hhu",               \
                        short : "%hd",                        \
                        unsigned short : "%hu",               \
                        int : "%d",                           \
                        unsigned int : "%u",                  \
                        long : "%ld",                         \
                        long long : "%lld",                   \
                        unsigned long : "%lu",                \
                        unsigned long long : "%"TAU_PRIu64,   \
                        float : "%f",                         \
                        double : "%f",                        \
                        long double : "%Lf",                  \
                        void* : "%p"),                        \
                (val))

#else
    // If we're here, this means that the Compiler does not support overloadable methods
    #define TAU_OVERLOAD_PRINTER(...)                                                              \
        tauPrintf("Error: Your compiler does not support overloadable methods.");                  \
        tauPrintf("If you think this was an error, please file an issue on Tau's Github repo.")
#endif // TAU_OVERLOADABLE

#ifndef TAU_NO_TESTING
    #define TAU_FAIL_IF_INSIDE_TESTSUITE    failIfInsideTestSuite__()
    #define TAU_ABORT_IF_INSIDE_TESTSUITE   abortIfInsideTestSuite__()
#else
    #define TAU_FAIL_IF_INSIDE_TESTSUITE    TAU_ABORT
    #define TAU_ABORT_IF_INSIDE_TESTSUITE   TAU_ABORT
#endif // TAU_NO_TESTING

// ifCondFailsThenPrint is the string representation of the opposite of the truthy value of `cond`
// For example, if `cond` is "!=", then `ifCondFailsThenPrint` will be `==`
#if defined(TAU_CAN_USE_OVERLOADABLES)
    #define __TAUCMP__(actual, expected, cond, space, macroName, failOrAbort)                  \
        do {                                                                                   \
            if(!((actual)cond(expected))) {                                                    \
                tauPrintf("%s:%u: ", __FILE__, __LINE__);                                      \
                tauColouredPrintf(TAU_COLOUR_BRIGHTRED_, "FAILED\n");                          \
                if(tauShouldDecomposeMacro(#actual, #expected, 0)) {                           \
                    tauColouredPrintf(TAU_COLOUR_BRIGHTCYAN_, "  In macro : ");                \
                    tauColouredPrintf(TAU_COLOUR_BRIGHTCYAN_, "%s( %s, %s )\n",                \
                                                                #macroName,                    \
                                                                #actual, #expected);           \
                }                                                                              \
                tauPrintf("  Expected : %s", #actual);                                         \
                printf(" %s ", #cond space);                                                   \
                TAU_OVERLOAD_PRINTER(expected);                                                \
                tauPrintf("\n");                                                               \
                                                                                               \
                tauPrintf("    Actual : %s", #actual);                                         \
                printf(" == ");                                                                \
                TAU_OVERLOAD_PRINTER(actual);                                                  \
                tauPrintf("\n");                                                               \
                failOrAbort;                                                                   \
                if(shouldAbortTest) {                                                          \
                    return;                                                                    \
                }                                                                              \
            }                                                                                  \
        }                                                                                      \
        while(0)

// TAU_OVERLOAD_PRINTER does not work on some compilers
#else
    #define __TAUCMP__(actual, expected, cond, space, macroName, failOrAbort)                          \
        do {                                                                                           \
            if(!((actual)cond(expected))) {                                                            \
                tauPrintf("%s:%u: ", __FILE__, __LINE__);                                              \
                tauColouredPrintf(TAU_COLOUR_BRIGHTRED_, "FAILED\n");                                  \
                if(tauShouldDecomposeMacro(#actual, #expected, 0)) {                                   \
                    tauColouredPrintf(TAU_COLOUR_BRIGHTCYAN_, "  In macro : ");                        \
                    tauColouredPrintf(TAU_COLOUR_BRIGHTCYAN_, "%s( %s, %s )\n",                        \
                                                                #macroName,                            \
                                                                #actual, #expected);                   \
                }                                                                                      \
                tauPrintf("  Expected : %s", #actual);                                                 \
                printf(" %s ", #cond space);                                                           \
                printf(#expected);                                                                     \
                tauPrintf("\n");                                                                       \
                                                                                                       \
                tauPrintf("    Actual : %s", #actual);                                                 \
                printf(" == ");                                                                        \
                printf(#actual);                                                                       \
                tauPrintf("\n");                                                                       \
                failOrAbort;                                                                           \
                if(shouldAbortTest) {                                                                  \
                    return;                                                                            \
                }                                                                                      \
            }                                                                                          \
        }                                                                                              \
        while(0)
#endif // TAU_CAN_USE_OVERLOADABLES

#define __TAUCMP_STR__(actual, expected, cond, ifCondFailsThenPrint, actualPrint, macroName, failOrAbort)       \
    do {                                                                                                        \
        if(strcmp(actual, expected) cond 0) {                                                                   \
            tauPrintf("%s:%u: ", __FILE__, __LINE__);                                                           \
            tauColouredPrintf(TAU_COLOUR_BRIGHTRED_, "FAILED\n");                                               \
            if(tauShouldDecomposeMacro(#actual, #expected, 1)) {                                                \
                    tauColouredPrintf(TAU_COLOUR_BRIGHTCYAN_, "  In macro : ");                                 \
                    tauColouredPrintf(TAU_COLOUR_BRIGHTCYAN_, "%s( %s, %s )\n",                                 \
                                                                #macroName,                                     \
                                                                #actual, #expected);                            \
                }                                                                                               \
            tauPrintf("  Expected : \"%s\" %s \"%s\"\n", actual, #ifCondFailsThenPrint, expected);              \
            tauPrintf("    Actual : %s\n", #actualPrint);                                                       \
            failOrAbort;                                                                                        \
            if(shouldAbortTest) {                                                                               \
                return;                                                                                         \
            }                                                                                                   \
        }                                                                                                       \
    }                                                                                                           \
    while(0)


static void tauPrintColouredIfDifferent(const tau_u8 ch, const tau_u8 ref) {
    if(ch == ref) {
        tauPrintf("%02X", ch);
    } else {
        tauColouredPrintf(TAU_COLOUR_BRIGHTYELLOW_, "%02X", ch);
    }
}


static void tauPrintHexBufCmp(const void* const buff, const void* const ref, const int size) {
    const tau_u8* const test_buff = TAU_CAST(const tau_u8* const, buff);
    const tau_u8* const ref_buff = TAU_CAST(const tau_u8* const, ref);

    tauColouredPrintf(TAU_COLOUR_CYAN_,"<");
    if(size != 0)
        tauPrintColouredIfDifferent(test_buff[0], ref_buff[0]);

    for(int i = 1; i < size; ++i) {
        printf(" ");
        tauPrintColouredIfDifferent(test_buff[i], ref_buff[i]);
    }
    tauColouredPrintf(TAU_COLOUR_CYAN_,">");
}


#define __TAUCMP_BUF__(actual, expected, len, cond, ifCondFailsThenPrint, actualPrint, macroName, failOrAbort)  \
    do {                                                                                                        \
        if(memcmp(actual, expected, len) cond 0) {                                                              \
            tauPrintf("%s:%u: ", __FILE__, __LINE__);                                                           \
            tauColouredPrintf(TAU_COLOUR_BRIGHTRED_, "FAILED\n");                                               \
            if(tauShouldDecomposeMacro(#actual, #expected, 1)) {                                                \
                    tauColouredPrintf(TAU_COLOUR_BRIGHTCYAN_, "  In macro : ");                                 \
                    tauColouredPrintf(TAU_COLOUR_BRIGHTCYAN_, "%s( %s, %s, %s )\n",                             \
                                                                #macroName,                                     \
                                                                #actual, #expected, #len);                      \
                }                                                                                               \
            tauPrintf("  Expected : "); tauPrintHexBufCmp(actual, expected, len);                               \
            tauPrintf(" %s ", #ifCondFailsThenPrint);                                                           \
            tauPrintHexBufCmp(expected, actual, len);                                                           \
            tauPrintf("\n    Actual : %s\n", #actualPrint);                                                     \
            failOrAbort;                                                                                        \
            if(shouldAbortTest) {                                                                               \
                return;                                                                                         \
            }                                                                                                   \
        }                                                                                                       \
    }                                                                                                           \
    while(0)

#define __TAUCMP_STRN__(actual, expected, n, cond, ifCondFailsThenPrint, actualPrint, macroName, failOrAbort)   \
    do {                                                                                                        \
        if(TAU_CAST(int, n) < 0) {                                                                              \
            tauColouredPrintf(TAU_COLOUR_BRIGHTRED_, "`n` cannot be negative\n");                               \
            TAU_ABORT;                                                                                          \
        }                                                                                                       \
        if(strncmp(actual, expected, n) cond 0) {                                                               \
            tauPrintf("%s:%u: ", __FILE__, __LINE__);                                                           \
            tauColouredPrintf(TAU_COLOUR_BRIGHTRED_, "FAILED\n");                                               \
            if(tauShouldDecomposeMacro(#actual, #expected, 1)) {                                                \
                    tauColouredPrintf(TAU_COLOUR_BRIGHTCYAN_, "  In macro : ");                                 \
                    tauColouredPrintf(TAU_COLOUR_BRIGHTCYAN_, "%s( %s, %s, %s)\n",                              \
                                                                #macroName,                                     \
                                                                #actual, #expected, #n);                        \
                }                                                                                               \
            tauPrintf("  Expected : \"%.*s\" %s \"%.*s\"\n", TAU_CAST(int, n), actual,                          \
                                                              #ifCondFailsThenPrint,                            \
                                                              TAU_CAST(int, n), expected);                      \
            tauPrintf("    Actual : %s\n", #actualPrint);                                                       \
            failOrAbort;                                                                                        \
            if(shouldAbortTest) {                                                                               \
                return;                                                                                         \
            }                                                                                                   \
        }                                                                                                       \
    }                                                                                                           \
    while(0)


#define __TAUCMP_TF(cond, actual, expected, negateSign, macroName, failOrAbort)     \
    do {                                                                            \
        if(negateSign(cond)) {                                                      \
            tauPrintf("%s:%u: ", __FILE__, __LINE__);                               \
            tauColouredPrintf(TAU_COLOUR_BRIGHTRED_, "FAILED\n");                   \
            if(tauShouldDecomposeMacro(#actual, TAU_NULL, 0)) {                     \
                    tauColouredPrintf(TAU_COLOUR_BRIGHTCYAN_, "  In macro : ");     \
                    tauColouredPrintf(TAU_COLOUR_BRIGHTCYAN_, "%s( %s )\n",         \
                                                                #macroName,         \
                                                                #cond);             \
                }                                                                   \
            tauPrintf("  Expected : %s\n", #expected);                              \
            tauPrintf("    Actual : %s\n", #actual);                                \
            failOrAbort;                                                            \
            if(shouldAbortTest) {                                                   \
                return;                                                             \
            }                                                                       \
        }                                                                           \
    } while(0)

/**
############################################
          {CHECK|REQUIRE} Macros
############################################
*/
#define CHECK_EQ(actual, expected)      __TAUCMP__(actual, expected, ==, "", CHECK_EQ, TAU_FAIL_IF_INSIDE_TESTSUITE)
#define CHECK_NE(actual, expected)      __TAUCMP__(actual, expected, !=, "", CHECK_NE, TAU_FAIL_IF_INSIDE_TESTSUITE)
#define CHECK_LT(actual, expected)      __TAUCMP__(actual, expected, < , " ", CHECK_LT, TAU_FAIL_IF_INSIDE_TESTSUITE)
#define CHECK_LE(actual, expected)      __TAUCMP__(actual, expected, <=, "", CHECK_LE, TAU_FAIL_IF_INSIDE_TESTSUITE)
#define CHECK_GT(actual, expected)      __TAUCMP__(actual, expected, > , " ", CHECK_GT, TAU_FAIL_IF_INSIDE_TESTSUITE)
#define CHECK_GE(actual, expected)      __TAUCMP__(actual, expected, >=, "", CHECK_GE, TAU_FAIL_IF_INSIDE_TESTSUITE)

#define REQUIRE_EQ(actual, expected)    __TAUCMP__(actual, expected, ==, "", REQUIRE_EQ, TAU_ABORT_IF_INSIDE_TESTSUITE)
#define REQUIRE_NE(actual, expected)    __TAUCMP__(actual, expected, !=, "", REQUIRE_NE, TAU_ABORT_IF_INSIDE_TESTSUITE)
#define REQUIRE_LT(actual, expected)    __TAUCMP__(actual, expected, < , " ", REQUIRE_LT, TAU_ABORT_IF_INSIDE_TESTSUITE)
#define REQUIRE_LE(actual, expected)    __TAUCMP__(actual, expected, <=, "", REQUIRE_LE, TAU_ABORT_IF_INSIDE_TESTSUITE)
#define REQUIRE_GT(actual, expected)    __TAUCMP__(actual, expected, > , " ", REQUIRE_GT, TAU_ABORT_IF_INSIDE_TESTSUITE)
#define REQUIRE_GE(actual, expected)    __TAUCMP__(actual, expected, >=, "", REQUIRE_GE, TAU_ABORT_IF_INSIDE_TESTSUITE)

// Whole-string checks
#define CHECK_STREQ(actual, expected)   __TAUCMP_STR__(actual, expected, !=, ==, not equal, CHECK_STREQ, TAU_FAIL_IF_INSIDE_TESTSUITE)
#define CHECK_STRNE(actual, expected)   __TAUCMP_STR__(actual, expected, ==, !=, equal, CHECK_STRNE, TAU_FAIL_IF_INSIDE_TESTSUITE)
#define REQUIRE_STREQ(actual, expected) __TAUCMP_STR__(actual, expected, !=, ==, not equal, REQUIRE_STREQ, TAU_ABORT_IF_INSIDE_TESTSUITE)
#define REQUIRE_STRNE(actual, expected) __TAUCMP_STR__(actual, expected, ==, !=, equal, REQUIRE_STRNE, TAU_ABORT_IF_INSIDE_TESTSUITE)

// Substring Checks
#define CHECK_SUBSTREQ(actual, expected, n)     __TAUCMP_STRN__(actual, expected, n, !=, ==, unequal substrings, CHECK_SUBSTREQ, TAU_FAIL_IF_INSIDE_TESTSUITE)
#define CHECK_SUBSTRNE(actual, expected, n)     __TAUCMP_STRN__(actual, expected, n, ==, !=, equal substrings, CHECK_SUBSTRNE, TAU_FAIL_IF_INSIDE_TESTSUITE)
#define REQUIRE_SUBSTREQ(actual, expected, n)   __TAUCMP_STRN__(actual, expected, n, !=, ==, unequal substrings, REQUIRE_SUBSTREQ, TAU_ABORT_IF_INSIDE_TESTSUITE)
#define REQUIRE_SUBSTRNE(actual, expected, n)   __TAUCMP_STRN__(actual, expected, n, ==, !=, equal substrings, REQUIRE_SUBSTRNE, TAU_ABORT_IF_INSIDE_TESTSUITE)

// Buffer Checks
#define CHECK_BUF_EQ(actual, expected, n)       __TAUCMP_BUF__(actual, expected, n, !=, ==, not equal, CHECK_BUF_EQ, TAU_FAIL_IF_INSIDE_TESTSUITE)
#define CHECK_BUF_NE(actual, expected, n)       __TAUCMP_BUF__(actual, expected, n, ==, !=, equal, CHECK_BUF_NE, TAU_FAIL_IF_INSIDE_TESTSUITE)
#define REQUIRE_BUF_EQ(actual, expected, n)     __TAUCMP_BUF__(actual, expected, n, !=, ==, not equal, REQUIRE_BUF_EQ, TAU_ABORT_IF_INSIDE_TESTSUITE)
#define REQUIRE_BUF_NE(actual, expected, n)     __TAUCMP_BUF__(actual, expected, n, ==, !=, equal, REQUIRE_BUF_NE, TAU_ABORT_IF_INSIDE_TESTSUITE)

// Note: The negate sign `!` must be there for {CHECK|REQUIRE}_TRUE
// Do not remove it
#define CHECK_TRUE(cond)      __TAUCMP_TF(cond, false, true, !, CHECK_TRUE, TAU_FAIL_IF_INSIDE_TESTSUITE)
#define CHECK_FALSE(cond)     __TAUCMP_TF(cond, true, false, , CHECK_FALSE, TAU_FAIL_IF_INSIDE_TESTSUITE)

#define REQUIRE_TRUE(cond)    __TAUCMP_TF(cond, false, true, !, REQUIRE_TRUE, TAU_ABORT_IF_INSIDE_TESTSUITE)
#define REQUIRE_FALSE(cond)   __TAUCMP_TF(cond, true, false, , REQUIRE_FALSE, TAU_ABORT_IF_INSIDE_TESTSUITE)

#define __TAUCHECKREQUIRE__(cond, failOrAbort, macroName, ...)                                 \
    do {                                                                                       \
        if(!(cond)) {                                                                          \
            tauPrintf("%s:%u: ", __FILE__, __LINE__);                                          \
            if((sizeof(char[]){__VA_ARGS__}) <= 1)                                             \
                tauColouredPrintf(TAU_COLOUR_BRIGHTRED_, "FAILED");                            \
            else                                                                               \
                tauColouredPrintf(TAU_COLOUR_BRIGHTRED_, __VA_ARGS__);                         \
            printf("\n");                                                                      \
            printf("The following assertion failed: \n");                                      \
            tauColouredPrintf(TAU_COLOUR_BRIGHTCYAN_, "    %s( %s )\n", #macroName, #cond);    \
            failOrAbort;                                                                       \
            if(shouldAbortTest) {                                                              \
                return;                                                                        \
            }                                                                                  \
        }                                                                                      \
    }                                                                                          \
    while(0)

// This is a little hack that allows a form of "polymorphism" to a macro - it allows a user to optionally pass
// an extra argument to a macro in {CHECK|REQUIRE}.
// The first argument is always the condition to check/assert. If this condition fails, by default a `FAILED`
// message is sent to STDOUT. If a second argument is passed to the macro (a string), this will be outputted
// instead.
//
// The MACRO_CHOOSER uses the list of arguments twice, first to form the name of the helper macro, and then to
// pass the arguments to that helper macro. It uses a standard trick to count the number of arguments to a macro.
//
// Neat hack from:
// https://stackoverflow.com/questions/3046889/optional-parameters-with-c-macros, and
// https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
#define GET_3RD_ARG(arg1, arg2, arg3, ...)   arg3

#define CHECK_1_ARGS(cond)              __TAUCHECKREQUIRE__(cond, TAU_FAIL_IF_INSIDE_TESTSUITE, CHECK, "FAILED")
#define CHECK_2_ARGS(cond, message)     __TAUCHECKREQUIRE__(cond, TAU_FAIL_IF_INSIDE_TESTSUITE, CHECK, message)
#define CHECK_MACRO_CHOOSER(...)        GET_3RD_ARG(__VA_ARGS__, CHECK_2_ARGS, CHECK_1_ARGS, )

#define REQUIRE_1_ARGS(cond)            __TAUCHECKREQUIRE__(cond, TAU_ABORT_IF_INSIDE_TESTSUITE, REQUIRE, "FAILED")
#define REQUIRE_2_ARGS(cond, message)   __TAUCHECKREQUIRE__(cond, TAU_ABORT_IF_INSIDE_TESTSUITE, REQUIRE, message)
#define REQUIRE_MACRO_CHOOSER(...)      GET_3RD_ARG(__VA_ARGS__, REQUIRE_2_ARGS, REQUIRE_1_ARGS, )

#define CHECK(...)      CHECK_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)
#define REQUIRE(...)    REQUIRE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define CHECK_NULL(val)       CHECK(val == TAU_NULL)
#define CHECK_NOT_NULL(val)   CHECK(val != TAU_NULL)

#define WARN(msg)                                                        \
    incrementWarnings();                                                 \
    tauColouredPrintf(TAU_COLOUR_YELLOW_, "%s:%u:\nWARNING: %s\n", __FILE__, __LINE__, #msg)

#ifdef __cplusplus
    #define SECTION(...)    \
        if(1)
    #define STATIC_REQUIRE(...)                  \
        static_assert(__VA_ARGS__, #__VA_ARGS__)
    #define STATIC_REQUIRE_FALSE(...)            \
        static_assert(!(__VA_ARGS__), "!(" #__VA_ARGS__ ")")
#else
    #define SECTION(...)                                            \
        WARN(Using "SECTION" in C results in limited diagnostics.); \
        if(1)
    #define STATIC_REQUIRE(...)                                              \
        WARN(Usage of "STATIC_REQUIRE" is not supported in C source files.)
    #define STATIC_REQUIRE_FALSE(...)                                        \
        WARN(Usage of "STATIC_REQUIRE_FALSE" is not supported in C source files.)
#endif // __cplusplus

/**
#########################################
             Implementation
#########################################
*/

#ifndef TAU_NO_TESTING
#ifdef _MSC_VER
    #define TAU_SNPRINTF(BUFFER, N, ...)   _snprintf_s(BUFFER, N, N, __VA_ARGS__)
#else
    #define TAU_SNPRINTF(...)              snprintf(__VA_ARGS__)
#endif // _MSC_VER

#define TEST(TESTSUITE, TESTNAME)                                                              \
    TAU_EXTERN tauTestStateStruct tauTestContext;                                              \
    static void _TAU_TEST_FUNC_##TESTSUITE##_##TESTNAME(void);                                 \
    TAU_TEST_INITIALIZER(tau_register_##TESTSUITE##_##TESTNAME) {                              \
        const tau_ull index = tauTestContext.numTestSuites++;                                  \
        const char* const namePart = #TESTSUITE "." #TESTNAME;                                 \
        const tau_ull nameSize = strlen(namePart) + 1;                                         \
        char* const name = TAU_PTRCAST(char* , malloc(nameSize));                              \
        tauTestContext.tests = TAU_PTRCAST(                                                    \
                                    tauTestSuiteStruct*,                                       \
                                    tau_realloc(TAU_PTRCAST(void* , tauTestContext.tests),     \
                                                sizeof(tauTestSuiteStruct) *                   \
                                                    tauTestContext.numTestSuites));            \
        tauTestContext.tests[index].func = &_TAU_TEST_FUNC_##TESTSUITE##_##TESTNAME;           \
        tauTestContext.tests[index].name = name;                                               \
        TAU_SNPRINTF(name, nameSize, "%s", namePart);                                          \
    }                                                                                          \
    void _TAU_TEST_FUNC_##TESTSUITE##_##TESTNAME(void)


#define TEST_F_SETUP(FIXTURE)                                                  \
    static void __TAU_TEST_FIXTURE_SETUP_##FIXTURE(struct FIXTURE* const tau)

#define TEST_F_TEARDOWN(FIXTURE)                                               \
    static void __TAU_TEST_FIXTURE_TEARDOWN_##FIXTURE(struct FIXTURE* const tau)

#define TEST_F(FIXTURE, NAME)                                                                            \
    TAU_EXTERN tauTestStateStruct tauTestContext;                                                        \
    static void __TAU_TEST_FIXTURE_SETUP_##FIXTURE(struct FIXTURE* const);                               \
    static void __TAU_TEST_FIXTURE_TEARDOWN_##FIXTURE(struct FIXTURE* const);                            \
    static void __TAU_TEST_FIXTURE_RUN_##FIXTURE##_##NAME(struct FIXTURE* const);                        \
                                                                                                         \
    static void __TAU_TEST_FIXTURE_##FIXTURE##_##NAME() {                                                \
        struct FIXTURE fixture;                                                                          \
        memset(&fixture, 0, sizeof(fixture));                                                            \
        __TAU_TEST_FIXTURE_SETUP_##FIXTURE(&fixture);                                                    \
        if(hasCurrentTestFailed == 1) {                                                                  \
            return;                                                                                      \
        }                                                                                                \
                                                                                                         \
        __TAU_TEST_FIXTURE_RUN_##FIXTURE##_##NAME(&fixture);                                             \
        __TAU_TEST_FIXTURE_TEARDOWN_##FIXTURE(&fixture);                                                 \
    }                                                                                                    \
                                                                                                         \
    TAU_TEST_INITIALIZER(tau_register_##FIXTURE##_##NAME) {                                              \
        const tau_ull index = tauTestContext.numTestSuites++;                                            \
        const char* const namePart = #FIXTURE "." #NAME;                                                 \
        const tau_ull nameSize = strlen(namePart) + 1;                                                   \
        char* name = TAU_PTRCAST(char* , malloc(nameSize));                                              \
        tauTestContext.tests = TAU_PTRCAST(                                                              \
                                    tauTestSuiteStruct*,                                                 \
                                    tau_realloc(TAU_PTRCAST(void*, tauTestContext.tests),                \
                                                                sizeof(tauTestSuiteStruct) *             \
                                                                        tauTestContext.numTestSuites));  \
        tauTestContext.tests[index].func = &__TAU_TEST_FIXTURE_##FIXTURE##_##NAME;                       \
        tauTestContext.tests[index].name = name;                                                         \
        TAU_SNPRINTF(name, nameSize, "%s", namePart);                                                    \
    }                                                                                                    \
    static void __TAU_TEST_FIXTURE_RUN_##FIXTURE##_##NAME(struct FIXTURE* const tau)


static int tauShouldFilterTest(const char* const filter, const char* const testcase);
static int tauShouldFilterTest(const char* const filter, const char* const testcase) {
    if(TAU_SOME(filter)) {
        const char* filter_curr = filter;
        const char* testcase_curr = testcase;
        const char* filter_wildcard = TAU_NULL;

        while ((*filter_curr != TAU_NULLCHAR) && (*testcase_curr != TAU_NULLCHAR)) {
            if(*filter_curr == '*') {
                // store the position of the wildcard
                filter_wildcard = filter_curr;

                // skip the wildcard character
                filter_curr++;

                while ((*filter_curr != TAU_NULLCHAR) && (*testcase_curr != TAU_NULLCHAR)) {
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

                if((*filter_curr == TAU_NULLCHAR) && (*testcase_curr == TAU_NULLCHAR))
                    return 0;

                // if the testcase has been exhausted, we don't have a match!
                if(*testcase_curr == TAU_NULLCHAR)
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

        if((*filter_curr != TAU_NULLCHAR) || ((*testcase_curr == TAU_NULLCHAR) && ((filter == filter_curr) ||
            (filter_curr[-1] != '*')))) {
            // We have a mismatch
            return 1;
        }
    }
    return 0;
}

static inline FILE* tau_fopen(const char* const filename, const char* const mode) {
#ifdef _MSC_VER
    FILE* file;
    if(fopen_s(&file, filename, mode) == 0) {
        return file;
    } else {
        return TAU_NULL;
    }
#else
    return fopen(filename, mode);
#endif // _MSC_VER
}


static void tau_help_() {
    printf("Usage: %s [options] [test...]\n", tau_argv0_);
    printf("\n");
    printf("Run the specified unit tests; or if the option '--skip' is used, run all\n");
    printf("tests in the suite but those listed. By default, if no tests are specified\n");
    printf("on the command line, all unit tests in the suite are run.\n");
    printf("\n");
    printf("Options:\n");
    printf("  --failed-output-only     Output only failed Test Suites");
    printf("  --filter=<filter>        Filter the test suites to run (e.g: Suite1*.a\n");
    printf("                             would run Suite1Case.a but not Suite1Case.b}\n");
#if defined(TAU_WIN_)
    printf("  --time                   Measure test duration\n");
#elif defined(TAU_HAS_POSIX_TIMER_)
    printf("  --time                   Measure test duration (real time)\n");
    printf("  --time=TIMER             Measure test duration, using given timer\n");
    printf("                               (TIMER is one of 'real', 'cpu')\n");
#endif // TAU_WIN_
    printf("  --no-summary             Suppress printing of test results summary\n");
    printf("  --output=<FILE>          Write an XUnit XML file to Enable XUnit output\n");
    printf("                             to the given file\n");
    printf("  --list                   List unit tests in the suite and exit\n");
    printf("  --no-color               Disable coloured output\n");
    printf("  --help                   Display this help and exit\n");
}


static tau_bool tauCmdLineRead(const int argc, const char* const * const argv) {
    // Coloured output
#ifdef TAU_UNIX_
    tauShouldColourizeOutput = isatty(STDOUT_FILENO);
#elif defined TAU_WIN_
    #ifdef _BORLANDC_
        tauShouldColourizeOutput = isatty(_fileno(stdout));
    #else
        tauShouldColourizeOutput = _isatty(_fileno(stdout));
    #endif // _BORLANDC_
#else
    tauShouldColourizeOutput = isatty(STDOUT_FILENO);
#endif // TAU_UNIX_

    // loop through all arguments looking for our options
    for(tau_ull i = 1; i < TAU_CAST(tau_ull, argc); i++) {
        /* Informational switches */
        const char* const helpStr = "--help";
        const char* const listStr = "--list";
        const char* const colourStr = "--no-color";
        const char* const summaryStr = "--no-summary";
        const char* const onlyFailedOutput = "--failed-output-only";
        /* Test config switches */
        const char* const filterStr = "--filter=";
        const char* const XUnitOutput = "--output=";

        // Help
        if(strncmp(argv[i], helpStr, strlen(helpStr)) == 0) {
            tau_help_();
            return tau_false;
        }

        // Only failed output
        else if(strncmp(argv[i], onlyFailedOutput, strlen(onlyFailedOutput)) == 0) {
            tauDisplayOnlyFailedOutput = 1;
        }

        // Filter tests
        else if(strncmp(argv[i], filterStr, strlen(filterStr)) == 0)
            // user wants to filter what test suites run!
            cmd_filter = argv[i] + strlen(filterStr);

        // Write XUnit XML file
        else if(strncmp(argv[i], XUnitOutput, strlen(XUnitOutput)) == 0)
            tauTestContext.foutput = tau_fopen(argv[i] + strlen(XUnitOutput), "w+");

        // List tests
        else if(strncmp(argv[i], listStr, strlen(listStr)) == 0) {
            for (i = 0; i < tauTestContext.numTestSuites; i++)
                tauPrintf("%s\n", tauTestContext.tests[i].name);
            tauDisplayTests = 1;
        }

        // Disable colouring
        else if(strncmp(argv[i], colourStr, strlen(colourStr)) == 0) {
            tauShouldColourizeOutput = 0;
        }

        // Disable Summary
        else if(strncmp(argv[i], summaryStr, strlen(summaryStr))) {
            tauDisableSummary = 1;
        }

        else {
            printf("ERROR: Unrecognized option: %s", argv[i]);
            return tau_false;
        }
    }

    return tau_true;
}

static int tauCleanup() {
    for (tau_ull i = 0; i < tauTestContext.numTestSuites; i++)
        free(TAU_PTRCAST(void* , tauTestContext.tests[i].name));

    free(TAU_PTRCAST(void* , tauStatsFailedTestSuites));
    free(TAU_PTRCAST(void* , tauTestContext.tests));

    if(tauTestContext.foutput)
        fclose(tauTestContext.foutput);

    return TAU_CAST(int, tauStatsNumTestsFailed);
}

// Triggers and runs all unit tests
static void tauRunTests() {
    // Run tests
    for(tau_ull i = 0; i < tauTestContext.numTestSuites; i++) {
        checkIsInsideTestSuite = 1;
        hasCurrentTestFailed = 0;

        if(tauShouldFilterTest(cmd_filter, tauTestContext.tests[i].name))
            continue;

        if(!tauDisplayOnlyFailedOutput) {
            tauColouredPrintf(TAU_COLOUR_BRIGHTGREEN_, "[ RUN      ] ");
            tauColouredPrintf(TAU_COLOUR_DEFAULT_, "%s\n", tauTestContext.tests[i].name);
        }

        if(tauTestContext.foutput)
            fprintf(tauTestContext.foutput, "<testcase name=\"%s\">", tauTestContext.tests[i].name);

        // Start the timer
        const double start = tauClock();

        // The actual test
        tauTestContext.tests[i].func();

        // Stop the timer
        const double duration = tauClock() - start;

        if(tauTestContext.foutput)
            fprintf(tauTestContext.foutput, "</testcase>\n");

        if(hasCurrentTestFailed == 1) {
            const tau_ull failed_testcase_index = tauStatsNumFailedTestSuites++;
            tauStatsFailedTestSuites = TAU_PTRCAST(tau_ull*,
                                            tau_realloc(TAU_PTRCAST(void*, tauStatsFailedTestSuites),
                                                          sizeof(tau_ull) * tauStatsNumFailedTestSuites));
            tauStatsFailedTestSuites[failed_testcase_index] = i;
            tauStatsNumTestsFailed++;
            tauColouredPrintf(TAU_COLOUR_BRIGHTRED_, "[  FAILED  ] ");
            tauColouredPrintf(TAU_COLOUR_DEFAULT_, "%s (", tauTestContext.tests[i].name);
            tauClockPrintDuration(duration);
            printf(")\n");
        } else {
            if(!tauDisplayOnlyFailedOutput) {
                tauColouredPrintf(TAU_COLOUR_BRIGHTGREEN_, "[       OK ] ");
                tauColouredPrintf(TAU_COLOUR_DEFAULT_, "%s (", tauTestContext.tests[i].name);
                tauClockPrintDuration(duration);
                printf(")\n");
            }
        }
    }
    tauColouredPrintf(TAU_COLOUR_BRIGHTGREEN_, "[==========] ");
    tauColouredPrintf(TAU_COLOUR_DEFAULT_, "%" TAU_PRIu64 " test suites ran\n", tauStatsTestsRan);
}


static inline int tau_main(const int argc, const char* const * const argv);
inline int tau_main(const int argc, const char* const * const argv) {
    tauStatsTotalTestSuites = TAU_CAST(tau_u64, tauTestContext.numTestSuites);
    tau_argv0_ = argv[0];

    // Start the entire Test Session timer
    const double start = tauClock();

    const tau_bool wasCmdLineReadSuccessful = tauCmdLineRead(argc, argv);
    if (tauDisplayTests)
        return tauCleanup();

    if(!wasCmdLineReadSuccessful)
        return tauCleanup();

    for (tau_ull i = 0; i < tauTestContext.numTestSuites; i++) {
        if(tauShouldFilterTest(cmd_filter, tauTestContext.tests[i].name))
            tauStatsSkippedTests++;
    }

    tauStatsTestsRan = tauStatsTotalTestSuites - tauStatsSkippedTests;

    // Begin tests`
    tauColouredPrintf(TAU_COLOUR_BRIGHTGREEN_, "[==========] ");
    tauColouredPrintf(TAU_COLOUR_BOLD_, "Running %" TAU_PRIu64 " test suites.\n", TAU_CAST(tau_u64, tauStatsTestsRan));

    if(tauTestContext.foutput) {
        fprintf(tauTestContext.foutput, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        fprintf(tauTestContext.foutput,
                "<testsuites tests=\"%" TAU_PRIu64 "\" name=\"All\">\n",
                TAU_CAST(tau_u64, tauStatsTestsRan));
        fprintf(tauTestContext.foutput,
                "<testsuite name=\"Tests\" tests=\"%" TAU_PRIu64 "\">\n",
                TAU_CAST(tau_u64, tauStatsTestsRan));
    }

    // Run tests
    tauRunTests();

    // End the entire Test Session timer
    const double duration = tauClock() - start;

    // Write a Summary
    tauColouredPrintf(TAU_COLOUR_BRIGHTGREEN_, "[  PASSED  ] %" TAU_PRIu64 " %s\n",
                            tauStatsTestsRan - tauStatsNumTestsFailed,
                            tauStatsTestsRan - tauStatsNumTestsFailed == 1 ? "suite" : "suites");
    tauColouredPrintf(TAU_COLOUR_BRIGHTRED_, "[  FAILED  ] %" TAU_PRIu64 " %s\n",
                            tauStatsNumTestsFailed,
                            tauStatsNumTestsFailed == 1 ? "suite" : "suites");

    if(!tauDisableSummary) {
        tauColouredPrintf(TAU_COLOUR_BOLD_, "\nSummary:\n");

        printf("    Total test suites:          %" TAU_PRIu64 "\n", tauStatsTotalTestSuites);
        printf("    Total suites run:           %" TAU_PRIu64 "\n", tauStatsTestsRan);
        printf("    Total warnings generated:   %" TAU_PRIu64 "\n", tauStatsNumWarnings);
        printf("    Total suites skipped:       %" TAU_PRIu64 "\n", tauStatsSkippedTests);
        printf("    Total suites failed:        %" TAU_PRIu64 "\n", tauStatsNumTestsFailed);
    }

    if(tauStatsNumTestsFailed > 0) {
        tauColouredPrintf(TAU_COLOUR_BRIGHTRED_, "FAILED: ");
        printf("%" TAU_PRIu64 " failed, %" TAU_PRIu64 " passed in ",
                            tauStatsNumTestsFailed,
                            tauStatsTestsRan - tauStatsNumTestsFailed);
        tauClockPrintDuration(duration);
        printf("\n");

        for (tau_ull i = 0; i < tauStatsNumFailedTestSuites; i++) {
            tauColouredPrintf(TAU_COLOUR_BRIGHTRED_, "  [ FAILED ] %s\n",
                            tauTestContext.tests[tauStatsFailedTestSuites[i]].name);
        }
    } else if(tauStatsNumTestsFailed == 0 && tauStatsTotalTestSuites > 0) {
        const tau_u64 total_tests_passed = tauStatsTestsRan - tauStatsNumTestsFailed;
        tauColouredPrintf(TAU_COLOUR_BRIGHTGREEN_, "SUCCESS: ");
        printf("%" TAU_PRIu64 " test suites passed in ", total_tests_passed);
        tauClockPrintDuration(duration);
        printf("\n");
    } else {
        tauColouredPrintf(TAU_COLOUR_BRIGHTYELLOW_, "WARNING: ");
        printf("No test suites were found. If you think this was an error, please file an issue on Tau's Github repo.");
        printf("\n");
    }

    if(tauTestContext.foutput)
        fprintf(tauTestContext.foutput, "</testsuite>\n</testsuites>\n");

    return tauCleanup();
}

/**
    We need to declare these variables here because they're used in multiple translation units (if compiled
    as so).
    For example, if test1.c(pp) #includes `tau/tau.h` and test2.c(pp) does the same, declaring the variables
    static will only make them exist in the first translation unit the compiler sees.
    We can get around this by compiling only one file (eg. main.c(pp)) and including the other source files
    using `#include`, but this would be counter-productive.

    If we used C++ as the base language for this project, we could have got around this problem using OOP
    constructs, but C++ is not (and should not) be the language this project is written in.
    With C, the best (and closest) thing we have are global variables that persists through the _entire_
    compilation project (all testing source files).
    See: https://stackoverflow.com/questions/1856599/when-to-use-static-keyword-before-global-variables
*/
#define TAU_ONLY_GLOBALS()                       \
    volatile int checkIsInsideTestSuite = 0;     \
    volatile int hasCurrentTestFailed = 0;       \
    volatile int shouldFailTest = 0;             \
    volatile int shouldAbortTest = 0;            \
    tau_u64 tauStatsNumWarnings = 0;

// If a user wants to define their own `main()` function, this _must_ be at the very end of the functtion
#define TAU_NO_MAIN()                                       \
    tauTestStateStruct tauTestContext = {0, 0, 0};          \
    TAU_ONLY_GLOBALS()

// Define a main() function to call into tau.h and start executing tests.
#define TAU_MAIN()                                                             \
    /* Define the global struct that will hold the data we need to run Tau. */ \
    tauTestStateStruct tauTestContext = {0, 0, 0};                             \
    TAU_ONLY_GLOBALS()                                                         \
                                                                               \
    int main(const int argc, const char* const * const argv) {                 \
        return tau_main(argc, argv);                                           \
    }

#endif // TAU_NO_TESTING

#ifdef TAU_NO_TESTING
    volatile int checkIsInsideTestSuite = 0;
    volatile int hasCurrentTestFailed = 0;
    // volatile int shouldFailTest = 0;
    // volatile int shouldAbortTest = 0;
#endif // TAU_NO_TESTING

TAU_DISABLE_DEBUG_WARNINGS_POP

#endif // TAU_H_
