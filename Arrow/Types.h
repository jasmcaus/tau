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

#ifdef __cplusplus
namespace Hazel {
#endif

// Base Types (similar to the Types in the Hazel Language) ==========================================
#if defined(ARROW_COMPILER_MSVC)
    #if _MSVC_VER < 1300 
        typedef unsigned char     ARROW_UInt8;
        typedef signed char       ARROW_Int8;
        typedef unsigned short    ARROW_UInt16;
        typedef signed short      ARROW_Int16;
        typedef unsigned int      ARROW_UInt32;
        typedef signed int        ARROW_Int32;
    #else 
        typedef unsigned __int8   ARROW_UInt8;
        typedef signed __int8     ARROW_Int8;
        typedef unsigned __int16  ARROW_UInt16;
        typedef signed __int16    ARROW_Int16;
        typedef unsigned __int32  ARROW_UInt32;
        typedef signed __int32    ARROW_Int32;
    #endif 

    typedef unsigned __int64  ARROW_UInt64; 
    typedef signed __int64    ARROW_Int64;

#else 
    #ifdef __cplusplus
        #include <cstdint>
    #else
        #include <stdint.h>
    #endif // __cplusplus

    typedef uint8_t   ARROW_UInt8; 
    typedef int8_t    ARROW_Int8; 
    typedef uint16_t  ARROW_UInt16; 
    typedef int16_t   ARROW_Int16; 
    typedef uint32_t  ARROW_UInt32; 
    typedef int32_t   ARROW_Int32; 
    typedef uint64_t  ARROW_UInt64; 
    typedef int64_t   ARROW_Int64; 
#endif // Hazel Basic Types 

// <windows.h> declares a typedef float FLOAT for its internal usage. 
// We need to be polite and respect that :)
// NOTE: <windows.h> is declared in Arrow during Hazel's Internal Tests
// #if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
//     #define ARROW_Float32     float
// #else 
//     typedef float  ARROW_Float32; 
// #endif // _WIN32
typedef float  ARROW_Float32; 
typedef double ARROW_Float64; 

// Bool-sy
typedef ARROW_Int8  ARROW_Bool8; 
typedef ARROW_Int16 ARROW_Bool16;
typedef ARROW_Int32 ARROW_Bool32; // Prefer this!

// Unicode codepoint
typedef ARROW_Int32 ARROW_Rune; 
#define ARROW_RUNE_INVALID ARROW_cast(ARROW_Rune)(0xfffd)
#define ARROW_RUNE_MAX     ARROW_cast(ARROW_Rune)(0x0010ffff)
#define ARROW_RUNE_BOM     ARROW_cast(ARROW_Rune)(0xfeff)
#define ARROW_RUNE_EOF     ARROW_cast(ARROW_Rune)(-1)

// Max and Min 
#ifndef ARROW_UInt8_MIN 
    #define ARROW_UInt8_MIN 0u
    #define ARROW_UInt8_MAX 0xffu
    #define ARROW_Int8_MIN  (-0x7f - 1)
    #define ARROW_Int8_MAX  0x7f

    #define ARROW_UInt16_MIN 0u
    #define ARROW_UInt16_MAX 0xffffu
    #define ARROW_Int16_MIN  (-0x7fff - 1)
    #define ARROW_Int16_MAX  0x7fff
 
    #define ARROW_UInt32_MIN 0u
    #define ARROW_UInt32_MAX 0xffffffffu
    #define ARROW_Int32_MIN  (-0x7fffffff - 1)
    #define ARROW_Int32_MAX  0x7fffffff

    #define ARROW_UInt64_MIN 0ull
    #define ARROW_UInt64_MAX 0xffffffffffffffffull
    #define ARROW_Int64_MIN  (-0x7fffffffffffffffll - 1)
    #define ARROW_Int64_MAX  0x7fffffffffffffffll

    #if defined(GB_ARCH_32_BIT)
        #define ARROW_UINTSIZE_MIX ARROW_UInt32_MIN
        #define ARROW_UINTSIZE_MAX ARROW_UInt32_MAX

        #define ARROW_INTSIZE_MIX ARROW_Int32_MIN
        #define ARROW_INTSIZE_MAX ARROW_Int32_MAX
    
    #elif defined(GB_ARCH_64_BIT)
        #define ARROW_UINTSIZE_MIX ARROW_UInt64_MIN
        #define ARROW_UINTSIZE_MAX ARROW_UInt64_MAX

        #define ARROW_INTSIZE_MIX ARROW_Int64_MIN
        #define ARROW_INTSIZE_MAX ARROW_Int64_MAX
    #endif 
#else
        #error Unknown architecture size. Hazel only supports 32-bit and 64-bit architectures.
#endif 

#define ARROW_Float32_MIN 1.17549435e-38f
#define ARROW_Float32_MAX 3.40282347e+38f

#define ARROW_Float64_MIN 2.2250738585072014e-308
#define ARROW_Float64_MAX 1.7976931348623157e+308

// The same thing as size_t 
typedef ARROW_UInt64     ARROW_Ull;
// The same thing as ptrdiff_t
typedef ARROW_Int64      ARROW_Ll;

// More Useful Types
#define ARROW_nullchar '\0' 
 
#ifndef ARROW_null 
    #ifdef __cplusplus
        #if __cplusplus >= 201103L
            #define ARROW_null    ARROW_nullptr 
        #else 
            #define ARROW_null    0
        #endif 
    #else 
        #define ARROW_null    ((void*)0)
    #endif // __cplusplus
#endif 


// ARROW_bool is a basic type in C++ and not C
// We could just have used <stdARROW_bool.h> but I prefer this as it results in a smaller binary
#ifndef __cplusplus
    typedef ARROW_Bool8 ARROW_bool;
    static const ARROW_bool ARROW_false = 0;
    static const ARROW_bool ARROW_true  = 1;
#endif 

// (U)Intptr is only here for semantic reasons really as this library will only support 32/64 bit OSes.
// Are there any modern OSes (not 16 bit) where ARROW_Intptr != ptrdiff_t/Ll ?
#if defined(_WIN64)
    typedef signed   __int64    ARROW_Intptr;
    typedef unsigned __int64    ARROW_UIntptr;
#elif defined(_WIN32)
    // To mark types changing their size, e.g. ARROW_Intptr
    #ifndef _W64
        #if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
            #define ARROW__W64 __w64
        #else
            #define ARROW__W64
        #endif
    #endif

    typedef _W64 signed int     ARROW_Intptr;
    typedef _W64 unsigned int   ARROW_UIntptr;
#else
    typedef  uintptr_t   ARROW_UIntptr;
    typedef  intptr_t    ARROW_Intptr;
#endif

#ifdef __cplusplus
} // namespace Hazel
#endif

#endif // ARROW_TYPES_H
