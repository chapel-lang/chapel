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

#include "foralls.h"

#include "astutil.h"
#include "AstVisitor.h"
#include "DeferStmt.h"
#include "driver.h"
#include "ForLoop.h"
#include "ForallStmt.h"
#include "iterator.h"
#include "passes.h"
#include "resolution.h"
#include "resolveFunction.h"
#include "stlUtil.h"
#include "stringutil.h"

const char* forallIntentTagDescription(ForallIntentTag tfiTag) {
  switch (tfiTag) {
    case TFI_DEFAULT:   return "default";
    case TFI_CONST:     return "const";
    case TFI_IN:        return "in";
    case TFI_CONST_IN:  return "const in";
    case TFI_REF:       return "ref";
    case TFI_CONST_REF: return "const ref";
    case TFI_REDUCE:    return "reduce";
  }
  INT_ASSERT(false);
  return "";
}


/////////////////////////////////
// ForallIntents
/////////////////////////////////

// constructor
ForallIntents::ForallIntents() :
  iterRec(NULL),
  leadIdx(NULL),
  leadIdxCopy(NULL)
{}

ForallIntents* ForallIntents::copy(SymbolMap* map, bool internal) {
  // If this fails, see localMap and update_symbols() in DECLARE_COPY().
  // NB we can't DECLARE_COPY(ForallIntents) because it has _this->astloc.
  INT_ASSERT(map && internal);

  // Start would-be copyInner().
  ForallIntents* _this = new ForallIntents();

  // alas std::vector does not have a constructor that "reserves" slots
  int nv = numVars();
  _this->fiVars.reserve(nv);
  _this->fIntents.reserve(nv);
  _this->riSpecs.reserve(nv);

  for (int i = 0; i < nv; i++) {
    _this->fiVars  .push_back(COPY_INT(fiVars[i]));
    _this->fIntents.push_back(         fIntents[i]);
    _this->riSpecs .push_back(COPY_INT(riSpecs[i]));
  }

  _this->iterRec     = COPY_INT(iterRec);
  _this->leadIdx     = COPY_INT(leadIdx);
  _this->leadIdxCopy = COPY_INT(leadIdxCopy);

  // Finish would-be copyInner().
  // No update_symbols() because !internal is false.

  return _this;
}

//
// Returns true if the child has been found (and replaced).
//
bool ForallIntents::replaceChildFI(Expr* oldAst, Expr* newAst) {
  INT_ASSERT(oldAst != NULL);
#define UPDATE(dest, newNode) \
    if ((dest) == oldAst) { (dest) = newNode; return true; }

  for (std::vector<Expr*>::iterator itv = fiVars.begin();
       itv != fiVars.end(); itv++)
    UPDATE(*itv, newAst);
    
  for (std::vector<Expr*>::iterator itr = riSpecs.begin();
       itr != riSpecs.end(); itr++)
    UPDATE(*itr, newAst);

  // Does this ever get invoked with newAst==NULL?
  // If so, the 'if' should read "if (newSE || !newAst)".
  INT_ASSERT(newAst);
  if (SymExpr* newSE = toSymExpr(newAst)) {
    UPDATE(iterRec,     newSE);
    UPDATE(leadIdx,     newSE);
    UPDATE(leadIdxCopy, newSE);
  }
  
  // nothing matched
#undef UPDATE
  return false;
}

// This is intended to mimick Expr::remove(), without 'this' being an Expr.
void ForallIntents::removeFI(Expr* parentB) {
  // If this fails need to use trace_remove() instead of remove_help()
  // - see Expr::remove().
  INT_ASSERT(parentB->parentSymbol);

  // "Remove" all ASTs that 'this' contains.
#define REMOVE(dest) if (dest) remove_help(dest, 'r')

  for_vector(Expr, var, fiVars) REMOVE(var);
  for_riSpecs_vector(ri, this)  REMOVE(ri);
  REMOVE(iterRec);
  REMOVE(leadIdx);
  REMOVE(leadIdxCopy);

#undef REMOVE    
}

