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

#ifndef MUON_TYPES_H
#define MUON_TYPES_H

#include <Muon/Compilers.h>

// Base Types (similar to the Types in the Muon Language) ==========================================
// typedef unsigned char       MUON_UInt8; 
// typedef          char       MUON_Int8;  
// typedef unsigned short      MUON_UInt16;
// typedef signed short        MUON_Int16; 
// typedef unsigned int        MUON_UInt32;
// typedef signed int          MUON_Int32; 
// typedef unsigned long long  MUON_UInt64;
// typedef long long           MUON_Int64; 
// typedef float               MUON_Float32; 
// typedef double              MUON_Float64; 
#if defined(MUON_COMPILER_MSVC)
    #if _MSVC_VER < 1300 
        typedef unsigned char     MUON_UInt8;
        typedef signed char       MUON_Int8;
        typedef unsigned short    MUON_UInt16;
        typedef signed short      MUON_Int16;
        typedef unsigned int      MUON_UInt32;
        typedef signed int        MUON_Int32;
    #else 
        typedef unsigned __int8   MUON_UInt8;
        typedef signed __int8     MUON_Int8;
        typedef unsigned __int16  MUON_UInt16;
        typedef signed __int16    MUON_Int16;
        typedef unsigned __int32  MUON_UInt32;
        typedef signed __int32    MUON_Int32;
    #endif 

    typedef unsigned __int64  MUON_UInt64; 
    typedef signed __int64    MUON_Int64;

#else 
    #ifdef __cplusplus
        #include <cstdint>
    #else
        #include <stdint.h>
    #endif // __cplusplus

    typedef uint8_t   MUON_UInt8; 
    typedef int8_t    MUON_Int8; 
    typedef uint16_t  MUON_UInt16; 
    typedef int16_t   MUON_Int16; 
    typedef uint32_t  MUON_UInt32; 
    typedef int32_t   MUON_Int32; 
    typedef uint64_t  MUON_UInt64; 
    typedef int64_t   MUON_Int64; 
#endif // Muon Basic Types 

typedef float  MUON_Float32; 
typedef double MUON_Float64; 

// Bool-sy
typedef MUON_Int8  MUON_Bool8; 
typedef MUON_Int16 MUON_Bool16;
typedef MUON_Int32 MUON_Bool32; // Prefer this!

// Unicode codepoint
typedef MUON_Int32 MUON_Rune; 
// The same thing as size_t 
typedef MUON_UInt64     MUON_Ull;
// The same thing as ptrdiff_t
typedef MUON_Int64      MUON_Ll;

// More Useful Types
#define MUON_NULLCHAR '\0' 
 
#ifndef MUON_NULL 
    #ifdef __cplusplus
        #if __cplusplus >= 201103L
            #define MUON_NULL    nullptr 
        #else 
            #define MUON_NULL    0
        #endif 
    #else 
        #define MUON_NULL    ((void*)0)
    #endif // __cplusplus
#endif 


// MUON_bool is a basic type in C++ and not C
// We could just have used <stdbool.h> but I prefer this as it results in a smaller binary
#ifdef __cplusplus
    #define MUON_bool   bool
    #define MUON_false  false
    #define MUON_true   true
#else
    typedef MUON_Bool8 MUON_bool;;
    static const MUON_bool MUON_false = 0;
    static const MUON_bool MUON_true  = 1;
#endif 

// (U)Intptr is only here for semantic reasons really as this library will only support 32/64 bit OSes.
// Are there any modern OSes (not 16 bit) where MUON_Intptr != ptrdiff_t/Ll ?
#if defined(_WIN64)
    typedef signed    __int64    MUON_Intptr;
    typedef unsigned  __int64    MUON_UIntptr;

#elif defined(_WIN32)
    // To mark types changing their size, e.g. MUON_Intptr
    #ifndef MUON__W64
        #if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
            #define MUON__W64 __w64
        #else
            #define MUON__W64
        #endif
    #endif

    typedef MUON__W64 signed int     MUON_Intptr;
    typedef MUON__W64 unsigned int   MUON_UIntptr;
#else
    typedef  uintptr_t   MUON_UIntptr;
    typedef  intptr_t    MUON_Intptr;
#endif

#endif // MUON_TYPES_H