#include "analysis.h"
#include "createAST.h"
#include "driver.h"
#include "if1.h"
#include "runAnalysis.h"

void RunAnalysis::run(Stmt* program) {
  if (analyzeNewAST) {
    if1->callback = new ACallbacks;
    init_ast();
    Vec<Stmt *> stmts;
    stmts.add(internalPreludeStmts);
    stmts.add(preludeStmts);
    stmts.add(programStmts);
    stmts.add(entryPoint);
    AST_to_IF1(stmts);
    do_analysis(filename);
  }
}
