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
#include "ForallStmt.h"
#include "expr.h"
#include "errorHandling.h"
#include "stmt.h"
#include "symbol.h"
#include "TryStmt.h"

/************************************* | **************************************
*                                                                             *
*   split init                                                                *
*                                                                             *
************************************** | *************************************/


bool isSplitInitExpr(Expr* e) {
  if (SymExpr* se = toSymExpr(e))
    if (se->symbol() == gSplitInit)
      return true;

  return false;
}

typedef enum {
  FOUND_NOTHING = 0,
  FOUND_RET,
  FOUND_THROW,
  FOUND_USE,
  FOUND_INIT
} found_init_t;

static bool allowSplitInit(Symbol* sym);
static found_init_t doFindInitPoints(Symbol* sym,
                                     Expr* start,
                                     std::vector<CallExpr*>& initAssigns,
                                     Expr*& usePreventingSplitInit,
                                     bool allowReturns,
                                     BlockStmt*& ignoreFirstEndInBlock);

bool findInitPoints(DefExpr* def,
                    std::vector<CallExpr*>& initAssigns,
                    Expr*& usePreventingSplitInit,
                    bool allowReturns) {
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

  BlockStmt* ignoreFirstEndInBlock = NULL;
  if (start != NULL)
    ignoreFirstEndInBlock = toBlockStmt(start->parentExpr);

  found_init_t found = doFindInitPoints(def->sym, start, initAssigns,
                                        usePreventingSplitInit,
                                        allowReturns,
                                        ignoreFirstEndInBlock);
  return (found == FOUND_INIT);
}

bool findInitPoints(CallExpr* defaultInit,
                    std::vector<CallExpr*>& initAssigns,
                    Expr*& usePreventingSplitInit,
                    bool allowReturns) {

  INT_ASSERT(defaultInit->isPrimitive(PRIM_DEFAULT_INIT_VAR));

  SymExpr* se = toSymExpr(defaultInit->get(1));
  Symbol* sym = se->symbol();

  if (allowSplitInit(sym) == false)
    return false;

  // PRIM_DEFAULT_INIT_VAR should already be at statement level.
  Expr* start = defaultInit->next;

  BlockStmt* ignoreFirstEndInBlock = NULL;
  if (start != NULL)
    ignoreFirstEndInBlock = toBlockStmt(start->parentExpr);


  found_init_t found = doFindInitPoints(sym, start, initAssigns,
                                        usePreventingSplitInit,
                                        allowReturns,
                                        ignoreFirstEndInBlock);
  return (found == FOUND_INIT);
}

static bool allowSplitInit(Symbol* sym) {

  // don't split init if the flag disabled it
  if (fNoSplitInit)
    return false;

  // split-init doesn't make sense for extern variables
  if (sym->hasFlag(FLAG_EXTERN))
    return false;

  // For now, disable split init on non-user code
  // unless there is no alternative
  if (sym->defPoint->getModule()->modTag != MOD_USER &&
      !isSplitInitExpr(sym->defPoint->init))
    return false;

  return true;
}

// When 'start' is a return/throw (or a block containing that, e.g),
// decide whether the variable should be considered split-initialized
// by a later applicable assignment statement.
static found_init_t handleReturnForInit(found_init_t foundReturn,
                                        Symbol* sym,
                                        Expr* cur,
                                        std::vector<CallExpr*>& initAssigns,
                                        Expr*& usePreventingSplitInit,
                                        bool allowReturns,
                                        BlockStmt*& ignoreFirstEndInBlock) {
  INT_ASSERT(foundReturn == FOUND_RET || foundReturn == FOUND_THROW);

  // Look for initializations after the return.
  found_init_t afterReturn = doFindInitPoints(sym,
                                              cur->next,
                                              initAssigns,
                                              usePreventingSplitInit,
                                              allowReturns,
                                              ignoreFirstEndInBlock);
  if (afterReturn == FOUND_INIT) {
    // Initializations after a throw are still initializations, e.g.
    return FOUND_INIT;
  } else if (afterReturn == FOUND_USE) {
    // use after return
    return FOUND_USE;
  } else {
    // return FOUND_RET or FOUND_THROW
    return foundReturn;
  }
}

