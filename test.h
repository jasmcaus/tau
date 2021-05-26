#pragma once 

#include <Muon/Types.h>
#include <Muon/Misc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    #define MUON_WIN_        1
    #pragma warning(push, 0)
        #include <Windows.h>
        #include <io.h>
    #pragma warning(pop)
#endif // _WIN32

// Enable the use of the non-standard keyword __attribute__ to silence warnings under some compilers
#if defined(__GNUC__) || defined(__clang__)
    #define MUON_ATTRIBUTE_(attr)    __attribute__((attr))
#else
    #define MUON_ATTRIBUTE_(attr)
#endif // __GNUC__

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
static inline double muon_clock() {
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

static void muon_clock_print_duration(double nanoseconds_duration) {
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

static MUON_UInt64 muon_stats_total_test_suites = 0;
static MUON_UInt64 muon_stats_tests_ran = 0;
static MUON_UInt64 muon_stats_tests_failed = 0;
static MUON_UInt64 muon_stats_skipped_tests = 0; 

// Overridden in `muon_main` if the cmdline option `--no-color` is passed
static int muon_should_colourize_output = 1;
static int muon_disable_summary = 0; 

static char* muon_argv0_ = MUON_NULL;
static const char* filter = MUON_NULL;

static MUON_Ll* muon_stats_failed_testcases = MUON_NULL; 
static MUON_Ll muon_stats_num_failed_testcases = 0;

static int hasCurrentTestFailed = 0;
static int checkIsInsideTestSuite = 0;

typedef void (*muon_testsuite_t)();
struct muon_test_s {
    muon_testsuite_t func;
    char* name;
};

struct muon_test_state_s {
    struct muon_test_s* tests;
    int num_test_suites;
};

static struct muon_test_state_s mt = {0, 0};

static inline void* muon_realloc(void* const ptr, int new_size) {
    void* const new_ptr = realloc(ptr, new_size);

    if(new_ptr == NULL)
        free(new_ptr);

    return new_ptr;
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


#if defined(_MSC_VER)
    #define MUON_WEAK     inline
    #define MUON_UNUSED
#else
    #define MUON_WEAK     __attribute__((weak))
    #define MUON_UNUSED   __attribute__((unused))
#endif // _MSC_VER

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
muon_coloured_printf_(int colour, const char* fmt, ...);
static inline int MUON_ATTRIBUTE_(format (printf, 2, 3))
muon_coloured_printf_(int colour, const char* fmt, ...) {
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


#define MUON_PRINTF(...)                                 \
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

    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(float f) { MUON_PRINTF("%f", MUON_CAST(double, f)); }
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(double d) { MUON_PRINTF("%f", d); }
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(long double d) { MUON_PRINTF("%Lf", d); }
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(int i) { MUON_PRINTF("%d", i); }
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(unsigned int i) { MUON_PRINTF("%u", i); }
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(long int i) { MUON_PRINTF("%ld", i); }
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(long unsigned int i) { MUON_PRINTF("%lu", i); }
    MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(const void* p) { MUON_PRINTF("%p", p); }

    // long long is in C++ only
    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) || defined(__cplusplus) && (__cplusplus >= 201103L)
        MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(long long int i);
        MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(long long unsigned int i);

        MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(long long int i) { MUON_PRINTF("%lld", i); }
        MUON_WEAK MUON_OVERLOADABLE void MUON_OVERLOAD_PRINTER(long long unsigned int i) { MUON_PRINTF("%llu", i); }
    #endif // __STDC_VERSION__

#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    #ifndef MUON_CAN_USE_OVERLOADABLES
        #define MUON_CAN_USE_OVERLOADABLES
    #endif // MUON_CAN_USE_OVERLOADABLES
    
    #define MUON_OVERLOAD_PRINTER(val)                                \
        MUON_PRINTF(_Generic((val),                                   \
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
    // If we're here, this means that the Compiler does not support overloadable methods
    #define MUON_OVERLOAD_PRINTER(...)                                                              \
        MUON_PRINTF("Error: Your compiler does not support overloadable methods.");                 \
        MUON_PRINTF("If you think this was an error, please file an issue on Muon's Github repo.")
#endif // MUON_OVERLOADABLE

#define TEST(TESTSUITE, TESTNAME)                                                       \
    static void _MUON_TEST_FUNC_##TESTSUITE##_##TESTNAME();                   \
    MUON_TEST_INITIALIZER(muon_register_##TESTSUITE##_##TESTNAME) {                     \
        const MUON_Ll index = mt.num_test_suites++;                                   \
        const char* name_part = #TESTSUITE "." #TESTNAME;                               \
        const MUON_Ll name_size = strlen(name_part) + 1;                                \
        char* name = MUON_PTRCAST(char* , malloc(name_size));                           \
        mt.tests = MUON_PTRCAST(struct muon_test_s* ,                                                       \
                                    muon_realloc(MUON_PTRCAST(void* , mt.tests), \
                                                sizeof(struct muon_test_s) *             \
                                                    mt.num_test_suites));       \
        mt.tests[index].func = &_MUON_TEST_FUNC_##TESTSUITE##_##TESTNAME;                  \
        mt.tests[index].name = name;                                            \
        MUON_SNPRINTF(name, name_size, "%s", name_part);                                \
    }       \
    void _MUON_TEST_FUNC_##TESTSUITE##_##TESTNAME()

// ifCondFailsThenPrint is the string representation of the opposite of the truthy value of `cond`
// For example, if `cond` is "!=", then `ifCondFailsThenPrint` will be `==`
#define __MUONCHECK__(actual, expected, cond, ifCondFailsThenPrint)       \
    do {                                                                  \
        muon_coloured_printf_(MUON_COLOUR_BRIGHTCYAN_, "    INSIDE __MUONCHECK__ = %d\n", checkIsInsideTestSuite); \
        if(!((actual)cond(expected))) {                                   \
            MUON_PRINTF("%s:%u: ", __FILE__, __LINE__);                   \
            muon_coloured_printf_(MUON_COLOUR_BRIGHTRED_, "FAILED\n");\
            MUON_PRINTF("  Expected : ");                                 \
            MUON_OVERLOAD_PRINTER(actual);                                \
            printf(" %s ", #cond);                                        \
            MUON_OVERLOAD_PRINTER(expected);                              \
            MUON_PRINTF("\n");                                            \
                                                                            \
            MUON_PRINTF("    Actual : ");                                 \
            MUON_OVERLOAD_PRINTER(actual);                                \
            printf(" %s ", #ifCondFailsThenPrint);                        \
            MUON_OVERLOAD_PRINTER(expected);                              \
            MUON_PRINTF("\n");                                            \
            muon_coloured_printf_(MUON_COLOUR_BRIGHTCYAN_, "        BEFORE CHECK:\n"); \
            muon_coloured_printf_(MUON_COLOUR_BRIGHTCYAN_, "            checkIsInsideTestSuite = %d\n", checkIsInsideTestSuite); \
            muon_coloured_printf_(MUON_COLOUR_BRIGHTCYAN_, "            hasCurrentTestFailed = %d\n", hasCurrentTestFailed); \
            failIfInsideTestSuite();                                      \
            muon_coloured_printf_(MUON_COLOUR_BRIGHTCYAN_, "        AFTER CHECK:\n"); \
            muon_coloured_printf_(MUON_COLOUR_BRIGHTCYAN_, "            checkIsInsideTestSuite = %d\n", checkIsInsideTestSuite); \
            muon_coloured_printf_(MUON_COLOUR_BRIGHTCYAN_, "            hasCurrentTestFailed = %d\n", hasCurrentTestFailed); \
        }                                                                 \
    }                                                                     \
    while(0)

#define CHECK_EQ(actual, expected)     __MUONCHECK__(actual, expected, ==, !=)


static void failIfInsideTestSuite() {
    if(checkIsInsideTestSuite == 1) {
        muon_coloured_printf_(MUON_COLOUR_BRIGHTYELLOW_, "INSIDE TEST SUITE\n");
        hasCurrentTestFailed = 1;
    }
}

static void run_all_tests() {
    for(int i=0; i<mt.num_test_suites; i++) {
        hasCurrentTestFailed = 0;
        checkIsInsideTestSuite = 1;

        muon_coloured_printf_(MUON_COLOUR_BRIGHTGREEN_, "[ RUN      ] ");
        muon_coloured_printf_(MUON_COLOUR_DEFAULT_, "%s\n", mt.tests[i].name);

        // Start the timer
        double start = muon_clock();

        mt.tests[i].func();

        // Stop the timer
        double duration = muon_clock() - start;

        // if(hasCurrentTestFailed == 1) {
        //     printf("TEST FAILED\n");
        // } else {
        //     printf("TEST DID NOT FAIL\n");
        // }
        if(hasCurrentTestFailed == 1) {
            const MUON_Ll failed_testcase_index = muon_stats_num_failed_testcases++;
            muon_stats_failed_testcases = MUON_PTRCAST(MUON_Ll*, 
                                            muon_realloc(MUON_PTRCAST(void* , muon_stats_failed_testcases), 
                                                          sizeof(MUON_Ll) * muon_stats_num_failed_testcases));
            muon_stats_failed_testcases[failed_testcase_index] = i;
            muon_stats_tests_failed++;
            muon_coloured_printf_(MUON_COLOUR_BRIGHTRED_, "[  FAILED  ] ");
            muon_coloured_printf_(MUON_COLOUR_DEFAULT_, "%s (", mt.tests[i].name);
            muon_clock_print_duration(duration);
            printf(")\n");
        } else {
            muon_coloured_printf_(MUON_COLOUR_BRIGHTGREEN_, "[       OK ] ");
            muon_coloured_printf_(MUON_COLOUR_DEFAULT_, "%s (", mt.tests[i].name);
            muon_clock_print_duration(duration);
            printf(")\n");
        }
    }
    muon_coloured_printf_(MUON_COLOUR_BRIGHTGREEN_, "[==========] ");
    muon_coloured_printf_(MUON_COLOUR_DEFAULT_, "%" MUON_PRIu64 " test suites ran\n", muon_stats_tests_ran);
}

static int muon_cleanup() {
    for (MUON_Ll i = 0; i < mt.num_test_suites; i++)
        free(MUON_PTRCAST(void* , mt.tests[i].name));

    free(MUON_PTRCAST(void* , muon_stats_failed_testcases));
    free(MUON_PTRCAST(void* , mt.tests));

    return MUON_CAST(int, muon_stats_tests_failed);
}

static inline int muon_main(int argc, char** argv);
inline int muon_main(int argc, char** argv) {
    muon_stats_total_test_suites = MUON_CAST(MUON_UInt64, mt.num_test_suites);
    muon_argv0_ = argv[0];

    muon_stats_tests_ran = muon_stats_total_test_suites - muon_stats_skipped_tests;

    // Begin tests`
    muon_coloured_printf_(MUON_COLOUR_BRIGHTGREEN_, "[==========] ");
    muon_coloured_printf_(MUON_COLOUR_BOLD_, "Running %" MUON_PRIu64 " test suites.\n", MUON_CAST(MUON_UInt64, muon_stats_tests_ran));

    // Start the entire Test Session timer
    double start = muon_clock();

    // Run tests
    run_all_tests();

    // End the entire Test Session timer
    double duration = muon_clock() - start;

    // Write a Summary
    muon_coloured_printf_(MUON_COLOUR_BRIGHTGREEN_, "[  PASSED  ] %" MUON_PRIu64 " suites\n", muon_stats_tests_ran - muon_stats_tests_failed);
    muon_coloured_printf_(MUON_COLOUR_BRIGHTRED_, "[  FAILED  ] %" MUON_PRIu64 " %s\n", muon_stats_tests_failed, muon_stats_tests_failed == 1 ? "suite" : "suites");

    if(!muon_disable_summary) {
        muon_coloured_printf_(MUON_COLOUR_BOLD_, "\nSummary:\n");

        printf("    Total test suites:      %" MUON_PRIu64 "\n", muon_stats_total_test_suites);
        printf("    Total suites run:       %" MUON_PRIu64 "\n", muon_stats_tests_ran);
        printf("    Total suites skipped:   %" MUON_PRIu64 "\n", muon_stats_skipped_tests);
        printf("    Total suites failed:    %" MUON_PRIu64 "\n", muon_stats_tests_failed);
    }

    if(muon_stats_tests_failed != 0) {
        muon_coloured_printf_(MUON_COLOUR_BRIGHTRED_, "FAILED: ");
        printf("%" MUON_PRIu64 " failed, %" MUON_PRIu64 " passed in ", muon_stats_tests_failed, muon_stats_tests_ran - muon_stats_tests_failed);
        muon_clock_print_duration(duration);
        printf("\n");
        
        for (MUON_Ll i = 0; i < muon_stats_num_failed_testcases; i++) {
            muon_coloured_printf_(MUON_COLOUR_BRIGHTRED_, "  [ FAILED ] %s\n", mt.tests[muon_stats_failed_testcases[i]].name);
        }
    } else {
        MUON_UInt64 total_tests_passed = muon_stats_tests_ran - muon_stats_tests_failed;
        muon_coloured_printf_(MUON_COLOUR_BRIGHTGREEN_, "SUCCESS: ");
        printf("%" MUON_PRIu64 " test suites passed in ", total_tests_passed);
        muon_clock_print_duration(duration);
        printf("\n");
    }

    return muon_cleanup();
}

static void change(int argc, char** argv) {
    printf("BEFORE =============== %d\n", hasCurrentTestFailed);
    muon_main(argc, argv);
    printf("AFTER ================ %d\n", hasCurrentTestFailed);
}