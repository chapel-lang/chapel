/*
 * Copyright 2004-2017 Cray Inc.
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

#include "passes.h"

#include "astutil.h"
#include "expr.h"
#include "ForLoop.h"
#include "iterator.h"
#include "resolution.h"
#include "stlUtil.h"
#include "stmt.h"
#include "symbol.h"

/* This pass implements a backwards (uses to defs) analysis
   to determine if certain reference Symbols are constant.

   The are two language features that require this backwards
   data flow (and they are related to each other).
   The first is the default argument intent for arrays - which is
   `const ref` if the array is not modified in the body of
   the function and `ref` if it is. The second is the `ref-pair`
   feature which was created for sparse arrays.

   This pass proceeds for the most part by collecting
   Symbols needing special attention and determining which
   of them can be marked as constant. (For a ContextCallExpr,
   the relevant Symbol is the temporary that stores the result
   of the ContextCallExpr). Next, it determines which of thes
   Symbols can be `const ref` and which must continue to be
   mutable `ref`. The general strategy there is to keep track
   of symbols of unknown const-ness.

   Then, it performs appropriate transformations,
   which amount to updating ContextCallExprs to use
   one call or the other (the ref-return version or
   the value/const-ref-return version).
 */

//static bool symbolIsSetLocal(Symbol* sym);
static bool symExprIsSet(SymExpr* sym);
static void lowerContextCall(ContextCallExpr* cc, bool useSetter);
static bool firstPassLowerContextCall(ContextCallExpr* cc);
static void lateConstCheck(std::map<BaseAST*, BaseAST*> & reasonNotConst);

static bool
symExprIsSetByDef(SymExpr* def) {
    // We're only looking for things that set the value.
    // We don't care about PRIM_MOVEs b/c they only set the reference.
    // We do care about PRIM_ASSIGN or if the argument is passed
    // to a function (typically = ) as ref, inout, or out argument.
    if (def->parentExpr) {
      if (CallExpr* parentCall = toCallExpr(def->parentExpr)) {
        if (parentCall->isPrimitive(PRIM_MOVE)) {
          // Ignore this def
          // We don't care about a PRIM_MOVE because it's setting
          // a reference
        } else {
          return true;
        }
      }
    }

    return false;
}

static bool
symExprIsSetByUse(SymExpr* use) {
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      if (FnSymbol* fn = call->isResolved()) {
        ArgSymbol* formal = actual_to_formal(use);

        // added in f2bc2b27
        //if (formal->defPoint->getFunction()->_this == formal)
        //  return true;

        if (formal->intent == INTENT_INOUT || formal->intent == INTENT_OUT) {
          // Shouldn't this be a Def, not a Use, then?
          INT_ASSERT(0);
          return true;
        }

        if (formal->type->symbol->hasFlag(FLAG_REF) &&
            (fn->hasFlag(FLAG_ALLOW_REF) ||
             formal->hasFlag(FLAG_WRAP_WRITTEN_FORMAL))) {
          // This case has to do with wrapper functions (promotion?)
          return true;
        }

        /*
      } else if (call->isPrimitive(PRIM_MOVE)) {

        // TODO: remove this branch (handled already)

        // If it's move lhs rhs, and se is rhs
        // ref is necessary for rhs if ref is necessary for lhs
        // Does this only come up for compiler-introduced moves?
        if (symbolIsSetLocal(toSymExpr(call->get(1))->symbol()))
          return true;

      } else if (call->isPrimitive(PRIM_ADDR_OF) ||
                 call->isPrimitive(PRIM_SET_REFERENCE)) {
        // TODO: remove this branch (handled already)

        // Handles inner functions
        if (CallExpr* parentCall = toCallExpr(call->parentExpr)) {
          if (parentCall->isPrimitive(PRIM_MOVE)) {
            if (symbolIsSetLocal(toSymExpr(parentCall->get(1))->symbol()))
              return true;
          }
        }

      } else if (call->isPrimitive(PRIM_GET_MEMBER) ||
                 call->isPrimitive(PRIM_GET_SVEC_MEMBER)) {
        // Getting a reference to a record/tuple member
        // the se would be the record/tuple variable
        CallExpr* move = toCallExpr(call->parentExpr);

        INT_ASSERT(move);
        INT_ASSERT(move->isPrimitive(PRIM_MOVE));

        if (symbolIsSetLocal(toSymExpr(move->get(1))->symbol()))
          return true;

         */
      } else if (call->isPrimitive(PRIM_SET_MEMBER)) {
        // PRIM_SET_MEMBER to set the pointer inside of a reference
        // counts as "setter"
        // the below conditional would better be isRefType()
        if (!call->get(2)->typeInfo()->refType)
          return true;

      } else if (call->isPrimitive(PRIM_RETURN) ||
                 call->isPrimitive(PRIM_YIELD)) {
        FnSymbol* inFn = toFnSymbol(call->parentSymbol);
        // It is not necessary to use the 'ref' version
        // if the function result is returned by 'const ref'.
        if (inFn->retTag == RET_CONST_REF) return false;
        // MPF: it seems to cause problems to return false
        // here when inFn->retTag is RET_VALUE.
        // TODO: can we add
        //if (inFn->retTag == RET_VALUE) return false;
        return true;

      } else if (call->isPrimitive(PRIM_WIDE_GET_LOCALE) ||
                 call->isPrimitive(PRIM_WIDE_GET_NODE)) {
        // If we are extracting a field from the wide pointer,
        // we need to keep it as a pointer.
        // Dereferencing would be premature.

        // MPF: This seems like a workaround to me
        // How can this be right for a ref-pair returning const-ref/ref?
        return true;
      }
    }

    return false;
}

static
bool symExprIsSet(SymExpr* se)
{
  // The ref is necessary if it is for an explicit ref var
  if (se->symbol()->hasFlag(FLAG_REF_VAR)) {
    return true;
  }

  // a ref is not necessary if the LHS is a value
  // (this can come up in recursive handling of a PRIM_MOVE).
  if (!se->symbol()->isRef())
    return false;

  // A ref is not necessary if we've already determined
  // that the ref symbol is `const` for some reason.
  if (se->symbol()->qualType().isConst())
    return false;

  bool ret = false;
  int defOrUse = isDefAndOrUse(se);
  if (defOrUse & 1) { // def
    ret |= symExprIsSetByDef(se);
  }
  if (defOrUse & 2) { // use
    ret |= symExprIsSetByUse(se);
  }

  return ret;
}

