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


// Compilers ==========================================
// MUON_COMPILER_...
#if defined(_MSC_VER)
    #define MUON_COMPILER_MSVC 1
#elif defined(__GNUC__)
    #define MUON_COMPILER_GCC 1
#elif defined(__clang__)
    #define MUON_COMPILER_CLANG 1
#else 
    #error Unknown Compiler (Muon currently supports only MSVC, GCC and Clang)
#endif 

#endif // MUON_COMPILERS_H