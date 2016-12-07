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

static void updateRefCalls();
static void inlineFunctionsImpl();
static void inlineFunction(FnSymbol* fn, std::set<FnSymbol*>& inlinedSet);
static void inlineCall(FnSymbol* fn, CallExpr* call);
static void updateDerefCalls();
static void inlineCleanup();

/************************************* | **************************************
*                                                                             *
* inline all functions with the inline flag                                   *
* remove unnecessary block statements and gotos                               *
*                                                                             *
************************************** | *************************************/

void inlineFunctions() {
  compute_call_sites();

  updateRefCalls();

  inlineFunctionsImpl();

  updateDerefCalls();

  inlineCleanup();
}

/************************************* | **************************************
*                                                                             *
* inline all functions with the inline flag                                   *
*                                                                             *
************************************** | *************************************/

static void inlineFunctionsImpl() {
  if (fNoInline == false) {
    std::set<FnSymbol*> inlinedSet;

    forv_Vec(FnSymbol, fn, gFnSymbols) {
      if (fn->hasFlag(FLAG_INLINE) == true &&
          inlinedSet.find(fn)      == inlinedSet.end()) {
        inlineFunction(fn, inlinedSet);
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Inline a function at every call site                                        *
*                                                                             *
************************************** | *************************************/

static void markFunction(FnSymbol* fn, std::set<FnSymbol*>& inlinedSet);
static void inlineBody(FnSymbol* fn, std::set<FnSymbol*>& inlinedSet);
static void simplifyBody(FnSymbol* fn);
static void inlineAtCallSites(FnSymbol* fn);

static void inlineFunction(FnSymbol* fn, std::set<FnSymbol*>& inlinedSet) {
  markFunction(fn, inlinedSet);

  inlineBody(fn, inlinedSet);

  simplifyBody(fn);

  inlineAtCallSites(fn);
}

static void markFunction(FnSymbol* fn, std::set<FnSymbol*>& inlinedSet) {
  inlinedSet.insert(fn);
}

//
// The Chapel specification requires that inlining be performed for any
// procedure with this linkage specifier.  It is a preventable user error
// if a recursive cycle is detected (directly or indirectly)
//
static void inlineBody(FnSymbol* fn, std::set<FnSymbol*>& inlinedSet) {
  std::vector<CallExpr*> calls;

  collectFnCalls(fn, calls);

  for_vector(CallExpr, call, calls) {
    if (call->parentSymbol) {
      FnSymbol* calledFn = call->resolvedFunction();

      if (calledFn->hasFlag(FLAG_INLINE)) {
        if (inlinedSet.find(calledFn) != inlinedSet.end()) {
          if (calledFn == fn) {
            USR_FATAL(call,
                      "Attempt to inline a function, '%s', that calls itself",
                      fn->name);
          } else {
            USR_FATAL(call,
                      "Detected a cycle back to inlined function '%s' while "
                      "inlining '%s'",
                      calledFn->name,
                      fn->name);
          }
        }

        inlineFunction(calledFn, inlinedSet);
      }
    }
  }
}

static void simplifyBody(FnSymbol* fn) {
  fn->collapseBlocks();

  removeUnnecessaryGotos(fn);

#if DEBUG_CP < 2    // That is, disabled if DEBUG_CP >= 2
  if (fNoCopyPropagation == false) {
    singleAssignmentRefPropagation(fn);
    localCopyPropagation(fn);
  }
#endif

  if (fNoDeadCodeElimination == false) {
    deadVariableElimination(fn);
    deadExpressionElimination(fn);
  }
}

static void inlineAtCallSites(FnSymbol* fn) {
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

/************************************* | **************************************
*                                                                             *
* inlines the function called by 'call' at that call site                     *
*                                                                             *
************************************** | *************************************/

static void inlineCall(FnSymbol* fn, CallExpr* call) {
  INT_ASSERT(call->resolvedFunction() == fn);

  SET_LINENO(call);

  Expr* stmt = call->getStmtExpr();

  //
  // calculate a map from actual symbols to formal symbols
  //
  SymbolMap map;

  for_formals_actuals(formal, actual, call) {
    map.put(formal, toSymExpr(actual)->symbol());
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

/************************************* | **************************************
*                                                                             *
* This is transition code to support the ref cleanup work                     *
*                                                                             *
************************************** | *************************************/

static void updateDerefCalls() {
  if (fNoInline == false) {
    forv_Vec(SymExpr, se, gSymExprs) {
      CallExpr* def = toCallExpr(se->parentExpr);

      if (def && def->isPrimitive(PRIM_DEREF)) {
        CallExpr* move = toCallExpr(def->parentExpr);

        INT_ASSERT(isMoveOrAssign(move));

        if (se->isRef() == false) {
          SET_LINENO(se);

          def->replace(se->copy());
        }
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Either remove the dead inlined calls or do some miscellaneous cleanup.      *
*                                                                             *
************************************** | *************************************/

static void inlineCleanup() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fNoInline                 == false &&
        fn->hasFlag(FLAG_INLINE)  ==  true &&
        fn->hasFlag(FLAG_VIRTUAL) == false) {
      fn->defPoint->remove();
    } else {
      fn->collapseBlocks();
      removeUnnecessaryGotos(fn);
    }
  }
}

/************************************* | **************************************
*                                                                             *
* NOAKES 2016/11/17                                                           *
*                                                                             *
* The following is transition logic while ref intents are being cleaned up.   *
*                                                                             *
* It was being applied in the inner loop of inlineFunctions but               *
*   1) That made inlineFunctions() a little more convoluted                   *
*   2) This transformation could/should be applied more generally             *
*                                                                             *
* There are now many functions that have at least one formal with a ref       *
* intent and, correctly, a non-ref type e.g. a Record rather than a           *
* Class _ref(Record).  However there are still call sites that continue to    *
* pass a temp that is Class _ref(t).                                          *
*                                                                             *
* The longer term path is to eliminate the creation of those ref types but in *
* the short term this inserts another tmp var that has the correct type and   *
* then uses a relatively new PRIMOP to fix up the type information.           *
*                                                                             *
* NB: This function assumes that compute_call_sites() has been called.        *
*                                                                             *
************************************** | *************************************/

static bool hasFormalWithRefIntent(FnSymbol* fn);

static void updateRefCalls() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (hasFormalWithRefIntent(fn) == true) {

      // Walk all of the call-sites and check types of actuals vs. formals
      forv_Vec(CallExpr, call, *fn->calledBy) {
        if (call->isResolved()) {
          Expr* stmt = call->getStmtExpr();

          SET_LINENO(call);

          for_formals_actuals(formal, actual, call) {
            SymExpr* se = toSymExpr(actual);

            // Is this a case in which we are Passing an actual that is
            // a ref(t) to a formal with type t and intent ref?
            //
            // If so modify the call site and
            //
            //   a) Introduce a tmp with qualified type ref t
            //   b) Pass that tmp instead
            if ((formal->intent & INTENT_REF) != 0     &&
                isReferenceType(formal->type) == false &&
                formal->type->getRefType()    == actual->typeInfo()) {

              // Introduce a ref temp
              VarSymbol* tmp  = newTemp(astr("i_", formal->name),
                                        formal->type);
              DefExpr*   def  = new DefExpr(tmp);
              CallExpr*  move = NULL;

              tmp->qual = QUAL_REF;
              move      = new CallExpr(PRIM_MOVE,
                                       tmp,
                                       new CallExpr(PRIM_SET_REFERENCE,
                                                    se->symbol()));

              stmt->insertBefore(def);
              stmt->insertBefore(move);

              // Replace the actual with the ref-temp
              actual->replace(new SymExpr(tmp));
            }
          }
        }
      }
    }
  }
}

static bool hasFormalWithRefIntent(FnSymbol* fn) {
  for_formals(formal, fn) {
    if ((formal->intent & INTENT_REF) != 0     &&
        isReferenceType(formal->type) == false) {
      return true;
    }
  }

  return false;
}
