#ifndef ARROW_H
#define ARROW_H

#include <Arrow/Types.h>

/************************
 *** Public interface ***
 ************************/

/* By default, "arrow.h" provides the main program entry point (function
 * main()). However, if the test suite is composed of multiple source files
 * which include "arrow.h", then this causes a problem of multiple main()
 * definitions. To avoid this problem, #define macro TEST_NO_MAIN in all
 * compilation units but one.
 */

/* Macro to specify list of unit tests in the suite.
 * The unit test implementation MUST provide list of unit tests it implements
 * with this macro:
 *
 *   TEST_LIST = {
 *       { "test1_name", test1_func_ptr },
 *       { "test2_name", test2_func_ptr },
 *       ...
 *       { NULL, NULL }     // zeroed record marking the end of the list
 *   };
 *
 * The list specifies names of each test (must be unique) and pointer to
 * a function implementing it. The function does not take any arguments
 * and has no return values, i.e. every test function has to be compatible
 * with this prototype:
 *
 *   void test_func(void);
 *
 * Note the list has to be ended with a zeroed record.
 */
// #define TEST_LIST               const struct arrow_test_ arrow_list_[]


/* Macros for testing whether an unit test succeeds or fails. These macros
 * can be used arbitrarily in functions implementing the unit tests.
 *
 * If any condition fails throughout execution of a test, the test fails.
 *
 * TEST_CHECK takes only one argument (the condition), TEST_CHECK allows
 * also to specify an error message to print out if the condition fails.
 * (It expects printf-like format string and its parameters). The macros
 * return non-zero (condition passes) or 0 (condition fails).
 *
 * That can be useful when more conditions should be checked only if some
 * preceding condition passes, as illustrated in this code snippet:
 *
 *   SomeStruct* ptr = allocate_some_struct();
 *   if(TEST_CHECK(ptr != NULL)) {
 *       TEST_CHECK(ptr->member1 < 100);
 *       TEST_CHECK(ptr->member2 > 200);
 *   }
 */
// #define TEST_CHECK(cond,...)   arrow_check_((cond), __FILE__, __LINE__, __VA_ARGS__)
#define TEST_CHECK(cond,...)   arrow_check_((cond), __FILE__, __LINE__, __VA_ARGS__)
#define TEST_ASSERT(cond,...)                                                 \
    do {                                                                       \
        if(!arrow_check_((cond), __FILE__, __LINE__, __VA_ARGS__))           \
            arrow_abort_();                                                  \
    } while(0)


#ifdef __cplusplus
/* Macros to verify that the code (the 1st argument) throws exception of given
 * type (the 2nd argument). (Note these macros are only available in C++.)
 *
 * TEST_EXCEPTION_ is like TEST_EXCEPTION but accepts custom printf-like
 * message.
 *
 * For example:
 *
 *   TEST_EXCEPTION(function_that_throw(), ExpectedExceptionType);
 *
 * If the function_that_throw() throws ExpectedExceptionType, the check passes.
 * If the function throws anything incompatible with ExpectedExceptionType
 * (or if it does not thrown an exception at all), the check fails.
 */
// #define TEST_EXCEPTION(code, exctype)                                          \
//     do {                                                                       \
//         bool exc_ok_ = false;                                                  \
//         const char *msg_ = NULL;                                               \
//         try {                                                                  \
//             code;                                                              \
//             msg_ = "No exception thrown.";                                     \
//         } catch(exctype const&) {                                              \
//             exc_ok_= true;                                                     \
//         } catch(...) {                                                         \
//             msg_ = "Unexpected exception thrown.";                             \
//         }                                                                      \
//         arrow_check_(exc_ok_, __FILE__, __LINE__, #code " throws " #exctype);\
//         if(msg_ != NULL)                                                       \
//             arrow_message_("%s", msg_);                                      \
//     } while(0)
#define TEST_EXCEPTION(code, exctype, ...)                                    \
    do {                                                                       \
        bool exc_ok_ = false;                                                  \
        const char *msg_ = NULL;                                               \
        try {                                                                  \
            code;                                                              \
            msg_ = "No exception thrown.";                                     \
        } catch(exctype const&) {                                              \
            exc_ok_= true;                                                     \
        } catch(...) {                                                         \
            msg_ = "Unexpected exception thrown.";                             \
        }                                                                      \
        arrow_check_(exc_ok_, __FILE__, __LINE__, __VA_ARGS__);              \
        if(msg_ != NULL)                                                       \
            arrow_message_("%s", msg_);                                      \
    } while(0)
#endif  /* #ifdef __cplusplus */


/* Sometimes it is useful to split execution of more complex unit tests to some
 * smaller parts and associate those parts with some names.
 *
 * This is especially handy if the given unit test is implemented as a loop
 * over some vector of multiple testing inputs. Using these macros allow to use
 * sort of subtitle for each iteration of the loop (e.g. outputting the input
 * itself or a name associated to it), so that if any TEST_CHECK condition
 * fails in the loop, it can be easily seen which iteration triggers the
 * failure, without the need to manually output the iteration-specific data in
 * every single TEST_CHECK inside the loop body.
 *
 * TEST_CASE allows to specify only single string as the name of the case,
 * TEST_CASE_ provides all the power of printf-like string formatting.
 *
 * Note that the test cases cannot be nested. Starting a new test case ends
 * implicitly the previous one. To end the test case explicitly (e.g. to end
 * the last test case after exiting the loop), you may use TEST_CASE(NULL).
 */
// #define TEST_CASE_(...)         arrow_case_(__VA_ARGS__)
// #define TEST_CASE(name)         arrow_case_("%s", name)


