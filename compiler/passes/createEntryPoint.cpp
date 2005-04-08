#include "createEntryPoint.h"
#include "runAnalysis.h"
#include "expr.h"
#include "filesToAST.h"
#include "stmt.h"
#include "symtab.h"


static ExprStmt* buildFnCallStmt(FnSymbol* fn) {
  return new ExprStmt(new FnCall(new Variable(fn), NULL));
}


static ModuleSymbol* findUniqueUserModule(ModuleSymbol* moduleList) {
  ModuleSymbol* userModule = NULL;

  while (moduleList) {
    if (!moduleList->internal && moduleList != commonModule) {
      if (userModule == NULL) {
        userModule = moduleList;
      } else {
        return NULL;  // two user modules defined
      }
    }
    moduleList = nextLink(ModuleSymbol, moduleList);
  }
  return userModule;
}


void CreateEntryPoint::run(ModuleSymbol* moduleList) {

  for (ModuleSymbol* mod = moduleList; mod; mod = nextLink(ModuleSymbol, mod)) {
    if (mod->internal || !ModuleDefContainsOnlyNestedModules(mod->stmts)) {
      if (mod != commonModule) {
        SymScope* saveScope = Symboltable::setCurrentScope(mod->modScope);
        mod->createInitFn();
        Symboltable::setCurrentScope(saveScope);
      }
    }
  }

  // add prelude initialization code to the entry point
  // BLC: This assumes there is some useful init code in the preludes;
  // is there?
  entryPoint = appendLink(entryPoint, buildFnCallStmt(internalPrelude->initFn));
  entryPoint = appendLink(entryPoint, buildFnCallStmt(prelude->initFn));

  // find main function if it exists; create one if not
  FnSymbol* mainFn = FnSymbol::mainFn;
  if (!mainFn) {
    ModuleSymbol* userModule = findUniqueUserModule(moduleList);
    if (userModule) {
      ExprStmt* initStmt = buildFnCallStmt(userModule->initFn);
      BlockStmt* mainBody = new BlockStmt(initStmt);
      SymScope* saveScope = Symboltable::getCurrentScope();
      Symboltable::setCurrentScope(userModule->modScope);
      DefStmt* maindefstmt = Symboltable::defineFunction("main", NULL, 
                                                         dtVoid, mainBody, 
                                                         true);
      Symboltable::setCurrentScope(saveScope);
      userModule->stmts->append(maindefstmt);
      mainFn = maindefstmt->fnDef();
    } else {
      USR_FATAL("Code defines multiple modules but no main function.");
    }
  } else {
    // tack call to main fn module's init call onto main fn's body
    ModuleSymbol* mainModule = 
      dynamic_cast<ModuleSymbol*>(mainFn->parentScope->symContext);
    if (!mainModule) {
      INT_FATAL(mainFn, "main function's parent scope wasn't a module scope");
    }
    ExprStmt* initStmt = buildFnCallStmt(mainModule->initFn);
    initStmt->append(mainFn->body);
    mainFn->body = new BlockStmt(initStmt);
    SET_BACK(mainFn->body);  // SJD: Eliminate please.
  }


  // add a call to main to the entry point's body
  ExprStmt* mainCallStmt = buildFnCallStmt(mainFn);
  entryPoint = appendLink(entryPoint, mainCallStmt);

   // create the new entry point module
  ModuleSymbol* entry = new ModuleSymbol("entryPoint", true);
  entry->stmts = entryPoint;
  entry->createInitFn();
  entryPoint = entry->stmts;

  RunAnalysis::entryStmtList = entryPoint;
}