/*

// Figure out if a symbol is set based upon its defs and uses.
// Does not properly handle certain cases, such as
// context calls. Used to handle extra temporaries
// sometimes added by the compiler.
static
bool symbolIsSetLocal(Symbol* sym)
{
  for_SymbolSymExprs(se, sym) {
    if (symExprIsSet(se))
      return true;
  }
  return false;
}
*/

static
bool callSetsSymbol(Symbol* sym, CallExpr* call)
{
  bool ret = false;
  for_alist(expr, call->argList) {
    if (SymExpr* se = toSymExpr(expr)) {
      if (se->symbol() == sym) {
        if (symExprIsSet(se)) {
          ret = true;
        }
      }
    }
  }
  return ret;
}

// Would choosing the refCall or valueCall change our determination
// about sym? If so, this function returns true.
static
bool contextCallItDepends(Symbol* sym, ContextCallExpr* cc) {
  CallExpr* refCall = cc->getRefCall();
  CallExpr* valueCall = cc->getRValueCall();

  if (callSetsSymbol(sym, refCall) != callSetsSymbol(sym, valueCall) ) {
    return true;
  }

  return false;
}

typedef std::map<Symbol*, std::vector<Symbol*> > revisitGraph_t;
typedef std::set<Symbol*> revisitUnknowns_t;

static
void markConst(Symbol* sym)
{
  ArgSymbol* arg = toArgSymbol(sym);
  // it is const, mark it so
  INT_ASSERT(sym->qual == QUAL_REF || sym->qual == QUAL_UNKNOWN);
  sym->qual = QUAL_CONST_REF;
  if (arg) arg->intent = INTENT_CONST_REF;
}
static
void markNotConst(Symbol* sym)
{
  ArgSymbol* arg = toArgSymbol(sym);

  // This could set sym->qual to e.g. QUAL_REF
  // but currently QUAL_REF is used both for
  // ref-with-unknown-constness and ref-not-const,
  // so we can just leave it alone.
  INT_ASSERT(!sym->qualType().isConst());
  if (arg) arg->intent = INTENT_REF;
}



/* Given a sym that we just decided should be considered non-const,
   propagate that information through the graph of dependent
   symbols.
 */
static
void transitivelyMarkNotConst(Symbol* sym,
                              revisitGraph_t & graph,
                              revisitUnknowns_t & unknownConstSyms,
                              std::map<BaseAST*, BaseAST*> & reasonNotConst)
{
  std::vector<Symbol*> & dependentSymbols = graph[sym];

  for_vector(Symbol, otherSym, dependentSymbols) {
    if (unknownConstSyms.count(otherSym) != 0) {
      // otherSym still has unknown const-ness
      // mark it as not-const
      markNotConst(otherSym);
      reasonNotConst[otherSym] = sym;
      unknownConstSyms.erase(otherSym);
      transitivelyMarkNotConst(otherSym, graph, unknownConstSyms, reasonNotConst);
    }
  }
}

static
bool inBuildTupleForChplIter(SymExpr* se)
{
  if (CallExpr* maybeBuildTuple = toCallExpr(se->parentExpr))
    if (CallExpr* maybeMove = toCallExpr(maybeBuildTuple->parentExpr))
      if (FnSymbol* maybeBuildTupleFn = maybeBuildTuple->isResolved())
        if (maybeBuildTupleFn->hasFlag(FLAG_BUILD_TUPLE))
          if (maybeMove->isPrimitive(PRIM_MOVE)) {
            SymExpr* lhs = toSymExpr(maybeMove->get(1));
            if (lhs->symbol()->hasFlag(FLAG_CHPL__ITER))
              return true;
          }
  return false;
}

// Get the non-fast-follower Follower
static
ForLoop* findFollowerForLoop(BlockStmt* block) {
  ForLoop* ret = NULL;
  Expr* e = block->body.first();
  while (e) {
    if (ForLoop* forLoop = toForLoop(e)) {
      return forLoop;
    }
    if (BlockStmt* blk = toBlockStmt(e)) {
      ret = findFollowerForLoop(blk);
      if (ret) return ret;
    }
    if (CondStmt* cond = toCondStmt(e)) {
      // Look in the else block to find the non-fast-follower
      // in case it is decided at run-time whether fast
      // followers can be used.
      ret = findFollowerForLoop(cond->elseStmt);
      if (ret) return ret;
    }
    e = e->next;
  }
  return NULL;
}

struct IteratorDetails {
  Expr*     iterable;
  Symbol*   index;
  Type*     iteratorClass;
  FnSymbol* iterator;

  IteratorDetails()
    : iterable(NULL), index(NULL), iteratorClass(NULL), iterator(NULL)
  {
  }
};


/* Collapse compiler-introduced copies of references
   to variables marked "index var"
   This handles chpl__saIdxCopy
 */
static
Symbol* collapseIndexVarReferences(Symbol* index)
{
  bool changed;
  do {
    changed = false;
    for_SymbolSymExprs(se, index) {
      if (CallExpr* move = toCallExpr(se->parentExpr)) {
        if (move->isPrimitive(PRIM_MOVE)) {
          SymExpr* lhs = toSymExpr(move->get(1));
          SymExpr* rhs = toSymExpr(move->get(2));
          INT_ASSERT(lhs && rhs); // or not normalized
          if (lhs->symbol()->hasFlag(FLAG_INDEX_VAR) &&
              rhs->symbol() == index) {
            changed = true;
            index = lhs->symbol();
          }
        }
      }
    }
  } while (changed == true);

  return index;
}

/* Sets detailsVector[i].index if possible
   Handles syntactically unpacked tuples such as

   for (a, b) in zip(A, B) { ... }
 */
