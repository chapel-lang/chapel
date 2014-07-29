#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "optimizations.h"


//
// inlines the function called by 'call' at that call site
//
static void
inlineCall(FnSymbol* fn, CallExpr* call) {
  INT_ASSERT(call->isResolved() == fn);
  SET_LINENO(call);

  Expr* stmt = call->getStmtExpr();

  //
  // calculate a map from actual symbols to formal symbols
  //
  SymbolMap map;
  for_formals_actuals(formal, actual, call) {
    SymExpr* se = toSymExpr(actual);
    INT_ASSERT(se);
    map.put(formal, se->var);
  }

  //
  // copy function body, inline it at call site, and update return
  //
  BlockStmt* block = fn->body->copy(&map);
  if (!preserveInlinedLineNumbers)
    reset_ast_loc(block, call);
  CallExpr* return_stmt = toCallExpr(block->body.last());
  if (!return_stmt || !return_stmt->isPrimitive(PRIM_RETURN))
    INT_FATAL(call, "function is not normalized");
  Expr* return_value = return_stmt->get(1);
  SymExpr* se = toSymExpr(return_value);
  // Ensure that the inlined function body does not attempt to return one of
  // the original function's formals.  This is equivalent to saying that if the
  // returned value is originally one of the formal argument symbols, that
  // symbol was replaced by it actual argument in the call to copy(&map) above.
  for_formals(formal, fn)
    INT_ASSERT(formal != toArgSymbol(se->var));
  return_stmt->remove();
  return_value->remove();
  stmt->insertBefore(block);
  if (fn->retType == dtVoid)
    stmt->remove();
  else
    call->replace(return_value);
}


//
// inline function fn at all call sites
// add inlined function to inlinedSet
// inline any functions that are called from within this function and
// should be inlined first
//
static void
inlineFunction(FnSymbol* fn, Vec<FnSymbol*>& inlinedSet) {
  inlinedSet.set_add(fn);
  Vec<CallExpr*> calls;
  collectFnCalls(fn, calls);
  forv_Vec(CallExpr, call, calls) {
    if (call->parentSymbol) {
      FnSymbol* fn = call->isResolved();
      if (fn->hasFlag(FLAG_INLINE)) {
        if (inlinedSet.set_in(fn))
          INT_FATAL(call, "recursive inlining detected");
        inlineFunction(fn, inlinedSet);
      }
    }
  }
  collapseBlocks(fn->body);
  removeUnnecessaryGotos(fn);
#if DEBUG_CP < 2    // That is, disabled if DEBUG_CP >= 2
  if (!fNoCopyPropagation) {
    singleAssignmentRefPropagation(fn);
    localCopyPropagation(fn);
  }
#endif
  if (!fNoDeadCodeElimination) {
    deadVariableElimination(fn);
    deadExpressionElimination(fn);
  }
  forv_Vec(CallExpr, call, *fn->calledBy) {
    if (call->isResolved()) {
      inlineCall(fn, call);
      if (report_inlining)
        printf("chapel compiler: reporting inlining"
               ", %s function was inlined\n", fn->cname);
    }
  }
}


//
// inline all functions with the inline flag
// remove unnecessary block statements and gotos
//
void
inlineFunctions(void) {
  if (!fNoInline) {
    compute_call_sites();
    Vec<FnSymbol*> inlinedSet;
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      if (fn->hasFlag(FLAG_INLINE) && !inlinedSet.set_in(fn))
        inlineFunction(fn, inlinedSet);
    }
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (!fNoInline && fn->hasFlag(FLAG_INLINE) && !fn->hasFlag(FLAG_VIRTUAL)) {
      fn->defPoint->remove();
    } else {
      collapseBlocks(fn->body);
      removeUnnecessaryGotos(fn);
    }
  }
}
