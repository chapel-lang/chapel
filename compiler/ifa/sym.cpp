#include "geysa.h"
#include "ast.h"
#include "if1.h"
#include "fa.h"
#include "builtin.h"

char *type_kind_string[] = {
  "NONE", "UNKNOWN", "LUB", "GLB", "PRODUCT", "RECORD", "VECTOR",
  "FUN", "REF", "TAGGED", "PRIMITIVE", "APPLICATION", "VARIABLE", "ALIAS"};

BasicSym::BasicSym(void) :
  name(NULL),
  in(NULL),
  type(NULL),
  aspect(NULL),
  must_specialize(NULL),
  must_implement(NULL),
  ast(NULL),
  var(NULL),
  asymbol(NULL),
  nesting_depth(0),
  cg_string(NULL),
  is_builtin(0),
  is_read_only(0),
  is_constant(0),
  is_lvalue(0),
  is_var(0),
  is_default_arg(0),
  is_exact_match(0),
  is_module(0),
  is_fun(0),
  is_symbol(0),
  is_pattern(0),
  is_rest(0),
  is_generic(0),
  is_external(0),
  intent(0),
  global_scope(0),
  function_scope(0),
  is_structure(0),
  is_meta_type(0),
  is_value_class(0),
  is_union_class(0),
  fun_returns_value(0),
  live(0),
  incomplete(0),
  type_kind(0),
  num_kind(0),
  num_index(0),
  clone_for_constants(0)
{}


Sym *
meta_apply(Sym *fn, Sym *arg) {
  if (fn->type_kind != Type_APPLICATION)
    return 0;
  assert(0);
  return 0;
}

Sym::Sym() :
  destruct_name(NULL),
  arg_name(NULL),
  constant(NULL),
  size(0),
  scope(NULL),
  labelmap(NULL),
  fun(NULL),
  code(NULL),
  self(NULL),
  ret(NULL),
  cont(NULL),
  instantiates(NULL),
  match_type(NULL),
  abstract_type(NULL),
  alias(NULL),
  init(NULL),
  meta_type(NULL),
  element(NULL),
  domain(NULL),
  temp(NULL)
{}

char *
Sym::pathname() {
  char *p = 0;
  if (asymbol)
    p = asymbol->pathname();
  if (!p && ast)
    p = ast->pathname();
  if (p)
    return p;
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
  int l = 0;
  if (asymbol)
    l = asymbol->line();
  if (!l && ast)
    l = ast->line();
  return l;
}

int
Sym::ast_id() {
  if (asymbol)
    return asymbol->ast_id();
  return id;
}

int
Sym::log_line() {
  if (asymbol)
    return asymbol->log_line();
  if (strstr(filename(), "prelude"))
    return 0;
  return line();
}

void
Sym::copy_values(Sym *s) {
  int temp_id = id;
  *this = *s;
  id = temp_id;
}

Sym *
Sym::clone() {
  if (asymbol)
    return asymbol->clone();
  Sym *new_sym = copy();
  return new_sym;
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


static int sprint_float_val(char* str, double val) {
  int numchars = sprintf(str, "%g", val);
  if (strchr(str, '.') == NULL && strchr(str, 'e') == NULL) {
    strcat(str, ".0");
    return numchars + 2;
  } else {
    return numchars;
  }
}

static int sprint_complex_val(char* str, double real, double imm) {
  int numchars = 0;
  numchars += sprintf(str+numchars, "(");
  numchars += sprint_float_val(str+numchars, real);
  numchars += sprintf(str+numchars, ",");
  numchars += sprint_float_val(str+numchars, imm);
  numchars += sprintf(str+numchars, ")");
  return numchars;
}


int 
sprint_imm(char *str, Immediate &imm) {
  int res = -1;
  switch (imm.const_kind) {
    case IF1_NUM_KIND_NONE:
      break;
    case IF1_NUM_KIND_UINT: {
      switch (imm.num_index) {
        case IF1_INT_TYPE_1: 
          res = sprintf(str, "%u", imm.v_bool); break;
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
      switch (imm.num_index) {
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
      switch (imm.num_index) {
        case IF1_FLOAT_TYPE_32:
          res = sprint_float_val(str, imm.v_float32); break;
        case IF1_FLOAT_TYPE_64:
          res = sprint_float_val(str, imm.v_float64); break;
        default: assert(!"case");
      }
      break;
    case IF1_NUM_KIND_COMPLEX:
      switch (imm.num_index) {
        case IF1_FLOAT_TYPE_32: 
          res = sprint_complex_val(str, imm.v_complex32.r, imm.v_complex32.i); break;
        case IF1_FLOAT_TYPE_64:
          res = sprint_complex_val(str, imm.v_complex64.r, imm.v_complex64.i); break;
        default: assert(!"case");
      }
      break;
    case IF1_CONST_KIND_STRING:
      res = sprintf(str, "%s", imm.v_string); break;
      break;
  }
  return res;
}

int 
fprint_imm(FILE *fp, Immediate &imm) {
  char str[80];
  int res;
  if ((res = sprint_imm(str, imm) >= 0))
    fputs(str, fp);
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

int
if1_numeric_size(IF1 *p, Sym *t) {
  switch (t->num_kind) {
    case IF1_NUM_KIND_NONE: assert(!"bad case"); break;
    case IF1_NUM_KIND_INT:
    case IF1_NUM_KIND_UINT:
      return 1 << (t->num_index ? t->num_index : 1) - 1; // bool takes 1 byte
    case IF1_NUM_KIND_FLOAT:
      return 2 + (2 * t->num_index);
    case IF1_NUM_KIND_COMPLEX:
      return 2 * (2 + (2 * t->num_index));
  }
  return 0;
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

Sym *
Sym::scalar_type() {
  if (type && type->num_kind)
    return type;
  forv_Sym(ss, dispatch_order)
    if (ss->type && ss->type->num_kind)
      return ss->type;
  return 0;
}

Sym *
Sym::coerce_to(Sym *to) {
  if (this == to)
    return this;
  Sym *s1 = this->scalar_type(), *s2 = to->scalar_type();
  if (s1 && s2) {
    Sym *t = coerce_num(s1, s2);
    if (t == s2)
      return s2;
    return NULL;
  }
  if (element) {
    Sym *s1 = element->scalar_type(), *s2 = to->scalar_type();
    if (s1 && s2) {
      Sym *t = coerce_num(s1, s2);
      if (t == s2)
        return s2;
      return NULL;
    }
  }
  if (s1) {
    if (to == sym_string)
      return to;
  }
  return NULL;
}

