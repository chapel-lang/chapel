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

#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "stmt.h"


//#define DEBUG_SYNC_ACCESS_FUNCTION_SET


//
// Compute set of functions that access sync variables.
//
static void
buildSyncAccessFunctionSet(Vec<FnSymbol*>& syncAccessFunctionSet) {
  Vec<FnSymbol*> syncAccessFunctionVec;

  //
  // Find all functions that directly call sync access primitives.
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (FnSymbol* parent = toFnSymbol(call->parentSymbol))
    {
      if (call->isPrimitive(PRIM_SYNC_INIT) ||
          call->isPrimitive(PRIM_SYNC_LOCK) ||
          call->isPrimitive(PRIM_SYNC_UNLOCK) ||
          call->isPrimitive(PRIM_SYNC_WAIT_FULL) ||
          call->isPrimitive(PRIM_SYNC_WAIT_EMPTY) ||
          call->isPrimitive(PRIM_SYNC_SIGNAL_FULL) ||
          call->isPrimitive(PRIM_SYNC_SIGNAL_EMPTY) ||
          call->isPrimitive(PRIM_SINGLE_INIT) ||
          call->isPrimitive(PRIM_SINGLE_LOCK) ||
          call->isPrimitive(PRIM_SINGLE_UNLOCK) ||
          call->isPrimitive(PRIM_SINGLE_WAIT_FULL) ||
          call->isPrimitive(PRIM_SINGLE_SIGNAL_FULL) ||
          call->isPrimitive(PRIM_WRITEEF) ||
          call->isPrimitive(PRIM_WRITEFF) ||
          call->isPrimitive(PRIM_WRITEXF) ||
          call->isPrimitive(PRIM_READFE) ||
          call->isPrimitive(PRIM_READFF) ||
          call->isPrimitive(PRIM_READXX) ||
          call->isPrimitive(PRIM_SYNC_IS_FULL) ||
          call->isPrimitive(PRIM_SINGLE_WRITEEF) ||
          call->isPrimitive(PRIM_SINGLE_READFF) ||
          call->isPrimitive(PRIM_SINGLE_READXX) ||
          call->isPrimitive(PRIM_SINGLE_IS_FULL)) {
        if (!parent->hasFlag(FLAG_DONT_DISABLE_REMOTE_VALUE_FORWARDING) &&
            !syncAccessFunctionSet.set_in(parent)) {
          syncAccessFunctionSet.set_add(parent);
          syncAccessFunctionVec.add(parent);
#ifdef DEBUG_SYNC_ACCESS_FUNCTION_SET
          printf("%s:%d %s\n", parent->getModule()->name, parent->linenum(), parent->name);
#endif
        }
      }
    }
  }

  //
  // Find all functions that indirectly call sync access primitives.
  //
  forv_Vec(FnSymbol, fn, syncAccessFunctionVec) {
    forv_Vec(CallExpr, caller, *fn->calledBy) {
      FnSymbol* parent = toFnSymbol(caller->parentSymbol);
      INT_ASSERT(parent);
      if (!parent->hasFlag(FLAG_DONT_DISABLE_REMOTE_VALUE_FORWARDING) &&
          !syncAccessFunctionSet.set_in(parent)) {
        syncAccessFunctionSet.set_add(parent);
        syncAccessFunctionVec.add(parent);
#ifdef DEBUG_SYNC_ACCESS_FUNCTION_SET
        printf("%s:%d %s\n", parent->getModule()->name, parent->linenum(), parent->name);
        printf("  %s:%d %s\n", fn->getModule()->name, fn->linenum(), fn->name);
#endif
      }
    }
  }
}


