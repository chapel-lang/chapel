/**************************************************************************
  Copyright (c) 2004-2013, Cray Inc.  (See LICENSE file for more details)
**************************************************************************/


// addInitCalls.c
//////////////////////////////////////////////////////////////////////////////////
// Add module initialization calls and  guards to the module init functions.
//
// Initially, the module initialization functions contain only the "loose" 
// statements found within the body of the corresponding module.  
// This pass adds calls to the module initialization functions for 
// parent modules as well as modules mentioned in "use" statements within
// the module.  Then, it wraps the entire initialization function in a guard 
// to ensure that it is run just once.
//
// This pass must be run after parallel().
//

#include "passes.h"
#include "stmt.h"
#include "build.h"
#include "astutil.h"

static void addModuleInitBlocks();
static void addInitGuards();
static void addInitGuard(FnSymbol* fn, FnSymbol* preInitFn);


void addInitCalls()
{
  addModuleInitBlocks();
  addInitGuards();
}


void addModuleInitBlocks() {
  forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
    // Not for the root module
    if (mod == rootModule) continue;

    FnSymbol* fn = toFnSymbol(mod->initFn);
    if (!fn)
      // Sometimes a module parsed on the command line
      // is not actually used, so its initializer is pruned during resolution.
      continue;

    SET_LINENO(mod);
    BlockStmt* initBlock = new BlockStmt();

    // If I have a parent, I need it initialized first,
    // since all of its symbols are visible to me.
    if (ModuleSymbol* parent = mod->defPoint->getModule())
      // The initializer for theProgram is called specially in main.c,
      // so we don't have to call it here.
      if (parent != theProgram && parent != rootModule)
        initBlock->insertAtTail(new CallExpr(parent->initFn));

    // Now, traverse my use statements, and call the initializer for each
    // module I use.
    forv_Vec(ModuleSymbol, usedMod, mod->modUseList) {
      initBlock->insertAtTail(new CallExpr(usedMod->initFn));
    }

    fn->insertAtHead(initBlock);
  }
}


// This function makes the initialization functions idempotent -- meaning that they can
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
static void addInitGuards(void) {
  // We need a function to drop the initializers into.
  SET_LINENO(baseModule);
  FnSymbol* preInitFn = new FnSymbol(astr("chpl__init_preInit"));
  preInitFn->retType = dtVoid;
  preInitFn->addFlag(FLAG_EXPORT);
  preInitFn->addFlag(FLAG_INSERT_LINE_FILE_INFO);
  theProgram->block->insertAtTail(new DefExpr(preInitFn));
  normalize(preInitFn);

  // Iterate all modules and select their module initialization functions.
  forv_Vec(ModuleSymbol, mod, gModuleSymbols) {
    if (mod == rootModule)
      continue;

    FnSymbol* fn = toFnSymbol(mod->initFn);
    if (!fn)
      // Sometimes a module parsed on the command line
      // is not actually used, so its initializer is pruned.
      continue;

    // Test if this fn has a nontrivial body.
    BlockStmt* body = fn->body;
    if (body->length() < 1)
      continue;

    // Alright then, add the guard.
    addInitGuard(fn, preInitFn);
  }
}

static void addInitGuard(FnSymbol* fn, FnSymbol* preInitFn)
{
    SET_LINENO(fn);
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
    preInitFn->insertBeforeReturn(asgnExprFalse);

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
