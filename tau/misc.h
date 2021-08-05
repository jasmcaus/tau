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

// Casts
#ifdef __cplusplus
    #define TAU_CAST(type, x)       static_cast<type>(x)
    #define TAU_PTRCAST(type, x)    reinterpret_cast<type>(x)
#else
    #define TAU_CAST(type, x)       ((type)x)
    #define TAU_PTRCAST(type, x)    ((type)x)
#endif // __cplusplus

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
