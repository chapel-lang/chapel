#include <stdint.h>
#include <stdio.h>
#include "gc.h"
#include "chpltypes.h"

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
typedef void *_CG_ref;
#define _CG_reply _CG_symbol
#define _CG_primitive _CG_symbol
#define _CG_make_tuple _CG_symbol
#define _CG_Symbol(_x, _y) ((void*)(uintptr_t)_x)

#define _CG_prim_closure(_c) (_c)GC_MALLOC(sizeof(*((_c)0)))
#define _CG_prim_tuple(_c) (_c)GC_MALLOC(sizeof(*((_c)0)))
#define _CG_prim_vector(_c, _n) (void*)GC_MALLOC(sizeof(_c*) * _n)
#define _CG_prim_new(_c) (_c)GC_MALLOC(sizeof(*((_c)0)))
#define _CG_prim_reply(_c, _r) return _r
#define _CG_prim_print(_x) printf("%d\n", (unsigned int)(uintptr_t)_x);
#define _CG_prim_add(_a, _op, _b) ((_a) + (_b))
#define _CG_prim_subtract(_a, _op, _b) ((_a) - (_b))
#define _CG_prim_mult(_a, _op, _b) ((_a) * (_b))
#define _CG_prim_div(_a, _op, _b) ((_a) / (_b))
#define _CG_prim_lessorequal(_a, _op, _b) ((_a) <= (_b))
#define _CG_prim_paren(_f, _a) ((*(_f))((_f), (_a)))
#define _CG_prim_set(_a, _b) (_a) = (_b)
#define _CG_prim_apply(_a, _b) ((*(_a)->e0)((_a)->e1))
#define _CG_make_apply(_r, _s, _f, _a) do { 	\
  _r = (_s)GC_MALLOC(sizeof(*((_s)0)));		\
  _r->e0 = _f;					\
  _r->e1 = _a;					\
} while (0)
