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
  currentLineno = -1;

  // SJD: Can't do this when dtString is defined because
  // prelude hasn't been made yet.  Need to do it after.
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
  entryPoint->insertAtTail(buildFnCallStmt(prelude->initFn));
  entryPoint->insertAtTail(buildFnCallStmt(commonModule->initFn));

  // find main function if it exists; create one if not
  FnSymbol* mainFn = FnSymbol::mainFn;
  BlockStmt* mainBody;
  ModuleSymbol* mainModule;
  if (!mainFn) {
    mainModule = findUniqueUserModule(moduleList);
    if (mainModule) {
      SymScope* saveScope = Symboltable::getCurrentScope();
      Symboltable::setCurrentScope(mainModule->modScope);
      mainBody = new BlockStmt();
      DefExpr* maindef = Symboltable::defineFunction("main", NULL, 
                                                         dtVoid, mainBody, 
                                                         true);
      Symboltable::setCurrentScope(saveScope);
      mainModule->stmts->insertAtTail(new ExprStmt(maindef));
      mainFn = dynamic_cast<FnSymbol*>(maindef->sym);
    } else {
      USR_FATAL("Code defines multiple modules but no main function.");
    }
  } else {
    // tack call to main fn module's init call onto main fn's body
    mainModule = dynamic_cast<ModuleSymbol*>(mainFn->parentScope->symContext);
    if (!mainModule) {
      INT_FATAL(mainFn, "main function's parent scope wasn't a module scope");
    }
    mainBody = mainFn->body;
  }
  mainBody->body->insertAtHead(buildFnCallStmt(mainModule->initFn));
  mainBody->body->insertAtHead(buildFnCallStmt(commonModule->initFn));

  // add a call to main to the entry point's body
  ExprStmt* mainCallStmt = buildFnCallStmt(mainFn);
  entryPoint->insertAtTail(mainCallStmt);

   // create the new entry point module
  ModuleSymbol* entry = new ModuleSymbol("entryPoint", MOD_INTERNAL);
  entry->stmts->add(entryPoint);
  entry->createInitFn();
  entryPoint = entry->stmts;

  RunAnalysis::entryStmtList = entry->stmts;
}
