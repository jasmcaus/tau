/*
 _______          _    _
|__   __|  /\    | |  | |
   | |    /  \   | |  | |  Tau - The Micro Testing Framework for C/C++
   | |   / /\ \  | |  | |  Language: C
   | |  / ____ \ | |__| |  https://github.com/jasmcaus/tau
   |_| /_/    \_\ \____/

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <@jasmcaus>
*/

#ifndef TAU_COMPILERS_H
#define TAU_COMPILERS_H

// Compilers
// Why is Clang here? See https://stackoverflow.com/a/66722270
#if defined(_MSC_VER) && !defined(__clang__)
    #define TAU_COMPILER_MSVC 1
#elif defined(__clang__)
    #define TAU_COMPILER_CLANG 1
#elif defined(__GNUC__) && !defined(INTEL_COMPILER) && !defined(llvm)
    #define TAU_COMPILER_GCC 1
#else
    #error Unknown Compiler (Tau currently supports only MSVC, GCC and Clang)
#endif

// Disable Compiler Warnings
// These are a big nuisance when compiling with external code
#if defined(TAU_COMPILER_MSVC)
    #define TAU_MSVC_SUPPRESS_WARNING_PUSH     __pragma(warning(push))
    #define TAU_MSVC_SUPPRESS_WARNING(w)       __pragma(warning(disable : w))
    #define TAU_MSVC_SUPPRESS_WARNING_POP      __pragma(warning(pop))
#else
    #define TAU_MSVC_SUPPRESS_WARNING_PUSH
    #define TAU_MSVC_SUPPRESS_WARNING(w)
    #define TAU_MSVC_SUPPRESS_WARNING_POP
#endif // TAU_COMPILER_MSVC

#if defined(TAU_COMPILER_CLANG)
    #define PRAGMA_TO_STR(x)                    _Pragma(#x)
    #define TAU_CLANG_SUPPRESS_WARNING_PUSH    _Pragma("clang diagnostic push")
    #define TAU_CLANG_SUPPRESS_WARNING_POP     _Pragma("clang diagnostic pop")
    #define TAU_CLANG_SUPPRESS_WARNING(w)      PRAGMA_TO_STR(clang diagnostic ignored w)
#else
    #define TAU_CLANG_SUPPRESS_WARNING_PUSH
    #define TAU_CLANG_SUPPRESS_WARNING(w)
    #define TAU_CLANG_SUPPRESS_WARNING_POP
#endif // TAU_COMPILER_CLANG

#if defined(TAU_COMPILER_GCC)
    #define PRAGMA_TO_STR(x)                    _Pragma(#x)
    #define TAU_GCC_SUPPRESS_WARNING_PUSH      _Pragma("GCC diagnostic push")
    #define TAU_GCC_SUPPRESS_WARNING_POP       _Pragma("GCC diagnostic pop")
    #define TAU_GCC_SUPPRESS_WARNING(w)        PRAGMA_TO_STR(GCC diagnostic ignored w)
#else
    #define TAU_GCC_SUPPRESS_WARNING_PUSH
    #define TAU_GCC_SUPPRESS_WARNING(w)
    #define TAU_GCC_SUPPRESS_WARNING_POP
#endif // TAU_COMPILER_GCC

