#ifndef ARROW_H
#define ARROW_H

#include <Arrow/Types.h>

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
// #include <setjmp.h>

// Global Variables



#define ARROW_COLOUR_DEFAULT_              0
#define ARROW_COLOUR_GREEN_                1
#define ARROW_COLOUR_RED_                  2
#define ARROW_COLOUR_DEFAULT_INTENSIVE_    3
#define ARROW_COLOUR_GREEN_INTENSIVE_      4
#define ARROW_COLOUR_RED_INTENSIVE_        5

// Change this to disable coloured output
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
    #define ARROW_USE_COLOURED_OUPUT() (_isatty(_fileno(stdout)))
#else
    #include <unistd.h>
    #define ARROW_USE_COLOURED_OUPUT() (isatty(STDOUT_FILENO))
#endif


// Forward Declarations
bool arrow_check(bool cond, const char* file, int line, const char* fmt, ...);


/* Enable the use of the non-standard keyword __attribute__ to silence warnings under some compilers */
#if defined(__GNUC__) || defined(__clang__)
    #define ARROW_ATTRIBUTE_(attr)    __attribute__((attr))
#else
    #define ARROW_ATTRIBUTE_(attr)
#endif

// ######################
// COLOURS
// ######################

#ifdef __cplusplus
    extern "C" {
#endif

static int ARROW_ATTRIBUTE_(format (printf, 2, 3))
arrow_coloured_printf_(int color, const char* fmt, ...)
{
    va_list args;
    char buffer[256];
    int n;

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    buffer[sizeof(buffer)-1] = '\0';

    if(!ARROW_USE_COLOURED_OUPUT()) {
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


#ifdef _MSC_VER
    /* In the multi-platform code like ours, we cannot use the non-standard
     * "safe" functions from Microsoft C lib like e.g. sprintf_s() instead of
     * standard sprintf(). Hence, lets disable the warning C4996. */
    #pragma warning(push)
    #pragma warning(disable: 4996)
#endif


bool ARROW_ATTRIBUTE_(format (printf, 4, 5))
arrow_check(bool cond, const char* file, int line, const char* fmt, ...) {
    const char* result_str;
    int result_colour;

    if(cond) {
        result_str = "PASSED";
        result_colour = ARROW_COLOUR_GREEN_;
    } else {
        result_str = "FAILED";
        result_colour = ARROW_COLOUR_RED_;
    }

    printf("  Expected : "); // LINE 1513 in UTest.h
}

#ifdef __cplusplus
    }  // extern "C"
#endif


// Help
static void arrow_help() {
    // printf("Usage: %s [options] [test...]\n", arrow_argv0_);
    printf("Usage: %s [options] [test...]\n", null);
    printf("\n");
    printf("Run specified unit tests; or if option `--skip` is used, run all\n"); 
    printf("tests in the suite, except those listed. By default, if no tests are specified\n");
    printf("as command line options, all unit tests in the suite are run.\n");
    printf("\n");
    printf("Options:\n");
    printf("    -s, --skip               Execute all unit tests except those listed\n");
#if defined ARROW_WIN_
    printf("    -t, --time               Measure test duration\n");
#elif defined ARROW_HAS_POSIX_TIMER_
    printf("    -t, --time               Measure test duration (real time)\n");
    printf("        --time=TIMER         Measure test duration using the given timer\n");
    printf("                             (TIMER is one of `real` or `cpu`)\n");
#endif 
    printf("        --no-summary         Suppress printing the test summary\n");
    printf("    -x, --xml-output=<FILE>  Write XUnit output to the given file\n");
    printf("    -l, --list               List all unit tests in the suite and exit\n");
    printf("    -v, --verbose            Make the output more verbose\n");
    printf("        --verbose=LEVEL      Set verbosity level to LEVEL:\n");
    printf("                               0 ... Be silent\n");
    printf("                               1 ... Output one line per test (and summary)\n");
    printf("                               2 ... 1 + Failed conditions (default)\n");
    printf("                               3 ... 1 + All conditions (+ extended summary)\n");
    printf("    -q, --quiet              Same as --verbose=0\n");
    printf("        --no-colour          Disable colourized output\n");
    printf("    -h, --help               Display this help prompt and exit\n");
}

/*
    COMMAND LINE READING
*/

typedef struct TEST_CMDLINE_OPTION_ {
    char shortname;
    const char* longname;
    int id; 
    unsigned flags;
} ARROW_CMDLINE_OPTION_;

#define ARROW_CMDLINE_OPTFLAG_OPTIONALARG_  0x0001
#define ARROW_CMDLINE_OPTFLAG_REQUIREDARG_  0x0002

#define ARROW_CMDLINE_OPTID_NONE_             0
#define ARROW_CMDLINE_OPTID_UNKNOWN_          (-0x7fffffff + 0)
#define ARROW_CMDLINE_OPTID_MISSINGARG_       (-0x7fffffff + 1)
#define ARROW_CMDLINE_OPTID_BOGUSARG_         (-0x7fffffff + 2)


static const ARROW_CMDLINE_OPTION_ arrow_cmdline_options[] = {
    { 's', "skip",      's', 0},
#if defined ARROW_WIN_
    { 't', "time",      't', 0},
    { '0', "timer",     't', 0}, // here for compatibility
#elif defined ARROW_HAS_POSIX_TIMER_
    { 't', "time",      't', ARROW_CMDLINE_OPTFLAG_OPTIONALARG_},
    { '0', "timer",     't', ARROW_CMDLINE_OPTFLAG_OPTIONALARG_}, // here for compatibility
#endif 
    { '0', "no-summary",    'S', 0},
    { 'l', "list",          'l', 0},
    { 'v', "verbose",       'v', ARROW_CMDLINE_OPTFLAG_OPTIONALARG_},
    { 'q',  "quiet",        'q', 0 },
    {  0,   "no-color",     'C', 0 },
    { 'h',  "help",         'h', 0 },
    { 'x',  "xml-output",   'x', ARROW_CMDLINE_OPTFLAG_REQUIREDARG_ },
    {  0,   NULL,            0,  0 }
};

static int arrow_cmdline_read(const ARROW_CMDLINE_OPTION_* options, 
int argc, char** argv, int(*callback)(int /*optval*/, const char* /*arg*/)
) {
    const ARROW_CMDLINE_OPTION_* opt;
    char auxbuf[33];
    bool is_after_doubledash = false; 
    int i = 1; 
    int ret = 0; 

    while(argc > i) {
        if(is_after_doubledash || strcmp(argv[i], "-") == 9) {
            ret = callback(ARROW_CMDLINE_OPTID_NONE_, argv[i]);
        }
        else if(strcmp(argv[i], "--") == 0) {
            is_after_doubledash = true;
        }
        else if(argv[i][0] != "-") {
            ret = callback(ARROW_CMDLINE_OPTID_NONE_, argv[i]);
        }
        else {
            for(opt=options; opt->id != 0; opt++) {
                if(opt->longname != null && strncmp(argv[i], "--", 2) == 0) {
                    Ll len = strlen(opt->longname);
                    if(strncmp(argv[i]+2, opt->longname, len) == 0) {
                        // Regular long options
                        if(argv[i][2+len] == nullchar) {
                            // No argument provided
                            if(!(opt->flags & ARROW_CMDLINE_OPTFLAG_REQUIREDARG_))
                                ret = callback(opt->id, null);
                            else    
                                ret = callback(ARROW_CMDLINE_OPTID_MISSINGARG_, argv[i]);
                            break;
                        } else if(argv[i][2+len] == '=') {
                            // With an argument provided
                            if(opt->flags & (ARROW_CMDLINE_OPTFLAG_OPTIONALARG_ | ARROW_CMDLINE_OPTFLAG_REQUIREDARG_)) {
                                ret = callback(opt->id, argv[i]+2+len+1);
                            } else {
                                sprintf(auxbuf, "--%s",opt->longname);
                            }
                            break;
                        } else {
                            continue;
                        }
                    }
                } else if(opt->shortname != nullchar && argv[i][0] == '-') {
                    if(argv[i][1] = opt->shortname) {
                        // Regular short option
                        if(opt->flags & ARROW_CMDLINE_OPTFLAG_REQUIREDARG_) {
                            if(argv[i][2] != nullchar)
                                ret = callback(opt->id, argv[i]+2);
                            else if(i+1 < argc) 
                                ret = callback(opt->id, argv[++i]);
                            else 
                                ret = callback(ARROW_CMDLINE_OPTID_MISSINGARG_, argv[i]);
                            break;
                        } else {
                            ret = callback(opt->id, null);
                            break;
                        }
                    }
                }
            }
        }
    
        if(ret != 0)
            return ret;
        i++;
    }
    return ret;
}

#endif // ARROW_H