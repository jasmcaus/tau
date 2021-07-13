r"""
    This Python script sets up all required tests as part of Tau's Internal Testing Mechanism. 

    It generates a bunch of random data (for example: text, letters and numbers) that Tau will test against.
"""

import random
random.seed(3)

INCLUDE_THIS_BEFORE_EACH_FILE = "#include <tau/tau.h>\n\n"
TEST_SUITE_BEGIN = "TEST(gen_tests_%(lang)s, DEATHTESTS_%(macro)s) {\n"
TEST_SUITE_END = "}\n\n"

def generate_random_string(n=None):
    # Any random number between 2 and 100
    if n is None:
        n = 100
    n = random.randint(2, n)

    choices = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$&()*+,-.:;<=>@^_{`~{|}'
    return ''.join(random.choice(choices) for i in range(n))

def generated_random_string_data():
    a = open('DeathTests/random_generated_strings.txt', 'w')
    try:
        for _ in range(2000):
            rand1 = generate_random_string()
            rand2 = generate_random_string()
            a.write(f'"{rand1}", "{rand2}"\n')
    finally:
        a.close()

# Begin with the String Macros
def test_string_macros(lang: str, filename: str ='DeathTests/test_string_macros'):
    if filename.endswith('.c'):
        filename = filename[:-2]
    if filename.endswith('.cpp'):
        filename = filename[:-3]
    
    # Add the language extension
    filename = filename + '.' + lang
    print(f'[INFO] Writing {filename}')

    STRING_MACROS_TEXT = """    %(macro)s("%(str1)s", "%(str2)s");\n"""
    NSTRING_MACROS_TEXT = """   %(macro)s("%(str1)s", "%(str2)s", %(n)s);\n"""

    STRING_EQUAL_MACROS = ['CHECK_STREQ', 'REQUIRE_STREQ' ]
    STRING_UNEQUAL_MACROS = ['CHECK_STRNE', 'REQUIRE_STRNE']
    NSTRING_EQUAL_MACROS = ['CHECK_SUBSTREQ', 'REQUIRE_SUBSTREQ']
    NSTRING_UNEQUAL_MACROS = ['CHECK_SUBSTRNE', 'REQUIRE_SUBSTRNE']

    # Open a file to write
    a = open(filename, 'w')

    try:
        a.write(INCLUDE_THIS_BEFORE_EACH_FILE)
        
        for macro in STRING_EQUAL_MACROS:
            a.write(TEST_SUITE_BEGIN % {'lang': lang, 'macro' : macro})
            
            # Do 100 tests for each macro
            for i in range(150):
                # Get a random string
                rand_str = generate_random_string()
                a.write(STRING_MACROS_TEXT % {
                            'macro': macro,
                            'str1': rand_str,
                            'str2': rand_str,
                        })
            a.write(TEST_SUITE_END)
        
        for macro in STRING_UNEQUAL_MACROS:
            a.write(TEST_SUITE_BEGIN % {'lang': lang, 'macro' : macro})
            
            # Do 100 tests for each macro
            for i in range(150):
                # Get a random string
                rand_str1 = generate_random_string()
                rand_str2 = generate_random_string()
                a.write(STRING_MACROS_TEXT % {
                            'macro': macro,
                            'str1': rand_str1,
                            'str2': rand_str2,
                        })
            a.write(TEST_SUITE_END)
        
        for macro in NSTRING_EQUAL_MACROS:
            a.write(TEST_SUITE_BEGIN % {'lang': lang, 'macro' : macro})
            
            # Do 100 tests for each macro
            for i in range(150):
                # Get a random string
                rand_str1 = generate_random_string()
                # Append maybe 20 characters to `rand_str`
                rand_str2 = rand_str1 + generate_random_string(20)
                a.write(NSTRING_MACROS_TEXT % {
                            'macro': macro,
                            'str1': rand_str1,
                            'str2': rand_str2,
                            'n': len(rand_str1)
                        })
            a.write(TEST_SUITE_END)
        
        for macro in NSTRING_UNEQUAL_MACROS:
            a.write(TEST_SUITE_BEGIN % {'lang': lang, 'macro' : macro})
            
            # Do 100 tests for each macro
            for i in range(150):
                # Get a random string (should ideally be unequal)
                rand_str1 = generate_random_string()
                rand_str2 = generate_random_string()

                a.write(NSTRING_MACROS_TEXT % {
                            'macro': macro,
                            'str1': rand_str1,
                            'str2': rand_str2,
                            'n': len(rand_str1)
                        })
            a.write(TEST_SUITE_END)

    finally:
        a.close()

# Because of the huge size of the resulting `.c` or `.cpp` file (~7k lines) when this script runs (MSVC 2017 runs out of 
# heap space because of this), we split the files into 2 seperate source files
def test_assertion_macros(lang: str, filename: str ='DeathTests/test_assertion_macros'):
    filename1 = filename + '_1'
    filename2 = filename + '_2'
    
    # Add the language extension
    filename1 = filename1 + '.' + lang
    filename2 = filename2 + '.' + lang
    print(f'[INFO] Writing {filename1} and {filename2}')

    ASSERTION_MACROS_TEXT = """    %(macro)s(%(num1)s, %(num2)s);\n"""
    ASSERTION_MACROS = [
        'CHECK_EQ', 'CHECK_NE', 'CHECK_LT', 'CHECK_LE', 'CHECK_GT', 'CHECK_GE', 
        'REQUIRE_EQ', 'REQUIRE_NE', 'REQUIRE_LT', 'REQUIRE_LE', 'REQUIRE_GT', 'REQUIRE_GE'
    ]

    # Open a file to write
    f1 = open(filename1, 'w')
    f2 = open(filename2, 'w')

    # Initially, set f to f1
    f = f1 

    intdata = open('DeathTests/random_integers.txt', 'r').readlines()
    count = 0

    try:
        f1.write(INCLUDE_THIS_BEFORE_EACH_FILE)
        f1.write(TEST_SUITE_BEGIN % {'lang': lang, 'macro' : 'ASSERTION_MACROS_1'})
        f2.write(INCLUDE_THIS_BEFORE_EACH_FILE)
        f2.write(TEST_SUITE_BEGIN % {'lang': lang, 'macro' : 'ASSERTION_MACROS_2'})

        for line in intdata:
            line = line.strip().strip('\n').split(',')
            line[1] = line[1].strip()
            count += 1
            # Half of the 1500 lines in `random_integers.txt`
            if count == 750:
                f = f2

            num1 = int(line[0])
            num2 = int(line[1])

            if num1 < num2:
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'CHECK_LT','num1': str(num1),'num2': str(num2)})
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'REQUIRE_LT','num1': str(num1),'num2': str(num2)})
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'CHECK_GT','num1': str(num2),'num2': str(num1)})
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'REQUIRE_GT','num1': str(num2),'num2': str(num1)})
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'CHECK_LE','num1': str(num1),'num2': str(num2)})
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'REQUIRE_LE','num1': str(num1),'num2': str(num2)})

            elif num1 > num2:
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'CHECK_GT','num1': str(num1),'num2': str(num2)})
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'REQUIRE_GT','num1': str(num1),'num2': str(num2)})
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'CHECK_LT','num1': str(num2),'num2': str(num1)})
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'REQUIRE_LT','num1': str(num2),'num2': str(num1)})
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'CHECK_GE','num1': str(num1),'num2': str(num2)})
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'REQUIRE_GE','num1': str(num1),'num2': str(num2)})

            elif num1 == num2:
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'CHECK_EQ','num1': str(num1),'num2': str(num2),})
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'REQUIRE_EQ','num1': str(num1),'num2': str(num2)})
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'CHECK_GE','num1': str(num1),'num2': str(num2)})
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'REQUIRE_GE','num1': str(num1),'num2': str(num2)})
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'CHECK_LE','num1': str(num1),'num2': str(num2)})
                f.write(ASSERTION_MACROS_TEXT % {'macro': 'REQUIRE_LE','num1': str(num1),'num2': str(num2)})

        f1.write(TEST_SUITE_END)
        f2.write(TEST_SUITE_END)
    finally:
        f1.close()
        f2.close()
    

if __name__ == '__main__':
    languages = ['c', 'cpp']
    # generated_random_string_data()

    for i in languages:
        test_string_macros(str(i))
        test_assertion_macros(str(i))