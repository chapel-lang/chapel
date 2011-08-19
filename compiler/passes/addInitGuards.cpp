// addInitGuards.c
//////////////////////////////////////////////////////////////////////////////////
// Add initialization guards to the module init functions.
//
// This pass must be run after buildDefaultFunctions().
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

    // Add:
    //  bool <init_fn_name>_p = false;
    // to the preInit function.
    const char* init_p = astr(fn->name, "_p"); // Add _p to make it a predicate.
    Symbol* var = new VarSymbol(init_p, dtBool);
    Expr* declExpr = new DefExpr(var);
    theProgram->block->insertAtTail(declExpr);

    // <init_fn_name>_p = true;
    // Is added to the preInit function.
    Expr* asgnExprFalse = new CallExpr(PRIM_MOVE, var, new SymExpr(gFalse));
    initFn->insertBeforeReturn(asgnExprFalse);

    // <init_fn_name>_p = true;
    // Precedes the body.
    Expr* asgnExprTrue = new CallExpr(PRIM_MOVE, var, new SymExpr(gTrue));
    fn->insertAtHead(asgnExprTrue);

    // if (<init_fn_name>_p) return;
    // Precedes everything, including the assignment we just added.
    Expr* thenExpr = new CallExpr(PRIM_RETURN, gVoid);
    BlockStmt* ifStmt = buildIfStmt(new SymExpr(var), thenExpr);
    fn->insertAtHead(ifStmt);

    normalize(fn);
  }
}
