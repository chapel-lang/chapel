/*
 * Copyright 2004-2019 Cray Inc.
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

#include "lifetime.h"

#include "AstVisitorTraverse.h"
#include "bb.h"
#include "bitVec.h"
#include "DecoratedClassType.h"
#include "driver.h"
#include "expr.h"
#include "ForLoop.h"
#include "ForallStmt.h"
#include "loopDetails.h"
#include "stlUtil.h"
#include "symbol.h"
#include "view.h"
#include "wellknown.h"

// These enable debug facilities
static const char* debugNilsForFn = "";
static const int debugNilsForId = 0;

/* Flow Sensitive Must Alias Analysis
   Following Muchnick p 296 "flow-sensitive must information"
     "In this case, aliasing is best characterized as a function
      from program points and variables to abstract storage locations
      (not sets of locations)". Also note that it is
      a transitive and symmetric relation.

   Note that "must alias" analysis is significantly easier to implement
   than "may alias" analysis.
 */

typedef enum {
  // Note - the order of these matters for the
  // part of the algorithm ensuring convergence by
  // only accepting changes that increase.

  MUST_ALIAS_IGNORED = 0, // no information; useful starting point.
                          // ignored when combining basic blocks

  MUST_ALIAS_REFVAR,      // reference refers to a particular variable

  MUST_ALIAS_ALLOCATED,   // refers to the statement allocating (aka 'new')

  MUST_ALIAS_NIL,         // it refers to nil

  MUST_ALIAS_UNKNOWN,     // might have been set, might point to nil
                          // who knows what it points to
} AliasType;

struct AliasLocation {
  AliasType type;
  // MUST_ALIAS_ALLOCATED -> CallExpr for the allocation
  //                         or condExpr of CondStmt establishing non-nil-ness
  // MUST_ALIAS_REFVAR -> ArgSymbol or VarSymbol being referred to
  // MUST_ALIAS_NIL -> a reason it is nil ex. assignment from nil
  //                   or condExpr of CondStmt establishing nil-ness
  BaseAST* location;
  AliasLocation() : type(MUST_ALIAS_IGNORED), location(NULL) { }
};

typedef std::map<Symbol*, AliasLocation> AliasMap;

static inline AliasLocation nilAliasLocation(BaseAST* reason) {
  AliasLocation ret;
  ret.type = MUST_ALIAS_NIL;
  ret.location = reason;
  return ret;
}

static inline AliasLocation unknownAliasLocation() {
  AliasLocation ret;
  ret.type = MUST_ALIAS_UNKNOWN;
  ret.location = NULL;
  return ret;
}

static inline AliasLocation allocatedAliasLocation(CallExpr* newExpr) {
  AliasLocation ret;
  ret.type = MUST_ALIAS_ALLOCATED;
  ret.location = newExpr;
  return ret;
}

static inline AliasLocation refAliasLocation(Symbol* referent) {
  AliasLocation ret;
  ret.type = MUST_ALIAS_REFVAR;
  ret.location = referent;
  return ret;
}

/* Gets whatever the symbol refers to, if known.
   If it's not known, returns NULL.
 */
static Symbol* getReferent(Symbol* sym, const AliasMap& aliasMap) {

  AliasLocation loc = unknownAliasLocation();

  {
    AliasMap::const_iterator it = aliasMap.find(sym);
    if (it != aliasMap.end()) {
      loc = it->second;
    }
  }

  if (sym->isRef()) {
    if (loc.type == MUST_ALIAS_REFVAR) {
      Symbol* referentSym = toSymbol(loc.location);
      INT_ASSERT(referentSym && !referentSym->isRef());
      return referentSym;
    }
  }

  return NULL;
}

static AliasLocation aliasLocationFromValue(Symbol* copyFrom,
                                            const AliasMap& aliasMap,
                                            CallExpr* inCall) {

  // we might be dereferencing an unknown pointer
  AliasLocation fromLocation = unknownAliasLocation();

  {
    AliasMap::const_iterator it = aliasMap.find(copyFrom);
    if (it != aliasMap.end()) {
      fromLocation = it->second;
    }
  }

  if (copyFrom->type == dtNil)
    fromLocation = nilAliasLocation(inCall);

  if (copyFrom->isRef()) {
    if (fromLocation.type == MUST_ALIAS_REFVAR) {
      Symbol* sym = toSymbol(fromLocation.location);
      INT_ASSERT(sym && !sym->isRef());
      AliasMap::const_iterator it = aliasMap.find(sym);
      if (it != aliasMap.end()) {
        return it->second;
      }
    }
  }

  // otherwise, return the location we found
  return fromLocation;
}