static
void findZipperedIndexVariables(Symbol* index, std::vector<IteratorDetails>&
    detailsVector)
{
  // Now, in a zippered-for, the index is actually
  // a tuple (of references typically). We need to find the
  // un-packed elements.

  for_SymbolSymExprs(indexSe, index) {
    if (CallExpr* indexSeParentCall =
        toCallExpr(indexSe->parentExpr)) {
      if (indexSeParentCall->isPrimitive(PRIM_GET_MEMBER) ||
          indexSeParentCall->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
        AggregateType* tupleType = toAggregateType(index->type);

        if (CallExpr* gpCall =
            toCallExpr(indexSeParentCall->parentExpr)) {
          if (gpCall->isPrimitive(PRIM_MOVE)) {
            SymExpr* lhsSe = toSymExpr(gpCall->get(1));
            SymExpr* gottenFieldSe = toSymExpr(indexSeParentCall->get(2));
            int i = 0;
            for_fields(field, tupleType) {
              if (gottenFieldSe->symbol() == field) {
                // setting .index twice probably means we are working
                // with some new & different AST
                INT_ASSERT(!detailsVector[i].index);
                detailsVector[i].index = lhsSe->symbol();
              }
              i++;
            }
          }
        }
      }
    }
  }

}


// TODO -- move this and related code to ForLoop.cpp

/* Gather important information about a loop.

leaderDetails is only relevant for leader/follower iterators
              and in that case refers to details of the leader loop.
followerForLoop is the follower ForLoop in leader/follower iteration.

When considering zippered iteration, detailsVector contains details of
the individually zippered iterations.

When considering non-zippered iteration, detailsVector contains exactly
one element. It storesinformation
about the loop. In leader/follower loops, it contains information about
the follower loop.

Always uses the non-fast-follower version of the follower loop.
 */
static
void gatherLoopDetails(ForLoop*  forLoop,
                       bool&     isForall,
                       IteratorDetails& leaderDetails,
                       ForLoop*& followerForLoop,
                       std::vector<IteratorDetails>& detailsVector)
{
  Symbol* index = forLoop->indexGet()->symbol();
  Symbol* iterator = forLoop->iteratorGet()->symbol();
  bool zippered = forLoop->zipperedGet();

  // find the variable marked with "chpl__iter" variable
  // in the loop header.
  Symbol* chpl_iter = NULL;
  {
    Expr* e = forLoop;
    while (e) {
      if (DefExpr* d = toDefExpr(e)) {
        Symbol* var = d->sym;
        if (var->hasFlag(FLAG_CHPL__ITER)) {
          chpl_iter = var;
          break;
        }
      }
      e = e->prev;
    }
  }

  bool forall = (chpl_iter != NULL);

  isForall = forall;
  detailsVector.clear();

  if (!forall) {
    // Handle for loops first
    if (! zippered) {
      // simple case of serial, non-zippered iteration
      // i.e. a non-zippered for loop
      // Find the PRIM_MOVE setting iterator
      SymExpr* def = iterator->getSingleDef();
      CallExpr* move = toCallExpr(def->parentExpr);
      INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
      CallExpr* getIteratorCall = toCallExpr(move->get(2));
      INT_ASSERT(getIteratorCall);

      // Collapse compiler-introduced copies of references
      // to variables marked "index var"
      // This handles chpl__saIdxCopy
      index = collapseIndexVarReferences(index);

      // The thing being iterated over is the argument to getIterator
      Expr* iterable = getIteratorCall->get(1);
      IteratorDetails details;
      details.iterable = iterable;
      details.index = index;
      details.iteratorClass = iterator->type;
      details.iterator = getTheIteratorFn(details.iteratorClass);

      IteratorDetails emptyDetails;
      leaderDetails = emptyDetails;
      followerForLoop = NULL;
      detailsVector.push_back(details);
      return;
    } else {
      // serial, zippered iteration
      // i.e. a zippered for loop
      // Find the call to _build_tuple
      SymExpr* def = iterator->getSingleDef();
      CallExpr* move = toCallExpr(def->parentExpr);
      INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
      CallExpr* buildTupleCall = toCallExpr(move->get(2));
      INT_ASSERT(buildTupleCall);
      // build up the detailsVector
      for_actuals(actual, buildTupleCall) {
        SymExpr* actualSe = toSymExpr(actual);
        INT_ASSERT(actualSe); // otherwise not normalized
        // Find the single definition of actualSe->var to find
        // the call to _getIterator.
        Symbol* tmpStoringGetIterator = actualSe->symbol();
        SymExpr* def = tmpStoringGetIterator->getSingleDef();
        CallExpr* move = toCallExpr(def->parentExpr);
        CallExpr* getIterator = toCallExpr(move->get(2));
        // The argument to _getIterator is the iterable
        Expr* iterable = getIterator->get(1);
        IteratorDetails details;
        details.iterable = iterable;
        details.index = NULL; // set below
        details.iteratorClass = getIterator->typeInfo();
        details.iterator = getTheIteratorFn(details.iteratorClass);

        detailsVector.push_back(details);
      }

      // Now, in a zippered-for, the index is actually
      // a tuple (of references typically). We need to find the
      // un-packed elements.

      findZipperedIndexVariables(index, detailsVector);

      IteratorDetails emptyDetails;
      leaderDetails = emptyDetails;
      followerForLoop = NULL;
      return;
    }
  } else {
    // Handle forall loops

    // It could be:
    // standalone iterator that is not zippered
    // leader-follower loop that is not zippered
    // leader-follower loop that is zippered

    // TODO -- use a flag here?
    bool leaderFollower = (0 == strcmp(index->name, "chpl__leadIdx"));

    if (!leaderFollower) {
      // parallel, non-zippered standalone
      // ie forall using standalone iterator
      // Find the PRIM_MOVE setting iterator
      SymExpr* def = chpl_iter->getSingleDef();
      CallExpr* move = toCallExpr(def->parentExpr);
      INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));

      Expr* iterable = move->get(2);

      // If the preceeding statement is a PRIM_MOVE setting
      // moveAddr, use its argument as the iterable.
      if (SymExpr* iterableSe = toSymExpr(iterable)) {
        CallExpr* prev = toCallExpr(move->prev);
        if (prev && prev->isPrimitive(PRIM_MOVE))
          if (SymExpr* lhs = toSymExpr(move->get(1)))
            if (lhs->symbol() == iterableSe->symbol())
              if (CallExpr* addrOf = toCallExpr(prev->get(2)))
                if (addrOf->isPrimitive(PRIM_ADDR_OF) ||
                    addrOf->isPrimitive(PRIM_SET_REFERENCE))
                iterable = addrOf->get(1);
      }

      INT_ASSERT(iterable);

      // Collapse compiler-introduced copies of references
      // to variables marked "index var"
      // This handles chpl__saIdxCopy
      index = collapseIndexVarReferences(index);

      IteratorDetails details;
      details.iterable = iterable;
      details.index = index;
      details.iteratorClass = iterator->type;
      details.iterator = getTheIteratorFn(details.iteratorClass);

      IteratorDetails emptyDetails;
      leaderDetails = emptyDetails;
      followerForLoop = NULL;
      detailsVector.push_back(details);
      return;
    } else {

      // Leader-follower iteration

      // Find the iterables

      SymExpr* def = chpl_iter->getSingleDef();
      CallExpr* move = toCallExpr(def->parentExpr);
      INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));

      if (!zippered) {
        Expr* iterable = move->get(2);
        INT_ASSERT(iterable);
        // Comes up in non-zippered leader-follower iteration
        IteratorDetails details;
        details.iterable = iterable;
        // Other details set below.
        detailsVector.push_back(details);
      } else {
        CallExpr* buildTupleCall = toCallExpr(move->get(2));
        INT_ASSERT(buildTupleCall);
        // build up the detailsVector
        for_actuals(actual, buildTupleCall) {
          SymExpr* actualSe = toSymExpr(actual);
          INT_ASSERT(actualSe); // otherwise not normalized
          // actualSe is the iterable in this case
          IteratorDetails details;
          details.iterable = actualSe;
          // Other details set below.
          detailsVector.push_back(details);
        }
      }

      leaderDetails.iterable = detailsVector[0].iterable;
      leaderDetails.index = index;
      leaderDetails.iteratorClass = iterator->typeInfo();
      leaderDetails.iterator = getTheIteratorFn(leaderDetails.iteratorClass);

      ForLoop* followerFor = findFollowerForLoop(forLoop);
      INT_ASSERT(followerFor);
      followerForLoop = followerFor;

      // Set the detailsVector based uppon the follower loop
      Symbol* followerIndex = followerFor->indexGet()->symbol();
      Symbol* followerIterator = followerFor->iteratorGet()->symbol();

      if (!zippered) {
        followerIndex = collapseIndexVarReferences(followerIndex);
        detailsVector[0].index = followerIndex;
        detailsVector[0].iteratorClass = followerIterator->typeInfo();
        detailsVector[0].iterator =
          getTheIteratorFn(detailsVector[0].iteratorClass);
      } else {
	// Set detailsVector[i].index
        findZipperedIndexVariables(followerIndex, detailsVector);

        // Figure out iterator class of zippered followers from
        // the tuple type.
        AggregateType* tupleType = toAggregateType(followerIterator->type);
        int i = 0;
        for_fields(field, tupleType) {
          detailsVector[i].iteratorClass = field->type;
          detailsVector[i].iterator =
            getTheIteratorFn(detailsVector[i].iteratorClass);

          i++;
        }
      }
      return;
    }
  }
}


