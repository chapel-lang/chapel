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
#include "resolution.h"
#include "stlUtil.h"
#include "symbol.h"
#include "view.h"
#include "wellknown.h"

// These enable debug facilities
static const char* debugLifetimesForFn = "";
static const int debugLifetimesForId = 0;
static const bool debugOutputOnError = false;

/* This file implements lifetime checking.

   The lifetime checker infers the lifetime of each symbol (how long what it
   refers to will be allocated). Globals have infinite lifetime, but a local
   simple value variable (e.g. an integer) has lifetime of the function block it
   is declared in.

   The checker works with two kinds of lifetimes for each symbol:
     * intrinsic lifetime, based upon the scope of the variable
       and when it will be destroyed. For references, the intrinsic lifetime
       is set to the lifetime of whatever the reference points to
       during the one moment it is initialized.
     * inferred lifetime, based on what that symbol is set to
       (e.g. for a variable of borrow class type, this is the minimum
        of the lifetimes stored in it)

   The checker emits errors for several cases:

     * intrinsic lifetime of a symbol is greater than its inferred lifetime
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
   * for non-method iterators, the return scope is the loop
     invoking the iterator

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


namespace {

  /* A Lifetime describes the limits on when a variable is
     usable. It might be infinite (e.g. referring to / borrowing
     a global). It might represent an argument that has
     return scope set on it. Beyond those flags, the key information
     here is fromSymbolScope - that points to the
     symbol that the analysis inferred as the value the
     variable could point to that has the least scope.
     In other words, the lexical scope of fromSymbolScope
     determines the lifetime.
   */
  struct Lifetime {
    // if non-NULL, the scope of this symbol represents the lifetime
    Symbol* fromSymbolScope;
    // relevant expression for this lifetime, for errors
    Expr* relevantExpr;
    // is this lifetime unknown?
    bool unknown;
    // is this lifetime infinite?
    bool infinite;
    // is this lifetime return scope - i.e. allowing it to be returned?
    bool returnScope;
    Lifetime()
      : fromSymbolScope(NULL), relevantExpr(NULL),
        unknown(true), infinite(false), returnScope(false)
    {
    }
  };

  /* A LifetimePair simply stores the combination of a
     ref lifetime and a borrow lifetime. The lifetime analysis
     for refs and borrows is slightly different and so both
     lifetimes are tracked simultaneously in a LifetimePair. */
  struct LifetimePair {
    // referent lifetime is the scope of what a ref variable might refer to
    Lifetime referent;
    // borrowed lifetime is the scope of what borrowed class instances
    // might refer to. This might be longer than the referent lifetime.
    Lifetime borrowed;
  };

  typedef std::map<Symbol*,LifetimePair> SymbolToLifetimeMap;
  typedef std::map<CallExpr*,LifetimePair> ReturnToLifetimeMap;

  /* A DetempGroup stores a group of Symbols that really refer
     to the same variable. The lifetime analysis uses a `detemp`
     map to find the `canonical` variable for a given Symbol so
     that it doesn't need to concern itself with temporary variables
     added by the compiler for one reason or other (such as reference
     temporaries or value temporaries that are set with PRIM_MOVE).

     The DetempGroup stores two things:
      1) a vector of Symbols that are conceptually the same variable
      2) a `favorite` Symbol - this is the Symbol to return when
         getting the canonical variable.
   */
  struct DetempGroup {
    Symbol* favorite;
    std::vector<Symbol*> elements;
    DetempGroup() : favorite(NULL), elements() { }
  };

  typedef std::map<Symbol*,DetempGroup*> SymbolToDetempGroupMap;
  typedef std::set<DetempGroup*> DetempGroupSet;
  typedef std::set<CallExpr*> CallExprSet;

  struct LifetimeState {
    // this mapping allows the pass to ignore certain compiler temps
    SymbolToDetempGroupMap detemp;
    CallExprSet callsToIgnore;

    // intrinsic lifetime is normally the scope of a variable.
    // It might be otherwise for array slices.
    // Unlike inferred lifetime, it never changes.
    SymbolToLifetimeMap intrinsicLifetime;

    // inferred lifetime is the lifetime the inference pass has assigned
    // to a variable. It should only be set in the event that the variable
    // possibly refers to / borrows another variable. When that happens,
    // one variable's inferred lifetime will become the minimum of the
    // other variable's intrinsic and inferred lifetimes.
    SymbolToLifetimeMap inferredLifetime;

    ~LifetimeState();

    Symbol* getCanonicalSymbol(Symbol* sym);

    LifetimePair intrinsicLifetimeForSymbol(Symbol* sym);

    bool setInferredLifetimeToMin(Symbol* sym, LifetimePair lt);
    LifetimePair inferredLifetimeForSymbol(Symbol* sym);

    // min of intrinsicLifetimeForSymbol, inferredLifetimeForSymbol
    LifetimePair combinedLifetimeForSymbol(Symbol* sym);
    // lifetime for use in inference
    LifetimePair lifetimeForActual(Symbol* sym,
                                   bool usedAsRef,
                                   bool usedAsBorrow,
                                   FnSymbol* inFn);

    LifetimePair inferredLifetimeForCall(CallExpr* call);
    LifetimePair inferredLifetimeForPrimitive(CallExpr* call);
    LifetimePair inferredLifetimeForExpr(Expr* call, bool usedAsRef, bool usedAsBorrow);
  };

  class GatherTempsVisitor : public AstVisitorTraverse {

    public:
      LifetimeState* lifetimes;
      virtual bool enterCallExpr(CallExpr* call);
  };
  class IntrinsicLifetimesVisitor : public AstVisitorTraverse {

    public:
      LifetimeState* lifetimes;
      FnSymbol* visitingFn;
      virtual bool enterDefExpr(DefExpr* def);
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
      std::set<Symbol*> erroredSymbols; // to avoid duplicate errors
      virtual bool enterCallExpr(CallExpr* call);
      void emitBadReturnErrors(CallExpr* call);
      void emitBadAssignErrors(CallExpr* call);
      void emitBadSetFieldErrors(CallExpr* call);
      void emitErrors();
  };

} /* end anon namespace */

