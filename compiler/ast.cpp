/*
  Copyright 2003-2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"

char *AST_name[] = {
#define S(_x) #_x,
#include "ast_kinds.h"
#undef S
};

char *cannonical_class = 0;
char *cannonical_self = 0;
char *cannonical_folded = 0;

AST *
AST::get(AST_kind k) {
  for (int i = 0; i < n; i++)
    if (v[i]->kind == k)
      return v[i];
  return NULL;
}

void 
AST::add(AST *a) { 
  if (a) { 
    if (!line) {
      pathname = a->pathname;
      line = a->line;
    }
    Vec<AST *>::add(a); 
  }
}

static void
dig_ast(AST *ast, D_ParseNode *pn) {
  if (pn->user.ast)
    ast->add(pn->user.ast);
  else
    for (int i = 0; i < d_get_number_of_children(pn); i++)
      dig_ast(ast, d_get_child(pn, i));
}

void
AST::add(D_ParseNode *pn) {
  dig_ast(this, pn);
} 

void
AST::set_location(D_ParseNode *pn) {
  scope_kind = pn->scope->kind;
  pathname = pn->start_loc.pathname;
  line = pn->start_loc.line;
}

AST *
AST::copy(PNodeMap *nmap) {
  AST *a = new AST(*this);
  if (nmap)
    for (int i = 0; i < a->pnodes.n; i++)
      a->pnodes.v[i] = nmap->get(a->pnodes.v[i]);
  for (int i = 0; i < a->n; i++)
    a->v[i] = a->v[i]->copy(nmap);
  return a;
}

AST::AST(AST_kind k, D_ParseNode *pn) {
  memset(this, 0, sizeof(*this)); // no virtuals here!
  kind = k;
  if (pn) {
    set_location(pn);
    add_below(pn);
  }
}

void
AST::add_below(D_ParseNode *pn) {
  for (int i = 0; i < d_get_number_of_children(pn); i++)
    dig_ast(this, d_get_child(pn, i));
}

static inline AST *
ast_qualified_ident_ident(AST *x) { 
  return x->v[x->n-1]; 
}

static char *
ast_qualified_ident_string(AST *ast) {
  char *s = ast->v[0]->string;
  s = s ? s : (char*)""; // global
  for (int i = 1; i < ast->n; i++) {
    char *ss = (char*)MALLOC(strlen(s) + strlen(ast->v[i]->string) + 3);
    strcpy(ss, s);
    strcat(ss,"::");
    strcat(ss, ast->v[i]->string);
    s = ss;
  }
  return s;
}

static Scope *
ast_qualified_ident_scope(AST *a) {
  int i = 0;
  Scope *s = a->scope;
  if (a->v[0]->kind == AST_global) {
    i = 1;
    s = s->global();
  }
  for (int x = i; x < a->n - 1; x++) {
    Sym *sym = s->get(a->v[x]->string);
    if (!sym || !sym->scope) {
      show_error("unresolved identifier qualifier '%s'", a, a->v[x]->string);
      return 0;
    }
    s = sym->scope;
  }
  return s;
}

static Sym *
ast_qualified_ident_sym(AST *a, Sym **container) {
  Scope *s = ast_qualified_ident_scope(a);
  if (!s)
    return NULL;
  AST *id = ast_qualified_ident_ident(a);
  return s->get(id->string, container);
}

Sym *
checked_ast_qualified_ident_sym(AST *ast, Sym **container = 0) {
  Sym *sym = ast_qualified_ident_sym(ast, container);
  if (!sym) { 
    show_error("unresolved identifier '%s'", ast, ast_qualified_ident_string(ast));
    return NULL;
  }
  ast->sym = sym;
  return sym;
}

static char *SPACES = "                                        "
                      "                                        ";
#define SP(_fp, _n) fputs(&SPACES[80-(_n)], _fp)
void
ast_print(FILE *fp, AST *a, int indent) {
  SP(fp, indent);
  fprintf(fp, "%s", AST_name[a->kind]);
  if (a->sym) {
    if (a->sym->constant)
      fprintf(fp, " constant %s", a->sym->constant);
    else if (a->sym->symbol)
      fprintf(fp, " symbol %s", a->sym->name);
    else if (a->sym->name)
      fprintf(fp, " sym %s", a->sym->name);
    else 
      fprintf(fp, " id(%d)", a->sym->id);
  }
  if (a->string)
    fprintf(fp, " %s", a->string);
  if (a->builtin)
    fprintf(fp, " builtin %s", a->builtin);
  fputs("\n", fp);
}

void
ast_pp(AST *a) {
  ast_print_recursive(stdout, a, 0);
}

void
ast_print_recursive(FILE *fp, AST *a, int indent) {
  ast_print(fp, a, indent);
  forv_AST(aa, *a)
    ast_print_recursive(fp, aa, indent + 1);
}

void
ast_write(AST *a, char *filename) {
  FILE *fp = fopen(filename, "w");
  ast_print_recursive(fp, a);
  fclose(fp);
}

static Sym *
new_constant(IF1 *i, char *string, char *constant_type) {
  char *e = string + strlen(string);
  while (e > string && isspace(e[-1])) e--;
  char *s = string;
  if (s + 1 < e && s[0] == '#') {
    if (s[1] == '"') {
      s += 2;
      e--;
    } else
      s++;
    return if1_make_symbol(i, s, e);
  }
  return if1_const(i, if1_get_builtin(i, constant_type), dupstr(s, e));
}

Sym *
new_sym(IF1 *i, Scope *scope, char *s, Sym *sym) {
  if (!sym)
    sym = if1_alloc_sym(i, s);
  if (!sym->in && scope) {
    Sym *scope_in = unalias_type(scope->in);
    // unnamed temporaries are local to the class or module
    if (scope_in && !scope_in->fun && scope_in->init && !s) {
      sym->in = scope_in->init;
      assert(sym->in);
    } else
      sym->in = scope->in;
  }
  if (s && scope)
    scope->put(s, sym);
  if (verbose_level > 2)
    printf("new sym %p %s in %p\n", sym, s, scope);
  return sym;
}

static void
set_builtin(IF1 *i, Sym *sym, char *start, char *end = 0) {
  if1_set_builtin(i, sym, start, end);
  if (!end) end = start + strlen(start);
  int x = 0;
  for (; builtin_strings[x]; x++)
    if ((int)strlen(builtin_strings[x]) == (int)(end-start) && 
	!strncmp(builtin_strings[x], start, end-start))
      goto Lfound;
  fail("builtin not found '%s'", dupstr(start, end));
 Lfound:
  switch (x) {
#define S(_n) case Builtin_##_n: sym_##_n = sym; break;
#include "builtin_symbols.h"
#undef S
    default: assert(!"bad case");
  }
  switch (x) {
    default: break;
    case Builtin_domain:
      sym->internal = Internal_Domain;
      break;
    case Builtin_sequence:
      sym->internal = Internal_Sequence;
      break;
    case Builtin_int8:
    case Builtin_int16:
    case Builtin_int32:
    case Builtin_int64:
    case Builtin_uint8:
    case Builtin_uint16:
    case Builtin_uint32:
    case Builtin_uint64:
    case Builtin_float32:
    case Builtin_float64:
    case Builtin_float80:
    case Builtin_float128:
    case Builtin_string:
    case Builtin_function:
      sym->type_kind = Type_PRIMITIVE;
      break;
    case Builtin_any: sym->type_kind = Type_SUM; break;
    case Builtin_tuple: sym->type_kind = Type_PRODUCT; break;
    case Builtin_vector: sym->type_kind = Type_VECTOR; break;
    case Builtin_void: sym->type_kind = Type_PRODUCT; break;
    case Builtin_ref: 
      sym->type_kind = Type_REF; 
      sym->has.add(if1_alloc_sym(i, if1_cannonicalize_string(i, "ref value")));
      break;
    case Builtin_symbol: 
      if1_set_symbols_type(i); 
      sym->type_kind = Type_PRIMITIVE;
      break;
    case Builtin_continuation: 
      sym->type_kind = Type_PRIMITIVE;
      break;
  }
}

static void
build_builtin_syms(IF1 *i, AST *ast) {
  AST *ident;
  if (ast->builtin) {
    switch (ast->kind) {
      case AST_ident:
	ident = ast;
	goto Lok;
      case AST_def_fun: {
	ident = ast->get(AST_ident);
	ast->sym = ident->sym = if1_alloc_sym(i, ident->string);
	set_builtin(i, if1_make_symbol(i, ident->string), ast->builtin);
	break;
      }
      case AST_in_module:
      case AST_def_type:
      case AST_qualified_ident: {
	ident = ast->get(AST_ident);
      Lok:
	ast->sym = ident->sym = if1_alloc_sym(i, ident->string);
	ast->sym->ast = ast;
	set_builtin(i, ident->sym, ast->builtin);
	break;
      }
      case AST_const:
	ast->sym = new_constant(i, ast->string, ast->constant_type);
	set_builtin(i, ast->sym, ast->builtin);
	break;
      default: assert(!"bad case");
    }
  }
  forv_AST(a, *ast)
    build_builtin_syms(i, a);
}

static void
build_constant_syms(IF1 *i, AST *ast) {
  switch (ast->kind) {
    case AST_const:
      assert(!ast->n);
      if (!ast->sym) {
	ast->sym = new_constant(i, ast->string, ast->constant_type);
	ast->sym->ast = ast;
      }
      break;
    default:
      break;
  }
  forv_AST(a, *ast)
    build_constant_syms(i, a);
}

static Type_kind
ast_to_type(AST *ast) {
  switch (ast->kind) {
    case AST_vector_type: return Type_VECTOR;
    case AST_ref_type: return Type_REF;
    case AST_product_type: return Type_PRODUCT;
    case AST_sum_type: return Type_SUM;
    case AST_fun_type: return Type_FUN;
    case AST_tagged_type: return Type_TAGGED;
    case AST_type_application: return Type_APPLICATION;
    case AST_record_type: return Type_RECORD;
    default: break;
  }
  assert(!"bad case");
  return Type_NONE;
}

static Sym * 
make_module(IF1 *i, char *mod_name, Scope *global, Sym *sym = 0) {
  sym = new_sym(i, global, mod_name, sym);
  sym->type = sym_module;
  sym->module = 1;
  sym->scope = new Scope(global, Scope_RECURSIVE, sym);
  if (sym != sym_system)
    sym->scope->add_dynamic(sym_system->scope);
  sym->labelmap = new LabelMap;

  Sym *fun = new_sym(i, sym->scope, "__init");
  fun->scope = new Scope(sym->scope, Scope_RECURSIVE, fun);
  fun->type = sym_function;
  fun->type_kind = Type_FUN;
  fun->type_sym = fun;
  sym->init = fun;
  return sym;
}

static Sym *
in_module(IF1 *i, char *mod_name, Scope *scope, Sym *sym = 0) {
  Scope *global = scope->global();
  Sym *s = global->hash.get(mod_name);
  if (!s || !s->scope) 
    s = make_module(i, mod_name, global, sym);
  assert(!sym || s == sym);
  return s;
}

/* define modules and types and defer functions
 */
