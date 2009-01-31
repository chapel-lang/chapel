#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "stmt.h"


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
    if (call->parentSymbol) {
      if (call->isPrimitive(PRIM_SYNC_INIT) ||
          call->isPrimitive(PRIM_SYNC_DESTROY) ||
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
          call->isPrimitive(PRIM_SYNC_RESET) ||
          call->isPrimitive(PRIM_READFE) ||
          call->isPrimitive(PRIM_READFF) ||
          call->isPrimitive(PRIM_READXX) ||
          call->isPrimitive(PRIM_SYNC_ISFULL) ||
          call->isPrimitive(PRIM_SINGLE_WRITEEF) ||
          call->isPrimitive(PRIM_SINGLE_RESET) ||
          call->isPrimitive(PRIM_SINGLE_READFF) ||
          call->isPrimitive(PRIM_SINGLE_READXX) ||
          call->isPrimitive(PRIM_SINGLE_ISFULL)) {
        FnSymbol* parent = toFnSymbol(call->parentSymbol);
        INT_ASSERT(parent);
        if (!parent->hasFlag(FLAG_DONT_DISABLE_REMOTE_VALUE_FORWARDING) &&
            !syncAccessFunctionSet.set_in(parent)) {
          syncAccessFunctionSet.set_add(parent);
          syncAccessFunctionVec.add(parent);
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
static bool
isSafeToDeref(Symbol* ref,
              Map<Symbol*,Vec<SymExpr*>*>& defMap,
              Map<Symbol*,Vec<SymExpr*>*>& useMap,
              Vec<Symbol*>* visited) {
  if (!visited) {
    Vec<Symbol*> newVisited;
    return isSafeToDeref(ref, defMap, useMap, &newVisited);
  }
  if (visited->set_in(ref))
    return true;
  visited->set_add(ref);

  int numDefs = (defMap.get(ref)) ? defMap.get(ref)->n : 0;
  if (isArgSymbol(ref) && numDefs > 0 || numDefs > 1)
    return false;

  for_uses(use, useMap, ref) {
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      if (call->isResolved()) {
        ArgSymbol* arg = actual_to_formal(use);
        if (!isSafeToDeref(arg, defMap, useMap, visited))
          return false;
      } else if (call->isPrimitive(PRIM_MOVE)) {
        SymExpr* newRef = toSymExpr(call->get(1));
        INT_ASSERT(newRef);
        if (!isSafeToDeref(newRef->var, defMap, useMap, visited))
          return false;
      } else if (!call->isPrimitive(PRIM_GET_REF))
        return false;
    } else
      return false;
  }

  return true;
}


//
// Convert reference args into values if they are only read and
// reading them early does not violate program semantics.
//
// pre-condition: the call graph is computed
//
void
remoteValueForwarding(Vec<FnSymbol*>& fns) {
  Vec<FnSymbol*> syncAccessFunctionSet;
  buildSyncAccessFunctionSet(syncAccessFunctionSet);

  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(defMap, useMap);

  forv_Vec(FnSymbol, fn, fns) {
    if (!syncAccessFunctionSet.set_in(fn)) {
      INT_ASSERT(fn->calledBy->n == 1);
      CallExpr* call = fn->calledBy->v[0];

      //
      // For each reference arg of simple type that is safe to dereference
      //
      for_formals(arg, fn) {
        if (arg->type->symbol->hasFlag(FLAG_REF) &&
            !isClassType(arg->type->getValueType()) &&
            isSafeToDeref(arg, defMap, useMap, NULL)) {

          //
          // Find actual for arg and dereference arg type.
          //
          SymExpr* actual = toSymExpr(formal_to_actual(call, arg));
          INT_ASSERT(actual && actual->var->type == arg->type);
          arg->type = arg->type->getValueType();

          //
          // Insert de-reference temp of value.
          //
          VarSymbol* deref = newTemp("rvfDerefTmp", arg->type);
          call->insertBefore(new DefExpr(deref));
          call->insertBefore(new CallExpr(PRIM_MOVE, deref,
                               new CallExpr(PRIM_GET_REF, actual->var)));
          actual->replace(new SymExpr(deref));

          //
          // Insert re-reference temps at use points.
          //
          for_uses(use, useMap, arg) {
            Expr* stmt = use->getStmtExpr();
            VarSymbol* reref = newTemp("rvfRerefTmp", actual->var->type);
            stmt->insertBefore(new DefExpr(reref));
            stmt->insertBefore(new CallExpr(PRIM_MOVE, reref,
                                 new CallExpr(PRIM_SET_REF, arg)));
            use->replace(new SymExpr(reref));
          }
        }
      }
    }
  }

  freeDefUseMaps(defMap, useMap);
}
