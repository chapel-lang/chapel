#include <typeinfo>
#include "geysa.h"
#include "analysis.h"
#include "expr.h"
#include "files.h"
#include "../traversals/getstuff.h"
#include "link.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "builtin.h"
#include "if1.h"
#include "fa.h"
#include "pdb.h"
#include "pnode.h"
#include "var.h"
#include "fun.h"
#include "prim.h"

class LabelMap : public Map<char *, BaseAST *> {};

static Sym *domain_start_index_symbol = 0;
static Sym *domain_next_index_symbol = 0;
static Sym *domain_valid_index_symbol = 0;
static Sym *expr_simple_seq_symbol = 0;
static Sym *expr_domain_symbol = 0;
static Sym *expr_create_domain_symbol = 0;
static Sym *expr_reduce_symbol = 0;
static Sym *write_symbol = 0;
static Sym *writeln_symbol = 0;
static Sym *read_symbol = 0;
static Sym *flood_symbol = 0;
static Sym *completedim_symbol = 0;
static Sym *array_index_symbol = 0;
static Sym *sizeof_symbol = 0;
static Sym *cast_symbol = 0;

static Sym *sym_index = 0;
static Sym *sym_domain = 0;
static Sym *sym_array = 0;
static Sym *sym_sequence = 0;
static Sym *sym_locale = 0;

ASymbol::ASymbol() : xsymbol(0) {
}

AInfo::AInfo() : xast(0), code(0), sym(0), rval(0) {
  label[0] = label[1] = 0;
}

char *AInfo::pathname() { 
  return xast->filename;
}

int AInfo::line() {
  return xast->lineno;
}

Sym *AInfo::symbol() {
  if (rval) return rval;
  return sym;
}

AST *AInfo::copy(Map<PNode *, PNode*> *nmap) {
  return NULL;
}

static void
close_symbols(Vec<Stmt *> &stmts, Vec<BaseAST *> &syms) {
  Vec<BaseAST *> set;
  // Have to add nilClassType by hand because traversals skip
  // over it since isNull() is true.  Perhaps the abstract
  // parent class should not be a nil?
  if (set.set_add(nilClassType))
    syms.add(nilClassType);
  forv_Stmt(a, stmts)
    while (a) {
      set.set_add(a);
      syms.add(a);
      a = dynamic_cast<Stmt *>(a->next);
    }
  forv_BaseAST(s, syms) {
    GetStuff getStuff(GET_ALL);
    s->traverse(&getStuff);
    forv_BaseAST(ss, getStuff.asts) if (ss) {
      assert(ss);
      if (set.set_add(ss))
	syms.add(ss);
    }
  }
}

static void
set_global_scope(Sym *s) {
  s->function_scope = 0;
  s->global_scope = 1;
}

static ASymbol *
new_ASymbol(Symbol *symbol = 0) {
  ASymbol *s = new ASymbol;
  char *name = 0;
  if (symbol && !symbol->isNull())
    name = symbol->name;
  if1_register_sym(if1, s, name);
  return s;
}

static ASymbol *
new_ASymbol(char *name) {
  ASymbol *s = new ASymbol;
  if1_register_sym(if1, s, name);
  return s;
}

void
ACallbacks::new_SUM_type(Sym *) {
}

Sym *
ACallbacks::new_Sym(char *name) {
  return new_ASymbol(name);
}

Sym *
ASymbol::copy() {
  ASymbol *s = new_ASymbol(); 
  s->copy_values(this);
  s->xsymbol = xsymbol;
  return s;
}

static Sym *
new_sym(char *name = 0, int global = 0) {
  Sym *s = new_ASymbol(name);
  if (!global)
    s->function_scope = 1;
  else
    s->global_scope = 1;
  return s;
}

static void
map_type(Type *t) {
  t->asymbol = new_ASymbol(t->name);
  t->asymbol->xsymbol = t;
  set_global_scope(t->asymbol);
}

static void
map_symbols(Vec<BaseAST *> &syms) {
  int symbols = 0, types = 0, exprs = 0, stmts = 0;
  if (verbose_level > 1)
    printf("map_symbols: %d\n", syms.n);
  forv_BaseAST(s, syms) {
    Symbol *sym = dynamic_cast<Symbol *>(s);
    if (sym) {
      sym->asymbol = new_ASymbol(sym);
      sym->asymbol->xsymbol = sym;
      if (!sym->scope) {
	sym->asymbol->global_scope = 1;
      } else {
	switch (sym->scope->type) {
	  default: assert(0);
	  case SCOPE_INTRINSIC:
	  case SCOPE_INTERNAL_PRELUDE:
	  case SCOPE_PRELUDE:
	  case SCOPE_MODULE:
	  case SCOPE_POSTPARSE:
	    sym->asymbol->global_scope = 1;
	    break;
	  case SCOPE_PARAM:
	  case SCOPE_FUNCTION:
	  case SCOPE_LOCAL:
	  case SCOPE_FORLOOP:
	    sym->asymbol->function_scope = 1;
	    break;
	  case SCOPE_CLASS: // handled as the symbols appears in code
	    break;
	}
      }
      symbols++;
      if (verbose_level > 1 && sym->name)
        printf("map_symbols: found Symbol '%s'\n", sym->name);
    } else {
      Type *t = dynamic_cast<Type *>(s);
      if (t) {
	map_type(t);
	t->asymbol = new_ASymbol(t->name);
	types++;
      } else {
	Expr *e = dynamic_cast<Expr *>(s);
	if (e) {
	  e->ainfo = new AInfo;
	  e->ainfo->xast = e;
	  exprs++;
	} else {
	  Stmt *st = dynamic_cast<Stmt *>(s);
	  st->ainfo = new AInfo;
	  st->ainfo->xast = s;
	  stmts++;
	}
      }
    }
  }
  if (verbose_level > 1)
    printf("map_symbols: BaseASTs: %d, Symbols: %d, Types: %d, Exprs: %d, Stmts: %d\n", 
	   syms.n, symbols, types, exprs, stmts);
}

static void 
build_record_type(Type *t, Sym *parent = 0) {
  t->asymbol->type_kind = Type_RECORD;
  if (parent) {
    t->asymbol->implements.add(parent);
    t->asymbol->includes.add(parent);
  }
}

static void
build_enum_element(Sym *ss, int i) {
  ss->type_kind = Type_PRIMITIVE;
  ss->implements.add(sym_enum_element);
  ss->includes.add(sym_enum_element);
  ss->type_sym = ss;
  ss->imm.v_int32 = i;
}

static void
build_symbols(Vec<BaseAST *> &syms) {
  forv_BaseAST(ss, syms) {
    Symbol *s = dynamic_cast<Symbol *>(ss);
    if (s) { 
      switch (s->astType) {
	case SYMBOL_PARAM: {
	  if (s->type && s->type != nilType && s->type != dtUnknown)
	    s->asymbol->type = s->type->asymbol;
	  break;
	}
	default: break;
      }
    }
  }
}

