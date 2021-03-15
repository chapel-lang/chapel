/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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
#include "CForLoop.h"
#include "DecoratedClassType.h"
#include "DeferStmt.h"
#include "DoWhileStmt.h"
#include "driver.h"
#include "expr.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "loopDetails.h"
#include "optimizations.h"
#include "resolution.h"
#include "stlUtil.h"
#include "stringutil.h"
#include "symbol.h"
#include "view.h"
#include "wellknown.h"
#include "WhileDoStmt.h"

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
  struct LifetimeState;
}

// Used to communicate lifetime queries between this and
// the unordered forall optimization
class LifetimeInformation {
  public:
    LifetimeState* lifetimes;
};


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
    // if non-nullptr, the scope of this symbol represents the lifetime
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
      : fromSymbolScope(nullptr), relevantExpr(nullptr),
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
    DetempGroup() : favorite(nullptr), elements() { }
  };

  struct DeinitOrderNode {
    int order; // a kind of statement numbering
    Expr* ast;
    DeinitOrderNode* nestedOrder; // nested blocks, if-then blocks
    DeinitOrderNode* elseOrder;
    DeinitOrderNode()
      : order(0), ast(nullptr),
        nestedOrder(nullptr), elseOrder(nullptr)
    { }
    DeinitOrderNode(Expr* e)
      : order(1), ast(e),
        nestedOrder(nullptr), elseOrder(nullptr)
    { }
  };

  typedef std::map<Symbol*,DetempGroup*> SymbolToDetempGroupMap;
  typedef std::set<DetempGroup*> DetempGroupSet;
  typedef std::set<CallExpr*> CallExprSet;
  typedef std::set<FnSymbol*> LocalFunctionsSet;
  typedef std::map<Symbol*, DeinitOrderNode> DeinitOrderMap;
  typedef std::map<Stmt*, DeinitOrderNode> DeinitOrderBlockMap;

  struct LifetimeState {
    // this mapping allows the pass to ignore certain compiler temps
    SymbolToDetempGroupMap detemp;
    CallExprSet callsToIgnore;

    // this mapping stores the order of deinitialization for each variable.
    // if a and b are variables, and deinitPoint(a) < deinitPoint(b),
    // then a has shorter lifetime than b.
    // the mapping intentionally assigns several deinits at the end of a block
    // the same value.
    DeinitOrderMap order;
    DeinitOrderBlockMap blockOrder;

    // We try to run the analysis as though task functions didn't
    // exist. This set is the set of functions we should consider
    // to be the currently analyzed function and can include task functions.
    LocalFunctionsSet inFns;

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
                                   bool usedAsBorrow);

    LifetimePair inferredLifetimeForCall(CallExpr* call);
    LifetimePair inferredLifetimeForPrimitive(CallExpr* call);
    LifetimePair inferredLifetimeForExpr(Expr* call, bool usedAsRef, bool usedAsBorrow);

    bool isLocalVariable(Symbol* sym);
    bool shouldPropagateLifetimeTo(CallExpr* call, Symbol* sym);

    DeinitOrderNode* deinitOrderNodeFor(Symbol* sym);
    bool helpIsLifetimeShorter(DeinitOrderNode* aNode, DeinitOrderNode* bNode);
    bool isLifetimeShorter(Lifetime a, Lifetime b);
    Lifetime minimumLifetime(Lifetime a, Lifetime b);
    LifetimePair minimumLifetimePair(LifetimePair a, LifetimePair b);
    bool isLifetimeUnspecifiedFormalOrdering(Lifetime a, Lifetime b);
  };

  class DeinitOrderVisitor final : public AstVisitorTraverse {

    public:
      LifetimeState* lifetimes;
      std::vector<DeinitOrderNode*> orderStack;
      bool debugging;
      int skipEarlyDeinitsForUnconditionalReturn;

      DeinitOrderVisitor(LifetimeState* lifetimes, bool debugging)
        : lifetimes(lifetimes), debugging(debugging),
          skipEarlyDeinitsForUnconditionalReturn(0)
      { }
      bool enterScope(Expr* block);
      void exitScope(Expr* block);

      void noteBlockOrder(BlockStmt* b);
      void noteUnconditionalReturnInParentBlocks(Expr* ret);

      // these are all just blocks for the analysis
      bool enterBlockStmt(BlockStmt* node) override;
      void exitBlockStmt(BlockStmt* node) override;
      bool enterForallStmt(ForallStmt* node) override;
      void exitForallStmt(ForallStmt* node) override;
      bool enterWhileDoStmt(WhileDoStmt* node) override;
      void exitWhileDoStmt(WhileDoStmt* node) override;
      bool enterDoWhileStmt(DoWhileStmt* node) override;
      void exitDoWhileStmt(DoWhileStmt* node) override;
      bool enterCForLoop(CForLoop* node) override;
      void exitCForLoop(CForLoop* node) override;
      bool enterForLoop(ForLoop* node) override;
      void exitForLoop(ForLoop* node) override;

      bool enterCondStmt(CondStmt* cond) override;
      bool enterCallExpr(CallExpr* call) override;
      bool enterGotoStmt(GotoStmt* g) override;
  };

  class GatherTempsVisitor final : public AstVisitorTraverse {

    public:
      LifetimeState* lifetimes;
      bool enterCallExpr(CallExpr* call) override;
  };
  class IntrinsicLifetimesVisitor final : public AstVisitorTraverse {

    public:
      LifetimeState* lifetimes;
      bool enterDefExpr(DefExpr* def) override;
      bool enterCallExpr(CallExpr* call) override;
  };
  class InferLifetimesVisitor final : public AstVisitorTraverse {

    public:
      LifetimeState* lifetimes;
      bool changed;

      bool enterCallExpr(CallExpr* call) override;
      bool enterForLoop(ForLoop* forLoop) override;

      void inferLifetimesForConstraint(CallExpr* forCall);
      void inferLifetimesForConstraint(CallExpr* forCall, Expr* constraintExpr);
  };
  class EmitLifetimeErrorsVisitor final : public AstVisitorTraverse {

    public:
      LifetimeState* lifetimes;
      std::set<Symbol*> erroredSymbols; // to avoid duplicate errors

      bool enterCallExpr(CallExpr* call) override;

      void emitBadReturnErrors(CallExpr* call);
      void emitBadAssignErrors(CallExpr* call);
      void emitBadSetFieldErrors(CallExpr* call);
      void emitErrors();
  };

  typedef enum {
    CONSTRAINT_LESS = -11,
    CONSTRAINT_LESS_EQ = -10,
    CONSTRAINT_UNKNOWN = 0,
    CONSTRAINT_EQUAL = 1,
    CONSTRAINT_GREATER_EQ = 10,
    CONSTRAINT_GREATER = 11,
  } constraint_t;

} /* end anon namespace */

static bool typeHasInfiniteBorrowLifetime(Type* type);
static bool isSubjectToRefLifetimeAnalysis(Symbol* sym);
static bool isSubjectToBorrowLifetimeAnalysis(Type* type);
static bool isSubjectToBorrowLifetimeAnalysis(Symbol* sym);
static bool recordContainsClassFields(AggregateType* at);
static bool recordContainsOwnedClassFields(AggregateType* at);
static bool aggregateContainsBorrowedClassFields(AggregateType* at);
static bool containsOwnedClass(Type* type);
//static bool isOrContainsBorrowedClass(Type* type);
static bool isOrRefersBorrowedClass(Type* type);
static bool isAnalyzedMoveOrAssignment(CallExpr* call);
static bool symbolHasInfiniteLifetime(Symbol* sym);
static Stmt* getDefBlock(Symbol* sym);
static Expr* getParentExprIncludingTaskFnCalls(Expr* cur);
static bool isBlockWithinBlock(Stmt* a, Stmt* b);
static constraint_t orderConstraintFromClause(Expr* expr, Symbol* a, Symbol* b);
static constraint_t orderConstraintFromClause(FnSymbol* fn, Symbol* a, Symbol* b);
static void printOrderConstraintFromClause(Expr* expr, Symbol* a, Symbol* b);
static void printOrderConstraintFromClause(FnSymbol* fn, Symbol* a, Symbol* b);
static Lifetime scopeLifetimeForSymbol(Symbol* sym);
static Lifetime infiniteLifetime();
static Lifetime unknownLifetime();
static LifetimePair infiniteLifetimePair();
static LifetimePair unknownLifetimePair();
static bool debuggingLifetimesForFn(FnSymbol* fn);
static void printLifetime(Lifetime lt);
static void printLifetimePair(LifetimePair lt);
static void printLifetimeState(LifetimeState* state);
static void printDeinitOrderTree(const char* prefix,  DeinitOrderNode* node);
static void handleDebugOutputOnError(Expr* e, LifetimeState* state);
static bool shouldCheckLifetimesInFn(FnSymbol* fn);
static void markArgumentsReturnScope(FnSymbol* fn);
static void checkFunction(FnSymbol* fn);

static bool isCallToFunctionReturningNotOwned(CallExpr* call);
static bool isUser(BaseAST* ast);

void checkLifetimesAndNilDereferences() {
  // Clear last error location so we get errors from nil checking
  // even if previous compiler code raised error on the same line.
  // This is necessary due to the use of printsSameLocationAsLastError
  // to hide multiple nil-checking errors from the same line.
  clearLastErrorLocation();

  // Mark all arguments with FLAG_SCOPE or FLAG_RETURN_SCOPE.
  // This needs to be done for all functions before the next
  // loop since it affects how calls are handled.
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    markArgumentsReturnScope(fn);
    adjustSignatureForNilChecking(fn);
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    // Perform nil checking on each function
    checkNilDereferencesInFn(fn);
    // Perform lifetime checking on each function
    checkFunction(fn);
  }

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    // Now that lifetime checking is complete, remove the lifetimeConstraints
    if (fn->lifetimeConstraints) {
      fn->lifetimeConstraints->remove();
      fn->lifetimeConstraints = nullptr;
    }
  }
}

static void checkFunction(FnSymbol* fn) {
  if (fNoLifetimeChecking == false || fNoEarlyDeinit == false) {
    // check lifetimes
    // e.g. borrow can't outlive borrowed-from
    checkLifetimesInFunction(fn);
  }
}

