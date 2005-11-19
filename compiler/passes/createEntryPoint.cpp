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
  if (mod->modtype == MOD_USER) {
    runOnce = stringcat("__run_", mod->name, "_firsttime");
    // create a boolean variable to guard module initialization
    DefExpr* varDefExpr = new DefExpr(new VarSymbol(runOnce, dtBoolean),
                                      new_BoolLiteral(true));
    // insert its definition in the common module's init function
    commonModule->initFn->insertAtHead(new ExprStmt(varDefExpr));
 
    // insert a set to false at the beginning of the current module's
    // definition (we'll wrap it in a conditional just below, after
    // filtering)
    Expr* assignVar = new CallExpr(OP_GETSNORM,
                                   new SymExpr(new UnresolvedSymbol(runOnce)),
                                   new_BoolLiteral(false));
    definition->insertAtHead(new ExprStmt(assignVar));
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
          OP_LOGNOT,
          new SymExpr(
            new UnresolvedSymbol(runOnce))), 
        new ReturnStmt(NULL));
    initFunBody->insertAtHead(testRun);
  }

  DefExpr* initFunDef = Symboltable::defineFunction(fnName, NULL, 
                                                    dtVoid, initFunBody);
  definition->insertAtHead(new ExprStmt(initFunDef));
  mod->initFn = dynamic_cast<FnSymbol*>(initFunDef->sym);
  mod->stmts->insertAtHead(definition);
}


void createEntryPoint(void) {
  currentLineno = -1;

  forv_Vec(ModuleSymbol, mod, allModules) {
    if (mod->modtype == MOD_INTERNAL || 
        !ModuleDefContainsOnlyNestedModules(mod->stmts)) {
      createInitFn(mod);
    }
  }
}
