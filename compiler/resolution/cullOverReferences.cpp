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
// Here the "value" function includes blank return intent
// and also const ref return intent.
//
// Returning 'false' means to use the 'ref' return intent version.
static bool
shouldUseByValueFunction(FnSymbol*                     fn,
                         SymExpr*                      se,
                         Map<Symbol*, Vec<SymExpr*>*>& defMap,
                         Map<Symbol*, Vec<SymExpr*>*>& useMap) {
  return !refNecessary(se, defMap, useMap);
}

static bool
refNecessary(SymExpr*                      se,
             Map<Symbol*, Vec<SymExpr*>*>& defMap,
             Map<Symbol*, Vec<SymExpr*>*>& useMap) {
  Vec<SymExpr*>* defs = defMap.get(se->var);

  if (defs && defs->n > 1) {
    // If se is a reference that is written to,
    // we need to keep the ref version.

    // If it is not a reference, it's not clear why
    // this code is being run...
    INT_ASSERT(se->getValType() != se->typeInfo());

    // We're only looking for things that set the value.
    // We don't care about PRIM_MOVEs b/c they only set the reference.
    // We do care about PRIM_ASSIGN or if the argument is passed
    // to a function (typically = ) as ref, inout, or out argument.
    bool valueIsSet = false;
    for_defs(def, defMap, se->var) {
      if (def->parentExpr) {
        if (CallExpr* parentCall = toCallExpr(def->parentExpr)) {
          if (parentCall->isPrimitive(PRIM_MOVE)) {
            // Ignore this def
            // We don't care about a PRIM_MOVE because it's setting
            // a reference
          } else {
            valueIsSet = true;
          }
        }
      }
    }

    if (valueIsSet) {
      return true;
    }
  }

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
        FnSymbol* inFn = toFnSymbol(call->parentSymbol);
        // It is not necessary to use the 'ref' version
        // if the function result is returned by 'const ref'.
        if (inFn->retTag == RET_CONST_REF) return false;
        // MPF: it seems to cause problems to return false
        // here when inFn->retTag is RET_VALUE.
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
// As far as I can tell this switch is, at best, only loosely coupled to
// the the setter param.   Most functions that return with ref-intent do
// not inspect the setter param but I think that some portions of the
// compiler assume that setter = true for the by-ref version and
// setter = false for the by-value version.  If this is correct then there
// could be programs in which the function replacement does not track the
// setter param correctly.
//
// Careful separation of "by-ref" behavior and the management of the
// setter param will require additional effort.
//

void cullOverReferences() {
  Map<Symbol*, Vec<SymExpr*>*> defMap;
  Map<Symbol*, Vec<SymExpr*>*> useMap;

  buildDefUseMaps(defMap, useMap);

  forv_Vec(ContextCallExpr, cc, gContextCallExprs) {
    // Make sure that the context call only has 2 options.
    INT_ASSERT(cc->options.length == 2);

    CallExpr* refCall = cc->getRefCall();
    CallExpr* valueCall = cc->getRValueCall();

    FnSymbol* refFn = refCall->isResolved();
    INT_ASSERT(refFn);
    FnSymbol* valueFn = valueCall->isResolved();
    INT_ASSERT(valueFn);

    bool useValueCall;

    CallExpr* move = NULL; // set if the call is in a PRIM_MOVE
    SymExpr* lhs = NULL; // lhs if call is in a PRIM_MOVE

    // Decide whether to use the value call or the ref call.
    // Always leave the ref call for iterators.
    // (It would be an improvement to choose the appropriate one
    //  based upon how the iterator is used, but such a feature
    //  would require specific support for iterators since yielding
    //  is not the same as returning.)
    move = toCallExpr(cc->parentExpr);
    if (refFn->isIterator())
      useValueCall = false;
    else if (move) {
      INT_ASSERT(move->isPrimitive(PRIM_MOVE));

      lhs = toSymExpr(move->get(1));

      INT_ASSERT(lhs);

      // Should we switch to the by-value form?
      useValueCall = shouldUseByValueFunction(refFn, lhs, defMap, useMap);
    } else {
      // e.g. array access in own statement like this:
      //   A(i)
      // should use 'getter'
      // MPF - note 2016-01: this code does not seem to be triggered
      // in the present compiler.
      useValueCall = true;
    }

    valueCall->remove();
    refCall->remove();

    if (useValueCall) {
      // Replace the ContextCallExpr with the value call
      cc->replace(valueCall);

      // Adjust the AST around the value call to include
      // a temporary to receive the value.

      // Adjust code to use value return version.
      // The other option is that retTag is RET_CONST_REF,
      // in which case no further adjustment is necessary.
      if (move && valueFn->retTag == RET_VALUE) {
        SET_LINENO(move);
        // Generate a value temp to receive the value
        VarSymbol* tmp  = newTemp(valueFn->retType);
        move->insertBefore(new DefExpr(tmp));

        if (requiresImplicitDestroy(valueCall)) {
          if (isUserDefinedRecord(valueFn->retType) == false) {
            tmp->addFlag(FLAG_INSERT_AUTO_COPY);
            tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
          } else {
            tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
          }
        }

        if (lhs && useMap.get(lhs->var) && useMap.get(lhs->var)->n > 0) {
          // If the LHS was used, set it to the address of the
          // new temporary (which is the function return value)

          FnSymbol* moveInFn = toFnSymbol(move->parentSymbol);
          INT_ASSERT(moveInFn);
          Symbol* retSymbol = moveInFn->getReturnSymbol();
          // Check: are we adding a return of a local variable ?
          for_uses(use, useMap, lhs->var) {
            if (CallExpr* useCall = toCallExpr(use->parentExpr))
              if (useCall->isPrimitive(PRIM_MOVE))
                if (SymExpr* useCallLHS = toSymExpr(useCall->get(1)))
                  if (useCallLHS->var == retSymbol) {
                    USR_FATAL_CONT(move, "illegal expression to return by ref");
                    USR_PRINT(refCall, "called function returns a value not a reference");
                  }
          }

          move->insertAfter(new CallExpr(PRIM_MOVE,
                                         lhs->var,
                                         new CallExpr(PRIM_ADDR_OF, tmp)));
        } else {
          // If the LHS was not used,
          // remove the old definition point since we have
          // provided a new one above.
          lhs->var->defPoint->remove();
        }

        // Replace the LHS with our new temporary
        lhs->var = tmp;
      }

    } else {
      // Replace the ContextCallExpr with the ref call
      cc->replace(refCall);
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