//
// This function adjusts calls to functions that have both
// a ref and non-ref version. These calls are represented with
// a ContextCallExpr. Whether to use the ref or value/const-ref
// version is determined based on how the result of the call is used.
//
// This pass studies all of these call-sites and determines if the
// "by-reference" version is "necessary".  If it is not, then the
// compiler attempts to switch in the "by-value" version. This requires
// the insertion of a temporary value near the call site.
//
// The compiler must also determine whether the "by value" implementation
// will have inserted an autoCopy.   If so then the compiler attaches
// the AUTO_COPY/AUTO_DESTROY flags as necessary to enable the
// callDestructors pass to operate correctly.
//
// This pass also concerns itself with lowering ArgSymbols with
// INTENT_REF_MAYBE_CONST to INTENT_REF or INTENT_CONST_REF, depending
// on whether the formal was "set" in the body of the function.
//
void cullOverReferences() {

  int breakOnId1 = 0; // These are for debugging
  int breakOnId2 = 0;

  /* A note about the structure of this pass:

     While it's interprocedural, it shouldn't present a problem for
     separate compilation. The reason is that functions compiled into
     a library should know whether or not they are setting their arguments.
     Combining that fact with the plan to make the compiler produce a
     "header file" in addition to "implementation files" as part of a library,
     there is no problem.

     (However, any interface describing a callable function / call-back
      function will need to have some other default (possibly blank
      argument intent leads to compilation error).

     Second, this pass works in a uses-to-defs manner and is an
     interprocedural analysis. However, that does not mean that it can't
     fit into a Pass Manager concept. In particular, it could be
     what LLVM calls a CallGraphSCCPass.
  */


  // Determine const-ness of two types of reference symbols:
  //  1) the symbol storing the ref/const ref result of a ref-pair invocation
  //     (aka the result of a ContextCallExpr).
  //  2) ArgSymbols with intent INTENT_REF_MAYBE_CONST

  // Also, adjust ContextCallExprs including the
  // case when the result is not saved.

  // First, collect Symbols that we need to determine the const-ness of
  std::vector<Symbol*> collectedSymbols;

  std::set<Symbol*> unknownConstSyms;
  //std::set<Symbol*> ignoredDefs;

  std::map<BaseAST*, BaseAST*> reasonNotConst;

  revisitGraph_t revisitGraph;

  forv_Vec(ArgSymbol, arg, gArgSymbols) {

    if (arg->id == breakOnId1 || arg->id == breakOnId2)
      gdbShouldBreakHere();

    if (arg->intent == INTENT_REF_MAYBE_CONST) {
      collectedSymbols.push_back(arg);
    }
  }

  forv_Vec(ContextCallExpr, cc, gContextCallExprs) {
    CallExpr* move = toCallExpr(cc->parentExpr);
    INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));

    SymExpr* lhs = toSymExpr(move->get(1));

    INT_ASSERT(lhs);

    // Lower certain degenerate cases
    if (firstPassLowerContextCall(cc)) {
      // ContextCall removed, no further action required on it
    } else {
      // ContextCall needs more analysis, do this in 2nd step
      collectedSymbols.push_back(lhs->symbol());
    }
  }

  // Next, determine the const-ness of the collectedSymbols.
  // Do this by looking at SymExprs referring to that symbol.

  // There are three cases for a given SymExpr:
  //  1) it's a write (ie def)
  //  2) it's a read (ie use)
  //  3) we can't tell yet because it depends on another Symbol's constness.
  //     This could be due to recursion with INTENT_REF_MAYBE_CONST ArgSymbols
  //     or due to nested calls with ContextCallExpr.

  // Note: code in this loop can append to collectedSymbols.
  for(size_t i = 0; i < collectedSymbols.size(); i++) {

    Symbol* sym = collectedSymbols[i];
    //printf("Working on symbol %i\n", sym->id);

    if (sym->id == breakOnId1 || sym->id == breakOnId2)
      gdbShouldBreakHere();

    // If we already determined that a symbol is const, no need to
    // do additional work here.
    if (sym->qualType().isConst())
      continue;

    bool setter = false;
    bool revisit = false;

    for_SymbolSymExprs(se, sym) {
      // Check several cases that might require other other
      // information to resolve. These can be added to the revisitGraph.
      if (CallExpr* call = toCallExpr(se->parentExpr)) {
        // check for the case that sym is passed a ContextCall
        // and the determination depends on which branch is chosen.
        if (ContextCallExpr* cc = toContextCallExpr(call->parentExpr)) {
          if (contextCallItDepends(sym, cc/*, ignoredDefs*/)) {
            // since lhs->symbol() is the result of a move from
            // a ContextCallExpr, it will already be in the list
            // of collectedSymbols.
            revisit = true;
            CallExpr* move = toCallExpr(cc->parentExpr);
            SymExpr* lhs = toSymExpr(move->get(1));
            // Make a note that determining how lhs->symbol()
            // is used (is it const or not?) will allow us to
            // resolve this ContextCallExpr.
            revisitGraph[lhs->symbol()].push_back(sym);
            continue; // move on to the next iteration
          }
        }

        // Check if sym is iterated over. In that case, what's the
        // index variable?
        {
          // Find enclosing PRIM_MOVE
          CallExpr* move = toCallExpr(se->parentExpr->getStmtExpr());
          if (!move->isPrimitive(PRIM_MOVE))
            move = NULL;

          if (move != NULL) {
            // Now, LHS of PRIM_MOVE is iterator variable
            SymExpr* lhs = toSymExpr(move->get(1));
            Symbol* iterator = lhs->symbol();

            // marked with chpl__iter or with type iterator class?
            if (iterator->hasFlag(FLAG_CHPL__ITER) ||
                iterator->type->symbol->hasFlag(FLAG_ITERATOR_CLASS)) {

//              printf("considering symbol %i\n", sym->id);
//              printf("considering iterator %i\n", iterator->id);

              // Scroll through exprs until we find ForLoop
              Expr* e = move;
              while (e && !isForLoop(e)) {
                e = e->next;
              }

              if (ForLoop* forLoop = toForLoop(e)) {
                // mIndex seems to correspond to indexOfInterest
                // in zippered for cases, this is a tuple of refs
                // leader-follower case, this is chpl__leadIdx
                //   ... in leader-follower case, there is a nested
                //   ForLoop with mIndex as a tuple of refs
//                printf("mIndex %i\n", forLoop->indexGet()->symbol()->id);
//                printf("mIterator %i\n", forLoop->iteratorGet()->symbol()->id);

                // Gather the loop details to understand the
                // correspondence between what was iterated over
                // and the index variables.

                bool isForall = false;
                IteratorDetails leaderDetails;
                ForLoop* followerForLoop = NULL;
                std::vector<IteratorDetails> detailsVector;

                gatherLoopDetails(forLoop, isForall, leaderDetails,
                                  followerForLoop, detailsVector);

                // Now use the correspondence

                bool handled = false;

                for (size_t i = 0; i < detailsVector.size(); i++) {
                  bool iteratorYieldsConstWhenConstThis = false;

                  Expr* iterable = detailsVector[i].iterable;
                  Symbol* index = detailsVector[i].index;
                  FnSymbol* iteratorFn  = detailsVector[i].iterator;
                  SymExpr* iterableSe = toSymExpr(iterable);
//                  printf("iterator fn is %i\n", iteratorFn->id);
//                  printf("iterable is %i\n", iterable->id);
//                  printf("index is %i\n", index->id);
//                  printf("sym is %i\n", sym->id);
//                  printf("in fn %s\n\n", forLoop->parentSymbol->name);

                  // In the future this could be based upon ref-pair
                  // iterators.
                  // For now, the compiler makes this adjustment for
                  // any iterator methods on array implementation classes.
                  if (iteratorFn->isMethod() &&
                      isArrayClass(iteratorFn->getFormal(1)->type))
                      iteratorYieldsConstWhenConstThis = true;

                  // Note, if we wanted to use the return intent
                  // of the iterator, it is overwritten in protoIteratorClass.

                  // This flag should be set for array iteration
                  if (iterableSe &&
                      iterableSe->symbol() == sym &&
                      iteratorYieldsConstWhenConstThis &&
                      index->isRef()) {
                    // Now the const-ness of the array depends
                    // on whether or not the yielded value is set

//                    printf("ADDING %i -> %i\n\n", index->id, sym->id);
                    collectedSymbols.push_back(index);
                    revisit = true;
                    revisitGraph[index].push_back(sym);
                    handled = true;
                    break;
                  }
                }

                if (handled)
                  continue; // continue outer loop
              }
            }
          }
        }

        if (FnSymbol* calledFn = call->isResolved()) {
          // Check for the case that sym is passed to an
          // array formal with blank intent. In that case,
          // it depends on the determination of the called function.
          ArgSymbol* formal = actual_to_formal(se);
          if (formal->intent == INTENT_REF_MAYBE_CONST) {
            // since it has INTENT_REF_MAYBE_CONST, it will
            // already be in the list of collectedSymbols.
            revisit = true;
            // Make a note that determining how formal
            // is used (const or not?) will allow us to resolve
            // this Symbol's const-ness.
            revisitGraph[formal].push_back(sym);
            continue; // move on to the next iteration
          }

          // Check for the case that sym is the _this
          // actual for a function marked with the flag
          // FLAG_REF_TO_CONST_WHEN_CONST_THIS
          // (which is used for field accessors among other things).
          // In that event, it depends on how the returned
          // value is used.
          if (calledFn->hasFlag(FLAG_REF_TO_CONST_WHEN_CONST_THIS) &&
              formal->hasFlag(FLAG_ARG_THIS)) {
            CallExpr* move = toCallExpr(call->parentExpr);
            if (move && move->isPrimitive(PRIM_MOVE)) {
              SymExpr* lhs = toSymExpr(move->get(1));
              Symbol* lhsSymbol = lhs->symbol();
              if (lhsSymbol->isRef() && lhsSymbol != sym) {
                collectedSymbols.push_back(lhsSymbol);
                revisit = true;
                revisitGraph[lhsSymbol].push_back(sym);
                continue; // move on to the next iteration
              }
            }
          }

        }

        // Check for the case that sym is moved to a compiler-introduced
        // variable, possibly with PRIM_MOVE tmp, PRIM_ADDR_OF sym
        if (call->isPrimitive(PRIM_ADDR_OF) ||
            call->isPrimitive(PRIM_SET_REFERENCE) ||
            call->isPrimitive(PRIM_GET_MEMBER) ||
            call->isPrimitive(PRIM_GET_SVEC_MEMBER))
            call = toCallExpr(call->parentExpr);

        if (call->isPrimitive(PRIM_MOVE)) {
          SymExpr* lhs = toSymExpr(call->get(1));
          Symbol* lhsSymbol = lhs->symbol();
          if (lhsSymbol != sym && lhsSymbol->isRef()) {
            collectedSymbols.push_back(lhsSymbol);
            revisit = true;
            revisitGraph[lhsSymbol].push_back(sym);
            continue; // move on to the next iteration
          }
        }
      }

      // Determine if se represents a "setting" or a "getting" mention of sym
      if (!setter && symExprIsSet(se)) {
        // Workaround for inaccurate tuple analysis: exclude the
        // _build_tuple call with a LHS that is setting a chpl__iter variable.
        if (! inBuildTupleForChplIter(se)) {
          setter = true;
          reasonNotConst[sym] = se;
          if (CallExpr* call = toCallExpr(se->parentExpr)) {
            if (call->isResolved()) {
              ArgSymbol* formal = actual_to_formal(se);
              reasonNotConst[se] = formal;
            }
          }
        }
      }
    }

    if (revisit) {
      if (setter) {
        // We decided to revisit this Symbol, but separate uses
        // determined it to be const. So don't revisit it.
        revisit = false;
      } else {
        // We might still decide to use setter.
        unknownConstSyms.insert(sym);
      }
    }

    if (!revisit) {
      if (setter) {
        // it's not CONST & it shouldn't be CONST
        markNotConst(sym);
      } else {
        // it is const, mark it so
        markConst(sym);
      }
    }
  }

    /* TODO -- do we need to move this logic here,
       so that it doesn't apply to arrays?

      else if (call->isPrimitive(PRIM_RETURN) ||
                 call->isPrimitive(PRIM_YIELD)) {
        FnSymbol* inFn = toFnSymbol(call->parentSymbol);
        // It is not necessary to use the 'ref' version
        // if the function result is returned by 'const ref'.
        if (inFn->retTag == RET_CONST_REF) return false;
        // MPF: it seems to cause problems to return false
        // here when inFn->retTag is RET_VALUE.
        // TODO: can we add
        //if (inFn->retTag == RET_VALUE) return false;
        return true;

      } else if (call->isPrimitive(PRIM_WIDE_GET_LOCALE) ||
                 call->isPrimitive(PRIM_WIDE_GET_NODE)) {
        // If we are extracting a field from the wide pointer,
        // we need to keep it as a pointer.
        // Dereferencing would be premature.

        // MPF: This seems like a workaround to me
        // How can this be right for a ref-pair returning const-ref/ref?
        return true;
      }
     */

  // Handle the graph of revisits
  // Note this could be a cyclic graph when there are recursive
  // functions.

  // This algorithm could be faster, O(n) instead of (n log n) if either:
  //   * we had QUAL_REF_MAYBE_CONST, or
  //   * it used Tarjan's SCC algorithm (which is conveniently available in LLVM
  //     (even for graph node/edge types of our choosing).

  {
    /*
    bool updated;

    do {
      updated = false;


      for (revisitGraph_t::iterator it = revisitGraph.begin();
           it != revisitGraph.end();
           ++it) {

        Symbol* sym = it->first;

        if (sym->id == breakOnId1 || sym->id == breakOnId2)
          gdbShouldBreakHere();

        std::vector<Symbol*> & value = it->second;

        if (unknownConstSyms.count(sym) == 0) {
          // We know if sym should use setter or not.
          // If it's a setter, propagate in graph
          if (!sym->qualType().isConst()) {
            for_vector(Symbol, setSym, value) {
              if (unknownConstSyms.count(setSym) != 0) {
                INT_ASSERT(!sym->qualType().isConst());
                unknownConstSyms.erase(setSym);
                updated = true;
              }
            }
          }
        }
      }
    } while (updated);
    */

    // First, propagate non-const-ness through the graph
    for (revisitGraph_t::iterator it = revisitGraph.begin();
         it != revisitGraph.end();
         ++it) {

      Symbol* sym = it->first;

      if (sym->id == breakOnId1 || sym->id == breakOnId2)
        gdbShouldBreakHere();

      if (unknownConstSyms.count(sym) == 0) {
        if (!sym->qualType().isConst()) {
          // If sym has known const-ness, and it's a setter,
          // propagate that information in the graph.
          transitivelyMarkNotConst(sym, revisitGraph, unknownConstSyms,
              reasonNotConst);
        }
      }
    }

    // Next, mark any remaining symbols with unknown const-ness
    // as const, since they are never set
    // (this accounts for cycles possibly due to recursive functions
    //  with blank-intent array formals)
    for (std::set<Symbol*>::iterator it = unknownConstSyms.begin();
         it != unknownConstSyms.end();
         ++it) {

      Symbol* sym = *it;

      // Anything we didn't remove from unknownConstSyms must be
      // in a cycle of const-ness

      if (sym->id == breakOnId1 || sym->id == breakOnId2)
        gdbShouldBreakHere();

      markConst(sym);
    }
  }

  // Now, lower ContextCalls and remove INTENT_REF_MAYBE_CONST.
  forv_Vec(ContextCallExpr, cc, gContextCallExprs) {
    // Some ContextCallExprs have already been removed above
    if (cc->parentExpr == NULL)
      continue;

    CallExpr* move = toCallExpr(cc->parentExpr);

    bool useSetter = false;

    if (move) {
      SymExpr* lhs = toSymExpr(move->get(1));
      Qualifier qual = lhs->symbol()->qualType().getQual();
      // Expecting only REF or CONST_REF at this point.
      INT_ASSERT(qual == QUAL_REF || qual == QUAL_CONST_REF);
      useSetter = (qual == QUAL_REF);
    }

    lowerContextCall(cc, useSetter);
  }

  // Now, lower ArgSymbol argument intent.
  /*for_vector(Symbol, sym, collectedSymbols) {
    if (ArgSymbol* arg = toArgSymbol(sym)) {
      INT_ASSERT(arg->intent == INTENT_REF_MAYBE_CONST);
      bool isConst = arg->qualType().isConst();
      if (isConst) {
        arg->intent = INTENT_CONST_REF;
      } else {
        arg->intent = INTENT_REF;
      }
    }
  }*/

  lateConstCheck(reasonNotConst);
}


