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

#include "initializerRules.h"

#include "astutil.h"
#include "expr.h"
#include "ForallStmt.h"
#include "InitNormalize.h"
#include "passes.h"
#include "resolution.h"
#include "stmt.h"
#include "TryStmt.h"
#include "type.h"
#include "typeSpecifier.h"

static bool     isInitStmt (CallExpr* stmt);

static bool     isUnacceptableTry(Expr* stmt);

static void     preNormalizeInit(FnSymbol* fn);

static void     unifyConditionalBranchLastField(AggregateType* at,
                                                CondStmt*      cond,
                                                InitNormalize* stateThen,
                                                InitNormalize* stateElse);

static DefExpr* toSuperFieldInit(AggregateType* at, CallExpr* expr);
static DefExpr* toLocalFieldInit(AggregateType* at, CallExpr* expr);

static bool     isAssignment(CallExpr* callExpr);
static bool     isSimpleAssignment(CallExpr* callExpr);
static bool     isCompoundAssignment(CallExpr* callExpr);

static void     transformSuperInit(CallExpr* initCall);

static bool     isStringLiteral(Expr* expr, const char* name);
static bool     isSymbolThis(Expr* expr);

static void     addSuperInit(FnSymbol* fn);

/************************************* | **************************************
*                                                                             *
* Attempt to assign a type to the symbol for each field in some of the        *
* simpler cases.                                                              *
*                                                                             *
* 2017/03/20 Noakes: This may set a direction for refactoring resolution in   *
* a subsequent release.                                                       *
*                                                                             *
************************************** | *************************************/

static Type*          typeForExpr(Expr* expr);
static AggregateType* typeForNewExpr(CallExpr* newExpr);

