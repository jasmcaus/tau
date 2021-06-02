/*
|  \/  | |  | |/ __ \| \ | |
| \  / | |  | | |  | |  \| |    Muon - The Micro Testing Framework for C/C++
| |\/| | |  | | |  | | . ` |    Languages: C, and C++
| |  | | |__| | |__| | |\  |    https://github.com/jasmcaus/Muon
|_|  |_|\____/ \____/|_| \_|

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

#ifndef MUON_COMPILERS_H
#define MUON_COMPILERS_H

// General compiler feature support table: https://en.cppreference.com/w/cpp/compiler_support
// MSVC C++11 feature support table: https://msdn.microsoft.com/en-us/library/hh567368.aspx
// GCC C++11 feature support table: https://gcc.gnu.org/projects/cxx-status.html
// 
// MSVC version table:
// https://en.wikipedia.org/wiki/Microsoft_Visual_C%2B%2B#Internal_version_numbering
// MSVC++ 14.2 (16) _MSC_VER == 1920 (Visual Studio 2019)
// MSVC++ 14.1 (15) _MSC_VER == 1910 (Visual Studio 2017)
// MSVC++ 14.0      _MSC_VER == 1900 (Visual Studio 2015)
// MSVC++ 12.0      _MSC_VER == 1800 (Visual Studio 2013)
// MSVC++ 11.0      _MSC_VER == 1700 (Visual Studio 2012)
// MSVC++ 10.0      _MSC_VER == 1600 (Visual Studio 2010)
// MSVC++ 9.0       _MSC_VER == 1500 (Visual Studio 2008)
// MSVC++ 8.0       _MSC_VER == 1400 (Visual Studio 2005)

// Compilers
#if defined(_MSC_VER)
    #define MUON_COMPILER_MSVC 1
#elif defined(__GNUC__)
    #define MUON_COMPILER_GCC 1
#elif defined(__clang__)
    #define MUON_COMPILER_CLANG 1
#else 
    #error Unknown Compiler (Muon currently supports only MSVC, GCC and Clang)
#endif 

// Disable Compiler Warnings
// These are a big nuisance when compiling with external code
#if defined(MUON_COMPILER_MSVC)
    #define MUON_MSVC_SUPPRESS_WARNING_PUSH     __pragma(warning(push))
    #define MUON_MSVC_SUPPRESS_WARNING(w)       __pragma(warning(disable : w))
    #define MUON_MSVC_SUPPRESS_WARNING_POP      __pragma(warning(pop))
#else
    #define MUON_MSVC_SUPPRESS_WARNING_PUSH
    #define MUON_MSVC_SUPPRESS_WARNING(w)
    #define MUON_MSVC_SUPPRESS_WARNING_POP
#endif // MUON_MSVC

#if defined(MUON_COMPILER_CLANG)
    #define PRAGMA_TO_STR(x)                    _Pragma(#x)
    #define MUON_CLANG_SUPPRESS_WARNING_PUSH    _Pragma("clang diagnostic push")
    #define MUON_CLANG_SUPPRESS_WARNING_POP     _Pragma("clang diagnostic pop")
    #define MUON_CLANG_SUPPRESS_WARNING(w)      PRAGMA_TO_STR(clang diagnostic ignored w)
#else
    #define MUON_CLANG_SUPPRESS_WARNING_PUSH
    #define MUON_CLANG_SUPPRESS_WARNING(w)
    #define MUON_CLANG_SUPPRESS_WARNING_POP
#endif // MUON_COMPILER_CLANG

#if defined(MUON_COMPILER_GCC)
    #define PRAGMA_TO_STR(x)                    _Pragma(#x)
    #define MUON_GCC_SUPPRESS_WARNING_PUSH      _Pragma("GCC diagnostic push")
    #define MUON_GCC_SUPPRESS_WARNING_POP       _Pragma("GCC diagnostic pop")
    #define MUON_GCC_SUPPRESS_WARNING(w)        PRAGMA_TO_STR(GCC diagnostic ignored w)
#else
    #define MUON_GCC_SUPPRESS_WARNING_PUSH
    #define MUON_GCC_SUPPRESS_WARNING(w)
    #define MUON_GCC_SUPPRESS_WARNING_POP
#endif // MUON_COMPILER_GCC


#ifndef MUON_DISABLE_WARNINGS
#define MUON_DISABLE_WARNINGS                                                                        \
    /* MSVC Warnings */                                                                              \
    MUON_MSVC_SUPPRESS_WARNING_PUSH                                                                  \
    /* Conditional expression is constant */                                                         \
    MUON_MSVC_SUPPRESS_WARNING(4127)                                                                 \
    /* Assignment within conditional expression */                                                   \
    MUON_MSVC_SUPPRESS_WARNING(4706)                                                                 \
    /* Unreferenced inline function has been removed */                                              \
    MUON_MSVC_SUPPRESS_WARNING(4514)                                                                 \
    /* Function not inlined*/                                                                        \
    MUON_MSVC_SUPPRESS_WARNING(4710)                                                                 \
    /* function 'x' selected for automatic inline expansion */                                       \
    MUON_MSVC_SUPPRESS_WARNING(4711)                                                                 \
    /* No function prototype given: converting '()' to '(void)' */                                   \
    MUON_MSVC_SUPPRESS_WARNING(4255)                                                                 \
    /* '__cplusplus' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif' */   \
    MUON_MSVC_SUPPRESS_WARNING(4668)                                                                 \
    /* 'fprintf' : format string '' requires an argument of type 'unsigned __int64', but variadic */ \
    /* argument 1 has type 'size_t' */                                                               \
    MUON_MSVC_SUPPRESS_WARNING(4777)                                                                 \
    /* https://stackoverflow.com/questions/39479163 - what's the difference between 4018 and 4389 */ \
    /* signed/unsigned mismatch */                                                                   \
    MUON_MSVC_SUPPRESS_WARNING(4388)                                                                 \
    /* expression' : signed/unsigned mismatch */                                                     \
    MUON_MSVC_SUPPRESS_WARNING(4018)                                                                 \
    /* Conversion from 'int' to 'unsigned long', signed/unsigned mismatch */                         \
    MUON_MSVC_SUPPRESS_WARNING(4365)                                                                 \
    /* format string expected in argument 'x' is not a string literal */                             \
    MUON_MSVC_SUPPRESS_WARNING(4774)                                                                 \
    /* 'var' : conversion from 'x' to 'y', possible loss of data */                                  \
    MUON_MSVC_SUPPRESS_WARNING(4267)                                                                 \
    /* In multi-platform code, we cannot use the non-standard "safe" functions from */               \
    /* Microsoft's C lib like e.g. sprintf_s() instead of standard sprintf(). */                     \
    MUON_MSVC_SUPPRESS_WARNING(4996)                                                                 \
    /* warning C4090: '=': different 'const' qualifiers */                                           \
    MUON_MSVC_SUPPRESS_WARNING(4090)                                                                 \
    /* io.h contains definitions for some structures with natural padding. This is uninteresting, */ \
    /* but for some reason, MSVC's behaviour is to warn about including this system header. That  */ \
    /* *is* interesting */                                                                           \
    MUON_MSVC_SUPPRESS_WARNING(4820)                                                                 \
    /* Miscellaneous */                                                                              \
    MUON_MSVC_SUPPRESS_WARNING(4061)                                                                 \
    MUON_MSVC_SUPPRESS_WARNING(4100)                                                                 \
    MUON_MSVC_SUPPRESS_WARNING(4201)                                                                 \
    MUON_MSVC_SUPPRESS_WARNING(5045)                                                                 \
    MUON_MSVC_SUPPRESS_WARNING(5105)                                                                 \
                                                                                                     \
    /* Clang */                                                                                      \
    MUON_CLANG_SUPPRESS_WARNING_PUSH                                                                 \
    MUON_CLANG_SUPPRESS_WARNING("-Wdisabled-macro-expansion")                                        \
    MUON_CLANG_SUPPRESS_WARNING("-Wlanguage-extension-token")                                        \
    MUON_CLANG_SUPPRESS_WARNING("-Wc++98-compat")                                                    \
    MUON_CLANG_SUPPRESS_WARNING("-Wc++98-compat-pedantic")                                           \
    MUON_CLANG_SUPPRESS_WARNING("-Wfloat-equal")                                                     \
    MUON_CLANG_SUPPRESS_WARNING("-Wmissing-variable-declarations")                                   \
    MUON_CLANG_SUPPRESS_WARNING("-Wreserved-id-macro")                                               \
    MUON_CLANG_SUPPRESS_WARNING("-Wunknown-pragmas")                                                 \
    MUON_CLANG_SUPPRESS_WARNING("-Wstrict-overflow")                                                 \
    MUON_CLANG_SUPPRESS_WARNING("-Wstrict-aliasing")                                                 \
    MUON_CLANG_SUPPRESS_WARNING("-Wsign-compare")                                                    \
    MUON_CLANG_SUPPRESS_WARNING("-Wsign-conversion")                                                 \
    MUON_CLANG_SUPPRESS_WARNING("-Wunused-macros")                                                   \
    MUON_CLANG_SUPPRESS_WARNING("-Wunused-function")                                                 \
    MUON_CLANG_SUPPRESS_WARNING("-Wunused-variable")                                                 \
    MUON_CLANG_SUPPRESS_WARNING("-Wunused-result")                                                   \
    MUON_CLANG_SUPPRESS_WARNING("-Wunused-parameter")                                                \
    MUON_CLANG_SUPPRESS_WARNING("-Wunused-local-typedefs")                                           \
    MUON_CLANG_SUPPRESS_WARNING("-Wswitch")                                                          \
    MUON_CLANG_SUPPRESS_WARNING("-Wswitch-enum")                                                     \
    MUON_CLANG_SUPPRESS_WARNING("-Wswitch-default")                                                  \
    MUON_CLANG_SUPPRESS_WARNING("-Wformat")                                                          \
    MUON_CLANG_SUPPRESS_WARNING("-Wformat-extra-args")                                               \
    MUON_CLANG_SUPPRESS_WARNING("-Wignored-qualifiers")                                              \
    MUON_CLANG_SUPPRESS_WARNING("-Wuninitialized")                                                   \
    MUON_CLANG_SUPPRESS_WARNING("-Wunused-const-variable")                                           \
    MUON_CLANG_SUPPRESS_WARNING("-Wnewline-eof")                                                     \
    MUON_CLANG_SUPPRESS_WARNING("-Wdeprecated-declarations")                                         \
                                                                                                     \
    /* GCC */                                                                                        \
    MUON_GCC_SUPPRESS_WARNING_PUSH                                                                   \
    MUON_GCC_SUPPRESS_WARNING("-Wunknown-pragmas")                                                   \
    MUON_GCC_SUPPRESS_WARNING("-Wpragmas")                                                           \
    MUON_GCC_SUPPRESS_WARNING("-Wmaybe-uninitialized")                                               \
    MUON_GCC_SUPPRESS_WARNING("-Wmultistatement-macros")                                             \
    MUON_GCC_SUPPRESS_WARNING("-Wdiscarded-qualifiers")                                              \
    MUON_GCC_SUPPRESS_WARNING("-Wdisabled-macro-expansion")                                          \
    MUON_GCC_SUPPRESS_WARNING("-Wlanguage-extension-token")                                          \
    MUON_GCC_SUPPRESS_WARNING("-Wc++98-compat")                                                      \
    MUON_GCC_SUPPRESS_WARNING("-Wc++98-compat-pedantic")                                             \
    MUON_GCC_SUPPRESS_WARNING("-Wfloat-equal")                                                       \
    MUON_GCC_SUPPRESS_WARNING("-Wmissing-variable-declarations")                                     \
    MUON_GCC_SUPPRESS_WARNING("-Wreserved-id-macro")                                                 \
    MUON_GCC_SUPPRESS_WARNING("-Wunknown-pragmas")                                                   \
    MUON_GCC_SUPPRESS_WARNING("-Wstrict-overflow")                                                   \
    MUON_GCC_SUPPRESS_WARNING("-Wstrict-aliasing")                                                   \
    MUON_GCC_SUPPRESS_WARNING("-Wsign-compare")                                                      \
    MUON_GCC_SUPPRESS_WARNING("-Wsign-conversion")                                                   \
    MUON_GCC_SUPPRESS_WARNING("-Wunused-macros")                                                     \
    MUON_GCC_SUPPRESS_WARNING("-Wunused-function")                                                   \
    MUON_GCC_SUPPRESS_WARNING("-Wunused-variable")                                                   \
    MUON_GCC_SUPPRESS_WARNING("-Wunused-result")                                                     \
    MUON_GCC_SUPPRESS_WARNING("-Wunused-parameter")                                                  \
    MUON_GCC_SUPPRESS_WARNING("-Wunused-local-typedefs")                                             \
    MUON_GCC_SUPPRESS_WARNING("-Wswitch")                                                            \
    MUON_GCC_SUPPRESS_WARNING("-Wswitch-enum")                                                       \
    MUON_GCC_SUPPRESS_WARNING("-Wswitch-default")                                                    \
    MUON_GCC_SUPPRESS_WARNING("-Wformat")                                                            \
    MUON_GCC_SUPPRESS_WARNING("-Wformat-extra-args")                                                 \
    MUON_GCC_SUPPRESS_WARNING("-Wdeprecated-declarations")
#endif // MUON_DISABLE_WARNINGS

#ifndef MUON_DISABLE_WARNINGS_POP
#define MUON_DISABLE_WARNINGS_POP     \
    MUON_MSVC_SUPPRESS_WARNING_POP    \
    MUON_CLANG_SUPPRESS_WARNING_POP   \
    MUON_GCC_SUPPRESS_WARNING_POP      
#endif // MUON_DISABLE_WARNINGS_POP

#endif // MUON_COMPILERS_H

