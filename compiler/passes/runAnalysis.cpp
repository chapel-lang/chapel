#include "analysis.h"
#include "driver.h"
#include "filesToAST.h"
#include "if1.h"
#include "runAnalysis.h"
#include "symbol.h"

void RunAnalysis::run(ModuleSymbol* moduleList) {
  if (analyzeNewAST) {
    if1->callback = new ACallbacks;
    init_ast();
    Vec<Stmt *> stmts;
    stmts.add(internalPrelude->stmts);
    stmts.add(prelude->stmts);
    ModuleSymbol* mod = moduleList;
    while (mod) {
      stmts.add(mod->stmts);

      mod = nextLink(ModuleSymbol, mod);
    }
    stmts.add(entryPoint);
    AST_to_IF1(stmts);
    // JOHN: what filename should be passed in for multiple modules?
    do_analysis(moduleList->filename);
  }
}
