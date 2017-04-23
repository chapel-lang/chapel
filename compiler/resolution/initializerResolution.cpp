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

#include "initializerResolution.h"

#include "astutil.h"
#include "caches.h"
#include "callInfo.h"
#include "expandVarArgs.h"
#include "expr.h"
#include "initializerRules.h"
#include "passes.h"
#include "resolution.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"
#include "view.h"
#include "visibleCandidates.h"
#include "visibleFunctions.h"

static
void resolveInitializer(CallExpr* call);

static
void resolveInitCall(CallExpr* call);

static
void resolveMatch(FnSymbol* fn);

/* Stolen from function resolution */
static void
filterInitCandidate(Vec<ResolutionCandidate*>& candidates,
                    ResolutionCandidate* currCandidate,
                    CallInfo& info);

static void
filterInitCandidate(Vec<ResolutionCandidate*>& candidates,
                    FnSymbol* fn,
                    CallInfo& info);

static FnSymbol*
instantiateInitSig(FnSymbol* fn, SymbolMap& subs, CallExpr* call);

static bool isRefWrapperForNonGenericRecord(AggregateType* at);

// Rewrite of instantiateSignature, for initializers.  Removed some bits,
// modified others.
/** Instantiate enough of the function for it to make it through the candidate
 *  filtering and disambiguation process.
 *
 * \param fn   Generic function to instantiate
 * \param subs Type substitutions to be made during instantiation
 * \param call Call that is being resolved
 */
static FnSymbol*
instantiateInitSig(FnSymbol* fn, SymbolMap& subs, CallExpr* call) {
  //
  // Handle tuples explicitly
  // (_build_tuple, tuple type constructor, tuple default constructor)
  //
  FnSymbol* tupleFn = createTupleSignature(fn, subs, call);
  if (tupleFn) return tupleFn;

  form_Map(SymbolMapElem, e, subs) {
    if (TypeSymbol* ts = toTypeSymbol(e->value)) {
      // This line is modified from instantiateSignature to allow the "this"
      // arg to remain generic until we have finished resolving the generic
      // portions of the initializer body's Phase 1.
      if (ts->type->symbol->hasFlag(FLAG_GENERIC) &&
          !e->key->hasFlag(FLAG_ARG_THIS))
        INT_FATAL(fn, "illegal instantiation with a generic type");
      TypeSymbol* nts = getNewSubType(fn, e->key, ts);
      if (ts != nts)
        e->value = nts;
    }
  }

  //
  // determine root function in the case of partial instantiation
  //
  FnSymbol* root = determineRootFunc(fn);

  //
  // determine all substitutions (past substitutions in a partial
  // instantiation plus the current substitutions) and change the
  // substitutions to refer to the root function's formal arguments
  //
  SymbolMap all_subs;
  determineAllSubs(fn, root, subs, all_subs);

  //
  // use cached instantiation if possible
  //
  if (FnSymbol* cached = checkCache(genericsCache, root, &all_subs)) {
    if (cached != (FnSymbol*)gVoid) {
      checkInfiniteWhereInstantiation(cached);
      return cached;
    } else
      return NULL;
  }

  SET_LINENO(fn);

  //
  // instantiate function
  //

  SymbolMap map;

  FnSymbol* newFn = instantiateFunction(fn, root, all_subs, call, subs, map);

  fixupTupleFunctions(fn, newFn, call);

  if (newFn->numFormals() > 1 && newFn->getFormal(1)->type == dtMethodToken) {
    newFn->getFormal(2)->type->methods.add(newFn);
  }

  newFn->tagIfGeneric();

  if (newFn->hasFlag(FLAG_GENERIC) == false &&
      evaluateWhereClause(newFn)   == false) {
    //
    // where clause evaluates to false so cache gVoid as a function
    //
    replaceCache(genericsCache, root, (FnSymbol*)gVoid, &all_subs);
    return NULL;
  }

  explainAndCheckInstantiation(newFn, fn);

  return newFn;
}

/** Candidate filtering logic specific to concrete functions.
 *
 * \param candidates    The list to add possible candidates to.
 * \param currCandidate The current candidate to consider.
 * \param info          The CallInfo object for the call site.
 */
static void
filterInitConcreteCandidate(Vec<ResolutionCandidate*>& candidates,
                            ResolutionCandidate* currCandidate,
                            CallInfo& info) {
  currCandidate->fn = expandIfVarArgs(currCandidate->fn, info);

  if (!currCandidate->fn) return;

  resolveTypedefedArgTypes(currCandidate->fn);

  if (!currCandidate->computeAlignment(info)) {
    return;
  }

  // We should reject this candidate if any of the situations handled by this
  // function are violated.
  if (checkResolveFormalsWhereClauses(currCandidate) == false)
    return;

  candidates.add(currCandidate);
}