void ForallIntents::verifyFI(Expr* parentE) const {
  int nv = numVars();
  INT_ASSERT((int)(fiVars.size())   == nv);
  INT_ASSERT((int)(fIntents.size()) == nv);
  INT_ASSERT((int)(riSpecs.size())  == nv);

  for (int i = 0; i < nv; i++) {
    Expr* fiVar = fiVars[i];
    if (SymExpr* fiVarSE = toSymExpr(fiVar)) {
      INT_ASSERT(isVarSymbol(fiVarSE->symbol()) ||
                 isArgSymbol(fiVarSE->symbol()));  // no modules, fns, etc.
    } else {
      // fiVars[i] is either resolved or unresolved sym expr; never NULL.
      INT_ASSERT(isUnresolvedSymExpr(fiVar));
      // These should be resolved during scopeResolve.
      INT_ASSERT(!normalized);
    }
    verifyNotOnList(fiVar);
    INT_ASSERT(fiVar->parentExpr == parentE);

    Expr* ri = riSpecs[i];
    INT_ASSERT(isReduce(i) == !!ri);
    if (ri) {
      // ri can be UnresolvedSymExpr, SymExpr, CallExpr, ... (?)
      verifyNotOnList(ri);
      INT_ASSERT(ri->parentExpr == parentE);
    }
  }

  INT_ASSERT(!iterRec || iterRec->parentExpr == parentE);
  INT_ASSERT(!leadIdx || leadIdx->parentExpr == parentE);
  INT_ASSERT(!leadIdxCopy || leadIdxCopy->parentExpr == parentE);
  verifyNotOnList(iterRec);
  verifyNotOnList(leadIdx);
  verifyNotOnList(leadIdxCopy);

  // ForallIntents are gone during resolve().
  INT_ASSERT(!resolved);
}

void ForallIntents::acceptFI(AstVisitor* visitor) {
  visitor->visitForallIntents(this);
}


//
// parser support
// These functions report a user error for an unexpected intent.
//

static ForallIntentTag argIntentToForallIntent(Expr* ref, IntentTag intent) {
  switch (intent) {
  case INTENT_IN:        return TFI_IN;
  case INTENT_CONST:     return TFI_CONST;
  case INTENT_CONST_IN:  return TFI_CONST_IN;
  case INTENT_REF:       return TFI_REF;
  case INTENT_CONST_REF: return TFI_CONST_REF;
  case INTENT_BLANK:     return TFI_DEFAULT;

  case INTENT_OUT:
  case INTENT_INOUT:
  case INTENT_PARAM:
  case INTENT_TYPE:
  case INTENT_REF_MAYBE_CONST:
    USR_FATAL_CONT(ref, "%s is not supported in a 'with' clause", intentDescrString(intent));
    return TFI_DEFAULT;
  }
  INT_ASSERT(false); // unexpected IntentTag; 'intent' contains garbage?
  return TFI_DEFAULT; // dummy
}

ShadowVarSymbol* ShadowVarSymbol::buildFromArgIntent(IntentTag intent,
                                                     Expr* ovar)
{
  ForallIntentTag  fi = argIntentToForallIntent(ovar, intent);
  const char*    name = toUnresolvedSymExpr(ovar)->unresolved;
  return new ShadowVarSymbol(fi, name, ovar);
}

ShadowVarSymbol* ShadowVarSymbol::buildFromReduceIntent(Expr* ovar,
                                                        Expr* riExpr)
{
  INT_ASSERT(riExpr != NULL);
  const char* name = toUnresolvedSymExpr(ovar)->unresolved;
  return new ShadowVarSymbol(TFI_REDUCE, name, ovar, riExpr);
}

void addForallIntent(ForallIntents* fi, Expr* var, IntentTag intent, Expr* ri) {
  ForallIntentTag tfi = ri ? TFI_REDUCE : argIntentToForallIntent(var, intent);
  fi->fiVars.push_back(var);
  fi->fIntents.push_back(tfi);
  fi->riSpecs.push_back(ri);
}

void addForallIntent(CallExpr* call, ShadowVarSymbol* svar) {
  call->insertAtTail(new DefExpr(svar));
}

//
// Returns true if 'ast' is directly under 'fi'.
// BTW in this case fi's enclosing BlockStmt is ast's parentExpr.
// BTW we could use AST_CALL_STDVEC/AST_CALL_CHILD for a "directly or
//  indirectly under fi" check, although we'd need extra effort
//  to make them return a bool and abort early if the answer is known.
//
bool astUnderFI(const Expr* ast, ForallIntents* fi) {
  if (!fi) return false;

#define CHECK(arg) { if (((Expr*)(arg)) == ast) return true; }

  for_vector(Expr, var, fi->fiVars) CHECK(var);
  for_riSpecs_vector(ri, fi)        CHECK(ri);
  CHECK(fi->iterRec);
  CHECK(fi->leadIdx);
  CHECK(fi->leadIdxCopy);

#undef CHECK
  // none found
  return false;
}  

