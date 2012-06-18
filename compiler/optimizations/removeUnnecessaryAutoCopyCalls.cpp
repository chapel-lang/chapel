#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "stmt.h"

//
// Remove autoCopy (and matching autoDestroy calls) that are
// unnecessary within a function
//
static void removeUnnecessaryAutoCopyCalls(FnSymbol* fn) {
  buildBasicBlocks(fn);
  for_vector(BasicBlock, bb, *fn->basicBlocks) {
    Map<Symbol*,Symbol*> equivalenceMap;
    Map<Symbol*,CallExpr*> autoCopyMap;
    for_vector(Expr, expr, bb->exprs) {
      if (CallExpr* call = toCallExpr(expr)) {
        if (call->isPrimitive(PRIM_MOVE)) {
          SymExpr* lhs = toSymExpr(call->get(1));
          INT_ASSERT(lhs);
          if (SymExpr* rhs = toSymExpr(call->get(2))) {
            if (Symbol* equiv = equivalenceMap.get(rhs->var))
              equivalenceMap.put(equiv, NULL);
            equivalenceMap.put(rhs->var, lhs->var);
            equivalenceMap.put(lhs->var, rhs->var);
            continue;
          } else if (CallExpr* rhs = toCallExpr(call->get(2))) {
            if (FnSymbol* ac = rhs->isResolved()) {
              if (ac->hasFlag(FLAG_REMOVABLE_AUTO_COPY) && rhs->argList.head) {
                SymExpr* se = toSymExpr(rhs->get(1));
                INT_ASSERT(se);
                autoCopyMap.put(se->var, rhs);
                continue;
              }
            }
          }
        } else if (FnSymbol* ad = call->isResolved()) {
          if (ad->hasFlag(FLAG_REMOVABLE_AUTO_DESTROY) && call->argList.head) {
            SymExpr* se = toSymExpr(call->get(1));
            INT_ASSERT(se);
            if (se->var->defPoint->parentSymbol == fn) {
              if (CallExpr* autoCopyCall = autoCopyMap.get(se->var)) {
                autoCopyCall->replace(autoCopyCall->get(1)->remove());
                call->replace(call->get(1)->remove());
                continue;
              } else if (Symbol* equiv = equivalenceMap.get(se->var)) {
                if (CallExpr* autoCopyCall = autoCopyMap.get(equiv)) {
                  autoCopyCall->replace(autoCopyCall->get(1)->remove());
                  call->replace(call->get(1)->remove());
                  continue;
                }
              }
            }
          }
        }
      }

      //
      // invalidate maps
      //
      Vec<SymExpr*> symExprs;
      collectSymExprs(expr, symExprs);
      forv_Vec(SymExpr, se, symExprs) {
        if (autoCopyMap.get(se->var))
          autoCopyMap.put(se->var, NULL);
        if (Symbol* equiv = equivalenceMap.get(se->var)) {
          equivalenceMap.put(equiv, NULL);
          equivalenceMap.put(se->var, NULL);
        }
      }
    }
  }
}

static bool
isPrimitiveInitCopy(Vec<Type*>& primitiveInitCopyTypeSet, Type* type) {
  if (primitiveInitCopyTypeSet.set_in(type))
    return true;
  if (is_bool_type(type) || is_int_type(type) || is_uint_type(type) ||
      is_real_type(type) || is_imag_type(type) || is_complex_type(type) ||
      is_enum_type(type) ||
      (isClass(type) && !isSyncType(type)))
    return true;
  if (isRecordWrappedType(type) ||
      isSyncType(type) ||
      type->symbol->hasFlag(FLAG_REF) ||
      type->symbol->hasFlag(FLAG_ITERATOR_RECORD))
    return false;
  if (isRecord(type)) {
    ClassType* ct = toClassType(type);
    INT_ASSERT(ct);
    for_fields(field, ct) {
      if (!isPrimitiveInitCopy(primitiveInitCopyTypeSet, field->type))
        return false;
    }
    return true;
  }
  return false;
}

void removeUnnecessaryAutoCopyCalls() {
  if (fNoRemoveCopyCalls)
    return;

  //
  // remove pointless initCopy calls, e.g., initCopy calls on records of
  // primitive types
  //
  compute_call_sites();

  Vec<Type*> primitiveInitCopyTypeSet;
  
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (!strcmp(fn->name, "chpl__initCopy") &&
        fn->numFormals() == 1 &&
        fn->getFormal(1)->type == fn->retType) {
      if (isPrimitiveInitCopy(primitiveInitCopyTypeSet, fn->retType)) {
        forv_Vec(CallExpr, call, *fn->calledBy) {
          call->replace(call->get(1)->remove());
        }
      }
    }
  }

  //
  // remove matched pairs of autoCopy and autoDestroy calls marked with the
  // removable auto destroy pragma
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    removeUnnecessaryAutoCopyCalls(fn);
  }
}