static void
build_types(Vec<BaseAST *> &syms) {
  Vec<Type *> types;
  forv_BaseAST(s, syms) {
    Type *t = dynamic_cast<Type *>(s);
    if (t) 
      types.add(t);
  }
  forv_Type(t, types) {
    make_type_sym(t->asymbol);
    switch (t->astType) {
      default: assert(!"case");
      case TYPE:
	t->asymbol->type_kind = Type_UNKNOWN;
	break;
      case TYPE_BUILTIN:
	if (t == dtVoid) {
	  t->asymbol->type_kind = Type_ALIAS;
	  t->asymbol->alias = sym_void;
	} else if (t == dtBoolean) {
	  t->asymbol->type_kind = Type_ALIAS;
	  t->asymbol->alias = sym_bool;
	} else if (t == dtInteger) {
	  t->asymbol->type_kind = Type_ALIAS;
	  t->asymbol->alias = sym_int;
	} else if (t == dtFloat) {
	  t->asymbol->type_kind = Type_ALIAS;
	  t->asymbol->alias = sym_float;
	} else if (t == dtComplex) {
	  t->asymbol->type_kind = Type_ALIAS;
	  t->asymbol->alias = sym_complex;
	} else if (t == dtString) {
	  t->asymbol->type_kind = Type_ALIAS;
	  t->asymbol->alias = sym_string;
	} else if (t == dtLocale) {
	} else if (t == dtUnknown) {
	  t->asymbol->type_kind = Type_UNKNOWN;
	} else
	  assert(!"case");
	break;
      case TYPE_ENUM: {
	t->asymbol->type_kind = Type_TAGGED;
	GetSymbols* getSymbols = new GetSymbols();
	t->traverseList(getSymbols);
	for (int i = 0; i < getSymbols->symbols.n; i++) {
	  BaseAST *s = getSymbols->symbols.v[i];
	  Sym *ss = dynamic_cast<Symbol*>(s)->asymbol;
	  build_enum_element(ss, i);
	  t->asymbol->has.add(ss);
	}
	break;
      }
      case TYPE_DOMAIN: 
	build_record_type(t, sym_domain);
	break;
      case TYPE_INDEX: 
	build_record_type(t, sym_tuple);
	break;
      case TYPE_ARRAY: {
	ArrayType *at = dynamic_cast<ArrayType*>(t);
	build_record_type(t, sym_array); 
	Sym *s = at->asymbol;
	s->element = new_sym();
	s->element->type = at->elementType->asymbol;
	s->element->is_var = 1;
	s->element->is_external = 1;
	break;
      }
      case TYPE_TUPLE: {	
	TupleType *tt = dynamic_cast<TupleType*>(t);
	forv_Vec(Type, c, tt->components) {
	  Sym *x = new_sym();
	  x->ast = c->asymbol->ast;
	  x->type = c->asymbol;
	  t->asymbol->has.add(x);
	}
	t->asymbol->implements.add(sym_tuple);
	t->asymbol->includes.add(sym_tuple);
	break;
      }
      case TYPE_USER: {
	UserType *tt = dynamic_cast<UserType*>(t);
	t->asymbol->type_kind = Type_ALIAS;
	t->asymbol->alias = tt->definition->asymbol;
	break;
      }
      case TYPE_CLASS: {
	// ClassType::definition handled below in build_classes()
	ClassType *tt = dynamic_cast<ClassType*>(t);
	t->asymbol->type_kind = Type_RECORD;
	tt->name->asymbol = (ASymbol*)tt->asymbol->type_sym;
	if (tt->parentClass) {
	  t->asymbol->implements.add(tt->parentClass->asymbol);
	  t->asymbol->includes.add(tt->parentClass->asymbol);
	}
	break;
      }
    }
  }
}

static void
new_primitive_type(Sym *&sym, char *name) {
  if (!sym)
    sym = new_sym(name, 1);
  sym->type_kind = Type_PRIMITIVE;
  if1_set_builtin(if1, sym, name);
}

static void
new_alias_type(Sym *&sym, char *name, Sym *alias) {
  if (!sym)
    sym = new_sym(name, 1);
  sym->type_kind = Type_ALIAS;
  sym->alias = alias;
  if1_set_builtin(if1, sym, name);
}

static void
new_sum_type(Sym *&sym, char *name, ...)  {
  if (!sym)
    sym = new_sym(name, 1);
  sym->type_kind = Type_SUM;
  if1_set_builtin(if1, sym, name);
  va_list ap;
  va_start(ap, name);
  Sym *s = 0;
  do {
    if ((s = va_arg(ap, Sym*)))
      sym->has.add(s);
  } while (s);
  forv_Sym(ss, sym->has)
    ss->implements.set_add(sym);
}

#if 0
static void
new_global_variable(Sym *&sym, char *name) {
  if (!sym)
    sym = new_sym(name, 1);
  sym->global_scope = 1;
  if1_set_builtin(if1, sym, name);
}
#endif

static void
builtin_Symbol(Type *dt, Sym **sym, char *name) {
  if (!dt->asymbol)
    map_type(dt);
  *sym = dt->asymbol;
  if1_set_builtin(if1, *sym, name);
  if (!dt->asymbol->type_kind)
    dt->asymbol->type_kind = Type_PRIMITIVE;
  ((ASymbol*)*sym)->xsymbol = dt;
}

