#include "alist.h"
#include "analysis.h"
#include "driver.h"
#include "filesToAST.h"
#include "if1.h"
#include "moduleList.h"
#include "runAnalysis.h"
#include "symbol.h"
#include "symtab.h"

int RunAnalysis::runCount = 0;
int RunAnalysis::isRunning = 0;
AList<Stmt>* RunAnalysis::entryStmtList = NULL;

void RunAnalysis::run(ModuleList* moduleList) {
  if (analyzeAST) {
    RunAnalysis::isRunning = 1;
    if1->callback = new ACallbacks;
    init_ast();
    Vec<AList<Stmt> *> stmts;
    ModuleSymbol* mod = moduleList->first();
    while (mod) {
      stmts.add(mod->stmts);

      mod = moduleList->next();
    }
    stmts.add(entryStmtList);
    AST_to_IF1(stmts);
    // BLC: John, what filename should be passed in for multiple modules?
    // I'm just passing in the first non-internal module's filename
    // JBP: that's fine, it is only used for debug, HTML and low level cg files
    char* firstUserModuleName = NULL;
    mod = moduleList->first();
    while (mod && (mod->modtype != MOD_USER)) {
      mod = moduleList->next();
    }
    if (mod) {
      firstUserModuleName = mod->filename;
    }
    //driver:do_analysis
    do_analysis(firstUserModuleName);
#if 0
    // test type_is_used
    forv_Sym(s, if1->allsyms) {
      if (s->type_kind) {
        if (s->asymbol->symbol && s->name && 
            dynamic_cast<Type*>(s->asymbol->symbol) &&
            dynamic_cast<TypeSymbol*>(dynamic_cast<Type*>(s->asymbol->symbol)->name))
          printf("%s is_used: %d\n", s->name, 
                 type_is_used(dynamic_cast<TypeSymbol*>(dynamic_cast<Type*>(s->asymbol->symbol)->name)));
      }
    }
#endif
    runCount++;
    RunAnalysis::isRunning = 0;
  }
}
