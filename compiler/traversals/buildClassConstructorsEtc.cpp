#include "buildClassConstructorsEtc.h"
#include "symtab.h"
#include "symscope.h"
#include "symbol.h"
#include "type.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "../traversals/fixup.h"

//#define CONSTRUCTOR_WITH_PARAMETERS 1

static void build_constructor(ClassType* class_type) {
  SymScope* saveScope = Symboltable::setCurrentScope(class_type->classScope);

  char* name = glomstrings(2, "_construct_", class_type->symbol->name);
  FnSymbol* fn = Symboltable::startFnDef(new FnSymbol(name));
  fn->cname = glomstrings(2, "_construct_", class_type->symbol->cname);

  ParamSymbol* args = NULL;
  forv_Vec(VarSymbol, tmp, class_type->fields) {
#if CONSTRUCTOR_WITH_PARAMETERS
    args = appendLink(args, new ParamSymbol(PARAM_BLANK, tmp->name, tmp->type, tmp->init ? tmp->init->copy() : tmp->type->defaultVal->copy()));
#else
#endif
  }

  BlockStmt* body = Symboltable::startCompoundStmt();
  Stmt* stmts = NULL;
  VarSymbol* _this = new VarSymbol("this", class_type);
  DefExpr* def_expr = new DefExpr(_this);
  _this->setDefPoint(def_expr);
  stmts = new DefStmt(def_expr);
  if (!(class_type->value || class_type->union_value)) {
    Expr* alloc_args = new IntLiteral("1", 1);
    alloc_args = appendLink(alloc_args, new SizeofExpr(class_type));
    alloc_args = appendLink(alloc_args, new StringLiteral("string"));
    Symbol* alloc_sym = Symboltable::lookupInternal("_chpl_malloc");
    Expr* alloc_call = new FnCall(new Variable(alloc_sym), alloc_args);
    Expr* alloc_lhs = new Variable(_this);
    Expr* alloc_rhs = new CastExpr(class_type, alloc_call);
    Expr* alloc_expr = new AssignOp(GETS_NORM, alloc_lhs, alloc_rhs);
    Stmt* alloc_stmt = new ExprStmt(alloc_expr);
    stmts = appendLink(stmts, alloc_stmt);
  }
#if CONSTRUCTOR_WITH_PARAMETERS
  ParamSymbol* ptmp = args;
#endif
  forv_Vec(VarSymbol, tmp, class_type->fields) {
    Expr* lhs = new MemberAccess(new Variable(_this), tmp);
#if CONSTRUCTOR_WITH_PARAMETERS
    Expr* rhs = new Variable(ptmp);
#else
    Expr* rhs = tmp->init ? tmp->init->copy() : tmp->type->defaultVal->copy();
    // hack for classes that are cloned; we don't actually want to
    // build the constructor for cloned classes until they are cloned
    if (!rhs) continue;
    // end hack
#endif
    Expr* assign_expr = new AssignOp(GETS_NORM, lhs, rhs);
    Stmt* assign_stmt = new ExprStmt(assign_expr);
    stmts = appendLink(stmts, assign_stmt);
#if CONSTRUCTOR_WITH_PARAMETERS
    ptmp = nextLink(ParamSymbol, ptmp);
#endif
  }

  stmts = appendLink(stmts, new ReturnStmt(new Variable(_this)));
  body = Symboltable::finishCompoundStmt(body, stmts);
  Expr* fn_def = Symboltable::finishFnDef(fn, args, class_type, body);
  class_type->constructor = new DefStmt(fn_def);

  SET_BACK(class_type->constructor);
  Symboltable::setCurrentScope(saveScope);
  TRAVERSE(dynamic_cast<Expr*>(class_type->symbol->defPoint)->stmt, new Fixup(), true);
}


static void build_union_id_enum(ClassType* class_type) {
  if (!class_type->union_value) {
    return;
  }

  EnumSymbol* id_list = NULL;
  forv_Vec(VarSymbol, tmp, class_type->fields) {
    char* id_name =
      glomstrings(4, "_", class_type->symbol->name, "_union_id_", tmp->name);
    EnumSymbol* id_symbol = new EnumSymbol(id_name, NULL);
    id_list = appendLink(id_list, id_symbol);
  }
  id_list->set_values();
  EnumType* enum_type = new EnumType(id_list);
  char* enum_name =
    glomstrings(3, "_", class_type->symbol->name, "_union_id");
  TypeSymbol* enum_symbol = new TypeSymbol(enum_name, enum_type);
  enum_type->addSymbol(enum_symbol);
  DefExpr* def_expr = new DefExpr(enum_symbol);
  enum_symbol->setDefPoint(def_expr);
  id_list->setDefPoint(def_expr);
  DefStmt* def_stmt = new DefStmt(def_expr);
  dynamic_cast<DefExpr*>(class_type->symbol->defPoint)->stmt->insertBefore(def_stmt);
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
  dynamic_cast<DefExpr*>(class_type->symbol->defPoint)->stmt->insertBefore(def_stmt);
}


BuildClassConstructorsEtc::BuildClassConstructorsEtc(void) {
  processInternalModules = false;
}


void BuildClassConstructorsEtc::postProcessExpr(Expr* expr) {
  DefExpr* def_expr = dynamic_cast<DefExpr*>(expr);

  if (!def_expr) {
    return;
  }

  if (TypeSymbol* type_symbol = dynamic_cast<TypeSymbol*>(def_expr->sym)) {
    if (ClassType* class_type = dynamic_cast<ClassType*>(type_symbol->type)) {
      build_constructor(class_type);
      build_union_id_enum(class_type);
      build_record_equality_function(class_type);
    }
  }
}
