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
 */

//#define DEBUG

namespace {

  struct Lifetime {
    // The lifetime is defined by:
    //   - the type of the symbol (is it a ref/ptr at all?)
    //   - where it is defined
    Symbol* fromSymbolReachability;
    bool infinite;
    bool returnScope;
  };

  typedef std::map<Symbol*,Lifetime> SymbolToLifetimeMap;
  typedef std::map<CallExpr*,Lifetime> ReturnToLifetimeMap;

  struct LifetimeState {
    SymbolToLifetimeMap lifetimes;
    Lifetime lifetimeForSymbol(Symbol* sym);
    Lifetime lifetimeForCallReturn(CallExpr* call);
  };

  class InferLifetimesVisitor : public AstVisitorTraverse {

    public:
      LifetimeState* lifetimes;
      virtual bool enterCallExpr (CallExpr* call);
  };
  class EmitLifetimeErrorsVisitor : public AstVisitorTraverse {
   
    public:
      LifetimeState* lifetimes;
      virtual bool enterCallExpr (CallExpr* call);
      void emitErrors();
  };

} /* end anon namespace */

static bool shouldPropagateLifetimeTo(Symbol* sym);
static bool symbolHasInfiniteLifetime(Symbol* sym);
static BlockStmt* getDefBlock(Symbol* sym);
static bool isBlockWithinBlock(BlockStmt* a, BlockStmt* b);
static bool isLifetimeShorter(Lifetime a, Lifetime b);
static Lifetime reachabilityLifetimeForSymbol(Symbol* sym);
static Lifetime infiniteLifetime();
#ifdef DEBUG
static void printLifetimeState(LifetimeState* state);
#endif

void checkLifetimes(void) {

  forv_Vec(FnSymbol, fn, gFnSymbols) {

    if (fn->hasFlag(FLAG_SAFE)) {
#ifdef DEBUG
      printf("Visiting function %s id %i\n", fn->name, fn->id);
      nprint_view(fn);
#endif

      LifetimeState state;

      // Infer lifetimes
      InferLifetimesVisitor infer;
      infer.lifetimes = &state;
      fn->accept(&infer);
 
#ifdef DEBUG
      printLifetimeState(&state);
#endif

      // Emit errors
      EmitLifetimeErrorsVisitor emit;
      emit.lifetimes = &state;
      fn->accept(&emit);
      emit.emitErrors();
    }
  }
}

#ifdef DEBUG
void printLifetimeState(LifetimeState* state)
{
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
#endif

Lifetime LifetimeState::lifetimeForSymbol(Symbol* sym) {
  if (lifetimes.count(sym) > 0)
    return lifetimes[sym];
  
  // Otherwise, create a basic lifetime for this variable.
  return reachabilityLifetimeForSymbol(sym);
}

Lifetime LifetimeState::lifetimeForCallReturn(CallExpr* call) {

  Lifetime minLifetime = infiniteLifetime();

  for_formals_actuals(formal, actual, call) {
    SymExpr* actualSe = toSymExpr(actual);
    INT_ASSERT(actualSe);
    Symbol* actualSym = actualSe->symbol();

    Lifetime actualLifetime = lifetimeForSymbol(actualSym);

    if (formal->hasFlag(FLAG_RETURN_SCOPE))
      return actualLifetime;

    if (isLifetimeShorter(actualLifetime, minLifetime)) {
      minLifetime = actualLifetime;
    }
  }

  // If no argument was marked with FLAG_RETURN_SCOPE,
  // just assume it's the minimum of the passed lifetimes.
  return minLifetime;
}


bool InferLifetimesVisitor::enterCallExpr(CallExpr* call) {

  if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
    SymExpr* lhsSe = toSymExpr(call->get(1));
    Symbol* lhs = lhsSe->symbol();

    if (shouldPropagateLifetimeTo(lhs)) {

      // Consider the RHS and handle the cases
      Expr* rhsExpr = call->get(2);
      CallExpr* rhsCallExpr = toCallExpr(rhsExpr);
      Lifetime lt = lifetimes->lifetimeForSymbol(lhs);

      if (rhsCallExpr) {
        if (rhsCallExpr->isPrimitive(PRIM_ADDR_OF) ||
            rhsCallExpr->isPrimitive(PRIM_SET_REFERENCE)) {
          // Propagate lifetime for refs across assignment
          SymExpr* se = toSymExpr(rhsCallExpr->get(1));
          INT_ASSERT(se);
          lt = lifetimes->lifetimeForSymbol(se->symbol());
        } else if (rhsCallExpr->resolvedFunction()) {
          lt = lifetimes->lifetimeForCallReturn(rhsCallExpr);
        } else {
          // TODO: lots of other primitives
          INT_ASSERT(0);
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

      lifetimes->lifetimes[lhs] = lt;
    }
  }

  return false;
}

bool EmitLifetimeErrorsVisitor::enterCallExpr(CallExpr* call) {

  FnSymbol* calledFn = call->resolvedFunction();

  bool isAssign = calledFn && 0 == strcmp("=", calledFn->name);
  if (call->isPrimitive(PRIM_MOVE) ||
      call->isPrimitive(PRIM_ASSIGN) ||
      isAssign) {
    
    SymExpr* lhsSe = toSymExpr(call->get(1));
    Symbol* lhs = lhsSe->symbol();
    Lifetime lhsLt = lifetimes->lifetimeForSymbol(lhs);

    // Raise errors for returning a scoped/lifetime'd variable
    if (lhs->hasFlag(FLAG_RVV)) {
      if (!lhsLt.infinite && !lhsLt.returnScope) {
        USR_FATAL_CONT(call, "Scoped variable cannot be returned");
        Symbol* from = lhsLt.fromSymbolReachability;
        USR_PRINT(from, "consider scope of %s", from->name);
      }
    }
    
    if (SymExpr* rhsSe = toSymExpr(call->get(2))) {
      Symbol* rhs = rhsSe->symbol();
      Lifetime rhsLt = lifetimes->lifetimeForSymbol(rhs);

      // Raise errors for init/assigning from a value with shorter lifetime
      // I.e. insist RHS lifetime is longer than LHS lifetime.
      // I.e. error if RHS lifetime is shorter than LHS lifetime.
      if (isLifetimeShorter(rhsLt, lhsLt)) {
        USR_FATAL_CONT(call,
                       "Scoped variable would outlive the value it is set to");
        Symbol* from = rhsLt.fromSymbolReachability;
        USR_PRINT(from, "consider scope of %s", from->name);
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
      USR_FATAL_CONT(key, "Scoped variable reachable after its lifetime ends"); 
    }
  }
}

/* Is the variable subject to lifetime analysis?
     - "borrowed" class instance
     - ref
 */
static bool shouldPropagateLifetimeTo(Symbol* sym) {
  // It needs to be a ref or a pointer type
  bool refOrPtr = isClass(sym->type) || sym->isRef();
  bool argOrVar = isArgSymbol(sym) || isVarSymbol(sym);

  if (refOrPtr && argOrVar) {
    Symbol* parentSymbol = sym->defPoint->parentSymbol;
    if (sym->hasFlag(FLAG_UNSAFE) || parentSymbol->hasFlag(FLAG_UNSAFE))
      return false;
    else if (sym->hasFlag(FLAG_SAFE) || parentSymbol->hasFlag(FLAG_SAFE))
      return true;
    else if (sym->hasFlag(FLAG_SCOPE))
      return true;
    else
      // Default: leave it off for now.
      return false;
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