/////////////////////////////////
//                             //
//    ForallStmt resolution    //
//                             //
/////////////////////////////////

// resolveForallHeader() resolves key parts of ForallStmt:
//
//  * find the target parallel iterator (standalone or leader) and resolve it
//  * issue an error, if neither is found
//  * handle forall intents, using implementForallIntentsNew()
//  * partly lower by building leader+follow loop(s) as needed
//
// This happens when resolveExpr() encounters the first iterated expression
// of the ForallStmt, because:
//
//  * The iterated expressions need special treatment - adding these(),
//    adding tag=iterKind.standalone/leader/follower, etc.
//    Plain resolving is not appropriate here.
//
//  * Other things in a ForallStmt need to be set up ahead of being resolved.
//    This is a convenient time to do so.
//
// Some transformations include:
//
//  * build leader+follow loop(s) as needed
//  * iteratedExpressions() alist is left with only 1 element,
//    which is the call to the parallel iterator (standalone or leader)
//  * follower iterator(s), if needed, are invoked from within the leader loop
//  * all inductionVariables()' DefExprs are moved to the original loop body

/////////// fsIterYieldType ///////////
static QualifiedType fsIterYieldType(ForallStmt* fs, FnSymbol* iterFn,
                                     FnSymbol* origIterFn,
                                     bool alreadyResolved);

/*
When we are dealing with a recursive parallel iterator, we call
fsIterYieldType() while resolving it. At that time, IteratorInfo
has not been created yet. Also the yield type is not yet available
anywhere, because it is the type of a tuple of the original return type
plus one component per shadow variable.

Ex. standalone iter walkdirs() in FileSystem module, as tested by:
  test/library/standard/FileSystem/filerator/bradc/findfiles-par.chpl

Therefore we compute this extended yield type manually.
*/
static QualifiedType buildIterYieldType(ForallStmt* fs, FnSymbol* iterFn, FnSymbol* origIterFn) {
  if (fs->numShadowVars() == 0) {
    // The iterator has not undergone extendLeader().
    // It still yields whatever the user wrote.
    // Its return symbol is still in tact.

    QualifiedType result(iterFn->getReturnSymbol()->type);
    // What is the right qualifier?
    return result;
  }

  // Otherwise, build the tuple mocking what iterFn yields, supposedly.
  CallExpr* constup = new CallExpr("_type_construct__tuple");

  // Yield type must have been declared by user for recursive iterator,
  // and the function that declaration is attached to is origIterFn.
  bool alreadyResolved = origIterFn->isResolved();
  QualifiedType origQt = fsIterYieldType(fs, origIterFn, NULL, alreadyResolved);
  Type* origYieldedType = origQt.type();

  INT_ASSERT(origYieldedType && origYieldedType != dtUnknown);
  constup->insertAtTail(origYieldedType->symbol);

  // The other tuple components are refs to shadow variables,
  // so their types come from respective outer variables.
  for_shadow_vars(svar, temp, fs) {
    Symbol* ovar = NULL;
    switch (svar->intent) {
      case TFI_DEFAULT:
      case TFI_CONST:
      case TFI_IN:
      case TFI_CONST_IN:
      case TFI_REF:
      case TFI_CONST_REF:
        ovar = svar->outerVarSym();
        break;

      case TFI_REDUCE:
        // ... except for reduce intents - they are TODO.
        USR_FATAL_CONT(svar, "Reduce intents are currently not implemented"
          " for forall- or for-loops over recursive parallel iterators");
        USR_PRINT(iterFn, "the parallel iterator is here");
        USR_STOP();
        break;
    }
    INT_ASSERT(ovar != NULL);
    INT_ASSERT(ovar->type != dtUnknown);

    constup->insertAtTail(ovar->type->getRefType()->symbol);
  }

  int numComponents = constup->numActuals();
  constup->insertAtHead(new_IntSymbol(numComponents));

  // Resolve it now.
  fs->insertBefore(constup);
  resolveCall(constup);
  constup->remove();

  // What is the right qualifier?
  QualifiedType result(constup->qualType().type());
  return result;
}


