#include "applyThisParameters.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"

ApplyThisParameters::ApplyThisParameters(void) {
  CurrentStruct = NULL;
}

void ApplyThisParameters::preProcessStmt(Stmt* stmt) {
  if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt)) {
    if (FnSymbol* fn = def_stmt->fnDef()) {
      if (fn->classBinding) {
        CurrentStruct = dynamic_cast<StructuralType*>(fn->classBinding->type);
      }
    }
  }
}

void ApplyThisParameters::postProcessStmt(Stmt* stmt) {
  if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt)) {
    if (FnSymbol* fn = def_stmt->fnDef()) {
      if (fn->classBinding) {
        CurrentStruct = NULL;
      }
    }
  }
}

void ApplyThisParameters::preProcessExpr(Expr* expr) {
  if (CurrentStruct) {
    if (Variable* member = dynamic_cast<Variable*>(expr)) {
      if (Symboltable::lookupInScope(member->var->name, 
                                     CurrentStruct->structScope)) {

        /* replacement of expr variable by memberaccess */
        if (FnSymbol* parentFn =
            dynamic_cast<FnSymbol*>(member->stmt->parentSymbol)) {
          if (member->var == parentFn->_this) {
            return;
          }
          Variable* base = new Variable(parentFn->_this);
          // propagate field's source location to the parent
          base->filename = member->filename;
          base->lineno = member->lineno;

          MemberAccess* repl = new MemberAccess(base, member->var);
          repl->lineno = expr->lineno;
          repl->filename = expr->filename;
          expr->replace(repl);
        }
      }
    }
  }
}