void checkLifetimesInFunction(FnSymbol* fn) {

  // No need to check lifetimes in string literal module.
  if (fn->getModule() == stringLiteralModule)
    return;
  // No need to lifetime check extern functions
  if (fn->hasFlag(FLAG_EXTERN))
    return;

  bool debugging = debuggingLifetimesForFn(fn);

  LifetimeState state;

  // We'll be analyzing this function and its task functions
  state.inFns.insert(fn);

  // Gather temps that are just aliases for something else
  GatherTempsVisitor gather;
  gather.lifetimes = &state;
  fn->accept(&gather);

  // Compute the deinitialization order for local variables
  DeinitOrderVisitor order(&state, debugging);
  fn->accept(&order);

  if (debugging) {
    for_set(FnSymbol, inFn, state.inFns) {
      printf("Visiting function %s id %i\n", inFn->name, inFn->id);
      nprint_view(inFn);
    }
    gdbShouldBreakHere();
  }

  // Figure out the scope for local variables / arguments
  IntrinsicLifetimesVisitor intrinsics;
  intrinsics.lifetimes = &state;
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

  if (shouldCheckLifetimesInFn(fn)) {
    // Emit errors
    EmitLifetimeErrorsVisitor emit;
    emit.lifetimes = &state;
    fn->accept(&emit);
    emit.emitErrors();
  }

  // Give forall unordered ops optimization a chance to check
  // lifetimes of certain variables.
  // Runs no matter flag setting to mark ends of foralls.
  LifetimeInformation info;
  info.lifetimes = &state;
  for_set(FnSymbol, inFn, state.inFns) {
    checkLifetimesForForallUnorderedOps(inFn, &info);
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

  if (fn->isMethod() && fn->_this != nullptr && !anyReturnScope &&
      fn->name != astrInit && fn->name != astrInitEquals &&
      !fn->lifetimeConstraints) {
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

static Symbol* getSymbolFromLifetimeClause(Expr* expr, bool &isRet) {
  if (CallExpr* call = toCallExpr(expr)) {
    if (call->isPrimitive(PRIM_LIFETIME_OF)) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);
      return se->symbol();
    } else if (call->isPrimitive(PRIM_RETURN)) {
      isRet = true;
      return nullptr;
    }
  }

  INT_FATAL("Unhandled");
  return nullptr;
}

static constraint_t mergeConstraints(constraint_t a, constraint_t b, bool& invalid) {
  invalid = false;
  switch (a) {
    case CONSTRAINT_LESS:
      switch (b) {
        case CONSTRAINT_LESS:
        case CONSTRAINT_LESS_EQ:
        case CONSTRAINT_UNKNOWN:
          return CONSTRAINT_LESS;
        case CONSTRAINT_EQUAL:
        case CONSTRAINT_GREATER_EQ:
        case CONSTRAINT_GREATER:
          invalid = true;
          break;
      }
      break;
    case CONSTRAINT_LESS_EQ:
      switch (b) {
        case CONSTRAINT_LESS:
          return CONSTRAINT_LESS;
        case CONSTRAINT_LESS_EQ:
        case CONSTRAINT_UNKNOWN:
          return CONSTRAINT_LESS_EQ;
        case CONSTRAINT_GREATER_EQ:
        case CONSTRAINT_EQUAL:
          return CONSTRAINT_EQUAL;
        case CONSTRAINT_GREATER:
          invalid = true;
          break;
      }
      break;
    case CONSTRAINT_UNKNOWN:
      return b;
    case CONSTRAINT_EQUAL:
      switch (b) {
        case CONSTRAINT_LESS:
          invalid = true;
          break;
        case CONSTRAINT_LESS_EQ:
        case CONSTRAINT_UNKNOWN:
        case CONSTRAINT_EQUAL:
        case CONSTRAINT_GREATER_EQ:
          return CONSTRAINT_EQUAL;
        case CONSTRAINT_GREATER:
          invalid = true;
          break;
      }
      break;
    case CONSTRAINT_GREATER_EQ:
      switch (b) {
        case CONSTRAINT_LESS:
          invalid = true;
          break;
        case CONSTRAINT_LESS_EQ:
        case CONSTRAINT_EQUAL:
          return CONSTRAINT_EQUAL;
        case CONSTRAINT_GREATER_EQ:
        case CONSTRAINT_UNKNOWN:
          return CONSTRAINT_GREATER_EQ;
        case CONSTRAINT_GREATER:
          return CONSTRAINT_GREATER;
      }
      break;
    case CONSTRAINT_GREATER:
      switch (b) {
        case CONSTRAINT_LESS:
        case CONSTRAINT_LESS_EQ:
        case CONSTRAINT_EQUAL:
          invalid = true;
          break;
        case CONSTRAINT_GREATER_EQ:
        case CONSTRAINT_UNKNOWN:
        case CONSTRAINT_GREATER:
          return CONSTRAINT_GREATER;
      }
      break;
      // no default -> error if one is added
  }

  return CONSTRAINT_UNKNOWN;
}


// -1 => isLifetimeShorter(a,b) == true
//                    lifetime a < lifetime b
// 0  => isLifetimeShorter(a,b) == false
//       order unknown or equal
// 1  => isLifetimeShorter(a,b) == false
//       lifetime a > lifetime b
static constraint_t orderConstraintFromClause(Expr* expr, Symbol* a, Symbol* b)
{
  if (CallExpr* call = toCallExpr(expr)) {
    if (call->isNamed(",")) {
      constraint_t v1, v2, res;
      bool invalid = false;
      res = CONSTRAINT_UNKNOWN;
      v1 = orderConstraintFromClause(call->get(1), a, b);
      v2 = orderConstraintFromClause(call->get(2), a, b);
      res = mergeConstraints(v1, v2, invalid);
      if (invalid)
        USR_FATAL(expr, "Conflicting inequality in lifetime clause");
      return res;
    } else if (call->isPrimitive(PRIM_RETURN)) {
      // No impact on isLifetimeShorter but could impact inference
      return CONSTRAINT_UNKNOWN;
    } else {
      Symbol* lhs = nullptr;
      Symbol* rhs = nullptr;
      bool lhsRet = false;
      bool rhsRet = false;

      lhs = getSymbolFromLifetimeClause(call->get(1), lhsRet);
      rhs = getSymbolFromLifetimeClause(call->get(2), lhsRet);
      if (rhsRet)
        USR_FATAL(call, "Cannot read lifetime of return in clause");
      if (lhsRet) {
        // return lifetime = rhs
        // No impact on isLifetimeShorter but could impact inference
        return CONSTRAINT_UNKNOWN;
      }

      INT_ASSERT(lhs && rhs);

      if ((a == lhs && b == rhs) ||
          (b == lhs && a == rhs)) {

        bool invert = false;
        if (a == rhs && b == lhs)
          invert = true;

        if (call->isNamed("=="))
          return CONSTRAINT_EQUAL;
        else if (call->isNamed("<"))
          return invert?CONSTRAINT_GREATER:CONSTRAINT_LESS;
        else if (call->isNamed("<=") || call->isNamed("="))
          return invert?CONSTRAINT_GREATER_EQ:CONSTRAINT_LESS_EQ;
        else if (call->isNamed(">"))
          return invert?CONSTRAINT_LESS:CONSTRAINT_GREATER;
        else if (call->isNamed(">="))
          return invert?CONSTRAINT_LESS_EQ:CONSTRAINT_GREATER_EQ;
        else
          INT_FATAL("Unhandled case");
      }
    }
  }

  return CONSTRAINT_UNKNOWN;
}


static constraint_t orderConstraintFromClause(FnSymbol* fn, Symbol* a, Symbol* b) {
  if (fn->lifetimeConstraints) {
    Expr* last = fn->lifetimeConstraints->body.last();
    return orderConstraintFromClause(last, a, b);
  } else if (isTaskFun(fn)) {
    // Compute constraints on the actual arguments, if any

    INT_FATAL("Task function formals should be handled by detemp map");
  }

  return CONSTRAINT_UNKNOWN;
}

static Symbol* returnLifetimeFromClause(Expr* expr) {

  if (CallExpr* call = toCallExpr(expr)) {
    if (call->isNamed(",")) {
      Symbol* v1 = nullptr;
      Symbol* v2 = nullptr;
      v1 = returnLifetimeFromClause(call->get(1));
      v2 = returnLifetimeFromClause(call->get(2));
      if (v1 && v2)
        USR_FATAL(expr,
                  "Lifetime clause includes multiple return specifications");
      return v1?v1:v2;
    } else {
      if (call->isPrimitive(PRIM_RETURN)) {
        CallExpr* sub = toCallExpr(call->get(1));
        INT_ASSERT(sub && sub->isPrimitive(PRIM_LIFETIME_OF));
        SymExpr* se = toSymExpr(sub->get(1));
        INT_ASSERT(se);
        return se->symbol();
      }
    }
  }

  return nullptr;
}


static Symbol* returnLifetimeFromFnClause(FnSymbol* fn) {
  if (fn->lifetimeConstraints == nullptr)
    return nullptr;

  Expr* last = fn->lifetimeConstraints->body.last();
  return returnLifetimeFromClause(last);
}

static bool isOuterVariable(FnSymbol* fn, Symbol* var) {
  for (Expr* cur = var->defPoint;
      cur != nullptr;
      cur = getParentExprIncludingTaskFnCalls(cur)) {
    if (cur->parentSymbol == fn)
      return false;
  }

  return true;
}


static bool isOuterLifetimeFromClause(LifetimeState& lifetimes, ArgSymbol* arg, Expr* clausePart) {
  if (CallExpr* call = toCallExpr(clausePart)) {
    if (call->isNamed(",")) {
      bool partOne = isOuterLifetimeFromClause(lifetimes, arg, call->get(1));
      bool partTwo = isOuterLifetimeFromClause(lifetimes, arg, call->get(2));
      if (partOne || partTwo)
        return true;
    } else if (call->isPrimitive(PRIM_RETURN)) {
      return false;
    } else {
      Symbol* lhs = nullptr;
      Symbol* rhs = nullptr;
      bool lhsRet = false;
      bool rhsRet = false;

      lhs = getSymbolFromLifetimeClause(call->get(1), lhsRet);
      rhs = getSymbolFromLifetimeClause(call->get(2), rhsRet);

      // X checking lifetimes.inFns.count is wrong because
      // inFns is e.g. borrowExample5EOB but lhs is in e.g. setit

      bool lhsOuter = lhs->hasFlag(FLAG_OUTER_VARIABLE) ||
                      isOuterVariable(arg->defPoint->getFunction(), lhs);
      bool rhsOuter = rhs->hasFlag(FLAG_OUTER_VARIABLE) ||
                      isOuterVariable(arg->defPoint->getFunction(), rhs);

      // arg > outerVariable
      if (lhs == arg && rhsOuter &&
          (call->isNamed("==") ||
           call->isNamed(">") ||
           call->isNamed(">=")))
        return true;

      // outerVariable > arg
      if (rhs == arg && lhsOuter &&
          (call->isNamed("==") ||
           call->isNamed("<") ||
           call->isNamed("<=") || call->isNamed("=")))
        return true;
    }
  }
  return false;
}

static bool isOuterLifetimeFromClause(LifetimeState& lifetimes, ArgSymbol* arg)
{
  if (FnSymbol* fn = arg->defPoint->getFunction())
    if (fn->lifetimeConstraints)
      if (Expr* last = fn->lifetimeConstraints->body.last())
        return isOuterLifetimeFromClause(lifetimes, arg, last);

  return false;
}

static void printOrderConstraintFromClause(Expr* expr, Symbol* a, Symbol* b)
{
  if (CallExpr* call = toCallExpr(expr)) {
    if (call->isNamed(",")) {
      printOrderConstraintFromClause(call->get(1), a, b);
      printOrderConstraintFromClause(call->get(2), a, b);
    } else {
      Symbol* lhs = nullptr;
      Symbol* rhs = nullptr;
      bool lhsRet = false;
      bool rhsRet = false;

      lhs = getSymbolFromLifetimeClause(call->get(1), lhsRet);
      rhs = getSymbolFromLifetimeClause(call->get(2), lhsRet);

      if ((a == lhs && b == rhs) ||
          (b == lhs && a == rhs)) {
        const char* lhsName = lhsRet?"return":lhs->name;
        const char* rhsName = rhsRet?"return":rhs->name;
        const char* calledName = nullptr;

        if (SymExpr* base = toSymExpr(call->baseExpr))
          calledName = base->symbol()->name;
        else if (UnresolvedSymExpr* base = toUnresolvedSymExpr(call->baseExpr))
          calledName = base->unresolved;

        FnSymbol* fn = call->getFunction();

        if (developer || isUser(fn)) {
          USR_PRINT(call, "called function %s", toString(fn));
          USR_PRINT(call, "includes lifetime constraint %s %s %s",
                    lhsName, calledName, rhsName);
        } else {
          USR_PRINT("called function %s", toString(fn));
          USR_PRINT("includes lifetime constraint %s %s %s",
                    lhsName, calledName, rhsName);
        }
      }
    }
  }

}

static void printOrderConstraintFromClause(FnSymbol* fn, Symbol* a, Symbol* b) {
  INT_ASSERT(fn->lifetimeConstraints);
  Expr* last = fn->lifetimeConstraints->body.last();
  printOrderConstraintFromClause(last, a, b);
}

static bool definedInTaskFunction(Symbol* sym) {
  if (DefExpr* def = sym->defPoint)
    if (FnSymbol* fn = def->getFunction())
      return isTaskFun(fn);

  return false;
}

static bool isTaskFunctionFormal(Symbol* sym) {
  return isArgSymbol(sym) && definedInTaskFunction(sym);
}


static bool shouldCheckLifetimesInFn(FnSymbol* fn) {
  if (fn->hasFlag(FLAG_UNSAFE))
    return false;
  if (fn->hasFlag(FLAG_SAFE))
    return true;

  // TODO
  if (fn->hasFlag(FLAG_COMPILER_GENERATED))
    return false;

  // We check task functions when visiting the code
  // calling them, so don't visit them when considering
  // the task function on its own.
  if (isTaskFun(fn))
    return false;

  ModuleSymbol* inMod = fn->getModule();
  if (inMod->hasFlag(FLAG_UNSAFE))
    return false;
  if (inMod->hasFlag(FLAG_SAFE))
    return true;

  return !fNoLifetimeChecking;
}

static bool debuggingLifetimesForFn(FnSymbol* fn)
{
  if (!fn) return false;
  if (fn->id == debugLifetimesForId) return true;
  if (0 == strcmp(debugLifetimesForFn, fn->name)) return true;
  return false;
}

static void printLifetime(Lifetime lt) {
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

static void printLifetimePair(LifetimePair lt) {
  printf("ref lifetime ");
  printLifetime(lt.referent);
  printf("borrow lifetime ");
  printLifetime(lt.borrowed);
  printf("\n");
}

static void printLifetimeState(LifetimeState* state)
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

  printf("deinit orders:\n");
  for (DeinitOrderMap::iterator it = state->order.begin();
       it != state->order.end();
       ++it) {
    Symbol* key = it->first;
    DeinitOrderNode& value = it->second;

    printf("deinit order for %s[%i]:\n", key->name, key->id);
    printDeinitOrderTree("", &value);
  }

  printf("deinit blockOrders:\n");
  for (DeinitOrderBlockMap::iterator it = state->blockOrder.begin();
       it != state->blockOrder.end();
       ++it) {
    Stmt* key = it->first;
    DeinitOrderNode& value = it->second;

    printf("deinit order for block %i:\n", key->id);
    printDeinitOrderTree("", &value);
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
    printf("Analyzed functions:\n");
    for (LocalFunctionsSet::iterator it = state->inFns.begin();
       it != state->inFns.end();
       ++it) {
      FnSymbol* inFn = *it;
      nprint_view(inFn);
    }
    printf("Expr is:\n");
    nprint_view(e);
    printf("Lifetime state is:\n");
    printLifetimeState(state);
    USR_STOP();
  }
}


/************************************* | **************************************
*                                                                             *
*  LifetimeState                                                              *
*                                                                             *
************************************** | *************************************/

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

LifetimePair LifetimeState::lifetimeForActual(Symbol* sym, bool usedAsRef, bool usedAsBorrow) {

  // TODO: Should this use the canonical symbol?
  // What if sym is an argument to a task function?

  // Careful here, as sym might not be a canonical symbol,
  // or it might be, so it being a ref or not is an uncertain matter.
  bool combineForRef = usedAsRef;
  bool combineForBorrow = usedAsBorrow && containsOwnedClass(sym->type);
  bool combineForOuter = !isLocalVariable(sym);
  bool combineForArg = isArgSymbol(sym) && !definedInTaskFunction(sym);
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
      (formal->getFunction()->name == astrInit ||
      formal->getFunction()->name == astrInitEquals))
    return true;

  // arguments marked with FLAG_SCOPE don't determine the lifetime
  // of a returned variable, since only return scope returns are allowed.
  if (formal->hasFlag(FLAG_SCOPE))
    return true;

  return false;
}