static int
define_types(IF1 *i, AST *ast, Vec<AST *> &funs, Scope *scope, int skip = 0) {
  if (!skip) {
    ast->scope = scope;
    switch (ast->kind) {
      case AST_in_module: {
	ast->sym = in_module(i, ast->get(AST_ident)->string, scope, ast->sym);
	scope = ast->sym->scope;
	break;
      }
      case AST_def_type: {
	Sym *sym = scope->get(ast->get(AST_ident)->string);
	if (sym && sym->type_kind != Type_UNKNOWN)
	  return show_error("duplicate identifier '%s'", ast, ast->get(AST_ident)->string);
	if (!sym) {
	  ast->sym = new_sym(i, scope, ast->get(AST_ident)->string, ast->sym);
	  ast->sym->ast = ast;
	} else {
	  assert(!ast->sym);
	  ast->sym = sym;
	}
	if (ast->sym->type_kind == Type_NONE || ast->sym->type_kind == Type_UNKNOWN) {
	  if (ast->n > 1)
	    ast->sym->type_kind = Type_ALIAS; // handled below
	  else
	    ast->sym->type_kind = Type_UNKNOWN;
	}
	scope = ast->sym->scope = new Scope(scope, Scope_RECURSIVE, ast->sym);
	if (verbose_level > 2)
	  printf("creating scope %X for %s\n", (int)ast->sym->scope, ast->sym->name);
	switch (ast->last()->kind) {
	  case AST_constraint:
	  case AST_def_type_param:
	  case AST_ident:
	  case AST_qualified_ident:
	    break;
	  default:
	    ast->last()->sym = ast->sym;
	    break;
	}
	break;
      }
      case AST_def_fun: // defer functions
	funs.add(ast);
	return 0;
      case AST_declare_ident: {
	Sym *sym = scope->get(ast->get(AST_ident)->string);
	if (sym && sym->type_kind != Type_UNKNOWN)
	  return show_error("duplicate identifier '%s'", ast, ast->get(AST_ident)->string);
	if (!sym)
	  sym = ast->sym = new_sym(i, scope, ast->get(AST_ident)->string, ast->sym);
	else {
	  assert(!ast->sym);;
	  ast->sym = sym;
	}
	break;
      }
      case AST_vector_type:
      case AST_ref_type:
      case AST_product_type:
      case AST_sum_type:
      case AST_fun_type:
      case AST_tagged_type:
      case AST_type_application:
      case AST_record_type:
	if (!ast->sym) {
	  ast->sym = new_sym(i, scope);
	  ast->sym->type_kind = ast_to_type(ast);
	  ast->sym->ast = ast;
	  if (ast->kind == AST_record_type)
	    scope = ast->sym->scope = new Scope(scope, Scope_RECURSIVE, scope->in);
	} else
	  ast->sym->type_kind = ast_to_type(ast);
	break;
      case AST_loop:
      case AST_with:
	ast->sym = new_sym(i, scope);
	ast->sym->ast = ast;
	scope = ast->sym->scope = new Scope(scope, Scope_RECURSIVE, scope->in);
	break;
      case AST_scope:
	scope = new Scope(scope, ast->scope_kind, scope->in);
	break;
      case AST_def_type_param:
	ast->sym = new_sym(i, scope, ast->get(AST_ident)->string);
	ast->sym->type_kind = Type_UNKNOWN;
	break;
      case AST_array_descriptor:
#if 0
	ast->sym = new_sym(i, scope);
	ast->sym->type_kind = Type_RECORD;
	ast->sym->type = ast->sym;
#else
	ast->sym = sym_array;
#endif
	break;
      default:
	break;
    }
  }
  forv_AST(a, *ast) {
    if (define_types(i, a, funs, scope) < 0)
      return -1;
    if (a->kind == AST_in_module)
      scope = a->sym->scope;
  }
  return 0;
}