static void
build_builtin_symbols() {
  if (!sym_system) {
    sym_system = new_sym("system", 1);
    if1_set_builtin(if1, sym_system, "system");
  }
  if (!sym_system->init)
    sym_system->init = new_sym("__init", 1);
  build_module(sym_system, sym_system->init);

  new_primitive_type(sym_anyclass, "anyclass");
  sym_anyclass->type_sym = sym_anyclass;
  new_primitive_type(sym_any, "any");
  new_primitive_type(sym_module, "module");
  new_primitive_type(sym_symbol, "symbol");
  if1_set_symbols_type(if1);
  new_primitive_type(sym_function, "function");
  new_primitive_type(sym_continuation, "continuation");
  new_primitive_type(sym_vector, "vector");
  new_primitive_type(sym_void, "void");
  if (!sym_object)
    sym_object = new_sym("object", 1);
  sym_object->type_kind = Type_RECORD;
  if1_set_builtin(if1, sym_object, "object");
  new_primitive_type(sym_list, "list");
  new_primitive_type(sym_ref, "ref");
  new_primitive_type(sym_value, "value");
  new_primitive_type(sym_set, "set");
  new_primitive_type(sym_sequence, "sequence");
  new_primitive_type(sym_int8, "int8");
  new_primitive_type(sym_int16, "int16");
  new_primitive_type(sym_int32, "int32");
  new_primitive_type(sym_int64, "int64");
  new_alias_type(sym_int, "int", sym_int64);
  new_primitive_type(sym_true, "true");
  new_primitive_type(sym_false, "false");
  new_primitive_type(sym_bool, "bool");
  sym_true->implements.set_add(sym_bool);
  sym_false->implements.set_add(sym_bool);
  new_primitive_type(sym_uint8, "uint8");
  new_primitive_type(sym_uint16, "uint16");
  new_primitive_type(sym_uint32, "uint32");
  new_primitive_type(sym_uint64, "uint64");
  new_alias_type(sym_uint, "uint", sym_uint64);
  new_sum_type(sym_anyint, "anyint", 
	       sym_int8, sym_int16, sym_int32, sym_int64, sym_bool,
	       sym_uint8, sym_uint16, sym_uint32, sym_uint64, 0);
  new_primitive_type(sym_size, "size");
  new_primitive_type(sym_enum_element, "enum_element");
  new_primitive_type(sym_float32, "float32");
  new_primitive_type(sym_float64, "float64");
  new_primitive_type(sym_float128, "float128");
  new_primitive_type(sym_float, "float");
  new_sum_type(sym_anyfloat, "anyfloat", 
	       sym_float32, sym_float64, sym_float128, 0);
  new_primitive_type(sym_complex32, "complex32");
  new_primitive_type(sym_complex64, "complex64");
  new_primitive_type(sym_complex128, "complex128");
  new_primitive_type(sym_complex, "complex");
  new_sum_type(sym_anycomplex, "anycomplex", 
	       sym_complex32, sym_complex64, sym_complex128, 0);
  new_sum_type(sym_anynum, "anynum", sym_bool, sym_anyint, sym_anyfloat, sym_anycomplex, 0);
  new_primitive_type(sym_char, "char");
  new_primitive_type(sym_string, "string");
  if (!sym_new_object) {
    sym_new_object = new_sym("new_object", 1);
    if1_set_builtin(if1, sym_new_object, "new_object");
  }
  
  //new_global_variable(sym_null, "null");
  new_primitive_type(sym_null, "null");

  sym_init = new_sym(); // placeholder

  ((ASymbol*)sym_void)->xsymbol = dtVoid;
  ((ASymbol*)sym_bool)->xsymbol = dtBoolean;
  ((ASymbol*)sym_int64)->xsymbol = dtInteger;
  ((ASymbol*)sym_float64)->xsymbol = dtFloat;
  ((ASymbol*)sym_complex64)->xsymbol = dtComplex;
  ((ASymbol*)sym_string)->xsymbol = dtString;

  builtin_Symbol(dtTuple, &sym_tuple, "tuple");
  builtin_Symbol(dtIndex, &sym_index, "index");
  builtin_Symbol(dtDomain, &sym_domain, "domain");
  builtin_Symbol(dtArray, &sym_array, "array");
  builtin_Symbol(dtLocale, &sym_locale, "locale");

#define S(_n) assert(sym_##_n);
#include "builtin_symbols.h"
#undef S
}

static int
import_symbols(Vec<BaseAST *> &syms) {
  map_symbols(syms);
  build_builtin_symbols();
  build_symbols(syms);
  build_types(syms);
  return 0;
}

// FUTURE: support for goto or named break
static int
define_labels(BaseAST *ast, LabelMap *labelmap) {
#if 0
  Stmt *stmt = dynamic_cast<Stmt *>(ast);
  switch (stmt->astType) {
    case AST_label:
      ast->label[0] = if1_alloc_label(i);
      ast->label[1] = ast->label[0];
      labelmap->put(ast->get(AST_ident)->string, ast);
      break;
    default: break;
  }
  GetStmts* getStmts = new GetStmts();
  ast->traverseList(getStmts);
  forv_BaseAST(a, getStmts->stmts)
    define_labels(a, labelmap);
#endif
  return 0;
}

static int
resolve_labels(BaseAST *ast, LabelMap *labelmap,
	       Label *return_label, Label *break_label = 0, Label *continue_label = 0)
{
  Stmt *stmt = dynamic_cast<Stmt *>(ast);
  switch (stmt->astType) {
    case STMT_WHILELOOP:
    case STMT_FORLOOP:
      continue_label = stmt->ainfo->label[0] = if1_alloc_label(if1);
      break_label = stmt->ainfo->label[1] = if1_alloc_label(if1);
      break;
    case STMT_RETURN:
      stmt->ainfo->label[0] = return_label;
      break;
#if 0
    case STMT_BREAK;
      if ((target = ast->get(AST_ident))) {
	target = labelmap->get(target->string);
	ast->label[0] = target->label[1];
      } else
	ast->label[0] = break_label;
      break;
    case STMT_CONTINUE:
      if ((target = ast->get(AST_ident))) {
	target = labelmap->get(target->string);
	ast->label[0] = target->label[0];
      } else
	ast->label[0] = continue_label;
      break;
    case STMT_GOTO:
      target = labelmap->get(ast->get(AST_ident)->string);
      ast->label[0] = target->label[0];
      break;
#endif
    default: break;
  }
  GetStmts* getStmts = new GetStmts();
  ast->traverseList(getStmts);
  forv_BaseAST(a, getStmts->stmts)
    if (resolve_labels(a, labelmap, return_label, break_label, continue_label) < 0)
      return -1;
  return 0;
}

static int
gen_vardef(BaseAST *a) {
  VarDefStmt *def = dynamic_cast<VarDefStmt*>(a);
  VarSymbol *var = def->var;
  Sym *s = var->asymbol;
  def->ainfo->sym = s;
  if (var->type) {
    s->type = var->type->asymbol;
    if (var->type->astType != TYPE_CLASS)
      s->is_var = 1;
  }
  if (!def->init->isNull()) {
    if1_gen(if1, &def->ainfo->code, def->init->ainfo->code);
    if1_move(if1, &def->ainfo->code, def->init->ainfo->rval, def->ainfo->sym, def->ainfo);
  } else if (!s->is_var) {
    show_error("missing value initializer", def->ainfo);
    return -1;
  } else {
    s->is_external = 1; // hack
  }
  return 0;
}

static int
gen_expr_stmt(BaseAST *a) {
  ExprStmt *expr = dynamic_cast<ExprStmt*>(a);
  expr->ainfo->code = expr->expr->ainfo->code;
  return 0;
}

static int
gen_while(BaseAST *a) {
  WhileLoopStmt *s = dynamic_cast<WhileLoopStmt*>(a);
  Code *body_code = 0;
  Vec<Stmt*> body;
  getLinkElements(body, s->body);
  forv_Stmt(ss, body)
    if1_gen(if1, &body_code, ss->ainfo->code);
  if1_loop(if1, &s->ainfo->code, s->ainfo->label[0], s->ainfo->label[1],
	   s->condition->ainfo->rval, 0, 
	   s->condition->ainfo->code, 0, 
	   body_code, s->ainfo);
  return 0;
}

static int
gen_forall_internal(AInfo *ainfo, Code *body, Vec<Symbol*> &indices, Vec<Expr*> &domains) {
  // setup code: evaluate domains and get starting indices
  Code *setup_code = 0, *send;
  forv_Expr(d, domains)
    if1_gen(if1, &setup_code, d->ainfo->code);
  send = if1_send(if1, &setup_code, 2, 0, sym_primitive, domain_start_index_symbol);
  forv_Expr(d, domains)
    if1_add_send_arg(if1, send, d->ainfo->rval);
  forv_Symbol(i, indices)
    if1_add_send_result(if1, send, i->asymbol);
  send->ast = ainfo;

  // loop condition code
  Code *condition_code = 0;
  Sym *condition_rval = new_sym();
  send = if1_send(if1, &condition_code, 2, 1, sym_primitive, domain_valid_index_symbol,
		  condition_rval);
  forv_Expr(d, domains)
    if1_add_send_arg(if1, send, d->ainfo->rval);
  forv_Symbol( i, indices)
    if1_add_send_arg(if1, send, i->asymbol);
  send->ast = ainfo;

  // next index code
  send = if1_send(if1, &body, 2, 0, sym_primitive, domain_next_index_symbol);
  forv_Expr(d, domains)
    if1_add_send_arg(if1, send, d->ainfo->rval);
  forv_Symbol( i, indices)
    if1_add_send_arg(if1, send, i->asymbol);
  forv_Symbol( i, indices)
    if1_add_send_result(if1, send, i->asymbol);
  send->ast = ainfo;

  // build loop
  if1_loop(if1, &ainfo->code, ainfo->label[0], ainfo->label[1],
	   condition_rval, setup_code, 
	   condition_code, 0, 
	   body, ainfo);
  return 0;
}