LifetimePair LifetimeState::inferredLifetimeForCall(CallExpr* call) {

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
  if (calledFn->isMethod() &&
      (calledFn->name == astrInit || calledFn->name == astrInitEquals)) {
    returnType = calledFn->getFormal(1)->getValType();
  } else if (calledFn->hasFlag(FLAG_FN_RETARG)) {
    ArgSymbol* retArg = toArgSymbol(toDefExpr(calledFn->formals.tail)->sym);
    INT_ASSERT(retArg && retArg->hasFlag(FLAG_RETARG));
    returnType = retArg->getValType();
  }

  returnsBorrow = isSubjectToBorrowLifetimeAnalysis(returnType);

  ArgSymbol* theOnlyOneThatMatters = nullptr;
  if (Symbol* sym = returnLifetimeFromFnClause(calledFn)) {
    // lifetime = outer/global variable -> infinite lifetime
    if (sym->defPoint->getFunction() != calledFn)
      return infiniteLifetimePair();

    INT_ASSERT(isArgSymbol(sym));
    theOnlyOneThatMatters = toArgSymbol(sym);
  }

  for_formals_actuals(formal, actual, call) {
    SymExpr* actualSe = toSymExpr(actual);
    INT_ASSERT(actualSe);
    Symbol* actualSym = actualSe->symbol();

    LifetimePair argLifetime = unknownLifetimePair();

    if (formalArgumentDoesNotImpactReturnLifetime(formal) ||
        (theOnlyOneThatMatters != nullptr && theOnlyOneThatMatters != formal))
      continue;

    if (returnsRef && formal->isRef()) {
      if (symbolHasInfiniteLifetime(actualSym)) {
        argLifetime.referent = infiniteLifetime();
      } else if (isSubjectToRefLifetimeAnalysis(actualSym) ||
                 isLocalVariable(actualSym)) {

        // Use the referent part of the actual's lifetime
        LifetimePair temp = lifetimeForActual(actualSym, returnsRef, returnsBorrow);
        argLifetime.referent = temp.referent;
      }
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
                                              returnsBorrow);
        argLifetime.borrowed = temp.borrowed;
      }
    }

    minLifetime = minimumLifetimePair(minLifetime, argLifetime);
  }

  return minLifetime;
}