static int
resolve_parameterized_type(IF1 *i, AST *ast) {
  Sym *sym;
  if (!(sym = checked_ast_qualified_ident_sym(ast->get(AST_qualified_ident))))
    return -1;
  if (ast->n > 1) {
    Sym *s = new_sym(i, ast->scope);
    s->type_kind = Type_APPLICATION;
    s->has.add(sym);
    for (int i = 1; i < ast->n; i++) {
      assert(ast->v[i]->kind == AST_type_param && ast->v[i]->sym);
      if (!(sym = checked_ast_qualified_ident_sym(ast->v[i]->get(AST_qualified_ident))))
	return -1;
      s->has.add(sym);
    }
    ast->sym = s;
  } else
    ast->sym = sym;
  return 0;
}

static void
set_scope_recursive(AST *ast, Scope *scope) {
  ast->scope = scope;
  forv_AST(a, *ast)
    set_scope_recursive(a, scope);
}

static int
scope_pattern(IF1 *i, AST *ast, Scope *scope) {
  switch (ast->kind) {
    case AST_pattern: {
      AST *ptype = ast->get(AST_pattern_type);
      ast->sym = new_sym(i, scope);
      if (ptype)
	ast->sym->type = ptype->sym;
      else
	ast->sym->type = sym_tuple;
      ast->sym->ast = ast;
      forv_AST(a, *ast) if (a != ptype) {
	if (scope_pattern(i, a, scope) < 0)
	  return -1;
	assert(a->sym);
	ast->sym->has.add(a->sym);
      }
      if (!ast->sym->has.n) {
	ast->sym = new_sym(i, scope);
	ast->sym->type = sym_tuple;
	ast->sym->ast = ast;
      } else if (ast->sym->has.n == 1)
	ast->sym = ast->sym->has.v[0];
      break;
    }
    case AST_arg:
    case AST_vararg: {
      set_scope_recursive(ast, scope);
      AST *c = ast->get(AST_const);
      AST *id = ast->get(AST_ident);
      AST *ty = ast->get(AST_constraint);
      if (ty)
	if (resolve_parameterized_type(i, ty) < 0)
	  return -1;
      if (id)
	ast->sym = id->sym = new_sym(i, scope, id->string);
      else if (c)
	ast->sym = ast->v[0]->sym;
      else
	ast->sym = new_sym(i, scope);
      ast->sym->ast = ast;
      if (ast->kind == AST_vararg)
	ast->sym->vararg = 1;
      if (ty)
	ast->sym->type = ty->sym;
      break;
    }
    default: break;
  }
  return 0;
}

static int
define_function(IF1 *i, AST *ast) {
  AST *fqid = ast->get(AST_qualified_ident);
  fqid->scope = ast->scope;
  Scope *fscope = ast_qualified_ident_scope(fqid);
  if (!fscope)
    return -1;
  AST *fid = ast_qualified_ident_ident(fqid);
  ast->sym = new_sym(i, fscope, fid->string, fqid->sym);
  Sym *tsym = new_sym(i, fscope, fid->string, if1_make_symbol(i, fid->string));
  tsym->ast = ast;
  ast->sym->scope = new Scope(ast->scope, Scope_RECURSIVE, ast->sym);
  if (fscope != ast->scope) {
    ast->sym->self = new_sym(i, ast->sym->scope, cannonical_self);
    ast->sym->self->read_only = 1;
    ast->sym->self->ast = ast;
    ast->sym->scope->add_dynamic(fscope, ast->sym->self);
  }
  for (int x = 1; x < ast->n - 1; x++)
    if (scope_pattern(i, ast->v[x], ast->sym->scope) < 0)
      return -1;
  ast->sym->cont = new_sym(i, ast->sym->scope);
  ast->sym->cont->ast = ast;
  ast->sym->labelmap = new LabelMap;
  ast->sym->ast = ast;
  return 0;
}

static int
scope_inherits(AST *ast, Sym *sym) {
  forv_AST(a, *ast) {
    if (a->kind == AST_includes || a->kind == AST_inherits) {
      if (!(a->sym = checked_ast_qualified_ident_sym(a->get(AST_qualified_ident))))
	return -1;
      sym->scope->add_dynamic(a->sym->scope);
    }
  }
  return 0;
}

static int
scope_constraints(AST *ast, Sym *sym) {
  forv_AST(a, *ast) {
    if (a->kind == AST_constraint) {
      if (!(a->sym = checked_ast_qualified_ident_sym(a->get(AST_qualified_ident))))
	return -1;
      sym->constraints.set_add(a->sym);
      sym->scope->add_dynamic(a->sym->scope);
    } else if (a->kind == AST_def_type_param) {
      sym->arg.add(a->sym);
      if (verbose_level > 2)
	printf("%s has param %s\n", sym->name, a->sym->name);
    }
  }
  return 0;
}

static int
resolve_types_and_define_recursive_functions(IF1 *i, AST *ast, int skip = 0) {
  if (!skip)
    switch (ast->kind) {
      case AST_pattern_type: {
	Sym *sym;
	if (!(sym = checked_ast_qualified_ident_sym(ast->get(AST_qualified_ident))))
	  return -1;
	ast->sym = sym;
	break;
      }
      case AST_inherits:
      case AST_implements:
      case AST_includes:
	if (resolve_parameterized_type(i, ast) < 0)
	  return -1;
	break;
      case AST_record_type:
	if (scope_inherits(ast, ast->sym) < 0) return -1;
	break;
      case AST_def_type:
	if (ast->is_value)
	  ast->sym->value = 1;
	if (scope_constraints(ast, ast->sym) < 0) return -1;
	break;
      case AST_where: {
	Sym *sym;
	if (!(sym = checked_ast_qualified_ident_sym(ast->get(AST_qualified_ident))))
	  return -1;
	if (scope_constraints(ast, sym) < 0) return -1;
	break;
      }
      case AST_def_fun:
	if (ast->scope->kind == Scope_RECURSIVE) 
	  if (define_function(i, ast) < 0)
	    return -1;
	return 0; // defer
      case AST_with: {
	AST *with_scope = ast->get(AST_with_scope);
	forv_AST(a, *with_scope) {
	  if (!a->sym)
	    if (!checked_ast_qualified_ident_sym(a))
	      return -1;
	  if (!a->sym->type)
	    show_error("with without declared type", ast);
	  ast->sym->scope->add_dynamic(a->sym->type->scope, a->sym);
	}
	break;
      }
      default: break;
    }
  forv_AST(a, *ast)
    if (resolve_types_and_define_recursive_functions(i, a) < 0)
      return -1;
  return 0;
}

static int
scope_idpattern(IF1 *i, AST *ast, Scope *scope) {
  switch (ast->kind) {
    case AST_ident: {
      Sym *sym = scope->get(ast->string);
      if (!sym || sym->type_kind != Type_UNKNOWN) {
	ast->sym = new_sym(i, scope, ast->string, ast->sym);
	ast->sym->ast = ast;
      } else {
	sym->type_kind = Type_NONE;
	ast->sym = sym;
      }
      if (ast->alt)
	ast->sym->alt = ast->alt;
    }
    break;
    case AST_pattern: {
      ast->sym = new_sym(i, scope, ast->string, ast->sym);
      AST *ptype = ast->get(AST_pattern_type);
      if (ptype)
	ast->sym->type = ptype->sym;
      else
	ast->sym->type = sym_tuple;
      ast->sym->ast = ast;
      forv_AST(a, *ast) if (a != ptype) {
	if (a->kind == AST_constraint) {
	  assert(!ast->sym->type);
	  if (resolve_parameterized_type(i, a) < 0)
	    return -1;
	  ast->sym->type = a->sym;
	} else {
	  if (scope_idpattern(i, a, scope) < 0)
	    return -1;
	  assert(a->sym);
	  ast->sym->has.add(a->sym);
	}
      }
      break;
    }
    default: break;
  }
  return 0;
}

