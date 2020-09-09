/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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
#include "CallExpr.h"
#include "DecoratedClassType.h"
#include "expr.h"
#include "stmt.h"
#include "view.h"

static VarSymbol *addFieldAccess(Symbol *receiver, const char *fieldName,
                                 Expr *insBefore, Expr *&insAfter,
                                 bool asRef) {
  Type *receiverType = receiver->type;

  AggregateType *aggType = toAggregateType(receiverType);
  if (aggType == NULL) {
    aggType = toDecoratedClassType(receiverType)->getCanonicalClass();
  }
  INT_ASSERT(aggType);

  Symbol *fieldSym = aggType->getField(fieldName);
  Type *fieldType = asRef ? fieldSym->type->getRefType() : fieldSym->type;
  VarSymbol *fieldRef = newTemp(fieldName, fieldType);
  insBefore->insertBefore(new DefExpr(fieldRef));

  CallExpr *getFieldRef = new CallExpr(PRIM_MOVE, fieldRef,
                                       new CallExpr(PRIM_GET_MEMBER,
                                                    receiver,
                                                    fieldSym));
  insAfter->insertAfter(getFieldRef);
  insAfter = getFieldRef;

  return fieldRef;
}

void removeInitOrAutoCopyPostResolution(CallExpr *call) {
  Expr *parentExpr = call->parentExpr;
  Expr *nextExpr = parentExpr->next;

  Symbol *argSym = NULL;
  Type *argType = NULL;
  if (SymExpr *se = toSymExpr(call->get(1))) {
    argSym = se->symbol();
    argType = argSym->getValType();
  }
  INT_ASSERT(argSym);
  INT_ASSERT(argType);

  call->replace(call->get(1)->remove());

  if (argType->symbol->hasFlag(FLAG_DOMAIN)) {
    Symbol *lhs = NULL;
    if (CallExpr *parentCall = toCallExpr(parentExpr)) {
      if (parentCall->isPrimitive(PRIM_MOVE)) {
        if (SymExpr *lhsSE = toSymExpr(parentCall->get(1))) {
          lhs = lhsSE->symbol();
        }
      }
    }
    INT_ASSERT(lhs);

    // we removed the first argument already, so definedConst is the first
    // argument now
    Expr *secondArg = call->get(1)->remove();
    SymExpr *se = toSymExpr(secondArg);
    INT_ASSERT(se);
    INT_ASSERT(se->symbol()->type == dtBool);

    Expr *anchor = nextExpr;

    VarSymbol *domInstance = addFieldAccess(lhs, "_instance", nextExpr, anchor,
                                            /*asRef=*/ false);
    VarSymbol *refToDefinedConst = addFieldAccess(domInstance, "definedConst",
                                                  nextExpr, anchor,
                                                  /*asRef=*/ true);
    CallExpr *setDefinedConst = new CallExpr(PRIM_MOVE, refToDefinedConst,
                                             secondArg);
    anchor->insertAfter(setDefinedConst);
  }
}
