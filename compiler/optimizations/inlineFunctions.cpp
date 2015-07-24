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
#include <set>

static void calculateFormalActualMap(SymbolMap& map, std::set<Symbol*>& noDR,
                                     FnSymbol* fn, CallExpr* call,
                                     Vec<FnSymbol*>& canRemoveRefTempSet);
static void removeUnnecessaryDerefs(std::set<Symbol*>& noDR, BlockStmt* block);


//
// inlines the function called by 'call' at that call site
//
static void
inlineCall(FnSymbol* fn, CallExpr* call, Vec<FnSymbol*>& canRemoveRefTempSet) {
  INT_ASSERT(call->isResolved() == fn);
  SET_LINENO(call);

  Expr* stmt = call->getStmtExpr();

  // calculate a map from actual symbols to formal symbols
  SymbolMap map;
  std::set<Symbol*> noDerefs;
  calculateFormalActualMap(map, noDerefs, fn, call, canRemoveRefTempSet);

  // copy function body
  BlockStmt* block = fn->body->copy(&map);
  if (!preserveInlinedLineNumbers)
    reset_ast_loc(block, call);
  if (!noDerefs.empty())
    removeUnnecessaryDerefs(noDerefs, block);

  // update the return
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

  // replace the call site
  stmt->insertBefore(block);
  if (fn->retType == dtVoid)
    stmt->remove();
  else
    call->replace(return_value);
}

#if 0
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
  collectCallExprs(fn, callExprs);

  for_vector(CallExpr, call, callExprs) {
    if (!call->primitive) {
      return false;
    } else if (call->isPrimitive(PRIM_SET_MEMBER)) {
      return false;
    }
  }

  return true;
}
#endif

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
// Build the map formal->actual.
//
// For formals with ref intents, remove ref temp actuals when applicable
// and use the symbol that the ref temp references.
//
// While there, compute which actuals we should remove derefs for.
//
static void calculateFormalActualMap(SymbolMap& map, std::set<Symbol*>& noDR,
                                     FnSymbol* fn, CallExpr* call,
                                     Vec<FnSymbol*>& canRemoveRefTempSet)
{
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

  // One may be tempted to compute 'noDR' in removeUnnecessaryDerefs().
  // However, by that time 'map' may contain additional symbols, like labels
  // and ArgSymbols from other functions, complicating computation of noDR.
  // So we do it here while it is simple.
  //
  form_Map(SymbolMapElem, el, map) {
    Symbol* formal = el->key;
    Symbol* actual = el->value;
    if (isReferenceType(formal->type) && formal->type != actual->type) {
      INT_ASSERT(formal->type->getValType() == actual->type);
      noDR.insert(actual);
    }
  }
}

static int breakOnRM = 0; //vass

//
// Remove 'deref' prim calls for the symbols in 'noDR'.
// Those symbols are e.g. where a ref temp actual was replaced
// with its referencee in calculateFormalActualMap().
//
static void removeUnnecessaryDerefs(std::set<Symbol*>& noDR, BlockStmt* block)
{
  if (block->id == breakOnRM)
    gdbShouldBreakHere(); //vass
  int cnt = 0; //vass
  std::vector<CallExpr*> allCalls;
  collectCallExprs(block, allCalls);
  for_vector(CallExpr, call, allCalls) {
    if (call->isPrimitive(PRIM_DEREF)) {
      SymExpr* se = toSymExpr(call->get(1));
      Symbol* arg = se->var;
      if (noDR.count(arg))
      { cnt++; //vass
        call->replace(new SymExpr(arg));
      }
    }
  }
  if (block->id == breakOnRM) {
    printf("cnt %d   block %d\n", cnt, block->id); //vass
    gdbShouldBreakHere(); //vass
  }
}


//
// inline function fn at all call sites
// add inlined function to inlinedSet
// inline any functions that are called from within this function and
// should be inlined first
//
static void
inlineFunction(FnSymbol* fn, Vec<FnSymbol*>& inlinedSet, Vec<FnSymbol*>& canRemoveRefTempSet) {
  std::vector<CallExpr*> calls;

  inlinedSet.set_add(fn);

  collectFnCalls(fn, calls);

  for_vector(CallExpr, call, calls) {
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
#if 0
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      if (canRemoveRefTemps(fn)) {
        canRemoveRefTempSet.set_add(fn);
      }
    }
#endif
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
