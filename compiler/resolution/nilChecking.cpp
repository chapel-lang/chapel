/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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
  MUST_ALIAS_IGNORED = 0, // no information; useful starting point.
                          // ignored when combining basic blocks

  MUST_ALIAS_REFVAR,      // reference refers to a particular variable
                          // (or borrow, if not a reference)

  MUST_ALIAS_ALLOCATED,   // refers to the statement allocating (aka 'new')

  MUST_ALIAS_NIL,         // it refers to nil

  MUST_ALIAS_UNKNOWN,     // might have been set, might point to nil
                          // who knows what it points to

  MUST_ALIAS_DEAD,        // it refers to dead memory along some path

} AliasType;

struct AliasLocation {
  AliasType type;
  // MUST_ALIAS_ALLOCATED -> CallExpr for the allocation
  //                         or condExpr of CondStmt establishing non-nil-ness
  // MUST_ALIAS_REFVAR -> ArgSymbol or VarSymbol being referred to
  // MUST_ALIAS_NIL, -> a reason it is nil ex. assignment from nil
  // MUST_ALIAS_DEAD    or condExpr of CondStmt establishing nil-ness
  BaseAST* location;
  AliasLocation() : type(MUST_ALIAS_IGNORED), location(NULL) { }
};

static bool isNonNilableType(Type* t);
static bool isOuterVar(Symbol* sym, FnSymbol* fn);
static void findNonNilableStoringNil(FnSymbol* fn);

typedef std::map<Symbol*, AliasLocation> AliasMap;

static inline AliasLocation nilAliasLocation(BaseAST* reason) {
  AliasLocation ret;
  ret.type = MUST_ALIAS_NIL;
  ret.location = reason;
  return ret;
}

static inline AliasLocation deadAliasLocation(BaseAST* reason) {
  AliasLocation ret;
  ret.type = MUST_ALIAS_DEAD;
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

  if (loc.type == MUST_ALIAS_REFVAR) {
    Symbol* referentSym = toSymbol(loc.location);
    INT_ASSERT(referentSym && !referentSym->isRef());
    return referentSym;
  }

  return NULL;
}

