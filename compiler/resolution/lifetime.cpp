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

#include "lifetime.h"

// TODO - prune these
#include "AstVisitorTraverse.h"
#include "astutil.h"
#include "driver.h"
#include "expr.h"
#include "ForLoop.h"
#include "iterator.h"
#include "loopDetails.h"
#include "postFold.h"
#include "resolution.h"
#include "stlUtil.h"
#include "stmt.h"
#include "symbol.h"
#include "view.h"
#include "wellknown.h"

/* This file implements lifetime checking.

   TODO: define lifetime and reachability.

   It includes some lifetime propagation. Lifetimes are only
   stored for symbols when they are non-obvious (i.e. they differ
   from the reachability of the symbol).

   * pointer/ref arguments are inferred to be return scope
   * A value containing indirections and with a non-infinite inferred
     lifetime cannot be returned from a function
        - TODO -- what about a locally declared record that is returned?
   * A value containining indirections cannot be set to a
     value with a longer lifetime
   * A scope value cannot be reachable after its lifetime ends

   * Lifetime inference assigns the minimum lifetime assigned
     to a scope variable as its lifetime.

   * records are subject to lifetime checking if they contain
     fields that are subject to lifetime checking.

   * Assumption: = operator overloads for records or classes
     handle lifetimes as if by-value. But note, ownership
     operations with Shared or Owned records count as "by value"
     here.


   TODO:
     - investigate if this can do escape checking for begins
     - decide on how to declare an owning vs borrowed class instance ptr
     - decide on how to declare ownership transfer arg vs
       borrowing arg
     - Should default for a method be borrowing from 'this'
       (vs arguments?)
     - improve error messages for the case in which a
       function calls another function that needs an annotation
     - check return scope better (there can only be one
       return scope argument per function? in that event, that's
       the only argument marked with return scope?)
     - tidy up implementation
 */

const char* debugLifetimesForFn = "";
const int debugLifetimesForId = 0;
const bool defaultToCheckingLifetimes = true;
const bool debugOutputOnError = false;

namespace {

  struct Lifetime {
    // if non-NULL, the scope of this symbol represents the lifetime
    Symbol* fromSymbolReachability;
    // relevant expression for this lifetime, for errors
    Expr* relevantExpr;
    // is this lifetime infinite?
    bool infinite;
    // is this lifetime return scope - i.e. allowing it to be returned?
    bool returnScope;
  };

  struct ScopeLifetime {
    // referent lifetime is the scope of what a ref variable might refer to
    Lifetime referent;
    // borrowed lifetime is the scope of what borrowed class instances
    // might refer to. This might be longer than the referent lifetime.
    Lifetime borrowed;
  };

  typedef std::map<Symbol*,ScopeLifetime> SymbolToLifetimeMap;
  typedef std::map<CallExpr*,ScopeLifetime> ReturnToLifetimeMap;

  struct LifetimeState {
    SymbolToLifetimeMap lifetimes;
    bool setLifetimeForSymbolToMin(Symbol* sym, ScopeLifetime lt);
    ScopeLifetime lifetimeForSymbol(Symbol* sym);
    ScopeLifetime lifetimeForActual(Symbol* sym);
    ScopeLifetime lifetimeForCallReturn(CallExpr* call);
    ScopeLifetime lifetimeForPrimitiveReturn(CallExpr* call, bool lhsIsRef);
  };

  class InferLifetimesVisitor : public AstVisitorTraverse {

    public:
      LifetimeState* lifetimes;
      bool changed;
      virtual bool enterCallExpr(CallExpr* call);
      virtual bool enterForLoop(ForLoop* forLoop);
  };
  class EmitLifetimeErrorsVisitor : public AstVisitorTraverse {

    public:
      LifetimeState* lifetimes;
      virtual bool enterCallExpr (CallExpr* call);
      void emitErrors();
  };

} /* end anon namespace */

static bool isLocalVariable(FnSymbol* fn, Symbol* sym);
static bool typeHasInfiniteLifetime(Type* type);
static bool isSubjectToRefLifetimeAnalysis(Symbol* sym);
static bool isSubjectToBorrowLifetimeAnalysis(Type* type);
static bool isSubjectToBorrowLifetimeAnalysis(Symbol* sym);
static bool recordContainsClassFields(AggregateType* at);
static bool recordContainsOwnedClassFields(AggregateType* at);
static bool recordContainsBorrowedClassFields(AggregateType* at);
static bool containsBorrowedClass(Type* type);
static bool containsOwnedClass(Type* type);
static bool shouldPropagateLifetimeTo(CallExpr* call, Symbol* sym);
static bool isAnalyzedMoveOrAssignment(CallExpr* call);
static bool symbolHasInfiniteLifetime(Symbol* sym);
static BlockStmt* getDefBlock(Symbol* sym);
static bool isBlockWithinBlock(BlockStmt* a, BlockStmt* b);
static bool isLifetimeShorter(Lifetime a, Lifetime b);
static Lifetime minimumLifetime(Lifetime a, Lifetime b);
static ScopeLifetime minimumScopeLifetime(ScopeLifetime a, ScopeLifetime b);
static Lifetime reachabilityLifetimeForSymbol(Symbol* sym);
//static ScopeLifetime reachabilityScopeLifetimeForSymbol(Symbol* sym);
static Lifetime infiniteLifetime();
static ScopeLifetime infiniteScopeLifetime();
static bool debuggingLifetimesForFn(FnSymbol* fn);
void printLifetime(Lifetime lt);
void printScopeLifetime(ScopeLifetime lt);
void printLifetimeState(LifetimeState* state);
static void handleDebugOutputOnError(Expr* e, LifetimeState* state);
static bool shouldCheckLifetimesInFn(FnSymbol* fn);

void checkLifetimes(void) {

  forv_Vec(FnSymbol, fn, gFnSymbols) {

    if (shouldCheckLifetimesInFn(fn)) {
      bool debugging = debuggingLifetimesForFn(fn);

      if (debugging) {
        printf("Visiting function %s id %i\n", fn->name, fn->id);
        nprint_view(fn);
        gdbShouldBreakHere();
      }

      LifetimeState state;

      // Infer lifetimes
      InferLifetimesVisitor infer;
      infer.lifetimes = &state;
      // Find minimum lifetime for all variables
      // Uses repeated iteration in order to avoid problems
      // such as where a lifetime for a temporary is incorrectly inferred
      // to be infinite, because only settings of a variable have
      // been infinite so far.
      do {
        infer.changed = false;
        fn->accept(&infer);
      } while (infer.changed == true);

      if (debugging) {
        printLifetimeState(&state);
      }

      // Emit errors
      EmitLifetimeErrorsVisitor emit;
      emit.lifetimes = &state;
      fn->accept(&emit);
      emit.emitErrors();
    }
  }
}

