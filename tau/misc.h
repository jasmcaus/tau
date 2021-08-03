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

#ifndef TAU_MISCELLANEOUS_H
#define TAU_MISCELLANEOUS_H

// Inline 
#ifdef __cplusplus
    #if defined(_MSC_VER) && _MSC_VER <= 1800 
        #define TAU_INLINE     __inline
    #elif !defined(__STDC_VERSION__)
        #define TAU_INLINE     __inline__
    #else 
        #define TAU_INLINE 
    #endif 
#else
    // We default to C's inline function
    // NOTE: GCC performs inline substitution for `inline` functions. This presents a problem because declared `inline`
    // functions are not visible to the linker and hence will result in ``undefined reference to `function` `` errors
    // A quick fix is to make sure `inline` functions are declared as static. 
    // 
    // We can enforce this here, but I'll wait for sometime. If we decide to go ahead with it, a simple text substitution
    // should work :)
    #define TAU_INLINE  inline
#endif 


// Force Inline
#ifndef TAU_ALWAYS_INLINE
    #if defined(_MSC_VER)
        #if _MSC_VER < 1300
            #define TAU_ALWAYS_INLINE
        #else 
            #define TAU_ALWAYS_INLINE   __forceinline
        #endif 
    #elif __has_attribute(always_inline) || defined(__GNUC__)
        #define TAU_ALWAYS_INLINE       __attribute__((__always_inline__)) inline
    #else 
        #define TAU_ALWAYS_INLINE       inline
    #endif 
#endif 


// No Inline 
#ifndef TAU_NOINLINE
    #if defined(_MSC_VER)
        #define TAU_NOINLINE   __declspec(noinline)
    #else 
        #define TAU_NOINLINE   __attribute__((noinline))
    #endif 
#endif 


// Casts
#ifdef __cplusplus
    #define TAU_CAST(type, x)       static_cast<type>(x)
    #define TAU_PTRCAST(type, x)    reinterpret_cast<type>(x)
#else
    #define TAU_CAST(type, x)       ((type)x)
    #define TAU_PTRCAST(type, x)    ((type)x)
#endif // __cplusplus


// Noexcept
#if defined(__cplusplus) && (__cplusplus >= 201103L)
    #define TAU_NOEXCEPT    noexcept
#else 
    #define TAU_NOEXCEPT
#endif // __cplusplus


// Nothrow
#if defined(__cplusplus) && defined(_MSC_VER)
    #define TAU_NOTHROW   __declspec(nothrow)
#else
    #define TAU_NOTHROW
#endif // __cplusplus


#define TAU_CONCATENATE_IMPL(s1, s2)   s1##s2
#define TAU_CONCATENATE(s1, s2)        TAU_CONCATENATE_IMPL(s1, s2)

#define TAU_MACRO_EXPAND(args)         args

#define TAU_STRINGIZE_IMPL(x)          #x
#define TAU_STRINGIZE(x)               TAU_STRINGIZE_IMPL(x)

// printf format-string specifiers for TAU_Int64 and TAU_UInt64 respectively
#if defined(_MSC_VER) && (_MSC_VER < 1920)
    #define TAU_PRId64 "I64d"
    #define TAU_PRIu64 "I64u"
#else
    // Avoid spurious trailing ‘%’ in format error
	// See: https://stackoverflow.com/questions/8132399/how-to-printf-uint64-t-fails-with-spurious-trailing-in-format
	#define __STDC_FORMAT_MACROS
    #include <inttypes.h>

    #define TAU_PRId64 PRId64
    #define TAU_PRIu64 PRIu64
#endif

// A signed sizeof is more useful 
#ifndef TAU_SIZEOF
    #define TAU_SIZEOF(x)    (TAU_Ll)(sizeof(x))
#endif 

// Get the type of `val`
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
    #define TAU_TYPEOF(val)                                     \
        printf("%s\n",                                          \
            _Generic((val),                                     \
                    signed char : "signed char",                \
                    unsigned char : "unsigned char",            \
                    signed short : "signed short",              \
                    unsigned short : "unsigned short",          \
                    signed int : "signed int",                  \
                    unsigned int : "unsigned int",              \
                    signed long long : "signed long long",      \
                    unsigned long long : "unsigned long long",  \
                    float : "float",                            \
                    double : "double",                          \
                    default: "unknown type"                     \
                ))
#else
    #define TAU_TYPEOF(val)
#endif 

#endif // TAU_MISCELLANEOUS_H