static AliasLocation aliasLocationFrom(Symbol* copyFrom,
                                       const AliasMap& aliasMap,
                                       CallExpr* inCall) {

  AliasLocation fromLocation = unknownAliasLocation();

  {
    AliasMap::const_iterator it = aliasMap.find(copyFrom);
    if (it != aliasMap.end()) {
      fromLocation = it->second;
    }
  }

  if (copyFrom->type == dtNil)
    fromLocation = nilAliasLocation(inCall);

  // otherwise, return the location we found
  return fromLocation;
}



static bool isClassIshType(Symbol* sym) {
  TypeSymbol* ts = sym->getValType()->symbol;
  return (isClassLike(ts->type) ||
          ts->hasFlag(FLAG_MANAGED_POINTER));
}

static bool isSymbolAnalyzed(Symbol* sym) {
  return sym->isRef() || isClassIshType(sym);
}


static bool isCheckedClassMethodCall(CallExpr* call) {
  FnSymbol* fn     = call->resolvedOrVirtualFunction();
  bool      retval = false;

  if (fn && fn->isMethod() && fn->_this) {
    if (0 == strcmp(fn->name, "borrow")) {
      // Ignore .borrow()
      // (at least until we decide that borrow-from-empty-owned
      //  is a problem, rather than use of the resulting nil)
      // Note that .borrow is available on borrows and unmanaged.
      retval = false;

    } else if (AggregateType* ct = toAggregateType(fn->_this->getValType())) {
      if (fn->numFormals()             >  0 &&
          fn->getFormal(1)->typeInfo() == fn->_this->typeInfo()) {
        if (isClassLike(ct)) {
          retval = true;
        }
      }
    }
  }

  return retval;
}

static void checkForNilDereferencesInCall(
    CallExpr* call,
    const AliasMap& aliasMap) {

  if (call->isPrimitive(PRIM_GET_MEMBER)       ||
      call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
      call->isPrimitive(PRIM_SET_MEMBER)       ||
      call->isPrimitive(PRIM_GETCID)           ||
      call->isPrimitive(PRIM_TESTCID)          ||
      isCheckedClassMethodCall(call)) {

    SymExpr* thisSe = toSymExpr(call->get(1));
    Symbol* thisSym = thisSe->symbol();
    // Doesn't raise errors for methods on managed pointer types (e.g. owned)
    // since:
    //   * right now owned etc can store nil
    //   * borrowing from nil owned isn't an error by itself
    //     but dereferencing that nil borrow would be
    if (isClassLike(thisSym->getValType())) {
      // Raise an error if it was definitely nil
      AliasMap::const_iterator it = aliasMap.find(thisSym);
      if (it != aliasMap.end()) {
        AliasLocation loc = it->second;
        if (loc.type == MUST_ALIAS_NIL) {
          USR_FATAL_CONT(call, "attempt to dereference nil");
          if (!thisSym->hasFlag(FLAG_TEMP)) {
            USR_PRINT(call, "variable %s is nil at this point", thisSym->name);
          }
          if (Expr* fromExpr = toExpr(loc.location)) {
            USR_PRINT(fromExpr, "this statement may be relevant");
          }
        }
      }
    }
  }
}

static void printAliasEntry(const char* prefix, Symbol* sym, AliasLocation loc)
{
  switch (loc.type) {
    case MUST_ALIAS_IGNORED:
      // print nothing
      break;
    case MUST_ALIAS_NIL:
      printf("%s nil: %i %s", prefix, sym->id, sym->name);
      break;
    case MUST_ALIAS_UNKNOWN:
      printf("%s unk: %i %s", prefix, sym->id, sym->name);
      break;
    case MUST_ALIAS_ALLOCATED:
      printf("%s new: %i %s",
             prefix, sym->id, sym->name);
      break;
    case MUST_ALIAS_REFVAR:
      printf("%s ref: %i %s",
             prefix, sym->id, sym->name);
      break;
    // no default -> get warning if more are added
  }

  if (loc.location) {
    int id = loc.location->id;
    if (Symbol* toSym = toSymbol(loc.location)) {
      printf(" -> sym %i %s", id, toSym->name);
    } else if (CallExpr* call = toCallExpr(loc.location)) {
      const char* fnName = "primitive";
      if (FnSymbol* calledFn = call->resolvedOrVirtualFunction())
        fnName = calledFn->name;
      printf(" -> call %i to %s", id, fnName);
    } else {
      printf(" -> unknown %i", id);
    }
  }
  printf("\n");
}

