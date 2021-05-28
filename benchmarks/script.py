MAIN_TEXT = \
"""
#include <Muon/Muon.h>
MUON_MAIN()

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

write_meta()