static bool shouldCheckLifetimesInFn(FnSymbol* fn) {
  if (fn->hasFlag(FLAG_UNSAFE))
    return false;
  if (fn->hasFlag(FLAG_SAFE))
    return true;
  if (fn->hasFlag(FLAG_COMPILER_GENERATED))
    return false;

  // this is a workaround for problems with init functions
  // where temporaries are used to store results that are
  // move'd into global variables.
  // It could conceivably also/alternatively check for
  // a lack of FLAG_INSERT_AUTO_DESTROY on the symbol
  // (but then the workaround would need to be elsewhere).
  if (fn->hasFlag(FLAG_MODULE_INIT))
    return false;


  ModuleSymbol* inMod = fn->getModule();
  if (inMod->hasFlag(FLAG_UNSAFE))
    return false;
  if (inMod->hasFlag(FLAG_SAFE))
    return true;


  return defaultToCheckingLifetimes;
}

static bool debuggingLifetimesForFn(FnSymbol* fn)
{
  if (!fn) return false;
  if (fn->id == debugLifetimesForId) return true;
  if (0 == strcmp(debugLifetimesForFn, fn->name)) return true;
  return false;
}

void printLifetime(Lifetime lt) {
  if (lt.infinite)
    printf("infinite ");
  if (lt.returnScope)
    printf("return_scope ");
  if (lt.fromSymbolReachability)
    printf("like %s[%i] ", lt.fromSymbolReachability->name, lt.fromSymbolReachability->id);
  if (lt.relevantExpr)
    printf("expr %i ", lt.relevantExpr->id);
}

void printScopeLifetime(ScopeLifetime lt) {
  printf("ref lifetime ");
  printLifetime(lt.referent);
  printf("borrow lifetime ");
  printLifetime(lt.borrowed);
  printf("\n");
}

void printLifetimeState(LifetimeState* state)
{
  printf("Lifetime state:\n");
  for (SymbolToLifetimeMap::iterator it = state->lifetimes.begin();
       it != state->lifetimes.end();
       ++it) {
    Symbol* key = it->first;
    ScopeLifetime value = it->second;

    printf("Symbol %s[%i] has lifetimes: ", key->name, key->id);
    printScopeLifetime(value);
  }
}

static void handleDebugOutputOnError(Expr* e, LifetimeState* state) {
  if (debugOutputOnError) {
    printf("Stopping due to debugOutputOnError\n");
    printf("Function is:\n");
    nprint_view(e->getFunction());
    printf("Expr is:\n");
    nprint_view(e);
    printf("Lifetime state is:\n");
    printLifetimeState(state);
    USR_STOP();
  }
}

// Sets lifetime for symbol sym to be lt if it's not in the table yet,
// or the minimum of the previous value and lt if it is.
//
// Returns true if the state was changed.
bool LifetimeState::setLifetimeForSymbolToMin(Symbol* sym, ScopeLifetime lt) {
  bool changed = false;

  int breakOnId = debugLifetimesForId;

  // Don't add it if there's no new information

  bool leaveOutBorrow = false;
  Lifetime reach = reachabilityLifetimeForSymbol(sym);

  // Leave out the symbol update if the update doesn't matter
  // This prevents errors based upon "scoped variables" when the
  // variable has no meaningful scope.
  // It would be reasonable to update callers to avoid this situation, too.
  if (!sym->isRef())
    lt.referent = infiniteLifetime();
  if (sym->isRef() &&
      (lt.referent.fromSymbolReachability == reach.fromSymbolReachability ||
       isLifetimeShorter(reach, lt.referent)))
    lt.referent = infiniteLifetime();

  if (!sym->isRef() &&
      containsOwnedClass(sym->type) &&
      (lt.borrowed.fromSymbolReachability ==
       reach.fromSymbolReachability ||
       isLifetimeShorter(reach, lt.borrowed)))
    leaveOutBorrow = true;

  if (lt.referent.infinite && leaveOutBorrow)
    return false;

  if (lifetimes.count(sym) == 0) {
    if (sym->id == breakOnId)
      gdbShouldBreakHere();

    lifetimes[sym] = lt;
    changed = true;
  } else {
    ScopeLifetime & value = lifetimes[sym];
    // This is just value = minimumScopeLifetime(value, lt)
    // with debugging code and change tracking.
    if (isLifetimeShorter(lt.referent, value.referent)) {
      if (sym->id == breakOnId)
        gdbShouldBreakHere();
      value.referent = lt.referent;
      changed = true;
    }
    if (isLifetimeShorter(lt.referent, value.referent)) {
      if (sym->id == breakOnId)
        gdbShouldBreakHere();
      value.referent = lt.referent;
      changed = true;
    }
  }

  return changed;
}

ScopeLifetime LifetimeState::lifetimeForSymbol(Symbol* sym) {

  ScopeLifetime ret = infiniteScopeLifetime();

  if (lifetimes.count(sym) > 0) {
    ret = lifetimes[sym];
  } else if (typeHasInfiniteLifetime(sym->type)) {
    // leave it infinite
  } else {
    // Otherwise, create a basic lifetime for this variable.
    //ret = reachabilityScopeLifetimeForSymbol(sym);
    // ret = infiniteScopeLifetime();
    // Leave it infinite for non-args
    // inference will sort it out based on assignments.

    // Adjust the returnScope field of these lifetimes.
    if (isArgSymbol(sym)) {
      Lifetime lt = reachabilityLifetimeForSymbol(sym);
      ret.referent = lt;
      ret.borrowed = lt;
      if (sym->isRef())
        ret.referent.returnScope = true;

      if (isSubjectToBorrowLifetimeAnalysis(sym->type))
        ret.borrowed.returnScope = true;
    }
  }

  /*
  if (!sym->isRef()) {
    // Make sure that the referent part is accurate
    //ret.referent = reachabilityLifetimeForSymbol(sym);
    ret.referent = infiniteScopeLifetime();
  }*/


  return ret;
}

