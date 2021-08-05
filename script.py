# import os 
# import os.path as osp 

# BASE = r'/home/jas/Documents/dev/adorad/adorad/core'
# HERE = osp.join(osp.dirname(__file__), 'tau')

# ADORAD_COPYRIGHT = """\
#           _____   ____  _____            _____  
#     /\   |  __ \ / __ \|  __ \     /\   |  __ \ 
#    /  \  | |  | | |  | | |__) |   /  \  | |  | | Adorad - The Fast, Expressive & Elegant Programming Language
#   / /\ \ | |  | | |  | |  _  /   / /\ \ | |  | | Languages: C, C++, and Assembly
#  / ____ \| |__| | |__| | | \ \  / ____ \| |__| | https://github.com/adorad/adorad/
# /_/    \_\_____/ \____/|_|  \_\/_/    \_\_____/ 
# """

# TAU_COPYRIGHT = """\
#  _______          _    _
# |__   __|  /\    | |  | |
#    | |    /  \   | |  | |  Tau - The Micro Testing Framework for C/C++
#    | |   / /\ \  | |  | |  Language: C
#    | |  / ____ \ | |__| |  https://github.com/jasmcaus/tau
#    |_| /_/    \_\ \____/
# """

# DEBUG_CHECK = """\

# TAU_DEBUG_CHECK(sizeof(TAU_UInt8)  == sizeof(TAU_Int8));
# TAU_DEBUG_CHECK(sizeof(TAU_UInt16) == sizeof(TAU_Int16));
# TAU_DEBUG_CHECK(sizeof(TAU_UInt32) == sizeof(TAU_Int32));
# TAU_DEBUG_CHECK(sizeof(TAU_UInt64) == sizeof(TAU_Int64));

# TAU_DEBUG_CHECK(sizeof(TAU_UInt8)  == 1);
# TAU_DEBUG_CHECK(sizeof(TAU_UInt16) == 2);
# TAU_DEBUG_CHECK(sizeof(TAU_UInt32) == 4);
# TAU_DEBUG_CHECK(sizeof(TAU_UInt64) == 8);

# TAU_DEBUG_CHECK(sizeof(TAU_Float32) == 4);
# TAU_DEBUG_CHECK(sizeof(TAU_Float64) == 8);
# """

# TYPES = (
#     'UInt8', 'Int8', 'UInt16', 'Int16', 'UInt32', 'Int32', 'UInt64', 'Int64', 'Float32', 'Float64', 'Bool8', 'Bool16', 'Bool32',
#     'Ull', 'Ll', 'UIntptr', 'Intptr'
# )

# STATICS = """
# // Statics!
# // static means 3-4 different things in C/C++!!
# #ifndef TAU_EXTERN
#     #define TAU_EXTERN       extern
# #endif 

# #ifndef TAU_STATIC
#     #define TAU_STATIC       static
# #endif

# #ifndef TAU_GLOBAL
#     #define TAU_GLOBAL       static // Global Variables
#     #define TAU_INTERNAL     static // Internal Linkage
#     #define TAU_LOCALPERSIST static // Local Persisting Variables  
# #endif 
# """

# MAX_AND_MIN = """\
# // Unicode codepoint
# typedef TAU_Int32 Rune; 
# #define TAU_RUNE_INVALID cast(Rune)(0xfffd)
# #define TAU_RUNE_MAX     cast(Rune)(0x0010ffff)
# #define TAU_RUNE_BOM     cast(Rune)(0xfeff)
# #define TAU_RUNE_EOF     cast(Rune)(-1)

# // Max and Min 
# #ifndef TAU_UInt8_MIN 
#     #define TAU_UInt8_MIN 0u
#     #define TAU_UInt8_MAX 0xffu
#     #define TAU_Int8_MIN  (-0x7f - 1)
#     #define TAU_Int8_MAX  0x7f

#     #define TAU_UInt16_MIN 0u
#     #define TAU_UInt16_MAX 0xffffu
#     #define TAU_Int16_MIN  (-0x7fff - 1)
#     #define TAU_Int16_MAX  0x7fff
 
#     #define TAU_UInt32_MIN 0u
#     #define TAU_UInt32_MAX 0xffffffffu
#     #define TAU_Int32_MIN  (-0x7fffffff - 1)
#     #define TAU_Int32_MAX  0x7fffffff

#     #define TAU_UInt64_MIN 0ull
#     #define TAU_UInt64_MAX 0xffffffffffffffffull
#     #define TAU_Int64_MIN  (-0x7fffffffffffffffll - 1)
#     #define TAU_Int64_MAX  0x7fffffffffffffffll

