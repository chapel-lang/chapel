#include "geysa.h"
#include "ast.h"
#include "if1.h"
#include "builtin.h"

Sym *
meta_apply(Sym *fn, Sym *arg) {
  if (fn->type_kind != Type_APPLICATION)
    return 0;
  assert(0);
  return 0;
}

char *
Sym::pathname() {
  if (ast)
    return ast->pathname();
  return "<unknown>";
}

char *
Sym::filename() {
  char *fn = pathname();
  char *r = strrchr(fn, '/');
  if (r) return r+1; else return fn;
}

int
Sym::line() {
  if (ast)
    return ast->line();
  return 0;
}

void
Sym::copy_values(Sym *s) {
  name = s->name;
  ast = s->ast;
}

Sym *
Sym::copy() {
  Sym *s = new_Sym();
  s->copy_values(this);
  return s;
}

int
compar_syms(const void *ai, const void *aj) {
  uint32 i = (*(Sym**)ai)->id;
  uint32 j = (*(Sym**)aj)->id;
  return (i > j) ? 1 : ((i < j) ? -1 : 0);
}

int
Sym::imm_int(int *result) {
  int i = 0;
  switch (type->num_kind) {
    default: return -1;
    case IF1_NUM_KIND_UINT: {
      switch (type->num_index) {
	case IF1_INT_TYPE_8: i = imm.v_uint8; break;
	case IF1_INT_TYPE_16: i = imm.v_uint16; break;
	case IF1_INT_TYPE_32: i = imm.v_uint32; break;
	case IF1_INT_TYPE_64: i = imm.v_uint64; break;
	default: return -1;
      }
      break;
    }
    case IF1_NUM_KIND_INT: {
      switch (type->num_index) {
	case IF1_INT_TYPE_8: i = imm.v_int8; break;
	case IF1_INT_TYPE_16: i = imm.v_int16; break;
	case IF1_INT_TYPE_32: i = imm.v_int32; break;
	case IF1_INT_TYPE_64: i = imm.v_int64; break;
	default: return -1;
      }
      break;
    }
  }
  *result = i;
  return 0;
}

Sym *
unalias_type(Sym *s) {
  if (!s)
    return s;
  if (s->type_kind == Type_ALIAS) {
    Vec<Sym *> aliases;
    do {
      if (!s->alias)
	return 0;
      Sym *ss = s->alias;
      if (aliases.set_in(ss))
	fail("circular type alias");
      aliases.set_add(ss);
      s = ss;
    } while (s->type_kind == Type_ALIAS);
  }
  return s;
}

int 
sprint(char *str, Immediate &imm, Sym *type) {
  int res = -1;
  switch (type->num_kind) {
    case IF1_NUM_KIND_NONE:
      break;
    case IF1_NUM_KIND_UINT: {
      switch (type->num_index) {
	case IF1_INT_TYPE_8: 
	  res = sprintf(str, "%u", imm.v_uint8); break;
	case IF1_INT_TYPE_16:
	  res = sprintf(str, "%u", imm.v_uint16); break;
	case IF1_INT_TYPE_32:
	  res = sprintf(str, "%u", imm.v_uint32); break;
	case IF1_INT_TYPE_64:
	  res = sprintf(str, "%llu", imm.v_uint64); break;
	default: assert(!"case");
      }
      break;
    }
    case IF1_NUM_KIND_INT: {
      switch (type->num_index) {
	case IF1_INT_TYPE_8: 
	  res = sprintf(str, "%d", imm.v_int8); break;
	case IF1_INT_TYPE_16:
	  res = sprintf(str, "%d", imm.v_int16); break;
	case IF1_INT_TYPE_32:
	  res = sprintf(str, "%d", imm.v_int32); break;
	case IF1_INT_TYPE_64:
	  res = sprintf(str, "%lld", imm.v_int64); break;
	default: assert(!"case");
      }
      break;
    }
    case IF1_NUM_KIND_FLOAT:
      switch (type->num_index) {
	case IF1_FLOAT_TYPE_32:
	  res = sprintf(str, "%f", imm.v_float32); break;
	case IF1_FLOAT_TYPE_64:
	  res = sprintf(str, "%f", imm.v_float64); break;
	default: assert(!"case");
      }
      break;
  }
  return res;
}

int 
print(FILE *fp, Immediate &imm, Sym *type) {
  char str[80];
  int res;
  if ((res = sprint(str, imm, type) >= 0))
    fprintf(fp, str);
  return res;
}

void
if1_set_int_type(IF1 *p, Sym *t, int signd, int size) {    
  int ss = 0;
  size >>= 3;
  while (size) { ss++; size >>= 1; }
  p->int_types[ss][signd] = t;
  t->num_kind = signd ? IF1_NUM_KIND_INT : IF1_NUM_KIND_UINT;
  t->num_index = ss;
}

void
if1_set_float_type(IF1 *p, Sym *t, int size) {
  int ss = 0;
  size >>= 4;
  ss = size - 1;
  p->float_types[ss] = t;
  t->num_kind = IF1_NUM_KIND_FLOAT;
  t->num_index = ss;
}

void
if1_set_complex_type(IF1 *p, Sym *t, int size) {
  int ss = 0;
  size >>= 4;
  ss = size - 1;
  p->complex_types[ss] = t;
  t->num_kind = IF1_NUM_KIND_COMPLEX;
  t->num_index = ss;
}

void
Sym::inherits_add(Sym *s) {
  implements.add(s);
  specializes.add(s);
  includes.add(s);
}  

void
Sym::must_implement_and_specialize(Sym *s) {
  assert(!must_implement && !must_specialize);
  must_implement = s;
  must_specialize = s;
}

int
Sym::is_scalar() {
  return sym_anynum->specializers.set_in(type) != 0;
}

Sym *
Sym::element_type() {
  return element;
}
