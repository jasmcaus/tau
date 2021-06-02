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

MUON_DISABLE_WARNINGS

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

#ifdef __cplusplus
    #define MUON_ABORT  std::abort()
#else 
    #define MUON_ABORT  exit(1)
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

#if defined(_MSC_VER) || defined(__cplusplus)
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
extern MUON_UInt64 muonStatsNumWarnings;

// Overridden in `muon_main` if the cmdline option `--no-color` is passed
static int muonShouldColourizeOutput = 1;
static int muonDisableSummary = 0;
static int muonDisplayOnlyFailedOutput = 0; 

static char* muon_argv0_ = MUON_NULL;
static const char* filter = MUON_NULL;

/**
    This helps us determine whether a CHECK or a REQUIRE are being called from within or outside a 
    a Test Suite. Muon supports both - so we need to handle this
    We could have determined this somehow from within a function, but this is cleaner a cleaner approach
    (which is the aim on Muon).

    Inside the TEST() initializer, this is set to true (because we are inside a Test Suite), so the 
    CHECKs and REQUIREs will do their thing and return the appropriate result. 
    If the assertion macro is not within the TEST() scope, it simply does not return anything - it only
    resets it back to false so that this same process occurs for the rest of the checks.
*/
extern volatile int checkIsInsideTestSuite;
extern volatile int hasCurrentTestFailed;
extern volatile int shouldFailTest;
extern volatile int shouldAbortTest;

/**
    This function is called from within a macro in the format {CHECK|REQUIRE)_*
    If we are inside a test suite _and_ a check fails, we need to be able to signal to Muon to handle this
    appropriately - fail the current test suite and carry on with the other checks (or move on to the next
    suite in the case of a REQUIRE)
*/
static void failIfInsideTestSuite();
static void failIfInsideTestSuite() {
    if(checkIsInsideTestSuite== 1) {
        hasCurrentTestFailed = 1;
        shouldFailTest = 1;
    }
}

static void abortIfInsideTestSuite();
static void abortIfInsideTestSuite() {
    if(checkIsInsideTestSuite== 1) {
        hasCurrentTestFailed = 1;
        shouldAbortTest = 1;
    }
}

