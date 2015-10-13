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


static void callGetterForValueReturn()
{
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
            // Danger, Will Robinson! We are about to take the address of this
            // temporary.  The temporary must remain in-scope for as long as
            // anything referred to in the return value of the original 'fn'.
            // We need to select a scope that contains all uses of the original
            // LHS se, and put the declaration for our temp variable there --
            // to make sure that the deref temp does not go out of scope before
            // references to it do.  Note #1.
            // For now, we just use the declaration scope of the original se.
            se->var->defPoint->insertAfter(new DefExpr(tmp));
#ifndef HILDE_MM
            // Why is this here?
            // Marking variables for autocopy/autodestroy ought to be done in
            // one place once and for all.  Is this that place?
            if (requiresImplicitDestroy(call)) {
              tmp->addFlag(FLAG_INSERT_AUTO_COPY);
              tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
            }
#endif
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
}


static void removeRefsToWrappedTypes()
{
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


// Look for and replace the pattern:
//  ('move' ref_tmp ('addr of' var1))
//  ('move' var2 ('deref' ref_tmp))
// with
//  ('move' var2 var1)
//
static void removeRefDerefPairs()
{
  // Walk all "addr of" primitives.
  forv_Vec(CallExpr, call, gCallExprs)
  {
    if (! call->isPrimitive(PRIM_ADDR_OF))
      continue;

    // Ignore calls that are not in the tree.
    if (! call->parentExpr)
      continue;

    // Get the statement-level expression containing this primitive.
    CallExpr* addrOfStmt = toCallExpr(call->getStmtExpr());
    INT_ASSERT(addrOfStmt);
    INT_ASSERT(addrOfStmt->isPrimitive(PRIM_MOVE));

    // Get the next statement.
    if (CallExpr* derefStmt = toCallExpr(addrOfStmt->next))
    {
      // Make sure it is a move
      if (! derefStmt->isPrimitive(PRIM_MOVE))
        continue;
      
      // See if it contains a deref
      if (CallExpr* deref = toCallExpr(derefStmt->get(2)))
      {
        // Make sure it is a deref primitive
        if (! deref->isPrimitive(PRIM_DEREF))
          continue;

        // See if its arg is the LHS of the addrOfStmt
        SymExpr* derefArg = toSymExpr(deref->get(1));
        INT_ASSERT(derefArg);
        SymExpr* refTmp = toSymExpr(addrOfStmt->get(1));
        INT_ASSERT(refTmp);
        if (derefArg->var == refTmp->var)
        {
          // Bingo.  Let's do the transformation.

          // Actually, we leave the deref in place in case there are other uses
          // of the refTmp.  But we replace the deref call with
          // the (non-ref) operand of the 'addr of' primitive.

          SymExpr* addrOfArg = toSymExpr(call->get(1));
          // We expect the argument of the "addr of" primitive to be a SymExpr.
          INT_ASSERT(addrOfArg);

          SET_LINENO(derefStmt);
          deref->replace(new SymExpr(addrOfArg->var));
        }
      }
    }
  }
}


//
// Replace getter calls with setter calls where the return type is only read.
// Also convert some references to values, to get required semantics.
//
void cullOverReferences() {
  callGetterForValueReturn();
  removeRefsToWrappedTypes();
  removeRefDerefPairs();
}


//########################################################################
//# NOTES
//#
//# Note #1: In places where the return value of a setter is immediately
//# dereferenced, we get code that looks like this:
//#      (875642 'move' tmp[954861](875648 call dsiAccess[875924] call_tmp[875634] i[875526]))
//#      (954865 'move' call_tmp[875531](954863 'addr of' tmp[954861]))
//#      (875650 'move' ret[875535](875652 'deref' call_tmp[875531]))
//# i.e. a getter followed by 'addr of' followed by 'deref'.
//# A later pass apparently knows how to collapse this out, but it creates
//# problems for AMM (as implemented).  It would probably be good to follow
//# the setter -> getter replacement with a pass that looks for this kind of
//# no-op and removes them.
//#
