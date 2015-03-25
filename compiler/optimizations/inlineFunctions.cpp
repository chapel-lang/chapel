/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "optimizations.h"

#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"

#include <vector>

static bool canRemoveRefTemps(FnSymbol* fn);
static CallExpr* findRefTempInit(SymExpr* se);

//
// inlines the function called by 'call' at that call site
//
static void
inlineCall(FnSymbol* fn, CallExpr* call, Vec<FnSymbol*>& canRemoveRefTempSet) {
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
    if ((formal->intent & INTENT_REF) && canRemoveRefTempSet.set_in(fn)) {
      if (se->var->hasFlag(FLAG_REF_TEMP)) {
        if (CallExpr* move = findRefTempInit(se)) {
          SymExpr* origSym = NULL;
          if (CallExpr* addrOf = toCallExpr(move->get(2))) {
            INT_ASSERT(addrOf->isPrimitive(PRIM_ADDR_OF));
            origSym = toSymExpr(addrOf->get(1));
          } else {
            origSym = toSymExpr(move->get(2));
          }
          INT_ASSERT(origSym);
          map.put(formal, origSym->var);
          se->var->defPoint->remove();
          move->remove();
          continue;
        }
      }
    }
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

// Ideally we would compute this after inling all nested functions, but that
// doesn't work due to some cases that explictly expect a ref and have deref
// calls. Future work would be to find those cases and make change this check
// to support nested inling if possible.
static bool canRemoveRefTemps(FnSymbol* fn) {
  if (!fn) // primitive
    return true;

  // Work around for a bug with non-default size bools in no-local. I believe
  // they dont work in no-local due to not preserving thier size during
  // codegen.
  // TODO: Check if this can be removed onces bool sizes are preserved
  //       ( test/types/scalar/bradc/bools[2].chpl )
  if (!fLocal) {
    for_formals(formal, fn) {
      if (is_bool_type(formal->typeInfo()))
        return false;
    }
  }

  std::vector<CallExpr*> callExprs;
  collectCallExprsSTL(fn, callExprs);

  for_vector(CallExpr, call, callExprs) {
    if (!call->primitive) {
      return false;
    } else if (call->isPrimitive(PRIM_SET_MEMBER)) {
      return false;
    }
  }

  return true;
}

// Search for the first assingment (a PRIM_MOVE) to a ref temp. If found, the
// CallExpr doing the assignment will be returned, otherwise NULL. This works
// because a ref temp's DefExpr and inital assignment are inserted togther
// inside of insertReferenceTemps.
static CallExpr* findRefTempInit(SymExpr* se) {
  Expr* expr = se->var->defPoint->next;
  while (expr) {
    if (CallExpr* call = toCallExpr(expr)) {
      if (call->isPrimitive(PRIM_MOVE)) {
        if (se->var == toSymExpr(call->get(1))->var) {
          if (CallExpr* nestedCall = toCallExpr(call->get(2))) {
            if (!nestedCall->isPrimitive(PRIM_ADDR_OF)) {
              return NULL;
            }
          }
          return call;
        }
      }
    }
    expr = expr->next;
  }
  return NULL;
}

//
// inline function fn at all call sites
// add inlined function to inlinedSet
// inline any functions that are called from within this function and
// should be inlined first
//
static void
inlineFunction(FnSymbol* fn, Vec<FnSymbol*>& inlinedSet, Vec<FnSymbol*>& canRemoveRefTempSet) {
  Vec<CallExpr*> calls;

  inlinedSet.set_add(fn);

  collectFnCalls(fn, calls);

  forv_Vec(CallExpr, call, calls) {
    if (call->parentSymbol) {
      FnSymbol* fn = call->isResolved();
      if (fn->hasFlag(FLAG_INLINE)) {
        if (inlinedSet.set_in(fn))
          INT_FATAL(call, "recursive inlining detected");
        inlineFunction(fn, inlinedSet, canRemoveRefTempSet);
      }
    }
  }

  fn->collapseBlocks();

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
      inlineCall(fn, call, canRemoveRefTempSet);

      if (report_inlining)
        printf("chapel compiler: reporting inlining, %s function was inlined\n",
               fn->cname);
    }
  }
}


//
// inline all functions with the inline flag
// remove unnecessary block statements and gotos
//
void
inlineFunctions() {
  if (!fNoInline) {
    Vec<FnSymbol*> inlinedSet;
    Vec<FnSymbol*> canRemoveRefTempSet;

    compute_call_sites();

    forv_Vec(FnSymbol, fn, gFnSymbols) {
      if (canRemoveRefTemps(fn)) {
        canRemoveRefTempSet.set_add(fn);
      }
    }

    forv_Vec(FnSymbol, fn, gFnSymbols) {
      if (fn->hasFlag(FLAG_INLINE) && !inlinedSet.set_in(fn))
        inlineFunction(fn, inlinedSet, canRemoveRefTempSet);
    }
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (!fNoInline && fn->hasFlag(FLAG_INLINE) && !fn->hasFlag(FLAG_VIRTUAL)) {
      fn->defPoint->remove();
    } else {
      fn->collapseBlocks();
      removeUnnecessaryGotos(fn);
    }
  }
}