// /* Maximal output per TEST_CASE call. Longer messages are cut.
//  * You may define another limit prior including "arrow.h"
//  */
// #ifndef TEST_CASE_MAXSIZE
//     #define TEST_CASE_MAXSIZE    64
// #endif


/* printf-like macro for outputting an extra information about a failure.
 *
 * Intended use is to output some computed output versus the expected value,
 * e.g. like this:
 *
 *   if(!TEST_CHECK(produced == expected)) {
 *       TEST_MSG("Expected: %d", expected);
 *       TEST_MSG("Produced: %d", produced);
 *   }
 *
 * Note the message is only written down if the most recent use of any checking
 * macro (like e.g. TEST_CHECK or TEST_EXCEPTION) in the current test failed.
 * This means the above is equivalent to just this:
 *
 *   TEST_CHECK(produced == expected);
 *   TEST_MSG("Expected: %d", expected);
 *   TEST_MSG("Produced: %d", produced);
 *
 * The macro can deal with multi-line output fairly well. It also automatically
 * adds a final new-line if there is none present.
 */
// #define TEST_MSG(...)           arrow_message_(__VA_ARGS__)


// /* Maximal output per TEST_MSG call. Longer messages are cut.
//  * You may define another limit prior including "arrow.h"
//  */
// #ifndef TEST_MSG_MAXSIZE
//     #define TEST_MSG_MAXSIZE    1024
// #endif


/* Macro for dumping a block of memory.
 *
 * Its intended use is very similar to what TEST_MSG is for, but instead of
 * generating any printf-like message, this is for dumping raw block of a
 * memory in a hexadecimal form:
 *
 *   TEST_CHECK(size_produced == size_expected &&
 *              memcmp(addr_produced, addr_expected, size_produced) == 0);
 *   TEST_DUMP("Expected:", addr_expected, size_expected);
 *   TEST_DUMP("Produced:", addr_produced, size_produced);
 */
// #define TEST_DUMP(title, addr, size)    arrow_dump_(title, addr, size)

/* Maximal output per TEST_DUMP call (in bytes to dump). Longer blocks are cut.
 * You may define another limit prior including "arrow.h"
 */
// #ifndef TEST_DUMP_MAXSIZE
//     #define TEST_DUMP_MAXSIZE   1024
// #endif

/**********************
 *** Implementation ***
 **********************/

/* The unit test files should not rely on anything below. */

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
    #define ARROW_UNIX_       1
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

/* Enable the use of the non-standard keyword __attribute__ to silence warnings under some compilers */
#if defined(__GNUC__) || defined(__clang__)
    #define ARROW_ATTRIBUTE_(attr)    __attribute__((attr))
#else
    #define ARROW_ATTRIBUTE_(attr)
#endif

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

/* Note our global private identifiers end with '_' to mitigate risk of clash
 * with the unit tests implementation. */

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

typedef void (*arrow_testcase_t)(int *, Ll);

struct arrow_test_data_s {
    arrow_testcase_t func;
    Ll index;
    char* name;

    // Acutest
    unsigned char flags;
    double duration;
};

struct arrow_test_s {
    struct arrow_test_data_s* tests;
    Ll num_tests;
    FILE* foutput;

    // Acutest
    const char* name;
    void (*func)(void);
};

// An extern to the global state that Arrow needs to execute
extern struct arrow_test_s  arrow_test_;

enum {
    ARROW_FLAG_RUN_ = 1 << 0,
    ARROW_FLAG_SUCCESS_ = 1 << 1,
    ARROW_FLAG_FAILURE_ = 1 << 2,
};

// extern const struct arrow_test_ arrow_list_[];

int arrow_check(int cond, const char* file, int line, const char* fmt, ...);
// void arrow_case_(const char* fmt, ...);
// void arrow_message_(const char* fmt, ...);
void arrow_dump_(const char* title, const void* addr, size_t size);
void arrow_abort_(void) ARROW_ATTRIBUTE_(noreturn);


#ifndef TEST_NO_MAIN

static char* arrow_argv0_ = NULL;
static Ll arrow_total_tests = 0; 
// static size_t arrow_list_size_ = 0;
static struct arrow_test_data_* arrow_test_data_ = NULL;
static size_t arrow_count_ = 0;
static bool arrow_no_summary_ = 0;
static int arrow_skip_mode_ = 0;
static int arrow_cond_failed_ = 0;
static int arrow_was_aborted_ = 0;
static FILE *arrow_xml_output_ = NULL;

static int arrow_stat_failed_units_ = 0;
static int arrow_stat_run_units_ = 0;

static const struct arrow_test_* arrow_current_test_ = NULL;
static int arrow_current_index_ = 0;
// static char arrow_case_name_[TEST_CASE_MAXSIZE] = "";
static int arrow_test_already_logged_ = 0;
// static int arrow_case_already_logged_ = 0;
static int arrow_verbose_level_ = 2;
static int arrow_test_failures_ = 0;
static int arrow_colorize_ = 0;
static int arrow_timer_ = 0;



#define ARROW_PRINTF(...) \
    if(arrow_test_.foutput) { \
        fprintf(arrow_test_.foutput, __VA_ARGS__); \
    } \
    printf(__VA_ARGS__) 


// String Macros
#if defined(__clang__)
    #define ARROW_STRNCMP(x, y, size)                                              \
        _Pragma("clang diagnostic push")                                             \
        _Pragma("clang diagnostic ignored \"-Wdisabled-macro-expansion\"")       \
        strncmp(x, y, size) _Pragma("clang diagnostic pop")
#else
    #define ARROW_STRNCMP(x, y, size)   strncmp(x, y, size)
#endif


