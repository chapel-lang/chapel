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
    if (moduleList->modtype == MOD_USER) {
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


CreateEntryPoint::CreateEntryPoint(void) :
  entryPoint(NULL)
{}


void CreateEntryPoint::run(ModuleSymbol* moduleList) {

  // SJD: Can't do this when dtString is defined because
  // internalPrelude hasn't been made yet.  Need to do it after.
  dtString->defaultConstructor =
    dynamic_cast<FnSymbol*>(Symboltable::lookupInternal("_init_string"));

  for (ModuleSymbol* mod = moduleList; mod; mod = nextLink(ModuleSymbol, mod)) {
    if (mod->modtype == MOD_INTERNAL || 
        !ModuleDefContainsOnlyNestedModules(mod->stmts)) {
      SymScope* saveScope = Symboltable::setCurrentScope(mod->modScope);
      mod->createInitFn();
      Symboltable::setCurrentScope(saveScope);
    }
  }

  // add prelude initialization code to the entry point
  // BLC: This assumes there is some useful init code in the preludes;
  // is there?
  entryPoint = appendLink(entryPoint, buildFnCallStmt(internalPrelude->initFn));
  entryPoint = appendLink(entryPoint, buildFnCallStmt(prelude->initFn));
  entryPoint = appendLink(entryPoint, buildFnCallStmt(commonModule->initFn));

  // find main function if it exists; create one if not
  FnSymbol* mainFn = FnSymbol::mainFn;
  if (!mainFn) {
    ModuleSymbol* userModule = findUniqueUserModule(moduleList);
    if (userModule) {
      ExprStmt* initStmt = buildFnCallStmt(commonModule->initFn);
      initStmt->append(buildFnCallStmt(userModule->initFn));
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
    ExprStmt* initStmt = buildFnCallStmt(commonModule->initFn);
    initStmt->append(buildFnCallStmt(mainModule->initFn));
    initStmt->append(mainFn->body);
    mainFn->body = new BlockStmt(initStmt);
  }


  // add a call to main to the entry point's body
  ExprStmt* mainCallStmt = buildFnCallStmt(mainFn);
  entryPoint = appendLink(entryPoint, mainCallStmt);

   // create the new entry point module
  ModuleSymbol* entry = new ModuleSymbol("entryPoint", MOD_INTERNAL);
  entry->stmts = entryPoint;
  entry->createInitFn();
  entryPoint = entry->stmts;

  RunAnalysis::entryStmtList = entryPoint;
}