static found_init_t doFindInitPoints(Symbol* sym,
                                     Expr* start,
                                     std::vector<CallExpr*>& initAssigns,
                                     Expr*& usePreventingSplitInit,
                                     bool allowReturns,
                                     BlockStmt*& ignoreFirstEndInBlock) {
  if (start == NULL)
    return FOUND_NOTHING;

  // Scroll forward in the block containing DefExpr for x.
  // Find 'x = ' before any use of 'x'.
  // Consider also { x = ... } and
  //               if _ { x = ... } else { x = ... }

  for (Expr* cur = start->getStmtExpr(); cur != NULL; cur = cur->next) {
    // x = ...
    if (CallExpr* call = toCallExpr(cur)) {
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

      // ignore the 1st PRIM_END_OF_STATEMENT we encounter in the block
      if (call->isPrimitive(PRIM_END_OF_STATEMENT) &&
          ignoreFirstEndInBlock != NULL &&
          toBlockStmt(call->parentExpr) == ignoreFirstEndInBlock) {
        // Stop looking for the 1st PRIM_END_OF_STATEMENT
        ignoreFirstEndInBlock = NULL;
        continue;
      }

      if (SymExpr* se = findSymExprFor(cur, sym)) {
        // Emit an error if split initialization is required
        usePreventingSplitInit = se;
        return FOUND_USE;
      }
    }

    // return - goto, PRIM_RETURN, or PRIM_THROW.
    if (isGotoStmt(cur) || isCallExpr(cur)) {
      GotoStmt* gt = toGotoStmt(cur);
      CallExpr* call = toCallExpr(cur);

      bool regularReturn = false;
      bool errorReturn = false;
      if (gt != NULL) {
        regularReturn = gt->gotoTag == GOTO_RETURN;
        errorReturn = gt->gotoTag == GOTO_ERROR_HANDLING_RETURN ||
                      gt->gotoTag == GOTO_ERROR_HANDLING;
      } else if (call != NULL) {
        regularReturn = call->isPrimitive(PRIM_RETURN);
        errorReturn = call->isPrimitive(PRIM_THROW);
      }

      if (regularReturn || errorReturn) {

        if (regularReturn && allowReturns == false) {
          // For e.g. out intents, the out intent must be initialized
          // upon return. Vs. for local variables, a variable can be
          // uninitialized when a return occurs.
          //
          // Neither need to be initialized for a throw.
          usePreventingSplitInit = cur;
          return FOUND_USE;
        }

        found_init_t foundReturn = regularReturn ? FOUND_RET : FOUND_THROW;
        return handleReturnForInit(foundReturn, sym, cur, initAssigns,
                                   usePreventingSplitInit, allowReturns,
                                   ignoreFirstEndInBlock);
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
                                              usePreventingSplitInit,
                                              allowReturns,
                                              ignoreFirstEndInBlock);
        if (found == FOUND_INIT) {
          return FOUND_INIT;
        } else if (found == FOUND_USE) {
          return FOUND_USE;
        } else if (found == FOUND_RET || found == FOUND_THROW) {
          return handleReturnForInit(found, sym, cur, initAssigns,
                                     usePreventingSplitInit, allowReturns,
                                     ignoreFirstEndInBlock);
        }
      }

    // try { x = ... }
    } else if (TryStmt* tr = toTryStmt(cur)) {
      Expr* start = tr->body()->body.first();
      found_init_t foundBody = doFindInitPoints(sym, start, initAssigns,
                                                usePreventingSplitInit,
                                                allowReturns,
                                                ignoreFirstEndInBlock);

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
          found_init_t foundCatch = doFindInitPoints(sym, start, inits,
                                                     use, allowReturns,
                                                     ignoreFirstEndInBlock);
          if (foundCatch == FOUND_USE || foundCatch == FOUND_INIT) {
            // Consider even an assignment in a catch block as a use
            usePreventingSplitInit = findSymExprFor(ctch, sym);
            return FOUND_USE;
          } else if (foundCatch != FOUND_RET && foundCatch != FOUND_THROW) {
            allCatchesRet = false;
            nonReturningCatch = ctch;
          }
        }
      }

      // if we got here, no catch used or inited the variable
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
      } else if (foundBody == FOUND_RET || foundBody == FOUND_THROW) {
        return handleReturnForInit(foundBody, sym, cur, initAssigns,
                                   usePreventingSplitInit, allowReturns,
                                   ignoreFirstEndInBlock);
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
      found_init_t foundIf = doFindInitPoints(sym, ifStart, ifAssigns,
                                              ifUse, allowReturns,
                                              ignoreFirstEndInBlock);
      found_init_t foundElse = FOUND_NOTHING;
      if (elseStart != NULL)
        foundElse = doFindInitPoints(sym, elseStart, elseAssigns,
                                     elseUse, allowReturns,
                                     ignoreFirstEndInBlock);

      bool ifInits = foundIf == FOUND_INIT;
      bool elseInits = foundElse == FOUND_INIT;
      bool ifReturns = foundIf == FOUND_THROW ||
                       (foundIf == FOUND_RET && allowReturns);
      bool elseReturns = foundElse == FOUND_THROW ||
                         (foundElse == FOUND_RET && allowReturns);
      if ((ifInits   && elseInits) ||
          (ifInits   && elseReturns) ||
          (ifReturns && elseInits)) {
        for_vector(CallExpr, call, ifAssigns) {
          initAssigns.push_back(call);
        }
        for_vector(CallExpr, call, elseAssigns) {
          initAssigns.push_back(call);
        }
        return FOUND_INIT;
      } else if (foundIf == FOUND_USE || foundElse == FOUND_USE) {
        // at least one of them must be FOUND_USE, so return that
        usePreventingSplitInit = cur;
        if (ifUse != NULL)
          usePreventingSplitInit = ifUse;
        if (elseUse != NULL)
          usePreventingSplitInit = elseUse;
        return FOUND_USE;
      } else if (foundIf == FOUND_INIT || foundElse == FOUND_INIT) {
        // initialized on one side but not the other
        usePreventingSplitInit = cur;
        return FOUND_USE;
      } else if ((foundIf == FOUND_THROW || foundIf == FOUND_RET) &&
                 (foundElse == FOUND_THROW || foundElse == FOUND_RET)) {
        // return is more strict than throws, so if one returns
        // but the other throws, consider it a return.
        found_init_t found = FOUND_RET;
        if (foundIf == FOUND_THROW && foundElse == FOUND_THROW)
          found = FOUND_THROW;
        return handleReturnForInit(found, sym, cur, initAssigns,
                                   usePreventingSplitInit, allowReturns,
                                   ignoreFirstEndInBlock);
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

/************************************* | **************************************
*                                                                             *
*   copy elision                                                              *
*                                                                             *
************************************** | *************************************/

struct CopyElisionState {
  bool lastIsCopy;
  bool foundEndOfStmtMentioning;
  std::vector<CallExpr*> points;

  void reset() {
    points.clear();
    lastIsCopy = false;
    foundEndOfStmtMentioning = true;
  }
};

typedef std::map<VarSymbol*, CopyElisionState> VarToCopyElisionState;
typedef std::set<Symbol*> VariablesSet;

static bool doFindCopyElisionPoints(Expr* start,
                                    VarToCopyElisionState& map,
                                    VariablesSet& eligible);

// If call is a copy initialization call (e.g. chpl__autoCopy)
// return the lhs and rhs variables representing the copy.
static bool findCopyElisionCandidate(CallExpr* call,
                                     Symbol*& lhs, Symbol*& rhs) {
  // a call to init=
  if (call->isNamedAstr(astrInitEquals)) {
    if (call->numActuals() >= 2) {
      if (SymExpr* lhsSe = toSymExpr(call->get(1))) {
        if (SymExpr* rhsSe = toSymExpr(call->get(2))) {
          if (lhsSe->getValType() == rhsSe->getValType()) {
            lhs = lhsSe->symbol();
            rhs = rhsSe->symbol();
            return true;
          }
        }
      }
    }
  }

  // a PRIM_MOVE / PRIM_ASSIGN from chpl__initCopy / chpl__autoCopy
  if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
    if (SymExpr* lhsSe = toSymExpr(call->get(1))) {
      if (CallExpr* rhsCall = toCallExpr(call->get(2))) {
        if (rhsCall->isNamed("chpl__initCopy") ||
            rhsCall->isNamed("chpl__autoCopy")) {
          if (rhsCall->numActuals() >= 1) {
            if (SymExpr* rhsSe = toSymExpr(rhsCall->get(1))) {
              if (lhsSe->getValType() == rhsSe->getValType()) {
                lhs = lhsSe->symbol();
                rhs = rhsSe->symbol();
                return true;
              }
            }
          }
        }
      }
    }
  }

  // a chpl__initCopy / chpl__autoCopy returning through RVV
  if (call->isNamed("chpl__initCopy") || call->isNamed("chpl__autoCopy")) {
    if (FnSymbol* calledFn = call->resolvedFunction()) {
      if (calledFn->hasFlag(FLAG_FN_RETARG) && call->numActuals() >= 2) {
        if (SymExpr* rhsSe = toSymExpr(call->get(1))) {
          if (SymExpr* lhsSe = toSymExpr(call->get(2))) {
            if (lhsSe->getValType() == rhsSe->getValType()) {
              lhs = lhsSe->symbol();
              rhs = rhsSe->symbol();
              return true;
            }
          }
        }
      }
    }
  }

  return false;
}

static void doElideCopies(VarToCopyElisionState &map) {
  for (VarToCopyElisionState::iterator it = map.begin();
       it != map.end();
       ++it) {
    VarSymbol* var = it->first;
    CopyElisionState& state = it->second;

    if (state.lastIsCopy) {
      std::vector<CallExpr*>& points = state.points;
      for_vector (CallExpr, call, points) {
        Symbol* lhs = NULL;
        Symbol* rhs = NULL;
        bool ok = false;
        ok = findCopyElisionCandidate(call, lhs, rhs);
        INT_ASSERT(ok && rhs == var);

        SET_LINENO(call);
        // Change the copy into a move and don't destroy the variable.
        call->convertToNoop();
        call->insertBefore(new CallExpr(PRIM_ASSIGN_ELIDED_COPY, lhs, var));
        var->addFlag(FLAG_MAYBE_COPY_ELIDED);

        if (isCheckErrorStmt(call->next)) {
          INT_FATAL("code needs adjustment for throwing initializers");
        }

      }
    }
  }

  // these are now already handled.
  map.clear();
}

static void noteUse(VarSymbol* var, VarToCopyElisionState& map) {
  VarToCopyElisionState::iterator it = map.find(var);
  if (it != map.end()) {
    CopyElisionState& state = it->second;
    state.reset();
  }
}

static void noteUses(Expr* e, VarToCopyElisionState& map) {
  std::vector<SymExpr*> symExprs;
  collectSymExprs(e, symExprs);
  for_vector (SymExpr, se, symExprs) {
    if (VarSymbol* var = toVarSymbol(se->symbol())) {
      noteUse(var, map);
    }
  }
}

static bool canCopyElideVar(Symbol* rhs) {
  return isVarSymbol(rhs) &&
         !rhs->isRef() &&
         rhs->hasFlag(FLAG_NO_AUTO_DESTROY) == false &&
         (rhs->hasFlag(FLAG_INSERT_AUTO_DESTROY) ||
          rhs->hasFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW));
}

