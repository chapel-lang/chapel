#include "specializeParenOpExprs.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"


void SpecializeParenOpExprs::postProcessExpr(Expr* expr) {
  Expr* paren_replacement = NULL;
  if (ParenOpExpr* paren = dynamic_cast<ParenOpExpr*>(expr)) {
    if (dynamic_cast<ArrayType*>(paren->baseExpr->typeInfo())) {
      paren_replacement = new ArrayRef(paren->baseExpr, paren->argList);
    }
    else if (Variable* baseVar = dynamic_cast<Variable*>(paren->baseExpr)) {
      if (!dynamic_cast<TypeSymbol*>(baseVar->var) && 
          dynamic_cast<TupleType*>(baseVar->typeInfo())) {
        paren_replacement = new TupleSelect(baseVar, paren->argList);
      } else if (StructuralType* ctype = dynamic_cast<StructuralType*>(baseVar->var->type)) {
        if (!dynamic_cast<TypeSymbol*>(baseVar->var)) {
          USR_FATAL(expr, "Invalid class constructor");
        }
        if (ctype->defaultConstructor) {
          paren_replacement = new ParenOpExpr(new Variable(new UnresolvedSymbol(ctype->defaultConstructor->name)), paren->argList);
        }
        else {
          INT_FATAL(expr, "constructor does not have a DefStmt");
        }
      } else if (strcmp(baseVar->var->name, "write") == 0) {
        paren_replacement = new IOCall(IO_WRITE, paren->baseExpr, paren->argList);
      } else if (strcmp(baseVar->var->name, "writeln") == 0) {
        paren_replacement = new IOCall(IO_WRITELN, paren->baseExpr, paren->argList);
      } else if (strcmp(baseVar->var->name, "read") == 0) {
        paren_replacement = new IOCall(IO_READ, paren->baseExpr, paren->argList);
      } else if (dynamic_cast<FnSymbol*>(baseVar->var)) {
        paren_replacement = new FnCall(baseVar, paren->argList);
      }
    }
  }
  if (paren_replacement) {
    expr->replace(paren_replacement);
  }
}