/** Candidate filtering logic specific to generic functions.
 *
 * \param candidates    The list to add possible candidates to.
 * \param currCandidate The current candidate to consider.
 * \param info          The CallInfo object for the call site.
 */
static void
filterInitGenericCandidate(Vec<ResolutionCandidate*>& candidates,
                           ResolutionCandidate* currCandidate,
                           CallInfo& info) {
  currCandidate->fn = expandIfVarArgs(currCandidate->fn, info);

  if (!currCandidate->fn) return;

  if (!currCandidate->computeAlignment(info)) {
    return;
  }

  if (checkGenericFormals(currCandidate) == false)
    return;

  // Compute the param/type substitutions for generic arguments.
  currCandidate->computeSubstitutions(true);

  /*
   * If no substitutions were made we can't instantiate this generic, and must
   * reject it.
   */
  if (currCandidate->substitutions.n > 0) {
    /*
     * Instantiate just enough of the generic to get through the rest of the
     * filtering and disambiguation processes.
     */
    currCandidate->fn = instantiateInitSig(currCandidate->fn, currCandidate->substitutions, info.call);

    if (currCandidate->fn != NULL) {
      filterInitCandidate(candidates, currCandidate, info);
    }
  }
}


/** Tests to see if a function is a candidate for resolving a specific call.  If
 *  it is a candidate, we add it to the candidate lists.
 *
 * This version of filterInitCandidate is called by other versions of
 * filterInitCandidate, and shouldn't be called outside this family of functions.
 *
 * \param candidates    The list to add possible candidates to.
 * \param currCandidate The current candidate to consider.
 * \param info          The CallInfo object for the call site.
 */
static void
filterInitCandidate(Vec<ResolutionCandidate*>& candidates,
                    ResolutionCandidate* currCandidate,
                    CallInfo& info) {

  if (currCandidate->fn->hasFlag(FLAG_GENERIC)) {
    filterInitGenericCandidate(candidates, currCandidate, info);

  } else {
    filterInitConcreteCandidate(candidates, currCandidate, info);
  }
}


/** Tests to see if a function is a candidate for resolving a specific call.  If
 *  it is a candidate, we add it to the candidate lists.
 *
 * This version of filterInitCandidate is called by code outside the filterInitCandidate
 * family of functions.
 *
 * \param candidates    The list to add possible candidates to.
 * \param currCandidate The current candidate to consider.
 * \param info          The CallInfo object for the call site.
 */
static void
filterInitCandidate(Vec<ResolutionCandidate*>& candidates, FnSymbol* fn,
                    CallInfo& info) {
  ResolutionCandidate* currCandidate = new ResolutionCandidate(fn);
  filterInitCandidate(candidates, currCandidate, info);

  if (candidates.tail() != currCandidate) {
    // The candidate was not accepted.  Time to clean it up.
    delete currCandidate;
  }
}

static void
doGatherInitCandidates(Vec<ResolutionCandidate*>& candidates,
                       Vec<FnSymbol*>& visibleFns,
                       CallInfo& info,
                       bool compilerGenerated) {

  forv_Vec(FnSymbol, visibleFn, visibleFns) {
    // Only consider user functions or compiler-generated functions
    if (visibleFn->hasFlag(FLAG_COMPILER_GENERATED) == compilerGenerated) {

      // Some expressions might resolve to methods without parenthesis.
      // If the call is marked with methodTag, it indicates the called
      // function should be a no-parens function or a type constructor.
      // (a type constructor call without parens uses default arguments)
      if (info.call->methodTag) {
        if (visibleFn->hasEitherFlag(FLAG_NO_PARENS, FLAG_TYPE_CONSTRUCTOR)) {
          // OK
        } else {
          // Skip this candidate
          continue;
        }
      }

      if (fExplainVerbose &&
          ((explainCallLine && explainCallMatch(info.call)) ||
           info.call->id == explainCallID))
      {
        USR_PRINT(visibleFn, "Considering function: %s", toString(visibleFn));
        if( info.call->id == breakOnResolveID ) {
          gdbShouldBreakHere();
        }
      }

      filterInitCandidate(candidates, visibleFn, info);
    }
  }
}

static void
gatherInitCandidates(Vec<ResolutionCandidate*>& candidates,
                     Vec<FnSymbol*>& visibleFns,
                     CallInfo& info) {

  // Search user-defined (i.e. non-compiler-generated) functions first.
  doGatherInitCandidates(candidates, visibleFns, info, false);

  // If no results, try again with any compiler-generated candidates.
  if (candidates.n == 0) {
    doGatherInitCandidates(candidates, visibleFns, info, true);
  }
}
/* end of function resolution steal */