// Handle certain degenerate cases, such as when a
// ContextCallExpr is not in a PRIM_MOVE.
static
bool firstPassLowerContextCall(ContextCallExpr* cc)
{
    // Make sure that the context call only has 2 options.
    INT_ASSERT(cc->options.length == 2);

    CallExpr* refCall = cc->getRefCall();
    CallExpr* valueCall = cc->getRValueCall();

    FnSymbol* refFn = refCall->isResolved();
    INT_ASSERT(refFn);
    FnSymbol* valueFn = valueCall->isResolved();
    INT_ASSERT(valueFn);

    CallExpr* move = NULL; // set if the call is in a PRIM_MOVE
    SymExpr* lhs = NULL; // lhs if call is in a PRIM_MOVE

    // Decide whether to use the value call or the ref call.
    // Always leave the ref call for iterators.
    // (It would be an improvement to choose the appropriate one
    //  based upon how the iterator is used, but such a feature
    //  would require specific support for iterators since yielding
    //  is not the same as returning.)
    move = toCallExpr(cc->parentExpr);
    if (refFn->isIterator()) {
      lowerContextCall(cc, true);
      return true;
    } else if (move) {
      INT_ASSERT(move->isPrimitive(PRIM_MOVE));
      lhs = toSymExpr(move->get(1));
      INT_ASSERT(lhs);
      // This case is complex and handled later.
      return false;
    } else {
      // e.g. array access in own statement like this:
      //   A(i)
      // should use 'getter'
      // MPF - note 2016-01: this code does not seem to be triggered
      // in the present compiler.
      lowerContextCall(cc, false);
      return true;
    }
}