// GTest-Like Macros
#define __ARROW_EXPECT(x, y, cond) \
    do { \
        ARROW_AUTO(x) xEval = (x) \
        ARROW_AUTO(y) yEval = (y) \
\
        if(!((xEval)cond(yEval))) { \
            ARROW_COLOURED_PRINTF(ARROW_COLOUR_RED_, "%s:%u: FAILURE\n", __FILE__, __LINE__); \
            ARROW_PRINTF("  Expected : "); \
            ARROW_PRINTER(xEval); \
            ARROW_PRINTF("\n"); \
            ARROW_PRINTF("    Actual : "); \
            ARROW_PRINTER(yEval); \
            ARROW_PRINTF("\n"); \
            *result = false; \
        } \
    } while(0)

#define EXPECT_EQ(x, y)     __ARROW_EXPECT(x, y, ==)
#define EXPECT_NE(x, y)     __ARROW_EXPECT(x, y, !=)
#define EXPECT_LT(x, y)     __ARROW_EXPECT(x, y, <)
#define EXPECT_LE(x, y)     __ARROW_EXPECT(x, y, <=)
#define EXPECT_GT(x, y)     __ARROW_EXPECT(x, y, >)
#define EXPECT_GE(x, y)     __ARROW_EXPECT(x, y, >=)


static void
arrow_cleanup_(void)
{
    free((void*) arrow_test_data_);
}

static void ARROW_ATTRIBUTE_(noreturn)
arrow_exit_(int exit_code)
{
    arrow_cleanup_();
    exit(exit_code);
}

#if defined ARROW_WIN_
    typedef LARGE_INTEGER arrow_timer_type_;
    static LARGE_INTEGER arrow_timer_freq_;
    static arrow_timer_type_ arrow_timer_start_;
    static arrow_timer_type_ arrow_timer_end_;

    static void
    arrow_timer_init_(void)
    {
        QueryPerformanceFrequency(&arrow_timer_freq_);
    }

    static void
    arrow_timer_get_time_(LARGE_INTEGER* ts)
    {
        QueryPerformanceCounter(ts);
    }

    static double
    arrow_timer_diff_(LARGE_INTEGER start, LARGE_INTEGER end)
    {
        double duration = (double)(end.QuadPart - start.QuadPart);
        duration /= (double)arrow_timer_freq_.QuadPart;
        return duration;
    }

    static void
    arrow_timer_print_diff_(void)
    {
        printf("%.6lf secs", arrow_timer_diff_(arrow_timer_start_, arrow_timer_end_));
    }
