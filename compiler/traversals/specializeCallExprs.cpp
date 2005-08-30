#include "specializeCallExprs.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"


static void
decomposeStmtFunction(CallExpr* call, char* newFunctionName) {
  for_alist(Expr, arg, call->argList) {
    call->parentStmt->insertBefore
      (new ExprStmt(new CallExpr(newFunctionName, arg->copy())));
  }
}


static Stmt* genExit(FnSymbol *fnSym) {
  BlockStmt* body = fnSym->body;
  SymScope* fnScope = body->blkScope;
  SymScope* prevScope = Symboltable::setCurrentScope(fnScope);
  ExprStmt *c = new ExprStmt(new CallExpr("exit", new_IntLiteral(0)));
  Symbol* retvalSym = Symboltable::lookupInCurrentScope("_retval");
  if (retvalSym) {
    BlockStmt *b = new BlockStmt(c);
    SymExpr* newRetExpr = new SymExpr(retvalSym);
    ReturnStmt *newRetStmt = new ReturnStmt(newRetExpr);
    c->insertAfter(newRetStmt);
    Symboltable::setCurrentScope(prevScope);
    return b;
  }
  Symboltable::setCurrentScope(prevScope);
  return c;
}


static ExprStmt* genWriteln(void) {
  return new ExprStmt(new CallExpr("writeln"));
}


static ExprStmt* genWrite(Expr* expression) {
  return new ExprStmt(new CallExpr("write", expression));
}


static ExprStmt* genIntWriteExpr(int newInt) {
  Expr* expression = new_IntLiteral(newInt);
  return genWrite(expression);
}


static ExprStmt* genStringWriteExpr(char* newString) {
  Expr* expression = new_StringLiteral(newString);
  return genWrite(expression);
}


void SpecializeCallExprs::postProcessStmt(Stmt* stmt) {
  if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(exprStmt->expr)) {
      if (SymExpr* baseVar = dynamic_cast<SymExpr*>(call->baseExpr)) {
        if (strcmp(baseVar->var->name, "assert") == 0) {
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
          thenBody->insertAtTail(genExit(call->parentFunction()));
          BlockStmt* thenStmt = new BlockStmt(thenBody);
          int length = call->argList->length();
          if (length != 1) {
            USR_FATAL(call->argList, "Assert takes exactly one "
                      "expression; you've given it %d.", length);
          }
          Expr* test = call->argList->only();
          Expr* notTest = new CallExpr(OP_LOGNOT, test->copy());
          Stmt* assertIfStmt = new CondStmt(notTest, thenStmt);
          call->parentStmt->insertBefore(assertIfStmt);
          call->parentStmt->remove();          

        } else if (strcmp(baseVar->var->name, "halt") == 0) {
          decomposeStmtFunction(call, "write");
          call->parentStmt->insertBefore(genWriteln());
          call->parentStmt->insertBefore(genExit(call->parentFunction()));
          call->parentStmt->remove();
        } else if (strcmp(baseVar->var->name, "read") == 0) {
          decomposeStmtFunction(call, "read");
          call->parentStmt->remove();
        } else if (strcmp(baseVar->var->name, "write") == 0) {
          decomposeStmtFunction(call, "write");
          call->parentStmt->remove();
        } else if (strcmp(baseVar->var->name, "writeln") == 0) {
          decomposeStmtFunction(call, "write");
          call->parentStmt->insertBefore(genWriteln());
          call->parentStmt->remove();
        }
      }
    }
  }
}


void SpecializeCallExprs::postProcessExpr(Expr* expr) {
  if (CallExpr* paren = dynamic_cast<CallExpr*>(expr)) {
    if (SymExpr* baseVar = dynamic_cast<SymExpr*>(paren->baseExpr)) {
      if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(baseVar->var)) {
        if (ClassType* ctype = dynamic_cast<ClassType*>(ts->definition)) {
          if (ctype->defaultConstructor) {
            expr->replace(
              new CallExpr(ctype->defaultConstructor->name, paren->argList));
          } else {
            INT_FATAL(expr, "structural type has no default constructor");
          }
        }
      }
    }
  }
}

