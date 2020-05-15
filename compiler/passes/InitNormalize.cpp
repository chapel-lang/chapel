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

#include "InitNormalize.h"

#include "AstVisitor.h"
#include "AstVisitorTraverse.h"
#include "ForallStmt.h"
#include "IfExpr.h"
#include "initializerRules.h"
#include "LoopExpr.h"
#include "stmt.h"
#include "astutil.h"

static bool isAssignment(CallExpr* callExpr);
static bool isSimpleAssignment(CallExpr* callExpr);
static bool isCompoundAssignment(CallExpr* callExpr);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

InitNormalize::InitNormalize(FnSymbol* fn) {
  mFn            = fn;
  mCurrField     = firstField(fn);
  mPhase         = startPhase(fn);
  mBlockType     = cBlockNormal;
  mPrevBlockType = cBlockNormal;
  mThisAsParent  = NULL;
}

InitNormalize::InitNormalize(BlockStmt* block, const InitNormalize& curr) {
  mFn            = curr.mFn;
  mCurrField     = curr.mCurrField;
  mPhase         = curr.mPhase;
  mThisAsParent  = curr.mThisAsParent;

  if (CallExpr* blockInfo = block->blockInfoGet()) {
    if        (blockInfo->isPrimitive(PRIM_BLOCK_BEGIN)       == true ||
               blockInfo->isPrimitive(PRIM_BLOCK_BEGIN_ON)    == true) {
      mBlockType = cBlockBegin;

    } else if (blockInfo->isPrimitive(PRIM_BLOCK_COBEGIN)     == true) {
      // Lydia NOTE 2017/07/26: If PRIM_BLOCK_COBEGIN_ONs are ever made, we
      // should match against them here
      mBlockType = cBlockCobegin;

    } else if (blockInfo->isPrimitive(PRIM_BLOCK_COFORALL)    == true ||
               blockInfo->isPrimitive(PRIM_BLOCK_COFORALL_ON) == true) {
      mBlockType = cBlockCoforall;

    } else if (blockInfo->isPrimitive(PRIM_BLOCK_ON) == true ||
               blockInfo->isPrimitive(PRIM_BLOCK_ELIDED_ON)) {
      mBlockType = cBlockOn;

    } else {
      INT_ASSERT(false);
    }

  } else {
    mBlockType = curr.mBlockType;
  }

  if (mBlockType != curr.mBlockType) {
    mPrevBlockType = curr.mBlockType;
  } else {
    mPrevBlockType = curr.mPrevBlockType;
  }
}

InitNormalize::InitNormalize(CondStmt* cond, const InitNormalize& curr) {
  mFn            = curr.mFn;
  mCurrField     = curr.mCurrField;
  mPhase         = curr.mPhase;
  mPrevBlockType = curr.mPrevBlockType;
  mBlockType     = curr.mBlockType;
  mThisAsParent  = curr.mThisAsParent;
}

InitNormalize::InitNormalize(LoopStmt* loop, const InitNormalize& curr) {
  mFn            = curr.mFn;
  mCurrField     = curr.mCurrField;
  mPhase         = curr.mPhase;
  mBlockType     = cBlockLoop;
  mThisAsParent  = curr.mThisAsParent;

  if (mBlockType != curr.mBlockType) {
    mPrevBlockType = curr.mBlockType;

  } else {
    mPrevBlockType = curr.mPrevBlockType;
  }
}

InitNormalize::InitNormalize(ForallStmt* loop, const InitNormalize& curr) {
  mFn            = curr.mFn;
  mCurrField     = curr.mCurrField;
  mPhase         = curr.mPhase;
  mBlockType     = cBlockForall;
  mThisAsParent  = curr.mThisAsParent;

  if (mBlockType != curr.mBlockType) {
    mPrevBlockType = curr.mBlockType;

  } else {
    mPrevBlockType = curr.mPrevBlockType;
  }
}

void InitNormalize::merge(const InitNormalize& fork) {
  mCurrField = fork.mCurrField;
  mPhase     = fork.mPhase;

  mImplicitFields.insert(fork.mImplicitFields.begin(),
                         fork.mImplicitFields.end());

  if (this->mThisAsParent == NULL) {
    this->mThisAsParent = fork.mThisAsParent;
  }
}

AggregateType* InitNormalize::type() const {
  return mFn != NULL ? toAggregateType(mFn->_this->type) : NULL;
}

FnSymbol* InitNormalize::theFn() const {
  return mFn;
}

InitNormalize::InitPhase InitNormalize::currPhase() const {
  return mPhase;
}

