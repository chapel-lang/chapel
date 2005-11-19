#include "specializeCallExprs.h"
#include "expr.h"
#include "../passes/filesToAST.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"



static void
decomposeIOCall(CallExpr* call, char* newFunctionName) {
  for_alist(Expr, arg, call->argList) {
    call->parentStmt->insertBefore
      (new ExprStmt(new CallExpr(newFunctionName, arg->copy())));
  }
}


static void
decomposeFileIOCall(CallExpr* call, char* newFunctionName, Expr* outfile) {
  for_alist(Expr, arg, call->argList) {
    call->parentStmt->insertBefore
      (new ExprStmt(new CallExpr(newFunctionName, outfile->copy(), 
                                 arg->copy())));
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


static ExprStmt* genFwriteln(Expr* outfile) {
  return new ExprStmt(new CallExpr("fwriteln", outfile->copy()));
}


static ExprStmt* genFwrite(Expr* expression) {
  Symbol* chplStdout = Symboltable::lookupInFileModuleScope("stdout");
  SymExpr* outfile = new SymExpr(chplStdout);
  return new ExprStmt(new CallExpr("fwrite", outfile, expression->copy()));
}


static ExprStmt* genIntWriteExpr(int newInt) {
  Expr* expression = new_IntLiteral(newInt);
  return genFwrite(expression);
}


static ExprStmt* genStringWriteExpr(char* newString) {
  Expr* expression = new_StringLiteral(newString);
  return genFwrite(expression);
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
          Symbol* chplStdout = Symboltable::lookupInFileModuleScope("stdout");
          SymExpr* outfile = new SymExpr(chplStdout);
          thenBody->insertAtTail(genFwriteln(outfile));
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
        } else if (strcmp(baseVar->var->name, "fread") == 0) {
          Expr* outfile = call->argList->popHead();
          decomposeFileIOCall(call, "fread", outfile);
          call->parentStmt->remove();
        } else if (strcmp(baseVar->var->name, "fwrite") == 0) {
          Expr* outfile = call->argList->popHead();
          decomposeFileIOCall(call, "fwrite", outfile);
          call->parentStmt->remove();
        } else if (strcmp(baseVar->var->name, "fwriteln") == 0) {
          Expr* outfile = call->argList->popHead();
          decomposeFileIOCall(call, "fwrite", outfile);
          call->parentStmt->insertBefore(genFwriteln(outfile));
          call->parentStmt->remove();
        } else if (strcmp(baseVar->var->name, "halt") == 0) {
          Symbol* chplStdout = Symboltable::lookupInFileModuleScope("stdout");
          SymExpr* outfile = new SymExpr(chplStdout);
          decomposeFileIOCall(call, "fwrite", outfile);
          call->parentStmt->insertBefore(genFwriteln(outfile));
          call->parentStmt->insertBefore(genExit(call->parentFunction()));
          call->parentStmt->remove();
        } else if (strcmp(baseVar->var->name, "read") == 0) {
          decomposeIOCall(call, "read");
          call->parentStmt->remove();
        } else if (strcmp(baseVar->var->name, "write") == 0) {
          Symbol* chplStdout = Symboltable::lookupInFileModuleScope("stdout");
          SymExpr* outfile = new SymExpr(chplStdout);
          decomposeFileIOCall(call, "fwrite", outfile);
          call->parentStmt->remove();
        } else if (strcmp(baseVar->var->name, "writeln") == 0) {
          Symbol* chplStdout = Symboltable::lookupInFileModuleScope("stdout");
          SymExpr* outfile = new SymExpr(chplStdout);
          decomposeFileIOCall(call, "fwrite", outfile);
          call->parentStmt->insertBefore(genFwriteln(outfile));
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


void specializeCallExprs(void) {
  Pass* pass = new SpecializeCallExprs();
  pass->run(Symboltable::getModules(pass->whichModules));
}