static int
variables_and_nonrecursive_functions(IF1 *i, AST *ast, int skip = 0) {
  if (!skip)
    switch (ast->kind) {
      case AST_indices: {
	forv_AST(a, *ast) {
	  a->sym = new_sym(i, a->scope, a->string, a->sym);
	  a->sym->ast = a;
	}
	break;
      }
      case AST_def_ident: {
	AST *id = ast->v[0];
	if (scope_idpattern(i, id, ast->scope) < 0)
	  return -1;
	ast->sym = id->sym;
	break;
      }
      case AST_def_fun:
	if (ast->scope->kind != Scope_RECURSIVE) 
	  if (define_function(i, ast) < 0)
	    return -1;
	return 0;
      case AST_qualified_ident: {
	Sym *container = 0;
	if (!ast->sym)
	  if (!checked_ast_qualified_ident_sym(ast, &container))
	    return -1;
	if (container)
	  ast->container = container;
	return 0;
      }
      default: break;
    }
  forv_AST(a, *ast)
    if (variables_and_nonrecursive_functions(i, a) < 0)
      return -1;
  return 0;
}

static int
build_scopes(IF1 *i, AST *ast, Scope *scope) {
  Vec<AST *> funs;
  ast->scope = scope;
  funs.add(ast);
  while (1) {
    Vec<AST *> last_funs;
    last_funs.move(funs);
    forv_AST(a, last_funs) {
      int fun = a->kind == AST_def_fun;
      scope = fun ? a->sym->scope : a->scope;
      if (define_types(i, a, funs, scope, fun) < 0) 
	return -1;
      if (resolve_types_and_define_recursive_functions(i, a, fun) < 0) 
	return -1;
      if (variables_and_nonrecursive_functions(i, a, fun) < 0) 
	return -1;
    }
    if (!funs.n)
      break;
  }
  return 0;
}

static int
build_types(IF1 *i, AST *ast) {
  forv_AST(a, *ast)
    if (build_types(i, a) < 0)
      return -1;
  switch (ast->kind) {
    case AST_type_param:
      ast->sym = ast->v[0]->sym;
      break;
    case AST_vector_type:
    case AST_ref_type:
    case AST_product_type:
    case AST_fun_type:
    case AST_tagged_type:
    case AST_type_application:
    case AST_record_type:
      forv_AST(a, *ast)
	if (a->sym) {
	  switch (a->kind) {
	    case AST_type_param:
	      ast->sym->has.add(a->sym);
	      break;
	    case AST_inherits:
	      ast->sym->implements.add(a->v[0]->sym);
	      ast->sym->includes.add(a->v[0]->sym);
	      break;
	    case AST_implements:
	      ast->sym->implements.add(a->v[0]->sym);
	      break;
	    case AST_includes:
	      ast->sym->includes.add(a->v[0]->sym);
	      break;
	    default:
	      ast->sym->has.add(a->sym);
	      break;
	  }
	}
      break;
    case AST_sum_type:
      forv_AST(a, *ast)
	if (a->sym)
	  a->sym->implements.set_add(ast->sym);
      break;
    case AST_def_type: {
      AST *last = ast->last();
      if (ast->sym->type_kind == Type_ALIAS) {
	if (last->kind != AST_def_type_param && last->kind != AST_constraint)
	  ast->sym->alias = last->sym;
	else
	  ast->sym->type_kind = Type_UNKNOWN; // can be resolved by a "where" statement
      }
      break;
    }
    case AST_constraint:
      forv_AST(a, *ast)
	if (!ast->sym)
	  ast->sym = a->sym;
      break;
    case AST_array_descriptor:
#if 0
      ast->sym->implements.add(sym_array);
      ast->sym->includes.add(sym_array);
#endif
      break;
    default: break;
  }
  return 0;
}

static int
unalias_types(IF1 *i, AST *ast) {
  forv_AST(a, *ast)
    if (unalias_types(i, a) < 0)
      return -1;
  if (ast->sym && !ast->sym->constraints.n) {
    Sym *s = unalias_type(ast->sym);
    if (s != ast->sym) {
      s->scope = ast->sym->scope;
      ast->sym = s;
    }
  }
  return 0;
}

static int
define_labels(IF1 *i, AST *ast, LabelMap *labelmap) {
  switch (ast->kind) {
    case AST_def_ident:
      if (!ast->get(AST_pattern)) {
	if (ast->def_ident_label) {
	  ast->label[0] = if1_alloc_label(i);
	  ast->label[1] = if1_alloc_label(i);
	  labelmap->put(ast->get(AST_ident)->string, ast);
	}
      }
      break;
    case AST_in_module:
    case AST_def_fun:
      labelmap = ast->sym->labelmap;
      break;
    case AST_label:
      ast->label[0] = if1_alloc_label(i);
      ast->label[1] = ast->label[0];
      labelmap->put(ast->get(AST_ident)->string, ast);
      break;
    default: break;
  }
  forv_AST(a, *ast)
    if (define_labels(i, a, labelmap) < 0)
      return -1;
  return 0;
}

static int
resolve_labels(IF1 *i, AST *ast, LabelMap *labelmap,
	       Label *break_label = 0, Label *continue_label = 0, Label *return_label = 0) 
{
  AST *target;
  switch (ast->kind) {
    case AST_def_fun:
      labelmap = ast->sym->labelmap;
      return_label = ast->label[0] = if1_alloc_label(i);
      break;
    case AST_loop:
      continue_label = ast->label[0] = if1_alloc_label(i);
      break_label = ast->label[1] = if1_alloc_label(i);
      break;
    case AST_break:
      if ((target = ast->get(AST_ident))) {
	target = labelmap->get(target->string);
	ast->label[0] = target->label[1];
      } else
	ast->label[0] = break_label;
      break;
    case AST_continue:
      if ((target = ast->get(AST_ident))) {
	target = labelmap->get(target->string);
	ast->label[0] = target->label[0];
      } else
	ast->label[0] = continue_label;
      break;
    case AST_goto:
      target = labelmap->get(ast->get(AST_ident)->string);
      ast->label[0] = target->label[0];
    case AST_return:
      ast->label[0] = return_label;
      break;
    case AST_op:
      if (ast->v[ast->op_index]->sym->name[0] == ',')
	ast->v[0]->in_tuple = 1;
      if (ast->v[ast->op_index]->sym->name[0] == '^' && ast->v[ast->op_index]->sym->name[1] == '^')
	ast->v[0]->in_apply = 1;
      break;
    case AST_scope:
      forv_AST(a, *ast)
	a->constructor = ast->scope->kind == Scope_PARALLEL ? Make_VECTOR : Make_SET;
      break;
    default: break;
  }
  forv_AST(a, *ast)
    if (resolve_labels(i, a, labelmap, break_label, continue_label, return_label) < 0)
      return -1;
  return 0;
}

void
setup_fun(IF1 *i, Sym *fn) {
}

static void
gen_fun(IF1 *i, AST *ast) {
  Sym *fn = ast->sym;
  AST *expr = ast->last();
  Code *body = NULL, *c;
  if1_gen(i, &body, expr->code);
  if (expr->rval)
    fn->ret = expr->rval;
  else
    fn->ret = sym_null;
  if1_label(i, &body, ast, ast->label[0]);
  c = if1_send(i, &body, 3, 0, sym_reply, fn->cont, fn->ret);
  c->ast = ast;
  int n = ast->n - 2;
  AST **args = &ast->v[1];
  Sym *as[n + 2];
  int iarg = 0;
  if (ast->sym->self) {
    as[iarg] = ast->sym->self;
    if (fn->name == cannonical_class)
      as[iarg]->type = ast->sym->in->type_sym;
    else
      as[iarg]->type = ast->sym->in;
    iarg++;
  }
  if (fn->name != cannonical_class && fn->name != cannonical_self) {
    as[iarg] = new_sym(i, fn->scope);
    as[iarg]->ast = ast;
    as[iarg]->type = if1_make_symbol(i, fn->name);
    iarg++;
  }
  for (int j = 0; j < n; j++)
    as[iarg + j] = args[j]->rval;
  if1_closure(i, fn, body, iarg + n, as);
  fn->type = sym_function;
  fn->type_kind = Type_FUN;
  fn->type_sym = fn;
  fn->ast = ast;
  ast->rval = new_sym(i, ast->scope);
  if1_move(i, &ast->code, fn, ast->rval, ast);
}