static void incrementWarnings() {
    muonStatsNumWarnings++;
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

/**
    Muon Timer 
    This method is useful in timing the execution of an Muon Test Suite
    To use this, simply call this function before and after the particular code block you want to time, 
    and their difference will give you the time (in seconds). 
    NOTE: This method has been edited to return the time (in nanoseconds). Depending on how large this value
    (e.g: 54890938849ns), we appropriately convert it to milliseconds/seconds before displaying it to stdout.
*/
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

static inline int MUON_isDigit(char c) { return c >= '0' && c <= '9'; }
// If the macro arguments can be decomposed further, we need to print the `In macro ..., so and so failed`
// This method signals whether this message should be printed
// 
// Note: the arguments are of type `char const*` as opposed to `char*`
// This helps mitigate the ``warning: ISO C++ forbids converting a string constant to 'char*'``
// See: https://stackoverflow.com/questions/20944784/why-is-conversion-from-string-constant-to-char-valid-in-c-but-invalid-in-c/20944858
static inline int muonShouldDecomposeMacro(char const* actual, char const* expected, int isStringCmp) {
    // Signal that the macro can be further decomposed if either of the following symbols are present
    int dots = 0;
    int numActualDigits = 0;
    int numExpectedDigits = 0;

    // If not inside a string comparison, we will return `1` only if we determine that `actual` is a variable 
    // name/expression (i.e for a value, we search through each character verifying that each is a digit
    // - for floats, we allow a maximum of 1 '.' char)
    if(!isStringCmp) {
        for(int i=0; i < strlen(actual); i++) {
            if(MUON_isDigit(actual[i])) { numActualDigits++; }
            else if(actual[i] == '.') { 
                dots++; 
                if(dots > 1) { return 1; }
            }
            else { return 1; }
        }
        // Do the same for `expected`
        dots = 0;
        for(int i=0; i < strlen(expected); i++) {
            if(MUON_isDigit(expected[i])) { numExpectedDigits++; }
            else if(expected[i] == '.') { 
                dots++; 
                if(dots > 1) { return 1; }
            }
            else { return 1; }
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

    MUON_GCC_SUPPRESS_WARNING_PUSH
    MUON_GCC_SUPPRESS_WARNING("-Wformat")
    MUON_GCC_SUPPRESS_WARNING("-Wformat-extra-args")

    #define MUON_OVERLOAD_PRINTER(val)                            \
        printf(_Generic((val),                                    \
                            char : "%c",                          \
                            char* : "%s",                         \
                            signed char : "%hhd",                 \
                            unsigned char : "%hhu",               \
                            short : "%hd",                        \
                            unsigned short : "%hu",               \
                            int : "%d",                           \
                            unsigned int : "%u",                  \
                            long : "%ld",                         \
                            long long : "%lld",                   \
                            unsigned long : "%lu",                \
                            unsigned long long : "%"MUON_PRIu64,  \
                            float : "%f",                         \
                            double : "%f",                        \
                            long double : "%Lf",                  \
                            void* : "%p"),                        \
                    (val))
    
    MUON_GCC_SUPPRESS_WARNING_POP
#else
    // If we're here, this means that the Compiler does not support overloadable methods
    #define MUON_OVERLOAD_PRINTER(...)                                                              \
        muonPrintf("Error: Your compiler does not support overloadable methods.");                  \
        muonPrintf("If you think this was an error, please file an issue on Muon's Github repo.")
#endif // MUON_OVERLOADABLE


// ifCondFailsThenPrint is the string representation of the opposite of the truthy value of `cond`
// For example, if `cond` is "!=", then `ifCondFailsThenPrint` will be `==`
#if defined(MUON_CAN_USE_OVERLOADABLES)
    #define __MUONCMP__(actual, expected, cond, space, macroName, failOrAbort)                  \
        do {                                                                                    \
            if(!((actual)cond(expected))) {                                                     \
                muonPrintf("%s:%u: ", __FILE__, __LINE__);                                      \
                muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");                         \
                if(muonShouldDecomposeMacro(#actual, #expected, 0)) {                           \
                    muonColouredPrintf(MUON_COLOUR_BRIGHTCYAN_, "  In macro : ");               \
                    muonColouredPrintf(MUON_COLOUR_BRIGHTCYAN_, "%s( %s, %s )\n",               \
                                                                #macroName,                     \
                                                                #actual, #expected);            \
                }                                                                               \
                muonPrintf("  Expected : %s", #actual);                                         \
                printf(" %s ", #cond space);                                                    \
                MUON_OVERLOAD_PRINTER(expected);                                                \
                muonPrintf("\n");                                                               \
                                                                                                \
                muonPrintf("    Actual : %s", #actual);                                         \
                printf(" == ");                                                                 \
                MUON_OVERLOAD_PRINTER(actual);                                                  \
                muonPrintf("\n");                                                               \
                failOrAbort();                                                                  \
            }                                                                                   \
        }                                                                                       \
        while(0)

// MUON_OVERLOAD_PRINTER does not work on some compilers
#else
    #define __MUONCMP__(actual, expected, cond, space, macroName, failOrAbort)                          \
        do {                                                                                            \
            if(!((actual)cond(expected))) {                                                             \
                muonPrintf("%s:%u: ", __FILE__, __LINE__);                                              \
                muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");                                 \
                if(muonShouldDecomposeMacro(#actual, #expected, 0)) {                                   \
                    muonColouredPrintf(MUON_COLOUR_BRIGHTCYAN_, "  In macro : ");                       \
                    muonColouredPrintf(MUON_COLOUR_BRIGHTCYAN_, "%s( %s, %s )\n",                       \
                                                                #macroName,                             \
                                                                #actual, #expected);                    \
                }                                                                                       \
                muonPrintf("  Expected : %s", #actual);                                                 \
                printf(" %s ", #cond space);                                                            \
                printf(#expected);                                                                      \
                muonPrintf("\n");                                                                       \
                                                                                                        \
                muonPrintf("    Actual : %s", #actual);                                                 \
                printf(" == ");                                                                         \
                printf(#actual);                                                                      \
                muonPrintf("\n");                                                                       \
                failOrAbort();                                                                          \
            }                                                                                           \
        }                                                                                               \
        while(0)
#endif // MUON_CAN_USE_OVERLOADABLES

#define __MUONCMP_STR__(actual, expected, cond, ifCondFailsThenPrint, actualPrint, macroName, failOrAbort)      \
    do {                                                                                                        \
        if(strcmp(actual, expected) cond 0) {                                                                   \
            muonPrintf("%s:%u: ", __FILE__, __LINE__);                                                          \
            muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");                                             \
            if(muonShouldDecomposeMacro(#actual, #expected, 1)) {                                               \
                    muonColouredPrintf(MUON_COLOUR_BRIGHTCYAN_, "  In macro : ");                               \
                    muonColouredPrintf(MUON_COLOUR_BRIGHTCYAN_, "%s( %s, %s )\n",                               \
                                                                #macroName,                                     \
                                                                #actual, #expected);                            \
                }                                                                                               \
            muonPrintf("  Expected : \"%s\" %s \"%s\"\n", actual, #ifCondFailsThenPrint, expected);             \
            muonPrintf("    Actual : %s\n", #actualPrint);                                                      \
            failOrAbort();                                                                                      \
            return;                                                                                             \
        }                                                                                                       \
    }                                                                                                           \
    while(0)  

#define __MUONCMP_STRN__(actual, expected, n, cond, ifCondFailsThenPrint, actualPrint, macroName, failOrAbort)  \
    do {                                                                                                        \
        if(MUON_CAST(int, n) < 0) {                                                                             \
            muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "`n` cannot be negative\n");                             \
            MUON_ABORT;                                                                                         \
        }                                                                                                       \
        if(strncmp(actual, expected, n) cond 0) {                                                               \
            muonPrintf("%s:%u: ", __FILE__, __LINE__);                                                          \
            muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");                                             \
            if(muonShouldDecomposeMacro(#actual, #expected, 1)) {                                               \
                    muonColouredPrintf(MUON_COLOUR_BRIGHTCYAN_, "  In macro : ");                               \
                    muonColouredPrintf(MUON_COLOUR_BRIGHTCYAN_, "%s( %s, %s, %s)\n",                            \
                                                                #macroName,                                     \
                                                                #actual, #expected, #n);                        \
                }                                                                                               \
            muonPrintf("  Expected : \"%.*s\" %s \"%.*s\"\n", MUON_CAST(int, n), actual,                        \
                                                              #ifCondFailsThenPrint,                            \
                                                              MUON_CAST(int, n), expected);                     \
            muonPrintf("    Actual : %s\n", #actualPrint);                                                      \
            failOrAbort();                                                                                      \
            return;                                                                                             \
        }                                                                                                       \
    }                                                                                                           \
    while(0)  


#define __MUONCMP_TF(cond, actual, expected, negateSign, macroName, failOrAbort)    \
    do {                                                                            \
        if(negateSign(cond)) {                                                      \
            muonPrintf("%s:%u: ", __FILE__, __LINE__);                              \
            muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED\n");                 \
            if(muonShouldDecomposeMacro(#actual, MUON_NULL, 0)) {                   \
                    muonColouredPrintf(MUON_COLOUR_BRIGHTCYAN_, "  In macro : ");   \
                    muonColouredPrintf(MUON_COLOUR_BRIGHTCYAN_, "%s( %s )\n",       \
                                                                #macroName,         \
                                                                #cond);             \
                }                                                                   \
            muonPrintf("  Expected : %s\n", #expected);                             \
            muonPrintf("    Actual : %s\n", #actual);                               \
            failOrAbort();                                                          \
        }                                                                           \
    } while(0)

/**
############################################
          {CHECK|REQUIRE} Macros
############################################
*/
#define CHECK_EQ(actual, expected)     __MUONCMP__(actual, expected, ==, "", CHECK_EQ, failIfInsideTestSuite)
#define CHECK_NE(actual, expected)     __MUONCMP__(actual, expected, !=, "", CHECK_NE, failIfInsideTestSuite)
#define CHECK_LT(actual, expected)     __MUONCMP__(actual, expected, < , " ", CHECK_LT, failIfInsideTestSuite)
#define CHECK_LE(actual, expected)     __MUONCMP__(actual, expected, <=, "", CHECK_LE, failIfInsideTestSuite)
#define CHECK_GT(actual, expected)     __MUONCMP__(actual, expected, > , " ", CHECK_GT, failIfInsideTestSuite)
#define CHECK_GE(actual, expected)     __MUONCMP__(actual, expected, >=, "", CHECK_GE, failIfInsideTestSuite)

#define REQUIRE_EQ(actual, expected)   __MUONCMP__(actual, expected, ==, "", REQUIRE_EQ, abortIfInsideTestSuite)
#define REQUIRE_NE(actual, expected)   __MUONCMP__(actual, expected, !=, "", REQUIRE_NE, abortIfInsideTestSuite)
#define REQUIRE_LT(actual, expected)   __MUONCMP__(actual, expected, < , " ", REQUIRE_LT, abortIfInsideTestSuite)
#define REQUIRE_LE(actual, expected)   __MUONCMP__(actual, expected, <=, "", REQUIRE_LE, abortIfInsideTestSuite)
#define REQUIRE_GT(actual, expected)   __MUONCMP__(actual, expected, > , " ", REQUIRE_GT, abortIfInsideTestSuite)
#define REQUIRE_GE(actual, expected)   __MUONCMP__(actual, expected, >=, "", REQUIRE_GE, abortIfInsideTestSuite)

// Whole-string checks
#define CHECK_STREQ(actual, expected)     __MUONCMP_STR__(actual, expected, !=, ==, not equal, CHECK_STREQ, failIfInsideTestSuite)
#define CHECK_STRNEQ(actual, expected)    __MUONCMP_STR__(actual, expected, ==, !=, equal, CHECK_STRNEQ, failIfInsideTestSuite)
#define REQUIRE_STREQ(actual, expected)   __MUONCMP_STR__(actual, expected, !=, ==, not equal, REQUIRE_STREQ, abortIfInsideTestSuite)
#define REQUIRE_STRNEQ(actual, expected)  __MUONCMP_STR__(actual, expected, ==, !=, equal, REQUIRE_STRNEQ, abortIfInsideTestSuite)

// Substring Checks
#define CHECK_STRNE(actual, expected, n)     __MUONCMP_STRN__(actual, expected, n, !=, ==, unequal substrings, CHECK_STRNE, failIfInsideTestSuite)
#define CHECK_STRNNE(actual, expected, n)    __MUONCMP_STRN__(actual, expected, n, ==, !=, equal substrings, CHECK_STRNNE, failIfInsideTestSuite) 
#define REQUIRE_STRNE(actual, expected, n)   __MUONCMP_STRN__(actual, expected, n, !=, ==, unequal substrings, REQUIRE_STRNE, abortIfInsideTestSuite)
#define REQUIRE_STRNNE(actual, expected, n)  __MUONCMP_STRN__(actual, expected, n, ==, !=, equal substrings, REQUIRE_STRNNE, abortIfInsideTestSuite)

// Note: The negate sign `!` must be there for {CHECK|REQUIRE}_TRUE
// Do not remove it
#define CHECK_TRUE(cond)      __MUONCMP_TF(cond, false, true, !, CHECK_TRUE, failIfInsideTestSuite)
#define CHECK_FALSE(cond)     __MUONCMP_TF(cond, true, false, , CHECK_FALSE, failIfInsideTestSuite)

#define REQUIRE_TRUE(cond)    __MUONCMP_TF(cond, false, true, !, REQUIRE_TRUE, abortIfInsideTestSuite)
#define REQUIRE_FALSE(cond)   __MUONCMP_TF(cond, true, false, , REQUIRE_FALSE, abortIfInsideTestSuite)

#define __MUONCHECKREQUIRE__(cond, failOrAbort, macroName, ...)                                \
    do {                                                                                       \
        if(!(cond)) {                                                                          \
            muonPrintf("%s:%u: ", __FILE__, __LINE__);                                         \
            if((sizeof(char[]){__VA_ARGS__}) <= 1)                                             \
                muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED");                          \
            else                                                                               \
                muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, __VA_ARGS__);                       \
            printf("\n");                                                                      \
            printf("The following assertion failed: \n");                                      \
            muonColouredPrintf(MUON_COLOUR_BRIGHTCYAN_, "    %s( %s )\n", #macroName, #cond);  \
            failOrAbort();                                                                     \
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
// https://stackoverflow.com/questions/3046889/optional-parameters-with-c-macros
// and: https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
#define GET_3RD_ARG(arg1, arg2, arg3, ...)   arg3

#define CHECK_1_ARGS(cond)              __MUONCHECKREQUIRE__(cond, failIfInsideTestSuite, CHECK, "FAILED")
#define CHECK_2_ARGS(cond, message)     __MUONCHECKREQUIRE__(cond, failIfInsideTestSuite, CHECK, message)
#define CHECK_MACRO_CHOOSER(...)        GET_3RD_ARG(__VA_ARGS__, CHECK_2_ARGS, CHECK_1_ARGS, )

#define REQUIRE_1_ARGS(cond)            __MUONCHECKREQUIRE__(cond, abortIfInsideTestSuite, REQUIRE, "FAILED")
#define REQUIRE_2_ARGS(cond, message)   __MUONCHECKREQUIRE__(cond, abortIfInsideTestSuite, REQUIRE, message)
#define REQUIRE_MACRO_CHOOSER(...)      GET_3RD_ARG(__VA_ARGS__, REQUIRE_2_ARGS, REQUIRE_1_ARGS, )

#define CHECK(...)      CHECK_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)
#define REQUIRE(...)    REQUIRE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#define CHECK_NULL(val)       CHECK(val == MUON_NULL)
#define CHECK_NOT_NULL(val)   CHECK(val != MUON_NULL)

#define WARN(msg)                                                        \
    incrementWarnings();                                                 \
    muonColouredPrintf(MUON_COLOUR_YELLOW_, "%s:%u: WARNING: %s\n", __FILE__, __LINE__, #msg)

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

#define TEST(TESTSUITE, TESTNAME)                                                               \
    MUON_EXTERN struct muonTestStateStruct muonTestContext;                                     \
    static void _MUON_TEST_FUNC_##TESTSUITE##_##TESTNAME();                                     \
    MUON_TEST_INITIALIZER(muon_register_##TESTSUITE##_##TESTNAME) {                             \
        const MUON_Ull index = muonTestContext.numTestSuites++;                                 \
        const char* namePart = #TESTSUITE "." #TESTNAME;                                        \
        const MUON_Ull nameSize = strlen(namePart) + 1;                                         \
        char* name = MUON_PTRCAST(char* , malloc(nameSize));                                    \
        muonTestContext.tests = MUON_PTRCAST(                                                   \
                                    struct muonTestSuiteStruct* ,                               \
                                    muon_realloc(MUON_PTRCAST(void* , muonTestContext.tests),   \
                                                sizeof(struct muonTestSuiteStruct) *            \
                                                    muonTestContext.numTestSuites));            \
        muonTestContext.tests[index].func = &_MUON_TEST_FUNC_##TESTSUITE##_##TESTNAME;          \
        muonTestContext.tests[index].name = name;                                               \
        MUON_SNPRINTF(name, nameSize, "%s", namePart);                                          \
    }                                                                                           \
    void _MUON_TEST_FUNC_##TESTSUITE##_##TESTNAME()


#define TEST_F_SETUP(FIXTURE)                                                  \
    static void __MUON_TEST_FIXTURE_SETUP_##FIXTURE(struct FIXTURE* muon)

#define TEST_F_TEARDOWN(FIXTURE)                                               \
    static void __MUON_TEST_FIXTURE_TEARDOWN_##FIXTURE(struct FIXTURE* muon)

#define TEST_F(FIXTURE, NAME)                                                  \
    MUON_EXTERN struct muonTestStateStruct muonTestContext;                    \
    static void __MUON_TEST_FIXTURE_SETUP_##FIXTURE(struct FIXTURE*);          \
    static void __MUON_TEST_FIXTURE_TEARDOWN_##FIXTURE(struct FIXTURE*);       \
    static void __MUON_TEST_FIXTURE_RUN_##FIXTURE##_##NAME(struct FIXTURE*);   \
                                                                               \
    static void __MUON_TEST_FIXTURE_##FIXTURE##_##NAME() {                     \
        struct FIXTURE fixture;                                                \
        memset(&fixture, 0, sizeof(fixture));                                  \
        __MUON_TEST_FIXTURE_SETUP_##FIXTURE(&fixture);                         \
        if (hasCurrentTestFailed == 1) {                                       \
            return;                                                            \
        }                                                                      \
                                                                               \
        __MUON_TEST_FIXTURE_RUN_##FIXTURE##_##NAME(&fixture);                  \
        __MUON_TEST_FIXTURE_TEARDOWN_##FIXTURE(&fixture);                      \
    }                                                                          \
                                                                               \
    MUON_TEST_INITIALIZER(muon_register_##FIXTURE##_##NAME) {                  \
        const MUON_Ull index = muonTestContext.numTestSuites++;                \
        const char* namePart = #FIXTURE "." #NAME;                             \
        const MUON_Ull nameSize = strlen(namePart) + 1;                        \
        char* name = MUON_PTRCAST(char* , malloc(nameSize));                   \
        muonTestContext.tests = MUON_PTRCAST(                                  \
                                    struct muonTestSuiteStruct*,               \
                                    muon_realloc(MUON_PTRCAST(void *, muonTestContext.tests),               \
                                                                sizeof(struct muonTestSuiteStruct) *        \
                                                                        muonTestContext.numTestSuites));    \
        muonTestContext.tests[index].func = &__MUON_TEST_FIXTURE_##FIXTURE##_##NAME;                        \
        muonTestContext.tests[index].name = name;                              \
        MUON_SNPRINTF(name, nameSize, "%s", namePart);                         \
    }                                                                          \
    static void __MUON_TEST_FIXTURE_RUN_##FIXTURE##_##NAME(struct FIXTURE* muon)


static int muonShouldFilterTest(const char* filter, const char* testcase);
static int muonShouldFilterTest(const char* filter, const char* testcase) {
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

        if((*filter_curr != MUON_NULLCHAR) || ((*testcase_curr == MUON_NULLCHAR) && ((filter == filter_curr) || 
            (filter_curr[-1] != '*')))) {
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

        printf("    Total test suites:          %" MUON_PRIu64 "\n", muonStatsTotalTestSuites);
        printf("    Total suites run:           %" MUON_PRIu64 "\n", muonStatsTestsRan);
        printf("    Total warnings generated:   %" MUON_PRIu64 "\n", muonStatsNumWarnings);
        printf("    Total suites skipped:       %" MUON_PRIu64 "\n", muonStatsSkippedTests);
        printf("    Total suites failed:        %" MUON_PRIu64 "\n", muonStatsNumTestsFailed);
    }

    if(muonStatsNumTestsFailed > 0) {
        muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "FAILED: ");
        printf("%" MUON_PRIu64 " failed, %" MUON_PRIu64 " passed in ", muonStatsNumTestsFailed, muonStatsTestsRan - muonStatsNumTestsFailed);
        muonClockPrintDuration(duration);
        printf("\n");
        
        for (MUON_Ull i = 0; i < muonStatsNumFailedTestSuites; i++) {
            muonColouredPrintf(MUON_COLOUR_BRIGHTRED_, "  [ FAILED ] %s\n", muonTestContext.tests[muonStatsFailedTestSuites[i]].name);
        }
    } else if(muonStatsNumTestsFailed == 0 && muonStatsTotalTestSuites > 0) {
        MUON_UInt64 total_tests_passed = muonStatsTestsRan - muonStatsNumTestsFailed;
        muonColouredPrintf(MUON_COLOUR_BRIGHTGREEN_, "SUCCESS: ");
        printf("%" MUON_PRIu64 " test suites passed in ", total_tests_passed);
        muonClockPrintDuration(duration);
        printf("\n");
    } else {
        muonColouredPrintf(MUON_COLOUR_BRIGHTYELLOW_, "WARNING: ");
        printf("No test suites were found. If you think this was an error, please file an issue on Muon's Github repo.");
        printf("\n");
    }


    if(muonTestContext.foutput)
        fprintf(muonTestContext.foutput, "</testsuite>\n</testsuites>\n");

    return muonCleanup();
}

/**
    We need to declare these variables here because they're used in multiple translation units (if compiled 
    as so).
    For example, if test1.c(pp) #includes `Muon/Muon.h` and test2.c(pp) does the same, declaring the variables
    static will only make them exist in the first translation unit the compiler sees. 
    We can get around this by compiling only one file (eg. main.c(pp)) and including the other source files 
    using `#include`, but this would be counter-productive.

    If we used C++ as the base language for this project, we could have got around this problem using OOP
    constructs, but C++ is not (and should not) be the language this project is written in. 
    With C, the best (and closest) thing we have are global variables that persists through the _entire_
    compilation project (all testing source files).
    See: https://stackoverflow.com/questions/1856599/when-to-use-static-keyword-before-global-variables
*/
#define MUON_ONLY_GLOBALS()                       \
    volatile int checkIsInsideTestSuite = 0;      \
    volatile int hasCurrentTestFailed = 0;              \
    volatile int shouldFailTest = 0;             \
    volatile int shouldAbortTest = 0;             \
    MUON_UInt64 muonStatsNumWarnings = 0;

// If a user wants to define their own `main()` function, this _must_ be at the very end of the functtion
#define MUON_NO_MAIN()                                        \
    struct muonTestStateStruct muonTestContext = {0, 0, 0};   \
    MUON_ONLY_GLOBALS()

// Define a main() function to call into muon.h and start executing tests.
#define MUON_MAIN()                                                             \
    /* Define the global struct that will hold the data we need to run Muon. */ \
    struct muonTestStateStruct muonTestContext = {0, 0, 0};                     \
    MUON_ONLY_GLOBALS()                                                         \
                                                                                \
    int main(int argc, char** argv) {                                           \
        return muon_main(argc, argv);                                           \
    }

MUON_DISABLE_WARNINGS_POP

#endif // MUON_TEST_H_