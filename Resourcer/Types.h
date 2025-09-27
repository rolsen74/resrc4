
/*
** SPDX-License-Identifier: Unlicense
** Written 2025 by Rene W. Olsen
** Target OS: AmigaOS4
*/

// --

#ifndef RESOURCER_TYPES_H
#define RESOURCER_TYPES_H

// --

/*
** -- Version v1.2 (26-Sep-2025) --
**
** S8     -  int8_t
** U8     - uint8_t
** S16    -  int16_t
** U16    - uint16_t
** S32    -  int32_t
** U32    - uint32_t
** S64    -  int64_t
** U64    - uint64_t
** PTR    - void *
** CHR    - char
** STR    - char *
** CSTR   - const char *
** MEM    - uint8_t *
** CMEM   - const uint8_t *
** MIN    - MIN(a,b) 
** MAX    - MAX(a,b)
** ABS    - ABS(x)
** TRUE   - 1
** FALSE  - 0
** USED   - __attribute__((used))
** UNUSED - __attribute__((unused))
** 
*/

// --

#ifndef U8
typedef uint8_t U8;
#endif

#ifndef U16
typedef uint16_t U16;
#endif

#ifndef U32
typedef uint32_t U32;
#endif

#ifndef U64
typedef uint64_t U64;
#endif

#ifndef S8
typedef int8_t S8;
#endif

#ifndef S16
typedef int16_t S16;
#endif

#ifndef S32
typedef int32_t S32;
#endif

#ifndef S64
typedef int64_t S64;
#endif

#ifndef PTR
typedef void *PTR;
#endif

#ifndef CHR
typedef char CHR;
#endif

#ifndef STR
typedef char *STR;
#endif

#ifndef CSTR
typedef const char *CSTR;
#endif

#ifndef MEM
typedef uint8_t *MEM;
#endif

#ifndef CMEM
typedef const uint8_t *CMEM;
#endif

#ifndef MIN
#define MIN(a,b)	((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#endif

#ifndef ABS
#define ABS(x) 		((x) < 0 ? -(x) : (x))
#endif

#ifndef TRUE
#define TRUE		1 
#endif

#ifndef FALSE
#define FALSE		0 
#endif

// --
#if defined(__GNUC__) || defined(__clang__)
// GCC or Clang compiler

#ifndef USED
#define USED		__attribute__((used))
#endif

#ifndef UNUSED
#define UNUSED		__attribute__((unused))
#endif

#else
// Unknown compiler

#ifndef USED
#define USED
#endif

#ifndef UNUSED
#define UNUSED
#endif

#endif
// --

#endif