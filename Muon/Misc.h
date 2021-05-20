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

#ifndef MUON_MISCELLANEOUS_H
#define MUON_MISCELLANEOUS_H


// Inline 
#ifdef __cplusplus
    #if defined(_MSC_VER) && _MSC_VER <= 1800 
        #define MUON_INLINE     __inline
    #elif !defined(__STDC_VERSION__)
        #define MUON_INLINE     __inline__
    #else 
        #define MUON_INLINE 
    #endif 
#else
    // We default to C's inline function
    // NOTE: GCC performs inline substitution for `inline` functions. This presents a problem because declared `inline`
    // functions are not visible to the linker and hence will result in ``undefined reference to `function` `` errors
    // A quick fix is to make sure `inline` functions are declared as static. 
    // 
    // We can enforce this here, but I'll wait for sometime. If we decide to go ahead with it, a simple text substitution
    // should work :)
    #define MUON_INLINE  inline
#endif 


// Force Inline
#ifndef MUON_ALWAYS_INLINE
    #if defined(_MSC_VER)
        #if _MSC_VER < 1300
            #define MUON_ALWAYS_INLINE
        #else 
            #define MUON_ALWAYS_INLINE   __forceinline
        #endif 
    #elif __has_attribute(always_inline) || defined(__GNUC__)
        #define MUON_ALWAYS_INLINE       __attribute__((__always_inline__)) inline
    #else 
        #define MUON_ALWAYS_INLINE       inline
    #endif 
#endif 


// No Inline 
#ifndef MUON_NOINLINE
    #if defined(_MSC_VER)
        #define MUON_NOINLINE   __declspec(noinline)
    #else 
        #define MUON_NOINLINE   __attribute__((noinline))
    #endif 
#endif 


// Casts
#ifdef __cplusplus
    #define MUON_CAST(type, x)       static_cast<type>(x)
    #define MUON_PTRCAST(type, x)    reinterpret_cast<type>(x)
#else
    #define MUON_CAST(type, x)       ((type)x)
    #define MUON_PTRCAST(type, x)    ((type)x)
#endif // __cplusplus


// Noexcept
#if defined(__cplusplus) && (__cplusplus >= 201103L)
    #define MUON_NOEXCEPT    noexcept
#else 
    #define MUON_NOEXCEPT
#endif // __cplusplus


// Nothrow
#if defined(__cplusplus) && defined(_MSC_VER)
    #define MUON_NOTHROW   __declspec(nothrow)
#else
    #define MUON_NOTHROW
#endif // __cplusplus


#define MUON_CONCATENATE_IMPL(s1, s2)   s1##s2
#define MUON_CONCATENATE(s1, s2)        MUON_CONCATENATE_IMPL(s1, s2)

#define MUON_MACRO_EXPAND(args)         args

#define MUON_STRINGIZE_IMPL(x)          #x
#define MUON_STRINGIZE(x)               MUON_STRINGIZE_IMPL(x)


// printf format-string specifiers for MUON_Int64 and MUON_UInt64 respectively
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


#ifndef MUON_STATIC
    #define MUON_STATIC     static
#endif

#ifndef MUON_GLOBAL
    #define MUON_GLOBAL       static // Global Variables
    #define MUON_INTERNAL     static // Internal Linkage
    #define MUON_LOCALPERSIST static // Local Persisting Variables  
#endif 

#endif // MUON_MISCELLANEOUS_H
