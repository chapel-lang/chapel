
#include "gc.h"

typedef void *_CG_symbol;
typedef void *_CG_fun;
typedef void *_CG_tuple;
typedef void *_CG_list;
typedef void *_CG_vector;
typedef void *_CG_continuation;
typedef void *_CG_any;
typedef void *_CG_nul;
typedef int _CG_int;
typedef void *_CG_ref;
#define _CG_reply _CG_symbol
#define _CG_primitive _CG_symbol
#define _CG_make_tuple _CG_symbol

#define Freply(_c, _r) return _r
#define Fadd(_a, _b) ((_a) + (_b))
#define Fop_paren(_f, _a) ((*(_f))((_f), (_a)))
#define Fset(_a, _b) (_a) = (_b)
#define Fapply(_a, _b) ((*(_a)->e0)((_a)->e1))
#define Fmake_apply(_r, _s, _f, _a) do { 	\
  _r = (_s)GC_MALLOC(sizeof(*((_s)0)));		\
  _r->e0 = _f;					\
  _r->e1 = _a;					\
} while (0)
