#include "buildClassConstructorsEtc.h"
#include "symtab.h"
#include "symscope.h"
#include "symbol.h"
#include "type.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "../traversals/fixup.h"

//#define CONSTRUCTOR_WITH_PARAMETERS


static void build_constructor(ClassType* class_type) {
  SymScope* saveScope = Symboltable::setCurrentScope(class_type->classScope);

  char* name = glomstrings(2, "_construct_", class_type->symbol->name);
  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol(name));
  class_type->defaultConstructor = fn;
  fn->isConstructor = true;
  fn->cname = glomstrings(2, "_construct_", class_type->symbol->cname);

  ParamSymbol* args = NULL;
  forv_Vec(VarSymbol, tmp, class_type->fields) {
#ifdef CONSTRUCTOR_WITH_PARAMETERS
    args = appendLink(args, new ParamSymbol(PARAM_BLANK, tmp->name, tmp->type, tmp->init ? tmp->init->copy() : tmp->type->defaultVal->copy()));
#else
#endif
  }

  BlockStmt* body = Symboltable::startCompoundStmt();
  Stmt* stmts = NULL;
  fn->_this = new VarSymbol("this", class_type);
  DefExpr* def_expr = new DefExpr(fn->_this);
  stmts = new DefStmt(def_expr);
  if (!class_type->value) {
    char* description = glomstrings(2, "instance of class ", class_type->symbol->name);
    Expr* alloc_args = new IntLiteral("1", 1);
    alloc_args = appendLink(alloc_args, new SizeofExpr(class_type));
    alloc_args = appendLink(alloc_args, new StringLiteral(description));
    Symbol* alloc_sym = Symboltable::lookupInternal("_chpl_malloc");
    Expr* alloc_call = new FnCall(new Variable(alloc_sym), alloc_args);
    Expr* alloc_lhs = new Variable(fn->_this);
    Expr* alloc_rhs = new CastExpr(class_type, alloc_call);
    Expr* alloc_expr = new AssignOp(GETS_NORM, alloc_lhs, alloc_rhs);
    Stmt* alloc_stmt = new ExprStmt(alloc_expr);
    stmts = appendLink(stmts, alloc_stmt);
  }
  class_type->buildConstructorBody(stmts, fn->_this);

  stmts = appendLink(stmts, new ReturnStmt(new Variable(fn->_this)));
  body = Symboltable::finishCompoundStmt(body, stmts);
  Expr* fn_def = Symboltable::finishFnDef(fn, args, class_type, body);
  class_type->constructor = new DefStmt(fn_def);
  SET_BACK(class_type->constructor);
  Symboltable::setCurrentScope(saveScope);
  if (dynamic_cast<SeqType*>(class_type)) {
    class_type->defaultVal = new FnCall(new Variable(fn), NULL);
    SET_BACK(class_type->defaultVal);
  }
  if (class_type->value) {
    class_type->defaultVal = new FnCall(new Variable(class_type->symbol), NULL);
    SET_BACK(class_type->defaultVal);
  }
  TRAVERSE(class_type->symbol->defPoint->stmt, new Fixup(), true);
}


static void build_union_id_enum(ClassType* class_type) {
  UnionType* unionType = dynamic_cast<UnionType*>(class_type);
  if (unionType) {
    unionType->buildFieldSelector();
  }
}


static void build_record_equality_function(ClassType* class_type) {
  if (!class_type->value) {
    return;
  }

  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("=="));
  ParamSymbol* arg1 = new ParamSymbol(PARAM_BLANK, "_arg1", class_type);
  ParamSymbol* arg2 = new ParamSymbol(PARAM_BLANK, "_arg2", class_type);
  arg1->append(arg2);
  Expr* cond = NULL;
  forv_Vec(VarSymbol, tmp, class_type->fields) {
    Expr* left = new MemberAccess(new Variable(arg1), tmp);
    Expr* right = new MemberAccess(new Variable(arg2), tmp);
    cond = (cond)
      ? new BinOp(BINOP_LOGAND, cond, new BinOp(BINOP_EQUAL, left, right))
      : new BinOp(BINOP_EQUAL, left, right);
  }
  Stmt* body = new ReturnStmt(cond);
  DefStmt* def_stmt = new DefStmt(Symboltable::finishFnDef(fn, arg1, dtBoolean, body));
  class_type->symbol->defPoint->stmt->insertBefore(def_stmt);
}


