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

#include "AstVisitorTraverse.h"
#include "driver.h"
#include "expr.h"
#include "ForLoop.h"
#include "loopDetails.h"
#include "UnmanagedClassType.h"
#include "stlUtil.h"
#include "symbol.h"
#include "view.h"
#include "wellknown.h"

/* This file implements lifetime checking.

   The lifetime checker infers the lifetime of each symbol (how long what it
   refers to will be allocated). Globals have infinite lifetime, but a local
   simple value variable (e.g. an integer) has lifetime of the function block it
   is declared in.

   The reachability of a symbol is simply the lexical block in which
   it is alive.

   The checker emits errors for several cases:

     * reachability of a symbol is greater than its lifetime
     * setting a symbol to another value with a shorter lifetime
     * returning a symbol with non-infinite and non-return lifetime
       (e.g. reference arguments are marked with return lifetime)

   This pass proceeds by computing the minimum lifetime of each
   local variable and then proceeds to look for the above error cases.
   Lifetime inference is particularly important to making this
   system usable.

   * pointer/ref arguments are inferred to be return scope
     generally, but for methods, the default is that only 'this'
     is return scope.

   When considering the lifetime of the result of a call,
   this pass assumes that the lifetime is the minimum of the
   lifetimes of actual arguments with return scope.

   Records are subject to lifetime checking if they contain
   fields that are subject to lifetime checking.

   Assumption: the initial version assumes that the = overload
   for a record does something reasonable and doesn't leave
   pointers aliasing each other. This can be tidied up once the
   type system differentiates between owned, raw, and borrowed pointers.


   TODO:
     - investigate if this can do escape checking for begins
     - decide on how to declare an owning vs borrowed class instance ptr
       - vs a raw one
     - decide on how to declare ownership transfer arg vs
       borrowing arg
       - returns infinite lifetime
       - returns lifetime matching this
       - returns lifetime matching arg
     - Should default for a method be borrowing from 'this'
       (vs arguments?)
     - improve error messages for the case in which a
       function calls another function that needs an annotation
     - check return scope better (there can only be one
       return scope argument per function? in that event, that's
       the only argument marked with return scope?)
     - consider the order of declarations, so that
       a borrow of a variable used in a deinit call
       won't be invalid if the borrow is from something
       being destroyed in the same block.
 */

// These enable debug facilities
const char* debugLifetimesForFn = "";
const int debugLifetimesForId = 0;
const bool debugOutputOnError = false;

namespace {

  /* A Lifetime describes the limits on when a variable is
     usable. It might be infinite (e.g. referring to / borrowing
     a global). It might represent an argument that has
     return scope set on it. Beyond those flags, the key information
     here is fromSymbolReachability - that points to the
     symbol that the analysis inferred as the value the
     variable could point to that has the least scope.
     In other words, the lexical scope of fromSymbolReachability
     determines the lifetime.
   */
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

  /* A ScopeLifetime simply stores the combination of a ref
     ref lifetime and a borrow lifetime. The lifetime analysis
     for refs and borrows is slightly different and so both
     lifetimes are tracked simultaneously in a ScopeLifetime. */
  struct ScopeLifetime {
    // referent lifetime is the scope of what a ref variable might refer to
    Lifetime referent;
    // borrowed lifetime is the scope of what borrowed class instances
    // might refer to. This might be longer than the referent lifetime.
    Lifetime borrowed;
  };

  typedef std::map<Symbol*,ScopeLifetime> SymbolToLifetimeMap;
  typedef std::map<CallExpr*,ScopeLifetime> ReturnToLifetimeMap;
  typedef std::map<Symbol*,Symbol*> SymbolToSymbolMap;

