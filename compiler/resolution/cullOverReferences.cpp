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
#include "passes.h"
#include "resolution.h"
#include "stmt.h"
#include "symbol.h"

static bool
refNecessary(SymExpr* se,
             Map<Symbol*,Vec<SymExpr*>*>& defMap,
             Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  Vec<SymExpr*>* defs = defMap.get(se->var);
  if (defs && defs->n > 1)
    return true;
  for_uses(use, useMap, se->var) {
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      if (FnSymbol* fn = call->isResolved()) {
        ArgSymbol* formal = actual_to_formal(use);
        if (formal->defPoint->getFunction()->_this == formal)
          return true;
        if (formal->intent == INTENT_INOUT || formal->intent == INTENT_OUT)
          return true;
        if (formal->type->symbol->hasFlag(FLAG_REF) &&
            (fn->hasFlag(FLAG_ALLOW_REF) ||
             formal->hasFlag(FLAG_WRAP_WRITTEN_FORMAL)))
          return true;
      } else if (call->isPrimitive(PRIM_MOVE)) {
        if (refNecessary(toSymExpr(call->get(1)), defMap, useMap))
          return true;
      } else if (call->isPrimitive(PRIM_GET_MEMBER) ||
                 call->isPrimitive(PRIM_GET_SVEC_MEMBER)) {
        CallExpr* move = toCallExpr(call->parentExpr);
        INT_ASSERT(move);
        INT_ASSERT(move->isPrimitive(PRIM_MOVE));
        if (refNecessary(toSymExpr(move->get(1)), defMap, useMap))
          return true;
      } else if (call->isPrimitive(PRIM_SET_MEMBER)) {
        if (!call->get(2)->typeInfo()->refType)
          return true;
      } else if (call->isPrimitive(PRIM_RETURN) ||
                 call->isPrimitive(PRIM_YIELD)) {
        return true;
      } else if (call->isPrimitive(PRIM_WIDE_GET_LOCALE) ||
                 call->isPrimitive(PRIM_WIDE_GET_NODE)) {
        // If we are extracting a field from the wide pointer, we need to keep it as a pointer.
        // Dereferencing would be premature.
        return true;
      } else if (call->isPrimitive(PRIM_DEREF) &&
                 isRecordWrappedType(se->var->type->getValType())) {
        // Heuristic: if we are dereferencing an array reference,
        // that reference may still be needed.
        Expr* callParent = call->parentExpr;
        INT_ASSERT(callParent);
        if (CallExpr* callParentCall = toCallExpr(callParent)) {
          if (callParentCall->isPrimitive(PRIM_MOVE)) {
            INT_ASSERT(call == callParentCall->get(2));
            SymExpr* dest = toSymExpr(callParentCall->get(1));
            INT_ASSERT(dest);
            if (dest->var->hasFlag(FLAG_COERCE_TEMP))
              return true;
          }
        }
      }
    }
  }
  return false;
}


//
// removes references that are not necessary
//
void cullOverReferences() {
  //
  // change call of reference function to value function
  //
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(defMap, useMap);
  forv_Vec(CallExpr, call, gCallExprs) {
    if (FnSymbol* fn = call->isResolved()) {
      if (FnSymbol* copy = fn->valueFunction) {
        if (CallExpr* move = toCallExpr(call->parentExpr)) {
          INT_ASSERT(move->isPrimitive(PRIM_MOVE));
          SymExpr* se = toSymExpr(move->get(1));
          INT_ASSERT(se);
          SET_LINENO(move);
          if (!refNecessary(se, defMap, useMap)) {
            SymExpr* base = toSymExpr(call->baseExpr);
            base->var = copy;
            VarSymbol* tmp = newTemp(copy->retType);
            move->insertBefore(new DefExpr(tmp));
            if (requiresImplicitDestroy(call)) {
              tmp->addFlag(FLAG_INSERT_AUTO_COPY);
              tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
            }
            if (useMap.get(se->var) && useMap.get(se->var)->n > 0) {
              move->insertAfter(new CallExpr(PRIM_MOVE, se->var,
                                  new CallExpr(PRIM_ADDR_OF, tmp)));
            } else {
              se->var->defPoint->remove();
            }
            se->var = tmp;
          }
        } else
          INT_FATAL(call, "unexpected case");
      }
    }
  }
  freeDefUseMaps(defMap, useMap);

  //
  // remove references to array wrapper records, domain wrapper
  // records, and iterator records; otherwise we can end up returning
  // a reference to a location that is on the stack
  //
  forv_Vec(DefExpr, def, gDefExprs) {
    if (!isTypeSymbol(def->sym) && def->sym->type) {
      if (Type* vt = def->sym->getValType()) {
        if (isRecordWrappedType(vt)) {
          def->sym->type = vt;
        }
      }
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        if (Type* vt = fn->retType->getValType()) {
          if (isRecordWrappedType(vt)) {
            fn->retType = vt;
            fn->retTag = RET_VALUE;
          }
        }
      }
    }
  }
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_DEREF) ||
        call->isPrimitive(PRIM_ADDR_OF)) {
      Type* vt = call->get(1)->typeInfo();
      if (isReferenceType(vt))
        vt = vt->getValType();
      if (isRecordWrappedType(vt))
        call->replace(call->get(1)->remove());
    }
    if (call->isPrimitive(PRIM_GET_MEMBER)) {
      Type* vt = call->get(2)->getValType();
      if (isRecordWrappedType(vt))
        call->primitive = primitives[PRIM_GET_MEMBER_VALUE];
    }
    if (call->isPrimitive(PRIM_GET_SVEC_MEMBER)) {
      Type* tupleType = call->get(1)->getValType();
      Type* vt = tupleType->getField("x1")->getValType();
      if (isRecordWrappedType(vt))
        call->primitive = primitives[PRIM_GET_SVEC_MEMBER_VALUE];
    }
    if (call->isPrimitive(PRIM_ARRAY_GET)) {
      Type* vt = call->getValType();
      if (isRecordWrappedType(vt))
        call->primitive = primitives[PRIM_ARRAY_GET_VALUE];
    }
  }
}
