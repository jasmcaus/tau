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

// Simple assertion checks (that don't require a message to STDOUT).
// Condition failure raises a compilation error (negative index) --> 0*2 == 0 + (-1) == -1!
// This macro is intended only for this file (tau/types.h)
#define TAU_STAT_ASSERT1__(cond, line)      typedef char static_assertion_at_line_##line[(!!(cond))*2-1]
#define TAU_STATIC_ASSERT(cond)             TAU_STAT_ASSERT1__(cond, __LINE__)

// Base types
#if defined(TAU_COMPILER_MSVC)
    #if _MSVC_VER < 1300
        typedef unsigned char     tau_u8;
        typedef signed char       tau_i8;
        typedef unsigned short    tau_u16;
        typedef signed short      tau_i16;
        typedef unsigned int      tau_u32;
        typedef signed int        tau_i32;
    #else
        typedef unsigned __int8   tau_u8;
        typedef signed __int8     tau_i8;
        typedef unsigned __int16  tau_u16;
        typedef signed __int16    tau_i16;
        typedef unsigned __int32  tau_u32;
        typedef signed __int32    tau_i32;
    #endif

    typedef unsigned __int64  tau_u64;
    typedef signed __int64    tau_i64;

#else
    #ifdef __cplusplus
        #include <cstdint>
    #else
        #include <stdint.h>
    #endif // __cplusplus

    typedef uint8_t   tau_u8;
    typedef int8_t    tau_i8;
    typedef uint16_t  tau_u16;
    typedef int16_t   tau_i16;
    typedef uint32_t  tau_u32;
    typedef int32_t   tau_i32;
    typedef uint64_t  tau_u64;
    typedef int64_t   tau_i64;
#endif // Tau Basic types/

typedef float  tau_f32;
typedef double tau_f64;

// Bool-sy
typedef tau_i8  tau_bool8;
typedef tau_i16 tau_bool16;
typedef tau_i32 tau_bool32; // Prefer this!

// The same thing as size_t
// tau_ull --> size_t
// tau_ll  --> ptrdiff_t
#if defined(_MSC_VER)
    #if !defined(_WIN64)
        typedef unsigned int tau_ull;
        typedef int          tau_ll;
    #else
        typedef tau_u64  tau_ull;
        typedef tau_i64   tau_ll;
    #endif // _WIN64
#else
    typedef tau_u64  tau_ull;
    typedef tau_i64   tau_ll;
#endif // _MSC_VER

// (U)Intptr is only here for semantic reasons really as this library will only support 32/64 bit OSes.
// Are there any modern OSes (not 16 bit) where tau_iptr != ptrdiff_t/tau_ll ?
#if defined(_WIN64)
    typedef signed    __int64    tau_iptr;
    typedef unsigned  __int64    tau_uptr;
#elif defined(_WIN32)
    // To mark types changing their size, e.g. tau_iptr
    #ifndef TAU__W64
        #if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && _MSC_VER >= 1300
            #define TAU__W64 __w64
        #else
            #define TAU__W64
        #endif
    #endif

    typedef TAU__W64 signed int     tau_iptr;
    typedef TAU__W64 unsigned int   tau_uptr;
#else
    typedef  uintptr_t   tau_uptr;
    typedef  intptr_t    tau_iptr;
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
#ifndef TAU_BOOL_TYPES_DEFINED
#define TAU_BOOL_TYPES_DEFINED
    #ifdef __cplusplus
        #define tau_bool   bool
        #define tau_false  false
        #define tau_true   true
    #else
        typedef tau_bool32 tau_bool;
        static const tau_bool tau_false = 0;
        static const tau_bool tau_true = 1;
    #endif // __cplusplus
#endif // TAU_BOOL_TYPES_DEFINED

TAU_STATIC_ASSERT(sizeof(tau_u8) == 1);                 // integers
TAU_STATIC_ASSERT(sizeof(tau_i8) == 1);
TAU_STATIC_ASSERT(sizeof(tau_u16) == 2);
TAU_STATIC_ASSERT(sizeof(tau_i16) == 2);
TAU_STATIC_ASSERT(sizeof(tau_u32) == 4);
TAU_STATIC_ASSERT(sizeof(tau_i32) == 4);
TAU_STATIC_ASSERT(sizeof(tau_u64) == 8);
TAU_STATIC_ASSERT(sizeof(tau_i64) == 8);
TAU_STATIC_ASSERT(sizeof(tau_f32) == 4);                // floats
TAU_STATIC_ASSERT(sizeof(tau_f64) == 8);
TAU_STATIC_ASSERT(sizeof(tau_bool8) == 1);              // bools
TAU_STATIC_ASSERT(sizeof(tau_bool16) == 2);
TAU_STATIC_ASSERT(sizeof(tau_bool32)  == 4);
// pointer sizes (differs on architectures like CHERI)
TAU_STATIC_ASSERT(sizeof(tau_iptr) == sizeof(tau_ll));  // tau_ull --> size_t
TAU_STATIC_ASSERT(sizeof(tau_uptr) == sizeof(tau_ull)); // tau_ll  --> ptrdiff_t

#endif // TAU_TYPES_H