static void
get_arg(IF1 *i, Code **c, AST *ast, Vec<Sym *> &args) {
  if (ast->kind != AST_qualified_ident || is_const(ast->rval))
    args.add(ast->rval);
  else {
    Sym *s = new_sym(i, ast->scope);
    if1_move(i, c, ast->rval, s, ast);
    args.add(s);
  }
}

static int
get_tuple_args(IF1 *i, Code **c, AST *ast, Vec<Sym *> &args) {
  if (ast->kind == AST_op && ast->v[ast->op_index]->sym->name[0] == ',') {
    int r = get_tuple_args(i, c, ast->v[0], args);
    get_arg(i, c, ast->v[2], args);
    return 1 + r;
  }
  get_arg(i, c, ast, args);
  return 1;
}

static int
get_apply_args(IF1 *i, Code **c, AST *ast, Vec<Sym *> &args) {
  if (ast->is_application) {
    int r = get_apply_args(i, c, ast->v[0], args);
    get_arg(i, c, ast->v[2], args);
    return 1 + r;
  }
  get_arg(i, c, ast, args);
  return 1;
}

static void
gen_new(IF1 *i, AST *ast) {
  Code **c = &ast->code;
  forv_AST(a, *ast)
    if1_gen(i, c, a->code);
  ast->rval = new_sym(i, ast->scope);
  Code *send = if1_send(i, c, 2, 1, sym_new, ast->last()->rval, ast->rval);
  send->ast = ast;
}

static Sym *
make_int(IF1 *i, int n) {
  if (n >= 0 && n < 10) {
    char c[2];
    c[0] = n + '0';
    c[1] = 0;
    return if1_const(i, sym_int, c);
  } else {
    char str[100];
    sprintf(str, "%d", n);
    return if1_const(i, sym_int, str);
  }
}

static void
gen_comma_op(IF1 *i, AST *ast, AST *a0, AST *a1) {
  Code **c = &ast->code;
  if (ast->in_tuple)
    return;
  Vec<Sym *> args;
  get_tuple_args(i, c, a0, args);
  get_arg(i, c, a1, args);
  Code *send = if1_send1(i, c);
  send->ast = ast;
  Sym *constructor;
  switch (ast->constructor) {
    case Make_TUPLE: constructor = sym_make_tuple; break;
    case Make_VECTOR: constructor = sym_make_vector; break;
    case Make_SET: constructor = sym_make_set; break;
  }
  if1_add_send_arg(i, send, constructor);
  if (ast->constructor == Make_VECTOR)
    if1_add_send_arg(i, send, make_int(i, ast->rank));
  forv_Sym(a, args)
    if1_add_send_arg(i, send, a);
  if1_add_send_result(i, send, ast->rval);
}

static void
gen_apply_op(IF1 *i, AST *ast, AST *a0, AST *a1) {
  Code **c = &ast->code;
  Sym *res = ast->rval;
  if (ast->in_apply)
    return;
  Vec<Sym *> args;
  get_apply_args(i, c, a0, args);
  if (a1)
    get_arg(i, c, a1, args);
  Code *send = if1_send1(i, c);
  send->ast = ast;
  forv_Sym(a, args)
    if1_add_send_arg(i, send, a);
  if1_add_send_result(i, send, res);
  res->lvalue = 1;
}

static void
gen_op(IF1 *i, AST *ast) {
  Code **c = &ast->code;
  Code *send = 0;
  ast->rval = new_sym(i, ast->scope);
  ast->rval->ast = ast;
  Sym *res = ast->rval;
  AST *a0 = ast->op_index ? ast->v[0] : 0, *a1 = ast->n > (int)(1 + ast->op_index) ? ast->last() : 0;
  if (a0) if1_gen(i, c, a0->code);
  if (a1) if1_gen(i, c, a1->code);
  if (ast->is_comma)
    gen_comma_op(i, ast, a0, a1);
  else if (ast->is_application) 
    gen_apply_op(i, ast, a0, a1);
  else if (ast->is_simple_assign && !a0->rval->is_var) {
    if (a0->rval->read_only)
      show_error("assignment to read-only symbol", ast);
    if1_move(i, c, a1->rval, a0->rval, ast);
    if1_move(i, c, a1->rval, res, ast);
  } else {
    Sym *args = new_sym(i, ast->scope);
    Sym *aa0 = NULL, *aa1 = NULL;
    if (a0) {
      if (is_const(a0->rval)) {
	aa0 = a0->rval;
      } else {
	Sym *s = new_sym(i, ast->scope);
	if1_move(i, c, a0->rval, s, ast);
	aa0 = s;
      }
    }
    if (a1) {
      if (is_const(a1->rval)) {
	aa1 = a1->rval;
      } else {
	Sym *s = new_sym(i, ast->scope);
	if1_move(i, c, a1->rval, s, ast);
	aa1 = s;
      }
    }
    int binary = ast->n > 2;
    int post_inc_dec = ast->is_inc_dec && ast->op_index == 1;
    if (post_inc_dec) {
      if1_move(i, c, aa0, res, ast);
      res = new_sym(i, ast->scope);
    }
    if (binary)
      send = if1_send(i, c, 4, 1, sym_make_tuple, aa0, ast->v[ast->op_index]->rval, aa1, args);
    else if (a0)
      send = if1_send(i, c, 3, 1, sym_make_tuple, aa0, ast->v[ast->op_index]->rval, args);
    else
      send = if1_send(i, c, 3, 1, sym_make_tuple, ast->v[ast->op_index]->rval, aa1, args);
    send->ast = ast;
    send = if1_send(i, c, 2, 1, sym_operator, args, res);
    send->ast = ast;
    res->lvalue = 1;
    if (ast->is_ref)
      aa1->lvalue = 1;
    if (ast->is_assign) {
      if (a0)
	if1_move(i, c, res, a0->rval, ast);
      else
	if1_move(i, c, res, a1->rval, ast);
    }
  }
}

static void
gen_indices(IF1 *i, AST *ast, AST *indices, AST *domain) {
  (void) domain;
  Sym *index = new_sym(i, ast->scope);
  index->type = sym_int;
  index->external = 1;
  forv_AST(a, *indices)
    if1_move(i, &ast->code, index, a->sym);
}

static void
gen_forall(IF1 *i, AST *ast) {
  AST *domain = ast->get(AST_domain);
  AST *indices = ast->get(AST_indices);
  AST *body = ast->last();
  if (indices)
    gen_indices(i, ast, indices, domain);
  if1_gen(i, &ast->code, body->code);
  ast->rval = new_sym(i, ast->scope);
  Code *send = if1_send(i, &ast->code, 3, 1, sym_array, domain->rval, body->rval, ast->rval);
  send->ast = ast;
}

static void
gen_loop(IF1 *i, AST *ast) {
  AST *cond = ast->get(AST_loop_cond);
  int dowhile = cond == ast->last();
  AST *body = dowhile ? ast->v[ast->n - 2] : ast->last();
  AST *before = ast->n > 2 ? ast->v[0] : 0;
  if1_loop(i, &ast->code, ast->label[0], ast->label[1],
	   cond->rval, before ? before->code : 0, 
	   cond->code, 0, body->code, ast);
  ast->rval = body->rval;
}

