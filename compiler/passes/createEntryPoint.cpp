#include "createEntryPoint.h"
#include "expr.h"
#include "filesToAST.h"
#include "stmt.h"
#include "symtab.h"

Stmt* CreateEntryPoint::entryPoint = NULL;

void CreateEntryPoint::addModuleInitToEntryPoint(ModuleSymbol* module) {
  ExprStmt* initFunCallStmt = ExprStmt::createFnCallStmt(module->initFn);
  entryPoint = appendLink(entryPoint, initFunCallStmt);
}


static ModuleSymbol* findUniqueUserModule(ModuleSymbol* moduleList) {
  ModuleSymbol* userModule = NULL;

  while (moduleList && !moduleList->isNull()) {
    if (!moduleList->internal) {
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
  // add prelude initialization code to the entry point
  // BLC: This assumes there is some useful init code in the preludes;
  // is there?
  CreateEntryPoint::addModuleInitToEntryPoint(internalPrelude);
  CreateEntryPoint::addModuleInitToEntryPoint(prelude);

  // find main function if it exists; create one if not
  FnSymbol* mainFn = FnSymbol::mainFn;
  if (mainFn->isNull()) {
    ModuleSymbol* userModule = findUniqueUserModule(moduleList);
    if (userModule) {
      ExprStmt* initStmt = ExprStmt::createFnCallStmt(userModule->initFn);
      BlockStmt* mainBody = new BlockStmt(initStmt);
      FnDefStmt* maindefstmt = Symboltable::defineFunction("main", nilSymbol, 
							   dtVoid, mainBody, 
							   true);
      userModule->stmts->append(maindefstmt);
      mainFn = maindefstmt->fn;
    } else {
      fail("code defines multiple modules but no main function");
    }
  }

  // add a call to main to the entry point's body
  ExprStmt* mainCallStmt = ExprStmt::createFnCallStmt(mainFn);
  entryPoint = appendLink(entryPoint, mainCallStmt);


  // create the new entry point module
  ModuleSymbol* entry = new ModuleSymbol("entryPoint", true);
  entry->stmts = entryPoint;
  entry->createInitFn();
  entryPoint = entry->stmts;
}
