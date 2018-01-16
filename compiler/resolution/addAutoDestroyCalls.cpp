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

#include "addAutoDestroyCalls.h"



#include "astutil.h"
#include "AutoDestroyScope.h"
#include "DeferStmt.h"
#include "errorHandling.h"
#include "expr.h"
#include "resolution.h"
#include "stlUtil.h"
#include "stmt.h"
#include "symbol.h"

#include <vector>

static void cullForDefaultConstructor(FnSymbol* fn);

static void walkBlock(FnSymbol*         fn,
                      AutoDestroyScope* parent,
                      BlockStmt*        block,
                      std::set<VarSymbol*>& ignoredVariables);

void addAutoDestroyCalls() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_DEFAULT_CONSTRUCTOR) == true) {
      cullForDefaultConstructor(fn);
    }

    std::set<VarSymbol*> empty;
    walkBlock(fn, NULL, fn->body, empty);
  }

  // Finally, remove all defer statements, since they have been lowered.
  forv_Vec(DeferStmt, defer, gDeferStmts) {
    defer->remove();
  }
}

//
// Retain current approach for pruning auto-destroy flags in constructors.
// We do not destroy variables that are written in to the fields of the
// object being initialized.
//

static void cullForDefaultConstructor(FnSymbol* fn) {
  if (isVarSymbol(fn->getReturnSymbol()) == true) {
    std::vector<DefExpr*>        defs;

    collectDefExprs(fn, defs);

    for_vector(DefExpr, def, defs) {
      if (VarSymbol* var = toVarSymbol(def->sym)) {
        if (var->hasFlag(FLAG_INSERT_AUTO_DESTROY) == true) {
          // Look for a use in a PRIM_SET_MEMBER where the field is a record
          // type, and remove the flag. (We don't actually check that var is
          // of record type, because chpl__autoDestroy() is a NO-OP when
          // applied to all other types.
          for_SymbolUses(se, var) {
            CallExpr* call = toCallExpr(se->parentExpr);

            if (call->isPrimitive(PRIM_SET_MEMBER) == true &&
                toSymExpr(call->get(3))->symbol()  == var) {
              var->removeFlag(FLAG_INSERT_AUTO_DESTROY);
            }
          }
        }
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
* Walk the statements for a function's top-level BlockStmt                    *
*                                                                             *
*   1) Collect variables that are marked to be auto destroyed                 *
*                                                                             *
*   2) Handle recursion in to nested sub blocks                               *
*                                                                             *
*   3) Insert auto destroy calls at the end of the block                      *
*                                                                             *
*      Functions with multiple return points require additional care when     *
*      traversing a function's top-level block.                               *
*                                                                             *
*      Nested return statements are implemented as one or more PRIM_MOVEs to  *
*      the return variable followed by a GOTO to the return label. The        *
*      return value may be different at each return and so the autoDestroy    *
*      sets for the locals are handled immediately before each GOTO.          *
*                                                                             *
*      Functions that accept formals with OUT intent or INOUT intent rely on  *
*      formal temps within the function.  These formal temps are written to   *
*      the final destination using compiler generated code that appears       *
*      immediately before the return statement and will be shared by every    *
*      return path. We refer to this code as the "function epilogue".         *
*                                                                             *
*      The function epilogue is informally defined as                         *
*                                                                             *
*        a) The return label for the common exit point                        *
*        b) The first compiler-generated statement to write to a formal temp  *
*                                                                             *
*      but is not formally represented in the AST. Hence we require code to   *
*      detect the start of epilogue.                                          *
*                                                                             *
* A final complication is that the current implementation may find goto       *
* statements in the middle of a BlockStmt; this is not currently removed      *
* before this phase.  To be conservative, we arrange to ignore any such       *
* dead code.  The possibility of multiple returns makes this a little         *
* convoluted for the function's top level block statement.                    *
*                                                                             *
************************************** | *************************************/

static VarSymbol*   definesAnAutoDestroyedVariable(const Expr* stmt);
static LabelSymbol* findReturnLabel(FnSymbol* fn);
static bool         isReturnLabel(const Expr*        stmt,
                                  const LabelSymbol* returnLabel);
static bool         isErrorLabel(const Expr*        stmt);
static bool isYieldStmt(const Expr* stmt);
static void gatherIgnoredVariablesForErrorHandling(
    CondStmt* cond,
    std::set<VarSymbol*>& ignoredVariables);