static int
gen_for(BaseAST *a) {
  ForLoopStmt *s = dynamic_cast<ForLoopStmt*>(a);
  Code *body = 0;
  Vec<Stmt*> body_stmts;
  getLinkElements(body_stmts, s->body);
  forv_Stmt(ss, body_stmts)
    if1_gen(if1, &body, ss->ainfo->code);
  Vec<Symbol*> indices;
  getLinkElements(indices, s->index);
  Vec<Expr*> domains;
  domains.add(s->domain);
  return gen_forall_internal(s->ainfo, body, indices, domains);
}

static int
gen_cond(BaseAST *a) {
  CondStmt *s = dynamic_cast<CondStmt*>(a);
  AInfo *ifelse = s->elseStmt->ainfo;
  if (s->elseStmt == nilStmt) ifelse = 0;
  if1_if(if1, &s->ainfo->code, s->condExpr->ainfo->code, s->condExpr->ainfo->rval, 
	 s->thenStmt->ainfo->code, s->thenStmt->ainfo->rval, ifelse ? ifelse->code : 0, 
	 ifelse ? ifelse->rval : 0, 
	 s->ainfo->rval, s->ainfo);
  return 0;
}

static int
undef_or_fn_expr(Expr *ast) {
  if (ast->astType == EXPR_VARIABLE) { 
    Variable *v = dynamic_cast<Variable *>(ast);
    if (v->var->astType == SYMBOL_UNRESOLVED || v->var->astType == SYMBOL_FN) {
      assert(ast->ainfo->rval->name);
      return 1;
    }
  }
  return 0;
}

static void
gen_move(Expr *e, Sym *s) {
  e->ainfo->rval = new_sym(s->name ? s->name : s->constant);
  e->ainfo->rval->ast = e->ainfo;
  if1_move(if1, &e->ainfo->code, s, e->ainfo->rval, e->ainfo);
}

static Sym *
gen_move(Sym *s, Expr *e) {
  Sym *ss = new_sym(s->name);
  ss->ast = e->ainfo;
  if1_move(if1, &e->ainfo->code, s, ss, e->ainfo);
  return ss;
}

static Sym *fnsym = 0;