//
// Given an iterator function, find the type that it yields.
// It would be fn->retType, alas protoIteratorClass() messes with that.
// This helper is ForallStmt-specific because of the assumptions it makes.
//
static QualifiedType fsIterYieldType(ForallStmt* fs, FnSymbol* iterFn,
                                     FnSymbol* origIterFn,
                                     bool alreadyResolved)
{
  if (iterFn->isIterator()) {
    if (IteratorInfo* ii = iterFn->iteratorInfo) {
      return ii->getValue->getReturnQualType();
    } else {
      // We are in the midst of resolving a recursive iterator.
      INT_ASSERT(alreadyResolved);
      return buildIterYieldType(fs, iterFn, origIterFn);
    }
  } else {
    // e.g. "proc these() return _value.these();"
    AggregateType* retType = toAggregateType(iterFn->retType);
    INT_ASSERT(retType && retType->symbol->hasFlag(FLAG_ITERATOR_RECORD));
    FnSymbol* iterator = retType->iteratorInfo->iterator;
    INT_ASSERT(iterator->isIterator()); // no more recursion?
    return fsIterYieldType(fs, iterator, origIterFn, alreadyResolved);
  }
}

// Return true if 'inner' is inside 'outer' following up the 'parent' chain.
// Return 'false' when 'stop' is reached.
// Caller responsibility: either 'outer' or 'stop' is always reached.
// (The stop prevents traversing long up-chains when the ForallStmt
// is deeply nested.)
static bool isContainedInWithStop(Expr* inner, Expr* outer, Expr* stop) {
  Expr* cur = inner->parentExpr;
  do {
    if (cur == outer) return true;
    if (cur == stop) return false;
    cur = cur->parentExpr;
  } while (cur);

  INT_ASSERT(false); // reached neither 'outer' nor 'stop'
  return false; // dummy
}

// Like in build.cpp, here for ForallStmt.
static BlockStmt*
buildFollowLoop(VarSymbol* iter,
                VarSymbol* leadIdxCopy,
                VarSymbol* followIter,
                VarSymbol* followIdx,
                BlockStmt* loopBody,
                Expr*      ref,
                bool       fast,
                bool       zippered) {
  BlockStmt* followBlock = new BlockStmt();
  ForLoop*   followBody  = new ForLoop(followIdx, followIter, loopBody, zippered, /*forall*/ false);

  // not needed:
  //destructureIndices(followBody, indices, new SymExpr(followIdx), false);

  followBlock->insertAtTail(new DefExpr(followIter));

  if (fast) {

    if (zippered) {
      followBlock->insertAtTail("'move'(%S, _getIteratorZip(_toFastFollowerZip(%S, %S)))", followIter, iter, leadIdxCopy);
    } else {
      followBlock->insertAtTail("'move'(%S, _getIterator(_toFastFollower(%S, %S)))",       followIter, iter, leadIdxCopy);
    }
  } else {

    if (zippered) {
      followBlock->insertAtTail("'move'(%S, _getIteratorZip(_toFollowerZip(%S, %S)))",     followIter, iter, leadIdxCopy);
    } else {
      followBlock->insertAtTail("'move'(%S, _getIterator(_toFollower(%S, %S)))",           followIter, iter, leadIdxCopy);
    }
  }
  followBlock->insertAtTail(new DeferStmt(new CallExpr("_freeIterator", followIter)));

  ref->insertAfter(followBlock); // otherwise it wouldn't normalize
  normalize(followBlock);
  followBlock->remove();

  followBlock->insertAtTail(new DefExpr(followIdx));

  followBlock->insertAtTail("{TYPE 'move'(%S, iteratorIndex(%S)) }", followIdx, followIter);

  followBlock->insertAtTail(followBody);

  return followBlock;
}

// Return NULL if the def is not found or is uncertain.
static Expr* findDefOfTemp(SymExpr* origSE)
{
  Symbol* origSym = origSE->symbol();
  if (!origSym->hasFlag(FLAG_TEMP)) return NULL;  // only temps

  SymExpr* otherSE = origSym->getSingleDef();

  if (CallExpr* def = toCallExpr(otherSE->parentExpr))
    if (def->isPrimitive(PRIM_MOVE))
      if (otherSE == def->get(1))
        return def->get(2);

  // uncertain situation
  return NULL;
}

static void removeOrigIterCall(SymExpr* origSE)
{
  INT_ASSERT(!origSE->inTree());

  Symbol* origSym = origSE->symbol();
  INT_ASSERT(origSym->hasFlag(FLAG_TEMP));

  SymExpr* otherSE = origSym->firstSymExpr();
  // Only one SE for this symbol
  INT_ASSERT(origSym->lastSymExpr() == otherSE);
  
  if (CallExpr* def = toCallExpr(otherSE->parentExpr))
    if (def->isPrimitive(PRIM_MOVE))
      if (otherSE == def->get(1)) {
        // Remove origSym - except caller will remove origSE.
        def->remove();
        origSym->defPoint->remove();
        return;
      }

  // The above did not succeed.
  INT_ASSERT(false);
}