// Normalizes the lifetime to the reachability as appropriate
// for cases such as getting the lifetime of a local variable.
// This doesn't always occur though, in the inference, so that
// a variable containing a borrow can have a borrow lifetime greater
// than its reachability.
ScopeLifetime LifetimeState::lifetimeForActual(Symbol* actualSym) {
  ScopeLifetime ret = lifetimeForSymbol(actualSym);

  // At a minimum, non-ref actuals should have lifetime = reachability
  if (!actualSym->isRef()) {
    Lifetime reachability = reachabilityLifetimeForSymbol(actualSym);
    if (isLifetimeShorter(reachability, ret.referent))
      ret.referent = reachability;
  }

  // At a minimum, a potential borrow from a record containing owned
  // has lifetime = reachibility.
  if (!actualSym->isRef() && containsOwnedClass(actualSym->type)) {
    Lifetime reachability = reachabilityLifetimeForSymbol(actualSym);
    if (isLifetimeShorter(reachability, ret.borrowed))
      ret.borrowed = reachability;
  }

  return ret;
}

ScopeLifetime LifetimeState::lifetimeForCallReturn(CallExpr* call) {

  FnSymbol* calledFn = call->resolvedOrVirtualFunction();
  if (calledFn->hasFlag(FLAG_RETURNS_INFINITE_LIFETIME))
    return infiniteScopeLifetime();

  // Class constructors and "_new" calls
  // return infinite lifetime. Why?
  //  * the result of 'new' is currently user-managed
  //  * ref fields are not supported in classes
  //    so they can't capture a ref argument
  if (isClass(call->typeInfo()) &&
      (calledFn->hasFlag(FLAG_CONSTRUCTOR) ||
       0 == strcmp("_new", calledFn->name))) {
    return infiniteScopeLifetime();
  }

  ScopeLifetime minLifetime = infiniteScopeLifetime();


  bool returnsRef = call->isRef() ||
                    calledFn->retTag == RET_REF ||
                    calledFn->retTag == RET_CONST_REF;

  bool returnsBorrow = false;
  Type* returnType = calledFn->retType;
  if (calledFn->isMethod() && 0 == strcmp("init", calledFn->name)) {
    returnType = calledFn->getFormal(1)->getValType();
  }
  returnsBorrow = isSubjectToBorrowLifetimeAnalysis(returnType);
  //&& containsBorrowedClass(returnType);

  /*bool containsOwned = false;
  if (AggregateType* at = toAggregateType(returnType))
    containsOwned = recordContainsOwnedClassFields(at);
   */

  for_formals_actuals(formal, actual, call) {
    SymExpr* actualSe = toSymExpr(actual);
    INT_ASSERT(actualSe);
    Symbol* actualSym = actualSe->symbol();

    ScopeLifetime argLifetime = infiniteScopeLifetime();

    if (returnsRef && formal->isRef() &&
        (isSubjectToRefLifetimeAnalysis(actualSym) ||
         isLocalVariable(call->getFunction(), actualSym)) &&
        // in, out arguments don't determine the lifetime of
        // a returned variable
        !((formal->originalIntent & INTENT_FLAG_IN) ||
          (formal->originalIntent & INTENT_FLAG_OUT))) {

      // Use the referent part of the actual's lifetime
      ScopeLifetime temp = lifetimeForActual(actualSym);
      argLifetime.referent = temp.referent;
    }

    if (returnsBorrow && isSubjectToBorrowLifetimeAnalysis(actualSym)) {
      // Use the borrowed part of the actual's lifetime
      ScopeLifetime temp = lifetimeForActual(actualSym);
      argLifetime.borrowed = temp.borrowed;
    }

    if (formal->hasFlag(FLAG_RETURN_SCOPE))
      return argLifetime;

    if (calledFn->hasFlag(FLAG_RETURN_SCOPE_THIS) &&
        formal == calledFn->_this)
      return argLifetime;

    minLifetime = minimumScopeLifetime(minLifetime, argLifetime);
  }

  // Something returning an "owned" class pointer has a minimum scope
  // of its own reachability.
  // (I.e. it can't have infinite borrow scope, anymore, say,
  //  because a borrow from the owned object might not be valid
  //  after the owning object is destroyed)
  /*if (containsOwned) {
    ScopeLifetime temp = lifetimeForSymbol(actualSym);
    argLifetime.borrowed = temp.borrowed;
  }*/

  // If no argument was marked with FLAG_RETURN_SCOPE,
  // just assume it's the minimum of the passed lifetimes.
  return minLifetime;
}

ScopeLifetime LifetimeState::lifetimeForPrimitiveReturn(CallExpr* call, bool
    lhsIsRef) {

  bool returnsRef = call->isRef();
  bool returnsBorrow = isSubjectToBorrowLifetimeAnalysis(call->typeInfo());

  if (call->isPrimitive(PRIM_GET_MEMBER) ||
      call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
      call->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
      call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE) ||
      call->isPrimitive(PRIM_ARRAY_GET_VALUE) ||
      call->isPrimitive(PRIM_ARRAY_GET)) {
    // Lifetime of a field is the lifetime of the aggregate.
    // Don't get confused by the VarSymbol representing the field
    SymExpr* actualSe = toSymExpr(call->get(1));
    INT_ASSERT(actualSe);
    Symbol* actualSym = actualSe->symbol();
    ScopeLifetime argLifetime = infiniteScopeLifetime();

    ScopeLifetime temp = lifetimeForActual(actualSym);

    if (returnsRef &&
        (isSubjectToRefLifetimeAnalysis(actualSym) ||
         isLocalVariable(call->getFunction(), actualSym))) {
      // Use the referent part of the actual's lifetime
      argLifetime.referent = temp.referent;
    }
    if (returnsRef && isClass(actualSym->getValType())) {
      // returning a ref to a class field should make the
      // lifetime of the ref == the lifetime of the borrow
      argLifetime.referent = temp.borrowed;
    }
    if (returnsBorrow &&
        isSubjectToBorrowLifetimeAnalysis(actualSym)) {
      // Use the borrowed part of the actual's lifetime
      argLifetime.borrowed = temp.borrowed;
    }

    return argLifetime;
  }/* else if (call->isPrimitive(PRIM_SET_REFERENCE) ||
             call->isPrimitive(PRIM_ADDR_OF)) {
    // TODO - is this section necessary?
    SymExpr* se = toSymExpr(call->get(1));
    INT_ASSERT(se);
    Symbol* actualSym = se->symbol();

    return lifetimeForActual(actualSym);
  }*/

  ScopeLifetime minLifetime = infiniteScopeLifetime();

  for_actuals(actual, call) {
    SymExpr* actualSe = toSymExpr(actual);
    INT_ASSERT(actualSe);
    Symbol* actualSym = actualSe->symbol();
    ScopeLifetime argLifetime = infiniteScopeLifetime();

    if (returnsRef &&
        (isSubjectToRefLifetimeAnalysis(actualSym) ||
         isLocalVariable(call->getFunction(), actualSym))) {
      // Use the referent part of the actual's lifetime
      ScopeLifetime temp = lifetimeForActual(actualSym);
      argLifetime.referent = temp.referent;
    }
    if (returnsBorrow &&
        isSubjectToBorrowLifetimeAnalysis(actualSym)) {
      // Use the borrowed part of the actual's lifetime
      ScopeLifetime temp = lifetimeForActual(actualSym);
      argLifetime.borrowed = temp.borrowed;
    }

    minLifetime = minimumScopeLifetime(minLifetime, argLifetime);
  }

  return minLifetime;
}


