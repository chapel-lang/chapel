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
#include "fa.h"
#include "pdb.h"
#include "pnode.h"

class LabelMap : public Map<char *, BaseAST *> {};

static Sym *domain_start_index_symbol = 0;
static Sym *domain_next_index_symbol = 0;
static Sym *domain_valid_index_symbol = 0;
static Sym *expr_simple_seq_symbol = 0;
static Sym *expr_domain_symbol = 0;
static Sym *expr_reduce_symbol = 0;
static Sym *write_symbol = 0;
static Sym *flood_symbol = 0;
static Sym *completedim_symbol = 0;

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
set_global_scope(Sym *s) {
  s->function_scope = 0;
  s->global_scope = 1;
}

static ASymbol *
new_ASymbol(char *name = 0) {
  ASymbol *s = new ASymbol;
  if1_register_sym(if1, s, name);
  return s;
}

static void
map_symbols(Vec<BaseAST *> &syms) {
  int symbols = 0, types = 0, exprs = 0, stmts = 0;
  if (verbose_level > 1)
    printf("map_symbols: %d\n", syms.n);
  forv_BaseAST(s, syms) {
    Symbol *sym = dynamic_cast<Symbol *>(s);
    if (sym) {
      sym->asymbol = new_ASymbol(sym->name);
      sym->asymbol->xsymbol = sym;
      sym->asymbol->function_scope = 1;
      symbols++;
      if (verbose_level > 1 && sym->name)
        printf("map_symbols: found Symbol '%s'\n", sym->name);
    } else {
      Type *t = dynamic_cast<Type *>(s);
      if (t) {
	t->asymbol = new_ASymbol(t->name->name);
	t->asymbol->xsymbol = t;
	set_global_scope(t->asymbol);
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
new_sym(char *name = 0, int global = 0) {
  Sym *s = if1_alloc_sym(if1, name);
  if (!global)
    s->function_scope = 1;
  else
    s->global_scope = 1;
  return s;
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
}

static void
new_global_variable(Sym *&sym, char *name) {
  if (!sym)
    sym = new_sym(name, 1);
  sym->global_scope = 1;
  if1_set_builtin(if1, sym, name);
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
  new_primitive_type(sym_tuple, "tuple");
  new_primitive_type(sym_void, "void");
  if (!sym_object)
    sym_object = new_sym("object", 1);
  sym_object->type_kind = Type_RECORD;
  if1_set_builtin(if1, sym_object, "object");
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
  if (!sym_new_object) {
    sym_new_object = new_sym("new_object", 1);
    if1_set_builtin(if1, sym_new_object, "new_object");
  }
  
  new_global_variable(sym_null, "null");
  sym_init = new_sym(); // placeholder
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
  send = if1_send(if1, &setup_code, 3, 1, sym_primitive, domain_start_index_symbol, s->domain->ainfo->rval, 
		  index);
  send->ast = s->ainfo;
  Sym *condition_rval = 0;
  Code *condition_code = 0;
  send = if1_send(if1, &condition_code, 4, 1, sym_primitive, domain_valid_index_symbol,
		  s->domain->ainfo->rval, index,
		  condition_rval);
  send->ast = s->ainfo;
  send = if1_send(if1, &body_code, 4, 1, sym_primitive, domain_next_index_symbol, 
		  s->domain->ainfo->rval, index, 
		  condition_rval);
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
    case STMT: assert(!"case"); break;
    case STMT_NOOP: break;
    case STMT_VARDEF: if (gen_vardef(ast) < 0) return -1; break;
    case STMT_TYPEDEF: break;
    case STMT_FNDEF: break;
    case STMT_EXPR: if (gen_expr_stmt(ast) < 0) return -1; break;
    case STMT_RETURN: {
      ReturnStmt *s = dynamic_cast<ReturnStmt*>(ast);
      Sym *fn = s->parentFn->asymbol;
      if (!s->expr->isNull())
	if1_move(if1, &s->ainfo->code, s->expr->ainfo->rval, fn->ret, s->ainfo);
      Code *c = if1_goto(if1, &s->ainfo->code, s->ainfo->label[0]);
      c->ast = s->ainfo;
      break;
    }
#if 0
      // fall through
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
      
    case EXPR: assert(!"case"); break;
    case EXPR_LITERAL: assert(!"case"); break;
    case EXPR_INTLITERAL: {
      IntLiteral *s = dynamic_cast<IntLiteral*>(ast);
      s->ainfo->rval = if1_const(if1, sym_int64, s->str);
      s->ainfo->rval->imm.v_int64 = s->val;
      break;
    }
    case EXPR_FLOATLITERAL: {
      FloatLiteral *s = dynamic_cast<FloatLiteral*>(ast);
      s->ainfo->rval = if1_const(if1, sym_float64, s->str);
      s->ainfo->rval->imm.v_float64 = s->val;
      break;
    }
    case EXPR_STRINGLITERAL: {
      StringLiteral *s = dynamic_cast<StringLiteral*>(ast);
      s->ainfo->rval = if1_const(if1, sym_string, s->str);
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
	case GETS_MULT: op = if1_make_symbol(if1, "*="); break;
	case GETS_DIV: op = if1_make_symbol(if1, "/="); break;
	case GETS_BITAND: op = if1_make_symbol(if1, "&="); break;
	case GETS_BITOR: op = if1_make_symbol(if1, "|="); break;
	case GETS_BITXOR: op = if1_make_symbol(if1, "^="); break;
	case GETS_BITSL: op = if1_make_symbol(if1, "<<="); break;
	case GETS_BITSR: op = if1_make_symbol(if1, ">>="); break;
      }
      Code *c = if1_send(if1, &s->ainfo->code, 4, 1, sym_operator, op, 
			 s->left->ainfo->rval, s->right->ainfo->rval,
			 s->ainfo->rval);
      c->ast = s->ainfo;
      break;
    }
    case EXPR_SIMPLESEQ: {
      SimpleSeqExpr *s = dynamic_cast<SimpleSeqExpr *>(ast);
      s->ainfo->rval = new_sym();
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
      if1_gen(if1, &s->ainfo->code, s->domains->ainfo->code);
      if1_gen(if1, &s->ainfo->code, s->forallExpr->ainfo->code);
      Code *send = if1_send(if1, &s->ainfo->code, 5, 1, sym_primitive, expr_domain_symbol, 
			    s->domains->ainfo->rval, s->indices->asymbol, s->forallExpr->ainfo->rval, 
			    s->ainfo->rval);
      send->ast = s->ainfo;
      break;
    }
    case EXPR_WRITECALL: {
      WriteCall *s = dynamic_cast<WriteCall *>(ast);
      s->ainfo->rval = new_sym();
      if1_gen(if1, &s->ainfo->code, s->baseExpr->ainfo->code);
      Vec<Expr *> args;
      getLinkElements(args, s->argList);
      forv_Vec(Expr, a, args)
	if1_gen(if1, &s->ainfo->code, a->ainfo->code);
      Code *send = if1_send1(if1, &s->ainfo->code);
      send->ast = s->ainfo;
      if1_add_send_arg(if1, send, sym_primitive);
      if1_add_send_arg(if1, send, write_symbol);
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
      if1_gen(if1, &s->ainfo->code, s->baseExpr->ainfo->code);
      Vec<Expr *> args;
      getLinkElements(args, s->argList);
      forv_Vec(Expr, a, args)
	if1_gen(if1, &s->ainfo->code, a->ainfo->code);
      Code *send = if1_send1(if1, &s->ainfo->code);
      send->ast = s->ainfo;
      assert(s->baseExpr->ainfo->rval->name);
      if1_add_send_arg(if1, send, if1_make_symbol(if1, s->baseExpr->ainfo->rval->name));
      forv_Vec(Expr, a, args)
	if1_add_send_arg(if1, send, a->ainfo->rval);
      if1_add_send_result(if1, send, s->ainfo->rval);
      break;
    }
    case EXPR_CAST: {
      CastExpr *s = dynamic_cast<CastExpr *>(ast);
      s->ainfo->rval = new_sym();
      Vec<Expr *> args;
      getLinkElements(args, s->argList);
      forv_Vec(Expr, a, args)
	if1_gen(if1, &s->ainfo->code, a->ainfo->code);
      Code *send = if1_send1(if1, &s->ainfo->code);
      send->ast = s->ainfo;
      if1_add_send_arg(if1, send, s->castType->asymbol->type_sym);
      forv_Vec(Expr, a, args)
	if1_add_send_arg(if1, send, a->ainfo->rval);
      if1_add_send_result(if1, send, s->ainfo->rval);
      break;
    }
    case EXPR_REDUCE: {
      ReduceExpr *s = dynamic_cast<ReduceExpr *>(ast);
      s->ainfo->rval = new_sym();
      if1_gen(if1, &s->ainfo->code, s->redDim->ainfo->code);
      if1_gen(if1, &s->ainfo->code, s->argExpr->ainfo->code);
      Code *send = if1_send(if1, &s->ainfo->code, 5, 1, sym_primitive, expr_reduce_symbol, 
			    s->reduceType->asymbol, s->redDim->ainfo->rval, s->argExpr->ainfo->rval, 
			    s->ainfo->rval);
      send->ast = s->ainfo;
      break;
    }
    case EXPR_TUPLE: {
      Tuple *s = dynamic_cast<Tuple *>(ast);
      s->ainfo->rval = new_sym();
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
  case SYMBOL:
  case SYMBOL_USEBEFOREDEF:
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
  Sym *as[args.n + 1];
  assert(f->fn->asymbol->name);
  as[0] = if1_make_symbol(if1, f->fn->asymbol->name);
  for (int i = 1; i < args.n + 1; i++)
    as[i] = args.v[i-1]->asymbol;
  if1_closure(if1, fn, body, args.n + 1, as);
  fn->ast = ast;
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
    if (s->name && !strcmp("__init", s->name)) {
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
    s->labelmap = new LabelMap;
    set_global_scope(s);
  }
  forv_Vec(FnDefStmt, f, fns) {
    if (define_labels(f->fn->body, f->fn->asymbol->labelmap) < 0) return -1;
    Label *return_label = f->ainfo->label[0] = if1_alloc_label(if1);
    if (resolve_labels(f->fn->body, f->fn->asymbol->labelmap, return_label) < 0) return -1;
    if (gen_if1(f->fn->body) < 0) return -1;
    if (gen_fun(f) < 0) return -1;
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
  domain_start_index_symbol = if1_make_symbol(if1, "domain_start_index");
  domain_next_index_symbol = if1_make_symbol(if1, "domain_next_index");
  domain_valid_index_symbol = if1_make_symbol(if1, "domain_valid_index");
  expr_simple_seq_symbol = if1_make_symbol(if1, "expr_simple_seq");
  expr_domain_symbol = if1_make_symbol(if1, "expr_domain");
  expr_reduce_symbol = if1_make_symbol(if1, "expr_reduce");
  write_symbol = if1_make_symbol(if1, "write");
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
  Vec<BaseAST *> aa;
  a->getStmtExprs(aa);
  if (aa.n)
    printf(" ");
  forv_BaseAST(b, aa)
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
  Vec<BaseAST *> aa;
  a->getBaseASTs(aa);
  if (aa.n)
    printf(" ");
  forv_BaseAST(b, aa)
    print_baseast(b, asts);
  printf(")");
}

static void
print_baseast(BaseAST *a) {
  Vec<BaseAST *> asts;
  print_baseast(a, asts);
  printf("\n");
}

static void
debug_new_ast(BaseAST *a, Vec<BaseAST *> &syms) {
  if (verbose_level > 1) {
    print_baseast(a);
    forv_BaseAST(s, syms)
      if (s->astType == STMT_FNDEF)
	print_ast(dynamic_cast<FnDefStmt*>(s)->fn->body);
  }
}

static void
domain_start_index(PNode *pn, EntrySet *es) {
}

static void
domain_next_index(PNode *pn, EntrySet *es) {
}

static void
domain_valid_index(PNode *pn, EntrySet *es) {
}

static void
expr_simple_seq(PNode *pn, EntrySet *es) {
}

static void
expr_domain(PNode *pn, EntrySet *es) {
}

static void
expr_reduce(PNode *pn, EntrySet *es) {
}

static void
finalize_symbols(IF1 *i) {
  forv_Sym(s, i->allsyms) {
    if (s->is_constant || s->is_symbol)
      s->function_scope = 1;
    else
      if (s->type_kind)
	s->global_scope = 1;
  }
}

static void
write_transfer_function(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_in(result, make_abstract_type(sym_int));
}

int
AST_to_IF1(BaseAST* a) {
  Vec<BaseAST *> syms;
  close_symbols(a, syms);
  init_symbols();
  debug_new_ast(a, syms);
  if (import_symbols(syms) < 0) return -1;
  if (build_functions(syms) < 0) return -1;
  pdb->fa->primitive_transfer_functions.put(domain_start_index_symbol, domain_start_index);
  pdb->fa->primitive_transfer_functions.put(domain_next_index_symbol, domain_next_index);
  pdb->fa->primitive_transfer_functions.put(domain_valid_index_symbol, domain_valid_index);
  pdb->fa->primitive_transfer_functions.put(expr_simple_seq_symbol, expr_simple_seq);
  pdb->fa->primitive_transfer_functions.put(expr_domain_symbol, expr_domain);
  pdb->fa->primitive_transfer_functions.put(expr_reduce_symbol, expr_reduce);
  pdb->fa->primitive_transfer_functions.put(write_symbol, write_transfer_function);
  finalize_types(if1);
  sym_null->is_external = 1;	// hack
  finalize_symbols(if1);
  return 0;
}