void modAndResolveInitCall (CallExpr* call, AggregateType* typeToNew) {
  // Convert the PRIM_NEW to a normal call
  call->primitive = NULL;
  call->baseExpr  = call->get(1)->remove();

  parent_insert_help(call, call->baseExpr);

  VarSymbol* new_temp  = newTemp("new_temp", typeToNew);
  DefExpr*   def       = new DefExpr(new_temp);

  if (isBlockStmt(call->parentExpr) == true) {
    call->insertBefore(def);

  } else {
    call->parentExpr->insertBefore(def);
  }

  // Invoking an instance method
  call->insertAtHead(new NamedExpr("this", new SymExpr(new_temp)));
  call->insertAtHead(new SymExpr(gMethodToken));

  resolveInitializer(call);

  // Because initializers determine the type they utilize based on the
  // execution of Phase 1, if the type is generic we will need to update the
  // type of the actual we are sending in for the this arg
  if (typeToNew->symbol->hasFlag(FLAG_GENERIC) == true) {
    new_temp->type = call->resolvedFunction()->_this->type;

    if (isClass(typeToNew) == true) {
      // use the allocator instead of directly calling the init method
      // Need to convert the call into the right format
      call->baseExpr->replace(new UnresolvedSymExpr("_new"));
      call->get(1)->replace(new SymExpr(new_temp->type->symbol));
      call->get(2)->remove();
      // Need to resolve the allocator
      resolveCall(call);
      resolveFns(call->resolvedFunction());

      def->remove();
    }
  }
}

static
void resolveInitializer(CallExpr* call) {
  // From resolveExpr() (removed the tryStack stuff)
  callStack.add(call);

  resolveInitCall(call);

  INT_ASSERT(call->isResolved());

  resolveMatch(call->resolvedFunction());

  callStack.pop();
}

static
void resolveInitCall(CallExpr* call) {
  // From resolveNormalCall()
  if( call->id == breakOnResolveID ) {
    printf("breaking on resolve call:\n");
    print_view(call);
    gdbShouldBreakHere();
  }

  temporaryInitializerFixup(call);

  resolveDefaultGenericType(call);

  // Make a CallInfo which doesn't care if the this argument is
  // generic, but otherwise should result in the same behavior.
  CallInfo info(call, false, true);


  Vec<FnSymbol*> visibleFns; // visible functions

  findVisibleFunctions(info, visibleFns);


  // Modified narrowing down the candidates to operate in an
  // initializer-specific manner
  Vec<ResolutionCandidate*> candidates;
  gatherInitCandidates(candidates, visibleFns, info);

  explainGatherCandidate(candidates, info, call);

  // Removed a whole bunch of stuff that resolveNormalCall did that I didn't
  // need, including ref/value pairs, some checkonly stuff, a match against
  // the name being "=", and stuff related to modifying const fields that
  // doesn't apply to initializers (which permit it in phase 1, but prevent it
  // in phase 2)

  Expr* scope = (info.scope) ? info.scope : getVisibilityBlock(call);
  bool explain = fExplainVerbose &&
    ((explainCallLine && explainCallMatch(call)) ||
     info.call->id == explainCallID);
  DisambiguationContext DC(&info.actuals, scope, explain);

  ResolutionCandidate* best = disambiguateByMatch(candidates, DC, FIND_NOT_REF_OR_CONST_REF);

  if (best && best->fn) {
    /*
     * Finish instantiating the body.  This is a noop if the function wasn't
     * partially instantiated.
     */

    instantiateBody(best->fn);

    if (explainCallLine && explainCallMatch(call)) {
      USR_PRINT(best->fn, "best candidate is: %s", toString(best->fn));
    }
  }

  // Future work note: the repeated check to best and best->fn means that we
  // could probably restructure this function to a better form.
  if (call->partialTag && (!best || !best->fn ||
                           !best->fn->hasFlag(FLAG_NO_PARENS))) {
    if (best != NULL) {
      // MPF 2016-0106 - this appears to be dead code
      // at least in a full single-locale test run.

      // best is deleted below with the other candidates
      best = NULL;
    }
  } else if (!best) {
    if (candidates.n > 0) {
      Vec<FnSymbol*> candidateFns;
      forv_Vec(ResolutionCandidate*, candidate, candidates) {
        candidateFns.add(candidate->fn);
      }

      printResolutionErrorAmbiguous(candidateFns, &info);
    } else {
      printResolutionErrorUnresolved(visibleFns, &info);
    }
  }
  // removed the creation of wrappers and the lvalue check call, as resolving
  // the _new call will handle all that stuff for us.
  // NOTE: This is unlikely to work for generic records.

  FnSymbol* resolvedFn = best != NULL ? best->fn : NULL;

  forv_Vec(ResolutionCandidate*, candidate, candidates) {
    delete candidate;
  }

  if (call->partialTag) {
    if (!resolvedFn) {
      return;
    }
    call->partialTag = false;
  }


  if (!resolvedFn) {
    INT_FATAL(call, "unable to resolve call");
  }

  if (resolvedFn && call->parentSymbol) {
    SET_LINENO(call);
    call->baseExpr->replace(new SymExpr(resolvedFn));
  }

  checkForStoringIntoTuple(call, resolvedFn);

  resolveNormalCallCompilerWarningStuff(resolvedFn);
}

