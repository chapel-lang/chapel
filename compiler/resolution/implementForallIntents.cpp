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

#include "implementForallIntents.h"

#include "resolveFunction.h"

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
      sym->hasFlag(FLAG_INSTANTIATED_PARAM)    || // also a param
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


// Return true if 'se' corresponds to a variable listed in a 'with' clause.
// (We do not consider those variables as "outer".)
// Exprs in riSpecs do not get this special treatment, however.
static bool isInWithClause(SymExpr* se) {
  if (BlockStmt* parent = toBlockStmt(se->parentExpr))
    if (ForallIntents* fi = parent->forallIntents) {
      for_vector(Expr, expr, fi->fiVars)
        if (expr == se)
          return true;
    }
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
    Symbol* sym = symExpr->symbol();

    if (isLcnSymbol(sym)) {
      if (!isCorrespIndexVar(block, sym) &&
          !isInWithClause(symExpr)       &&
          isOuterVar(sym, block))
        uses.put(sym, markUnspecified);
    }
  }
}

// Not to be invoked upon a reduce intent.
static void setShadowVarFlags(Symbol* ovar, VarSymbol* svar, IntentTag intent) {
  if (intent & INTENT_FLAG_CONST) {
    svar->addFlag(FLAG_CONST);
    if (!ovar->isConstant())
      svar->addFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT);
  }
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

      if (tiIntent == INTENT_REF || tiIntent == INTENT_REF_MAYBE_CONST) {
        // for efficiency
        pruneit = true;

      } else if (isMethodToken) {
        // If MT is present, _toLeader(..., _mt...) does not get resolved.
        // See e.g. parallel/taskPar/figueroa/taskParallel.chpl
        pruneit = true;

      } else if (isAtomicType(ovar->type)) {
        // Currently we need it because sync variables do not get tupled
        // and detupled properly when threading through the leader iterator.
        // See e.g. test/distributions/dm/s7.chpl
        // Atomic vars might not work either.
        // And anyway, only 'ref' intent makes sense here.
        pruneit = true;

        USR_WARN(defChplIter,
                 "sync, single, or atomic var '%s' currently can be passed into the forall loop by 'ref' intent only - %s is ignored",
                 ovar->name,
                 tiMarker ? intentDescrString(tiMarker->intent) : "default intent");

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
      svar->type = dtUnknown;
    } else {
      setShadowVarFlags(ovar, svar, tiIntent); // instead of arg intents
    }

    outerVars.push_back(ovar);
    shadowVars.push_back(svar);
    reduceGVars.push_back(globalOp);

    e->value = svar;

    numShadowVars++;
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
        // pass chpl__reduceGlob instead
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
    {
      // Need to adjust svar's type.
      INT_ASSERT(svar->type == ovar->type->getValType()); // current state
      svar->type = ovar->type->getRefType();
      INT_ASSERT(svar->type);
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
          if (seekArg1->symbol() == leadIdxCopySym)
            if (SymExpr* seekArg2 = toSymExpr(seekCall->get(2)))
              if (seekArg2->symbol() == leadIdxSym) {
                lcCall = seekCall;
                break;
              }
  INT_ASSERT(lcCall);
  return lcCall;
}

static void insertExtractFromYield(Expr* ref, int ix,
                                   Symbol* dest, Symbol* srcTuple)
{
  // Generating CallExpr(SymExpr(srcTuple), new_IntSymbol(ix))
  // did not work for me. So we generate srcTuple.x1 etc.  -vass 7'2014
  char buf[16];  sprintf(buf, "x%d", ix);

  ref->insertBefore("'move'(%S, '.v'(%S,%S))",
                    dest, srcTuple, new_StringSymbol(buf));
}

