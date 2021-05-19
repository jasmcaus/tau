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

#ifndef MUON_TYPES_H
#define MUON_TYPES_H

#ifdef __cplusplus
namespace Hazel {
#endif

// Base Types (similar to the Types in the Hazel Language) ==========================================
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
#endif // Hazel Basic Types 

// <windows.h> declares a typedef float FLOAT for its internal usage. 
// We need to be polite and respect that :)
// NOTE: <windows.h> is declared in Muon during Hazel's Internal Tests
// #if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
//     #define MUON_Float32     float
// #else 
//     typedef float  MUON_Float32; 
// #endif // _WIN32
typedef float  MUON_Float32; 
typedef double MUON_Float64; 

// Bool-sy
typedef MUON_Int8  MUON_Bool8; 
typedef MUON_Int16 MUON_Bool16;
typedef MUON_Int32 MUON_Bool32; // Prefer this!

// Unicode codepoint
typedef MUON_Int32 MUON_Rune; 
#define MUON_RUNE_INVALID MUON_cast(MUON_Rune)(0xfffd)
#define MUON_RUNE_MAX     MUON_cast(MUON_Rune)(0x0010ffff)
#define MUON_RUNE_BOM     MUON_cast(MUON_Rune)(0xfeff)
#define MUON_RUNE_EOF     MUON_cast(MUON_Rune)(-1)

// Max and Min 
#ifndef MUON_UInt8_MIN 
    #define MUON_UInt8_MIN 0u
    #define MUON_UInt8_MAX 0xffu
    #define MUON_Int8_MIN  (-0x7f - 1)
    #define MUON_Int8_MAX  0x7f

    #define MUON_UInt16_MIN 0u
    #define MUON_UInt16_MAX 0xffffu
    #define MUON_Int16_MIN  (-0x7fff - 1)
    #define MUON_Int16_MAX  0x7fff
 
    #define MUON_UInt32_MIN 0u
    #define MUON_UInt32_MAX 0xffffffffu
    #define MUON_Int32_MIN  (-0x7fffffff - 1)
    #define MUON_Int32_MAX  0x7fffffff

    #define MUON_UInt64_MIN 0ull
    #define MUON_UInt64_MAX 0xffffffffffffffffull
    #define MUON_Int64_MIN  (-0x7fffffffffffffffll - 1)
    #define MUON_Int64_MAX  0x7fffffffffffffffll

    #if defined(MUON_ARCH_32BIT)
        #define MUON_UINTSIZE_MIX MUON_UInt32_MIN
        #define MUON_UINTSIZE_MAX MUON_UInt32_MAX

        #define MUON_INTSIZE_MIX MUON_Int32_MIN
        #define MUON_INTSIZE_MAX MUON_Int32_MAX
    
    #elif defined(MUON_ARCH_64BIT)
        #define MUON_UINTSIZE_MIX MUON_UInt64_MIN
        #define MUON_UINTSIZE_MAX MUON_UInt64_MAX

        #define MUON_INTSIZE_MIX MUON_Int64_MIN
        #define MUON_INTSIZE_MAX MUON_Int64_MAX
    #endif 
#else
        #error Unknown architecture size. Hazel only supports 32-bit and 64-bit architectures.
#endif 

#define MUON_Float32_MIN 1.17549435e-38f
#define MUON_Float32_MAX 3.40282347e+38f

#define MUON_Float64_MIN 2.2250738585072014e-308
#define MUON_Float64_MAX 1.7976931348623157e+308

// The same thing as size_t 
typedef MUON_UInt64     MUON_Ull;
// The same thing as ptrdiff_t
typedef MUON_Int64      MUON_Ll;

// More Useful Types
#define MUON_nullchar '\0' 
 
#ifndef MUON_null 
    #ifdef __cplusplus
        #if __cplusplus >= 201103L
            #define MUON_null    MUON_nullptr 
        #else 
            #define MUON_null    0
        #endif 
    #else 
        #define MUON_null    ((void*)0)
    #endif // __cplusplus
#endif 


// MUON_bool is a basic type in C++ and not C
// We could just have used <stdMUON_bool.h> but I prefer this as it results in a smaller binary
#ifndef __cplusplus
    typedef MUON_Bool8 MUON_bool;
    static const MUON_bool MUON_false = 0;
    static const MUON_bool MUON_true  = 1;
#endif 

// (U)Intptr is only here for semantic reasons really as this library will only support 32/64 bit OSes.
// Are there any modern OSes (not 16 bit) where MUON_Intptr != ptrdiff_t/Ll ?
#if defined(_WIN64)
    typedef signed   __int64    MUON_Intptr;
    typedef unsigned __int64    MUON_UIntptr;
#elif defined(_WIN32)
    // To mark types changing their size, e.g. MUON_Intptr
    #ifndef _W64
        #if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
            #define MUON__W64 __w64
        #else
            #define MUON__W64
        #endif
    #endif

    typedef _W64 signed int     MUON_Intptr;
    typedef _W64 unsigned int   MUON_UIntptr;
#else
    typedef  uintptr_t   MUON_UIntptr;
    typedef  intptr_t    MUON_Intptr;
#endif

#ifdef __cplusplus
} // namespace Hazel
#endif

#endif // MUON_TYPES_H