bool InitNormalize::isPhase0() const {
  return mPhase == cPhase0;
}

bool InitNormalize::isPhase1() const {
  return mPhase == cPhase1;
}

bool InitNormalize::isPhase2() const {
  return mPhase == cPhase2;
}

DefExpr* InitNormalize::currField() const {
  return mCurrField;
}

bool InitNormalize::isFieldReinitialized(DefExpr* field) const {
  AggregateType* at     = toAggregateType(mFn->_this->type);
  Expr*          ptr    = at->fields.head;
  bool           retval = false;

  while (ptr != NULL && ptr != mCurrField && retval == false) {
    if (field == ptr) {
      retval = true;
    } else {
      ptr    = ptr->next;
    }
  }

  INT_ASSERT(ptr != NULL);

  return retval;
}

bool InitNormalize::inLoopBody() const {
  return mBlockType == cBlockLoop;
}

bool InitNormalize::inParallelStmt() const {
  return mBlockType == cBlockBegin   ||
         mBlockType == cBlockCobegin  ;
}

bool InitNormalize::inCoforall() const {
  return mBlockType == cBlockCoforall;
}

bool InitNormalize::inForall() const {
  return mBlockType == cBlockForall;
}

bool InitNormalize::inOn() const {
  return mBlockType == cBlockOn;
}

bool InitNormalize::inOnInLoopBody() const {
  return inOn() && mPrevBlockType == cBlockLoop;
}

bool InitNormalize::inOnInParallelStmt() const {
  return inOn() && (mPrevBlockType == cBlockBegin ||
                    mPrevBlockType == cBlockCobegin);
}

bool InitNormalize::inOnInCoforall() const {
  return inOn() && mPrevBlockType == cBlockCoforall;
}