static void gatherIgnoredVariablesForYield(
    Expr* stmt,
    std::set<VarSymbol*>& ignoredVariables);

static void walkBlock(FnSymbol*         fn,
                      AutoDestroyScope* parent,
                      BlockStmt*        block,
                      std::set<VarSymbol*>& ignoredVariables) {
  AutoDestroyScope scope(parent, block);

  LabelSymbol*     retLabel   = (parent == NULL) ? findReturnLabel(fn) : NULL;
  bool             isDeadCode = false;

  // Updating the variableToExclude is a good start, but an iterator
  // can have multiple yields in it. If each yield consumes the value,
  // shouldn't we be updating ignoredVariables for each yielded value?
  // (and going back to things moved to it?) Since once it is yielded,
  // it is no longer available for destruction?

  for_alist(stmt, block->body) {
    //
    // Handle the current statement
    //

    // TODO -- maybe we need to handle breakLabel and continueLabel here?
  
    // Once a variable is yielded, it should no longer be auto-destroyed,
    // since such destruction is the responsibility of
    // the calling loop.
    if (isYieldStmt(stmt)) {
      gatherIgnoredVariablesForYield(stmt, ignoredVariables);

    // AutoDestroy locals at the start of the error-handling label
    // (when exiting a try block without error)
    } else if (isErrorLabel(stmt) == true) {
      scope.insertAutoDestroys(fn, stmt, ignoredVariables);

    // AutoDestroy primary locals at start of function epilogue (1)
    } else if (isReturnLabel(stmt, retLabel) == true) {
      scope.insertAutoDestroys(fn, stmt, ignoredVariables);

    // Be conservative about unreachable code before the epilogue
    } else if (isDeadCode == false) {
      // Collect variables that should be autoDestroyed
      if (VarSymbol* var = definesAnAutoDestroyedVariable(stmt)) {
        scope.variableAdd(var);

      // Collect defer statements to run during cleanup
      } else if (DeferStmt* defer = toDeferStmt(stmt)) {
        scope.deferAdd(defer);

      // AutoDestroy primary locals at start of function epilogue (2)
      } else if (scope.handlingFormalTemps(stmt) == true) {
        scope.insertAutoDestroys(fn, stmt, ignoredVariables);

      // Recurse in to a BlockStmt (or sub-classes of BlockStmt e.g. a loop)
      } else if (BlockStmt* subBlock = toBlockStmt(stmt)) {
        walkBlock(fn, &scope, subBlock, ignoredVariables);

      // Recurse in to the BlockStmt(s) of a CondStmt
      } else if (CondStmt*  cond     = toCondStmt(stmt))  {

        std::set<VarSymbol*> toIgnore(ignoredVariables);

        if (isCheckErrorStmt(cond))
          gatherIgnoredVariablesForErrorHandling(cond, toIgnore);

        walkBlock(fn, &scope, cond->thenStmt, toIgnore);

        if (cond->elseStmt != NULL)
          walkBlock(fn, &scope, cond->elseStmt, toIgnore);

      }
    }

    //
    // Handle the end of a block
    // For the purposes of this pass, a block ends either
    // with a GotoStmt or when we run out of next statements.
    //
    GotoStmt* gotoStmt = toGotoStmt(stmt);
    if (gotoStmt != NULL || stmt->next == NULL) {

      // Don't visit any later code in this block
      // (don't add variable definitions, etc, above).
      isDeadCode = true;

      // The main block for a function or a simple sub-block
      if (parent == NULL || gotoStmt == NULL) {
        scope.insertAutoDestroys(fn, stmt, ignoredVariables);

      // Currently unprepared for a nested scope that ends in a goto
      } else {
        switch (gotoStmt->gotoTag) {
          case GOTO_RETURN:
          case GOTO_CONTINUE:
          case GOTO_BREAK:
          case GOTO_ERROR_HANDLING:
          case GOTO_BREAK_ERROR_HANDLING:
            scope.insertAutoDestroys(fn, stmt, ignoredVariables);
            break;

          case GOTO_NORMAL:
          case GOTO_GETITER_END:
          case GOTO_ITER_RESUME:
          case GOTO_ITER_END:
           // MDN 2016/03/18 Need to revisit these cases
           break;
        }
      }
    }
  }
}