// Replaces 'origSE' in the tree with the result.
static CallExpr* buildForallParIterCall(ForallStmt* pfs, SymExpr* origSE)
{
  CallExpr* iterCall = NULL;

  if (origSE->symbol()->type->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
    // Our iterable expression is an iterator call.

    CallExpr* origIterCall = toCallExpr(findDefOfTemp(origSE));
    // What to do if we do not find it?
    INT_ASSERT(origIterCall);

    FnSymbol* origTarget = origIterCall->resolvedFunction();
    INT_ASSERT(origTarget);

    const char* targetName = origTarget->name;
    if (!strncmp(targetName, "_parloopexpr", 12))
      // a forall loop over a (possibly zippered) forall expression, ex.:
      //  test/reductions/deitz/test_maxloc_reduce_wmikanik_bug2.chpl
      targetName = astr("_iterator_for_loopexpr", targetName + 12);

    iterCall = origIterCall->copy();
    iterCall->baseExpr = new UnresolvedSymExpr(targetName);

  } else {
    // Not an iterator call, so add a call to these().
    iterCall = new CallExpr("these", gMethodToken, origSE->copy());
  }

  origSE->replace(iterCall);

  return iterCall;
}

static void checkForExplicitTagArgs(CallExpr* iterCall) {
  int cnt = 0;
  for_actuals(actual, iterCall) {
    ++cnt;
    if ((actual->qualType().type()->getValType() == gStandaloneTag->type) ||
        (isNamedExpr(actual) && toNamedExpr(actual)->name == astrTag)
    ) {
      USR_FATAL_CONT(iterCall, "user invocation of a parallel iterator should not supply tag arguments -- they are added implicitly by the compiler");
      USR_PRINT(iterCall, "actual argument %d of the iterator call", cnt);
      USR_STOP();
    }
  }
}

static bool acceptUnmodifiedIterCall(ForallStmt* pfs, CallExpr* iterCall)
{
  return pfs->iterCallAlreadyTagged();
}

static bool findStandaloneOrLeader(ForallStmt* pfs, CallExpr* iterCall)
{
  bool gotParallel = false;
  bool gotSA = true;

  checkForExplicitTagArgs(iterCall);

  // We are starting with a serial-iterator call.
  // Transform it to a standalone/leader call.
  NamedExpr* tag = new NamedExpr("tag", new SymExpr(gStandaloneTag));
  iterCall->insertAtTail(tag);

  // try standalone
  if (!pfs->zippered()) {
    gotParallel = tryResolveCall(iterCall);
  }

  // try leader
  if (!gotParallel) {
    gotSA = false;
    tag->actual->replace(new SymExpr(gLeaderTag));
    gotParallel = tryResolveCall(iterCall);
  }

  if (!gotParallel) {
    // Cannot USR_FATAL_CONT in general: e.g. if these() is not found,
    // we do not know the type of the index variable.
    // Without which we cannot typecheck the loop body.
    if (iterCall->isNamed("these") && isTypeExpr(iterCall->get(2)))
      USR_FATAL(iterCall, "unable to iterate over type '%s'", toString(iterCall->get(2)->getValType()));
    else
      USR_FATAL(iterCall, "A%s leader iterator is not found for the iterable expression in this forall loop", pfs->zippered() ? "" : " standalone or");
  }

  return gotSA;
}