static void printAliasMap(const char* prefix,
                          const AliasMap& OUT,
                          const AliasMap* OnlyDifferencesFrom = NULL)
{
  for (AliasMap::const_iterator it = OUT.begin();
       it != OUT.end();
       ++it) {
    Symbol* sym = it->first;
    AliasLocation loc = it->second;

    if (OnlyDifferencesFrom != NULL) {
      AliasMap::const_iterator it = OnlyDifferencesFrom->find(sym);
      if (it != OnlyDifferencesFrom->end()) {
        AliasLocation was = it->second;
        if (was.type == loc.type &&
            was.location == loc.location)
          // Ignore this one since it's not new
          continue;
      }
    }

    printAliasEntry(prefix, sym, loc);
  }
}

static bool isRecordInitOrReturn(CallExpr* call, SymExpr*& lhsSe, CallExpr*& initOrCtor) {

  if (call->isPrimitive(PRIM_MOVE) ||
      call->isPrimitive(PRIM_ASSIGN)) {
    if (CallExpr* rhsCallExpr = toCallExpr(call->get(2))) {
      if (rhsCallExpr->resolvedOrVirtualFunction()) {
        if (AggregateType* at = toAggregateType(rhsCallExpr->typeInfo())) {
          if (isRecord(at)) {
            SymExpr* se = toSymExpr(call->get(1));
            INT_ASSERT(se);
            lhsSe = se;
            initOrCtor = rhsCallExpr;
            return true;
          }
        }
      }
    }
  }

  if (FnSymbol* calledFn = call->resolvedOrVirtualFunction()) {
    if (calledFn->isMethod() && calledFn->name == astrInit) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);
      Symbol* sym = se->symbol();
      if (isRecord(sym->type)) {
        lhsSe = se;
        initOrCtor = call;
        return true;
      }
    } else if (calledFn->hasFlag(FLAG_FN_RETARG)) {
      for_formals_actuals(formal, actual, call) {
        if (formal->hasFlag(FLAG_RETARG)) {
          if (isRecord(formal->getValType())) {
            SymExpr* se = toSymExpr(actual);
            INT_ASSERT(se);
            lhsSe = se;
            initOrCtor = call;
            return true;
          }
        }
      }
    }
  }

  lhsSe = NULL;
  initOrCtor = NULL;
  return false;
}

static bool isOuterVar(Symbol* sym, FnSymbol* fn) {

  if (sym == gVoid)
    return false;

  if (sym->defPoint->parentSymbol == fn)
    return false;

  return true;
}

static void update(AliasMap& OUT, Symbol* lhs, AliasLocation loc) {
  if (isSymbolAnalyzed(lhs))
    OUT[lhs] = loc;
}