LifetimePair LifetimeState::inferredLifetimeForPrimitive(CallExpr* call) {

  bool returnsRef = call->isRef();
  bool returnsBorrow = isSubjectToBorrowLifetimeAnalysis(call->typeInfo());

  if (call->isPrimitive(PRIM_GET_MEMBER) ||
      call->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
      call->isPrimitive(PRIM_GET_SVEC_MEMBER) ||
      call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE) ||
      call->isPrimitive(PRIM_ARRAY_GET)) {
    // Lifetime of a field is the lifetime of the aggregate.
    // Don't get confused by the VarSymbol representing the field
    SymExpr* actualSe = toSymExpr(call->get(1));
    INT_ASSERT(actualSe);
    Symbol* actualSym = actualSe->symbol();
    LifetimePair argLifetime = unknownLifetimePair();

    LifetimePair temp = lifetimeForActual(actualSym, returnsRef, returnsBorrow);

    if (returnsRef &&
        (isSubjectToRefLifetimeAnalysis(actualSym) ||
         isLocalVariable(actualSym))) {
      // Use the referent part of the actual's lifetime
      argLifetime.referent = temp.referent;
    }
    if (returnsRef && isClassLikeOrPtr(actualSym->getValType())) {
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
         isLocalVariable(actualSym))) {
      // Use the referent part of the actual's lifetime
      LifetimePair temp = lifetimeForActual(actualSym, returnsRef, returnsBorrow);
      argLifetime.referent = temp.referent;
    }
    if (returnsBorrow &&
        isSubjectToBorrowLifetimeAnalysis(actualSym)) {
      // Use the borrowed part of the actual's lifetime
      LifetimePair temp = lifetimeForActual(actualSym, returnsRef, returnsBorrow);
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
    SymExpr* rhsSe = toSymExpr(rhsExpr);
    INT_ASSERT(rhsSe);
    Symbol* rhs = getCanonicalSymbol(rhsSe->symbol());

    lp = lifetimeForActual(rhs, usedAsRef, usedAsBorrow);
  }

  return lp;
}



/* Is the sym argument a local variable in the currently analyzed function?
 */
bool LifetimeState::isLocalVariable(Symbol* sym) {
  bool isValueArg = isArgSymbol(sym) &&
                    !definedInTaskFunction(sym) &&
                    !sym->isRef();
  bool isLocal = isVarSymbol(sym) || isValueArg;

  return isLocal && (inFns.count(sym->defPoint->getFunction()) != 0);
}

/************************************* | **************************************
*                                                                             *
*  GatherTemps                                                                *
*                                                                             *
************************************** | *************************************/

static bool isAutoDestroyableVariable(Symbol* sym) {
  TypeSymbol* ts = sym->getValType()->symbol;

  return ts->hasFlag(FLAG_POD) == false &&
         sym->hasFlag(FLAG_NO_AUTO_DESTROY) == false &&
         (sym->hasFlag(FLAG_INSERT_AUTO_DESTROY) ||
          sym->hasFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW));
}

static void addSymbolToDetempGroup(Symbol* sym, DetempGroup* group) {
  group->elements.push_back(sym);

  Symbol* old = group->favorite;

  bool preferSym = false;
  // Should we prefer sym to old?
  // Or leave old there as the preferred symbol?
  if (old == nullptr) {
    preferSym = true;
  } else {
    Stmt* oldBlock = getDefBlock(old);
    Stmt* symBlock = getDefBlock(sym);

    bool oldInSym = isBlockWithinBlock(oldBlock, symBlock);
    bool symInOld = isBlockWithinBlock(symBlock, oldBlock);
    bool oldGlobal = isGlobal(old);
    bool symGlobal = isGlobal(sym);
    bool oldTemp = old->hasFlag(FLAG_TEMP);
    bool symTemp = sym->hasFlag(FLAG_TEMP);
    bool oldRef = old->isRef();
    bool symRef = sym->isRef();
    bool oldDestroyed = isAutoDestroyableVariable(old);
    bool symDestroyed = isAutoDestroyableVariable(sym);

    // Prefer sym if it's in an outer block
    // (e.g. actual variable passed to task function formal,
    //  task function formal vs inner task function formal)
    if ((isTaskFunctionFormal(old) || isTaskFunctionFormal(sym)) &&
        (oldInSym || symInOld)) {
      preferSym = oldInSym;
    } else if (oldGlobal != symGlobal) {
      preferSym = symGlobal;
    } else if (oldTemp != symTemp) {
      preferSym = !symTemp;
    } else if (oldRef != symRef) {
      preferSym = !symRef;
    } else if (oldDestroyed != symDestroyed) {
      preferSym = symDestroyed;
    } else {
      // break ties based on shorter name and then lower id
      int symlen = strlen(sym->name);
      int oldlen = strlen(old->name);
      if (symlen != oldlen)
        preferSym = symlen < oldlen;
      else
        preferSym = sym->id < old->id;
    }
  }

  if (preferSym)
    group->favorite = sym;
}

static void addPairToDetempMap(Symbol* a, Symbol* b,
                               SymbolToDetempGroupMap& map) {

  // Is a in the map? Is b in the map?
  bool aInMap = map.count(a) > 0;
  bool bInMap = map.count(b) > 0;

  DetempGroup* group = nullptr;
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
  // Traverses into task functions below when also
  // handling task function arguments.

  if (call->isPrimitive(PRIM_MOVE)) {
    SymExpr* lhsSe = toSymExpr(call->get(1));
    INT_ASSERT(lhsSe);
    Symbol* lhs = lhsSe->symbol();

    // don't de-temp the return value
    if (lhs->hasFlag(FLAG_RVV))
      return false;

    Symbol* a = nullptr;
    Symbol* b = nullptr;

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
      addPairToDetempMap(b, a, lifetimes->detemp);
      lifetimes->callsToIgnore.insert(call);
    }
  }

  if (FnSymbol* calledFn = call->resolvedFunction()) {
    if (isTaskFun(calledFn)) {
      lifetimes->inFns.insert(calledFn);

      // Add formal arguments -> actual arguments to the detemp map
      for_formals_actuals(formal, actual, call) {
        if (SymExpr* actualSe = toSymExpr(actual)) {
          Symbol* actualSym = actualSe->symbol();
          addPairToDetempMap(actualSym, formal, lifetimes->detemp);
        }
      }
      lifetimes->callsToIgnore.insert(call);

      // Gather temps in the called function
      calledFn->body->accept(this);
    }
  }

  return false;
}

/************************************* | **************************************
*                                                                             *
*  DeinitOrder                                                                *
*                                                                             *
************************************** | *************************************/

static void printDeinitOrderTree(const char* prefix, DeinitOrderNode* node) {

  int astId = 0;
  if (node->ast != nullptr)
    astId = node->ast->id;

  bool anyElse = false;
  for (DeinitOrderNode* cur = node; cur != nullptr; cur = cur->nestedOrder) {
    if (cur->elseOrder)
      anyElse = true;
  }

  if (anyElse == false) {
    printf("%s", prefix);
    for (DeinitOrderNode* cur = node; cur != nullptr; cur = cur->nestedOrder) {
      if (cur->ast)
        astId = cur->ast->id;
      if (cur != node)
        printf(".");
      printf("%i", cur->order);
    }
    printf(" ast=%i\n", astId);
  } else {
    // print something more complex
    printf("%sorder=%i ast=%i\n", prefix, node->order, astId);
    if (node->nestedOrder != nullptr) {
      printf("%snested:\n", prefix);
      printDeinitOrderTree(astr(prefix, "  "), node->nestedOrder);
    }
    if (node->elseOrder != nullptr) {
      printf("%selse:\n", prefix);
      printDeinitOrderTree(astr(prefix, "  "), node->elseOrder);
    }
  }
}

// prints e.g. 1.3.4 with no newline
static void printOrderSummary(std::vector<DeinitOrderNode*>& orderStack) {
  for (size_t i = 1; i < orderStack.size(); i++) {
    printf("  ");
  }

  for (size_t i = 0; i < orderStack.size(); i++) {
    if (i != 0)
      printf(".");
    printf("%i", orderStack[i]->order);
  }
}

static void clearDeinitOrderNodeSubtree(DeinitOrderNode* node) {
  if (node->nestedOrder != nullptr) {
    delete node->nestedOrder;
    node->nestedOrder = nullptr;
  }
  if (node->elseOrder != nullptr) {
    delete node->elseOrder;
    node->elseOrder = nullptr;
  }
}

static void nextDeinitOrderNode(DeinitOrderNode* node, Expr* e) {
  node->order++;
  node->ast = e;
  clearDeinitOrderNodeSubtree(node);
}

static void addToDeinitOrderTree(DeinitOrderNode* dst, DeinitOrderNode* src) {

  // If dst was referring to a different thing from src, we ignore
  // the update from src. This comes up with chpl__autoDestroy calls
  // added just before the function epilogue even though there was an
  // unconditional return before that.
  if (dst->order != 0 && dst->order != src->order)
    return;

  dst->order = src->order;
  dst->ast = src->ast;

  // create new nodes for any sub-nodes if they are not present already.
  if (src->nestedOrder != nullptr && dst->nestedOrder == nullptr) {
    dst->nestedOrder = new DeinitOrderNode();
  }
  if (src->elseOrder != nullptr && dst->elseOrder == nullptr) {
    dst->elseOrder = new DeinitOrderNode();
  }

  // copy the sub-nodes, recursively
  if (src->nestedOrder != nullptr)
    addToDeinitOrderTree(dst->nestedOrder, src->nestedOrder);
  if (src->elseOrder != nullptr)
    addToDeinitOrderTree(dst->elseOrder, src->elseOrder);
}

bool DeinitOrderVisitor::enterScope(Expr* block) {
  if (orderStack.empty()) {
    DeinitOrderNode* next = new DeinitOrderNode(block);
    next->order = 1;
    orderStack.push_back(next);

  } else {
    // count order for block being a new statement
    nextDeinitOrderNode(orderStack.back(), block);

    if (debugging) {
      printOrderSummary(orderStack);
      printf(" block %i\n", block->id);
    }

    DeinitOrderNode* next = new DeinitOrderNode(block);
    orderStack.back()->nestedOrder = next;
    orderStack.push_back(next);
  }

  return true;
}

void DeinitOrderVisitor::exitScope(Expr* block) {

  INT_ASSERT(!orderStack.empty());
  DeinitOrderNode* last = orderStack.back();
  clearDeinitOrderNodeSubtree(last);

  if (BlockStmt* b = toBlockStmt(block)) {
    noteBlockOrder(b);
  }

  // remove the orderStack added in enterBlockStmt
  orderStack.pop_back();

  if (!orderStack.empty()) {
    // count order for whatever calls follow block being in new statements
    nextDeinitOrderNode(orderStack.back(), nullptr);
  } else {
    INT_ASSERT(block->parentExpr == nullptr);
  }
}

