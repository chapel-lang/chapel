#ifndef _chpltypes_H_
#define _chpltypes_H_

#include <stdint.h>

typedef signed char int8;
typedef short       int16;
typedef int         int32;
typedef long long   int64;
// typedef long long int128;            // WAW: needs fixing
typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;
// typedef unsigned long long uint128;  // WAW: needs fixing

#define int8fmt "%hhd"
#define int16fmt "%hd"
#define int32fmt "%d"
#define int64fmt "%lld"
#define uint8fmt "%hhu"
#define uint16fmt "%hu"
#define uint32fmt "%u"
#define uint64fmt "%llu"

typedef uint32 uint;
typedef float       float32;
typedef double      float64;
typedef long double float128;
typedef struct { float32 r; float32 i; } complex64;
typedef struct { float64 r; float64 i; } complex128;
typedef struct { float128 r; float128 i; } complex256;

#endif