static void checkCall(
    FnSymbol* fn,
    BasicBlock* bb,
    CallExpr* call,
    const std::vector<Symbol*> symbols,
    AliasMap& OUT,
    bool raiseErrors,
    bool & storeToUnknown,
    bool & storeToGlobals) {

  Symbol* lhsSym = NULL;
  Expr* rhsExpr = NULL;

  bool moveLike = false;
  bool assign = false;
  CallExpr* userCall = NULL;
  SymExpr* initSe = NULL;
  CallExpr* initCall = NULL;

  // set moveLike and userCall
  if (call->isPrimitive(PRIM_MOVE) ||
      call->isPrimitive(PRIM_ASSIGN)) {
    initSe = toSymExpr(call->get(1));
    lhsSym = initSe->symbol();
    rhsExpr = call->get(2);
    moveLike = true;
    userCall = toCallExpr(rhsExpr);
  } else if (call->isNamedAstr(astrSassign)) {
    initSe = toSymExpr(call->get(1));
    lhsSym = initSe->symbol();
    rhsExpr = call->get(2);
    moveLike = true;
    assign = true;
    userCall = call;
  } else if (isRecordInitOrReturn(call, initSe, initCall)) {
    lhsSym = initSe->symbol();
    rhsExpr = initCall;
    moveLike = true;
    userCall = call;
  } else {
    userCall = call;
  }

  FnSymbol* userCalledFn = NULL;
  if (userCall) {
    userCalledFn = userCall->resolvedOrVirtualFunction();
  }

  bool ignoreGlobalUpdates = false;

  // raise errors if the call is problematic somehow
  if (userCall && raiseErrors) {
    checkForNilDereferencesInCall(userCall, OUT);
  }

  if (moveLike) {
    // lhsSym and rhsExpr set above

    if (lhsSym->isRef()) {
      // move-ish, but lhs is a ref

      if (assign && isClassIshType(lhsSym)) {
        // e.g. owned = nil
        SymExpr* se = toSymExpr(call->get(2));
        Symbol* rhsSym = se->symbol();
        Symbol* referent = getReferent(lhsSym, OUT);
        if (referent)
          update(OUT, referent, aliasLocationFromValue(rhsSym, OUT, call));
        else if (lhsSym->hasFlag(FLAG_RETARG) == false)
          storeToUnknown = true;

      } else if (userCalledFn != NULL) {
        // e.g.
        //   ref r = returnsRefArg(x);
        // We can't say what 'r' refers to without analyzing that fn
        //   - it could refer to a global
        //   - it could refer to a by-ref argument
        //   - it could refer to a field in a heap-allocated class

        // This could be another class, ref-to-unknown, if it helps
        update(OUT, lhsSym, unknownAliasLocation());
      } else {
        if (userCall == NULL) {
          // Acts like a move
          SymExpr* se = toSymExpr(call->get(2));
          Symbol* rhsSym = se->symbol();
          if (call->isPrimitive(PRIM_MOVE)) {
            update(OUT, lhsSym, aliasLocationFrom(rhsSym, OUT, call));
          } else {
            Symbol* referent = getReferent(lhsSym, OUT);
            if (referent)
              update(OUT, referent, aliasLocationFromValue(rhsSym, OUT, call));
            else if (lhsSym->hasFlag(FLAG_RETARG) == false)
              storeToUnknown = true;
          }
        } else {
          if (userCall->isPrimitive(PRIM_ADDR_OF) ||
              userCall->isPrimitive(PRIM_SET_REFERENCE)) {
            SymExpr* se = toSymExpr(userCall->get(1));
            Symbol* rhs = se->symbol();
            if (rhs->isRef())
              // if rhs is a ref, just propagate any MUST_ALIAS_REFVAR
              update(OUT, lhsSym, aliasLocationFrom(rhs, OUT, call));
            else
              // otherwise, make a new MUST_ALIAS_REFVAR
              update(OUT, lhsSym, refAliasLocation(se->symbol()));
          } else {
            update(OUT, lhsSym, unknownAliasLocation());
          }
        }
      } // ending (userCalledFn == NULL)

    } else {
      // move-ish, LHS is not a ref

      // handle certain cases for PRIM_MOVE that we understand
      if (rhsExpr->typeInfo() == dtNil) {
        update(OUT, lhsSym, nilAliasLocation(call));

      } else if (SymExpr* se = toSymExpr(rhsExpr)) {
        update(OUT, lhsSym, aliasLocationFromValue(se->symbol(), OUT, call));

      } else if (userCalledFn != NULL) {

        if (isClassIshType(lhsSym)) {
          // defaultOf for classes results in nil
          if (userCalledFn->name == astr_defaultOf) {
            update(OUT, lhsSym, nilAliasLocation(call));
            ignoreGlobalUpdates = true;

          // new for classes is allocation
          // note that this is adjusted below for 'new owned(instance)'
          } else if (userCalledFn->name == astrNew) {
            update(OUT, lhsSym, allocatedAliasLocation(userCall));
            ignoreGlobalUpdates = true;

          // for copy-init or borrow, lhs tracks rhs
          // for owned, initCopy might invalidate RHS; handled below
          } else if (userCalledFn->hasFlag(FLAG_INIT_COPY_FN) ||
                     userCalledFn->hasFlag(FLAG_AUTO_COPY_FN) ||
                     0 == strcmp(userCalledFn->name, "borrow")) {
            Symbol* rhsSym = toSymExpr(userCall->get(1))->symbol();
            update(OUT, lhsSym, aliasLocationFromValue(rhsSym, OUT, call));
            ignoreGlobalUpdates = true;
          } else {
            Expr* nilFromActual = NULL;
            for_formals_actuals(formal, actual, userCall) {
              if (formal->hasFlag(FLAG_NIL_FROM_ARG)) {
                nilFromActual = actual;
              }
            }
            if (nilFromActual != NULL) {
              SymExpr* actualSe = toSymExpr(nilFromActual);
              Symbol* actualSym = actualSe->symbol();
              update(OUT, lhsSym, aliasLocationFromValue(actualSym, OUT, call));
              ignoreGlobalUpdates = true;
            }
          }

        } else {
          update(OUT, lhsSym, unknownAliasLocation());
        }

      } else {
        // userCalledFn == NULL
        // handle primitives

        if (userCall->isPrimitive(PRIM_DEREF))
          update(OUT, lhsSym,
                 aliasLocationFromValue(toSymExpr(userCall->get(1))->symbol(),
                                        OUT, call));

        else if (userCall->isPrimitive(PRIM_CAST) &&
                 isClassIshType(lhsSym))
          update(OUT, lhsSym,
                 aliasLocationFromValue(toSymExpr(userCall->get(2))->symbol(),
                                        OUT, call));
        else
          update(OUT, lhsSym, unknownAliasLocation());

      }
    }
  }

  // Handle FLAG_LEAVES_ARG_NIL, ref arguments
  if (userCalledFn != NULL) {

    // any global variables could possibly be set
    // by the function call
    if (ignoreGlobalUpdates == false)
      storeToGlobals = true;

    for_formals_actuals(formal, actual, userCall) {
      if (formal->hasFlag(FLAG_LEAVES_ARG_NIL)) {
        Symbol* actualSym = toSymExpr(actual)->symbol();
        if (actualSym->isRef()) {
          Symbol* referent = getReferent(actualSym, OUT);
          if (referent)
            update(OUT, referent, nilAliasLocation(call));
        } else {
          update(OUT, actualSym, nilAliasLocation(call));
        }

      } else if (formal->intent == INTENT_REF &&
                 /* ignoring functions returning record by ref */
                 actual != initSe) {
        // It could set the actual
        Symbol* actualSym = toSymExpr(actual)->symbol();
        if (actualSym->isRef()) {
          Symbol* referent = getReferent(actualSym, OUT);
          if (referent)
            update(OUT, referent, unknownAliasLocation());
          else if (actualSym->hasFlag(FLAG_RETARG) == false)
            storeToUnknown = true;

        } else if (isSymbolAnalyzed(actualSym)) {
          update(OUT, actualSym, unknownAliasLocation());
        }
      }
    }
  }
}

