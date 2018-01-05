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
#include "stmt.h"
#include "TryStmt.h"
#include "type.h"
#include "typeSpecifier.h"

enum InitStyle {
  STYLE_NONE,
  STYLE_SUPER_INIT,
  STYLE_THIS_INIT,
  STYLE_BOTH
};

static bool     isInitStmt (Expr* stmt);
static bool     isSuperInit(Expr* stmt);
static bool     isThisInit (Expr* stmt);

static bool     isUnacceptableTry(Expr* stmt);

static void     preNormalizeInit(FnSymbol* fn);

static DefExpr* toSuperFieldInit(AggregateType* at, CallExpr* expr);
static DefExpr* toLocalFieldInit(AggregateType* at, CallExpr* expr);

static bool     isAssignment(CallExpr* callExpr);
static bool     isSimpleAssignment(CallExpr* callExpr);
static bool     isCompoundAssignment(CallExpr* callExpr);

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

      // Don't do anything about type or param fields.  Generic var/const
      // fields are okay, though.
      if (defExpr->sym->hasFlag(FLAG_PARAM) ||
          defExpr->sym->hasFlag(FLAG_TYPE_VARIABLE))
        continue;


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
  if (fn->hasFlag(FLAG_NO_PARENS) ==  true) {
    USR_FATAL(fn, "an initializer cannot be declared without parentheses");

  } else if (isReturnVoid(fn)     == false) {
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

static InitNormalize preNormalize(BlockStmt*    block,
                                  InitNormalize state);

static InitNormalize preNormalize(BlockStmt*    block,
                                  InitNormalize state,
                                  Expr*         start);

static CallExpr* createCallToSuperInit(FnSymbol* fn);

static bool      isSuperInit(Expr* stmt);
static bool      isThisInit(Expr* stmt);
static bool      hasReferenceToThis(Expr* expr);
static bool      isMethodCall(CallExpr* callExpr);

static void preNormalizeInit(FnSymbol* fn) {
  ArgSymbol*     _this = fn->getFormal(2);
  AggregateType* at    = toAggregateType(fn->_this->type);
  InitNormalize  state(fn);

  if (_this->intent == INTENT_BLANK) {
    if (isRecord(at) == true) {
      _this->intent = INTENT_REF;
    }
  }

  if (at->isGeneric() == true) {
    fn->_this->addFlag(FLAG_DELAY_GENERIC_EXPANSION);
  }

  if (fn->throwsError() == true) {
    // For now.  Remove when we allow it.
    USR_FATAL(fn, "initializers are not yet allowed to throw errors");
  }

  // The body contains at least one instance of this.init()
  // i.e. the body is not empty and we do not need to insert super.init()
  if (state.isPhase0() == true) {
    preNormalize(fn->body, state);

  // The body contains at least one instance of super.init()
  // i.e. the body is not empty and we do not need to insert super.init()
  } else if (state.isPhase1() == true) {
    preNormalize(fn->body, state);

  // 1) Insert field initializers before the first statement
  // 2) Pre-normalize the phase2 statements
  } else if (at->symbol->hasFlag(FLAG_EXTERN) == true) {
    Expr* head = fn->body->body.head;

    state.initializeFieldsBefore(head);

    preNormalize(fn->body, state, head);

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

    fn->body->insertAtHead(noop);

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

static InitNormalize preNormalize(BlockStmt*    block,
                                  InitNormalize state) {
  return preNormalize(block, state, block->body.head);
}

static InitNormalize preNormalize(BlockStmt*    block,
                                  InitNormalize state,
                                  Expr*         stmt) {
  // This sub-block may have a different phase than the parent
  state.checkPhase(block);

  while (stmt != NULL) {
    if (isUnacceptableTry(stmt) == true) {
      USR_FATAL(stmt,
                "Only catch-less try! statements are allowed in initializers"
                " for now");
    }

    if (isDefExpr(stmt) == true) {
      if (state.fieldUsedBeforeInitialized(stmt) == true) {
        USR_FATAL(stmt, "Field used before it is initialized");
      }

      stmt = stmt->next;

    } else if (CallExpr* callExpr = toCallExpr(stmt)) {
      if (callExpr->isPrimitive(PRIM_THROW)) {
        USR_FATAL(callExpr, "initializers are not yet allowed to throw errors");
      }

      // Stmt is super.init() or this.init()
      if (isInitStmt(callExpr) == true) {
        if (state.isPhase2() == true) {
          if (isSuperInit(callExpr) == true) {
            USR_FATAL(stmt, "use of super.init() call in phase 2");

          } else if (isThisInit(callExpr) == true) {
            USR_FATAL(stmt, "use of this.init() call in phase 2");

          } else {
            INT_ASSERT(false);
          }

        } else if (state.inLoopBody() == true) {
          if (isSuperInit(callExpr) == true) {
            USR_FATAL(stmt, "use of super.init() call in loop body");

          } else if (isThisInit(callExpr) == true) {
            USR_FATAL(stmt, "use of this.init() call in loop body");

          } else {
            INT_ASSERT(false);
          }

        } else if (state.inParallelStmt() == true) {
          if (isSuperInit(callExpr) == true) {
            USR_FATAL(stmt,
                      "use of super.init() call in a parallel statement");

          } else if (isThisInit(callExpr) == true) {
            USR_FATAL(stmt,
                      "use of this.init() call in a parallel statement");

          } else {
            INT_ASSERT(false);
          }

        } else if (state.inCoforall() == true) {
          if (isSuperInit(callExpr) == true) {
            USR_FATAL(stmt,
                      "use of super.init() call in a coforall loop body");

          } else if (isThisInit(callExpr) == true) {
            USR_FATAL(stmt,
                      "use of this.init() call in a coforall loop body");

          } else {
            INT_ASSERT(false);
          }

        } else if (state.inForall() == true) {
          if (isSuperInit(callExpr) == true) {
            USR_FATAL(stmt,
                      "use of super.init() call in a forall loop body");

          } else if (isThisInit(callExpr) == true) {
            USR_FATAL(stmt,
                      "use of this.init() call in a forall loop body");

          } else {
            INT_ASSERT(false);
          }

        } else if (state.inOn() == true) {
          if (isSuperInit(callExpr) == true) {
            USR_FATAL(stmt,
                      "use of super.init() call in an on block");

          } else if (isThisInit(callExpr) == true) {
            USR_FATAL(stmt,
                      "use of this.init() call in an on block");

          } else {
            INT_ASSERT(false);
          }

        } else {
          stmt = state.completePhase1(callExpr);
        }

      // Stmt is simple/compound assignment to a local field
      } else if (DefExpr* field = toLocalFieldInit(state.type(), callExpr)) {
        if (state.isPhase0() == true) {
          USR_FATAL(stmt,
                    "field initialization not allowed before this.init()");

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

        } else if (isCompoundAssignment(callExpr) == true) {
          USR_FATAL(stmt,
                    "cannot apply compound assignment to field \"%s\" "
                    "in phase 1",
                    field->sym->name);

        } else if (state.isFieldReinitialized(field) == true) {
          USR_FATAL(stmt,
                    "multiple initializations of field \"%s\"",
                    field->sym->name);

        } else if (state.inLoopBody() == true ||
                   state.inOnInLoopBody() == true) {
          USR_FATAL(stmt,
                    "can't initialize field \"%s\" inside a "
                    "loop during phase 1 of initialization",
                    field->sym->name);

        } else if (state.inParallelStmt() == true ||
                   state.inOnInParallelStmt() == true) {
          USR_FATAL(stmt,
                    "can't initialize field \"%s\" inside a "
                    "parallel statement during phase 1 of initialization",
                    field->sym->name);

        } else if (state.inCoforall() == true ||
                   state.inOnInCoforall() == true) {
          USR_FATAL(stmt,
                    "can't initialize field \"%s\" inside a "
                    "coforall during phase 1 of initialization",
                    field->sym->name);


        } else if (state.inForall() == true ||
                   state.inOnInForall() == true) {
          USR_FATAL(stmt,
                    "can't initialize field \"%s\" inside a "
                    "forall during phase 1 of initialization",
                    field->sym->name);


        } else {
          stmt = state.fieldInitFromInitStmt(field, callExpr);
        }

      // Stmt is assignment to a super field
      } else if (DefExpr* field = toSuperFieldInit(state.type(), callExpr)) {
        // Only valid during Phase 2
        if (state.isPhase2() == false) {
          USR_FATAL(stmt,
                    "can't set value of field \"%s\" from parent type "
                    "during phase 1 of initialization",
                    field->sym->name);
        } else {
          stmt = stmt->next;
        }
      // No action required
      } else {
        if (state.fieldUsedBeforeInitialized(stmt) == true) {
          USR_FATAL(stmt, "Field used before it is initialized");

        } else if (state.isPhase2()             == false &&
                   hasReferenceToThis(callExpr) == true) {
          USR_FATAL(stmt,
                    "can't pass \"this\" as an actual to a function "
                    "during phase 1 of initialization");


        } else if (state.isPhase2()       == false &&
                   isMethodCall(callExpr) == true) {
          USR_FATAL(stmt,
                    "cannot call a method "
                    "during phase 1 of initialization");

        } else {
          stmt = stmt->next;
        }
      }

    } else if (CondStmt* cond = toCondStmt(stmt)) {
      if (cond->elseStmt == NULL) {
        InitNormalize::InitPhase phaseThen = state.startPhase(cond->thenStmt);
        InitNormalize            stateThen = preNormalize(
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
            USR_FATAL(cond,
                      "cannot initialize fields in an if statement "
                      "in phase 1");
          }
        }

      } else {
        InitNormalize stateThen = preNormalize(cond->thenStmt,
                                               InitNormalize(cond, state));

        InitNormalize stateElse = preNormalize(cond->elseStmt,
                                               InitNormalize(cond, state));

        if (state.isPhase2() == false) {
          // Only one branch contained an init
          if (stateThen.isPhase2() != stateElse.isPhase2()) {
            USR_FATAL(cond,
                      "Both arms of a conditional must use this.init() "
                      "or super.init() in phase 1");

          } else if (stateThen.currField() != stateElse.currField()) {
            USR_FATAL(cond,
                      "Both arms of a conditional must initialize the same "
                      "fields in phase 1");

          } else {
            state.merge(stateThen);
          }
        }
      }

      stmt = stmt->next;

    } else if (LoopStmt* loop = toLoopStmt(stmt)) {
      preNormalize((BlockStmt*) stmt, InitNormalize(loop, state));
      stmt = stmt->next;

    } else if (ForallStmt* forall = toForallStmt(stmt)) {
      preNormalize(forall->loopBody(), InitNormalize(forall, state));
      stmt = stmt->next;

    } else if (BlockStmt* block = toBlockStmt(stmt)) {
      state.merge(preNormalize(block, InitNormalize(block, state)));
      stmt  = stmt->next;

    } else {
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

static bool hasReferenceToThis(Expr* expr) {
  bool retval = false;

  if (isUnresolvedSymExpr(expr) == true) {
    retval = false;

  } else if (SymExpr* symExpr = toSymExpr(expr)) {
    if (ArgSymbol* arg = toArgSymbol(symExpr->symbol())) {
      retval = arg->hasFlag(FLAG_ARG_THIS);
    }

  } else if (CallExpr* callExpr = toCallExpr(expr)) {
    for_actuals(actual, callExpr) {
      if (hasReferenceToThis(actual) == true) {
        retval = true;
        break;
      }
    }

  } else if (NamedExpr* named = toNamedExpr(expr)) {
    retval = hasReferenceToThis(named->actual);

  } else {
    INT_ASSERT(false);
  }

  return retval;
}

static bool isMethodCall(CallExpr* callExpr) {
  bool retval = false;

  if (CallExpr* base = toCallExpr(callExpr->baseExpr)) {
    if (base->isNamedAstr(astrSdot) == true) {
      if (SymExpr* lhs = toSymExpr(base->get(1))) {
        if (ArgSymbol* arg = toArgSymbol(lhs->symbol())) {
          retval = arg->hasFlag(FLAG_ARG_THIS);

          // Should only happen for the modifications I made earlier.
          UnresolvedSymExpr* calledSe = toUnresolvedSymExpr(base->get(2));
          if (calledSe) {
            if (strstr(calledSe->unresolved, "_if_fn")       != 0 ||
                strstr(calledSe->unresolved, "_parloopexpr") != 0) {
              // Only mark it as a method call if it is not a compiler inserted
              // loop or conditional expression function.
              retval = false;
            }
          }
        }
      }
    }
  }

  return retval;
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

static const char* initName(Expr*     stmt);
static const char* initName(CallExpr* expr);
static bool        isSymbolThis(Expr* expr);
static bool        isStringLiteral(Expr* expr, const char* name);
static bool        isUnresolvedSymbol(Expr* expr, const char* name);

static bool isInitStmt(Expr* stmt) {
  return isSuperInit(stmt) == true || isThisInit(stmt) == true;
}

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

/* Determine if the expr given is a banned error handling construct */
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
  forv_Vec(Type, t, at->dispatchParents) {
    if (AggregateType* pt = toAggregateType(t)) {
      if (DefExpr* field = toLocalFieldInit(pt, callExpr)) {
        return field;
      }
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
* Functions to detect uses of this.init(args) or super.init(args)             *
*                                                                             *
*   The 1st function iterates over statements in an FnSymbol.                 *
*   The 2nd function tests a single stmt.                                     *
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
