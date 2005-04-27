#include "insertAnonymousDomains.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"
#include "view.h"

void InsertAnonymousDomains::preProcessStmt(Stmt* stmt) {
  currentStmt = stmt;
  if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt)) {
    if (Symbol* sym = def_stmt->typeDef()) {
      currentScope = sym->parentScope;
    }
    else if (Symbol* sym = def_stmt->varDef()) {
      currentScope = sym->parentScope;
    } else {
      currentScope = NULL;
    }
  }
  else {
    currentScope = NULL;
  }
}


void InsertAnonymousDomains::preProcessType(Type* type) {
  static int uid = 1;

  if (!currentScope || !currentStmt) {
    return;
  }

  ArrayType* array_type = dynamic_cast<ArrayType*>(type);

  if (!array_type) {
    return;
  }

  ForallExpr* forall = dynamic_cast<ForallExpr*>(array_type->domain);

  if (!forall || dynamic_cast<Variable*>(forall->domains)) {
    return;
  }

  char* name = glomstrings(2, "_anon_domain_", intstring(uid++));

  DomainType* domain_type = new DomainType(forall->domains->length());

  SymScope* saveScope = Symboltable::setCurrentScope(currentScope);
  VarSymbol* domain_sym = new VarSymbol(name, domain_type);
  DefExpr* def_expr = new DefExpr(domain_sym, 
                                  new UserInitExpr(forall->copy()));
  DefStmt* def_stmt = new DefStmt(def_expr);
  ForallExpr* new_forall = new ForallExpr(new Variable(domain_sym), 
                                          forall->indices);
  array_type->domain->replace(new_forall);
  currentStmt->insertBefore(def_stmt);
  Symboltable::setCurrentScope(saveScope);
}


void InsertAnonymousDomains::postProcessStmt(Stmt* stmt) {
  currentStmt = NULL;
  currentScope = NULL;
}