bool InitNormalize::inOnInForall() const {
  return inOn() && mPrevBlockType == cBlockForall;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void InitNormalize::completePhase1(CallExpr* initStmt) {
  if        (isThisInit(initStmt)  == true) {
    mCurrField = NULL;

  } else if (isSuperInit(initStmt) == true) {
    initializeFieldsBefore(initStmt);

  } else if (isInitDone(initStmt)  == true) {
    initializeFieldsBefore(initStmt);

  } else {
    INT_ASSERT(false);
  }

  // Allow users to treat 'this' as the initialized type
  if (type()->isClass()) {
    initStmt->insertAfter(new CallExpr(PRIM_SETCID, new SymExpr(mFn->_this)));
  }

  mPhase = cPhase2;
}

void InitNormalize::completePhase0(CallExpr* initStmt) {
  if (isSuperInit(initStmt) == true) {
    mPhase = cPhase1;
  } else {
    INT_FATAL("completePhase0 expected to be called with super.init");
  }
}

void InitNormalize::initializeFieldsAtTail(BlockStmt* block, DefExpr* endField) {
  if (mCurrField != NULL && mCurrField != endField) {
    Expr* noop = new CallExpr(PRIM_NOOP);

    block->insertAtTail(noop);

    initializeFieldsBefore(noop, endField);

    noop->remove();
  }
}

void InitNormalize::initializeFieldsBefore(Expr* insertBefore,
                                           DefExpr* endField) {
  while (mCurrField != NULL && mCurrField != endField) {
    DefExpr* field = mCurrField;

    initializeField(insertBefore, field);

    mCurrField = toDefExpr(mCurrField->next);
  }
}

void InitNormalize::initializeField(Expr* insertBefore,
                                    DefExpr* field,
                                    Expr* userInit) const {
  Expr* initExpr = userInit;
  Expr* typeExpr = field->exprType;
  Expr* ret      = NULL;

  if (initExpr == NULL && field->init != NULL) {
    initExpr = field->init->copy();
  }

  if (typeExpr == NULL && initExpr == NULL) {
    USR_FATAL_CONT(insertBefore,
                   "can't omit initialization of field \"%s\", "
                   "no type or default value provided",
                   field->sym->name);
  } else if (field->sym->hasEitherFlag(FLAG_PARAM, FLAG_TYPE_VARIABLE)) {
    if (typeExpr != NULL && initExpr == NULL) {
      ret = genericFieldInitTypeWoutInit(insertBefore, field);

    } else if (typeExpr != NULL && initExpr != NULL) {
      ret = genericFieldInitTypeWithInit(insertBefore, field, initExpr);

    } else if (typeExpr == NULL && initExpr != NULL) {
      ret = genericFieldInitTypeInference(insertBefore, field, initExpr);

    } else {
      INT_ASSERT(false);
    }
  } else if (typeExpr == NULL && field->init == NULL) {
    // Special case: initializing 'var x;' field from 'userInit'
    INT_ASSERT(userInit != NULL);
    ret = genericFieldInitTypeInference(insertBefore, field, initExpr);

  } else if (typeExpr != NULL && initExpr == NULL) {
    ret = fieldInitTypeWoutInit(insertBefore, field);

  } else if (typeExpr != NULL && initExpr != NULL) {
    ret = fieldInitTypeWithInit(insertBefore, field, initExpr);

  } else if (typeExpr == NULL && initExpr != NULL) {
    //INT_ASSERT(field->sym->type == dtUnknown);
    ret = fieldInitTypeInference(insertBefore, field, initExpr);

  } else {
    INT_ASSERT(false);
  }

  if (ret != NULL) {
    processThisUses(ret);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

Expr* InitNormalize::genericFieldInitTypeWoutInit(Expr*    insertBefore,
                                                  DefExpr* field) const {
  INT_ASSERT(field->sym->hasFlag(FLAG_PARAM));

  SET_LINENO(insertBefore);

  Type* type = field->sym->type;

  VarSymbol* tmp      = newTemp("tmp", type);
  DefExpr*   tmpDefn  = new DefExpr(tmp);
  CallExpr*  tmpInit  = new CallExpr(PRIM_DEFAULT_INIT_VAR,
                                     tmp, field->exprType->copy());

  tmp->addFlag(FLAG_PARAM);

  Symbol*    _this    = mFn->_this;
  Symbol*    name     = new_CStringSymbol(field->sym->name);
  CallExpr*  fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, tmp);

  insertBefore->insertBefore(tmpDefn);
  insertBefore->insertBefore(tmpInit);
  insertBefore->insertBefore(fieldSet);

  return tmpInit;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

Expr* InitNormalize::genericFieldInitTypeWithInit(Expr*    insertBefore,
                                                  DefExpr* field,
                                                  Expr*    initExpr) const {
  INT_ASSERT(field->sym->hasFlag(FLAG_PARAM));

  SET_LINENO(insertBefore);

  CallExpr* cast     = createCast(initExpr, field->exprType->copy());

  Symbol*   name     = new_CStringSymbol(field->sym->name);
  Symbol*   _this    = mFn->_this;

  // TODO - coerce, don't cast
  CallExpr* fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, cast);

  insertBefore->insertBefore(fieldSet);

  return cast;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

Expr* InitNormalize::genericFieldInitTypeInference(Expr*    insertBefore,
                                                   DefExpr* field,
                                                   Expr*    initExpr) const {
  bool isParam   = field->sym->hasFlag(FLAG_PARAM);
  bool isTypeVar = field->sym->hasFlag(FLAG_TYPE_VARIABLE);

  SET_LINENO(insertBefore);

  // e.g.
  //   var x = <immediate>;
  //   var y = <identifier>;
  if (isSymExpr(initExpr)) {
    if (mFn->isDefaultInit()) {
      Symbol*    name     = new_CStringSymbol(field->sym->name);
      Symbol*    _this    = mFn->_this;
      CallExpr*  fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, initExpr);

      insertBefore->insertBefore(fieldSet);

    } else {
      VarSymbol* tmp      = newTemp("tmp");
      DefExpr*   tmpDefn  = new DefExpr(tmp);
      PrimitiveTag tag    = isTypeVar ? PRIM_MOVE : PRIM_INIT_VAR;
      CallExpr*  tmpInit  = new CallExpr(tag, tmp, initExpr);

      Symbol*    _this    = mFn->_this;
      Symbol*    name     = new_CStringSymbol(field->sym->name);
      CallExpr*  fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, tmp);

      if (isParam) {
        tmp->addFlag(FLAG_PARAM);
      } else if (isTypeVar) {
        tmp->addFlag(FLAG_TYPE_VARIABLE);
      }

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);
    }

  // e.g.
  //   var x = f(...);
  //   var y = new MyRecord(...);
  } else if (isUnresolvedSymExpr(initExpr)) {
    // Don't worry too much about it, resolution will handle this.
    // TODO: add comment with example of this case
    Symbol*   _this    = mFn->_this;
    Symbol*   name     = new_CStringSymbol(field->sym->name);
    CallExpr* fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, initExpr);

    insertBefore->insertBefore(fieldSet);

  } else {
    if (CallExpr* initCall = toCallExpr(initExpr)) {
      if ((isParam || isTypeVar) && initCall->isPrimitive(PRIM_NEW) == true) {
        const char* kind = isTypeVar ? "type" : "param";
        USR_FATAL(initExpr,
                  "Cannot initialize %s field '%s' with 'new' expression",
                  kind, field->sym->name);
      }
    }

    VarSymbol* tmp      = newTemp("tmp");
    DefExpr*   tmpDefn  = new DefExpr(tmp);
    PrimitiveTag tag    = isTypeVar ? PRIM_MOVE : PRIM_INIT_VAR;
    CallExpr*  tmpInit  = new CallExpr(tag, tmp, initExpr);

    Symbol*    _this    = mFn->_this;
    Symbol*    name     = new_CStringSymbol(field->sym->name);
    CallExpr*  fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, tmp);

    if (isParam) {
      tmp->addFlag(FLAG_PARAM);
    } else if (isTypeVar) {
      tmp->addFlag(FLAG_TYPE_VARIABLE);
    }

    insertBefore->insertBefore(tmpDefn);
    insertBefore->insertBefore(tmpInit);
    insertBefore->insertBefore(fieldSet);

  }

  return initExpr;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