static int
gen_if1(BaseAST *ast) {
  // bottom's up
  GetStuff getStuff(GET_STMTS|GET_EXPRS);
  ast->traverse(&getStuff);
  forv_BaseAST(a, getStuff.asts)
    if (gen_if1(a) < 0)
      return -1;
  switch (ast->astType) {
    case STMT: assert(ast->isNull()); break;
    case STMT_NOOP: break;
    case STMT_VARDEF: if (gen_vardef(ast) < 0) return -1; break;
    case STMT_TYPEDEF: break;
    case STMT_FNDEF: break;
    case STMT_EXPR: if (gen_expr_stmt(ast) < 0) return -1; break;
    case STMT_RETURN: {
      ReturnStmt *s = dynamic_cast<ReturnStmt*>(ast);
      Sym *fn = fnsym; // s->parentFn->asymbol;
      if (!s->expr->isNull()) {
	if1_gen(if1, &s->ainfo->code, s->expr->ainfo->code);
	if1_move(if1, &s->ainfo->code, s->expr->ainfo->rval, fn->ret, s->ainfo);
      }
      Code *c = if1_goto(if1, &s->ainfo->code, s->ainfo->label[0]);
      c->ast = s->ainfo;
      break;
    }
#if 0
    case STMT_GOTO:
    case STMT_BREAK:
    case STMT_CONTINUE:
    {
      Code *c = if1_goto(i, &ast->code, ast->label[0]);
      c->ast = ast;
      break;
    }
#endif
    case STMT_BLOCK: {
      BlockStmt *s = dynamic_cast<BlockStmt*>(ast);
      Vec<Stmt *> stmts;
      getLinkElements(stmts, s->body);
      forv_Stmt(ss, stmts)
	if1_gen(if1, &s->ainfo->code, ss->ainfo->code);
      break;
    }
    case STMT_WHILELOOP: gen_while(ast); break;
    case STMT_FORLOOP: gen_for(ast); break;
    case STMT_COND: gen_cond(ast); break;
      
    case EXPR: {
      Expr *e = dynamic_cast<Expr*>(ast);
      assert(ast->isNull()); 
      e->ainfo->rval = sym_null;
      break;
    }
    case EXPR_LITERAL: assert(!"case"); break;
    case EXPR_BOOLLITERAL: {
      BoolLiteral *s = dynamic_cast<BoolLiteral*>(ast);
      Sym *c = if1_const(if1, sym_bool, s->str);
      c->imm.v_bool = s->val;
      break;
    }
    case EXPR_INTLITERAL: {
      IntLiteral *s = dynamic_cast<IntLiteral*>(ast);
      Sym *c = if1_const(if1, sym_int64, s->str);
      c->imm.v_int64 = s->val;
      gen_move(s, c);
      break;
    }
    case EXPR_FLOATLITERAL: {
      FloatLiteral *s = dynamic_cast<FloatLiteral*>(ast);
      Sym *c = if1_const(if1, sym_float64, s->str);
      c->imm.v_float64 = s->val;
      gen_move(s, c);
      break;
    }
    case EXPR_COMPLEXLITERAL: {
      ComplexLiteral *s = dynamic_cast<ComplexLiteral*>(ast);
      Sym *c = if1_const(if1, sym_complex64, s->str);
      c->imm.v_complex64.r = s->realVal;
      c->imm.v_complex64.i = s->imagVal;
      break;
    }
    case EXPR_STRINGLITERAL: {
      StringLiteral *s = dynamic_cast<StringLiteral*>(ast);
      Sym *c = if1_const(if1, sym_string, s->str);
      gen_move(s, c);
      break;
    }
    case EXPR_VARIABLE: {
      Variable *s = dynamic_cast<Variable*>(ast);
      Sym *sym = s->var->asymbol;
      // hack
      if (s->var->asymbol->name && 
	  !strcmp(s->var->asymbol->name, "self")) {
	sym = fnsym->self;
      }
      s->ainfo->sym = sym;
      gen_move(s, sym);
      break;
    }
    case EXPR_UNOP: {
      UnOp *s = dynamic_cast<UnOp*>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      if1_gen(if1, &s->ainfo->code, s->operand->ainfo->code);
      Sym *op = 0;
      switch (s->type) {
	default: assert(!"case");
	case UNOP_PLUS: op = if1_make_symbol(if1, "+"); break;
	case UNOP_MINUS: op = if1_make_symbol(if1, "-"); break;
	case UNOP_LOGNOT: op = if1_make_symbol(if1, "!"); break;
	case UNOP_BITNOT: op = if1_make_symbol(if1, "~"); break;
      }
      
      Code *c = if1_send(if1, &s->ainfo->code, 3, 1, sym_operator, op, 
			 s->operand->ainfo->rval, s->ainfo->rval);
      c->ast = s->ainfo;
      break;
    }
    case EXPR_SPECIALBINOP:
    case EXPR_BINOP: {
      BinOp *s = dynamic_cast<BinOp*>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      if1_gen(if1, &s->ainfo->code, s->left->ainfo->code);
      if1_gen(if1, &s->ainfo->code, s->right->ainfo->code);
      Sym *op = 0;
      switch (s->type) {
	default: assert(!"case");
	case BINOP_PLUS: op = if1_make_symbol(if1, "+"); break;
	case BINOP_MINUS: op = if1_make_symbol(if1, "-"); break;
	case BINOP_MULT: op = if1_make_symbol(if1, "*"); break;
	case BINOP_DIV: op = if1_make_symbol(if1, "/"); break;
	case BINOP_MOD: op = if1_make_symbol(if1, "%"); break;
	case BINOP_EQUAL: op = if1_make_symbol(if1, "=="); break;
	case BINOP_LEQUAL: op = if1_make_symbol(if1, "<="); break;
	case BINOP_GEQUAL: op = if1_make_symbol(if1, ">="); break;
	case BINOP_GTHAN: op = if1_make_symbol(if1, ">"); break;
	case BINOP_LTHAN: op = if1_make_symbol(if1, "<"); break;
	case BINOP_NEQUAL: op = if1_make_symbol(if1, "!="); break;
	case BINOP_BITAND: op = if1_make_symbol(if1, "&"); break;
	case BINOP_BITOR: op = if1_make_symbol(if1, "|"); break;
	case BINOP_BITXOR: op = if1_make_symbol(if1, "^"); break;
	case BINOP_LOGAND: op = if1_make_symbol(if1, "&&"); break;
	case BINOP_LOGOR: op = if1_make_symbol(if1, "||"); break;
	case BINOP_EXP: op = if1_make_symbol(if1, "**"); break;
	case BINOP_BY: op = if1_make_symbol(if1, "by"); break;
      }
      Code *c = if1_send(if1, &s->ainfo->code, 4, 1, sym_operator,
			 s->left->ainfo->rval, op, s->right->ainfo->rval,
			 s->ainfo->rval);
      c->ast = s->ainfo;
      break;
    }
    case EXPR_MEMBERACCESS: {
      MemberAccess *s = dynamic_cast<MemberAccess*>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      s->ainfo->sym = s->ainfo->rval;
      if1_gen(if1, &s->ainfo->code, s->base->ainfo->code);
      Sym *op = if1_make_symbol(if1, ".");
      Sym *selector = 0;
      if (
#if 0
	s->member->astType == SYMBOL ||
	  s->member->astType == SYMBOL_UNRESOLVED ||
	  s->member->astType == SYMBOL_FN ||
	  (s->member->astType == SYMBOL_VAR && 
	   (((dynamic_cast<VarSymbol*>(s->member))->type->astType == TYPE_BUILTIN)))
#else
	1
#endif
)
      {
	assert(s->member->asymbol->name);
	selector = if1_make_symbol(if1, s->member->asymbol->name);
      } else
	selector = s->member->asymbol;
      Code *c = if1_send(if1, &s->ainfo->code, 4, 1, sym_operator,
			 s->base->ainfo->rval, op, selector,
			 s->ainfo->rval);
      c->ast = s->ainfo;
      s->ainfo->rval->is_lvalue = 1;
      break;
    }
    case EXPR_ASSIGNOP: {
      AssignOp *s = dynamic_cast<AssignOp*>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      s->ainfo->sym = s->left->ainfo->sym;
      if1_gen(if1, &s->ainfo->code, s->left->ainfo->code);
      if1_gen(if1, &s->ainfo->code, s->right->ainfo->code);
      Sym *op = 0;
      Sym *rval = s->right->ainfo->rval;
      switch (s->type) {
	default: assert(!"case");
	case GETS_NORM: op = 0; break;
	case GETS_PLUS: op = if1_make_symbol(if1, "+"); break;
	case GETS_MINUS: op = if1_make_symbol(if1, "-"); break;
	case GETS_MULT: op = if1_make_symbol(if1, "*"); break;
	case GETS_DIV: op = if1_make_symbol(if1, "/"); break;
	case GETS_BITAND: op = if1_make_symbol(if1, "&"); break;
	case GETS_BITOR: op = if1_make_symbol(if1, "|"); break;
	case GETS_BITXOR: op = if1_make_symbol(if1, "^"); break;
      }
      if (op) {
	rval = new_sym();
	rval->ast = s->ainfo;
	Code *c = if1_send(if1, &s->ainfo->code, 4, 1, sym_operator,
			   s->left->ainfo->rval, op, s->right->ainfo->rval,
			   rval);
	c->ast = s->ainfo;
      }
      if (!s->left->ainfo->sym)
	show_error("assignment to non-lvalue", s->ainfo);
      if (s->left->ainfo->sym->is_var) {
	Sym *rrval = new_sym();
	rrval->ast = s->ainfo;
	Code *c = if1_send(if1, &s->ainfo->code, 4, 1, sym_operator,
			   s->left->ainfo->rval, sym_assign, rval,
			   rrval);
	c->ast = s->ainfo;
	rval = rrval;
      }
      if1_move(if1, &s->ainfo->code, rval, s->left->ainfo->sym, s->ainfo);
      if1_move(if1, &s->ainfo->code, rval, s->ainfo->rval, s->ainfo);
      break;
    }
    case EXPR_SIMPLESEQ: {
      SimpleSeqExpr *s = dynamic_cast<SimpleSeqExpr *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      if1_gen(if1, &s->ainfo->code, s->lo->ainfo->code);
      if1_gen(if1, &s->ainfo->code, s->hi->ainfo->code);
      if1_gen(if1, &s->ainfo->code, s->str->ainfo->code);
      Code *send = if1_send(if1, &s->ainfo->code, 5, 1, sym_primitive, expr_simple_seq_symbol, 
			    s->lo->ainfo->rval, s->hi->ainfo->rval, s->str->ainfo->rval, 
			    s->ainfo->rval);
      send->ast = s->ainfo;
      break;
    }
    case EXPR_FLOOD: {
      FloodExpr *s = dynamic_cast<FloodExpr *>(ast);
      s->ainfo->rval = flood_symbol;
      break;
    }
    case EXPR_COMPLETEDIM: {
      CompleteDimExpr *s = dynamic_cast<CompleteDimExpr *>(ast);
      s->ainfo->rval = completedim_symbol;
      break;
    }
    case EXPR_DOMAIN: {
      DomainExpr *s = dynamic_cast<DomainExpr *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      Vec<Expr *> domains;
      getLinkElements(domains, s->domains);
      Vec<Symbol *> indices;
      getLinkElements(indices, s->indices);
      if (!s->forallExpr->isNull()) { // forall expression
	Code *body = 0;
	forv_Vec(Expr, d, domains)
	  if1_gen(if1, &body, d->ainfo->code);
	if (gen_forall_internal(s->ainfo, body, indices, domains) < 0)
	  return -1;
      } else {
	forv_Vec(Expr, d, domains)
	  if1_gen(if1, &s->ainfo->code, d->ainfo->code);
	Code *send = if1_send(if1, &s->ainfo->code, 2, 1, sym_primitive, expr_create_domain_symbol, 
			      s->ainfo->rval);
	forv_Vec(Expr, d, domains)
	  if1_add_send_arg(if1, send, d->ainfo->rval);
	send->ast = s->ainfo;
      }
      break;
    }
    case EXPR_IOCALL: {
      IOCall *s = dynamic_cast<IOCall *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      if1_gen(if1, &s->ainfo->code, s->baseExpr->ainfo->code);
      Vec<Expr *> args;
      getLinkElements(args, s->argList);
      if (args.n == 1 && args.v[0]->isNull())
	args.n--;
      forv_Vec(Expr, a, args)
	if1_gen(if1, &s->ainfo->code, a->ainfo->code);
      Code *send = if1_send1(if1, &s->ainfo->code);
      send->ast = s->ainfo;
      if1_add_send_arg(if1, send, sym_primitive);
      switch (s->ioType) {
	case IO_WRITELN:
	  if1_add_send_arg(if1, send, writeln_symbol);
	  break;
	case IO_WRITE:
	  if1_add_send_arg(if1, send, write_symbol);
	  break;
	case IO_READ:
	  if1_add_send_arg(if1, send, read_symbol); 
	  break;
      }
      forv_Vec(Expr, a, args)
	if1_add_send_arg(if1, send, a->ainfo->rval);
      if1_add_send_result(if1, send, s->ainfo->rval);
      break;
    }
    case EXPR_FNCALL:
    case EXPR_ARRAYREF:
    case EXPR_PARENOP: {
      ParenOpExpr *s = dynamic_cast<ParenOpExpr *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      if1_gen(if1, &s->ainfo->code, s->baseExpr->ainfo->code);
      Vec<Expr *> args;
      getLinkElements(args, s->argList);
      if (args.n == 1 && args.v[0]->isNull())
	args.n--;
      forv_Vec(Expr, a, args)
	if1_gen(if1, &s->ainfo->code, a->ainfo->code);
      int use_symbol = undef_or_fn_expr(s->baseExpr);
      Sym *symbol = NULL;
      char *n = s->baseExpr->ainfo->rval->name;
      if (n && !strcmp(n, "__primitive"))
	symbol = sym_primitive;
      else if (n && !strcmp(n, "__operator"))
	symbol = sym_operator;
      else if (use_symbol)
	symbol = gen_move(if1_make_symbol(if1, n), s);
      Code *send = if1_send1(if1, &s->ainfo->code);
      send->ast = s->ainfo;
      if (symbol)
	if1_add_send_arg(if1, send, symbol);
      else
	if1_add_send_arg(if1, send, s->baseExpr->ainfo->rval);
      forv_Vec(Expr, a, args)
	if1_add_send_arg(if1, send, a->ainfo->rval);
      if1_add_send_result(if1, send, s->ainfo->rval);
      s->ainfo->rval->is_lvalue = 1;
      s->ainfo->sym = s->ainfo->rval;
      break;
    }
    case EXPR_CAST: {
      CastExpr *s = dynamic_cast<CastExpr *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      if1_gen(if1, &s->ainfo->code, s->expr->ainfo->code);
      Code *send = if1_send(if1, &s->ainfo->code, 4, 1, sym_primitive, cast_symbol, 
			    s->newType->asymbol->type_sym, s->expr->ainfo->rval, s->ainfo->rval);
      send->ast = s->ainfo;
      break;
    }
    case EXPR_REDUCE: {
      ReduceExpr *s = dynamic_cast<ReduceExpr *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      if1_gen(if1, &s->ainfo->code, s->redDim->ainfo->code);
      if1_gen(if1, &s->ainfo->code, s->argExpr->ainfo->code);
      Code *send = if1_send(if1, &s->ainfo->code, 5, 1, sym_primitive, expr_reduce_symbol, 
			    s->reduceType->asymbol, s->redDim->ainfo->rval, 
			    s->argExpr->ainfo->rval, s->ainfo->rval);
      send->ast = s->ainfo;
      break;
    }
    case EXPR_TUPLE: {
      Tuple *s = dynamic_cast<Tuple *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      Vec<Expr *> args;
      getLinkElements(args, s->exprs);
      forv_Vec(Expr, a, args)
	if1_gen(if1, &s->ainfo->code, a->ainfo->code);
      Code *send = if1_send1(if1, &s->ainfo->code);
      send->ast = s->ainfo;
      if1_add_send_arg(if1, send, sym_make_tuple);
      forv_Vec(Expr, a, args)
	if1_add_send_arg(if1, send, a->ainfo->rval);
      if1_add_send_result(if1, send, s->ainfo->rval);
      break;
    }
    case EXPR_SIZEOF: {
      SizeofExpr *s = dynamic_cast<SizeofExpr *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      Code *send = if1_send(if1, &s->ainfo->code, 3, 1, sym_primitive, sizeof_symbol, 
			    s->type->asymbol->type_sym, s->ainfo->rval);
      send->ast = s->ainfo;
      break;
    }
  case SYMBOL:
  case SYMBOL_UNRESOLVED:
  case SYMBOL_MODULE:
  case SYMBOL_VAR:
  case SYMBOL_PARAM:
  case SYMBOL_TYPE:
  case SYMBOL_CLASS:
  case SYMBOL_REDUCE:
  case SYMBOL_FN:
  case SYMBOL_ENUM:
  case TYPE:
  case TYPE_BUILTIN:
  case TYPE_ENUM:
  case TYPE_DOMAIN:
  case TYPE_INDEX:
  case TYPE_ARRAY:
  case TYPE_USER:
  case TYPE_CLASS:
  case TYPE_TUPLE:
  case AST_TYPE_END:
    assert(!"case");
    break;
  }
  return 0;
}

