/*
  Copyright 2003-2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include <ctype.h>
#include "parse.h"
#include "geysa.h"
#include "ast.h"
#include "parse_ast.h"
#include "if1.h"
#include "scope.h"
#include "prim.h"
#include "builtin.h"
#include "fa.h"
#include "dump.h"
#include "var.h"
#include "graph.h"
#include "fun.h"
#include "pdb.h"
#include "pnode.h"

//#define TEST_RETURN 1

class LabelMap : public Map<char *, ParseAST *> {};

char *AST_name[] = {
#define S(_x) #_x,
#include "ast_kinds.h"
#undef S
};

char *cannonical_class = 0;
char *cannonical_self = 0;
char *cannonical_folded = 0;
Sym *operator_symbol = 0;
Sym *print_symbol = 0;

static void
dump_ast_tree(FILE *fp, Fun *f, ParseAST *a, int indent = 0) {
  switch (a->kind) {
    case AST_def_fun: return;
    case AST_const: 
    case AST_def_ident: 
      if (!a->sym->var || !a->sym->var->avars.n)
	return;
      break;
    case AST_def_type:
      if (!unalias_type(a->sym)->creators.n)
	return;
      break;
    default: break;
  }
  for (int i = 0; i < indent; i++) putc(' ', fp);
  fprintf(fp, "<LI>%s ", AST_name[a->kind]);
  if (a->sym) {
    if (a->sym->constant) {
      if (!a->sym->type->num_kind)
	fprintf(fp, " constant %s", a->sym->constant);
      else {
	fprintf(fp, " constant ");
	print(fp, a->sym->imm, a->sym->type);
      }
    } else if (a->sym->is_symbol)
      fprintf(fp, " symbol %s", a->sym->name);
    else if (a->sym->name)
      fprintf(fp, " sym %s", a->sym->name);
    else 
      fprintf(fp, " id(%d)", a->sym->id);
  }
  Sym *s = a->sym;
  if (!s)
    s = a->rval;
  Sym *t = type_info(a, s);
  if (t) {
    fprintf(fp, " : ");
    dump_sym_name(fp, t);
  }
  if (a->string)
    fprintf(fp, " %s", a->string);
  if (a->builtin)
    fprintf(fp, " builtin %s", a->builtin);
  if (!a->sym || !a->sym->constant) {
    Vec<Sym *> consts;
    if (constant_info(a, consts, s)) {
      fprintf(fp, " constants {");
      forv_Sym(s, consts) {
	fprintf(fp, " ");
	print(fp, s->imm, s->type);
      }
      fprintf(fp, " }");
    }
  }
  Vec<Fun *> funs;
  call_info(f, a, funs);
  if (funs.n) {
    fprintf(fp, " calls ");
    dump_fun_list(fp, funs);
  }
  if (a->prim)
    fprintf(fp, " primitive %s", a->prim->name);
  fputs("\n", fp);
  if (a->children.n) {
    for (int i = 0; i < indent; i++) putc(' ', fp);
    fprintf(fp, "<UL>\n");
    forv_ParseAST(aa, a->children)
      dump_ast_tree(fp, f, aa, indent + 1);
    for (int i = 0; i < indent; i++) putc(' ', fp);
    fprintf(fp, "</UL>\n");
  }
}

void
ParseAST::dump(FILE *fp, Fun *f) {
  if (kind == AST_def_fun)
    dump_ast_tree(fp, f, last());
  else
    dump_ast_tree(fp, f, this);
}

static int 
graph_it(ParseAST *a) {
  switch (a->kind) {
    case AST_def_fun: return 0;
    case AST_const: 
    case AST_def_ident: 
      if (!a->sym->var || !a->sym->var->avars.n)
	return 0;
      break;
    case AST_def_type:
      if (!unalias_type(a->sym)->creators.n)
	return 0;
      break;
    default: break;
  }
  return 1;
}

static void
graph_ast_nodes(FILE *fp, ParseAST *a) {
  graph_node(fp, a, AST_name[a->kind]);
  forv_ParseAST(aa, a->children)
    if (graph_it(aa))
      graph_ast_nodes(fp, aa);
}

static void
graph_ast_edges(FILE *fp, ParseAST *a) {
  forv_ParseAST(aa, a->children)
    if (graph_it(aa)) {
      graph_edge(fp, a, aa);
      graph_ast_edges(fp, aa);
    }
}

void
ParseAST::graph(FILE *fp) {
  ParseAST *ast = this;
  if (kind == AST_def_fun)
    ast = last();
  graph_ast_nodes(fp, ast);
  graph_ast_edges(fp, ast);
}

char *
ParseAST::pathname() {
  if (_pathname)
    return _pathname;
  return "<unknown>";
}

int
ParseAST::line() {
  return _line;
}

ParseAST *
new_AST(AST_kind k, D_ParseNode *pn) {
  return new ParseAST(k, pn);
}

ParseAST *
ParseAST::get(AST_kind k) {
  for (int i = 0; i < children.n; i++)
    if (children.v[i]->kind == k)
      return children.v[i];
  return NULL;
}

void 
ParseAST::add(ParseAST *a) { 
  if (a) { 
    if (!_line) {
      _pathname = a->pathname();
      _line = a->line();
    }
    a->parent = this;
    children.add(a); 
  }
}

static void
dig_ast(ParseAST *ast, D_ParseNode *pn) {
  if (pn->user.ast)
    ast->add(pn->user.ast);
  else
    for (int i = 0; i < d_get_number_of_children(pn); i++)
      dig_ast(ast, d_get_child(pn, i));
}

void
ParseAST::add(D_ParseNode *pn) {
  dig_ast(this, pn);
} 

void
ParseAST::set_location(D_ParseNode *pn) {
  scope_kind = pn->scope->kind;
  _pathname = pn->start_loc.pathname;
  _line = pn->start_loc.line;
}

ParseAST *
copy_internal(ParseAST *ast, Map<PNode *, PNode*> *nmap) {
  ParseAST *a = new ParseAST(*ast);
  if (nmap)
    for (int i = 0; i < a->pnodes.n; i++)
      a->pnodes.v[i] = nmap->get(a->pnodes.v[i]);
  for (int i = 0; i < a->children.n; i++)
    a->children.v[i] = copy_internal(a->children.v[i], nmap);
  return a;
}

AST *
ParseAST::copy(Map<PNode *, PNode*> *nmap) {
  return copy_internal(this, nmap);
}

ParseAST::ParseAST(AST_kind k, D_ParseNode *pn) {
  // count on GC to clear the object ??
  kind = k;
  if (pn) {
    set_location(pn);
    add_below(pn);
  }
}

void
ParseAST::set_location_and_add(D_ParseNode *pn) {
  set_location(pn);
  add(pn);
}

void
ParseAST::add_below(D_ParseNode *pn) {
  for (int i = 0; i < d_get_number_of_children(pn); i++)
    dig_ast(this, d_get_child(pn, i));
}

static inline ParseAST *
ast_qualified_ident_ident(ParseAST *x) { 
  return x->last();
}

static char *
ast_qualified_ident_string(ParseAST *ast) {
  char *s = ast->children.v[0]->string;
  s = s ? s : (char*)""; // global
  for (int i = 1; i < ast->children.n; i++) {
    char *ss = (char*)MALLOC(strlen(s) + strlen(ast->children.v[i]->string) + 3);
    strcpy(ss, s);
    strcat(ss,"::");
    strcat(ss, ast->children.v[i]->string);
    s = ss;
  }
  return s;
}

static Scope *
ast_qualified_ident_scope(ParseAST *a) {
  int i = 0;
  Scope *s = a->scope;
  if (a->children.v[0]->kind == AST_global) {
    i = 1;
    s = s->global();
  }
  for (int x = i; x < a->children.n - 1; x++) {
    Sym *sym = s->get(a->children.v[x]->string);
    if (!sym || !sym->scope) {
      show_error("unresolved identifier qualifier '%s'", a, a->children.v[x]->string);
      return 0;
    }
    s = sym->scope;
  }
  return s;
}

static Sym *
ast_qualified_ident_sym(ParseAST *a, Sym **container) {
  Scope *s = ast_qualified_ident_scope(a);
  if (!s)
    return NULL;
  ParseAST *id = ast_qualified_ident_ident(a);
  return s->get(id->string, container);
}

Sym *
checked_ast_qualified_ident_sym(ParseAST *ast, Sym **container = 0) {
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
ast_print(FILE *fp, ParseAST *a, int indent) {
  SP(fp, indent);
  fprintf(fp, "%s", AST_name[a->kind]);
  if (a->sym) {
    if (a->sym->is_constant)
      fprintf(fp, " constant %s", a->sym->constant);
    else if (a->sym->is_symbol)
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
ast_pp(ParseAST *a) {
  ast_print_recursive(stdout, a, 0);
}

void
ast_print_recursive(FILE *fp, ParseAST *a, int indent) {
  ast_print(fp, a, indent);
  forv_ParseAST(aa, a->children)
    ast_print_recursive(fp, aa, indent + 1);
}

void
ast_write(ParseAST *a, char *filename) {
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
build_builtin_syms(IF1 *i, ParseAST *ast) {
  ParseAST *ident;
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
	ast->sym->ast = ast;
	set_builtin(i, ast->sym, ast->builtin);
	break;
      default: assert(!"bad case");
    }
  }
  forv_ParseAST(a, ast->children)
    build_builtin_syms(i, a);
}

static void
build_constant_syms(IF1 *i, ParseAST *ast) {
  switch (ast->kind) {
    case AST_const:
      assert(!ast->children.n);
      if (!ast->sym)
	ast->sym = new_constant(i, ast->string, ast->constant_type);
      break;
    default:
      break;
  }
  forv_ParseAST(a, ast->children)
    build_constant_syms(i, a);
}

static Type_kind
ast_to_type(ParseAST *ast) {
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
  sym->scope = new Scope(global, Scope_RECURSIVE, sym);
  sym->labelmap = new LabelMap;
  if (sym != sym_system)
    sym->scope->add_dynamic(sym_system->scope);
  Sym *fun = new_sym(i, sym->scope, "__init");
  fun->scope = new Scope(sym->scope, Scope_RECURSIVE, fun);
  build_module(sym, fun);
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
define_types(IF1 *i, ParseAST *ast, Vec<ParseAST *> &funs, Scope *scope, int skip = 0) {
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
	  if (ast->children.n > 1)
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
  forv_ParseAST(a, ast->children) {
    if (define_types(i, a, funs, scope) < 0)
      return -1;
    if (a->kind == AST_in_module)
      scope = a->sym->scope;
  }
  return 0;
}

static int
resolve_parameterized_type(IF1 *i, ParseAST *ast) {
  Sym *sym;
  if (!(sym = checked_ast_qualified_ident_sym(ast->get(AST_qualified_ident))))
    return -1;
  if (ast->children.n > 1) {
    Sym *s = new_sym(i, ast->scope);
    s->type_kind = Type_APPLICATION;
    s->has.add(sym);
    for (int i = 1; i < ast->children.n; i++) {
      assert(ast->children.v[i]->kind == AST_type_param && ast->children.v[i]->sym);
      if (!(sym = checked_ast_qualified_ident_sym(ast->children.v[i]->get(AST_qualified_ident))))
	return -1;
      s->has.add(sym);
    }
    ast->sym = s;
  } else
    ast->sym = sym;
  return 0;
}

static void
set_scope_recursive(ParseAST *ast, Scope *scope) {
  ast->scope = scope;
  forv_ParseAST(a, ast->children)
    set_scope_recursive(a, scope);
}

static int
scope_pattern(IF1 *i, ParseAST *ast, Scope *scope) {
  switch (ast->kind) {
    case AST_pattern: {
      ParseAST *ptype = ast->get(AST_pattern_type);
      ast->sym = new_sym(i, scope);
      if (ptype)
	ast->sym->type = ptype->sym;
      else
	ast->sym->type = sym_tuple;
      ast->sym->ast = ast;
      forv_ParseAST(a, ast->children) if (a != ptype) {
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
      ParseAST *c = ast->get(AST_const);
      ParseAST *id = ast->get(AST_ident);
      ParseAST *ty = ast->get(AST_constraint);
      if (ty)
	if (resolve_parameterized_type(i, ty) < 0)
	  return -1;
      if (id)
	ast->sym = id->sym = new_sym(i, scope, id->string);
      else if (c)
	ast->sym = ast->children.v[0]->sym;
      else
	ast->sym = new_sym(i, scope);
      ast->sym->ast = ast;
      if (ast->kind == AST_vararg)
	ast->sym->is_vararg = 1;
      if (ty)
	ast->sym->type = ty->sym;
      break;
    }
    default: break;
  }
  return 0;
}

static int
define_function(IF1 *i, ParseAST *ast) {
  ParseAST *fqid = ast->get(AST_qualified_ident);
  fqid->scope = ast->scope;
  Scope *fscope = ast_qualified_ident_scope(fqid);
  if (!fscope)
    return -1;
  ParseAST *fid = ast_qualified_ident_ident(fqid);
  ast->sym = new_sym(i, fscope, fid->string, fqid->sym);
  Sym *tsym = new_sym(i, fscope, fid->string, if1_make_symbol(i, fid->string));
  tsym->ast = ast;
  ast->sym->scope = new Scope(ast->scope, Scope_RECURSIVE, ast->sym);
  if (fscope != ast->scope) {
    ast->sym->self = new_sym(i, ast->sym->scope, cannonical_self);
    ast->sym->self->is_read_only = 1;
    ast->sym->self->ast = ast;
    ast->sym->scope->add_dynamic(fscope, ast->sym->self);
  }
  for (int x = 1; x < ast->children.n - 1; x++)
    if (scope_pattern(i, ast->children.v[x], ast->sym->scope) < 0)
      return -1;
  ast->sym->cont = new_sym(i, ast->sym->scope);
  ast->sym->cont->ast = ast;
  ast->sym->labelmap = new LabelMap;
  ast->sym->ast = ast;
#ifdef TEST_RETURN
  ast->sym->ret = new_sym(i, ast->sym->scope);
  ast->sym->ret->ast = ast;
  ast->sym->ret->is_lvalue = 1;
#endif
  return 0;
}

static int
scope_inherits(ParseAST *ast, Sym *sym) {
  forv_ParseAST(a, ast->children) {
    if (a->kind == AST_includes || a->kind == AST_inherits) {
      if (!(a->sym = checked_ast_qualified_ident_sym(a->get(AST_qualified_ident))))
	return -1;
      sym->scope->add_dynamic(a->sym->scope);
    }
  }
  return 0;
}

static int
scope_constraints(ParseAST *ast, Sym *sym) {
  forv_ParseAST(a, ast->children) {
    if (a->kind == AST_constraint) {
      if (!(a->sym = checked_ast_qualified_ident_sym(a->get(AST_qualified_ident))))
	return -1;
      if (!sym->constraints)
	sym->constraints = new Vec<Sym *>;
      sym->constraints->set_add(a->sym);
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
resolve_types_and_define_recursive_functions(IF1 *i, ParseAST *ast, int skip = 0) {
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
	  ast->sym->is_value_class = 1;
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
	ParseAST *with_scope = ast->get(AST_with_scope);
	forv_ParseAST(a, with_scope->children) {
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
  forv_ParseAST(a, ast->children)
    if (resolve_types_and_define_recursive_functions(i, a) < 0)
      return -1;
  return 0;
}

static int
scope_idpattern(IF1 *i, ParseAST *ast, Scope *scope) {
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
      if (ast->alt_name)
	ast->sym->alt_name = ast->alt_name;
    }
    break;
    case AST_pattern: {
      ast->sym = new_sym(i, scope, ast->string, ast->sym);
      ParseAST *ptype = ast->get(AST_pattern_type);
      if (ptype)
	ast->sym->type = ptype->sym;
      else
	ast->sym->type = sym_tuple;
      ast->sym->ast = ast;
      forv_ParseAST(a, ast->children) if (a != ptype) {
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
variables_and_nonrecursive_functions(IF1 *i, ParseAST *ast, int skip = 0) {
  if (!skip)
    switch (ast->kind) {
      case AST_indices: {
	forv_ParseAST(a, ast->children) {
	  a->sym = new_sym(i, a->scope, a->string, a->sym);
	  a->sym->ast = a;
	}
	break;
      }
      case AST_def_ident: {
	ParseAST *id = ast->children.v[0];
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
  forv_ParseAST(a, ast->children)
    if (variables_and_nonrecursive_functions(i, a) < 0)
      return -1;
  return 0;
}

static int
build_scopes(IF1 *i, ParseAST *ast, Scope *scope) {
  Vec<ParseAST *> funs;
  ast->scope = scope;
  funs.add(ast);
  while (1) {
    Vec<ParseAST *> last_funs;
    last_funs.move(funs);
    forv_ParseAST(a, last_funs) {
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
build_types(IF1 *i, ParseAST *ast) {
  forv_ParseAST(a, ast->children)
    if (build_types(i, a) < 0)
      return -1;
  switch (ast->kind) {
    case AST_type_param:
      ast->sym = ast->children.v[0]->sym;
      break;
    case AST_vector_type:
    case AST_ref_type:
    case AST_product_type:
    case AST_fun_type:
    case AST_tagged_type:
    case AST_type_application:
    case AST_record_type:
      forv_ParseAST(a, ast->children)
	if (a->sym) {
	  switch (a->kind) {
	    case AST_type_param:
	      ast->sym->has.add(a->sym);
	      break;
	    case AST_inherits:
	      ast->sym->implements.add(a->children.v[0]->sym);
	      ast->sym->includes.add(a->children.v[0]->sym);
	      break;
	    case AST_implements:
	      ast->sym->implements.add(a->children.v[0]->sym);
	      break;
	    case AST_includes:
	      ast->sym->includes.add(a->children.v[0]->sym);
	      break;
	    default:
	      ast->sym->has.add(a->sym);
	      break;
	  }
	}
      break;
    case AST_sum_type:
      forv_ParseAST(a, ast->children)
	if (a->sym)
	  a->sym->implements.set_add(ast->sym);
      break;
    case AST_def_type: {
      ParseAST *last = ast->last();
      if (ast->sym->type_kind == Type_ALIAS) {
	if (last->kind != AST_def_type_param && last->kind != AST_constraint)
	  ast->sym->alias = last->sym;
	else
	  ast->sym->type_kind = Type_UNKNOWN; // can be resolved by a "where" statement
      }
      break;
    }
    case AST_constraint:
      forv_ParseAST(a, ast->children)
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
unalias_types(IF1 *i, ParseAST *ast) {
  forv_ParseAST(a, ast->children)
    if (unalias_types(i, a) < 0)
      return -1;
  if (ast->sym && !ast->sym->constraints) {
    Sym *s = unalias_type(ast->sym);
    if (s != ast->sym) {
      s->scope = ast->sym->scope;
      ast->sym = s;
    }
  }
  return 0;
}

static int
define_labels(IF1 *i, ParseAST *ast, LabelMap *labelmap) {
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
  forv_ParseAST(a, ast->children)
    if (define_labels(i, a, labelmap) < 0)
      return -1;
  return 0;
}

static int
resolve_labels(IF1 *i, ParseAST *ast, LabelMap *labelmap,
	       Label *break_label = 0, Label *continue_label = 0, Label *return_label = 0) 
{
  ParseAST *target;
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
      if (ast->children.v[ast->op_index]->sym->name[0] == ',')
	ast->children.v[0]->in_tuple = 1;
      if (ast->children.v[ast->op_index]->sym->name[0] == '^' && ast->children.v[ast->op_index]->sym->name[1] == '^')
	ast->children.v[0]->in_apply = 1;
      break;
    case AST_scope:
      forv_ParseAST(a, ast->children)
	a->constructor = ast->scope->kind == Scope_PARALLEL ? Make_VECTOR : Make_SET;
      break;
    default: break;
  }
  forv_ParseAST(a, ast->children)
    if (resolve_labels(i, a, labelmap, break_label, continue_label, return_label) < 0)
      return -1;
  return 0;
}

static void
gen_fun(IF1 *i, ParseAST *ast) {
  Sym *fn = ast->sym;
  ParseAST *expr = ast->last();
  Code *body = NULL, *c;
  if1_gen(i, &body, expr->code);
#ifdef TEST_RETURN
  if (expr->rval)
    if1_move(i, &body, expr->rval, fn->ret, ast);
  else
    if1_move(i, &body, sym_null, fn->ret, ast);
#else 
  if (expr->rval)
    fn->ret = expr->rval;
  else
    fn->ret = sym_null;
#endif
  if1_label(i, &body, ast, ast->label[0]);
  c = if1_send(i, &body, 3, 0, sym_reply, fn->cont, fn->ret);
  c->ast = ast;
  int n = ast->children.n - 2;
  ParseAST **args = &ast->children.v[1];
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
get_arg(IF1 *i, Code **c, ParseAST *ast, Vec<Sym *> &args) {
  if (ast->kind != AST_qualified_ident || ast->rval->is_constant || ast->rval->is_symbol)
    args.add(ast->rval);
  else {
    Sym *s = new_sym(i, ast->scope);
    if1_move(i, c, ast->rval, s, ast);
    args.add(s);
  }
}

static int
get_tuple_args(IF1 *i, Code **c, ParseAST *ast, Vec<Sym *> &args) {
  if (ast->kind == AST_op && ast->children.v[ast->op_index]->sym->name[0] == ',') {
    int r = get_tuple_args(i, c, ast->children.v[0], args);
    get_arg(i, c, ast->children.v[2], args);
    return 1 + r;
  }
  get_arg(i, c, ast, args);
  return 1;
}

static int
get_apply_args(IF1 *i, Code **c, ParseAST *ast, Vec<Sym *> &args) {
  if (ast->is_application) {
    int r = get_apply_args(i, c, ast->children.v[0], args);
    get_arg(i, c, ast->children.v[2], args);
    return 1 + r;
  }
  get_arg(i, c, ast, args);
  return 1;
}

static void
gen_new(IF1 *i, ParseAST *ast) {
  Code **c = &ast->code;
  forv_ParseAST(a, ast->children)
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
gen_comma_op(IF1 *i, ParseAST *ast, ParseAST *a0, ParseAST *a1) {
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
gen_apply_op(IF1 *i, ParseAST *ast, ParseAST *a0, ParseAST *a1) {
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
  res->is_lvalue = 1;
}

static void
gen_op(IF1 *i, ParseAST *ast) {
  Code **c = &ast->code;
  Code *send = 0;
  ast->rval = new_sym(i, ast->scope);
  ast->rval->ast = ast;
  Sym *res = ast->rval;
  ParseAST *a0 = ast->op_index ? ast->children.v[0] : 0;
  ParseAST *a1 = ast->children.n > (int)(1 + ast->op_index) ? ast->last() : 0;
  if (a0) if1_gen(i, c, a0->code);
  if (a1) if1_gen(i, c, a1->code);
  if (ast->is_comma)
    gen_comma_op(i, ast, a0, a1);
  else if (ast->is_application) 
    gen_apply_op(i, ast, a0, a1);
  else if (ast->is_simple_assign && !a0->rval->is_var) {
    if (a0->rval->is_read_only)
      show_error("assignment to read-only symbol", ast);
    if1_move(i, c, a1->rval, a0->rval, ast);
    if1_move(i, c, a1->rval, res, ast);
  } else {
    Sym *args = new_sym(i, ast->scope);
    Sym *aa0 = NULL, *aa1 = NULL;
    if (a0) {
      if (a0->rval->is_constant || a0->rval->is_symbol) {
	aa0 = a0->rval;
      } else {
	Sym *s = new_sym(i, ast->scope);
	if1_move(i, c, a0->rval, s, ast);
	aa0 = s;
      }
    }
    if (a1) {
      if (a1->rval->is_constant || a1->rval->is_symbol) {
	aa1 = a1->rval;
      } else {
	Sym *s = new_sym(i, ast->scope);
	if1_move(i, c, a1->rval, s, ast);
	aa1 = s;
      }
    }
    int binary = ast->children.n > 2;
    int post_inc_dec = ast->is_inc_dec && ast->op_index == 1;
    if (post_inc_dec) {
      if1_move(i, c, aa0, res, ast);
      res = new_sym(i, ast->scope);
    }
    if (binary)
      send = if1_send(i, c, 4, 1, sym_make_tuple, aa0, ast->children.v[ast->op_index]->rval, 
		      aa1, args);
    else if (a0)
      send = if1_send(i, c, 3, 1, sym_make_tuple, aa0, ast->children.v[ast->op_index]->rval, args);
    else
      send = if1_send(i, c, 3, 1, sym_make_tuple, ast->children.v[ast->op_index]->rval, aa1, args);
    send->ast = ast;
    send = if1_send(i, c, 2, 1, operator_symbol, args, res);
    send->ast = ast;
    res->is_lvalue = 1;
    if (ast->is_ref)
      aa1->is_lvalue = 1;
    if (ast->is_assign) {
      if (a0)
	if1_move(i, c, res, a0->rval, ast);
      else
	if1_move(i, c, res, a1->rval, ast);
    }
  }
}

static void
gen_indices(IF1 *i, ParseAST *ast, ParseAST *indices, ParseAST *domain) {
  (void) domain;
  Sym *index = new_sym(i, ast->scope);
  index->type = sym_int;
  index->is_external = 1; // hack to make non-constant
  forv_ParseAST(a, indices->children)
    if1_move(i, &ast->code, index, a->sym);
}

static void
gen_forall(IF1 *i, ParseAST *ast) {
  ParseAST *domain = ast->get(AST_domain);
  ParseAST *indices = ast->get(AST_indices);
  ParseAST *body = ast->last();
  if (indices)
    gen_indices(i, ast, indices, domain);
  if1_gen(i, &ast->code, body->code);
  ast->rval = new_sym(i, ast->scope);
  Code *send = if1_send(i, &ast->code, 3, 1, sym_array, domain->rval, body->rval, ast->rval);
  send->ast = ast;
}

static void
gen_loop(IF1 *i, ParseAST *ast) {
  ParseAST *cond = ast->get(AST_loop_cond);
  int dowhile = cond == ast->last();
  ParseAST *body = dowhile ? ast->children.v[ast->children.n - 2] : ast->last();
  ParseAST *before = ast->children.n > 2 ? ast->children.v[0] : 0;
  if1_loop(i, &ast->code, ast->label[0], ast->label[1],
	   cond->rval, before ? before->code : 0, 
	   cond->code, 0, body->code, ast);
  ast->rval = body->rval;
}

static void
gen_if(IF1 *i, ParseAST *ast) {
  ParseAST *ifcond = ast->children.v[0];
  ParseAST *ifif = ast->children.v[1];
  ParseAST *ifelse = ast->children.v[2];
  ast->rval = new_sym(i, ast->scope);
  if1_if(i, &ast->code, ifcond->code, ifcond->rval, ifif->code, ifif->rval,
	 ifelse ? ifelse->code:0, ifelse ? ifelse->rval:0, ast->rval, ast);
}

static void
gen_constructor(IF1 *i, ParseAST *ast) {
  Sym *constructor;
  Vec<Sym *> args;
  forv_ParseAST(a, ast->children) {
    if (a->kind != AST_qualified_ident || a->rval->is_constant || a->rval->is_symbol) {
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
      if (!ast->children.n)
	constructor = sym_make_set;
      else
	constructor = sym_make_tuple;
      break;
    case AST_list: 
      if (!ast->children.n)
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
  if (ast->kind == AST_index && ast->children.n < 3)
    if1_add_send_arg(i, send, make_int(i, 1));
  if1_add_send_result(i, send, ast->rval);
}

static int
define_type_init(IF1 *i, ParseAST *ast, Sym **container_scope, Sym **container) {
  ParseAST *rec = ast->get(AST_record_type);
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
pre_gen_top_down(IF1 *i, ParseAST *ast, Sym *container_scope, Sym *container = 0) {
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
  forv_ParseAST(a, ast->children)
    if (pre_gen_top_down(i, a, container_scope, container) < 0)
      return -1;
  return 0;
}

static int
pre_gen_bottom_up(ParseAST *ast) {
  forv_ParseAST(a, ast->children)
    if (pre_gen_bottom_up(a) < 0)
      return -1;
  switch (ast->kind) {
    case AST_vector: {
      uint rank = 0;
      forv_ParseAST(a, ast->children)
	if (a->rank > rank)
	  rank = a->rank;
      if (ast->children.n > 1)
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
      char *op = ast->children.v[ast->op_index]->sym->name;
      ast->is_inc_dec = (op[0] == '+' && op[1] == '+') || (op[0] == '-' && op[1] == '-');
      ast->is_simple_assign = ((op[0] == '=') && !op[1]);
      ast->is_assign = (op[1] == '=' && op[0] != '=' && op[0] != '<' && op[0] != '>') 
	|| ast->is_simple_assign || ast->is_inc_dec;
      ast->is_ref = op[0] == '&' && ast->op_index == 0;
      ast->is_application = (op[0] == '^' && op[1] == '^') || op[0] == '(';
      ast->is_comma = op[0] == ',';
      if (ast->is_comma) {
	uint rank = 1;
	forv_ParseAST(a, ast->children)
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
gen_assign(IF1 *i, ParseAST *ast, ParseAST *val, Sym *in, Sym *out) {
  Sym *args = new_sym(i, ast->scope);
  Code *send = if1_send(i, &ast->code, 4, 1, sym_make_tuple, in, sym_assign, val->rval, args);
  send->ast = ast;
  send = if1_send(i, &ast->code, 2, 1, operator_symbol, args, out);
  send->ast = ast;
}

static Sym *
value_type(IF1 *i, ParseAST *ast, Sym *s) {
  Sym *v = new_sym(i, ast->scope);
  v->type = s;
  v->is_external = 1; // force non-specific type
  return v;
}

static void
gen_def_ident_value(IF1 *i, ParseAST *ast, ParseAST *constraint, ParseAST *val) {
  // arrays
  ParseAST *arr = constraint->get(AST_array_descriptor);
  if (arr) { 
    Sym *rval = new_sym(i, ast->scope);
    Sym *domain = NULL, *init = NULL;
    ParseAST *arr_dom = arr->get(AST_domain);  // if the domain is declared use it
    if (arr_dom)
      domain = arr_dom->rval;
    ParseAST *indices = arr->get(AST_indices);
    ParseAST *element_type = constraint->last();
    if (element_type != arr)
      init = value_type(i, ast, element_type->sym);
    if (val) {
      if (!domain && val->kind == AST_forall) { // if it has a domain and we need one, use it
	ParseAST *domain_ast = val->get(AST_domain);
	domain = domain_ast->rval;
	ParseAST *element = val->last();
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
    if (declared_type->num_kind) { // numbers
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
gen_container(IF1 *i, ParseAST *ast) {
  Sym *rval = new_sym(i, ast->scope);
  Code *send = if1_send(i, &ast->code, 4, 1, sym_operator, 
			ast->container, sym_period, if1_make_symbol(i, ast->sym->name), 
			rval);
  send->ast = ast;
  rval->is_lvalue = 1;
  return rval;
}

static void
gen_def_ident(IF1 *i, ParseAST *ast) {
  ParseAST *constraint = ast->get(AST_constraint);
  ParseAST *pattern = ast->get(AST_pattern);
  ParseAST *val = ast->last();
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
    if (constraint && constraint->sym->is_value_class) 
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
  if (ast->is_var)
    ast->rval->is_var = 1;
  if (ast->is_const)
    ast->rval->is_single_assign = 1;
}

static void
gen_type(IF1 *i, ParseAST *ast) {
  ParseAST *rec = ast->get(AST_record_type);
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
gen_if1(IF1 *i, ParseAST *ast) {
  // bottom's up
  forv_ParseAST(a, ast->children)
    if (gen_if1(i, a) < 0)
      return -1;
  switch (ast->kind) {
    case AST_def_type: gen_type(i, ast); break;
    case AST_def_fun: gen_fun(i, ast); break;
    case AST_def_ident: gen_def_ident(i, ast); break;
    case AST_pattern: {
      ast->rval = ast->sym; 
      ParseAST *ptype = ast->get(AST_pattern_type);
      if (ast->children.n > (ptype ? 2 : 1))
	ast->rval->is_pattern = 1;
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
      break;
    case AST_list:
    case AST_vector:
    case AST_object:
    case AST_index:
       gen_constructor(i, ast); break;
    case AST_scope:
    case AST_block:
      forv_ParseAST(a, ast->children)
	if1_gen(i, &ast->code, a->code);
      if (ast->children.n)
	ast->rval = ast->last()->rval; 
      break;
    case AST_indices:
      forv_ParseAST(a, ast->children)
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
#ifdef TEST_RETURN
    case AST_return:	
      if1_move(i, &ast->code, ast->last()->rval, ast->scope->in->ret, ast);
      // fall through
    case AST_break:
    case AST_continue: {
      Code *c = if1_goto(i, &ast->code, ast->label[0]);
      c->ast = ast;
      break;
    }
#endif
    default: 
      if (ast->children.n == 1) {
	if (ast->code)
	  if1_gen(i, &ast->code, ast->children.v[0]->code);
	else
	  ast->code = ast->children.v[0]->code;
	ast->rval = ast->children.v[0]->rval;
      } else
	forv_ParseAST(a, ast->children)
	  if1_gen(i, &ast->code, a->code);
      break;
  }
  return 0;
}

static Sym *
collect_module_init(IF1 *i, ParseAST *ast, Sym *mod) {
  forv_ParseAST(a, ast->children) 
    switch (a->kind) {
      case AST_in_module: 
	mod = a->sym->init; break;
      case AST_block: 
	forv_ParseAST(a, ast->children)
	  mod = collect_module_init(i, a, mod);
	break;
      default: 
	if1_gen(i, &mod->code, a->code);
	if (!mod->ast)
	  mod->ast = new_AST(AST_block);
	(dynamic_cast<ParseAST*>(mod->ast))->add(a);
	mod->ret = a->rval;
	break;
    }
  return mod;
}

static int
build_functions(IF1 *i, ParseAST *ast, Sym *mod) {
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
    if (s->is_module) {
      Sym *fn = s->init;
      fn->ret = sym_null;
      fn->cont = new_sym(i, fn->scope);
      fn->cont->ast = s->ast;
      Code *body = NULL;
      if1_gen(i, &body, fn->code);
      if1_send(i, &body, 3, 0, sym_reply, fn->cont, fn->ret);
      Sym *as = new_sym(i, fn->scope);
      as->type = fn;
      as->ast = s->ast;
      if1_closure(i, fn, body, 1, &as);
    }
  }
}

ParseAST *
ast_qid(Sym *s) {
  ParseAST *a = new_AST(AST_qualified_ident);
  a->sym = s;
  return a;
}

ParseAST *
ast_symbol(IF1 *i, char *s) {
  ParseAST *a = new_AST(AST_const);
  a->sym = if1_make_symbol(i, s);
  return a;
}

ParseAST *
ast_call(IF1 *i, int n, ...) {
  assert(n > 0);
  va_list ap;
  va_start(ap, n);
  Sym *s = va_arg(ap, Sym *);
  ParseAST *a = ast_qid(s);
  for (int x = 1; x < n; x++) {
    ParseAST *aa = new_AST(AST_op);
    aa->add(a);
    aa->add(ast_symbol(i, "^^"));
    aa->add(ast_qid(va_arg(ap, Sym *)));
    a = aa;
  }
  if (n == 1) {
    ParseAST *aa = new_AST(AST_op);
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
  fn->scope = new Scope((dynamic_cast<ParseAST*>(fn->ast))->scope, Scope_RECURSIVE, fn);
  Sym *rval = 0;
  Code *body = 0;
  ParseAST *ast = new_AST(AST_block);
  forv_Sym(s, i->allsyms)
    if (s->is_module) {
      rval = new_sym(i, fn->scope);
      Code *send = if1_send(i, &body, 1, 1, s->init, rval);
      ParseAST *a = ast_call(i, 1, s->init);
      send->ast = a;
      ast->add(a);
    }
  fn->cont = new_sym(i, fn->scope);
  fn->ret = sym_null;
  if1_send(i, &body, 3, 0, sym_reply, fn->cont, fn->ret);
  Sym *as = if1_make_symbol(i, fn->name);
  if (!as->ast)
    as->ast = ast;
  if1_closure(i, fn, body, 1, &as);
  fn->ast = ast;
}

static void
global_asserts() {
  assert(Scope_INHERIT == D_SCOPE_INHERIT && Scope_RECURSIVE == D_SCOPE_RECURSIVE);
  assert(Scope_PARALLEL == D_SCOPE_PARALLEL && Scope_SEQUENTIAL == D_SCOPE_SEQUENTIAL);
}

static void
finalize_sym(Sym *f, Sym *s) {
  if (s->in == f)
    s->function_scope = 1;
  else
    assert(!s->function_scope);
  if (s->is_pattern)
    forv_Sym(ss, s->has)
      finalize_sym(f, ss);
}

static void
finalize_fun_symbols(Sym *f, Code *code) {
  forv_Sym(s, code->lvals)
    finalize_sym(f, s);
  forv_Sym(s, code->rvals)
    finalize_sym(f, s);
  forv_Code(c, code->sub)
    finalize_fun_symbols(f, c);
}

// Set Sym::function_scope and Sym::global_scope for flow analysis
static void
finalize_symbols(IF1 *i) {
  forv_Sym(f, i->allclosures) {
    finalize_fun_symbols(f, f->code);
    forv_Sym(s, f->has)
      finalize_sym(f, s);
    if (f->self)
      finalize_sym(f, f->self);
    finalize_sym(f, f->ret);
    finalize_sym(f, f->cont);
  }
  forv_Sym(s, i->allsyms) {
    if (s->is_constant || s->is_symbol)
      s->function_scope = 1;
    else
      if (!s->in || s->in->is_module || s->type_kind)
	s->global_scope = 1;
  }
}

static void
print_transfer_function(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_in(result, make_abstract_type(sym_int));
}

static void
add_primitive_transfer_functions() {
  pdb->fa->primitive_transfer_functions.put(print_symbol, print_transfer_function);
}

int
ast_gen_if1(IF1 *i, Vec<ParseAST *> &av) {
  Scope *global = new Scope();
  cannonical_class = if1_cannonicalize_string(i, "class");
  cannonical_self = if1_cannonicalize_string(i, "self");
  cannonical_folded = if1_cannonicalize_string(i, "< folded >");
  operator_symbol = if1_make_symbol(if1, "operator");
  print_symbol = if1_make_symbol(if1, "print");
  global_asserts();
  forv_ParseAST(a, av)
    build_builtin_syms(i, a);
#define S(_n) assert(sym_##_n);
#include "builtin_symbols.h"
#undef S
  set_primitive_types(i);
  make_module(i, sym_system->name, global, sym_system);
  forv_ParseAST(a, av)
    build_constant_syms(i, a);
  Sym *user_mod = in_module(i, if1_cannonicalize_string(i, "user"), global);
  Scope *scope = user_mod->scope;
  forv_ParseAST(a, av)
    if (build_scopes(i, a, scope) < 0) 
      return -1;
  forv_ParseAST(a, av)
    if (build_types(i, a) < 0) 
      return -1;
  forv_ParseAST(a, av)
    if (unalias_types(i, a) < 0) 
      return -1;
  finalize_types(i);
  forv_ParseAST(a, av)
    if (ast_constant_fold(i, a) < 0)
      return -1;
  forv_ParseAST(a, av)
    if (build_functions(i, a, user_mod) < 0) 
      return -1;
  build_modules(i);
  build_init(i);
  finalize_symbols(i);
  add_primitive_transfer_functions();
  return 0;
}

