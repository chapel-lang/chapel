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

#include "InitNormalize.h"

#include "stmt.h"

static bool isSuperInit(Expr* stmt);
static bool isThisInit (Expr* stmt);

static bool isAssignment(CallExpr* callExpr);
static bool isSimpleAssignment(CallExpr* callExpr);
static bool isCompoundAssignment(CallExpr* callExpr);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

InitNormalize::InitNormalize(FnSymbol* fn) {
  mFn         = fn;
  mCurrField  = firstField(fn);
  mPhase      = startPhase(fn);
  mBlockType  = cBlockNormal;
}

InitNormalize::InitNormalize(BlockStmt* block, const InitNormalize& curr) {
  mFn         = curr.mFn;
  mCurrField  = curr.mCurrField;
  mPhase      = curr.mPhase;

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
}

InitNormalize::InitNormalize(CondStmt* cond, const InitNormalize& curr) {
  mFn         = curr.mFn;
  mCurrField  = curr.mCurrField;
  mPhase      = curr.mPhase;
  mBlockType  = cBlockCond;
}

InitNormalize::InitNormalize(LoopStmt* loop, const InitNormalize& curr) {
  mFn         = curr.mFn;
  mCurrField  = curr.mCurrField;
  mPhase      = curr.mPhase;
  mBlockType  = cBlockLoop;
}


void InitNormalize::merge(const InitNormalize& fork) {
  mCurrField = fork.mCurrField;
  mPhase     = fork.mPhase;
}

AggregateType* InitNormalize::type() const {
  return mFn != NULL ? toAggregateType(mFn->_this->type) : NULL;
}

FnSymbol* InitNormalize::theFn() const {
  return mFn;
}

bool InitNormalize::isRecord() const {
  return ::isRecord(type());
}

bool InitNormalize::isClass() const {
  return ::isClass(type());
}

