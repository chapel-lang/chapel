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

static bool refNecessary(SymExpr* se);
static void lowerContextCall(ContextCallExpr* cc, bool useSetter);
static bool firstPassLowerContextCall(ContextCallExpr* cc);

static bool
refNecessaryForDef(SymExpr* def) {
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
refNecessaryForUse(SymExpr* use) {
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

      } else if (call->isPrimitive(PRIM_MOVE)) {
        // If it's move lhs rhs, and se is rhs
        // ref is necessary for rhs if ref is necessary for lhs
        // Does this only come up for compiler-introduced moves?
        if (refNecessary(toSymExpr(call->get(1))))
          return true;

      } else if (call->isPrimitive(PRIM_GET_MEMBER) ||
                 call->isPrimitive(PRIM_GET_SVEC_MEMBER)) {
        // Getting a reference to a record/tuple member
        // the se would be the record/tuple variable
        CallExpr* move = toCallExpr(call->parentExpr);

        INT_ASSERT(move);
        INT_ASSERT(move->isPrimitive(PRIM_MOVE));

        if (refNecessary(toSymExpr(move->get(1))))
          return true;

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
bool refNecessary(SymExpr* se)
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
    ret |= refNecessaryForDef(se);
  }
  if (defOrUse & 2) { // use
    ret |= refNecessaryForUse(se);
  }

  return ret;
}

static
bool refNecessaryForSymbolInCall(Symbol* sym, CallExpr* call, std::vector<SymExpr*> & collectSettingSymExprs)
{
  bool ret = false;
  for_alist(expr, call->argList) {
    if (SymExpr* se = toSymExpr(expr)) {
      if (se->symbol() == sym) {
        if (refNecessary(se)) {
          ret = true;
          collectSettingSymExprs.push_back(se);
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

  std::vector<SymExpr*> settingSymExprs;

  if (refNecessaryForSymbolInCall(sym, refCall, settingSymExprs) !=
      refNecessaryForSymbolInCall(sym, valueCall, settingSymExprs) ) {
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
                              revisitUnknowns_t & unknownConstSyms)
{
  std::vector<Symbol*> & dependentSymbols = graph[sym];

  for_vector(Symbol, otherSym, dependentSymbols) {
    if (unknownConstSyms.count(otherSym) != 0) {
      // otherSym still has unknown const-ness
      // mark it as not-const
      markNotConst(otherSym);
      unknownConstSyms.erase(otherSym);
      transitivelyMarkNotConst(otherSym, graph, unknownConstSyms);
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

  for_vector(Symbol, sym, collectedSymbols) {

    if (sym->id == breakOnId1 || sym->id == breakOnId2)
      gdbShouldBreakHere();

    bool setter = false;
    bool revisit = false;

    for_SymbolSymExprs(se, sym) {
      // check for the case that sym is passed a ContextCall
      // and the determination depends on which branch is chosen.
      if (CallExpr* call = toCallExpr(se->parentExpr)) {
        if (ContextCallExpr* cc = toContextCallExpr(call->parentExpr)) {
          if (contextCallItDepends(sym, cc/*, ignoredDefs*/)) {
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
      }

      // Check for the case that sym is passed to an
      // array formal with blank intent. In that case,
      // it depends on the determination of the called funciton.
      if (CallExpr* call = toCallExpr(se->parentExpr)) {
        if (call->isResolved()) {
          ArgSymbol* formal = actual_to_formal(se);
          if (formal->intent == INTENT_REF_MAYBE_CONST) {
            revisit = true;
            // Make a note that determining how formal
            // is used (const or not?) will allow us to resolve
            // this Symbol's const-ness.
            revisitGraph[formal].push_back(sym);
            continue; // move on to the next iteration
          }
        }
      }

      // Determine if se represents a "setting" or a "getting" mention of sym
      setter |= refNecessary(se);
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
          transitivelyMarkNotConst(sym, revisitGraph, unknownConstSyms);
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
    CallExpr* move = toCallExpr(cc->parentExpr);
    SymExpr* lhs = toSymExpr(move->get(1));
    Qualifier qual = lhs->symbol()->qualType().getQual();
    // Expecting only REF or CONST_REF at this point.
    INT_ASSERT(qual == QUAL_REF || qual == QUAL_CONST_REF);
    bool useSetter = (qual == QUAL_REF);
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
                    USR_PRINT(refCall, "called function returns a value not a reference");
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
