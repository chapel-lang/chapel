/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

/*** splitInit
 ***
 *** Contains functions for handling split init during normalize
 *** and during resolution.
 ***
 *** It needs to happen in both places because anything adjusting
 *** types needs to happen before resolution, but out intent handling
 *** must happen after.
 ***
 ***/

#include "splitInit.h"

#include "CatchStmt.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "TryStmt.h"

bool isSplitInitExpr(Expr* e) {
  if (SymExpr* se = toSymExpr(e))
    if (se->symbol() == gSplitInit)
      return true;

  return false;
}

typedef enum {
  FOUND_NOTHING = 0,
  FOUND_RET, // throw or return
  FOUND_USE,
  FOUND_INIT
} found_init_t;

static bool allowSplitInit(Symbol* sym);
static found_init_t doFindInitPoints(Symbol* sym,
                                     Expr* start,
                                     std::vector<CallExpr*>& initAssigns,
                                     Expr*& usePreventingSplitInit);

bool findInitPoints(DefExpr* def,
                    std::vector<CallExpr*>& initAssigns,
                    Expr*& usePreventingSplitInit) {
  // split initialization doesn't make sense to try for e.g.
  //  var x = 25;
  if (def->init != NULL && !isSplitInitExpr(def->init))
    return false;

  // no extern variables / vars used in inner functions
  if (allowSplitInit(def->sym) == false)
    return false;

  Expr* start = NULL;
  if (def->getStmtExpr() != NULL)
    start = def->getStmtExpr()->next;

  found_init_t found = doFindInitPoints(def->sym, start, initAssigns,
                                        usePreventingSplitInit);
  return (found == FOUND_INIT);
}

bool findInitPoints(CallExpr* defaultInit,
                    std::vector<CallExpr*>& initAssigns,
                    Expr*& usePreventingSplitInit) {

  INT_ASSERT(defaultInit->isPrimitive(PRIM_DEFAULT_INIT_VAR));

  SymExpr* se = toSymExpr(defaultInit->get(1));
  Symbol* sym = se->symbol();

  if (allowSplitInit(sym) == false)
    return false;

  // PRIM_DEFAULT_INIT_VAR should already be at statement level.
  Expr* start = defaultInit->next;

  found_init_t found = doFindInitPoints(sym, start, initAssigns,
                                        usePreventingSplitInit);
  return (found == FOUND_INIT);
}

static bool allowSplitInit(Symbol* sym) {
  // split-init doesn't make sense for extern variables
  if (sym->hasFlag(FLAG_EXTERN))
    return false;

  // Check for mentions of this variable in other functions (inner functions)
  FnSymbol* fn = sym->defPoint->getFunction();
  for_SymbolSymExprs(se, sym) {
    if (se->getFunction() != fn)
      return false; // use in inner function detected; disable split init
  }

  return true;
}

// Returns a SymExpr if there is an init or use at or after cur
static SymExpr* checkForUseAfterReturn(Symbol* sym, Expr* start) {
  for (Expr* cur = start; cur != NULL; cur = cur->next) {
    if (SymExpr* se = findSymExprFor(cur, sym)) {
      return se;
    }
  }

  return NULL;
}