static void
gen_if(IF1 *i, AST *ast) {
  AST *ifcond = ast->v[0];
  AST *ifif = ast->v[1];
  AST *ifthen = ast->v[2];
  ast->rval = new_sym(i, ast->scope);
  if1_if(i, &ast->code, ifcond->code, ifcond->rval, ifif->code, ifif->rval,
	 ifthen ? ifthen->code:0, ifthen ? ifthen->rval:0, ast->rval, ast);
}

static void
gen_constructor(IF1 *i, AST *ast) {
  Sym *constructor;
  Vec<Sym *> args;
  forv_AST(a, *ast) {
    if (a->kind != AST_qualified_ident || is_const(a->rval)) {
      if1_gen(i, &ast->code, a->code);
      args.add(a->rval); 
      assert(a->rval);
    } else {
      if1_gen(i, &ast->code, a->code);
      Sym *s = new_sym(i, ast->scope);
      if1_move(i, &ast->code, a->rval, s, ast);
      args.add(s); 
    }
  }
  Code *send = if1_send1(i, &ast->code);
  send->ast = ast;
  ast->rval = new_sym(i, ast->scope);
  switch (ast->kind) {
    default: assert(!"bad case"); break;
    case AST_object: 
      if (!ast->n)
	constructor = sym_make_set;
      else
	constructor = sym_make_tuple;
      break;
    case AST_list: 
      if (!ast->n)
	constructor = sym_make_tuple;
      else
	constructor = sym_make_list;
      break;
    case AST_vector: 
      constructor = sym_make_vector; 
      break;
    case AST_index:
      constructor = sym_sequence;
      break;
  }
  if1_add_send_arg(i, send, constructor);
  if (ast->kind == AST_vector)
    if1_add_send_arg(i, send, make_int(i, ast->rank));
  for (int x = 0; x < args.n; x++)
    if1_add_send_arg(i, send, args.v[x]);
  if (ast->kind == AST_index && ast->n < 3)
    if1_add_send_arg(i, send, make_int(i, 1));
  if1_add_send_result(i, send, ast->rval);
}

static int
define_type_init(IF1 *i, AST *ast, Sym **container_scope, Sym **container) {
  AST *rec = ast->get(AST_record_type);
  if (rec) {
    Scope *scope = ast->sym->scope;
    Sym *fn = new_sym(i, scope, "__init");
    fn->ast = ast;
    fn->scope = new Scope(ast->scope, Scope_RECURSIVE, fn);
    assert(!ast->sym->init);
    ast->sym->init = fn;
    fn->ret = sym_null;
    fn->cont = new_sym(i, fn->scope);
    fn->cont->ast = ast;
    fn->self = new_sym(i, fn->scope, cannonical_self);
    fn->self->ast = ast;
    fn->self->type = ast->sym;
    fn->type = sym_function;
    fn->type_kind = Type_FUN;
    fn->type_sym = fn;
    *container_scope = ast->sym;
    *container = fn->self;
  }
  return 0;
}

static int
pre_gen_top_down(IF1 *i, AST *ast, Sym *container_scope, Sym *container = 0) {
  switch (ast->kind) {
    default: break;
    case AST_def_type: 
      if (define_type_init(i, ast, &container_scope, &container) < 0)
	return -1;
      break;
    case AST_def_fun:
      container_scope = container = 0;
      break;
    case AST_def_ident:
    case AST_qualified_ident:
      if (container_scope && ast->sym->in == container_scope) {
	assert(!ast->container);
	ast->container = container;
      }
      break;
  }
  forv_AST(a, *ast)
    if (pre_gen_top_down(i, a, container_scope, container) < 0)
      return -1;
  return 0;
}

static int
pre_gen_bottom_up(AST *ast) {
  forv_AST(a, *ast)
    if (pre_gen_bottom_up(a) < 0)
      return -1;
  switch (ast->kind) {
    case AST_vector: {
      uint rank = 0;
      forv_AST(a, *ast)
	if (a->rank > rank)
	  rank = a->rank;
      if (ast->n > 1)
	ast->rank = rank + 1;
      else
	ast->rank = rank;
      break;
    }
    case AST_block: {
      ast->rank = ast->last()->rank;
      break;
    }
    case AST_op: {
      char *op = ast->v[ast->op_index]->sym->name;
      ast->is_inc_dec = (op[0] == '+' && op[1] == '+') || (op[0] == '-' && op[1] == '-');
      ast->is_simple_assign = ((op[0] == '=') && !op[1]);
      ast->is_assign = (op[1] == '=' && op[0] != '=' && op[0] != '<' && op[0] != '>') 
	|| ast->is_simple_assign || ast->is_inc_dec;
      ast->is_ref = op[0] == '&' && ast->op_index == 0;
      ast->is_application = (op[0] == '^' && op[1] == '^') || op[0] == '(';
      ast->is_comma = op[0] == ',';
      if (ast->is_comma) {
	uint rank = 1;
	forv_AST(a, *ast)
	  if (!a->in_tuple) {
	    if (a->rank + 1 > rank)
	      rank = a->rank + 1;
	  } else
	    if (a->rank > rank)
	      rank = a->rank;
	ast->rank = rank;
      }
      break;
    }
    default: break;
  }
  return 0;
}

static void
gen_assign(IF1 *i, AST *ast, AST *val, Sym *in, Sym *out) {
  Sym *args = new_sym(i, ast->scope);
  Code *send = if1_send(i, &ast->code, 4, 1, sym_make_tuple, in, sym_assign, val->rval, args);
  send->ast = ast;
  send = if1_send(i, &ast->code, 2, 1, sym_operator, args, out);
  send->ast = ast;
}

static Sym *
value_type(IF1 *i, AST *ast, Sym *s) {
  Sym *v = new_sym(i, ast->scope);
  v->type = s;
  v->external = 1;
  return v;
}

static void
gen_def_ident_value(IF1 *i, AST *ast, AST *constraint, AST *val) {
  // arrays
  AST *arr = constraint->get(AST_array_descriptor);
  if (arr) { 
    Sym *rval = new_sym(i, ast->scope);
    Sym *domain = NULL, *init = NULL;
    AST *arr_dom = arr->get(AST_domain);  // if the domain is declared use it
    if (arr_dom)
      domain = arr_dom->rval;
    AST *indices = arr->get(AST_indices);
    AST *element_type = constraint->last();
    if (element_type != arr)
      init = value_type(i, ast, element_type->sym);
    if (val) {
      if (!domain && val->kind == AST_forall) { // if it has a domain and we need one, use it
	AST *domain_ast = val->get(AST_domain);
	domain = domain_ast->rval;
	AST *element = val->last();
	if (!init && element != domain_ast && element->kind == AST_const)
	  init = element->rval;
      } else if (!init && val->kind == AST_const) // if it has a scalar initializer, use it
	init = val->rval;
    }
    if (domain) {
      Code *send;
      if (init)
	send = if1_send(i, &ast->code, 3, 1, sym_array, domain, init, rval);
      else
	send = if1_send(i, &ast->code, 2, 1, sym_array, domain, rval);
      send->ast = ast;
    } else {
      show_error("missing domain in array initializer", ast);
    }
    if (indices)
      gen_indices(i, ast, indices, arr_dom);
    if ((!init || !init->constant) && val) {
      if (val)
	if1_gen(i, &ast->code, val->code);
      Sym *rrval = new_sym(i, ast->scope);
      gen_assign(i, ast, val, rval, rrval);
      rval = rrval;
    }
    if1_move(i, &ast->code, rval, ast->rval, ast);
  }
    
  // other values
  else { 
    if (val)
      if1_gen(i, &ast->code, val->code);
    Sym *declared_type = constraint->sym;
    if (declared_type->num_type) { // numbers
      if (val && val->kind == AST_const && val->sym->type == declared_type)
	if1_move(i, &ast->code, val->sym, ast->sym, ast);
      else {
	Sym *rval = value_type(i, ast, declared_type);
	if (val) {
	  Sym *rrval = new_sym(i, ast->scope);
	  gen_assign(i, ast, val, rval, rrval);
	  rval = rrval;
	}
	if1_move(i, &ast->code, rval, ast->rval, ast);
      }
    } else {
      Sym *rval = new_sym(i, ast->scope);
      Code *send = if1_send(i, &ast->code, 2, 1, sym_new, declared_type, rval);
      send->ast = ast;
      if (val) {
	Sym *rrval = new_sym(i, ast->scope);
	gen_assign(i, ast, val, rval, rrval);
	rval = rrval;
      }
      if1_move(i, &ast->code, rval, ast->rval, ast);
    }
  }
}

