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
#include "resolution.h"
#include "stmt.h"
#include "symbol.h"

static bool
refNecessary(SymExpr*                      se,
             Map<Symbol*, Vec<SymExpr*>*>& defMap,
             Map<Symbol*, Vec<SymExpr*>*>& useMap);

//
// Should we switch to the "value" function?
//
// Generally we continue to so do for non-lvalue references when a value
// is acceptable.  However functions that return strings prefer to use
// the by-ref version so long as there is no specialization on
// the setter param
static bool
shouldUseByValueFunction(FnSymbol*                     fn,
                         SymExpr*                      se,
                         Map<Symbol*, Vec<SymExpr*>*>& defMap,
                         Map<Symbol*, Vec<SymExpr*>*>& useMap) {
  bool retval = false;

  if (refNecessary(se, defMap, useMap) == false) {
    if (isString(se->var->type->getValType()) &&
        fn->hasFlag(FLAG_FN_REF_USES_SETTER) == false) {
      retval = false;

    } else {
      retval = true;
    }
  }

  return retval;
}

static bool
refNecessary(SymExpr*                      se,
             Map<Symbol*, Vec<SymExpr*>*>& defMap,
             Map<Symbol*, Vec<SymExpr*>*>& useMap) {
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
        // If we are extracting a field from the wide pointer,
        // we need to keep it as a pointer.
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
// Michael Noakes: 2016/01/07
// Currently every function that is declared as return-by-ref
// is implemented as a pair of functions.
//
//   1) A function that returns a ref
//   2) A function that returns a value
//
// Additionally the "by-ref" version includes a pointer to the "by-value"
// version.
//
// This is done independently of whether the setter param is used in the
// the function body and without careful attention to whether the function
// actually returns a value or a ref.
//
// The early phases of resolution have bound the call-site for every
// return-by-ref function to the "by-ref" version of the function.
//
// This pass studies all of these call-sites and determines if the
// "by-reference" version is "necessary".  If it is not, then the
// compiler attempts to switch in the "by-value" version. This requires
// the insertion of a temporary value near the call site.
//
// The compiler must also determine whether the "by value" implementation
// will have inserted an autoCopy.   If so then the compiler attaches
// the AUTO_COPY/AUTO_DESTROY flags as necessary to enable the
// callDestructors pass to operate correctly.
//
// As far as I can tell this switch is, at best, only lossely coupled to
// the the setter param.   Most functions that return with ref-intent do
// not inspect the setter param but I think that some portions of the
// compiler assume that setter = true for the by-ref version and
// setter = false for the by-value version.  If this is correct then there
// could be programs in which the function replacement does not track the
// setter paramc orrectly.
//
// Careful separation of "by-ref" behavior and the management of the
// setter param will require additional effort.
//

void cullOverReferences() {
  Map<Symbol*, Vec<SymExpr*>*> defMap;
  Map<Symbol*, Vec<SymExpr*>*> useMap;

  buildDefUseMaps(defMap, useMap);

  forv_Vec(CallExpr, call, gCallExprs) {
    // Is this call to a non-primitive function?
    if (FnSymbol* fn = call->isResolved()) {
      // Does that function have a "value" counter part?
      if (FnSymbol* valueFn = fn->valueFunction) {
        if (CallExpr* move = toCallExpr(call->parentExpr)) {
          INT_ASSERT(move->isPrimitive(PRIM_MOVE));

          SymExpr* se = toSymExpr(move->get(1));

          INT_ASSERT(se);

          // Should we switch to the by-value form?
          if (shouldUseByValueFunction(fn, se, defMap, useMap) == true) {
            SET_LINENO(move);

            SymExpr*   base = toSymExpr(call->baseExpr);
            VarSymbol* tmp  = newTemp(valueFn->retType);

            // Swap in the by-value implementation
            base->var = valueFn;

            // Generate a value temp to receive the value
            move->insertBefore(new DefExpr(tmp));

            if (requiresImplicitDestroy(call)) {
              if (isString(valueFn->retType) == false) {
                tmp->addFlag(FLAG_INSERT_AUTO_COPY);
                tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
              } else {
                tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
              }
            }

            if (useMap.get(se->var) && useMap.get(se->var)->n > 0) {
              move->insertAfter(new CallExpr(PRIM_MOVE,
                                             se->var,
                                             new CallExpr(PRIM_ADDR_OF, tmp)));
            } else {
              se->var->defPoint->remove();
            }

            se->var = tmp;
          }
        } else {
          INT_FATAL(call, "unexpected case");
        }
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
            fn->retTag  = RET_VALUE;
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
      Type* vt        = tupleType->getField("x1")->getValType();

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
