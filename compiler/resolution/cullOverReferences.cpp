/*
 * Copyright 2004-2017 Cray Inc.
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
refNecessary(SymExpr*                      se);

//
// Should we switch to the "value" function?
//
// Here the "value" function includes blank return intent
// and also const ref return intent.
//
// Returning 'false' means to use the 'ref' return intent version.
static bool
shouldUseByValueFunction(FnSymbol*                     fn,
                         SymExpr*                      se) {
  return !refNecessary(se);
}

static bool
refNecessary(SymExpr*                      se) {
  // The ref is necessary if it is for an explicit ref var
  if (se->symbol()->hasFlag(FLAG_REF_VAR)) {
    return true;
  }

  // a ref is not necessary if the LHS is a value
  // (this can come up in recursive handling of a PRIM_MOVE).
  if (se->getValType() == se->typeInfo())
    return false;

  // Is there more than one definition for se?
  // count defs up to some maximum.
  bool more_than_one_def = false;
  int def_count = 0;
  for_SymbolDefs(def, se->symbol()) {
    def_count++;
    if (def_count > 1) {
      more_than_one_def = true;
      break;
    }
  }

  if (more_than_one_def) {
    // If se is a reference that is written to,
    // we need to keep the ref version.

    // We're only looking for things that set the value.
    // We don't care about PRIM_MOVEs b/c they only set the reference.
    // We do care about PRIM_ASSIGN or if the argument is passed
    // to a function (typically = ) as ref, inout, or out argument.
    bool valueIsSet = false;
    for_SymbolDefs(def, se->symbol()) {
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

  for_SymbolUses(use, se->symbol()) {
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      if (FnSymbol* fn = call->isResolved()) {
        ArgSymbol* formal = actual_to_formal(use);

        // added in f2bc2b27
        //if (formal->defPoint->getFunction()->_this == formal)
        //  return true;

        if (formal->intent == INTENT_INOUT || formal->intent == INTENT_OUT)
          return true;

        if (formal->type->symbol->hasFlag(FLAG_REF) &&
            (fn->hasFlag(FLAG_ALLOW_REF) ||
             formal->hasFlag(FLAG_WRAP_WRITTEN_FORMAL)))
          return true;

      } else if (call->isPrimitive(PRIM_MOVE)) {
        if (refNecessary(toSymExpr(call->get(1))))
          return true;

      } else if (call->isPrimitive(PRIM_GET_MEMBER) ||
                 call->isPrimitive(PRIM_GET_SVEC_MEMBER)) {
        CallExpr* move = toCallExpr(call->parentExpr);

        INT_ASSERT(move);
        INT_ASSERT(move->isPrimitive(PRIM_MOVE));

        if (refNecessary(toSymExpr(move->get(1))))
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

      } /*else if (call->isPrimitive(PRIM_DEREF) &&
                 isRecordWrappedType(se->symbol()->type->getValType())) {
        // Heuristic: if we are dereferencing an array reference,
        // that reference may still be needed.
        Expr* callParent = call->parentExpr;

        INT_ASSERT(callParent);

        if (CallExpr* callParentCall = toCallExpr(callParent)) {
          if (callParentCall->isPrimitive(PRIM_MOVE)) {
            INT_ASSERT(call == callParentCall->get(2));

            SymExpr* dest = toSymExpr(callParentCall->get(1));

            INT_ASSERT(dest);

            if (dest->symbol()->hasFlag(FLAG_COERCE_TEMP))
              return true;
          }
        }
      } */
    }
  }

  return false;
}


//
// This function adjusts calls to functions that have both
// a ref and non-ref version. These calls are represented with
// a ContextCallExpr. Whether to use the ref or value/const-ref
// version is determined based on how the result of the call is used.
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
//
void cullOverReferences() {
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
      useValueCall = shouldUseByValueFunction(refFn, lhs);
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

        if (lhs && lhs->symbol()->isUsed()) {
          // If the LHS was used, set it to the address of the
          // new temporary (which is the function return value)

          FnSymbol* moveInFn = toFnSymbol(move->parentSymbol);
          INT_ASSERT(moveInFn);
          Symbol* retSymbol = moveInFn->getReturnSymbol();
          // Check: are we adding a return of a local variable ?
          for_SymbolUses(use, lhs->symbol()) {
            if (CallExpr* useCall = toCallExpr(use->parentExpr))
              if (useCall->isPrimitive(PRIM_MOVE))
                if (SymExpr* useCallLHS = toSymExpr(useCall->get(1)))
                  if (useCallLHS->symbol() == retSymbol) {
                    USR_FATAL_CONT(move, "illegal expression to return by ref");
                    USR_PRINT(refCall, "called function returns a value not a reference");
                  }
          }

          move->insertAfter(new CallExpr(PRIM_MOVE,
                                         lhs->symbol(),
                                         new CallExpr(PRIM_ADDR_OF, tmp)));
        } else {
          // If the LHS was not used,
          // remove the old definition point since we have
          // provided a new one above.
          lhs->symbol()->defPoint->remove();
        }

        // Replace the LHS with our new temporary
        lhs->setSymbol(tmp);
      }

    } else {
      // Replace the ContextCallExpr with the ref call
      cc->replace(refCall);
    }
  }
}
