/*
    /\    |  __ \|  __ \ / __ \| |  | |
   /  \   | |__) | |__) | |  | | |  | |
  / /\ \  |  _  /|  _  /| |  | | |/\| |
 / ____ \ | | \ \| | \ \| |__| | /  \ |
/_/    \_\|_|__\_|_|__\__\____/|_ /\ _|


Licensed under the MIT License <http://opensource.org/licenses/MIT>
SPDX-License-Identifier: MIT
Copyright (c) 2021 Jason Dsouza <http://github.com/jasmcaus>
*/

#ifndef ARROW_TYPES_H
#define ARROW_TYPES_H

// Base Types (similar to the Types in the Hazel Language) ==========================================
#if defined(ARROW_COMPILER_MSVC)
    #if _MSVC_VER < 1300 
        typedef unsigned char     UInt8;
        typedef signed char       Int8;
        typedef unsigned short    UInt16;
        typedef signed short      Int16;
        typedef unsigned int      UInt32;
        typedef signed int        Int32;
    #else 
        typedef unsigned __int8   UInt8;
        typedef signed __int8     Int8;
        typedef unsigned __int16  UInt16;
        typedef signed __int16    Int16;
        typedef unsigned __int32  UInt32;
        typedef signed __int32    Int32;
    #endif 

    typedef unsigned __int64  UInt64; 
    typedef signed __int64    Int64;

#else 
    #ifdef __cplusplus
        #include <cstdint>
    #else
        #include <stdint.h>
    #endif // __cplusplus

    typedef uint8_t   UInt8; 
    typedef int8_t    Int8; 
    typedef uint16_t  UInt16; 
    typedef int16_t   Int16; 
    typedef uint32_t  UInt32; 
    typedef int32_t   Int32; 
    typedef uint64_t  UInt64; 
    typedef int64_t   Int64; 
#endif // Hazel Basic Types 

typedef float  Float32; 
typedef double Float64; 

// Bool-sy
typedef Int8  Bool8; 
typedef Int16 Bool16;
typedef Int32 Bool32; // Prefer this!

// Unicode codepoint
typedef Int32 Rune; 
#define ARROW_RUNE_INVALID cast(Rune)(0xfffd)
#define ARROW_RUNE_MAX     cast(Rune)(0x0010ffff)
#define ARROW_RUNE_BOM     cast(Rune)(0xfeff)
#define ARROW_RUNE_EOF     cast(Rune)(-1)

// Max and Min 
#ifndef UInt8_MIN 
    #define UInt8_MIN 0u
    #define UInt8_MAX 0xffu
    #define Int8_MIN  (-0x7f - 1)
    #define Int8_MAX  0x7f

    #define UInt16_MIN 0u
    #define UInt16_MAX 0xffffu
    #define Int16_MIN  (-0x7fff - 1)
    #define Int16_MAX  0x7fff
 
    #define UInt32_MIN 0u
    #define UInt32_MAX 0xffffffffu
    #define Int32_MIN  (-0x7fffffff - 1)
    #define Int32_MAX  0x7fffffff

    #define UInt64_MIN 0ull
    #define UInt64_MAX 0xffffffffffffffffull
    #define Int64_MIN  (-0x7fffffffffffffffll - 1)
    #define Int64_MAX  0x7fffffffffffffffll

    #if defined(GB_ARCH_32_BIT)
        #define UINTSIZE_MIX UInt32_MIN
        #define UINTSIZE_MAX UInt32_MAX

        #define INTSIZE_MIX Int32_MIN
        #define INTSIZE_MAX Int32_MAX
    
    #elif defined(GB_ARCH_64_BIT)
        #define UINTSIZE_MIX UInt64_MIN
        #define UINTSIZE_MAX UInt64_MAX

        #define INTSIZE_MIX Int64_MIN
        #define INTSIZE_MAX Int64_MAX
    #endif 
#else
        #error Unknown architecture size. Hazel only supports 32-bit and 64-bit architectures.
#endif 

#define Float32_MIN 1.17549435e-38f
#define Float32_MAX 3.40282347e+38f

#define Float64_MIN 2.2250738585072014e-308
#define Float64_MAX 1.7976931348623157e+308


// The same thing as size_t 
#ifndef _Ull_DEFINED
    #define _Ull_DEFINED
    #undef Ull

    #ifdef _WIN64
        typedef unsigned __int64 Ull;
    #else
        typedef unsigned int Ull;
    #endif //_WIN64
#endif

// The same thing as ptrdiff_t
#ifndef _Ll_DEFINED
    #define _Ll_DEFINED
    #undef Ll

    #ifdef _WIN64
        typedef __int64 Ll;
    #else
        typedef int Ll;
    #endif //_WIN64
#endif


// More Useful Types
#define nullchar '\0' 
 
#ifndef null 
    #ifdef __cplusplus
        #if __cplusplus >= 201103L
            #define null    nullptr 
        #else 
            #define null    0
        #endif 
    #else 
        #define null    (void*)0
    #endif // __cplusplus
#endif 


// bool is a basic type in C++ and not C
// We could just have used <stdbool.h> but I prefer this as it results in a smaller binary
#ifndef __cplusplus
    typedef Bool8 bool;
    static const bool false = 0;
    static const bool true  = 1;
#endif 

// #ifndef __cplusplus
//     #define bool   _Bool
//     #define true   1
//     #define false  0
// #else
    /* Supporting _Bool in C++ is a GCC extension.  */
    // #define _Bool	bool
// #endif // __cplusplus 


// (U)Intptr is only here for semantic reasons really as this library will only support 32/64 bit OSes.
// Are there any modern OSes (not 16 bit) where Intptr != ptrdiff_t/Ll ?
#if defined(_WIN64)
    typedef signed   __int64    Intptr;
    typedef unsigned __int64    UIntptr;
#elif defined(_WIN32)
    // To mark types changing their size, e.g. Intptr
    #ifndef _W64
        #if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
            #define _W64 __w64
        #else
            #define _W64
        #endif
    #endif

    typedef _W64 signed int     Intptr;
    typedef _W64 unsigned int   UIntptr;
#else
    typedef  uintptr_t   UIntptr;
    typedef  intptr_t    Intptr;
#endif

#endif // ARROW_TYPES_H