static bool isRecordInitOrReturn(CallExpr* call, Symbol*& lhs, CallExpr*& initOrCtor) {

  if (call->isPrimitive(PRIM_MOVE) ||
      call->isPrimitive(PRIM_ASSIGN)) {
    if (CallExpr* rhsCallExpr = toCallExpr(call->get(2))) {
      if (/*FnSymbol* calledFn = */rhsCallExpr->resolvedOrVirtualFunction()) {
        if (AggregateType* at = toAggregateType(rhsCallExpr->typeInfo())) {
          if (isRecord(at) /*&& calledFn->hasFlag(FLAG_CONSTRUCTOR)*/) {
            SymExpr* se = toSymExpr(call->get(1));
            INT_ASSERT(se);
            lhs = se->symbol();
            initOrCtor = rhsCallExpr;
            return true;
          }
        }
      }
    }
  }

  if (FnSymbol* calledFn = call->resolvedOrVirtualFunction()) {
    if (calledFn->isMethod() && 0 == strcmp("init", calledFn->name)) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);
      Symbol* sym = se->symbol();
      if (isRecord(sym->type)) {
        lhs = sym;
        initOrCtor = call;
        return true;
      }
    }
  }

  lhs = NULL;
  initOrCtor = NULL;
  return false;
}

bool InferLifetimesVisitor::enterCallExpr(CallExpr* call) {

  if (call->id == debugLifetimesForId)
    gdbShouldBreakHere();

  Symbol* initSym = NULL;
  CallExpr* initCall = NULL;
  if (isRecordInitOrReturn(call, initSym, initCall) &&
      shouldPropagateLifetimeTo(call, initSym)) {

    AggregateType* at = toAggregateType(initSym->type);
    INT_ASSERT(at);
    ScopeLifetime lt = infiniteScopeLifetime();
    if (recordContainsBorrowedClassFields(at)) {
      lt = minimumScopeLifetime(lt, lifetimes->lifetimeForCallReturn(initCall));
    }
    // Something returning an "owned" class pointer has a minimum scope
    // of its own reachability.
    // (I.e. it can't have infinite borrow scope, anymore, say,
    //  because a borrow from the owned object might not be valid
    //  after the owning object is destroyed)
    if (recordContainsOwnedClassFields(at)) {
      ScopeLifetime temp = infiniteScopeLifetime();
      temp.borrowed = reachabilityLifetimeForSymbol(initSym);
      //lt = minimumScopeLifetime(lt, lifetimes->lifetimeForSymbol(initSym));
      lt = minimumScopeLifetime(lt, temp);
    }

    lt.borrowed.relevantExpr = call;
    changed |= lifetimes->setLifetimeForSymbolToMin(initSym, lt);

  } else if (isAnalyzedMoveOrAssignment(call)) {

    SymExpr* lhsSe = toSymExpr(call->get(1));
    Symbol* lhs = lhsSe->symbol();

    if (shouldPropagateLifetimeTo(call, lhs)) {

      // Consider the RHS and handle the cases
      Expr* rhsExpr = call->get(2);
      CallExpr* rhsCallExpr = toCallExpr(rhsExpr);
      ScopeLifetime lt = infiniteScopeLifetime();

      if (rhsCallExpr) {
        if (rhsCallExpr->resolvedOrVirtualFunction()) {
          lt = lifetimes->lifetimeForCallReturn(rhsCallExpr);
        } else {
          // Includes propagating refs across PRIM_ADDR_OF/PRIM_SET_REFERENCE
          lt = lifetimes->lifetimeForPrimitiveReturn(rhsCallExpr, lhs->isRef());
        }
      } else {
        SymExpr* se = toSymExpr(rhsExpr);
        INT_ASSERT(se);
        // TODO - this doesn't handle records using default =
        if (call->isPrimitive(PRIM_MOVE) || isClass(se->symbol()->type)) {
          // Propagate lifetime for refs across moves
          // lifetime for classes across any of the options
          lt = lifetimes->lifetimeForSymbol(se->symbol());
        }
      }

      if (lhs->isRef()) {
        // ref lifetime should only be adjusted when we set the ref
        // (vs update the pointed-to-value, as with calling operator = on it)
        if (rhsExpr->isRef() && call->isPrimitive(PRIM_MOVE))
          lt.referent.relevantExpr = call;
        else
          lt.referent = infiniteLifetime();
      } else {
        // Non-ref symbols use the symbol itself as the ref lifetime
        // (to be used in case something refers to it)
        lt.referent = reachabilityLifetimeForSymbol(lhs);
      }

      lt.borrowed.relevantExpr = call;
      changed |= lifetimes->setLifetimeForSymbolToMin(lhs, lt);
    }
  }

  return false;
}

