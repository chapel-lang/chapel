#include "specializeParenOpExprs.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"


static void
decomposeStmtFunction(ParenOpExpr* parenOpExpr, char* newFunctionName) {
  Stmt* replacements = NULL;
  for (Expr* arg = parenOpExpr->argList; arg; arg = nextLink(Expr, arg)) {
    Variable* function = new Variable(new UnresolvedSymbol(newFunctionName));
    Stmt* replacement = new ExprStmt(new ParenOpExpr(function, arg->copy()));
    replacements = appendLink(replacements, replacement);
  }
  if (replacements) {
    parenOpExpr->parentStmt->insertBefore(replacements);
  }
}


void PreSpecializeParenOpExprs::postProcessStmt(Stmt* stmt) {
  if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
    if (ParenOpExpr* parenOpExpr = dynamic_cast<ParenOpExpr*>(exprStmt->expr)) {
      if (Variable* baseVar = dynamic_cast<Variable*>(parenOpExpr->baseExpr)) {
        if (strcmp(baseVar->var->name, "write") == 0) {
          decomposeStmtFunction(parenOpExpr, "write");
          parenOpExpr->parentStmt->extract();
        } else if (strcmp(baseVar->var->name, "writeln") == 0) {
          decomposeStmtFunction(parenOpExpr, "write");
          Expr* writeln = new Variable(new UnresolvedSymbol("writeln"));
          Expr* callWriteln = new ParenOpExpr(writeln, NULL);
          parenOpExpr->parentStmt->insertBefore(new ExprStmt(callWriteln));
          parenOpExpr->parentStmt->extract();
        }
      }
    }
  }
}


void PreSpecializeParenOpExprs::postProcessExpr(Expr* expr) {
  Expr* paren_replacement = NULL;
  if (ParenOpExpr* paren = dynamic_cast<ParenOpExpr*>(expr)) {
    if (dynamic_cast<ArrayType*>(paren->baseExpr->typeInfo())) {
      if (!analyzeAST)
        paren_replacement = new ArrayRef(paren->baseExpr, paren->argList);
    } else if (dynamic_cast<TupleType*>(paren->baseExpr->typeInfo())) {
      if (!analyzeAST)
        paren_replacement = new TupleSelect(paren->baseExpr, paren->argList);
    } else if (Variable* baseVar = dynamic_cast<Variable*>(paren->baseExpr)) {
      if (!dynamic_cast<TypeSymbol*>(baseVar->var) && 
          dynamic_cast<TupleType*>(baseVar->typeInfo())) {
        if (!analyzeAST)
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
        //        paren_replacement = new IOCall(IO_WRITE, paren->baseExpr, paren->argList);
      } else if (strcmp(baseVar->var->name, "writeln") == 0) {
        //        paren_replacement = new IOCall(IO_WRITELN, paren->baseExpr, paren->argList);
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


void SpecializeParenOpExprs::postProcessExpr(Expr* expr) {
  if (!analyzeAST)
    return;
  Expr* paren_replacement = NULL;
  if (ParenOpExpr* paren = dynamic_cast<ParenOpExpr*>(expr)) {
    if (dynamic_cast<ArrayType*>(paren->baseExpr->typeInfo())) {
      paren_replacement = new ArrayRef(paren->baseExpr, paren->argList);
    } else if (dynamic_cast<TupleType*>(paren->baseExpr->typeInfo())) {
      paren_replacement = new TupleSelect(paren->baseExpr, paren->argList);
    } else if (Variable* baseVar = dynamic_cast<Variable*>(paren->baseExpr)) {
      if (!dynamic_cast<TypeSymbol*>(baseVar->var) && 
          dynamic_cast<TupleType*>(baseVar->typeInfo())) {
        paren_replacement = new TupleSelect(baseVar, paren->argList);
      } else if (dynamic_cast<StructuralType*>(baseVar->var->type)) {
      } else if (strcmp(baseVar->var->name, "write") == 0) {
      } else if (strcmp(baseVar->var->name, "writeln") == 0) {
      } else if (strcmp(baseVar->var->name, "read") == 0) {
      } else if (dynamic_cast<FnSymbol*>(baseVar->var) && !dynamic_cast<FnCall*>(expr)) {
        paren_replacement = new FnCall(baseVar, paren->argList);
      }
    }
  }
  if (paren_replacement) {
    Vec<FnSymbol *> fns;
    call_info(expr, fns);
    if (fns.n > 1)
      INT_FATAL(expr, "unable to resolve call");
    if (fns.n == 1)
      paren_replacement->resolved = fns.v[0];
    expr->replace(paren_replacement);
  }
}
