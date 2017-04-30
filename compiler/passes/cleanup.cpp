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

//
// cleanup
//
// This pass cleans up the AST after parsing. It handles
// transformations that would be difficult to do while parsing.
//

#include "passes.h"

#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

static void cleanup(ModuleSymbol* module);

static void normalizeNestedFunctionExpressions(FnSymbol* fn);

static void normalizeLoopIterExpressions(FnSymbol* fn);

static void flattenScopelessBlock(BlockStmt* block);

static void destructureTupleAssignment(CallExpr* call);

static void flattenPrimaryMethod(TypeSymbol* ts, FnSymbol* fn);

static void applyAtomicTypeToPrimaryMethod(TypeSymbol* ts, FnSymbol* fn);

static void changeCastInWhere(FnSymbol* fn);

static void addParensToDeinitFns(FnSymbol* fn);

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

        } else if (strncmp("_iterator_for_loopexpr", fn->name, 22) == 0) {
          normalizeLoopIterExpressions(fn);
        }
      }
    }
  }

  for_vector(BaseAST, ast, asts) {
    SET_LINENO(ast);

    if (BlockStmt* block = toBlockStmt(ast)) {
      if (block->blockTag == BLOCK_SCOPELESS && block->list != NULL) {
        flattenScopelessBlock(block);
      }

    } else if (CallExpr* call = toCallExpr(ast)) {
      if (call->isNamed("_build_tuple") == true) {
        destructureTupleAssignment(call);
      }

    } else if (DefExpr* def = toDefExpr(ast)) {
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        // Is this function defined within a type i.e. is it a method?
        if (TypeSymbol* ts = toTypeSymbol(fn->defPoint->parentSymbol)) {
          flattenPrimaryMethod(ts, fn);
          applyAtomicTypeToPrimaryMethod(ts, fn);
        }

        changeCastInWhere(fn);
        addParensToDeinitFns(fn);
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

  } else {
    Expr* stmt = def->getStmtExpr();

    def->replace(new UnresolvedSymExpr(fn->name));

    stmt->insertBefore(def);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void normalizeLoopIterExpressions(FnSymbol* fn) {
  DefExpr*  def    = fn->defPoint;
  FnSymbol* parent = toFnSymbol(def->parentSymbol);
  Symbol*   parSym = parent->defPoint->parentSymbol;

  INT_ASSERT(strncmp("_parloopexpr", parent->name, 12) == 0 ||
             strncmp("_seqloopexpr", parent->name, 12) == 0);

  // Walk up through nested loop expressions
  while (strncmp("_parloopexpr", parSym->name, 12) == 0  ||
         strncmp("_seqloopexpr", parSym->name, 12) == 0) {
    parent = toFnSymbol(parSym);
    parSym = parent->defPoint->parentSymbol;
  }

  def->remove();

  // Move the parent
  if (TypeSymbol* ts = toTypeSymbol(parSym)) {
    AggregateType* ct = toAggregateType(ts->type);

    INT_ASSERT(ct);

    ct->addDeclarations(def);

  } else {
    parent->defPoint->insertBefore(def);
  }
}

/************************************* | **************************************
*                                                                             *
* Move the statements in a block out of the block                             *
*                                                                             *
************************************** | *************************************/

static void flattenScopelessBlock(BlockStmt* block) {
  for_alist(stmt, block->body) {
    stmt->remove();

    block->insertBefore(stmt);
  }

  block->remove();
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
      parent->isNamed("=") == true &&
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

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void changeCastInWhere(FnSymbol* fn) {
  if (fn->where != NULL) {
    std::vector<BaseAST*> asts;

    collect_asts(fn->where, asts);

    for_vector(BaseAST, ast, asts) {
      if (CallExpr* call = toCallExpr(ast)) {
        if (call->isCast() == true) {
          Expr* to   = call->castTo();
          Expr* from = call->castFrom();

          to->remove();
          from->remove();

          call->replace(new CallExpr(PRIM_IS_SUBTYPE, to, from));
        }
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Make paren-less decls act as paren-ful.                                     *
* Otherwise "arg.deinit()" in proc chpl__delete(arg) would not resolve.       *
*                                                                             *
************************************** | *************************************/

static void addParensToDeinitFns(FnSymbol* fn) {
  if (fn->hasFlag(FLAG_DESTRUCTOR)) {
    fn->removeFlag(FLAG_NO_PARENS);
  }
}
