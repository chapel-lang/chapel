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

#include "AutoDestroyScope.h"

#include "expr.h"
#include "DeferStmt.h"
#include "resolution.h"
#include "stmt.h"
#include "symbol.h"

/************************************* | **************************************
*                                                                             *
* Track the state of lexical scopes during the execution of a function.       *
*                                                                             *
*   1) We only track variables that have an autoDestroy flag                  *
*                                                                             *
*   2) The compiler introduces "formal temps" to manage formals with out and  *
*      in-out concrete intents.  If a function has multiple returns then any  *
*      formal temps must be handled differently from other locals.            *
*                                                                             *
************************************** | *************************************/

static VarSymbol* variableToExclude(FnSymbol*  fn, Expr* refStmt);

static bool       isReturnStmt(const Expr* stmt);

static BlockStmt* findBlockForTarget(GotoStmt* stmt);

AutoDestroyScope::AutoDestroyScope(const AutoDestroyScope* parent,
                                   const BlockStmt*        block) {
  mParent        = parent;
  mBlock         = block;

  mLocalsHandled = false;
}

void AutoDestroyScope::variableAdd(VarSymbol* var) {
  if (var->hasFlag(FLAG_FORMAL_TEMP) == false) {
    mLocalsAndDefers.push_back(var);
  } else {
    mFormalTemps.push_back(var);
  }
}

void AutoDestroyScope::deferAdd(DeferStmt* defer) {
  mLocalsAndDefers.push_back(defer);
}

//
// Functions have an informal epilogue defined by code that
//
//   1) appears after a common "return label" (if present)
//   2) copies values to out/in-out formals
//
// We must      destroy the primaries before (1).
// We choose to destroy the primaries before (2).
//
// This code detects the start of (2)
//
bool AutoDestroyScope::handlingFormalTemps(const Expr* stmt) const {
  bool retval = false;

  if (mLocalsHandled == false) {
    if (const CallExpr* call = toConstCallExpr(stmt)) {
      if (FnSymbol* fn = call->resolvedFunction()) {
        if (fn->hasFlag(FLAG_ASSIGNOP) == true && call->numActuals() == 2) {
          SymExpr* lhs = toSymExpr(call->get(1));
          SymExpr* rhs = toSymExpr(call->get(2));

          if (lhs                                      != NULL &&
              rhs                                      != NULL &&
              isArgSymbol(lhs->symbol())               == true &&
              rhs->symbol()->hasFlag(FLAG_FORMAL_TEMP) == true) {
            retval = true;
          }
        }
      }
    }
  }

  return retval;
}

// If the refStmt is a goto then we need to recurse
// to the block that contains the target of the goto
void AutoDestroyScope::insertAutoDestroys(FnSymbol* fn, Expr* refStmt,
                                          const std::set<VarSymbol*>& ignored) {
  GotoStmt*               gotoStmt   = toGotoStmt(refStmt);
  bool                    recurse    = (gotoStmt != NULL) ? true : false;
  BlockStmt*              forTarget  = findBlockForTarget(gotoStmt);
  VarSymbol*              excludeVar = variableToExclude(fn, refStmt);
  const AutoDestroyScope* scope      = this;
  bool                    includeParent    = false;

  if (gotoStmt != NULL && gotoStmt->gotoTag == GOTO_ERROR_HANDLING)
    includeParent = true;

  // Error handling gotos need to include auto-destroys
  // for any in-scope variables for the block containing
  // the error-handling label.
  // Compare with while/break, say, in which the
  // variables in the parent block are assumed to be destroyed by the
  // parent block.

  // Problem: this loop terminates because
  // scope->mBlock == forTarget by the time we should be running it.
  while (scope != NULL) {
    // stop when block == forTarget for non-error-handling gotos
    if (scope->mBlock == forTarget && includeParent == false)
      break;

    scope->variablesDestroy(refStmt, excludeVar, ignored);

    // stop if recurse == false or if block == forTarget
    if (recurse == false)
      break;
    if (scope->mBlock == forTarget)
      break;

    scope = scope->mParent;
  }

  mLocalsHandled = true;
}

// If 'refStmt' is in a shadow variable's initBlock(),
// return that svar's deinitBlock(). Otherwise return NULL.
static BlockStmt* shadowVarsDeinitBlock(Expr* refStmt) {
  if (ShadowVarSymbol* svar = toShadowVarSymbol(refStmt->parentSymbol))
    if (refStmt->parentExpr == svar->initBlock())
      return svar->deinitBlock();

  return NULL;
}

