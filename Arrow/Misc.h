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

#ifndef ARROW_MISCELLANEOUS_H
#define ARROW_MISCELLANEOUS_H

#ifdef __cplusplus
namespace Hazel {
#endif


// Defines 
#ifndef ARROW_DEF 
    #ifdef ARROW_EXTERN
        #define ARROW_DEF extern
    #else 
        #define ARROW_DEF static
    #endif 
#endif // ARROW_DEF


// Inline 
#ifdef __cplusplus
    #if defined(_MSC_VER) && _MSC_VER <= 1800 
        #define ARROW_inline  __inline
    #elif !defined(__STDC_VERSION__)
        #define ARROW_inline __inline__
    #else 
        #define ARROW_inline 
    #endif 
#else
    // We default to C's inline function
    // NOTE: GCC performs inline substitution for `inline` functions. This presents a problem because declared `inline`
    // functions are not visible to the linker and hence will result in ``undefined reference to `function` `` errors
    // A quick fix is to make sure `inline` functions are declared as static. 
    // 
    // We can enforce this here, but I'll wait for sometime. If we decide to go ahead with it, a simple text substitution
    // should work :)
    #define ARROW_inline  inline
#endif 


// Force Inline
#ifndef force_inline
    #if defined(_MSC_VER)
        #if _MSC_VER < 1300
            #define ARROW_force_inline
        #else 
            #define ARROW_force_inline   __forceinline
        #endif 
    #else 
        #define ARROW_force_inline       __attribute__((__always_inline__))
    #endif 
#endif 


// No Inline 
#ifndef no_inline
    #if defined(_MSC_VER)
        #define ARROW_no_inline   __declspec(noinline)
    #else 
        #define ARROW_no_inline   __attribute__((noinline))
    #endif 
#endif 


// Casts
#ifdef __cplusplus
    #define ARROW_cast(type, x)       static_cast<type>(x)
    #define ARROW_ptrcast(type, x)    reinterpret_cast<type>(x)
#else
    #define ARROW_cast(type, x)       ((type)x)
    #define ARROW_ptrcast(type, x)    ((type)x)
#endif // __cplusplus


// Noexcept
#if defined(__cplusplus) && (__cplusplus >= 201103L)
    #define ARROW_noexcept    noexcept
#endif // __cplusplus


// Nothrow
#if defined(__cplusplus) && defined(_MSC_VER)
    #define ARROW_nothrow   __declspec(nothrow)
#else
    #define ARROW_nothrow
#endif // __cplusplus


// printf Format-string specifiers for ARROW_Int64 and ARROW_UInt64 respectively
#define ARROW_PRId64     "I64d"
#define ARROW_PRIu64     "I64u"


// A signed sizeof is more useful 
#ifndef ARROW_SIZEOF
    #define ARROW_SIZEOF(x)     (Ll)(sizeof(x))
#endif 


// Statics!
// static means 3-4 different things in C/C++!!
#ifndef ARROW_GLOBAL
    #define ARROW_GLOBAL       static // Global Variables
    #define ARROW_INTERNAL     static // Internal Linkage
    #define ARROW_LOCALPERSIST static // Local Persisting Variables  
#endif 

#ifdef __cplusplus
} // namespace Hazel
#endif

#endif // ARROW_MISCELLANEOUS_H