static void build_record_inequality_function(ClassType* class_type) {
  if (!class_type->value) {
    return;
  }

  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("!="));

  ParamSymbol* arg1 = new ParamSymbol(PARAM_BLANK, "_arg1", class_type);
  ParamSymbol* arg2 = new ParamSymbol(PARAM_BLANK, "_arg2", class_type);
  arg1->append(arg2);
  Expr* cond = NULL;
  forv_Vec(VarSymbol, tmp, class_type->fields) {
    Expr* left = new MemberAccess(new Variable(arg1), tmp);
    Expr* right = new MemberAccess(new Variable(arg2), tmp);
    cond = (cond)
      ? new BinOp(BINOP_LOGOR, cond, new BinOp(BINOP_EQUAL, left, right))
      : new BinOp(BINOP_NEQUAL, left, right);
  }
  Stmt* body = new ReturnStmt(cond);
  DefStmt* def_stmt = new DefStmt(Symboltable::finishFnDef(fn, arg1, dtBoolean, body));
  class_type->symbol->defPoint->stmt->insertBefore(def_stmt);
}


static void build_record_assignment_function(ClassType* class_type) {
  if (!class_type->value) {
    return;
  }

  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("="));

  ParamSymbol* arg1 = new ParamSymbol(PARAM_BLANK, "_arg1", class_type);
  ParamSymbol* arg2 = new ParamSymbol(PARAM_BLANK, "_arg2", class_type);
  arg1->append(arg2);
  Stmt* body = NULL;
  forv_Vec(VarSymbol, tmp, class_type->fields) {
    Expr* left = new MemberAccess(new Variable(arg1), tmp);
    Expr* right = new MemberAccess(new Variable(arg2), tmp);
    Expr* assign_expr = new AssignOp(GETS_NORM, left, right);
    body = appendLink(body, new ExprStmt(assign_expr));
  }
  Stmt* block_stmt = new BlockStmt(body);
  DefStmt* def_stmt = new DefStmt(Symboltable::finishFnDef(fn, arg1, dtVoid, block_stmt));
  class_type->symbol->defPoint->stmt->insertBefore(def_stmt);
}


static void build_tuple_assignment_function(TupleType* tuple_type) {
  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol("="));

  ParamSymbol* arg1 = new ParamSymbol(PARAM_BLANK, "_arg1", tuple_type);
  ParamSymbol* arg2 = new ParamSymbol(PARAM_BLANK, "_arg2", tuple_type);
  arg1->append(arg2);
  Stmt* body = NULL;
  for (int i = 1; i <= tuple_type->components.n; i++) {
    Expr* left =
      new TupleSelect(new Variable(arg1), new IntLiteral(intstring(i), i));
    Expr* right =
      new TupleSelect(new Variable(arg2), new IntLiteral(intstring(i), i));
    Expr* assign_expr = new AssignOp(GETS_NORM, left, right);
    body = appendLink(body, new ExprStmt(assign_expr));
  }
  BlockStmt* block_stmt = new BlockStmt(body);
  DefStmt* def_stmt = new DefStmt(Symboltable::finishFnDef(fn, arg1, dtVoid, block_stmt));
  tuple_type->symbol->defPoint->stmt->insertBefore(def_stmt);
}


void BuildClassConstructorsEtc::postProcessExpr(Expr* expr) {
  DefExpr* def_expr = dynamic_cast<DefExpr*>(expr);

  if (!def_expr) {
    return;
  }

  if (TypeSymbol* type_symbol = dynamic_cast<TypeSymbol*>(def_expr->sym)) {
    if (ClassType* class_type = dynamic_cast<ClassType*>(type_symbol->type)) {
      build_union_id_enum(class_type);
      build_constructor(class_type);
      build_record_equality_function(class_type);
      build_record_inequality_function(class_type);
      build_record_assignment_function(class_type);
    } else if (TupleType* tuple_type = dynamic_cast<TupleType*>(type_symbol->type)) {
      build_tuple_assignment_function(tuple_type);
    }
  }
}