static
void lowerContextCall(ContextCallExpr* cc, bool useSetter)
{
    // Make sure that the context call only has 2 options.
    INT_ASSERT(cc->options.length == 2);

    CallExpr* refCall = cc->getRefCall();
    CallExpr* valueCall = cc->getRValueCall();

    FnSymbol* refFn = refCall->isResolved();
    INT_ASSERT(refFn);
    FnSymbol* valueFn = valueCall->isResolved();
    INT_ASSERT(valueFn);

    bool useValueCall = !useSetter;

    CallExpr* move = NULL; // set if the call is in a PRIM_MOVE
    SymExpr* lhs = NULL; // lhs if call is in a PRIM_MOVE

    // Decide whether to use the value call or the ref call.
    // Always leave the ref call for iterators.
    // (It would be an improvement to choose the appropriate one
    //  based upon how the iterator is used, but such a feature
    //  would require specific support for iterators since yielding
    //  is not the same as returning.)
    move = toCallExpr(cc->parentExpr);
    if (refFn->isIterator())
      useValueCall = false;
    else if (move) {
      lhs = toSymExpr(move->get(1));
      // useValueCall set from useSetter argument to this function
    } else {
      // e.g. array access in own statement like this:
      //   A(i)
      // should use 'getter'
      // MPF - note 2016-01: this code does not seem to be triggered
      // in the present compiler.
      useValueCall = true;
    }

    valueCall->remove();
    refCall->remove();

    if (useValueCall) {
      // Replace the ContextCallExpr with the value call
      cc->replace(valueCall);

      // Adjust the AST around the value call to include
      // a temporary to receive the value.

      // Adjust code to use value return version.
      // The other option is that retTag is RET_CONST_REF,
      // in which case no further adjustment is necessary.
      if (move && valueFn->retTag == RET_VALUE) {
        SET_LINENO(move);
        // Generate a value temp to receive the value
        VarSymbol* tmp  = newTemp(valueFn->retType);
        move->insertBefore(new DefExpr(tmp));

        if (requiresImplicitDestroy(valueCall)) {
          if (isUserDefinedRecord(valueFn->retType) == false) {
            tmp->addFlag(FLAG_INSERT_AUTO_COPY);
            tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
          } else {
            tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
          }
        }

        if (lhs && lhs->symbol()->isUsed()) {
          // If the LHS was used, set it to the address of the
          // new temporary (which is the function return value)

          FnSymbol* moveInFn = toFnSymbol(move->parentSymbol);
          INT_ASSERT(moveInFn);
          Symbol* retSymbol = moveInFn->getReturnSymbol();
          // Check: are we adding a return of a local variable ?
          for_SymbolUses(use, lhs->symbol()) {
            if (CallExpr* useCall = toCallExpr(use->parentExpr))
              if (useCall->isPrimitive(PRIM_MOVE))
                if (SymExpr* useCallLHS = toSymExpr(useCall->get(1)))
                  if (useCallLHS->symbol() == retSymbol) {
                    USR_FATAL_CONT(move, "illegal expression to return by ref");
                    USR_PRINT(valueCall->isResolved(), "called function returns a value not a reference");
                  }
          }

          move->insertAfter(new CallExpr(PRIM_MOVE,
                                         lhs->symbol(),
                                         new CallExpr(PRIM_ADDR_OF, tmp)));
        } else {
          // If the LHS was not used,
          // remove the old definition point since we have
          // provided a new one above.
          lhs->symbol()->defPoint->remove();
        }

        // Replace the LHS with our new temporary
        lhs->setSymbol(tmp);
      }

    } else {
      // Replace the ContextCallExpr with the ref call
      cc->replace(refCall);
    }
}