// Is this a DefExpr that defines a variable that might be autoDestroyed?
static VarSymbol* definesAnAutoDestroyedVariable(const Expr* stmt) {
  VarSymbol* retval = NULL;

  if (const DefExpr* expr = toConstDefExpr(stmt)) {
    if (VarSymbol* var = toVarSymbol(expr->sym))
      retval = (isAutoDestroyedVariable(var) == true) ? var : NULL;
  }

  return retval;
}

//
// The return label is one of the logical markers for the start of
// the function epilogue but is not currently well marked.
//
// Identify it by inspecting any goto statements.  If there are
// multiple jumps to the return label then verify they are all
// the same.
//

static LabelSymbol* findReturnLabel(FnSymbol* fn) {
  std::vector<GotoStmt*> gotoStmts;
  BlockStmt*             block  = fn->body;
  LabelSymbol*           retval = NULL;

  collectGotoStmts(block, gotoStmts);

  for_vector(GotoStmt, gotoStmt, gotoStmts) {
    if (gotoStmt->isGotoReturn() == true) {
      LabelSymbol* target = gotoStmt->gotoTarget();

      if (retval == NULL)
        retval = target;
      else
        INT_ASSERT(retval == target);
    }
  }

  return retval;
}

// Is this the definition of the return label?
static bool isReturnLabel(const Expr* stmt, const LabelSymbol* returnLabel) {
  bool retval = false;

  if (returnLabel != NULL) {
    if (const DefExpr*  expr = toConstDefExpr(stmt)) {
      if (LabelSymbol* label = toLabelSymbol(expr->sym)) {
        retval = (label == returnLabel) ? true : false;
      }
    }
  }

  return retval;
}

// Is this the definition of an error handling label?
static bool isErrorLabel(const Expr* stmt) {
  if (const DefExpr*  expr = toConstDefExpr(stmt)) {
    if (LabelSymbol* label = toLabelSymbol(expr->sym)) {
      if (label->hasFlag(FLAG_ERROR_LABEL))
        return true;
    }
  }

  return false;
}

static bool isYieldStmt(const Expr* stmt) {
  if (const CallExpr* call = toConstCallExpr(stmt))
    if (call->isPrimitive(PRIM_YIELD))
      return true;

  return false;
}
/*
 This function identifies variables that have not yet been initialized
 because the function that would initialize them has thrown an error.
 The error handling can add 'gotos' to handle such errors in-between
 a DefExpr and the AST nodes that cause that variable to be initialized.
 This wouldn't have been possible before error handling.

  For example, consider the following Chapel code and AST:

  try {
    var x = returnOrThrow(1);
    writeln(x);
  }


  'def' error:Error;
  'def' x;
  'def' call_tmp;
  'move' call_tmp 'call' returnOrThrow(1, error)
  if('check error' error)
  {
    gotoErrorHandling handler;
  }
  'move' x 'call' chpl__initCopy(call_tmp)
  'call' writeln(x)

  In that event, 'call_tmp' might be stack trash (or at least
  a value that was not a result of initialization) if returnOrThrow
  threw an error. So 'deinit' shouldn't be called on 'call_tmp' or 'x'
  (since these variables really are the same and the existence of both
   is an artifact of the current implementation)
*/
static void gatherIgnoredVariablesForErrorHandling(
    CondStmt* cond,
    std::set<VarSymbol*>& ignoredVariables)
{

  // Look for the function call immediately preceding
  // that throws. Is it returning a variable that we will
  // want to auto-destroy?

  VarSymbol* ignore = NULL;
  if (CallExpr* move = toCallExpr(cond->prev)) {
    if (move->isPrimitive(PRIM_MOVE)) {
      if (CallExpr* call = toCallExpr(move->get(2))) {
        if (FnSymbol* fn = call->resolvedFunction()) {
          if (fn->throwsError()) {
            SymExpr *se = toSymExpr(move->get(1));
            ignore = toVarSymbol(se->symbol());
            ignoredVariables.insert(ignore);
          }
        }
      }
    }
  }

  // If ignore is set, it might be a callTmp,
  // track a subsequent PRIM_MOVE to expand the
  // set of ignored variables to include the
  // relevant user variable.
  if (ignore != NULL) {
    if (CallExpr* move = toCallExpr(cond->next)) {
      if (move->isPrimitive(PRIM_MOVE)) {
        SymExpr *dstSe = toSymExpr(move->get(1));
        SymExpr *srcSe = NULL;

        if (CallExpr* subCall = toCallExpr(move->get(2))) {
          if (FnSymbol* calledFn = subCall->resolvedFunction())
            if (calledFn->hasFlag(FLAG_INIT_COPY_FN) ||
                calledFn->hasFlag(FLAG_AUTO_COPY_FN))
              srcSe = toSymExpr(subCall->get(1));
        } else {
          srcSe = toSymExpr(move->get(2));
        }

        if (dstSe != NULL && srcSe != NULL &&
            srcSe->symbol() == ignore)
          ignoredVariables.insert(toVarSymbol(dstSe->symbol()));
      }
    }
  }
}

