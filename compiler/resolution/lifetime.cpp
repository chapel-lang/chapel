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

/* This file implements lifetime checking.

   * pointer/ref arguments are inferred to be return scope
   * A value containing indirections and with a non-infinite inferred
     lifetime cannot be returned from a function
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
 */

const char* debugLifetimesForFn = "";
const int debugLifetimesForId = 0;
const bool defaultToCheckingLifetimes = false;
const bool debugOutputOnError = false;

namespace {

  struct Lifetime {
    // The lifetime is defined by:
    //   - the type of the symbol (is it a ref/ptr at all?)
    //   - where it is defined
    Symbol* fromSymbolReachability;
    Expr* relevantExpr;
    bool infinite;
    bool returnScope;
  };

  typedef std::map<Symbol*,Lifetime> SymbolToLifetimeMap;
  typedef std::map<CallExpr*,Lifetime> ReturnToLifetimeMap;

  struct LifetimeState {
    SymbolToLifetimeMap lifetimes;
    bool setLifetimeForSymbolToMin(Symbol* sym, Lifetime lt);
    Lifetime lifetimeForSymbol(Symbol* sym);
    Lifetime lifetimeForCallReturn(CallExpr* call);
    Lifetime lifetimeForPrimitiveReturn(CallExpr* call);
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
static bool isSubjectToLifetimeAnalysis(Symbol* sym);
static bool shouldPropagateLifetimeTo(FnSymbol* inFn, Symbol* sym);
static bool isAnalyzedMoveOrAssignment(CallExpr* call);
static bool symbolHasInfiniteLifetime(Symbol* sym);
static BlockStmt* getDefBlock(Symbol* sym);
static bool isBlockWithinBlock(BlockStmt* a, BlockStmt* b);
static bool isLifetimeShorter(Lifetime a, Lifetime b);
static Lifetime reachabilityLifetimeForSymbol(Symbol* sym);
static Lifetime infiniteLifetime();
static bool debuggingLifetimesForFn(FnSymbol* fn);
static void printLifetimeState(LifetimeState* state);
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

void printLifetimeState(LifetimeState* state)
{
  printf("Lifetime state:\n");
  for (SymbolToLifetimeMap::iterator it = state->lifetimes.begin();
       it != state->lifetimes.end();
       ++it) {
    Symbol* key = it->first;
    Lifetime value = it->second;

    printf("Symbol %s[%i] has lifetime ", key->name, key->id);
    if (value.infinite)
      printf("infinite ");
    if (value.returnScope)
      printf("return_scope ");
    if (value.fromSymbolReachability)
      printf("like %s[%i] ", value.fromSymbolReachability->name,
          value.fromSymbolReachability->id);
    printf("\n");
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
bool LifetimeState::setLifetimeForSymbolToMin(Symbol* sym, Lifetime lt) {
  bool changed = false;

  int breakOnId = debugLifetimesForId;

  if (lifetimes.count(sym) == 0) {
    if (sym->id == breakOnId)
      gdbShouldBreakHere();
    lifetimes[sym] = lt;
    changed = true;
  } else {
    if (isLifetimeShorter(lt, lifetimes[sym])) {
      if (sym->id == breakOnId)
        gdbShouldBreakHere();
      lifetimes[sym] = lt;
      changed = true;
    }
  }

  return changed;
}

Lifetime LifetimeState::lifetimeForSymbol(Symbol* sym) {
  if (lifetimes.count(sym) > 0)
    return lifetimes[sym];

  // If it has type nil, it has infinite lifetime
  if (sym->type == dtNil)
    return infiniteLifetime();

  // Otherwise, create a basic lifetime for this variable.
  return reachabilityLifetimeForSymbol(sym);
}

Lifetime LifetimeState::lifetimeForCallReturn(CallExpr* call) {
  Lifetime minLifetime = infiniteLifetime();

  FnSymbol* calledFn = call->resolvedOrVirtualFunction();
  if (calledFn->hasFlag(FLAG_RETURNS_INFINITE_LIFETIME))
    return infiniteLifetime();

  // Constructors and "_new" calls
  // return infinite lifetime. Why?
  //  * the result of 'new' is currently user-managed
  //  * ref fields are not supported in classes
  //    so records can't capture a ref argument
  if (isClass(call->typeInfo()) &&
      (calledFn->hasFlag(FLAG_CONSTRUCTOR) ||
      0 == strcmp("_new", calledFn->name))) {
    return infiniteLifetime();
  }

  // cannot use the lifetime of their arguments.
  for_formals_actuals(formal, actual, call) {
    SymExpr* actualSe = toSymExpr(actual);
    INT_ASSERT(actualSe);
    Symbol* actualSym = actualSe->symbol();

    if (isSubjectToLifetimeAnalysis(actualSym) ||
        (isLocalVariable(call->getFunction(), actualSym) &&
         isSubjectToLifetimeAnalysis(formal))) {
      Lifetime actualLifetime = lifetimeForSymbol(actualSym);

      if (formal->hasFlag(FLAG_RETURN_SCOPE))
        return actualLifetime;

      if (isLifetimeShorter(actualLifetime, minLifetime)) {
        minLifetime = actualLifetime;
      }
    }
  }

  // If no argument was marked with FLAG_RETURN_SCOPE,
  // just assume it's the minimum of the passed lifetimes.
  return minLifetime;
}

Lifetime LifetimeState::lifetimeForPrimitiveReturn(CallExpr* call) {
  Lifetime minLifetime = infiniteLifetime();

  for_actuals(actual, call) {
    SymExpr* actualSe = toSymExpr(actual);
    INT_ASSERT(actualSe);
    Symbol* actualSym = actualSe->symbol();

    if (isSubjectToLifetimeAnalysis(actualSym) ||
        isLocalVariable(call->getFunction(), actualSym)) {
      Lifetime actualLifetime = lifetimeForSymbol(actualSym);

      if (isLifetimeShorter(actualLifetime, minLifetime)) {
        minLifetime = actualLifetime;
      }
    }
  }

  // If no argument was marked with FLAG_RETURN_SCOPE,
  // just assume it's the minimum of the passed lifetimes.
  return minLifetime;
}


bool InferLifetimesVisitor::enterCallExpr(CallExpr* call) {

  if (call->id == debugLifetimesForId)
    gdbShouldBreakHere();

  if (isAnalyzedMoveOrAssignment(call)) {

    SymExpr* lhsSe = toSymExpr(call->get(1));
    Symbol* lhs = lhsSe->symbol();

    if (shouldPropagateLifetimeTo(call->getFunction(), lhs)) {

      // Consider the RHS and handle the cases
      Expr* rhsExpr = call->get(2);
      CallExpr* rhsCallExpr = toCallExpr(rhsExpr);
      Lifetime lt = infiniteLifetime();

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
        if (call->isPrimitive(PRIM_MOVE) ||
            isClass(se->symbol()->type)) {
          // Propagate lifetime for refs across assignment
          lt = lifetimes->lifetimeForSymbol(se->symbol());
        }
      }

      lt.relevantExpr = call;
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

    INT_ASSERT(iterable);
    INT_ASSERT(index);

    // Also check if we are iterating using these() method
    // ex. functions/ferguson/ref-pair/const-error-iterated*
    if (!iterableSe)
      if (CallExpr* iterableCall = toCallExpr(iterable))
        if (iterableCall->isNamed("these"))
          iterableSe = toSymExpr(iterableCall->get(1));

    // Gather lifetime for iterable
    Lifetime iterableLifetime = infiniteLifetime();
    if (iterableSe) {
      iterableLifetime = lifetimes->lifetimeForSymbol(iterableSe->symbol());
    } else if (CallExpr* iterableCall = toCallExpr(iterable)) {
      iterableLifetime = lifetimes->lifetimeForCallReturn(iterableCall);
    } else {
      INT_FATAL("Error finding iterable");
    }

    // Set lifetime of iteration variable to lifetime of the iterable (expr).
    iterableLifetime.relevantExpr = forLoop;
    changed |= lifetimes->setLifetimeForSymbolToMin(index, iterableLifetime);
  }

  return true;
}


bool EmitLifetimeErrorsVisitor::enterCallExpr(CallExpr* call) {

  FnSymbol* calledFn = call->resolvedOrVirtualFunction();

  if (isAnalyzedMoveOrAssignment(call)) {

    SymExpr* lhsSe = toSymExpr(call->get(1));
    Symbol* lhs = lhsSe->symbol();
    Lifetime lhsLt = lifetimes->lifetimeForSymbol(lhs);

    if (isSubjectToLifetimeAnalysis(lhs)) {

      // If we can't make sense of the RHS, assume it's infinite lifetime
      Lifetime rhsLt = infiniteLifetime();

      Symbol* rhsSym = NULL;

      if (SymExpr* rhsSe = toSymExpr(call->get(2))) {
        rhsSym = rhsSe->symbol();
        if (isSubjectToLifetimeAnalysis(rhsSym))
          rhsLt = lifetimes->lifetimeForSymbol(rhsSym);

      } else if (CallExpr* subCall = toCallExpr(call->get(2))) {
        if (subCall->resolvedOrVirtualFunction())
          rhsLt = lifetimes->lifetimeForCallReturn(subCall);
      }

      // Raise errors for returning a scoped/lifetime'd variable
      if (lhs->hasFlag(FLAG_RVV)) {
        if (!rhsLt.infinite) {
          if (!rhsLt.returnScope) {
            if (rhsSym && !rhsSym->hasFlag(FLAG_TEMP))
              USR_FATAL_CONT(call, "Scoped variable %s cannot be returned",
                             rhsSym->name);
            else
              USR_FATAL_CONT(call, "Scoped variable cannot be returned");

            Symbol* from = rhsLt.fromSymbolReachability;
            USR_PRINT(from, "consider scope of %s", from->name);
            handleDebugOutputOnError(call, lifetimes);
          }
          if (calledFn &&
              calledFn->hasFlag(FLAG_RETURNS_INFINITE_LIFETIME) &&
              rhsLt.returnScope) {
            if (rhsSym && !rhsSym->hasFlag(FLAG_TEMP))
              USR_FATAL_CONT(call, "Return scope variable %s cannot be returned in function returning infinite lifetime",
                  rhsSym->name);
            else
              USR_FATAL_CONT(call, "Return scope variable cannot be returned in function returning infinite lifetime");

            handleDebugOutputOnError(call, lifetimes);
          }
        }
      } else {
        // For the purposes of this check, lhsLt should be no more
        // than the reachability lifetime for that symbol.
        Lifetime lhsReachability = reachabilityLifetimeForSymbol(lhs);
        if (isLifetimeShorter(lhsReachability, lhsLt))
          lhsLt = lhsReachability;

        // Raise errors for init/assigning from a value with shorter lifetime
        // I.e. insist RHS lifetime is longer than LHS lifetime.
        // I.e. error if RHS lifetime is shorter than LHS lifetime.
        if (isLifetimeShorter(rhsLt, lhsLt)) {
          if (lhs && !lhs->hasFlag(FLAG_TEMP))
            USR_FATAL_CONT(call, "Scoped variable %s would outlive the value it is set to", lhs->name);
          else
            USR_FATAL_CONT(call, "Scoped variable would outlive the value it is set to");
          Symbol* from = rhsLt.fromSymbolReachability;
          USR_PRINT(from, "consider scope of %s", from->name);
          handleDebugOutputOnError(call, lifetimes);
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
    Lifetime value = it->second;

    Lifetime reachability = reachabilityLifetimeForSymbol(key);
    if (isLifetimeShorter(value, reachability)) {
      Expr* at = key->defPoint;
      if (value.relevantExpr)
        at = value.relevantExpr;
      if (!key->hasFlag(FLAG_TEMP))
        USR_FATAL_CONT(at, "Scoped variable %s reachable after lifetime ends", key->name);
      else
        USR_FATAL_CONT(at, "Scoped variable reachable after lifetime ends");

      Symbol* from = value.fromSymbolReachability;
      USR_PRINT(from, "consider scope of %s", from->name);
      handleDebugOutputOnError(key->defPoint, lifetimes);
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

/* Is the variable subject to lifetime analysis?
     - "borrowed" class instance
     - ref
 */
static bool isSubjectToLifetimeAnalysis(Symbol* sym) {
  bool argOrVar = isArgSymbol(sym) || isVarSymbol(sym);

  if (!argOrVar)
    return false;

  bool isRecordContainingFieldsSubjectToAnalysis = false;
  if (isRecord(sym->type)) {
    AggregateType* at = toAggregateType(sym->type);
    for_fields(field, at) {
      if (isSubjectToLifetimeAnalysis(field)) {
	isRecordContainingFieldsSubjectToAnalysis = true;
	break;
      }
    }
  }

  // this is a workaround for non-optimal AST for iteration
  if (sym->type->symbol->hasFlag(FLAG_ITERATOR_RECORD))
    isRecordContainingFieldsSubjectToAnalysis = true;

  // It needs to be:
  //  - a pointer type
  //  - a ref
  //  - a record containing refs/class pointers
  //    (or an iterator record)
  if (!(isClass(sym->type) ||
        sym->isRef() ||
        isRecordContainingFieldsSubjectToAnalysis))
    return false;

  if (sym->type->symbol->hasFlag(FLAG_C_PTR_CLASS))
    return false;

  if (sym->hasFlag(FLAG_UNSAFE))
    return false;

  if (sym->hasFlag(FLAG_INDEX_OF_INTEREST))
    return false;

  return true;
}

static bool shouldPropagateLifetimeTo(FnSymbol* inFn, Symbol* sym) {

  if (!isSubjectToLifetimeAnalysis(sym))
    return false;

  if (!isLocalVariable(inFn, sym))
    return false;

  // Lifetime for index variables is set by infer's for loop visitor
  if (sym->hasFlag(FLAG_INDEX_VAR))
    return false;

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

  // Only consider function calls to "=" for lifetime propagation
  //
  // (we assume that a user supplied = function handles lifetimes in a
  //  reasonable manner)
  if (calledFn && 0 == strcmp("=", calledFn->name)) {
    if (isClass(call->get(1)->getValType()) &&
        isClass(call->get(2)->getValType()))
      return true;
    if (calledFn->hasFlag(FLAG_COMPILER_GENERATED))
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

  if (a.infinite) // a inifinite, b infinite or not
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

static Lifetime reachabilityLifetimeForSymbol(Symbol* sym) {
  Lifetime lt;
  lt.fromSymbolReachability = sym;
  lt.relevantExpr = NULL;
  lt.infinite = symbolHasInfiniteLifetime(sym);
  //lt.returnScope = sym->hasFlag(FLAG_RETURN_SCOPE);
  // FLAG_SCOPE should probably inhibit the following behavior
  // (i.e. if the user put 'scope' we shouldn't infer 'return scope').
  lt.returnScope = isArgSymbol(sym) && (sym->isRef() || isClass(sym->type));
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

 */
