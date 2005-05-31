#include "createEntryPoint.h"
#include "expr.h"
#include "filesToAST.h"
#include "moduleList.h"
#include "runAnalysis.h"
#include "stmt.h"
#include "symtab.h"


static ExprStmt* buildFnCallStmt(FnSymbol* fn) {
  Variable* variable = new Variable(fn);
  variable->lineno = -1;
  FnCall* fnCall = new FnCall(variable);
  fnCall->lineno = -1;
  ExprStmt* exprStmt = new ExprStmt(fnCall);
  exprStmt->lineno = -1;
  return exprStmt;
}


static ModuleSymbol* findUniqueUserModule(ModuleList* moduleList) {
  ModuleSymbol* userModule = NULL;

  ModuleSymbol* mod = moduleList->first();
  while (mod) {
    if (mod->modtype == MOD_USER) {
      if (userModule == NULL) {
        userModule = mod;
      } else {
        return NULL;  // two user modules defined
      }
    }
    mod = moduleList->next();
  }
  return userModule;
}


CreateEntryPoint::CreateEntryPoint(void) :
  entryPoint(new AList<Stmt>())
{}


void CreateEntryPoint::run(ModuleList* moduleList) {

  // SJD: Can't do this when dtString is defined because
  // internalPrelude hasn't been made yet.  Need to do it after.
  dtString->defaultConstructor =
    dynamic_cast<FnSymbol*>(Symboltable::lookupInternal("_init_string"));

  for (ModuleSymbol* mod = moduleList->first(); mod; mod = moduleList->next()) {
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
  entryPoint->add(buildFnCallStmt(internalPrelude->initFn));
  entryPoint->add(buildFnCallStmt(prelude->initFn));
  entryPoint->add(buildFnCallStmt(commonModule->initFn));

  // find main function if it exists; create one if not
  FnSymbol* mainFn = FnSymbol::mainFn;
  if (!mainFn) {
    ModuleSymbol* userModule = findUniqueUserModule(moduleList);
    if (userModule) {
      AList<Stmt>* initStmts = new AList<Stmt>();
      initStmts->add(buildFnCallStmt(commonModule->initFn));
      initStmts->add(buildFnCallStmt(userModule->initFn));
      BlockStmt* mainBody = new BlockStmt(initStmts);
      SymScope* saveScope = Symboltable::getCurrentScope();
      Symboltable::setCurrentScope(userModule->modScope);
      DefStmt* maindefstmt = Symboltable::defineFunction("main", NULL, 
                                                         dtVoid, mainBody, 
                                                         true);
      Symboltable::setCurrentScope(saveScope);
      userModule->stmts->add(maindefstmt);
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
    AList<Stmt>* initStmts = new AList<Stmt>();
    initStmts->add(buildFnCallStmt(commonModule->initFn));
    initStmts->add(buildFnCallStmt(mainModule->initFn));
    initStmts->add(mainFn->body);
    mainFn->body = new BlockStmt(initStmts);
  }


  // add a call to main to the entry point's body
  ExprStmt* mainCallStmt = buildFnCallStmt(mainFn);
  entryPoint->add(mainCallStmt);

   // create the new entry point module
  ModuleSymbol* entry = new ModuleSymbol("entryPoint", MOD_INTERNAL);
  entry->stmts->add(entryPoint);
  entry->createInitFn();
  entryPoint = entry->stmts;

  RunAnalysis::entryStmtList = entry->stmts;
}
