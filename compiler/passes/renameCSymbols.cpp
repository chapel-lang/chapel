#include "renameCSymbols.h"
#include "symtab.h"


static FnSymbol* ensureMainExists(ModuleSymbol* moduleList) {
  FnSymbol* mainFn = FnSymbol::mainFn;
  if (mainFn->isNull()) {
    if (moduleList->next->isNull()) {
      // TODO: factor this together with similar code in filesToAST.cpp?
      FnCall* initCall = new FnCall(new Variable(moduleList->initFn));
      ExprStmt* initStmt = new ExprStmt(initCall);
      BlockStmt* mainBody = new BlockStmt(initStmt);
      FnDefStmt* maindefstmt = Symboltable::defineFunction("main", nilSymbol, 
							   dtVoid, mainBody, 
							   true);
      moduleList->stmts->append(maindefstmt);
      mainFn = maindefstmt->fn;
      FnSymbol::mainFn = mainFn;
    } else {
      fail("code defines multiple modules but no main function");
    }
  }

  return mainFn;
}


void RenameCSymbols::run(ModuleSymbol* moduleList) {
  FnSymbol* mainFn = ensureMainExists(moduleList);

  mainFn->cname = "_chpl_main";
}
