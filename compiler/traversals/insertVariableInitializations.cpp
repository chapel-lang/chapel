#include <typeinfo>
#include "insertVariableInitializations.h"
#include "symtab.h"
#include "symscope.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "stringutil.h"


static Stmt* basic_default_init_stmt(Stmt* stmt, VarSymbol* var, Type* type) {
  if (var->noDefaultInit && var->type != dtString) {
    return NULL;
  } else if (type->defaultValue) {
    Expr* lhs = new SymExpr(var);
    Expr* rhs = new SymExpr(type->defaultValue);
    Expr* init_expr = new CallExpr(OP_GETSNORM, lhs, rhs);
    return new ExprStmt(init_expr);
  } else if (type->defaultConstructor) {
    Expr* lhs = new SymExpr(var);
    Expr* constructor_variable = new SymExpr(type->defaultConstructor);
    Expr* rhs = new CallExpr(constructor_variable);
    Expr* init_expr = new CallExpr(OP_GETSNORM, lhs, rhs);
    return new ExprStmt(init_expr);
  } else {
    INT_FATAL(var, "Failed to insert default initialization");
    return NULL;
  }
}


static void insert_config_init(Stmt* stmt, VarSymbol* var, Type* type) {

  // SJD: Note I want this to be a single macro INIT_CONFIG with the
  // default expression at the end and the if statement tucked away,
  // but because complex literals can't be passed, can't do this.
  // Need a traversal to change complex literals into complex variable
  // temporaries for function calls.

  Expr* init_expr = var->defPoint->init ? var->defPoint->init : new SymExpr(type->defaultValue);
  AList<Expr>* args = new AList<Expr>(new SymExpr(var));
  args->insertAtTail(new SymExpr(type->symbol));
  args->insertAtTail(new_StringLiteral(copystring(var->name)));
  args->insertAtTail(new_StringLiteral(dynamic_cast<Symbol*>(var->parentScope->astParent)->name));
  CallExpr* call = new CallExpr(initConfigFn, args);
  Expr* assign = new CallExpr(OP_GETSNORM, var, init_expr->copy());
  ExprStmt* assign_stmt = new ExprStmt(assign);
  CondStmt* cond_stmt = new CondStmt(call, assign_stmt);
  stmt->insertBefore(cond_stmt);
}


static void insert_basic_init(Stmt* stmt, VarSymbol* var, Type* type) {
  if (!is_Scalar_Type(type) || !var->defPoint->init) {
    if (!var->defPoint->exprType) { // SJD: UGH
      if (Stmt* init_stmt = basic_default_init_stmt(stmt, var, type)) {
        stmt->insertBefore(init_stmt);
      }
    }
  }

  if (var->defPoint->exprType) {
    Expr* lhs = new SymExpr(var);
    Expr* rhs = var->defPoint->exprType->copy();
    Expr* init_expr = new CallExpr(OP_GETSNORM, lhs, rhs);
    stmt->insertBefore(new ExprStmt(init_expr));
  }

  if (var->defPoint->init) {
    Expr* lhs = new SymExpr(var);
    Expr* rhs = var->defPoint->init->copy();
    if (var->defPoint->initAssign.n) {
      if (var->defPoint->initAssign.n != 1) {
        INT_FATAL(var->defPoint, "Unable to resolve initialization assignment");
      }
#ifdef OVERLOADED_ASSIGNMENT_FIXED
      Expr *init_expr = new CallExpr(var->defPoint->initAssign.v[0],
                                     lhs, rhs);
#else
      Expr *init_expr = new CallExpr(OP_GETSNORM, lhs, rhs);
#endif
      stmt->insertBefore(new ExprStmt(init_expr));
    } else {
      Expr* init_expr = new CallExpr(OP_GETSNORM, lhs, rhs);
      stmt->insertBefore(new ExprStmt(init_expr));
    }
  }
}


static void insert_init(Stmt* stmt, VarSymbol* var, Type* type) {
  if (var->varClass == VAR_CONFIG) {
    insert_config_init(stmt, var, type);
  } else {
    insert_basic_init(stmt, var, type);
  }
}


InsertVariableInitializations::InsertVariableInitializations() {
  whichModules = MODULES_CODEGEN;
}


void InsertVariableInitializations::postProcessStmt(Stmt* stmt) {
  if (ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt)) {
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr_stmt->expr)) {
      if (dynamic_cast<TypeSymbol*>(expr_stmt->parentSymbol)) {
        return;
      }
      if (VarSymbol* var = dynamic_cast<VarSymbol*>(defExpr->sym)) {
        insert_init(stmt, var, var->type);
      }
    }
  }
}