bool InferLifetimesVisitor::enterForLoop(ForLoop* forLoop) {
  // Gather the loop details to understand the
  // correspondence between what was iterated over
  // and the index variables.

  bool isForall = false;
  IteratorDetails leaderDetails;
  ForLoop* followerForLoop = NULL;
  std::vector<IteratorDetails> detailsVector;

  gatherLoopDetails(forLoop, isForall, leaderDetails,
                    followerForLoop, detailsVector);

  for (size_t i = 0; i < detailsVector.size(); i++) {
    Expr* iterable = detailsVector[i].iterable;
    Symbol* index = detailsVector[i].index;
    SymExpr* iterableSe = toSymExpr(iterable);

    INT_ASSERT(index);

    // Also check if we are iterating using these() method
    // ex. functions/ferguson/ref-pair/const-error-iterated*
    if (!iterableSe)
      if (CallExpr* iterableCall = toCallExpr(iterable))
        if (iterableCall->isNamed("these"))
          if (iterableCall->numActuals() >= 1)
            iterableSe = toSymExpr(iterableCall->get(1));

    // Gather lifetime for iterable
    ScopeLifetime iterableLifetime = infiniteScopeLifetime();
    if (iterableSe) {
      iterableLifetime = lifetimes->lifetimeForSymbol(iterableSe->symbol());
    } else if (CallExpr* iterableCall = toCallExpr(iterable)) {
      iterableLifetime = lifetimes->lifetimeForCallReturn(iterableCall);
    } else {
      // Assume infinite lifetime, as iterable takes no arguments
      // (e.g. enum value iteration)
    }

    // Set lifetime of iteration variable to lifetime of the iterable (expr).
    iterableLifetime.referent.relevantExpr = forLoop;
    iterableLifetime.borrowed.relevantExpr = forLoop;
    changed |= lifetimes->setLifetimeForSymbolToMin(index, iterableLifetime);
  }

  return true;
}

static void emitError(Expr* inExpr,
                      const char* msg1, const char* msg2,
                      Symbol* relevantSymbol,
                      Lifetime relevantLifetime,
                      LifetimeState* lifetimes) {

  char buf[256];
  if (relevantSymbol && !relevantSymbol->hasFlag(FLAG_TEMP)) {
    snprintf(buf, sizeof(buf), "%s %s %s", msg1, relevantSymbol->name, msg2);
    USR_FATAL_CONT(inExpr, buf);
  } else {
    snprintf(buf, sizeof(buf), "%s %s", msg1, msg2);
    USR_FATAL_CONT(inExpr, buf);
  }

  Symbol* fromSym = relevantLifetime.fromSymbolReachability;
  Expr* fromExpr = relevantLifetime.relevantExpr;
  if (fromSym && !fromSym->hasFlag(FLAG_TEMP))
    USR_PRINT(fromSym, "consider scope of %s", fromSym->name);
  else if (fromExpr && inExpr->astloc != fromExpr->astloc)
    USR_PRINT(fromExpr, "consider result here");

  handleDebugOutputOnError(inExpr, lifetimes);
}


bool EmitLifetimeErrorsVisitor::enterCallExpr(CallExpr* call) {

  FnSymbol* calledFn = call->resolvedOrVirtualFunction();

  if (call->id == 410686)
    gdbShouldBreakHere();

  if (isAnalyzedMoveOrAssignment(call)) {

    SymExpr* lhsSe = toSymExpr(call->get(1));
    Symbol* lhs = lhsSe->symbol();
    ScopeLifetime lhsLt = lifetimes->lifetimeForSymbol(lhs);

    if (isSubjectToRefLifetimeAnalysis(lhs) ||
        isSubjectToBorrowLifetimeAnalysis(lhs)) {

      // If we can't make sense of the RHS, assume it's infinite lifetime
      ScopeLifetime rhsLt = infiniteScopeLifetime();

      Symbol* rhsSym = NULL;

      if (SymExpr* rhsSe = toSymExpr(call->get(2))) {
        rhsSym = rhsSe->symbol();
        if (isSubjectToRefLifetimeAnalysis(rhsSym) ||
            isSubjectToBorrowLifetimeAnalysis(rhsSym))
          rhsLt = lifetimes->lifetimeForSymbol(rhsSym);

      } else if (CallExpr* subCall = toCallExpr(call->get(2))) {
        if (subCall->resolvedOrVirtualFunction())
          rhsLt = lifetimes->lifetimeForCallReturn(subCall);
        else
          rhsLt = lifetimes->lifetimeForPrimitiveReturn(subCall, lhs->isRef());
      }

      // Raise errors for returning a scoped/lifetime'd variable
      // For the purposes of this check, it's only considered to
      // be a problem to return a variable with scope other than
      // it's own scope, since doing otherwise would disallow
      // returning any record..
      /* This has the problem that it might give a different
         result when the minimum is minimumed more
      bool rhsLtIsReachability = false;
      if (rhsSym && isRecord(rhsSym->type) &&
          rhsLt.borrowed.fromSymbolReachability == rhsSym)
        rhsLtIsReachability = true;
       */

      if (lhs->hasFlag(FLAG_RVV)) {
        if (lhs->isRef()) {
          // check returning a reference
          if (!rhsLt.referent.infinite) {
            if (!rhsLt.referent.returnScope) {
              emitError(call,
                        "Reference to scoped variable",
                        "cannot be returned",
                        rhsSym, rhsLt.referent, lifetimes);
            }
            if (calledFn &&
                calledFn->hasFlag(FLAG_RETURNS_INFINITE_LIFETIME) &&
                rhsLt.referent.returnScope) {
              emitError(call,
                        "Reference to return scoped variable",
                        "cannot be returned in function returning infinite lifetime",
                        rhsSym, rhsLt.referent, lifetimes);
            }
          }
        }

        if (isSubjectToBorrowLifetimeAnalysis(lhs) &&
            containsBorrowedClass(lhs->type)) {
          // check returning a borrow
          if (!rhsLt.borrowed.infinite /*&& !rhsLtIsReachability*/) {
            if (!rhsLt.borrowed.returnScope) {
              emitError(call,
                        "Scoped variable",
                        "cannot be returned",
                        rhsSym, rhsLt.borrowed, lifetimes);
            }
            if (calledFn &&
                calledFn->hasFlag(FLAG_RETURNS_INFINITE_LIFETIME) &&
                rhsLt.borrowed.returnScope) {
              emitError(call,
                        "Return scope variable",
                        "cannot be returned in function returning infinite lifetime",
                        rhsSym, rhsLt.borrowed, lifetimes);
            }
          }
        }
      } else {
        // For the purposes of this check, lhsLt should be no more
        // than the reachability lifetime for that symbol.
        Lifetime lhsReachability = reachabilityLifetimeForSymbol(lhs);
        Lifetime lhsReachableRefLifetime = minimumLifetime(lhsReachability,
            lhsLt.referent);
        Lifetime lhsReachableBorrowLifetime = minimumLifetime(lhsReachability, lhsLt.borrowed);

        // Raise errors for init/assigning from a value with shorter lifetime
        // I.e. insist RHS lifetime is longer than LHS lifetime.
        // I.e. error if RHS lifetime is shorter than LHS lifetime.
        if (call->isPrimitive(PRIM_MOVE) &&
            lhs->isRef() &&
            isLifetimeShorter(rhsLt.referent, lhsReachableRefLifetime)) {
          emitError(call,
                    "Reference to scoped variable",
                    "would outlive the value it refers to",
                    lhs, rhsLt.referent, lifetimes);
        }
        if (isLifetimeShorter(rhsLt.borrowed, lhsReachableBorrowLifetime)) {
          emitError(call,
                    "Scoped variable",
                    "would outlive the value it is set to",
                    lhs, rhsLt.borrowed, lifetimes);
        }
      }
    }
  }

  return false;
}


