#include "analysis.h"
#include "createEntryPoint.h"
#include "driver.h"
#include "filesToAST.h"
#include "if1.h"
#include "runAnalysis.h"
#include "symbol.h"
#include "symtab.h"

int RunAnalysis::runCount = 0;

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
    // I'm just passing in the first non-internal module's filename
    char* firstUserModuleName = NULL;
    while (moduleList && (moduleList->internal || moduleList == commonModule)) {
      moduleList = nextLink(ModuleSymbol, moduleList);
    }
    if (moduleList) {
      firstUserModuleName = moduleList->filename;
    }
    do_analysis(firstUserModuleName);
    runCount++;
  }
}