static found_init_t doFindInitPoints(Symbol* sym,
                                     Expr* start,
                                     std::vector<CallExpr*>& initAssigns,
                                     Expr*& usePreventingSplitInit) {
  if (start == NULL)
    return FOUND_NOTHING;

  // Scroll forward in the block containing DefExpr for x.
  // Find 'x = ' before any use of 'x'.
  // Consider also { x = ... } and
  //               if _ { x = ... } else { x = ... }

  for (Expr* cur = start->getStmtExpr(); cur != NULL; cur = cur->next) {
    // x = ...
    if (CallExpr* call = toCallExpr(cur)) {
      // ignore PRIM_END_OF_STATEMENT
      if (!call->isPrimitive(PRIM_END_OF_STATEMENT)) {
        if (call->isNamedAstr(astrSassign)) {
          if (SymExpr* se = toSymExpr(call->get(1))) {
            if (se->symbol() == sym) {
              if (findSymExprFor(call->get(2), sym) == NULL) {
                // careful with e.g.
                //  x = x + 1;  or y = 1:y.type;
                initAssigns.push_back(call);
                return FOUND_INIT;
              }
            }
          }
        }

        if (SymExpr* se = findSymExprFor(cur, sym)) {
          // Emit an error if split initialization is required
          usePreventingSplitInit = se;
          return FOUND_USE;
        }

        if (call->isPrimitive(PRIM_RETURN) || call->isPrimitive(PRIM_THROW)) {
          if (SymExpr* se = checkForUseAfterReturn(sym, call->next)) {
            usePreventingSplitInit = se;
            return FOUND_USE;
          } else {
            return FOUND_RET;
          }
        }
      }

    // return
    } else if (GotoStmt* gt = toGotoStmt(cur)) {
      if (gt->gotoTag == GOTO_RETURN) {
        if (SymExpr* se = checkForUseAfterReturn(sym, gt->next)) {
          usePreventingSplitInit = se;
          return FOUND_USE;
        } else {
          return FOUND_RET;
        }
      }

    // { x = ... }
    } else if (BlockStmt* block = toBlockStmt(cur)) {
      if (block->isLoopStmt() || block->isRealBlockStmt() == false) {
        // Loop / on / begin / etc - just check for uses
        if (SymExpr* se = findSymExprFor(cur, sym)) {
          usePreventingSplitInit = se;
          return FOUND_USE;
        }
      } else {
        // non-loop block
        Expr* start = block->body.first();
        found_init_t found = doFindInitPoints(sym, start, initAssigns,
                                              usePreventingSplitInit);
        if (found == FOUND_INIT) {
          return FOUND_INIT;
        } else if (found == FOUND_USE) {
          return FOUND_USE;
        } else if (found == FOUND_RET) {
          return FOUND_RET;
        }
      }

    } else if (TryStmt* tr = toTryStmt(cur)) {
      Expr* start = tr->body()->body.first();
      found_init_t foundBody = doFindInitPoints(sym, start, initAssigns,
                                                usePreventingSplitInit);

      bool allCatchesRet = true;
      CatchStmt* nonReturningCatch = NULL;

      if (foundBody == FOUND_USE)
        return FOUND_USE;

      // if there are any catches, check them for uses;
      // also a catch block prevents initialization in the try body
      for_alist(elt, tr->_catches) {
        if (CatchStmt* ctch = toCatchStmt(elt)) {
          std::vector<CallExpr*> inits;
          Expr* use = NULL;
          Expr* start = ctch->body()->body.first();
          found_init_t foundCatch = doFindInitPoints(sym, start, inits, use);
          if (foundCatch == FOUND_USE || foundCatch == FOUND_INIT) {
            // Consider even an assignment in a catch block as a use
            usePreventingSplitInit = findSymExprFor(ctch, sym);
            return FOUND_USE;
          } else if (foundCatch != FOUND_RET) {
            allCatchesRet = false;
            nonReturningCatch = ctch;
          }
        }
      }

      // if we got here, no catch returned FOUND_USE.

      if (foundBody == FOUND_USE) {
        INT_FATAL("handled above");
      } else if (foundBody == FOUND_INIT) {
        if (allCatchesRet) {
          // all catches are either FOUND_RET or FOUND_INIT
          return FOUND_INIT;
        } else {
          usePreventingSplitInit = nonReturningCatch;
          return FOUND_USE;
        }
      }

    // if _ { x = ... } else { x = ... }
    } else if (CondStmt* cond = toCondStmt(cur)) {
      if (SymExpr* se = findSymExprFor(cond->condExpr, sym)) {
        usePreventingSplitInit = se;
        return FOUND_USE;
      }

      Expr* ifStart = cond->thenStmt->body.first();
      Expr* elseStart = cond->elseStmt ? cond->elseStmt->body.first() : NULL;
      std::vector<CallExpr*> ifAssigns;
      std::vector<CallExpr*> elseAssigns;
      Expr* ifUse = NULL;
      Expr* elseUse = NULL;
      found_init_t foundIf = doFindInitPoints(sym, ifStart, ifAssigns, ifUse);
      found_init_t foundElse = FOUND_NOTHING;
      if (elseStart != NULL)
        foundElse = doFindInitPoints(sym, elseStart, elseAssigns, elseUse);

      if ((foundIf == FOUND_INIT && foundElse == FOUND_INIT) ||
          (foundIf == FOUND_INIT && foundElse == FOUND_RET) ||
          (foundIf == FOUND_RET && foundElse == FOUND_INIT)) {
        for_vector(CallExpr, call, ifAssigns) {
          initAssigns.push_back(call);
        }
        for_vector(CallExpr, call, elseAssigns) {
          initAssigns.push_back(call);
        }
        return FOUND_INIT;
      } else if (foundIf == FOUND_INIT || foundElse == FOUND_INIT) {
        // initialized on one side but not the other
        usePreventingSplitInit = cur;
        return FOUND_USE;
      } else if (foundIf == FOUND_USE || foundElse == FOUND_USE) {
        // at least one of them must be FOUND_USE, so return that
        usePreventingSplitInit = cur;
        if (ifUse != NULL)
          usePreventingSplitInit = ifUse;
        if (elseUse != NULL)
          usePreventingSplitInit = elseUse;
        return FOUND_USE;
      } else if (foundIf == FOUND_RET && foundElse == FOUND_RET) {
        return FOUND_RET;
      }

    } else {
      // Look for uses of 'x' before the first assignment
      if (SymExpr* se = findSymExprFor(cur, sym)) {
        // In that case, can't convert '=' to initialization
        usePreventingSplitInit = se;
        return FOUND_USE;
      }
    }

    if (fVerify) {
      // Redundantly check for uses
      if (!isEndOfStatementMarker(cur))
        if (findSymExprFor(cur, sym) != NULL)
          INT_FATAL("use not found above");
    }
  }

  return FOUND_NOTHING;
}
