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

void removeInitOrAutoCopyPostResolution(CallExpr *call) {
  Expr *parentExpr = call->parentExpr;
  Expr *nextExpr = parentExpr->next;
  bool isDomain = false;

  if (SymExpr *se = toSymExpr(call->get(1))) {
    isDomain = se->symbol()->getValType()->symbol->hasFlag(FLAG_DOMAIN);
  }

  call->replace(call->get(1)->remove());

  if (isDomain) {
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

    if (SymExpr *se = toSymExpr(secondArg)) {
      if (se->symbol()->type == dtBool) {  // must be definedConst
        AggregateType *lhsAggType = toAggregateType(lhs->type);
        Symbol *domInstanceField = lhsAggType->getField("_instance");
        VarSymbol *domInstance = newTemp("dom_instance_tmp",
                                         domInstanceField->type);
        nextExpr->insertBefore(new DefExpr(domInstance));
        CallExpr *getInstance = new CallExpr(PRIM_MOVE, domInstance,
                                             new CallExpr(PRIM_GET_MEMBER,
                                                          lhs,
                                                          domInstanceField));

        AggregateType *domInstAggType = toDecoratedClassType(domInstance->type)->getCanonicalClass();
        
        Symbol *definedConstField = domInstAggType->getField("definedConst");
        VarSymbol *definedConstRef = newTemp("defined_const_tmp",
                                             definedConstField->type->getRefType());
        nextExpr->insertBefore(new DefExpr(definedConstRef));

        CallExpr *getDefinedConstRef = new CallExpr(PRIM_MOVE, definedConstRef,
                                                    new CallExpr(PRIM_GET_MEMBER,
                                                                 domInstance,
                                                                 definedConstField));
        CallExpr *setDefinedConst = new CallExpr(PRIM_MOVE, 
                                                 definedConstRef,
                                                 secondArg);

        nextExpr->insertAfter(getInstance);
        getInstance->insertAfter(getDefinedConstRef);
        getDefinedConstRef->insertAfter(setDefinedConst);
      }
    }
  }
}
