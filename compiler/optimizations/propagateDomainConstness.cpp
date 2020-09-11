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
static bool addNoopAsNextExprIfNeeded(Expr *baseExpr, Expr *&nextExpr);
static void setDefinedConstForDomainSymbol(Symbol *domainSym, Expr *nextExpr,
                                           Expr *anchor, Symbol *isConst);
static void setDefinedConstForDomainField(Symbol *thisSym, Symbol *fieldSym,
                                          Expr *nextExpr, Symbol *isConst);

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

  Expr *nextExpr = call->next;
  bool addNoop = addNoopAsNextExprIfNeeded(call, nextExpr);

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

  if (addNoop) {
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
  Expr *nextExpr = parentExpr->next;
  bool addNoop = addNoopAsNextExprIfNeeded(parentExpr, nextExpr);

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
      Expr *anchor = nextExpr;
      setDefinedConstForDomainSymbol(lhs, nextExpr, anchor, isConst);

      if (addNoop) {
        nextExpr->remove();
      }
    }
  }
}

static bool addNoopAsNextExprIfNeeded(Expr *baseExpr, Expr *&nextExpr) {
  bool addNoop = false;

  if (nextExpr == NULL) {
    addNoop = true;
  }
  else if (CallExpr *nextCall = toCallExpr(nextExpr)) {
    if (nextCall->isPrimitive(PRIM_RETURN)) {
      addNoop = true;
    }
  }
  else {
    // I don't know what this next expression could be, so add a NOOP just in
    // case
    addNoop = true;
  }

  if (addNoop) {
    CallExpr *noop = new CallExpr(PRIM_NOOP);
    baseExpr->insertAfter(noop);
    nextExpr = noop;
  }

  return addNoop;
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
  Type *receiverType = receiver->getValType();

  AggregateType *aggType = toAggregateType(receiverType);
  if (aggType == NULL) {
    aggType = toDecoratedClassType(receiverType)->getCanonicalClass();
  }
  INT_ASSERT(aggType);

  //Symbol *fieldSym = aggType->getValType()->getField(fieldName);
  //Type *fieldType = asRef ? fieldSym->type->getRefType() : fieldSym->type;
  //

  Symbol *fieldSym = NULL;
  if (aggType->symbol->hasFlag(FLAG_REF)) {
    fieldSym = aggType->getValType()->getField(fieldName);
  }
  else {
    fieldSym = aggType->getField(fieldName);
  }
  Type *fieldType = asRef ? fieldSym->type->getRefType() : fieldSym->type;
  //if (asRef) {
    //fieldType = wideRefMap.get(fieldType);
  //}
  //Type *fieldType = fieldSym->type;
  //if (asRef) {
    //if (fieldType->symbol->hasFlag(FLAG_REF) ||
        //fieldType->symbol->hasFlag(FLAG_WIDE_REF)) { 

    //}
    //else {
      //fieldType = fieldType->getRefType();
    //}
  //}
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

//static void addFieldSet(Symbol *receiver, const char *fieldName,
                              //Expr *&insAfter, Symbol *setTo) {
  //Type *receiverType = receiver->type;

  //AggregateType *aggType = toAggregateType(receiverType);
  //if (aggType == NULL) {
    //aggType = toDecoratedClassType(receiverType)->getCanonicalClass();
  //}
  //INT_ASSERT(aggType);

  //Symbol *fieldSym = aggType->getField(fieldName);
  ////Type *fieldType = fieldSym->type;
  //CallExpr *setField = new CallExpr(PRIM_SET_MEMBER, receiver, fieldSym, setTo);

  //insAfter->insertAfter(setField);
  //insAfter = setField;
//}

static void setDefinedConstForDomainSymbol(Symbol *domainSym, Expr *nextExpr,
                                           Expr *anchor, Symbol *isConst) {
  VarSymbol *domInstance = addFieldAccess(domainSym, "_instance",
                                          nextExpr, anchor, /*asRef=*/ true);

  VarSymbol *refToDefinedConst = addFieldAccess(domInstance, "definedConst",
                                                nextExpr, anchor,
                                                true);

  //addFieldSet(domInstance, "definedConst", anchor, isConst);

  CallExpr *setDefinedConst = new CallExpr(PRIM_MOVE, refToDefinedConst,
                                           isConst);

  anchor->insertAfter(setDefinedConst);
}

static void setDefinedConstForDomainField(Symbol *thisSym, Symbol *fieldSym,
                                          Expr *nextExpr, Symbol *isConst) {
    Expr *anchor = nextExpr;
    VarSymbol *domSym = addFieldAccess(thisSym, fieldSym->name,
                                       nextExpr, anchor, /*asRef=*/false);
    setDefinedConstForDomainSymbol(domSym, nextExpr, anchor, isConst);
}

