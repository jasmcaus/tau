/*
 * Acutest -- Another C/C++ Unit Test facility
 * <https://github.com/mity/acutest>
 *
 * Copyright 2013-2020 Martin Mitas
 * Copyright 2019 Garrett D'Amore
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef ACUTEST_H
#define ACUTEST_H


/************************
 *** Public interface ***
 ************************/

/* By default, "acutest.h" provides the main program entry point (function
 * main()). However, if the test suite is composed of multiple source files
 * which include "acutest.h", then this causes a problem of multiple main()
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
#define TEST_LIST               const struct acutest_test_ acutest_list_[]


/* Macros for testing whether an unit test succeeds or fails. These macros
 * can be used arbitrarily in functions implementing the unit tests.
 *
 * If any condition fails throughout execution of a test, the test fails.
 *
 * TEST_CHECK takes only one argument (the condition), TEST_CHECK_ allows
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
#define TEST_CHECK_(cond,...)   acutest_check_((cond), __FILE__, __LINE__, __VA_ARGS__)
#define TEST_CHECK(cond)        acutest_check_((cond), __FILE__, __LINE__, "%s", #cond)


/* These macros are the same as TEST_CHECK_ and TEST_CHECK except that if the
 * condition fails, the currently executed unit test is immediately aborted.
 *
 * That is done either by calling abort() if the unit test is executed as a
 * child process; or via longjmp() if the unit test is executed within the
 * main Acutest process.
 *
 * As a side effect of such abortion, your unit tests may cause memory leaks,
 * unflushed file descriptors, and other phenomena caused by the abortion.
 *
 * Therefore you should not use these as a general replacement for TEST_CHECK.
 * Use it with some caution, especially if your test causes some other side
 * effects to the outside world (e.g. communicating with some server, inserting
 * into a database etc.).
 */
#define TEST_ASSERT_(cond,...)                                                 \
    do {                                                                       \
        if(!acutest_check_((cond), __FILE__, __LINE__, __VA_ARGS__))           \
            acutest_abort_();                                                  \
    } while(0)