static Sym *
gen_container(IF1 *i, AST *ast) {
  Sym *rval = new_sym(i, ast->scope);
  Code *send = if1_send(i, &ast->code, 4, 1, sym_primitive, 
			ast->container, sym_period, if1_make_symbol(i, ast->sym->name), 
			rval);
  send->ast = ast;
  rval->lvalue = 1;
  return rval;
}

static void
gen_def_ident(IF1 *i, AST *ast) {
  AST *constraint = ast->get(AST_constraint);
  AST *pattern = ast->get(AST_pattern);
  AST *val = ast->last();
  if (val == constraint)
    val = 0;
  if (val && val->sym && val->sym->type_kind) {
    assert(!constraint);
    constraint = val;
    val = 0;
  }
  if (ast->container)
    ast->rval = gen_container(i, ast);
  else
    ast->rval = ast->sym;
  if (constraint) {
    assert(constraint->sym);
    ast->sym->type = constraint->sym;
  }
  if (ast->sym != sym_init) { // don't init the initial function
    // declared to be a value type
    if (constraint && constraint->sym->value) 
      gen_def_ident_value(i, ast, constraint, val);
    else {
      if (val)
	if1_gen(i, &ast->code, val->code);
      if (val && val->rval) {
	if (pattern) {
	  Code *s = if1_send(i, &ast->code, 2, 1, sym_destruct, val->rval, ast->rval);
	  s->ast = ast;
	} else
	  if1_move(i, &ast->code, val->rval, ast->rval, ast); 
      }
    }
  }
  if (ast->is_var) {
    ast->rval->lvalue = 1;
    ast->rval->is_var = 1;
  }
  if (ast->is_const)
    ast->rval->single_assign = 1;
}

static void
gen_type(IF1 *i, AST *ast) {
  AST *rec = ast->get(AST_record_type);
  if (rec) {
    // build __init function
    Sym *fn = ast->sym->init;
    Code *body = NULL, *c;
    forv_Sym(s, ast->sym->includes) {
      Sym *tself = new_sym(i, fn->scope);
      tself->aspect = s;
      if1_move(i, &body, ast->sym->init->self, tself, ast); 
      Sym *rval = new_sym(i, fn->scope);
      Code *send = if1_send(i, &body, 2, 1, tself, s->init, rval);
      send->ast = ast;
    }
    if1_gen(i, &body, rec->code);
    if1_label(i, &body, ast, ast->label[0]);
    c = if1_send(i, &body, 3, 0, sym_reply, fn->cont, fn->ret);
    c->ast = ast;
    Sym *as[2];
    as[0] = fn->self;
    as[1] = new_sym(i, fn->scope);
    as[1]->ast = ast;
    as[1]->type = if1_make_symbol(i, fn->name);
    if1_closure(i, fn, body, 2, as);
    ast->rval = new_sym(i, ast->scope);
    if1_move(i, &ast->code, fn, ast->rval, ast);
  }
}

static int
gen_if1(IF1 *i, AST *ast) {
  // bottom's up
  forv_AST(a, *ast)
    if (gen_if1(i, a) < 0)
      return -1;
  switch (ast->kind) {
    case AST_def_type: gen_type(i, ast); break;
    case AST_def_fun: gen_fun(i, ast); break;
    case AST_def_ident: gen_def_ident(i, ast); break;
    case AST_pattern: {
      ast->rval = ast->sym; 
      AST *ptype = ast->get(AST_pattern_type);
      if (ast->n > (ptype ? 2 : 1))
	ast->rval->pattern = 1;
      break;
    }
    case AST_qualified_ident:
      if (ast->container) {
	ast->rval = gen_container(i, ast);
	break;
      } else
	// fall through
	;
    case AST_const:
    case AST_arg: 
    case AST_vararg: 
      ast->rval = ast->sym;
      if (ast->is_var)
	ast->rval->lvalue = 1;
      break;
    case AST_list:
    case AST_vector:
    case AST_object:
    case AST_index:
       gen_constructor(i, ast); break;
    case AST_scope:
    case AST_block:
      forv_AST(a, *ast)
	if1_gen(i, &ast->code, a->code);
      if (ast->n)
	ast->rval = ast->last()->rval; 
      break;
    case AST_indices:
      forv_AST(a, *ast)
	a->rval = ast->sym;
      break;
    case AST_cross_product:
      if1_gen(i, &ast->code, ast->last()->code);
      ast->rval = ast->last()->rval;
      break;
    case AST_domain:
      ast->rval = ast->last()->rval;
      break;
    case AST_forall: gen_forall(i, ast); break;
    case AST_loop: gen_loop(i, ast); break;
    case AST_op: gen_op(i, ast); break;
    case AST_new: gen_new(i, ast); break;
    case AST_if: gen_if(i, ast); break;
    default: 
      if (ast->n == 1) {
	if (ast->code)
	  if1_gen(i, &ast->code, ast->v[0]->code);
	else
	  ast->code = ast->v[0]->code;
	ast->rval = ast->v[0]->rval;
      } else
	forv_AST(a, *ast)
	  if1_gen(i, &ast->code, a->code);
      break;
  }
  return 0;
}

static Sym *
collect_module_init(IF1 *i, AST *ast, Sym *mod) {
  forv_AST(a, *ast) 
    switch (a->kind) {
      case AST_in_module: 
	mod = a->sym->init; break;
      case AST_block: 
	forv_AST(a, *ast)
	  mod = collect_module_init(i, a, mod);
	break;
      default: 
	if1_gen(i, &mod->code, a->code);
	if (!mod->ast)
	  mod->ast = new AST(AST_block);
	mod->ast->add(a);
	mod->ret = a->rval;
	break;
    }
  return mod;
}

static int
build_functions(IF1 *i, AST *ast, Sym *mod) {
  if (define_labels(i, ast, mod->labelmap) < 0) return -1;
  if (resolve_labels(i, ast, mod->labelmap) < 0) return -1;
  if (pre_gen_top_down(i, ast, 0, 0) < 0) return -1;
  if (pre_gen_bottom_up(ast) < 0) return -1;
  if (gen_if1(i, ast) < 0) return -1;
  collect_module_init(i, ast, mod->init);
  return 0;
}

static void
build_modules(IF1 *i) {
  forv_Sym(s, i->allsyms) {
    if (s->module) {
      Sym *fn = s->init;
      fn->ret = sym_null;
      fn->cont = new_sym(i, fn->scope);
      Code *body = NULL;
      if1_gen(i, &body, fn->code);
      if1_send(i, &body, 3, 0, sym_reply, fn->cont, fn->ret);
      Sym *as = new_sym(i, fn->scope);
      as->type = fn;
      if1_closure(i, fn, body, 1, &as);
    }
  }
}