static void gatherIgnoredVariablesForYield(
    Expr* yieldStmt,
    std::set<VarSymbol*>& ignoredVariables)
{
  CallExpr* yield = toCallExpr(yieldStmt);
  SymExpr* yieldedSe = toSymExpr(yield->get(1));
  INT_ASSERT(yieldedSe);
  VarSymbol* yieldedVar = toVarSymbol(yieldedSe->symbol());
  INT_ASSERT(yieldedVar);
  QualifiedType t = yieldedVar->qualType();

  if (isUserDefinedRecord(t.type()) && ! t.isRef()) {

    SymExpr* foundSe = findSourceOfYield(yield);
    VarSymbol* var = toVarSymbol(foundSe->symbol());

    if (var &&
        var->hasEitherFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW,
                           FLAG_EXPR_TEMP))
      ignoredVariables.insert(var);
  }
}

/************************************* | **************************************
*                                                                             *
* A shared utility function for resolution.                                   *
*                                                                             *
************************************** | *************************************/

bool isAutoDestroyedVariable(Symbol* sym) {
  bool retval = false;

  if (VarSymbol* var = toVarSymbol(sym)) {
    if ((var->hasFlag(FLAG_INSERT_AUTO_DESTROY) == true &&
         var->hasFlag(FLAG_NO_AUTO_DESTROY)     == false) ||

        // This logic seems wrong somehow, but if I comment it out,
        // I get memory leaks in a missing deinit after the assign
        // in reader_chpl implementing stdinInit()

        (var->hasFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW) == true/*  &&
         var->type->symbol->hasFlag(FLAG_ITERATOR_RECORD)        == false &&
         // TODO - can we remove this isRefCountedType?
         // X
         isRefCountedType(var->type)                             == false*/)) {

      retval = (var->isType() == false && autoDestroyMap.get(var->type) != 0);
    }
  }

  return retval;
}

// For a yield of a variable, such as iter f() { var x=...; yield x; },
// unlike returning, the yield will result in a copy. This function helps
// to identify the source variable in such cases, even as there might be
// various compiler temporaries in between. It returns the SymExpr referring
// to the yielded variable that was used in the last PRIM_MOVE reading it.
SymExpr* findSourceOfYield(CallExpr* yield) {

  SymExpr* yieldedSe = toSymExpr(yield->get(1));
  Symbol* yieldedSym = yieldedSe->symbol();

  Expr* expr = yield;
  Symbol* needle = yieldedSym;
  SymExpr* foundSe = yieldedSe;

  // Walk backwards to figure out where the yielded symbol came
  // from and to find the spot at which we should add an
  // autoCopy call.
  while (expr != NULL && needle != NULL) {
    if (CallExpr* move = toCallExpr(expr)) {
      if (move->isPrimitive(PRIM_MOVE) == true) {
        SymExpr*   lhs    = toSymExpr(move->get(1));
        VarSymbol* lhsVar = toVarSymbol(lhs->symbol());

        if (needle == lhsVar) {
          if (SymExpr* rhs = toSymExpr(move->get(2))) {
            VarSymbol* rhsVar = toVarSymbol(rhs->symbol());

            if (rhsVar) {
              needle = rhsVar;

              if (rhsVar->hasFlag(FLAG_INSERT_AUTO_DESTROY) ||
                  rhsVar->hasFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW)) {
                foundSe = rhs;
                break;
              }
            }
          } else {
            needle = NULL;
          }
        }
      }
    }

    expr = expr->prev;
  }

  return foundSe;
}

