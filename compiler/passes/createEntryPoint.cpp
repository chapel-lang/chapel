#include "createEntryPoint.h"
#include "expr.h"
#include "filesToAST.h"
#include "runAnalysis.h"
#include "stmt.h"
#include "symtab.h"


static ExprStmt* buildCallExprStmt(FnSymbol* fn) {
  SymExpr* variable = new SymExpr(fn);
  variable->lineno = -1;
  CallExpr* fnCall = new CallExpr(variable);
  fnCall->lineno = -1;
  ExprStmt* exprStmt = new ExprStmt(fnCall);
  exprStmt->lineno = -1;
  return exprStmt;
}


static ModuleSymbol* findUniqueUserModule(Vec<ModuleSymbol*>* modules) {
  ModuleSymbol* userModule = NULL;

  forv_Vec(ModuleSymbol, mod, *modules) {
    if (mod->modtype == MOD_USER) {
      if (userModule == NULL) {
        userModule = mod;
      } else {
        return NULL;  // two user modules defined
      }
    }
  }
  return userModule;
}


void CreateEntryPoint::run(Vec<ModuleSymbol*>* modules) {
  currentLineno = -1;

  forv_Vec(ModuleSymbol, mod, *modules) {
    if (mod->modtype == MOD_INTERNAL || 
        !ModuleDefContainsOnlyNestedModules(mod->stmts)) {
      mod->createInitFn();
    }
  }

  // find main function if it exists; create one if not
  FnSymbol* mainFn = FnSymbol::mainFn;
  BlockStmt* mainBody;
  ModuleSymbol* mainModule;
  if (!mainFn) {
    mainModule = findUniqueUserModule(modules);
    if (mainModule) {
      mainBody = new BlockStmt();
      DefExpr* maindef = Symboltable::defineFunction("main", NULL, 
                                                     dtVoid, mainBody);
      mainModule->stmts->insertAtTail(new ExprStmt(maindef));
      mainFn = dynamic_cast<FnSymbol*>(maindef->sym);
    } else {
      USR_FATAL("Code defines multiple modules but no main function.");
    }
  } else {
    // tack call to main fn module's init call onto main fn's body
    mainModule = dynamic_cast<ModuleSymbol*>(mainFn->parentScope->astParent);
    if (!mainModule) {
      INT_FATAL(mainFn, "main function's parent scope wasn't a module scope");
    }
    mainBody = mainFn->body;
  }
  mainBody->body->insertAtHead(buildCallExprStmt(mainModule->initFn));
  mainBody->body->insertAtHead(buildCallExprStmt(commonModule->initFn));
  mainBody->body->insertAtHead(buildCallExprStmt(prelude->initFn));
}