void DeinitOrderVisitor::noteBlockOrder(BlockStmt* b) {
  if (!isShadowVarSymbol(b->parentSymbol)) {
    // note shadow var scopes at conclusion of forall loops

    // note the last statement in the block for use in finding the
    // deinit order of symbols defined in this block.
    // The noteUnconditionalReturnInParentBlocks might take precedence
    // over this one though.
    addToDeinitOrderTree(&lifetimes->blockOrder[b], orderStack.front());
  }
}

bool DeinitOrderVisitor::enterBlockStmt(BlockStmt* node) {
  if (!node->isScopeless() && !isCondStmt(node->parentExpr)) {
    enterScope(node);
  }

  return true;
}
void DeinitOrderVisitor::exitBlockStmt(BlockStmt* node) {
  if (!node->isScopeless() && !isCondStmt(node->parentExpr)) {
    exitScope(node);
  } else {
    noteBlockOrder(node);
  }
}
bool DeinitOrderVisitor::enterForallStmt(ForallStmt* node) {
  return enterScope(node);
}
void DeinitOrderVisitor::exitForallStmt(ForallStmt* node) {
  // note the order for shadow-var symbols
  addToDeinitOrderTree(&lifetimes->blockOrder[node], orderStack.front());

  exitScope(node);
}

bool DeinitOrderVisitor::enterWhileDoStmt(WhileDoStmt* node) {
  return enterScope(node);
}
void DeinitOrderVisitor::exitWhileDoStmt(WhileDoStmt* node) {
  exitScope(node);
}
bool DeinitOrderVisitor::enterDoWhileStmt(DoWhileStmt* node) {
  return enterScope(node);
}
void DeinitOrderVisitor::exitDoWhileStmt(DoWhileStmt* node) {
  exitScope(node);
}
bool DeinitOrderVisitor::enterCForLoop(CForLoop* node) {
  return enterScope(node);
}
void DeinitOrderVisitor::exitCForLoop(CForLoop* node) {
  exitScope(node);
}
bool DeinitOrderVisitor::enterForLoop(ForLoop* node) {
  return enterScope(node);
}
void DeinitOrderVisitor::exitForLoop(ForLoop* node) {
  exitScope(node);
}

static Expr* findUnconditionalReturn(BlockStmt* block) {
  // Check, does the block always return?
  for (Expr* cur = block->body.first(); cur != nullptr; cur = cur->next) {
    if (GotoStmt* g = toGotoStmt(cur))
      if (g->gotoTag == GOTO_RETURN ||
          g->gotoTag == GOTO_ERROR_HANDLING_RETURN)
        return g;

    if (CallExpr* c = toCallExpr(cur))
      if (c->isPrimitive(PRIM_RETURN))
        return c;

    if (BlockStmt* b = toBlockStmt(cur))
      if (b->blockTag == BLOCK_NORMAL)
        if (Expr* e = findUnconditionalReturn(b))
          return e;

    if (CondStmt* c = toCondStmt(cur)) {
      Expr* thenRet = nullptr;
      Expr* elseRet = nullptr;
      if (c->thenStmt != nullptr)
        thenRet = findUnconditionalReturn(c->thenStmt);
      if (c->elseStmt != nullptr)
        elseRet = findUnconditionalReturn(c->elseStmt);
      if (thenRet != nullptr && elseRet != nullptr)
        return elseRet;
    }
  }

  return nullptr;
}

bool DeinitOrderVisitor::enterCondStmt(CondStmt* cond) {

  // consider anything in the condition to be in a new statement
  nextDeinitOrderNode(orderStack.back(), cond);

  // visit the conditional itself
  if (cond->condExpr) {
    cond->condExpr->accept(this);
  }

  if (debugging) {
    printOrderSummary(orderStack);
    printf(" cond %i\n", cond->id);
  }

  Expr* thenRet = nullptr;
  Expr* elseRet = nullptr;
  if (cond->thenStmt != nullptr)
    thenRet = findUnconditionalReturn(cond->thenStmt);
  if (cond->elseStmt != nullptr)
    elseRet = findUnconditionalReturn(cond->elseStmt);

  if (cond->thenStmt != nullptr) {
    bool skip = (thenRet != nullptr && elseRet == nullptr);
    if (skip)
      skipEarlyDeinitsForUnconditionalReturn++;

    DeinitOrderNode* thenNode = new DeinitOrderNode(cond->thenStmt);
    orderStack.back()->nestedOrder = thenNode;
    orderStack.push_back(thenNode);

    if (debugging) {
      printOrderSummary(orderStack);
      printf(" cond then %i skip=%i\n", cond->thenStmt->id, (int) skip);
    }

    cond->thenStmt->accept(this);

    INT_ASSERT(orderStack.back() == thenNode);
    clearDeinitOrderNodeSubtree(thenNode);
    orderStack.pop_back();

    if (skip)
      skipEarlyDeinitsForUnconditionalReturn--;
  }
  if (cond->elseStmt != nullptr) {
    bool skip = (elseRet != nullptr && thenRet == nullptr);

    if (skip)
      skipEarlyDeinitsForUnconditionalReturn++;

    DeinitOrderNode* elseNode = new DeinitOrderNode(cond->elseStmt);
    orderStack.back()->elseOrder = elseNode;
    orderStack.push_back(elseNode);

    if (debugging) {
      printOrderSummary(orderStack);
      printf(" cond else %i skip=%i\n", cond->elseStmt->id, (int) skip);
    }

    cond->elseStmt->accept(this);

    INT_ASSERT(orderStack.back() == elseNode);
    clearDeinitOrderNodeSubtree(elseNode);
    orderStack.pop_back();

    if (skip)
      skipEarlyDeinitsForUnconditionalReturn--;
  }

  // any calls after the cond statement should get a new order number
  nextDeinitOrderNode(orderStack.back(), nullptr);

  // already visited contained AST
  return false;
}

void DeinitOrderVisitor::noteUnconditionalReturnInParentBlocks(Expr* r) {
  // Go up blocks, avoiding loops and conditionals, marking their
  // end point in blockOrder.

  for (Expr* e = r; e != nullptr; e = e->parentExpr) {
    if (BlockStmt* b = toBlockStmt(e)) {
      if (b->blockTag == BLOCK_NORMAL)
        addToDeinitOrderTree(&lifetimes->blockOrder[b], orderStack.front());
      else
        return;
    }

    if (isCondStmt(e) || isForallStmt(e) || isLoopStmt(e))
      return;
  }
}

bool DeinitOrderVisitor::enterCallExpr(CallExpr* call) {
  // Traverse into task functions
  if (FnSymbol* calledFn = call->resolvedFunction()) {
    if (isTaskFun(calledFn)) {
      calledFn->body->accept(this);
      return false;
    }
  }

  // Ignore calls addressed by detemping
  if (lifetimes->callsToIgnore.count(call))
    return false;

  if (call->isPrimitive(PRIM_ASSIGN_ELIDED_COPY)) {
    SymExpr* se = toSymExpr(call->get(2));
    Symbol* sym = lifetimes->getCanonicalSymbol(se->symbol());

    if (skipEarlyDeinitsForUnconditionalReturn == 0 &&
        isAutoDestroyedVariable(sym)) {
      if (debugging) {
        printOrderSummary(orderStack);
        printf(" copy elide %s[%i] in call %i\n", sym->name, sym->id, call->id);
      }

      addToDeinitOrderTree(&lifetimes->order[sym], orderStack.front());
    } else if (debugging) {
      printOrderSummary(orderStack);
      printf(" skipping copy elide %s[%i] in call %i\n",
             sym->name, sym->id, call->id);
    }

    // no need to consider nested calls
    return false;
  }

  if (call->isPrimitive(PRIM_END_OF_STATEMENT)) {
    nextDeinitOrderNode(orderStack.back(), call);
  }

  if (call->isPrimitive(PRIM_RETURN)) {
    noteUnconditionalReturnInParentBlocks(call);
  }

  if (debugging) {
    printOrderSummary(orderStack);
    if (call->isPrimitive(PRIM_END_OF_STATEMENT))
      printf(" end of statement %i\n", call->id);
    else
      printf(" call %i\n", call->id);
  }

  // no need to visit nested calls
  return false;
}

bool DeinitOrderVisitor::enterGotoStmt(GotoStmt* g) {
  if (g->gotoTag == GOTO_RETURN ||
      g->gotoTag == GOTO_ERROR_HANDLING_RETURN) {
    noteUnconditionalReturnInParentBlocks(g);
  }
  return false;
}

/************************************* | **************************************
*                                                                             *
*  IntrinsicLifetimesVisitor                                                  *
*                                                                             *
************************************** | *************************************/