#     #if defined(TAU_ARCH_32BIT)
#         #define UINTSIZE_MIX TAU_UInt32_MIN
#         #define UINTSIZE_MAX TAU_UInt32_MAX

#         #define INTSIZE_MIX TAU_Int32_MIN
#         #define INTSIZE_MAX TAU_Int32_MAX
    
#     #elif defined(TAU_ARCH_64BIT)
#         #define UINTSIZE_MIX TAU_UInt64_MIN
#         #define UINTSIZE_MAX TAU_UInt64_MAX

#         #define INTSIZE_MIX TAU_Int64_MIN
#         #define INTSIZE_MAX TAU_Int64_MAX
#     #endif 
# #else
#         #error Unknown architecture size. Tau only supports 32-bit and 64-bit architectures.
# #endif 

# #define TAU_Float32_MIN 1.17549435e-38f
# #define TAU_Float32_MAX 3.40282347e+38f

# #define TAU_Float64_MIN 2.2250738585072014e-308
# #define TAU_Float64_MAX 1.7976931348623157e+308
# """

# WARNING = r"""
# #ifndef TAU_DISABLE_DEBUG_WARNINGS
# #define TAU_DISABLE_DEBUG_WARNINGS                                                                    \
#     /* MSVC Warnings */                                                                               \
#     TAU_MSVC_SUPPRESS_WARNING_PUSH                                                                    \
#     /* Conditional expression is constant */                                                          \
#     TAU_MSVC_SUPPRESS_WARNING(4127)                                                                   \
#     /* Assignment within conditional expression */                                                    \
#     TAU_MSVC_SUPPRESS_WARNING(4706)                                                                   \
#     /* Unreferenced inline function has been removed */                                               \
#     TAU_MSVC_SUPPRESS_WARNING(4514)                                                                   \
#     /* Function not inlined*/                                                                         \
#     TAU_MSVC_SUPPRESS_WARNING(4710)                                                                   \
#     /* function 'x' selected for automatic inline expansion */                                        \
#     TAU_MSVC_SUPPRESS_WARNING(4711)                                                                   \
#     /* No function prototype given: converting '()' to '(void)' */                                    \
#     TAU_MSVC_SUPPRESS_WARNING(4255)                                                                   \
#     /* '__cplusplus' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif' */    \
#     TAU_MSVC_SUPPRESS_WARNING(4668)                                                                   \
#     /* 'fprintf' : format string '' requires an argument of type 'unsigned __int64', but variadic */  \
#     /* argument 1 has type 'size_t' */                                                                \
#     TAU_MSVC_SUPPRESS_WARNING(4777)                                                                   \
#     /* https://stackoverflow.com/questions/39479163 - what's the difference between 4018 and 4389 */  \
#     /* signed/unsigned mismatch */                                                                    \
#     TAU_MSVC_SUPPRESS_WARNING(4388)                                                                   \
#     /* expression' : signed/unsigned mismatch */                                                      \
#     TAU_MSVC_SUPPRESS_WARNING(4018)                                                                   \
#     /* Conversion from 'int' to 'unsigned long', signed/unsigned mismatch */                          \
#     TAU_MSVC_SUPPRESS_WARNING(4365)                                                                   \
#     /* format string expected in argument 'x' is not a string literal */                              \
#     TAU_MSVC_SUPPRESS_WARNING(4774)                                                                   \
#     /* 'var' : conversion from 'x' to 'y', possible loss of data */                                   \
#     TAU_MSVC_SUPPRESS_WARNING(4267)                                                                   \
#     /* In multi-platform code, we cannot use the non-standard "safe" functions from */                \
#     /* Microsoft's C lib like e.g. sprintf_s() instead of standard sprintf(). */                      \
#     TAU_MSVC_SUPPRESS_WARNING(4996)                                                                   \
#     /* warning C4090: '=': different 'const' qualifiers */                                            \
#     TAU_MSVC_SUPPRESS_WARNING(4090)                                                                   \
#     /* io.h contains definitions for some structures with natural padding. This is uninteresting, */  \
#     /* but for some reason, MSVC's behaviour is to warn about including this system header. That  */  \
#     /* *is* interesting */                                                                            \
#     TAU_MSVC_SUPPRESS_WARNING(4820)                                                                   \
#     /* Miscellaneous */                                                                               \
#     TAU_MSVC_SUPPRESS_WARNING(4061)                                                                   \
#     TAU_MSVC_SUPPRESS_WARNING(4100)                                                                   \
#     TAU_MSVC_SUPPRESS_WARNING(4201)                                                                   \
#     TAU_MSVC_SUPPRESS_WARNING(5045)                                                                   \
#     TAU_MSVC_SUPPRESS_WARNING(5105)                                                                   \
#                                                                                                       \
#     /* Clang */                                                                                       \
#     TAU_CLANG_SUPPRESS_WARNING_PUSH                                                                   \
#     TAU_CLANG_SUPPRESS_WARNING("-Wdisabled-macro-expansion")                                          \
#     TAU_CLANG_SUPPRESS_WARNING("-Wlanguage-extension-token")                                          \
#     TAU_CLANG_SUPPRESS_WARNING("-Wc++98-compat")                                                      \
#     TAU_CLANG_SUPPRESS_WARNING("-Wc++98-compat-pedantic")                                             \
#     TAU_CLANG_SUPPRESS_WARNING("-Wfloat-equal")                                                       \
#     TAU_CLANG_SUPPRESS_WARNING("-Wmissing-variable-declarations")                                     \
#     TAU_CLANG_SUPPRESS_WARNING("-Wreserved-id-macro")                                                 \
#     TAU_CLANG_SUPPRESS_WARNING("-Wunknown-pragmas")                                                   \
#     TAU_CLANG_SUPPRESS_WARNING("-Wstrict-overflow")                                                   \
#     TAU_CLANG_SUPPRESS_WARNING("-Wstrict-aliasing")                                                   \
#     TAU_CLANG_SUPPRESS_WARNING("-Wsign-compare")                                                      \
#     TAU_CLANG_SUPPRESS_WARNING("-Wsign-conversion")                                                   \
#     TAU_CLANG_SUPPRESS_WARNING("-Wunused-macros")                                                     \
#     TAU_CLANG_SUPPRESS_WARNING("-Wunused-function")                                                   \
#     TAU_CLANG_SUPPRESS_WARNING("-Wunused-variable")                                                   \
#     TAU_CLANG_SUPPRESS_WARNING("-Wunused-result")                                                     \
#     TAU_CLANG_SUPPRESS_WARNING("-Wunused-parameter")                                                  \
#     TAU_CLANG_SUPPRESS_WARNING("-Wunused-local-typedefs")                                             \
#     TAU_CLANG_SUPPRESS_WARNING("-Wswitch")                                                            \
#     TAU_CLANG_SUPPRESS_WARNING("-Wswitch-enum")                                                       \
#     TAU_CLANG_SUPPRESS_WARNING("-Wswitch-default")                                                    \
#     TAU_CLANG_SUPPRESS_WARNING("-Wformat")                                                            \
#     TAU_CLANG_SUPPRESS_WARNING("-Wformat-extra-args")                                                 \
#     TAU_CLANG_SUPPRESS_WARNING("-Wignored-qualifiers")                                                \
#     TAU_CLANG_SUPPRESS_WARNING("-Wuninitialized")                                                     \
#     TAU_CLANG_SUPPRESS_WARNING("-Wunused-const-variable")                                             \
#     TAU_CLANG_SUPPRESS_WARNING("-Wnewline-eof")                                                       \
#     TAU_CLANG_SUPPRESS_WARNING("-Wdeprecated-declarations")                                           \
#                                                                                                       \
#     /* GCC */                                                                                         \
#     TAU_GCC_SUPPRESS_WARNING_PUSH                                                                     \
#     TAU_GCC_SUPPRESS_WARNING("-Wunknown-pragmas")                                                     \
#     TAU_GCC_SUPPRESS_WARNING("-Wpragmas")                                                             \
#     TAU_GCC_SUPPRESS_WARNING("-Wmaybe-uninitialized")                                                 \
#     TAU_GCC_SUPPRESS_WARNING("-Wmultistatement-macros")                                               \
#     TAU_GCC_SUPPRESS_WARNING("-Wdiscarded-qualifiers")                                                \
#     TAU_GCC_SUPPRESS_WARNING("-Wdisabled-macro-expansion")                                            \
#     TAU_GCC_SUPPRESS_WARNING("-Wlanguage-extension-token")                                            \
#     TAU_GCC_SUPPRESS_WARNING("-Wc++98-compat")                                                        \
#     TAU_GCC_SUPPRESS_WARNING("-Wc++98-compat-pedantic")                                               \
#     TAU_GCC_SUPPRESS_WARNING("-Wfloat-equal")                                                         \
#     TAU_GCC_SUPPRESS_WARNING("-Wmissing-variable-declarations")                                       \
#     TAU_GCC_SUPPRESS_WARNING("-Wreserved-id-macro")                                                   \
#     TAU_GCC_SUPPRESS_WARNING("-Wunknown-pragmas")                                                     \
#     TAU_GCC_SUPPRESS_WARNING("-Wstrict-overflow")                                                     \
#     TAU_GCC_SUPPRESS_WARNING("-Wstrict-aliasing")                                                     \
#     TAU_GCC_SUPPRESS_WARNING("-Wsign-compare")                                                        \
#     TAU_GCC_SUPPRESS_WARNING("-Wsign-conversion")                                                     \
#     TAU_GCC_SUPPRESS_WARNING("-Wunused-macros")                                                       \
#     TAU_GCC_SUPPRESS_WARNING("-Wunused-function")                                                     \
#     TAU_GCC_SUPPRESS_WARNING("-Wunused-variable")                                                     \
#     TAU_GCC_SUPPRESS_WARNING("-Wunused-result")                                                       \
#     TAU_GCC_SUPPRESS_WARNING("-Wunused-parameter")                                                    \
#     TAU_GCC_SUPPRESS_WARNING("-Wunused-local-typedefs")                                               \
#     TAU_GCC_SUPPRESS_WARNING("-Wswitch")                                                              \
#     TAU_GCC_SUPPRESS_WARNING("-Wswitch-enum")                                                         \
#     TAU_GCC_SUPPRESS_WARNING("-Wswitch-default")                                                      \
#     TAU_GCC_SUPPRESS_WARNING("-Wformat")                                                              \
#     TAU_GCC_SUPPRESS_WARNING("-Wformat-extra-args")                                                   \
#     TAU_GCC_SUPPRESS_WARNING("-Wsuggest-attribute=format")                                            \
#     TAU_GCC_SUPPRESS_WARNING("-Wdeprecated-declarations")
# #endif // TAU_DISABLE_DEBUG_WARNINGS