#ifndef TAU_DISABLE_DEBUG_WARNINGS
#define TAU_DISABLE_DEBUG_WARNINGS                                                                    \
    /* MSVC Warnings */                                                                               \
    TAU_MSVC_SUPPRESS_WARNING_PUSH                                                                    \
    /* Conditional expression is constant */                                                          \
    TAU_MSVC_SUPPRESS_WARNING(4127)                                                                   \
    /* Assignment within conditional expression */                                                    \
    TAU_MSVC_SUPPRESS_WARNING(4706)                                                                   \
    /* Unreferenced inline function has been removed */                                               \
    TAU_MSVC_SUPPRESS_WARNING(4514)                                                                   \
    /* Function not inlined*/                                                                         \
    TAU_MSVC_SUPPRESS_WARNING(4710)                                                                   \
    /* function 'x' selected for automatic inline expansion */                                        \
    TAU_MSVC_SUPPRESS_WARNING(4711)                                                                   \
    /* No function prototype given: converting '()' to '(void)' */                                    \
    TAU_MSVC_SUPPRESS_WARNING(4255)                                                                   \
    /* '__cplusplus' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif' */    \
    TAU_MSVC_SUPPRESS_WARNING(4668)                                                                   \
    /* 'fprintf' : format string '' requires an argument of type 'unsigned __int64', but variadic */  \
    /* argument 1 has type 'size_t' */                                                                \
    TAU_MSVC_SUPPRESS_WARNING(4777)                                                                   \
    /* https://stackoverflow.com/questions/39479163 - what's the difference between 4018 and 4389 */  \
    /* signed/unsigned mismatch */                                                                    \
    TAU_MSVC_SUPPRESS_WARNING(4388)                                                                   \
    /* expression' : signed/unsigned mismatch */                                                      \
    TAU_MSVC_SUPPRESS_WARNING(4018)                                                                   \
    /* Conversion from 'int' to 'unsigned long', signed/unsigned mismatch */                          \
    TAU_MSVC_SUPPRESS_WARNING(4365)                                                                   \
    /* format string expected in argument 'x' is not a string literal */                              \
    TAU_MSVC_SUPPRESS_WARNING(4774)                                                                   \
    /* 'var' : conversion from 'x' to 'y', possible loss of data */                                   \
    TAU_MSVC_SUPPRESS_WARNING(4267)                                                                   \
    /* In multi-platform code, we cannot use the non-standard "safe" functions from */                \
    /* Microsoft's C lib like e.g. sprintf_s() instead of standard sprintf(). */                      \
    TAU_MSVC_SUPPRESS_WARNING(4996)                                                                   \
    /* warning C4090: '=': different 'const' qualifiers */                                            \
    TAU_MSVC_SUPPRESS_WARNING(4090)                                                                   \
    /* io.h contains definitions for some structures with natural padding. This is uninteresting, */  \
    /* but for some reason, MSVC's behaviour is to warn about including this system header. That  */  \
    /* *is* interesting */                                                                            \
    TAU_MSVC_SUPPRESS_WARNING(4820)                                                                   \
    /* Miscellaneous */                                                                               \
    TAU_MSVC_SUPPRESS_WARNING(4061)                                                                   \
    TAU_MSVC_SUPPRESS_WARNING(4100)                                                                   \
    TAU_MSVC_SUPPRESS_WARNING(4201)                                                                   \
    TAU_MSVC_SUPPRESS_WARNING(5045)                                                                   \
    TAU_MSVC_SUPPRESS_WARNING(5105)                                                                   \
                                                                                                      \
    /* Clang */                                                                                       \
    TAU_CLANG_SUPPRESS_WARNING_PUSH                                                                   \
    TAU_CLANG_SUPPRESS_WARNING("-Wdisabled-macro-expansion")                                          \
    TAU_CLANG_SUPPRESS_WARNING("-Wlanguage-extension-token")                                          \
    TAU_CLANG_SUPPRESS_WARNING("-Wc++98-compat")                                                      \
    TAU_CLANG_SUPPRESS_WARNING("-Wc++98-compat-pedantic")                                             \
    TAU_CLANG_SUPPRESS_WARNING("-Wfloat-equal")                                                       \
    TAU_CLANG_SUPPRESS_WARNING("-Wmissing-variable-declarations")                                     \
    TAU_CLANG_SUPPRESS_WARNING("-Wreserved-id-macro")                                                 \
    TAU_CLANG_SUPPRESS_WARNING("-Wunknown-pragmas")                                                   \
    TAU_CLANG_SUPPRESS_WARNING("-Wstrict-overflow")                                                   \
    TAU_CLANG_SUPPRESS_WARNING("-Wstrict-aliasing")                                                   \
    TAU_CLANG_SUPPRESS_WARNING("-Wsign-compare")                                                      \
    TAU_CLANG_SUPPRESS_WARNING("-Wsign-conversion")                                                   \
    TAU_CLANG_SUPPRESS_WARNING("-Wunused-macros")                                                     \
    TAU_CLANG_SUPPRESS_WARNING("-Wunused-function")                                                   \
    TAU_CLANG_SUPPRESS_WARNING("-Wunused-variable")                                                   \
    TAU_CLANG_SUPPRESS_WARNING("-Wunused-result")                                                     \
    TAU_CLANG_SUPPRESS_WARNING("-Wunused-parameter")                                                  \
    TAU_CLANG_SUPPRESS_WARNING("-Wunused-local-typedefs")                                             \
    TAU_CLANG_SUPPRESS_WARNING("-Wswitch")                                                            \
    TAU_CLANG_SUPPRESS_WARNING("-Wswitch-enum")                                                       \
    TAU_CLANG_SUPPRESS_WARNING("-Wswitch-default")                                                    \
    TAU_CLANG_SUPPRESS_WARNING("-Wformat")                                                            \
    TAU_CLANG_SUPPRESS_WARNING("-Wformat-extra-args")                                                 \
    TAU_CLANG_SUPPRESS_WARNING("-Wignored-qualifiers")                                                \
    TAU_CLANG_SUPPRESS_WARNING("-Wuninitialized")                                                     \
    TAU_CLANG_SUPPRESS_WARNING("-Wunused-const-variable")                                             \
    TAU_CLANG_SUPPRESS_WARNING("-Wnewline-eof")                                                       \
    TAU_CLANG_SUPPRESS_WARNING("-Wdeprecated-declarations")                                           \
                                                                                                      \
    /* GCC */                                                                                         \
    TAU_GCC_SUPPRESS_WARNING_PUSH                                                                     \
    TAU_GCC_SUPPRESS_WARNING("-Wunknown-pragmas")                                                     \
    TAU_GCC_SUPPRESS_WARNING("-Wpragmas")                                                             \
    TAU_GCC_SUPPRESS_WARNING("-Wmaybe-uninitialized")                                                 \
    TAU_GCC_SUPPRESS_WARNING("-Wmultistatement-macros")                                               \
    TAU_GCC_SUPPRESS_WARNING("-Wdiscarded-qualifiers")                                                \
    TAU_GCC_SUPPRESS_WARNING("-Wdisabled-macro-expansion")                                            \
    TAU_GCC_SUPPRESS_WARNING("-Wlanguage-extension-token")                                            \
    TAU_GCC_SUPPRESS_WARNING("-Wc++98-compat")                                                        \
    TAU_GCC_SUPPRESS_WARNING("-Wc++98-compat-pedantic")                                               \
    TAU_GCC_SUPPRESS_WARNING("-Wfloat-equal")                                                         \
    TAU_GCC_SUPPRESS_WARNING("-Wmissing-variable-declarations")                                       \
    TAU_GCC_SUPPRESS_WARNING("-Wreserved-id-macro")                                                   \
    TAU_GCC_SUPPRESS_WARNING("-Wunknown-pragmas")                                                     \
    TAU_GCC_SUPPRESS_WARNING("-Wstrict-overflow")                                                     \
    TAU_GCC_SUPPRESS_WARNING("-Wstrict-aliasing")                                                     \
    TAU_GCC_SUPPRESS_WARNING("-Wsign-compare")                                                        \
    TAU_GCC_SUPPRESS_WARNING("-Wsign-conversion")                                                     \
    TAU_GCC_SUPPRESS_WARNING("-Wunused-macros")                                                       \
    TAU_GCC_SUPPRESS_WARNING("-Wunused-function")                                                     \
    TAU_GCC_SUPPRESS_WARNING("-Wunused-variable")                                                     \
    TAU_GCC_SUPPRESS_WARNING("-Wunused-result")                                                       \
    TAU_GCC_SUPPRESS_WARNING("-Wunused-parameter")                                                    \
    TAU_GCC_SUPPRESS_WARNING("-Wunused-local-typedefs")                                               \
    TAU_GCC_SUPPRESS_WARNING("-Wswitch")                                                              \
    TAU_GCC_SUPPRESS_WARNING("-Wswitch-enum")                                                         \
    TAU_GCC_SUPPRESS_WARNING("-Wswitch-default")                                                      \
    TAU_GCC_SUPPRESS_WARNING("-Wformat")                                                              \
    TAU_GCC_SUPPRESS_WARNING("-Wformat-extra-args")                                                   \
    TAU_GCC_SUPPRESS_WARNING("-Wsuggest-attribute=format")                                            \
    TAU_GCC_SUPPRESS_WARNING("-Wdeprecated-declarations")
#endif // TAU_DISABLE_DEBUG_WARNINGS

#ifndef TAU_DISABLE_DEBUG_WARNINGS_POP
#define TAU_DISABLE_DEBUG_WARNINGS_POP     \
    TAU_MSVC_SUPPRESS_WARNING_POP          \
    TAU_CLANG_SUPPRESS_WARNING_POP         \
    TAU_GCC_SUPPRESS_WARNING_POP
#endif // TAU_DISABLE_DEBUG_WARNINGS_POP

#endif // TAU_COMPILERS_H
