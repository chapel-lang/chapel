/*
 * Copyright 2004-2020 Cray Inc.
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

//
// cleanup
//
// This pass cleans up the AST after parsing. It handles
// transformations that would be difficult to do while parsing.
//

#include "passes.h"

#include "astutil.h"
#include "build.h"
#include "CatchStmt.h"
#include "expr.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "wellknown.h"

static void cleanup(ModuleSymbol* module);

static void normalizeNestedFunctionExpressions(FnSymbol* fn);

static void destructureTupleAssignment(CallExpr* call);

static void replaceIsSubtypeWithPrimitive(CallExpr* call,
                                          bool proper, bool coerce);

static void flattenPrimaryMethod(TypeSymbol* ts, FnSymbol* fn);

static void applyAtomicTypeToPrimaryMethod(TypeSymbol* ts, FnSymbol* fn);

static void fixupVoidReturnFn(FnSymbol* fn);

void cleanup() {
  std::vector<ModuleSymbol*> mods;

  ModuleSymbol::getTopLevelModules(mods);

  for_vector(ModuleSymbol, mod, mods) {
    cleanup(mod);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void cleanup(ModuleSymbol* module) {
  std::vector<BaseAST*> asts;

  collect_asts(module, asts);

  for_vector(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast)) {
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        SET_LINENO(def);

        if (fn->hasFlag(FLAG_COMPILER_NESTED_FUNCTION) == true) {
          normalizeNestedFunctionExpressions(fn);

        }
      }
    }
  }

  for_vector(BaseAST, ast, asts) {
    SET_LINENO(ast);

    if (BlockStmt* block = toBlockStmt(ast)) {
      if (block->blockTag == BLOCK_SCOPELESS && block->list != NULL) {
        block->flattenAndRemove();
      }

    } else if (CallExpr* call = toCallExpr(ast)) {
      if (call->isNamed("_build_tuple"))
        destructureTupleAssignment(call);
      else if (call->isNamed("isSubtype"))
        replaceIsSubtypeWithPrimitive(call, false, false);
      else if (call->isNamed("isProperSubtype"))
        replaceIsSubtypeWithPrimitive(call, true, false);
      else if (call->isNamed("isCoercible"))
        replaceIsSubtypeWithPrimitive(call, false, true);

    } else if (DefExpr* def = toDefExpr(ast)) {
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        // Is this function defined within a type i.e. is it a method?
        if (TypeSymbol* ts = toTypeSymbol(fn->defPoint->parentSymbol)) {
          flattenPrimaryMethod(ts, fn);
          applyAtomicTypeToPrimaryMethod(ts, fn);
        }

        fixupVoidReturnFn(fn);
      }
    } else if (CatchStmt* catchStmt = toCatchStmt(ast)) {
      catchStmt->cleanup();
    }
  }

  if (module == stringLiteralModule && !fMinimalModules) {
    // Fix calls to chpl_createStringWithLiteral to use resolved expression.
    // For compiler performance reasons, we'd like to have new_StringSymbol
    // emit calls to a resolved function; however new_StringSymbol might
    // run before that function is parsed. So fix up any literals created
    // during parsing here.
    INT_ASSERT(gChplCreateStringWithLiteral != NULL);
    const char* name = gChplCreateStringWithLiteral->name;

    for_vector(BaseAST, ast, asts) {
      if (CallExpr* call = toCallExpr(ast)) {
        if (UnresolvedSymExpr* urse = toUnresolvedSymExpr(call->baseExpr)) {
          if (urse->unresolved == name) {
            SET_LINENO(urse);
            urse->replace(new SymExpr(gChplCreateStringWithLiteral));
          }
        }
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Moves expressions that are parsed as nested function definitions into their *
* own statement; during parsing, these are embedded in call expressions       *
*                                                                             *
************************************** | *************************************/

static void normalizeNestedFunctionExpressions(FnSymbol* fn) {
  DefExpr* def = fn->defPoint;

  if (TypeSymbol* ts = toTypeSymbol(def->parentSymbol)) {
    AggregateType* ct = toAggregateType(ts->type);

    INT_ASSERT(ct);

    def->replace(new UnresolvedSymExpr(fn->name));

    ct->addDeclarations(def);

  } else if (isArgSymbol(def->parentSymbol)) {
    Expr* stmt = def->getStmtExpr();

    def->replace(new UnresolvedSymExpr(fn->name));

    stmt->insertBefore(def);

  } else {
    Expr* stmt = def->getStmtExpr();

    def->replace(new UnresolvedSymExpr(fn->name));

    stmt->insertBefore(def);
  }
}


/************************************* | **************************************
*                                                                             *
* destructureTupleAssignment                                                  *
*                                                                             *
*    (i,j) = expr;    ==>    i = expr(1);                                     *
*                            j = expr(2);                                     *
*                                                                             *
* note: handles recursive tuple destructuring, (i,(j,k)) = ...                *
*                                                                             *
************************************** | *************************************/

static void      insertDestructureStatements(Expr*     S1,
                                             Expr*     S2,
                                             CallExpr* lhs,
                                             Expr*     rhs);

static CallExpr* destructureChk(CallExpr* lhs, Expr* rhs);

static CallExpr* destructureErr();

static void destructureTupleAssignment(CallExpr* call) {
  CallExpr* parent = toCallExpr(call->parentExpr);

  if (parent               != NULL &&
      parent->isNamedAstr(astrSassign) &&
      parent->get(1)       == call) {
    VarSymbol* rtmp = newTemp();
    Expr*      S1   = new CallExpr(PRIM_MOVE, rtmp, parent->get(2)->remove());
    Expr*      S2   = new CallExpr(PRIM_NOOP);

    rtmp->addFlag(FLAG_EXPR_TEMP);
    rtmp->addFlag(FLAG_MAYBE_TYPE);
    rtmp->addFlag(FLAG_MAYBE_PARAM);

    call->getStmtExpr()->replace(S1);

    S1->insertBefore(new DefExpr(rtmp));
    S1->insertAfter(S2);

    insertDestructureStatements(S1, S2, call, new SymExpr(rtmp));

    S2->remove();
  }
}


