/**
 * @file compiler_port.h
 * @brief Compiler specific definitions
 *
 * @section License
 *
 * Copyright (C) 2010-2021 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Ultimate.
 *
 * This software is provided under a commercial license. You may
 * use this software under the conditions stated in the license
 * terms. This source code cannot be redistributed.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 2.1.2
 **/

#ifndef _COMPILER_PORT_H
#define _COMPILER_PORT_H

//Dependencies
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>

//C++ guard
#ifdef __cplusplus
extern "C" {
#endif

//Types
typedef char char_t;
typedef signed int int_t;
typedef unsigned int uint_t;
typedef uint32_t systime_t;

#if !defined(R_TYPEDEFS_H) && !defined(USE_CHIBIOS_2)
   typedef int bool_t;
#endif

#if defined(__linux__)
   #define PRIuSIZE "zu"
   #define PRIXSIZE "zX"
   #define PRIuTIME "lu"
#elif defined(_WIN32)
   #define PRIuSIZE "Iu"
   #define PRIXSIZE "IX"
   #define PRIuTIME "lu"
#elif defined(__XC32)
   #define PRIuSIZE "u"
   #define PRIXSIZE "X"
   #define PRIuTIME "u"
#elif defined(__CWCC__)
   #define PRIu8 "u"
   #define PRIu16 "u"
   #define PRIu32 "u"
   #define PRIx8 "x"
   #define PRIx16 "x"
   #define PRIx32 "x"
   #define PRIX8 "X"
   #define PRIX16 "X"
   #define PRIX32 "X"
   #define PRIuSIZE "u"
   #define PRIXSIZE "X"
   #define PRIuTIME "u"
#else
   #define PRIuSIZE "u"
   #define PRIXSIZE "X"
   #define PRIuTIME "lu"
#endif

#if defined(__CC_ARM)
   #undef PRIu8
   #define PRIu8 "u"
   #undef PRIu16
   #define PRIu16 "u"
#endif

#if defined(__MCUXPRESSO)
   #undef PRIu64
   #define PRIu64 "llu"
#endif

//ARM compiler V6?
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
   char *strtok_r(char *s, const char *delim, char **last);
//GCC compiler?
#elif defined(__GNUC__)
   int strcasecmp(const char *s1, const char *s2);
   int strncasecmp(const char *s1, const char *s2, size_t n);
   char *strtok_r(char *s, const char *delim, char **last);
//CodeWarrior compiler?
#elif defined(__CWCC__)
   typedef uint32_t time_t;
   int strcasecmp(const char *s1, const char *s2);
   int strncasecmp(const char *s1, const char *s2, size_t n);
   char *strtok_r(char *s, const char *delim, char **last);
//TI ARM compiler?
#elif defined(__TI_ARM__)
   int strcasecmp(const char *s1, const char *s2);
   int strncasecmp(const char *s1, const char *s2, size_t n);
   char *strtok_r(char *s, const char *delim, char **last);
//Microchip XC32 compiler?
#elif defined(__XC32)
   #define sprintf _sprintf
   int sprintf(char * str, const char * format, ...);
   int strcasecmp(const char *s1, const char *s2);
   int strncasecmp(const char *s1, const char *s2, size_t n);
   char *strtok_r(char *s, const char *delim, char **last);
#endif

//ARM compiler V6?
#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
   #undef __start_packed
   #define __start_packed
   #undef __end_packed
   #define __end_packed __attribute__((packed))
   #define __weak __attribute__((weak))
//GCC compiler?
#elif defined(__GNUC__)
   #undef __start_packed
   #define __start_packed
   #undef __end_packed
   #define __end_packed __attribute__((__packed__))
   #define __weak __attribute__((weak))
//ARM compiler?
#elif defined(__CC_ARM)
   #pragma anon_unions
   #undef __start_packed
   #define __start_packed __packed
   #undef __end_packed
   #define __end_packed
//IAR compiler?
#elif defined(__IAR_SYSTEMS_ICC__)
   #undef __start_packed
   #define __start_packed __packed
   #undef __end_packed
   #define __end_packed
//CodeWarrior compiler?
#elif defined(__CWCC__)
   #undef __start_packed
   #define __start_packed
   #undef __end_packed
   #define __end_packed
   #define __weak
//TI ARM compiler?
#elif defined(__TI_ARM__)
   #undef __start_packed
   #define __start_packed
   #undef __end_packed
   #define __end_packed __attribute__((__packed__))
   #define __weak
//Win32 compiler?
#elif defined(_WIN32)
   #undef interface
   #undef __start_packed
   #define __start_packed
   #undef __end_packed
   #define __end_packed
   #define __weak
#endif

//C++ guard
#ifdef __cplusplus
}
#endif

#endif