#elif defined ARROW_HAS_POSIX_TIMER_
    static clockid_t arrow_timer_id_;
    typedef struct timespec arrow_timer_type_;
    static arrow_timer_type_ arrow_timer_start_;
    static arrow_timer_type_ arrow_timer_end_;

    static void
    arrow_timer_init_(void)
    {
        if(arrow_timer_ == 1)
            arrow_timer_id_ = CLOCK_MONOTONIC;
        else if(arrow_timer_ == 2)
            arrow_timer_id_ = CLOCK_PROCESS_CPUTIME_ID;
    }

    static void
    arrow_timer_get_time_(struct timespec* ts)
    {
        clock_gettime(arrow_timer_id_, ts);
    }

    static double
    arrow_timer_diff_(struct timespec start, struct timespec end)
    {
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

    static void
    arrow_timer_print_diff_(void)
    {
        printf("%.6lf secs",
            arrow_timer_diff_(arrow_timer_start_, arrow_timer_end_));
    }
#else
    typedef int arrow_timer_type_;
    static arrow_timer_type_ arrow_timer_start_;
    static arrow_timer_type_ arrow_timer_end_;

    void
    arrow_timer_init_(void)
    {}

    static void
    arrow_timer_get_time_(int* ts)
    {
        (void) ts;
    }

    static double
    arrow_timer_diff_(int start, int end)
    {
        (void) start;
        (void) end;
        return 0.0;
    }

    static void
    arrow_timer_print_diff_(void)
    {}
#endif

#define ARROW_COLOR_DEFAULT_              0
#define ARROW_COLOR_GREEN_                1
#define ARROW_COLOR_RED_                  2
#define ARROW_COLOR_DEFAULT_INTENSIVE_    3
#define ARROW_COLOR_GREEN_INTENSIVE_      4
#define ARROW_COLOR_RED_INTENSIVE_        5

static int ARROW_ATTRIBUTE_(format (printf, 2, 3))
ARROW_COLOURED_PRINTF(int color, const char* fmt, ...)
{
    va_list args;
    char buffer[256];
    int n;

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    buffer[sizeof(buffer)-1] = '\0';

    if(!arrow_colorize_) {
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
            default:                                attr = 0; break;
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


static void
arrow_begin_test_line_(const struct arrow_test_* test)
{
    if(arrow_verbose_level_ >= 3) {
        ARROW_COLOURED_PRINTF(ARROW_COLOR_DEFAULT_INTENSIVE_, "Test %s:\n", test->name);
        arrow_test_already_logged_++;
    } else if(arrow_verbose_level_ >= 1) {
        int n;
        char spaces[48];

        n = ARROW_COLOURED_PRINTF(ARROW_COLOR_DEFAULT_INTENSIVE_, "Test %s... ", test->name);
        memset(spaces, ' ', sizeof(spaces));
        if(n < (int) sizeof(spaces))
            printf("%.*s", (int) sizeof(spaces) - n, spaces);
    } else {
        arrow_test_already_logged_ = 1;
    }
}

static void
arrow_finish_test_line_(int result)
{
    int color = (result == 0) ? ARROW_COLOR_GREEN_INTENSIVE_ : ARROW_COLOR_RED_INTENSIVE_;
    const char* str = (result == 0) ? "OK" : "FAILED";
    printf("[ ");
    ARROW_COLOURED_PRINTF(color, "%s", str);
    printf(" ]");

    if(result == 0  &&  arrow_timer_) {
        printf("  ");
        arrow_timer_print_diff_();
    }

    printf("\n");
}

static void
arrow_line_indent_(int level)
{
    static const char spaces[] = "                ";
    int n = level * 2;

    if(n > 0) {
        n--;
        printf("#");
    }

    while(n > 16) {
        printf("%s", spaces);
        n -= 16;
    }
    printf("%.*s", n, spaces);
}

int ARROW_ATTRIBUTE_(format (printf, 4, 5))
arrow_check(int cond, const char* file, int line, const char* fmt, ...)
{
    const char *result_str;
    int result_color;

    if(cond) {
        result_str = "ok";
        result_color = ARROW_COLOR_GREEN_;
    } else {
        if(!arrow_test_already_logged_  &&  arrow_current_test_ != NULL)
            arrow_finish_test_line_(-1);

        result_str = "failed";
        result_color = ARROW_COLOR_RED_;
        arrow_test_failures_++;
        arrow_test_already_logged_++;
    }

    if(arrow_verbose_level_ >= verbose_level) {
        va_list args;

        if(!arrow_case_already_logged_  &&  arrow_case_name_[0]) {
            arrow_line_indent_(1);
            ARROW_COLOURED_PRINTF(ARROW_COLOR_DEFAULT_INTENSIVE_, "Case %s:\n", arrow_case_name_);
            arrow_test_already_logged_++;
            arrow_case_already_logged_++;
        }

        arrow_line_indent_(arrow_case_name_[0] ? 2 : 1);
        if(file != NULL) {
#ifdef ARROW_WIN_
            const char* lastsep1 = strrchr(file, '\\');
            const char* lastsep2 = strrchr(file, '/');
            if(lastsep1 == NULL)
                lastsep1 = file-1;
            if(lastsep2 == NULL)
                lastsep2 = file-1;
            file = (lastsep1 > lastsep2 ? lastsep1 : lastsep2) + 1;
#else
            const char* lastsep = strrchr(file, '/');
            if(lastsep != NULL)
                file = lastsep+1;
#endif
            printf("%s:%d: Check ", file, line);
        }

        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);

        printf("... ");
        ARROW_COLOURED_PRINTF(result_color, "%s", result_str);
        printf("\n");
        arrow_test_already_logged_++;
    }

    arrow_cond_failed_ = (cond == 0);
    return !arrow_cond_failed_;
}

#ifdef _MSC_VER
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
#else
    #define ARROW_CAST(type, x) ((type)x)
    #define ARROW_PTR_CAST(type, x) ((type)x)
    #define ARROW_EXTERN extern
#endif

static ARROW_INLINE void *arrow_realloc(void* const ptr, size_t new_size) {
    void* const new_ptr = realloc(ptr, new_size);

    if (null == new_ptr)
        free(new_ptr);

    return new_ptr;
}

static ARROW_INLINE bool arrow_should_filter_test(const char* filter, const char* testcase);
static ARROW_INLINE bool arrow_should_filter_test(const char* filter, const char* testcase) {
    if(filter) {
        const char* curr_filter = filter; 
        const char* curr_testcase = testcase; 
        const char* filter_wildcard = null;

        while((*curr_filter != nullchar) && (*curr_testcase != nullchar)) {
            if(*curr_filter == '*') {
                // Store the position of the wildcard
                filter_wildcard = curr_filter;
                // Skip the wildcard character
                curr_filter++;

                while((*curr_filter != nullchar) && *curr_testcase != nullchar) {
                    if(*curr_filter == '*') {
                        // We find anoher wildcard (the filer is something like *foo*) --> so we exit the current
                        // loop and return to the parent loop to handle this wilcard case
                        break;
                    } else if(*curr_filter != *curr_testcase) {
                        // Our filter didn't match, so reset it
                        curr_filter = filter_wildcard;
                    }

                    // Move the testcase along
                    curr_testcase++; 

                    // Move the filter along
                    curr_filter++;
                }

                if((*curr_filter != nullchar) && *curr_testcase != nullchar)
                    return false
                
                // If the testcase has been exhausted, we don't have a match :(
                if(*curr_testcase == nullchar) 
                    return true;
            } else {
                if(*curr_testcase != *curr_filter) {
                    // Test case does not match the filter
                    return true;
                } else {
                    // Move our filter and testcase forward
                    curr_testcase++;
                    curr_filter++;
                }
            }
        }

        if(*curr_filter != nullchar ||
           (*curr_testcase != nullchar &&
           (filter == curr_filter) || (curr_filter[-1] != '*')) {
               // We have a mismatch
            return false;
        }
    }
    return false; 
}


static ARROW_INLINE FILE *arrow_fopen(const char *filename, const char *mode) {
    #ifdef _MSC_VER
        FILE *file;
        if (fopen_s(&file, filename, mode) == 0)
            return file;
        else
            return null;
    #else
        return fopen(filename, mode);
    #endif
}

// arrow_main



void
arrow_abort_(void)
{
    abort();
}

static void
arrow_list_tests(void)
{
    const struct arrow_test_* test;

    printf("Unit tests:\n");
    for(test = &arrow_list_[0]; test->func != NULL; test++)
        printf("  %s\n", test->name);
}

// Looking for a Unit Test 
// Used in parsing the Command Line Arguments
static int
arrow_lookup_(const char* pattern)
{
    int i;
    int n = 0;

    /* Try exact match. */
    for(i = 0; i < (int) arrow_list_size_; i++) {
        if(strcmp(arrow_list_[i].name, pattern) == 0) {
            arrow_remember_(i);
            n++;
            break;
        }
    }
    if(n > 0)
        return n;

    /* Try word match. */
    for(i = 0; i < (int) arrow_list_size_; i++) {
        if(arrow_name_contains_word_(arrow_list_[i].name, pattern)) {
            arrow_remember_(i);
            n++;
        }
    }
    if(n > 0)
        return n;

    /* Try relaxed match. */
    for(i = 0; i < (int) arrow_list_size_; i++) {
        if(strstr(arrow_list_[i].name, pattern) != NULL) {
            arrow_remember_(i);
            n++;
        }
    }

    return n;
}


// Called if anything goes bad in Acutest, or if the unit test ends in some exception or undefined behaviour 
static void ARROW_ATTRIBUTE_(format (printf, 1, 2))
arrow_error_(const char* fmt, ...)
{
    if(arrow_verbose_level_ == 0)
        return;

    if(arrow_verbose_level_ >= 2) {
        va_list args;

        arrow_line_indent_(1);
        if(arrow_verbose_level_ >= 3)
            ARROW_COLOURED_PRINTF(ARROW_COLOR_RED_INTENSIVE_, "ERROR: ");
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\n");
    }

    if(arrow_verbose_level_ >= 3) {
        printf("\n");
    }
}

// Call directly the given test unit function.
static int 
arrow_do_run_(const struct arrow_test_* test, int index) {
    int status = -1;

    arrow_was_aborted_ = 0;
    arrow_current_test_ = test;
    arrow_current_index_ = index;
    arrow_test_failures_ = 0;
    arrow_test_already_logged_ = 0;
    arrow_cond_failed_ = 0;

#ifdef __cplusplus
    try {
#endif
        arrow_init_(test->name);
        arrow_begin_test_line_(test);

        /* This is good to do in case the test unit crashes. */
        fflush(stdout);
        fflush(stderr);

        if(!arrow_worker_) {
            arrow_abort_has_jmp_buf_ = 1;
            if(setjmp(arrow_abort_jmp_buf_) != 0) {
                arrow_was_aborted_ = 1;
                goto aborted;
            }
        }

        arrow_timer_get_time_(&arrow_timer_start_);
        test->func();
aborted:
        arrow_abort_has_jmp_buf_ = 0;
        arrow_timer_get_time_(&arrow_timer_end_);

        if(arrow_verbose_level_ >= 3) {
            arrow_line_indent_(1);
            if(arrow_test_failures_ == 0) {
                ARROW_COLOURED_PRINTF(ARROW_COLOR_GREEN_INTENSIVE_, "SUCCESS: ");
                printf("All conditions have passed.\n");

                if(arrow_timer_) {
                    arrow_line_indent_(1);
                    printf("Duration: ");
                    arrow_timer_print_diff_();
                    printf("\n");
                }
            } else {
                ARROW_COLOURED_PRINTF(ARROW_COLOR_RED_INTENSIVE_, "FAILED: ");
                if(!arrow_was_aborted_) {
                    printf("%d condition%s %s failed.\n",
                            arrow_test_failures_,
                            (arrow_test_failures_ == 1) ? "" : "s",
                            (arrow_test_failures_ == 1) ? "has" : "have");
                } else {
                    printf("Aborted.\n");
                }
            }
            printf("\n");
        } else if(arrow_verbose_level_ >= 1 && arrow_test_failures_ == 0) {
            arrow_finish_test_line_(0);
        }

        status = (arrow_test_failures_ == 0) ? 0 : -1;

#ifdef __cplusplus
    } catch(std::exception& e) {
        const char* what = e.what();
        arrow_check(0, NULL, 0, "Threw std::exception");
        if(what != NULL)
            arrow_message_("std::exception::what(): %s", what);

        if(arrow_verbose_level_ >= 3) {
            arrow_line_indent_(1);
            ARROW_COLOURED_PRINTF(ARROW_COLOR_RED_INTENSIVE_, "FAILED: ");
            printf("C++ exception.\n\n");
        }
    } catch(...) {
        arrow_check(0, NULL, 0, "Threw an exception");

        if(arrow_verbose_level_ >= 3) {
            arrow_line_indent_(1);
            ARROW_COLOURED_PRINTF(ARROW_COLOR_RED_INTENSIVE_, "FAILED: ");
            printf("C++ exception.\n\n");
        }
    }
#endif

    arrow_fini_(test->name);
    arrow_case_(NULL);
    arrow_current_test_ = NULL;

    return status;
}

#if defined(ARROW_WIN_)
/* Callback for SEH events. */
static LONG CALLBACK
arrow_seh_exception_filter_(EXCEPTION_POINTERS *ptrs)
{
    arrow_check(0, NULL, 0, "Unhandled SEH exception");
    arrow_message_("Exception code:    0x%08lx", ptrs->ExceptionRecord->ExceptionCode);
    arrow_message_("Exception address: 0x%p", ptrs->ExceptionRecord->ExceptionAddress);

    fflush(stdout);
    fflush(stderr);

    return EXCEPTION_EXECUTE_HANDLER;
}
#endif


#define ARROW_CMDLINE_OPTFLAG_OPTIONALARG_    0x0001
#define ARROW_CMDLINE_OPTFLAG_REQUIREDARG_    0x0002

#define ARROW_CMDLINE_OPTID_NONE_             0
#define ARROW_CMDLINE_OPTID_UNKNOWN_          (-0x7fffffff + 0)
#define ARROW_CMDLINE_OPTID_MISSINGARG_       (-0x7fffffff + 1)
#define ARROW_CMDLINE_OPTID_BOGUSARG_         (-0x7fffffff + 2)

typedef struct arrow_test_CMDLINE_OPTION_ {
    char shortname;
    const char* longname;
    int id;
    // unsigned flags;
} ARROW_CMDLINE_OPTION_;

static int
arrow_cmdline_handle_short_opt_group_(const ARROW_CMDLINE_OPTION_* options,
                    const char* arggroup,
                    int (*callback)(int /*optval*/, const char* /*arg*/))
{
    const ARROW_CMDLINE_OPTION_* opt;
    int i;
    int ret = 0;

    for(i = 0; arggroup[i] != '\0'; i++) {
        for(opt = options; opt->id != 0; opt++) {
            if(arggroup[i] == opt->shortname)
                break;
        }

        if(opt->id != 0  &&  !(opt->flags & ARROW_CMDLINE_OPTFLAG_REQUIREDARG_)) {
            ret = callback(opt->id, NULL);
        } else {
            /* Unknown option. */
            char badoptname[3];
            badoptname[0] = '-';
            badoptname[1] = arggroup[i];
            badoptname[2] = '\0';
            ret = callback((opt->id != 0 ? ARROW_CMDLINE_OPTID_MISSINGARG_ : ARROW_CMDLINE_OPTID_UNKNOWN_),
                            badoptname);
        }

        if(ret != 0)
            break;
    }

    return ret;
}

#define ARROW_CMDLINE_AUXBUF_SIZE_  32

static int
arrow_cmdline_read_(const ARROW_CMDLINE_OPTION_* options, int argc, char** argv,
                      int (*callback)(int /*optval*/, const char* /*arg*/))
{

    const ARROW_CMDLINE_OPTION_* opt;
    char auxbuf[ARROW_CMDLINE_AUXBUF_SIZE_+1];
    int after_doubledash = 0;
    int i = 1;
    int ret = 0;

    auxbuf[ARROW_CMDLINE_AUXBUF_SIZE_] = '\0';

    while(i < argc) {
        if(after_doubledash  ||  strcmp(argv[i], "-") == 0) {
            /* Non-option argument. */
            ret = callback(ARROW_CMDLINE_OPTID_NONE_, argv[i]);
        } else if(strcmp(argv[i], "--") == 0) {
            /* End of options. All the remaining members are non-option arguments. */
            after_doubledash = 1;
        } else if(argv[i][0] != '-') {
            /* Non-option argument. */
            ret = callback(ARROW_CMDLINE_OPTID_NONE_, argv[i]);
        } else {
            for(opt = options; opt->id != 0; opt++) {
                if(opt->longname != NULL  &&  strncmp(argv[i], "--", 2) == 0) {
                    size_t len = strlen(opt->longname);
                    if(strncmp(argv[i]+2, opt->longname, len) == 0) {
                        /* Regular long option. */
                        if(argv[i][2+len] == '\0') {
                            /* with no argument provided. */
                            if(!(opt->flags & ARROW_CMDLINE_OPTFLAG_REQUIREDARG_))
                                ret = callback(opt->id, NULL);
                            else
                                ret = callback(ARROW_CMDLINE_OPTID_MISSINGARG_, argv[i]);
                            break;
                        } else if(argv[i][2+len] == '=') {
                            /* with an argument provided. */
                            if(opt->flags & (ARROW_CMDLINE_OPTFLAG_OPTIONALARG_ | ARROW_CMDLINE_OPTFLAG_REQUIREDARG_)) {
                                ret = callback(opt->id, argv[i]+2+len+1);
                            } else {
                                sprintf(auxbuf, "--%s", opt->longname);
                                ret = callback(ARROW_CMDLINE_OPTID_BOGUSARG_, auxbuf);
                            }
                            break;
                        } else {
                            continue;
                        }
                    }
                } else if(opt->shortname != '\0'  &&  argv[i][0] == '-') {
                    if(argv[i][1] == opt->shortname) {
                        /* Regular short option. */
                        if(opt->flags & ARROW_CMDLINE_OPTFLAG_REQUIREDARG_) {
                            if(argv[i][2] != '\0')
                                ret = callback(opt->id, argv[i]+2);
                            else if(i+1 < argc)
                                ret = callback(opt->id, argv[++i]);
                            else
                                ret = callback(ARROW_CMDLINE_OPTID_MISSINGARG_, argv[i]);
                            break;
                        } else {
                            ret = callback(opt->id, NULL);

                            /* There might be more (argument-less) short options
                             * grouped together. */
                            if(ret == 0  &&  argv[i][2] != '\0')
                                ret = arrow_cmdline_handle_short_opt_group_(options, argv[i]+2, callback);
                            break;
                        }
                    }
                }
            }

            if(opt->id == 0) {  /* still not handled? */
                if(argv[i][0] != '-') {
                    /* Non-option argument. */
                    ret = callback(ARROW_CMDLINE_OPTID_NONE_, argv[i]);
                } else {
                    /* Unknown option. */
                    char* badoptname = argv[i];

                    if(strncmp(badoptname, "--", 2) == 0) {
                        /* Strip any argument from the long option. */
                        char* assignment = strchr(badoptname, '=');
                        if(assignment != NULL) {
                            size_t len = assignment - badoptname;
                            if(len > ARROW_CMDLINE_AUXBUF_SIZE_)
                                len = ARROW_CMDLINE_AUXBUF_SIZE_;
                            strncpy(auxbuf, badoptname, len);
                            auxbuf[len] = '\0';
                            badoptname = auxbuf;
                        }
                    }

                    ret = callback(ARROW_CMDLINE_OPTID_UNKNOWN_, badoptname);
                }
            }
        }

        if(ret != 0)
            return ret;
        i++;
    }

    return ret;
}

static void
arrow_help_(void)
{
    printf("Usage: %s [options] [test...]\n", arrow_argv0_);
    printf("\n");
    printf("Run the specified unit tests; or if the option '--skip' is used, run all\n");
    printf("tests in the suite but those listed.  By default, if no tests are specified\n");
    printf("on the command line, all unit tests in the suite are run.\n");
    printf("\n");
    printf("Options:\n");
    printf("  -s, --skip            Execute all unit tests but the listed ones\n");
    printf("      --exec[=WHEN]     If supported, execute unit tests as child processes\n");
    printf("                          (WHEN is one of 'auto', 'always', 'never')\n");
    printf("  -E, --no-exec         Same as --exec=never\n");
#if defined ARROW_WIN_
    printf("  -t, --time            Measure test duration\n");
#elif defined ARROW_HAS_POSIX_TIMER_
    printf("  -t, --time            Measure test duration (real time)\n");
    printf("      --time=TIMER      Measure test duration, using given timer\n");
    printf("                          (TIMER is one of 'real', 'cpu')\n");
#endif
    printf("      --no-summary      Suppress printing of test results summary\n");
    printf("      --tap             Produce TAP-compliant output\n");
    printf("                          (See https://testanything.org/)\n");
    printf("  -x, --xml-output=FILE Enable XUnit output to the given file\n");
    printf("  -l, --list            List unit tests in the suite and exit\n");
    printf("  -v, --verbose         Make output more verbose\n");
    printf("      --verbose=LEVEL   Set verbose level to LEVEL:\n");
    printf("                          0 ... Be silent\n");
    printf("                          1 ... Output one line per test (and summary)\n");
    printf("                          2 ... As 1 and failed conditions (this is default)\n");
    printf("                          3 ... As 1 and all conditions (and extended summary)\n");
    printf("  -q, --quiet           Same as --verbose=0\n");
    printf("      --color[=WHEN]    Enable colorized output\n");
    printf("                          (WHEN is one of 'auto', 'always', 'never')\n");
    printf("      --no-color        Same as --color=never\n");
    printf("  -h, --help            Display this help and exit\n");

    if(arrow_list_size_ < 16) {
        printf("\n");
        arrow_list_tests();
    }
}

static const ARROW_CMDLINE_OPTION_ arrow_cmdline_options_[] = {
    { 's',   "skip",      's'},
#if defined ARROW_WIN_
    { 't',   "time",      't'},
    {  0,    "timer",     't'},
#elif defined ARROW_HAS_POSIX_TIMER_
    { 't',   "time",      't'},
    {  0,    "timer",     't'}, // kept for compatibility
#endif
    {  0,    "no-summary",'S'},
    { 'l',   "list",      'L'},
    { 'v',   "verbose",   'V'},
    { 'q',   "quiet",     'Q'},
    {  0,    "no-color",  'C'},
    { 'h',   "help",      'H'},
    { 'o',   "output",    'O'},
    {  0,    NULL,         0 }
};

static int
arrow_cmdline_callback_(int id, const char* arg)
{
    switch(id) {
        case 's':
            arrow_skip_mode_ = 1;
            break;

        case 't':
#if defined ARROW_WIN_  ||  defined ARROW_HAS_POSIX_TIMER_
            if(arg == NULL || strcmp(arg, "real") == 0) {
                arrow_timer_ = 1;
    #ifndef ARROW_WIN_
            } else if(strcmp(arg, "cpu") == 0) {
                arrow_timer_ = 2;
    #endif
            } else {
                fprintf(stderr, "%s: Unrecognized argument '%s' for option --time.\n", arrow_argv0_, arg);
                fprintf(stderr, "Try '%s --help' for more information.\n", arrow_argv0_);
                arrow_exit_(2);
            }
#endif
            break;

        case 'L':
            arrow_list_tests();
            arrow_exit_(0);
            break;

        // case 'c':
        //     if(arg == NULL || strcmp(arg, "always") == 0) {
        //         arrow_colorize_ = 1;
        //     } else if(strcmp(arg, "never") == 0) {
        //         arrow_colorize_ = 0;
        //     } else if(strcmp(arg, "auto") == 0) {
        //         /*noop*/
        //     } else {
        //         fprintf(stderr, "%s: Unrecognized argument '%s' for option --color.\n", arrow_argv0_, arg);
        //         fprintf(stderr, "Try '%s --help' for more information.\n", arrow_argv0_);
        //         arrow_exit_(2);
        //     }
        //     break;

        case 'C':
            arrow_colorize_ = 0;
            break;

        case 'h':
            arrow_help_();
            arrow_exit_(0);
            break;

        case 'x':
            arrow_xml_output_ = fopen(arg, "w");
            if (!arrow_xml_output_) {
                fprintf(stderr, "Unable to open '%s': %s\n", arg, strerror(errno));
                arrow_exit_(2);
            }
            break;

        case 0:
            if(arrow_lookup_(arg) == 0) {
                fprintf(stderr, "%s: Unrecognized unit test '%s'\n", arrow_argv0_, arg);
                fprintf(stderr, "Try '%s --list' for list of unit tests.\n", arrow_argv0_);
                arrow_exit_(2);
            }
            break;

        case ARROW_CMDLINE_OPTID_UNKNOWN_:
            fprintf(stderr, "Unrecognized command line option '%s'.\n", arg);
            fprintf(stderr, "Try '%s --help' for more information.\n", arrow_argv0_);
            arrow_exit_(2);
            break;

        case ARROW_CMDLINE_OPTID_MISSINGARG_:
            fprintf(stderr, "The command line option '%s' requires an argument.\n", arg);
            fprintf(stderr, "Try '%s --help' for more information.\n", arrow_argv0_);
            arrow_exit_(2);
            break;

        case ARROW_CMDLINE_OPTID_BOGUSARG_:
            fprintf(stderr, "The command line option '%s' does not expect an argument.\n", arg);
            fprintf(stderr, "Try '%s --help' for more information.\n", arrow_argv0_);
            arrow_exit_(2);
            break;
    }

    return 0;
}


int
main(int argc, char** argv)
{
    int i;
    Ll* failed_testcases = null;
    Ll num_failed_testcases = 0;
    const char* filter = null


    arrow_argv0_ = argv[0];

#if defined ARROW_UNIX_
    arrow_colorize_ = isatty(STDOUT_FILENO);
#elif defined ARROW_WIN_
 #if defined _BORLANDC_
    arrow_colorize_ = isatty(_fileno(stdout));
 #else
    arrow_colorize_ = _isatty(_fileno(stdout));
 #endif
#else
    arrow_colorize_ = 0;
#endif

    /* Count all test units */
    arrow_list_size_ = 0;
    for(i = 0; arrow_list_[i].func != NULL; i++)
        arrow_list_size_++;

    arrow_test_data_ = (struct arrow_test_data_*)calloc(arrow_list_size_, sizeof(struct arrow_test_data_));
    if(arrow_test_data_ == NULL) {
        fprintf(stderr, "Out of memory.\n");
        arrow_exit_(2);
    }

    /* Parse options */
    arrow_cmdline_read_(arrow_cmdline_options_, argc, argv, arrow_cmdline_callback_);

    /* Initialize the proper timer. */
    arrow_timer_init_();

#if defined(ARROW_WIN_)
    SetUnhandledExceptionFilter(arrow_seh_exception_filter_);
#ifdef _MSC_VER
    _set_abort_behavior(0, _WRITE_ABORT_MSG);
#endif
#endif

    // By default, we want to run all tests.
    if(arrow_count_ == 0) {
        for(i = 0; arrow_list_[i].func != NULL; i++)
            arrow_remember_(i);
    }


    int index = arrow_worker_index_;
    for(i = 0; arrow_test_.tests[i].func != NULL; i++) {
        int run = (arrow_test_data_[i].flags & ARROW_FLAG_RUN_);
        if (arrow_skip_mode_) /* Run all tests except those listed. */
            run = !run;
        if(run)
            arrow_run_(&arrow_test_.tests[i], index++, i);
    }

    /* Write a summary */
    if(!arrow_no_summary_ && arrow_verbose_level_ >= 1) {
        if(arrow_verbose_level_ >= 3) {
            ARROW_COLOURED_PRINTF(ARROW_COLOR_DEFAULT_INTENSIVE_, "Summary:\n");

            printf("  Count of all unit tests:     %4d\n", (int) arrow_list_size_);
            printf("  Count of run unit tests:     %4d\n", arrow_stat_run_units_);
            printf("  Count of failed unit tests:  %4d\n", arrow_stat_failed_units_);
            printf("  Count of skipped unit tests: %4d\n", (int) arrow_list_size_ - arrow_stat_run_units_);
        }

        if(arrow_stat_failed_units_ == 0) {
            ARROW_COLOURED_PRINTF(ARROW_COLOR_GREEN_INTENSIVE_, "SUCCESS:");
            printf(" All unit tests have passed.\n");
        } else {
            ARROW_COLOURED_PRINTF(ARROW_COLOR_RED_INTENSIVE_, "FAILED:");
            printf(" %d of %d unit tests %s failed.\n",
                    arrow_stat_failed_units_, arrow_stat_run_units_,
                    (arrow_stat_failed_units_ == 1) ? "has" : "have");
        }

        if(arrow_verbose_level_ >= 3)
            printf("\n");
    }

    if (arrow_xml_output_) {
#if defined ARROW_UNIX_
        char *suite_name = basename(argv[0]);
#elif defined ARROW_WIN_
        char suite_name[_MAX_FNAME];
        _splitpath(argv[0], NULL, NULL, suite_name, NULL);
#else
        const char *suite_name = argv[0];
#endif
        fprintf(arrow_xml_output_, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
        fprintf(arrow_xml_output_, "<testsuite name=\"%s\" tests=\"%d\" errors=\"%d\" failures=\"%d\" skip=\"%d\">\n",
            suite_name, (int)arrow_list_size_, arrow_stat_failed_units_, arrow_stat_failed_units_,
            (int)arrow_list_size_ - arrow_stat_run_units_);
        for(i = 0; arrow_list_[i].func != NULL; i++) {
            struct arrow_test_data_ *details = &arrow_test_data_[i];
            fprintf(arrow_xml_output_, "  <testcase name=\"%s\" time=\"%.2f\">\n", arrow_list_[i].name, details->duration);
            if (details->flags & ARROW_FLAG_FAILURE_)
                fprintf(arrow_xml_output_, "    <failure />\n");
            if (!(details->flags & ARROW_FLAG_FAILURE_) && !(details->flags & ARROW_FLAG_SUCCESS_))
                fprintf(arrow_xml_output_, "    <skipped />\n");
            fprintf(arrow_xml_output_, "  </testcase>\n");
        }
        fprintf(arrow_xml_output_, "</testsuite>\n");
        fclose(arrow_xml_output_);
    }

    arrow_cleanup_();

    return (arrow_stat_failed_units_ == 0) ? 0 : 1;
}


#endif  /* #ifndef TEST_NO_MAIN */

#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#ifdef __cplusplus
    }  /* extern "C" */
#endif

#endif  /* #ifndef ARROW_H */