//
// Return true iff it is safe to dereference a reference arg.  It is
// safe to dereference iff the reference is not modified and any use
// of the reference is a simple dereference or is passed or moved to
// another reference that is safe to dereference.
//
// The argument safeSettableField is used to ignore SET_MEMBER in that
// case of testing whether a reference field can be replaced with a
// value; this handles the case where the reference field is
// reassigned to itself (probably of another instance)
//
static bool
isSafeToDeref(Symbol* ref,
              Map<Symbol*,Vec<SymExpr*>*>& defMap,
              Map<Symbol*,Vec<SymExpr*>*>& useMap,
              Vec<Symbol*>* visited,
              Symbol* safeSettableField) {
  if (!visited) {
    Vec<Symbol*> newVisited;
    return isSafeToDeref(ref, defMap, useMap, &newVisited, safeSettableField);
  }
  if (visited->set_in(ref))
    return true;
  visited->set_add(ref);

  int numDefs = (defMap.get(ref)) ? defMap.get(ref)->n : 0;
  if ((isArgSymbol(ref) && numDefs > 0) || numDefs > 1)
    return false;

  for_uses(use, useMap, ref) {
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      if (call->isResolved()) {
        ArgSymbol* arg = actual_to_formal(use);
        if (!isSafeToDeref(arg, defMap, useMap, visited, safeSettableField))
          return false;
      } else if (call->isPrimitive(PRIM_MOVE)) {
        SymExpr* newRef = toSymExpr(call->get(1));
        INT_ASSERT(newRef);
        if (!isSafeToDeref(newRef->var, defMap, useMap, visited, safeSettableField))
          return false;
      } else if (call->isPrimitive(PRIM_SET_MEMBER) && safeSettableField) {
        SymExpr* se = toSymExpr(call->get(2));
        INT_ASSERT(se);
        if (se->var != safeSettableField)
          return false;
      } else if (!call->isPrimitive(PRIM_DEREF))
        return false; // what cases does this preclude? can this be an assert?
    } else
      return false; // what cases does this preclude? can this be an assert?
  }

  return true;
}



static bool isSufficientlyConst(ArgSymbol* arg) {
  Type* argvaltype = arg->getValType();

  if (argvaltype->symbol->hasFlag(FLAG_ARRAY)) {
    // Arg is an array, so it's sufficiently constant (because this
    // refers to the descriptor, not the array's values\n");
    return true;
  }

  //
  // See if this argument is 'const in'; if it is, it's a good
  // candidate for remote value forwarding.  My current thinking is
  // that we should not forward 'const ref' arguments because the
  // const-ness only means that the callee will not modify them, not
  // that the caller won't.  If someone can successfully argue that
  // I'm being too conservative, I'm open to that.  My thinking is
  // that I'd rather find a case that we think we could be r.v.f.'ing
  // later on than to have to chase down a race condition due to
  // optimizing too aggressively.
  //
  // Why the additional check against 'ref' types?  Because some
  // compiler-created arguments currently indicate ref-ness only via
  // the type and not the intent.  See the big comment I added in
  // addVarsToFormals() (flattenFunctions.cpp) in this same commit for
  // an example.  A case that currently fails without this test is:
  //
  //     test/multilocale/bradc/needMultiLocales/remoteReal.chpl
  //
  if (arg->intent == INTENT_CONST_IN  &&
      !arg->type->symbol->hasFlag(FLAG_REF)) {
    return true;
  }


  // We may want to add additional cases here as we discover them

  // otherwise, conservatively assume it varies
  return false;
}


