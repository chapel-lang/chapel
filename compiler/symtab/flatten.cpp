#include "flatten.h"
#include "symtab.h"
#include "stmt.h"
#include "expr.h"
#include "stringutil.h"


void Flatten::processSymbol(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    if (fn->typeBinding && fn->defPoint->parentSymbol == fn->typeBinding) {
      Stmt* insertPoint = fn->typeBinding->defPoint->parentStmt;
      while (!dynamic_cast<ModuleSymbol*>(insertPoint->parentSymbol)) {
        insertPoint = insertPoint->parentSymbol->defPoint->parentStmt;
      }
      ModuleSymbol* moduleSymbol = dynamic_cast<ModuleSymbol*>(insertPoint->parentSymbol);
      SymScope* saveScope =
        Symboltable::setCurrentScope(moduleSymbol->modScope);
      ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(fn->defPoint->parentStmt->copy(true));
      DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr);
      FnSymbol* newFn = dynamic_cast<FnSymbol*>(defExpr->sym);
      newFn->cname = copystring(fn->cname);
      insertPoint->insertBefore(exprStmt);
      fn->defPoint->parentStmt->remove();
      fn->parentScope->remove(fn);
      Symboltable::removeScope(fn->paramScope);
      StructuralType* structuralType = dynamic_cast<StructuralType*>(fn->typeBinding->type);
      for (int i = 0; i < structuralType->methods.n; i++) {
        if (structuralType->methods.v[i] == fn) {
          structuralType->methods.v[i] = newFn;
        }
      }
      Symboltable::setCurrentScope(saveScope);
    }
  }
}