static bool isRecordInitOrReturn(CallExpr* call, Symbol*& lhs, CallExpr*& initOrCtor, LifetimeState* lifetimes) {

  SymExpr* gotLHS = nullptr;
  CallExpr* gotCall = nullptr;
  if (isRecordInitOrReturn(call, gotLHS, gotCall)) {
    INT_ASSERT(gotLHS);
    if (gotCall) {
      lhs = lifetimes->getCanonicalSymbol(gotLHS->symbol());
      initOrCtor = gotCall;
      return true;
    }
  }

  lhs = nullptr;
  initOrCtor = nullptr;
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

    // Arguments with e.g. lifetime arg > someGlobal
    // will have infinite lifetime
    if (isOuterLifetimeFromClause(*lifetimes, arg)) {
      lp = infiniteLifetimePair();
    }
  } else if (VarSymbol* var = toVarSymbol(sym)) {
    // Don't bother getting intrinsic lifetime for RVV
    if (var->hasFlag(FLAG_RVV))
        lp = unknownLifetimePair();

    // For local value variables, set ref lifetime to reachability.
    if (!var->isRef() && lifetimes->isLocalVariable(var)) {
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

  return true;
}

static bool isFunctionReturningNotOwned(FnSymbol* fn) {
  if (fn &&
      fn->hasEitherFlag(FLAG_RETURN_NOT_OWNED,
                        FLAG_RETURNS_ALIASING_ARRAY))
    return true;

  return false;
}

static bool isCallToFunctionReturningNotOwned(CallExpr* call) {
  FnSymbol* calledFn = call->resolvedOrVirtualFunction();
  return isFunctionReturningNotOwned(calledFn);
}



bool IntrinsicLifetimesVisitor::enterCallExpr(CallExpr* call) {

  if (call->id == debugLifetimesForId)
    gdbShouldBreakHere();

  // Traverse into task functions
  if (FnSymbol* calledFn = call->resolvedFunction())
    if (isTaskFun(calledFn))
      calledFn->body->accept(this);

  // Ignore calls that de-temping allows us to ignore
  if (lifetimes->callsToIgnore.count(call))
    return false;

  Symbol* initSym = nullptr;
  CallExpr* initCall = nullptr;

  Lifetime lt = unknownLifetime();

  if (isRecordInitOrReturn(call, initSym, initCall, lifetimes) &&
      lifetimes->shouldPropagateLifetimeTo(call, initSym)) {

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
        lifetimes->shouldPropagateLifetimeTo(rhsCall, initSym)) {
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

/************************************* | **************************************
*                                                                             *
*  InferLifetimesVisitor                                                      *
*                                                                             *
************************************** | *************************************/

bool InferLifetimesVisitor::enterCallExpr(CallExpr* call) {

  if (call->id == debugLifetimesForId)
    gdbShouldBreakHere();

  // Traverse into task functions
  if (FnSymbol* calledFn = call->resolvedFunction())
    if (isTaskFun(calledFn))
      calledFn->body->accept(this);

  // Ignore calls that de-temping allows us to ignore
  if (lifetimes->callsToIgnore.count(call))
    return false;

  Symbol* initSym = nullptr;
  CallExpr* initCall = nullptr;
  if (isRecordInitOrReturn(call, initSym, initCall, lifetimes) &&
      lifetimes->shouldPropagateLifetimeTo(call, initSym)) {

    AggregateType* at = toAggregateType(initSym->getValType());
    INT_ASSERT(at && isRecord(at));
    LifetimePair lp = unknownLifetimePair();

    if (aggregateContainsBorrowedClassFields(at) ||
        isCallToFunctionReturningNotOwned(initCall)) {
      lp = lifetimes->inferredLifetimeForCall(initCall);
    }
    changed |= lifetimes->setInferredLifetimeToMin(initSym, lp);

  } else if (isAnalyzedMoveOrAssignment(call)) {

    SymExpr* lhsSe = toSymExpr(call->get(1));
    Symbol* lhs = lifetimes->getCanonicalSymbol(lhsSe->symbol());

    if (lifetimes->shouldPropagateLifetimeTo(call, lhs)) {

      Expr* rhsExpr = call->get(2);

      bool usedAsRef = lhs->isRef() && rhsExpr->isRef();
      bool usedAsBorrow = isOrContainsBorrowedClass(lhs->type);

      LifetimePair lp = lifetimes->inferredLifetimeForExpr(rhsExpr,
                                                           usedAsRef,
                                                           usedAsBorrow);

      if (lhs->isRef() && rhsExpr->isRef() && call->isPrimitive(PRIM_MOVE)) {
        // When setting the reference, set its intrinsic lifetime.
        if (!lp.referent.unknown || !lp.borrowed.unknown) {
          if (lhs->id == debugLifetimesForId)
            gdbShouldBreakHere();

          LifetimePair & intrinsic = lifetimes->intrinsicLifetime[lhs];
          intrinsic = lifetimes->minimumLifetimePair(intrinsic, lp);
        }
      }

      if (!(lhs->isRef() && rhsExpr->isRef() && call->isPrimitive(PRIM_MOVE)))
        // lhs can't have ref lifetime if it isn't a ref
        lp.referent = unknownLifetime();

      if (!isOrContainsBorrowedClass(lhs->type))
        // lhs can't have borrow lifetime if it can't borrow
        lp.borrowed = unknownLifetime();

      lp.referent.relevantExpr = call;
      lp.borrowed.relevantExpr = call;
      changed |= lifetimes->setInferredLifetimeToMin(lhs, lp);

      // Additionally, if LHS is a reference to a known aggregate,
      // update that thing's borrow lifetime.
      if (lhs->isRef() && usedAsBorrow && call->isPrimitive(PRIM_MOVE)) {
        LifetimePair & intrinsic = lifetimes->intrinsicLifetime[lhs];
        if (!intrinsic.referent.unknown && intrinsic.referent.fromSymbolScope) {
          LifetimePair p = lp;
          p.referent = unknownLifetime();
          if (!p.borrowed.unknown &&
              !p.borrowed.infinite &&
              p.borrowed.returnScope) {
            Symbol* referredSym = intrinsic.referent.fromSymbolScope;
            changed |= lifetimes->setInferredLifetimeToMin(referredSym, lp);
          }
        }
      }
    }
  }

  // Consider the lifetime constraints and adjust the
  // inferred lifetimes of the results.
  inferLifetimesForConstraint(call);

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

  bool isForall = false;
  IteratorDetails leaderDetails;
  ForLoop* followerForLoop = nullptr;
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
    if (iterable != nullptr)
      if (AggregateType* at = toAggregateType(iterable->getValType()))
        if (at->iteratorInfo != nullptr)
          if (FnSymbol* fn = getTheIteratorFn(at))
            method = (fn->_this != nullptr);

    bool usedAsRef = index->isRef();
    bool usedAsBorrow = isOrContainsBorrowedClass(index->type);

    // Gather lifetime for iterable
    LifetimePair lp = unknownLifetimePair();
    if (iterableSe) {
      Symbol* iterableSym = iterableSe->symbol();

      lp = lifetimes->lifetimeForActual(iterableSym, usedAsRef, usedAsBorrow);

      // TODO: fold this in to lifetimeForActual
      if (symbolHasInfiniteLifetime(iterableSym)) {
        lp = infiniteLifetimePair();
      } else {
        if (!isSubjectToRefLifetimeAnalysis(index))
          lp.referent = unknownLifetime();
        if (!isSubjectToBorrowLifetimeAnalysis(index))
          lp.borrowed = unknownLifetime();
      }

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
        intrinsic = lifetimes->minimumLifetimePair(intrinsic, lp);
      }
    }

    // Set lifetime of iteration variable to lifetime of the iterable (expr).
    lp.referent.relevantExpr = forLoop;
    lp.borrowed.relevantExpr = forLoop;

    if (method == false) {
      LifetimePair loopScope = lifetimes->intrinsicLifetimeForSymbol(index);
      lp = lifetimes->minimumLifetimePair(lp, loopScope);
    }

    changed |= lifetimes->setInferredLifetimeToMin(index, lp);
  }

  return true;
}

void InferLifetimesVisitor::inferLifetimesForConstraint(CallExpr* forCall) {
  if (FnSymbol* fn = forCall->resolvedOrVirtualFunction()) {
    if (fn->lifetimeConstraints) {
      Expr* last = fn->lifetimeConstraints->body.last();
      inferLifetimesForConstraint(forCall, last);
    }
  }
}

void InferLifetimesVisitor::inferLifetimesForConstraint(CallExpr* forCall, Expr* constraintExpr) {
  if (CallExpr* constraint = toCallExpr(constraintExpr)) {
    if (constraint->isNamed(",")) {
      inferLifetimesForConstraint(forCall, constraint->get(1));
      inferLifetimesForConstraint(forCall, constraint->get(2));
    } else if (constraint->isNamed("=")) {
      Expr* a = constraint->get(1);
      Expr* b = constraint->get(2);
      ArgSymbol* constraintLhs = nullptr;
      ArgSymbol* constraintRhs = nullptr;
      bool ignore = false;
      constraintLhs = toArgSymbol(getSymbolFromLifetimeClause(a, ignore));
      constraintRhs = toArgSymbol(getSymbolFromLifetimeClause(b, ignore));
      Symbol* actualLhs = nullptr;
      Symbol* actualRhs = nullptr;
      if (constraintLhs && constraintRhs) {
        for_formals_actuals(formal, actualExpr, forCall) {
          if (SymExpr* actualSe = toSymExpr(actualExpr)) {
            if (formal == constraintLhs)
              actualLhs = actualSe->symbol();
            if (formal == constraintRhs)
              actualRhs = actualSe->symbol();
          }
        }
      }
      LifetimePair lp = lifetimes->inferredLifetimeForSymbol(actualRhs);
      changed = lifetimes->setInferredLifetimeToMin(actualLhs, lp);
    }
  }
}


/************************************* | **************************************
*                                                                             *
*  EmitLifetimeErrorsVisitor                                                  *
*                                                                             *
************************************** | *************************************/

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

  BaseAST* place = findUserPlace(inExpr);

  if (relevantSymbol && !relevantSymbol->hasFlag(FLAG_TEMP)) {
    USR_FATAL_CONT(place, "%s %s %s", msg1, relevantSymbol->name, msg2);
  } else {
    USR_FATAL_CONT(place, "%s %s", msg1, msg2);
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

  // Traverse into task functions
  if (FnSymbol* calledFn = call->resolvedFunction())
    if (isTaskFun(calledFn))
      calledFn->body->accept(this);

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

  // If it's a call to a user function that has lifetime constraints,
  // check that the call site meets those constraints.
  if (FnSymbol* fn = call->resolvedOrVirtualFunction()) {
    if (fn->lifetimeConstraints) {
      int i, j;
      // Consider all pairs of actuals.
      // I'm sure there's a more efficient way to do this.
      i = 0;
      for_formals_actuals(formal1, actual1expr, call) {
        i++;
        SymExpr* actual1se = toSymExpr(actual1expr);
        INT_ASSERT(actual1se);
        Symbol* actual1sym = actual1se->symbol();

        j = 0;
        for_formals_actuals(formal2, actual2expr, call) {
          j++;

          // Only consider j > i
          if (j <= i) continue;

          SymExpr* actual2se = toSymExpr(actual2expr);
          INT_ASSERT(actual2se);
          Symbol* actual2sym = actual2se->symbol();

          // Determine if there is a lifetime constraint for these two
          // formals.
          constraint_t order = orderConstraintFromClause(fn, formal1, formal2);
          if (order != CONSTRAINT_UNKNOWN && order != CONSTRAINT_EQUAL) {
            LifetimePair a1lp =
              lifetimes->combinedLifetimeForSymbol(actual1sym);
            LifetimePair a2lp =
              lifetimes->combinedLifetimeForSymbol(actual2sym);

            Lifetime relevantLifetime;
            Symbol* relevantSymbol = nullptr;
            bool error = false;
            bool ref = false;

            // For the purposes of this check, use scope lifetime
            // if above lifetime is unknown.
            if (a1lp.borrowed.unknown)
              a1lp.borrowed = scopeLifetimeForSymbol(actual1sym);
            if (a2lp.borrowed.unknown)
              a2lp.borrowed = scopeLifetimeForSymbol(actual2sym);


            // see also
            //   arrays/ferguson/pushback-no-leak.chpl
            if (isOrRefersBorrowedClass(formal1->getValType()) &&
                isOrRefersBorrowedClass(formal2->getValType())) {
              if ((order == CONSTRAINT_LESS || order == CONSTRAINT_LESS_EQ) &&
                  lifetimes->isLifetimeShorter(a2lp.borrowed, a1lp.borrowed)) {
                error = true;
                ref = false;
                relevantLifetime = a2lp.borrowed;
                relevantSymbol = actual2sym;
              } else if ((order == CONSTRAINT_GREATER ||
                          order == CONSTRAINT_GREATER_EQ) &&
                         lifetimes->isLifetimeShorter(a1lp.borrowed,
                                                      a2lp.borrowed)) {
                error = true;
                ref = false;
                relevantLifetime = a1lp.borrowed;
                relevantSymbol = actual1sym;
              }
            }

            if (error) {
              emitError(call,
                        ref?"Reference actual argument":"Actual argument",
                        "does not meet called function lifetime constraint",
                        relevantSymbol, relevantLifetime, lifetimes);
              printOrderConstraintFromClause(fn, formal1, formal2);
            }
          }
        }
      }
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
  Symbol* rhsSym = nullptr;
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
    } else if (lifetimes->isLifetimeShorter(rhsLt.referent,
                                            lhsIntrinsic.referent)) {
      emitError(call,
                "Reference to scoped variable",
                "would outlive the value it refers to",
                lhs, rhsLt.referent, lifetimes);
      erroredSymbols.insert(lhs);
    } else if (lhsInferred.referent.unknown ||
               lhsInferred.referent.infinite) {
      // OK, not an error
    } else if (lifetimes->isLifetimeShorter(rhsLt.referent,
                                            lhsInferred.referent)) {
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
    } else if (lifetimes->isLifetimeShorter(rhsLt.borrowed,
                                            lhsIntrinsic.borrowed)) {
      emitError(call,
                "Scoped variable",
                "would outlive the value it is set to",
                lhs, rhsLt.borrowed, lifetimes);
      erroredSymbols.insert(lhs);
    } else if (lifetimes->
                 isLifetimeUnspecifiedFormalOrdering(rhsLt.borrowed,
                                                     lhsIntrinsic.borrowed)) {
      emitError(call,
                "Setting borrowed formal",
                "illegal without specifying formal lifetime constraint",
                lhs, rhsLt.borrowed, lifetimes);
      erroredSymbols.insert(lhs);
    } else if (lhsInferred.borrowed.unknown ||
               lhsInferred.borrowed.infinite) {
      // OK, not an error
    } else if (lifetimes->isLifetimeShorter(rhsLt.borrowed,
                                            lhsInferred.borrowed)) {
      emitError(call,
                "Scoped variable",
                "would outlive the value it is set to",
                lhs, rhsLt.borrowed, lifetimes);
      erroredSymbols.insert(lhs);
    } else if (lifetimes->
                 isLifetimeUnspecifiedFormalOrdering(rhsLt.borrowed,
                                                     lhsInferred.borrowed)) {
      emitError(call,
                "Setting borrowed formal",
                "illegal without specifying formal lifetime constraint",
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
    } else if (lifetimes->isLifetimeShorter(rhsLt.referent,
                                            lhsIntrinsic.referent)) {
      emitError(call,
                "Reference field",
                "would outlive the value it refers to",
                field, rhsLt.referent, lifetimes);
      erroredSymbols.insert(lhs);
    } else if (lhsInferred.referent.unknown ||
               lhsInferred.referent.infinite) {
      // OK, not an error
    } else if (lifetimes->isLifetimeShorter(rhsLt.referent,
                                            lhsInferred.referent)) {
      emitError(call,
                "Reference field",
                "would outlive the value it refers to",
                field, rhsLt.referent, lifetimes);
      erroredSymbols.insert(lhs);
    }
    // TODO unspecified formal lifetimes?
  }

  if (isOrContainsBorrowedClass(field->type)) {
    // setting a borrow, so check borrow lifetimes
    if (lhsIntrinsic.borrowed.unknown) {
      // OK, not an error
    } else if (lifetimes->isLifetimeShorter(rhsLt.borrowed,
                                            lhsIntrinsic.borrowed)) {
      emitError(call,
                "Field",
                "would outlive the value it is set to",
                field, rhsLt.borrowed, lifetimes);
      erroredSymbols.insert(lhs);
    } else if (lhsInferred.borrowed.unknown ||
               lhsInferred.borrowed.infinite) {
      // OK, not an error
    } else if (lifetimes->isLifetimeShorter(rhsLt.borrowed,
                                            lhsInferred.borrowed)) {
      emitError(call,
                "Field",
                "would outlive the value it is set to",
                field, rhsLt.borrowed, lifetimes);
      erroredSymbols.insert(lhs);
    }
    // TODO unspecified formal lifetimes?
  }
}


void EmitLifetimeErrorsVisitor::emitErrors() {
  for (SymbolToLifetimeMap::iterator it = lifetimes->inferredLifetime.begin();
       it != lifetimes->inferredLifetime.end();
       ++it) {
    Symbol* key = it->first;
    LifetimePair inferred = it->second;

    LifetimePair intrinsic = lifetimes->intrinsicLifetimeForSymbol(key);

    Lifetime scope = scopeLifetimeForSymbol(key);

    // Ignore the RVV for this check since that's tested in acceptCall
    // (see test lifetimes/bug-like-timezones.chpl)
    if (key->hasEitherFlag(FLAG_RVV,FLAG_RETARG))
      continue;

    // Don't emit errors again when we already made an error for that symbol
    if (erroredSymbols.count(key))
      continue;

    bool user = !key->hasFlag(FLAG_TEMP);

    if (key->isRef()) {
      if (// check if intrinsic lifetime < symbol lifetime
          (!intrinsic.referent.unknown && user &&
           lifetimes->isLifetimeShorter(intrinsic.referent, scope)) ||
          // check if inferred lifetime < symbol lifetime
          (!inferred.referent.unknown && user &&
           lifetimes->isLifetimeShorter(inferred.referent, scope)) ||
          // check if inferred lifetime < intrinsic lifetime
          (!inferred.referent.unknown &&
           !intrinsic.referent.unknown &&
           lifetimes->isLifetimeShorter(inferred.referent,
                                        intrinsic.referent))) {
        Expr* at = key->defPoint;
        if (inferred.referent.relevantExpr)
          at = inferred.referent.relevantExpr;

        emitError(at,
                  "Reference to scoped variable",
                  "reachable after lifetime ends",
                  key, inferred.referent, lifetimes);
      }
    }

    if (isOrContainsBorrowedClass(key->type)) {
      if (// check if intrinsic lifetime < symbol lifetime
          (!intrinsic.borrowed.unknown && user &&
           lifetimes->isLifetimeShorter(intrinsic.borrowed, scope)) ||
          // check if inferred lifetime < symbol lifetime
          (!inferred.borrowed.unknown && user &&
           lifetimes->isLifetimeShorter(inferred.borrowed, scope)) ||
          // check if inferred lifetime < intrinsic lifetime
          (!inferred.borrowed.unknown &&
           !intrinsic.borrowed.unknown &&
           lifetimes->isLifetimeShorter(inferred.borrowed,
                                        intrinsic.borrowed))) {
        Expr* at = key->defPoint;
        if (inferred.borrowed.relevantExpr)
          at = inferred.borrowed.relevantExpr;

        emitError(at,
                  "Scoped variable",
                  "reachable after lifetime ends",
                  key, inferred.borrowed, lifetimes);
      }
    }

    // check refs of borrows
    if (isOrContainsBorrowedClass(key->type) &&
        !inferred.borrowed.unknown &&
        !inferred.referent.unknown &&
        lifetimes->isLifetimeShorter(inferred.borrowed, inferred.referent)) {
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

bool outlivesBlock(LifetimeInformation* info, Symbol* sym, BlockStmt* block) {
  LifetimeState* lifetimes = info->lifetimes;
  INT_ASSERT(lifetimes);
  LifetimePair lp = lifetimes->inferredLifetimeForSymbol(sym);
  bool referentOutlivesBlock = false;
  if (lp.referent.unknown == false) {
    if (lp.referent.infinite || lp.referent.returnScope) {
      referentOutlivesBlock = true;
    } else if (lp.referent.fromSymbolScope != nullptr) {
      Stmt* referentDefBlock = getDefBlock(lp.referent.fromSymbolScope);
      if (isBlockWithinBlock(referentDefBlock, block) == false)
        referentOutlivesBlock = true;
    }
  }
  return referentOutlivesBlock;
}

// variables with a type with `infinite borrow lifetime` should have
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

  if (DecoratedClassType* dt = toDecoratedClassType(type))
    if (dt->isUnmanaged())
      // unmanaged class instances have infinite lifetime
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

static bool aggregateContainsBorrowedClassFields(AggregateType* at) {

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

bool isOrContainsBorrowedClass(Type* type) {
  type = type->getValType();

  // First check for it is a borrowed class type
  if (isClassLike(type) &&
      !typeHasInfiniteBorrowLifetime(type))
    return true; // borrow -- `unmanaged` has `infinite lifetime`

  // Next check for records containing borrowed class type
  if (AggregateType* at = toAggregateType(type))
    if (at->isRecord())
      return aggregateContainsBorrowedClassFields(at);

  return false;
}

static bool isOrRefersBorrowedClass(Type* type) {
  if (isOrContainsBorrowedClass(type))
    return true;

  if (type->symbol->hasFlag(FLAG_ARRAY_OF_BORROWS))
    return true;

  if (AggregateType* at = toAggregateType(type))
    if (aggregateContainsBorrowedClassFields(at))
      return true;

  return false;
}

static bool isSubjectToBorrowLifetimeAnalysis(Type* type) {
  type = type->getValType();

  if (!(isRecord(type) || isClassLikeOrPtr(type)))
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
  if (!(isClassLikeOrPtr(type) ||
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


bool LifetimeState::shouldPropagateLifetimeTo(CallExpr* call, Symbol* sym) {

  if (!(isSubjectToRefLifetimeAnalysis(sym) ||
        isSubjectToBorrowLifetimeAnalysis(sym)))
    return false;

  if (!isLocalVariable(sym))
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

  if (call->isPrimitive(PRIM_MOVE) ||
      call->isPrimitive(PRIM_ASSIGN) ||
      call->isPrimitive(PRIM_ASSIGN_ELIDED_COPY))
    return true;

  FnSymbol* calledFn = call->resolvedOrVirtualFunction();

  // Only consider compiler-generated function calls to "=" for lifetime
  // propagation. Only consider those functions that are operating
  // on a record containing borrows or on class instances.
  //
  // (we assume that a user supplied = function for a class with owned
  //  pointers handles lifetimes in a reasonable manner)
  if (calledFn && calledFn->name == astrSassign) {
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
        if (aggregateContainsBorrowedClassFields(at))
          return true;
  }

  return false;
}

/*
   Global variables have infinite lifetime.
 */
static bool symbolHasInfiniteLifetime(Symbol* sym) {

  if (sym == nullptr)
    return true;

  Symbol* parentSymbol = sym->defPoint->parentSymbol;
  if (isModuleSymbol(parentSymbol)) {
    // Global variables have infinite lifetime
    return true;
  }

  return false;
}

// Returns a BlockStmt or a ForallStmt
static Stmt* getDefBlock(Symbol* sym) {
  Expr* defPoint = sym->defPoint;

  if (isTaskFunctionFormal(sym)) {
    // Use the function's block
    FnSymbol* fn = sym->getFunction();
    return fn->body;
  }

  while (defPoint) {
    if (BlockStmt* block = toBlockStmt(defPoint))
      if (block->blockTag == BLOCK_NORMAL)
        return block;

    if (ForallStmt* fs = toForallStmt(defPoint))
      return fs;

    if (ShadowVarSymbol* sv = toShadowVarSymbol(defPoint->parentSymbol))
      defPoint = sv->defPoint;

    defPoint = defPoint->parentExpr;
  }
  return nullptr;
}

static Expr* getParentExprIncludingTaskFnCalls(Expr* cur) {
  Expr* parent = cur->parentExpr;
  if (parent == nullptr) {
    if (FnSymbol* inFn = cur->getFunction()) {
      if (isTaskFun(inFn)) {
        for_SymbolSymExprs(se, inFn) {
          CallExpr* call = toCallExpr(se->parentExpr);
          if (se == call->baseExpr) {
            // Consider the call point the "parent"
            return call;
          }
        }
      }
    }
  }

  return parent;
}

// This could definitely be implemented in a faster way.
// Works with ForallStmts and BlockStmts.
static bool isBlockWithinBlock(Stmt* a, Stmt* b) {
  Expr* findParent = b;
  for (Expr* cur = a; cur; cur = getParentExprIncludingTaskFnCalls(cur)) {
    if (cur == findParent)
      return true;
  }
  return false;
}

bool LifetimeState::isLifetimeUnspecifiedFormalOrdering(Lifetime a,
                                                        Lifetime b) {
  if (isLifetimeShorter(a, b)) // a < b
    return false;
  else if (isLifetimeShorter(b, a)) // b < a
    return false;
  else if (a.fromSymbolScope == b.fromSymbolScope)
    return false;
  else if (!isArgSymbol(a.fromSymbolScope) || !isArgSymbol(b.fromSymbolScope))
    return false;
  else if (a.fromSymbolScope->defPoint->parentSymbol !=
           b.fromSymbolScope->defPoint->parentSymbol)
    // Not the same function
    return false;

  ArgSymbol* aArg = toArgSymbol(a.fromSymbolScope);
  ArgSymbol* bArg = toArgSymbol(b.fromSymbolScope);
  FnSymbol* fn = a.fromSymbolScope->defPoint->getFunction();
  INT_ASSERT(fn != nullptr && aArg != nullptr && bArg != nullptr);

  constraint_t c = orderConstraintFromClause(fn, aArg, bArg);
  if (c != CONSTRAINT_UNKNOWN)
    return false;
  // TODO - make this exception more reasonable
  if (fn->getModule()->modTag != MOD_USER) {
    if (fn->name == astrSassign)
      return false;
    if (fn->name == astrSswap)
      return false;
  }

  return true;
}

/* Consider two variables/arguments a and b.
   Is the lifetime of a strictly within the lifetime for b?
      - e.g. if a is declared in a block nested inside the lifetime of b
 */
bool LifetimeState::isLifetimeShorter(Lifetime a, Lifetime b) {
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
    if (isArgSymbol(aSym) && isArgSymbol(bSym)) {
      FnSymbol* aFn = aSym->getFunction();
      FnSymbol* bFn = bSym->getFunction();
      if (aFn == bFn && aFn->lifetimeConstraints != nullptr) {
        // TODO: transitivity ?
        constraint_t c = orderConstraintFromClause(aFn, aSym, bSym);
        return (c == CONSTRAINT_LESS || c == CONSTRAINT_LESS_EQ);
      }
    }

    // Check order[aSym] vs order[bSym]
    DeinitOrderNode* aNode = deinitOrderNodeFor(aSym);
    DeinitOrderNode* bNode = deinitOrderNodeFor(bSym);
    DeinitOrderNode* aCur = aNode;
    DeinitOrderNode* bCur = bNode;
    bool anyElse = false;

    while (aCur != nullptr && bCur != nullptr) {
      if (aCur->order < bCur->order)
        return true;
      if (aCur->order > bCur->order)
        return false;

      if (aCur->elseOrder || bCur->elseOrder)
        anyElse = true;

      aCur = aCur->nestedOrder;
      bCur = bCur->nestedOrder;
    }

    if (anyElse)
      if (helpIsLifetimeShorter(aNode, bNode))
        return true;

    // Otherwise, the order was similar. Check for FLAG_DEAD_LAST_MENTION
    // differences.
    bool aMaybeDeadEarly = aSym->hasFlag(FLAG_DEAD_LAST_MENTION);
    bool bMaybeDeadEarly = bSym->hasFlag(FLAG_DEAD_LAST_MENTION);

    if      (aMaybeDeadEarly == false && bMaybeDeadEarly == false)
      return false;  // don't worry about order for end-of-block decls
                     // doing so would prevent swap from working.
    else if (aMaybeDeadEarly == true  && bMaybeDeadEarly == true)
      return false;  // OK: call-temps deinited together
    else if (aMaybeDeadEarly == true  && bMaybeDeadEarly == false)
      return true;   // a has shorter lifetime than b
    else if (aMaybeDeadEarly == false && bMaybeDeadEarly == true)
      return false;   // b has shorter lifetime than a
  }

  return false;
}

bool LifetimeState::helpIsLifetimeShorter(DeinitOrderNode* aNode,
                                          DeinitOrderNode* bNode) {
  DeinitOrderNode* aCur = aNode;
  DeinitOrderNode* bCur = bNode;

  if (aCur->order < bCur->order)
    return true;
  if (aCur->order > bCur->order)
    return false;

  bool shorter = false;
  if (aCur->nestedOrder && bCur->nestedOrder)
    shorter |= helpIsLifetimeShorter(aCur->nestedOrder, bCur->nestedOrder);
  if (aCur->elseOrder && bCur->elseOrder)
    shorter |= helpIsLifetimeShorter(aCur->elseOrder, bCur->elseOrder);

  return shorter;
}

DeinitOrderNode* LifetimeState::deinitOrderNodeFor(Symbol* sym) {
  DeinitOrderMap::iterator it = order.find(sym);
  if (it != order.end())
    return &it->second;

  // otherwise, use the order from the final location in the defining
  // block as recorded in blockOrder
  Stmt* block = getDefBlock(sym);
  if (block == nullptr && isArgSymbol(sym))
    block = sym->defPoint->getFunction()->body;

  DeinitOrderBlockMap::iterator it2 = blockOrder.find(block);
  INT_ASSERT(it2 != blockOrder.end());
  return &it2->second;
}

Lifetime LifetimeState::minimumLifetime(Lifetime a, Lifetime b) {
  if (isLifetimeShorter(a, b))
    return a;
  else
    return b;
}


LifetimePair LifetimeState::minimumLifetimePair(LifetimePair a,
                                                LifetimePair b) {
  LifetimePair ret;
  ret.referent = minimumLifetime(a.referent, b.referent);
  ret.borrowed = minimumLifetime(a.borrowed, b.borrowed);
  return ret;
}

static Lifetime scopeLifetimeForSymbol(Symbol* sym) {
  Lifetime lt;
  lt.fromSymbolScope = sym;
  lt.relevantExpr = nullptr;
  lt.unknown = false;
  lt.infinite = symbolHasInfiniteLifetime(sym);
  lt.returnScope = false;
  return lt;
}

static Lifetime unknownLifetime() {
  Lifetime lt;
  lt.fromSymbolScope = nullptr;
  lt.relevantExpr = nullptr;
  lt.unknown = true;
  lt.infinite = false;
  lt.returnScope = false;
  return lt;
}

static Lifetime infiniteLifetime() {
  Lifetime lt;
  lt.fromSymbolScope = nullptr;
  lt.relevantExpr = nullptr;
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
