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

#ifndef TAU_TYPES_H
#define TAU_TYPES_H

#include <tau/compilers.h>

// Base types
#if defined(TAU_COMPILER_MSVC)
    #if _MSVC_VER < 1300 
        typedef unsigned char     TAU_UInt8;
        typedef signed char       TAU_Int8;
        typedef unsigned short    TAU_UInt16;
        typedef signed short      TAU_Int16;
        typedef unsigned int      TAU_UInt32;
        typedef signed int        TAU_Int32;
    #else 
        typedef unsigned __int8   TAU_UInt8;
        typedef signed __int8     TAU_Int8;
        typedef unsigned __int16  TAU_UInt16;
        typedef signed __int16    TAU_Int16;
        typedef unsigned __int32  TAU_UInt32;
        typedef signed __int32    TAU_Int32;
    #endif 

    typedef unsigned __int64  TAU_UInt64; 
    typedef signed __int64    TAU_Int64;

#else 
    #ifdef __cplusplus
        #include <cstdint>
    #else
        #include <stdint.h>
    #endif // __cplusplus

    typedef uint8_t   TAU_UInt8; 
    typedef int8_t    TAU_Int8; 
    typedef uint16_t  TAU_UInt16; 
    typedef int16_t   TAU_Int16; 
    typedef uint32_t  TAU_UInt32; 
    typedef int32_t   TAU_Int32; 
    typedef uint64_t  TAU_UInt64; 
    typedef int64_t   TAU_Int64; 
#endif // Tau Basic types/ 

typedef float  TAU_Float32; 
typedef double TAU_Float64; 

// Bool-sy
typedef TAU_Int8  TAU_Bool8; 
typedef TAU_Int16 TAU_Bool16;
typedef TAU_Int32 TAU_Bool32; // Prefer this!

// The same thing as size_t 
// TAU_Ull --> size_t
// TAU_Ll  --> ptrdiff_t
#if defined(_MSC_VER)
    #if !defined(_WIN64)
        typedef unsigned int TAU_Ull;
        typedef int          TAU_Ll;
    #else 
        typedef TAU_UInt64  TAU_Ull;
        typedef TAU_Int64   TAU_Ll;
    #endif // _WIN64
#else
    typedef TAU_UInt64  TAU_Ull;
    typedef TAU_Int64   TAU_Ll;
#endif // _MSC_VER

// (U)Intptr is only here for semantic reasons really as this library will only support 32/64 bit OSes.
// Are there any modern OSes (not 16 bit) where TAU_Intptr != ptrdiff_t/TAU_Ll ?
#if defined(_WIN64)
    typedef signed    __int64    TAU_Intptr;
    typedef unsigned  __int64    TAU_UIntptr;
#elif defined(_WIN32)
    // To mark types changing their size, e.g. TAU_Intptr
    #ifndef TAU__W64
        #if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
            #define TAU__W64 __w64
        #else
            #define TAU__W64
        #endif 
    #endif

    typedef TAU__W64 signed int     TAU_Intptr;
    typedef TAU__W64 unsigned int   TAU_UIntptr;
#else
    typedef  uintptr_t   TAU_UIntptr;
    typedef  intptr_t    TAU_Intptr;
#endif

// More Useful types/
#define TAU_NULLCHAR '\0' 
 
#ifndef TAU_NULL 
    #ifdef __cplusplus
        #if __cplusplus >= 201103L
            #define TAU_NULL    nullptr 
        #else 
            #define TAU_NULL    0
        #endif 
    #else 
        #define TAU_NULL    ((void*)0)
    #endif // __cplusplus
#endif 

// bool is a basic type in C++ and not C
#ifndef TAU_Bool_types_defined
#define TAU_Bool_types_defined
    #ifdef __cplusplus
        #define TAU_Bool   bool
        #define TAU_false  false
        #define TAU_true   true
    #else
        typedef TAU_Bool32 TAU_Bool;
        static const TAU_Bool TAU_false = 0;
        static const TAU_Bool TAU_true = 1;
    #endif // __cplusplus   
#endif // TAU_Bool_types_defined

#endif // TAU_TYPES_H