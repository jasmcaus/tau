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

#ifndef MUON_MISCELLANEOUS_H
#define MUON_MISCELLANEOUS_H

#ifdef __cplusplus
namespace Hazel {
#endif


// Defines 
#ifndef MUON_DEF 
    #ifdef MUON_EXTERN
        #define MUON_DEF extern
    #else 
        #define MUON_DEF static
    #endif 
#endif // MUON_DEF


// Inline 
#ifdef __cplusplus
    #if defined(_MSC_VER) && _MSC_VER <= 1800 
        #define MUON_inline  __inline
    #elif !defined(__STDC_VERSION__)
        #define MUON_inline __inline__
    #else 
        #define MUON_inline 
    #endif 
#else
    // We default to C's inline function
    // NOTE: GCC performs inline substitution for `inline` functions. This presents a problem because declared `inline`
    // functions are not visible to the linker and hence will result in ``undefined reference to `function` `` errors
    // A quick fix is to make sure `inline` functions are declared as static. 
    // 
    // We can enforce this here, but I'll wait for sometime. If we decide to go ahead with it, a simple text substitution
    // should work :)
    #define MUON_inline  inline
#endif 


// Force Inline
#ifndef force_inline
    #if defined(_MSC_VER)
        #if _MSC_VER < 1300
            #define MUON_force_inline
        #else 
            #define MUON_force_inline   __forceinline
        #endif 
    #else 
        #define MUON_force_inline       __attribute__((__always_inline__))
    #endif 
#endif 


// No Inline 
#ifndef no_inline
    #if defined(_MSC_VER)
        #define MUON_no_inline   __declspec(noinline)
    #else 
        #define MUON_no_inline   __attribute__((noinline))
    #endif 
#endif 


// Casts
#ifdef __cplusplus
    #define MUON_cast(type, x)       static_cast<type>(x)
    #define MUON_ptrcast(type, x)    reinterpret_cast<type>(x)
#else
    #define MUON_cast(type, x)       ((type)x)
    #define MUON_ptrcast(type, x)    ((type)x)
#endif // __cplusplus


// Noexcept
#if defined(__cplusplus) && (__cplusplus >= 201103L)
    #define MUON_noexcept    noexcept
#endif // __cplusplus


// Nothrow
#if defined(__cplusplus) && defined(_MSC_VER)
    #define MUON_nothrow   __declspec(nothrow)
#else
    #define MUON_nothrow
#endif // __cplusplus


// printf Format-string specifiers for MUON_Int64 and MUON_UInt64 respectively
#ifdef __clang__
    #define MUON_PRId64     "lld"
    #define MUON_PRIu64     "llu"
#else 
    #define MUON_PRId64     "I64d"
    #define MUON_PRIu64     "I64u"
#endif  // __clang__


// A signed sizeof is more useful 
#ifndef MUON_SIZEOF
    #define MUON_SIZEOF(x)     (Ll)(sizeof(x))
#endif 


// Statics!
// static means 3-4 different things in C/C++!!
#ifndef MUON_GLOBAL
    #define MUON_GLOBAL       static // Global Variables
    #define MUON_INTERNAL     static // Internal Linkage
    #define MUON_LOCALPERSIST static // Local Persisting Variables  
#endif 

#ifdef __cplusplus
} // namespace Hazel
#endif

#endif // MUON_MISCELLANEOUS_H
