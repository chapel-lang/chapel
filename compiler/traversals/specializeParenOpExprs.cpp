#include "specializeParenOpExprs.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"


static void
decomposeStmtFunction(ParenOpExpr* parenOpExpr, char* newFunctionName) {
  for (Expr* arg = parenOpExpr->argList->first(); arg; arg = parenOpExpr->argList->next()) {
    Variable* function = new Variable(new UnresolvedSymbol(newFunctionName));
    Stmt* replacement = new ExprStmt(new ParenOpExpr(function, new AList<Expr>(arg->copy())));
    parenOpExpr->parentStmt->insertBefore(replacement);
  }
}


static ExprStmt* genExit(void) {
  Expr* rtexit = new Variable(new UnresolvedSymbol("exit"));
  IntLiteral* exitZero = new IntLiteral(0);
  AList<Expr>* status = new AList<Expr>(exitZero);
  Expr* callRtexit = new ParenOpExpr(rtexit, status);
  return new ExprStmt(callRtexit);
}


static ExprStmt* genWriteln(void) {
  Expr* writeln = new Variable(new UnresolvedSymbol("writeln"));
  Expr* callWriteln = new ParenOpExpr(writeln);
  return new ExprStmt(callWriteln);
}


static ExprStmt* genWrite(Literal* expression) {
  Expr* write = new Variable(new UnresolvedSymbol("write"));
  AList<Expr>* msg = new AList<Expr>(expression);
  Expr* writeMsg = new ParenOpExpr(write, msg);
  return new ExprStmt(writeMsg);
}


static ExprStmt* genIntWriteExpr(int newInt) {
  IntLiteral* expression = new IntLiteral(newInt);
  return genWrite(expression);
}


static ExprStmt* genStringWriteExpr(char* newString) {
  StringLiteral* expression = new StringLiteral(newString);
  return genWrite(expression);
}


void SpecializeParenOpExprs::postProcessStmt(Stmt* stmt) {
  if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
    if (ParenOpExpr* parenOpExpr = dynamic_cast<ParenOpExpr*>(exprStmt->expr)) {
      if (Variable* baseVar = dynamic_cast<Variable*>(parenOpExpr->baseExpr)) {
        if (strcmp(baseVar->var->name, "assert") == 0) {
          BlockStmt* thenStmt = Symboltable::startCompoundStmt();

          ExprStmt* writeAssert = genStringWriteExpr("***Error:  Assertion at ");
          AList<Stmt>* thenBody = new AList<Stmt>(writeAssert);
          ExprStmt* writeFilename = genStringWriteExpr(stmt->filename);
          thenBody->insertAtTail(writeFilename);
          ExprStmt* writeColon = genStringWriteExpr(":");
          thenBody->insertAtTail(writeColon);
          ExprStmt* writeLinenumber = genIntWriteExpr(stmt->lineno);
          thenBody->insertAtTail(writeLinenumber);
          ExprStmt* writeFailed = genStringWriteExpr(" failed***");
          thenBody->insertAtTail(writeFailed);
          thenBody->insertAtTail(genWriteln());
          thenBody->insertAtTail(genExit());

          thenStmt = Symboltable::finishCompoundStmt(thenStmt, thenBody);
          int length = parenOpExpr->argList->length();
          if (length != 1) {
            USR_FATAL(parenOpExpr->argList, "Assert takes exactly one "
                      "expression; you've given it %d.", length);
          }
          Expr* test = parenOpExpr->argList->only();
          Expr* notTest = new UnOp(UNOP_LOGNOT, test->copy());
          Stmt* assertIfStmt = new CondStmt(notTest, thenStmt);
          parenOpExpr->parentStmt->insertBefore(assertIfStmt);
          parenOpExpr->parentStmt->remove();          

        } else if (strcmp(baseVar->var->name, "halt") == 0) {
          decomposeStmtFunction(parenOpExpr, "write");
          parenOpExpr->parentStmt->insertBefore(genWriteln());
          parenOpExpr->parentStmt->insertBefore(genExit());
          parenOpExpr->parentStmt->remove();
        } else if (strcmp(baseVar->var->name, "read") == 0) {
          decomposeStmtFunction(parenOpExpr, "read");
          parenOpExpr->parentStmt->remove();
        } else if (strcmp(baseVar->var->name, "write") == 0) {
          decomposeStmtFunction(parenOpExpr, "write");
          parenOpExpr->parentStmt->remove();
        } else if (strcmp(baseVar->var->name, "writeln") == 0) {
          decomposeStmtFunction(parenOpExpr, "write");
          parenOpExpr->parentStmt->insertBefore(genWriteln());
          parenOpExpr->parentStmt->remove();
        }
      }
    }
  }
}


void SpecializeParenOpExprs::postProcessExpr(Expr* expr) {
  Expr* paren_replacement = NULL;
  if (ParenOpExpr* paren = dynamic_cast<ParenOpExpr*>(expr)) {
    if (Variable* baseVar = dynamic_cast<Variable*>(paren->baseExpr)) {
      if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(baseVar->var)) {
        if (StructuralType* ctype = dynamic_cast<StructuralType*>(ts->definition)) {
          if (ctype->defaultConstructor) {
            paren_replacement = new ParenOpExpr(new Variable(new UnresolvedSymbol(ctype->defaultConstructor->name)), paren->argList);
          } else {
            INT_FATAL(expr, "structural type has no default constructor");
          }
        }
      } else if (dynamic_cast<FnSymbol*>(baseVar->var)) {
        paren_replacement = new FnCall(baseVar, paren->argList);
      }
    }
  }
  if (paren_replacement) {
    expr->replace(paren_replacement);
  }
}

