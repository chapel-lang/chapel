/*
  Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include <typeinfo>
#include "geysa.h"
#include "analysis.h"
#include "expr.h"
#include "files.h"
#include "link.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "builtin.h"
#include "if1.h"

class LabelMap : public Map<char *, BaseAST *> {};

Sym *__domain_start_index = 0;
Sym *__domain_next_index = 0;
Sym *__domain_valid_index = 0;

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
  return NULL;
}

AST *AInfo::copy(Map<PNode *, PNode*> *nmap) {
  return NULL;
}

static void
close_symbols(BaseAST *a, Vec<BaseAST *> &syms) {
  Vec<BaseAST *> set;
  while (a) {
    set.set_add(a);
    syms.add(a);
    a = dynamic_cast<BaseAST *>(a->next);
  }
  forv_BaseAST(s, syms) {
    Vec<BaseAST *> moresyms;
    s->getBaseASTs(moresyms);
    forv_BaseAST(ss, moresyms) {
      assert(ss);
      if (set.set_add(ss))
        syms.add(ss);
    }
  }
}

static void
map_symbols(Vec<BaseAST *> &syms) {
  int symbols = 0, types = 0, exprs = 0, stmts = 0;
  if (verbose_level > 1)
    printf("map_symbols: %d\n", syms.n);
  forv_BaseAST(s, syms) {
    Symbol *sym = dynamic_cast<Symbol *>(s);
    if (sym) {
      sym->asymbol = new ASymbol;
      sym->asymbol->xsymbol = sym;
      symbols++;
      if (verbose_level > 1 && sym->name)
        printf("map_symbols: found Symbol '%s'\n", sym->name);
    } else {
      Type *t = dynamic_cast<Type *>(s);
      if (t) {
	t->asymbol = new ASymbol;
	t->asymbol->xsymbol = t;
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
build_enum_element(Sym *ss) {
  ss->type_kind = Type_PRIMITIVE;
  ss->implements.add(sym_enum_element);
  ss->includes.add(sym_enum_element);
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
	} else if (t == dtFloat) {
	  t->asymbol->type_kind = Type_ALIAS;
	  t->asymbol->alias = sym_float;
	} else if (t == dtString) {
	  t->asymbol->type_kind = Type_ALIAS;
	  t->asymbol->alias = sym_string;
	} else
	  t->asymbol->type_kind = Type_UNKNOWN;
	break;
      case TYPE_ENUM: {
	t->asymbol->type_kind = Type_TAGGED;
	Vec<BaseAST *> syms;
	t->getSymbols(syms);
	forv_BaseAST(s, syms) {
	  Sym *ss = dynamic_cast<Symbol*>(s)->asymbol;
	  build_enum_element(ss);
	  t->asymbol->has.add(ss);
	}
	break;
      }
      case TYPE_DOMAIN: build_record_type(t, sym_domain); break;
      case TYPE_INDEX: build_record_type(t, sym_tuple); break;
      case TYPE_SUBDOMAIN: build_record_type(t, sym_domain); break;
      case TYPE_SUBINDEX: build_record_type(t, sym_tuple); break;
      case TYPE_ARRAY: build_record_type(t, sym_array); break;
      case TYPE_USER: {
	UserType *tt = dynamic_cast<UserType*>(t);
	t->asymbol->type_kind = Type_ALIAS;
	t->asymbol->alias = tt->definition->asymbol;
	break;
      }
      case TYPE_CLASS: {
	ClassType *tt = dynamic_cast<ClassType*>(t);
	t->asymbol->type_kind = Type_RECORD;
	t->asymbol->implements.add(tt->parentClass->asymbol);
	t->asymbol->includes.add(tt->parentClass->asymbol);
	break;
      }
    }
  }
}

static Sym *
new_sym(char *name = 0) {
  return if1_alloc_sym(if1, name);
}

static void
new_primitive_type(Sym *&sym, char *name) {
  if (!sym)
    sym = new_sym(name);
  sym->type_kind = Type_PRIMITIVE;
}

static void
new_alias_type(Sym *&sym, char *name, Sym *alias) {
  if (!sym)
    sym = new_sym(name);
  sym->type_kind = Type_ALIAS;
  sym->alias = alias;
}

static void
new_sum_type(Sym *&sym, char *name, ...)  {
  if (!sym)
    sym = new_sym(name);
  sym->type_kind = Type_SUM;
  va_list ap;
  va_start(ap, name);
  Sym *s = 0;
  do {
    if ((s = va_arg(ap, Sym*)))
      sym->has.add(s);
  } while (s);
}

static void
new_global_variable(Sym *&sym, char *name) {
  if (!sym)
    sym = new_sym(name);
  sym->global_scope = 1;
}

static void
new_symbol(Sym *&sym, char *name) {
  if (!sym)
    sym = if1_make_symbol(if1, name);
}

static void
build_builtin_symbols() {
  if (!sym_system)
    sym_system = new_sym("system");
  if (!sym_system->init)
    sym_system->init = new_sym("__init");
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
  new_primitive_type(sym_tuple, "tuple");
  new_primitive_type(sym_void, "void");
  if (!sym_object)
    sym_object = new_sym("object");
  sym_object->type_kind = Type_RECORD;
  new_primitive_type(sym_list, "list");
  new_primitive_type(sym_ref, "ref");
  new_primitive_type(sym_value, "value");
  new_primitive_type(sym_catagory, "catagory");
  new_primitive_type(sym_set, "set");
  new_primitive_type(sym_sequence, "sequence");
  new_primitive_type(sym_domain, "domain");
  new_primitive_type(sym_array, "array");
  new_primitive_type(sym_int8, "int8");
  new_primitive_type(sym_int16, "int16");
  new_primitive_type(sym_int32, "int32");
  new_primitive_type(sym_int64, "int64");
  new_alias_type(sym_int, "int", sym_int64);
  new_primitive_type(sym_uint8, "uint8");
  new_primitive_type(sym_uint16, "uint16");
  new_primitive_type(sym_uint32, "uint32");
  new_primitive_type(sym_uint64, "uint64");
  new_alias_type(sym_uint, "uint", sym_uint64);
  new_sum_type(sym_anyint, "anyint", 
	       sym_int8, sym_int16, sym_int32, sym_int64,
	       sym_uint8, sym_uint16, sym_uint32, sym_uint64, 0);
  new_primitive_type(sym_size, "size");
  new_alias_type(sym_bool, "bool", sym_int);
  new_primitive_type(sym_enum_element, "enum_element");
  new_primitive_type(sym_float32, "float32");
  new_primitive_type(sym_float64, "float64");
  new_primitive_type(sym_float80, "float80");
  new_primitive_type(sym_float128, "float128");
  new_primitive_type(sym_float, "float");
  new_sum_type(sym_anyfloat, "anyfloat", 
	       sym_float32, sym_float64, sym_float80, sym_float128, 0);
  new_primitive_type(sym_anynum, "anynum");
  new_primitive_type(sym_char, "char");
  new_primitive_type(sym_string, "string");
  if1_set_symbols_type(if1);
  
  new_global_variable(sym_null, "null");
  new_symbol(sym_reply, "reply");
  new_symbol(sym_make_tuple, "make_tuple");
  new_symbol(sym_make_vector, "make_vector");
  new_symbol(sym_make_list, "make_list");
  new_symbol(sym_make_continuation, "make_continuation");
  new_symbol(sym_make_set, "make_set");
  new_symbol(sym_new, "new");
  new_symbol(sym_new_object, "new_object");
  new_symbol(sym_index, "index");
  new_symbol(sym_operator, "operator");
  new_symbol(sym_print, "print");
  new_symbol(sym_destruct, "destruct");
  new_symbol(sym_meta_apply, "meta_apply");
  new_symbol(sym_period, "period");
  new_symbol(sym_assign, "assign");
  new_global_variable(sym_init, "init");
#define S(_n) assert(sym_##_n);
#include "builtin_symbols.h"
#undef S
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
  Vec<BaseAST *> asts;
  ast->getStmts(asts);
  forv_BaseAST(a, asts)
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
  Vec<BaseAST *> asts;
  ast->getStmts(asts);
  forv_BaseAST(a, asts)
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
    if (typeid(var->type) != typeid(ClassType))
      s->is_var = 1;
  }
  if (def->init) {
    if1_gen(if1, &def->ainfo->code, def->init->ainfo->code);
    if1_move(if1, &def->ainfo->code, def->init->ainfo->rval, def->ainfo->sym, def->ainfo);
  } else if (!s->is_var) {
    show_error("missing value initializer", def->ainfo);
    return -1;
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
gen_for(BaseAST *a) {
  ForLoopStmt *s = dynamic_cast<ForLoopStmt*>(a);
  Code *body_code = 0, *send;
  Vec<Stmt*> body;
  getLinkElements(body, s->body);
  forv_Stmt(ss, body)
    if1_gen(if1, &body_code, ss->ainfo->code);
  Sym *index = new_sym();
  Code *setup_code = 0;
  if1_gen(if1, &setup_code, s->domain->ainfo->code);
  send = if1_send(if1, &setup_code, 2, 1, __domain_start_index, s->domain->ainfo->rval, index);
  send->ast = s->ainfo;
  Sym *condition_rval = 0;
  Code *condition_code = 0;
  send = if1_send(if1, &condition_code, 3, 1, __domain_valid_index, 
	   s->domain->ainfo->rval, index, condition_rval);
  send->ast = s->ainfo;
  send =if1_send(if1, &body_code, 3, 1, __domain_next_index, 
	   s->domain->ainfo->rval, index, condition_rval);
  send->ast = s->ainfo;
  if1_loop(if1, &s->ainfo->code, s->ainfo->label[0], s->ainfo->label[1],
	   condition_rval, setup_code, 
	   condition_code, 0, 
	   body_code, s->ainfo);
  return 0;
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
gen_if1(BaseAST *ast) {
  // bottom's up
  Vec<BaseAST *> asts;
  ast->getStmtExprs(asts);
  forv_BaseAST(a, asts) 
    if (gen_if1(a) < 0)
      return -1;
  switch (ast->astType) {
    case STMT_NOOP: break;
    case STMT_VARDEF: if (gen_vardef(ast) < 0) return -1; break;
    case STMT_TYPEDEF: break;
    case STMT_FNDEF: break;
    case STMT_EXPR: if (gen_expr_stmt(ast) < 0) return -1; break;
#if 0
    case STMT_RETURN: 
      if1_move(if1, &ast->ainfo->code, ast->expr->ainfo->rval, ast->scope->in->ret, ast);
      // fall through
    case STMT_BREAK:
    case STMT_CONTINUE:
    {
      Code *c = if1_goto(i, &ast->code, ast->label[0]);
      c->ast = ast;
      break;
    }
#endif
    case STMT_BLOCK: break;
    case STMT_WHILELOOP: gen_while(ast); break;
    case STMT_FORLOOP: gen_for(ast); break;
    case STMT_COND: gen_cond(ast); break;
      
    case EXPR_INTLITERAL: {
      IntLiteral *s = dynamic_cast<IntLiteral*>(ast);
      s->ainfo->rval = new_sym(s->str);
      s->ainfo->rval->constant = s->str;
      s->ainfo->rval->type = sym_int64;
      s->ainfo->rval->imm.v_int64 = s->val;
      break;
    }
    case EXPR_FLOATLITERAL: {
      FloatLiteral *s = dynamic_cast<FloatLiteral*>(ast);
      s->ainfo->rval = new_sym(s->str);
      s->ainfo->rval->constant = s->str;
      s->ainfo->rval->type = sym_float64;
      s->ainfo->rval->imm.v_float64 = s->val;
      break;
    }
    case EXPR_STRINGLITERAL: {
      StringLiteral *s = dynamic_cast<StringLiteral*>(ast);
      s->ainfo->rval = new_sym(s->str);
      s->ainfo->rval->constant = s->str;
      s->ainfo->rval->type = sym_string;
      break;
    }
    case EXPR_VARIABLE: {
      Variable *s = dynamic_cast<Variable*>(ast);
      s->ainfo->rval = s->var->asymbol;
      break;
    }
    case EXPR_UNOP: {
      UnOp *s = dynamic_cast<UnOp*>(ast);
      s->ainfo->rval = new_sym();
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
	case BINOP_NEQUALS: op = if1_make_symbol(if1, "!="); break;
	case BINOP_BITAND: op = if1_make_symbol(if1, "&"); break;
	case BINOP_BITOR: op = if1_make_symbol(if1, "|"); break;
	case BINOP_BITXOR: op = if1_make_symbol(if1, "^"); break;
	case BINOP_BITSL: op = if1_make_symbol(if1, "<<"); break;
	case BINOP_BITSR: op = if1_make_symbol(if1, ">>"); break;
	case BINOP_LOGAND: op = if1_make_symbol(if1, "&&"); break;
	case BINOP_LOGOR: op = if1_make_symbol(if1, "||"); break;
	case BINOP_EXP: op = if1_make_symbol(if1, "**"); break;
	case BINOP_BY: op = if1_make_symbol(if1, "by"); break;
	case BINOP_DOT: op = if1_make_symbol(if1, "."); break;
      }
      Code *c = if1_send(if1, &s->ainfo->code, 4, 1, sym_operator, op, 
			 s->left->ainfo->rval, s->right->ainfo->rval,
			 s->ainfo->rval);
      c->ast = s->ainfo;
      break;
    }
    case EXPR_ASSIGNOP: {
      AssignOp *s = dynamic_cast<AssignOp*>(ast);
      s->ainfo->rval = new_sym();
      if1_gen(if1, &s->ainfo->code, s->left->ainfo->code);
      if1_gen(if1, &s->ainfo->code, s->right->ainfo->code);
      Sym *op = 0;
      switch (s->type) {
	default: assert(!"case");
	case GETS_NORM: op = if1_make_symbol(if1, "="); break;
	case GETS_PLUS: op = if1_make_symbol(if1, "+="); break;
	case GETS_MINUS: op = if1_make_symbol(if1, "-="); break;
	case GETS_TIMES: op = if1_make_symbol(if1, "*="); break;
	case GETS_DIV: op = if1_make_symbol(if1, "/="); break;
	case GETS_BITAND: op = if1_make_symbol(if1, "&="); break;
	case GETS_BITOR: op = if1_make_symbol(if1, "|="); break;
	case GETS_BITXOR: op = if1_make_symbol(if1, "^="); break;
	case GETS_LSH: op = if1_make_symbol(if1, "<<="); break;
	case GETS_RSH: op = if1_make_symbol(if1, ">>="); break;
      }
      Code *c = if1_send(if1, &s->ainfo->code, 4, 1, sym_operator, op, 
			 s->left->ainfo->rval, s->right->ainfo->rval,
			 s->ainfo->rval);
      c->ast = s->ainfo;
      break;
    }
#if 0
  EXPR_SIMPLESEQ,
  EXPR_FLOOD,
  EXPR_COMPLETEDIM,
  EXPR_DOMAIN,
  EXPR_PARENOP,
  EXPR_CAST,
  EXPR_FNCALL,
  EXPR_WRITECALL,
  EXPR_ARRAYREF,
  EXPR_REDUCE,
  EXPR_TUPLE,
#endif
    default: assert(0);
  }
  return 0;
}

static int
build_functions(Vec<BaseAST *> &syms) {
  Vec<FnDefStmt *> fns;
  forv_BaseAST(s, syms)
    if (s->astType == STMT_FNDEF)
      fns.add(dynamic_cast<FnDefStmt*>(s)); 
  forv_Vec(FnDefStmt, f, fns) {
    Sym *s = f->fn->asymbol;
    s->name = f->fn->name;
    s->cont = new_sym();
    s->cont->ast = f->ainfo;
    s->labelmap = new LabelMap;
  }
  forv_Vec(FnDefStmt, f, fns) {
    if (define_labels(f->fn->body, f->fn->asymbol->labelmap) < 0) return -1;
    Label *return_label = f->ainfo->label[0] = if1_alloc_label(if1);
    if (resolve_labels(f->fn->body, f->fn->asymbol->labelmap, return_label) < 0) return -1;
    if (gen_if1(f) < 0) return -1;
  }
  return 0;
}

static int
import_symbols(Vec<BaseAST *> &syms) {
  map_symbols(syms);
  build_builtin_symbols();
  build_types(syms);
  return 0;
}

static void
init_symbols() {
  __domain_start_index = if1_make_symbol(if1, "__domain_start_index");
  __domain_next_index = if1_make_symbol(if1, "__domain_next_index");
  __domain_valid_index = if1_make_symbol(if1, "__domain_valid_index");
}

int
analyze_new_ast(BaseAST* a) {
  Vec<BaseAST *> syms;
  close_symbols(a, syms);
  init_symbols();
  if (import_symbols(syms) < 0) return -1;
  if (build_functions(syms) < 0) return -1;
  return 0;
}