static bool canCopyElideCall(CallExpr* call, Symbol* lhs, Symbol* rhs) {
  return canCopyElideVar(rhs) &&
         rhs->getValType() == lhs->getValType() &&
         rhs->defPoint->parentSymbol == call->parentSymbol;
}

// returns true if there was an unconditional return
static bool doFindCopyElisionPoints(Expr* start,
                                    VarToCopyElisionState& map,
                                    VariablesSet& eligible) {

  if (start == NULL)
    return false;

  // Scroll forwards in the block containing start.
  for (Expr* cur = start->getStmtExpr(); cur != NULL; cur = cur->next) {

    // handle calls:
    //   copy-init e.g. var x = y
    //   PRIM_END_OF_STATEMENT
    if (CallExpr* call = toCallExpr(cur)) {
      Symbol* lhs = NULL;
      Symbol* rhs = NULL;
      bool foundCopy = findCopyElisionCandidate(call, lhs, rhs);

      // handle same-type copy-init from a variable
      if (foundCopy &&
          canCopyElideCall(call, lhs, rhs) &&
          eligible.count(rhs)) {
        // copy-init from a variable
        VarSymbol* var = toVarSymbol(rhs);
        CopyElisionState& state = map[var];
        state.lastIsCopy = true;
        state.foundEndOfStmtMentioning = false;
        state.points.clear();
        state.points.push_back(call);

      // handle PRIM_END_OF_STATEMENT
      } else if (call->isPrimitive(PRIM_END_OF_STATEMENT)) {
        std::vector<SymExpr*> symExprs;
        collectSymExprs(call, symExprs);

        // allow 1 end of statement following a copy elision.
        // two loops to handle duplicate mentions of same variable in the prim
        for_vector (SymExpr, se, symExprs) {
          if (VarSymbol* var = toVarSymbol(se->symbol())) {
            VarToCopyElisionState::iterator it = map.find(var);
            if (it != map.end()) {
              CopyElisionState& state = it->second;
              if (state.lastIsCopy) {
                if (state.foundEndOfStmtMentioning)
                  state.reset();
              }
            }
          }
        }
        for_vector (SymExpr, se, symExprs) {
          if (VarSymbol* var = toVarSymbol(se->symbol())) {
            VarToCopyElisionState::iterator it = map.find(var);
            if (it != map.end()) {
              CopyElisionState& state = it->second;
              state.foundEndOfStmtMentioning = true;
            }
          }
        }

      // handle other calls
      } else {
        noteUses(cur, map);
      }
    }

    // var x;
    if (DefExpr* def = toDefExpr(cur)) {

      // note variables that are defined as eligible
      if (canCopyElideVar(def->sym))
        eligible.insert(def->sym);

      noteUses(def, map);

    // return / throw
    } else if (isGotoStmt(cur) || isCallExpr(cur)) {
      GotoStmt* gt = toGotoStmt(cur);
      CallExpr* call = toCallExpr(cur);

      bool regularReturn = false;
      bool errorReturn = false;
      if (gt != NULL) {
        regularReturn = gt->gotoTag == GOTO_RETURN;
        errorReturn = gt->gotoTag == GOTO_ERROR_HANDLING_RETURN ||
                      gt->gotoTag == GOTO_ERROR_HANDLING;
      } else if (call != NULL) {
        regularReturn = call->isPrimitive(PRIM_RETURN);
        errorReturn = call->isPrimitive(PRIM_THROW);
      }

      if (regularReturn || errorReturn) {
        doElideCopies(map);
        return true; // don't look at dead code after this.
      }

    // { ... }  (nested block)
    } else if (BlockStmt* block = toBlockStmt(cur)) {

      if (block->isLoopStmt() || block->isRealBlockStmt() == false) {
        // Loop / on / begin / etc - just check for uses
        Expr* start = block->body.first();
        VariablesSet newEligible;
        doFindCopyElisionPoints(start, map, newEligible);
      } else {
        // non-loop block
        Expr* start = block->body.first();
        bool returned = doFindCopyElisionPoints(start, map, eligible);
        if (returned)
          return true; // stop traversing if it returned
      }

      // If we had a reason to, we could remove variables going
      // out of scope from `map` and from `eligible`.
      // If we did so, we would want to run doElideCopies on
      // those variables here.
      //
      // However this is not strictly necessary as variables cannot
      // be used outside of the scope in which they are declared.

    } else if (ForallStmt* forall = toForallStmt(cur)) {
      Expr* start = forall->loopBody()->body.first();
      VariablesSet newEligible;
      doFindCopyElisionPoints(start, map, newEligible);

    // try { ... }
    } else if (isTryStmt(cur)) {
      INT_FATAL("try statement not expected at this point in compilation");

    // if _ { ... } else { ... }
    } else if (CondStmt* cond = toCondStmt(cur)) {

      // note uses from the CondExpr and also thenStmt and elseStmt.
      //
      // any copy-inits in the conditional should rule out
      // a elision of a copy init before it.
      //
      // if there are copy-inits inside the conditional we will handle
      // that below.
      noteUses(cond, map);

      Expr* ifStart = cond->thenStmt->body.first();
      Expr* elseStart = cond->elseStmt ? cond->elseStmt->body.first() : NULL;

      // there is an else block

      // a variable copy-inited from in both the if and else is OK
      // but a variable copy-inited from on one side but not the other is not.

      VarToCopyElisionState ifMap;
      VarToCopyElisionState elseMap;

      bool ifRet = false;
      bool elseRet = false;
      {
        VariablesSet ifEligible = eligible;
        ifRet = doFindCopyElisionPoints(ifStart, ifMap, ifEligible);
      }

      if (elseStart != NULL) {
        VariablesSet elseEligible = eligible;
        elseRet = doFindCopyElisionPoints(elseStart, elseMap, elseEligible);
      }

      if (ifRet || elseRet) {
        if (ifRet == false) {
          // elseRet == true, so just note any copy inits from if
          for (VarToCopyElisionState::iterator it = ifMap.begin();
               it != ifMap.end();
               ++it) {
            VarSymbol* var = it->first;
            CopyElisionState& state = it->second;
            if (state.lastIsCopy)
              map[var] = state;
          }
        }
        if (elseRet == false) {
          // ifRet == true, so just note any copy inits from else
          for (VarToCopyElisionState::iterator it = elseMap.begin();
               it != elseMap.end();
               ++it) {
            VarSymbol* var = it->first;
            CopyElisionState& state = it->second;
            if (state.lastIsCopy)
              map[var] = state;
          }
        }

        // if both if and else return, the conditional returns.
        if (ifRet && elseRet)
          return true;

      } else {
        // neither if nor else returns

        // Look for variables copy-inited from in both the if and else
        // and adjust the main map accordingly.

        // Note that uses are already noted above.

        // The loop below relies on the maps being ordered.
        VarToCopyElisionState::key_compare comp = map.key_comp();
        VarToCopyElisionState::iterator ifIt = ifMap.begin();
        VarToCopyElisionState::iterator elseIt = elseMap.begin();

        while (ifIt != ifMap.end() && elseIt != elseMap.end()) {
          VarSymbol* ifVar = ifIt->first;
          VarSymbol* elseVar = elseIt->first;
          if (comp(ifVar, elseVar)) {
            // if element was less, so advance if iterator
            ++ifIt;
          } else if (comp(elseVar, ifVar)) {
            // else element was less, so else iterator
            ++elseIt;
          } else {
            // ifVar == elseVar
            VarSymbol* var = ifVar;
            INT_ASSERT(var == elseVar);
            CopyElisionState& ifState = ifIt->second;
            CopyElisionState& elseState = elseIt->second;
            if (ifState.lastIsCopy && elseState.lastIsCopy) {
              // Both are copy elision candidates. Note the locations.
              CopyElisionState& state = map[var];
              state.lastIsCopy = true;
              state.foundEndOfStmtMentioning = false;
              state.points.clear();
              for_vector (CallExpr, point, ifState.points) {
                state.points.push_back(point);
              }
              for_vector (CallExpr, point, elseState.points) {
                state.points.push_back(point);
              }
            }
            ++ifIt;
            ++elseIt;
          }
        }
        // no need to handle leftovers (e.g. ifIt not at end)
        // because we marked uses above.
      }
    } else {
      // Look for uses of 'x'
      noteUses(cur, map);
    }
  }

  return false;
}

void elideCopies(FnSymbol* fn) {
  if (fNoCopyElision == false) {
    VarToCopyElisionState map;
    VariablesSet eligible;

    doFindCopyElisionPoints(fn->body->body.first(), map, eligible);

    // run elide copies again in case the function did not terminate
    // with a return statement.
    doElideCopies(map);
  }
}
