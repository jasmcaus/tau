#include "acutest.h"

void test_tutorial(void)
{
    void* mem;

    mem = malloc(10);
    TEST_CHECK(mem != NULL);

    mem = realloc(mem, 20);
    TEST_CHECK(mem != NULL);

    free(mem);
}

void test_time(void)
{
    int count = 0; 
    for(int i=0; i<1000000000; i++) 
        count++;
    
    TEST_CHECK(count == 1000000000);

}


TEST_LIST = {
    { "tutorial", test_tutorial },
    { "time",     test_time },
    { NULL, NULL }
};
