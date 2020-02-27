/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
#include "DecoratedClassType.h"
#include "driver.h"
#include "resolution.h"
#include "stmt.h"
#include "stlUtil.h"
#include "wellknown.h"

// 'markPruned' replaced deletion from SymbolMap, which does not work well.
Symbol*           markPruned      = NULL;

// initial value for 'uses' SymbolMap
Symbol*           markUnspecified = NULL;


/************************************* | **************************************
*                                                                             *
* Several Chapel parallel statements accept an optional task-intent clause.   *
*                                                                             *
* Consider a begin statement that includes a task-intent clause for example.  *
* This is represented as a block stmt that includes                           *
*                                                                             *
*   1) a blockInfo property for "begin block"                                 *
*   2) a byrefVars property that is a PRIM_ACTUALS_LIST CallExpr              *
*   3) the body of the begin stmt                                             *
*   4) a call to update the endCount when the block completes                 *
*                                                                             *
* The byrefVars property is then a list of pairs { marker, variable } where   *
* marker is one of 6 constant ArgSymbols that encode the intent.              *
*                                                                             *
* 2017/05/18: The challenge is to prevent these ArgSymbols from being purged  *
* by the cleanAST() phase at the end of each pass.  There is no  well defined *
* way to do this currently.                                                   *
*                                                                             *
* The approach that is used here is to wrap the ArgSymbols in a dummy         *
* function and insert this function in to the tree.                           *
*                                                                             *
* It is inserted in to rootModule to avoid cluttering the scope for the       *
* module "theProgram".  This in turn implies that it must be pre-normalized   *
* to avoid triggering the AST checks for --verify.                            *
*                                                                             *
* This dummy function is never resolved and is purged at the end of the       *
* function resolution pass.                                                   *
*                                                                             *
************************************** | *************************************/

static ArgSymbol* tiMarkBlank     = NULL;
static ArgSymbol* tiMarkIn        = NULL;
static ArgSymbol* tiMarkConstDflt = NULL;
static ArgSymbol* tiMarkConstIn   = NULL;
static ArgSymbol* tiMarkConstRef  = NULL;
static ArgSymbol* tiMarkRef       = NULL;

void initForTaskIntents() {
  FnSymbol* tiMarkHost = NULL;

  markPruned      = gVoid;
  markUnspecified = gNil;

  tiMarkBlank     = new ArgSymbol(INTENT_BLANK,
                                  "tiMarkBlank",
                                  dtNothing);

  tiMarkIn        = new ArgSymbol(INTENT_IN,
                                  "tiMarkIn",
                                  dtNothing);

  tiMarkConstDflt = new ArgSymbol(INTENT_CONST,
                                  "tiMarkConstDflt",
                                  dtNothing);

  tiMarkConstIn   = new ArgSymbol(INTENT_CONST_IN,
                                  "tiMarkConstIn",
                                  dtNothing);

  tiMarkConstRef  = new ArgSymbol(INTENT_CONST_REF,
                                  "tiMarkConstRef",
                                  dtNothing);

  tiMarkRef       = new ArgSymbol(INTENT_REF,
                                  "tiMarkRef",
                                  dtNothing);

  tiMarkHost = new FnSymbol("tiMarkHost");

  tiMarkHost->insertFormalAtTail(tiMarkBlank);
  tiMarkHost->insertFormalAtTail(tiMarkIn);
  tiMarkHost->insertFormalAtTail(tiMarkConstDflt);
  tiMarkHost->insertFormalAtTail(tiMarkConstIn);
  tiMarkHost->insertFormalAtTail(tiMarkConstRef);
  tiMarkHost->insertFormalAtTail(tiMarkRef);

  tiMarkHost->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));

  rootModule->block->insertAtTail(new DefExpr(tiMarkHost));
}

