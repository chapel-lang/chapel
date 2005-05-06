#include "applyThisParameters.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

ApplyThisParameters::ApplyThisParameters(void) {
  currentFunction = NULL;
}

void ApplyThisParameters::preProcessStmt(Stmt* stmt) {
  if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt)) {
    if (FnSymbol* fn = def_stmt->fnDef()) {
      currentFunction = fn;
    }
  }
}

void ApplyThisParameters::postProcessStmt(Stmt* stmt) {
  if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt)) {
    if (def_stmt->fnDef()) {
      currentFunction = NULL;
    }
  }
}

void ApplyThisParameters::preProcessExpr(Expr* expr) {
  if (Variable* member = dynamic_cast<Variable*>(expr)) {
    if (currentFunction && currentFunction->classBinding) {

      if (member->var->parentScope && member->var->parentScope->type == SCOPE_CLASS) {
        // Steve, this isn't the best test?  Is there a better way to
        // know if we need a this applied here.  Can we do this in
        // ScopeResolveSymbols??

//       StructuralType* structuralType =
//         dynamic_cast<StructuralType*>(currentFunction->classBinding->type);

//       if (Symbol* symbol = Symboltable::lookupInScope(member->var->name, 
//                                                       structuralType->structScope)) {

        if (FnSymbol* constructor = dynamic_cast<FnSymbol*>(member->var)) {
          if (constructor->isConstructor) {
            return;
          }
        }

        if (member->var == currentFunction->_this) {
          return;
        }

        Variable* base = new Variable(currentFunction->_this);
        // propagate field's source location to the parent
        base->filename = member->filename;
        base->lineno = member->lineno;

        MemberAccess* memberAccess = new MemberAccess(base, member->var);
        memberAccess->lineno = expr->lineno;
        memberAccess->filename = expr->filename;
        expr->replace(memberAccess);
      }
    }
  }
}
