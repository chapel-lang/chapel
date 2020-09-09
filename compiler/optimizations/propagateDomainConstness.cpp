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
#include "stmt.h"
#include "CallExpr.h"
#include "DecoratedClassType.h"
#include "expr.h"
#include "view.h"

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

  bool isDomainRecord = argType->symbol->hasFlag(FLAG_DOMAIN);
  bool isDomainInstance = isDomainClass(argType);

  call->replace(call->get(1)->remove());

  if (isDomainRecord || isDomainInstance) {

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

        Expr *anchor = nextExpr;

        VarSymbol *domInstance;

        if (isDomainRecord) {
          AggregateType *lhsAggType = toAggregateType(lhs->type);
          Symbol *domInstanceField = lhsAggType->getField("_instance");
          domInstance = newTemp("dom_instance_tmp",
                                domInstanceField->type);
          nextExpr->insertBefore(new DefExpr(domInstance));
          CallExpr *getInstance = new CallExpr(PRIM_MOVE, domInstance,
                                               new CallExpr(PRIM_GET_MEMBER,
                                                            lhs,
                                                            domInstanceField));
          nextExpr->insertAfter(getInstance);
          anchor = getInstance;
        }
        else {
          std::cout << "Not touching domain class\n";
          nprint_view(lhs);
          return;
          //domInstance = toVarSymbol(lhs);
        }

        VarSymbol *nonnilInst = newTemp("nonnil_instance_tmp", dtBool);
        nextExpr->insertBefore(new DefExpr(nonnilInst));

        anchor->insertAfter(new CallExpr(PRIM_MOVE, nonnilInst,
                     new CallExpr(PRIM_NOTEQUAL, domInstance, gNil)));

        BlockStmt *thenBlock = new BlockStmt();

        AggregateType *domInstAggType = toAggregateType(domInstance->type);
        if (domInstAggType == NULL) {
          domInstAggType = toDecoratedClassType(domInstance->type)->getCanonicalClass();
        }

        INT_ASSERT(domInstAggType);

        Symbol *definedConstField = domInstAggType->getField("definedConst");
        VarSymbol *definedConstRef = newTemp("defined_const_tmp",
                                             definedConstField->type->getRefType());
        thenBlock->insertAtTail(new DefExpr(definedConstRef));

        CallExpr *getDefinedConstRef = new CallExpr(PRIM_MOVE, definedConstRef,
                                                    new CallExpr(PRIM_GET_MEMBER,
                                                                 domInstance,
                                                                 definedConstField));
        CallExpr *setDefinedConst = new CallExpr(PRIM_MOVE, 
                                                 definedConstRef,
                                                 secondArg);

        nprint_view(getDefinedConstRef);

        thenBlock->insertAtTail(getDefinedConstRef);
        thenBlock->insertAtTail(setDefinedConst);

        anchor->insertAfter(new CondStmt(new SymExpr(nonnilInst), thenBlock));
      }
    }
  }
}