// Is 'firstExpr' the first expr of a conditional branch?
// If so, set 'parentCond' and 'inThenBranch'.
static bool atStartOfCondBranch(Expr* firstExpr, BasicBlock* predBB,
                                CondStmt*& parentCond, bool& inThenBranch) {
  Expr* curr = firstExpr;
  do {
    if (curr->prev != NULL) return false; // not the first expr
    Expr* parent = curr->parentExpr;

    if (CondStmt* pc = toCondStmt(parent)) {
      if (curr == pc->thenStmt)
        { parentCond = pc; inThenBranch = true; return true; }
      if (curr == pc->elseStmt)
        { parentCond = pc; inThenBranch = false; return true; }
      return false;
    }

    curr = parent;
  } while (curr != NULL);

  return false;
}

// If 'expr' is a SymExpr, return the CallExpr that is 'move'-ed into it.
static Expr* getSingleDefExpr(Expr* expr) {
  if (SymExpr* SE = toSymExpr(expr))
   if (SymExpr* seDef = SE->symbol()->getSingleDef())
    if (CallExpr* move = toCallExpr(seDef->parentExpr))
     if (move->isPrimitive(PRIM_MOVE))
      return move->get(2);
  return NULL;
}

//
// If we see this pattern:
//
//   move( shouldHandleError, check error( error ) )
//   if shouldHandleError
//     { we are here ... }
//
// then update 'error' to be "allocated", as it is surely non-nil.
//
static void adjustMapForCatchBlock(CondStmt* cond, bool inThenBranch,
                                   AliasMap& OUT) {
  // We could heuristically check if cond->condExpr's symbol
  // is named "shouldHandleError".
  if (CallExpr* CE = toCallExpr(getSingleDefExpr(cond->condExpr))) {
    if (CE->isPrimitive(PRIM_CHECK_ERROR)) {
      if (SymExpr* errorSE = toSymExpr(CE->get(1))) {
        // Found the pattern.
        Symbol* errorSym = errorSE->symbol();
        // In this pattern, errorSym->name is "error".
        AliasLocation errorAL;
        errorAL.type = inThenBranch ? MUST_ALIAS_ALLOCATED : MUST_ALIAS_NIL;
        errorAL.location = cond->condExpr;
        update(OUT, errorSym, errorAL);
      }
    }
  }
}