void AutoDestroyScope::variablesDestroy(Expr*      refStmt,
                                        VarSymbol* excludeVar,
                                        const std::set<VarSymbol*>& ignored) const {
  // Handle the primary locals
  if (mLocalsHandled == false) {
    Expr*  insertBeforeStmt = refStmt;
    Expr*  noop             = NULL;
    size_t count            = mLocalsAndDefers.size();

    // If this is a simple nested block, insert after the final stmt
    // But always insert the destruction calls in reverse declaration order.
    // Do not get tricked by sequences of unreachable code
    if (refStmt->next == NULL) {
      if (mParent != NULL && !isGotoStmt(refStmt)) {
        SET_LINENO(refStmt);
        // Add a PRIM_NOOP to insert before
        noop = new CallExpr(PRIM_NOOP);
        if (BlockStmt* deinitBlock = shadowVarsDeinitBlock(refStmt)) {
          // 'deinitBlock' may already have deinit() of the shadow var.
          // The shadow var was probably the last thing that was initialized
          // in its initBlock(). So it should be the first to be DEinitialized
          // in the deinitBlock. Everything else, then, should go after that.
          deinitBlock->insertAtTail(noop);
        } else {
          refStmt->insertAfter(noop);
        }
        insertBeforeStmt = noop;
      }
    }

    for (size_t i = 1; i <= count; i++) {
      BaseAST*  localOrDefer = mLocalsAndDefers[count - i];
      VarSymbol* var = toVarSymbol(localOrDefer);
      DeferStmt* defer = toDeferStmt(localOrDefer);
      // This code only handles VarSymbols and DeferStmts.
      // It handles both in one vector because the order
      // of interleaving matters.
      INT_ASSERT(var || defer);

      if (var != NULL && var != excludeVar && ignored.count(var) == 0) {
        if (FnSymbol* autoDestroyFn = autoDestroyMap.get(var->type)) {
          SET_LINENO(var);

          INT_ASSERT(autoDestroyFn->hasFlag(FLAG_AUTO_DESTROY_FN));

          CallExpr* autoDestroy = new CallExpr(autoDestroyFn, var);

          insertBeforeStmt->insertBefore(autoDestroy);
        }
      }

      if (defer != NULL) {
        SET_LINENO(defer);
        BlockStmt* deferBlockCopy = defer->body()->copy();
        insertBeforeStmt->insertBefore(deferBlockCopy);
        deferBlockCopy->flattenAndRemove();
      }
    }

    // remove the PRIM_NOOP if we added one.
    if (noop != NULL)
      noop->remove();
  }

  // Handle the formal temps
  if (isReturnStmt(refStmt) == true) {
    size_t count = mFormalTemps.size();

    for (size_t i = 1; i <= count; i++) {
      VarSymbol* var = mFormalTemps[count - i];

      if (FnSymbol* autoDestroyFn = autoDestroyMap.get(var->type)) {
        SET_LINENO(var);

        refStmt->insertBefore(new CallExpr(autoDestroyFn, var));
      }
    }
  }
}

// Walk backwards from the current statement to determine if a sequence of
// moves have copied a variable that is marked for auto destruction in to
// the dedicated return-temp within the current scope.
//
// Note that the value we are concerned about may be copied in to one or
// more temporary variables between being copied to the return temp.
static VarSymbol* variableToExclude(FnSymbol* fn, Expr* refStmt) {
  VarSymbol* retVar = toVarSymbol(fn->getReturnSymbol());
  VarSymbol* retval = NULL;

  // TODO: migrate variableToExclude to addAutoDestroys
  // and the excluded set.

  if (retVar != NULL) {
    if (typeNeedsCopyInitDeinit(retVar->type) ||
        fn->hasFlag(FLAG_INIT_COPY_FN)) {
      VarSymbol* needle = retVar;
      Expr*      expr   = refStmt;

      // Walk backwards looking for the variable that is being returned
      while (retval == NULL && expr != NULL && needle != NULL) {
        if (CallExpr* move = toCallExpr(expr)) {
          if (move->isPrimitive(PRIM_MOVE) == true) {
            SymExpr*   lhs    = toSymExpr(move->get(1));
            VarSymbol* lhsVar = toVarSymbol(lhs->symbol());

            if (needle == lhsVar) {
              if (SymExpr* rhs = toSymExpr(move->get(2))) {
                VarSymbol* rhsVar = toVarSymbol(rhs->symbol());

                if (isAutoDestroyedVariable(rhsVar) == true) {
                  retval = rhsVar;
                } else {
                  needle = rhsVar;
                }
              } else {
                needle = NULL;
              }
            }
          }
        }

        expr = expr->prev;
      }
    }
  }

  return retval;
}

static bool isReturnStmt(const Expr* stmt) {
  bool retval = false;

  if (const CallExpr* expr = toConstCallExpr(stmt)) {
    retval = expr->isPrimitive(PRIM_RETURN);
  }

  return retval;
}

// Find the block stmt that encloses the target of this gotoStmt
static BlockStmt* findBlockForTarget(GotoStmt* stmt) {
  BlockStmt* retval = NULL;

  if (stmt != NULL && stmt->isGotoReturn() == false) {
    SymExpr* labelSymExpr = toSymExpr(stmt->label);
    Expr*    ptr          = labelSymExpr->symbol()->defPoint;

    while (ptr != NULL && isBlockStmt(ptr) == false) {
      ptr = ptr->parentExpr;
    }

    retval = toBlockStmt(ptr);

    INT_ASSERT(retval);
  }

  return retval;
}
