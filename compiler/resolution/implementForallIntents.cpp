/*
 * Copyright 2004-2014 Cray Inc.
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

#include "stlUtil.h"
#include "stringutil.h"
#include "stmt.h"
#include "symbol.h"
#include "astutil.h"
#include "passes.h"
#include "resolution.h"


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


//
// A forall-intents variation on findOuterVars() in createTaskFunctions.cpp:
// Find all symbols used in 'block' and defined outside of it.
//
static void findOuterVars(BlockStmt* block, SymbolMap* uses) {
  std::vector<SymExpr*> symExprs;
  collectSymExprsSTL(block, symExprs);
  for_vector(SymExpr, symExpr, symExprs) {
    Symbol* sym = symExpr->var;
    if (toVarSymbol(sym) || toArgSymbol(sym))
      if (!isCorrespIndexVar(block, sym) && isOuterVar(sym, block))
        uses->put(sym,gNil);
  }
}


//
// Create a new var, for use as a shadow var in the body of the forall loop,
// for each un-pruned variable in 'uses'.
// Add the original var to 'outerVars' and the new var to 'shadowVars'
// so an ordered traversal of the two lists gives matching pairs.
// Count the number of these variables into 'numOuterVars'.
//
static void createShadowVars(SymbolMap* uses, int& numOuterVars,
                             std::vector<Symbol*>& outerVars,
                             std::vector<Symbol*>& shadowVars)
{
  numOuterVars = 0;

  form_Map(SymbolMapElem, e, *uses) {
    INT_ASSERT(e->key); // todo: if holds, remove 'if' throughout
    if (e->value != markPruned) {
      Symbol* ovar = e->key;
      // If ovar is a reference, e.g. an index variable of
      // a 'var' iterator, we do not want to force
      // that ref type onto 'svar'. Otherwise the generated
      // code will store into *svar without initializing svar
      // first. Todo: what if ovar is a domain?
      Symbol* svar = new VarSymbol(ovar->name, ovar->type->getValType());
      svar->addFlag(FLAG_CONST); // todo: replace with arg intents
      outerVars.push_back(ovar);
      shadowVars.push_back(svar);
      INT_ASSERT(e->value == gNil); // meaning e->value is not set, yet
      e->value = svar;
      numOuterVars++;
    }
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


// ForallLeaderArgs: add actuals to _toLeader/Zip()
// (The leader will be converted to accept them during resolution.)
static void addActualsTo_toLeader(Symbol* serIterSym,
                                  std::vector<Symbol*>& outerVars)
{
  // Find that call to _toLeader or _toLeaderZip,
  // starting from DefExpr of chpl__iter...
  CallExpr* tlCall = NULL;
  for (Expr* seekExpr = serIterSym->defPoint->next; seekExpr;
       seekExpr = seekExpr->next)
    if (CallExpr* seekCall = toCallExpr(seekExpr))
      if (seekCall->isPrimitive(PRIM_MOVE))
        if (CallExpr* seekSubCall = toCallExpr(seekCall->get(2)))
          if (seekSubCall->isNamed("_toLeader") ||
              seekSubCall->isNamed("_toLeaderZip")) {
            tlCall = seekSubCall;
            break;
          }
  INT_ASSERT(tlCall);
  Expr* tlStmt = tlCall->getStmtExpr();
  // .. and add the actuals.
  for_vector(Symbol, ovar, outerVars) {
    Symbol* actual = ovar;
    // If it is a reference, dereference it. E.g. lsms-parallel-n1.chpl.
    if (isReferenceType(ovar->type)) {
      VarSymbol* deref = newTemp(ovar->name, ovar->type->getValType());
      tlStmt->insertBefore(new DefExpr(deref));
      tlStmt->insertBefore(new_Expr("'move'(%S, 'deref'(%S))", deref, ovar));
      actual = deref;
    }
    tlCall->insertAtTail(actual);
  }
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

  lcCall->insertBefore(new_Expr("'move'(%S, '.v'(%S,%S))",
                                dest, leadIdx, new_StringSymbol(buf)));
}


static void detupleLeadIdx(Symbol* leadIdxSym, Symbol* leadIdxCopySym,
                           std::vector<Symbol*>& shadowVars)
{
  // ForallLeaderArgs: detuple what the leader yields.
  // (The leader will be converted to yield a tuple during resolution.)
  // Find the assignment leadIdxCopy:=leadIdx,
  // starting from DefExpr of chpl__leadIdxCopy.
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

  // ... and add the detupling.
  // First, for leadIdxCopy.
  int ix = 1;
  extractFromLeaderYield(lcCall, ix, leadIdxCopySym, leadIdxSym);

  // Then, for the shadow vars.
  for_vector(Symbol, svar, shadowVars) {
    lcCall->insertBefore(new DefExpr(svar));
    extractFromLeaderYield(lcCall, ++ix, svar, leadIdxSym);
    svar->addFlag(FLAG_INSERT_AUTO_DESTROY);
  }

  // Finally, remove the original assignment.
  lcCall->remove();
}

//
// A forall-intents variation on replaceVarUsesWithFormals() in
// createTaskFunctions.cpp:
//   fn          --> block
//   ArgSymbol   --> symbol
//   if (e->key) --> INT_ASSERT
//
static void
replaceVarUsesWithFormals(Expr* block, SymbolMap* vars) {
  if (vars->n == 0) return;
  std::vector<SymExpr*> symExprs;
  collectSymExprsSTL(block, symExprs);
  form_Map(SymbolMapElem, e, *vars) {
    Symbol* sym = e->key;
    INT_ASSERT(sym); // todo: if this succeeds, remove such 'if' elsewhere
    if (e->value != markPruned) {
      SET_LINENO(sym);
      Symbol* arg = e->value;
      for_vector(SymExpr, se, symExprs) {
        if (se->var == sym) {
          se->var = arg;
        }
      }
    }
  }
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
  BlockStmt* bFollowerLoop = NULL;

  for (Expr* curr = defFollowIter->next; curr; curr = curr->next)
    if (BlockStmt* nestB = toBlockStmt(curr))
      if (nestB->isForLoop())
        if (SymExpr* seFollowIter = toSymExpr(nestB->blockInfoGet()->get(2)))
          if (seFollowIter->var == defFollowIter->sym) {
            bFollowerLoop = nestB;
            break;
          }

  INT_ASSERT(bFollowerLoop);  // ensure we found it

  // find the forall loop body
  BlockStmt* bForallBody = NULL;
  for (Expr* curr = bFollowerLoop->body.head; curr; curr = curr->next)
    if (BlockStmt* nestB = toBlockStmt(curr))
      if (nestB->byrefVars) {
        // what else can it be?
        INT_ASSERT(nestB->byrefVars->isPrimitive(PRIM_FORALL_LOOP));
        bForallBody = nestB;
        break;
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
  findBlockWithDefOf(defChplIter->next, "chpl__leadIdxCopy",
                     b1, defLeadIdxCopy);

  if (fNoFastFollowers) {
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

static void getOuterVars(BlockStmt* body, Symbol*& serIterSym,
                         Symbol*& leadIdxSym, Symbol*& leadIdxCopySym,
                         SymbolMap*& uses)
{
  CallExpr* const byrefVars = body->byrefVars;
  INT_ASSERT(byrefVars->isPrimitive(PRIM_FORALL_LOOP));

  // Extract  chpl__iter,  chpl__leadIdx,  chpl__leadIdxCopy.
  SymExpr* serIterSE     = toSymExpr(byrefVars->get(1)->remove());
  SymExpr* leadIdxSE     = toSymExpr(byrefVars->get(1)->remove());
  SymExpr* leadIdxCopySE = toSymExpr(byrefVars->get(1)->remove());
  INT_ASSERT(serIterSE && leadIdxSE && leadIdxCopySE);

  serIterSym     = serIterSE->var;
  leadIdxSym     = leadIdxSE->var;
  leadIdxCopySym = leadIdxCopySE->var;

  // do the same as in 'if (needsCapture(fn))' below
  uses = new SymbolMap();
  findOuterVars(body, uses);
  pruneOuterVars(uses, byrefVars);
  pruneThisArg(body->parentSymbol, uses, true);
}

static void verifyOuterVars(BlockStmt* body2, Symbol* serIterSym1,
                            Symbol* leadIdxSym1, Symbol* leadIdxCopySym1,
                            SymbolMap* uses1, int numOuterVars1)
{
  Symbol *serIterSym2, *leadIdxSym2, *leadIdxCopySym2;
  SymbolMap* uses2;
  getOuterVars(body2, serIterSym2, leadIdxSym2, leadIdxCopySym2, uses2);

  INT_ASSERT(serIterSym1     == serIterSym2);
  INT_ASSERT(leadIdxSym1     == leadIdxSym2);
  INT_ASSERT(leadIdxCopySym1 == leadIdxCopySym2);

  int numOuterVars2 = 0;
  form_Map(SymbolMapElem, e, *uses2) {
    if (e->value != markPruned) {
      numOuterVars2++;
      Symbol* var2 = e->key;
      SymbolMapElem* elem1 = uses1->get_record(var2);
      INT_ASSERT(elem1 && elem1->key == var2);
    }
  }
  INT_ASSERT(numOuterVars1 == numOuterVars2);
}

//
// This is Part 1 of implementing forall intents, executed earlier than Part 2:
// * discover the relevant AST pieces of a forall loop
// * compute outer variables
// * does some AST transformations
//
void implementForallIntents1(DefExpr* defChplIter)
{
  // Find the corresponding forall loop body,
  // or two bodies if fast followers are enabled.
  BlockStmt* forallBody1;
  BlockStmt* forallBody2;
  discoverForallBodies(defChplIter, forallBody1, forallBody2);

  // If both bodies are present, I expect them to be copies of one another.
  // So we discover everything for the first one and verify
  // that it's the same for the second one.
  // I think swapping the first and the second in the below
  // would be equivalent, when both are present.

  // Once we found it/them, process the first forall body clone.
  // Stash away misc things for comparison with the second one.

  Symbol *serIterSym1, *leadIdxSym1, *leadIdxCopySym1;
  SymbolMap* uses1;

  getOuterVars(forallBody1, serIterSym1, leadIdxSym1, leadIdxCopySym1, uses1);

  // Create shadow variables. We keep the vectors to ensure consistent
  // iteration order in for_vector(outerVars) vs. for_vector(shadowVars).
  // Todo: initialize the vectors with capacity=uses->n, i.e. max #vars.
  std::vector<Symbol*> outerVars;
  std::vector<Symbol*> shadowVars;
  int numOuterVars1;
  SET_LINENO(forallBody1);
  createShadowVars(uses1, numOuterVars1, outerVars, shadowVars);

  if (numOuterVars1 > 0) {

    checkForRecordsWithArrayFields(defChplIter, outerVars);

    addActualsTo_toLeader(serIterSym1, outerVars);

    detupleLeadIdx(leadIdxSym1, leadIdxCopySym1, shadowVars);

    // replace outer vars with shadows in the loop body
    replaceVarUsesWithFormals(forallBody1, uses1);
  }

  forallBody1->byrefVars->remove();

  // Now process the second clone, using what we already computed.

  if (forallBody2)
  {
    // for assertions only
    if (fVerify)
      verifyOuterVars(forallBody2, serIterSym1, leadIdxSym1, leadIdxCopySym1,
                      uses1, numOuterVars1);

    // Threading through the leader call was done for forallBody1.

    if (numOuterVars1 > 0)
      // same outer variables, same shadow variables as for forallBody1
      replaceVarUsesWithFormals(forallBody2, uses1);

    forallBody2->byrefVars->remove();
  }

  // todo: do we need to deallocate 'uses'?
}


//
//-----------------------------------------------------------------------------
//  implementForallIntents2()
//-----------------------------------------------------------------------------
//
// This is Part 2 - it processes each call to _toLeader() or _toLeaderZip().
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
      if (call->isPrimitive(PRIM_MOVE))
        if (SymExpr* dest = toSymExpr(call->get(1)))
          if (dest->var == origRetSym) {
            VarSymbol* newOrigRet = newTemp("localRet", origRetSym->type);
            curr->insertBefore(new DefExpr(newOrigRet));
            dest->var = newOrigRet;
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
  collectSymExprsSTL(parentFn, symExprs);
  for_vector(SymExpr, se, symExprs)
    INT_ASSERT(se->var != origRet);

  // If none are found, we can yank origRet.
  origRet->defPoint->remove();
}

//
// Propagate 'extraActuals' through the task constructs, implementing
// task intents. See the header comment for extendLeader().
//
static void propagateExtraLeaderArgs(CallExpr* call, VarSymbol* retSym,
                                     int numExtraArgs, Symbol* extraActuals[],
                                     bool nested)
{
  FnSymbol* fn = call->isResolved();
  INT_ASSERT(fn); // callee's responsibility
  if (fn->hasFlag(FLAG_WRAPPER)) {
    // We are not handling void-returning wrappers at the moment.
    INT_ASSERT(!(fn->getReturnSymbol() == gVoid || fn->retType == dtVoid));
  }

  Symbol* extraFormals[numExtraArgs];
  for (int ix = 0; ix < numExtraArgs; ix++) {
    Symbol*     eActual = extraActuals[ix];
    // Use named args to disambiguate from the already-existing iterator args,
    // just in case. This necessitates toNamedExpr() in handleCaptureArgs().
    const char* eName   = nested ? eActual->name :
      strcmp(eActual->name, "_tuple_expand_tmp_")
      ? astr("_x", istr(ix+1), "_", eActual->name) // uniquify arg name
      : astr("_x", istr(ix+1), "_tet");
  
    ArgSymbol*  eFormal = new ArgSymbol(INTENT_BLANK, eName, eActual->type);
    extraFormals[ix] = eFormal;
    call->insertAtTail(new NamedExpr(eName, new SymExpr(eActual)));
    fn->insertFormalAtTail(eFormal);
  }

  // Propagate recursively into task functions and yields.
  std::vector<CallExpr*> rCalls;
  collectMyCallExprsSTL(fn, rCalls, fn);
  for_vector(CallExpr, rcall, rCalls) {
    if (rcall->isPrimitive(PRIM_YIELD)) {
      // Make a tuple that includes the extra args.
      Expr* origRetArg = rcall->get(1)->remove();
      VarSymbol* newOrigRet = localizeYieldForExtendLeader(origRetArg, rcall);
      CallExpr* buildTuple = new CallExpr("_build_tuple", newOrigRet);
      for (int ix = 0; ix < numExtraArgs; ix++)
        buildTuple->insertAtTail(new SymExpr(extraFormals[ix]));
      rcall->insertBefore(new_Expr("'move'(%S,%E)", retSym, buildTuple));
      rcall->insertAtTail(new SymExpr(retSym));

    } else if (FnSymbol* tfn = resolvedToTaskFun(rcall)) {
      // 'rcall' better be the only call to 'tfn'.
      // The following assert is a weak assurance of that.
      // For a strong assurance, we could additionally build a set of task
      // functions, calls to which we have seen.
      // OTOH our normal call verification should suffice: it will fail
      // the first propagated call if a second call propagates to same tfn.
      INT_ASSERT(tfn->defPoint->parentSymbol == fn);
      // Propagate the extra args recursively into 'tfn'.
      propagateExtraLeaderArgs(rcall, retSym, numExtraArgs, extraFormals, true);
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

  int numExtraArgs = origToLeaderCall->numActuals()-1;
  INT_ASSERT(numExtraArgs > 0); // we shouldn't be doing all this otherwise
  Expr* origArg = origToLeaderCall->get(1);
  Symbol* extraActuals[numExtraArgs];
  for (int ix = 0; ix < numExtraArgs; ix++) {
    origArg = origArg->next;
    SymExpr* origSE = toSymExpr(origArg);
    INT_ASSERT(origSE); // if it is not a symbol, still need to make it happen
    extraActuals[ix] = origSE->var;
  }

  // Setup the new return/yield symbol.
  VarSymbol* retSym  = newTemp("ret"); // its type is to be inferred
  Symbol* origRetSym = iterFn->replaceReturnSymbol(retSym, /*newRetType*/NULL);
  origRetSym->defPoint->insertBefore(new DefExpr(retSym));
  origRetSym->name = "origRet";

  propagateExtraLeaderArgs(call, retSym, numExtraArgs, extraActuals, false);

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