  struct LifetimeState {
    SymbolToLifetimeMap lifetimes;
    SymbolToSymbolMap detemp;
    Symbol* getCanonicalSymbol(Symbol* sym);
    Lifetime reachabilityForSymbol(Symbol* sym);
    bool setLifetimeForSymbolToMin(Symbol* sym, ScopeLifetime lt);
    ScopeLifetime lifetimeForSymbol(Symbol* sym);
    ScopeLifetime lifetimeForActual(Symbol* sym);
    ScopeLifetime lifetimeForCallReturn(CallExpr* call);
    ScopeLifetime lifetimeForPrimitiveReturn(CallExpr* call);
  };

  class GatherRefTempsVisitor : public AstVisitorTraverse {

    public:
      LifetimeState* lifetimes;
      virtual bool enterCallExpr(CallExpr* call);
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
static void markArgumentsReturnScope(FnSymbol* fn);
static void checkLifetimesInFunction(FnSymbol* fn);

void checkLifetimes(void) {
  // Mark all arguments with FLAG_SCOPE or FLAG_RETURN_SCOPE.
  // This needs to be done for all functions before the next
  // loop since it affects how calls are handled.
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    markArgumentsReturnScope(fn);
  }

  // Perform lifetime checking on each function
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    checkLifetimesInFunction(fn);
  }
}