InitNormalize::InitPhase  InitNormalize::currPhase() const {
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

bool InitNormalize::inCondStmt() const {
  return mBlockType == cBlockCond;
}

bool InitNormalize::inParallelStmt() const {
  return mBlockType == cBlockBegin   ||
         mBlockType == cBlockCobegin  ;
}

bool InitNormalize::inCoforall() const {
  return mBlockType == cBlockCoforall;
}

bool InitNormalize::inOn() const {
  return mBlockType == cBlockOn;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

Expr* InitNormalize::completePhase1(Expr* initStmt) {
  Expr* retval = initStmt->next;

  if (isSuperInit(initStmt) == true) {
    initializeFieldsBefore(initStmt);

    if (isRecord() == true) {
      initStmt->remove();
    }

  } else if (isThisInit(initStmt) == true) {
    mCurrField = NULL;

  } else {
    INT_ASSERT(false);
  }

  mPhase = cPhase2;

  return retval;
}

void InitNormalize::initializeFieldsBefore(Expr* insertBefore) {
  while (mCurrField != NULL) {
    DefExpr* field         = mCurrField;
    bool     isTypeUnknown = mCurrField->sym->type == dtUnknown;

    if (field->exprType == NULL && field->init == NULL) {
      USR_FATAL_CONT(insertBefore,
                     "can't omit initialization of field \"%s\", "
                     "no type or default value provided",
                     field->sym->name);
    } else if (field->sym->hasFlag(FLAG_PARAM) ||
               field->sym->hasFlag(FLAG_TYPE_VARIABLE)) {
      if (field->exprType != NULL && field->init == NULL) {
        genericFieldInitTypeWoutInit (insertBefore, field);

      } else if ((field->exprType != NULL  && field->init != NULL)  ||
                 (isTypeUnknown   == false && field->init != NULL)) {
        genericFieldInitTypeWithInit (insertBefore, field, field->init->copy());

      } else if (field->exprType == NULL && field->init != NULL) {
        genericFieldInitTypeInference(insertBefore, field, field->init->copy());

      } else {
        INT_ASSERT(false);
      }

    } else if (field->exprType != NULL && field->init == NULL) {
      fieldInitTypeWoutInit (insertBefore, field);

    } else if ((field->exprType != NULL  && field->init != NULL)  ||
               (isTypeUnknown   == false && field->init != NULL)) {
      fieldInitTypeWithInit (insertBefore, field, field->init->copy());

    } else if (field->exprType == NULL && field->init != NULL) {
      fieldInitTypeInference(insertBefore, field, field->init->copy());

    } else {
      INT_ASSERT(false);
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

    updateFieldsMember(tmpExpr);

    insertBefore->insertBefore(tmpDefn);
    insertBefore->insertBefore(tmpInit);
    insertBefore->insertBefore(fieldSet);
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

  updateFieldsMember(initExpr);

  insertBefore->insertBefore(fieldSet);
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
      VarSymbol*  tmp;

      if (type == dtAny) {
        tmp = newTemp("tmp");
      } else {
        tmp = newTemp("tmp", type);
      }
      DefExpr*    tmpDefn   = new DefExpr(tmp);
      CallExpr*   tmpInit   = new CallExpr(PRIM_MOVE, tmp, initExpr);

      tmp->addFlag(FLAG_TYPE_VARIABLE);

      Symbol*    _this    = mFn->_this;
      Symbol*    name     = new_CStringSymbol(field->sym->name);
      CallExpr*  fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, tmp);

      if (isFieldAccessible(initExpr) == false) {
        INT_ASSERT(false);
      }

      updateFieldsMember(initExpr);

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);

    } else if (isPrimitiveScalar(type) == true) {
      VarSymbol*  tmp       = newTemp("tmp", type);
      DefExpr*    tmpDefn   = new DefExpr(tmp);
      CallExpr*   tmpInit   = new CallExpr(PRIM_MOVE, tmp, initExpr);

      if (isParam == true) {
        tmp->addFlag(FLAG_PARAM);
      }

      Symbol*     name      = new_CStringSymbol(field->sym->name);
      Symbol*     _this     = mFn->_this;
      CallExpr*   fieldSet  = new CallExpr(PRIM_INIT_FIELD, _this, name, tmp);

      if (isFieldAccessible(initExpr) == false) {
        INT_ASSERT(false);
      }

      updateFieldsMember(initExpr);

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);

    } else {
      VarSymbol* tmp      = newTemp("tmp");
      DefExpr*   tmpDefn  = new DefExpr(tmp);
      CallExpr*  tmpInit  = new CallExpr(PRIM_INIT_VAR, tmp, initExpr);

      if (isParam == true) {
        tmp->addFlag(FLAG_PARAM);
      }

      Symbol*    _this    = mFn->_this;
      Symbol*    name     = new_CStringSymbol(field->sym->name);
      CallExpr*  fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, tmp);

      if (isFieldAccessible(initExpr) == false) {
        INT_ASSERT(false);
      }

      updateFieldsMember(initExpr);

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);
    }

  // e.g.
  //   var x = f(...);
  //   var y = new MyRecord(...);
  } else if (CallExpr* initCall = toCallExpr(initExpr)) {
    if (initCall->isPrimitive(PRIM_NEW) == true) {
      INT_ASSERT(false);

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

      updateFieldsMember(initExpr);

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);

    } else {
      VarSymbol* tmp      = newTemp("tmp");
      DefExpr*   tmpDefn  = new DefExpr(tmp);
      CallExpr*  tmpInit  = new CallExpr(PRIM_INIT_VAR, tmp, initExpr);

      if (isParam == true) {
        tmp->addFlag(FLAG_PARAM);
      }

      Symbol*    _this    = mFn->_this;
      Symbol*    name     = new_CStringSymbol(field->sym->name);
      CallExpr*  fieldSet = new CallExpr(PRIM_INIT_FIELD, _this, name, tmp);

      if (isFieldAccessible(initExpr) == false) {
        INT_ASSERT(false);
      }

      updateFieldsMember(initExpr);

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);
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

    updateFieldsMember(tmpExpr);

    insertBefore->insertBefore(tmpDefn);
    insertBefore->insertBefore(tmpInit);
    insertBefore->insertBefore(fieldSet);
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

    updateFieldsMember(initExpr);

    insertBefore->insertBefore(tmpDefn);
    insertBefore->insertBefore(tmpInit);
    insertBefore->insertBefore(fieldSet);

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

      updateFieldsMember(argExpr);

      insertBefore->insertBefore(fieldSet);

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

      updateFieldsMember(initExpr);

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);
    }

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

    updateFieldsMember(tmpExpr);

    if (isFieldAccessible(initExpr) == false) {
      INT_ASSERT(false);
    }

    updateFieldsMember(initExpr);

    insertBefore->insertBefore(tmpDefn);
    insertBefore->insertBefore(tmpMove);
    insertBefore->insertBefore(tmpAssign);
    insertBefore->insertBefore(fieldSet);
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

      updateFieldsMember(initExpr);

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);

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

      updateFieldsMember(initExpr);

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);
    }

  // e.g.
  //   var x = f(...);
  //   var y = new MyRecord(...);
  } else if (CallExpr* initCall = toCallExpr(initExpr)) {
    if (initCall->isPrimitive(PRIM_NEW) == true) {
      INT_ASSERT(false);

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

      updateFieldsMember(initExpr);

      insertBefore->insertBefore(tmpDefn);
      insertBefore->insertBefore(tmpInit);
      insertBefore->insertBefore(fieldSet);
    }

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
  AggregateType* at     = type();
  bool           retval = true;

  if (SymExpr* symExpr = toSymExpr(expr)) {
    Symbol* sym = symExpr->symbol();

    if (sym->isImmediate() == true) {
      retval = true;

    } else if (DefExpr* field = toLocalField(at, symExpr)) {
      if (isFieldInitialized(field) == true) {
        retval = true;
      } else {
        USR_FATAL(expr,
                  "'%s' used before defined (first used here)",
                  field->sym->name);
      }

    } else if (DefExpr* field = toSuperField(at, symExpr)) {
      if (isPhase2() == true) {
        retval = true;
      } else {
        USR_FATAL(expr,
                  "Cannot access parent field '%s' during phase 1",
                  field->sym->name);
      }

    } else {
      retval = true;
    }

  } else if (CallExpr* callExpr = toCallExpr(expr)) {
    if (DefExpr* field = toLocalField(at, callExpr)) {
      if (isFieldInitialized(field) == true) {
        retval = true;
      } else {
        USR_FATAL(expr,
                  "'%s' used before defined (first used here)",
                  field->sym->name);
      }

    } else if (DefExpr* field = toSuperField(at, callExpr)) {
      if (isPhase2() == true) {
        retval = true;
      } else {
        USR_FATAL(expr,
                  "Cannot access parent field '%s' during phase 1",
                  field->sym->name);
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
        SymExpr* field = new SymExpr(new_CStringSymbol(sym->name));

        symExpr->replace(new CallExpr(PRIM_GET_MEMBER, _this, field));

      } else {
        USR_FATAL(expr,
                  "'%s' used before defined (first used here)",
                  field->sym->name);
      }

    } else if (DefExpr* field = toSuperField(symExpr)) {
      USR_FATAL(expr,
                "Cannot access parent field '%s' during phase 1",
                field->sym->name);
    }

  } else if (CallExpr* callExpr = toCallExpr(expr)) {
    if (isFieldAccess(callExpr) == false) {
      for_actuals(actual, callExpr) {
        updateFieldsMember(actual);
      }
    }

  } else if (isNamedExpr(expr) == true) {

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
*                                                                             *
*                                                                             *
************************************** | *************************************/

DefExpr* InitNormalize::toLocalField(SymExpr* expr) const {
  return toLocalField(type(), expr);
}

DefExpr* InitNormalize::toLocalField(CallExpr* expr) const {
  return toLocalField(type(), expr);
}

DefExpr* InitNormalize::toLocalField(AggregateType* at,
                                     const char*    name) const {
  Expr*    currField = at->fields.head;
  DefExpr* retval    = NULL;

  while (currField != NULL && retval == NULL) {
    DefExpr*   defExpr = toDefExpr(currField);
    VarSymbol* var     = toVarSymbol(defExpr->sym);

    if (strcmp(var->name, name) == 0) {
      retval    = defExpr;
    } else {
      currField = currField->next;
    }
  }

  return retval;
}

DefExpr* InitNormalize::toLocalField(AggregateType* at, SymExpr* expr) const {
  Expr*    currField = at->fields.head;
  Symbol*  sym       = expr->symbol();
  DefExpr* retval    = NULL;

  while (currField != NULL && retval == NULL) {
    DefExpr* defExpr = toDefExpr(currField);

    if (sym == defExpr->sym) {
      retval    = defExpr;
    } else {
      currField = currField->next;
    }
  }

  return retval;
}

DefExpr* InitNormalize::toLocalField(AggregateType* at, CallExpr* expr) const {
  DefExpr* retval = NULL;

  if (expr->isNamed(".") == true) {
    SymExpr* base = toSymExpr(expr->get(1));
    SymExpr* name = toSymExpr(expr->get(2));

    if (base != NULL && name != NULL) {
      VarSymbol* var = toVarSymbol(name->symbol());

      // The base is <this> and the slot is a fieldName
      if (base->symbol()->hasFlag(FLAG_ARG_THIS) == true &&

          var                                    != NULL &&
          var->immediate                         != NULL &&
          var->immediate->const_kind             == CONST_KIND_STRING) {
        retval = toLocalField(at, var->immediate->v_string);
      }
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

DefExpr* InitNormalize::toSuperField(SymExpr* expr) const {
  return toSuperField(type(), expr);
}

DefExpr* InitNormalize::toSuperField(AggregateType* at,
                                     const char*    name) const {
  forv_Vec(Type, t, at->dispatchParents) {
    if (AggregateType* pt = toAggregateType(t)) {
      if (DefExpr* field = toLocalField(pt, name)) {
        return field;
      }
    }
  }

  return NULL;
}

DefExpr* InitNormalize::toSuperField(AggregateType* at, SymExpr*  expr) const {
  forv_Vec(Type, t, at->dispatchParents) {
    if (AggregateType* pt = toAggregateType(t)) {
      if (DefExpr* field = toLocalField(pt, expr)) {
        return field;
      }
    }
  }

  return NULL;
}

DefExpr* InitNormalize::toSuperField(AggregateType* at, CallExpr* expr) const {
  forv_Vec(Type, t, at->dispatchParents) {
    if (AggregateType* pt = toAggregateType(t)) {
      if (DefExpr* field = toLocalField(pt, expr)) {
        return field;
      }
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
  InitPhase retval = cPhase2;

  while (stmt != NULL && retval == cPhase2) {
    if (isDefExpr(stmt) == true) {
      stmt = stmt->next;

    } else if (CallExpr* callExpr = toCallExpr(stmt)) {
      if        (isThisInit(callExpr)  == true) {
        retval = cPhase0;

      } else if (isSuperInit(callExpr) == true) {
        retval = cPhase1;

      } else {
        stmt   = stmt->next;
      }

    } else if (CondStmt* cond = toCondStmt(stmt)) {
      if (cond->elseStmt == NULL) {
        InitPhase thenPhase = startPhase(cond->thenStmt);

        if (thenPhase != cPhase2) {
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

        } else {
          stmt   = stmt->next;
        }
      }

    } else if (BlockStmt* block = toBlockStmt(stmt)) {
      InitPhase phase = startPhase(block);

      if (phase != cPhase2) {
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

void InitNormalize::checkPhase(BlockStmt* block) {
  if (mPhase == cPhase0) {
    InitPhase newPhase = startPhase(block);

    if (newPhase == cPhase1) {
      mPhase = newPhase;
    }
  }
}

DefExpr* InitNormalize::firstField(FnSymbol* fn) const {
  AggregateType* at     = toAggregateType(fn->_this->type);
  DefExpr*       retval = toDefExpr(at->fields.head);

  // Skip the pseudo-field "super"
  if (::isClass(at) == true) {
    retval = toDefExpr(retval->next);
  }

  // Skip the pseudo-field "outer" (if present)
  if (retval                             != NULL &&
      retval->exprType                   == NULL &&
      retval->init                       == NULL &&
      strcmp(retval->sym->name, "outer") ==    0) {
    retval = toDefExpr(retval->next);
  }

  return retval;
}

Expr* InitNormalize::fieldInitFromInitStmt(DefExpr*  field,
                                           CallExpr* initStmt) {
  Expr* retval = NULL;

  if (field != mCurrField) {
    INT_ASSERT(isFieldReinitialized(field) == false);

    while (field != mCurrField) {
      fieldInitFromField(initStmt);

      mCurrField = toDefExpr(mCurrField->next);
    }
  }

  // Now that omitted fields have been handled, see if RHS is OK
  if (fieldUsedBeforeInitialized(initStmt) == true) {
    USR_FATAL(initStmt, "Field used before it is initialized");
  }

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

bool InitNormalize::fieldUsedBeforeInitialized(Expr* expr) const {
  bool retval = false;

  if (DefExpr* defExpr = toDefExpr(expr)) {
    if (defExpr->init != NULL) {
      retval = fieldUsedBeforeInitialized(defExpr->init);
    }

  } else if (CallExpr* callExpr = toCallExpr(expr)) {
    retval = fieldUsedBeforeInitialized(callExpr);
  }

  return retval;
}

bool InitNormalize::fieldUsedBeforeInitialized(CallExpr* callExpr) const {
  bool retval = false;

  if (isAssignment(callExpr) == true) {
    retval = fieldUsedBeforeInitialized(callExpr->get(2));

  } else if (DefExpr* field = toLocalField(type(), callExpr)) {
    retval = isFieldInitialized(field) == true ? false : true;

  } else {
    for_actuals(actual, callExpr) {
      if (fieldUsedBeforeInitialized(actual) == true) {
        retval = true;
        break;
      }
    }
  }

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

    case cBlockCond:
      printf("cond\n");
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

static const char* initName(Expr*     stmt);

static const char* initName(CallExpr* expr);

static bool        isStringLiteral(Expr* expr, const char* name);

static bool        isUnresolvedSymbol(Expr* expr, const char* name);

static bool        isSymbolThis(Expr* expr);

static bool isSuperInit(Expr* stmt) {
  const char* name = initName(stmt);

  return name != NULL && strcmp(name, "super") == 0 ? true : false;
}

static bool isThisInit(Expr* stmt) {
  const char* name = initName(stmt);

  return name != NULL && strcmp(name, "this")  == 0 ? true : false;
}


static const char* initName(Expr* stmt) {
  const char* retval = NULL;

  if (CallExpr* call = toCallExpr(stmt)) {
    if (CallExpr* inner = toCallExpr(call->baseExpr)) {
      retval = initName(inner);
    }
  }

  return retval;
}

//
// Extract init name for the inner part of the call i.e.
//
//    this.init(args);
//      => call(".", this,                     "init")
//
//    super.init(args)
//      => call(".", unresolved("super"),      "init")     // records
//      => call(".", call(".", this, "super"), "init")     // classes
//

static const char* initName(CallExpr* expr) {
  const char* retval = NULL;

  if (expr->numActuals()                    ==    2 &&
      expr->isNamedAstr(astrSdot)           == true &&
      isStringLiteral(expr->get(2), "init") == true) {

    if (isSymbolThis(expr->get(1)) == true) {
      retval = "this";

    } else {
      // "super" is an unresolved symbol for records
      if (isUnresolvedSymbol(expr->get(1), "super") == true) {
        retval = "super";

      // "super" is a expr to a field accessor for classes
      } else if (CallExpr* subExpr = toCallExpr(expr->get(1))) {
        if (subExpr->numActuals()                     == 2    &&
            subExpr->isNamedAstr(astrSdot)            == true &&
            isSymbolThis(subExpr->get(1))             == true &&
            isStringLiteral(subExpr->get(2), "super") == true) {
          retval = "super";
        }
      }
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


static bool isStringLiteral(Expr* expr, const char* name) {
  bool retval = false;

  if (SymExpr* sym = toSymExpr(expr)) {
    if (VarSymbol* var = toVarSymbol(sym->symbol())) {
      if (var->immediate->const_kind == CONST_KIND_STRING) {
        retval = strcmp(var->immediate->v_string, name) == 0;
      }
    }
  }

  return retval;
}

static bool isUnresolvedSymbol(Expr* expr, const char* name) {
  bool retval = false;

  if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(expr)) {
    retval = strcmp(sym->unresolved, name) == 0;
  }

  return retval;
}

static bool isSymbolThis(Expr* expr) {
  bool retval = false;

  if (SymExpr* sym = toSymExpr(expr)) {
    retval = sym->symbol()->hasFlag(FLAG_ARG_THIS);
  }

  return retval;
}