static int
gen_fun(FnDefStmt *f) {
  Sym *fn = f->fn->asymbol;
  Code *body = 0;
  if1_gen(if1, &body, f->fn->body->ainfo->code);
  AInfo *ast = f->ainfo;
  if1_move(if1, &body, sym_null, fn->ret, ast);
  if1_label(if1, &body, ast, ast->label[0]);
  Code *c = if1_send(if1, &body, 3, 0, sym_reply, fn->cont, fn->ret);
  c->ast = ast;
  Vec<Symbol *> args;
  getLinkElements(args, f->fn->formals);
  Sym *as[args.n + 2];
  int iarg = 0;
  assert(f->fn->asymbol->name);
  if (f->fn->scope->type == SCOPE_CLASS)
    as[iarg++] = fn->self;
  if (strcmp(f->fn->asymbol->name, "self") != 0) {
    as[iarg++] = new_sym(f->fn->asymbol->name);
    as[iarg-1]->type = if1_make_symbol(if1, f->fn->asymbol->name);
  }
  for (int i = 0; i < args.n; i++)
    as[iarg++] = args.v[i]->asymbol;
  if1_closure(if1, fn, body, iarg, as);
  fn->ast = ast;
  return 0;
}

static int
build_function(FnDefStmt *f) {
  Sym *s = f->fn->asymbol;
  if (verbose_level > 1 && f->fn->name)
    printf("build_functions: %s\n", f->fn->name);
  if (s->name && !strcmp("__entryPoint", s->name)) {
    if1_set_builtin(if1, s, "init");
    sym_init = s;
  }
  s->type = sym_function;
  s->type_kind = Type_FUN;
  s->type_sym = s;
  s->cont = new_sym();
  s->cont->ast = f->ainfo;
  s->ret = new_sym();
  s->ret->ast = f->ainfo;
  s->ret->is_lvalue = 1;
  s->labelmap = new LabelMap;
  if (f->fn->scope->type == SCOPE_CLASS) {
    s->self = new_sym("self"); // hack
    s->self->ast = f->ainfo;
    s->self->type = dynamic_cast<TypeSymbol *>(f->fn->scope->symContext)->type->asymbol;
  }
  set_global_scope(s);
  if (define_labels(f->fn->body, f->fn->asymbol->labelmap) < 0) return -1;
  Label *return_label = f->ainfo->label[0] = if1_alloc_label(if1);
  if (resolve_labels(f->fn->body, f->fn->asymbol->labelmap, return_label) < 0) return -1;
  fnsym = f->fn->asymbol; // hack
  if (gen_if1(f->fn->body) < 0) return -1;
  if (gen_fun(f) < 0) return -1;
  return 0;
}

