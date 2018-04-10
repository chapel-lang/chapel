/*
 * Copyright 2004-2018 Cray Inc.
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

#include "ForallStmt.h"
#include "initializerRules.h"
#include "stmt.h"
#include "astutil.h"

static bool mightBeSyncSingleExpr(DefExpr* field);

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

    } else if (blockInfo->isPrimitive(PRIM_BLOCK_ON) == true) {
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

void InitNormalize::initializeFieldsAtTail(BlockStmt* block) {
  if (mCurrField != NULL) {
    Expr* noop = new CallExpr(PRIM_NOOP);

    block->insertAtTail(noop);

    initializeFieldsBefore(noop);

    noop->remove();
  }
}

void InitNormalize::initializeFieldsThroughField(BlockStmt* block,
                                                 DefExpr*   field) {
  Expr* insertBefore = new CallExpr(PRIM_NOOP);

  block->insertAtTail(insertBefore);

  Expr* endCondition = ((field == NULL)? NULL : field);

  while (mCurrField != NULL && mCurrField != endCondition) {
    DefExpr* field = mCurrField;

    if (isOuterField(field) == true) {
      // The outer field is a compiler generated field.  Handle it specially.
      makeOuterArg();

    } else {
      if (field->exprType == NULL && field->init == NULL) {
        USR_FATAL_CONT(insertBefore,
                       "can't omit initialization of field \"%s\", "
                       "no type or default value provided",
                       field->sym->name);

      } else if (field->sym->hasFlag(FLAG_PARAM)         == true ||
                 field->sym->hasFlag(FLAG_TYPE_VARIABLE) == true) {
        if        (field->exprType != NULL && field->init == NULL) {
          genericFieldInitTypeWoutInit (insertBefore, field);

        } else if (field->exprType != NULL && field->init != NULL) {
          genericFieldInitTypeWithInit (insertBefore,
                                        field,
                                        field->init->copy());

        } else if (field->exprType == NULL && field->init != NULL) {
          genericFieldInitTypeInference(insertBefore,
                                        field,
                                        field->init->copy());

        } else {
          INT_ASSERT(false);
        }

      } else if (field->init != NULL) {
        Expr* initCopy    = field->init->copy();
        bool  isTypeKnown = mCurrField->sym->type != dtUnknown;

        if (isTypeKnown == true) {
          fieldInitTypeWithInit (insertBefore, field, initCopy);

        } else if (field->exprType != NULL) {
          fieldInitTypeWithInit (insertBefore, field, initCopy);

        } else {
          fieldInitTypeInference(insertBefore, field, initCopy);
        }

      } else {
        fieldInitTypeWoutInit(insertBefore, field);
      }
    }

    mCurrField = toDefExpr(mCurrField->next);
  }

  insertBefore->remove();
}

void InitNormalize::initializeFieldsBefore(Expr* insertBefore) {
  while (mCurrField != NULL) {
    DefExpr* field = mCurrField;

    if (isOuterField(field) == true) {
      // The outer field is a compiler generated field.  Handle it specially.
      makeOuterArg();

    } else {
      if (field->exprType == NULL && field->init == NULL) {
        USR_FATAL_CONT(insertBefore,
                       "can't omit initialization of field \"%s\", "
                       "no type or default value provided",
                       field->sym->name);

      } else if (field->sym->hasFlag(FLAG_PARAM)         == true ||
                 field->sym->hasFlag(FLAG_TYPE_VARIABLE) == true) {
        if        (field->exprType != NULL && field->init == NULL) {
          genericFieldInitTypeWoutInit (insertBefore, field);

        } else if (field->exprType != NULL && field->init != NULL) {
          genericFieldInitTypeWithInit (insertBefore,
                                        field,
                                        field->init->copy());

        } else if (field->exprType == NULL && field->init != NULL) {
          genericFieldInitTypeInference(insertBefore,
                                        field,
                                        field->init->copy());

        } else {
          INT_ASSERT(false);
        }

      } else if (field->init != NULL) {
        Expr* initCopy    = field->init->copy();
        bool  isTypeKnown = mCurrField->sym->type != dtUnknown;

        if (isTypeKnown == true) {
          fieldInitTypeWithInit (insertBefore, field, initCopy);

        } else if (field->exprType != NULL) {
          fieldInitTypeWithInit (insertBefore, field, initCopy);

        } else {
          fieldInitTypeInference(insertBefore, field, initCopy);
        }

      } else {
        fieldInitTypeWoutInit(insertBefore, field);
      }
    }

    mCurrField = toDefExpr(mCurrField->next);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void InitNormalize::genericFieldInitTypeWoutInit(Expr*    insertBefore,
                                                 DefExpr* field) const {
  INT_ASSERT(field->sym->hasFlag(FLAG_PARAM));

  SET_LINENO(insertBefore);

  Type* type = field->sym->type;

  if (isPrimitiveScalar(type) == true ||
      isNonGenericClass(type) == true) {
    VarSymbol* tmp      = newTemp("tmp", type);
    DefExpr*   tmpDefn  = new DefExpr(tmp);
    CallExpr*  tmpExpr  = new CallExpr("_defaultOf", type->symbol);
    CallExpr*  tmpInit  = new CallExpr(PRIM_MOVE, tmp, tmpExpr);

    tmp->addFlag(FLAG_PARAM);

    Symbol*    name     = new_CStringSymbol(field->sym->name);
    Symbol*    _this    = mFn->_this;
    CallExpr*  fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, tmp);

    insertBefore->insertBefore(tmpDefn);
    insertBefore->insertBefore(tmpInit);
    insertBefore->insertBefore(fieldSet);

  } else if (isNonGenericRecordWithInitializers(type) == true) {
    VarSymbol* tmp      = newTemp("tmp", type);
    DefExpr*   tmpDefn  = new DefExpr(tmp);
    CallExpr*  tmpInit  = new CallExpr("init", gMethodToken, tmp);

    tmp->addFlag(FLAG_PARAM);

    Symbol*    name     = new_CStringSymbol(field->sym->name);
    Symbol*    _this    = mFn->_this;
    CallExpr*  fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, tmp);

    insertBefore->insertBefore(tmpDefn);
    insertBefore->insertBefore(tmpInit);
    insertBefore->insertBefore(fieldSet);

  } else {
    VarSymbol* tmp      = newTemp("tmp", type);
    DefExpr*   tmpDefn  = new DefExpr(tmp);
    CallExpr*  tmpExpr  = new CallExpr(PRIM_INIT, field->exprType->copy());
    CallExpr*  tmpInit  = new CallExpr(PRIM_MOVE, tmp, tmpExpr);

    tmp->addFlag(FLAG_PARAM);

    Symbol*    _this    = mFn->_this;
    Symbol*    name     = new_CStringSymbol(field->sym->name);
    CallExpr*  fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, tmp);

    if (isFieldAccessible(tmpExpr) == false) {
      INT_ASSERT(false);
    }

    insertBefore->insertBefore(tmpDefn);
    insertBefore->insertBefore(tmpInit);
    insertBefore->insertBefore(fieldSet);

    updateFieldsMember(tmpExpr);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool isNewExpr(Expr* expr);

void InitNormalize::genericFieldInitTypeWithInit(Expr*    insertBefore,
                                                 DefExpr* field,
                                                 Expr*    initExpr) const {
  INT_ASSERT(field->sym->hasFlag(FLAG_PARAM));

  SET_LINENO(insertBefore);

  CallExpr* cast     = createCast(initExpr, field->exprType->copy());

  Symbol*   name     = new_CStringSymbol(field->sym->name);
  Symbol*   _this    = mFn->_this;

  CallExpr* fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, cast);

  if (isFieldAccessible(initExpr) == false) {
    INT_ASSERT(false);
  }

  insertBefore->insertBefore(fieldSet);

  updateFieldsMember(initExpr);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void InitNormalize::genericFieldInitTypeInference(Expr*    insertBefore,
                                                  DefExpr* field,
                                                  Expr*    initExpr) const {
  bool isParam   = field->sym->hasFlag(FLAG_PARAM);
  bool isTypeVar = field->sym->hasFlag(FLAG_TYPE_VARIABLE);

  SET_LINENO(insertBefore);

  // e.g.
  //   var x = <immediate>;
  //   var y = <identifier>;
  if (SymExpr* initSym = toSymExpr(initExpr)) {
    Type* type = initSym->symbol()->type;

    if (isTypeVar == true) {
      VarSymbol* tmp = NULL;

      if (type == dtAny) {
        tmp = newTemp("tmp");
      } else {
        tmp = newTemp("tmp", type);
      }

      DefExpr*  tmpDefn  = new DefExpr(tmp);
      CallExpr* tmpInit  = new CallExpr(PRIM_MOVE, tmp, initExpr);
      Symbol*   _this    = mFn->_this;
      Symbol*   name     = new_CStringSymbol(field->sym->name);
      CallExpr* fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, tmp);

      tmp->addFlag(FLAG_TYPE_VARIABLE);

      if (isFieldAccessible(initExpr) == false) {
        INT_ASSERT(false);
      }

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);

      updateFieldsMember(initExpr);

    } else if (isPrimitiveScalar(type) == true) {
      VarSymbol* tmp      = newTemp("tmp", type);
      DefExpr*   tmpDefn  = new DefExpr(tmp);
      CallExpr*  tmpInit  = new CallExpr(PRIM_MOVE, tmp, initExpr);

      Symbol*    name     = new_CStringSymbol(field->sym->name);
      Symbol*    _this    = mFn->_this;
      CallExpr*  fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, tmp);

      if (isParam == true) {
        tmp->addFlag(FLAG_PARAM);
      }

      if (isFieldAccessible(initExpr) == false) {
        INT_ASSERT(false);
      }

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);

      updateFieldsMember(initExpr);

    } else {
      VarSymbol* tmp      = newTemp("tmp");
      DefExpr*   tmpDefn  = new DefExpr(tmp);
      CallExpr*  tmpInit  = new CallExpr(PRIM_INIT_VAR, tmp, initExpr);

      Symbol*    _this    = mFn->_this;
      Symbol*    name     = new_CStringSymbol(field->sym->name);
      CallExpr*  fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, tmp);

      if (isParam == true) {
        tmp->addFlag(FLAG_PARAM);
      }

      if (isFieldAccessible(initExpr) == false) {
        INT_ASSERT(false);
      }

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);

      updateFieldsMember(initExpr);
    }

  // e.g.
  //   var x = f(...);
  //   var y = new MyRecord(...);
  } else if (CallExpr* initCall = toCallExpr(initExpr)) {
    if ((isParam || isTypeVar) && initCall->isPrimitive(PRIM_NEW) == true) {
      if (isTypeVar == true) {
        USR_FATAL(initExpr,
                  "Cannot initialize type field '%s' with 'new' expression",
                  field->sym->name);

      } else {
        INT_ASSERT(isParam == true);

        USR_FATAL(initExpr,
                  "Cannot initialize param field '%s' with 'new' expression",
                  field->sym->name);
      }

    } else if (isTypeVar == true) {
      VarSymbol* tmp      = newTemp("tmp");
      DefExpr*   tmpDefn  = new DefExpr(tmp);
      CallExpr*  tmpInit  = new CallExpr(PRIM_MOVE, tmp, initExpr);

      tmp->addFlag(FLAG_TYPE_VARIABLE);

      Symbol*    _this    = mFn->_this;
      Symbol*    name     = new_CStringSymbol(field->sym->name);
      CallExpr*  fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, tmp);

      if (isFieldAccessible(initExpr) == false) {
        INT_ASSERT(false);
      }

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);

      updateFieldsMember(initExpr);

    } else {
      VarSymbol* tmp      = newTemp("tmp");
      DefExpr*   tmpDefn  = new DefExpr(tmp);
      CallExpr*  tmpInit  = new CallExpr(PRIM_INIT_VAR, tmp, initExpr);

      Symbol*    _this    = mFn->_this;
      Symbol*    name     = new_CStringSymbol(field->sym->name);
      CallExpr*  fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, tmp);

      if (isParam == true) {
        tmp->addFlag(FLAG_PARAM);
      }

      if (isFieldAccessible(initExpr) == false) {
        INT_ASSERT(false);
      }

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);

      updateFieldsMember(initExpr);
    }

  } else if (isUnresolvedSymExpr(initExpr)) {
    // Don't worry too much about it, resolution will handle this.
    Symbol*   _this    = mFn->_this;
    Symbol*   name     = new_CStringSymbol(field->sym->name);
    CallExpr* fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, initExpr);

    insertBefore->insertBefore(fieldSet);

  } else {
    INT_ASSERT(false);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void InitNormalize::fieldInitTypeWoutInit(Expr*    insertBefore,
                                          DefExpr* field) const {
  SET_LINENO(insertBefore);

  Type* type = field->sym->type;

  if (isPrimitiveScalar(type) == true ||
      isNonGenericClass(type) == true) {
    VarSymbol* tmp      = newTemp("tmp", type);
    DefExpr*   tmpDefn  = new DefExpr(tmp);
    CallExpr*  tmpExpr  = new CallExpr("_defaultOf", type->symbol);
    CallExpr*  tmpInit  = new CallExpr(PRIM_MOVE, tmp, tmpExpr);

    Symbol*    name     = new_CStringSymbol(field->sym->name);
    Symbol*    _this    = mFn->_this;
    CallExpr*  fieldSet = new CallExpr(PRIM_SET_MEMBER, _this, name, tmp);

    insertBefore->insertBefore(tmpDefn);
    insertBefore->insertBefore(tmpInit);
    insertBefore->insertBefore(fieldSet);

  } else if (isNonGenericRecordWithInitializers(type) == true) {
    VarSymbol* tmp      = newTemp("tmp", type);
    DefExpr*   tmpDefn  = new DefExpr(tmp);
    CallExpr*  tmpInit  = new CallExpr("init", gMethodToken, tmp);

    Symbol*    name     = new_CStringSymbol(field->sym->name);
    Symbol*    _this    = mFn->_this;
    CallExpr*  fieldSet = new CallExpr(PRIM_SET_MEMBER, _this, name, tmp);

    insertBefore->insertBefore(tmpDefn);
    insertBefore->insertBefore(tmpInit);
    insertBefore->insertBefore(fieldSet);

  } else {
    VarSymbol* tmp      = newTemp("tmp", type);
    DefExpr*   tmpDefn  = new DefExpr(tmp);
    CallExpr*  tmpExpr  = new CallExpr(PRIM_INIT, field->exprType->copy());
    CallExpr*  tmpInit  = new CallExpr(PRIM_MOVE, tmp, tmpExpr);

    Symbol*    _this    = mFn->_this;
    Symbol*    name     = new_CStringSymbol(field->sym->name);
    CallExpr*  fieldSet = new CallExpr(PRIM_SET_MEMBER, _this, name, tmp);

    if (isFieldAccessible(tmpExpr) == false) {
      INT_ASSERT(false);
    }

    insertBefore->insertBefore(tmpDefn);
    insertBefore->insertBefore(tmpInit);
    insertBefore->insertBefore(fieldSet);

    updateFieldsMember(tmpExpr);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void InitNormalize::fieldInitTypeWithInit(Expr*    insertBefore,
                                          DefExpr* field,
                                          Expr*    initExpr) const {
  SET_LINENO(insertBefore);

  Type* type = field->sym->type;

  if (isPrimitiveScalar(type) == true ||
      isNonGenericClass(type) == true) {
    VarSymbol* tmp      = newTemp("tmp", type);
    DefExpr*   tmpDefn  = new DefExpr(tmp);
    CallExpr*  tmpInit  = new CallExpr("=", tmp, initExpr);

    Symbol*    name     = new_CStringSymbol(field->sym->name);
    Symbol*    _this    = mFn->_this;
    CallExpr*  fieldSet = new CallExpr(PRIM_SET_MEMBER, _this, name, tmp);

    if (isFieldAccessible(initExpr) == false) {
      INT_ASSERT(false);
    }

    insertBefore->insertBefore(tmpDefn);
    insertBefore->insertBefore(tmpInit);
    insertBefore->insertBefore(fieldSet);

    updateFieldsMember(initExpr);

  } else if (isNonGenericRecordWithInitializers(type) == true) {
    if (isNewExpr(initExpr) == true) {
      VarSymbol* tmp      = newTemp("tmp", type);
      DefExpr*   tmpDefn  = new DefExpr(tmp);

      Expr*      arg      = toCallExpr(initExpr)->get(1)->remove();
      CallExpr*  argExpr  = toCallExpr(arg);

      Symbol*    name     = new_CStringSymbol(field->sym->name);
      Symbol*    _this    = mFn->_this;
      CallExpr*  fieldSet = new CallExpr(PRIM_SET_MEMBER, _this, name, tmp);

      insertBefore->insertBefore(tmpDefn);

      // This call must be in tree before extending argExpr
      insertBefore->insertBefore(argExpr);

      // Convert it in to a use of the init method
      argExpr->baseExpr->replace(new UnresolvedSymExpr("init"));

      // Add _mt and _this (insert at head in reverse order)
      argExpr->insertAtHead(tmp);
      argExpr->insertAtHead(gMethodToken);

      if (isFieldAccessible(argExpr) == false) {
        INT_ASSERT(false);
      }

      insertBefore->insertBefore(fieldSet);

      updateFieldsMember(argExpr);

    } else {
      VarSymbol* tmp      = newTemp("tmp", type);
      DefExpr*   tmpDefn  = new DefExpr(tmp);
      CallExpr*  tmpInit  = new CallExpr("init", gMethodToken, tmp, initExpr);

      Symbol*    name     = new_CStringSymbol(field->sym->name);
      Symbol*    _this    = mFn->_this;
      CallExpr*  fieldSet = new CallExpr(PRIM_SET_MEMBER, _this, name, tmp);

      if (isFieldAccessible(initExpr) == false) {
        INT_ASSERT(false);
      }

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);

      updateFieldsMember(initExpr);
    }

  } else if (theFn()->hasFlag(FLAG_COMPILER_GENERATED) == true &&
             field->init                               == NULL &&
             mightBeSyncSingleExpr(field)              == true) {
    // The type of the field depends on something that hasn't been determined
    // yet.  It is entirely possible that the type will end up as a sync or
    // single and so we need to flag this field initialization for resolution
    // to handle
    Symbol*   _this    = mFn->_this;
    Symbol*   name     = new_CStringSymbol(field->sym->name);
    CallExpr* fieldSet = new CallExpr(PRIM_INIT_MAYBE_SYNC_SINGLE_FIELD,
                                      _this,
                                      name,
                                      initExpr);

    if (isFieldAccessible(initExpr) == false) {
      INT_ASSERT(false);
    }

    insertBefore->insertBefore(fieldSet);

    updateFieldsMember(initExpr);

  } else if (field->exprType == NULL) {
    VarSymbol* tmp       = newTemp("tmp", type);
    DefExpr*   tmpDefn   = new DefExpr(tmp);

    // Set the value for TMP
    CallExpr*  tmpAssign = new CallExpr("=", tmp,  initExpr);

    Symbol*    _this     = mFn->_this;
    Symbol*    name      = new_CStringSymbol(field->sym->name);
    CallExpr*  fieldSet  = new CallExpr(PRIM_SET_MEMBER, _this, name, tmp);

    if (isFieldAccessible(initExpr) == false) {
      INT_ASSERT(false);
    }

    insertBefore->insertBefore(tmpDefn);
    insertBefore->insertBefore(tmpAssign);
    insertBefore->insertBefore(fieldSet);

    updateFieldsMember(initExpr);

  } else {
    VarSymbol* tmp       = newTemp("tmp", type);
    DefExpr*   tmpDefn   = new DefExpr(tmp);

    // Applies a type to TMP
    CallExpr*  tmpExpr   = new CallExpr(PRIM_INIT, field->exprType->copy());
    CallExpr*  tmpMove   = new CallExpr(PRIM_MOVE, tmp,  tmpExpr);

    // Set the value for TMP
    CallExpr*  tmpAssign = new CallExpr("=",       tmp,  initExpr);

    Symbol*    _this     = mFn->_this;
    Symbol*    name      = new_CStringSymbol(field->sym->name);
    CallExpr*  fieldSet  = new CallExpr(PRIM_SET_MEMBER, _this, name, tmp);

    if (isFieldAccessible(tmpExpr) == false) {
      INT_ASSERT(false);
    }

    if (isFieldAccessible(initExpr) == false) {
      INT_ASSERT(false);
    }

    insertBefore->insertBefore(tmpDefn);
    insertBefore->insertBefore(tmpMove);
    insertBefore->insertBefore(tmpAssign);
    insertBefore->insertBefore(fieldSet);

    updateFieldsMember(tmpExpr);

    updateFieldsMember(initExpr);
  }
}

static bool isNewExpr(Expr* expr) {
  bool retval = false;

  if (CallExpr* callExpr = toCallExpr(expr)) {
    retval = callExpr->isPrimitive(PRIM_NEW);
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void InitNormalize::fieldInitTypeInference(Expr*    insertBefore,
                                           DefExpr* field,
                                           Expr*    initExpr) const {
  SET_LINENO(insertBefore);

  // e.g.
  //   var x = <immediate>;
  //   var y = <identifier>;
  if (SymExpr* initSym = toSymExpr(initExpr)) {
    Type* type = initSym->symbol()->type;

    if (isPrimitiveScalar(type) == true) {
      VarSymbol*  tmp       = newTemp("tmp", type);
      DefExpr*    tmpDefn   = new DefExpr(tmp);
      CallExpr*   tmpInit   = new CallExpr(PRIM_MOVE, tmp, initExpr);

      Symbol*     name      = new_CStringSymbol(field->sym->name);
      Symbol*     _this     = mFn->_this;
      CallExpr*   fieldSet  = new CallExpr(PRIM_SET_MEMBER, _this, name, tmp);

      if (isFieldAccessible(initExpr) == false) {
        INT_ASSERT(false);
      }

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);

      updateFieldsMember(initExpr);

    } else {
      VarSymbol* tmp      = newTemp("tmp");
      DefExpr*   tmpDefn  = new DefExpr(tmp);
      CallExpr*  tmpInit  = new CallExpr(PRIM_INIT_VAR, tmp, initExpr);

      Symbol*    _this    = mFn->_this;
      Symbol*    name     = new_CStringSymbol(field->sym->name);
      CallExpr*  fieldSet = new CallExpr(PRIM_SET_MEMBER, _this, name, tmp);

      if (isFieldAccessible(initExpr) == false) {
        INT_ASSERT(false);
      }

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);

      updateFieldsMember(initExpr);
    }

  // e.g.
  //   var x = f(...);
  //   var y = new MyRecord(...);
  } else if (isCallExpr(initExpr) == true) {
    VarSymbol* tmp      = newTemp("tmp");
    DefExpr*   tmpDefn  = new DefExpr(tmp);
    CallExpr*  tmpInit  = new CallExpr(PRIM_INIT_VAR, tmp, initExpr);

    Symbol*    _this    = mFn->_this;
    Symbol*    name     = new_CStringSymbol(field->sym->name);
    CallExpr*  fieldSet = new CallExpr(PRIM_SET_MEMBER, _this, name, tmp);

    if (isFieldAccessible(initExpr) == false) {
      INT_ASSERT(false);
    }

    insertBefore->insertBefore(tmpDefn);
    insertBefore->insertBefore(tmpInit);
    insertBefore->insertBefore(fieldSet);

    updateFieldsMember(initExpr);

  } else {
    INT_ASSERT(false);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool InitNormalize::isFieldAccessible(Expr* expr) const {
  AggregateType* at      = type();
  bool           retval  = true;

  if (SymExpr* symExpr = toSymExpr(expr)) {
    Symbol* sym = symExpr->symbol();

    if (sym->isImmediate() == true) {
      retval = true;

    } else if (DefExpr* field = at->toLocalField(symExpr)) {
      // INIT TODO: Disallow phase 0 access of local fields.
      if (isFieldInitialized(field) == true) {
        retval = true;

      } else {
        USR_FATAL(expr,
                  "'%s' used before defined (first used here)",
                  field->sym->name);
      }

    } else if (DefExpr* field = at->toSuperField(symExpr)) {
      if (isPhase0() == true) {
        USR_FATAL(expr,
                  "Cannot access parent field \"%s\" before super.init() or this.init()",
                  field->sym->name);
      } else {
        retval = true;
      }

    } else {
      retval = true;
    }

  } else if (CallExpr* callExpr = toCallExpr(expr)) {
    if (DefExpr* field = at->toLocalField(callExpr)) {
      if (isFieldInitialized(field) == true) {
        retval = true;

      } else {
        USR_FATAL(expr,
                  "'%s' used before defined (first used here)",
                  field->sym->name);
      }

    } else if (DefExpr* field = at->toSuperField(callExpr)) {
      if (isPhase0() == true) {
        USR_FATAL(expr,
                  "Cannot access parent field \"%s\" before super.init() or this.init()",
                  field->sym->name);
      } else {
        retval = true;
      }

    } else {
      for_actuals(actual, callExpr) {
        if (isFieldAccessible(actual) == false) {
          retval = false;
          break;
        }
      }
    }

  } else if (isNamedExpr(expr) == true) {
    retval = true;

  } else if (isUnresolvedSymExpr(expr) == true) {
    // Resolution will handle this case better.
    retval = true;

  } else {
    INT_ASSERT(false);
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void InitNormalize::updateFieldsMember(Expr* expr) const {
  if (SymExpr* symExpr = toSymExpr(expr)) {
    Symbol* sym = symExpr->symbol();

    if (DefExpr* field = toLocalField(symExpr)) {
      if (isFieldInitialized(field) == true) {
        SymExpr* _this = new SymExpr(mFn->_this);
        SymExpr* name  = new SymExpr(new_CStringSymbol(sym->name));

        symExpr->replace(new CallExpr(PRIM_GET_MEMBER, _this, name));

      } else {
        USR_FATAL(expr,
                  "'%s' used before defined (first used here)",
                  field->sym->name);
      }

    } else if (DefExpr* field = toSuperField(symExpr)) {
      if (isPhase0() == true) {
        USR_FATAL(expr,
                  "Cannot access parent field \"%s\" before super.init() or this.init()",
                  field->sym->name);
      } else {
        SymExpr* _this = new SymExpr(mFn->_this);
        SymExpr* name  = new SymExpr(new_CStringSymbol(sym->name));

        if (field->sym->hasFlag(FLAG_PARAM)         == true ||
            field->sym->hasFlag(FLAG_TYPE_VARIABLE) == true) {
          symExpr->replace(new CallExpr(PRIM_GET_MEMBER_VALUE, _this, name));

        } else {
          symExpr->replace(new CallExpr(PRIM_GET_MEMBER,       _this, name));
        }
      }
    }

  } else if (CallExpr* callExpr = toCallExpr(expr)) {
    if (isFieldAccess(callExpr) == false) {
      handleInsertedMethodCall(callExpr);

      for_actuals(actual, callExpr) {
        updateFieldsMember(actual);
      }
    }

  } else if (NamedExpr* named = toNamedExpr(expr)) {
    updateFieldsMember(named->actual);

  } else if (isUnresolvedSymExpr(expr) == true) {

  } else {
    INT_ASSERT(false);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool InitNormalize::isFieldAccess(CallExpr* callExpr) const {
  bool retval = false;

  if (callExpr->isNamed(".") == true) {
    if (SymExpr* lhs = toSymExpr(callExpr->get(1))) {
      if (ArgSymbol* arg = toArgSymbol(lhs->symbol())) {
        retval = arg->hasFlag(FLAG_ARG_THIS);
      }
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* If the call is to a method on our type, we need to transform it into        *
*  something we'll recognize as a method call.                                *
*                                                                             *
* This is necessary so that later we can see the if and loop expr "method     *
* calls" written for field initialization and let them work properly.         *
*                                                                             *
************************************** | *************************************/

