/*
 * Copyright 2004-2016 Cray Inc.
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

#include "passes.h"

#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"

#include <set>
#include <vector>

//
// inlines the function called by 'call' at that call site
//
static void inlineCall(FnSymbol* fn, CallExpr* call) {
  INT_ASSERT(call->resolvedFunction() == fn);

  SET_LINENO(call);

  Expr* stmt = call->getStmtExpr();

  //
  // calculate a map from actual symbols to formal symbols
  //
  SymbolMap map;

  for_formals_actuals(formal, actual, call) {
    SymExpr* se = toSymExpr(actual);

    INT_ASSERT(se);

    if ((formal->intent & INTENT_REF) != 0     &&
        isReferenceType(formal->type) == false &&
        formal->type->getRefType()    == actual->typeInfo()) {
      Expr*      point = call->getStmtExpr();
      VarSymbol* tmp   = newTemp(astr("i_", formal->name), formal->type);
      DefExpr*   def   = new DefExpr(tmp);
      CallExpr*  move  = new CallExpr(PRIM_MOVE,
                                      tmp,
                                      new CallExpr(PRIM_SET_REFERENCE,
                                                   se->symbol()));
      tmp->qual = QUAL_REF;

      point->insertBefore(def);
      point->insertBefore(move);

      map.put(formal, tmp);
    } else {
      map.put(formal, se->symbol());
    }
  }

  //
  // copy function body, inline it at call site, and update return
  //
  BlockStmt* block = fn->body->copy(&map);

  if (!preserveInlinedLineNumbers)
    reset_ast_loc(block, call);

  CallExpr* returnStmt = toCallExpr(block->body.last());

  if (returnStmt == NULL || !returnStmt->isPrimitive(PRIM_RETURN))
    INT_FATAL(call, "function is not normalized");

  Expr*    returnValue = returnStmt->get(1);
  SymExpr* se          = toSymExpr(returnValue);

  // Ensure that the inlined function body does not attempt to return one of
  // the original function's formals.  This is equivalent to saying that if the
  // returned value is originally one of the formal argument symbols, that
  // symbol was replaced by it actual argument in the call to copy(&map) above.
  for_formals(formal, fn) {
    INT_ASSERT(formal != toArgSymbol(se->symbol()));
  }

  returnStmt->remove();
  returnValue->remove();

  stmt->insertBefore(block);

  if (fn->retType == dtVoid) {
    stmt->remove();
  } else {
    call->replace(returnValue);
  }
}

//
// inline function fn at all call sites
// add inlined function to inlinedSet
// inline any functions that are called from within this function and
// should be inlined first
//
static void inlineFunction(FnSymbol* fn, std::set<FnSymbol*>& inlinedSet) {
  std::vector<CallExpr*> calls;

  inlinedSet.insert(fn);

  collectFnCalls(fn, calls);

  for_vector(CallExpr, call, calls) {
    if (call->parentSymbol) {
      FnSymbol* fn = call->resolvedFunction();

      if (fn->hasFlag(FLAG_INLINE)) {
        if (inlinedSet.find(fn) != inlinedSet.end()) {
          INT_FATAL(call, "recursive inlining detected");
        }

        inlineFunction(fn, inlinedSet);
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
      inlineCall(fn, call);

      if (report_inlining) {
        printf("chapel compiler: reporting inlining, "
               "%s function was inlined\n",
               fn->cname);
      }
    }
  }
}


//
// inline all functions with the inline flag
// remove unnecessary block statements and gotos
//
void inlineFunctions() {
  if (!fNoInline) {
    std::set<FnSymbol*> inlinedSet;

    compute_call_sites();

    forv_Vec(FnSymbol, fn, gFnSymbols) {
      if (fn->hasFlag(FLAG_INLINE) == true &&
          inlinedSet.find(fn)      == inlinedSet.end()) {
        inlineFunction(fn, inlinedSet);
      }
    }

    forv_Vec(SymExpr, se, gSymExprs) {
      CallExpr* def = toCallExpr(se->parentExpr);

      if (def && def->isPrimitive(PRIM_DEREF)) {
        CallExpr* move = toCallExpr(def->parentExpr);

        INT_ASSERT(isMoveOrAssign(move));

        if (!se->isRef()) {
          SET_LINENO(se);
          def->replace(se->copy());
        }
      }
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