// Return the tiMark symbol for the given ForallIntentTag.
// Do not invoke on TFI_REDUCE, TPV and helper intents.
ArgSymbol* tiMarkForForallIntent(ShadowVarSymbol* svar) {
  switch (svar->intent) {
    case TFI_DEFAULT:     return tiMarkBlank;
    case TFI_CONST:       return tiMarkConstDflt;
    case TFI_IN:          return tiMarkIn;
    case TFI_CONST_IN:    return tiMarkConstIn;
    case TFI_REF:         return tiMarkRef;
    case TFI_CONST_REF:   return tiMarkConstRef;

    case TFI_TASK_PRIVATE:
      USR_FATAL_CONT(svar, "task-private variables are not supported in with-clauses for coforall/cobegin/begin blocks");
      break;

    case TFI_IN_PARENT:
    case TFI_REDUCE:      // reduce intents are handled in addTaskIntent()
    case TFI_REDUCE_OP:
    case TFI_REDUCE_PARENT_AS:
    case TFI_REDUCE_PARENT_OP:
      INT_FATAL("unexpected intent in tiMarkForForallIntent()");
      break;
  }
  return tiMarkBlank; //dummy
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

// Is 'type' a Reduce/Scan Op?
// similar to isArrayClass()
bool isReduceOp(Type* type) {
  bool retval = false;

  type = canonicalDecoratedClassType(type);

  if (type->symbol->hasFlag(FLAG_REDUCESCANOP) == true) {
    retval = true;

  } else if (AggregateType* at = toAggregateType(type)) {
    forv_Vec(AggregateType, t, at->dispatchParents) {
      if (isReduceOp(t) == true) {
        retval = true;
        break;
      }
    }
  }

  return retval;
}

//
// Set up anchors, if not already, so we can add reduction-related code
// via refRef->insertBefore() within 'fn'.
//
// "redRef" is short for "reference for reduction".
// redRef1 goes at the beginning of fn, redRef2 at the end.
//
static void setupRedRefs(FnSymbol* fn, bool nested,
                         Expr*& redRef1, Expr*& redRef2)
{
  if (redRef1) return;

  // We will insert new ASTs at the beginning of 'fn' -> before 'redRef1',
  // and at the end of 'fn' -> before 'redRef2'.
  redRef1 = new CallExpr("redRef1");
  redRef2 = new CallExpr("redRef2");
  fn->insertAtHead(redRef1);
  fn->insertBeforeEpilogue(redRef2);
  if (nested) {
    // move redRef2 one up so it is just before _downEndCount()
    CallExpr* dc = toCallExpr(redRef2->prev);
    INT_ASSERT(dc && dc->isNamed("_downEndCount"));
    dc->insertBefore(redRef2->remove());
  }
}

//
// We won't need the redRef anchors any more. Remove them if we set them up.
//
static void cleanupRedRefs(Expr*& redRef1, Expr*& redRef2) {
  if (!redRef1) return;
  redRef1->remove();
  redRef2->remove();
  redRef1 = redRef2 = NULL;
}

//
// Find the _waitEndCount and _endCountFree calls that comes after 'fromHere'.
// Since these two calls come together, it is prettier to insert
// our stuff after the latter.
//
static Expr* findTailInsertionPoint(Expr* fromHere, bool isCoforall) {
  Expr*     curr   = (isCoforall) ? fromHere->parentExpr : fromHere;
  CallExpr* result = NULL;

  while ((curr = curr->next)) {
    if (CallExpr* call = toCallExpr(curr))
      if (call->isNamed("_waitEndCount")) {
        result = call;
        break;
      }
  }

  INT_ASSERT(result);

  // MPF 2017-08-25:
  // This used to also cover _endCountFree, but now it is in
  // a DeferStmt before _waitEndCount, so finding _waitEndCount is
  // sufficient.
  return result;
}

/*
To implement task reduce intents, we follow the steps in
propagateExtraLeaderArgs() and setupOneReduceIntent()

I.e. add the following to the AST:

* before 'call'
    def globalOp = new reduceType(origSym.type);

    // incorporate the initial value of 'origSym'
    // using one of initialAccumulate() or accumulate(), ex:
    globalOp.accumulate(origSym);

* pass globalOp to call();
  corresponding formal in 'fn': parentOp

* inside 'fn'
    def currOp = parentOp.clone()
    def symReplace = currOp.identify;
    ...
    currOp.accumulate(symReplace);
    parentOp.combine(currOp);
    delete currOp;

* after 'call' and its _waitEndCount()
    origSym = parentOp.generate();
    delete parentOp;

Put in a different way, a coforall like this:

    var x: int;
    coforall ITER with (OP reduce x) {
      BODY(x);      // will typically include:  x OP= something;
    }

with its corresponding task-function representation:

    var x: int;
    proc coforall_fn() {
      BODY(x);
    }
    call coforall_fn();

is transformed into

    var x: int;
    var globalOp = new OP_SCAN_REDUCE_CLASS(x.type);
    if (canResolveMethod(globalOp, "initialAccumulate", x))
      globalOp.initialAccumulate(x);
    else
      globalOp.accumulate(x);

    proc coforall_fn(parentOp) {
      var currOp = parentOp.clone()
      var symReplace = currOp.identify;

      BODY(symReplace);

      currOp.accumulate(symReplace);
      parentOp.combine(currOp);
      delete currOp;
    }

    call coforall_fn(globalOp);
    // wait for endCount - not shown
    x = globalOp.generate();
    delete globalOp;

Todo: to support cobegin constructs, need to share 'globalOp'
across all fn+call pairs for the same construct.
*/

static void insertInitialAccumulate(Expr* headAnchor,
                                    VarSymbol* globalOp,
                                    Symbol* origSym) {
  Expr* condExpr = new_Expr("'method call resolves'(%S, %S, %S)", globalOp,
                            new_StringSymbol("initialAccumulate"), origSym);
  Expr* thenCase = new_Expr("initialAccumulate(%S, %S, %S)",
                            gMethodToken, globalOp, origSym);
  Expr* elseCase = new_Expr("accumulate(%S, %S, %S)",
                            gMethodToken, globalOp, origSym);
  CondStmt* condStmt = new CondStmt(condExpr, thenCase, elseCase);
  headAnchor->insertBefore(condStmt);
}

static void addReduceIntentSupport(FnSymbol* fn, CallExpr* call,
                                   TypeSymbol* reduceType, Symbol* origSym,
                                   ArgSymbol*& newFormal, Symbol*& newActual,
                                   Symbol*& symReplace, bool isCoforall,
                                   Expr*& redRef1, Expr*& redRef2)
{
  setupRedRefs(fn, true, redRef1, redRef2);
  VarSymbol* globalOp = new VarSymbol("reduceGlobal");
  globalOp->addFlag(FLAG_NO_CAPTURE_FOR_TASKING);
  newActual = globalOp;

  VarSymbol* eltType = newTemp("redEltType");
  eltType->addFlag(FLAG_MAYBE_TYPE);

  Expr* headAnchor = call;
  if (isCoforall) headAnchor = headAnchor->parentExpr;
  headAnchor->insertBefore(new DefExpr(eltType));
  headAnchor->insertBefore("'move'(%S, 'typeof'(%S))", eltType, origSym);
  headAnchor->insertBefore(new DefExpr(globalOp));

  Type* newT = getDecoratedClass(reduceType->type, CLASS_TYPE_GENERIC_NONNIL);

  CallExpr* newOp = new CallExpr(PRIM_NEW,
                                 newT->symbol,
                                 new NamedExpr("eltType", new SymExpr(eltType)),
                                 new NamedExpr(astr_chpl_manager,
                                             new SymExpr(dtUnmanaged->symbol)));
  headAnchor->insertBefore(new CallExpr(PRIM_MOVE, globalOp, newOp));

  insertInitialAccumulate(headAnchor, globalOp, origSym);

  Expr* tailAnchor = findTailInsertionPoint(call, isCoforall);
  // origSym = globalOp.generate() (via genTemp); delete globalOp;
  VarSymbol* genTemp = newTemp("genTemp");
  genTemp->addFlag(FLAG_INSERT_AUTO_DESTROY);
  tailAnchor->insertAfter(new DefExpr(genTemp),
                          new CallExpr(PRIM_MOVE, genTemp,
                            new CallExpr("generate", gMethodToken, globalOp)),
                          new CallExpr("=", origSym, genTemp),
                          new CallExpr("chpl__delete", globalOp));

  ArgSymbol* parentOp = new ArgSymbol(INTENT_BLANK, "reduceParent", dtUnknown);
  newFormal = parentOp;

  VarSymbol* currOp = new VarSymbol("reduceCurr");
  VarSymbol* svar  = new VarSymbol(origSym->name, origSym->type);
  svar->addFlag(FLAG_INSERT_AUTO_DESTROY);
  symReplace = svar;

  redRef1->insertBefore(new DefExpr(currOp));
  redRef1->insertBefore("'move'(%S, clone(%S,%S))", // init
                        currOp, gMethodToken, parentOp);
  redRef1->insertBefore(new DefExpr(svar));
  redRef1->insertBefore("'move'(%S, identity(%S,%S))", // init
                        svar, gMethodToken, currOp);

  redRef2->insertBefore(new CallExpr("accumulate",
                                     gMethodToken, currOp, svar));
  redRef2->insertBefore(new CallExpr("chpl__reduceCombine",
                                     parentOp, currOp));
  redRef2->insertBefore(new CallExpr("chpl__cleanupLocalOp",
                                     parentOp, currOp));
}

// Is 'sym' an index var in the coforall loop
// for which the 'fn' was created?
static bool isCorrespCoforallIndex(FnSymbol* fn, Symbol* sym)
{
  if (!sym->hasFlag(FLAG_COFORALL_INDEX_VAR))
    return false;

  // If 'sym' is for the loop that 'call' belongs to,
  // they both come from the same BlockStmt.
  BlockStmt* block = toBlockStmt(fn->defPoint->parentExpr);
  INT_ASSERT(block);

  // I conjecture that if 'sym' comes from a different block,
  // it ain't going to be from that loop.
  if (sym->defPoint->parentExpr != block)
    return false;

  // FYI: presently, for a 'coforall', the enclosing block is a for loop.
  INT_ASSERT(block->isForLoop());

  // We could verify that 'sym' is defined via a 'move'
  // from the _indexOfInterest variable referenced by the SymExpr
  // block->blockInfoGet()->get(1). (It's a move from a tuple component
  // of _indexOfInterest, for zippered coforall loops.)
  //
  return true;
}

// We use modified versions of these in flattenFunctions.cpp:
//  isOuterVar(), findOuterVars();
//  addVarsToFormals() + replaceVarUsesWithFormals() ->
//    addVarsToFormalsActuals() + replaceVarUses()

//
// Should we consider this symbol as possibly an outer variable
// w.r.t. 'fn'?
//
static bool considerAsOuterVar(Symbol* sym, FnSymbol* fn) {
  if (sym->defPoint->parentSymbol == fn         || // defined in 'fn'
      sym->isParameter()                        || // includes isImmediate()
      sym->hasFlag(FLAG_INSTANTIATED_PARAM)     || // a param, too (during resolution)
      sym->defPoint->parentSymbol == rootModule || // a system symbol
      sym->hasFlag(FLAG_TEMP)                   || // a temp

      // NB 'type' formals do not have INTENT_TYPE
      sym->hasFlag(FLAG_TYPE_VARIABLE)     // 'type' aliases or formals
  ) {
    // these are either not variables or not defined outside of 'fn'
    return false;
  }

  if (isCorrespCoforallIndex(fn, sym))
    // we do not want these either
    return false;

  // otherwise, yes, consider it
  return true;
}


// Is 'sym' a non-const variable (including formals) defined outside of 'fn'?
// This is a modification of isOuterVar() from flattenFunctions.cpp.
//
static bool
isOuterVar(Symbol* sym, FnSymbol* fn) {
  Symbol* symParent = sym->defPoint->parentSymbol;
  Symbol* parent = fn->defPoint->parentSymbol;

  while (true) {
    if (!isFnSymbol(parent) && !isModuleSymbol(parent))
      return false;
    if (symParent == parent)
      return true;
    if (!parent->defPoint)
      // Only happens when parent==rootModule (right?). This means symParent
      // is not in any of the lexically-enclosing functions/modules, so
      // it's gotta be within 'fn'.
      return false;

    // continue to the enclosing scope
    INT_ASSERT(parent->defPoint->parentSymbol &&
               parent->defPoint->parentSymbol != parent); // ensure termination
    parent = parent->defPoint->parentSymbol;
  }
}

static void
findOuterVars(FnSymbol* fn, SymbolMap& uses) {
  std::vector<SymExpr*> SEs;
  collectLcnSymExprs(fn, SEs);

  for_vector(SymExpr, symExpr, SEs) {
      Symbol* sym = symExpr->symbol();

      if (considerAsOuterVar(sym, fn) && isOuterVar(sym, fn))
          uses.put(sym, markUnspecified);
  }
}

// Mark the variables listed in 'with' clauses, if any, with tiMark markers.
static void markOuterVarsWithIntents(CallExpr* byrefVars, SymbolMap& uses) {
  if (!byrefVars) return;
  Symbol* marker = NULL;

  // The actuals alternate:
  //  (tiMark arg | reduce opExpr), task-intent variable [, repeat]
  for_actuals(actual, byrefVars) {
    SymExpr* se = toSymExpr(actual);
    INT_ASSERT(se); // comes as an UnresolvedSymExpr from the parser,
                    // should have been resolved in ScopeResolve
                    // or it is a SymExpr over a tiMark ArgSymbol
                    //                 or over chpl__reduceGlob
    Symbol* var = se->symbol();
    if (marker) {
      checkTypeParamTaskIntent(se);
      SymbolMapElem* elem = uses.get_record(var);
      if (elem) {
        elem->value = marker;
      } else {
        if (isVarSymbol(marker)) {
          // this is a globalOp created in setupOneReduceIntent()
          INT_ASSERT(!strcmp(marker->name, "chpl__reduceGlob"));
          USR_WARN(byrefVars, "the variable '%s' is given a reduce intent and not mentioned in the loop body - it will have the unit value after the loop", var->name);
        }
      }
      marker = NULL;
    } else {
      marker = var;
      INT_ASSERT(marker);  // otherwise the alternation logic will not work
    }
  }
  INT_ASSERT(!marker);
}

// 'this' (the receiver) should *always* be passed by reference - because
// we want any updates to it in a task construct to be visible outside.
// That includes the implicit 'this' in the constructor - see
// the commit message for r21602. So we exclude those from consideration.
// While there, we prune other things for forall intents.
static void pruneOuterVars(Symbol* parent, SymbolMap& uses) {
  form_Map(SymbolMapElem, e, uses) {
      Symbol* sym = e->key;
      if (e->value != markPruned) {
        if (sym->hasFlag(FLAG_ARG_THIS))
          e->value = markPruned;
        else if (e->value != tiMarkIn        &&
                 sym->hasFlag(FLAG_CONST)    &&
                 !sym->hasFlag(FLAG_REF_VAR) &&
                 isGlobal(sym))
          // Do not handle global constants, unless they are passed by 'in'.
          // (We rely on the fact that 'in' intent must be specified
          // explicitly, it cannot be the default intent.)
          // That way the original broadcast variable will be accessed.
          // Without this exclusion, those variables may be serialized
          // to remote locales, causing extra comm. Ex. see this test:
          //  optimizations/remoteValueForwarding/serialization/domains.chpl
          //
          // Note that this does not prevent handling of global 'const ref'
          // variables. For robust treatment that takes into consideration
          // the type of the variable and its concrete intent, handling of
          // task intents should be done during resolution.
          e->value = markPruned;
      }
  }
}

//
// The 'vars' map describes the outer variables referenced
// in the task function 'fn', which is invoked by 'call'.
//
// BTW since call+fn have just been created to represent
// a syntactic task construct (begin or cobegin or coforall),
// they are in a 1:1 correspondence.
//
// Each key of 'vars' is a variable referenced in 'fn'.
// The corresponding value is one of:
//   markPruned   - if we should not do anything about that variable
//   a "tiMarker" - if the variable is an outer variable;
//                  the marker indicates the intent for this variable,
//                  it is markUnspecified if the intent is not given explicitly
//   a TypeSymbol - the same for the special case where the user requested
//                  a reduce intent for this variable (see below)
//
// For a variable mentioned in a reduce intent in the 'with' clause,
// the parser maps it to the name of the appropriate reduction class.
// scopeResolve() resolves it to the class's TypeSymbol.
// markOuterVarsWithIntents() places that into the 'vars' map.
//

//
// For each outer variable of the task function 'fn':
//   * add an appropriate actual to 'call' and formal to 'fn'
//   * update that variable's entry in 'vars' to be the symbol
//     that the outer variable should be replaced with in the body of 'fn'
//
// Upon entry to addVarsToFormalsActuals(), 'vars' specified the task intent
// for each outer variable (see the above comment about 'vars').
// If it is a non-reduce intent, the actual is the outer variable itself
// and the outer variable is replaced with the corresponding formal.
// For a reduce intent, see the comment for addReduceIntentSupport().
//
static void
addVarsToFormalsActuals(FnSymbol* fn, SymbolMap& vars,
                        CallExpr* call, bool isCoforall)
{
  Expr *redRef1 = NULL, *redRef2 = NULL;
  form_Map(SymbolMapElem, e, vars) {
      Symbol* sym = e->key;
      if (e->value != markPruned) {
        SET_LINENO(sym);
        ArgSymbol* newFormal = NULL;
        Symbol*    newActual = NULL;
        Symbol*    symReplace = NULL;

        // If we see a TypeSymbol here, it came from a reduce intent.
        // (See the above comment about 'vars'.)
        if (TypeSymbol* reduceType = toTypeSymbol(e->value)) {
          bool gotError = false;
          // For cobegin, these will report the error for each task.
          // So maybe make it no-cont to avoid duplication?
          if (!isReduceOp(reduceType->type)) {
            USR_FATAL_CONT(call, "%s is not a valid reduction for a reduce intent", reduceType->name);
            gotError = true;
          }
          if (!isCoforall) {
            USR_FATAL_CONT(call, "reduce intents are not available for 'begin' and are not implemented for 'cobegin'");
            gotError = true;
          }
          if (gotError) continue; // skip addReduceIntentSupport() etc.

          addReduceIntentSupport(fn, call, reduceType, sym,
                                 newFormal, newActual, symReplace,
                                 isCoforall, redRef1, redRef2);
        } else {
          IntentTag argTag = INTENT_BLANK;
          if (ArgSymbol* tiMarker = toArgSymbol(e->value))
            argTag = tiMarker->intent;
          else
            INT_ASSERT(e->value == markUnspecified);

          newFormal = new ArgSymbol(argTag, sym->name, sym->type);
          if (sym->hasFlag(FLAG_COFORALL_INDEX_VAR))
            newFormal->addFlag(FLAG_COFORALL_INDEX_VAR);

          if (ArgSymbol* symArg = toArgSymbol(sym))
            if (symArg->hasFlag(FLAG_MARKED_GENERIC))
              newFormal->addFlag(FLAG_MARKED_GENERIC);
          newActual = e->key;
          symReplace = newFormal;
          // MPF 2017-03-09
          // I don't think this check should be here; it depends
          // on the type and in my experiments type is usually dtUnknown
          // at this point.
          if (!newActual->isConstant() && newFormal->isConstant())
            newFormal->addFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT);
        }

        call->insertAtTail(newActual);
        fn->insertFormalAtTail(newFormal);
        e->value = symReplace;  // aka vars->put(sym, symReplace);
      }
  }
  cleanupRedRefs(redRef1, redRef2);
}

