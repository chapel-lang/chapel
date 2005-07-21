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
    return new NoOpStmt();
  } else if (type->defaultVal) {
    Expr* lhs = new Variable(var);
    Expr* rhs = type->defaultVal->copy();
    Expr* init_expr = new AssignOp(GETS_NORM, lhs, rhs);
    return new ExprStmt(init_expr);
  } else if (type->defaultConstructor) {
    Expr* lhs = new Variable(var);
    Expr* constructor_variable = new Variable(type->defaultConstructor);
    Expr* rhs = new FnCall(constructor_variable);
    Expr* init_expr = new AssignOp(GETS_NORM, lhs, rhs);
    return new ExprStmt(init_expr);
  } else {
    INT_FATAL(var, "Failed to insert default initialization");
    return new NoOpStmt();
  }
}


static void insert_config_init(Stmt* stmt, VarSymbol* var, Type* type) {

  // SJD: Note I want this to be a single macro INIT_CONFIG with the
  // default expression at the end and the if statement tucked away,
  // but because complex literals can't be passed, can't do this.
  // Need a traversal to change complex literals into complex variable
  // temporaries for function calls.

  Expr* init_expr = var->defPoint->init ? var->defPoint->init->expr : type->defaultVal;
  AList<Expr>* args = new AList<Expr>(new Variable(var));
  args->insertAtTail(new Variable(type->symbol));
  args->insertAtTail(new StringLiteral(copystring(var->name)));
  args->insertAtTail(new StringLiteral(var->parentScope->symContext->name));
  Symbol* init_config = Symboltable::lookupInternal("_INIT_CONFIG");
  FnCall* call = new FnCall(new Variable(init_config), args);
  Expr* assign = new AssignOp(GETS_NORM, new Variable(var), init_expr->copy());
  ExprStmt* assign_stmt = new ExprStmt(assign);
  CondStmt* cond_stmt = new CondStmt(call, new BlockStmt(new AList<Stmt>(assign_stmt)));
  stmt->insertBefore(cond_stmt);
}


static void insert_basic_init(Stmt* stmt, VarSymbol* var, Type* type) {
  if (!is_Scalar_Type(type) || !var->defPoint->init) {
    if (!var->defPoint->exprType) { // SJD: UGH
      stmt->insertBefore(basic_default_init_stmt(stmt, var, type));
    }
  }

  if (var->defPoint->exprType) {
    Expr* lhs = new Variable(var);
    Expr* rhs = var->defPoint->exprType->copy();
    Expr* init_expr = new AssignOp(GETS_NORM, lhs, rhs);
    stmt->insertBefore(new ExprStmt(init_expr));
  }

  if (var->defPoint->init) {
    Expr* lhs = new Variable(var);
    Expr* rhs = var->defPoint->init->expr->copy();
    Expr* init_expr = new AssignOp(GETS_NORM, lhs, rhs);
    stmt->insertBefore(new ExprStmt(init_expr));
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