void EmitLifetimeErrorsVisitor::emitErrors() {
  for (SymbolToLifetimeMap::iterator it = lifetimes->lifetimes.begin();
       it != lifetimes->lifetimes.end();
       ++it) {
    Symbol* key = it->first;
    ScopeLifetime value = it->second;

    Lifetime reachability = reachabilityLifetimeForSymbol(key);

    // Ignore the RVV for this check
    // (see test lifetimes/tz.chpl)
    if (key->hasFlag(FLAG_RVV))
      continue;

    if (isLifetimeShorter(value.referent, reachability)) {
      Expr* at = key->defPoint;
      if (value.referent.relevantExpr)
        at = value.referent.relevantExpr;

      emitError(at,
                "Reference to scoped variable",
                "reachable after lifetime ends",
                key, value.referent, lifetimes);
    }

    if (isLifetimeShorter(value.borrowed, reachability)) {
      Expr* at = key->defPoint;
      if (value.borrowed.relevantExpr)
        at = value.borrowed.relevantExpr;

      emitError(at,
                "Scoped variable",
                "reachable after lifetime ends",
                key, value.referent, lifetimes);
    }
  }
}

/* Is the sym argument a local variable in fn?
 */
static bool isLocalVariable(FnSymbol* fn, Symbol* sym) {
  bool isValueArg = isArgSymbol(sym) && !sym->isRef();
  bool isLocal = isVarSymbol(sym) || isValueArg;

  return sym->defPoint->getFunction() == fn && isLocal;
}

static bool typeHasInfiniteLifetime(Type* type) {

  // If it has type nil, it has infinite lifetime
  if (type == dtNil)
    return true;

  // low-level ddata type not subject to analysis
  if (type->symbol->hasFlag(FLAG_DATA_CLASS))
    return true;

  // Types for C compatability are assumed to have infinite lifetime.
  if (type->symbol->hasFlag(FLAG_C_PTR_CLASS) ||
      type->symbol->hasFlag(FLAG_EXTERN) ||
      type == dtCVoidPtr ||
      type == dtCFnPtr)
    return true;

  // Locale type has infinite lifetime
  // (since locales exist for the entire program run)
  if (isSubClass(type, dtLocale))
    return true;

  return false;
}

static bool isSubjectToRefLifetimeAnalysis(Symbol* sym) {
  bool argOrVar = isArgSymbol(sym) || isVarSymbol(sym);

  if (!argOrVar)
    return false;

  if (!sym->isRef())
    return false;

  // Symbols marked "unsafe" are not subject to analysis.
  if (sym->hasFlag(FLAG_UNSAFE))
    return false;

  return true;
}

// A record containing an "owned" pointer counts
// since it needs to be subject to lifetime analysis
// for a 'borrow()' function.
// So this detects "owned" or "borrowed" class fields.
static bool recordContainsClassFields(AggregateType* at) {

  for_fields(field, at) {
    if (isClass(field->type) &&
        !typeHasInfiniteLifetime(field->type) &&
        !field->hasFlag(FLAG_UNSAFE))
      return true;

    if (isRecord(field->type) &&
        recordContainsClassFields(toAggregateType(field->type)))
      return true;
  }

  return false;
}

static bool recordContainsOwnedClassFields(AggregateType* at) {

  for_fields(field, at) {
    if (isClass(field->type) &&
        field->hasFlag(FLAG_OWNED))
      return true;

    if (isRecord(field->type) &&
        recordContainsOwnedClassFields(toAggregateType(field->type)))
      return true;
  }

  return false;
}

static bool recordContainsBorrowedClassFields(AggregateType* at) {

  for_fields(field, at) {
    if (isClass(field->type) &&
        !typeHasInfiniteLifetime(field->type) &&
        !field->hasFlag(FLAG_UNSAFE) &&
        !field->hasFlag(FLAG_OWNED))
      return true;

    if (isRecord(field->type) &&
        recordContainsBorrowedClassFields(toAggregateType(field->type)))
      return true;
  }

  return false;
}

static bool containsBorrowedClass(Type* type) {
  type = type->getValType();

  if (isClass(type) && !typeHasInfiniteLifetime(type))
    return true;

  if (isRecord(type))
    return recordContainsBorrowedClassFields(toAggregateType(type));

  return false;
}

static bool containsOwnedClass(Type* type) {
  type = type->getValType();

  if (isRecord(type))
    return recordContainsOwnedClassFields(toAggregateType(type));

  return false;
}

static bool isSubjectToBorrowLifetimeAnalysis(Type* type) {
  type = type->getValType();

  if (!(isRecord(type) || isClass(type)))
    return false;

  bool isRecordContainingFieldsSubjectToAnalysis = false;
  if (isRecord(type)) {
    AggregateType* at = toAggregateType(type);
    isRecordContainingFieldsSubjectToAnalysis = recordContainsClassFields(at);
  }

  // this is a workaround for non-optimal AST for iteration
  if (type->symbol->hasFlag(FLAG_ITERATOR_RECORD))
    isRecordContainingFieldsSubjectToAnalysis = true;

  // It needs to be:
  //  - a pointer type
  //  - a record containing refs/class pointers
  //    (or an iterator record)
  if (!(isClass(type) ||
        isRecordContainingFieldsSubjectToAnalysis))
    return false;

  if (typeHasInfiniteLifetime(type))
    return false;

  return true;
}