static void replaceVarUses(Expr* topAst, SymbolMap& vars) {
  if (vars.n == 0) return;
  std::vector<SymExpr*> symExprs;
  collectSymExprs(topAst, symExprs);
  form_Map(SymbolMapElem, e, vars) {
    Symbol* oldSym = e->key;
    if (e->value != markPruned) {
      SET_LINENO(oldSym);
      Symbol* newSym = e->value;
      for_vector(SymExpr, se, symExprs)
        if (se->symbol() == oldSym)
          se->setSymbol(newSym);
    }
  }
}

static
bool isAtomicFunctionWithOrderArgument(FnSymbol* fnSymbol, ArgSymbol** order = NULL)
{
  if( !fnSymbol ) return false;
  Symbol* _this = fnSymbol->_this;
  if( !_this ) return false;
  if( !_this->typeInfo()->symbol->hasFlag(FLAG_ATOMIC_TYPE) ) return false;
  // is the last formal the order= argument?
  // Note that it must have the type specified since inferring it happens
  // in a later pass (resolution).
  int numFormals = fnSymbol->numFormals();
  if( numFormals >= 1 ) {
    ArgSymbol* lastFormal = fnSymbol->getFormal(numFormals);
    int has_order_type = lastFormal->typeInfo()->symbol->hasEitherFlag(FLAG_MEMORY_ORDER_TYPE, FLAG_C_MEMORY_ORDER_TYPE);
    int has_order_name = (0 == strcmp(lastFormal->name, "order"));
    if( has_order_name && ! has_order_type ) {
      INT_FATAL(lastFormal, "atomic method has order without type");
    }
    if( has_order_type ) {
      if( order ) *order = lastFormal;
      return true;
    }
  }
  return false;
}