AST *
ast_qid(Sym *s) {
  AST *a = new AST(AST_qualified_ident);
  a->sym = s;
  return a;
}

AST *
ast_symbol(IF1 *i, char *s) {
  AST *a = new AST(AST_const);
  a->sym = if1_make_symbol(i, s);
  return a;
}

AST *
ast_call(IF1 *i, int n, ...) {
  assert(n > 0);
  va_list ap;
  va_start(ap, n);
  Sym *s = va_arg(ap, Sym *);
  AST *a = ast_qid(s);
  for (int x = 1; x < n; x++) {
    AST *aa = new AST(AST_op);
    aa->add(a);
    aa->add(ast_symbol(i, "^^"));
    aa->add(ast_qid(va_arg(ap, Sym *)));
    a = aa;
  }
  if (n == 1) {
    AST *aa = new AST(AST_op);
    aa->add(a);
    aa->add(ast_symbol(i, "^^"));
    a = aa;
  }
  va_end(ap);
  return a;
}

static void
build_init(IF1 *i) {
  Sym *fn = sym_init;
  fn->type = sym_function;
  fn->type_kind = Type_FUN;
  fn->type_sym = fn;
  fn->scope = new Scope(fn->ast->scope, Scope_RECURSIVE, fn);
  Sym *rval = 0;
  Code *body = 0;
  AST *ast = new AST(AST_block);
  forv_Sym(s, i->allsyms)
    if (s->module) {
      rval = new_sym(i, fn->scope);
      Code *send = if1_send(i, &body, 1, 1, s->init, rval);
      send->ast = ast_call(i, 1, s->init);
      ast->add(send->ast);
    }
  fn->cont = new_sym(i, fn->scope);
  fn->ret = sym_null;
  if1_send(i, &body, 3, 0, sym_reply, fn->cont, fn->ret);
  Sym *as = if1_make_symbol(i, fn->name);
  if1_closure(i, fn, body, 1, &as);
  fn->ast = ast;
}

static void
global_asserts() {
  assert(Scope_INHERIT == D_SCOPE_INHERIT && Scope_RECURSIVE == D_SCOPE_RECURSIVE);
  assert(Scope_PARALLEL == D_SCOPE_PARALLEL && Scope_SEQUENTIAL == D_SCOPE_SEQUENTIAL);
}

static void
unalias_implements_constraints(IF1 *i) {
  // unalias
  forv_Sym(s, i->allsyms) {
    for (int x = 0; x < s->implements.n; x++)
      if (s->implements.v[x])
	s->implements.v[x] = unalias_type(s->implements.v[x]);
    for (int x = 0; x < s->constraints.n; x++)
      if (s->constraints.v[x])
	s->constraints.v[x] = unalias_type(s->constraints.v[x]);
    s->type = unalias_type(s->type);
    s->in = unalias_type(s->in);
    if (s->type_kind != Type_NONE)
      s->type = s;
  }
  forv_Sym(s, i->allsyms) {
    Sym *us = unalias_type(s);
    if (s != us) {
      forv_Sym(ss, s->implements) if (ss) {
	assert(ss != us);
	us->implements.set_add(ss);
      }
    }
  }
}

static void 
closure_of_implements(IF1 *i) {
  int changed = 1;
  while (changed) {
    changed = 0;
    forv_Sym(s, i->allsyms) {
      if (s->implements.n) {
	Vec<Sym *> supers(s->implements);
	forv_Sym(ss, supers) if (ss)
	  forv_Sym(sss, ss->implements) if (sss)
	    changed = s->implements.set_add(sss) || changed;
      }
    }
  }
  forv_Sym(s, i->allsyms)
    s->implements.set_to_vec();
}

static void
collect_includes(Sym *s, Vec<Sym *> &include_set, Vec<Sym *> &includes, Vec<Sym *> &in_includes) {
  if (!include_set.in(s) && in_includes.set_add(s)) {
    forv_Sym(ss, s->includes)
      collect_includes(ss, include_set, includes, in_includes);
    if (include_set.set_add(s))
      includes.add(s);
  }
}

static void
collect_include_vars(Sym *s, Sym *in = 0) {
  Vec<Sym *> saved;
  if (!in)
    saved.move(s->has);
  else
    in->has.append(s->has);
  forv_Sym(ss, s->includes)
    collect_include_vars(ss, in ? in : s);
  if (!in)
    s->has.append(saved);
}

static void
include_instance_variables(IF1 *i) {
  Vec<Sym *> include_set, includes;
  forv_Sym(s, i->allsyms) {
    Vec<Sym *> in_includes;
    if (s->includes.n)
      collect_includes(s, include_set, includes, in_includes);
  }
  forv_Sym(s, includes) 
    if (s->includes.n)
      collect_include_vars(s);
}

static void
set_value_for_value_classes(IF1 *i) {
  sym_value->value = 1;
  sym_anynum->value = 1;
  Vec<Sym *> implementers;
  forv_Sym(s, i->allsyms) {
    if (s->implements.n)
      implementers.add(s);
  }	
  int changed = 1;
  while (changed) {
    changed = 0;
    forv_Sym(s, implementers)
      forv_Sym(ss, s->implements)
        if (ss->value && !s->value) { 
          changed = 1;
	  s->value = 1;
        }
  }
}

static void
make_type_syms(IF1 *i) {
  sym_anyclass->type_sym = sym_anyclass;
  forv_Sym(s, i->allsyms) {
    if (s->type_kind) {
      if (!s->type_sym) {
	s->type_sym = new_sym(i);
	s->type_sym->meta = 1;
	s->type_sym->in = s->in;
	s->type_sym->name = s->name;
	s->type_sym->type = s->type_sym;
	s->type_sym->ast = s->ast;
	s->type_sym->type_sym = s;
	s->type_sym->type_kind = Type_PRIMITIVE;
      }
    }
  }
}

static void
finalize_types(IF1 *i) {
  unalias_implements_constraints(i);
  closure_of_implements(i);
  include_instance_variables(i);
  // unalias builtin symbols
#define S(_n) sym_##_n = unalias_type(sym_##_n);
#include "builtin_symbols.h"
#undef S
  set_value_for_value_classes(i);
  make_type_syms(i);
}

int
ast_gen_if1(IF1 *i, Vec<AST *> &av) {
  Scope *global = new Scope();
  cannonical_class = if1_cannonicalize_string(i, "class");
  cannonical_self = if1_cannonicalize_string(i, "self");
  cannonical_folded = if1_cannonicalize_string(i, "< folded >");
  global_asserts();
  forv_AST(a, av)
    build_builtin_syms(i, a);
#define S(_n) assert(sym_##_n);
#include "builtin_symbols.h"
#undef S
  set_primitive_types(i);
  make_module(i, sym_system->name, global, sym_system);
  forv_AST(a, av)
    build_constant_syms(i, a);
  Sym *user_mod = in_module(i, if1_cannonicalize_string(i, "user"), global);
  Scope *scope = user_mod->scope;
  forv_AST(a, av)
    if (build_scopes(i, a, scope) < 0) 
      return -1;
  forv_AST(a, av)
    if (build_types(i, a) < 0) 
      return -1;
  forv_AST(a, av)
    if (unalias_types(i, a) < 0) 
      return -1;
  finalize_types(i);
  forv_AST(a, av)
    if (ast_constant_fold(i, a) < 0)
      return -1;
  forv_AST(a, av)
    if (build_functions(i, a, user_mod) < 0) 
      return -1;
  build_modules(i);
  build_init(i);
  return 0;
}
