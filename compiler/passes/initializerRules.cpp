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

#include "initializerRules.h"

#include "astutil.h"
#include "expr.h"
#include "LoopStmt.h"
#include "stlUtil.h"
#include "stmt.h"
#include "symbol.h"
#include "type.h"
#include "typeSpecifier.h"

#include <map>

enum InitStyle {
  STYLE_NONE,
  STYLE_SUPER_INIT,
  STYLE_THIS_INIT,
  STYLE_BOTH
};

static InitStyle findInitStyle(FnSymbol* fn);
static InitStyle findInitStyle(Expr*     expr);

static void      preNormalizeNonGenericInit(FnSymbol* fn);
static void      preNormalizeGenericInit(FnSymbol* fn);

/************************************* | **************************************
*                                                                             *
* Attempt to assign a type to the symbol for each field in some of the        *
* simpler cases.                                                              *
*                                                                             *
* 2017/03/20 Noakes: This may set a direction for refactoring resolution in   *
* a subsequent release.                                                       *
*                                                                             *
************************************** | *************************************/

void preNormalizeFields(AggregateType* at) {
  for_alist(field, at->fields) {
    if (DefExpr* defExpr = toDefExpr(field)) {
      if (Expr* typeExpr = defExpr->exprType) {
        Type* type = typeForTypeSpecifier(typeExpr);

        // var x, y : Foo
        //   =>
        // var x : Foo;
        // var y : typeof(x);       // Handle this case
        if (type == NULL) {
          if (CallExpr* callExpr = toCallExpr(typeExpr)) {
            if (callExpr->isPrimitive(PRIM_TYPEOF) == true) {
              if (SymExpr* varExpr = toSymExpr(callExpr->get(1))) {
                Type* t = varExpr->symbol()->type;

                type = (t != dtUnknown) ? t : NULL;
              }
            }
          }
        }

        if (type != NULL) {
          Symbol* sym = defExpr->sym;

          if (sym->hasFlag(FLAG_CONST) == true) {
            sym->qual = QUAL_CONST_VAL;
            sym->type = type;

          } else {
            sym->qual = QUAL_VAL;
            sym->type = type;
          }
        }
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool isReturnVoid(FnSymbol* fn);

void preNormalizeInitMethod(FnSymbol* fn) {
  AggregateType* at = toAggregateType(fn->_this->type);

  if (fn->hasFlag(FLAG_NO_PARENS) == true) {
    USR_FATAL(fn, "an initializer cannot be declared without parentheses");

  } else if (isReturnVoid(fn) == false) {
    USR_FATAL(fn, "an initializer cannot return a non-void result");

  } else if (isNonGenericRecord(at) == true ||
             isNonGenericClass(at)  == true) {
    preNormalizeNonGenericInit(fn);

  } else {
    preNormalizeGenericInit(fn);
  }
}

//
// Initializers cannot
//   1) Declare a return type other than void
//   2) Contain a return expression with a value

static bool isReturnVoid(FnSymbol* fn) {
  bool retval = true;

  if (fn->retExprDefinesNonVoid() == true) {
    USR_FATAL(fn, "initializers cannot declare a return type");
    retval = false;

  } else {
    std::vector<CallExpr*> calls;

    collectCallExprs(fn->body, calls);

    for (size_t i = 0; i < calls.size() && retval == true; i++) {
      if (calls[i]->isPrimitive(PRIM_RETURN) == true) {
        SymExpr* value = toSymExpr(calls[i]->get(1));

        if (value == NULL || value->symbol()->type != dtVoid) {
          USR_FATAL(calls[i], "initializers cannot return a value");
          retval = false;
        }
      }
    }
  }

  fn->retType = dtVoid;

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void      fieldInitFromStmt(CallExpr* stmt,
                                   FnSymbol* fn,
                                   DefExpr*  field);

static void      fieldInitFromField(Expr*     insertBefore,
                                    FnSymbol* fn,
                                    DefExpr*  field);

static DefExpr*  toSuperFieldInit(AggregateType* at, CallExpr* callExpr);
static DefExpr*  toLocalFieldInit(AggregateType* at, CallExpr* callExpr);
static CallExpr* createFieldAccess(FnSymbol* fn, DefExpr* field);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

class IterState {
public:
                  IterState(FnSymbol* fn);
                  IterState(CondStmt* cond, const IterState& curr);
                  IterState(LoopStmt* loop, const IterState& curr);

  AggregateType*  type()                                                 const;
  bool            isRecord()                                             const;
  bool            isClass()                                              const;

  bool            isPhase1()                                             const;
  bool            isPhase2()                                             const;
  Expr*           completePhase1(Expr* insertBefore);
  void            initializeFieldsBefore(Expr* insertBefore);

  bool            isFieldReinitialized(DefExpr* field)                   const;
  bool            inLoopBody()                                           const;
  bool            inCondStmt()                                           const;

  DefExpr*        currField()                                            const;

  void            fieldInitFromInitStmt(DefExpr*  field,
                                        CallExpr* callExpr);

private:
  enum BlockType {
    cBlockNormal,
    cBlockCond,
    cBlockLoop,
  };

                  IterState();

  bool            startsInPhase1(FnSymbol* fn)                           const;
  DefExpr*        firstField(FnSymbol* fn)                               const;

  FnSymbol*       mFn;
  DefExpr*        mCurrField;
  bool            mIsPhase1;
  BlockType       mBlockType;
};

IterState::IterState(FnSymbol* fn) {
  mFn         = fn;
  mCurrField  = firstField(fn);
  mIsPhase1   = startsInPhase1(fn);
  mBlockType  = cBlockNormal;
}

// Only used to generate error messages
IterState::IterState(CondStmt* cond, const IterState& curr) {
  mFn         = curr.mFn;
  mCurrField  = curr.mCurrField;
  mIsPhase1   = curr.mIsPhase1;
  mBlockType  = cBlockCond;
}

// Only used to generate error messages
IterState::IterState(LoopStmt* loop, const IterState& curr) {
  mFn         = curr.mFn;
  mCurrField  = curr.mCurrField;
  mIsPhase1   = curr.mIsPhase1;
  mBlockType  = cBlockLoop;
}

AggregateType* IterState::type() const {
  return mFn != NULL ? toAggregateType(mFn->_this->type) : NULL;
}

bool IterState::isRecord() const {
  return ::isRecord(type());
}

bool IterState::isClass() const {
  return ::isClass(type());
}

bool IterState::isPhase1() const {
  return mIsPhase1;
}

bool IterState::isPhase2() const {
  return !mIsPhase1;
}

bool IterState::isFieldReinitialized(DefExpr* field) const {
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

bool IterState::inLoopBody() const {
  return mBlockType == cBlockLoop;
}

bool IterState::inCondStmt() const {
  return mBlockType == cBlockCond;
}

Expr* IterState::completePhase1(Expr* initStmt) {
  Expr* retval = initStmt->next;

  initializeFieldsBefore(initStmt);

  if (isRecord() == true) {
    initStmt->remove();
  }

  mIsPhase1 = false;

  return retval;
}

void IterState::initializeFieldsBefore(Expr* insertBefore) {
  while (mCurrField != NULL) {
    fieldInitFromField(insertBefore, mFn, mCurrField);

    mCurrField = toDefExpr(mCurrField->next);
  }
}

DefExpr* IterState::currField() const {
  return mCurrField;
}

bool IterState::startsInPhase1(FnSymbol* fn) const {
  InitStyle initStyle = findInitStyle(fn);

  return initStyle != STYLE_NONE ? true : false;
}

DefExpr* IterState::firstField(FnSymbol* fn) const {
  AggregateType* at   = toAggregateType(fn->_this->type);
  Expr*          head = at->fields.head;

  // Skip the psuedo-field "super"
  if (::isClass(at) == true) {
    head = head->next;
  }

  return toDefExpr(head);
}

void IterState::fieldInitFromInitStmt(DefExpr* field, CallExpr* callExpr) {
  // This is the initializer for the current field
  if (field == mCurrField) {
    fieldInitFromStmt(callExpr, mFn, field);

    mCurrField = toDefExpr(mCurrField->next);

  } else {
    INT_ASSERT(isFieldReinitialized(field) == false);

    while (field != mCurrField) {
      fieldInitFromField(callExpr, mFn, mCurrField);

      mCurrField = toDefExpr(mCurrField->next);
    }

    fieldInitFromStmt(callExpr, mFn, field);

    mCurrField = toDefExpr(mCurrField->next);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static IterState preNormalize(BlockStmt* block, IterState state);
static IterState preNormalize(BlockStmt* block, IterState state, Expr* start);

static CallExpr* createCallToSuperInit(FnSymbol* fn);

static bool      isInitStmt(Expr* stmt);
static bool      isSuperInit(Expr* stmt);
static bool      isThisInit(Expr* stmt);

static void preNormalizeNonGenericInit(FnSymbol* fn) {
  AggregateType* at = toAggregateType(fn->_this->type);
  IterState      state(fn);

  // This implies the body contains at least one instance of super.init()
  // and/or this.init() i.e. the body is not empty and we do not need to
  // insert super.init()
  if (state.isPhase1() == true) {
    preNormalize(fn->body, state);

  // 1) Insert super.init()
  // 2) Insert field initializers before super.init()
  // 3) Pre-normalize the phase2 statements
  } else if (isClass(at) == true) {
    CallExpr* superInit = createCallToSuperInit(fn);

    fn->body->insertAtHead(superInit);

    state.initializeFieldsBefore(superInit);

    preNormalize(fn->body, state, superInit->next);

  // 1) Insert field initializers before the first statement
  // 2) Pre-normalize the phase2 statements
  } else if (isRecord(at) == true && fn->body->body.head != NULL) {
    Expr* head = fn->body->body.head;

    state.initializeFieldsBefore(head);

    preNormalize(fn->body, state, head);

  // A degenerate initializer with no body
  // 1) Insert a NO-OP as an anchor
  // 2) Insert field initializers before the NO-OP
  // 3) Remove the NO-OP
  } else if (isRecord(at) == true && fn->body->body.head == NULL) {
    Expr* noop = new CallExpr(PRIM_NOOP);

    state.initializeFieldsBefore(noop);

    noop->remove();

  } else {
    INT_ASSERT(false);
  }

  // If this is a non-generic class then create a type method
  // to wrap this initializer
  if (isClass(at) == true && at->isGeneric() == false) {
    buildClassAllocator(fn);

    fn->addFlag(FLAG_INLINE);
  }
}

static IterState preNormalize(BlockStmt* block, IterState state) {
  return preNormalize(block, state, block->body.head);
}

static IterState preNormalize(BlockStmt* block, IterState state, Expr* stmt) {
  while (stmt != NULL) {
    if (isDefExpr(stmt) == true) {
      stmt = stmt->next;

    } else if (CallExpr* callExpr = toCallExpr(stmt)) {
      if (isInitStmt(callExpr) == true) {
        if (state.isPhase2() == true) {
          INT_ASSERT(false);

        } else if (state.inLoopBody() == true) {
          if (isSuperInit(callExpr) == true) {
            USR_FATAL(stmt, "use of super.init() call in loop body");

          } else if (isThisInit(callExpr) == true) {
            USR_FATAL(stmt, "use of this.init() call in loop body");

          } else {
            INT_ASSERT(false);
          }

        } else if (state.inCondStmt() == true) {
          INT_ASSERT(false);

        } else {
          stmt = state.completePhase1(stmt);
        }

      } else if (DefExpr* field = toLocalFieldInit(state.type(), callExpr)) {
        if (state.isPhase2() == true) {

        } else if (state.isFieldReinitialized(field) == true) {
          USR_FATAL(stmt,
                    "multiple initializations of field \"%s\"",
                    field->sym->name);

        } else if (state.inLoopBody() == true) {
          USR_FATAL(stmt,
                    "can't initialize field \"%s\" inside a "
                    "loop during phase 1 of initialization",
                    field->sym->name);

        } else if (state.inCondStmt() == true) {
          USR_FATAL(stmt,
                    "can't initialize field \"%s\" inside a "
                    "conditional during phase 1 of initialization",
                    field->sym->name);

        } else {
          state.fieldInitFromInitStmt(field, callExpr);
        }

        stmt = stmt->next;

      } else if (DefExpr* field = toSuperFieldInit(state.type(), callExpr)) {
        USR_FATAL(stmt,
                  "can't set value of field \"%s\" from parent type "
                  "during phase 1 of initialization",
                  field->sym->name);

      } else {
        stmt = stmt->next;
      }

    } else if (CondStmt* cond = toCondStmt(stmt)) {
      // Focus on phase 1
      if (state.isPhase1() == true) {
        preNormalize(cond->thenStmt, IterState(cond, state));

        if (cond->elseStmt != NULL) {
          preNormalize(cond->elseStmt, IterState(cond, state));
        }
      }

      stmt = stmt->next;

    } else if (LoopStmt* loop = toLoopStmt(stmt)) {
      // Focus on phase 1
      if (state.isPhase1() == true) {
        preNormalize((BlockStmt*) stmt, IterState(loop, state));
      }

      stmt = stmt->next;

    } else if (BlockStmt* block = toBlockStmt(stmt)) {
      state = preNormalize(block, state);
      stmt  = stmt->next;

    } else {
      // Focus on phase 1
      if (state.isPhase1() == true) {
        INT_ASSERT(false);
      }

      stmt = stmt->next;
    }
  }

  return state;
}

// Pre-normalized call to the function super.init with no arguments
static CallExpr* createCallToSuperInit(FnSymbol* fn) {
  Symbol*   superSym  = new_CStringSymbol("super");
  CallExpr* superCall = new CallExpr(".", fn->_this, superSym);

  Symbol*   initSym   = new_CStringSymbol("init");

  return new CallExpr(new CallExpr(".", superCall, initSym));
}

static bool isInitStmt(Expr* stmt) {
  return findInitStyle(stmt) != STYLE_NONE       ? true : false;
}

static bool isSuperInit(Expr* stmt) {
  return findInitStyle(stmt) == STYLE_SUPER_INIT ? true : false;
}

static bool isThisInit(Expr* stmt) {
  return findInitStyle(stmt) == STYLE_THIS_INIT  ? true : false;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void fieldInitTypeWoutInit(Expr*     stmt,
                                  FnSymbol* fn,
                                  DefExpr*  field);

static void fieldInitTypeWithInit(Expr*     stmt,
                                  FnSymbol* fn,
                                  DefExpr*  field,
                                  Expr*     initExpr);

static void fieldInitTypeInference(Expr*     insertBefore,
                                   FnSymbol* fn,
                                   DefExpr*  field,
                                   Expr*     initExpr);

static void fieldInitFromStmt(CallExpr* stmt,
                              FnSymbol* fn,
                              DefExpr*  field) {
  if (field->exprType != NULL) {
    fieldInitTypeWithInit (stmt, fn, field, stmt->get(2));

  } else {
    fieldInitTypeInference(stmt, fn, field, stmt->get(2));
  }
}

static void fieldInitFromField(Expr*     insertBefore,
                               FnSymbol* fn,
                               DefExpr*  field) {
  if        (field->exprType == NULL && field->init == NULL) {
    INT_ASSERT(false);

  } else if (field->exprType != NULL && field->init == NULL) {
    fieldInitTypeWoutInit(insertBefore, fn, field);

  } else if (field->exprType != NULL && field->init != NULL) {
    fieldInitTypeWithInit(insertBefore, fn, field, field->init);

  } else if (field->exprType == NULL && field->init != NULL) {
    fieldInitTypeInference(insertBefore, fn, field, field->init);

  } else {
    INT_ASSERT(false);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void fieldInitTypeWoutInit(Expr*     stmt,
                                  FnSymbol* fn,
                                  DefExpr*  field) {
  SET_LINENO(stmt);

  Type* type = field->sym->type;

  if (isPrimitiveScalar(type) == true) {
    CallExpr*  defVal = new CallExpr("_defaultOf", type->symbol);
    CallExpr*  access = createFieldAccess(fn, field);

    stmt->insertBefore(new CallExpr("=",       access,   defVal));

  } else if (isNonGenericClass(type) == true) {
    CallExpr*  defVal = new CallExpr("_defaultOf", type->symbol);
    CallExpr*  access = createFieldAccess(fn, field);

    stmt->insertBefore(new CallExpr("=",       access,   defVal));


  } else if (isNonGenericRecordWithInitializers(type) == true) {
#if 0
    defExpr->insertAfter(new CallExpr("init", gMethodToken, var));
#endif

    INT_ASSERT(false);

  } else {
    VarSymbol* typeTemp = newTemp("type_tmp");
    SymExpr*   temp     = new SymExpr(typeTemp);

    Expr*      typeExpr = field->exprType->copy();
    CallExpr*  initCall = new CallExpr(PRIM_INIT, typeExpr);
    Symbol*    _this    = fn->_this;
    Symbol*    name     = new_CStringSymbol(field->sym->name);

    if (field->sym->hasFlag(FLAG_PARAM) == true) {
      typeTemp->addFlag(FLAG_PARAM);
    }

    stmt->insertBefore(new DefExpr(typeTemp));
    stmt->insertBefore(new CallExpr(PRIM_MOVE, typeTemp, initCall));
    stmt->insertBefore(new CallExpr(PRIM_INIT_FIELD, _this, name, temp));
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void fieldInitTypeWithInit(Expr*     stmt,
                                  FnSymbol* fn,
                                  DefExpr*  field,
                                  Expr*     initExpr) {
  SET_LINENO(stmt);

  Type* type     = field->sym->type;

  //
  // e.g. const x : int     = 10;
  //      var   y : int(32) = 20;
  //
  //      var   x : MyCls   = new MyCls(1, 2);
  //
  // Noakes 2017/03/21
  //    Use a temp to compute the value for the init-expression and
  //    use PRIM_MOVE to initialize x.  This simplifies const checking
  //    for the first case and supports a current limitation for RVF
  //
  if (isPrimitiveScalar(type) == true ||
      isNonGenericClass(type) == true) {
    CallExpr* fieldAccess = createFieldAccess(fn, field);

    stmt->insertBefore(new CallExpr("=", fieldAccess, initExpr->copy()));

  } else if (isNonGenericRecordWithInitializers(type) == true) {
#if 0
    if (isNewExpr(initExpr) == true) {
      Symbol*   var     = defExpr->sym;
      Expr*     arg     = toCallExpr(initExpr)->get(1)->remove();
      CallExpr* argExpr = toCallExpr(arg);

      // Insert the arg portion of the initExpr back into tree
      defExpr->insertAfter(argExpr);

      // Convert it in to a use of the init method
      argExpr->baseExpr->replace(new UnresolvedSymExpr("init"));

      // Add _mt and _this (insert at head in reverse order)
      argExpr->insertAtHead(var);
      argExpr->insertAtHead(gMethodToken);

    } else {
      Symbol*   var    = defExpr->sym;
      CallExpr* init   = new CallExpr("init", gMethodToken, var);
      CallExpr* assign = new CallExpr("=",    var,          initExpr);

      defExpr->insertAfter(init);
      init->insertAfter(assign);
    }
#endif

    INT_ASSERT(false);

  } else {
#if 0
    Symbol*    var      = defExpr->sym;
    VarSymbol* typeTemp = newTemp("type_tmp");
    DefExpr*   typeDefn = new DefExpr(typeTemp);
    Expr*      typeExpr = defExpr->exprType->remove();
    CallExpr*  initCall = new CallExpr(PRIM_INIT, typeExpr);
    CallExpr*  initMove = new CallExpr(PRIM_MOVE, typeTemp,  initCall);
    CallExpr*  assign   = new CallExpr("=",       typeTemp,  initExpr);

    defExpr ->insertAfter(typeDefn);
    typeDefn->insertAfter(initMove);
    initMove->insertAfter(assign);
    assign  ->insertAfter(new CallExpr(PRIM_MOVE, var, typeTemp));
#endif

    INT_ASSERT(false);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void fieldInitTypeInference(Expr*     stmt,
                                   FnSymbol* fn,
                                   DefExpr*  field,
                                   Expr*     initExpr) {
  SET_LINENO(stmt);

  // e.g.
  //   var x = <immediate>;
  //   var y = <identifier>;
  if (SymExpr* initSym = toSymExpr(initExpr)) {
    Type* type = initSym->symbol()->type;

    if (isPrimitiveScalar(type) == true) {
      CallExpr* fieldAccess = createFieldAccess(fn, field);

      stmt->insertBefore(new CallExpr("=", fieldAccess, initExpr->copy()));

    } else {
      Symbol* _this = fn->_this;
      Symbol* name  = new_CStringSymbol(field->sym->name);
      Expr*   value = initExpr->copy();

      stmt->insertBefore(new CallExpr(PRIM_INIT_FIELD, _this, name, value));
    }

  // e.g.
  //   var x = f(...);
  //   var y = new MyRecord(...);
  } else if (CallExpr* initCall = toCallExpr(initExpr)) {
    if (initCall->isPrimitive(PRIM_NEW) == true) {
#if 0
      AggregateType* type = typeForNewExpr(initCall);

      if (isNonGenericRecordWithInitializers(type) == true) {
        Expr*     arg1    = initCall->get(1)->remove();
        CallExpr* argExpr = toCallExpr(arg1);

        // Insert the arg portion of the initExpr back into tree
        defExpr->insertAfter(argExpr);

        // Convert it in to a use of the init method
        argExpr->baseExpr->replace(new UnresolvedSymExpr("init"));

        // Add _mt and _this (insert at head in reverse order)
        argExpr->insertAtHead(var);
        argExpr->insertAtHead(gMethodToken);

      } else {
        defExpr->insertAfter(new CallExpr(PRIM_MOVE, var, initExpr));
      }

      if (type != NULL && type->isGeneric() == false) {
        var->type = type;
      }
#endif

      INT_ASSERT(false);

    } else {
      Symbol* _this = fn->_this;
      Symbol* name  = new_CStringSymbol(field->sym->name);
      Expr*   value = initExpr->copy();

      stmt->insertBefore(new CallExpr(PRIM_INIT_FIELD, _this, name, value));
    }

  } else {
    INT_ASSERT(false);
  }
}

/************************************* | **************************************
*                                                                             *
* Determine if the callExpr represents an initialization for a field i.e.     *
*                                                                             *
*   call("=", call(".", <this>, <fieldName>), <value>);                       *
*                                                                             *
* If so return the field                                                      *
*                                                                             *
************************************** | *************************************/

static DefExpr* fieldByName(AggregateType* at, const char* name);

static DefExpr* toSuperFieldInit(AggregateType* at, CallExpr* callExpr) {
  forv_Vec(Type, t, at->dispatchParents) {
    AggregateType* pt = toAggregateType(t);

    if (DefExpr* field = toLocalFieldInit(pt, callExpr)) {
      return field;
    }
  }

  return NULL;
}

static DefExpr* toLocalFieldInit(AggregateType* at, CallExpr* callExpr) {
  DefExpr* retval = NULL;

  // The outer call has assignment syntax
  if (at != NULL && callExpr->isNamed("=") == true) {
    if (CallExpr* lhs = toCallExpr(callExpr->get(1))) {

      // The inner call has dot syntax
      if (lhs->isNamed(".") == true) {
        SymExpr* base = toSymExpr(lhs->get(1));
        SymExpr* name = toSymExpr(lhs->get(2));

        if (base != NULL && name != NULL) {
          VarSymbol* var = toVarSymbol(name->symbol());

          // The base is <this> and the slot is a fieldName
          if (base->symbol()->hasFlag(FLAG_ARG_THIS) == true &&

              var                                    != NULL &&
              var->immediate                         != NULL &&
              var->immediate->const_kind             == CONST_KIND_STRING) {
            retval = fieldByName(at, var->immediate->v_string);
          }
        }
      }
    }
  }

  return retval;
}

// Return the field with the given name
static DefExpr* fieldByName(AggregateType* at, const char* name) {
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

static CallExpr* createFieldAccess(FnSymbol* fn, DefExpr* field) {
  UnresolvedSymExpr* dot   = new UnresolvedSymExpr(".");
  Symbol*            _this = fn->_this;
  Symbol*            name  = new_CStringSymbol(field->sym->name);

  return new CallExpr(dot, _this, name);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void        phase1Analysis(FnSymbol* fn);


static void        errorCases(AggregateType* at,
                            DefExpr*       curField,
                            const char*    fieldName,
                            bool           seenField[],
                            Expr*          call);

static const char* getFieldName(Expr* curExpr);

static Expr*       modifyFieldAccess(Expr*    fieldAccess,
                                     DefExpr* curField);

static bool        isLaterFieldAccess(DefExpr*    curField,
                                      const char* fieldName);

static bool        loopAnalysis(BlockStmt*     loop,
                                DefExpr*       curField,
                                bool           seenField[],
                                int*           index,
                                AggregateType* at);

static void        insertOmittedField(Expr*          next,
                                      DefExpr*       field,
                                      AggregateType* at);

static bool        isParentField(AggregateType* at,
                                 const char*    name);

static Expr*       getNextStmt(Expr*      curExpr,
                               BlockStmt* body,
                               bool       enterLoops);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void preNormalizeGenericInit(FnSymbol* fn) {
  AggregateType* at        = toAggregateType(fn->_this->type);
  InitStyle      initStyle = findInitStyle(fn);

  // The body is pure phase 2
  if (initStyle == STYLE_NONE) {
    SET_LINENO(fn->body);

    Symbol*   superSym  = new_CStringSymbol("super");
    CallExpr* superCall = new CallExpr(".", fn->_this, superSym);

    Symbol*   initSym   = new_CStringSymbol("init");
    CallExpr* initCall  = new CallExpr(".", superCall, initSym);

    CallExpr* superInit = new CallExpr(initCall);

    // For classes:  we need to insert super.init();
    // For records:  this is a transient marker for phase 1 analysis
    fn->body->insertAtHead(superInit);

    phase1Analysis(fn);

    // Records should not call super.init() so remove the phase1 marker
    if (isRecord(at) == true) {
      superInit->remove();
    }

  // One or more uses of this.init();
  } else if (initStyle == STYLE_THIS_INIT) {
    phase1Analysis(fn);

  // At least one use of this.init();
  } else {
    phase1Analysis(fn);

    // 2017/03/16: Record inheritance is not fully defined yet
    //   The user may have added super.init() to a record initializer
    //   to separate phase 1 from phase 2.  However this will not resolve.
    //   Remove it/them now that preNormalize is complete.
    if (isRecord(at) == true) {
      std::vector<CallExpr*> calls;

      collectMyCallExprs(fn, calls, fn);

      for (size_t i = 0; i < calls.size(); i++) {
        if (findInitStyle(calls[i]) == STYLE_SUPER_INIT) {
          calls[i]->remove();
        }
      }
    }
  }

  // Insert phase 2 analysis here


  // If this is a non-generic class then create a type method
  // to wrap this initializer
  if (isClass(at) == true && at->isGeneric() == false) {
    buildClassAllocator(fn);

    fn->addFlag(FLAG_INLINE);
  }
}

/************************************* | **************************************
*                                                                             *
* Finds the appropriate next statement to examine                             *
*                                                                             *
************************************** | *************************************/

static
Expr* getNextStmt(Expr* curExpr, BlockStmt* body, bool enterLoops) {
  Expr* toReturn = NULL;
  if (curExpr->next) {
    // If there is a next statement at this level, return it (in some form)
    toReturn = curExpr->next;
  } else if (curExpr->parentExpr != body) {
    // Otherwise, if we have dived into some nested scope, try going up a level
    // from that scope to find the next statement.  If there isn't one, keep
    // going up until there is a next statement or we reach "body".
    Expr* scope = curExpr->parentExpr;
    while (scope->next == NULL && scope->parentExpr != body) {
      scope = scope->parentExpr;
    }
    toReturn = scope->next;
  }
  if (toReturn != NULL) {
    while (BlockStmt* block = toBlockStmt(toReturn)) {
      if (block->isLoopStmt() && !enterLoops) {
        // We found a loop and we don't want to dive into loops.  Return it.
        return block;
      } else {
        // continue to dive in
        toReturn = block->body.head;
      }
    }
    // While loop is exited the first time we find an Expr that isn't a
    // BlockStmt.  We return from the loop if the Expr is a BlockStmt we don't
    // want to dive into.
  }
  return toReturn;
}

static void phase1Analysis(FnSymbol* fn) {
  Symbol*        _this     = fn->_this;
  AggregateType* at        = toAggregateType(_this->type);

  bool*          seenField = (bool*) calloc(at->fields.length, sizeof(bool));

  DefExpr*       curField  = toDefExpr(at->fields.head);

  BlockStmt*     body      = fn->body;
  Expr*          curExpr   = body->body.head;
  InitStyle      isInit    = findInitStyle(curExpr);

  int            index     = 0;

  if (curField != NULL && isClass(at) == true) {
    INT_ASSERT(curField->sym->hasFlag(FLAG_SUPER_CLASS));

    // the super field is always first
    seenField[0] = true;
  }

  if (isInit == STYLE_NONE) {
    // solution to fence post issue of diving into nested block statements
    BlockStmt* block = toBlockStmt(curExpr);
    while (block && !block->isLoopStmt()) {
      curExpr = block->body.head;
      block = toBlockStmt(curExpr);
    }
  }

  // We are guaranteed to never reach the end of the body, due to the
  // conditional surrounding the call to this function.
  while (curField != NULL || (isInit == STYLE_NONE)) {
    // Verify that:
    // - fields are initialized in declaration order
    // - The "this" instance is only used to clarify a field initialization (or
    //   to refer to a field which has already been initialized
    // - (no later fields are accessed by earlier ones?  Or will def/use
    //   analysis handle this?)
    // - no parent fields are accessed
    // - loops and parallel statements do not contain field initialization
    //   - no begin expressions?

    // Additionally, perform the following actions:
    // - add initialization for omitted fields
    if (curField != NULL && (isInit == STYLE_NONE)) {
      // still have phase 1 statements and fields left to traverse

      if (BlockStmt* block = toBlockStmt(curExpr)) {
        if (block->isLoopStmt()) {
          // Special handling for loops.
          bool foundInit = loopAnalysis(block,
                                        curField,
                                        seenField,
                                        &index,
                                        at);

          if (foundInit) {
            // If the init call was in the loop body we just checked,
            // any further action is incorrect (including the insertion
            // of leftover omitted fields, since those fields would be
            // inserted into the loop body and we don't allow that),
            // so just exit the loop through the body and allow normal
            // clean up to occur.
            break;
          }
          curExpr = getNextStmt(curExpr, body, false);
          isInit = findInitStyle(curExpr);
          continue;
        }
      }

      if (const char* fieldname = getFieldName(curExpr)) {
        if (!strcmp(fieldname, curField->sym->name)) {
          // It's a match! Change the assignment into a special primitive, in
          // case generics are involved on this type
          curExpr = modifyFieldAccess(curExpr, curField);
          // Advance both and move on
          curField = toDefExpr(curField->next);
          curExpr = getNextStmt(curExpr, body, false);
          isInit = findInitStyle(curExpr);
          seenField[index] = true;
          index++;

        } else if (isLaterFieldAccess(curField, fieldname)) {
          insertOmittedField(curExpr, curField, at);
          index++;
          curField = toDefExpr(curField->next);

        } else {
          // It's not a valid field access at all.  Error cases!
          errorCases(at, curField, fieldname, seenField, curExpr);
          curExpr = getNextStmt(curExpr, body, false);
          isInit = findInitStyle(curExpr);
        }

      } else {
        // Wasn't a field access, only update curExpr;
        curExpr = getNextStmt(curExpr, body, false);
        isInit = findInitStyle(curExpr);
      }

    } else if (curField != NULL) {
      // only fields left

      insertOmittedField(curExpr, curField, at);
      curField = toDefExpr(curField->next);
      index++;
    } else {
      // only phase 1 statements left.

      if (BlockStmt* block = toBlockStmt(curExpr)) {
        if (block->isLoopStmt()) {
          // Special handling for loops.
          bool foundInit = loopAnalysis(block,
                                        curField,
                                        seenField,
                                        &index,
                                        at);

          if (foundInit) {
            // If the init call was in the loop body we just checked,
            // any further action is incorrect (including the insertion
            // of leftover omitted fields, since those fields would be
            // inserted into the loop body and we don't allow that),
            // so just exit the loop through the body and allow normal
            // clean up to occur.
            break;
          }
        }

      } else if (const char* fieldname = getFieldName(curExpr)) {
        errorCases(at, curField, fieldname, seenField, curExpr);
      }

      curExpr = getNextStmt(curExpr, body, false);
      isInit = findInitStyle(curExpr);
    }
  }

  free(seenField);
}

// Checks if the current expression is a call expression that sets the value
// of a field, and if so returns that field name.
static
const char* getFieldName(Expr* curExpr) {
  if (CallExpr* call = toCallExpr(curExpr)) {
    if (call->isNamed("=")) {
      if (CallExpr* inner = toCallExpr(call->get(1))) {
        if (inner->isNamed(".")) {
          SymExpr* potenThis = toSymExpr(inner->get(1));
          if (potenThis && potenThis->symbol()->hasFlag(FLAG_ARG_THIS)) {
            // It's an access of this!
            SymExpr* sym = toSymExpr(inner->get(2));
            INT_ASSERT(sym);
            // Could it be . . . a field?  The anticipation is killing me!
            if (VarSymbol* var = toVarSymbol(sym->symbol())) {
              if (var->immediate->const_kind == CONST_KIND_STRING) {
                return var->immediate->v_string;
              }
            }
          }
        }
      }
    }
  }
  return NULL;
}

// Transforms the assignments to fields we found in the initializer body into
// a special form, that will allow function resolution to not complain when
// they are encountered if we are still determining the generic instantiation
// of the type
static
Expr* modifyFieldAccess(Expr* fieldAccess, DefExpr* curField) {
  CallExpr* call = toCallExpr(fieldAccess);
  INT_ASSERT(call);
  CallExpr* inner = toCallExpr(call->get(1));
  INT_ASSERT(inner);
  SymExpr* argThis = toSymExpr(inner->get(1)->remove());
  // Fair assumptions, since we just came from getFieldName giving us the
  // appropriate set up.


  SET_LINENO(fieldAccess);

  CallExpr* replacement = new CallExpr(PRIM_INIT_FIELD,
                                       argThis,
                                       new_CStringSymbol(curField->sym->name));

  for_actuals(actual, call) {
    if (actual == call->get(1)) {
      // The first arg is the this.field access, which we have already
      // represented in the new call.
      continue;
    }
    replacement->insertAtTail(actual->copy());
    // Don't want to remove those as that will mess with the for loop traversal
    // so insert a copy of them into the new call
  }
  call->replace(replacement);
  return replacement;
}

static
bool isLaterFieldAccess(DefExpr* curField, const char* fieldname) {
  DefExpr* next = curField;
  while (next) {
    if (!strcmp(next->sym->name, fieldname)) {
      return true;
    }
    next = toDefExpr(next->next);
  }
  return false;
}

// If the fieldname given is the name of a field after the last seen field in
// the class type t, then everything is fine.  Otherwise, give an appropriate
// error message depending on if the field is being initialized out of order,
// if the field has already been initialized, or if what is being initialized
// is not a field at all.
static
void errorCases(AggregateType* t, DefExpr* curField, const char* fieldname,
                bool seenField[], Expr* call) {
  int index = 0;
  for (DefExpr* fieldDef = toDefExpr(t->fields.head); fieldDef != curField;
       fieldDef = toDefExpr(fieldDef->next), index++) {
    if (!strcmp(fieldDef->sym->name, fieldname)) {
      // We found a field match before the field we most recently saw
      // initialization for.
      if (seenField[index]) {
        // There was a previous initialization of this same field
        USR_FATAL_CONT(call, "multiple initializations of field \"%s\"", fieldname);
      } else {
        USR_FATAL_CONT(call, "field initialization out of order");
        USR_PRINT(call, "initialization of fields before .init() call must be in field declaration order");
      }
      return; // exit early due to error case.
    }
  }
  if (isParentField(t, fieldname)) {
    USR_FATAL_CONT(call, "can't set value of field \"%s\" from parent type during phase 1 of initialization", fieldname);
  } else {
    // We didn't find the field match, even on our parent type.  It is a method.
    USR_FATAL_CONT(call, "attempted method call too early during initialization");
  }
}

static
bool isParentField(AggregateType* t, const char *name) {
  if (t->dispatchParents.n > 0) {
    forv_Vec(Type, pt, t->dispatchParents) {
      if (AggregateType* cpt = toAggregateType(pt)) {
        for_fields(field, cpt) {
          if (!strcmp(field->name, name)) {
            return true;
          }
        }
        if (isParentField(cpt, name)) {
          return true;
        }
      }
    }
    return false;
  } else {
    return false;
  }
}

static void insertOmittedField(Expr* next, DefExpr* field, AggregateType* t) {
  SET_LINENO(next);

  const char* nextField = field->sym->name;

  // Do something appropriate with "super"
  if (field->sym->hasFlag(FLAG_SUPER_CLASS)) {

  // For all other fields, insert an assignment into that field with the given
  // initialization, if we have one.
  } else if (!field->init && !field->exprType) {
    USR_FATAL_CONT(next,
                   "can't omit initialization of field \"%s\", "
                   "no type or default value provided",
                   nextField);

  } else {
    Symbol*   _this   = toFnSymbol(next->parentSymbol)->_this;

    CallExpr* newInit = new CallExpr(PRIM_INIT_FIELD,
                                     new SymExpr(_this),
                                     new_CStringSymbol(nextField));

    if (field->init) {
      newInit->insertAtTail(field->init->copy());

    } else {
      INT_ASSERT(field->exprType);

      VarSymbol* tmp     = newTemp("call_tmp");

      next->insertBefore(new DefExpr(tmp));

      next->insertBefore(new CallExpr(PRIM_MOVE,
                                      new SymExpr(tmp),
                                      new CallExpr(PRIM_INIT,
                                                   field->exprType->copy())));

      if (field->sym->hasFlag(FLAG_PARAM) == true) {
        tmp->addFlag(FLAG_PARAM);
      }

      newInit->insertAtTail(new SymExpr(tmp));
    }
    next->insertBefore(newInit);
  }
}

// Takes in the current expr (which must be a loop), the current field
// expected, the list of fields seen, the index into that list, the type whose
// fields are being iterated over (this is for error message purposes), and the
// expected init call for this function.  Walks the statements in the loop, and
// if any of them are field initializations, will generate an error message
// (as field initialization must be executed in order during Phase 1, and
// cannot be repeated).  If the initCall is encountered while performing this
// traversal, immediately cease performing this check and indicate that we
// encountered it by returning "true".
static bool loopAnalysis(BlockStmt*     loop,
                         DefExpr*       curField,
                         bool           seenField[],
                         int*           index,
                         AggregateType* t) {
  Expr*     stmt   = loop->body.head;
  InitStyle isInit = findInitStyle(stmt);

  while (stmt != NULL) {
    if (BlockStmt* inner = toBlockStmt(stmt)) {
      bool sawInit = loopAnalysis(inner, curField, seenField, index, t);
      if (sawInit) {
        return sawInit;
      }
    }
    if (isInit == STYLE_SUPER_INIT || isInit == STYLE_THIS_INIT) {
      // We encountered the .init() call while traversing this loop.  Stop
      // performing Phase 1 analysis and return.
      USR_FATAL_CONT(stmt,
                     "use of %s.init() call in loop body",
                     isInit == STYLE_SUPER_INIT ? "super" : "this");

      return true;
    }

    if (const char* fieldname = getFieldName(stmt)) {
      if (curField && !strcmp(fieldname, curField->sym->name)) {
        USR_FATAL_CONT(stmt,
                       "can't initialize field \"%s\" inside a "
                       "loop during phase 1 of initialization",
                       fieldname);
        seenField[*index] = true;
        (*index)++;
        stmt = getNextStmt(stmt, loop, true);
        isInit = findInitStyle(stmt);
      } else if (curField && isLaterFieldAccess(curField, fieldname)) {
        (*index)++;
        curField = toDefExpr(curField->next);
      } else {
        // Something else invalid has occurred
        // Should I also warn that it is occurring inside a loop?
        errorCases(t, curField, fieldname, seenField, stmt);
        stmt = getNextStmt(stmt, loop, true);
        isInit = findInitStyle(stmt);
      }
    } else {
      stmt = getNextStmt(stmt, loop, true);
      isInit = findInitStyle(stmt);
    }
  }
  // traverse the statements of the loop, diving into loops and blocks as
  // encountered

  return false;
}

/************************************* | **************************************
*                                                                             *
* Functions to detect uses of this.init(args) or super.init(args)             *
*                                                                             *
*   The 1st function iterates over statements in an FnSymbol.                 *
*   The 2nd function tests a single stmt.                                     *
*                                                                             *
************************************** | *************************************/

static InitStyle findInitStyleInner(CallExpr* expr);
static bool      isStringLiteral(Expr* expr, const char* name);
static bool      isUnresolvedSymbol(Expr* expr, const char* name);
static bool      isSymbolThis(Expr* expr);

static InitStyle findInitStyle(FnSymbol* fn) {
  // Behavior is not yet correct for super/this.init() calls within
  // if statements.  TODO: fix this
  Expr*      curExpr = fn->body->body.head;
  BlockStmt* block   = toBlockStmt(curExpr);
  InitStyle  retval  = STYLE_NONE;

  // Peel off nested top-level blocks
  while (block != NULL && block->isLoopStmt() == false) {
    curExpr = block->body.head;
    block   = toBlockStmt(curExpr);
  }

  while (curExpr != NULL && retval == STYLE_NONE) {
    if ((retval = findInitStyle(curExpr)) == STYLE_NONE) {
      curExpr = getNextStmt(curExpr, fn->body, true);
    }
  }

  return retval;
}

//
// Determine if the current stmt is one of
//
//    this.init(args);
//      => call(call(".", this,                     "init") .. args)
//
//    super.init(args)
//      => call(call(".", unresolved("super"),      "init") .. args)  or
//      => call(call(".", call(".", this, "super"), "init") .. args)
//

static InitStyle findInitStyle(Expr* stmt) {
  InitStyle retval = STYLE_NONE;

  if (CallExpr* call = toCallExpr(stmt)) {
    if (CallExpr* inner = toCallExpr(call->baseExpr)) {
      retval = findInitStyleInner(inner);
    }
  }

  return retval;
}

//
// Determine if the current expr is one of
//
//    this.init(args);
//      => call(".", this,                     "init")
//
//    super.init(args)
//      => call(".", unresolved("super"),      "init")     // records
//      => call(".", call(".", this, "super"), "init")     // classes
//

static InitStyle findInitStyleInner(CallExpr* call) {
  InitStyle retval = STYLE_NONE;

  if (call->numActuals()                    ==    2 &&
      call->isNamed(".")                    == true &&
      isStringLiteral(call->get(2), "init") == true) {

    if (isSymbolThis(call->get(1)) == true) {
      retval = STYLE_THIS_INIT;

    } else {
      // "super" is an unresolved symbol for records
      if (isUnresolvedSymbol(call->get(1), "super") == true) {
        retval = STYLE_SUPER_INIT;

      // "super" is a call to a field accessor for classes
      } else if (CallExpr* subCall = toCallExpr(call->get(1))) {
        if (subCall->numActuals()                     == 2    &&
            subCall->isNamed(".")                     == true &&
            isSymbolThis(subCall->get(1))             == true &&
            isStringLiteral(subCall->get(2), "super") == true) {
          retval = STYLE_SUPER_INIT;
        }
      }
    }
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

/************************************* | **************************************
*                                                                             *
* Consider                                                                    *
*                                                                             *
*   var x = new MyClass(10, 20, 30);                                          *
*                                                                             *
* and assume MyClass defines an initializer that accepts 3 integers.          *
*                                                                             *
* The goal is to allocate an instance of MyClass on the heap and then invoke  *
* the appropriate init method on this instance.                               *
*                                                                             *
* This is implemented by defining an internal "type method" on MyClass that   *
* performs the allocation and then invokes the init method on the resulting   *
* instance.  Note that there is a distinct _new method for every init         *
* method.                                                                     *
*                                                                             *
************************************** | *************************************/

FnSymbol* buildClassAllocator(FnSymbol* initMethod) {
  Symbol*        _this       = initMethod->_this;
  AggregateType* at          = toAggregateType(_this->type);

  SET_LINENO(at);

  FnSymbol*      fn          = new FnSymbol("_new");
  BlockStmt*     body        = fn->body;
  ArgSymbol*     type        = new ArgSymbol(INTENT_BLANK, "t", at);
  VarSymbol*     newInstance = newTemp("instance", at);
  CallExpr*      allocCall   = callChplHereAlloc(at);
  CallExpr*      initCall    = NULL;

  if (initMethod->hasFlag(FLAG_RESOLVED)) {
    initCall = new CallExpr(initMethod, gMethodToken, newInstance);
  } else {
    initCall = new CallExpr("init", gMethodToken, newInstance);
  }

  type->addFlag(FLAG_TYPE_VARIABLE);

  fn->addFlag(FLAG_METHOD);
  fn->addFlag(FLAG_COMPILER_GENERATED);

  fn->retType = at;

  // Add the formal that provides the type for the type method
  fn->insertFormalAtTail(type);

  //
  // Walk the formals to the init method
  // Ignore _mt and _this
  //   1) add a corresponding formal to the new type method
  //   2) add that formal to the call to "init"
  //
  int count = 1;

  for_formals(formal, initMethod) {
    // Ignore _mt and this
    if (count >= 3) {
      ArgSymbol* arg = formal->copy();

      fn->insertFormalAtTail(arg);
      initCall->insertAtTail(new SymExpr(arg));
    }

    count = count + 1;
  }

  // Construct the body
  body->insertAtTail(new DefExpr(newInstance));

  body->insertAtTail(new CallExpr(PRIM_MOVE,   newInstance, allocCall));
  body->insertAtTail(new CallExpr(PRIM_SETCID, newInstance));

  body->insertAtTail(initCall);

  body->insertAtTail(new CallExpr(PRIM_RETURN, newInstance));


  // Insert the definition in to the tree
  at->symbol->defPoint->insertBefore(new DefExpr(fn));

  return fn;
}