//
// Convert reference args into values if they are only read and
// reading them early does not violate program semantics.
//
// pre-condition: the call graph is computed
//
void
remoteValueForwarding(Vec<FnSymbol*>& fns) {
  if (fNoRemoteValueForwarding)
    return;

  Vec<FnSymbol*> syncAccessFunctionSet;
  buildSyncAccessFunctionSet(syncAccessFunctionSet);

  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(defMap, useMap);

  //
  // change reference type fields in loop body argument classes
  // (created when transforming recursive leader iterators into
  // recursive functions) to value type fields if safe
  //
  forv_Vec(AggregateType, ct, gAggregateTypes) {
    if (ct->symbol->hasFlag(FLAG_LOOP_BODY_ARGUMENT_CLASS)) {
      for_fields(field, ct) {
        if (field->type->symbol->hasFlag(FLAG_REF)) {
          INT_ASSERT(!defMap.get(field));
          bool safeToDeref = true;
          for_uses(use, useMap, field) {
            CallExpr* call = toCallExpr(use->parentExpr);
            INT_ASSERT(call);
            if (call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
              CallExpr* move = toCallExpr(call->parentExpr);
              INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
              SymExpr* lhs = toSymExpr(move->get(1));
              INT_ASSERT(lhs);
              if (!isSafeToDeref(lhs->var, defMap, useMap, NULL, field)) {
                safeToDeref = false;
                break;
              }
            } else if (!call->isPrimitive(PRIM_SET_MEMBER))
              INT_FATAL(field, "unexpected case");
          }
          if (safeToDeref) {
            Type* vt = field->getValType();
            for_uses(use, useMap, field) {
              CallExpr* call = toCallExpr(use->parentExpr);
              INT_ASSERT(call);
              SET_LINENO(call);
              if (call->isPrimitive(PRIM_SET_MEMBER)) {
                Symbol* tmp = newTemp("derefTmp", vt);
                call->insertBefore(new DefExpr(tmp));
                call->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_DEREF, call->get(3)->remove())));
                call->insertAtTail(tmp);
              } else if (call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
                CallExpr* move = toCallExpr(call->parentExpr);
                INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
                Symbol* tmp = newTemp(vt);
                move->insertBefore(new DefExpr(tmp));
                move->insertBefore(new CallExpr(PRIM_MOVE, tmp, call->remove()));
                move->insertAtTail(new CallExpr(PRIM_ADDR_OF, tmp));
              } else
                INT_FATAL(field, "unexpected case");
            }
            field->type = vt;
          }
        }
      }
    }
  }

  forv_Vec(FnSymbol, fn, fns) {

    //
    // For each reference arg that is safe to dereference
    //
    for_formals(arg, fn) {

      /* if this function accesses sync vars and the argument is not
         const, then we cannot remote value forward the argument due
         to the fence implied by the sync var accesses */
      if (syncAccessFunctionSet.set_in(fn) && !isSufficientlyConst(arg)) {
        continue;
      }

      // If this argument is a reference atomic type, we need to preserve
      // reference semantics, i.e. that the referenced atomic gets updated.
      // Therefore, dereferencing a ref atomic and forwarding its value is not
      // what we want.  That is, all atomics implicitly disable remote value
      // forwarding.
      // See resolveFormals() [functionResolution.cpp:839] for where we decide
      // to convert atomic formals to ref formals.
      if (isAtomicType(arg->type))
        continue;

      if (arg->type->symbol->hasFlag(FLAG_REF) &&
          isSafeToDeref(arg, defMap, useMap, NULL, NULL)) {

       // Dereference the arg type.
       Type* prevArgType = arg->type;
       arg->type = arg->getValType();

       forv_Vec(CallExpr, call, *fn->calledBy) {
        //
        // Find actual for arg.
        //
        SymExpr* actual = toSymExpr(formal_to_actual(call, arg));
        INT_ASSERT(actual && actual->var->type == prevArgType);
        SET_LINENO(actual);
        
        //
        // Insert de-reference temp of value.
        //
        VarSymbol* deref = newTemp("RVFderefTmp", arg->type);
        call->insertBefore(new DefExpr(deref));
        call->insertBefore(new CallExpr(PRIM_MOVE, deref,
                                        new CallExpr(PRIM_DEREF, actual->var)));
        actual->replace(new SymExpr(deref));
       }  // for call
        
        //
        // Insert re-reference temps at use points.
        //
        for_uses(use, useMap, arg) {
          SET_LINENO(use);
          CallExpr* call = toCallExpr(use->parentExpr);
          if (call && call->isPrimitive(PRIM_DEREF)) {
            call->replace(new SymExpr(arg));
          } else if (call && call->isPrimitive(PRIM_MOVE)) {
            use->replace(new CallExpr(PRIM_ADDR_OF, arg));
          } else {
            Expr* stmt = use->getStmtExpr();
            VarSymbol* reref = newTemp("RVFrerefTmp", prevArgType);
            stmt->insertBefore(new DefExpr(reref));
            stmt->insertBefore(new CallExpr(PRIM_MOVE, reref,
                                            new CallExpr(PRIM_ADDR_OF, arg)));
            use->replace(new SymExpr(reref));
          }
        }
      }
    }
  }

  freeDefUseMaps(defMap, useMap);
}
