/*
  Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"
#include "prim.h"
#include "if1.h"
#include "builtin.h"
#include "fa.h"
#include "parse_ast.h"

static void
cast(Sym *s, Sym *t, Immediate *im) {
#include "cast_code.cpp"
}

#define DO_FOLD(_op) \
      switch (res_type->num_kind) { \
	case IF1_NUM_KIND_NONE: \
	  break; \
	case IF1_NUM_KIND_UINT: { \
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
	case IF1_NUM_KIND_INT: { \
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
	case IF1_NUM_KIND_FLOAT: \
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
      switch (res_type->num_kind) { \
	case IF1_NUM_KIND_NONE: \
	  break; \
	case IF1_NUM_KIND_UINT: { \
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
	case IF1_NUM_KIND_INT: { \
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
	case IF1_NUM_KIND_FLOAT: \
	  switch (res_type->num_index) { \
	    default: assert(!"case"); \
	  } \
	  break; \
      }

#define DO_FOLD1(_op) \
      switch (res_type->num_kind) { \
	case IF1_NUM_KIND_NONE: \
	  break; \
	case IF1_NUM_KIND_UINT: { \
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
	case IF1_NUM_KIND_INT: { \
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
	case IF1_NUM_KIND_FLOAT: \
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
      switch (res_type->num_kind) { \
	case IF1_NUM_KIND_NONE: \
	  break; \
	case IF1_NUM_KIND_UINT: { \
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
	case IF1_NUM_KIND_INT: { \
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
	case IF1_NUM_KIND_FLOAT: \
	  switch (res_type->num_index) { \
	    default: assert(!"case"); \
	  } \
	  break; \
      }

static int
fold_constant(IF1 *i, ParseAST *ast) {
  Sym *a, *b = 0;
  Sym *res_type;
  Immediate im1, im2;
  if (ast->prim->nargs == 3) {
    a = ast->children.v[0]->sym;
    b = ast->children.v[2]->sym;
  } else {
    if (ast->prim->pos == 0)
      a = ast->children.v[1]->sym;
    else
      a = ast->children.v[0]->sym;
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
  if (!a->is_constant || (b && !b->is_constant))
    return 0;
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
    case P_prim_plus: DO_FOLD1(+); break;
    case P_prim_minus: DO_FOLD1(-); break;
    case P_prim_bnot: DO_FOLD1I(~); break;
    case P_prim_not: DO_FOLD1(!); break;
    default: return 0;
  }
  ast->sym->constant = cannonical_folded;	
  ast->sym->is_constant = 1;
  ast->sym->in = 0;
  ast->sym->type = res_type;
  ast->kind = AST_const;
  return 0;
}

void
convert_constant_to_immediate(Sym *sym) {
  if (!sym->is_constant)	
    return;
  switch (sym->type->num_kind) {
    case IF1_NUM_KIND_NONE:
      break;
    case IF1_NUM_KIND_UINT: {
      switch (sym->type->num_index) {
	case IF1_INT_TYPE_8: 
	  if (sym->constant[0] != '\'')
	    sym->imm.v_uint8 = strtoul(sym->constant, 0, 0);
	  else {
	    if (sym->constant[1] != '\\')
	      sym->imm.v_uint8 = sym->constant[1];
	    else
	      sym->imm.v_uint8 = sym->constant[2];
	  }
	  break;
	case IF1_INT_TYPE_16:
	  sym->imm.v_uint16 = strtoul(sym->constant, 0, 0); break;
	case IF1_INT_TYPE_32:
	  sym->imm.v_uint32 = strtoul(sym->constant, 0, 0); break;
	case IF1_INT_TYPE_64:
	  sym->imm.v_uint64 = strtoull(sym->constant, 0, 0); break;
	default: assert(!"case");
      }
      break;
    }
    case IF1_NUM_KIND_INT: {
      switch (sym->type->num_index) {
	case IF1_INT_TYPE_8: 
	  if (sym->constant[0] != '\'')
	    sym->imm.v_int8 = strtoul(sym->constant, 0, 0);
	  else {
	    if (sym->constant[1] != '\\')
	      sym->imm.v_int8 = sym->constant[1];
	    else
	      sym->imm.v_int8 = sym->constant[2];
	  }
	  break;
	case IF1_INT_TYPE_16:
	  sym->imm.v_int16 = strtol(sym->constant, 0, 0); break;
	case IF1_INT_TYPE_32:
	  sym->imm.v_int32 = strtol(sym->constant, 0, 0); break;
	case IF1_INT_TYPE_64:
	  sym->imm.v_int64 = strtoll(sym->constant, 0, 0); break;
	default: assert(!"case");
      }
      break;
    }
    case IF1_NUM_KIND_FLOAT:
      switch (sym->type->num_index) {
	case IF1_FLOAT_TYPE_32:
	  sym->imm.v_float32 = strtod(sym->constant, 0); break;
	case IF1_FLOAT_TYPE_64:
	  sym->imm.v_float64 = strtod(sym->constant, 0); break;
	default: assert(!"case");
      }
      break;
  }
}

int
ast_constant_fold(IF1 *i, ParseAST *ast) {
  forv_ParseAST(a, ast->children)
    if (ast_constant_fold(i, a) < 0)
      return -1;
  switch (ast->kind) {
    case AST_const:
      convert_constant_to_immediate(ast->sym);
      break;
    case AST_op: {
      ast->prim = i->primitives->find(ast);
      if (ast->prim)
	if (fold_constant(i, ast) < 0)
	  return -1;
      break;
    }
    default: break;
  }
  return 0;
}


