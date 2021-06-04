/*
 _______           _    _
|__   __|   /\    | |  | |
   | |     /  \   | |  | |  Tau - The Micro Testing Framework for C/C++
   | |    / /\ \  | |  | |  Language: C
   | |   / ____ \ | |__| |  https://github.com/jasmcaus/tau
   |_|__/_/    \_\ \____/ 

Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

#ifndef TAU_COMPILERS_H
#define TAU_COMPILERS_H

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

#endif // TAU_COMPILERS_H