static bool isLocalVariable(FnSymbol* fn, Symbol* sym);
static bool typeHasInfiniteBorrowLifetime(Type* type);
static bool isSubjectToRefLifetimeAnalysis(Symbol* sym);
static bool isSubjectToBorrowLifetimeAnalysis(Type* type);
static bool isSubjectToBorrowLifetimeAnalysis(Symbol* sym);
static bool recordContainsClassFields(AggregateType* at);
static bool recordContainsOwnedClassFields(AggregateType* at);
static bool recordContainsBorrowedClassFields(AggregateType* at);
static bool containsOwnedClass(Type* type);
static bool isOrContainsBorrowedClass(Type* type);
static bool shouldPropagateLifetimeTo(CallExpr* call, Symbol* sym);
static bool isAnalyzedMoveOrAssignment(CallExpr* call);
static bool symbolHasInfiniteLifetime(Symbol* sym);
static BlockStmt* getDefBlock(Symbol* sym);
static bool isBlockWithinBlock(BlockStmt* a, BlockStmt* b);
static bool isLifetimeShorter(Lifetime a, Lifetime b);
static Lifetime minimumLifetime(Lifetime a, Lifetime b);
static LifetimePair minimumLifetimePair(LifetimePair a, LifetimePair b);
static Lifetime scopeLifetimeForSymbol(Symbol* sym);
static Lifetime infiniteLifetime();
static Lifetime unknownLifetime();
static LifetimePair infiniteLifetimePair();
static LifetimePair unknownLifetimePair();
static bool debuggingLifetimesForFn(FnSymbol* fn);
void printLifetime(Lifetime lt);
void printLifetimePair(LifetimePair lt);
void printLifetimeState(LifetimeState* state);
static void handleDebugOutputOnError(Expr* e, LifetimeState* state);
static bool shouldCheckLifetimesInFn(FnSymbol* fn);
static void markArgumentsReturnScope(FnSymbol* fn);
static void checkFunction(FnSymbol* fn);

static bool isCallToFunctionReturningNotOwned(CallExpr* call);

void checkLifetimes(void) {
  // Mark all arguments with FLAG_SCOPE or FLAG_RETURN_SCOPE.
  // This needs to be done for all functions before the next
  // loop since it affects how calls are handled.
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    markArgumentsReturnScope(fn);
    adjustSignatureForNilChecking(fn);
  }

  // Perform lifetime checking on each function
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    checkFunction(fn);
  }
}

static void checkFunction(FnSymbol* fn) {
  if (shouldCheckLifetimesInFn(fn)) {
    // check lifetimes
    // e.g. borrow can't outlive borrowed-from
    checkLifetimesInFunction(fn);
  }

  if (fCompileTimeNilChecking) {
    // Determine cases where the compiler can prove
    // a reference-type variable is 'nil'
    // TODO: enable this for internal modules as well.
    // It was initially enabled only for user code
    // as a convenience during development.
    if (fn->getModule()->modTag == MOD_USER)
      findNilDereferences(fn);

    // TODO:
    // Determine cases where the compiler can prove
    // a class-type variable is not 'nil'
  }
}