static bool isSubjectToBorrowLifetimeAnalysis(Symbol* sym) {
  bool argOrVar = isArgSymbol(sym) || isVarSymbol(sym);

  if (!argOrVar)
    return false;

  if (!isSubjectToBorrowLifetimeAnalysis(sym->type))
    return false;

  // Symbols marked "unsafe" are not subject to analysis.
  if (sym->hasFlag(FLAG_UNSAFE))
    return false;

  // this is a workaround for non-optimal AST for iteration
  if (sym->hasFlag(FLAG_INDEX_OF_INTEREST))
    return false;

  return true;
}


static bool shouldPropagateLifetimeTo(CallExpr* call, Symbol* sym) {

  FnSymbol* inFn = call->getFunction();

  if (!(isSubjectToRefLifetimeAnalysis(sym) ||
        isSubjectToBorrowLifetimeAnalysis(sym)))
    return false;

  if (!isLocalVariable(inFn, sym))
    return false;

  // Lifetime for index variables is set by infer's for loop visitor
  if (sym->hasFlag(FLAG_INDEX_VAR))
    return false;

  // Don't propagate lifetime to RVV - we check moves to it
  // separately
  if (sym->hasFlag(FLAG_RVV))
    return false;

  // Detect old-style record construction. The constructed record
  // should not have its lifetime set by the constructor call.
  // Instead, it should start with lifetime = reachability.

  // e.g. l7.chpl
/*  if (CallExpr* rhsCallExpr = toCallExpr(call->get(2))) {
    if (FnSymbol* calledFn = rhsCallExpr->resolvedOrVirtualFunction()) {
      if (AggregateType* at = toAggregateType(rhsCallExpr->typeInfo())) {
        if (isRecord(at) &&
            calledFn->hasFlag(FLAG_CONSTRUCTOR) &&
            recordContainsOwnedClassFields(at))
          return false;
      }
    }
  }*/

//  if (!sym->hasFlag(FLAG_TEMP))
//    return false;

  //Symbol* parentSymbol = sym->defPoint->parentSymbol;
  // unsafe fns not lifetime analyzed
  //if (parentSymbol->hasFlag(FLAG_UNSAFE))
  //  return false;
  //else
  /*if (parentSymbol->hasFlag(FLAG_SAFE))
    return true;
  else if (sym->hasFlag(FLAG_SAFE))
    return true;
  else if (sym->hasFlag(FLAG_SCOPE))
    return true;
  else
    return defaultToCheckingLifetimes;
   */
  return true;
}

static bool isAnalyzedMoveOrAssignment(CallExpr* call) {

  if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN))
    return true;

  FnSymbol* calledFn = call->resolvedOrVirtualFunction();

  // Only consider compiler-generated function calls to "=" for lifetime
  // propagation. Only consider those functions that are operating
  // on a record containing borrows or on class instances.
  //
  // (we assume that a user supplied = function for a class with owned
  //  pointers handles lifetimes in a reasonable manner)
  if (calledFn && 0 == strcmp("=", calledFn->name)) {
    if (isClassOrNil(call->get(1)->getValType()) &&
        isClassOrNil(call->get(2)->getValType()))
      return true;
    if (AggregateType* at = toAggregateType(call->get(1)->getValType()))
      if (calledFn->hasFlag(FLAG_COMPILER_GENERATED))
        if (recordContainsBorrowedClassFields(at))
          return true;
  }

  return false;
}

/*
   Global variables have infinite lifetime.
   Unsafe/raw class instance pointers have infinite lifetime.
 */
static bool symbolHasInfiniteLifetime(Symbol* sym) {

  if (sym == NULL)
    return true;

  Symbol* parentSymbol = sym->defPoint->parentSymbol;
  if (isModuleSymbol(parentSymbol)) {
    // Global variables have infinite lifetime
    return true;
  }

  if (isClass(sym->type) && sym->hasFlag(FLAG_RAW)) {
    // raw class instances have infinite lifetime
    return true;
  }

  return false;
}

static BlockStmt* getDefBlock(Symbol* sym) {
  Expr* defPoint = sym->defPoint;
  while (defPoint && !isBlockStmt(defPoint)) {
    defPoint = defPoint->parentExpr;
  }
  return toBlockStmt(defPoint);
}

// This could definately be implemented in a faster way.
static bool isBlockWithinBlock(BlockStmt* a, BlockStmt* b) {
  Expr* findParent = b;
  for (Expr* cur = a; cur; cur = cur->parentExpr) {
    if (cur == findParent)
      return true;
  }
  return false;
}

/* Consider two variables/arguments a and b.
   Is the lifetime of a within the lifetime for b?
      - e.g. if a is declared in a block nested inside the lifetime of b
 */
static bool isLifetimeShorter(Lifetime a, Lifetime b) {

  if (a.infinite) // a infinite, b infinite or not
    return false;
  else if (b.infinite) // a not infinite, b infinite
    return true;
  else if (a.fromSymbolReachability == b.fromSymbolReachability)
    return false;
  else {
    Symbol* aSym = a.fromSymbolReachability;
    Symbol* bSym = b.fromSymbolReachability;
    BlockStmt* aBlock = getDefBlock(aSym);
    BlockStmt* bBlock = getDefBlock(bSym);
    if (aBlock == bBlock) {
      // TODO: check the order of the declarations
    } else {
      return isBlockWithinBlock(aBlock, bBlock);
    }
  }

  return false;
}

static Lifetime minimumLifetime(Lifetime a, Lifetime b) {
  if (isLifetimeShorter(a, b))
    return a;
  else
    return b;
}


static ScopeLifetime minimumScopeLifetime(ScopeLifetime a, ScopeLifetime b) {
  ScopeLifetime ret;
  ret.referent = minimumLifetime(a.referent, b.referent);
  ret.borrowed = minimumLifetime(a.borrowed, b.borrowed);
  return ret;
}

