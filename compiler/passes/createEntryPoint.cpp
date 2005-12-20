#include "createEntryPoint.h"
#include "astutil.h"
#include "expr.h"
#include "filesToAST.h"
#include "runAnalysis.h"
#include "stmt.h"
#include "symtab.h"
#include "stringutil.h"


static bool stmtIsGlob(Stmt* stmt) {

  if (stmt == NULL) {
    INT_FATAL("Non-Stmt found in StmtIsGlob");
  }
  if (ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt)) {
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr_stmt->expr)) {
      if (dynamic_cast<FnSymbol*>(defExpr->sym) ||
          dynamic_cast<TypeSymbol*>(defExpr->sym)) {
        return true;
      }
    }
  }
  return false;
}


static void createInitFn(ModuleSymbol* mod) {
  char* fnName = stringcat("__init_", mod->name);
  AList<Stmt>* globstmts = NULL;
  AList<Stmt>* initstmts = NULL;
  AList<Stmt>* definition = mod->stmts;

  // BLC: code to run user modules once only
  char* runOnce = NULL;
  if (mod->modtype == MOD_USER && !fnostdincs) {
    runOnce = stringcat("__run_", mod->name, "_firsttime");
    // create a boolean variable to guard module initialization
    DefExpr* varDefExpr = new DefExpr(new VarSymbol(runOnce, dtBoolean),
                                      new_BoolLiteral(true));
    // insert its definition in the common module's init function
    commonModule->initFn->insertAtHead(varDefExpr);
 
    // insert a set to false at the beginning of the current module's
    // definition (we'll wrap it in a conditional just below, after
    // filtering)
    Expr* assignVar = new CallExpr(OP_MOVE,
                                   new SymExpr(new UnresolvedSymbol(runOnce)),
                                   new_BoolLiteral(false));
    definition->insertAtHead(assignVar);
  }

  definition->filter(stmtIsGlob, globstmts, initstmts);

  definition = globstmts;
  BlockStmt* initFunBody;
  if (initstmts->isEmpty()) {
    initFunBody = new BlockStmt();
  } else {
    initFunBody = new BlockStmt(initstmts);
  }
  initFunBody->blkScope = mod->modScope;
  if (runOnce) {
    // put conditional in front of body
    Stmt* testRun =
      new CondStmt(
        new CallExpr(
          "not",
          new SymExpr(
            new UnresolvedSymbol(runOnce))), 
        new ReturnStmt());
    initFunBody->insertAtHead(testRun);
  }

  mod->initFn = new FnSymbol(fnName);
  mod->initFn->retType = dtVoid;
  mod->initFn->body = initFunBody;
  definition->insertAtHead(new DefExpr(mod->initFn));
  mod->stmts->insertAtHead(definition);
}


void createEntryPoint(void) {
  currentLineno = -1;

  forv_Vec(ModuleSymbol, mod, allModules) {
    createInitFn(mod);
  }
}
