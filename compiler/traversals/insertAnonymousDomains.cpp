#include "insertAnonymousDomains.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "symtab.h"
#include "stringutil.h"


void InsertAnonymousDomains::preProcessStmt(Stmt* stmt) {
  currentStmt = stmt;
  if (TypeDefStmt* type_def = dynamic_cast<TypeDefStmt*>(stmt)) {
    currentScope = type_def->type_sym->parentScope;
  }
  else if (VarDefStmt* var_def = dynamic_cast<VarDefStmt*>(stmt)) {
    currentScope = var_def->var->parentScope;
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

  DomainType* domain_type = new DomainType(1);

  SymScope* saveScope = Symboltable::setCurrentScope(currentScope);
  VarSymbol* domain_sym = new VarSymbol(name, domain_type, forall->copy());
  VarDefStmt* def_stmt = new VarDefStmt(domain_sym);
  domain_sym->setDefPoint(def_stmt);
  array_type->domain->replace(new ForallExpr(new Variable(domain_sym)));
  currentStmt->insertBefore(def_stmt);
  Symboltable::setCurrentScope(saveScope);
}


void InsertAnonymousDomains::postProcessStmt(Stmt* stmt) {
  currentStmt = NULL;
  currentScope = NULL;
}