void InitNormalize::handleInsertedMethodCall(CallExpr* call) const {
  if (UnresolvedSymExpr* us = toUnresolvedSymExpr(call->baseExpr)) {
    bool alreadyMethod = false;
    if (call->numActuals() > 0) {
      SymExpr* firstArg = toSymExpr(call->get(1));
      if (firstArg && firstArg->symbol() == gMethodToken) {
        alreadyMethod = true;
      }
    }

    if (alreadyMethod == false) {
      AggregateType* at      = type();
      bool           matches = false;

      // Note: doesn't handle inherited methods.
      forv_Vec(FnSymbol, fn, at->methods) {
        if (strcmp(us->unresolved, fn->name) == 0) {
          matches = true;
          break;
        }
      }

      if (matches) {
        CallExpr* replacement = new CallExpr(astrSdot, mFn->_this);
        replacement->insertAtTail(us->remove());
        call->baseExpr = replacement;
        parent_insert_help(call, replacement);
      }
    }
  }
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
  if (at->isClass() == true) {

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

bool InitNormalize::isOuterField(DefExpr* field) const {
  return type()->outer == field->sym;
}

void InitNormalize::makeOuterArg() {
  AggregateType* at        = type();
  Type*          outerType = at->outer->type;

  outerType->methods.add(mFn);

  mFn->_outer = new ArgSymbol(INTENT_BLANK, "outer", outerType);

  mFn->_outer->addFlag(FLAG_GENERIC);

  mFn->_this->defPoint->insertAfter(new DefExpr(mFn->_outer));

  mFn->insertAtHead(new CallExpr(PRIM_SET_MEMBER,
                                 mFn->_this,
                                 new_CStringSymbol("outer"),
                                 mFn->_outer));
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

static void collectThisUses(Expr* expr, std::vector<CallExpr*>& uses) {
  if (CallExpr* call = toCallExpr(expr)) {
    if (isAssignment(call) == true) {
      if (CallExpr* LHS = toCallExpr(call->get(1))) {
        if (isThisDot(LHS) == true) {
          if (LHS->square == true) {
            // this.x[1] = blah;
            uses.push_back(LHS);
          }
        } else {
          // this.x.foo = blah;
          collectThisUses(LHS, uses);
        }
      }
      collectThisUses(call->get(2), uses);
    } else if (isThisDot(call) == true) {
      uses.push_back(call);
    } else {
      // this.foo(1,2,3);
      collectThisUses(call->baseExpr, uses);

      bool passesThis = false;

      // foo(this.x);
      for_actuals(actual, call) {
        SymExpr* actSe = NULL;
        if (SymExpr* se = toSymExpr(actual)) {
          actSe = se;
        } else if (NamedExpr* named = toNamedExpr(actual)) {
          actSe = toSymExpr(named->actual);
        }
        if (actSe && actSe->symbol()->hasFlag(FLAG_ARG_THIS)) {
          passesThis = true;
        }
        collectThisUses(actual, uses);
      }

      // foo(1,2, this);
      if (passesThis == true) {
        uses.push_back(call);
      }
    }
  }
}

// TODO: split into 'isMethodCall' and 'isMethodException'.
// TODO: Handle 'c = new C()' where 'C' is a nested type
static bool isMethodCall(CallExpr* call) {
  bool retval = false;

  if (CallExpr* parent = toCallExpr(call->parentExpr)) {
    if (parent->baseExpr == call) {
      retval = true;
      if (UnresolvedSymExpr* se = toUnresolvedSymExpr(call->get(2))) {
        if (strstr(se->unresolved, "_if_fn") != NULL ||
            strstr(se->unresolved, "_parloopexpr") != NULL) {
          // Don't consider compiler-inserted loop/conditional functions
          retval = false;
        }
      }
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
      if (strcmp(method->name, methodName) == 0) {
        retval = true;
        break;
      }
    }

    if (retval == false) {
      AggregateType* parent = type->dispatchParents.v[0];
      retval = typeHasMethod(parent, methodName);
    }
  }

  return retval;
}

void InitNormalize::processThisUses(Expr* expr) {
  if (isPhase2() != true) {
    if (CallExpr* call = toCallExpr(expr)) {
      processThisUses(call);
    } else if (DefExpr* def = toDefExpr(expr)) {
      processThisUses(def->init);
    }
  }
}

//
// This method processes uses of 'this' and either:
// 1) Checks for semantic errors
// 2) Updates 'this' to be mThisAsParent
//
// The errors caught are:
// 1) use-before-init
// 2) cast 'this'
// 3) call child-only method in phase one
//
void InitNormalize::processThisUses(CallExpr* call) {
  std::vector<CallExpr*> uses;
  collectThisUses(call, uses);

  int numErrors = 0;

  for_vector(CallExpr, use, uses) {
    if (DefExpr* field = type()->toLocalField(use)) {
      if (isFieldInitialized(field) == false) {
        USR_FATAL_CONT(call, "Field \"%s\" used before it is initialized", field->sym->name);
        numErrors += 1;
      }
    } else if (DefExpr* field = type()->toSuperField(use)) {
      if (isPhase0() == true) {
        USR_FATAL_CONT(call, "Parent field \"%s\" used before it is initialized", field->sym->name);
        numErrors += 1;
      }
    } else if (isMethodCall(use)) {
      if (isPhase0() == true) {
        USR_FATAL_CONT(call, "cannot call a method before super.init() or this.init()");
        numErrors += 1;
      } else if (type()->isRecord()) {
        USR_FATAL_CONT(call, "cannot call a method on a record during phase 1 of initialization");
        numErrors += 1;
      } else {
        // Check if the called method can be dynamically dispatched, otherwise
        // it's an error.
        Immediate* imm = getSymbolImmediate(toSymExpr(use->get(2))->symbol());
        const char* methodName = imm->string_value();
        AggregateType* parentType = type()->dispatchParents.v[0];
        if (typeHasMethod(parentType, methodName) == false) {
          USR_FATAL_CONT(call, "cannot call method \"%s\" on type \"%s\" in phase one", methodName, type()->symbol->name);
          USR_PRINT(call, "\"this\" in phase one treated as parent type \"%s\" for method calls", parentType->symbol->name);
          numErrors += 1;
        }

        SymExpr* thisSE = toSymExpr(use->get(1));
        if (thisSE != NULL && thisSE->symbol()->hasFlag(FLAG_ARG_THIS)) {
          thisSE->replace(new SymExpr(mThisAsParent));
        }
      }
    } else {
      if (isPhase0()) {
        USR_FATAL_CONT(call, "cannot pass \"this\" to a function before calling super.init() or this.init()");
        numErrors += 1;
      } else if (type()->isRecord()) {
        USR_FATAL_CONT(call, "cannot pass \"this\" to a function in phase 1 of initialization");
        numErrors += 1;
      } else if (use->isPrimitive(PRIM_CAST) || use->isNamed("_cast")) {
        USR_FATAL_CONT(call, "cannot cast \"this\" in phase one");
        numErrors += 1;
      } else {
        // Find 'this' usage(s) in call and replace them with mThisAsParent
        for_actuals(actual, use) {
          SymExpr* actSe = NULL;
          if (SymExpr* se = toSymExpr(actual)) {
            actSe = se;
          } else if (NamedExpr* named = toNamedExpr(actual)) {
            actSe = toSymExpr(named->actual);
          }
          if (actSe != NULL && actSe->symbol()->hasFlag(FLAG_ARG_THIS)) {
            actSe->replace(new SymExpr(mThisAsParent));
          }
        }
      }
    }
  }

  if (numErrors > 0) {
    USR_STOP();
  }
}

Expr* InitNormalize::fieldInitFromInitStmt(DefExpr*  field,
                                           CallExpr* initStmt) {
  Expr* retval = NULL;

  if (field != mCurrField) {
    INT_ASSERT(isFieldReinitialized(field) == false);

    while (field != mCurrField) {
      fieldInitFromField(initStmt);
      mImplicitFields.insert(mCurrField);

      mCurrField = toDefExpr(mCurrField->next);
    }
  }

  processThisUses(initStmt);

  retval     = fieldInitFromStmt(initStmt, field);
  mCurrField = toDefExpr(mCurrField->next);

  return retval;
}


Expr* InitNormalize::fieldInitFromStmt(CallExpr* stmt, DefExpr* field) const {
  Expr* insertBefore = stmt;
  Expr* initExpr     = stmt->get(2)->remove();
  Expr* retval       = stmt->next;

  // Initialize the field using the RHS of the source stmt
  if (field->sym->hasFlag(FLAG_PARAM)) {
    if (field->exprType != NULL) {
      genericFieldInitTypeWithInit(insertBefore, field, initExpr);

    } else {
      genericFieldInitTypeInference(insertBefore, field, initExpr);

    }

  } else if (field->sym->hasFlag(FLAG_TYPE_VARIABLE)) {
    genericFieldInitTypeInference(insertBefore, field, initExpr);

  } else if (field->exprType == NULL && field->init == NULL) {
    // Field is a generic var or const
    genericFieldInitTypeInference(insertBefore, field, initExpr);

  } else if (field->exprType != NULL) {
    // Field is concrete
    fieldInitTypeWithInit (insertBefore, field, initExpr);

  } else {
    // Field is concrete
    fieldInitTypeInference(insertBefore, field, initExpr);
  }

  // Remove the (degenerate) source version of the field assignment
  stmt->remove();

  return retval;
}

void InitNormalize::fieldInitFromField(Expr* insertBefore) {
  DefExpr* field = mCurrField;

  if        (field->exprType == NULL && field->init == NULL) {
    USR_FATAL_CONT(insertBefore,
                   "can't omit initialization of field \"%s\", "
                   "no type or default value provided",
                   field->sym->name);

  } else if (field->sym->hasFlag(FLAG_PARAM) ||
             field->sym->hasFlag(FLAG_TYPE_VARIABLE)) {

    if (field->exprType != NULL && field->init == NULL) {
      genericFieldInitTypeWoutInit (insertBefore, field);

    } else if ((field->exprType != NULL  && field->init != NULL)) {
      genericFieldInitTypeWithInit (insertBefore, field, field->init->copy());

    } else if (field->exprType == NULL && field->init != NULL) {
      genericFieldInitTypeInference(insertBefore, field, field->init->copy());

    } else {
      INT_ASSERT(false);
    }

  } else if (field->exprType != NULL && field->init == NULL) {
    fieldInitTypeWoutInit (insertBefore, field);

  } else if (field->exprType != NULL && field->init != NULL) {
    Expr* initCopy = field->init->copy();

    fieldInitTypeWithInit (insertBefore, field, initCopy);

  } else if (field->exprType == NULL && field->init != NULL) {
    Expr* initCopy = field->init->copy();

    fieldInitTypeInference(insertBefore, field, initCopy);

  } else {
    INT_ASSERT(false);
  }
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

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

// The type of the field is not yet determined either due to being entirely a
// type alias, or due to being a call to a function that returns a type.
// Therefore, we must be cautious and marking this field initialization as
// potentially a sync or single, so that when we know its type at resolution,
// we can respond appropriately.
static bool mightBeSyncSingleExpr(DefExpr* field) {
  bool retval = false;

  if (SymExpr* typeSym = toSymExpr(field->exprType)) {
    if (typeSym->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
      retval = true;
    }

  } else if (CallExpr* typeCall = toCallExpr(field->exprType)) {
    /*if (typeCall->isPrimitive(PRIM_QUERY_TYPE_FIELD)) { // might be necessary
      retval = true;
    } else */

    if (typeCall->isPrimitive() == false) {
      // The call is not a known primitive.
      // We have to assume that it is a type function being called,
      // and type functions could return a sync or single type.
      retval = true;
    }
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

  if (callExpr->isNamedAstr(astrSequals) == true) {
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