static int
build_classes(Vec<BaseAST *> &syms) {
  Vec<ClassType *> classes;
  forv_BaseAST(s, syms)
    if (s->astType == TYPE_CLASS)
      classes.add(dynamic_cast<ClassType*>(s)); 
  if (verbose_level > 1)
    printf("build_classes: %d classes\n", classes.n);
  forv_Vec(ClassType, c, classes) {
    Sym *csym = c->asymbol;
    Vec<Stmt *> stmts;
    getLinkElements(stmts, c->definition);
    forv_BaseAST(s, stmts) {
      switch (s->astType) {
	default: break;
	case STMT_VARDEF: {
	  VarDefStmt *def = dynamic_cast<VarDefStmt*>(s);
	  VarSymbol *var = def->var;
	  csym->has.add(var->asymbol);
	  break;
	}
      }
    }
  }
  return 0;
}

static int
build_functions(Vec<BaseAST *> &syms) {
  forv_BaseAST(s, syms)
    if (s->astType == STMT_FNDEF)
      if (build_function(dynamic_cast<FnDefStmt*>(s)) < 0)
        return -1;
  return 0;
}

static void
init_symbols() {
  domain_start_index_symbol = if1_make_symbol(if1, "domain_start_index");
  domain_next_index_symbol = if1_make_symbol(if1, "domain_next_index");
  domain_valid_index_symbol = if1_make_symbol(if1, "domain_valid_index");
  expr_simple_seq_symbol = if1_make_symbol(if1, "expr_simple_seq");
  expr_domain_symbol = if1_make_symbol(if1, "expr_domain");
  expr_create_domain_symbol = if1_make_symbol(if1, "expr_create_domain");
  expr_reduce_symbol = if1_make_symbol(if1, "expr_reduce");
  sizeof_symbol = if1_make_symbol(if1, "sizeof");
  cast_symbol = if1_make_symbol(if1, "cast");
  write_symbol = if1_make_symbol(if1, "write");
  writeln_symbol = if1_make_symbol(if1, "writeln");
  read_symbol = if1_make_symbol(if1, "read");
  array_index_symbol = if1_make_symbol(if1, "array_index");
  flood_symbol = if1_make_symbol(if1, "*");
  completedim_symbol = if1_make_symbol(if1, "..");
}

static void
print_ast(BaseAST *a, Vec<BaseAST *> &asts) {
  if (!asts.set_add(a)) {
    printf("(%d *)", (int)a->astType);
    return;
  }
  printf("(%d", (int)a->astType);
  GetStuff getStuff(GET_STMTS|GET_EXPRS);
  a->traverse(&getStuff);
  if (getStuff.asts.n)
    printf(" ");
  forv_BaseAST(b, getStuff.asts)
    print_ast(b, asts);
  printf(")");
}

static void
print_ast(BaseAST *a) {
  Vec<BaseAST *> asts;
  print_ast(a, asts);
  printf("\n");
}

static void
print_baseast(BaseAST *a, Vec<BaseAST *> &asts) {
  if (!asts.set_add(a)) {
    printf("(%d *)", (int)a->astType);
    return;
  }
  printf("(%d", (int)a->astType);
  GetStuff* getStuff = new GetStuff(GET_STMTS|GET_EXPRS);
  a->traverse(getStuff);
  if (getStuff->asts.n)
    printf(" ");
  forv_BaseAST(b, getStuff->asts)
    print_ast(b, asts);
  printf(")");
}

static void
print_baseast(BaseAST *a) {
  Vec<BaseAST *> asts;
  print_baseast(a, asts);
  printf("\n");
}

static void
debug_new_ast(Vec<Stmt *> &stmts, Vec<BaseAST *> &syms) {
  if (verbose_level > 1) {
    forv_Stmt(s, stmts)
      print_baseast(s);
    forv_BaseAST(s, syms)
      if (s->astType == STMT_FNDEF)
	print_ast(dynamic_cast<FnDefStmt*>(s)->fn->body);
  }
}

static void
finalize_symbols(IF1 *i) {
  forv_Sym(s, i->allsyms) {
    if (s->is_constant || s->is_symbol)
      set_global_scope(s);
    else
      if (s->type_kind)
	set_global_scope(s);
  }
}

static void
domain_start_index(PNode *pn, EntrySet *es) {
  forv_Var(v, pn->lvals) {
    AVar *index = make_AVar(v, es);
    creation_point(index, sym_index);
  }
}

static void
domain_next_index(PNode *pn, EntrySet *es) {
  forv_Var(v, pn->lvals) {
    AVar *index = make_AVar(v, es);
    creation_point(index, sym_index);
  }
}

static void
integer_result(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_in(result, make_abstract_type(sym_int));
}

static void
cast_value(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *t = make_AVar(pn->rvals.v[2], es);
  assert(pn->rvals.n == 4);
  Sym *ts = t->var->sym->type_sym;
  if (ts) {
    if (ts->num_kind)
      update_in(result, make_abstract_type(ts));
    else
      creation_point(result, ts);
  }
}

static void
expr_domain(PNode *pn, EntrySet *es) {
  assert(0);
}

static void
expr_reduce(PNode *pn, EntrySet *es) {
  assert(0);
}

static void
expr_simple_seq(PNode *pn, EntrySet *es) {
  AVar *container = make_AVar(pn->lvals.v[0], es);
  creation_point(container, sym_sequence);
}

static void
expr_create_domain(PNode *pn, EntrySet *es) {
  AVar *container = make_AVar(pn->lvals.v[0], es);
  creation_point(container, sym_domain);
}