static Lifetime reachabilityLifetimeForSymbol(Symbol* sym) {
  Lifetime lt;
  lt.fromSymbolReachability = sym;
  lt.relevantExpr = NULL;
  lt.infinite = symbolHasInfiniteLifetime(sym);
  lt.returnScope = false;
  return lt;
}

/*
static ScopeLifetime reachabilityScopeLifetimeForSymbol(Symbol* sym) {
  Lifetime lt = reachabilityLifetimeForSymbol(sym);
  ScopeLifetime ret;
  ret.referent = lt;
  ret.borrowed = lt;

  // Adjust the returnScope field of these lifetimes.
  if (isArgSymbol(sym)) {
    if (sym->isRef())
      ret.referent.returnScope = true;

    if (isSubjectToBorrowLifetimeAnalysis(sym->type))
      ret.borrowed.returnScope = true;
  }

  return ret;
}
*/

static Lifetime infiniteLifetime() {
  Lifetime lt;
  lt.fromSymbolReachability = NULL;
  lt.relevantExpr = NULL;
  lt.infinite = true;
  lt.returnScope = false;
  return lt;
}

static ScopeLifetime infiniteScopeLifetime() {
  Lifetime lt = infiniteLifetime();
  ScopeLifetime ret;
  ret.referent = lt;
  ret.borrowed = lt;
  return ret;
}

/* Types of class instance pointers:
     * raw/unsafe - could be ptr in Owned/Shared
     * borrow - want this to be just var x:ClassType one day.
 */

/* Cases to catch

    - returning a ref to a local variable
      ... including through the ref-identity function
    - returning a ref to a field & destroying record before using ref
    - store result of a 'borrow' in a global variable
    - other scenario: new returns Owned
       - class instance created and stored in a global variable
       - Owned record initialized with it and is destroyed, deleting instance
       - the global variable is dereferenced
 */


/* Language design questions:
    * how do you say which kind of class instance pointer you have?

      - borrow vs owning reference

    * how does one indicate relationship of returned ref/borrow
       lifetime to arguments / calling scope?
    * how do you specify lifetime based how do you specify the different types of pointers?
       - explicitly (Rust)
       - 'return scope' (D)
       - inference
          - D - ?
          - minimum lifetime of arguments?

    Is it possible to have a Owned/Shared reset to a new value?
      Then the lifetime of the object is shorter.

       MyShared.clear();
       MyShared.retain(OtherPtr);
       MyShared = YourShared;

       MyOwned.clear();
       MyOwned.retain(OtherPtr);
       MyOwned = YourOwned;

       -> mark functions that potentially invalidate ?
       -> it is illegal to have any borrows at that moment the invalidation
          occurs?

          Isn't this the same as the array-resize-with-ref case?
          Maybe reasonable to completely ignore checking for it...

      ArrayOfOwned[i] = OtherOwned;
       -> would mean no borrows allowed to other elements,
          because ArrayOfOwned[i] assumed to invalidate all of the array?
 */

 /*

Challenging cases for this analysis:

String.chpl copyRemoteBuffer - allocates and returns something
   (should be returning a new Owned)

returning nil -- fixed (was just an AST pattern issue)

method that returns something with infinite lifetime (e.g. always returning nil)
   - locale.here returns chpl_localeID_to_locale(here_id)

Linked list destruction, linked list on bare pointers
  e.g. chpl_deinitModules
  e.g. TaskErrors.init

  var next = start;
  while next {
    const curr = prev;
    prev = curr.next
    delete curr;
  }

  Problem is that 'curr' is inner scope as compared to 'next'.

  var first:MyClass;
  for e in something.iterateClasses() {
    first = e;
  }


This is a problem in D too
// compile with dmd -c -dip1000
int g1 = 1;
int g2 = 2;
void test1() @safe {

  scope int*[2] array;
  array[0] = &g1;
  array[1] = &g2;

  scope int* first = null;
  for (int i = 0; i < 2; i++) {
    auto cur = array[i];
    if (first == null) {
      first = cur;
    }
  }
}
void test2() @safe {

  int[2] array;
  array[0] = 1;
  array[1] = 2;

  scope int* first = null;
  for (int i = 0; i < 2; i++) {
    auto cur = &array[i];
    if (first == null) {
      first = cur;
    }
  }
}

These cases seem to be handled by a strategy of inferring the lifetime
  of each variable to be a minimum of what it is set to & explicitly
  checking that reachability < lifetime.


ChapelError:241 proc message

first = nil;
while ... {
  if first == nil then
     first = e;
}

This is something the compiler translates to
first = nil;
while ... {
  if first:object == nil then
     first = e;
}


Problem here is that first might be inferred to have infinite lifetime,
and then at the time the assignment is run, infinite lifetime
will be returned for the cast to object,and then problems ensue.

Resolve that by putting the minimum-lifetime-finding in a loop while changed.


Returning a tuple of class instances e.g. BaseDom.remove().

resolved by fixing inference for var x:MyClass = nil;

openfd error resolved by fixing out intent


test/release/examples/hello4-datapar-dist.chpl error -
range is passed to chpl__buildDomainExpr, sets the lifetime
of the result to the lifetime of the range, but the
result is ultimately stored in a global variable.

 - fix: lifetime of _domain/etc instance variables is not managed, instance
   "unsafe"

test/modules/standard/Spawn was giving lifetime errors for the result
of spawn being stored in a global variable after initialization routine
returned.

 - fix: make the 'locale' type not subject to lifetime analysis
        (channel has a locale pointer in it, this was considered a borrow).

*** There is a fundamental challenge with borrow checking
    for constructed records. This can be seen in test l7
    vs. test l11. The AST forms are different for initializers
    vs constructors too.

    The question comes down to - what should the borrow-checking lifetime
    of
       new SomeRecord(SomeArgument)
    be ?

    It's especially interesting to consider the case that SomeArgument
    itself is a record containing a borrow.

    On one hand, if 'new SomeRecord' does not include any arguments,
    or if all of its arguments are globals (or have infinite lifetime
    for another reason), it seems that the new record should have
    infinite borrow lifetime.

    On the other hand, if SomeRecord contains a pointer it "owns", it
    should have lifetime equal to its reachability.

    Solution: consider "Owned" pointers to be different from "Unsafe" pointers.

    _newDistribution and _getDistribution seem to have an issue in
    this area. The thing about _array etc is that they sometimes own the
    pointer and sometimes borrow it.

 */