static void addParIdxVarsAndRestruct(ForallStmt* fs, bool gotSA) {
  // First, save away the user loop as its own BlockStmt.
  // Make it the last thing in the new fs->loopBody().
  BlockStmt* userLoopBody = fs->loopBody();
  BlockStmt* newLoopBody = new BlockStmt();
  userLoopBody->replace(newLoopBody);
  newLoopBody->insertAtTail(userLoopBody);

  // Now, add parIdx*.
  //
  // These are the original buildForallLoopStmt(), with "lead" -> "par".
  // We could retain the original names i.e. gotSA ? saIdx : leadIdx.
  VarSymbol* parIdx     = newTemp("chpl__parIdx");
  VarSymbol* parIdxCopy = newTemp("chpl__parIdxCopy");

  // This is how these flags have been set historically.
  // Todo make them the same regardless of gotSA.
  if (gotSA) {
    // This is needed in setConstFlagsAndCheckUponMove():
    parIdx->addFlag(FLAG_INDEX_OF_INTEREST);

    parIdxCopy->addFlag(FLAG_INDEX_VAR);

    // If we add FLAG_INSERT_AUTO_DESTROY, 'filename' gets double-delete'd in:
    //   test/library/standard/FileSystem/filerator/bradc/findfiles-par.chpl
    //parIdxCopy->addFlag(FLAG_INSERT_AUTO_DESTROY);

  } else {
    //?? parIdx->addFlag(FLAG_INDEX_OF_INTEREST);
    parIdxCopy->addFlag(FLAG_INDEX_VAR);
    parIdxCopy->addFlag(FLAG_INSERT_AUTO_DESTROY);
  }

  Expr* mark = userLoopBody;
  mark->insertBefore(new DefExpr(parIdx));
  mark->insertBefore(new DefExpr(parIdxCopy));

  // detuple into induction variables
  // Todo: do it outside userLoopBody instead? NB these need to be placed
  // after detuple parIdx->parIdxCopy and after the DefExprs for ind vars.
  //
  AList& indvars = fs->inductionVariables();
  int idx = indvars.length;
  if (idx == 1) {
    userLoopBody->insertAtHead("'move'(%S,%S)", toDefExpr(indvars.head)->sym,
                               parIdxCopy);
  } else {
    for_alist_backward(def, indvars)
      userLoopBody->insertAtHead("'move'(%S,%S(%S))", toDefExpr(def)->sym,
                                 parIdxCopy, new_IntSymbol(idx--));
  }

  // Move induction variables' DefExprs to the loop body.
  // That's where their scope is; ex. deinit them at end of each iteration.
  // Do it now, before the loop body gets cloned for and dissolves into
  // the scaffolding for fast-followers.
  //
  for_alist_backward(def, indvars)
    userLoopBody->insertAtHead(def->remove());
}

static void resolveParallelIteratorAndIdxVar(ForallStmt* pfs,
                                             CallExpr* iterCall,
                                             FnSymbol* origIterator,
                                             bool gotSA)
{
  // The par iterator probably has been extendLeader()-ed for forall intents.
  FnSymbol* parIter = iterCall->resolvedFunction();
  bool alreadyResolved = parIter->isResolved();

  resolveFunction(parIter);

  // Set QualifiedType of the index variable.
  QualifiedType iType = fsIterYieldType(pfs, parIter,
                                        origIterator, alreadyResolved);
  VarSymbol* idxVar = parIdxVar(pfs);

  if (idxVar->id == breakOnResolveID)
    gdbShouldBreakHere();
  idxVar->type = iType.type();
  idxVar->qual = iType.getQual();
}

static Expr* rebuildIterableCall(ForallStmt* pfs,
                                 CallExpr* iterCall,
                                 Expr* origExprFlw)
{
  INT_ASSERT(iterCall == pfs->firstIteratedExpr()); // still here?

  if (!pfs->zippered()) {
    INT_ASSERT(!iterCall->next);
    // no tuple building here
    return origExprFlw;
  }

  int origLength = pfs->iteratedExpressions().length;
  CallExpr* result = new CallExpr("_build_tuple", origExprFlw);
  while (Expr* curr = iterCall->next)
    result->insertAtTail(curr->remove());

  // todo: remove the assert and origLength
  INT_ASSERT(result->numActuals() == origLength);
  return result;
}

