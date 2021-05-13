#include <windows.h>
#include <stdio.h>

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
    printf("QueryPerformanceFrequency failed!\n");

    PCFreq = (double)(li.QuadPart);

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return (double)(li.QuadPart-CounterStart)/PCFreq;
}

int main()
{
    StartCounter();
    Sleep(100);
    printf("%.4lf sec\n", GetCounter());
    return 0;
}