void preNormalizeFields(AggregateType* at) {
  for_alist(field, at->fields) {
    if (DefExpr* defExpr = toDefExpr(field)) {
      if (defExpr->sym->hasFlag(FLAG_PARAM)         == false &&
          defExpr->sym->hasFlag(FLAG_TYPE_VARIABLE) == false) {
        Type* type = NULL;

        if (Expr* typeExpr = defExpr->exprType) {
          type = typeForTypeSpecifier(typeExpr, false);

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

        } else if (defExpr->init != NULL) {
          type = typeForExpr(defExpr->init);
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

// Infer the type of an expression for simple cases.
//   1) An immediate value for one of the primitive scalars
//   2) A new expr
static Type* typeForExpr(Expr* expr) {
  Type* retval = NULL;

  if (SymExpr* symExpr = toSymExpr(expr)) {
    Type* type = symExpr->symbol()->type;

    if (isPrimitiveScalar(type) == true) {
      retval = type;
    }

  } else if (CallExpr* initCall = toCallExpr(expr)) {
    if (initCall->isPrimitive(PRIM_NEW) == true) {
      retval = typeForNewExpr(initCall);
    }
  }

  return retval;
}

static AggregateType* typeForNewExpr(CallExpr* newExpr) {
  AggregateType* retval = NULL;

  if (CallExpr* constructor = toCallExpr(newExpr->get(1))) {
    if (SymExpr* baseExpr = toSymExpr(constructor->baseExpr)) {
      if (TypeSymbol* sym = toTypeSymbol(baseExpr->symbol())) {
        if (AggregateType* type = toAggregateType(sym->type)) {
          if (isClass(type) == true || isRecord(type) == true) {
            retval = type;
          }
        }
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

static bool isReturnVoid(FnSymbol* fn);

void preNormalizeInitMethod(FnSymbol* fn) {
  if (fn->hasFlag(FLAG_NO_PARENS) == true) {
    USR_FATAL(fn, "an initializer cannot be declared without parentheses");

  } else if (isReturnVoid(fn) == false) {
    USR_FATAL(fn, "an initializer cannot return a non-void result");

  } else {
    preNormalizeInit(fn);

    errorOnFieldsInArgList(fn);
  }
}

// Generates an error if a field is used in the argument list to an initializer
// or constructor
// Lydia NOTE 09/14/17: Make this static and unexported once constructors are
// deprecated
void errorOnFieldsInArgList(FnSymbol* fn) {
  for_formals(formal, fn) {
    std::vector<SymExpr*> symExprs;

    collectSymExprs(formal, symExprs);

    for_vector(SymExpr, se, symExprs) {
      if (se->symbol() == fn->_this) {
        USR_FATAL_CONT(se,
                       "invalid access of class member in "
                       "initializer argument list");

        break;
      }
    }
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

    collectMyCallExprs(fn->body, calls, fn);

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

static void          preNormalizeInitRecord(FnSymbol* fn);

static void          preNormalizeInitClass(FnSymbol* fn);

static InitNormalize preNormalize(AggregateType* at,
                                  BlockStmt*     block,
                                  InitNormalize  state);

static InitNormalize preNormalize(AggregateType* at,
                                  BlockStmt*     block,
                                  InitNormalize  state,
                                  Expr*          start);

static void preNormalizeInit(FnSymbol* fn) {
  AggregateType* at = toAggregateType(fn->_this->type);

  if (fn->throwsError() == true) {
    USR_FATAL(fn, "initializers are not yet allowed to throw errors");

  } else if (at->isRecord() == true) {
    preNormalizeInitRecord(fn);

  } else if (at->isClass()  == true) {
    preNormalizeInitClass(fn);

  } else {
    INT_ASSERT(false);
  }
}

static void preNormalizeInitRecord(FnSymbol* fn) {
  InitNormalize  state(fn);

  AggregateType* at    = toAggregateType(fn->_this->type);
  ArgSymbol*     _this = toArgSymbol(fn->_this);

  // The body contains at least one instance of this.init() or super.init()
  if (state.isPhase0() == true || state.isPhase1() == true) {
    InitNormalize finalState = preNormalize(at, fn->body, state);
    finalState.initializeFieldsAtTail(fn->body);

  } else {
    INT_ASSERT(false);
  }

  if (at->isGeneric() == true) {
    fn->_this->addFlag(FLAG_DELAY_GENERIC_EXPANSION);
  }

  if (_this->intent   == INTENT_BLANK) {
    _this->intent = INTENT_REF;
  }
}

static void preNormalizeInitClass(FnSymbol* fn) {
  InitNormalize  state(fn);

  AggregateType* at = toAggregateType(fn->_this->type);

  // The body contains at least one instance of this.init() or super.init()
  // i.e. the body is not empty and we do not need to insert super.init()
  if (state.isPhase0() == true) {

    InitNormalize finalState = preNormalize(at, fn->body, state);
    finalState.initializeFieldsAtTail(fn->body);

    // If an initDone was not encountered, we need to set the class ID on our
    // way out of this initializer.
    if (finalState.isPhase1() == true) {
      fn->insertAtTail(new CallExpr(PRIM_SETCID, fn->_this));
    }

  // The body contains zero or more instances of this.initDone()
  // A super.init is not present because the state is phase one, so we need to
  // insert one.
  } else if (state.isPhase1() == true) {
    Expr* startStmt = fn->body->body.head;

    CallExpr* dummy = new CallExpr("init", gMethodToken, fn->_this);
    fn->insertAtHead(dummy);
    state.makeThisAsParent(dummy);
    dummy->remove();

    // Call with 'startStmt' to avoid processing the 'this as parent' setup
    InitNormalize finalState = preNormalize(at, fn->body, state, startStmt);
    finalState.initializeFieldsAtTail(fn->body);

    addSuperInit(fn);

    // If an initDone was not encountered, we need to set the class ID on our
    // way out of this initializer.
    if (finalState.isPhase1() == true) {
      fn->insertAtTail(new CallExpr(PRIM_SETCID, fn->_this));
    }

  } else {
    INT_ASSERT(false);
  }

  if (at->isGeneric() == false) {
    buildClassAllocator(fn);
    fn->addFlag(FLAG_INLINE);

  } else {
    fn->_this->addFlag(FLAG_DELAY_GENERIC_EXPANSION);
  }
}

static void checkLocalPhaseOneErrors(const InitNormalize& state,
                                     DefExpr* field,
                                     CallExpr* callExpr) {
  if (isCompoundAssignment(callExpr) == true) {
    USR_FATAL(callExpr,
              "cannot apply compound assignment to field \"%s\" "
              "in phase 1",
              field->sym->name);

  } else if (state.isFieldReinitialized(field) == true) {
    USR_FATAL(callExpr,
              "multiple initializations of field \"%s\"",
              field->sym->name);

  } else if (state.inLoopBody()     == true ||
             state.inOnInLoopBody() == true) {
    USR_FATAL(callExpr,
              "can't initialize field \"%s\" inside a "
              "loop during phase 1 of initialization",
              field->sym->name);

  } else if (state.inParallelStmt()     == true ||
             state.inOnInParallelStmt() == true) {
    USR_FATAL(callExpr,
              "can't initialize field \"%s\" inside a "
              "parallel statement during phase 1 of initialization",
              field->sym->name);

  } else if (state.inCoforall()     == true ||
             state.inOnInCoforall() == true) {
    USR_FATAL(callExpr,
              "can't initialize field \"%s\" inside a "
              "coforall during phase 1 of initialization",
              field->sym->name);


  } else if (state.inForall()     == true ||
             state.inOnInForall() == true) {
    USR_FATAL(callExpr,
              "can't initialize field \"%s\" inside a "
              "forall during phase 1 of initialization",
              field->sym->name);
  }
}

static void checkInvalidInit(InitNormalize& state, CallExpr* callExpr) {
  const char* initName = NULL;
  if (isSuperInit(callExpr) == true) {
    initName = "super.init()";
  } else if (isThisInit(callExpr) == true) {
    initName = "this.init()";
  } else if (isInitDone(callExpr) == true) {
    initName = "this.complete()";
  }

  if (initName == NULL) {
    INT_FATAL("Called 'checkInvalidInit' with invalid call");
  }

  if (state.isPhase2() == true) {
    USR_FATAL(callExpr, "use of %s call in phase 2", initName);

  } else if (state.inLoopBody() == true) {
    USR_FATAL(callExpr, "use of %s call in loop body", initName);

  } else if (state.inParallelStmt() == true) {
    USR_FATAL(callExpr,
              "use of %s call in a parallel statement", initName);

  } else if (state.inCoforall() == true) {
    USR_FATAL(callExpr,
              "use of %s call in a coforall loop body", initName);

  } else if (state.inForall() == true) {
    USR_FATAL(callExpr,
              "use of %s call in a forall loop body", initName);

  } else if (state.inOn() == true) {
    USR_FATAL(callExpr,
              "use of %s call in an on block", initName);
  }
}

static InitNormalize preNormalize(AggregateType* at,
                                  BlockStmt*     block,
                                  InitNormalize  state) {
  return preNormalize(at, block, state, block->body.head);
}

static InitNormalize preNormalize(AggregateType* at,
                                  BlockStmt*     block,
                                  InitNormalize  state,
                                  Expr*          stmt) {
  // This sub-block may have a different phase than the parent
  state.checkPhase(block);

  while (stmt != NULL) {
    if (isUnacceptableTry(stmt) == true) {
      USR_FATAL(stmt,
                "Only catch-less try! statements are allowed in "
                "initializers for now");

    } else if (isDefExpr(stmt) == true) {
      state.processThisUses(stmt);

      stmt = stmt->next;

    } else if (CallExpr* callExpr = toCallExpr(stmt)) {
      if (callExpr->isPrimitive(PRIM_THROW) == true) {
        USR_FATAL(callExpr,
                  "initializers are not yet allowed to throw errors");

      // Stmt is super.init() or this.init()
      } else if (isInitStmt(callExpr) == true) {
        checkInvalidInit(state, callExpr);
        if (isThisInit(callExpr) == true) {
          INT_ASSERT(state.isPhase0() == true);
          state.completePhase1(callExpr);

          stmt = callExpr->next;

        } else if (isSuperInit(callExpr) == true) {
          Expr* next = callExpr->next;

          INT_ASSERT(state.isPhase0() == true);
          state.completePhase0(callExpr);

          if (at->isRecord() == true) {
            USR_FATAL_CONT(stmt, "super.init() not allowed in records");
            callExpr->remove();

          } else if (at->symbol->hasFlag(FLAG_EXTERN) == true) {
            callExpr->remove();

          } else {
            transformSuperInit(callExpr);
            state.makeThisAsParent(callExpr);
          }

          stmt = next;

        } else {
          INT_ASSERT(false);
        }

      } else if (isInitDone(callExpr) == true) {
        Expr* next = stmt->next;

        checkInvalidInit(state, callExpr);
        state.completePhase1(callExpr);

        stmt->remove();

        stmt = next;

      // Stmt is simple/compound assignment to a local field
      } else if (DefExpr* field = toLocalFieldInit(state.type(), callExpr)) {
        if (state.isPhase0() == true) {
          USR_FATAL(stmt,
                    "field initialization not allowed before super.init() or this.init()");

        } else if (state.isPhase2() == true) {
          if (field->sym->hasFlag(FLAG_CONST) == true) {
            USR_FATAL(stmt,
                      "cannot update a const field, \"%s\", in phase 2",
                      field->sym->name);

          } else if (field->sym->hasFlag(FLAG_PARAM) == true) {
            USR_FATAL(stmt,
                      "cannot update a param field, \"%s\", in phase 2",
                      field->sym->name);

          } else if (field->sym->hasFlag(FLAG_TYPE_VARIABLE)) {
            USR_FATAL(stmt,
                      "cannot update a type field, \"%s\", in phase 2",
                      field->sym->name);

          } else {
            stmt = stmt->next;
          }
        } else if (state.isFieldInitialized(field) == false) {
          checkLocalPhaseOneErrors(state, field, callExpr);
          stmt = state.fieldInitFromInitStmt(field, callExpr);
        } else if (state.isFieldImplicitlyInitialized(field) == true) {
          USR_FATAL_CONT(stmt,
                         "Field \"%s\" initialized out of order",
                         field->sym->name);
          USR_PRINT(stmt, "initialization of fields before .init() call must be in field declaration order");
          USR_STOP();

        } else if ((field->sym->hasFlag(FLAG_CONST) == true ||
                    field->sym->hasFlag(FLAG_PARAM) == true ||
                    field->sym->hasFlag(FLAG_TYPE_VARIABLE) == true) &&
                   state.isFieldReinitialized(field) == true) {
          USR_FATAL(callExpr,
                    "multiple initializations of field \"%s\"",
                    field->sym->name);

        } else {
          stmt = stmt->next;
        }

      // Stmt is assignment to a super field
      } else if (DefExpr* field = toSuperFieldInit(state.type(), callExpr)) {
        if (state.isPhase2() == false) {
          USR_FATAL(stmt,
                    "can't set value of field \"%s\" from parent type "
                    "during phase 1 of initialization",
                    field->sym->name);

        } else {
          if (field->sym->hasFlag(FLAG_CONST) == true) {
            USR_FATAL(stmt,
                      "cannot update a const field, \"%s\", "
                      "from parent type in phase 2",
                      field->sym->name);

          } else if (field->sym->hasFlag(FLAG_PARAM) == true) {
            USR_FATAL(stmt,
                      "cannot update a param field, \"%s\", "
                      "from parent type in phase 2",
                      field->sym->name);

          } else if (field->sym->hasFlag(FLAG_TYPE_VARIABLE)) {
            USR_FATAL(stmt,
                      "cannot update a type field, \"%s\", "
                      "from parent type in phase 2",
                      field->sym->name);

          } else {
            stmt = stmt->next;
          }
        }

      // No action required
      } else {
        state.processThisUses(stmt);

        stmt = stmt->next;
      }

    } else if (CondStmt* cond = toCondStmt(stmt)) {
      if (cond->elseStmt == NULL) {
        InitNormalize::InitPhase phaseThen = state.startPhase(cond->thenStmt);
        InitNormalize            stateThen = preNormalize(
                                                  at,
                                                  cond->thenStmt,
                                                  InitNormalize(cond, state));

        if (state.isPhase2() == false) {
          if (stateThen.isPhase2() == true) {
            if (phaseThen == InitNormalize::cPhase0) {
              USR_FATAL(cond,
                        "use of this.init() in a conditional stmt "
                        "in phase 1");

            } else if (phaseThen == InitNormalize::cPhase1) {
              USR_FATAL(cond,
                        "use of super.init() in a conditional stmt "
                        "in phase 1");

            } else {
              INT_ASSERT(false);
            }
          }

          if (stateThen.currField() != state.currField()) {
            cond->elseStmt = new BlockStmt();
            insert_help(cond->elseStmt, cond, cond->parentSymbol);

            state.initializeFieldsThroughField(cond->elseStmt,
                                               stateThen.currField());
            INT_ASSERT(stateThen.currField() == state.currField());
          }
        }

      } else {
        InitNormalize stateThen = preNormalize(at,
                                               cond->thenStmt,
                                               InitNormalize(cond, state));

        InitNormalize stateElse = preNormalize(at,
                                               cond->elseStmt,
                                               InitNormalize(cond, state));

        if (state.isPhase2() == false) {
          // Only one branch contained an init
          if (stateThen.isPhase2() != stateElse.isPhase2()) {
            USR_FATAL(cond,
                      "Both arms of a conditional must use this.init() "
                      "or this.complete() in phase 1");

          } else if (stateThen.currField() != stateElse.currField()) {
            unifyConditionalBranchLastField(at, cond, &stateThen, &stateElse);
            stateThen.merge(stateElse);
            state.merge(stateThen);

          } else {
            state.merge(stateThen);
          }
        }
      }

      stmt = stmt->next;

    } else if (LoopStmt* loop = toLoopStmt(stmt)) {
      preNormalize(at, (BlockStmt*) stmt, InitNormalize(loop, state));
      stmt = stmt->next;

    } else if (ForallStmt* forall = toForallStmt(stmt)) {
      preNormalize(at,
                   forall->loopBody(),
                   InitNormalize(forall, state));
      stmt = stmt->next;

    } else if (BlockStmt* block = toBlockStmt(stmt)) {
      state.merge(preNormalize(at,
                               block,
                               InitNormalize(block, state)));
      stmt = stmt->next;

    } else {
      stmt = stmt->next;
    }
  }

  return state;
}

void unifyConditionalBranchLastField(AggregateType* at,
                                     CondStmt*      cond,
                                     InitNormalize* stateThen,
                                     InitNormalize* stateElse) {
  DefExpr* thenFieldDef = stateThen->currField();
  DefExpr* elseFieldDef = stateElse->currField();
  int thenFieldPos = -1;
  int elseFieldPos = -1;

  if (thenFieldDef != NULL) {
    thenFieldPos = at->getFieldPosition(thenFieldDef->name());
  }

  if (elseFieldDef != NULL) {
    elseFieldPos = at->getFieldPosition(elseFieldDef->name());
  }

  if (thenFieldPos != -1 && elseFieldPos != -1) {
    // Both branches still have fields left to initialize
    if (thenFieldPos > elseFieldPos) {
      // The then branch is further along, so update the else branch
      stateElse->initializeFieldsThroughField(cond->elseStmt,
                                              thenFieldDef);

    } else if (elseFieldPos > thenFieldPos) {
      // The else branch is further along, so update the then branch
      stateThen->initializeFieldsThroughField(cond->thenStmt,
                                              elseFieldDef);

    } else {
      // Should never happen, as this function should only be called with
      // different field positions.
      INT_ASSERT(false);
    }
  } else if (thenFieldPos == -1) {
    // The then branch initialized the last field, update the else branch
    stateElse->initializeFieldsThroughField(cond->elseStmt,
                                            thenFieldDef);
  } else {
    // elseFieldPos == -1
    // The else branch initialized the last field, update the then branch
    stateThen->initializeFieldsThroughField(cond->thenStmt,
                                            elseFieldDef);
  }
}

/************************************* | **************************************
*                                                                             *
*  Determine if the current stmt is one of                                    *
*                                                                             *
*     this.init(args);                                                        *
*       => call(call(".", this,                     "init") .. args)          *
*                                                                             *
*     super.init(args)                                                        *
*       => call(call(".", unresolved("super"),      "init") .. args)  or      *
*       => call(call(".", call(".", this, "super"), "init") .. args)          *
*                                                                             *
************************************** | *************************************/

static const char* initName(CallExpr* stmt);
static const char* initNameInner(CallExpr* expr);
static bool        isUnresolvedSymbol(Expr* expr, const char* name);

static bool isInitStmt(CallExpr* stmt) {
  return isSuperInit(stmt) == true || isThisInit(stmt) == true;
}

bool isSuperInit(CallExpr* stmt) {
  const char* name = initName(stmt);

  return name != NULL && strcmp(name, "super") == 0 ? true : false;
}

bool isThisInit(CallExpr* stmt) {
  const char* name = initName(stmt);

  return name != NULL && strcmp(name, "this")  == 0 ? true : false;
}

static const char* initName(CallExpr* call) {
  const char* retval = NULL;

  if (CallExpr* inner = toCallExpr(call->baseExpr)) {
    retval = initNameInner(inner);
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

static const char* initNameInner(CallExpr* expr) {
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

static bool isUnresolvedSymbol(Expr* expr, const char* name) {
  bool retval = false;

  if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(expr)) {
    retval = strcmp(sym->unresolved, name) == 0;
  }

  return retval;
}

bool isInitDone(CallExpr* callExpr) {
  bool retval = false;

  if (callExpr->numActuals() == 0) {
    if (UnresolvedSymExpr* usym = toUnresolvedSymExpr(callExpr->baseExpr)) {
      if (strcmp(usym->unresolved, "complete") == 0) {
        retval = true;
      }

    } else if (CallExpr* subCall = toCallExpr(callExpr->baseExpr)) {
      if (subCall->numActuals()                        ==    2 &&
          subCall->isNamedAstr(astrSdot)               == true &&
          isStringLiteral(subCall->get(2), "complete") == true) {

        if (isSymbolThis(subCall->get(1)) == true) {
          retval = true;
        }
      }
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* Determine if the expr given is a banned error handling construct            *
*                                                                             *
************************************** | *************************************/

static bool isUnacceptableTry(Expr* stmt) {
  bool retval = false;

  if (TryStmt* ts = toTryStmt(stmt)) {
    if (ts->tryBang() == false) {
      // Only allow try! statements in initializers at the moment
      retval = true;

    } else if (ts->_catches.length != 0) {
      // But don't allow any catches for a try! statement, as that might
      // permit the initializer to return without having completely initialized
      // the instance.
      retval = true;
    }
  }

  return retval;
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

static DefExpr* toLocalField(AggregateType* at, CallExpr* expr);

static DefExpr* fieldByName(AggregateType* at, const char* name);

static DefExpr* toSuperFieldInit(AggregateType* at, CallExpr* callExpr) {
  forv_Vec(AggregateType, pt, at->dispatchParents) {
    if (DefExpr* field = toLocalFieldInit(pt, callExpr)) {
      return field;
    }
  }

  return NULL;
}

static DefExpr* toLocalFieldInit(AggregateType* at, CallExpr* callExpr) {
  DefExpr* retval = NULL;

  // The outer call has assignment syntax
  if (at != NULL && isAssignment(callExpr) == true) {
    if (CallExpr* lhs = toCallExpr(callExpr->get(1))) {
      retval = toLocalField(at, lhs);
    }
  }

  return retval;
}

static DefExpr* toLocalField(AggregateType* at, CallExpr* expr) {
  DefExpr* retval = NULL;

  if (expr->isNamedAstr(astrSdot)) {
    SymExpr* base = toSymExpr(expr->get(1));
    SymExpr* name = toSymExpr(expr->get(2));

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
  ArgSymbol*     type        = new ArgSymbol(INTENT_BLANK, "chpl_t", at);
  VarSymbol*     newInstance = newTemp("instance", at);
  CallExpr*      allocCall   = callChplHereAlloc(at);
  CallExpr*      initCall    = NULL;

  if (initMethod->hasFlag(FLAG_RESOLVED)) {
    initCall = new CallExpr(initMethod, gMethodToken, newInstance);
  } else {
    initCall = new CallExpr("init", gMethodToken, newInstance);
  }

  if (initMethod->where != NULL) {
    fn->where = initMethod->where->copy();
  }

  type->addFlag(FLAG_TYPE_VARIABLE);

  fn->setMethod(true);

  fn->addFlag(FLAG_COMPILER_GENERATED);
  fn->addFlag(FLAG_LAST_RESORT);

  if (initMethod->hasFlag(FLAG_SUPPRESS_LVALUE_ERRORS)) {
    fn->addFlag(FLAG_SUPPRESS_LVALUE_ERRORS);
  }

  fn->retType = at;

  // Add the formal that provides the type for the type method
  fn->insertFormalAtTail(type);

  //
  // Walk the formals to the init method
  // Ignore _mt and _this
  //   1) add a corresponding formal to the new type method
  //   2) add that formal to the call to "init"
  //
  int       count = 1;
  SymbolMap initArgToNewArgMap;

  for_formals(formal, initMethod) {
    // Ignore _mt and this
    if (count >= 3) {
      ArgSymbol* arg = formal->copy();

      initArgToNewArgMap.put(formal, arg);

      fn->insertFormalAtTail(arg);

      if (arg->variableExpr != NULL) {
        // The argument is a vararg.  If we pass it in plainly, it will be
        // treated as a single argument instead of the multiple arguments that
        // we want it to be treated as.  Expand the tuple before passing it in.
        initCall->insertAtTail(new CallExpr(PRIM_TUPLE_EXPAND,
                                            new SymExpr(arg)));
      } else {
        initCall->insertAtTail(new SymExpr(arg));
      }

      if (arg->hasFlag(FLAG_INSTANTIATED_PARAM)) {
        // Continue the building of the parameter map, since these two args are
        // linked.
        paramMap.put(arg, paramMap.get(formal));
      }
    }

    count = count + 1;
  }

  // Don't reference arguments to the initializer in the _new argument list
  // or where clause.
  update_symbols(fn, &initArgToNewArgMap);

  // Construct the body
  body->insertAtTail(new DefExpr(newInstance));

  body->insertAtTail(new CallExpr(PRIM_MOVE,   newInstance, allocCall));
  body->insertAtTail(new CallExpr(PRIM_SETCID, newInstance));

  body->insertAtTail(initCall);

  body->insertAtTail(new CallExpr(PRIM_RETURN, newInstance));

  // Insert the definition in to the tree
  at->symbol->defPoint->insertBefore(new DefExpr(fn));

  normalize(fn);

  return fn;
}

static void addSuperInit(FnSymbol* fn) {
  BlockStmt* body     = fn->body;

  VarSymbol* tmp      = newTemp("super_tmp");

  Symbol*    _this    = fn->_this;
  Symbol*    superSym = new_CStringSymbol("super");
  CallExpr*  superGet = new CallExpr(PRIM_GET_MEMBER_VALUE, _this, superSym);

  tmp->addFlag(FLAG_SUPER_TEMP);

  // Adding at head therefore add in reverse order
  body->insertAtHead(new CallExpr("init",    gMethodToken, tmp));
  body->insertAtHead(new CallExpr(PRIM_MOVE, tmp,          superGet));
  body->insertAtHead(new DefExpr(tmp));
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

bool hasInitDone(BlockStmt* block) {
  Expr* stmt   = block->body.head;
  bool  retval = false;

  while (stmt != NULL && retval == false) {
    if (CallExpr* callExpr = toCallExpr(stmt)) {
      retval = isInitDone(callExpr);

    } else if (CondStmt* cond = toCondStmt(stmt)) {
      if (cond->elseStmt == NULL) {
        retval = hasInitDone(cond->thenStmt);

      } else {
        retval = hasInitDone(cond->thenStmt) || hasInitDone(cond->elseStmt);
      }

    } else if (BlockStmt* block = toBlockStmt(stmt)) {
      retval = hasInitDone(block);

    } else if (ForallStmt* block = toForallStmt(stmt)) {
      retval = hasInitDone(block->loopBody());
    }

    stmt = stmt->next;
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* Transform `call(".", call(".", this, "super"), "init")` into                *
* `call(".", call(PRIM_GET_MEMBER_VALUE, this, "super"), "init")`             *
*                                                                             *
************************************** | *************************************/

static void transformSuperInit(CallExpr* initCall) {
  CallExpr* initBase = toCallExpr(initCall->baseExpr);

  if (CallExpr* sub = toCallExpr(initBase->get(1))) {
    if (sub->numActuals()          == 2 &&
        sub->isNamedAstr(astrSdot) == true) {
      Expr*      thisExpr  = sub->get(1);
      Expr*      superExpr = sub->get(2);

      CallExpr*  getValue  = new CallExpr(PRIM_GET_MEMBER_VALUE,
                                          thisExpr->remove(),
                                          superExpr->remove());

      VarSymbol* superTmp  = newTemp("super_tmp");

      INT_ASSERT(isSymbolThis(thisExpr)              == true);
      INT_ASSERT(isStringLiteral(superExpr, "super") == true);

      superTmp->addFlag(FLAG_SUPER_TEMP);

      initCall->insertBefore(new DefExpr(superTmp));
      initCall->insertBefore(new CallExpr(PRIM_MOVE, superTmp, getValue));

      sub->replace(new SymExpr(superTmp));
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

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

static bool isSymbolThis(Expr* expr) {
  bool retval = false;

  if (SymExpr* sym = toSymExpr(expr)) {
    retval = sym->symbol()->hasFlag(FLAG_ARG_THIS);
  }

  return retval;
}

//
// Builds the list of AggregateTypes in the hierarchy of `at` that have or
// require a postinit.
//
// Let's say we have a series of classes named from A through Z, where Z
// inherits from Y and so on. If class Q has a postinit, this function will
// add AggregateTypes to 'chain' from Q through Z with 'Q' at the head.
//
static bool buildPostInitChain(AggregateType* at,
                              std::vector<AggregateType*>& chain) {
  bool ret = false;

  // Works around odd case with _ddata
  AggregateType* parent = NULL;
  if (at->isClass() == true && at->dispatchParents.size() > 0) {
    parent = at->dispatchParents.v[0];
  }

  if (at == dtObject) {
    ret = false;
  } else if (at->isRecord()) {
    if (at->hasPostInitializer()) {
      chain.push_back(at);
      ret = true;
    }
  } else if (parent != NULL && buildPostInitChain(parent, chain) == true) {
    ret = true;
    chain.push_back(at);
  } else if (at->hasPostInitializer()) {
    ret = true;
    chain.push_back(at);
  }

  return ret;
}

static bool isSuperPostInit(CallExpr* stmt) {
  bool retval = false;

  if (CallExpr* call = toCallExpr(stmt->baseExpr)) {
    if (call->numActuals()                        ==    2 &&
        call->isNamedAstr(astrSdot)               == true &&
        isStringLiteral(call->get(2), "postinit") == true) {
      if (CallExpr* subExpr = toCallExpr(call->get(1))) {
        if (subExpr->numActuals()                     == 2    &&
            subExpr->isNamedAstr(astrSdot)            == true &&
            isSymbolThis(subExpr->get(1))             == true &&
            isStringLiteral(subExpr->get(2), "super") == true) {
          retval = true;
        }
      }
    }
  }

  return retval;
}

static bool hasSuperPostInit(BlockStmt* block) {
  Expr* stmt   = block->body.head;
  bool  retval = false;

  while (stmt != NULL && retval == false) {
    if (CallExpr* callExpr = toCallExpr(stmt)) {
      retval = isSuperPostInit(callExpr);

    } else if (CondStmt* cond = toCondStmt(stmt)) {
      if (cond->elseStmt == NULL) {
        retval = hasSuperPostInit(cond->thenStmt);

      } else {
        retval = hasSuperPostInit(cond->thenStmt) ||
                 hasSuperPostInit(cond->elseStmt);
      }

    } else if (BlockStmt* block = toBlockStmt(stmt)) {
      retval = hasSuperPostInit(block);

    } else if (ForallStmt* block = toForallStmt(stmt)) {
      retval = hasSuperPostInit(block->loopBody());
    }

    stmt = stmt->next;
  }

  return retval;
}

//
// Inserts a call to super.postinit if none exists anywhere in 'fn'
//
// TODO: what should we do with super.postinits in conditionals?
// TODO: merge with addSuperInit
//
static void insertSuperPostInit(FnSymbol* fn) {
  if (hasSuperPostInit(fn->body) == false) {
    SET_LINENO(fn);
    BlockStmt* body     = fn->body;

    VarSymbol* tmp      = newTemp("super_tmp");

    Symbol*    _this    = fn->_this;
    Symbol*    superSym = new_CStringSymbol("super");
    CallExpr*  superGet = new CallExpr(PRIM_GET_MEMBER_VALUE, _this, superSym);

    tmp->addFlag(FLAG_SUPER_TEMP);

    // Adding at head therefore add in reverse order
    body->insertAtHead(new CallExpr("postinit", gMethodToken, tmp));
    body->insertAtHead(new CallExpr(PRIM_MOVE,  tmp,          superGet));
    body->insertAtHead(new DefExpr(tmp));
  }
}

//
// Creates a method named "postinit" on 'at'. A call to "super.postinit" is
// added if 'at' is a class.
//
static void buildPostInit(AggregateType* at) {
  SET_LINENO(at);
  FnSymbol* fn     = new FnSymbol("postinit");
  ArgSymbol* _mt   = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);
  ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", at);

  fn->cname = fn->name;
  fn->_this = _this;

  _this->addFlag(FLAG_ARG_THIS);

  fn->insertFormalAtTail(_mt);
  fn->insertFormalAtTail(_this);

  DefExpr* def = new DefExpr(fn);

  at->symbol->defPoint->insertBefore(def);

  fn->setMethod(true);
  fn->addFlag(FLAG_METHOD_PRIMARY);

  if (at->isClass()) {
    insertSuperPostInit(fn);
  }

  at->methods.add(fn);
}

//
// Inserts a zero-arg where-less postinit for `at` unless one already exists.
// Also inserts a super.postinit for any postinit if omitted.
//
// Returns number of errors found.
//
static int insertPostInit(AggregateType* at, bool insertSuper) {
  int ret = 0;

  bool found = false;

  forv_Vec(FnSymbol, method, at->methods) {
    if (method->isPostInitializer()) {
      if (method->where == NULL) {
        found = true;
      }
      if (method->formals.length > 2) {
        // Only accept method token and 'this'
        ret += 1;
        USR_FATAL_CONT(method, "postinit must have zero arguments");
      }
      if (at->isClass() == true && insertSuper == true) {
        insertSuperPostInit(method);
      }
    }
  }

  if (found == false) {
    buildPostInit(at);
  }

  return ret;
}

static std::set<AggregateType*> postinitCache;

//
// Inserts postinit methods and calls to super.postinit as needed.
//
// If an ancestor of 'at' has a postinit, then every aggregate between 'at' and
// that ancestor must have a postinit as well.
//
// If postinits on a type have a where clause, insert a where-less postinit
// so the ancestors will always be called.
//
void preNormalizePostInit(AggregateType* at) {
  // Cache results to avoid extra work and to avoid looking for various forms
  // of super.postinit.
  if (postinitCache.find(at) != postinitCache.end()) {
    return;
  }

  // First element of 'chain' is the highest ancestor
  std::vector<AggregateType*> chain;
  buildPostInitChain(at, chain);

  int errors = 0;
  for_vector(AggregateType, cur, chain) {
    if (postinitCache.find(cur) == postinitCache.end()) {
      if (cur->hasInitializers() == false) {
        cur->symbol->addFlag(FLAG_USE_DEFAULT_INIT);
      }
      // Don't insert a super.init for the highest ancestor
      bool insertSuper = cur != chain.front();
      errors += insertPostInit(cur, insertSuper);
      postinitCache.insert(cur);
    }
  }

  if (errors > 0) {
    USR_STOP();
  }
}
