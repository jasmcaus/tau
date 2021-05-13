#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
    #define ACUTEST_WIN_        1
    #include <windows.h>
    #include <io.h>
#endif
#include <stdio.h>

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
    printf("%.6lf ms", acutest_timer_diff_(acutest_timer_start_, acutest_timer_end_));
    // printf("%.6lf secs", acutest_timer_diff_(acutest_timer_start_, acutest_timer_end_));
}

int main() {
    acutest_timer_type_ start, end;
    acutest_timer_init_();
    double s = 35000 / (double)acutest_timer_freq_.QuadPart;
    printf("%f", s);
}