static void replaceIsSubtypeWithPrimitive(CallExpr* call,
                                          bool proper, bool coerce) {
  Expr* sub = call->get(1);
  Expr* sup = call->get(2);
  sub->remove();
  sup->remove();

  PrimitiveTag prim = proper ? PRIM_IS_PROPER_SUBTYPE : PRIM_IS_SUBTYPE;
  if (coerce)
    prim = PRIM_IS_COERCIBLE;

  call->replace(new CallExpr(prim, sup, sub));
}

//
// If call is an empty return statement, e.g. "return;"
// then change it into a return of the 'void' symbol: "return _void;"
// and mark the function it is in with FLAG_VOID_NO_RETURN_VALUE.
//
static void insertVoidReturnSymbols(CallExpr* call) {
  INT_ASSERT(call->isPrimitive(PRIM_RETURN));

  if (call->numActuals() == 0) {
    FnSymbol* fn = call->getFunction();
    INT_ASSERT(fn);
    call->insertAtTail(gVoid);
    fn->addFlag(FLAG_VOID_NO_RETURN_VALUE);
  }
}

//
// Mark functions with no return statements and functions with only empty
// return statements with FLAG_VOID_NO_RETURN_VALUE. Change empty return
// statements to return the value '_void'.
//
static void fixupVoidReturnFn(FnSymbol* fn) {
  std::vector<CallExpr*> callExprs;
  collectCallExprs(fn, callExprs);
  bool foundReturn = false;
  // Pass expandExternArrayCalls builds a wrapper for the extern function
  // and returns the value the extern function returned.  It marks the
  // extern function with FLAG_EXTERN_FN_WITH_ARRAY_ARG, which tells us
  // that we need to be able to handle the wrapper returning the result
  // of a call to it.  If the extern function had a 'void' return, treat
  // it as a void value.
  if (fn->hasFlag(FLAG_EXTERN_FN_WITH_ARRAY_ARG)) {
    return;
  }

  for_vector(CallExpr, call, callExprs) {
    if (call->isPrimitive(PRIM_RETURN)) {
      foundReturn = true;
      insertVoidReturnSymbols(call);
    }
  }
  if (!foundReturn) {
    fn->addFlag(FLAG_VOID_NO_RETURN_VALUE);
  }
}


static void insertDestructureStatements(Expr*     S1,
                                        Expr*     S2,
                                        CallExpr* lhs,
                                        Expr*     rhs) {
  int       index = 0;
  CallExpr* test  = destructureChk(lhs, rhs);
  CallExpr* err   = destructureErr();

  S1->getStmtExpr()->insertAfter(buildIfStmt(test, err));

  for_actuals(expr, lhs) {
    UnresolvedSymExpr* se = toUnresolvedSymExpr(expr->remove());

    index = index + 1;

    if (se == NULL || strcmp(se->unresolved, "chpl__tuple_blank") != 0) {
      CallExpr* nextLHS = toCallExpr(expr);
      Expr*     nextRHS = new CallExpr(rhs->copy(), new_IntSymbol(index));

      if (nextLHS != NULL && nextLHS->isNamed("_build_tuple") == true) {
        insertDestructureStatements(S1, S2, nextLHS, nextRHS);

      } else {
        VarSymbol* lhsTmp = newTemp();
        CallExpr*  addrOf = new CallExpr(PRIM_ADDR_OF, expr);

        lhsTmp->addFlag(FLAG_MAYBE_PARAM);

        S1->insertBefore(new DefExpr(lhsTmp));
        S1->insertBefore(new CallExpr(PRIM_MOVE, lhsTmp, addrOf));

        S2->insertBefore(new CallExpr("=", lhsTmp, nextRHS));
      }
    }
  }
}

static CallExpr* destructureChk(CallExpr* lhs, Expr* rhs) {
  CallExpr* dot  = new CallExpr(".", rhs->copy(), new_CStringSymbol("size"));

  return new CallExpr("!=", new_IntSymbol(lhs->numActuals()), dot);
}

static CallExpr* destructureErr() {
  const char* msg  = NULL;
  Symbol*     zero = new_IntSymbol(0);

  msg = "tuple size must match the number of grouped variables";

  return new CallExpr("compilerError", new_StringSymbol(msg), zero);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void flattenPrimaryMethod(TypeSymbol* ts, FnSymbol* fn) {
  Expr*    insertPoint = ts->defPoint;
  DefExpr* def         = fn->defPoint;

  while (isTypeSymbol(insertPoint->parentSymbol)) {
    insertPoint = insertPoint->parentSymbol->defPoint;
  }

  insertPoint->insertBefore(def->remove());

  if (fn->userString != NULL && fn->name != ts->name) {
    if (strncmp(fn->userString, "ref ", 4) == 0) {
      // fn->userString of "ref foo()"
      // Move "ref " before the type name so we end up with "ref Type.foo()"
      // instead of "Type.ref foo()"
      fn->userString = astr("ref ", ts->name, ".", fn->userString + 4);

    } else {
      fn->userString = astr(ts->name, ".", fn->userString);
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void applyAtomicTypeToPrimaryMethod(TypeSymbol* ts, FnSymbol* fn) {
  if (ts->hasFlag(FLAG_ATOMIC_TYPE)) {
    fn->addFlag(FLAG_ATOMIC_TYPE);
  }
}
