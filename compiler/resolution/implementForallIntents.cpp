/*
 * Copyright 2004-2015 Cray Inc.
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

#include "resolution.h"

#include "astutil.h"
#include "ForLoop.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "resolveIntents.h"


//
//-----------------------------------------------------------------------------
//  implementForallIntents1()
//-----------------------------------------------------------------------------
//
// This is Part 1 of implementing forall intents, executed earlier than Part 2:
// * discover the relevant AST pieces of a forall loop
// * compute outer variables
// * does some AST transformations
//


//
// Is 'sym' the user index variable for the forall loop body 'block' ?
//
static bool isCorrespIndexVar(BlockStmt* block, Symbol* sym)
{
  if (!sym->hasFlag(FLAG_INDEX_VAR))
    return false;

  return sym->defPoint->parentExpr == block->parentExpr;
}


//
// A forall-intents variation on isOuterVar() in createTaskFunctions.cpp:
// Is 'sym' defined outside 'block'?
//
static bool isOuterVar(Symbol* sym, BlockStmt* block) {
  if (sym->isParameter()               || // includes isImmediate()
      sym->hasFlag(FLAG_TEMP)          || // exclude these

      // Consts need no special semantics for begin/cobegin/coforall/on.
      // Implementation-wise, it is uniform with consts in nested functions.
      sym->hasFlag(FLAG_CONST)         ||

      // NB 'type' formals do not have INTENT_TYPE
      sym->hasFlag(FLAG_TYPE_VARIABLE)     // 'type' aliases or formals
  ) {
    // these are either not variables or not defined outside of 'fn'
    return false;
  }

  DefExpr*  defPt = sym->defPoint;
  Expr* parentExp = defPt->parentExpr;

  while (true) {
    if (!parentExp) {
      Symbol* parentSym = defPt->parentSymbol;
      if (isModuleSymbol(parentSym))
        // We reached the outermost level and did not come across 'block'.
        return true;

      defPt     = parentSym->defPoint;
      parentExp = defPt->parentExpr;
      continue;
    }
    if (parentExp == block)
      return false;

    parentExp = parentExp->parentExpr;
  }
  INT_ASSERT(false);
  return false; // dummy
}


// Do not count variables in 'with' clauses as "outer".
static bool isInWithClause(SymExpr* se) {
  if (Expr* parentExpr = se->parentExpr)
    if (CallExpr* parentCall = toCallExpr(parentExpr))
      if (parentCall->isPrimitive(PRIM_FORALL_LOOP))
        return true;
  return false;
}


//
// A forall-intents variation on findOuterVars() in createTaskFunctions.cpp:
// Find all symbols used in 'block' and defined outside of it.
//
static void findOuterVars(BlockStmt* block, SymbolMap& uses) {
  std::vector<SymExpr*> symExprs;

  collectSymExprs(block, symExprs);

  for_vector(SymExpr, symExpr, symExprs) {
    Symbol* sym = symExpr->var;

    if (isLcnSymbol(sym)) {
      if (!isCorrespIndexVar(block, sym) &&
          !isInWithClause(symExpr)       &&
          isOuterVar(sym, block))
        uses.put(sym, markUnspecified);
    }
  }
}

// Not to be invoked upon a reduce intent.
static void setShadowVarFlags(VarSymbol* svar, IntentTag intent) {
  if (intent & INTENT_FLAG_CONST)
    svar->addFlag(FLAG_CONST);
  if (intent & INTENT_FLAG_REF) {
    INT_ASSERT(!(intent & INTENT_FLAG_IN));
    svar->addFlag(FLAG_REF_VAR);
  } else {
    // If this assert fails, we need to handle this case.
    INT_ASSERT(intent & INTENT_FLAG_IN);
  }
  // If this assert fails, we need to handle this case.
  INT_ASSERT(!(intent & INTENT_FLAG_OUT));
  // These do not make sense for task/forall intents.
  INT_ASSERT(!(intent & (INTENT_FLAG_PARAM | INTENT_FLAG_TYPE)));
}

//
// Create a new var, for use as a shadow var in the body of the forall loop,
// for each un-pruned variable in 'uses'.
// Add the original var to 'outerVars' and the new var to 'shadowVars'
// so an ordered traversal of the two lists gives matching pairs.
// Count the number of these variables into 'numOuterVars'.
//
static void createShadowVars(DefExpr* defChplIter, SymbolMap& uses,
                             int& numShadowVars, int& totOuterVars,
                             std::vector<Symbol*>& outerVars,
                             std::vector<Symbol*>& shadowVars,
                             std::vector<Symbol*>& reduceGVars)
{
  numShadowVars = totOuterVars = 0;
  // we will have at most that many elements
  // todo instead have a single vector holding 3 Symbol*
  //      plus perhaps "isReduce" and "pruneit" booleans
  const int maxVars = uses.n;
  outerVars.reserve(maxVars);
  shadowVars.reserve(maxVars);
  reduceGVars.reserve(maxVars);

  form_Map(SymbolMapElem, e, uses) {
    if (e->value == markPruned)
      continue;

    totOuterVars++;
    Symbol* ovar = e->key;
    // If ovar is a reference, e.g. an index variable of
    // a 'var' iterator, we do not want to force
    // that ref type onto 'svar'. Otherwise the generated
    // code will store into *svar without initializing svar
    // first. Todo: what if ovar is a domain?
    Type* valtype = ovar->type->getValType();

    //
    // Distinguish the following cases for the outer variable 'ovar':
    //
    //   (C1) e->value == markUnspecified
    //        ==> ovar is not listed in the 'with' clause
    //
    //   (C2) e->value is one of "tiMark" ArgSymbols
    //        ==> get the forall intent from that tiMark
    //
    //   (C3) e->value is a "chpl__reduceGlob" VarSymbol
    //        ==> it is a reduce intent
    //
    // If (C1) or (C2), we may want to "prune" this variable,
    // i.e. do not handle it under a forall intent, instead
    // resort to the usual lexical scoping.
    //
    // If we do not prune, we compute (ovar, svar, globalOp)
    // to push onto our lists.
    //
    bool isUnspec = (e->value == markUnspecified);
    VarSymbol* svar;
    VarSymbol* globalOp = isUnspec ? NULL : toVarSymbol(e->value);
    bool isReduce = globalOp != NULL;
    IntentTag tiIntent = INTENT_BLANK;

    if (!isReduce) {
      ArgSymbol* tiMarker = NULL;

      // Compute the intent
      if (isUnspec) {
        // start with blank intent
      } else {
        tiMarker = toArgSymbol(e->value);
        INT_ASSERT(tiMarker);
        tiIntent = tiMarker->intent;
      }

      bool isMethodToken = ovar->type == dtMethodToken;
      // concreteIntent() does not work for MT.
      if (!isMethodToken)
        tiIntent = concreteIntent(tiIntent, valtype);

      // See if we want to prune it.
      bool pruneit = false;

      if (tiIntent == INTENT_REF) {
        // for efficiency
        pruneit = true;
      } else if (isMethodToken) {
        // If MT is present, _toLeader(..., _mt...) does not get resolved.
        // See e.g. parallel/taskPar/figueroa/taskParallel.chpl
        pruneit = true;
      } else if (isSyncType(ovar->type) || isAtomicType(ovar->type)) {
        // Currently we need it because sync variables do not get tupled
        // and detupled properly when threading through the leader iterator.
        // See e.g. test/distributions/dm/s7.chpl
        // Atomic vars might not work either.
        // And anyway, only 'ref' intent makes sense here.
        pruneit = true;
        USR_WARN(defChplIter, "sync, single, or atomic var '%s' currently can be passed into the forall loop by 'ref' intent only - %s is ignored", ovar->name, tiMarker ? intentDescrString(tiMarker->intent) : "default intent");
      } else if (isRecordWrappedType(ovar->type) &&
                 !(tiIntent & INTENT_FLAG_REF)) {
        // Threading through the leader for non-ref intents
        // may not work correctly for arrays/domains, so we avoid it.
        tiIntent = (tiIntent & INTENT_FLAG_CONST) ?
          INTENT_CONST_REF : INTENT_REF;
        USR_WARN(defChplIter, "Arrays, domains, and distributions currently can be passed into the forall loop by 'const', 'ref' or 'const ref' intent only. '%s' will be passed by %s.", ovar->name, intentDescrString(tiIntent));
      }

      if (pruneit) {
        e->value = markPruned;  // our loops ignore such case explicitly
        continue; // form_Map(uses)
      }
    }  // if !isReduce
    
    svar = new VarSymbol(ovar->name, valtype);
    if (isReduce) {
      if (ovar->hasFlag(FLAG_CONST))
        USR_FATAL_CONT(defChplIter,
          "reduce intent is applied to a 'const' variable %s", ovar->name);
      // The shadow variable will assume the reference from the leadIdx tuple.
      svar->addFlag(FLAG_REF_VAR);
      svar->type = valtype->getRefType();
    } else {
      setShadowVarFlags(svar, tiIntent); // instead of arg intents
    }
    outerVars.push_back(ovar);
    shadowVars.push_back(svar);
    reduceGVars.push_back(globalOp);
    e->value = svar;
    numShadowVars++;
  }
}


// Issue a warning when the outer var is a record with an array
// or domain field. That's because when we yield such a record
// as part of the yield tuple in the leader, we copy it, whereas
// semantically it should be a const ref.
static void
checkForRecordsWithArrayFields(Expr* ref, std::vector<Symbol*>& outerVars) {
  for_vector(Symbol, sym, outerVars)
    if (AggregateType* at = toAggregateType(sym->type->getValType())) {
      const char* varKind = NULL;
      if (isRecord(at)) varKind = "record";
      else if (isUnion(at)) varKind = "union";
      else INT_ASSERT(isClass(at));
      //todo: do we want this more aggressive warning?
      // if (varKind)
      //   USR_WARN(ref, "Achtung! var %s  type %s  decl %s\n",
      //            sym->name, sym->type->symbol->name, sym->stringLoc());
      if (varKind)
        for_alist(fieldExpr, at->fields)
          if (DefExpr* fieldDef = toDefExpr(fieldExpr))
            //
            // This test would result in too many warnings:
            //   blankIntentForType(fieldDef->sym->type) != INTENT_CONST_IN
            // We warn only about array/domain because copying them
            // is expensive.
            //
            if (isRecordWrappedType(fieldDef->sym->type)) {
              // If this assert fails: (a) should this Symbol really
              // be subject to forall intents? If so, (b) need to modify
              // USR_WARN below not to print its name.
              INT_ASSERT(!sym->hasFlag(FLAG_TEMP));
              USR_WARN(ref, "The blank forall intent for record and union variables is temporarily implemented as a copy, not 'const ref'. As a result, the %s variable '%s' is affected. Its field %s: %s inside the loop will be a copy, not alias, of its field outside the loop. Use a task-intent-clause to pass it by reference, e.g. 'with (ref %s)'.",
                       varKind, sym->name,
                       fieldDef->sym->name, fieldDef->sym->type->symbol->name,
                       sym->name);
              break; // one warning per variable is enough
            }
    }
}


// ForallLeaderArgs: add actuals to _toLeader/_toLeaderZip()/_toStandalone().
// (The leader will be converted to accept them during resolution.)
static void addActualsTo_toLeader(Symbol* serIterSym, int& numLeaderActuals,
                                  std::vector<Symbol*>& outerVars,
                                  std::vector<Symbol*>& shadowVars,
                                  std::vector<Symbol*>& reduceGVars)
{
  // Find that call to _toLeader() or similar,
  // starting from DefExpr of chpl__iter...
  CallExpr* tlCall = NULL;
  for (Expr* seekExpr = serIterSym->defPoint->next; seekExpr;
       seekExpr = seekExpr->next)
    if (CallExpr* seekCall = toCallExpr(seekExpr))
      if (seekCall->isPrimitive(PRIM_MOVE))
        if (CallExpr* seekSubCall = toCallExpr(seekCall->get(2)))
          if (seekSubCall->isNamed("_toLeader") ||
              seekSubCall->isNamed("_toLeaderZip") ||
              seekSubCall->isNamed("_toStandalone")) {
            tlCall = seekSubCall;
            break;
          }
  INT_ASSERT(tlCall);
  numLeaderActuals = 0;
  Expr* tlStmt = tlCall->getStmtExpr();
  // .. and add the actuals.
  for (size_t idx = 0; idx < outerVars.size(); idx++) {
    Symbol* ovar  = outerVars[idx];
    Symbol* svar  = shadowVars[idx];
    Symbol* globalOp = reduceGVars[idx];
    bool isReduce = globalOp != NULL;

    // keep in sync with setupShadowVarForRefIntents()
    if (!isReduce && svar->hasFlag(FLAG_REF_VAR)) {
      // createShadowVars() keeps 'const ref'-intent vars, drops 'ref'-vars
      INT_ASSERT(svar->hasFlag(FLAG_CONST));
      // Nothing to be done here - we will rely on lexical scoping.
      // Separately, ensure that we did not miss a reduction intent:
      INT_ASSERT(!globalOp);
    } else {
      Symbol* actual = ovar;
      if (isReduce) {
        // pass chpl__reduceGlob insetad
        actual = globalOp;
      } else if (isReferenceType(ovar->type)) {
        // If it is a reference, dereference it. E.g. m-lsms.chpl (-nl 1?).
        VarSymbol* deref = newTemp(ovar->name, ovar->type->getValType());
        tlStmt->insertBefore(new DefExpr(deref));
        tlStmt->insertBefore("'move'(%S, 'deref'(%S))", deref, ovar);
        actual = deref;
      }
      tlCall->insertAtTail(actual);
      numLeaderActuals++;
    }
  }
}


// Returns true if this variable has been taken care of.
static bool setupShadowVarForRefIntents(CallExpr* lcCall,
                                        Symbol* ovar, Symbol* svar)
{
  // keep in sync with addActualsTo_toLeader()
  // this function is invoked only when !isReduce
  if (svar->hasFlag(FLAG_REF_VAR)) {
    // createShadowVars() keeps 'const ref'-intent vars, drops 'ref'-vars
    INT_ASSERT(svar->hasFlag(FLAG_CONST));
    lcCall->insertBefore(new DefExpr(svar));
    if (isRecordWrappedType(ovar->type)) {
      // Just bit-copy, not "assign". Since 'svar' lives within the
      // forall loop body, no ref counter increment/decrement is needed.
      lcCall->insertBefore(new CallExpr(PRIM_MOVE, svar, ovar));
    } else {
      // Need to adjust svar's type.
      INT_ASSERT(svar->type == ovar->type->getValType()); // current state
      svar->type = ovar->type->getRefType();
      if (isReferenceType(ovar->type)) {
        // 'ovar' is already a reference, copy that reference.
        lcCall->insertBefore(new CallExpr(PRIM_MOVE, svar, ovar));
      } else {
        // Take a reference of 'ovar'.
        lcCall->insertBefore(new CallExpr(PRIM_MOVE, svar,
                               new CallExpr(PRIM_ADDR_OF, ovar)));
      }
    }
    return true;
  }
  // Not taken care of.
  return false;
}

static CallExpr* findLeadIdxCopyInit(Symbol* leadIdxSym,
                                     Symbol* leadIdxCopySym)
{
  CallExpr* lcCall = NULL;
  for (Expr* seekExpr = leadIdxCopySym->defPoint->next; seekExpr;
       seekExpr = seekExpr->next)
    if (CallExpr* seekCall = toCallExpr(seekExpr))
      if (seekCall->isPrimitive(PRIM_MOVE))
        if (SymExpr* seekArg1 = toSymExpr(seekCall->get(1)))
          if (seekArg1->var == leadIdxCopySym)
            if (SymExpr* seekArg2 = toSymExpr(seekCall->get(2)))
              if (seekArg2->var == leadIdxSym) {
                lcCall = seekCall;
                break;
              }
  INT_ASSERT(lcCall);
  return lcCall;
}

//
// Extracts 'ix'-th component from 'leadIdx' tuple into 'dest', i.e.
// inserts 'dest = leadIdx(ix)' before 'lcCall'.
//
// UMM note: the leader will create copies of the outer variables
// via task intents, then yield those copies via a tuple.
// This extracts such a copy and stores it into a shadow variable.
// Strictly speaking, a copy constructor aka chpl__initCopy()
// needs to be invoked from tuple component into the shadow variable,
// then the tuple component should be destructed.
// We forego these two operations, simply transfering the bits,
// because the cases where that matters are going to be rare.
//
static void extractFromLeaderYield(CallExpr* lcCall, int ix,
                                   Symbol* dest, Symbol* leadIdx)
{
  // Generating CallExpr(SymExpr(leadIdx), new_IntSymbol(ix))
  // did not work for me. So we generate leadIdx.x1 etc.  -vass 7'2014
  char buf[16];  sprintf(buf, "x%d", ix);

  lcCall->insertBefore("'move'(%S, '.v'(%S,%S))",
                       dest, leadIdx, new_StringSymbol(buf));
}

static void detupleLeadIdx(Symbol* leadIdxSym, Symbol* leadIdxCopySym,
                           int numLeaderActuals,
                           std::vector<Symbol*>& outerVars,
                           std::vector<Symbol*>& shadowVars,
                           std::vector<Symbol*>& reduceGVars)
{
  // ForallLeaderArgs: detuple what the leader yields.
  // (The leader will be converted to yield a tuple during resolution.)
  // Find the assignment leadIdxCopy:=leadIdx,
  // starting from DefExpr of chpl__leadIdxCopy.
  CallExpr* lcCall = findLeadIdxCopyInit(leadIdxSym, leadIdxCopySym);

  // ... and add the detupling.
  // First, for leadIdxCopy.
  int ix = 1;
  if (numLeaderActuals > 0)
    extractFromLeaderYield(lcCall, ix, leadIdxCopySym, leadIdxSym);

  // Then, for the shadow vars.
  for (size_t idx = 0; idx < outerVars.size(); idx++) {
    Symbol* ovar     = outerVars[idx];
    Symbol* svar     = shadowVars[idx];
    Symbol* globalOp = reduceGVars[idx];
    bool isReduce    = globalOp != NULL;
    if (!isReduce && setupShadowVarForRefIntents(lcCall, ovar, svar)) {
      // handled
    } else {
      // non-ref or reduce intents
      INT_ASSERT(numLeaderActuals > 0);
      lcCall->insertBefore(new DefExpr(svar));
      extractFromLeaderYield(lcCall, ++ix, svar, leadIdxSym);
      if (!isReduce) svar->addFlag(FLAG_INSERT_AUTO_DESTROY);
    }
  }

  // Finally, remove the original assignment.
  if (numLeaderActuals > 0)
    lcCall->remove();
}

static void findBlockWithDefOf(Expr* start, const char* varName,
                               BlockStmt*& resultBlock, DefExpr*& resultDef)
{
  for (Expr* curr = start; curr; curr = curr->next)
    if (BlockStmt* nestB = toBlockStmt(curr))
      if (Expr* nestH = nestB->body.head)
        if (DefExpr* def = toDefExpr(nestH))
          if (!strcmp(def->sym->name, varName)) {
            resultBlock = nestB;
            resultDef   = def;
            return;
          }

  INT_ASSERT(false); // not found... need to search harder?
  resultDef = NULL;  // suppress "may be used uninitialized" in callers
}

static BlockStmt* discoverFromFollowIter(DefExpr* defFollowIter)
{
  // find the follower for loop
  ForLoop* bFollowerLoop = NULL;

  for (Expr* curr = defFollowIter->next; curr; curr = curr->next) {
    if (ForLoop* forLoop = toForLoop(curr)) {
      if (SymExpr* seFollowIter = forLoop->iteratorGet()) {
        if (seFollowIter->var == defFollowIter->sym) {
          bFollowerLoop = forLoop;
          break;
        }
      }
    }
  }

  INT_ASSERT(bFollowerLoop);  // ensure we found it

  // find the forall loop body
  BlockStmt* bForallBody = NULL;

  for (Expr* curr = bFollowerLoop->body.head; curr; curr = curr->next) {
    if (BlockStmt* nestB = toBlockStmt(curr)) {
      if (nestB->byrefVars) {
        // what else can it be?
        INT_ASSERT(nestB->byrefVars->isPrimitive(PRIM_FORALL_LOOP));
        bForallBody = nestB;
        break;
      }
    }
  }

  INT_ASSERT(bForallBody);  // ensure we found it

  return bForallBody;
}

static void discoverForallBodies(DefExpr* defChplIter,
                                 BlockStmt*& firstForallBody,
                                 BlockStmt*& secondForallBody)
{
  // Find the forall loop body/bodies
  // by traversing the AST created in buildForallLoopStmt().

  BlockStmt* b1;
  DefExpr*   defLeadIdxCopy;
  bool       sa = !strcmp(defChplIter->sym->name, "chpl__iterSA");

  findBlockWithDefOf(defChplIter->next,
                     sa ? "chpl__saIdxCopy" : "chpl__leadIdxCopy",
                     b1, defLeadIdxCopy);

  if (sa) {
    // standalone case - get the first (and only) block that follows

    firstForallBody  = NULL;
    secondForallBody = NULL;
    for (Expr* curr = defLeadIdxCopy->next; curr; curr = curr->next)
      if (BlockStmt* block = toBlockStmt(curr)) {
        INT_ASSERT(block->byrefVars->isPrimitive(PRIM_FORALL_LOOP));
        firstForallBody = block;
        // this loop is for asserts only
        for (Expr* check = firstForallBody->next; check; check = check->next)
          INT_ASSERT(!isBlockStmt(check));
        break;
      }
    INT_ASSERT(firstForallBody);

  } else if (fNoFastFollowers) {
    // no fast followers

    BlockStmt* b2;
    DefExpr*   defFollowIter;
    findBlockWithDefOf(defLeadIdxCopy->next, "chpl__followIter",
                       b2, defFollowIter);

    firstForallBody = discoverFromFollowIter(defFollowIter);
    secondForallBody = NULL;

  } else {
    // yes fast followers

    //
    // If the function has been resolved, the current implementation is doomed
    // - because the 'if' that we will rely on may have been folded away.
    //
    // In that case we'd have to traverse all CallExprs
    // looking for SymExpr(s) for defChplIter->sym
    // that is/are in the PRIM_FORALL_LOOP CallExpr.
    // That/those will be our forall loop bodies.
    //
    // BTW FLAG_RESOLVED is already set on defChplIter->parentSymbol.
    // That's because it is set before the function body is resolved.
    // So I am testing for dtUnknown instead.
    //
    INT_ASSERT(defChplIter->sym->type == dtUnknown);

    // Find   if ... { def chpl__fastFollowIter; ... }
    //        else   { def chpl__followIter; ... }

    // start with the 'else'
    CondStmt* ifFfStmt      = NULL;
    DefExpr*  defFollowIter = NULL;
    for (Expr* curr = defLeadIdxCopy->next; curr; curr = curr->next)
      if (CondStmt* condS = toCondStmt(curr))
        if (BlockStmt* elseS = condS->elseStmt)
          if (elseS->body.head)
            if (DefExpr* def = toDefExpr(elseS->body.head))
              if (!strcmp(def->sym->name, "chpl__followIter")) {
                // OK, here is our 'if' and 'else'.
                ifFfStmt = condS;
                defFollowIter = def;
                break;
              }
    INT_ASSERT(ifFfStmt && defFollowIter); // ensure we found them

    // on to the 'then'
    BlockStmt* thenS = ifFfStmt->thenStmt;
    INT_ASSERT(thenS && thenS->body.head);
    DefExpr* defFastFollowIter = toDefExpr(thenS->body.head);
    INT_ASSERT(defFastFollowIter &&
               !strcmp(defFastFollowIter->sym->name, "chpl__fastFollowIter"));

    firstForallBody = discoverFromFollowIter(defFastFollowIter);
    secondForallBody = discoverFromFollowIter(defFollowIter);
  }
}

static void getIterSymbols(BlockStmt* body, Symbol*& serIterSym,
                           Symbol*& leadIdxSym, Symbol*& leadIdxCopySym)
{
  CallExpr* const byrefVars = body->byrefVars;
  INT_ASSERT(byrefVars->isPrimitive(PRIM_FORALL_LOOP));

  // Extract  chpl__iterLF,  chpl__leadIdx,  chpl__leadIdxCopy
  // or       chpl__iterSA,  chpl__saIdx,    chpl__saIdxCopy
  SymExpr* serIterSE     = toSymExpr(byrefVars->get(1)->remove());
  SymExpr* leadIdxSE     = toSymExpr(byrefVars->get(1)->remove());
  SymExpr* leadIdxCopySE = toSymExpr(byrefVars->get(1)->remove());
  INT_ASSERT(serIterSE && leadIdxSE && leadIdxCopySE);

  serIterSym     = serIterSE->var;
  leadIdxSym     = leadIdxSE->var;
  leadIdxCopySym = leadIdxCopySE->var;
}

static void getOuterVars(BlockStmt* body, SymbolMap& uses)
{
  CallExpr* const byrefVars = body->byrefVars;
  INT_ASSERT(byrefVars->isPrimitive(PRIM_FORALL_LOOP));

  // do the same as in 'if (needsCapture(fn))' in createTaskFunctions()
  findOuterVars(body, uses);
  markOuterVarsWithIntents(byrefVars, uses);
  pruneThisArg(body->parentSymbol, uses);
}

static void verifyOuterVars(BlockStmt* body2,
                            SymbolMap& uses1, int totOuterVars1)
{
  SymbolMap uses2;
  getOuterVars(body2, uses2);

  int totOuterVars2 = 0;
  form_Map(SymbolMapElem, e, uses2) {
    if (e->value != markPruned) {
      totOuterVars2++;
      Symbol* var2 = e->key;
      SymbolMapElem* elem1 = uses1.get_record(var2);
      INT_ASSERT(elem1 && elem1->key == var2);
    }
  }
  INT_ASSERT(totOuterVars1 == totOuterVars2);
}

//
// This is Part 1 of implementing forall intents, executed earlier than Part 2:
// * discover the relevant AST pieces of a forall loop
// * compute outer variables
// * does some AST transformations
//
void implementForallIntents1(DefExpr* defChplIter)
{
  //
  // Find the corresponding forall loop body(s).
  //
  // The following scenarios are defined in build.cpp
  // and matched against in the following:
  //
  // - Leader-follower case, when fast followers are enabled:
  //  - forallBody1 and forallBody2 are non-NULL
  //  - forallBody2->byrefVars is a PRIM_FORALL_LOOP whose first arguments are:
  //      chpl__iterLF, chpl__leadIdx, chpl__leadIdxCopy
  //
  // - Leader-follower case, when fast followers are disabled:
  //  - forallBody2 == NULL
  //  - forallBody1->byrefVars is what forallBody2->byrefVars is above
  //
  // - In all three cases, the user-speicified contents of the 'with' clause
  //   are appended to forallBody1/2->byrefVars specified above
  //
  // - Standalone case:
  //  - forallBody2 == NULL
  //  - forallBody1->byrefVars is a PRIM_FORALL_LOOP whose first arguments are:
  //      chpl__iterSA, chpl__saIdx, chpl__saIdxCopy
  //
  BlockStmt* forallBody1;
  BlockStmt* forallBody2;
  discoverForallBodies(defChplIter, forallBody1, forallBody2);

  // If both bodies are present, I expect them to be copies of one another,
  // except for byrefVars field (see above).
  // So we discover everything for the first one and verify
  // that it's the same for the second one.
  // Once we found it/them, process the first forall body clone.
  // Stash away misc things for comparison with the second one.

  Symbol *serIterSym, *leadIdxSym, *leadIdxCopySym;
  SymbolMap uses1;

  if (forallBody2)
    getIterSymbols(forallBody2, serIterSym, leadIdxSym, leadIdxCopySym);
  else
    getIterSymbols(forallBody1, serIterSym, leadIdxSym, leadIdxCopySym);

  getOuterVars(forallBody1, uses1);

  // Create shadow variables. We keep the vectors to ensure consistent
  // iteration order in for_vector(outerVars) vs. for_vector(shadowVars).
  std::vector<Symbol*> outerVars;
  std::vector<Symbol*> shadowVars;

  // Reduction-intent-related 'globalOp' variables, when non-NULL:
  // A 'globalOp' is an instance of a ReduceScanOp subclass.
  // It is created right before the forall, then threaded through and
  // yielded out of the leader as 'localOp'.
  // After the forall, outerVar = globalOp.generate().
  // Todo: these are so infrequent, store them e.g. in SymbolMaps instead?
  std::vector<Symbol*> reduceGVars;

  int numShadowVars, totOuterVars1;
  SET_LINENO(forallBody1);
  createShadowVars(defChplIter, uses1, numShadowVars, totOuterVars1,
                   outerVars, shadowVars, reduceGVars);

  if (numShadowVars > 0) {
    int numLeaderActuals; // set in addActualsTo_toLeader()

    checkForRecordsWithArrayFields(defChplIter, outerVars);

    addActualsTo_toLeader(serIterSym, numLeaderActuals,
                          outerVars, shadowVars, reduceGVars);

    detupleLeadIdx(leadIdxSym, leadIdxCopySym, numLeaderActuals,
                   outerVars, shadowVars, reduceGVars);

    // replace outer vars with shadows in the loop body
    replaceVarUses(forallBody1, uses1);
  }

  forallBody1->byrefVars->remove();

  // Now process the second clone, using what we already computed.

  if (forallBody2)
  {
    // for assertions only
    if (fVerify)
      verifyOuterVars(forallBody2, uses1, totOuterVars1);

    // Threading through the leader call was done for forallBody1.

    if (numShadowVars > 0)
      // same outer variables, same shadow variables as for forallBody1
      replaceVarUses(forallBody2, uses1);

    forallBody2->byrefVars->remove();
  }
}


//
//-----------------------------------------------------------------------------
//  implementForallIntents2()
//-----------------------------------------------------------------------------
//
// This is Part 2 - it processes each call to _toLeader() or _toLeaderZip()
// or _toStandalone().
//
// Throughout this file, for historical reasons, terms/identifiers with
// "leader" relate to both leader and standalone iterators.
//


// Todo: for a given leader, we could also cache specializations
// for each set of outer variables that we have seen.
// Like SymbolMapCache?
static Map<FnSymbol*,FnSymbol*> pristineLeaderIterators;


static FnSymbol* copyLeaderFn(FnSymbol* origFn, bool ignore_isResolved) {
  FnSymbol* copyFn = origFn->copy();
  origFn->defPoint->insertAfter(new DefExpr(copyFn));

  // also update paramMap
  // It makes sense to handle all param formals, not just "tag".
  Expr* oFormal = origFn->formals.head;
  Expr* cFormal = copyFn->formals.head;
  do {
    // assert: have at least "tag" formal; same #formals for origFn and copyFn
    INT_ASSERT(oFormal && cFormal);

    ArgSymbol* oArg = toArgSymbol(toDefExpr(oFormal)->sym);
    ArgSymbol* cArg = toArgSymbol(toDefExpr(cFormal)->sym);
    INT_ASSERT(oArg && cArg);
    Symbol* oMapped = paramMap.get(oArg);
    if (oMapped)
      paramMap.put(cArg, oMapped);
    // if !oMapped, nothing to do

    oFormal = oFormal->next;
    cFormal = cFormal->next;
  } while (oFormal);
  INT_ASSERT(!cFormal); // same #formals for origFn and copyFn

  // When invoked from resolveFns(), fn->isResolved() aka FLAG_RESOLVED
  // is already set, although resolution is not yet started.
  // fn->copy() carries this flag over, which is confusing. Reset it.
  if (ignore_isResolved) {
    // Even if this assert fails, we still want to remove FLAG_RESOLVED.
    INT_ASSERT(origFn->isResolved());
    copyFn->removeFlag(FLAG_RESOLVED);
  }

  //
  // We do not want copyFn to be already resolved because:
  //
  // * We need to resolve the _build_tuple calls that we will insert.
  //   If copyFn were already resolved, we could resolve those calls by hand
  //   like so:  resolveCall(bldTplCall); resolveFns(bldTplCall->isResolved());
  //   for each bldTplCall - the inserted _build_tuple call.
  //
  // * If copyFn were already resolved, chpl__leaderIdx's type would
  //   already have been set -- before we convert the leader's yield values
  //   from <whatever> to tuple(<whatever>, shadow variables).
  //   So we would need to redo chpl__leaderIdx's type and whatever it affects.
  //
  // Currently, as we assert, copyFn is not resolved - it will get resolved
  // right after this.
  //
  INT_ASSERT(!copyFn->isResolved());

  return copyFn;
}

void stashPristineCopyOfLeaderIter(FnSymbol* origLeader,
                                   bool ignore_isResolved)
{
  if (!pristineLeaderIterators.get(origLeader)) {
    // If it's already resolved, it's too late to stash away.
    // copyLeaderFn() verifies that after the fact.
    FnSymbol* leaderToStash = copyLeaderFn(origLeader, ignore_isResolved);
    pristineLeaderIterators.put(origLeader, leaderToStash);
  }
}

//
// origRetExpr's symbol will no longer be recognized as a yield symbol in
// localizeReturnSymbols(), which will be called after resolution.
// So we are going to localize it right away.
//
// For that, search back from 'ref' from a PRIM_MOVE that sets that symbol,
// Create a new symbol then change that PRIM_MOVE to update this new symbol
// instead.
//
// Return this new symbol.
//
static VarSymbol* localizeYieldForExtendLeader(Expr* origRetExpr, Expr* ref) {
  SymExpr* orse = toSymExpr(origRetExpr);
  INT_ASSERT(orse);
  Symbol* origRetSym = orse->var;
  for (Expr* curr = ref->prev; curr; curr = curr->prev)
    if (CallExpr* call = toCallExpr(curr))
      if (call->isPrimitive(PRIM_MOVE) ||
          call->isNamed("="))
        if (SymExpr* dest = toSymExpr(call->get(1)))
          if (dest->var == origRetSym) {
            VarSymbol* newOrigRet = newTemp("localRet", origRetSym->type);
            call->insertBefore(new DefExpr(newOrigRet));
            dest->var = newOrigRet;
            if (call->isNamed("=")) {
              // We are "initializing" localRet, not "assigning" to it.
              // An autoCopy of the r.h.s. will be inserted by a later pass.
              // David requests creating a new CallExpr instead of patching
              // the existing one.
              CallExpr* init = new CallExpr(PRIM_MOVE);
              for_actuals(actual, call)
                init->insertAtTail(actual->remove());
              call->replace(init);
            }
            return newOrigRet; // done
          }
  INT_ASSERT(false); // did not find the assignment
  return NULL;       // dummy
}

//
// Verify that 'origRet' is not used, and remove it from the tree.
//
static void checkAndRemoveOrigRetSym(Symbol* origRet, FnSymbol* parentFn) {
  // parentFn and this assert are just sanity checking for the caller
  INT_ASSERT(origRet->defPoint->parentSymbol == parentFn);

  std::vector<SymExpr*> symExprs;
  collectSymExprs(parentFn, symExprs);
  for_vector(SymExpr, se, symExprs)
    if (se->var == origRet) {
      // It may appear in a no-init assignment.
      bool OK = false;
      if (CallExpr* parent = toCallExpr(se->parentExpr))
        if (parent->isPrimitive(PRIM_MOVE))
          if (CallExpr* rhs = toCallExpr(parent->get(2)))
            if (rhs->isPrimitive(PRIM_NO_INIT)) {
              OK = true;
              parent->remove();
            }
      INT_ASSERT(OK);
    }

  // If none are found, we can yank origRet.
  origRet->defPoint->remove();
}

static void setupRedRefs(FnSymbol* fn, bool nested,
                         Expr*& redRef1, Expr*& redRef2)
{
  if (redRef1) return;

  // We will insert new ASTs at the beginning of 'fn' -> before 'redRef1',
  // and at the end of 'fn' -> before 'redRef2'.
  redRef1 = new CallExpr("redRef1");
  redRef2 = new CallExpr("redRef2");
  if (nested) {
    fn->insertAtHead(redRef1);
  } else {
    // Be cute - add new stuff past the defs of 'ret' and 'origRet'.
    fn->body->body.head->next->insertAfter(redRef1);
  }
  // insert before _downEndCount
  // by inserting before return then moving one up past _downEndCount
  fn->insertBeforeReturn(redRef2);
  CallExpr* dc = toCallExpr(redRef2->prev);
  INT_ASSERT(dc && dc->isNamed("_downEndCount"));
  dc->insertBefore(redRef2->remove());
}

static void cleanupRedRefs(Expr*& redRef1, Expr*& redRef2) {
  if (!redRef1) return;
  redRef1->remove();
  redRef2->remove();
  redRef1 = redRef2 = NULL;
}

// like isArrayClass()
static bool isReduceOp(Type* type) {
  if (type->symbol->hasFlag(FLAG_REDUCESCANOP))
    return true;
  forv_Vec(Type, t, type->dispatchParents)
    if (isReduceOp(t))
      return true;
  return false;
}

static const char* astrArg(int ix, const char* add1) {
  return astr("x", istr(ix+1), "_", add1);
}

//
// Propagate 'extraActuals' through the task constructs, implementing
// task intents. See the header comment for extendLeader().
//
static void propagateExtraLeaderArgs(CallExpr* call, VarSymbol* retSym,
                                     int numExtraArgs, Symbol* extraActuals[],
                                     bool reduceArgs[], bool nested)
{
  FnSymbol* fn = call->isResolved();
  INT_ASSERT(fn); // callee's responsibility
  if (fn->hasFlag(FLAG_WRAPPER)) {
    // We are not handling void-returning wrappers at the moment.
    INT_ASSERT(!(fn->getReturnSymbol() == gVoid || fn->retType == dtVoid));
  }

  Expr *redRef1 = NULL, *redRef2 = NULL;
  Symbol* extraFormals[numExtraArgs];
  Symbol* shadowVars[numExtraArgs];
  bool gotNestedReduce = false;

  for (int ix = 0; ix < numExtraArgs; ix++) {
    Symbol*     eActual = extraActuals[ix];
    bool        isReduce = nested ? reduceArgs[ix] :
        // todo: eliminate potential false positives
        // i.e. when there is a proper outer variable of a ReduceScanOp type
        isReduceOp(eActual->type);
    if (!nested) reduceArgs[ix] = isReduce;

    // Use named args to disambiguate from the already-existing iterator args,
    // just in case. This necessitates toNamedExpr() in handleCaptureArgs().
    const char* eName   =
      isReduce ? astrArg(ix, "reduceParent") :
        nested ? eActual->name :
          strcmp(eActual->name, "_tuple_expand_tmp_") ?
            astrArg(ix, eActual->name) // uniquify arg name
            : astrArg(ix, "tet");
  
    ArgSymbol*  eFormal = new ArgSymbol(INTENT_BLANK, eName, eActual->type);
    extraFormals[ix] = eFormal;
    call->insertAtTail(new NamedExpr(eName, new SymExpr(eActual)));
    fn->insertFormalAtTail(eFormal);

    // In leader outside any taskFn just use reduceParent.
    // Todo: also skip if there are no other taskFns or yields in 'fn'.
    if (isReduce && nested) {
      // We shouldn't bother with all this when it is not a task function.
      INT_ASSERT(isTaskFun(fn));
      gotNestedReduce = true;
      setupRedRefs(fn, nested, redRef1, redRef2);
      ArgSymbol* parentOp = eFormal; // the reduceParent arg
      VarSymbol* currOp   = new VarSymbol(astrArg(ix, "reduceCurr"));
      VarSymbol* svar     = new VarSymbol(astrArg(ix, "shadowVar"));
      redRef1->insertBefore(new DefExpr(currOp));
      redRef1->insertBefore("'move'(%S, clone(%S,%S))", // init
                            currOp, gMethodToken, parentOp);
      redRef1->insertBefore(new DefExpr(svar));
      redRef1->insertBefore("'move'(%S, identity(%S,%S))", // init
                            svar, gMethodToken, currOp);
      redRef2->insertBefore("accumulate(%S,%S,%S)",
                            gMethodToken, currOp, svar);
      redRef2->insertBefore("chpl__reduceCombine(%S,%S)", parentOp, currOp);
      redRef2->insertBefore("chpl__cleanupLocalOp(%S,%S)", parentOp, currOp);
      // use currOp instead of parentOp for yielding and passing to taskFns
      extraFormals[ix] = currOp;
      shadowVars[ix]   = svar;
    } else {
      shadowVars[ix] = NULL;
    }
  }

  cleanupRedRefs(redRef1, redRef2);

  // Propagate recursively into task functions and yields.
  std::vector<CallExpr*> rCalls;
  collectMyCallExprs(fn, rCalls, fn);
  for_vector(CallExpr, rcall, rCalls) {
    if (rcall->isPrimitive(PRIM_YIELD)) {
      // Make a tuple that includes the extra args.
      Expr* origRetArg = rcall->get(1)->remove();
      VarSymbol* newOrigRet = localizeYieldForExtendLeader(origRetArg, rcall);
      // We need to yield references to svars as part of tuples,
      // hence the _allow_ref version. However, with some promotion wrappers
      // e.g. npb/is/diten/is.chpl, this hits the bug that autoCopy on a tuple
      // with a ref component dereferences that component.
      // So we avoid _allow_ref if we do not have reduce intents.
      const char* buildName =
        gotNestedReduce ? "_build_tuple_always_allow_ref" : "_build_tuple";
      CallExpr* buildTuple = new CallExpr(buildName, newOrigRet);

      // add tuple components
      for (int ix = 0; ix < numExtraArgs; ix++) {
        bool isReduce = reduceArgs[ix];
        Symbol* svar = shadowVars[ix];
        if (isReduce && !svar)
          // Currently we do not create a shadow var for yields in this case.
          USR_FATAL(rcall, "yields outside of task constructs in the leader or standalone iterator are not supported with reduce intents");
        Symbol* tFormal;
        if (isReduce) {
          // pass 'svar' by reference
          VarSymbol* sref = new VarSymbol(astrArg(ix, "svarRef"));
          rcall->insertBefore(new DefExpr(sref));
          rcall->insertBefore("'move'(%S, 'addr of'(%S))", sref, svar);
          tFormal = sref;
        } else {
          tFormal = extraFormals[ix];
        }
        buildTuple->insertAtTail(new SymExpr(tFormal));
      }

      rcall->insertBefore("'move'(%S,%E)", retSym, buildTuple);
      rcall->insertAtTail(new SymExpr(retSym));

    } else if (FnSymbol* tfn = resolvedToTaskFun(rcall)) {
      // 'rcall' better be the only call to 'tfn'.
      // The following assert is a weak assurance of that.
      // For a strong assurance, we could additionally build a set of task
      // functions, calls to which we have seen.
      // OTOH our normal call verification should suffice: it will fail
      // the first propagated call if a second call propagates to same tfn.
      INT_ASSERT(tfn->defPoint->parentSymbol == fn);
      // Reduce intents do not make sense when a 'begin' outlives the iterator.
      if (tfn->hasFlag(FLAG_BEGIN))
        USR_FATAL_CONT(tfn, "reduce intents are not implemented with leader and standalone iterators that include 'begin' statement(s)");
      // Propagate the extra args recursively into 'tfn'.
      propagateExtraLeaderArgs(rcall, retSym, numExtraArgs,
                               extraFormals, reduceArgs, true);
    }
  }
}

//
// ForallLeaderArgs: propagate the extra args through the leader,
// specifically through a fresh copy of the leader.
// That is, replace
//   iter ITERATOR(originalArgs...) {
//     ... yield originalYield; ...
//   }
// with
//   iter ITERATOR(originalArgs..., forallOuterVars...) {
//     ... yield (originalYield, forallOuterVars...); ...
//   }
// where 'outerVars' are propagated through task constructs
// as per task intents. Since createTaskFunctions() is done
// earlier (before resolution), we have to do task intents
// for forallOuterVars ourselves.
//
static void extendLeader(CallExpr* call, CallExpr* origToLeaderCall) {
  FnSymbol* origIterFn = call->isResolved();
  INT_ASSERT(origIterFn);  // caller's responsibility

  // Replace the callee with a clone.
  if (origIterFn->isResolved()) {
    // ... using a pristine copy if the callee is already resolved.
    // See the comment on an assert in copyLeaderFn().
    origIterFn = pristineLeaderIterators.get(origIterFn);
    INT_ASSERT(origIterFn);
  }

  FnSymbol* iterFn = copyLeaderFn(origIterFn, /*ignore_isResolved:*/false);
  toSymExpr(call->baseExpr)->var = iterFn;

  // Setup the new return/yield symbol.
  VarSymbol* retSym  = newTemp("ret"); // its type is to be inferred
  Symbol* origRetSym = iterFn->replaceReturnSymbol(retSym, /*newRetType*/NULL);
  origRetSym->defPoint->insertBefore(new DefExpr(retSym));
  origRetSym->name = "origRet";

  int numExtraArgs = origToLeaderCall->numActuals()-1;
  INT_ASSERT(numExtraArgs > 0); // we shouldn't be doing all this otherwise
  Expr* origArg = origToLeaderCall->get(1);
  Symbol* extraActuals[numExtraArgs];
  bool    reduceArgs[numExtraArgs];   // computed in propagateExtraLeaderArgs
  for (int ix = 0; ix < numExtraArgs; ix++) {
    origArg = origArg->next;
    SymExpr* origSE = toSymExpr(origArg);
    INT_ASSERT(origSE); // if it is not a symbol, still need to make it happen
    extraActuals[ix] = origSE->var;
  }
  INT_ASSERT(!origArg->next); // we should have processed all args

  propagateExtraLeaderArgs(call, retSym, numExtraArgs,
                           extraActuals, reduceArgs, false);

  checkAndRemoveOrigRetSym(origRetSym, iterFn);
}

void implementForallIntents2(CallExpr* call, CallExpr* origToLeaderCall) {
  if (origToLeaderCall->numActuals() <= 1) {
    // No variables to propagate => no extendLeader.
    // Ensure we have a pristine copy for the other case.
    FnSymbol* origLeader = call->isResolved();
    INT_ASSERT(origLeader);

    if (!pristineLeaderIterators.get(origLeader))
      stashPristineCopyOfLeaderIter(origLeader, /*ignore_isResolved:*/ false);
  } else {
    extendLeader(call, origToLeaderCall);
  }
}