static void buildLeaderLoopBody(ForallStmt* pfs, Expr* iterExpr) {
  bool zippered = pfs->zippered();

  // Set up the follower call etc.

  // Extract leadIdxCopy and its type.
  VarSymbol* leadIdxCopy     = parIdxCopyVar(pfs);

  BlockStmt* resultBlock     = new BlockStmt();
  // cf in build.cpp: new ForLoop(leadIdx, leadIter, NULL, zippered)
  BlockStmt* leadForLoop     = new BlockStmt();

  VarSymbol* iterRec         = newTemp("chpl__iterLF"); // serial iter, LF case

  VarSymbol* followIdx       = newTemp("chpl__followIdx");
  VarSymbol* followIter      = newTemp("chpl__followIter");
  BlockStmt* followBlock     = NULL;

  iterRec->addFlag(FLAG_EXPR_TEMP);
  iterRec->addFlag(FLAG_CHPL__ITER);
  iterRec->addFlag(FLAG_CHPL__ITER_NEWSTYLE);

  followIdx->addFlag(FLAG_INDEX_OF_INTEREST);

  resultBlock->insertAtTail(new DefExpr(iterRec));
  resultBlock->insertAtTail(new CallExpr(PRIM_MOVE, iterRec, iterExpr));
  Expr* toNormalize = resultBlock->body.tail;

  // todo rename loopBody to userBody
  BlockStmt* loopBody = userLoop(pfs);

  // user loop body to refer to followIdx instead of leadIdxCopy
  for_SymbolSymExprs(se, leadIdxCopy)
    if (isContainedInWithStop(se, loopBody, pfs->loopBody()))
      se->replace(new SymExpr(followIdx));

  loopBody->remove();

  followBlock = buildFollowLoop(iterRec,
                                leadIdxCopy,
                                followIter,
                                followIdx,
                                loopBody,
                                pfs,
                                false,
                                zippered);

  if (fNoFastFollowers == false) {
    // from the original buildForallLoopStmt()
    Symbol* T1 = newTemp();
    Symbol* T2 = newTemp();

    VarSymbol* fastFollowIdx   = newTemp("chpl__fastFollowIdx");
    VarSymbol* fastFollowIter  = newTemp("chpl__fastFollowIter");
    BlockStmt* fastFollowBlock = NULL;


    T1->addFlag(FLAG_EXPR_TEMP);
    T1->addFlag(FLAG_MAYBE_PARAM);

    T2->addFlag(FLAG_EXPR_TEMP);
    T2->addFlag(FLAG_MAYBE_PARAM);

    leadForLoop->insertAtTail(new DefExpr(T1));
    leadForLoop->insertAtTail(new DefExpr(T2));

    if (zippered == false) {
      leadForLoop->insertAtTail("'move'(%S, chpl__staticFastFollowCheck(%S))",    T1, iterRec);
      leadForLoop->insertAtTail(new CondStmt(new SymExpr(T1),
                                          new_Expr("'move'(%S, chpl__dynamicFastFollowCheck(%S))",    T2, iterRec),
                                          new_Expr("'move'(%S, %S)", T2, gFalse)));
    } else {
      leadForLoop->insertAtTail("'move'(%S, chpl__staticFastFollowCheckZip(%S))", T1, iterRec);
      leadForLoop->insertAtTail(new CondStmt(new SymExpr(T1),
                                          new_Expr("'move'(%S, chpl__dynamicFastFollowCheckZip(%S))", T2, iterRec),
                                          new_Expr("'move'(%S, %S)", T2, gFalse)));
    }

    SymbolMap map;
    map.put(followIdx, fastFollowIdx);
    BlockStmt* loopBodyForFast = loopBody->copy(&map);

    fastFollowBlock = buildFollowLoop(iterRec,
                                      leadIdxCopy,
                                      fastFollowIter,
                                      fastFollowIdx,
                                      loopBodyForFast,
                                      pfs,
                                      true,
                                      zippered);

    leadForLoop->insertAtTail(new CondStmt(new SymExpr(T2), fastFollowBlock, followBlock));
  } else {
    leadForLoop->insertAtTail(followBlock);
  }

  // Must happen before any resolving ex. resolveBlockStmt below.
  // Otherwise functions defined within the forall body are not visible.
  // Ex. functions/vass/ref-intent-bug-2big.chpl
  pfs->loopBody()->insertAtTail(leadForLoop);

  pfs->insertBefore(resultBlock);
  normalize(toNormalize); // requires inTree()
  resolveBlockStmt(resultBlock);
  resultBlock->flattenAndRemove();
}

// see also comments above
CallExpr* resolveForallHeader(ForallStmt* pfs, SymExpr* origSE)
{
  CallExpr* retval = NULL;

  if (pfs->id == breakOnResolveID) gdbShouldBreakHere();

  // We only get here for origSE==firstIteratedExpr() .
  // If at that time there are other elements in iterExprs(), we remove them.
  INT_ASSERT(origSE == pfs->firstIteratedExpr());

  CallExpr* iterCall = buildForallParIterCall(pfs, origSE);

  // So we know where iterCall is.
  INT_ASSERT(iterCall         == pfs->firstIteratedExpr());
  INT_ASSERT(origSE->inTree() == false);

  bool gotSA = acceptUnmodifiedIterCall(pfs, iterCall) ||
               findStandaloneOrLeader(pfs, iterCall);
  resolveCallAndCallee(iterCall, false);

  FnSymbol* origIterFn = iterCall->resolvedFunction();

  // ex. resolving the par iter failed and 'pfs' is under "if chpl__tryToken"
  if (tryFailure == false) {
    addParIdxVarsAndRestruct(pfs, gotSA);

    implementForallIntentsNew(pfs, iterCall);

    resolveParallelIteratorAndIdxVar(pfs, iterCall, origIterFn, gotSA);

    if (gotSA) {
      if (origSE->qualType().type()->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
        removeOrigIterCall(origSE);
      }
    } else {
      buildLeaderLoopBody(pfs, rebuildIterableCall(pfs, iterCall, origSE));
    }

    INT_ASSERT(iterCall == pfs->firstIteratedExpr());        // still here?
    INT_ASSERT(iterCall == pfs->iteratedExpressions().tail); // only 1 elem

    retval = iterCall;
  }

  return retval;
}

