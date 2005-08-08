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


static ExprStmt* genExit(void) {
  return new ExprStmt(new CallExpr("exit", new IntLiteral(0)));
}


static ExprStmt* genWriteln(void) {
  return new ExprStmt(new CallExpr("writeln"));
}


static ExprStmt* genWrite(Literal* expression) {
  return new ExprStmt(new CallExpr("write", expression));
}


static ExprStmt* genIntWriteExpr(int newInt) {
  IntLiteral* expression = new IntLiteral(newInt);
  return genWrite(expression);
}


static ExprStmt* genStringWriteExpr(char* newString) {
  StringLiteral* expression = new StringLiteral(newString);
  return genWrite(expression);
}


void SpecializeCallExprs::postProcessStmt(Stmt* stmt) {
  if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(exprStmt->expr)) {
      if (Variable* baseVar = dynamic_cast<Variable*>(call->baseExpr)) {
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
          call->parentStmt->insertBefore(genExit());
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
    if (Variable* baseVar = dynamic_cast<Variable*>(paren->baseExpr)) {
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

