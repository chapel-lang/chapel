#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "stmt.h"

//
// compute set of functions that access sync variables
//
static void
computeSyncAccessFunctionSet(Vec<FnSymbol*>& syncAccessFunctionSet) {
  Vec<FnSymbol*> syncAccessFunctionVec;

  //
  // first- look for sync access primitives
  //
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->parentSymbol) {
      if (call->isPrimitive(PRIMITIVE_SYNC_INIT) ||
          call->isPrimitive(PRIMITIVE_SYNC_DESTROY) ||
          call->isPrimitive(PRIMITIVE_SYNC_LOCK) ||
          call->isPrimitive(PRIMITIVE_SYNC_UNLOCK) ||
          call->isPrimitive(PRIMITIVE_SYNC_WAIT_FULL) ||
          call->isPrimitive(PRIMITIVE_SYNC_WAIT_EMPTY) ||
          call->isPrimitive(PRIMITIVE_SYNC_SIGNAL_FULL) ||
          call->isPrimitive(PRIMITIVE_SYNC_SIGNAL_EMPTY) ||
          call->isPrimitive(PRIMITIVE_SINGLE_INIT) ||
          call->isPrimitive(PRIMITIVE_SINGLE_LOCK) ||
          call->isPrimitive(PRIMITIVE_SINGLE_UNLOCK) ||
          call->isPrimitive(PRIMITIVE_SINGLE_WAIT_FULL) ||
          call->isPrimitive(PRIMITIVE_SINGLE_SIGNAL_FULL) ||
          call->isPrimitive(PRIMITIVE_WRITEEF) ||
          call->isPrimitive(PRIMITIVE_WRITEFF) ||
          call->isPrimitive(PRIMITIVE_WRITEXF) ||
          call->isPrimitive(PRIMITIVE_SYNC_RESET) ||
          call->isPrimitive(PRIMITIVE_READFE) ||
          call->isPrimitive(PRIMITIVE_READFF) ||
          call->isPrimitive(PRIMITIVE_READXX) ||
          call->isPrimitive(PRIMITIVE_SYNC_ISFULL) ||
          call->isPrimitive(PRIMITIVE_SINGLE_WRITEEF) ||
          call->isPrimitive(PRIMITIVE_SINGLE_RESET) ||
          call->isPrimitive(PRIMITIVE_SINGLE_READFF) ||
          call->isPrimitive(PRIMITIVE_SINGLE_READXX) ||
          call->isPrimitive(PRIMITIVE_SINGLE_ISFULL)) {
        FnSymbol* parent = toFnSymbol(call->parentSymbol);
        INT_ASSERT(parent);
        if (!syncAccessFunctionSet.set_in(parent)) {
          syncAccessFunctionSet.set_add(parent);
          syncAccessFunctionVec.add(parent);
        }
      }
    }
  }

  //
  // third- traverse the call graph to find all functions that call
  // sync access primitives through another function call
  //
  forv_Vec(FnSymbol, fn, syncAccessFunctionVec) {
    forv_Vec(CallExpr, caller, *fn->calledBy) {
      FnSymbol* parent = toFnSymbol(caller->parentSymbol);
      INT_ASSERT(parent);
      if (!syncAccessFunctionSet.set_in(parent)) {
        syncAccessFunctionSet.set_add(parent);
        syncAccessFunctionVec.add(parent);
      }
    }
  }
}


static bool
isSafeToDeref(Symbol* startSym,
              Symbol* ref,
              FnSymbol* fn,
              Map<Symbol*,Vec<SymExpr*>*>& defMap,
              Map<Symbol*,Vec<SymExpr*>*>& useMap,
              Vec<FnSymbol*>* visited) {
  if (!visited) {
    Vec<FnSymbol*> newVisited;
    return isSafeToDeref(startSym, ref, fn, defMap, useMap, &newVisited);
  }
  if (visited->set_in(fn))
    return true;
  visited->set_add(fn);

  if (defMap.get(ref) && defMap.get(ref)->n > 0) {
    if (startSym == ref || defMap.get(ref)->n != 1) {
      return false;
    }
  }

  for_uses(use, useMap, ref) {
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      if (FnSymbol* newfn = call->isResolved()) {
        ArgSymbol* formal = actual_to_formal(use);
        if (isSafeToDeref(startSym, formal, newfn, defMap, useMap, visited)) {
          continue;
        } else {
          return false;
        }
      } else {
        if (call->isPrimitive(PRIMITIVE_GET_REF)) {
          continue;
        }
        if (call->isPrimitive(PRIMITIVE_MOVE)) {
          SymExpr* newRef = toSymExpr(call->get(1));
          INT_ASSERT(newRef);
          Vec<FnSymbol*> newVisited;
          if (isSafeToDeref(startSym, newRef->var, fn, defMap, useMap, &newVisited)) {
            continue;
          } else {
            return false;
          }
        }
      }  
 
    }
  }
  return true;
}

static void
passReadOnlyReferencesByValue(CallExpr* call, FnSymbol* fn) {
  Vec<ArgSymbol*> args;
  for_formals(arg, fn) {
    if (arg->typeInfo()->symbol->hasFlag(FLAG_REF))
      if (!toClassType(arg->type->getValueType()))
        args.add(arg);
  }

  if (!args.n)
    return;

  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(fn, defMap, useMap);

  forv_Vec(ArgSymbol, arg, args) {
    if (!defMap.get(arg) || defMap.get(arg)->n == 0) {
      if (isSafeToDeref(arg, arg, fn, defMap, useMap, NULL)) {
        arg->type = arg->type->getValueType();
        Expr* actual = formal_to_actual(call, arg);
        INT_ASSERT(actual);
        if (actual->typeInfo()->symbol->hasFlag(FLAG_REF)) {
          VarSymbol* var = newTemp("deref", arg->type);
          call->insertBefore(new DefExpr(var));
          call->insertBefore(new CallExpr(PRIMITIVE_MOVE, var, new CallExpr(PRIMITIVE_GET_REF, actual->copy())));
          actual->replace(new SymExpr(var));
        }

        for_uses(use, useMap, arg) {
          VarSymbol* ref = newTemp("ref", arg->typeInfo()->getReferenceType());
          use->getStmtExpr()->insertBefore(new DefExpr(ref));
          use->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, ref, new CallExpr(PRIMITIVE_SET_REF, arg)));
          use->replace(new SymExpr(ref));
        }
      }
    }
  }

  freeDefUseMaps(defMap, useMap);
}


//
// Convert reference arguments into values if they are only read and
// reading them early does not violate program semantics.
//
// pre-condition: the call graph is computed
//
void
optimizeReadOnlyReferenceArguments(Vec<FnSymbol*>& fns) {
  Vec<FnSymbol*> syncAccessFunctionSet;
  computeSyncAccessFunctionSet(syncAccessFunctionSet);

  forv_Vec(FnSymbol, fn, fns) {
    if (!syncAccessFunctionSet.set_in(fn)) {
      forv_Vec(CallExpr, call, *fn->calledBy) {
        passReadOnlyReferencesByValue(call, fn);
      }
    }
  }
}