///////////////////////////////
//                           //
//    ForallStmt lowering    //
//                           //
///////////////////////////////

// lowerForallStmts() removes each ForallStmt from AST,
// replacing it with explicit loop(s) etc.
// Currently this is done towards the end of resolution.
// Our goal is to push it to a later pass, ideally all the way
// to lowerIterators.

void lowerForallStmts() {
  forv_Vec(ForallStmt, fs, gForallStmts) {
    if (!fs->inTree() || !fs->getFunction()->isResolved())
      continue;

    // formerly nonLeaderParCheckInt()
    FnSymbol* parent = fs->getFunction();
    // If isTaskFun(parent), error is still reported in nonLeaderParCheckInt.
    if (parent->isIterator() && !parent->hasFlag(FLAG_INLINE_ITERATOR))
      USR_FATAL_CONT(fs, "invalid use of parallel construct in serial iterator");

    CallExpr* parIterCall = toCallExpr(fs->firstIteratedExpr());
    INT_ASSERT(parIterCall && !parIterCall->next); // expected
    SET_LINENO(parIterCall);
    BlockStmt* PARBlock = new BlockStmt();
    fs->replace(PARBlock); // so we can resolve PARBlock below
    // Maybe move this ->replace() down to resolveBlockStmt(PARBlock) ?
    // If so, add ->remove() to fs->iterExprs() and perhaps others.

    // From the original buildStandaloneForallLoopStmt(), with "sa" -> "par".
    VarSymbol* iterRec = newTemp("chpl__iterPAR"); // serial iter, PAR case
    VarSymbol* parIter = newTemp("chpl__parIter");
    VarSymbol* parIdx  = parIdxVar(fs);

    iterRec->addFlag(FLAG_CHPL__ITER);
    iterRec->addFlag(FLAG_CHPL__ITER_NEWSTYLE);
    iterRec->addFlag(FLAG_MAYBE_REF);
    iterRec->addFlag(FLAG_EXPR_TEMP);

    parIter->addFlag(FLAG_EXPR_TEMP);
    // Too late to do it here - it's needed in setConstFlagsAndCheckUponMove().
    //parIdx->addFlag(FLAG_INDEX_OF_INTEREST);
    parIdx->addFlag(FLAG_INDEX_VAR);

    PARBlock->insertAtTail(new DefExpr(iterRec));
    PARBlock->insertAtTail(new DefExpr(parIter));
    DefExpr* parIdxDef = parIdx->defPoint;
    INT_ASSERT(parIdxDef == fs->loopBody()->body.head);
    PARBlock->insertAtTail(parIdxDef->remove());

    PARBlock->insertAtTail(new CallExpr(PRIM_MOVE, iterRec, parIterCall));
    PARBlock->insertAtTail("'move'(%S, _getIterator(%S))", parIter, iterRec);
    PARBlock->insertAtTail(new DeferStmt(new CallExpr("_freeIterator", parIter)));
    PARBlock->insertAtTail("{TYPE 'move'(%S, iteratorIndex(%S)) }", parIdx, parIter);

   { // shadow the above SET_LINENO
    SET_LINENO(fs->loopBody());

    ForLoop* PARBody = new ForLoop(parIdx, parIter, NULL, /* zippered */ false, /*forall*/ true);

    PARBlock->insertAtTail(PARBody);
    resolveBlockStmt(PARBlock);
    PARBlock->flattenAndRemove(); // into where 'fs' used to be

    BlockStmt* userBody = userLoop(fs);
    while (Expr* def = fs->inductionVariables().tail)
      userBody->insertAtHead(def->remove());

    while (Expr* svdef = fs->shadowVariables().tail)
      fs->loopBody()->insertAtHead(svdef->remove());

    userBody->flattenAndRemove();          // into fs->loopBody()
    PARBody->insertAtTail(fs->loopBody()); // loopBody is already resolved
    fs->loopBody()->flattenAndRemove();    // into PARBody
   }
  }
}
