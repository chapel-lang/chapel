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
inlineCall(CallExpr* call) {
  SET_LINENO(call);

  Expr* stmt = call->getStmtExpr();
  FnSymbol* fn = call->isResolved();

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

  reset_line_info(block, call->lineno);
  CallExpr* return_stmt = toCallExpr(block->body.last());
  if (!return_stmt || !return_stmt->isPrimitive(PRIMITIVE_RETURN))
    INT_FATAL(call, "function is not normalized");
  Expr* return_value = return_stmt->get(1);
  SymExpr* se = toSymExpr(return_value);
  if (!se || isArgSymbol(se->var))
    INT_FATAL(fn, "inlined function cannot return an argument symbol");
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
  Vec<BaseAST*> asts;
  collect_asts(fn, asts);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (FnSymbol* fn = call->isResolved()) {
        if (call->parentSymbol && fn->hasFlag(FLAG_INLINE)) {
          if (inlinedSet.set_in(fn))
            INT_FATAL(call, "recursive inlining detected");
          inlineFunction(fn, inlinedSet);
        }
      }
    }
  }
  collapseBlocks(fn->body);
  removeUnnecessaryGotos(fn);
  if (!fNoCopyPropagation) {
    singleAssignmentRefPropagation(fn);
    localCopyPropagation(fn);
  }
 if (!fNoDeadCodeElimination)
    deadVariableElimination(fn);
  deadExpressionElimination(fn);
  forv_Vec(CallExpr, call, *fn->calledBy) {
    inlineCall(call);
    if (report_inlining)
      printf("chapel compiler: reporting inlining"
             ", %s function was inlined\n", fn->cname);
  }
}


//
// inline all functions with the inline flag
// remove unnecessary block statements and gotos
//
void
inlineFunctions(void) {
  if (fNoInline) {
    forv_Vec(FnSymbol, fn, gFns) {
      collapseBlocks(fn->body);
      removeUnnecessaryGotos(fn);
      deadExpressionElimination(fn);
    }
    return;
  }

  compute_call_sites();

  Vec<FnSymbol*> inlinedSet;
  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->hasFlag(FLAG_INLINE) && !inlinedSet.set_in(fn))
      inlineFunction(fn, inlinedSet);
  }

  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->hasFlag(FLAG_INLINE)) {
      fn->defPoint->remove();
    } else {
      collapseBlocks(fn->body);
      removeUnnecessaryGotos(fn);
    }
  }
}