static void checkLifetimesInFunction(FnSymbol* fn) {
  if (shouldCheckLifetimesInFn(fn)) {
    bool debugging = debuggingLifetimesForFn(fn);

    if (debugging) {
      printf("Visiting function %s id %i\n", fn->name, fn->id);
      nprint_view(fn);
      gdbShouldBreakHere();
    }

    LifetimeState state;

    // Gather temps that are just aliases for something else
    GatherRefTempsVisitor gather;
    gather.lifetimes = &state;
    fn->accept(&gather);

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

static void markArgumentsReturnScope(FnSymbol* fn) {
  // Default for methods is to mark 'this' argument with return scope.
  // Default for everything else is to mark all arguments with return scope.
  bool anyReturnScope = false;
  // Figure out if any arguments have FLAG_RETURN_SCOPE on them.
  // If not, we default to putting it on all of them.
  // If so, we put FLAG_SCOPE on the rest.
  for_formals(formal, fn) {
    if (formal->hasFlag(FLAG_RETURN_SCOPE))
      anyReturnScope = true;
  }

  if (fn->isMethod() && fn->_this != NULL && !anyReturnScope) {
    // Methods default to 'this' return scope
    fn->_this->addFlag(FLAG_RETURN_SCOPE);
    anyReturnScope = true;
  }

  for_formals(formal, fn) {
    if (formal->hasFlag(FLAG_SCOPE) ||
        formal->hasFlag(FLAG_RETURN_SCOPE)) {
      // OK
    } else {
      // Set it to the default
      if (anyReturnScope)
        formal->addFlag(FLAG_SCOPE);
      else
        formal->addFlag(FLAG_RETURN_SCOPE);
    }
  }

  for_formals(formal, fn) {
    INT_ASSERT(formal->hasFlag(FLAG_SCOPE) ||
               formal->hasFlag(FLAG_RETURN_SCOPE));
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


  return fLifetimeChecking;
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
  printf("detemps:\n");
  for (SymbolToSymbolMap::iterator it = state->detemp.begin();
       it != state->detemp.end();
       ++it) {
    Symbol* key = it->first;
    Symbol* value = it->second;
    printf("Symbol %s[%i] detemps to %s[%i]\n",
           key->name, key->id, value->name, value->id);
  }
  printf("inferred lifetimes:\n");
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

Lifetime LifetimeState::reachabilityForSymbol(Symbol* sym) {
  Symbol* canonical = getCanonicalSymbol(sym);
  Lifetime ret = reachabilityLifetimeForSymbol(canonical);
  return ret;
}

// Ignore reference temporaries - gets the relevant user variable
// if possible.
// This enables this pass to easily handle the result of return-by-ref-arg.
Symbol* LifetimeState::getCanonicalSymbol(Symbol* sym) {
  while (detemp.count(sym) > 0)
    sym = detemp[sym];

  return sym;
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
  Lifetime reach = reachabilityForSymbol(sym);

  // Leave out the symbol update if the update doesn't matter
  // This prevents errors based upon "scoped variables" when the
  // variable has no meaningful scope.
  // It would be reasonable to update callers to avoid this situation, too.
  if (!sym->isRef())
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
    if (isLifetimeShorter(lt.borrowed, value.borrowed)) {
      if (sym->id == breakOnId)
        gdbShouldBreakHere();
      value.borrowed = lt.borrowed;
      changed = true;
    }
  }

  return changed;
}

static bool intentIsLocalVariable(IntentTag tag) {
  switch (tag) {
    case INTENT_IN:
    case INTENT_OUT:
    case INTENT_INOUT:
    case INTENT_CONST_IN:
      return true;
    case INTENT_CONST:
    case INTENT_REF:
    case INTENT_CONST_REF:
      return false;
    case INTENT_REF_MAYBE_CONST:
    case INTENT_PARAM:
    case INTENT_TYPE:
    case INTENT_BLANK:
      // These might be original intent, but shouldn't
      // be encountered for the current intent.
      return false;
  }
  INT_FATAL("case not handled");
  return false;
}

ScopeLifetime LifetimeState::lifetimeForSymbol(Symbol* sym) {

  // Ignore reference temporaries
  sym = getCanonicalSymbol(sym);

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
    if (ArgSymbol* arg = toArgSymbol(sym)) {
      Lifetime lt = reachabilityForSymbol(sym);
      ret.referent = lt;
      ret.borrowed = lt;
      if (arg->hasFlag(FLAG_RETURN_SCOPE)) {
        if (sym->isRef() &&
            !intentIsLocalVariable(arg->intent) &&
            !intentIsLocalVariable(arg->originalIntent))
          ret.referent.returnScope = true;

        if (isSubjectToBorrowLifetimeAnalysis(sym->type))
          ret.borrowed.returnScope = true;
      }
    }
  }

  return ret;
}

// Normalizes the lifetime to the reachability as appropriate
// for cases such as getting the lifetime of a local variable.
// This doesn't always occur though, in the inference, so that
// a variable containing a borrow can have a borrow lifetime greater
// than its reachability.
ScopeLifetime LifetimeState::lifetimeForActual(Symbol* actualSym) {
  // Ignore reference temporaries
  actualSym = getCanonicalSymbol(actualSym);

  ScopeLifetime ret = lifetimeForSymbol(actualSym);

  // At a minimum, non-ref actuals should have lifetime = reachability
  if (!actualSym->isRef()) {
    Lifetime reachability = reachabilityForSymbol(actualSym);
    if (isLifetimeShorter(reachability, ret.referent))
      ret.referent = reachability;
  }

  // At a minimum, a potential borrow from a record containing owned
  // has lifetime = reachability.
  if (!actualSym->isRef() && containsOwnedClass(actualSym->type)) {
    Lifetime reachability = reachabilityForSymbol(actualSym);
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
  if (isClassLike(call->typeInfo()) &&
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
  } else if(calledFn->hasFlag(FLAG_FN_RETARG)) {
    ArgSymbol* retArg = toArgSymbol(toDefExpr(calledFn->formals.tail)->sym);
    INT_ASSERT(retArg && retArg->hasFlag(FLAG_RETARG));
    returnType = retArg->getValType();
  }

  returnsBorrow = isSubjectToBorrowLifetimeAnalysis(returnType);

  for_formals_actuals(formal, actual, call) {
    SymExpr* actualSe = toSymExpr(actual);
    INT_ASSERT(actualSe);
    Symbol* actualSym = actualSe->symbol();

    ScopeLifetime argLifetime = infiniteScopeLifetime();

    // in, out, retarg arguments don't determine the lifetime of
    // a returned variable
    if (formal->hasFlag(FLAG_RETARG) ||
        formal->hasFlag(FLAG_ERROR_VARIABLE) ||
        (formal->originalIntent & INTENT_FLAG_IN) ||
        (formal->originalIntent & INTENT_FLAG_OUT))
      continue;

    // arguments marked with FLAG_SCOPE don't determine the lifetime
    // of a returned variable, since only return scope returns are allowed.
    if (formal->hasFlag(FLAG_SCOPE))
      continue;

    if (returnsRef && formal->isRef() &&
        (isSubjectToRefLifetimeAnalysis(actualSym) ||
         isLocalVariable(call->getFunction(), actualSym))) {

      // Use the referent part of the actual's lifetime
      ScopeLifetime temp = lifetimeForActual(actualSym);
      argLifetime.referent = temp.referent;
    }

    if (returnsBorrow && isSubjectToBorrowLifetimeAnalysis(actualSym)) {
      // Use the borrowed part of the actual's lifetime
      ScopeLifetime temp = lifetimeForActual(actualSym);
      argLifetime.borrowed = temp.borrowed;
    }

    minLifetime = minimumScopeLifetime(minLifetime, argLifetime);
  }

  return minLifetime;
}

ScopeLifetime LifetimeState::lifetimeForPrimitiveReturn(CallExpr* call) {

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
    if (returnsRef && isClassLike(actualSym->getValType())) {
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
  }

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


bool GatherRefTempsVisitor::enterCallExpr(CallExpr* call) {
  if (call->isPrimitive(PRIM_MOVE)) {
    SymExpr* lhsSe = toSymExpr(call->get(1));
    INT_ASSERT(lhsSe);
    Symbol* lhs = lhsSe->symbol();

    // don't de-temp the return value
    if (lhs->hasFlag(FLAG_RVV))
      return false;

    if (SymExpr* rhsSe = toSymExpr(call->get(2))) {
      Symbol* rhs = rhsSe->symbol();
      if (rhs->hasFlag(FLAG_TEMP) && !lhs->hasFlag(FLAG_TEMP)) {
        lifetimes->detemp[rhs] = lhs;
      } else if (lhs->hasFlag(FLAG_TEMP) && !rhs->hasFlag(FLAG_TEMP)) {
        lifetimes->detemp[lhs] = rhs;
      }
    } else if (CallExpr* rhsCall = toCallExpr(call->get(2))) {
      if (rhsCall->isPrimitive(PRIM_ADDR_OF) ||
          rhsCall->isPrimitive(PRIM_SET_REFERENCE)) {
        SymExpr* rhsSe = toSymExpr(rhsCall->get(1));
        Symbol* rhs = rhsSe->symbol();
        if (lhs->hasEitherFlag(FLAG_TEMP,FLAG_REF_TEMP))
          lifetimes->detemp[lhs] = rhs;
      }
    }
  }
  return false;
}

static bool isRecordInitOrReturn(CallExpr* call, Symbol*& lhs, CallExpr*& initOrCtor, LifetimeState* lifetimes) {

  if (call->isPrimitive(PRIM_MOVE) ||
      call->isPrimitive(PRIM_ASSIGN)) {
    if (CallExpr* rhsCallExpr = toCallExpr(call->get(2))) {
      if (rhsCallExpr->resolvedOrVirtualFunction()) {
        if (AggregateType* at = toAggregateType(rhsCallExpr->typeInfo())) {
          if (isRecord(at)) {
            SymExpr* se = toSymExpr(call->get(1));
            INT_ASSERT(se);
            lhs = lifetimes->getCanonicalSymbol(se->symbol());
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
        lhs = lifetimes->getCanonicalSymbol(sym);
        initOrCtor = call;
        return true;
      }
    } else if (calledFn->hasFlag(FLAG_FN_RETARG)) {
      for_formals_actuals(formal, actual, call) {
        if (formal->hasFlag(FLAG_RETARG)) {
          if (isRecord(formal->getValType())) {
            SymExpr* se = toSymExpr(actual);
            INT_ASSERT(se);
            Symbol* sym = se->symbol();
            lhs = lifetimes->getCanonicalSymbol(sym);
            initOrCtor = call;
            return true;
          }
        }
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
  if (isRecordInitOrReturn(call, initSym, initCall, lifetimes) &&
      shouldPropagateLifetimeTo(call, initSym)) {

    AggregateType* at = toAggregateType(initSym->getValType());
    INT_ASSERT(at && isRecord(at));
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
      FnSymbol* calledFn = initCall->resolvedOrVirtualFunction();
      if (calledFn &&
          calledFn->hasEitherFlag(FLAG_RETURN_NOT_OWNED,
                                  FLAG_RETURNS_ALIASING_ARRAY)) {
        lt = minimumScopeLifetime(lt, lifetimes->lifetimeForCallReturn(initCall));
      } else {
        ScopeLifetime temp = infiniteScopeLifetime();
        temp.borrowed = lifetimes->reachabilityForSymbol(initSym);
        lt = minimumScopeLifetime(lt, temp);
      }
    }

    lt.borrowed.relevantExpr = call;
    changed |= lifetimes->setLifetimeForSymbolToMin(initSym, lt);

  } else if (isAnalyzedMoveOrAssignment(call)) {

    SymExpr* lhsSe = toSymExpr(call->get(1));
    Symbol* lhs = lhsSe->symbol();

    lhs = lifetimes->getCanonicalSymbol(lhs);

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
          lt = lifetimes->lifetimeForPrimitiveReturn(rhsCallExpr);
        }
      } else {
        SymExpr* se = toSymExpr(rhsExpr);
        INT_ASSERT(se);
        // TODO - this doesn't handle records using default =
        if (call->isPrimitive(PRIM_MOVE) || isClassLike(se->symbol()->type)) {
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
        lt.referent = lifetimes->reachabilityForSymbol(lhs);
      }

      lt.borrowed.relevantExpr = call;
      changed |= lifetimes->setLifetimeForSymbolToMin(lhs, lt);
    }
  }

  return false;
}

bool InferLifetimesVisitor::enterForLoop(ForLoop* forLoop) {

  if (forLoop->id == debugLifetimesForId)
    gdbShouldBreakHere();

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
      iterableLifetime = lifetimes->lifetimeForActual(iterableSe->symbol());
    } else if (CallExpr* iterableCall = toCallExpr(iterable)) {
      if (iterableCall->resolvedOrVirtualFunction())
        iterableLifetime = lifetimes->lifetimeForCallReturn(iterableCall);
      else
        iterableLifetime = lifetimes->lifetimeForPrimitiveReturn(iterableCall);
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

  if (call->id == debugLifetimesForId)
    gdbShouldBreakHere();

  if (isAnalyzedMoveOrAssignment(call)) {

    FnSymbol* calledFn = NULL;
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
        if (FnSymbol* fn = subCall->resolvedOrVirtualFunction()) {
          rhsLt = lifetimes->lifetimeForCallReturn(subCall);
          calledFn = fn;
        } else {
          rhsLt = lifetimes->lifetimeForPrimitiveReturn(subCall);
        }
      }

      // Don't worry about returning an "owned" type
      // that is the result of a call returning a record by value.
      if (containsOwnedClass(lhs->type)) {
        Symbol* ignoredLhs = NULL;
        CallExpr* ignoredCall = NULL;
        if (isRecordInitOrReturn(call, ignoredLhs, ignoredCall, lifetimes))
          rhsLt.borrowed = infiniteLifetime();
      }

      if (lhs->hasEitherFlag(FLAG_RVV,FLAG_RETARG)) {
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

        if (isSubjectToBorrowLifetimeAnalysis(lhs)) {
          // check returning a borrow
          if (!rhsLt.borrowed.infinite) {
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
        Lifetime lhsReachability = lifetimes->reachabilityForSymbol(lhs);
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

    Lifetime reachability = lifetimes->reachabilityForSymbol(key);

    // Ignore the RVV for this check
    // (see test lifetimes/bug-like-timezones.chpl)
    if (key->hasEitherFlag(FLAG_RVV,FLAG_RETARG))
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

  // Types for C compatibility are assumed to have infinite lifetime.
  if (type->symbol->hasFlag(FLAG_C_PTR_CLASS) ||
      type->symbol->hasFlag(FLAG_EXTERN) ||
      type == dtCVoidPtr ||
      type == dtCFnPtr)
    return true;

  // Locale type has infinite lifetime
  // (since locales exist for the entire program run)
  if (isSubClass(type, dtLocale))
    return true;

  if (isUnmanagedClassType(type)) {
    // unmanaged class instances have infinite lifetime
    return true;
  }

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
    if (isClassLike(field->type) &&
        (field->hasFlag(FLAG_OWNED) ||
         (!typeHasInfiniteLifetime(field->type) &&
          !field->hasFlag(FLAG_UNSAFE))))
      return true;

    if (isRecord(field->type) &&
        recordContainsClassFields(toAggregateType(field->type)))
      return true;
  }

  return false;
}

static bool recordContainsOwnedClassFields(AggregateType* at) {

  for_fields(field, at) {
    if (isClassLike(field->type) &&
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
    if (isClassLike(field->type) &&
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

static bool containsOwnedClass(Type* type) {
  type = type->getValType();

  if (isRecord(type))
    return recordContainsOwnedClassFields(toAggregateType(type));

  return false;
}

static bool isSubjectToBorrowLifetimeAnalysis(Type* type) {
  type = type->getValType();

  if (!(isRecord(type) || isClassLike(type)))
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
  if (!(isClassLike(type) ||
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
  if (sym->hasEitherFlag(FLAG_RVV,FLAG_RETARG))
    return false;

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
    if (isClassLikeOrNil(call->get(1)->getValType()) &&
        isClassLikeOrNil(call->get(2)->getValType()))
      // Only consider same-type class assignment overloads
      // (or those where one type is nil)
      if (call->get(1)->getValType() == call->get(2)->getValType() ||
          call->get(1)->getValType() == dtNil ||
          call->get(2)->getValType() == dtNil)
        return true;
    if (AggregateType* at = toAggregateType(call->get(1)->getValType()))
      if (isRecord(at) && calledFn->hasFlag(FLAG_COMPILER_GENERATED))
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

  return false;
}

static BlockStmt* getDefBlock(Symbol* sym) {
  Expr* defPoint = sym->defPoint;
  while (defPoint) {
    if (BlockStmt* block = toBlockStmt(defPoint))
      if (block->blockTag == BLOCK_NORMAL)
        return block;

    defPoint = defPoint->parentExpr;
  }
  return NULL;
}

// This could definitely be implemented in a faster way.
static bool isBlockWithinBlock(BlockStmt* a, BlockStmt* b) {
  Expr* findParent = b;
  for (Expr* cur = a; cur; cur = cur->parentExpr) {
    if (cur == findParent)
      return true;
  }
  return false;
}

/* Consider two variables/arguments a and b.
   Is the lifetime of a strictly within the lifetime for b?
      - e.g. if a is declared in a block nested inside the lifetime of b
 */
static bool isLifetimeShorter(Lifetime a, Lifetime b) {

  if (a.infinite) // a infinite, b infinite or not
    return false;
  else if (b.infinite) // a not infinite, b infinite
    return true;
  if (a.returnScope && !b.returnScope) // a return scope, b not
    return false;
  else if (!a.returnScope && b.returnScope) // a not return scope, b is
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
      return false;
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
