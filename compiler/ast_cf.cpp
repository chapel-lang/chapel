/*
  Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"

static void
cast(Sym *s, Sym *t, Immediate *im) {
#include "cast_code.cpp"
}

#define DO_FOLD(_op) \
      switch (res_type->num_type) { \
	case IF1_NUM_TYPE_NONE: \
	  break; \
	case IF1_NUM_TYPE_UINT: { \
	  switch (res_type->num_index) { \
	    case IF1_INT_TYPE_8:  \
	      ast->sym->imm.v_uint8 = im1.v_uint8 _op im2.v_uint8; break; \
	    case IF1_INT_TYPE_16: \
	      ast->sym->imm.v_uint16 = im1.v_uint16 _op im2.v_uint16; break; \
	    case IF1_INT_TYPE_32: \
	      ast->sym->imm.v_uint32 = im1.v_uint32 _op im2.v_uint32; break; \
	    case IF1_INT_TYPE_64: \
	      ast->sym->imm.v_uint64 = im1.v_uint64 _op im2.v_uint64; break; \
	    default: assert(!"case"); \
	  } \
	  break; \
	} \
	case IF1_NUM_TYPE_INT: { \
	  switch (res_type->num_index) { \
	    case IF1_INT_TYPE_8:  \
	      ast->sym->imm.v_int8 = im1.v_int8 _op im2.v_int8; break; \
	    case IF1_INT_TYPE_16: \
	      ast->sym->imm.v_int16 = im1.v_int16 _op im2.v_int16; break; \
	    case IF1_INT_TYPE_32: \
	      ast->sym->imm.v_int32 = im1.v_int32 _op im2.v_int32; break; \
	    case IF1_INT_TYPE_64: \
	      ast->sym->imm.v_int64 = im1.v_int64 _op im2.v_int64; break; \
	    default: assert(!"case"); \
	  } \
	  break; \
	} \
	case IF1_NUM_TYPE_FLOAT: \
	  switch (res_type->num_index) { \
	    case IF1_FLOAT_TYPE_32: \
	      ast->sym->imm.v_float32 = im1.v_float32 _op im2.v_float32; break; \
	    case IF1_FLOAT_TYPE_64: \
	      ast->sym->imm.v_float64 = im1.v_float64 _op im2.v_float64; break; \
	    default: assert(!"case"); \
	  } \
	  break; \
      }

#define DO_FOLDI(_op) \
      switch (res_type->num_type) { \
	case IF1_NUM_TYPE_NONE: \
	  break; \
	case IF1_NUM_TYPE_UINT: { \
	  switch (res_type->num_index) { \
	    case IF1_INT_TYPE_8:  \
	      ast->sym->imm.v_uint8 = im1.v_uint8 _op im2.v_uint8; break; \
	    case IF1_INT_TYPE_16: \
	      ast->sym->imm.v_uint16 = im1.v_uint16 _op im2.v_uint16; break; \
	    case IF1_INT_TYPE_32: \
	      ast->sym->imm.v_uint32 = im1.v_uint32 _op im2.v_uint32; break; \
	    case IF1_INT_TYPE_64: \
	      ast->sym->imm.v_uint64 = im1.v_uint64 _op im2.v_uint64; break; \
	    default: assert(!"case"); \
	  } \
	  break; \
	} \
	case IF1_NUM_TYPE_INT: { \
	  switch (res_type->num_index) { \
	    case IF1_INT_TYPE_8:  \
	      ast->sym->imm.v_int8 = im1.v_int8 _op im2.v_int8; break; \
	    case IF1_INT_TYPE_16: \
	      ast->sym->imm.v_int16 = im1.v_int16 _op im2.v_int16; break; \
	    case IF1_INT_TYPE_32: \
	      ast->sym->imm.v_int32 = im1.v_int32 _op im2.v_int32; break; \
	    case IF1_INT_TYPE_64: \
	      ast->sym->imm.v_int64 = im1.v_int64 _op im2.v_int64; break; \
	    default: assert(!"case"); \
	  } \
	  break; \
	} \
	case IF1_NUM_TYPE_FLOAT: \
	  switch (res_type->num_index) { \
	    default: assert(!"case"); \
	  } \
	  break; \
      }

#define DO_FOLD1(_op) \
      switch (res_type->num_type) { \
	case IF1_NUM_TYPE_NONE: \
	  break; \
	case IF1_NUM_TYPE_UINT: { \
	  switch (res_type->num_index) { \
	    case IF1_INT_TYPE_8:  \
	      ast->sym->imm.v_uint8 = _op im1.v_uint8; break; \
	    case IF1_INT_TYPE_16: \
	      ast->sym->imm.v_uint16 = _op im1.v_uint16; break; \
	    case IF1_INT_TYPE_32: \
	      ast->sym->imm.v_uint32 = _op im1.v_uint32; break; \
	    case IF1_INT_TYPE_64: \
	      ast->sym->imm.v_uint64 = _op im1.v_uint64; break; \
	    default: assert(!"case"); \
	  } \
	  break; \
	} \
	case IF1_NUM_TYPE_INT: { \
	  switch (res_type->num_index) { \
	    case IF1_INT_TYPE_8:  \
	      ast->sym->imm.v_int8 = _op im1.v_int8; break; \
	    case IF1_INT_TYPE_16: \
	      ast->sym->imm.v_int16 = _op im1.v_int16; break; \
	    case IF1_INT_TYPE_32: \
	      ast->sym->imm.v_int32 = _op im1.v_int32; break; \
	    case IF1_INT_TYPE_64: \
	      ast->sym->imm.v_int64 = _op im1.v_int64; break; \
	    default: assert(!"case"); \
	  } \
	  break; \
	} \
	case IF1_NUM_TYPE_FLOAT: \
	  switch (res_type->num_index) { \
	    case IF1_FLOAT_TYPE_32: \
	      ast->sym->imm.v_float32 = _op im1.v_float32; break; \
	    case IF1_FLOAT_TYPE_64: \
	      ast->sym->imm.v_float64 =  _op im1.v_float64; break; \
	    default: assert(!"case"); \
	  } \
	  break; \
      }

#define DO_FOLD1I(_op) \
      switch (res_type->num_type) { \
	case IF1_NUM_TYPE_NONE: \
	  break; \
	case IF1_NUM_TYPE_UINT: { \
	  switch (res_type->num_index) { \
	    case IF1_INT_TYPE_8:  \
	      ast->sym->imm.v_uint8 = _op im1.v_uint8; break; \
	    case IF1_INT_TYPE_16: \
	      ast->sym->imm.v_uint16 = _op im1.v_uint16; break; \
	    case IF1_INT_TYPE_32: \
	      ast->sym->imm.v_uint32 = _op im1.v_uint32; break; \
	    case IF1_INT_TYPE_64: \
	      ast->sym->imm.v_uint64 = _op im1.v_uint64; break; \
	    default: assert(!"case"); \
	  } \
	  break; \
	} \
	case IF1_NUM_TYPE_INT: { \
	  switch (res_type->num_index) { \
	    case IF1_INT_TYPE_8:  \
	      ast->sym->imm.v_int8 = _op im1.v_int8; break; \
	    case IF1_INT_TYPE_16: \
	      ast->sym->imm.v_int16 = _op im1.v_int16; break; \
	    case IF1_INT_TYPE_32: \
	      ast->sym->imm.v_int32 = _op im1.v_int32; break; \
	    case IF1_INT_TYPE_64: \
	      ast->sym->imm.v_int64 = _op im1.v_int64; break; \
	    default: assert(!"case"); \
	  } \
	  break; \
	} \
	case IF1_NUM_TYPE_FLOAT: \
	  switch (res_type->num_index) { \
	    default: assert(!"case"); \
	  } \
	  break; \
      }

static int
fold_constant(IF1 *i, AST *ast, char *foldstr) {
  Sym *a, *b = 0;
  Sym *res_type;
  Immediate im1, im2;
  if (ast->prim->nargs == 3) {
    a = ast->v[0]->sym;
    b = ast->v[2]->sym;
  } else {
    if (ast->prim->pos == 0)
      a = ast->v[1]->sym;
    else
      a = ast->v[0]->sym;
  }
  switch (ast->prim->ret_types[0]) {
    default: return 0;
    case PRIM_TYPE_ANY_NUM_AB:
      res_type = coerce_num(a->type, b->type);     
      break;
    case PRIM_TYPE_ANY_NUM_A:
    case PRIM_TYPE_ANY_INT_A:
      res_type = a->type;
      break;
    case PRIM_TYPE_BOOL:
      res_type = sym_bool;
      break;
  }
  cast(a, res_type, &im1);
  if (b)
    cast(b, res_type, &im2);
  assert(!ast->sym);
  ast->sym = new_sym(i, ast->scope);
  switch (ast->prim->index) {
    case P_prim_mult: DO_FOLD(*); break;
    case P_prim_div: DO_FOLD(/); break;
    case P_prim_mod: DO_FOLDI(%); break;
    case P_prim_add: DO_FOLD(+); break;
    case P_prim_subtract: DO_FOLD(-); break;
    case P_prim_lsh: DO_FOLDI(<<); break;
    case P_prim_rsh: DO_FOLDI(>>); break;
    case P_prim_less: DO_FOLD(<); break;
    case P_prim_lessorequal: DO_FOLD(<=); break;
    case P_prim_greater: DO_FOLD(>); break;
    case P_prim_greaterorequal: DO_FOLD(>=); break;
    case P_prim_equal: DO_FOLD(==); break;
    case P_prim_notequal: DO_FOLD(!=); break;
    case P_prim_and: DO_FOLD(&&); break;
    case P_prim_xor: DO_FOLDI(^); break;
    case P_prim_or: DO_FOLD(||); break;
    case P_prim_land: DO_FOLDI(&); break;
    case P_prim_lor: DO_FOLDI(|); break;
    case P_prim_preinc: DO_FOLD1(++); break;
    case P_prim_predec: DO_FOLD1(--); break;
    case P_prim_plus: DO_FOLD1(+); break;
    case P_prim_minus: DO_FOLD1(-); break;
    case P_prim_bnot: DO_FOLD1I(~); break;
    case P_prim_not: DO_FOLD1(!); break;
    case P_prim_postinc: DO_FOLD1(++); break;
    case P_prim_postdec: DO_FOLD1(--); break;
    default: return 0;
  }
  ast->sym->constant = foldstr;
  ast->sym->in = 0;
  ast->sym->type = res_type;
  ast->kind = AST_const;
  return 0;
}

static int
ast_constant_fold_internal(IF1 *i, AST *ast, char *foldstr) {
  forv_AST(a, *ast)
    if (ast_constant_fold_internal(i, a, foldstr) < 0)
      return -1;
  switch (ast->kind) {
    case AST_const: {
      switch (ast->sym->type->num_type) {
	case IF1_NUM_TYPE_NONE:
	  break;
	case IF1_NUM_TYPE_UINT: {
	  switch (ast->sym->type->num_index) {
	    case IF1_INT_TYPE_8: 
	      ast->sym->imm.v_uint8 = strtoul(ast->sym->constant, 0, 0); break;
	    case IF1_INT_TYPE_16:
	      ast->sym->imm.v_uint16 = strtoul(ast->sym->constant, 0, 0); break;
	    case IF1_INT_TYPE_32:
	      ast->sym->imm.v_uint32 = strtoul(ast->sym->constant, 0, 0); break;
	    case IF1_INT_TYPE_64:
	      ast->sym->imm.v_uint64 = strtoull(ast->sym->constant, 0, 0); break;
	    default: assert(!"case");
	  }
	  break;
	}
	case IF1_NUM_TYPE_INT: {
	  switch (ast->sym->type->num_index) {
	    case IF1_INT_TYPE_8: 
	      ast->sym->imm.v_int8 = strtol(ast->sym->constant, 0, 0); break;
	    case IF1_INT_TYPE_16:
	      ast->sym->imm.v_int16 = strtol(ast->sym->constant, 0, 0); break;
	    case IF1_INT_TYPE_32:
	      ast->sym->imm.v_int32 = strtol(ast->sym->constant, 0, 0); break;
	    case IF1_INT_TYPE_64:
	      ast->sym->imm.v_int64 = strtoll(ast->sym->constant, 0, 0); break;
	    default: assert(!"case");
	  }
	  break;
	}
	case IF1_NUM_TYPE_FLOAT:
	  switch (ast->sym->type->num_index) {
	    case IF1_FLOAT_TYPE_32:
	      ast->sym->imm.v_float32 = strtod(ast->sym->constant, 0); break;
	    case IF1_FLOAT_TYPE_64:
	      ast->sym->imm.v_float64 = strtod(ast->sym->constant, 0); break;
	    default: assert(!"case");
	  }
	  break;
      }
      break;
    }
    case AST_op: {
      ast->prim = i->primitives->find(ast);
      if (ast->prim)
	if (fold_constant(i, ast, foldstr) < 0)
	  return -1;
      break;
    }
    default: break;
  }
  return 0;
}

int
ast_constant_fold(IF1 *i, AST *ast) {
  return ast_constant_fold_internal(i, ast, if1_cannonicalize_string(i, "< folded >"));
}
