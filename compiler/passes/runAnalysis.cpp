#include "analysis.h"
#include "createEntryPoint.h"
#include "driver.h"
#include "filesToAST.h"
#include "if1.h"
#include "runAnalysis.h"
#include "symbol.h"

void RunAnalysis::run(ModuleSymbol* moduleList) {
  if (analyzeAST) {
    if1->callback = new ACallbacks;
    init_ast();
    Vec<Stmt *> stmts;
    ModuleSymbol* mod = moduleList;
    while (mod) {
      stmts.add(mod->stmts);

      mod = nextLink(ModuleSymbol, mod);
    }
    stmts.add(CreateEntryPoint::entryPoint);
    AST_to_IF1(stmts);
    // BLC: John, what filename should be passed in for multiple modules?
    do_analysis(moduleList->filename);
  }
}
