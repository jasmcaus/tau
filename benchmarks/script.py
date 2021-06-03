import os 
import time 


MAIN_TEXT = \
"""
#include <tau/tau.h>
TAU_MAIN()

"""
META_TEXT =\
"""
TEST(%(i)s, %(i)s) { CHECK_EQ(%(i)s, %(i)s); }
"""

def write_meta(filename='16k_assertions_CHECK_EQ.c'):
    print(f'[INFO] Writing {filename}')
    TEXT = META_TEXT
    a = open(filename, 'w')

    try:
        a.write(MAIN_TEXT)
        for i in range(16000):
            a.write(TEXT % {'i': i})
    finally:
        a.close()

def timeexec():
    st = time.time()
    os.system('test.exe --failed-output-only')
    end = time.time()
    print(f"Took {end-st}s")

if __name__ == '__main__':
    # write_meta()
    timeexec()
