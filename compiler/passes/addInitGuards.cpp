// addInitGuards.c
//////////////////////////////////////////////////////////////////////////////////
// Add initialization guards to the module init functions.
//
// This pass must be run after parallel().
//
// This makes the initialization functions idempotent -- meaning that they can
// be executed any number of times, but the net effect is as if they were only 
// called once.  That is done using the idiom:
//
//      /* Assume flag_p is initially false. */
//      if (flag_p) return;
//      flag_p = true;
//      ...     // Rest of initialization code.
//
// The guard code is added only if the initialization function has a nontrivial body.
//
// This pass also creates the function "chpl__init_preInit()", which 
// initializes all of the initialization flags to false.
//

#include "passes.h"
#include "stmt.h"
#include "build.h"
#include "astutil.h"

void addInitGuards(void) {
  // We need a function to drop the initializers into.
  FnSymbol* initFn = new FnSymbol(astr("chpl__init_preInit"));
  initFn->retType = dtVoid;
  initFn->addFlag(FLAG_EXPORT);
  initFn->addFlag(FLAG_INSERT_LINE_FILE_INFO);
  initFn->addFlag(FLAG_PROC_ITER_KW_USED); // ProcIter: remove
  theProgram->block->insertAtTail(new DefExpr(initFn));
  normalize(initFn);

  // Iterate all functions and select just the module initialization functions.
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (! fn->hasFlag(FLAG_MODULE_INIT))
      continue;

    // Test if this fn has a nontrivial body.
    BlockStmt* body = fn->body;
    if (body->length() < 1)
      continue;

    // Alright then, add the guard.

    // The declaration:
    //      var <init_fn_name>_p : bool;
    // is added to the end of the program block.
    const char* init_p = astr(fn->name, "_p"); // Add _p to make it a predicate.
    Symbol* var = new VarSymbol(init_p, dtBool);
    Expr* declExpr = new DefExpr(var);
    theProgram->block->insertAtTail(declExpr);

    // The assignment:
    //      <init_fn_name>_p = false;
    // is added at the end of chpl__init_preInit().
    // This means the module has not yet been initialized.
    Expr* asgnExprFalse = new CallExpr(PRIM_MOVE, var, new SymExpr(gFalse));
    initFn->insertBeforeReturn(asgnExprFalse);

    // The assignment:
    //      <init_fn_name>_p = true;
    // is added to the start of the module initialization function.
    // This means that the module has been initialized.
    Expr* asgnExprTrue = new CallExpr(PRIM_MOVE, var, new SymExpr(gTrue));
    fn->insertAtHead(asgnExprTrue);

    // The guard:
    //      if (<init_fn_name>_p) goto _exit_<init_fn_name>.
    // Precedes everything in the module initialization function,
    // including the assignment we just added.
    LabelSymbol* label = new LabelSymbol(astr("_exit_", fn->name));
    fn->insertBeforeReturnAfterLabel(new DefExpr(label));
    Expr* gotoExit = new GotoStmt(GOTO_NORMAL, label);
    Expr* ifStmt = new CondStmt(new SymExpr(var), gotoExit);
    fn->insertAtHead(ifStmt);
  }
}