static AliasLocation aliasLocationFromValue(Symbol* copyFrom,
                                            const AliasMap& aliasMap,
                                            CallExpr* inCall) {

  // we might be dereferencing an unknown pointer
  AliasLocation fromLocation = unknownAliasLocation();

  if (copyFrom->type == dtNil) {
    fromLocation = nilAliasLocation(inCall);

  } else {
    AliasMap::const_iterator it = aliasMap.find(copyFrom);
    if (it != aliasMap.end()) {
      fromLocation = it->second;
    }
  }

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



static bool isClassIshType(Type* t) {
  TypeSymbol* ts = t->getValType()->symbol;
  return (isClassLike(ts->type) ||
          ts->hasFlag(FLAG_MANAGED_POINTER));
}
static bool isClassIshType(Symbol* sym) {
  return isClassIshType(sym->getValType());
}

static bool isSymbolAnalyzed(Symbol* sym) {
  return sym->isRef() ||
         isClassIshType(sym) ||
         (isRecord(sym->type) &&
          sym->hasFlag(FLAG_MAYBE_COPY_ELIDED) &&
          !sym->type->symbol->hasFlag(FLAG_POD));
}

// If 'call' invokes postfix!, return the result type.
// Ignore postfix! calls on arrays - needed if we have a 'proc postfix!(a:[])'.
static Type* isPostfixBangCall(CallExpr* call) {
  if (FnSymbol* fn = call->resolvedFunction()) {
    if (fn->name == astrPostfixBang) {
      if (call->get(1)->getValType()->symbol->hasFlag(FLAG_ARRAY))
        return NULL;
      INT_ASSERT(call->numActuals() == 1); // `!` takes a single arg
      return fn->retType;
    }
  }
  return NULL;
}

static bool isCheckedClassMethodCall(CallExpr* call) {
  FnSymbol* fn     = call->resolvedOrVirtualFunction();
  bool      retval = false;

  if (fn && fn->isMethod() && fn->_this) {
    if (fn->name == astrBorrow) {
      // Ignore .borrow()
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

static void issueNilError(const char* message, Expr* ref,
                          Symbol* var, Symbol* referent, AliasLocation loc) {

  // Ignore multiple errors on the same line
  if (printsSameLocationAsLastError(ref))
    return;

  USR_FATAL_CONT(ref, "%s", message);

  Symbol* v  = var;
  if (referent != NULL) {
    if (!var->hasFlag(FLAG_TEMP) && !referent->hasFlag(FLAG_TEMP))
      USR_PRINT(ref, "'%s' refers to '%s'", var->name, referent->name);
    if (!referent->hasFlag(FLAG_TEMP))
      v = referent;
  }

  if (Expr* fromExpr = toExpr(loc.location)) {
    CallExpr* call = toCallExpr(fromExpr);
    FnSymbol* fn = call ? call->resolvedOrVirtualFunction() : NULL;
    const char* why = NULL;
    if (call && call->isPrimitive(PRIM_ASSIGN_ELIDED_COPY))
      why = "is dead due to copy elision here";
    else if (fn && fn->hasFlag(FLAG_AUTO_DESTROY_FN))
      why = "is dead due to deinitialization here";
    else if (loc.type == MUST_ALIAS_DEAD)
      why = "is dead due to ownership transfer here";
    else
      why = "is set to nil here";

    if (v->hasFlag(FLAG_TEMP))
      USR_PRINT(fromExpr, "value %s", why);
    else
      USR_PRINT(fromExpr, "'%s' %s", v->name, why);
  }
}

static void replaceBangWithCast(CallExpr* call, Type* type, Symbol* arg) {
  SET_LINENO(call);
  Symbol* toCast = arg;

  Type* argType = arg->getValType();
  if (argType->symbol->hasFlag(FLAG_MANAGED_POINTER)) {
    // Extract the 'chpl_p' field.
    Symbol* pField = toAggregateType(argType)->getField("chpl_p");
    VarSymbol* pTemp = newTempConst(pField->type);
    call->getStmtExpr()->insertBefore(new DefExpr(pTemp));
    call->getStmtExpr()->insertBefore("'move'(%S,'.v'(%S,%S))",
                                      pTemp, arg, pField);
    toCast = pTemp;
  }

  CallExpr* cast = new CallExpr(PRIM_CAST, type->symbol, toCast);
  call->replace(cast);
}

static void checkForNilDereferencesInCall(
    CallExpr* call,
    const AliasMap& aliasMap) {

  Symbol* alreadyErrorSym = NULL;

  if (call->isPrimitive(PRIM_GET_MEMBER)       ||
      call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
      call->isPrimitive(PRIM_SET_MEMBER)       ||
      call->isPrimitive(PRIM_GETCID)           ||
      call->isPrimitive(PRIM_TESTCID)          ||
      isCheckedClassMethodCall(call)) {

    SymExpr* thisSe = toSymExpr(call->get(1));
    Symbol* thisSym = thisSe->symbol();
    Type* t = thisSym->getValType();
    // Doesn't raise errors for methods on nilable managed pointer types
    // (e.g. owned) since:
    //   * borrowing from nil owned isn't an error by itself
    //     but dereferencing that nil borrow would be
    if (isClassLike(t) || isNonNilableType(t)) {
      // Raise an error if it was definitely nil
      AliasMap::const_iterator it = aliasMap.find(thisSym);
      if (it != aliasMap.end()) {
        AliasLocation loc = it->second;
        if (loc.type == MUST_ALIAS_NIL || loc.type == MUST_ALIAS_DEAD) {
          issueNilError("attempt to dereference nil", call, thisSym, NULL, loc);
          alreadyErrorSym = thisSym;
        }
      }
    }
  } else if (Type* resultType = isPostfixBangCall(call)) {
    Symbol* argSym = toSymExpr(call->get(1))->symbol();
    AliasMap::const_iterator it = aliasMap.find(argSym);
    if (it != aliasMap.end()) {
      AliasLocation loc = it->second;
      Symbol* referent = NULL;
      if (loc.type == MUST_ALIAS_REFVAR) {
        referent = getReferent(argSym, aliasMap);
        if (referent != NULL)
          loc = aliasLocationFromValue(referent, aliasMap, call);
      }
      if (loc.type == MUST_ALIAS_NIL) {
        issueNilError("applying postfix-! to nil",
                      call, argSym, referent, loc);
        alreadyErrorSym = argSym;
      } else if (loc.type == MUST_ALIAS_DEAD) {
        issueNilError("applying postfix-! to dead value",
                      call, argSym, referent, loc);
        alreadyErrorSym = argSym;
      } else if (loc.type == MUST_ALIAS_ALLOCATED) {
        // Postfix-! will always succeed. Replace it with a cast.
        replaceBangWithCast(call, resultType, argSym);
      }
    }
  } else if (call->isPrimitive(PRIM_RETURN) && call->numActuals() >= 1) {
    Symbol* argSym = toSymExpr(call->get(1))->symbol();
    AliasMap::const_iterator it = aliasMap.find(argSym);
    if (it != aliasMap.end()) {
      AliasLocation loc = it->second;
      Symbol* referent = NULL;
      if (loc.type == MUST_ALIAS_REFVAR) {
        referent = getReferent(argSym, aliasMap);
        if (referent != NULL)
          loc = aliasLocationFromValue(referent, aliasMap, call);
      }

      if (loc.type == MUST_ALIAS_DEAD) {
        issueNilError("Illegal return of dead value",
                      call, argSym, referent, loc);
        alreadyErrorSym = argSym;
      }
    }
  }

  // For records and non-nilable class types, check that any argument
  // to a user function is not dead.
  // Also check for transferring ownership out of an unknown reference
  // to a non-nilable owned.
  FnSymbol* inFn = call->getFunction();
  if (FnSymbol* calledFn = call->resolvedOrVirtualFunction()) {
    if (!calledFn->hasFlag(FLAG_AUTO_DESTROY_FN) &&
        !calledFn->hasFlag(FLAG_UNSAFE) &&
        !(inFn != NULL && inFn->hasFlag(FLAG_LEAVES_ARG_NIL))) {
      int i = 0;
      for_formals_actuals(formal, actual, call) {
        i++;
        Symbol* argSym = toSymExpr(actual)->symbol();

        if (formal->hasFlag(FLAG_RETARG))
          continue; // ignore ret args for this check
        if (i == 1 &&
            calledFn->isMethod() &&
            (calledFn->name == astrInit || calledFn->name == astrInitEquals))
          continue; // ignore this in init functions for this check
        if (i == 1 && calledFn->name == astrSassign)
          continue; // ignore lhs of = for this check
        if (formal->intent == INTENT_OUT ||
            formal->originalIntent == INTENT_OUT)
          continue; // ignore out formals for this check

        Type* valType = argSym->getValType();
        bool isNonNilableC = isNonNilableType(valType);
        bool isNilableC = (isNonNilableC == false && isClassIshType(valType));
        bool isRec = isRecord(valType) && !isNonNilableC && !isNilableC;
        if (isNonNilableC || isRec) {
          AliasLocation loc = unknownAliasLocation();
          AliasMap::const_iterator it = aliasMap.find(argSym);
          if (it != aliasMap.end()) {
            loc = it->second;
          }

          Symbol* referent = NULL;
          if (loc.type == MUST_ALIAS_REFVAR) {
            referent = getReferent(argSym, aliasMap);
            if (referent != NULL)
              loc = aliasLocationFromValue(referent, aliasMap, call);
          }

          if (loc.type == MUST_ALIAS_DEAD ||
              (loc.type == MUST_ALIAS_NIL && isNonNilableC)) {
            if (argSym != alreadyErrorSym) {
              issueNilError("Illegal use of dead value",
                            call, argSym, referent, loc);
              alreadyErrorSym = argSym;
            }
          }

          // check for transferring ownership out of unk ref to non-nilable
          if (formal->hasFlag(FLAG_LEAVES_ARG_NIL)) {
            Symbol* actualSym = argSym;
            if (referent != NULL)
              actualSym = referent;

            const char* error = NULL;
            if (isArgSymbol(actualSym)) {
              // leaves-arg-nil OK for e.g. owned initCopy/init= etc.
              if (actualSym->hasFlag(FLAG_LEAVES_ARG_NIL)) {
                // OK
              } else if (formal->intent == INTENT_IN ||
                         formal->intent == INTENT_CONST_IN ||
                         formal->originalIntent == INTENT_IN ||
                         formal->originalIntent == INTENT_CONST_IN) {
                // OK
              } else {
                error = "Cannot transfer ownership from non-nilable class argument";
              }
            } else if (actualSym->hasFlag(FLAG_TEMP) &&
                       loc.type == MUST_ALIAS_ALLOCATED) {
              // OK, allow for compiler temporaries
            } else if (isOuterVar(actualSym, call->getFunction())) {
              error = "Cannot transfer ownership from a non-nilable outer variable";
            } else if (argSym->isRef()) {
              error = "Cannot transfer ownership from this non-nilable reference variable";
            }
            if (error != NULL)
              issueNilError(error, call, argSym, NULL, loc);
          }
        }
        i++;
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
    case MUST_ALIAS_DEAD:
      printf("%s ded: %i %s", prefix, sym->id, sym->name);
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

static bool isOuterVar(Symbol* sym, FnSymbol* fn) {

  if (sym == gVoid)
    return false;

  if (sym->defPoint->parentSymbol == fn)
    return false;

  return true;
}

// This is a version of isNonNilableClassType() for post-resolution,
// when we can't invoke classTypeDecorator() / getManagedPtrBorrowType()
// because the field 'chpl_t' is no more. So instead we rely on a flag.
static bool isNonNilableType(Type* t) {
  Symbol* ts = t->symbol;
  if (ts->hasFlag(FLAG_MANAGED_POINTER))
    return ts->hasFlag(FLAG_MANAGED_POINTER_NONNILABLE);
  else
    return isNonNilableClassType(t);
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

  if (call->id == debugNilsForId)
    gdbShouldBreakHere();

  // set moveLike and userCall
  if (call->isPrimitive(PRIM_MOVE) ||
      call->isPrimitive(PRIM_ASSIGN) ||
      call->isPrimitive(PRIM_ASSIGN_ELIDED_COPY)) {
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
                     userCalledFn->hasFlag(FLAG_AUTO_COPY_FN)) {
            Symbol* rhsSym = toSymExpr(userCall->get(1))->symbol();
            update(OUT, lhsSym, aliasLocationFromValue(rhsSym, OUT, call));
            ignoreGlobalUpdates = true;
          } else if (userCalledFn->name == astrPostfixBang) {
            // the outcome of postfix-! is always non-nilable
            update(OUT, lhsSym, allocatedAliasLocation(call));
          } else if (userCalledFn->name == astrBorrow &&
                     userCalledFn->isMethod() && userCalledFn->_this) {
            // a borrow creates an alias to the original variable
            SymExpr* se = toSymExpr(userCall->get(1));
            Symbol* rhs = se->symbol();
            if (rhs->isRef())
              // if rhs is a ref, just propagate any MUST_ALIAS_REFVAR
              update(OUT, lhsSym, aliasLocationFrom(rhs, OUT, call));
            else
              // otherwise, make a new MUST_ALIAS_REFVAR
              update(OUT, lhsSym, refAliasLocation(se->symbol()));
          } else {
            Expr* nilFromActual = NULL;
            Expr* retActual = NULL;
            for_formals_actuals(formal, actual, userCall) {
              if (formal->hasFlag(FLAG_NIL_FROM_ARG)) {
                nilFromActual = actual;
              } else if (formal->hasFlag(FLAG_RETARG)) {
                retActual = actual;
              }
            }

            if (nilFromActual != NULL) {
              SymExpr* actualSe = toSymExpr(nilFromActual);
              Symbol* actualSym = actualSe->symbol();
              update(OUT, lhsSym, aliasLocationFromValue(actualSym, OUT, call));
              ignoreGlobalUpdates = true;

            } else if (retActual != NULL) {
              // Analogously to the "non-nilable return type" case below.
              Symbol* retSymbol = toSymExpr(retActual)->symbol();
              if (isNonNilableType(retActual->getValType()))
                update(OUT, retSymbol, allocatedAliasLocation(call));

            } else if (isNonNilableType(userCalledFn->retType->getValType())) {
              // Non-nilable return type - assume we get a non-nil.
              // We check this as the last resort due to holes in typechecking.
              // For example, this allows us to know that x.borrow() returns nil
              // when x is nil even when it is of a non-nilable type - because
              // that check comes earlier. When the holes are fixed, this check
              // can go all the way to the top of the "if (moveLike)" case.
              update(OUT, lhsSym, allocatedAliasLocation(call));
            }
          }

        } else {
          // Assume record init or by-value return produces something not
          // storing nil.
          update(OUT, lhsSym, allocatedAliasLocation(call));
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
        Type* t = actualSym->getValType();
        bool canBeNil = isClassIshType(t) && !isNonNilableType(t);
        if (actualSym->isRef()) {
          Symbol* referent = getReferent(actualSym, OUT);
          if (referent) {
            if (canBeNil)
              update(OUT, referent, nilAliasLocation(call));
            else
              update(OUT, referent, deadAliasLocation(call));
          }
        } else {
          if (canBeNil)
            update(OUT, actualSym, nilAliasLocation(call));
          else
            update(OUT, actualSym, deadAliasLocation(call));
        }

      } else if (formal->intent == INTENT_REF &&
                 /* ignoring functions returning record by ref */
                 actual != initSe &&
                 /* ignoring field accessors */
                 !userCalledFn->hasFlag(FLAG_FIELD_ACCESSOR)
                 ) {
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

  // Handle making the RHS of PRIM_ASSIGN_ELIDED_COPY dead
  if (call->isPrimitive(PRIM_ASSIGN_ELIDED_COPY)) {
    if (SymExpr* rhsSe = toSymExpr(rhsExpr)) {
      Symbol* actualSym = rhsSe->symbol();
      if (actualSym->isRef()) {
        Symbol* referent = getReferent(actualSym, OUT);
        if (referent)
          update(OUT, referent, deadAliasLocation(call));
      } else {
        update(OUT, actualSym, deadAliasLocation(call));
      }
    }
  }

  // Handle making destroyed record variables dead
  if (userCalledFn != NULL && userCalledFn->hasFlag(FLAG_AUTO_DESTROY_FN)) {
    SymExpr* se = toSymExpr(userCall->get(1));
    Symbol* sym = se->symbol();
    if (sym->isRef()) {
      Symbol* referent = getReferent(sym, OUT);
      if (referent)
        update(OUT, referent, deadAliasLocation(call));
    } else {
      update(OUT, sym, deadAliasLocation(call));
    }
  }
}

// Update 'sym' with "allocated" or "nil"
static void updateForConditional(Symbol* sym, bool isAllocated,
                                 Expr* location, AliasMap& OUT) {
  AliasLocation errorAL;
  errorAL.type = isAllocated ? MUST_ALIAS_ALLOCATED : MUST_ALIAS_NIL;
  errorAL.location = location;
  update(OUT, sym, errorAL);
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
// SE->symbol() is known to be nil/non-nil. If it comes from a MOVE or CAST,
// propagate to their arguments.
// For efficiency, the caller should assure isSymbolAnalyzed(SE).
//
// Returns true and skips updating 'OUT' if the branch in question is unreachable.
//
static bool adjustTestArgChain(SymExpr* SE, bool isAllocated, AliasMap& OUT) {
  SymExpr* curr = SE;
  // Loop over the definitions for the temps.
  while (true) {
    AliasMap::const_iterator it = OUT.find(curr->symbol());
    if (it != OUT.end()) {
      AliasLocation loc = it->second;
      if (loc.type == MUST_ALIAS_REFVAR) {
        Symbol* sym = toSymbol(loc.location);
        AliasMap::const_iterator it2 = OUT.find(sym);
        if (it2 != OUT.end()) {
          loc = it2->second;
        }
      }

      if (( isAllocated && loc.type == MUST_ALIAS_NIL)        ||
          (!isAllocated && loc.type == MUST_ALIAS_ALLOCATED) )
        // We are testing ==nil on something that's not nil, or visa versa.
        return true;
    }

    // Not worth updating OUT if 'curr' is the only use (a common case?).
    if (curr->symbol()->getSingleUse() != curr)
      updateForConditional(curr->symbol(), isAllocated, curr, OUT);

    if (! curr->symbol()->hasFlag(FLAG_TEMP))
      // Stop following the use->def chain to safeguard against corner cases
      // of user var being concurrently modified after being read into 'curr'.
      break;

    if (CallExpr* CE = toCallExpr(getSingleDefExpr(curr)))
      if (CE->isPrimitive(PRIM_MOVE)   ||
          CE->isPrimitive(PRIM_ASSIGN) ||
          CE->isPrimitive(PRIM_CAST)   )
        if ((curr = toSymExpr(CE->get(2))))
          continue;

    // None of the above cases - do not continue.
    break;
  }
  return false;
}

//
// Is 'expr' a SymExpr for 'nil' ?
//
// We are not testing simply "expr->getValType() == dtNil"
// to guard against assuming v==nil inside BLOCK in this corner case:
//   proc f(ref arg) { arg = new MyClass(); return nil; }
//   var v = nil;
//   if v == f(v) then BLOCK;
// AST:
//   tmp1 = cast v: object  // tmp1=nil
//   tmp2 = f(v)            // tmp2=nil, v=new MyClass
//   tmp3 = tmp1 == tmp2    // true
//   if tmp3 then ...       // 'v' is not nil here
//
static bool isNilSymExpr(Expr* expr) {
  if (SymExpr* se = toSymExpr(expr))
    if (se->symbol() == gNil)
      return true;
  return false;
}

// Helper for tests like ==(arg1,nil) and !=(nil,arg2).
// Bonus todo: handle ==(arg1,arg2) when arg2 is in the OUT map
// and !=(arg1,arg2) when arg2 is known to be nil.
// Returns true if the branch in question is unreachable.
static bool adjustWhenNilCmp(Expr* arg1, Expr* arg2,
                             bool isAllocated, AliasMap& OUT) {
  if (isNilSymExpr(arg1))
   if (SymExpr* SE2 = toSymExpr(arg2))
    if (isClassIshType(SE2->symbol())) // just in case
     return adjustTestArgChain(SE2, isAllocated, OUT);

  if (isNilSymExpr(arg2)) {
   if (SymExpr* SE1 = toSymExpr(arg1))
    if (isClassIshType(SE1->symbol()))
     return adjustTestArgChain(SE1, isAllocated, OUT);
  }

  return false;
}

//
// Handle a CondStmt that looks like this:
//   move temp, _cond_test(something)
//   if temp then ... else ...
// where 'something' is:
//   * a class
//   * a result of a == or != involving classes and/or nil
//
// Returns true if the branch in question is unreachable.
//
static bool doAdjustForConditional(CondStmt* cond, bool inThenBranch,
                                   AliasMap& OUT) {

  Expr* theCond = skip_cond_test(getSingleDefExpr(cond->condExpr));
  if (SymExpr* testArg = toSymExpr(theCond))
    {
     if (isClassIshType(testArg->symbol()))
      // in Chapel: if obj then ...
      return adjustTestArgChain(testArg, inThenBranch, OUT);

     if (is_bool_type(testArg->getValType()))
      // look for ==(t,nil), !=(nil,t), etc.
      if (CallExpr* TE = toCallExpr(getSingleDefExpr(testArg)))
      {
       // If 'nil' becomes a param-like literal, it will be tricky
       // to figure out that == or != compare against it.
       if (TE->isNamed("!=") && TE->numActuals() == 2)
         return adjustWhenNilCmp(TE->get(1), TE->get(2), inThenBranch, OUT);

       if (TE->isNamed("==") && TE->numActuals() == 2)
         return adjustWhenNilCmp(TE->get(1), TE->get(2), !inThenBranch, OUT);
      }
    }

  return false;
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
        updateForConditional(errorSym, inThenBranch, cond->condExpr, OUT);
      }
    }
  }
}

// Update 'OUT' based on being inside a conditional,
// if 'bb' is the starting BasicBlock of the then- or else- branch.
// Returns true if 'bb' is unreachable.
static bool adjustMapForConditional(BasicBlock* bb, AliasMap& OUT,
                                    bool trace) {
  if (bb->ins.size() != 1 || bb->exprs.size() == 0)
    return false; // quick check says we are not at start of a cond branch

  CondStmt* parentCond = NULL;
  bool    inThenBranch = true;
  if (!atStartOfCondBranch(bb->exprs[0], bb->ins[0], parentCond, inThenBranch))
    return false; // not at start of a cond branch for sure

  AliasMap* traceCopy = NULL;
  if (trace) traceCopy = new AliasMap(OUT);

  bool unreachable = doAdjustForConditional(parentCond, inThenBranch, OUT);
  adjustMapForCatchBlock(parentCond, inThenBranch, OUT);

  if (trace) {
    if (unreachable) {
      printf("in unreachable %s-branch of conditional %d\n",
             inThenBranch ? "then" : "else", parentCond->id);
    } else {
      printf("in %s-branch of conditional %d\n",
             inThenBranch ? "then" : "else", parentCond->id);
      printAliasMap("++", OUT, traceCopy);
    }
    delete traceCopy;
  }

  return unreachable;
}

static void checkBasicBlock(
    FnSymbol* fn,
    BasicBlock* bb,
    const std::vector<Symbol*> symbols,
    const AliasMap& IN,
    AliasMap& OUT,
    bool debugging,
    bool raiseErrors) {

  OUT = IN;
  bool trace = debugging;

  bool unreachable = adjustMapForConditional(bb, OUT, trace);
  if (unreachable) {
    if (trace) printf("skipping unreachable block\n");
    OUT.clear(); // combine() will use the other AliasMap unchanged
    return;
  }

  for_vector(Expr, expr, bb->exprs) {

    bool storeToUnknown = false;
    bool storeToGlobals = false;
    AliasMap* traceCopy = NULL;

    // Ignore end-of-statement calls
    if (CallExpr* call = toCallExpr(expr))
      if (call->isPrimitive(PRIM_END_OF_STATEMENT))
        continue;
    // Ignore DefExprs -- instead, find the initialization of the var
    if (isDefExpr(expr))
      continue;

    if (trace) {
      traceCopy = new AliasMap(OUT);

      printf("\nvisiting:");
      nprint_view(expr);

      printAliasMap("bb", OUT);
    }

    if (expr->id == debugNilsForId)
      gdbShouldBreakHere();

    if (ForallStmt* forall = toForallStmt(expr)) {

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
          if (storeToUnknown || isOuterVar(sym, fn)) {
            Type* t = sym->getValType();
            if (OUT[sym].type == MUST_ALIAS_DEAD ||
                (OUT[sym].type == MUST_ALIAS_NIL && isNonNilableType(t))) {
              // Don't reset it - using a dead variable or a non-nilable
              // class storing nil is a type system error. Setting it back
              // to a value once it was nil is still an error.
            } else {
              OUT[sym] = unknownAliasLocation();
            }
          }
        }
      }
    }

    if (trace) {
      printAliasMap("+-", OUT, traceCopy);
      delete traceCopy;
    }
  }
}

// todo - just check if there are any symbols to track
static void gatherVariablesToCheck(FnSymbol* fn,
                                   std::vector<Symbol*>& idxToSym,
                                   bool debugging) {

  // Gather the variables to consider
  std::map<Symbol*, int> symToIdx;

  int index = 0;
  std::vector<SymExpr*> symExprs;
  collectLcnSymExprs(fn, symExprs);
  for_vector(SymExpr, se, symExprs) {
      Symbol* sym = se->symbol();
      if (isSymbolAnalyzed(sym)) {
        if (symToIdx.count(sym) == 0) {
          // Add it to the map/array
          symToIdx.insert(std::pair<Symbol*, int>(sym, index));
          idxToSym.push_back(sym);
          INT_ASSERT((int)(idxToSym.size()-1) == index);
          index++;
          if (debugging) printf("tracking %i %s\n", sym->id, sym->name);
        }
      }
  }
}

//
// Sets the formals, including ref intents, initially to "unknown".
// For formals of non-nilable types, set them to "allocated".
//
static void setupInitialMap(FnSymbol* fn, std::vector<AliasMap>& INs,
                            std::vector<int> forwardOrder, bool debugging) {
  AliasMap& INini = INs[forwardOrder[0]];
  for_formals(formal, fn) {
    if (isSymbolAnalyzed(formal)) {
      AliasLocation loc;
      loc.type = isNonNilableType(formal->getValType())
                 ? MUST_ALIAS_ALLOCATED : MUST_ALIAS_UNKNOWN;
      loc.location = formal->defPoint;
      INini[formal] = loc;
      if (debugging) printAliasEntry("formal", formal, loc);
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
          (was.location == loc.location ||
           was.type != MUST_ALIAS_REFVAR)) {
        // OK, do nothing
        // only difference is reason for nil
      } else if (was.type == MUST_ALIAS_IGNORED) {
        if (debugging) {
          printf("ignored here -> get from bb %i\n", (int) bb->id);
          printAliasEntry("   ", sym, loc);
        }
        nextIn[sym] = loc;
        changed = true;
      } else if (was.type == MUST_ALIAS_DEAD || loc.type == MUST_ALIAS_DEAD) {
        // One branch in makes it dead -> keep it dead
        if (was.type != MUST_ALIAS_DEAD) {
          nextIn[sym] = loc;
          changed = true;
        }
        nextIn[sym] = unknownAliasLocation();
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

static void findNilDereferencesInFn(FnSymbol* fn) {

  if (fn->hasFlag(FLAG_UNSAFE))
    return;
  if (fn == initStringLiterals)
    return;
  if (fn->hasFlag(FLAG_INIT_COPY_FN) ||
      fn->hasFlag(FLAG_AUTO_COPY_FN) ||
      fn->hasFlag(FLAG_AUTO_DESTROY_FN) ||
      fn->hasFlag(FLAG_UNSAFE) ||
      fn->hasFlag(FLAG_IGNORE_TRANSFER_ERRORS) ||
      fn->hasFlag(FLAG_ERRONEOUS_COPY) ||
      fn->hasFlag(FLAG_BUILD_TUPLE) ||
      fn->hasFlag(FLAG_INIT_TUPLE))
    return;

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

  setupInitialMap(fn, IN, forwardOrder, debugging);

  // these arrays help track which basic blocks need visiting
  std::vector<int> visit(nbbs);
  std::vector<int> changed(nbbs);
  for (size_t i = 0; i < nbbs; i++) {
    visit[i] = true;
    changed[i] = false;
  }

  bool anychanged = true;

  while (anychanged) {
    if (debugging) printf("\n""In fixed point loop for %s()\n", fn->name);

    for (size_t i = 0; i < nbbs; i++) {
      changed[i] = false;
    }

    for (size_t ii = 0; ii < nbbs; ii++) {
      int i = forwardOrder[ii]; // visit basic block i
      if (visit[i]) {
        BasicBlock* bb = (*fn->basicBlocks)[i];
        if (debugging) printf("\n""Running checkBasicBlock on bb %i\n", i);

        checkBasicBlock(fn, bb, idxToSym, IN[i], OUT[i],
                        debugging, /*raise errors?*/ false);

        if (debugging) printf("\n""Combining into successors of bb %i\n", i);

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
    if (debugging) printf("\n""Raising errors on bb %i\n", (int)i);

    checkBasicBlock(fn, (*fn->basicBlocks)[i], idxToSym, IN[i], OUT[i],
                    debugging, /*raise errors?*/ true);
  }
}

void checkNilDereferencesInFn(FnSymbol* fn) {
  if (fCompileTimeNilChecking) {
    findNilDereferencesInFn(fn);
    findNonNilableStoringNil(fn);
  }
}

void adjustSignatureForNilChecking(FnSymbol* fn) {
  if (fn->_this != NULL) {
    if (fn->hasFlag(FLAG_NIL_FROM_THIS))
      fn->_this->addFlag(FLAG_NIL_FROM_ARG);
    if (fn->hasFlag(FLAG_LEAVES_THIS_NIL))
      fn->_this->addFlag(FLAG_LEAVES_ARG_NIL);
  }

  // adjust compiler-generated = and init= for types containing owned
  if (fn->name == astrSassign &&
      fn->hasFlag(FLAG_COMPILER_GENERATED) &&
      fn->numFormals() >= 1) {
    ArgSymbol* rhs = fn->getFormal(2);
    Type* t = rhs->getValType();
    if (t->symbol->hasFlag(FLAG_COPY_MUTATES))
      rhs->addFlag(FLAG_LEAVES_ARG_NIL);
  } else if (fn->name == astrInitEquals &&
             fn->hasFlag(FLAG_COMPILER_GENERATED) &&
             fn->numFormals() >= 2) {
    ArgSymbol* rhs = fn->getFormal(2);
    Type* t = rhs->getValType();
    if (t->symbol->hasFlag(FLAG_COPY_MUTATES))
      rhs->addFlag(FLAG_LEAVES_ARG_NIL);
  }

  bool leavesAnyArgNil = false;
  for_formals(formal, fn) {
    if (formal->hasFlag(FLAG_LEAVES_ARG_NIL))
      leavesAnyArgNil = true;
  }
  if (leavesAnyArgNil)
    fn->addFlag(FLAG_LEAVES_ARG_NIL);
}

typedef std::map<Symbol*,Expr*> SymbolToNilMap;

class FindInvalidNonNilables : public AstVisitorTraverse {
  public:
    // key - a variable of interest
    // value - NULL if that variable isn't possibly nil now
    //         an Expr* setting it to nil if it is possibly nil now
    SymbolToNilMap varsToNil;
    // Only present errors once per symbol
    std::set<Symbol*> erroredSymbols;
    virtual bool enterDefExpr(DefExpr* def);
    virtual bool enterCallExpr(CallExpr* call);
    virtual void exitCallExpr(CallExpr* call);
    virtual void visitSymExpr(SymExpr* se);
};

static bool isNonNilableTypeOrRecordContaining(Type* t) {
  if (isNonNilableType(t))
    return true;

  if (isRecord(t)) {
    AggregateType* at = toAggregateType(t);
    for_fields(field, at) {
      if (isNonNilableTypeOrRecordContaining(field->type))
        return true;
    }
  }

  return false;
}

static bool isNonNilableVariable(Symbol* sym) {
  return (isVarSymbol(sym) || isArgSymbol(sym)) &&
         isNonNilableTypeOrRecordContaining(sym->getValType());
}

static bool isTrackedNonNilableVariable(Symbol* sym) {
  if (isNonNilableVariable(sym)) {
    return true;
  }

  return false;
}

bool FindInvalidNonNilables::enterDefExpr(DefExpr* def) {

  Symbol* sym = def->sym;

  if (isTrackedNonNilableVariable(sym)) {
    if (varsToNil.count(sym) == 0) {
      // Assume that the variable is not nil at initialization-time.
      varsToNil[sym] = NULL;
    }
  }

  return true;
}

bool FindInvalidNonNilables::enterCallExpr(CallExpr* call) {
  if (FnSymbol* calledFn = call->resolvedOrVirtualFunction()) {
    if (isTaskFun(calledFn)) {
      calledFn->body->accept(this);
      return false;
    }
  }
  return true;
}

void FindInvalidNonNilables::exitCallExpr(CallExpr* call) {
  if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
    // handle some compiler-added temps
    SymExpr* lhsSe = toSymExpr(call->get(1));
    Symbol* lhs = lhsSe->symbol();
    if (SymExpr* rhsSe = toSymExpr(call->get(2))) {
      Symbol* rhs = rhsSe->symbol();
      if (isTrackedNonNilableVariable(lhs) && isTrackedNonNilableVariable(rhs))
        if (varsToNil[rhs] != NULL)
          varsToNil[lhs] = call;
    }
  } else if (call->resolvedOrVirtualFunction() != NULL) {
    for_formals_actuals(formal, actual, call) {
      SymExpr* actualSe = toSymExpr(actual);
      Symbol* actualSym = actualSe->symbol();
      if (isNonNilableVariable(actualSym) &&
          ! actualSym->hasFlag(FLAG_UNSAFE) ) {
        if (formal->hasFlag(FLAG_LEAVES_ARG_NIL)) {
          if (isTrackedNonNilableVariable(actualSym) &&
              varsToNil.count(actualSym) != 0) {
            varsToNil[actualSym] = call;
          } else if (actualSym->hasFlag(FLAG_TEMP) && !actualSym->isRef()) {
            // No error for now for value temps.
          } else {
            Expr* astPoint = findLocationIgnoringInternalInlining(call);
            FnSymbol* inFn = astPoint->getFunction();
            astlocT point = astPoint->astloc;
            if (inFn->userInstantiationPointLoc.filename != NULL)
              point = inFn->userInstantiationPointLoc;

            const char* error = NULL;
            if (isArgSymbol(actualSym)) {
              // leaves-arg-nil OK for e.g. owned initCopy/init= etc.
              if (actualSym->hasFlag(FLAG_LEAVES_ARG_NIL) == false)
                error = "Cannot transfer ownership from non-nilable reference argument";
            } else if (isOuterVar(actualSym, call->getFunction())) {
              error = "Cannot transfer ownership from a non-nilable outer variable";
            } else if (!actualSym->hasFlag(FLAG_DEAD_LAST_MENTION)) {
              error = "Cannot transfer ownership from a non-nilable variable with a potentially captured alias";
            } else {
              error = "Cannot transfer ownership from this non-nilable variable";
            }

            if (error != NULL && erroredSymbols.count(actualSym) == 0) {
              erroredSymbols.insert(actualSym);
              if (printsUserLocation(astPoint))
                USR_FATAL_CONT(astPoint, "%s", error);
              else
                USR_FATAL_CONT(point, "%s", error);
            }
          }
        }
      }
    }
  }
}

void FindInvalidNonNilables::visitSymExpr(SymExpr* se) {
  Symbol* sym = se->symbol();
  if (isNonNilableVariable(sym)) {
    if (Expr* e = varsToNil[sym]) {
      bool error = true;
      // Don't worry about a PRIM_END_OF_STATEMENT if it follows the
      // expression
      // Don't worry about autoDestroy calls
      if (CallExpr* parentCall = toCallExpr(se->getStmtExpr())) {
        if (parentCall->isPrimitive(PRIM_END_OF_STATEMENT)) {
          error = false;
          for (Expr* cur = e; cur != NULL; cur = cur->next) {
            if (CallExpr* curCall = toCallExpr(cur)) {
              if (curCall->isPrimitive(PRIM_END_OF_STATEMENT)) {
                if (curCall != parentCall)
                  error = true;
                break;
              }
            }
          }
        } else if (FnSymbol* calledFn = parentCall->resolvedFunction()) {
          if (calledFn->hasFlag(FLAG_AUTO_DESTROY_FN))
            error = false;
        }
      }

      if (error && erroredSymbols.count(sym) == 0) {
        erroredSymbols.insert(sym);
        USR_FATAL_CONT(se, "mention of non-nilable variable after ownership is transferred out of it");
        USR_PRINT(e, "ownership transfer occurred here");
      }
    }
  }
}

static void findNonNilableStoringNil(FnSymbol* fn) {
  // don't check special functions (owned/shared etc need to write these)
  if (fn->hasFlag(FLAG_INIT_COPY_FN) ||
      fn->hasFlag(FLAG_AUTO_COPY_FN) ||
      fn->hasFlag(FLAG_AUTO_DESTROY_FN) ||
      fn->hasFlag(FLAG_UNSAFE) ||
      fn->hasFlag(FLAG_IGNORE_TRANSFER_ERRORS) ||
      fn->hasFlag(FLAG_ERRONEOUS_COPY) ||
      fn->hasFlag(FLAG_BUILD_TUPLE) ||
      fn->hasFlag(FLAG_INIT_TUPLE))
    return;

  // don't check inside functions already saying they leave an argument nil
  // (we could.. but we'd have to track through references etc, and
  //  these are by definition internal)
  if (fn->hasFlag(FLAG_LEAVES_ARG_NIL))
    return;

  // We'll check task functions while traversing the rest
  if (isTaskFun(fn))
    return;

  FindInvalidNonNilables visitor;
  fn->body->accept(&visitor);
}
