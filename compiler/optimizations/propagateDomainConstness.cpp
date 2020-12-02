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
#include "IfExpr.h"
#include "stmt.h"
#include "view.h"

#include <set>

static void setDefinedConstForDefExprWithIfExprs(Expr* e);
static VarSymbol *addFieldAccess(Symbol *receiver, const char *fieldName,
                                 Expr *insBefore, Expr *&insAfter,
                                 bool asRef);
static VarSymbol *addFieldAccess(Symbol *receiver, const char *fieldName,
                                 Expr *insBefore, Expr *&insAfter,
                                 bool asRef);
static Expr *getNextExprOrCreateNoop(Expr *baseExpr, bool &createdNoop);
static void setDefinedConstForDomainField(Symbol *thisSym, Symbol *fieldSym,
                                          Expr *nextExpr, Symbol *isConst);
static void setDefinedConstForDomainSymbol(Symbol *domainSym,
                                           Expr *insBeforeMarker,
                                           Expr *&insAfterMarker,
                                           Symbol *isConst);

// tries to determine if the DefExpr looks like a constant domain definition,
// and changes pertinent arguments in the CallExprs as necessary
void setDefinedConstForDefExprIfApplicable(DefExpr* defExpr,
                                           std::set<Flag>* flags) {
  if (defExpr->init != NULL) {
    if (flags->count(FLAG_CONST) == 0) {
      setDefinedConstForDefExprWithIfExprs(defExpr->init);
    }
  }
}

// Add code right after a PRIM_SET_MEMBER, if it is setting a const domain
// member in an aggregate type
void setDefinedConstForPrimSetMemberIfApplicable(CallExpr *call) {
  INT_ASSERT(call->isPrimitive(PRIM_SET_MEMBER));

  bool createdNoop;
  Expr *nextExpr = getNextExprOrCreateNoop(call, createdNoop);

  // PRIM_SET_MEMBER args:
  // 1: this
  // 2: field
  // 3: src
  Symbol *thisSym = NULL;
  if (SymExpr *se = toSymExpr(call->get(1))) {
    thisSym = se->symbol();
  }

  Symbol *fieldSym = NULL;
  Type *fieldType = NULL;
  if (SymExpr *se = toSymExpr(call->get(2))) {
    fieldSym = se->symbol();
    fieldType = fieldSym->getValType();
  }
  INT_ASSERT(fieldSym);
  INT_ASSERT(fieldType);

  if (!fieldSym->hasFlag(FLAG_TYPE_VARIABLE)) {
    if (fieldType->symbol->hasFlag(FLAG_DOMAIN)) {
      Symbol *isConst = fieldSym->hasFlag(FLAG_CONST) ? gTrue : gFalse;
      setDefinedConstForDomainField(thisSym, fieldSym, nextExpr, isConst);
    }
  }

  if (createdNoop) {
    nextExpr->remove();
  }
}

// go through all PRIM_SET_MEMBER in an initializer, and adjust them if needed
void setDefinedConstForFieldsInInitializer(FnSymbol *fn) {
  std::vector<CallExpr *> calls;
  collectCallExprs(fn->body, calls);
  for_vector(CallExpr, call, calls) {
    if(call->isPrimitive(PRIM_SET_MEMBER)) {
      if (fn->_this == toSymExpr(call->get(1))->symbol()) {
        setDefinedConstForPrimSetMemberIfApplicable(call);
      }
    }
  }
}

// help removing chpl__initCopy and chpl__autoCopy after resolution while
// retaining constness information therein
void removeInitOrAutoCopyPostResolution(CallExpr *call) {
  Expr *parentExpr = call->parentExpr;

  bool createdNoop;
  Expr *nextExpr = getNextExprOrCreateNoop(parentExpr, createdNoop);

  Symbol *argSym = NULL;
  Type *argType = NULL;
  if (SymExpr *se = toSymExpr(call->get(1))) {
    argSym = se->symbol();
    argType = argSym->getValType();
  }
  INT_ASSERT(argSym);
  INT_ASSERT(argType);

  call->replace(call->get(1)->remove());

  // we removed the first argument already, so definedConst is the first
  // argument now
  SymExpr *secondArg = toSymExpr(call->get(1)->remove());
  INT_ASSERT(secondArg);

  Symbol *isConst = secondArg->symbol();
  INT_ASSERT(isConst->type == dtBool);

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

    if (!isShadowVarSymbol(lhs)) {
      setDefinedConstForDomainSymbol(lhs, nextExpr, isConst);

      if (createdNoop) {
        nextExpr->remove();
      }
    }
  }
}