static void printReason(BaseAST* reason)
{
  Expr* expr = toExpr(reason);
  if (Symbol* s = toSymbol(reason))
    expr = s->defPoint;
  ModuleSymbol* mod = expr->getModule();

  if (developer || mod->modTag == MOD_USER) {
    if (isArgSymbol(reason) || isFnSymbol(reason))
      USR_PRINT(reason, "to ref formal here");
    else
      USR_PRINT(reason, "passed as ref here");

    // useful for debugging this pass
    // USR_PRINT(reason, "id %i", reason->id);
  }

}


/* Since const-checking can depend on ref-pair determination
   or upon the determination of whether an array formal with
   blank intent is passed by ref or by value, do final const checking here.

   TODO: decide if we also need const checking in functionResolution.cpp.
 */
static void lateConstCheck(std::map<BaseAST*, BaseAST*> & reasonNotConst)
{
  return; // TODO disabled for now

  forv_Vec(CallExpr, call, gCallExprs) {

    // Ignore calls removed earlier by this pass.
    if (call->parentExpr == NULL)
      continue;


    if (FnSymbol* calledFn = call->isResolved()) {
      char cn1 = calledFn->name[0];
      const char* calleeParens = (isalpha(cn1) || cn1 == '_') ? "()" : "";
      // resolved calls
      for_formals_actuals(formal, actual, call) {
        bool error = false;
        if (actual->qualType().isConst() && ! formal->qualType().isConst()) {

          // But... it's OK if we're calling a function marked
          // FLAG_REF_TO_CONST_WHEN_CONST_THIS and the result is
          // marked const. In that case, we pretend that the `this`
          // argument would be marked const too.
          if (calledFn->hasFlag(FLAG_REF_TO_CONST_WHEN_CONST_THIS) &&
              formal->hasFlag(FLAG_ARG_THIS)) {
            CallExpr* move = toCallExpr(call->parentExpr);
            if (move && move->isPrimitive(PRIM_MOVE)) {
              SymExpr* lhs = toSymExpr(move->get(1));
              Symbol* lhsSym = lhs->symbol();
              if (lhsSym->qualType().isConst())
                ; // OK, lhsSym has const Qualifier
              else if (lhsSym->hasFlag(FLAG_REF_TO_CONST))
                ; // OK, lhsSym is marked with FLAG_REF_TO_CONST
              else
                error = true; // l-value error
            }
          } else {
            error = true;
          }
        }

        // For now, ignore errors with tuple construction/build_tuple
        if (calledFn->hasFlag(FLAG_BUILD_TUPLE) ||
            (calledFn->hasFlag(FLAG_CONSTRUCTOR) &&
             calledFn->retType->symbol->hasFlag(FLAG_TUPLE)))
          error = false;

        // For now, ignore errors with `this` formal
        if (formal->hasFlag(FLAG_ARG_THIS))
          error = false;

        if (error) {
          //gdbShouldBreakHere(); // Debug

          USR_FATAL_CONT(actual,
                        "const actual is passed to %s formal '%s'"
                        " of %s%s",
                        formal->intentDescrString(),
                        formal->name,
                        calledFn->name, calleeParens);

          printReason(formal);

          BaseAST* reason = reasonNotConst[formal];
          BaseAST* lastReason = formal;
          while (reason) {

            BaseAST* printCause = reason;

            // If the last reason and the this reason are both Symbols,
            // try to figure out what links them by looking at uses
            // of lastReason.
            if (isSymbol(lastReason) && isArgSymbol(reason)) {
              Symbol* lastSym = toSymbol(lastReason);
              ArgSymbol* curFormal = toArgSymbol(reason);
              for_SymbolSymExprs(se, lastSym) {
                if (CallExpr* parentCall = toCallExpr(se->parentExpr))
                  if (parentCall->isResolved())
                    if (curFormal == actual_to_formal(se)) {
                      printReason(se);
                      break;
                    }
              }
            }

            // Go from LHS VarSymbol to called fn
            // for better reporting for the reason of context-call
            if (VarSymbol* v = toVarSymbol(reason)) {
              for_SymbolDefs(def, v) {
                if (CallExpr* parentCall = toCallExpr(def->parentExpr))
                  if (parentCall->isPrimitive(PRIM_MOVE))
                    if (CallExpr* rhsCall = toCallExpr(parentCall->get(2)))
                      if (FnSymbol* rhsCalledFn = rhsCall->isResolved()) {
                        printReason(def);
                        printReason(rhsCalledFn);
                        printCause = NULL;
                        break;
                      }
              }
            }

            if (printCause) {
              // Print out an annotation line
              printReason(printCause);
            }

            if (reasonNotConst.count(reason) != 0) {
              lastReason = reason;
              reason = reasonNotConst[reason];
            } else
              break;
          }
        }
      }
    }

    // For now, don't check primitives. Compiler can be loose
    // with const-ness on its own internal temporaries.
    /*else {
      // Primitives
      Expr* dest = NULL;
      Expr* src = NULL;
      bool isSetting = getSettingPrimitiveDstSrc(call, &dest, &src);
      if (isSetting) {
        if (SymExpr* destSe = toSymExpr(dest)) {
          if (destSe->symbol()->qualType().isConst()) {
	    USR_FATAL_CONT(call, "setting const variable");
          }
        }
      }
    }*/
  }
}
