#include "analysis.h"
#include "driver.h"
#include "filesToAST.h"
#include "if1.h"
#include "module.h"
#include "runAnalysis.h"

void RunAnalysis::run(Module* moduleList) {
  if (analyzeNewAST) {
    if1->callback = new ACallbacks;
    init_ast();
    Vec<Stmt *> stmts;
    stmts.add(internalPreludeStmts);
    stmts.add(preludeStmts);
    Module* mod = moduleList;
    while (mod) {
      stmts.add(mod->stmts);

      mod = nextLink(Module, mod);
    }
    stmts.add(entryPoint);
    AST_to_IF1(stmts);
    // JOHN: what filename should be passed in for multiple modules?
    do_analysis(moduleList->filename);
  }
}