Expr* InitNormalize::fieldInitTypeWoutInit(Expr*    insertBefore,
                                           DefExpr* field) const {

  if (field->sym->hasFlag(FLAG_NO_INIT))
    return NULL;

  SET_LINENO(insertBefore);

  Type* type = field->sym->type;

  VarSymbol* tmp      = newTemp("tmp", type);
  DefExpr*   tmpDefn  = new DefExpr(tmp);
  CallExpr*  tmpInit  = new CallExpr(PRIM_DEFAULT_INIT_VAR,
                                     tmp, field->exprType->copy());

  Symbol*    _this    = mFn->_this;
  Symbol*    name     = new_CStringSymbol(field->sym->name);
  CallExpr*  fieldSet = new CallExpr(PRIM_SET_MEMBER, _this, name, tmp);

  insertBefore->insertBefore(tmpDefn);
  insertBefore->insertBefore(tmpInit);
  insertBefore->insertBefore(fieldSet);

  return tmpInit;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

Expr* InitNormalize::fieldInitTypeWithInit(Expr*    insertBefore,
                                           DefExpr* field,
                                           Expr*    initExpr) const {
  SET_LINENO(insertBefore);

  Type* type = field->sym->type;

  if (mFn->isDefaultInit()) {
    // For default-initializers, copy happens at the callsite
    Symbol* _this = mFn->_this;
    Symbol* name = new_CStringSymbol(field->sym->name);
    CallExpr* fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, initExpr);

    insertBefore->insertBefore(fieldSet);

    return initExpr;

  } else {
    // Do not set type of 'tmp' so that resolution will infer it later
    VarSymbol* tmp       = newTemp("tmp");
    DefExpr*   tmpDefn   = new DefExpr(tmp);
    Expr*      checkType = NULL;

    if (field->exprType == NULL) {
      checkType = new SymExpr(type->symbol);
    } else {
      checkType = field->exprType->copy();
    }

    // Set the value for TMP
    CallExpr*  tmpInit = new CallExpr(PRIM_INIT_VAR,
                                      tmp,  initExpr, checkType);

    Symbol*    _this     = mFn->_this;
    Symbol*    name      = new_CStringSymbol(field->sym->name);
    // Calling PRIM_INIT_FIELD here instead of PRIM_SET_MEMBER
    // helps with classes/ferguson/generic-field - check that
    // test if it is revisited.
    CallExpr*  fieldSet  = new CallExpr(PRIM_INIT_FIELD, _this, name, tmp);

    insertBefore->insertBefore(tmpDefn);
    insertBefore->insertBefore(tmpInit);
    insertBefore->insertBefore(fieldSet);

    return tmpInit;
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

Expr* InitNormalize::fieldInitTypeInference(Expr*    insertBefore,
                                            DefExpr* field,
                                            Expr*    initExpr) const {
  SET_LINENO(insertBefore);

  if (mFn->isDefaultInit()) {
    Symbol* _this = mFn->_this;
    Symbol* name = new_CStringSymbol(field->sym->name);
    CallExpr* fieldSet = new CallExpr(PRIM_SET_MEMBER, _this, name, initExpr);

    insertBefore->insertBefore(fieldSet);

  } else {
    VarSymbol* tmp      = newTemp("tmp");
    DefExpr*   tmpDefn  = new DefExpr(tmp);
    CallExpr*  tmpInit  = new CallExpr(PRIM_INIT_VAR, tmp, initExpr);

    Symbol*    _this    = mFn->_this;
    Symbol*    name     = new_CStringSymbol(field->sym->name);
    CallExpr*  fieldSet = new CallExpr(PRIM_SET_MEMBER, _this, name, tmp);

    insertBefore->insertBefore(tmpDefn);
    insertBefore->insertBefore(tmpInit);
    insertBefore->insertBefore(fieldSet);
  }

  return initExpr;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

DefExpr* InitNormalize::toLocalField(SymExpr* expr) const {
  return type()->toLocalField(expr);
}

DefExpr* InitNormalize::toLocalField(CallExpr* expr) const {
  return type()->toLocalField(expr);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

DefExpr* InitNormalize::toSuperField(SymExpr* expr) const {
  return type()->toSuperField(expr);
}

DefExpr* InitNormalize::toSuperField(AggregateType* at,
                                     const char*    name) const {
  forv_Vec(AggregateType, pt, at->dispatchParents) {
    if (DefExpr* field = pt->toLocalField(name)) {
      return field;
    }
  }

  return NULL;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool InitNormalize::isFieldInitialized(const DefExpr* field) const {
  const DefExpr* ptr    = mCurrField;
  bool           retval = true;

  while (ptr != NULL && retval == true) {
    if (ptr == field) {
      retval = false;
    } else {
      ptr = toConstDefExpr(ptr->next);
    }
  }

  return retval;
}

bool InitNormalize::isFieldImplicitlyInitialized(DefExpr* field) const {
  return mImplicitFields.find(field) != mImplicitFields.end();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

InitNormalize::InitPhase InitNormalize::startPhase(FnSymbol*  fn)    const {
  return startPhase(fn->body);
}

InitNormalize::InitPhase InitNormalize::startPhase(BlockStmt* block) const {
  Expr*     stmt   = block->body.head;
  const InitPhase defaultPhase = cPhase1;
  InitPhase retval = defaultPhase;

  while (stmt != NULL && retval == defaultPhase) {
    if (isDefExpr(stmt) == true) {
      stmt = stmt->next;

    } else if (CallExpr* callExpr = toCallExpr(stmt)) {
      if        (isThisInit(callExpr)  == true) {
        retval = cPhase0;
        break;

      } else if (isSuperInit(callExpr) == true) {
        retval = cPhase0;
        break;

      } else if (isInitDone(callExpr)  == true) {
        retval = cPhase1;
        break;

      } else {
        stmt   = stmt->next;
      }

    } else if (CondStmt* cond = toCondStmt(stmt)) {
      if (cond->elseStmt == NULL) {
        InitPhase thenPhase = startPhase(cond->thenStmt);

        if (thenPhase != defaultPhase) {
          retval = thenPhase;
        } else {
          stmt   = stmt->next;
        }

      } else {
        InitPhase thenPhase = startPhase(cond->thenStmt);
        InitPhase elsePhase = startPhase(cond->elseStmt);

        if        (thenPhase == cPhase0 || elsePhase == cPhase0) {
          retval = cPhase0;

        } else if (thenPhase == cPhase1 || elsePhase == cPhase1) {
          retval = cPhase1;
          // We want to keep looking in case there's a super.init further
          // down.
          stmt = stmt->next;

        } else {
          stmt   = stmt->next;
        }
      }

    } else if (BlockStmt* block = toBlockStmt(stmt)) {
      InitPhase phase = startPhase(block);

      if (phase != defaultPhase) {
        retval = phase;
      } else {
        stmt   = stmt->next;
      }

    } else if (ForallStmt* forall = toForallStmt(stmt)) {
      // Nothing to normalize in fRecIter*
      INT_ASSERT(forall->fRecIterIRdef == NULL);

      InitPhase phase = startPhase(forall->loopBody());

      if (phase != defaultPhase) {
        retval = phase;
      } else {
        stmt   = stmt->next;
      }

    } else {
      stmt = stmt->next;
    }
  }

  return retval;
}

//
// Catch case a case like:
//
// proc init(cond:bool) {
//   if cond {
//     this.initDone();
//   } else {
//     this.init();
//   }
// }
//
// The initializer begins in phase zero, which is the state we inherit upon
// entering the 'then' branch of the conditional statement. This is the
// incorrect phase though, so we need to re-check the start phase of the
// 'then' block and use that instead.
//
// Note though that we don't want to update the state unless there is an
// initDone. Otherwise simple conditionals like this would advance the outer
// phase to phase 1:
//
// proc init(cond:bool) {
//   if cond {
//     writeln("then");
//   } else {
//     writeln("else");
//   }
//   super.init();
// }
//
// INIT TODO: Can we restrict this operation to just conditionals?
//
void InitNormalize::checkPhase(BlockStmt* block) {
  if (mPhase == cPhase0) {
    InitPhase newPhase = startPhase(block);

    if (newPhase == cPhase1) {
      if (hasInitDone(block)) {
        mPhase = newPhase;
      }
    }
  }
}

DefExpr* InitNormalize::firstField(FnSymbol* fn) const {
  AggregateType* at     = toAggregateType(fn->_this->type);
  DefExpr*       retval = toDefExpr(at->fields.head);

  // Skip the pseudo-field "super"
  if (at->isClass() == true && at->symbol->hasFlag(FLAG_NO_OBJECT) == false) {

    if (at->isGeneric() == true) {
      AggregateType* pt = toAggregateType(retval->sym->type);

      INT_ASSERT(pt);

      if (pt->isGeneric() == true) {
        // If the super type is generic, label it so that we can
        // handle that appropriately during initializer resolution
        retval->sym->addFlag(FLAG_DELAY_GENERIC_EXPANSION);
      }
    }

    retval = toDefExpr(retval->next);
  }

  return retval;
}

//
// Initialize the 'mThisAsParent' field
//
// mThisAsParent points to the same data as 'this', but has the parent type
// of 'this'. This method also inserts a call to set the class ID of
// mThisAsParent (and 'this') so that we only dynamically dispatch methods to
// the parent.
//
void InitNormalize::makeThisAsParent(CallExpr* call) {
  if (type()->symbol->hasFlag(FLAG_NO_OBJECT)) {
    return;
  }

  // type parentType = this.super.type;
  // var thisAsParent : parentType = (_cast parentType this);

  INT_ASSERT(this->mThisAsParent == NULL);

  VarSymbol* parentType = newTemp();
  DefExpr*   parentDef  = new DefExpr(parentType);
  parentType->addFlag(FLAG_TYPE_VARIABLE);

  // Let resolution figure out the type for us
  // (move parentType (typeof (.v this super)))
  CallExpr* getSuper = new CallExpr(PRIM_GET_MEMBER_VALUE, mFn->_this, new_CStringSymbol("super"));
  CallExpr* typeInit = new CallExpr(PRIM_MOVE, parentType,
                                    new CallExpr(PRIM_TYPEOF, getSuper));

  // Important to use PRIM_CAST here, otherwise resolution will complain about
  // 'this' having an unknown type if the class is generic.
  VarSymbol* thisAsParent = newTemp("chpl__thisAsParent");
  SymExpr*   tapType      = new SymExpr(parentType);
  CallExpr*  tapInit      = new CallExpr(PRIM_CAST, parentType, new SymExpr(mFn->_this));
  DefExpr*   tapDef       = new DefExpr(thisAsParent, tapInit, tapType);

  CallExpr* setCID = new CallExpr(PRIM_SETCID, thisAsParent);

  call->insertAfter(setCID);
  call->insertAfter(tapDef);
  call->insertAfter(typeInit);
  call->insertAfter(parentDef);

  this->mThisAsParent = thisAsParent;
}

VarSymbol* InitNormalize::getThisAsParent() const {
  return mThisAsParent;
}

static bool isThisDot(CallExpr* call) {
  bool retval = false;

  if (call->isNamedAstr(astrSdot) == true) {
    SymExpr* base = toSymExpr(call->get(1));
    if (base != NULL &&
        base->symbol()->hasFlag(FLAG_ARG_THIS)) {
      retval = true;
    }
  }
  
  return retval;
}

static bool isMethodCall(CallExpr* call) {
  bool retval = false;

  if (CallExpr* parent = toCallExpr(call->parentExpr)) {
    if (parent->baseExpr == call) {
      retval = true;
    }
  }

  return retval;
}

//
// Returns true if 'type' or its parents have a method named 'methodName'
//
// Note: Assumes only one entry in dispatchParents.
//
static bool typeHasMethod(AggregateType* type, const char* methodName) {
  bool retval = false;

  INT_ASSERT(type != NULL);

  if (type != dtObject) {
    forv_Vec(FnSymbol, method, type->methods) {
      if (method && strcmp(method->name, methodName) == 0) {
        retval = true;
        break;
      }
    }

    if (retval == false && isClass(type)) {
      AggregateType* parent = type->dispatchParents.v[0];
      retval = typeHasMethod(parent, methodName);
    }
  }

  return retval;
}

class ProcessThisUses : public AstVisitorTraverse
{
  public:
    ProcessThisUses(const InitNormalize* state) {
      this->state = state;
    }
    virtual ~ProcessThisUses() { }

    virtual void visitSymExpr(SymExpr* node);
    virtual bool enterCallExpr(CallExpr* node);
    virtual bool enterFnSym(FnSymbol* node);

  private:
    const InitNormalize* state;
};

void ProcessThisUses::visitSymExpr(SymExpr* node) {
  DefExpr* field = NULL;

  if (node->symbol()->hasFlag(FLAG_ARG_THIS)) {
    if (DefExpr* parentDef = toDefExpr(node->parentExpr)) {
      if (parentDef->sym->hasFlag(FLAG_REF_VAR)) {
        USR_FATAL_CONT(node, "cannot take a reference to \"this\" before this.complete()");
      } else {
        USR_FATAL_CONT(node, "cannot initialize a variable from \"this\" before this.complete()");
      }
    } else {
      CallExpr* call = NULL;
      Expr* cur = node;
      while (call == NULL && cur->parentExpr != NULL) {
        if (CallExpr* parent = toCallExpr(cur->parentExpr)) {
          call = parent;
        } else {
          cur = cur->parentExpr;
        }
      }

      if (call && call->isPrimitive() == false) {
        if (state->isPhase0()) {
          USR_FATAL_CONT(node, "cannot pass \"this\" to a function before calling super.init() or this.init()");
        } else if (state->type()->isRecord()) {
          USR_FATAL_CONT(node, "cannot pass a record to a function before this.complete()");
        }
      }

      if (isClass(state->type())) {
        node->setSymbol(state->getThisAsParent());
      }
    }
  } else if (DefExpr* local = state->type()->toLocalField(node)) {
    field = local;
    if (state->isFieldInitialized(field) == false) {
      USR_FATAL_CONT(node,
                     "field \"%s\" used before it is initialized",
                     field->sym->name);
    }
  } else if (DefExpr* super = state->type()->toSuperField(node)) {
    field = super;
    if (state->isPhase0()) {
      USR_FATAL_CONT(node,
                     "cannot access parent field \"%s\" before super.init() or this.init()",
                     field->sym->name);
    }
  }

  if (field != NULL) {
    PrimitiveTag tag = PRIM_GET_MEMBER;
    if (field->sym->hasEitherFlag(FLAG_PARAM, FLAG_TYPE_VARIABLE)) {
      tag = PRIM_GET_MEMBER_VALUE;
    }

    node->replace(new CallExpr(tag,
                               state->theFn()->_this,
                               new_CStringSymbol(field->sym->name)));

  }
}

bool ProcessThisUses::enterCallExpr(CallExpr* node) {
  AggregateType* type = state->type();
  if (DefExpr* field = type->toLocalField(node)) {
    if (state->isFieldInitialized(field) == false) {
      USR_FATAL_CONT(node,
                     "field \"%s\" used before it is initialized",
                     field->sym->name);
    } else if (isCallExpr(node->parentExpr)) {
      // this.myField.something
      if (typeHasMethod(type, field->sym->name)) {
        USR_FATAL_CONT(node, "cannot call field-accessor method \"%s\" before this.complete()", field->sym->name);
      }
      node->baseExpr->remove();
      node->baseExpr = NULL;
      node->primitive = primitives[PRIM_GET_MEMBER];

    }
    return false;
  } else if (DefExpr* field = type->toSuperField(node)) {
    if (state->isPhase0()) {
      USR_FATAL_CONT(node,
                     "cannot access parent field \"%s\" before super.init() or this.init()",
                     field->sym->name);
    } else if (state->isPhase1()) {
      node->baseExpr->remove();
      node->baseExpr = NULL;
      node->primitive = primitives[PRIM_GET_MEMBER];
    }
    return false;
  } else if (isAssignment(node)) {
    if (CallExpr* LHS = toCallExpr(node->get(1))) {
      if (isThisDot(LHS)) {
        if (LHS->square == false) {
          node->get(1)->accept(this);
          // Regular 'this.x = ' , just look at the RHS
          node->get(2)->accept(this);
          return false;
        }
      }
    }
  } else if (isThisDot(node) && isMethodCall(node)) {
    if (state->isPhase0()) {
      USR_FATAL_CONT(node, "cannot call a method before super.init() or this.init()");
      return false;
    } else if (type->isRecord()) {
      USR_FATAL_CONT(node, "cannot call a method on a record before this.complete()");
      return false;
    } else {
      Immediate*     imm        = getSymbolImmediate(toSymExpr(node->get(2))->symbol());
      const char*    methodName = imm->string_value();
      AggregateType* parentType = type->dispatchParents.v[0];

      if (typeHasMethod(parentType, methodName) == false) {
        USR_FATAL_CONT(node, "cannot call method \"%s\" on type \"%s\" before this.complete()", methodName, type->symbol->name);
        USR_PRINT(node, "before this.complete() \"this\" is treated as parent type \"%s\" for method calls", parentType->symbol->name);
        return false;
      }
    }
  } else if (node->isPrimitive(PRIM_CAST) || node->isNamedAstr(astr_cast)) {
    if (SymExpr* se = toSymExpr(node->get(2))) {
      if (se->symbol()->hasFlag(FLAG_ARG_THIS)) {
        USR_FATAL_CONT(node, "cannot cast \"this\" before this.complete()");
        return false;
      }
    }
  }

  return true;
}

bool ProcessThisUses::enterFnSym(FnSymbol* node) {
  // Skip nested functions for now
  return false;
}

void InitNormalize::processThisUses(Expr* expr) const {
  if (isPhase2() != true) {
    ProcessThisUses ptu(this);
    expr->accept(&ptu);
  }
}

Expr* InitNormalize::fieldInitFromInitStmt(DefExpr*  field,
                                           CallExpr* initStmt) {
  Expr* retval = NULL;

  if (field != mCurrField) {
    INT_ASSERT(isFieldReinitialized(field) == false);

    while (field != mCurrField) {
      initializeField(initStmt, mCurrField);
      mImplicitFields.insert(mCurrField);

      mCurrField = toDefExpr(mCurrField->next);
    }
  }

  Expr* initExpr = initStmt->get(2)->remove();
  retval         = initStmt->next;

  initializeField(initStmt, field, initExpr);
  initStmt->remove();

  mCurrField = toDefExpr(mCurrField->next);

  return retval;
}


void InitNormalize::describe(int offset) const {
  char pad[512];

  for (int i = 0; i < offset; i++) {
    pad[i] = ' ';
  }

  pad[offset] = '\0';

  printf("%s#<InitNormalize\n", pad);

  printf("%s  Phase: %s\n", pad, phaseToString(mPhase));

  printf("%s  Block: ",       pad);

  switch (mBlockType) {
    case cBlockNormal:
      printf("normal\n");
      break;

    case cBlockLoop:
      printf("loop\n");
      break;

    case cBlockBegin:
      printf("begin\n");
      break;

    case cBlockCobegin:
      printf("cobegin\n");
      break;

    case cBlockCoforall:
      printf("coforall\n");
      break;

    case cBlockForall:
      printf("forall\n");
      break;

    case cBlockOn:
      printf("on\n");
      break;
  }

  printf("%s>\n", pad);
}

const char*
InitNormalize::phaseToString(InitNormalize::InitPhase phase) const {
  const char* retval = "?";

  switch (phase) {
    case cPhase0:
      retval = "Phase0";
      break;

    case cPhase1:
      retval = "Phase1";
      break;

    case cPhase2:
      retval = "Phase2";
      break;
  }

  return retval;
}

static bool isAssignment(CallExpr* callExpr) {
  bool retval = false;

  if (isSimpleAssignment(callExpr)   == true ||
      isCompoundAssignment(callExpr) == true) {
    retval = true;
  }

  return retval;
}

static bool isSimpleAssignment(CallExpr* callExpr) {
  bool retval = false;

  if (callExpr->isNamedAstr(astrSassign) == true) {
    retval = true;
  }

  return retval;
}

static bool isCompoundAssignment(CallExpr* callExpr) {
  bool retval = false;

  if (callExpr->isNamed("+=")  == true ||
      callExpr->isNamed("-=")  == true ||
      callExpr->isNamed("*=")  == true ||
      callExpr->isNamed("/=")  == true ||
      callExpr->isNamed("**=") == true ||
      callExpr->isNamed("%=")  == true ||
      callExpr->isNamed("&=")  == true ||
      callExpr->isNamed("|=")  == true ||
      callExpr->isNamed("^=")  == true ||
      callExpr->isNamed("&&=") == true ||
      callExpr->isNamed("||=") == true ||
      callExpr->isNamed("<<=") == true ||
      callExpr->isNamed(">>=") == true) {
    retval = true;
  }

  return retval;
}
