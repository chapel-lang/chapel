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
  currentFilename = call->filename;
  currentLineno = call->lineno;

  Expr* stmt = call->getStmtExpr();
  FnSymbol* fn = call->isResolved();

  //
  // insert temporary symbols for all actuals that require one and
  // calculate a map from actual symbols to formal symbols
  //
  // sjd: do we need these temporaries given those we insert inside a
  //      function during resolution to maintain copy semantics
  //
  ASTMap map;
  for_formals_actuals(formal, actual, call) {
    if (SymExpr* se = toSymExpr(actual)) {
        map.put(formal, se->var);
    } else {

      //
      // try to eliminate temporaries
      //
      if (formal->requiresCPtr())
        INT_FATAL(actual, "illegal reference actual encountered in inlining");
      VarSymbol* temp = new VarSymbol("_tmp", actual->typeInfo());
      temp->isCompilerTemp = true;
      stmt->insertBefore(new DefExpr(temp));
      stmt->insertBefore(new CallExpr(PRIMITIVE_MOVE, temp, actual->remove()));
      map.put(formal, temp);

    }
  }

  //
  // copy function body, inline it at call site, and update return
  //
  BlockStmt* block = fn->body->copy(&map);
  reset_file_info(block, call->lineno, call->filename);
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
        if (fn->hasPragma("inline") && !inlinedSet.set_in(fn)) {
          inlineFunction(fn, inlinedSet);
        }
      }
    }
  }
  collapseBlocks(fn->body);
  removeUnnecessaryGotos(fn);
  if (!fNoCopyPropagation)
    localCopyPropagation(fn);
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
  if (fBaseline)
    return;

  if (fNoInline) {
    forv_Vec(FnSymbol, fn, gFns) {
      collapseBlocks(fn->body);
      removeUnnecessaryGotos(fn);
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