// Update 'OUT' based on being inside a conditional,
// if 'bb' is the starting BasicBlock of the then- or else- branch.
static void adjustMapForConditional(BasicBlock* bb, AliasMap& OUT) {
  if (bb->ins.size() != 1 || bb->exprs.size() == 0)
    return; // quick check says we are not at start of a cond branch

  CondStmt* parentCond = NULL;
  bool    inThenBranch = true;
  if (!atStartOfCondBranch(bb->exprs[0], bb->ins[0], parentCond, inThenBranch))
    return; // not at start of a cond branch for sure

  adjustMapForCatchBlock(parentCond, inThenBranch, OUT);
}

static void checkBasicBlock(
    FnSymbol* fn,
    BasicBlock* bb,
    const std::vector<Symbol*> symbols,
    const AliasMap& IN,
    AliasMap& OUT,
    bool raiseErrors) {

  OUT = IN;
  adjustMapForConditional(bb, OUT);

  for_vector(Expr, expr, bb->exprs) {

    bool storeToUnknown = false;
    bool storeToGlobals = false;

    bool trace = (0 == strcmp(fn->name, debugNilsForFn));

    AliasMap* traceCopy = NULL;

    if (trace) {
      traceCopy = new AliasMap(OUT);

      printf("\nvisiting:");
      nprint_view(expr);

      printAliasMap("bb", OUT);
    }

    if (expr->id == debugNilsForId)
      gdbShouldBreakHere();

    if (isDefExpr(expr)) {
      // Ignore DefExprs
      // instead, find the defaultOf or whatever initializes them.
    } else if (ForallStmt* forall = toForallStmt(expr)) {

      for_alist(expr, forall->iteratedExpressions()) {
        if (CallExpr* call = toCallExpr(expr)) {
          checkCall(fn, bb, call, symbols, OUT, raiseErrors,
                    storeToUnknown, storeToGlobals);
        }
      }

    } else if (CallExpr* call = toCallExpr(expr)) {
      checkCall(fn, bb, call, symbols, OUT, raiseErrors,
                storeToUnknown, storeToGlobals);

    }

    // Handle storeToUnknown, storeToGlobals
    // TODO: storeToUnknown should only give up on
    // other variables of compatible types
    if (storeToGlobals || storeToUnknown) {
      for_vector(Symbol, sym, symbols) {
        if (sym->isConstValWillNotChange() == false) {
          if (storeToUnknown || isOuterVar(sym, fn))
            OUT[sym] = unknownAliasLocation();
        }
      }
    }

    if (trace) {
      printAliasMap("+-", OUT, traceCopy);
      delete traceCopy;
    }
  }
}

static void gatherVariablesToCheck(FnSymbol* fn,
                                   std::vector<Symbol*>& idxToSym,
                                   bool debugging) {

  // Gather the variables to consider
  std::map<Symbol*, int> symToIdx;

  int index = 0;
  std::vector<SymExpr*> symExprs;
  collectSymExprs(fn, symExprs);
  for_vector(SymExpr, se, symExprs) {
    Symbol* sym = se->symbol();
    if (isArgSymbol(sym) || isVarSymbol(sym)) {
      if (isSymbolAnalyzed(sym)) {
        if (symToIdx.count(sym) == 0) {
          // Add it to the map/array
          symToIdx.insert(std::pair<Symbol*, int>(sym, index));
          idxToSym.push_back(sym);
          INT_ASSERT((int)(idxToSym.size()-1) == index);
          index++;
          if (debugging) {
            printf("Tracking symbol %i %s\n", sym->id, sym->name);
          }
        }
      }
    }
  }
}