//
// Converts blocks implementing various task constructs into
// functions, so they can be invoked by a separate task.
//
// The body of the original block becomes the body of the function,
// and the inline location of the block is replaced by a call to the
// implementing function.
//
// A subsequent step (flattenNesteFunctions) adds arguments to these
// functions to pass in values or references from the context which
// are used in the body of the block.
//
// As a special case, the target locale is prepended to the arguments passed
// to the "on" function.
//
void createTaskFunctions(void) {

  if( fCacheRemote ) {
    // Add fences to Atomics methods
    //  -- or do it with a flag on the network atomic impl fns
    //  for each method in an atomics type that has an order= argument,
    //   and which does not start/end with chpl_rmem_consist_maybe_release,
    //   add chpl_rmem_consist_maybe_release(order)
    //   add chpl_rmem_consist_maybe_acquire(order)
    //  only do this when the remote data cache is enabled.
    // Go through TypeSymbols looking for flag ATOMIC_TYPE
    forv_Vec(ModuleSymbol, module, gModuleSymbols) {
      if( module->hasFlag(FLAG_ATOMIC_MODULE) ) {
        // we could do this with for_alist ... as in getFunctions()
        // instead of creating a copy of the list of functions here.
        std::vector<FnSymbol*> moduleFunctions =
          module->getTopLevelFunctions(false);
        for_vector(FnSymbol, fnSymbol, moduleFunctions) {
          ArgSymbol* order = NULL;
          // Does this function have an order= argument?
          // If so, add memory consistency functions (future - if they are not
          // already there).
          if( isAtomicFunctionWithOrderArgument(fnSymbol, &order) ) {
            SET_LINENO(fnSymbol);
            fnSymbol->insertAtHead(
                new CallExpr("chpl_rmem_consist_maybe_release", order));
            fnSymbol->insertBeforeEpilogue(
                new CallExpr("chpl_rmem_consist_maybe_acquire", order));
          }
        }
      }
    }
  }

  // Process task-creating constructs. We include 'on' blocks, too.
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (block->isLoopStmt() == true) {
      // Loops are not a parallel block construct, so do nothing.
      // The isLoopStmt() test guards the call blockInfoGet() below
      // from issuing "Migration" warnings.

    } else if (CallExpr* const info = block->blockInfoGet()) {
      SET_LINENO(block);

      FnSymbol* fn = NULL;

      if (info->isPrimitive(PRIM_BLOCK_BEGIN)) {
        fn = new FnSymbol("begin_fn");
        fn->addFlag(FLAG_BEGIN);
      } else if (info->isPrimitive(PRIM_BLOCK_COBEGIN)) {
        fn = new FnSymbol("cobegin_fn");
        fn->addFlag(FLAG_COBEGIN_OR_COFORALL);
      } else if (info->isPrimitive(PRIM_BLOCK_COFORALL)) {
        fn = new FnSymbol("coforall_fn");
        fn->addFlag(FLAG_COBEGIN_OR_COFORALL);
      } else if (info->isPrimitive(PRIM_BLOCK_ELIDED_ON)) {
        // ignore it until after resolution. It will be removed in
        // callDestructors.
      } else if (info->isPrimitive(PRIM_BLOCK_ON) ||
                 info->isPrimitive(PRIM_BLOCK_BEGIN_ON) ||
                 info->isPrimitive(PRIM_BLOCK_COBEGIN_ON) ||
                 info->isPrimitive(PRIM_BLOCK_COFORALL_ON)) {
        fn = new FnSymbol("on_fn");
        fn->addFlag(FLAG_ON);

        // Remove the param arg that distinguishes a local-on
        SymExpr* isLocalOn = toSymExpr(info->argList.head->remove());
        if (isLocalOn->symbol() == gTrue) {
          fn->addFlag(FLAG_LOCAL_ON);

          // Insert runtime check
          if (!fNoLocalChecks) {
            SymExpr* curNodeID = new SymExpr(gNodeID);

            // Copy call that gets target nodeID
            VarSymbol* targetNodeID = newTemp("local_on_tmp", NODE_ID_TYPE);
            block->insertBefore(new DefExpr(targetNodeID));
            block->insertBefore(new CallExpr(PRIM_MOVE, targetNodeID, new CallExpr("chpl_nodeFromLocaleID", info->argList.head->copy())));

            // Build comparison
            CallExpr* neq = new CallExpr(PRIM_NOTEQUAL, curNodeID, targetNodeID);

            // Build error
            CallExpr* err = new CallExpr(PRIM_RT_ERROR, new_CStringSymbol("Local-on is not local"));

            CondStmt* cond = new CondStmt(neq, err);
            block->insertBefore(cond);
          }
        }

        if (info->isPrimitive(PRIM_BLOCK_BEGIN_ON)) {
          fn->addFlag(FLAG_NON_BLOCKING);
          fn->addFlag(FLAG_BEGIN);
        }
        if (info->isPrimitive(PRIM_BLOCK_COBEGIN_ON) ||
            info->isPrimitive(PRIM_BLOCK_COFORALL_ON)) {
          fn->addFlag(FLAG_NON_BLOCKING);
          fn->addFlag(FLAG_COBEGIN_OR_COFORALL);
        }

        ArgSymbol* arg = new ArgSymbol(INTENT_CONST_IN, "dummy_locale_arg", dtLocaleID);
        fn->insertFormalAtTail(arg);
      }
      else if (info->isPrimitive(PRIM_BLOCK_LOCAL) ||
               info->isPrimitive(PRIM_BLOCK_UNLOCAL))
        ; // Not a parallel block construct, so do nothing special.
      else
        INT_FATAL(block, "Unhandled blockInfo case.");

      if (fn) {
        INT_ASSERT(isTaskFun(fn));
        CallExpr* call = new CallExpr(fn);

        bool needsMemFence = true; // only used with fCacheRemote
        bool isBlockingOn = false;

        if( info->isPrimitive(PRIM_BLOCK_ON) ) {
          isBlockingOn = true;
        }

        // Add the call to the outlined task function.
        block->insertBefore(call);

        if( fCacheRemote ) {
          Symbol* parent = block->parentSymbol;
          if( parent ) {
            if( parent->hasFlag( FLAG_NO_REMOTE_MEMORY_FENCE ) ) {
              // Do not add remote memory barriers.
              needsMemFence = false;
            } else {
              FnSymbol* fnSymbol = toFnSymbol(parent);

              // For methods on atomic types, we do not add the memory
              // barriers, because these functions have an 'order'
              // argument, which needs to get passed to the memory barrier,
              // so they are handled above.
              needsMemFence = ! isAtomicFunctionWithOrderArgument(fnSymbol);
            }
          }
        }

        if (fn->hasFlag(FLAG_ON)) {
          // This puts the target locale expression "onExpr" at the start of the call.
          call->insertAtTail(block->blockInfoGet()->get(1)->remove());
        }

        block->insertBefore(new DefExpr(fn));

        if( fCacheRemote ) {
          /* We don't need to add a fence for the parent side of
             PRIM_BLOCK_BEGIN_ON
             PRIM_BLOCK_COBEGIN_ON
             PRIM_BLOCK_COFORALL_ON
             PRIM_BLOCK_BEGIN
               since upEndCount takes care of it. */

          // If we need memory barriers, put them around the call to the
          // task function. These memory barriers are ensuring memory
          // consistency.  Spawn barrier (release) is needed for any
          // blocking on statement. Other statements, including cobegin,
          // coforall, begin handle this in upEndCount.
          if( needsMemFence && isBlockingOn )
            call->insertBefore(new CallExpr("chpl_rmem_consist_release"));
        }

        if( fCacheRemote ) {
          // Join barrier (acquire) is needed for a blocking on, and it
          // will make sure that writes in the on statement are available
          // to the caller. Nonblocking on or begin don't block so it
          // doesn't make sense to acquire barrier after running them.
          // coforall, cobegin, and sync blocks do this in waitEndCount.
          if( needsMemFence && isBlockingOn )
            call->insertAfter(new CallExpr("chpl_rmem_consist_acquire"));
        }

        info->remove();

        // Now build the fn for the task or on statement.

        if( fCacheRemote ) {
          // We do a 'start' (acquire) memory barrier to prevent the task
          // from re-using cached elements from another task. This could
          // conceivably be handled by the tasking layer, but they already
          // have enough to worry about...

          // In order to support direct calls for on statements
          // with a target that is local, instead of adding
          // the fence to the task function, we instruct
          // create_block_fn_wrapper to do it on our behalf.
          fn->addFlag(FLAG_WRAPPER_NEEDS_START_FENCE);
        }

        // This block becomes the body of the new function.
        // It is flattened so _downEndCount appears in the same scope as the
        // function formals added below.
        for_alist(stmt, block->body)
          fn->insertAtTail(stmt->remove());

        if( fCacheRemote ) {
          // In order to make sure that any 'put' from the task is completed,
          // we do a 'finish' (release) barrier. If it's a begin,
          // nonblocking on, coforall, or cobegin though this will be
          // handled in _downEndCount -- so we just need to add the barrier
          // here for a blocking on statement. We don't add it redundantly
          // because other parts of the compiler rely on finding _downEndCount
          // at the end of certain functions.

          // As with FLAG_WRAPPER_NEEDS_START_FENCE above,
          // ask create_block_fn_wrapper to add the fence if it
          // is needed.
          if( isBlockingOn )
            fn->addFlag(FLAG_WRAPPER_NEEDS_FINISH_FENCE);
        }

        fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
        fn->retType = dtVoid;

        if (needsCapture(fn)) { // note: does not apply to blocking on stmts.

          // Convert referenced variables to explicit arguments.
          SymbolMap uses;
          findOuterVars(fn, uses);

          markOuterVarsWithIntents(block->byrefVars, uses);
          pruneOuterVars(call->parentSymbol, uses);

          if (block->byrefVars != NULL)
            block->byrefVars->remove();

          bool isCoforall = info->isPrimitive(PRIM_BLOCK_COFORALL) ||
                            info->isPrimitive(PRIM_BLOCK_COFORALL_ON);

          addVarsToFormalsActuals(fn, uses, call, isCoforall);
          replaceVarUses(fn->body, uses);
        }
      } // if fn
    } // if blockInfo

    // 'byrefVars' should have been eliminated for those blocks where it is
    // syntactically allowed, and should be always empty for anything else.
    INT_ASSERT(!block->byrefVars);

  } // for block

}  // createTaskFunctions()
