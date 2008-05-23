#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "optimizations.h"


//
// copyMap is useful for debugging when trying to determine where an
// expression or symbol came from; since it may be copied many times
// when inlining functions inside functions inside functions ..., the
// copyMap points to the original, a BaseAST that existed before this
// pass even started.
//
// example: If you want to see where a CallExpr, say with id ==
// 813899, came from before inlining, just trace it back in the
// copyMap when breaking at the end of inlineFunctions.  You need to
// break before the next pass because the inlined functions will be
// deleted.  To trace it back, use the following:
//
// (gdb) p copyMap.get(ast(813899))->id
// 809206
// (gdb) p copyMap.get(ast(809206))->id
// 809157
// (gdb) p copyMap.get(ast(809157))->id
// 663565
//
// Then to see the ast in the original function, use
//
// (gdb) lv ast(663565)
//
// To make the copyMap active, uncomment the line
//
//#define _INLINE_FUNCTIONS_USE_COPY_MAP_
//
#ifdef _INLINE_FUNCTIONS_USE_COPY_MAP_
static Map<BaseAST*,BaseAST*> copyMap;
#endif


//
// inlines the function called by 'call' at that call site
//
static void
inlineCall(CallExpr* call) {
  currentLineno = call->lineno;

  Expr* stmt = call->getStmtExpr();
  FnSymbol* fn = call->isResolved();

  //
  // calculate a map from actual symbols to formal symbols
  //
  ASTMap map;
  for_formals_actuals(formal, actual, call) {
    SymExpr* se = toSymExpr(actual);
    INT_ASSERT(se);
    map.put(formal, se->var);
  }

  //
  // copy function body, inline it at call site, and update return
  //
  BlockStmt* block = fn->body->copy(&map);

#ifdef _INLINE_FUNCTIONS_USE_COPY_MAP_
  form_Map(ASTMapElem, e, map) {
    copyMap.put(e->value, e->key);
  }
#endif

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
  collect_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = toCallExpr(ast)) {
      if (FnSymbol* fn = call->isResolved()) {
        if (call->parentSymbol && fn->hasPragma("inline")) {
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
// inline all functions with the inline pragma
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
    if (fn->hasPragma("inline") && !inlinedSet.set_in(fn))
      inlineFunction(fn, inlinedSet);
  }

  forv_Vec(FnSymbol, fn, gFns) {
    if (fn->hasPragma("inline")) {
      fn->defPoint->remove();
    } else {
      collapseBlocks(fn->body);
      removeUnnecessaryGotos(fn);
    }
  }
}