void checkLifetimesInFunction(FnSymbol* fn) {

  bool debugging = debuggingLifetimesForFn(fn);

  if (debugging) {
    printf("Visiting function %s id %i\n", fn->name, fn->id);
    nprint_view(fn);
    gdbShouldBreakHere();
  }

  LifetimeState state;

  // Gather temps that are just aliases for something else
  GatherTempsVisitor gather;
  gather.lifetimes = &state;
  fn->accept(&gather);

  // Figure out the scope for local variables / arguments
  IntrinsicLifetimesVisitor intrinsics;
  intrinsics.lifetimes = &state;
  intrinsics.visitingFn = fn;
  fn->accept(&intrinsics);

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

  if (fn->isMethod() && fn->_this != NULL && !anyReturnScope &&
      fn->name != astrInit) {
    // Methods default to 'this' return scope
    // ('init' functions aren't really methods for this purpose)
    fn->_this->addFlag(FLAG_RETURN_SCOPE);
    anyReturnScope = true;
  }

  for_formals(formal, fn) {
    if (formal->hasFlag(FLAG_SCOPE) ||
        formal->hasFlag(FLAG_RETURN_SCOPE)) {
      // OK
    } else {
      // Set it to the default
      if (anyReturnScope == false || formal->originalIntent == INTENT_IN)
        formal->addFlag(FLAG_RETURN_SCOPE);
      else
        formal->addFlag(FLAG_SCOPE);
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

  // TODO
  if (fn->hasFlag(FLAG_COMPILER_GENERATED))
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
  if (lt.unknown)
    printf("unknown ");
  if (lt.infinite)
    printf("infinite ");
  if (lt.returnScope)
    printf("return_scope ");
  if (lt.fromSymbolScope)
    printf("like %s[%i] ", lt.fromSymbolScope->name, lt.fromSymbolScope->id);
  if (lt.relevantExpr)
    printf("expr %i ", lt.relevantExpr->id);
}

void printLifetimePair(LifetimePair lt) {
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
  for (SymbolToDetempGroupMap::iterator it = state->detemp.begin();
       it != state->detemp.end();
       ++it) {
    Symbol* key = it->first;
    DetempGroup* value = it->second;
    Symbol* favorite = value->favorite;
    if (key != favorite) {
      printf("Symbol %s[%i] detemps to %s[%i]\n",
             key->name, key->id, favorite->name, favorite->id);
    }
  }
  printf("intrinsic lifetimes:\n");
  for (SymbolToLifetimeMap::iterator it = state->intrinsicLifetime.begin();
       it != state->intrinsicLifetime.end();
       ++it) {
    Symbol* key = it->first;
    LifetimePair value = it->second;

    printf("Symbol %s[%i] has intrinsic lifetime: ", key->name, key->id);
    printLifetimePair(value);
  }
  printf("inferred lifetimes:\n");
  for (SymbolToLifetimeMap::iterator it = state->inferredLifetime.begin();
       it != state->inferredLifetime.end();
       ++it) {
    Symbol* key = it->first;
    LifetimePair value = it->second;

    printf("Symbol %s[%i] has inferred lifetimes: ", key->name, key->id);
    printLifetimePair(value);
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

LifetimeState::~LifetimeState() {
  // free the detemp groups

  // First, gather them, not counting duplicates
  DetempGroupSet set;
  for (SymbolToDetempGroupMap::iterator it = detemp.begin();
       it != detemp.end();
       ++it) {
    DetempGroup* value = it->second;
    set.insert(value);
  }

  // Now, delete them
  for (DetempGroupSet::iterator it = set.begin();
       it != set.end();
       ++it) {
    DetempGroup* value = *it;
    delete value;
  }
}

// Ignore reference temporaries - gets the relevant user variable
// if possible.
// This enables this pass to easily handle the result of return-by-ref-arg.
Symbol* LifetimeState::getCanonicalSymbol(Symbol* sym) {

  if (detemp.count(sym) > 0) {
    sym = detemp[sym]->favorite;
  }

  return sym;
}

LifetimePair LifetimeState::intrinsicLifetimeForSymbol(Symbol* sym) {

  // Ignore reference temporaries
  sym = getCanonicalSymbol(sym);
  LifetimePair lp = unknownLifetimePair();
  if (intrinsicLifetime.count(sym)) {
    // local variables and arguments should already be in the mapping
    // after IntrinsicLifetimesVisitor has run.
    lp = intrinsicLifetime[sym];
  } else {
    // global/outer variables aren't gathered into the mapping, but these
    // are considered to have infinite lifetime.
    if (sym->isRef())
      lp.referent = infiniteLifetime();

    lp.borrowed = infiniteLifetime();
  }

  return lp;
}

// Sets lifetime for symbol sym to be lt if it's not in the table yet,
// or the minimum of the previous value and lt if it is.
//
// Returns true if the state was changed.
bool LifetimeState::setInferredLifetimeToMin(Symbol* sym, LifetimePair lt) {
  bool changed = false;

  int breakOnId = debugLifetimesForId;

  // Leave out the symbol update if the update doesn't matter
  // This prevents errors based upon "scoped variables" when the
  // variable has no meaningful scope.
  // It would be reasonable to update callers to avoid this situation, too.
  if (!isSubjectToRefLifetimeAnalysis(sym))
    lt.referent = unknownLifetime();
  if (!isSubjectToBorrowLifetimeAnalysis(sym))
    lt.borrowed = unknownLifetime();
  // TODO -- consider making the above into asserts.

  // Don't bother storing unknown.
  if (lt.referent.unknown && lt.borrowed.unknown)
    return false;

  if (inferredLifetime.count(sym) == 0) {
    if (sym->id == breakOnId)
      gdbShouldBreakHere();

    inferredLifetime[sym] = lt;
    changed = true;
  } else {
    LifetimePair & value = inferredLifetime[sym];
    // This is just value = minimumLifetimePair(value, lt)
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

LifetimePair LifetimeState::inferredLifetimeForSymbol(Symbol* sym) {
  // Ignore reference temporaries
  sym = getCanonicalSymbol(sym);

  LifetimePair lp = unknownLifetimePair();
  if (inferredLifetime.count(sym)) {
    lp = inferredLifetime[sym];
  }

  return lp;
}


// Returns the minimum of the intrinsic and inferred lifetimes for sym.
LifetimePair LifetimeState::combinedLifetimeForSymbol(Symbol* sym) {
  // Ignore reference temporaries
  sym = getCanonicalSymbol(sym);

  LifetimePair intrinsic = intrinsicLifetimeForSymbol(sym);
  LifetimePair inferred = inferredLifetimeForSymbol(sym);

  LifetimePair ret = minimumLifetimePair(intrinsic, inferred);

  return ret;
}

// Normalizes the lifetime to the intrinsic lifetime as appropriate
// for cases such as getting the lifetime of a local variable.
// This doesn't always occur though, in the inference, so that
// a variable containing a borrow can have an inferred lifetime greater
// than its intrinsic lifetime.

LifetimePair LifetimeState::lifetimeForActual(Symbol* sym, bool usedAsRef, bool usedAsBorrow, FnSymbol* inFn) {

  // Careful here, as sym might not be a canonical symbol,
  // or it might be, so it being a ref or not is an uncertain matter.
  bool combineForRef = usedAsRef;
  bool combineForBorrow = usedAsBorrow && containsOwnedClass(sym->type);
  bool combineForOuter = !isLocalVariable(inFn, sym);
  bool combineForArg = isArgSymbol(sym);
  if (combineForRef || combineForBorrow || combineForOuter || combineForArg)
    return combinedLifetimeForSymbol(sym);

  return inferredLifetimeForSymbol(sym);
}

static bool formalArgumentDoesNotImpactReturnLifetime(ArgSymbol* formal)
{
  // in, out, retarg arguments don't determine the lifetime of
  // a returned variable
  if (formal->hasFlag(FLAG_RETARG) ||
      formal->hasFlag(FLAG_ERROR_VARIABLE) ||
      (formal->originalIntent & INTENT_FLAG_OUT))
    return true;

  // record initializer's this argument doesn't impact return lifetime
  if (formal->hasFlag(FLAG_ARG_THIS) &&
      formal->getFunction()->name == astrInit)
    return true;

  // arguments marked with FLAG_SCOPE don't determine the lifetime
  // of a returned variable, since only return scope returns are allowed.
  if (formal->hasFlag(FLAG_SCOPE))
    return true;

  return false;
}

LifetimePair LifetimeState::inferredLifetimeForCall(CallExpr* call) {

  FnSymbol* inFn = call->getFunction();
  FnSymbol* calledFn = call->resolvedOrVirtualFunction();
  if (calledFn->hasFlag(FLAG_RETURNS_INFINITE_LIFETIME))
    return infiniteLifetimePair();

  // "_new" calls return infinite lifetime. Why?
  //  * the result of 'new' is currently user-managed
  //  * ref fields are not supported in classes
  //    so they can't capture a ref argument
  if (isClassLike(call->typeInfo()) &&
      calledFn->hasFlag(FLAG_NEW_WRAPPER)) {
    return infiniteLifetimePair();
  }

  LifetimePair minLifetime = unknownLifetimePair();

  minLifetime.referent.relevantExpr = call;
  minLifetime.borrowed.relevantExpr = call;

  bool returnsRef = call->isRef() ||
                    calledFn->retTag == RET_REF ||
                    calledFn->retTag == RET_CONST_REF;

  bool returnsBorrow = false;
  Type* returnType = calledFn->retType;
  if (calledFn->isMethod() && calledFn->name == astrInit) {
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

    LifetimePair argLifetime = unknownLifetimePair();

    if (formalArgumentDoesNotImpactReturnLifetime(formal))
      continue;

    if (returnsRef && formal->isRef() &&
        (isSubjectToRefLifetimeAnalysis(actualSym) ||
         isLocalVariable(call->getFunction(), actualSym))) {

      // Use the referent part of the actual's lifetime
      LifetimePair temp = lifetimeForActual(actualSym, returnsRef, returnsBorrow, inFn);
      argLifetime.referent = temp.referent;
    }

    if (returnsBorrow && isSubjectToBorrowLifetimeAnalysis(actualSym)) {
      bool infer = true;

      if (isCallToFunctionReturningNotOwned(call)) {
        // yep, infer; behave as a borrow even if record contains only owned.
      } else if (!returnsRef && !isOrContainsBorrowedClass(returnType)) {
        // If the call returns a record by value, and that record
        // does not contain borrows (but presumably does contain owned),
        // do not infer the borrow lifetime.
        infer = false;
      }

      if (infer) {
        // Use the borrowed part of the actual's lifetime
        LifetimePair temp = lifetimeForActual(actualSym, returnsRef,
                                              returnsBorrow, inFn);
        argLifetime.borrowed = temp.borrowed;
      }
    }

    minLifetime = minimumLifetimePair(minLifetime, argLifetime);
  }

  return minLifetime;
}

LifetimePair LifetimeState::inferredLifetimeForPrimitive(CallExpr* call) {

  FnSymbol* inFn = call->getFunction();
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
    LifetimePair argLifetime = unknownLifetimePair();

    LifetimePair temp = lifetimeForActual(actualSym, returnsRef, returnsBorrow, inFn);

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

    argLifetime.referent.relevantExpr = call;
    argLifetime.borrowed.relevantExpr = call;
    return argLifetime;
  }

  LifetimePair minLifetime = unknownLifetimePair();

  minLifetime.referent.relevantExpr = call;
  minLifetime.borrowed.relevantExpr = call;

  for_actuals(actual, call) {
    SymExpr* actualSe = toSymExpr(actual);
    INT_ASSERT(actualSe);
    Symbol* actualSym = actualSe->symbol();
    LifetimePair argLifetime = unknownLifetimePair();

    if (returnsRef &&
        (isSubjectToRefLifetimeAnalysis(actualSym) ||
         isLocalVariable(call->getFunction(), actualSym))) {
      // Use the referent part of the actual's lifetime
      LifetimePair temp = lifetimeForActual(actualSym, returnsRef, returnsBorrow, inFn);
      argLifetime.referent = temp.referent;
    }
    if (returnsBorrow &&
        isSubjectToBorrowLifetimeAnalysis(actualSym)) {
      // Use the borrowed part of the actual's lifetime
      LifetimePair temp = lifetimeForActual(actualSym, returnsRef, returnsBorrow, inFn);
      argLifetime.borrowed = temp.borrowed;
    }

    minLifetime = minimumLifetimePair(minLifetime, argLifetime);
  }

  return minLifetime;
}

LifetimePair LifetimeState::inferredLifetimeForExpr(Expr* rhsExpr, bool usedAsRef, bool usedAsBorrow) {

  LifetimePair lp = unknownLifetimePair();

  lp.referent.relevantExpr = rhsExpr;
  lp.borrowed.relevantExpr = rhsExpr;

  // Consider the RHS and handle the cases
  CallExpr* rhsCallExpr = toCallExpr(rhsExpr);

  if (rhsCallExpr) {
    if (rhsCallExpr->resolvedOrVirtualFunction()) {
      lp = inferredLifetimeForCall(rhsCallExpr);
    } else {
      // Includes propagating refs across PRIM_ADDR_OF/PRIM_SET_REFERENCE
      lp = inferredLifetimeForPrimitive(rhsCallExpr);
    }
  } else {
    FnSymbol* inFn = rhsExpr->getFunction();

    SymExpr* rhsSe = toSymExpr(rhsExpr);
    INT_ASSERT(rhsSe);
    Symbol* rhs = getCanonicalSymbol(rhsSe->symbol());

    lp = lifetimeForActual(rhs, usedAsRef, usedAsBorrow, inFn);
  }

  return lp;
}

static void addSymbolToDetempGroup(Symbol* sym, DetempGroup* group) {
  group->elements.push_back(sym);

  Symbol* oldFavorite = group->favorite;

  bool preferSym = false;
  // Should we prefer sym to oldFavorite?
  if (oldFavorite == NULL)
    preferSym = true;
  else if (isGlobal(sym) != isGlobal(oldFavorite))
    preferSym = isGlobal(sym);
  else if (sym->hasFlag(FLAG_TEMP) != oldFavorite->hasFlag(FLAG_TEMP))
    preferSym = !sym->hasFlag(FLAG_TEMP);
  else if (sym->isRef() != oldFavorite->isRef())
    preferSym = !sym->isRef();
  else
    preferSym = (strlen(sym->name) < strlen(oldFavorite->name));

  if (preferSym)
    group->favorite = sym;
}

static void addPairToDetempMap(Symbol* a, Symbol* b,
                               SymbolToDetempGroupMap& map) {
  // Is a in the map? Is b in the map?
  bool aInMap = map.count(a) > 0;
  bool bInMap = map.count(b) > 0;

  DetempGroup* group = NULL;
  if (!aInMap && !bInMap) {
    // Create a new map entry for both of them.
    group = new DetempGroup();
    addSymbolToDetempGroup(a, group);
    addSymbolToDetempGroup(b, group);
    // Map each symbol to the group containing it
    map[a] = group;
    map[b] = group;
  } else if (aInMap && !bInMap) {
    // Add to a's map entry
    group = map[a];
    addSymbolToDetempGroup(b, group);
    map[b] = group;
  } else if (bInMap && !aInMap) {
    // Add to b's map entry
    group = map[b];
    addSymbolToDetempGroup(a, group);
    map[a] = group;
  } else {
    // Both are in the map, so join the groups
    group = map[a];
    DetempGroup* other = map[b];
    if (group == other) return; // do nothing if they already refer to same

    // This intentionally copies the elements to a local buffer
    std::vector<Symbol*> elts = other->elements;
    // Move each element to the group
    for (std::vector<Symbol*>::iterator it = elts.begin();
         it != elts.end();
         ++it) {
      Symbol* value = *it;
      addSymbolToDetempGroup(value, group);
      map[value] = group;
    }
    INT_ASSERT(map[b] == group); // should have been done by above code
    delete other;
  }
}

bool GatherTempsVisitor::enterCallExpr(CallExpr* call) {
  if (call->isPrimitive(PRIM_MOVE)) {
    SymExpr* lhsSe = toSymExpr(call->get(1));
    INT_ASSERT(lhsSe);
    Symbol* lhs = lhsSe->symbol();

    // don't de-temp the return value
    if (lhs->hasFlag(FLAG_RVV))
      return false;

    Symbol* a = NULL;
    Symbol* b = NULL;

    if (SymExpr* rhsSe = toSymExpr(call->get(2))) {
      Symbol* rhs = rhsSe->symbol();
      if (rhs->hasFlag(FLAG_TEMP) || lhs->hasFlag(FLAG_TEMP)) {
        a = lhs;
        b = rhs;
      }
    } else if (CallExpr* rhsCall = toCallExpr(call->get(2))) {
      if (rhsCall->isPrimitive(PRIM_ADDR_OF) ||
          rhsCall->isPrimitive(PRIM_SET_REFERENCE)) {
        SymExpr* rhsSe = toSymExpr(rhsCall->get(1));
        Symbol* rhs = rhsSe->symbol();
        if (lhs->hasEitherFlag(FLAG_TEMP,FLAG_REF_TEMP)) {
          a = lhs;
          b = rhs;
        }
      }
    }

    if (a && b) {
      addPairToDetempMap(a, b, lifetimes->detemp);
      lifetimes->callsToIgnore.insert(call);
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
    if (calledFn->isMethod() && calledFn->name == astrInit) {
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

bool IntrinsicLifetimesVisitor::enterDefExpr(DefExpr* def) {

  Symbol* sym = lifetimes->getCanonicalSymbol(def->sym);

  // Start with a lifetime already determined, in case we
  // visit the same one more than once due to canonicalizing temps
  LifetimePair lp = unknownLifetimePair();
  if (lifetimes->intrinsicLifetime.count(sym))
    lp = lifetimes->intrinsicLifetime[sym];

  if (ArgSymbol* arg = toArgSymbol(sym)) {

    // For arguments, set the lifetime == scope for symbol
    Lifetime lt = scopeLifetimeForSymbol(sym);

    lp.referent = lt;
    lp.borrowed = lt;

    if (arg->hasFlag(FLAG_RETURN_SCOPE)) {
      if (sym->isRef() &&
          !intentIsLocalVariable(arg->intent) &&
          !intentIsLocalVariable(arg->originalIntent))
        lp.referent.returnScope = true;
      if (isSubjectToBorrowLifetimeAnalysis(sym->type))
        lp.borrowed.returnScope = true;
    }
  } else if (VarSymbol* var = toVarSymbol(sym)) {
    // Don't bother getting intrinsic lifetime for RVV
    if (var->hasFlag(FLAG_RVV))
        lp = unknownLifetimePair();

    // For local value variables, set ref lifetime to reachability.
    if (!var->isRef() && isLocalVariable(visitingFn, var)) {
      lp.referent = scopeLifetimeForSymbol(sym);
    }
    // Otherwise, the ref variable doesn't have an intrinsic lifetime
    // (but it will have an inferred lifetime).

    // For local variables containing a borrow,
    // we need an error if variable exists longer than a borrow.
    //
    // Records are handled in the enterCallExpr visitor for
    // wherever they are initialized.
    if (isSubjectToBorrowLifetimeAnalysis(sym->type) &&
        !isRecord(sym->getValType()) &&
        !sym->hasFlag(FLAG_TEMP) &&
        !sym->hasFlag(FLAG_ARG_THIS) // for constructors
       ) {
      lp.borrowed = scopeLifetimeForSymbol(sym);
    }
  }

  if (!lp.referent.unknown || lp.borrowed.unknown) {

    // Handle types that should have infinite borrow lifetime
    if (sym->type && typeHasInfiniteBorrowLifetime(sym->type))
      lp.borrowed = infiniteLifetime();

    if (sym->id == debugLifetimesForId)
      gdbShouldBreakHere();

    lifetimes->intrinsicLifetime[sym] = lp;
  }

  return false;
}

static bool isCallToFunctionReturningNotOwned(CallExpr* call) {

  FnSymbol* calledFn = call->resolvedOrVirtualFunction();
  if (calledFn &&
      calledFn->hasEitherFlag(FLAG_RETURN_NOT_OWNED,
                              FLAG_RETURNS_ALIASING_ARRAY))
    return true;

  return false;
}


bool IntrinsicLifetimesVisitor::enterCallExpr(CallExpr* call) {

  if (call->id == debugLifetimesForId)
    gdbShouldBreakHere();

  // Ignore calls that de-temping allows us to ignore
  if (lifetimes->callsToIgnore.count(call))
    return false;

  Symbol* initSym = NULL;
  CallExpr* initCall = NULL;

  Lifetime lt = unknownLifetime();

  if (isRecordInitOrReturn(call, initSym, initCall, lifetimes) &&
      shouldPropagateLifetimeTo(call, initSym)) {

    AggregateType* at = toAggregateType(initSym->getValType());
    INT_ASSERT(at && isRecord(at));

    // Start with a lifetime already determined, in case we
    // visit the same one more than once due to canonicalizing temps
    if (lifetimes->intrinsicLifetime.count(initSym))
      lt = lifetimes->intrinsicLifetime[initSym].borrowed;

    if (recordContainsOwnedClassFields(at)) {
      if (isCallToFunctionReturningNotOwned(initCall)) {
        // leave it unknown.
      } else {
        // set it to this variable's reachability, under the assumption that
        // it will be destroyed when it goes out of scope.
        lt = scopeLifetimeForSymbol(initSym);
      }
    }

  }

  if (call->isPrimitive(PRIM_MOVE)) {
    SymExpr* lhsSe = toSymExpr(call->get(1));
    Expr* rhsExpr = call->get(2);
    Symbol* lhs = lhsSe->symbol();
    CallExpr* rhsCall = toCallExpr(rhsExpr);
    initSym = lifetimes->getCanonicalSymbol(lhs);

    if (initSym->hasFlag(FLAG_TEMP) &&
        rhsCall &&
        shouldPropagateLifetimeTo(rhsCall, initSym)) {
      FnSymbol* calledFn = rhsCall->resolvedOrVirtualFunction();
      // "_new" calls return infinite lifetime. Why?
      //  * the result of 'new' is currently user-managed
      //  * ref fields are not supported in classes
      //    so they can't capture a ref argument
      // This code is here to handle the case that the result of
      // the new is stored into a temp.
      if (isClassLike(rhsCall->typeInfo()) && calledFn &&
          calledFn->hasFlag(FLAG_NEW_WRAPPER)) {

        // Start with a lifetime already determined, in case we
        // visit the same one more than once due to canonicalizing temps
        if (lifetimes->intrinsicLifetime.count(initSym))
          lt = lifetimes->intrinsicLifetime[initSym].borrowed;

        lt = infiniteLifetime();
      }
    }
    // note: the inference visitor does set intrinsic lifetimes
    // when they depend on inferred lifetimes.
  }

  if (initSym && !lt.unknown) {

    if (initSym->id == debugLifetimesForId)
      gdbShouldBreakHere();

    lifetimes->intrinsicLifetime[initSym].borrowed = lt;
  }

  return false;
}


bool InferLifetimesVisitor::enterCallExpr(CallExpr* call) {

  if (call->id == debugLifetimesForId)
    gdbShouldBreakHere();

  // Ignore calls that de-temping allows us to ignore
  if (lifetimes->callsToIgnore.count(call))
    return false;

  Symbol* initSym = NULL;
  CallExpr* initCall = NULL;
  if (isRecordInitOrReturn(call, initSym, initCall, lifetimes) &&
      shouldPropagateLifetimeTo(call, initSym)) {

    AggregateType* at = toAggregateType(initSym->getValType());
    INT_ASSERT(at && isRecord(at));
    LifetimePair lp = unknownLifetimePair();

    if (recordContainsBorrowedClassFields(at) ||
        isCallToFunctionReturningNotOwned(initCall)) {
      lp = lifetimes->inferredLifetimeForCall(initCall);
    }
    changed |= lifetimes->setInferredLifetimeToMin(initSym, lp);

  } else if (isAnalyzedMoveOrAssignment(call)) {

    SymExpr* lhsSe = toSymExpr(call->get(1));
    Symbol* lhs = lifetimes->getCanonicalSymbol(lhsSe->symbol());

    if (shouldPropagateLifetimeTo(call, lhs)) {

      Expr* rhsExpr = call->get(2);

      bool usedAsRef = lhs->isRef() && rhsExpr->isRef();
      bool usedAsBorrow = isOrContainsBorrowedClass(lhs->type);

      LifetimePair lp = lifetimes->inferredLifetimeForExpr(rhsExpr,
                                                           usedAsRef,
                                                           usedAsBorrow);

      if (lhs->isRef() && rhsExpr->isRef()) {
        // When setting the reference, set its intrinsic lifetime.
        if (!lp.referent.unknown || !lp.borrowed.unknown) {
          if (lhs->id == debugLifetimesForId)
            gdbShouldBreakHere();

          LifetimePair & intrinsic = lifetimes->intrinsicLifetime[lhs];
          intrinsic = minimumLifetimePair(intrinsic, lp);
        }
      }

      if (!(lhs->isRef() && rhsExpr->isRef()))
        // lhs can't have ref lifetime if it isn't a ref
        lp.referent = unknownLifetime();

      if (!isOrContainsBorrowedClass(lhs->type))
        // lhs can't have borrow lifetime if it can't borrow
        lp.borrowed = unknownLifetime();

      lp.referent.relevantExpr = call;
      lp.borrowed.relevantExpr = call;
      changed |= lifetimes->setInferredLifetimeToMin(lhs, lp);
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

  //  1. default: the lifetime of a method that is an iterator
  //     is the lifetime of the receiver
  //  2. default: the lifetime of a non-method iterator
  //     is the loop itself.
  //     TODO: Or check if the iterator may own?

  FnSymbol* inFn = forLoop->getFunction();
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

    bool method = true;

    // Also check if we are iterating using these() method
    // ex. functions/ferguson/ref-pair/const-error-iterated*
    if (!iterableSe) {
      if (CallExpr* iterableCall = toCallExpr(iterable)) {
        if (iterableCall->isNamed("these")) {
          if (iterableCall->numActuals() >= 1) {
            iterableSe = toSymExpr(iterableCall->get(1));
            iterable = iterableSe;
          }
        }
      }
    }

    // Figure out if the iterator is a method or not.
    // gatherLoopDetails currently can return a user type (e.g. a record)
    // as the iterable in the event that 'these' is called.
    // For that reason, if we can't find the iterator anywhere, we
    // assume it's a method.
    if (iterable != NULL)
      if (AggregateType* at = toAggregateType(iterable->getValType()))
        if (at->iteratorInfo != NULL)
          if (FnSymbol* fn = getTheIteratorFn(at))
            method = (fn->_this != NULL);

    bool usedAsRef = index->isRef();
    bool usedAsBorrow = isOrContainsBorrowedClass(index->type);

    // Gather lifetime for iterable
    LifetimePair lp = unknownLifetimePair();
    if (iterableSe) {
      Symbol* iterableSym = iterableSe->symbol();

      lp = lifetimes->lifetimeForActual(iterableSym,
                                        usedAsRef, usedAsBorrow, inFn);

      if (!isSubjectToRefLifetimeAnalysis(iterableSym))
        lp.referent = unknownLifetime();
      if (!isSubjectToBorrowLifetimeAnalysis(iterableSym))
        lp.borrowed = unknownLifetime();

    } else if (CallExpr* iterableCall = toCallExpr(iterable)) {
      if (iterableCall->resolvedOrVirtualFunction())
        lp = lifetimes->inferredLifetimeForCall(iterableCall);
      else
        lp = lifetimes->inferredLifetimeForPrimitive(iterableCall);

    } else {
      // Assume infinite lifetime, as iterable takes no arguments
      // (e.g. enum value iteration)
      lp = infiniteLifetimePair();
    }

    // Set intrinsic lifetime for index here at it's definition point
    if (index->isRef()) {
      if (!lp.referent.unknown || !lp.borrowed.unknown) {
        if (index->id == debugLifetimesForId)
          gdbShouldBreakHere();

        LifetimePair & intrinsic = lifetimes->intrinsicLifetime[index];
        intrinsic = minimumLifetimePair(intrinsic, lp);
      }
    }

    // Set lifetime of iteration variable to lifetime of the iterable (expr).
    lp.referent.relevantExpr = forLoop;
    lp.borrowed.relevantExpr = forLoop;

    if (method == false) {
      LifetimePair loopScope = lifetimes->intrinsicLifetimeForSymbol(index);
      lp = minimumLifetimePair(lp, loopScope);
    }

    changed |= lifetimes->setInferredLifetimeToMin(index, lp);
  }

  return true;
}

static bool isDevOnly(BaseAST* ast) {
  FnSymbol* fn = ast->getFunction();
  ModuleSymbol* mod = ast->getModule();

  if (mod->modTag == MOD_INTERNAL) {
    return true;
  } else if (fn &&
             fn->hasFlag(FLAG_LINE_NUMBER_OK) == false &&
             fn->hasFlag(FLAG_COMPILER_GENERATED)) {
    return true;
  }

  return false;
}
static bool isUser(BaseAST* ast) {
  return !isDevOnly(ast);
}

static BaseAST* findUserPlace(BaseAST* ast) {
  if (developer)
    return ast;

  if (isUser(ast))
    return ast;

  // Otherwise, look at the call sites to find
  // the user call.
  // Note that instantiation points are not available
  // at this point in compilation.
  // This only goes 1 level up. Doing more than that
  // would have to detect recursion, and at that point
  // there's probably a better approach.
  if (FnSymbol* inFn = ast->getFunction()) {
    for_SymbolSymExprs(se, inFn) {
      if (isUser(se)) {
        ast = se;
      }
    }
  }

  // If we can't do any better, just return the internal place.
  return ast;
}

static void emitError(Expr* inExpr,
                      const char* msg1, const char* msg2,
                      Symbol* relevantSymbol,
                      Lifetime relevantLifetime,
                      LifetimeState* lifetimes) {

  char buf[256];

  BaseAST* place = findUserPlace(inExpr);

  if (relevantSymbol && !relevantSymbol->hasFlag(FLAG_TEMP)) {
    snprintf(buf, sizeof(buf), "%s %s %s", msg1, relevantSymbol->name, msg2);
    USR_FATAL_CONT(place, buf);
  } else {
    snprintf(buf, sizeof(buf), "%s %s", msg1, msg2);
    USR_FATAL_CONT(place, buf);
  }

  Symbol* fromSym = relevantLifetime.fromSymbolScope;
  Expr* fromExpr = relevantLifetime.relevantExpr;
  if (fromSym && !fromSym->hasFlag(FLAG_TEMP) && isUser(fromSym))
    USR_PRINT(fromSym, "consider scope of %s", fromSym->name);
  else if (fromExpr && inExpr->astloc != fromExpr->astloc && isUser(fromExpr))
    USR_PRINT(fromExpr, "consider result here");

  handleDebugOutputOnError(inExpr, lifetimes);
}


bool EmitLifetimeErrorsVisitor::enterCallExpr(CallExpr* call) {

  if (call->id == debugLifetimesForId)
    gdbShouldBreakHere();

  // Ignore calls that de-temping allows us to ignore
  if (lifetimes->callsToIgnore.count(call))
    return false;

  if (isAnalyzedMoveOrAssignment(call)) {

    SymExpr* lhsSe = toSymExpr(call->get(1));
    Symbol* lhs = lhsSe->symbol();

    if (isSubjectToRefLifetimeAnalysis(lhs) ||
        isSubjectToBorrowLifetimeAnalysis(lhs)) {

      if (lhs->hasEitherFlag(FLAG_RVV,FLAG_RETARG)) {
        emitBadReturnErrors(call);
      } else {
        emitBadAssignErrors(call);
      }
    }
  } else if (call->isPrimitive(PRIM_SET_MEMBER)) {

    Symbol* field = toSymExpr(call->get(2))->symbol();

    if (isSubjectToRefLifetimeAnalysis(field) ||
        isSubjectToBorrowLifetimeAnalysis(field)) {

      emitBadSetFieldErrors(call);
    }
  }

  return false;
}

void EmitLifetimeErrorsVisitor::emitBadReturnErrors(CallExpr* call) {

  SymExpr* lhsSe = toSymExpr(call->get(1));
  Symbol* lhs = lhsSe->symbol();
  Expr* rhsExpr = call->get(2);

  FnSymbol* inFn = call->getFunction();

  // track rhs symbol for error messages
  Symbol* rhsSym = NULL;
  if (SymExpr* rhsSe = toSymExpr(rhsExpr)) {
    rhsSym = lifetimes->getCanonicalSymbol(rhsSe->symbol());
  }

  bool usedAsRef = lhs->isRef() && rhsExpr->isRef();

  LifetimePair rhsLt = lifetimes->inferredLifetimeForExpr(rhsExpr,
                                                          usedAsRef,
                                                          false);

  // Check returns
  if (lhs->isRef() && call->isPrimitive(PRIM_MOVE)) {
    // check returning a reference
    if (rhsLt.referent.infinite || rhsLt.referent.unknown) {
      // OK, not an error
    } else {
      if (inFn &&
          inFn->hasFlag(FLAG_RETURNS_INFINITE_LIFETIME) &&
          rhsLt.referent.returnScope) {
        emitError(call,
                  "Reference to return scoped variable",
                  "cannot be returned in function "
                  "returning infinite lifetime",
                  rhsSym, rhsLt.referent, lifetimes);
      } else if (!rhsLt.referent.returnScope) {
        emitError(call,
                  "Reference to scoped variable",
                  "cannot be returned",
                  rhsSym, rhsLt.referent, lifetimes);
      }
    }
  }

  if (isSubjectToBorrowLifetimeAnalysis(lhs->type)) {
    // You might think this should be isOrContainsBorrowedClass
    // but we also allow owned classes so that we can handle
    // cases like array slices, where the function says to borrow
    // (vs own).

    // check returning a borrow
    if (rhsLt.borrowed.infinite || rhsLt.borrowed.unknown) {
      // OK, not an error
    } else {
      if (inFn &&
          inFn->hasFlag(FLAG_RETURNS_INFINITE_LIFETIME) &&
          rhsLt.borrowed.returnScope) {
        emitError(call,
                  "Return scope variable",
                  "cannot be returned in function "
                  "returning infinite lifetime",
                  rhsSym, rhsLt.borrowed, lifetimes);
      } else if (!rhsLt.borrowed.returnScope) {
        emitError(call,
                  "Scoped variable",
                  "cannot be returned",
                  rhsSym, rhsLt.borrowed, lifetimes);
      }
    }
  }
}

void EmitLifetimeErrorsVisitor::emitBadAssignErrors(CallExpr* call) {

  SymExpr* lhsSe = toSymExpr(call->get(1));
  Symbol* lhs = lhsSe->symbol();
  Expr* rhsExpr = call->get(2);

  LifetimePair lhsInferred = lifetimes->inferredLifetimeForSymbol(lhs);
  LifetimePair lhsIntrinsic = lifetimes->intrinsicLifetimeForSymbol(lhs);

  bool usedAsRef = lhs->isRef() && rhsExpr->isRef();
  bool usedAsBorrow = isOrContainsBorrowedClass(lhs->type);


  LifetimePair rhsLt = lifetimes->inferredLifetimeForExpr(rhsExpr,
                                                          usedAsRef,
                                                          usedAsBorrow);

  // Raise errors for init/assigning from a value with shorter lifetime
  // I.e. insist RHS lifetime is longer than LHS lifetime.
  // I.e. error if RHS lifetime is shorter than LHS lifetime.
  if (call->isPrimitive(PRIM_MOVE) && lhs->isRef() &&
      !lhs->hasFlag(FLAG_INDEX_VAR) // workaround for forall loop AST
     ) {
    // Setting a reference so check ref lifetimes
    if (lhsIntrinsic.referent.unknown) {
      // OK, not an error
    } else if (isLifetimeShorter(rhsLt.referent, lhsIntrinsic.referent)) {
      emitError(call,
                "Reference to scoped variable",
                "would outlive the value it refers to",
                lhs, rhsLt.referent, lifetimes);
      erroredSymbols.insert(lhs);
    } else if (lhsInferred.referent.unknown ||
               lhsInferred.referent.infinite) {
      // OK, not an error
    } else if (isLifetimeShorter(rhsLt.referent, lhsInferred.referent)) {
      emitError(call,
                "Reference to scoped variable",
                "would outlive the value it refers to",
                lhs, rhsLt.referent, lifetimes);
      erroredSymbols.insert(lhs);
    }
  }

  if (isOrContainsBorrowedClass(lhs->type)) {
    // setting a borrow, so check borrow lifetimes
    if (lhsIntrinsic.borrowed.unknown) {
      // OK, not an error
    } else if (isLifetimeShorter(rhsLt.borrowed, lhsIntrinsic.borrowed)) {
      emitError(call,
                "Scoped variable",
                "would outlive the value it is set to",
                lhs, rhsLt.borrowed, lifetimes);
      erroredSymbols.insert(lhs);
    } else if (lhsInferred.borrowed.unknown ||
               lhsInferred.borrowed.infinite) {
      // OK, not an error
    } else if (isLifetimeShorter(rhsLt.borrowed, lhsInferred.borrowed)) {
      emitError(call,
                "Scoped variable",
                "would outlive the value it is set to",
                lhs, rhsLt.borrowed, lifetimes);
      erroredSymbols.insert(lhs);
    }
  }
}

void EmitLifetimeErrorsVisitor::emitBadSetFieldErrors(CallExpr* call) {

  Symbol* lhs = toSymExpr(call->get(1))->symbol();
  Symbol* field = toSymExpr(call->get(2))->symbol();
  Expr* rhsExpr = call->get(3);

  LifetimePair lhsInferred = lifetimes->inferredLifetimeForSymbol(lhs);
  LifetimePair lhsIntrinsic = lifetimes->intrinsicLifetimeForSymbol(lhs);
  bool usedAsRef = lhs->isRef() && rhsExpr->isRef();
  bool usedAsBorrow = isOrContainsBorrowedClass(field->type);

  LifetimePair rhsLt = lifetimes->inferredLifetimeForExpr(rhsExpr,
                                                          usedAsRef,
                                                          usedAsBorrow);

  if (field->isRef() && rhsExpr->isRef()) {
    // Setting a reference so check ref lifetimes
    if (lhsIntrinsic.referent.unknown) {
      // OK, not an error
    } else if (isLifetimeShorter(rhsLt.referent, lhsIntrinsic.referent)) {
      emitError(call,
                "Reference field",
                "would outlive the value it refers to",
                field, rhsLt.referent, lifetimes);
      erroredSymbols.insert(lhs);
    } else if (lhsInferred.referent.unknown ||
               lhsInferred.referent.infinite) {
      // OK, not an error
    } else if (isLifetimeShorter(rhsLt.referent, lhsInferred.referent)) {
      emitError(call,
                "Reference field",
                "would outlive the value it refers to",
                field, rhsLt.referent, lifetimes);
      erroredSymbols.insert(lhs);
    }
  }

  if (isOrContainsBorrowedClass(field->type)) {
    // setting a borrow, so check borrow lifetimes
    if (lhsIntrinsic.borrowed.unknown) {
      // OK, not an error
    } else if (isLifetimeShorter(rhsLt.borrowed, lhsIntrinsic.borrowed)) {
      emitError(call,
                "Field",
                "would outlive the value it is set to",
                field, rhsLt.borrowed, lifetimes);
      erroredSymbols.insert(lhs);
    } else if (lhsInferred.borrowed.unknown ||
               lhsInferred.borrowed.infinite) {
      // OK, not an error
    } else if (isLifetimeShorter(rhsLt.borrowed, lhsInferred.borrowed)) {
      emitError(call,
                "Field",
                "would outlive the value it is set to",
                field, rhsLt.borrowed, lifetimes);
      erroredSymbols.insert(lhs);
    }
  }
}


void EmitLifetimeErrorsVisitor::emitErrors() {
  for (SymbolToLifetimeMap::iterator it = lifetimes->inferredLifetime.begin();
       it != lifetimes->inferredLifetime.end();
       ++it) {
    Symbol* key = it->first;
    LifetimePair inferred = it->second;

    LifetimePair intrinsic = lifetimes->intrinsicLifetimeForSymbol(key);

    // Ignore the RVV for this check since that's tested in acceptCall
    // (see test lifetimes/bug-like-timezones.chpl)
    if (key->hasEitherFlag(FLAG_RVV,FLAG_RETARG))
      continue;

    // Don't emit errors again when we already made an error for that symbol
    if (erroredSymbols.count(key))
      continue;

    if (key->isRef() &&
        !inferred.referent.unknown &&
        !intrinsic.referent.unknown &&
        isLifetimeShorter(inferred.referent, intrinsic.referent)) {
      Expr* at = key->defPoint;
      if (inferred.referent.relevantExpr)
        at = inferred.referent.relevantExpr;

      emitError(at,
                "Reference to scoped variable",
                "reachable after lifetime ends",
                key, inferred.referent, lifetimes);
    }

    if (isOrContainsBorrowedClass(key->type) &&
        !inferred.borrowed.unknown &&
        !intrinsic.borrowed.unknown &&
        isLifetimeShorter(inferred.borrowed, intrinsic.borrowed)) {
      Expr* at = key->defPoint;
      if (inferred.borrowed.relevantExpr)
        at = inferred.borrowed.relevantExpr;

      emitError(at,
                "Scoped variable",
                "reachable after lifetime ends",
                key, inferred.borrowed, lifetimes);
    }

    // check refs of borrows
    if (isOrContainsBorrowedClass(key->type) &&
        !inferred.borrowed.unknown &&
        !inferred.referent.unknown &&
        isLifetimeShorter(inferred.borrowed, inferred.referent)) {
      Expr* at = key->defPoint;
      if (inferred.borrowed.relevantExpr)
        at = inferred.borrowed.relevantExpr;

      emitError(at,
                "reference points to scoped variable",
                "reachable after lifetime ends",
                key, inferred.borrowed, lifetimes);
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

// A variables with a type with `infinite borrow lifetime` should have
// infinite intrinsic borrow lifetime.
static bool typeHasInfiniteBorrowLifetime(Type* type) {

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
         (!typeHasInfiniteBorrowLifetime(field->type) &&
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
    if (isOrContainsBorrowedClass(field->type) &&
        !field->hasFlag(FLAG_UNSAFE) &&
        !field->hasFlag(FLAG_OWNED))
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

static bool isOrContainsBorrowedClass(Type* type) {
  type = type->getValType();

  // First check for it is a borrowed class type
  if (isClassLike(type) &&
      !typeHasInfiniteBorrowLifetime(type))
    return true; // borrow -- `unmanaged` has `infinite lifetime`

  // Next check for records containing borrowed class type
  if (AggregateType* at = toAggregateType(type))
    if (at->isRecord())
      return recordContainsBorrowedClassFields(at);

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
  if (calledFn && calledFn->name == astrSequals /* "=" */ ) {
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
  if (a.unknown) // a unknown, b unknown or not
    return false;
  else if (b.unknown) // a not unknown, b unknown
    return true;
  if (a.infinite) // a infinite, b infinite or not
    return false;
  else if (b.infinite) // a not infinite, b infinite
    return true;
  if (a.returnScope && !b.returnScope) // a return scope, b not
    return false;
  else if (!a.returnScope && b.returnScope) // a not return scope, b is
    return true;
  else if (a.fromSymbolScope == b.fromSymbolScope)
    return false;
  else {
    Symbol* aSym = a.fromSymbolScope;
    Symbol* bSym = b.fromSymbolScope;
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


static LifetimePair minimumLifetimePair(LifetimePair a, LifetimePair b) {
  LifetimePair ret;
  ret.referent = minimumLifetime(a.referent, b.referent);
  ret.borrowed = minimumLifetime(a.borrowed, b.borrowed);
  return ret;
}

static Lifetime scopeLifetimeForSymbol(Symbol* sym) {
  Lifetime lt;
  lt.fromSymbolScope = sym;
  lt.relevantExpr = NULL;
  lt.unknown = false;
  lt.infinite = symbolHasInfiniteLifetime(sym);
  lt.returnScope = false;
  return lt;
}

static Lifetime unknownLifetime() {
  Lifetime lt;
  lt.fromSymbolScope = NULL;
  lt.relevantExpr = NULL;
  lt.unknown = true;
  lt.infinite = false;
  lt.returnScope = false;
  return lt;
}

static Lifetime infiniteLifetime() {
  Lifetime lt;
  lt.fromSymbolScope = NULL;
  lt.relevantExpr = NULL;
  lt.unknown = false;
  lt.infinite = true;
  lt.returnScope = false;
  return lt;
}

static LifetimePair unknownLifetimePair() {
  Lifetime lt = unknownLifetime();
  LifetimePair ret;
  ret.referent = lt;
  ret.borrowed = lt;
  return ret;
}

static LifetimePair infiniteLifetimePair() {
  Lifetime lt = infiniteLifetime();
  LifetimePair ret;
  ret.referent = lt;
  ret.borrowed = lt;
  return ret;
}