// Copied from resolveFns(FnSymbol* fn) in functionResolution.
// Removed code for extern functions (since I don't think it will apply),
// iterators, type constructors, and FLAG_PRIVATIZED_CLASS.
void resolveMatch(FnSymbol* fn) {
  if (fn->isResolved())
    return;

  if (fn->id == breakOnResolveID) {
    printf("breaking on resolve fn:\n");
    print_view(fn);
    gdbShouldBreakHere();
  }

  fn->addFlag(FLAG_RESOLVED);

  insertFormalTemps(fn);

  bool wasGeneric = fn->_this->type->symbol->hasFlag(FLAG_GENERIC);
  if (wasGeneric) {
    AggregateType* at = toAggregateType(fn->_this->type);
    INT_ASSERT(at);
    bool res = at->setNextGenericField();
    INT_ASSERT(res);
  }

  resolveBlockStmt(fn->body);

  if (wasGeneric == true && isClass(fn->_this->type) == true) {
    FnSymbol* classAlloc = buildClassAllocator(fn);

    normalize(classAlloc);
  }

  if (tryFailure) {
    fn->removeFlag(FLAG_RESOLVED);
    return;
  }

  resolveReturnType(fn);

  toAggregateType(fn->_this->type)->initializerResolved = true;

  //
  // insert casts as necessary
  //
  insertAndResolveCasts(fn);

  //
  // make sure methods are in the methods list
  //
  ensureInMethodList(fn);
}

void temporaryInitializerFixup(CallExpr* call) {
  if (UnresolvedSymExpr* usym = toUnresolvedSymExpr(call->baseExpr)) {
    // Support super.init() calls (for instance) when the super type does not
    // define either an initializer or a constructor.  Also ignores errors from
    // improperly inserted .init() calls (so be sure to check here if something
    // is behaving oddly - Lydia, 08/19/16)
    if (strcmp(usym->unresolved, "init") == 0 &&
        call->numActuals()               >= 2 &&
        !isNamedExpr(call->get(2))) {
      // Arg 2 will be a NamedExpr to "this" if we're in an intentionally
      // inserted initializer call
      SymExpr* _mt = toSymExpr(call->get(1));
      SymExpr* sym = toSymExpr(call->get(2));

      INT_ASSERT(sym != NULL);

      if (AggregateType* ct = toAggregateType(sym->symbol()->type)) {

        if (isRefWrapperForNonGenericRecord(ct) == false &&
            ct->initializerStyle                == DEFINES_NONE_USE_DEFAULT) {

          // This code should be removed when the compiler generates
          // initializers as the default method of construction and
          // initialization for a type (Lydia note, 08/19/16)
          usym->unresolved = astr("_construct_", ct->symbol->name);

          _mt->remove();
        }
      }
    }
  }
}


//
// Noakes 2017/03/26
//   The function temporaryInitializerFixup is designed to update
//   certain calls to init() while the initializer update matures.
//
//   Unfortunately this transformation is triggered incorrectly for uses of
//           this.init(...);
//
//   inside initializers for non-generic records.
//
//   For those uses of init() the "this" argument has currently has type
//   _ref(<Record>) rather than <Record>
//
//  This rather unfortunate function catches this case and enables the
//  transformation to be skipped.
//
static bool isRefWrapperForNonGenericRecord(AggregateType* at) {
  bool retval = false;

  if (isClass(at)                           == true &&
      strncmp(at->symbol->name, "_ref(", 5) == 0    &&
      at->fields.length                     == 1) {
    Symbol* sym = toDefExpr(at->fields.head)->sym;

    if (strcmp(sym->name, "_val") == 0) {
      retval = isNonGenericRecordWithInitializers(sym->type);
    }
  }

  return retval;
}














void removeAggTypeFieldInfo() {
  forv_Vec(AggregateType, at, gAggregateTypes) {
    if (at->symbol->defPoint && at->symbol->defPoint->parentSymbol) {
      // Still in the tree
      if (at->initializerStyle == DEFINES_INITIALIZER) {
        // Defined an initializer (so we left its init and exprType information
        // in the tree)
        for_fields(field, at) {
          if (field->defPoint->exprType) {
            field->defPoint->exprType->remove();
          }
          if (field->defPoint->init) {
            field->defPoint->init->remove();
          }
          // Remove the init and exprType information.
        }
      }
    }
  }
}