static Expr* getNextExprOrCreateNoop(Expr *baseExpr, bool &createdNoop) {
  createdNoop = false;
  Expr *nextExpr = baseExpr->next;

  if (nextExpr == NULL) {
    createdNoop = true;
  }
  else if (CallExpr *nextCall = toCallExpr(nextExpr)) {
    if (nextCall->isPrimitive(PRIM_RETURN)) {
      createdNoop = true;
    }
  }
  else {
    // I don't know what this next expression could be, so add a NOOP just in
    // case
    createdNoop = true;
  }

  if (createdNoop) {
    CallExpr *noop = new CallExpr(PRIM_NOOP);
    baseExpr->insertAfter(noop);
    return noop;
  }

  return nextExpr;
}
                
static void setDefinedConstForDefExprWithIfExprs(Expr* e) {
  if (CallExpr *initCall = toCallExpr(e)) {
    if (initCall->isNamed("chpl__buildDomainExpr")) {
      initCall->argList.last()->replace(new SymExpr(gFalse));
      return;
    }
    else if (initCall->isNamed("chpl__distributed")) {
      initCall->get(3)->replace(new SymExpr(gFalse));
      return;
    }
  }
  else if (IfExpr *initIf = toIfExpr(e)) {
    setDefinedConstForDefExprWithIfExprs(initIf->getThenStmt()->body.head);
    setDefinedConstForDefExprWithIfExprs(initIf->getElseStmt()->body.head);
  }
}

static VarSymbol *addFieldAccess(Symbol *receiver, const char *fieldName,
                                 Expr *insBefore, Expr *&insAfter,
                                 bool asRef) {

  AggregateType *aggType =
      toAggregateType(canonicalClassType(receiver->getValType()));
  INT_ASSERT(aggType);

  Symbol *fieldSym = aggType->getField(fieldName);
  Type *fieldType = asRef ? fieldSym->type->getRefType() : fieldSym->type;
  
  VarSymbol *fieldRef = newTemp(fieldName, fieldType);
  fieldRef->addFlag(FLAG_UNSAFE);  // this is a short-lived temp
  insBefore->insertBefore(new DefExpr(fieldRef));

  PrimitiveTag primTag = asRef ? PRIM_GET_MEMBER : PRIM_GET_MEMBER_VALUE;
  CallExpr *getField= new CallExpr(PRIM_MOVE, fieldRef,
                                   new CallExpr(primTag,
                                                receiver,
                                                fieldSym));
  insAfter->insertAfter(getField);
  insAfter = getField;

  return fieldRef;
}

static void setDefinedConstForDomainSymbol(Symbol *domainSym,
                                           Expr *insBeforeMarker,
                                           Expr *&insAfterMarker,
                                           Symbol *isConst) {

  VarSymbol *domInstance = addFieldAccess(domainSym, "_instance",
                                          insBeforeMarker, insAfterMarker,
                                          /*asRef=*/ true);

  VarSymbol *refToDefinedConst = addFieldAccess(domInstance, "definedConst",
                                                insBeforeMarker, insAfterMarker,
                                                /*asRef=*/ true);

  CallExpr *setDefinedConst = new CallExpr(PRIM_MOVE, refToDefinedConst,
                                           isConst);

  insAfterMarker->insertAfter(setDefinedConst);
  insAfterMarker = insAfterMarker->next;
}

void setDefinedConstForDomainSymbol(Symbol *domainSym, Expr *nextExpr,
                                    Symbol *isConst) {
  CallExpr *noop = new CallExpr(PRIM_NOOP);
  nextExpr->insertBefore(noop);

  Expr *insBeforeMarker = noop;
  Expr *insAfterMarker = noop;

  setDefinedConstForDomainSymbol(domainSym, insBeforeMarker, insAfterMarker,
                                 isConst);

  noop->remove();
}

static void setDefinedConstForDomainField(Symbol *thisSym, Symbol *fieldSym,
                                          Expr *nextExpr, Symbol *isConst) {
    Expr *insBeforeMarker = nextExpr;
    Expr *insAfterMarker = nextExpr;
    VarSymbol *domSym = addFieldAccess(thisSym, fieldSym->name,
                                       insBeforeMarker, insAfterMarker,
                                       /* asRef = */false);
    setDefinedConstForDomainSymbol(domSym, insBeforeMarker, insAfterMarker,
                                   isConst);
}