# #ifndef TAU_DISABLE_DEBUG_WARNINGS_POP
# #define TAU_DISABLE_DEBUG_WARNINGS_POP     \
#     TAU_MSVC_SUPPRESS_WARNING_POP          \
#     TAU_CLANG_SUPPRESS_WARNING_POP         \
#     TAU_GCC_SUPPRESS_WARNING_POP      
# #endif // TAU_DISABLE_DEBUG_WARNINGS_POP

# #endif // TAU_COMPILERS_H
# """
 
# for file in os.listdir(BASE):
#     if file.endswith(('compilers.h', 'types.h')):
#         with open(osp.join(BASE, file), 'r') as f:
#             s = f.read()

#         s = s.replace(ADORAD_COPYRIGHT, TAU_COPYRIGHT)
#         s = s.replace('CSTL_', 'TAU_')
#         s = s.replace('Adorad', 'Tau')
#         s = s.replace('adorad', 'tau')
#         s = s.replace('tau/core', 'tau')
#         s = s.replace('#include <tau/debug.h>\n', '')

#         for type in TYPES:
#             s = s.replace(type, f'TAU_{type}')
#         s = s.replace('TAU_UTAU_', 'TAU_U')
#         s = s.replace('nullchar', 'TAU_NULLCHAR')
#         s = s.replace('null', 'TAU_NULL')
#         s = s.replace('TAU_NULLptr', 'nullptr')
#         s = s.replace('Bool_', 'TAU_Bool_')
#         s  = s.replace('typedef TAU_Bool32 bool', 'typedef TAU_Bool32 TAU_Bool')
#         s = s.replace('#define false', '#define TAU_false')
#         s = s.replace('#define true', '#define TAU_true')
#         s = s.replace('#define TAU_false  false', '#define TAU_Bool   bool\n        #define TAU_false  false')
#         s = s.replace('static const bool false', 'static const TAU_Bool TAU_false')
#         s = s.replace('static const bool true', 'static const TAU_Bool TAU_true')
#         s = s.replace('cast', 'TAU_CAST')
#         s = s.replace('ptrTAU_CAST', 'TAU_PTRCAST')
#         s = s.replace('static_TAU_CAST', 'static_cast')
#         s = s.replace('reinterpret_TAU_CAST', 'reinterpret_cast')


#         s = s.replace(STATICS, "")
#         s = s.replace(f'\n{MAX_AND_MIN}', "")
#         s = s.replace('#define TAU_TYPEOF(val) ', '#define TAU_TYPEOF(val)  ')
#         s = s.replace('(U)TAU_Intptr', '(U)Intptr')
#         s = s.replace('_W64', 'TAU__W64')
#         s = s.replace('TAU_DEBUG_CHECK(sizeof(TAU_UIntptr) == sizeof(TAU_Intptr));\n\n', '')
#         s = s.replace(DEBUG_CHECK, '')
        
#         if file.endswith('compilers.h'):
#             s = s.replace('\n#endif // TAU_COMPILERS_H', WARNING)
        
#         # Write changes
#         with open(osp.join(HERE, file), 'w') as f:
#             f.write(s)