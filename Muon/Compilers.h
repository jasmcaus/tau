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


#ifndef MUON_DISABLE_WARNINGS
#define MUON_DISABLE_WARNINGS
    #ifdef MUON_COMPILER_MSVC
        // Conditional expression is constant
        #pragma warning(disable : 4127)

        // Assignment within conditional expression
        #pragma warning(disable : 4706)

        // Unreferenced inline function has been removed
        #pragma warning(disable : 4514)
        
        // Function not inlined
        #pragma warning(disable : 4710)
        
        // function 'x' selected for automatic inline expansion
        #pragma warning(disable : 4711)

        // No function prototype given: converting '()' to '(void)'
        #pragma warning(disable : 4255)

        // '__cplusplus' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
        #pragma warning(disable : 4668)

        // 'fprintf' : format string '' requires an argument of type 'unsigned __int64', but variadic argument 1 
        // has type 'size_t'
        #pragma warning(disable : 4777)

        // https://stackoverflow.com/questions/39479163 what's the difference between 4018 and 4389
        // signed/unsigned mismatch
        #pragma warning(disable : 4388)

        // expression' : signed/unsigned mismatch
        #pragma warning(disable : 4018)

        // Conversion from 'int' to 'unsigned long', signed/unsigned mismatch
        #pragma warning(disable : 4365)

        // format string expected in argument 'x' is not a string literal
        #pragma warning(disable : 4774)

        // 'var' : conversion from 'x' to 'y', possible loss of data
        #pragma warning(disable : 4267)

        // In multi-platform code like ours, we cannot use the non-standard "safe" functions from 
        // Microsoft's C lib like e.g. sprintf_s() instead of standard sprintf().
        #pragma warning(disable: 4996)

        // warning C4090: '=': different 'const' qualifiers
        #pragma warning(disable : 4090)

        // io.h contains definitions for some structures with natural padding. This is uninteresting, but for 
        // some reason, MSVC's behaviour is to warn about including this system header. That *is* interesting
        #pragma warning(disable : 4820)

        // Miscellaneous
        #pragma warning(disble : 4061)
        #pragma warning(disble : 4100)
        #pragma warning(disble : 4201)
        #pragma warning(disble : 5045)
        #pragma warning(disble : 5105)

        #pragma warning(push, 1)
    #endif // MUON_COMPILER_MSVC

    #ifdef MUON_COMPILER_CLANG
        _Pragma("clang diagnostic push")                                             
        _Pragma("clang diagnostic ignored "-Wdisabled-macro-expansion"") 
        _Pragma("clang diagnostic ignored "-Wlanguage-extension-token"")     
        _Pragma("clang diagnostic ignored "-Wc++98-compat"")
        _Pragma("clang diagnostic ignored "-Wc++98-compat-pedantic"")
        _Pragma("clang diagnostic ignored "-Wfloat-equal"")  
        _Pragma("clang diagnostic ignored "-Wmissing-variable-declarations"")
        _Pragma("clang diagnostic ignored "-Wreserved-id-macro"")
        _Pragma("clang diagnostic ignored "-Wunknown-pragmas"")
        _Pragma("clang diagnostic ignored "-Wstrict-overflow"")
        _Pragma("clang diagnostic ignored "-Wstrict-aliasing"")
        _Pragma("clang diagnostic ignored "-Wsign-compare"")
        _Pragma("clang diagnostic ignored "-Wsign-conversion"")
        _Pragma("clang diagnostic ignored "-Wunused-macros"")
        _Pragma("clang diagnostic ignored "-Wunused-function"")
        _Pragma("clang diagnostic ignored "-Wunused-variable"")
        _Pragma("clang diagnostic ignored "-Wunused-result"")
        _Pragma("clang diagnostic ignored "-Wunused-parameter"")
        _Pragma("clang diagnostic ignored "-Wunused-local-typedefs"")
        _Pragma("clang diagnostic ignored "-Wswitch"")
        _Pragma("clang diagnostic ignored "-Wswitch-enum"")
        _Pragma("clang diagnostic ignored "-Wswitch-default"")
        _Pragma("clang diagnostic ignored "-Wformat"")
        _Pragma("clang diagnostic ignored "-Wformat-extra-args"")
        _Pragma("clang diagnostic ignored "-Wignored-qualifiers"")
        _Pragma("clang diagnostic ignored "-Wuninitialized"")
        _Pragma("clang diagnostic ignored "-Wunused-const-variable"")
        _Pragma("clang diagnostic ignored "-Wnewline-eof"")
        _Pragma("clang diagnostic ignored "-Wdeprecated-declarations"")
    #endif // MUON_COMPILER_CLANG

    #if defined(MUON_COMPILER_GCC)
        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wunknown-pragmas"
        #pragma GCC diagnostic ignored "-Wpragmas"
        #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
        #pragma GCC diagnostic ignored "-Wmultistatement-macros"
        #pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
        #pragma GCC diagnostic ignored "-Wdisabled-macro-expansion" 
        #pragma GCC diagnostic ignored "-Wlanguage-extension-token"     
        #pragma GCC diagnostic ignored "-Wc++98-compat"
        #pragma GCC diagnostic ignored "-Wc++98-compat-pedantic"
        #pragma GCC diagnostic ignored "-Wfloat-equal"  
        #pragma GCC diagnostic ignored "-Wmissing-variable-declarations"
        #pragma GCC diagnostic ignored "-Wreserved-id-macro"
        #pragma GCC diagnostic ignored "-Wunknown-pragmas"
        #pragma GCC diagnostic ignored "-Wstrict-overflow"
        #pragma GCC diagnostic ignored "-Wstrict-aliasing"
        #pragma GCC diagnostic ignored "-Wsign-compare"
        #pragma GCC diagnostic ignored "-Wsign-conversion"
        #pragma GCC diagnostic ignored "-Wunused-macros"
        #pragma GCC diagnostic ignored "-Wunused-function"
        #pragma GCC diagnostic ignored "-Wunused-variable"
        #pragma GCC diagnostic ignored "-Wunused-result"
        #pragma GCC diagnostic ignored "-Wunused-parameter"
        #pragma GCC diagnostic ignored "-Wunused-local-typedefs"
        #pragma GCC diagnostic ignored "-Wswitch"
        #pragma GCC diagnostic ignored "-Wswitch-enum"
        #pragma GCC diagnostic ignored "-Wswitch-default"
        #pragma GCC diagnostic ignored "-Wformat"
        #pragma GCC diagnostic ignored "-Wformat-extra-args"
        #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    #endif // MUON_COMPILER_GCC
#endif // MUON_DISABLE_WARNINGS

#endif // MUON_COMPILERS_H

