#include <stdio.h>
#include <stdint.h>

#define USE_GC

#ifdef LEAK_DETECT
#define GC_DEBUG
#include "gc.h"
#define MALLOC(n) GC_MALLOC(n)
#define CALLOC(m,n) GC_MALLOC((m)*(n))
#define FREE(p) GC_FREE(p)
#define REALLOC(p,n) GC_REALLOC((p),(n))
#define CHECK_LEAKS() GC_gcollect()
#define MEM_INIT() GC_INIT()
#else
#ifdef USE_GC
#include "gc.h"
#define MALLOC GC_MALLOC
#define REALLOC GC_REALLOC
#define FREE(_x)
#define malloc dont_use_malloc_use_MALLOC_instead
#define relloc dont_use_realloc_use_REALLOC_instead
#define free dont_use_free_use_FREE_instead
#define MEM_INIT() GC_INIT()
#else
#include <stdlib.h>
#define MALLOC malloc
#define REALLOC realloc
#define FREE free
#define MEM_INIT()
#endif
#endif

typedef char int8;
typedef unsigned char uint8;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;
typedef short int16;
typedef unsigned short uint16;
/* typedef uint32 uint; * already part of most systems */
typedef float float32;
typedef double float64;
typedef struct { float32 r; float32 i; } complex32;
typedef struct { float64 r; float64 i; } complex64;

typedef void *_CG_symbol;
typedef void *_CG_function;
typedef void *_CG_tuple;
typedef void *_CG_list;
typedef void *_CG_vector;
typedef void *_CG_continuation;
typedef void *_CG_any;
typedef void *_CG_null;
typedef void *_CG_void;
typedef int _CG_int;
typedef int _CG_bool;
typedef uint8 _CG_uint8;
typedef uint16 _CG_uint16;
typedef uint32 _CG_uint32;
typedef uint64 _CG_uint64;
typedef int8 _CG_int8;
typedef int16 _CG_int16;
typedef int32 _CG_int32;
typedef int64 _CG_int64;
typedef float32 _CG_float32;
typedef float64 _CG_float64;
typedef complex32 _CG_complex32;
typedef complex64 _CG_complex64;
typedef char * _CG_string;
typedef void *_CG_ref;
#define _CG_reply _CG_symbol
#define _CG_primitive _CG_symbol
#define _CG_make_tuple _CG_symbol
#define _CG_Symbol(_x, _y) ((void*)(uintptr_t)_x)
#define null ((void*)0)
#define bool int
#define true 1
#define false 0
#define __init ((void*)0)

#define _CG_prim_primitive_chapel_defexpr(_c) ((void*)0)
#define _CG_prim_coerce(_t, _v) ((_t)_v)
#define _CG_prim_closure(_c) (_c)GC_MALLOC(sizeof(*((_c)0)))
#define _CG_prim_tuple(_c) (_c)GC_MALLOC(sizeof(*((_c)0)))
#define _CG_prim_vector(_c, _n) (void*)GC_MALLOC(sizeof(_c*) * _n)
#define _CG_prim_new(_c) (_c)GC_MALLOC(sizeof(*((_c)0)))
#define _CG_prim_reply(_s, _c, _r) return _r
#define _CG_prim_primitive(_p, _x) printf("%d\n", (unsigned int)(uintptr_t)_x);
#define _CG_prim_add(_a, _op, _b) ((_a) + (_b))
#define _CG_prim_subtract(_a, _op, _b) ((_a) - (_b))
#define _CG_prim_mult(_a, _op, _b) ((_a) * (_b))
#define _CG_prim_div(_a, _op, _b) ((_a) / (_b))
#define _CG_prim_and(_a, _op, _b) ((_a) & (_b))
#define _CG_prim_xor(_a, _op, _b) ((_a) ^ (_b))
#define _CG_prim_or(_a, _op, _b) ((_a) | (_b))
#define _CG_prim_lor(_a, _op, _b) ((_a) || (_b))
#define _CG_prim_land(_a, _op, _b) ((_a) && (_b))
#define _CG_prim_bnot(_op, _a) (~(_a))
#define _CG_prim_lessorequal(_a, _op, _b) ((_a) <= (_b))
#define _CG_prim_equal(_a, _op, _b) ((_a) == (_b))
#define _CG_prim_paren(_f, _a) ((*(_f))((_f), (_a)))
#define _CG_prim_set(_a, _b) (_a) = (_b)
#define _CG_prim_apply(_a, _b) ((*(_a)->e0)((_a)->e1))
#define _CG_make_apply(_r, _s, _f, _a) do {     \
  _r = (_s)GC_MALLOC(sizeof(*((_s)0)));         \
  _r->e0 = _f;                                  \
  _r->e1 = _a;                                  \
} while (0)

static inline int _CG_prim_primitive_write(_CG_symbol s, ...) {
  return 0;
}

/* BLC: These seem to be required due to the new methodTokenType
   code inserted at the high-level */
#define _CG_prim_primitive_sizeof(x) sizeof(x)
#define _methodTokenType char
#define _CG_prim_primitive_cast(type, x) (x)
#define _chpl_alloc(_lvalue, _type) ((_type)GC_MALLOC(sizeof(*(_lvalue))))