static bool combine(FnSymbol* fn,
                    BasicBlock* bb,
                    AliasMap& nextIn,
                    const AliasMap& from,
                    bool debugging) {

  bool changed = false;

  for (AliasMap::const_iterator it = from.begin();
       it != from.end();
       ++it) {
    Symbol* sym = it->first;
    AliasLocation loc = it->second;

    if (loc.type == MUST_ALIAS_IGNORED)
      continue;

    if (nextIn.count(sym) == 0) {
      nextIn[sym] = loc;
      if (debugging) {
        printf("{} here -> get from bb %i\n", (int) bb->id);
        printAliasEntry("   ", sym, loc);
      }
      changed = true;
    } else {
      AliasLocation was = nextIn[sym];
      if (debugging) {
        printf("combining from bb %i\n", (int) bb->id);
        printAliasEntry("    nextIn", sym, was);
        printAliasEntry("    inbb", sym, loc);
      }

      // combine alias locations
      if (was.type == loc.type &&
          was.location == loc.location) {
        // OK, do nothing
      } else if (was.type == MUST_ALIAS_NIL &&
                 loc.type == MUST_ALIAS_NIL ) {
        // OK, do nothing
        // only difference is reason for nil
      } else if (was.type == MUST_ALIAS_IGNORED) {
        if (debugging) {
          printf("ignored here -> get from bb %i\n", (int) bb->id);
          printAliasEntry("   ", sym, loc);
        }
        nextIn[sym] = loc;
        changed = true;
      } else {
        // Conflicting input -> unknown
        nextIn[sym] = unknownAliasLocation();
        changed = true;
      }

      if (debugging) {
        printf("result of combining from bb %i\n", (int) bb->id);
        printAliasEntry("    nextIn", sym, nextIn[sym]);
      }
    }
  }

  return changed;
}

void findNilDereferences(FnSymbol* fn) {

  bool debugging = 0 == strcmp(fn->name, debugNilsForFn) ||
                   fn->id == debugNilsForId;

  if (debugging) {
    printf("Visiting function %s id %i\n", fn->name, fn->id);
    nprint_view(fn);
    gdbShouldBreakHere();
  }

  BasicBlock::buildBasicBlocks(fn);

  // Stores variables we are considering
  std::vector<Symbol*> idxToSym;
  gatherVariablesToCheck(fn, idxToSym, debugging);

  // If there were no variables to check, don't continue
  if (idxToSym.size() == 0)
    return;

  size_t nbbs = fn->basicBlocks->size();

  // allocate the IN and OUT data flow sets
  std::vector<AliasMap> IN(nbbs);
  std::vector<AliasMap> OUT(nbbs);

  std::vector<int> forwardOrder;
  BasicBlock::computeForwardOrder(fn, forwardOrder);

  INT_ASSERT(forwardOrder.size() == nbbs);

  // these arrays help track which basic blocks need visiting
  std::vector<int> visit(nbbs);
  std::vector<int> changed(nbbs);
  for (size_t i = 0; i < nbbs; i++) {
    visit[i] = true;
    changed[i] = false;
  }

  bool anychanged = true;

  while (anychanged) {
    if (debugging)
      printf("In fixed point loop for %s\n", fn->name);

    for (size_t i = 0; i < nbbs; i++) {
      changed[i] = false;
    }

    for (size_t ii = 0; ii < nbbs; ii++) {
      int i = forwardOrder[ii]; // visit basic block i
      if (visit[i]) {
        BasicBlock* bb = (*fn->basicBlocks)[i];
        if (debugging) {
          printf("running checkBasicBlock on bb %i\n", i);
        }
        checkBasicBlock(fn, bb, idxToSym, IN[i], OUT[i],
                        /*raise errors?*/ false);

        // We just computed OUT. That might impact the IN values
        // for successor blocks. If it does, update them now.
        // If any of them change, update changed.
        for_vector(BasicBlock, bbout, bb->outs) {
          int j = bbout->id;
          changed[j] = combine(fn, bbout, IN[j], OUT[i], debugging);
        }
      }
    }

    anychanged = false;
    for (size_t i = 0; i < nbbs; i++) {
      if (changed[i])
        anychanged = true;
      visit[i] = changed[i];
    }
  }

  for (size_t i = 0; i < nbbs; i++) {
    checkBasicBlock(fn, (*fn->basicBlocks)[i], idxToSym,
                    IN[i], OUT[i],
                    /*raise errors?*/ true);
  }
}

void adjustSignatureForNilChecking(FnSymbol* fn) {
  if (fn->_this != NULL) {
    if (fn->hasFlag(FLAG_NIL_FROM_THIS))
      fn->_this->addFlag(FLAG_NIL_FROM_ARG);
    if (fn->hasFlag(FLAG_LEAVES_THIS_NIL))
      fn->_this->addFlag(FLAG_LEAVES_ARG_NIL);
  }
}
