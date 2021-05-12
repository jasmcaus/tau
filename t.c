#include <Arrow/Types.h>
#include <Arrow/Misc.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    #define ARROW_WIN_        1
    #include <windows.h>
    #include <io.h>
#endif


static bool arrow_should_colourize_output = true;
static bool arrow_disable_summary = false; 

#define ARROW_COLOR_DEFAULT_              0
#define ARROW_COLOR_GREEN_                1
#define ARROW_COLOR_RED_                  2
#define ARROW_COLOR_DEFAULT_INTENSIVE_    3
#define ARROW_COLOR_GREEN_INTENSIVE_      4
#define ARROW_COLOR_RED_INTENSIVE_        5

static int
arrow_coloured_printf_(int color, const char* fmt, ...) {
    va_list args;
    char buffer[256];
    int n;

    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    buffer[sizeof(buffer)-1] = '\0';

    // if(!arrow_should_colourize_output) {
    //     return printf("%s", buffer);
    // }

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


int main() {
    arrow_coloured_printf_(ARROW_COLOR_RED_, "SUCCESS:");
    return 0;
}