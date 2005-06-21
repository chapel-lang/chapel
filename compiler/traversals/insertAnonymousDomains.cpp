#include "insertAnonymousDomains.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"
#include "view.h"


static void insertDomainTemp(ForallExpr* forall,
                             Stmt* currentStmt,
                             ArrayType* array_type) {
  static int uid = 1;

  char* name = glomstrings(2, "_anon_domain_", intstring(uid++));

  DomainType* domain_type = new DomainType(forall->domains->length());

  SymScope* saveScope = Symboltable::setCurrentScope(currentStmt->parentScope);
  VarSymbol* domain_sym = new VarSymbol(name, domain_type);
  DefExpr* def_expr = new DefExpr(domain_sym, 
                                  new UserInitExpr(forall->copy()));
  DefStmt* def_stmt = new DefStmt(def_expr);
  currentStmt->insertBefore(def_stmt);

  ForallExpr* new_forall =
    new ForallExpr(new AList<Expr>(new Variable(domain_sym)), 
                   forall->indices->copy());

  // weird scoping issues
  array_type->domain->parentScope = currentStmt->parentScope;

  array_type->domain->replace(new_forall);
  Symboltable::setCurrentScope(saveScope);
}


void InsertAnonymousDomains::preProcessStmt(Stmt* stmt) {
  currentStmt = stmt;
}


void InsertAnonymousDomains::postProcessType(Type* type) {
  if (!currentStmt) {
    return;
  }

  ArrayType* array_type = dynamic_cast<ArrayType*>(type);

  if (!array_type) {
    return;
  }

  ForallExpr* forall = dynamic_cast<ForallExpr*>(array_type->domain);

  if (!forall || dynamic_cast<Variable*>(forall->domains->first())) {
    return;
  }

  insertDomainTemp(forall, currentStmt, array_type);
}


void InsertAnonymousDomains::postProcessStmt(Stmt* stmt) {
  currentStmt = NULL;
}