static void
write_transfer_function(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_in(result, make_abstract_type(sym_int));
}

static void
read_transfer_function(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_in(result, make_abstract_type(sym_int));
}

static void
array_index(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *array = make_AVar(pn->rvals.v[2], es);
  set_container(result, array);
  forv_CreationSet(a, *array->out) if (a) {
    if (a->sym->element) {
      AVar *element = unique_AVar(a->sym->element->var, a);
      flow_vars(element, result);
    }
  }
}

int 
ast_to_if1(Vec<Stmt *> &stmts) {
  Vec<BaseAST *> syms;
  close_symbols(stmts, syms);
  init_symbols();
  debug_new_ast(stmts, syms);
  if (import_symbols(syms) < 0) return -1;
  if (build_classes(syms) < 0) return -1;
  if (build_functions(syms) < 0) return -1;
  pdb->fa->primitive_transfer_functions.put(
    domain_start_index_symbol->name, new RegisteredPrim(domain_start_index));
  pdb->fa->primitive_transfer_functions.put(
    domain_next_index_symbol->name, new RegisteredPrim(domain_next_index));
  pdb->fa->primitive_transfer_functions.put(
    domain_valid_index_symbol->name, new RegisteredPrim(integer_result));
  pdb->fa->primitive_transfer_functions.put(
    expr_simple_seq_symbol->name, new RegisteredPrim(expr_simple_seq));
  pdb->fa->primitive_transfer_functions.put(
    expr_domain_symbol->name, new RegisteredPrim(expr_domain));
  pdb->fa->primitive_transfer_functions.put(
    expr_create_domain_symbol->name, new RegisteredPrim(expr_create_domain));
  pdb->fa->primitive_transfer_functions.put(
    expr_reduce_symbol->name, new RegisteredPrim(expr_reduce));
  pdb->fa->primitive_transfer_functions.put(
    sizeof_symbol->name, new RegisteredPrim(integer_result));
  pdb->fa->primitive_transfer_functions.put(
    cast_symbol->name, new RegisteredPrim(cast_value));
  pdb->fa->primitive_transfer_functions.put(
    write_symbol->name, new RegisteredPrim(write_transfer_function));
  pdb->fa->primitive_transfer_functions.put(
    writeln_symbol->name, new RegisteredPrim(write_transfer_function));
  pdb->fa->primitive_transfer_functions.put(
    read_symbol->name, new RegisteredPrim(read_transfer_function));
  pdb->fa->primitive_transfer_functions.put(
    array_index_symbol->name, new RegisteredPrim(array_index));
  if1_set_primitive_types(if1);
  finalize_types(if1);
  sym_null->is_external = 1;	// hack
  finalize_symbols(if1);
  return 0;
}

int
AST_to_IF1(Vec<Stmt *> &stmts) {
  if (ast_to_if1(stmts) < 0)
    fail("unable to analyze AST\n");
  return 0;
}

void 
print_AST_Expr_types(BaseAST *ast) {
  GetStuff getStuff(GET_STMTS|GET_EXPRS);
  ast->traverse(&getStuff);
  forv_BaseAST(a, getStuff.asts)
    print_AST_Expr_types(a);
  Expr *x = dynamic_cast<Expr*>(ast);
  if (x) {
    if (x->ainfo->rval && x->ainfo->rval->var) {
      printf("%s %d %s %d\n", x->ainfo->rval->name ? x->ainfo->rval->name : "", 
                           x->ainfo->rval->id, 
                           x->ainfo->rval->var->type->name ?  x->ainfo->rval->var->type->name : "", 
                           x->ainfo->rval->var->type->id);
      printf("%X\n", (int)type_info(x->ainfo));
    }
    Type *t = type_info(ast);
    assert(t);
  }
}

void 
print_AST_types() {
  forv_Fun(f, pdb->fa->funs) {
    AInfo *a = dynamic_cast<AInfo *>(f->ast);
    FnDefStmt *def = dynamic_cast<FnDefStmt *>(a->xast);
    print_AST_Expr_types(def->fn->body);
  }
}

static void
ast_sym_info(BaseAST *a, Symbol *s, AST **ast, Sym **sym) {
  *ast = 0;
  *sym = 0;
  Expr *e = dynamic_cast<Expr *>(a);
  if (e)
    *ast = e->ainfo;
  else {
    Stmt *stmt = dynamic_cast<Stmt *>(a);
    if (stmt)
       *ast = stmt->ainfo;
    else {
      Symbol *symbol = dynamic_cast<Symbol *>(a);
      if (symbol)
        *sym = symbol->asymbol;
      else {
        Type *t = dynamic_cast<Type *>(a);
        *sym = t->asymbol;
      }
    }
  }
  if (!sym)
    *sym = s->asymbol;
}

Type *
type_info(BaseAST *a, Symbol *s) {
  AST *ast = 0;
  Sym *sym = 0;
  ast_sym_info(a, s, &ast, &sym);
  Sym *type = 0;
  if (ast) {
    type = type_info(ast, sym);
    goto Ldone;
  }
#ifdef COMPLETE_TYPING
  assert(sym);
#endif
  if (!sym)
    return dtUnknown;
  if (sym->var) {
    type = sym->var->type;
    goto Ldone;
  }
  if (sym->type) {
    type = sym->type;
    goto Ldone;
  }
 Ldone:
#ifdef COMPLETE_TYPING
  assert(type);
#endif
  if (!type)
    return dtUnknown;
  ASymbol *asymbol = dynamic_cast<ASymbol *>(type);
  BaseAST *atype = asymbol->xsymbol;
  Type *btype = dynamic_cast<Type *>(atype);
#ifdef COMPLETE_TYPING
  assert(btype);
#endif
  if (!btype)
    return dtUnknown;
  return btype;
}

void 
call_info(FnSymbol *f, BaseAST *a, Vec<FnSymbol *> &fns) {
  fns.clear();
  Fun *ff = f->asymbol->fun;
  AST *ast = 0;
  Expr *e = dynamic_cast<Expr *>(a);
  if (e)
    ast = e->ainfo;
  else {
    Stmt *stmt = dynamic_cast<Stmt *>(a);
    if (stmt)
       ast = stmt->ainfo;
  }
  assert(ast);
  Vec<Fun *> funs;
  call_info(ff, ast, funs);
  forv_Fun(f, funs) {
    ASymbol *asymbol = dynamic_cast<ASymbol *>(f->sym);
    FnSymbol *fs = dynamic_cast<FnSymbol *>(asymbol->xsymbol);
    assert(fs);
    fns.add(fs);
  }
}

int 
constant_info(BaseAST *a, Vec<Symbol *> &constants, Symbol *s) {
  constants.clear();
  AST *ast = 0;
  Sym *sym = 0;
  ast_sym_info(a, s, &ast, &sym);
  Vec<Sym *> consts;
  constant_info(ast, consts, sym);
  forv_Sym(ss, consts) {
    ASymbol *asymbol = dynamic_cast<ASymbol *>(ss);
    Symbol *fs = dynamic_cast<Symbol *>(asymbol->xsymbol);
    assert(fs);
    constants.add(fs);
  }
  return constants.n;
}

