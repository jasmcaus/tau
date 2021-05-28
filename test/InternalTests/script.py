r"""
    This Python script sets up all required tests as part of Muon's Internal Testing Mechanism. 

    It generates a bunch of random data (for example: text, letters and numbers) that Muon will test against.
"""

import random

INCLUDE_THIS_BEFORE_EACH_FILE = "#include <Muon/Muon.h>\n\n"

TEST_SUITE_BEGIN = "TEST(gen_tests_%(lang)s, %(macro)s) {\n"
TEST_SUITE_END = "}\n\n"

STRING_MACROS_TEXT = """    %(macro)s("%(str1)s", "%(str2)s");\n"""
NSTRING_MACROS_TEXT = """   %(macro)s("%(str1)s", "%(str2)s");\n"""


def generate_random_string():
    # Any random number between 2 and 100
    n = random.randint(2, 100)

    choices = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!#$&()*+,-.:;<=>?@^_{`~{|}'
    return ''.join(random.choice(choices) for i in range(n))


# Begin with the String Macros
def test_string_macros(lang: str, filename: str ='test_string_macros'):
    if filename.endswith('.c'):
        filename = filename[:-2]
    if filename.endswith('.cpp'):
        filename = filename[:-3]
    
    # Add the language extension
    filename = filename + '.' + lang
    print(f'[INFO] Writing {filename}')

    STRING_EQUAL_MACROS = ['CHECK_STREQ', 'REQUIRE_STREQ' ]
    STRING_UNEQUAL_MACROS = ['CHECK_STRNEQ', 'REQUIRE_STRNEQ']
    NSTRING_EQUAL_MACROS = ['CHECK_STRNE', 'REQUIRE_STRNE']
    NSTRING_UNEQUAL_MACROS = ['CHECK_STRNNE', 'REQUIRE_STRNNE']

    # Open a file to write
    a = open(filename, 'w')

    try:
        a.write(INCLUDE_THIS_BEFORE_EACH_FILE)
        
        for macro in STRING_EQUAL_MACROS:
            a.write(TEST_SUITE_BEGIN % {'lang': lang, 'macro' : "STR_EQUAL_" + macro})
            
            # Do 100 tests for each macro
            for i in range(100):
                # Get a random string
                rand_str = generate_random_string()

                a.write(STRING_MACROS_TEXT % {
                            'macro': macro,
                            'str1': rand_str,
                            'str2': rand_str,
                        })
            
            a.write(TEST_SUITE_END)

    finally:
        a.close()


if __name__ == '__main__':
    languages = ['c', 'cpp']
    for i in languages:
        test_string_macros(str(i))