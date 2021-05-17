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
        #define inline  __inline
    #elif !defined(__STDC_VERSION__)
        #define inline __inline__
    #else 
        #define inline 
    #endif 
#else
    // We default to C's inline function
    // NOTE: GCC performs inline substitution for `inline` functions. This presents a problem because declared `inline`
    // functions are not visible to the linker and hence will result in ``undefined reference to `function` `` errors
    // A quick fix is to make sure `inline` functions are declared as static. 
    // 
    // We can enforce this here, but I'll wait for sometime. If we decide to go ahead with it, a simple text substitution
    // should work :)
    #define inline  inline
#endif 


// Force Inline
#ifndef force_inline
    #if defined(_MSC_VER)
        #if _MSC_VER < 1300
            #define force_inline
        #else 
            #define force_inline   __forceinline
        #endif 
    #else 
        #define force_inline       __attribute__((__always_inline__))
    #endif 
#endif 


// No Inline 
#ifndef no_inline
    #if defined(_MSC_VER)
        #define no_inline   __declspec(noinline)
    #else 
        #define no_inline   __attribute__((noinline))
    #endif 
#endif 


// Casts
#ifdef __cplusplus
    #define cast(type, x)       static_cast<type>(x)
    #define ptrcast(type, x)    reinterpret_cast<type>(x)
#else
    #define cast(type, x)       ((type)x)
    #define ptrcast(type, x)    ((type)x)
#endif // __cplusplus


// Noexcept
#if defined(__cplusplus) && (__cplusplus >= 201103L)
    #define noexcept    noexcept
#endif // __cplusplus


// Nothrow
#if defined(__cplusplus) && defined(_MSC_VER)
    #define nothrow   __declspec(nothrow)
#else
    #define nothrow
#endif // __cplusplus


// printf Format-string specifiers for Int64 and UInt64 respectively
#if defined(_MSC_VER) && (_MSC_VER < 1920)
    #define ARROW_PRId64     "I64d"
    #define ARROW_PRIu64     "I64u"
#endif // _MSC_VER


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


// Execute power operations
// long power(long x, long y) {
//     int total; 
    
//     if(y == 0) return 1; 
//     else if(y == 1) return x; 
//     else if(y == -1) return 1/x; 
//     else if(y > 0) {
//         total = x; 
//         total *= power(x, y-1); 
//     } 
//     else {
//         total = 1/x; 
//         total *= power(x, y+1); 
//     }

//     return total; 
// }


#ifdef __cplusplus
} // namespace Hazel
#endif

#endif // ARROW_MISCELLANEOUS_H