static void insertExtractFromYieldAtHead(BlockStmt* block, int ix,
                                         Symbol* dest, Symbol* srcTuple) {
  insertExtractFromYield(block->body.head, ix, dest, srcTuple);
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
// We forego these two operations, simply transferring the bits,
// because the cases where that matters are going to be rare.
//
static void extractFromLeaderYield(CallExpr* lcCall, int ix,
                                   Symbol* dest, Symbol* leadIdx) {
  insertExtractFromYield(lcCall, ix, dest, leadIdx);
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
        if (seFollowIter->symbol() == defFollowIter->sym) {
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
      if (nestB->forallIntents) {
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
        INT_ASSERT(block->forallIntents);
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
    // In that case we'd have to traverse all forallIntents
    // looking for SymExpr(s) for defChplIter->sym.
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
  ForallIntents* const fi = body->forallIntents;

  // Extract  chpl__iterLF,  chpl__leadIdx,  chpl__leadIdxCopy
  // or       chpl__iterSA,  chpl__saIdx,    chpl__saIdxCopy
  serIterSym     = fi->iterRec->symbol();
  leadIdxSym     = fi->leadIdx->symbol();
  leadIdxCopySym = fi->leadIdxCopy->symbol();
}

// Mark the variables listed in 'with' clauses, if any, with tiMark markers.
// Same as markOuterVarsWithIntents() in createTaskFunctions.cpp,
// except uses ForallIntents.
static void markOuterVarsWithIntents(ForallIntents* fi, SymbolMap& uses) {
  if (!fi) return;
  int nv = fi->numVars();

  for (int i = 0; i < nv; i++) {
    Symbol* marker = NULL;
    if (fi->isReduce(i))
      marker = toSymExpr(fi->riSpecs[i])->symbol();
    else
      // TODO: avoid this wrapper, which is here for historical reasons.
      // Requires using something fancier than SymbolMap.
      marker = tiMarkForForallIntent(fi->fIntents[i]);

    Symbol* var = toSymExpr(fi->fiVars[i])->symbol();
    SymbolMapElem* elem = uses.get_record(var);
    if (elem) {
      elem->value = marker;
    } else {
      if (isVarSymbol(marker)) {
        // this is a globalOp created in setupOneReduceIntent()
        INT_ASSERT(!strcmp(marker->name, "chpl__reduceGlob"));
        USR_WARN(fi->riSpecs[i], "the variable '%s' is given a reduce intent and not mentioned in the loop body - it will have the unit value after the loop", var->name);
      }
    }
  }
}

static void getOuterVars(BlockStmt* body, SymbolMap& uses)
{
  INT_ASSERT(body->forallIntents);

  // do the same as in 'if (needsCapture(fn))' in createTaskFunctions()
  findOuterVars(body, uses);
  markOuterVarsWithIntents(body->forallIntents, uses);
  pruneOuterVars(body->parentSymbol, uses);
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
  // Sometimes defChplIter is introduced for ForallStmt. If so, ignore.
  if (defChplIter->sym->hasFlag(FLAG_CHPL__ITER_NEWSTYLE))
    return;

  if (!beforeLoweringForallStmts) {
    // We got here due to resolveBlockStmt(PARBlock) in lowerForallStmts().
    BlockStmt* forallBlock = toBlockStmt(defChplIter->parentExpr);
    INT_ASSERT(forallBlock && !forallBlock->forallIntents);
    // If so, forall intents must have been already handled.
    return;
  }
  
  //
  // Find the corresponding forall loop body(s).
  //
  // The following scenarios are defined in build.cpp
  // and matched against in the following:
  //
  // - Leader-follower case, when fast followers are enabled:
  //  - forallBody1 and forallBody2 are non-NULL
  //  - forallBody2->forallIntents->{iterRec,leadIdx,leadIdxCopy} contain:
  //      chpl__iterLF, chpl__leadIdx, chpl__leadIdxCopy
  //
  // - Leader-follower case, when fast followers are disabled:
  //  - forallBody2 == NULL
  //  - forallBody1->forallIntents contains what's listed above for
  //      forallBody2->forallIntents
  //
  // - Standalone case:
  //  - forallBody2 == NULL
  //  - forallBody1-forallIntents->{iterRec,leadIdx,leadIdxCopy} contain:
  //      chpl__iterSA, chpl__saIdx, chpl__saIdxCopy
  //
  // - In all three cases, the user-specified contents of the 'with' clause
  //   are in forallBody1/2->forallIntents (when non-NULL, see above),
  //   specifically in fiVars/fIntents/riSpecs.
  //
  BlockStmt* forallBody1;
  BlockStmt* forallBody2;
  discoverForallBodies(defChplIter, forallBody1, forallBody2);

  // If both bodies are present, I expect them to be copies of one another,
  // except for forallIntents field (see above).
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

    addActualsTo_toLeader(serIterSym, numLeaderActuals,
                          outerVars, shadowVars, reduceGVars);

    detupleLeadIdx(leadIdxSym, leadIdxCopySym, numLeaderActuals,
                   outerVars, shadowVars, reduceGVars);

    // replace outer vars with shadows in the loop body
    replaceVarUses(forallBody1, uses1);
  }

  forallBody1->removeForallIntents();

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

    forallBody2->removeForallIntents();
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
Map<FnSymbol*,FnSymbol*> pristineLeaderIterators;


FnSymbol* copyLeaderFn(FnSymbol* origFn, bool ignore_isResolved) {
  SET_LINENO(origFn->defPoint);
  FnSymbol* copyFn = origFn->copy();
  copyFn->addFlag(FLAG_INVISIBLE_FN);
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
  //   like so:  resolveCall(bldTplCall); resolveFns(bldTplCall->resolvedFunction());
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
// When PRIM_TO_LEADER resolves to a call to _iterator_for_loopexprNN(),
// aka 'origIter', all it does is invoke _toLeader on its argument.
// If so, do not do extendLeader() on it. Simply thread the extra args
// from origToLeaderCall into that _toLeader call.
//
// BTW _iterator_for_loopexprNN is created in buildLeaderIteratorFn()
// invoked from buildForallLoopExpr().
//
static void propagateExtraArgsForLoopIter(CallExpr* call,
                                          CallExpr* origToLeaderCall,
                                          FnSymbol* origIter)
{
  // Clone 'origIter' because we are messing with it.
  FnSymbol* newIter = copyLeaderFn(origIter, false);
  SymExpr* baseSE = toSymExpr(call->baseExpr);
  INT_ASSERT(baseSE && baseSE->symbol() == origIter);
  baseSE->replace(new SymExpr(newIter));

  // Find the _toLeader call.
  std::vector<CallExpr*> callsInIter;
  collectCallExprs(newIter, callsInIter);
  CallExpr* toLeaderCall = NULL;
  for_vector(CallExpr, callInIter, callsInIter) {
    if (callInIter->isNamed("_toLeader") ||
        callInIter->isNamed("_toLeaderZip") ) {
      // There must be only one such call.
      INT_ASSERT(!toLeaderCall);
      toLeaderCall = callInIter;
    }
  }
  INT_ASSERT(toLeaderCall);  // We must have found it.

  // The first argument is the iterator class, it does not participate.
  Expr* firstArg = origToLeaderCall->get(1);
  // Handle the other args
  while (Expr* secondArg = firstArg->next) {
    SymExpr* currActual = toSymExpr(secondArg->remove());
    Symbol* currVar = currActual->symbol();
    call->insertAtTail(currActual);
    ArgSymbol* currFormal = new ArgSymbol(INTENT_BLANK, currVar->name, currVar->type);
    newIter->insertFormalAtTail(currFormal);
    toLeaderCall->insertAtTail(currFormal);
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
VarSymbol* localizeYieldForExtendLeader(Expr* origRetExpr, Expr* ref) {
  SymExpr* orse = toSymExpr(origRetExpr);
  INT_ASSERT(orse);
  Symbol* origRetSym = orse->symbol();
  for (Expr* curr = ref->prev; curr; curr = curr->prev)
    if (CallExpr* call = toCallExpr(curr))
      if (call->isPrimitive(PRIM_MOVE) ||
          call->isNamedAstr(astrSequals))
        if (SymExpr* dest = toSymExpr(call->get(1)))
          if (dest->symbol() == origRetSym) {
            VarSymbol* newOrigRet = newTemp("localRet", origRetSym->type);
            call->insertBefore(new DefExpr(newOrigRet));
            dest->setSymbol(newOrigRet);
            if (call->isNamedAstr(astrSequals)) {
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
void checkAndRemoveOrigRetSym(Symbol* origRet, FnSymbol* parentFn) {
  // parentFn and this assert are just sanity checking for the caller
  INT_ASSERT(origRet->defPoint->parentSymbol == parentFn);

  std::vector<SymExpr*> symExprs;
  collectSymExprs(parentFn, symExprs);
  for_vector(SymExpr, se, symExprs)
    if (se->symbol() == origRet) {
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

//
// Set up anchors, if not already, so we can add reduction-related code
// via refRef->insertBefore() within 'fn'.
//
// "redRef" is short for "reference for reduction".
// redRef1 goes at the beginning of fn, redRef2 at the end.
//
void setupRedRefs(FnSymbol* fn, bool nested, Expr*& redRef1, Expr*& redRef2)
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
void cleanupRedRefs(Expr*& redRef1, Expr*& redRef2) {
  if (!redRef1) return;
  redRef1->remove();
  redRef2->remove();
  redRef1 = redRef2 = NULL;
}

// Is 'type' a Reduce/Scan Op?
// similar to isArrayClass()
bool isReduceOp(Type* type) {
  bool retval = false;

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
// createShadowVarIfNeeded()
//
/*
General Context
---------------

Let's say we are extending the parallel iterator with a variable
that corresponds to a reduce intent. For example, from this:

  iter myIter(param tag == standalone) {
    ... yield 555; ...
  }

to this:

  iter myIter(param tag == standalone, x1_reduceParent:SumReduceScanOp) {
    var x1_shadowVarReduc = x1_reduceParent.identity;
    ... yield (555, addr-of(x1_shadowVarReduc)); ...
    x1_reduceParent.accumulate(x1_shadowVarReduc);
  }

Particular Concern
------------------

If the above yield is actually within a forall:

  forall ... {
    ... yield (555, addr-of(x1_shadowVarReduc)); ...
  }

then we need to create a reduce-intented shadow variable for this
forall, corresponding to x1_shadowVarReduc, and yield it instead:

  forall ... with (x1_reduceParent reduce x1_shadowVarReduc) {
    // here, "x1_shadowVarReduc" is the shadow variable:
    ... yield (555, addr-of(x1_shadowVarReduc)); ...
  }

Look Out For...
---------------

Need to handle these cases:

* The yield is in >1 nested foralls --> create a new shadow var for each:

  iter myIter(param tag == standalone, x1_reduceParent:SumReduceScanOp) {
    var x1_shadowVarReduc = x1_reduceParent.identity;

    forall ... with (x1_reduceParent reduce x1_shadowVarReduc) {
      // shadow var: x1_shadowVarReduc'

      forall ... with (x1_reduceParent reduce x1_shadowVarReduc') {
        // shadow var: x1_shadowVarReduc''

        ... yield (555, addr-of(x1_shadowVarReduc'')); ...
      }
    }

    x1_reduceParent.accumulate(x1_shadowVarReduc);
  }

* Multiple yields within the same forall(s) --> use the same shadow var:

    forall ... with (x1_reduceParent reduce x1_shadowVarReduc) {
      // shadow var: x1_shadowVarReduc'

      ... yield (555, addr-of(x1_shadowVarReduc'')); ...
      ... yield (666, addr-of(x1_shadowVarReduc'')); ...
    }

*/
static Symbol* createShadowVarIfNeeded(ShadowVarSymbol *shadowvar,
                                       Symbol* parentOp, Symbol* svar,
                                       CallExpr* yieldCall)
{
  ForallStmt* efs = enclosingForallStmt(yieldCall);
  if (!efs)
    // the original variable is just fine
    return svar;

  // In the "old" world, we call this only for reductions.
  ForallIntentTag intent = shadowvar ? shadowvar->intent : TFI_REDUCE;

  if (ForallStmt* efs2 = enclosingForallStmt(efs)) {
    // TODO arrange for a chain of shadow variables.
    // This should be straightforward. Presently not implemented.

    switch (intent) {
      case TFI_CONST_IN:
      case TFI_REF:
      case TFI_CONST_REF:
        // For these intents, OK to go without a new shadow variable.
        return svar;

      case TFI_IN:
      case TFI_REDUCE:
        // May result in data races or incorrect behavior
        // if we don't handle multiple enclosing foralls.
        USR_FATAL_CONT(yieldCall, "a parallel iterator with a yield nested in two or more enclosing forall statements is not currently implemented in the presence of an 'in' or 'reduce' intent.");
        USR_PRINT(efs, "the immediately enclosing forall statement is here");
        USR_PRINT(efs2, "the next enclosing forall statement is here");
        // If we continue, we may get a const violation on x*_shadowVarReduc.
        USR_STOP();
        break;

      case TFI_DEFAULT:
      case TFI_CONST:
        // These cannot be 'in' intents AFAIK. Rule them out just in case.
        INT_ASSERT(false);   // don't give me an abstract intent
        break;
    }

    return svar; // dummy
  }

  // Check whether we  have already processed another yield within this forall
  // and created a shadow variable. If so, reuse it.
  // TODO need to look up in outer enclosing forall statements, if applicable.
  for_shadow_vars(efsShadVar, temp, efs) // linear search, for simplicity
    if (efsShadVar->outerVarSym() == svar)
      return efsShadVar;

  // Need to create a new shadow variable.
  SET_LINENO(svar); // or efs?

  Expr* spec = NULL;
  if (intent == TFI_REDUCE) {

    // For a reduce intent, we need a new reduce op.
    // If we reuse 'parentOp', it will accumulate all the values twice.
    VarSymbol* cloneOp = new VarSymbol("fsCloneOp");
    efs->insertBefore(new DefExpr(cloneOp));
    efs->insertBefore("'move'(%S,clone(%S,%S))", cloneOp, gMethodToken, parentOp);
    efs->insertAfter("chpl__delete(%S)", cloneOp);
    spec = new SymExpr(cloneOp);
  }

  // The new shadow variable for 'svar' at 'efs'.
  ShadowVarSymbol* result = new ShadowVarSymbol(intent, svar->name,
                                                new SymExpr(svar), spec);
  efs->shadowVariables().insertAtTail(new DefExpr(result));

  return result;
}

//
// Since a promotion-wrapper leader (PWL) merely invokes another leader
// in a for loop, all we need for that is to pass x1_reduceParent into
// PWL as a new formal, then pass that formal into the _toLeader call
// within PWL.  We *need* to bypass the machinery related to handling the
// loop index - otherwise an extraneous new x1_svarRef will be generated.
//
void addArgsToToLeaderCallForPromotionWrapper(FnSymbol* fn,
                                                     int numExtraArgs,
                                                     Symbol* extraFormals[])
{
  std::vector<CallExpr*> calls;
  collectCallExprs(fn, calls);
  int toleaderCnt = 0;
  for_vector(CallExpr, call, calls)
    if (call->isNamed("_toLeader") || call->isNamed("_toLeaderZip")) {
      toleaderCnt++;
      for (int ix = 0; ix < numExtraArgs; ix++)
        call->insertAtTail(extraFormals[ix]);
    }
  INT_ASSERT(toleaderCnt == 1);
}

// Is 'call' invoking a parallel iterator?
// Since the call is not resolved, we can't use isLeaderIterator(),
// and this implementation is a heuristic.
bool callingParallelIterator(CallExpr* call) {
  // Check 'call' for an actual argument that's a parallel tag.
  // Todo: handle the case where the actual's value is not known yet.
  for_actuals(actual, call) {

    Expr* nonameActual = actual;
    if (NamedExpr* ne = toNamedExpr(nonameActual)) {
      nonameActual = ne->actual;
    }

    if (SymExpr* se = toSymExpr(nonameActual)) {
      Symbol* tag = se->symbol();
      // a quick check first
      if (tag->type == gLeaderTag->type) {
        if (tag == gLeaderTag ||
            tag == gStandaloneTag ||
            paramMap.get(tag) == gLeaderTag ||
            paramMap.get(tag) == gStandaloneTag)
          // yep, most likely over parallel iterator
          return true;
      }
    }
  }

  return false;
}

// Is 'forLoop' a loop over a parallel iterator?
// If so, return the call to that iterator.
static CallExpr* findCallToParallelIterator(ForLoop* forLoop) {
  Symbol* iterSym = forLoop->iteratorGet()->symbol();

  // Find an assignment to 'iterSym'.
  CallExpr* asgnToIter = NULL;
  for (Expr* curr = forLoop->prev; curr; curr = curr->prev)
    if (CallExpr* call = toCallExpr(curr))
      if (call->isPrimitive(PRIM_MOVE))
        if (SymExpr* lhs1 = toSymExpr(call->get(1)))
          if (lhs1->symbol() == iterSym) {
            asgnToIter = call;
            break;
          }
  INT_ASSERT(asgnToIter);

  // We have:
  //   move( call_tmp call( ITERATOR args... ) )
  //   move( _iterator call( _getIterator call_tmp ) )
  // We need to see if args... contain a leader or standalone tag.
  // 'asgnToIter' is the second of the above moves. Find the first one.
  CallExpr* rhs1 = toCallExpr(asgnToIter->get(2));
  INT_ASSERT(rhs1 && rhs1->isNamed("_getIterator"));
  Symbol* calltemp = toSymExpr(rhs1->get(1))->symbol();
  CallExpr* asgnToCallTemp = NULL;
  for (Expr* curr = asgnToIter->prev; curr; curr = curr->prev)
    if (CallExpr* call = toCallExpr(curr))
      if (call->isPrimitive(PRIM_MOVE))
        if (SymExpr* lhs2 = toSymExpr(call->get(1)))
          if (lhs2->symbol() == calltemp) {
            asgnToCallTemp = call;
            break;
          }

  // Sometimes, e.g. forall over a range - ri-5-c.chpl, _getIterator is invoked
  // on a non-temp. We notice this by the absence of asgnToCallTemp.
  // In such a case, the iterator will be nontemp.these(),
  // which is not a parallel iterator.
  if (!asgnToCallTemp)
    return NULL;

  CallExpr* iterCall = toCallExpr(asgnToCallTemp->get(2));
  INT_ASSERT(iterCall);

  if (callingParallelIterator(iterCall))
    return iterCall;

  // no signs of a parallel iterator
  return NULL;
}

// Is 'rcall' nested in "eflopi" (Enclosing For-Loop Over Parallel Iterator) ?
// If so, return the call to that iterator.
static void eflopiFind(CallExpr* rcall,
                       ForLoop*& eflopiLoop, CallExpr*& eflopiCall)
{
  Expr* curr = rcall;
  INT_ASSERT(!eflopiCall); // caller's responsibility
  while ((curr = curr->parentExpr)) {
    if (ForLoop* forLoop = toForLoop(curr)) {
      // is this a loop over a parallel iterator?
      if (CallExpr* iterCall = findCallToParallelIterator(forLoop)) {
        if (eflopiCall) {
          USR_FATAL_CONT(rcall, "This yield statement is in a parallel iterator that is used in a forall loop with reduce intent(s). It is currently not implemented properly because the yield statement itself is nested in two or more loops over a parallel iterator(s).");
          // Continue like there is no eflopi.
          return;
        }
        eflopiLoop = forLoop;
        eflopiCall = iterCall;
      }
    }
  }
}

//
// In the body of this ForLoop, whose original _indexOfInterest is 'origIOI':
//  * declare a new variable 'newIOI'
//  * set that newIOI as origIOI(1)
//  * replace all previously-existing references to origIOI with newIOI
//
static void redirectToNewIOI(ForLoop* eflopiLoop) {
  Symbol* origIOI   = eflopiLoop->indexGet()->symbol();
  VarSymbol* newIOI = newTemp("origIndexOfInterest");

  SymbolMap ioiMap;
  ioiMap.put(origIOI, newIOI);
  // process only the loop body
  AST_CALL_LIST(eflopiLoop, ForLoop, body, update_symbols, &ioiMap);

  insertExtractFromYieldAtHead(eflopiLoop, 1, newIOI, origIOI);
  eflopiLoop->insertAtHead(new DefExpr(newIOI));
}

// Handle a yield within propagateExtraLeaderArgs().
static void propagateThroughYield(CallExpr* rcall,
                                  FnSymbol* parentFn,
                                  int numExtraArgs,
                                  VarSymbol* retSym,
                                  Symbol* extraActuals[],
                                  Symbol* extraFormals[],
                                  Symbol* shadowVars[],
                                  bool reduceArgs[],
                                  bool nested,
                                  Expr*& redRef1,
                                  Expr*& redRef2)
{
  // Make a tuple that includes the extra args.
  Expr* origRetArg = rcall->get(1)->remove();
  VarSymbol* newOrigRet = localizeYieldForExtendLeader(origRetArg, rcall);
  CallExpr* buildTuple = new CallExpr("_build_tuple_always_allow_ref",
                                      newOrigRet);

  // Check for an "eflopi" (Enclosing For-Loop Over a Parallel Iterator).
  // This is specific to a given yield.
  // Todo: handle multiple yields in a single eflopi.
  bool      eflopiChecked = false;
  ForLoop*  eflopiLoop    = NULL;
  CallExpr* eflopiCall    = NULL;
  int       eflopiIdx     = 1;
  CallExpr* eflopiHelper  = NULL;

  // add tuple components
  for (int ix = 0; ix < numExtraArgs; ix++) {
    bool isReduce = reduceArgs[ix];
    Symbol* svar = shadowVars[ix];
    Symbol* tupleComponent;
    if (isReduce) {
      // Todo: handle eflopi case when !isReduce.
      if (!eflopiChecked) {
        eflopiChecked = true;
        eflopiFind(rcall,
                   // sets these if appropriate:
                   eflopiLoop, eflopiCall);
      }
      Symbol* parentOp = extraFormals[ix];
      // not resolved yet: INT_ASSERT(isReduceOp(extraActuals[ix]->type));

      if (eflopiCall) {
        //
        // Convert the eflopiLoop loop similarly to how a forall is handled:
        //
        // * pass parentOp/currOp to its eflopi's iterator
        //    --> done earlier in propagateExtraLeaderArgs()
        //        the same way as for non-eflopi reduce-intent args
        //
        // * propagate that parentOp/currOp within eflopi's iterator
        //   and have it yield a tuple containing
        //   the corresponding shadow var by reference
        //    --> this is done by calling implementForallIntents2()
        //        as caused by eflopiHelper, eflopiMap
        //
        // * detuple the value yielded by eflopi's iterator
        //   into the original yielded value plus shadow variable(s)
        //   for reduce intents
        //    --> done below
        //
        // * yield the shadow variable as part of rcall's tuple
        //    --> this is done via 'tupleComponent' as in non-eflopi case
        //

        // Detuple the value yielded by eflopi's parallel iterator
        //
        if (eflopiIdx == 1) {
          // do only once for this yield
          redirectToNewIOI(eflopiLoop);
        }
        Symbol* origIOI = eflopiLoop->indexGet()->symbol();
        // If it fails, replace newOrigRet with newIOI in buildTuple.
        INT_ASSERT(newOrigRet != origIOI);

        // Extract and yield the shadow variable reference.
        Symbol* rvar = new VarSymbol(intentArgName(ix, "eflopiRvar"));
        rvar->addFlag(FLAG_REF_VAR);
        eflopiIdx++;
        insertExtractFromYieldAtHead(eflopiLoop, eflopiIdx, rvar, origIOI);
        eflopiLoop->insertAtHead(new DefExpr(rvar));
        tupleComponent = rvar;

        // Cause implementForallIntents2() over eflopi's parallel iterator.
        if (!eflopiHelper)
          eflopiHelper = new CallExpr(PRIM_ACTUALS_LIST, gVoid); // dummy arg
        eflopiHelper->insertAtTail(parentOp);

      } else {
        if (!svar) {
          INT_ASSERT(!nested); // nested case is handled above
          setupRedRefs(parentFn, nested, redRef1, redRef2);
          // Todo: skip these additions if the current 'rcall' yield
          // is going to be compiled away, e.g. if it is
          // within a param conditional on a not-taken branch.
          svar = new VarSymbol(intentArgName(ix, "shadowVarReduc"));
          svar->addFlag(FLAG_INSERT_AUTO_DESTROY);
          VarSymbol* stemp  = newTemp("svrTmp");
          redRef1->insertBefore(new DefExpr(svar));
          redRef1->insertBefore(new DefExpr(stemp));
          redRef1->insertBefore("'move'(%S, identity(%S,%S))",
                                stemp, gMethodToken, parentOp);
          redRef1->insertBefore("'move'(%S, chpl__autoCopy(%S))",
                                svar, stemp);
          redRef2->insertBefore("accumulate(%S,%S,%S)",
                                gMethodToken, parentOp, svar);
          shadowVars[ix] = svar;
        }
        // pass 'svar' by reference
        // If the yield is inside a forall, (create and) pass
        // the corresponding shadow variable instead.
        Symbol* ssvar = createShadowVarIfNeeded(NULL, parentOp, svar, rcall);
        // todo: have a single 'sref' per 'svar', not one for each yield
        VarSymbol* sref = new VarSymbol(intentArgName(ix, "svarRef"));
        rcall->insertBefore(new DefExpr(sref));
        rcall->insertBefore("'move'(%S, 'addr of'(%S))", sref, ssvar);
        tupleComponent = sref;
      }
    } else {
      tupleComponent = extraFormals[ix];
    }
    buildTuple->insertAtTail(new SymExpr(tupleComponent));
  }

  rcall->insertBefore("'move'(%S,%E)", retSym, buildTuple);
  rcall->insertAtTail(new SymExpr(retSym));
  if (eflopiHelper)
    eflopiMap[eflopiCall] = eflopiHelper;
}

static void propagateRecursively(FnSymbol* parentFn,
                                 FnSymbol* currentFn,
                                 int numExtraArgs,
                                 VarSymbol* retSym,
                                 Symbol* extraActuals[],
                                 Symbol* extraFormals[],
                                 Symbol* shadowVars[],
                                 bool reduceArgs[],
                                 bool nested,
                                 Expr*& redRef1,
                                 Expr*& redRef2);

//
// Propagate 'extraActuals' through the task constructs, implementing
// task intents. See the header comment for extendLeader().
//
// * 'call' gets each of extraActuals[ix] added as an actual
// * callee 'fn' gets a newly-created corresponding formal: extraFormals[ix]
//
// Within 'fn' :
// * for each call to a task function: do the above recursively
// * for each yield: convert to yielding (prev yield value, extraFormals[*])
//
// For a reduce-intent position -- when reduceArgs[ix]:
// * extraFormals[ix] is a parentOp - passed to task functions, extra treatment
// * shadowVars[ix] is created - passed to yields, extra treatment
//
static void propagateExtraLeaderArgs(CallExpr* call, VarSymbol* retSym,
                                     int numExtraArgs, Symbol* extraActuals[],
                                     bool reduceArgs[], bool nested)
{
  FnSymbol* fn = call->resolvedFunction();

  INT_ASSERT(fn); // callee's responsibility

  if (fn->hasFlag(FLAG_WRAPPER)) {
    // We are not handling void-returning wrappers at the moment.
    INT_ASSERT(!(fn->getReturnSymbol() == gVoid || fn->retType == dtVoid));
  }

  Expr*   redRef1 = NULL, *redRef2 = NULL;
  Symbol* extraFormals[numExtraArgs];
  Symbol* shadowVars[numExtraArgs];

  for (int ix = 0; ix < numExtraArgs; ix++) {
    Symbol* eActual  = extraActuals[ix];
    bool    isReduce = nested ? reduceArgs[ix] : isReduceOp(eActual->type);

    if (!nested) {
      reduceArgs[ix] = isReduce;
    }

    // Use named args to disambiguate from the already-existing iterator args,
    // just in case. This necessitates toNamedExpr() in handleCaptureArgs().
    const char* eName   =
      isReduce ? intentArgName(ix, "reduceParent") :
        nested ? eActual->name :
          strcmp(eActual->name, "_tuple_expand_tmp_") ?
            intentArgName(ix, eActual->name) // uniquify arg name
            : intentArgName(ix, "tet");

    ArgSymbol*  eFormal = new ArgSymbol(INTENT_BLANK, eName, eActual->type);

    extraFormals[ix] = eFormal;

    call->insertAtTail(new NamedExpr(eName, new SymExpr(eActual)));

    fn->insertFormalAtTail(eFormal);

    // In leader outside any taskFn just use reduceParent.
    // Todo: also skip if there are no other taskFns or yields in 'fn'.
    if (isReduce && nested) {
      // We shouldn't bother with all this when it is not a task function.
      INT_ASSERT(isTaskFun(fn));
      setupRedRefs(fn, nested, redRef1, redRef2);
      ArgSymbol* parentOp = eFormal; // the reduceParent arg
      VarSymbol* currOp   = new VarSymbol(intentArgName(ix, "reduceCurr"));
      VarSymbol* svar     = new VarSymbol(intentArgName(ix, "shadowVar"));
      svar->addFlag(FLAG_INSERT_AUTO_DESTROY);
      VarSymbol* stemp    = newTemp("svTmp");
      redRef1->insertBefore(new DefExpr(currOp));
      redRef1->insertBefore("'move'(%S, clone(%S,%S))", // init
                            currOp, gMethodToken, parentOp);
      redRef1->insertBefore(new DefExpr(svar));
      redRef1->insertBefore(new DefExpr(stemp));
      redRef1->insertBefore("'move'(%S, identity(%S,%S))",
                            stemp, gMethodToken, currOp);
      redRef1->insertBefore("'move'(%S, chpl__autoCopy(%S))",
                            svar, stemp);
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

  if (!nested && fn->hasFlag(FLAG_PROMOTION_WRAPPER)) {
    INT_ASSERT(!redRef1); // no need to clean them up
    addArgsToToLeaderCallForPromotionWrapper(fn, numExtraArgs, extraFormals);
  } else {
    propagateRecursively(fn, fn, numExtraArgs, retSym,
                         extraActuals, extraFormals, shadowVars, reduceArgs,
                         nested, redRef1, redRef2);
    cleanupRedRefs(redRef1, redRef2);
  }
}

// Propagate recursively into task functions and yields.
static void propagateRecursively(FnSymbol* parentFn,
                                 FnSymbol* currentFn,
                                 int numExtraArgs,
                                 VarSymbol* retSym,
                                 Symbol* extraActuals[],
                                 Symbol* extraFormals[],
                                 Symbol* shadowVars[],
                                 bool reduceArgs[],
                                 bool nested,
                                 Expr*& redRef1,
                                 Expr*& redRef2)
{
  std::vector<CallExpr*> rCalls;
  collectMyCallExprs(currentFn, rCalls, currentFn);

  for_vector(CallExpr, rcall, rCalls) {
    if (rcall->isPrimitive(PRIM_YIELD)) {

      propagateThroughYield(rcall, parentFn, numExtraArgs, retSym,
                            extraActuals, extraFormals,
                            shadowVars, reduceArgs,
                            nested, redRef1, redRef2);

    } else if (FnSymbol* tfn = resolvedToTaskFun(rcall)) {
     if (needsCapture(tfn)) {
      // 'rcall' better be the only call to 'tfn'.
      // The following assert is a weak assurance of that.
      // For a strong assurance, we could additionally build a set of task
      // functions, calls to which we have seen.
      // OTOH our normal call verification should suffice: it will fail
      // the first propagated call if a second call propagates to same tfn.
      INT_ASSERT(tfn->defPoint->parentSymbol == currentFn);

      if (tfn->hasFlag(FLAG_BEGIN)) {
        // (A) Reduce intents do not make sense when a 'begin' outlives
        //     the iterator. There used to be a check for that here.
        // (B) However, currently there can be no yields in a 'begin'
        //     - see checkControlFlow().
        //     Without a yield, the above is not a concern.
        // (C) Generally, if a task function does not have a yield,
        //     there is nothing to be done w.r.t. forall intents.
        //     We could check the entire tfn for yields. Instead,
        //     we just do a fast check for FLAG_BEGIN
        //     as a conservative approximation.
        //     We verify the absence of yields, however.
        if (fVerify) {  // for assertions only
          std::vector<CallExpr*> bCalls;
          collectMyCallExprs(tfn, bCalls, tfn);
          for_vector(CallExpr, bcall, bCalls)
            INT_ASSERT(!bcall->isPrimitive(PRIM_YIELD));
        }
      } else {
        // Propagate the extra args recursively into 'tfn'.
        propagateExtraLeaderArgs(rcall, retSym, numExtraArgs,
                                 extraFormals, reduceArgs, true);
      }
     } else {
      // !needsCapture(tfn) => descend into 'tfn' without argument intents.
      propagateRecursively(parentFn, tfn, numExtraArgs, retSym,
                           extraActuals, extraFormals, shadowVars, reduceArgs,
                           nested, redRef1, redRef2);
     }
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
static void extendLeader(CallExpr* call, CallExpr* origToLeaderCall,
                         FnSymbol* origIterFn)
{
  // Replace the callee with a clone.
  if (origIterFn->isResolved()) {
    // ... using a pristine copy if the callee is already resolved.
    // See the comment on an assert in copyLeaderFn().
    origIterFn = pristineLeaderIterators.get(origIterFn);
    INT_ASSERT(origIterFn);
  }

  FnSymbol* iterFn = copyLeaderFn(origIterFn, /*ignore_isResolved:*/false);
  iterFn->instantiationPoint = getVisibilityBlock(call);
  call->baseExpr->replace(new SymExpr(iterFn));

  // Setup the new return/yield symbol.
  VarSymbol* retSym  = NULL;
  Symbol* origRetSym = NULL;

  if (!iterFn->hasFlag(FLAG_PROMOTION_WRAPPER)) {
    retSym  = newTemp("ret"); // its type is to be inferred
    origRetSym = iterFn->replaceReturnSymbol(retSym, /*newRetType*/NULL);
    origRetSym->defPoint->insertBefore(new DefExpr(retSym));
    origRetSym->name = "origRet";
  }

  int numExtraArgs = origToLeaderCall->numActuals()-1;
  INT_ASSERT(numExtraArgs > 0); // we shouldn't be doing all this otherwise
  Expr* origArg = origToLeaderCall->get(1);
  Symbol* extraActuals[numExtraArgs];
  bool    reduceArgs[numExtraArgs];   // computed in propagateExtraLeaderArgs
  for (int ix = 0; ix < numExtraArgs; ix++) {
    origArg = origArg->next;
    SymExpr* origSE = toSymExpr(origArg);
    INT_ASSERT(origSE); // if it is not a symbol, still need to make it happen
    extraActuals[ix] = origSE->symbol();
  }
  INT_ASSERT(!origArg->next); // we should have processed all args

  propagateExtraLeaderArgs(call, retSym, numExtraArgs,
                           extraActuals, reduceArgs, false);

  if (origRetSym) {
    checkAndRemoveOrigRetSym(origRetSym, iterFn);
  }
}

void implementForallIntents2(CallExpr* call, CallExpr* origToLeaderCall) {
  FnSymbol* origLeader = call->resolvedFunction();
  INT_ASSERT(origLeader);  // caller responsibility

  if (origToLeaderCall->numActuals() <= 1) {
    // No variables to propagate => no extendLeader.
    // Ensure we have a pristine copy for the other case.
    if (!pristineLeaderIterators.get(origLeader)) {
      stashPristineCopyOfLeaderIter(origLeader, /*ignore_isResolved:*/ false);
    }
  } else {
    if (strncmp(origLeader->name, "_iterator_for_loopexpr", 22) == 0) {
      propagateExtraArgsForLoopIter(call, origToLeaderCall, origLeader);
    } else {
      extendLeader(call, origToLeaderCall, origLeader);
    }
  }
}

// Heuristically find the wrapped call.
// Keep in sync with unresolveWrapper() below.
static CallExpr* findWrappedCall(FnSymbol* wrapper) {
  CallExpr* ret = toCallExpr(wrapper->body->body.tail);
  INT_ASSERT(ret->isPrimitive(PRIM_RETURN));
  // We want the third-last statement.
  CallExpr* move = toCallExpr(ret->prev->prev);
  INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
  INT_ASSERT(!strcmp(toSymExpr(move->get(1))->symbol()->name, "wrap_call_tmp"));
  CallExpr* wCall = toCallExpr(move->get(2));
  INT_ASSERT(wCall);
  return wCall;
}

// Check the expectations we had when we called findCallToParallelIterator()
// and computed eflopiHelper.
static void ifi2checkAssumptions(FnSymbol* dest) {
  INT_ASSERT(dest->hasFlag(FLAG_ITERATOR_FN));
  INT_ASSERT(isLeaderIterator(dest) || isStandaloneIterator(dest));
}

// Cause 'wrapper' to be resolved anew.
static void unresolveWrapper(FnSymbol* wrapper) {
  wrapper->removeFlag(FLAG_RESOLVED);

  // like findWrappedCall() above
  CallExpr* retCall = toCallExpr(wrapper->body->body.tail);
  INT_ASSERT(retCall && retCall->isPrimitive(PRIM_RETURN));
  SymExpr* retSE = toSymExpr(retCall->get(1));
  INT_ASSERT(retSE && !strcmp(retSE->symbol()->name, "ret"));
  CallExpr* move = toCallExpr(retCall->prev);
  INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
  INT_ASSERT(toSymExpr(move->get(1))->symbol() == retSE->symbol());
  SymExpr* wrapSE = toSymExpr(move->get(2));
  INT_ASSERT(wrapSE && !strcmp(wrapSE->symbol()->name, "wrap_call_tmp"));

  // the type of these needs to be resolved anew
  retSE->symbol()->type = wrapSE->symbol()->type = dtUnknown;
}

// Handle the wrapper if applicable.
void implementForallIntents2wrapper(CallExpr* call, CallExpr* eflopiHelper)
{
  FnSymbol* dest = call->resolvedFunction();

  if (!dest->hasFlag(FLAG_WRAPPER)) {
    // the simple case
    ifi2checkAssumptions(dest);

    implementForallIntents2(call, eflopiHelper);

  } else {
    // the wrapper case

    // Do not modify the original wrapper because it is probably in a cache
    // and may be reused for an unrelated call. Create a clone.
    FnSymbol* wDest = dest->copy();
    wDest->addFlag(FLAG_INVISIBLE_FN);
    wDest->instantiationPoint = getVisibilityBlock(call);
    // Do we also need to update paramMap like in copyLeaderFn() ?
    dest->defPoint->insertAfter(new DefExpr(wDest));
    call->baseExpr->replace(new SymExpr(wDest));

    // invoke ifi2 on the wrapped call
    CallExpr* wCall = findWrappedCall(wDest);

    Expr* savedLastArg = wCall->argList.tail;
    // these counts are only for assertions
    int savedNumArgsC = call->numActuals();
    int savedNumArgsW = wCall->numActuals();
    int numExtraArgs  = eflopiHelper->numActuals()-1;

    ifi2checkAssumptions(wCall->resolvedFunction());

    implementForallIntents2(wCall, eflopiHelper);

    INT_ASSERT(wCall->numActuals() == savedNumArgsW + numExtraArgs);

    // propagate the additions to the original call
    bool goahead = false;
    // Recall that 'wCall' is in the wrapper 'wDest', calling the iterator.
    for_formals_actuals(formal, actual, wCall) {
      if (!goahead) {
        // skip pre-existing args
        if (actual == savedLastArg)
          goahead = true;
        continue;
      }

      ArgSymbol* wFormal = formal->copy();
      // If these fail, figure out the intent. See also copyFormalForWrapper().
      INT_ASSERT(isClass(formal->type));
      INT_ASSERT(formal->intent == INTENT_BLANK); // or 'const' or 'in'-like

      wDest->insertFormalAtTail(wFormal);
      actual->replace(new SymExpr(wFormal));
      call->insertAtTail(actual);
    }

    INT_ASSERT(call->numActuals()  == savedNumArgsC + numExtraArgs);
    INT_ASSERT(wCall->numActuals() == savedNumArgsW + numExtraArgs);

    // We cloned the wrapper 'dest' into 'wDest'.  wDest's call 'wCall'
    // now invokes a clone of the iterator that 'dest' was invoking.
    // The above code modifies both 'wCall' and the iterator that it invokes.
    // Alas, 'wCall' inherits FLAG_RESOLVED and its return type from 'dest'
    // and these are no longer appropriate due to these modifications.
    //
    // So 'wDest' needs to be resolved again. To make that happen,
    // we un-resolve its relevant pieces.
    //
    if (wDest->isResolved()) {
      unresolveWrapper(wDest);
    }
  }
}


//
//-----------------------------------------------------------------------------
//  implementForallIntentsNew()
//-----------------------------------------------------------------------------
//
// The counterparts of implementForallIntents1() and implementForallIntents2()
// for the (new) ForallStmt-based representation.
//

/////////////////////////////////////////////////////////////////////////////
// ForallIntentTag <-> IntentTag

IntentTag argIntentForForallIntent(ForallIntentTag tfi) {
  switch (tfi) {
    case TFI_DEFAULT:   return INTENT_BLANK;
    case TFI_CONST:     return INTENT_CONST;
    case TFI_IN:        return INTENT_IN;
    case TFI_CONST_IN:  return INTENT_CONST_IN;
    case TFI_REF:       return INTENT_REF;
    case TFI_CONST_REF: return INTENT_CONST_REF;
    case TFI_REDUCE:
      INT_ASSERT(false);    // don't know what to return
      return INTENT_BLANK;  // dummy
  }
  INT_ASSERT(false);    // unexpected ForallIntentTag; 'tfi' contains garbage?
  return INTENT_BLANK;  // dummy
}

static ForallIntentTag forallIntentForArgIntent(IntentTag intent) {
  switch (intent) {
    case INTENT_IN:        return TFI_IN;
    case INTENT_CONST:     return TFI_CONST;
    case INTENT_CONST_IN:  return TFI_CONST_IN;
    case INTENT_REF:       return TFI_REF;
    case INTENT_CONST_REF: return TFI_CONST_REF;
    case INTENT_BLANK:     return TFI_DEFAULT;
    case INTENT_REF_MAYBE_CONST: return TFI_REF; //todo: TFI_REF_MAYBE_CONST ?
    default:
      INT_ASSERT(false);   // don't know what to return
      return TFI_DEFAULT;  // dummy
  }
  INT_ASSERT(false);   // unexpected IntentTag; 'intent' contains garbage?
  return TFI_DEFAULT;  // dummy
}

static void resolveSVarIntent(ShadowVarSymbol* svar) {
  switch (svar->intent) {
    case TFI_DEFAULT:
      svar->intent = forallIntentForArgIntent(
                       concreteIntent(INTENT_BLANK, svar->type->getValType()));
      break;
    case TFI_CONST:
      svar->intent = forallIntentForArgIntent(
                       concreteIntent(INTENT_CONST, svar->type->getValType()));
      break;
    case TFI_IN:
    case TFI_CONST_IN:
    case TFI_REF:
    case TFI_CONST_REF:
    case TFI_REDUCE:
      // nothing to do
      break;
  }
}

/////////////////////////////////////////////////////////////////////////////
// findOuterVarsNew() and helpers
//

// Is 'sym' defined outside 'block'?
static bool isOuterVarNew(Symbol* sym, BlockStmt* block) {
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

static bool isFsIndexVar(ForallStmt* fs, Symbol* sym)
{
  if (!sym->hasFlag(FLAG_INDEX_VAR))
    return false;

  return sym->defPoint->list == &fs->inductionVariables();
}

static bool isFsShadowVar(ForallStmt* fs, Symbol* sym)
{
  if (!isShadowVarSymbol(sym))
    return false;

  return sym->defPoint->list == &fs->shadowVariables();
}

//
// A forall-intents variation on findOuterVars() in createTaskFunctions.cpp:
// Find all symbols used in 'block' and defined outside of it.
//
static void findOuterVarsNew(ForallStmt* fs, SymbolMap& outer2shadow,
                             BlockStmt* block)
{
  std::vector<SymExpr*> symExprs;
  collectSymExprs(block, symExprs);

  for_vector(SymExpr, symExpr, symExprs) {
    Symbol* sym = symExpr->symbol();

    if (isLcnSymbol(sym)             && // include only variable-like things
        sym->type != dtMethodToken   && // not a method token
        sym->defPoint->parentSymbol != rootModule && // not a system symbol
        !sym->isParameter()          && // not a param, including isImmediate()
        !sym->hasFlag(FLAG_INSTANTIATED_PARAM) && // not a param, again
        !sym->hasFlag(FLAG_TYPE_VARIABLE)      && // not a type alias or formal
        !sym->hasFlag(FLAG_TEMP)     && // not a temp
        !isFsIndexVar(fs, sym)       && // not fs's index var
        !isFsShadowVar(fs, sym)      && // not fs's intent var
        !sym->hasFlag(FLAG_ARG_THIS) && // todo: no special case for 'this'
        isOuterVarNew(sym, block)       // it must be an outer variable
    ) {
      // if not there already
      if (!outer2shadow.get(sym)) {
        // OK, add it
        ShadowVarSymbol* ss = new ShadowVarSymbol(TFI_DEFAULT, sym->name,
                                                  new SymExpr(sym));
        outer2shadow.put(sym, ss);
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// setup for a reduce intent
//

//
// Incorporate the initial value of the outer variable:
//   globalOp.initialAccumulate(outerVar)
// If this does not resolve, instead call:
//   globalOp.accumulate(outerVar)
//
static void insertAndResolveInitialAccumulate(ForallStmt* fs, BlockStmt* hld,
                                            Symbol* globalOp, Symbol* outerVar)
{
  CallExpr* initAccum = new CallExpr("initialAccumulate", gMethodToken,
                                     globalOp, outerVar);
  if (hld)
    hld->insertAtTail(initAccum);
  else
    fs->insertBefore(initAccum);

  FnSymbol* initAccumOutcome = tryResolveCall(initAccum);

  if (initAccumOutcome) {
    // Yes, use initialAccumulate().

  } else {
    // No, call accumulate() instead.
    CallExpr* accum = new CallExpr("accumulate", gMethodToken,
                                   globalOp, outerVar);
    initAccum->replace(accum);
    initAccum = accum;
    initAccumOutcome = tryResolveCall(initAccum);

    if (initAccumOutcome) {
      // OK, will use accumulate();

    } else {
      // The user must provide at least one of the two.
      USR_FATAL_CONT(initAccum, "the reduction operation of the reduce intent for the variable %s does not provide initialAccumulate() or accumulate() that accepts the initial value of this variable", outerVar->name);

      // We can continue compilation meanwhile.
      initAccum->remove();
      return;
    }
  }

  resolveFnForCall(initAccumOutcome, initAccum);
}

// Finalize the reduction:  outerVar = globalOp.generate()
static void insertFinalGenerate(Expr* ref, Symbol* fiVarSym, Symbol* globalOp) {
  Expr* next = ref->next; // nicer ordering of the following insertions
  INT_ASSERT(next);
  VarSymbol* genTemp = newTemp("chpl_gentemp");
  next->insertBefore(new DefExpr(genTemp));
  next->insertBefore("'move'(%S, generate(%S,%S))",
                     genTemp, gMethodToken, globalOp);
  next->insertBefore(new CallExpr("=", fiVarSym, genTemp));
}

static Symbol* setupRiGlobalOp(ForallStmt* fs, Symbol* fiVarSym,
                               Expr* origRiSpec, TypeSymbol* riTypeSym,
                               Expr* eltTypeArg)
{
  BlockStmt* hld = new BlockStmt(); // "holder"
  fs->insertBefore(hld);
      
  VarSymbol* globalOp =  newTemp("chpl__reduceGlob");
  hld->insertAtTail(new DefExpr(globalOp));

  if (!eltTypeArg) {
    Type* eltType   = fiVarSym->qualType().type()->getValType();
    // fiVar should have been resolved already
    INT_ASSERT(eltType != dtUnknown && eltType != dtAny);

    if (eltType->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
      // arg = typeof(fiVar)
      VarSymbol* typeofTemp = newTemp("chpl__typeofET");
      typeofTemp->addFlag(FLAG_TYPE_VARIABLE);
      hld->insertAtTail(new DefExpr(typeofTemp));
      hld->insertAtTail("'move'(%S,'typeof'(%S))", typeofTemp, fiVarSym);
      eltTypeArg = new SymExpr(typeofTemp);
    } else {
      // Forego typeof(), use the type directly.
      eltTypeArg = new SymExpr(eltType->symbol);
    }
  }

  hld->insertAtTail("'move'(%S, 'new'(%S,%E))", globalOp, riTypeSym,
                    new NamedExpr("eltType", eltTypeArg));

  fs->insertAfter("chpl__delete(%S)", globalOp);
  insertFinalGenerate(fs, fiVarSym, globalOp);

  resolveBlockStmt(hld);
  insertAndResolveInitialAccumulate(fs, hld, globalOp, fiVarSym);
  hld->flattenAndRemove();

  // Todo: this replace() is somewhat expensive.
  // Can instead we update fs->riSpecs[i] aka 'riSpec' in-place?
  origRiSpec->replace(new SymExpr(globalOp));
  
  return globalOp;
}

static void handleRISpec(ForallStmt* fs, ShadowVarSymbol* svar)
{
  if (svar->reduceGlobalOp != NULL)
    // Already handled, ex. in tupcomForYieldInForall().
    return;

  Symbol* globalOp = NULL;
  Symbol* fiVarSym = svar->outerVarSym();
  Expr*   riSpec   = svar->reduceOpExpr();
  SET_LINENO(riSpec);

  if (SymExpr* riSE = toSymExpr(riSpec)) {
    Symbol* riSym = riSE->symbol();

    if (TypeSymbol* riTypeSym = toTypeSymbol(riSym)) {
      globalOp = setupRiGlobalOp(fs, fiVarSym, riSpec, riTypeSym, NULL);

    } else {
      INT_ASSERT(isLcnSymbol(riSym)); // what else can a globalOp be??

      insertAndResolveInitialAccumulate(fs, NULL, riSym, fiVarSym);

      // The user will manage allocation and deallocation of 'riSym'.
      // So we only need to add a call generate().
      insertFinalGenerate(fs, fiVarSym, riSym);
      globalOp = riSym;
    }

  } else if (CallExpr* riSpecCall = toCallExpr(riSpec)) {
    SymExpr* riTypeSE = toSymExpr(riSpecCall->baseExpr);
    INT_ASSERT(riTypeSE); // need to handle baseExpr anyway
    TypeSymbol* riTypeSym = toTypeSymbol(riTypeSE->symbol());
    INT_ASSERT(riTypeSym); // non-types should be handled elsewhere

    if (riSpecCall->numActuals() != 1)
      USR_FATAL(riSpecCall, "for a reduce intent, the 'reduce' keyword must be preceded by the reduction operator or the name of the reduction class with the single optional argument indicating the type of the reduction input");

    riTypeSE->remove();
    Expr* eltTypeArg = riSpecCall->get(1)->remove();
    globalOp = setupRiGlobalOp(fs, fiVarSym, riSpec, riTypeSym, eltTypeArg);

  } else {
    // What else can this be?
    INT_FATAL(fs, "not implemented");
  }

  svar->reduceGlobalOp = globalOp;
}

/////////////////////////////////////////////////////////////////////////////

static void getOuterVarsNew(ForallStmt* fs, SymbolMap& outer2shadow,
                            BlockStmt* body)
{
  if (fs->needToHandleOuterVars())
    // do the same as in 'if (needsCapture(fn))' in createTaskFunctions()
    findOuterVarsNew(fs, outer2shadow, body);

  for_shadow_vars(sv, temp, fs)
    if (sv->isReduce())
      handleRISpec(fs, sv);
}

// Append the new ShadowVarSymbols we accumulated to fs->shadowVariables().
static void appendNewShadowVars(ForallStmt* fs, SymbolMap& outer2shadow) {
  form_Map(SymbolMapElem, elem, outer2shadow)
    fs->shadowVariables().insertAtTail(new DefExpr(elem->value));
}

// Not to be invoked upon a reduce intent.
// All our shadow variables are expected to be refs.
static void setShadowVarFlagsNew(Symbol* ovar, ShadowVarSymbol* svar, IntentTag intent) {
  // These do not make sense for task/forall intents.
  INT_ASSERT(!(intent & (INTENT_FLAG_PARAM | INTENT_FLAG_TYPE)));
  // If this assert fails, we need to handle this case.
  INT_ASSERT(!(intent & INTENT_FLAG_OUT));

  if (intent & INTENT_FLAG_CONST) {
    svar->addFlag(FLAG_CONST);
    svar->addFlag(FLAG_REF_VAR);
    svar->qual = QUAL_CONST_REF;
    if (intent == INTENT_CONST_IN)
      // Enables canForwardValue(), ex.
      //   release/examples/benchmarks/hpcc/fft.chpl
      svar->addFlag(FLAG_REF_TO_IMMUTABLE);
    if (!ovar->isConstant())
      svar->addFlag(FLAG_CONST_DUE_TO_TASK_FORALL_INTENT);
  } else {
    svar->addFlag(FLAG_REF_VAR);
    svar->qual = QUAL_REF;
  }
}

static void processShadowVarsNew(ForallStmt* fs, BlockStmt* body, int& numShadowVars)
{
  // todo: prune right away, get rid of 'numShadowVars'

  for_shadow_vars(svar, temp, fs) {
    if (svar->isReduce())
    {
      Symbol* ovar = svar->outerVarSym();
      if (ovar && ovar->hasFlag(FLAG_CONST))
        USR_FATAL_CONT(fs,
          "reduce intent is applied to a 'const' variable %s", ovar->name);

      // The shadow variable will assume the reference from the leadIdx tuple.
      svar->addFlag(FLAG_REF_VAR);
      svar->qual = QUAL_REF;
      svar->type = dtUnknown;
    }
    else
    {
      Symbol* ovar = svar->outerVarSym();
      svar->type = ovar->type->getRefType();
      resolveSVarIntent(svar);

      // If ovar is a reference, e.g. an index variable of
      // a 'var' iterator, we do not want to force
      // that ref type onto 'svar'. Otherwise the generated
      // code will store into *svar without initializing svar
      // first. Todo: what if ovar is a domain?
      Type* valtype = ovar->type->getValType();

      IntentTag  tiIntent =
        concreteIntent(argIntentForForallIntent(svar->intent), valtype);

      // See if we want to prune it.
      bool pruneit = false;

      if (tiIntent == INTENT_REF || tiIntent == INTENT_REF_MAYBE_CONST) {
        // do we want this? does this lead to more efficient generated code?
         pruneit = true;

      } else if (isAtomicType(ovar->type)) {
        // Currently we need it because sync variables do not get tupled
        // and detupled properly when threading through the leader iterator.
        // See e.g. test/distributions/dm/s7.chpl
        // Atomic vars might not work either.
        // And anyway, only 'ref' intent makes sense here.
        pruneit = true;

        USR_WARN(fs, "an atomic var currently can be passed into a forall loop by 'ref' intent only - %s is ignored for '%s'", intentDescrString(tiIntent), ovar->name);
      }

      if (pruneit) {
        // Todo: remove it from fs->shadowVariables() right away.
        svar->pruneit = true;
        continue; // for_shadow_vars
      }

      setShadowVarFlagsNew(ovar, svar, tiIntent);
    }
    numShadowVars++;
  }
}

/////////////////////////////////////////////////////////////////////////////

//
// Undo the outerVar-to-shadowVar conversion inside 'body'
// for those ShadowVarSymbols that are marked with 'pruneit'.
//
// Ideally, we won't prune, so won't need to do this.
//
static void pruneShadowVars(ForallStmt* fs, BlockStmt* body,
                            SymbolMap& outer2shadow, int numInitialVars,
                            int numShadowVars, bool& needToReplace)
{
  INT_ASSERT(fs->numShadowVars() > numShadowVars); // can be ==; shouldn't be <

  // There are two pieces to undo-ing a given shadow variable:
  //  (a) replace its references within the loop 'body' with its outer variable,
  //  (b) remove its DefExpr.
  //
  // For (a): given that we have not yet performed the outer-to-shadow
  // conversion within the loop body for the variables in 'outer2shadow',
  // no need to undo them there. So we look only at the initial variables
  // in fs->shadowVariables() before 'outer2shadow' kicked in.
  //
  // While there, we keep track of whether there is anything un-pruned left
  // in outer2shadow using numToReplace.

  int idx = 0;
  bool needToRevert = false;
  int  numToReplace = fs->numShadowVars() - numInitialVars;

  for_shadow_var_defs(svd, temp, fs) {
    ++idx;
    if (ShadowVarSymbol* svs = toShadowVarSymbol(svd->sym)) {
      if (svs->pruneit) {
        svd->remove();
        if (idx <= numInitialVars) {
          needToRevert = true;
        } else {
          SymbolMapElem* elem = outer2shadow.get_record(svs->outerVarSym());
          INT_ASSERT(elem);
          elem->value = markPruned;
          numToReplace--;
        }
      }
    }
  }

  if (needToRevert) {
    std::vector<SymExpr*> symExprs;
    collectSymExprs(body, symExprs);
    for_vector(SymExpr, se, symExprs)
      if (ShadowVarSymbol* svar = toShadowVarSymbol(se->symbol()))
        if (svar->pruneit)
          se->setSymbol(svar->outerVarSym());
  }

  // otherwise ensure there is nothing to replace
  if (fVerify && !needToRevert) {
    std::vector<SymExpr*> symExprs;
    collectSymExprs(body, symExprs);
    for_vector(SymExpr, se, symExprs)
      if (ShadowVarSymbol* svar = toShadowVarSymbol(se->symbol()))
        INT_ASSERT(!svar->pruneit);
  }

  if (numToReplace > 0)
    needToReplace = true;
}

/////////////////////////////////////////////////////////////////////////////

static void addActualsToParCallNew(ForallStmt* fs, CallExpr* parCall)
{
  Expr* parStmt = parCall->getStmtExpr();

  for_shadow_vars(svar, temp, fs)
  {
    Symbol* ovar = svar->outerVarSym();
    Symbol* globalOp = svar->reduceGlobalOp;
    Symbol* actual = NULL;

    // Pass 'ovar' into the parallel iterator.
    //
    // The intent of the corresponding iterator formal is set appropriately
    // in extendLeaderNew() / propagateExtraLeaderArgsNew().

    switch (svar->intent)
    {
      // by reference
      case TFI_REF:
      case TFI_CONST_REF:
        actual = ovar;
        break;

      // by value
      case TFI_IN:
      case TFI_CONST_IN:
        if (ovar->isRef()) {
          // If it is a reference, dereference it. E.g. m-lsms.chpl (-nl 1?)
          // or test/parallel/forall/vass/intents-all-int.chpl.
          VarSymbol* deref = newTemp(ovar->name, ovar->type->getValType());
          parStmt->insertBefore(new DefExpr(deref));
          parStmt->insertBefore("'move'(%S, 'deref'(%S))", deref, ovar);
          actual = deref;
        } else {
          actual = ovar;
        }
        break;

      case TFI_REDUCE:
        actual = globalOp;
        break;

      case TFI_DEFAULT:
      case TFI_CONST:
        // These should not appear here because all intents must be concrete
        // by now. An abstract intent would not let us distinguish between
        // by-ref and by-val, which we need for adjustments done above.
        INT_ASSERT(false);
        break;
    }

    parCall->insertAtTail(actual);
  }
}

static void extractFromLeaderYieldNew(Expr* ref, int ix,
                                   Symbol* dest, Symbol* leadIdx) {
  insertExtractFromYield(ref, ix, dest, leadIdx);
}

static void detupleLeadIdxNew(ForallStmt* fs,
                           Symbol* leadIdxSym, Symbol* leadIdxCopySym,
                           Expr* ref, int numLeaderActuals)
{
  int ix = 0;

  // first, leadIdxCopy
  if (numLeaderActuals > 0)
    extractFromLeaderYieldNew(ref, ++ix, leadIdxCopySym, leadIdxSym);

  for_shadow_vars(svar, temp, fs)
    extractFromLeaderYieldNew(ref, ++ix, svar, leadIdxSym);
}

static void detupleLeadIdxNew(ForallStmt* fs, int numLeaderActuals)
{
  VarSymbol* leadIdx     = parIdxVar(fs);
  VarSymbol* leadIdxCopy = parIdxCopyVar(fs);
  BlockStmt* fbody       = userLoop(fs);

  if (numLeaderActuals == 0)
    fbody->insertBefore("'move'(%S, %S)", leadIdxCopy, leadIdx);

  detupleLeadIdxNew(fs, leadIdx, leadIdxCopy, fbody, numLeaderActuals);
}

// Same as replaceVarUses() in createTaskFunctions.
static void replaceVarUsesNew(BlockStmt* body, SymbolMap& outer2shadow) {
  std::vector<SymExpr*> symExprs;
  collectSymExprs(body, symExprs);

  form_Map(SymbolMapElem, e, outer2shadow) {
    if (e->value == markPruned)
      continue;
    Symbol* oldSym = e->key;
    Symbol* newSym = e->value;
    INT_ASSERT(!toShadowVarSymbol(newSym)->pruneit);
    for_vector(SymExpr, se, symExprs)
      if (se->symbol() == oldSym)
        se->setSymbol(newSym);
  }
}

static void addParIdxCopy(ForallStmt* fs) {
  VarSymbol* parIdx     = parIdxVar(fs);
  VarSymbol* parIdxCopy = parIdxCopyVar(fs);
  parIdxCopy->defPoint->insertAfter("'move'(%S,%S)", parIdxCopy, parIdx);
}

/////////////////////////////////////////////////////////////////////////////

static void implementForallIntents1New(ForallStmt* fs, CallExpr* parCall) {
  SymbolMap            outer2shadow;
  BlockStmt*           forallBody1 = userLoop(fs);
  int                  numShadowVars = 0;
  bool                 needToReplace = false;
  SET_LINENO(forallBody1);

  getOuterVarsNew(fs, outer2shadow, forallBody1);

  // At this point, fs->shadowVariables() and outer2shadow are disjoint sets.
  //
  // (A) fs->shadowVariables() correspond to the explicit intents
  // in the with-clause. The occurrences of those variables in the loop body
  // scopeResolve to the corresponding ShadowVarSymbols. getOuterVarsNew()
  // does not perceive them as "outer".
  //
  // (B) The variables in outer2shadow are the ones mentioned in the loop body
  // and not yet converted to shadow variables.
  //
  // appendNewShadowVars() adds the (B) vars to the (A) set.
  // 'outer2shadow' stays unchanged.
  // Save the size of (A) before the addition.
  int numInitialVars = (fs->shadowVariables()).length;
  appendNewShadowVars(fs, outer2shadow);

  processShadowVarsNew(fs, forallBody1, numShadowVars); // updates numShadowVars

  if (fs->numShadowVars() == numShadowVars)
    needToReplace = (outer2shadow.n > 0);
  else
    pruneShadowVars(fs, forallBody1, outer2shadow, numInitialVars,
                    numShadowVars, needToReplace); // updates needToReplace

  if (fs->numShadowVars() == 0)
  {
    addParIdxCopy(fs);
  }
  else
  {
    addActualsToParCallNew(fs, parCall);
    detupleLeadIdxNew(fs, fs->numShadowVars());
    if (needToReplace)
      replaceVarUsesNew(forallBody1, outer2shadow);
  }
}

/////////////////////////////////////////////////////////////////////////////

// implementForallIntentsNew() -- based on "new" ForallStmt representation

static void checkForNonIterator(CallExpr* parCall) {
  FnSymbol* dest = parCall->resolvedFunction();
  AggregateType* retType = toAggregateType(dest->retType);
  if (!retType || !retType->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
    USR_FATAL_CONT(parCall, "The iterable-expression resolves to a non-iterator function '%s' when looking for a parallel iterator", dest->name);
    USR_PRINT(dest, "The function '%s' is declared here", dest->name);
    USR_STOP();
  }
}

//
// Performs both implementForallIntents1 and implementForallIntents2.
// 'parCall' must have already been resolved.
//
void implementForallIntentsNew(ForallStmt* fs, CallExpr* parCall)
{
  INT_ASSERT(parCall == fs->firstIteratedExpr());

  implementForallIntents1New(fs, parCall);

  checkForNonIterator(parCall);

  if (fs->numShadowVars() > 0)
   implementForallIntents2New(fs, parCall);
}