#define TEST_ASSERT(cond)                                                      \
    do {                                                                       \
        if(!acutest_check_((cond), __FILE__, __LINE__, "%s", #cond))           \
            acutest_abort_();                                                  \
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
#define TEST_EXCEPTION(code, exctype)                                          \
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
        acutest_check_(exc_ok_, __FILE__, __LINE__, #code " throws " #exctype);\
        if(msg_ != NULL)                                                       \
            acutest_message_("%s", msg_);                                      \
    } while(0)
#define TEST_EXCEPTION_(code, exctype, ...)                                    \
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
        acutest_check_(exc_ok_, __FILE__, __LINE__, __VA_ARGS__);              \
        if(msg_ != NULL)                                                       \
            acutest_message_("%s", msg_);                                      \
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
#define TEST_CASE_(...)         acutest_case_(__VA_ARGS__)
#define TEST_CASE(name)         acutest_case_("%s", name)


/* Maximal output per TEST_CASE call. Longer messages are cut.
 * You may define another limit prior including "acutest.h"
 */
#ifndef TEST_CASE_MAXSIZE
    #define TEST_CASE_MAXSIZE    64
#endif


/**********************
 *** Implementation ***
 **********************/

/* The unit test files should not rely on anything below. */

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    #define ACUTEST_WIN_        1
    #include <windows.h>
    #include <io.h>
#endif


/* Enable the use of the non-standard keyword __attribute__ to silence warnings under some compilers */
#if defined(__GNUC__) || defined(__clang__)
    #define ACUTEST_ATTRIBUTE_(attr)    __attribute__((attr))
#else
    #define ACUTEST_ATTRIBUTE_(attr)
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


struct acutest_test_ {
    const char* name;
    void (*func)(void);
};

struct acutest_test_data_ {
    unsigned char flags;
    double duration;
};

enum {
    ACUTEST_FLAG_RUN_ = 1 << 0,
    ACUTEST_FLAG_SUCCESS_ = 1 << 1,
    ACUTEST_FLAG_FAILURE_ = 1 << 2,
};

extern const struct acutest_test_ acutest_list_[];

int acutest_check_(int cond, const char* file, int line, const char* fmt, ...);
void acutest_case_(const char* fmt, ...);
void acutest_abort_(void) ACUTEST_ATTRIBUTE_(noreturn);


#ifndef TEST_NO_MAIN

static char* acutest_argv0_ = NULL;
static size_t acutest_list_size_ = 0;
static struct acutest_test_data_* acutest_test_data_ = NULL;
static size_t acutest_count_ = 0;
static int acutest_cond_failed_ = 0;
static int acutest_was_aborted_ = 0;

static int acutest_stat_failed_units_ = 0;
static int acutest_stat_run_units_ = 0;

static const struct acutest_test_* acutest_current_test_ = NULL;
static int acutest_current_index_ = 0;
static char acutest_case_name_[TEST_CASE_MAXSIZE] = "";
static int acutest_test_already_logged_ = 0;
static int acutest_case_already_logged_ = 0;
static int acutest_verbose_level_ = 2;
static int acutest_test_failures_ = 0;
static int acutest_colorize_ = 0;
static int acutest_timer_ = 1;


static void
acutest_cleanup_(void)
{
    free((void*) acutest_test_data_);
}

static void ACUTEST_ATTRIBUTE_(noreturn)
acutest_exit_(int exit_code)
{
    acutest_cleanup_();
    exit(exit_code);
}

typedef LARGE_INTEGER acutest_timer_type_;
static LARGE_INTEGER acutest_timer_freq_;
static acutest_timer_type_ acutest_timer_start_;
static acutest_timer_type_ acutest_timer_end_;

static void
acutest_timer_init_(void)
{
    QueryPerformanceFrequency(&acutest_timer_freq_);
}

static void
acutest_timer_get_time_(LARGE_INTEGER* ts)
{
    QueryPerformanceCounter(ts);
}

static double
acutest_timer_diff_(LARGE_INTEGER start, LARGE_INTEGER end)
{
    double duration = (double)(end.QuadPart - start.QuadPart);
    duration /= (double)acutest_timer_freq_.QuadPart;
    return duration;
}

static void
acutest_timer_print_diff_(void)
{
    printf("%.6lf sec", acutest_timer_diff_(acutest_timer_start_, acutest_timer_end_));
}

#define ACUTEST_COLOR_DEFAULT_              0
#define ACUTEST_COLOR_GREEN_                1
#define ACUTEST_COLOR_RED_                  2
#define ACUTEST_COLOR_DEFAULT_INTENSIVE_    3
#define ACUTEST_COLOR_GREEN_INTENSIVE_      4
#define ACUTEST_COLOR_RED_INTENSIVE_        5

static int ACUTEST_ATTRIBUTE_(format (printf, 2, 3))
acutest_colored_printf_(int color, const char* fmt, ...)
{
    va_list args;
    char buffer[256];
    int n;

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    buffer[sizeof(buffer)-1] = '\0';

    if(!acutest_colorize_) {
        return printf("%s", buffer);
    }

#if defined ACUTEST_UNIX_
    {
        const char* col_str;
        switch(color) {
            case ACUTEST_COLOR_GREEN_:              col_str = "\033[0;32m"; break;
            case ACUTEST_COLOR_RED_:                col_str = "\033[0;31m"; break;
            case ACUTEST_COLOR_GREEN_INTENSIVE_:    col_str = "\033[1;32m"; break;
            case ACUTEST_COLOR_RED_INTENSIVE_:      col_str = "\033[1;31m"; break;
            case ACUTEST_COLOR_DEFAULT_INTENSIVE_:  col_str = "\033[1m"; break;
            default:                                col_str = "\033[0m"; break;
        }
        printf("%s", col_str);
        n = printf("%s", buffer);
        printf("\033[0m");
        return n;
    }
#elif defined ACUTEST_WIN_
    {
        HANDLE h;
        CONSOLE_SCREEN_BUFFER_INFO info;
        WORD attr;

        h = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleScreenBufferInfo(h, &info);

        switch(color) {
            case ACUTEST_COLOR_GREEN_:              attr = FOREGROUND_GREEN; break;
            case ACUTEST_COLOR_RED_:                attr = FOREGROUND_RED; break;
            case ACUTEST_COLOR_GREEN_INTENSIVE_:    attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
            case ACUTEST_COLOR_RED_INTENSIVE_:      attr = FOREGROUND_RED | FOREGROUND_INTENSITY; break;
            case ACUTEST_COLOR_DEFAULT_INTENSIVE_:  attr = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY; break;
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
acutest_begin_test_line_(const struct acutest_test_* test) {
    if(acutest_verbose_level_ >= 3) {
        acutest_colored_printf_(ACUTEST_COLOR_DEFAULT_INTENSIVE_, "Test %s:\n", test->name);
        acutest_test_already_logged_++;
    } 
}

static void
acutest_finish_test_line_(int result)
{
    int color = (result == 0) ? ACUTEST_COLOR_GREEN_INTENSIVE_ : ACUTEST_COLOR_RED_INTENSIVE_;
    const char* str = (result == 0) ? "OK" : "FAILED";
    printf("[ ");
    acutest_colored_printf_(color, "%s", str);
    printf(" ]");

    if(result == 0  &&  acutest_timer_) {
        printf("  ");
        acutest_timer_print_diff_();
    }

    printf("\n");
}

static void
acutest_line_indent_(int level)
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

int ACUTEST_ATTRIBUTE_(format (printf, 4, 5))
acutest_check_(int cond, const char* file, int line, const char* fmt, ...)
{
    const char *result_str;
    int result_color;
    int verbose_level;

    if(cond) {
        result_str = "ok";
        result_color = ACUTEST_COLOR_GREEN_;
        verbose_level = 3;
    } else {
        if(!acutest_test_already_logged_  &&  acutest_current_test_ != NULL)
            acutest_finish_test_line_(-1);

        result_str = "failed";
        result_color = ACUTEST_COLOR_RED_;
        verbose_level = 2;
        acutest_test_failures_++;
        acutest_test_already_logged_++;
    }

    if(acutest_verbose_level_ >= verbose_level) {
        va_list args;

        if(!acutest_case_already_logged_  &&  acutest_case_name_[0]) {
            acutest_line_indent_(1);
            acutest_colored_printf_(ACUTEST_COLOR_DEFAULT_INTENSIVE_, "Case %s:\n", acutest_case_name_);
            acutest_test_already_logged_++;
            acutest_case_already_logged_++;
        }

        acutest_line_indent_(acutest_case_name_[0] ? 2 : 1);
        if(file != NULL) {
#ifdef ACUTEST_WIN_
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
        acutest_colored_printf_(result_color, "%s", result_str);
        acutest_test_already_logged_++;
    }

    acutest_cond_failed_ = (cond == 0);
    return !acutest_cond_failed_;
}

void ACUTEST_ATTRIBUTE_(format (printf, 1, 2))
acutest_case_(const char* fmt, ...)
{
    va_list args;

    if(acutest_verbose_level_ < 2)
        return;

    if(acutest_case_name_[0]) {
        acutest_case_already_logged_ = 0;
        acutest_case_name_[0] = '\0';
    }

    if(fmt == NULL)
        return;

    va_start(args, fmt);
    vsnprintf(acutest_case_name_, sizeof(acutest_case_name_) - 1, fmt, args);
    va_end(args);
    acutest_case_name_[sizeof(acutest_case_name_) - 1] = '\0';

    if(acutest_verbose_level_ >= 3) {
        acutest_line_indent_(1);
        acutest_colored_printf_(ACUTEST_COLOR_DEFAULT_INTENSIVE_, "Case %s:\n", acutest_case_name_);
        acutest_test_already_logged_++;
        acutest_case_already_logged_++;
    }
}

/* This is called just before each test */
static void
acutest_init_(const char *test_name)
{
#ifdef TEST_INIT
    TEST_INIT
    ; /* Allow for a single unterminated function call */
#endif

    /* Suppress any warnings about unused variable. */
    (void) test_name;
}

/* This is called after each test */
static void
acutest_fini_(const char *test_name)
{
#ifdef TEST_FINI
    TEST_FINI
    ; /* Allow for a single unterminated function call */
#endif

    /* Suppress any warnings about unused variable. */
    (void) test_name;
}

void
acutest_abort_(void) {
    if(acutest_current_test_ != NULL) {
        acutest_fini_(acutest_current_test_->name);
        abort();
    }
}

static void
acutest_list_names_(void)
{
    const struct acutest_test_* test;

    printf("Unit tests:\n");
    for(test = &acutest_list_[0]; test->func != NULL; test++)
        printf("  %s\n", test->name);
}

static void
acutest_remember_(int i)
{
    if(acutest_test_data_[i].flags & ACUTEST_FLAG_RUN_)
        return;

    acutest_test_data_[i].flags |= ACUTEST_FLAG_RUN_;
    acutest_count_++;
}

static void
acutest_set_success_(int i, int success)
{
    acutest_test_data_[i].flags |= success ? ACUTEST_FLAG_SUCCESS_ : ACUTEST_FLAG_FAILURE_;
}

static void
acutest_set_duration_(int i, double duration)
{
    acutest_test_data_[i].duration = duration;
}


/* Call directly the given test unit function. */
static int
acutest_do_run_(const struct acutest_test_* test, int index)
{
    int status = -1;

    acutest_was_aborted_ = 0;
    acutest_current_test_ = test;
    acutest_current_index_ = index;
    acutest_test_failures_ = 0;
    acutest_test_already_logged_ = 0;
    acutest_cond_failed_ = 0;

#ifdef __cplusplus
    try {
#endif
        acutest_init_(test->name);
        acutest_begin_test_line_(test);

        /* This is good to do in case the test unit crashes. */
        fflush(stdout);
        fflush(stderr);

        acutest_timer_get_time_(&acutest_timer_start_);
        test->func();
aborted:
        acutest_timer_get_time_(&acutest_timer_end_);

        if(acutest_verbose_level_ >= 3) {
            acutest_line_indent_(1);
            if(acutest_test_failures_ == 0) {
                acutest_colored_printf_(ACUTEST_COLOR_GREEN_INTENSIVE_, "SUCCESS: ");
                printf("All conditions have passed.\n");

                if(acutest_timer_) {
                    acutest_line_indent_(1);
                    printf("Duration: ");
                    acutest_timer_print_diff_();
                    printf("\n");
                }
            } else {
                acutest_colored_printf_(ACUTEST_COLOR_RED_INTENSIVE_, "FAILED: ");
                if(!acutest_was_aborted_) {
                    printf("%d condition%s %s failed.\n",
                            acutest_test_failures_,
                            (acutest_test_failures_ == 1) ? "" : "s",
                            (acutest_test_failures_ == 1) ? "has" : "have");
                } else {
                    printf("Aborted.\n");
                }
            }
            printf("\n");
        } else if(acutest_verbose_level_ >= 1 && acutest_test_failures_ == 0) {
            acutest_finish_test_line_(0);
        }

        status = (acutest_test_failures_ == 0) ? 0 : -1;

#ifdef __cplusplus
    } catch(std::exception& e) {
        const char* what = e.what();
        acutest_check_(0, NULL, 0, "Threw std::exception");
        if(what != NULL)
            acutest_message_("std::exception::what(): %s", what);

        if(acutest_verbose_level_ >= 3) {
            acutest_line_indent_(1);
            acutest_colored_printf_(ACUTEST_COLOR_RED_INTENSIVE_, "FAILED: ");
            printf("C++ exception.\n\n");
        }
    } catch(...) {
        acutest_check_(0, NULL, 0, "Threw an exception");

        if(acutest_verbose_level_ >= 3) {
            acutest_line_indent_(1);
            acutest_colored_printf_(ACUTEST_COLOR_RED_INTENSIVE_, "FAILED: ");
            printf("C++ exception.\n\n");
        }
    }
#endif

    acutest_fini_(test->name);
    acutest_case_(NULL);
    acutest_current_test_ = NULL;

    return status;
}

/* Trigger the unit test. If possible (and not suppressed) it starts a child
 * process who calls acutest_do_run_(), otherwise it calls acutest_do_run_()
 * directly. */
static void
acutest_run_(const struct acutest_test_* test, int index, int master_index)
{
    int failed = 1;
    acutest_timer_type_ start, end;

    acutest_current_test_ = test;
    acutest_test_already_logged_ = 0;
    acutest_timer_get_time_(&start);

    /* Child processes suppressed through --no-exec. */
    failed = (acutest_do_run_(test, index) != 0);

    acutest_timer_get_time_(&end);

    acutest_current_test_ = NULL;

    acutest_stat_run_units_++;
    if(failed)
        acutest_stat_failed_units_++;

    acutest_set_success_(master_index, !failed);
    acutest_set_duration_(master_index, acutest_timer_diff_(start, end));
}


int main(int argc, char** argv) {
    int i;
    acutest_argv0_ = argv[0];

#if defined ACUTEST_UNIX_
    acutest_colorize_ = isatty(STDOUT_FILENO);
#elif defined ACUTEST_WIN_
 #if defined _BORLANDC_
    acutest_colorize_ = isatty(_fileno(stdout));
 #else
    acutest_colorize_ = _isatty(_fileno(stdout));
 #endif
#else
    acutest_colorize_ = 0;
#endif

    /* Count all test units */
    acutest_list_size_ = 0;
    for(i = 0; acutest_list_[i].func != NULL; i++)
        acutest_list_size_++;

    acutest_test_data_ = (struct acutest_test_data_*)calloc(acutest_list_size_, sizeof(struct acutest_test_data_));
    if(acutest_test_data_ == NULL) {
        fprintf(stderr, "Out of memory.\n");
        acutest_exit_(2);
    }


    /* Initialize the proper timer. */
    acutest_timer_init_();

    /* By default, we want to run all tests. */
    if(acutest_count_ == 0) {
        for(i = 0; acutest_list_[i].func != NULL; i++)
            acutest_remember_(i);
    }


    int index = 0;
    for(i = 0; acutest_list_[i].func != NULL; i++) {
        int run = (acutest_test_data_[i].flags & ACUTEST_FLAG_RUN_);
        if(run)
            acutest_run_(&acutest_list_[i], index++, i);
    }

    /* Write a summary */
    if(acutest_verbose_level_ >= 1) {
        if(acutest_verbose_level_ >= 3) {
            acutest_colored_printf_(ACUTEST_COLOR_DEFAULT_INTENSIVE_, "Summary:\n");

            printf("  Count of all unit tests:     %4d\n", (int) acutest_list_size_);
            printf("  Count of run unit tests:     %4d\n", acutest_stat_run_units_);
            printf("  Count of failed unit tests:  %4d\n", acutest_stat_failed_units_);
            printf("  Count of skipped unit tests: %4d\n", (int) acutest_list_size_ - acutest_stat_run_units_);
        }

        if(acutest_stat_failed_units_ == 0) {
            acutest_colored_printf_(ACUTEST_COLOR_GREEN_INTENSIVE_, "SUCCESS:");
            printf(" All unit tests have passed.\n");
        } else {
            acutest_colored_printf_(ACUTEST_COLOR_RED_INTENSIVE_, "FAILED:");
            printf(" %d of %d unit tests %s failed.\n",
                    acutest_stat_failed_units_, acutest_stat_run_units_,
                    (acutest_stat_failed_units_ == 1) ? "has" : "have");
        }

        if(acutest_verbose_level_ >= 3)
            printf("\n");
    }

    acutest_cleanup_();

    return (acutest_stat_failed_units_ == 0) ? 0 : 1;
}


#endif  /* #ifndef TEST_NO_MAIN */

#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#ifdef __cplusplus
    }  /* extern "C" */
#endif

#endif  /* #ifndef ACUTEST_H */