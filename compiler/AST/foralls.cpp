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

// foralls.h, foralls.cpp - support for forall loops

#include "expr.h"
#include "foralls.h"
#include "stmt.h"
#include "astutil.h"
#include "stlUtil.h"
#include "passes.h" // for normalized, resolved
#include "AstVisitor.h"
#include "ForLoop.h"
#include "resolution.h"
#include "iterator.h"

enum ParIterResult { pSA = 1, pLeader, pFailed };

const char* tfiTagDescrString(TFITag tfiTag) {
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

static TFITag it2tfi(Expr* ref, IntentTag intent) {
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
    USR_FATAL_CONT(ref, "%s is not supported in a 'with' clause",
                        intentDescrString(intent));
    return TFI_DEFAULT;
  }
  INT_ASSERT(false); // unexpected IntentTag; 'intent' contains garbage?
  return TFI_DEFAULT; // dummy
}

void addForallIntent(ForallIntents* fi, Expr* var, IntentTag intent, Expr* ri) {
  TFITag tfi = ri ? TFI_REDUCE : it2tfi(var, intent);
  fi->fiVars.push_back(var);
  fi->fIntents.push_back(tfi);
  fi->riSpecs.push_back(ri);
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
// ForallStmt
/////////////////////////////////

// convert _build_tuple args to AList
// todo: have the parser build AList directly or at least keep it as PRIM_ZIP
// i.e. skip zipToTuple()
static void convertIterator(AList& list, Expr* iter, bool zippered) {
  if (zippered) {
    CallExpr* icall = toCallExpr(iter);
    INT_ASSERT(icall && icall->isNamed("_build_tuple"));
    for_actuals(actual, icall)
      list.insertAtTail(actual->remove());
  } else {
    list.insertAtTail(iter);
  }
}

ForallStmt::ForallStmt(BlockStmt* var, Expr* iter, bool zippered,
                       ForallIntents* intents, BlockStmt* body) :
  Stmt(E_ForallStmt),
  fVar(var),
  fZippered(zippered),
  fWith(intents),
  fBody(body)
{
  fIter.parent = this;

  if (iter)
    convertIterator(fIter, iter, zippered);

  // caller must provide these
  INT_ASSERT(fVar);
  INT_ASSERT(fVar->blockTag == BLOCK_NORMAL);
  INT_ASSERT(fWith);
  INT_ASSERT(fBody);
  INT_ASSERT(fBody->blockTag == BLOCK_NORMAL);

  if (fVar->body.tail != fBody) {
    INT_ASSERT(!fBody->parentExpr); // fBody better not be inTree()
    fVar->insertAtTail(fBody);
  }

  gForallStmts.add(this);

  // Should not create any new ones after they have been lowered.
  INT_ASSERT(beforeLoweringForallStmts);
}

ForallStmt::~ForallStmt() {
  delete fWith;
}

ForallStmt* ForallStmt::copyInner(SymbolMap* map) {
  BlockStmt* newVars = toBlockStmt(COPY_INT(fVar));
  BlockStmt* newBody = toBlockStmt(newVars->body.tail);
  ForallStmt* _this  = new ForallStmt(newVars, NULL, 
                        fZippered,
                        COPY_INT(fWith),
                        newBody);

  for_alist(expr, fIter)
    _this->fIter.insertAtTail(COPY_INT(expr));

  return _this;
}

void ForallStmt::replaceChild(Expr* oldAst, Expr* newAst) {
  if (oldAst == fBody || oldAst == fVar) {
    if (!newAst)
      (oldAst == fBody ? fBody : fVar) = NULL;
    else if (BlockStmt* newBlock = toBlockStmt(newAst)) {
      INT_ASSERT(!newBlock->isLoopStmt());
      (oldAst == fBody ? fBody : fVar) = newBlock;
    } else
      // newAst does not fit - caller responsibility
      INT_ASSERT(false);

  } else if (fWith->replaceChildFI(oldAst, newAst))
    ; //good

  else
    // did not find oldAst in our ForallStmt - caller responsibility
    INT_ASSERT(false);
}

void ForallStmt::verify() {
  Expr::verify(E_ForallStmt);

  INT_ASSERT(fIter.length > 0);

  if (fIter.parent != this)
    INT_FATAL(this, "ForallStmt::verify. Bad fIter.parent");

  for_alist(expr, fIter) {
    if (expr->parentExpr != this)
      INT_FATAL(this, "ForallStmt::verify. Bad fIter.expr->parentExpr");
  }

  INT_ASSERT(fWith);
  fWith->verifyFI(this);

  INT_ASSERT(fVar);
  verifyParent(fVar, "ForallStmt::fVar");
  verifyNotOnList(fVar);
  // should be a normal block
  INT_ASSERT(fVar->blockTag == BLOCK_NORMAL);
  INT_ASSERT(!fVar->blockInfoGet());
  INT_ASSERT(!fVar->isLoopStmt());
  INT_ASSERT(!fVar->modUses);
  INT_ASSERT(!fVar->userLabel);
  INT_ASSERT(!fVar->byrefVars);
  INT_ASSERT(!fVar->forallIntents);

  // fBody must be the last thing in fVar
  INT_ASSERT(fBody);
  INT_ASSERT(fVar->body.tail == fBody);

  // should be a normal block
  INT_ASSERT(fBody->blockTag == BLOCK_NORMAL);
  INT_ASSERT(!fBody->blockInfoGet());
  INT_ASSERT(!fBody->isLoopStmt());
  INT_ASSERT(!fBody->modUses);
  INT_ASSERT(!fBody->userLabel);
  INT_ASSERT(!fBody->byrefVars);
  INT_ASSERT(!fBody->forallIntents);

  // ForallStmt are gone during resolve().
  INT_ASSERT(!resolved);
}

void ForallStmt::accept(AstVisitor* visitor) {
  if (visitor->enterForallStmt(this)) {
    for_alist(next_ast, fIter)
      next_ast->accept(visitor);
    // not doing: fIter->accept(visitor);
    fWith->acceptFI(visitor); // aka visitor->visitForallIntents(fWith);
    fVar->accept(visitor); // includes fBody
    visitor->exitForallStmt(this);
  }
}

Expr* ForallStmt::getFirstChild() {
  // Should we cater to fWith? Should fVar come earlier?
  return fIter.head;
}

Expr* ForallStmt::getFirstExpr() {
  return fIter.head->getFirstExpr();
}

Expr* ForallStmt::getNextExpr(Expr* expr) {
  if (expr == fIter.tail)
    return fVar->getFirstExpr();
  else
    return this;
}

bool isForallStmtIterator(ForallStmt* fs, Expr* expr) {
  return expr->list && expr->list == &(fs->fIter);
}

ForallStmt* enclosingForallStmt(Expr* expr) {
  for (Expr* curr = expr->parentExpr; curr; curr = curr->parentExpr)
    if (ForallStmt* fs = toForallStmt(curr))
      return fs;
  return NULL;
}

int reduceIntentIdx(ForallStmt* fs, Symbol* reVar) {
  ForallIntents* fi = fs->fWith;
  int nv = fi->numVars();
  for (int i = 0; i < nv; i++)
    if (fi->isReduce(i))
      if (SymExpr* varSE = toSymExpr(fi->fiVars[i]))
        if (varSE->symbol() == reVar)
          return i;

  // Did not see 'reVar' with a reduce intent.
  return -1;
}

VarSymbol* forallIndexVar(ForallStmt* fs) {
  DefExpr* def = toDefExpr(fs->fVar->body.head);
  INT_ASSERT(def);
  VarSymbol* iv = toVarSymbol(def->sym);
  INT_ASSERT(iv);
  return iv;
}

VarSymbol* forallIdxCopyVar(ForallStmt* fs, VarSymbol* idxVar) {
  DefExpr* def = toDefExpr(idxVar->defPoint->next);
  INT_ASSERT(def);
  VarSymbol* iv = toVarSymbol(def->sym);
  INT_ASSERT(iv);
  return iv;
}

static VarSymbol* forallIdxCopyVar(ForallStmt* fs) {
  return forallIdxCopyVar(fs, forallIndexVar(fs));
}


/////////////////////////////////
//                             //
//    lowering a ForallStmt    //
//                             //
/////////////////////////////////

//
// Given an iterator function, find the type that it yields.
// It would be fn->retType, alas protoIteratorClass() messes with that.
// This helper is ForallStmt-specific because of the error message it issues.
//
static QualifiedType fsIterYieldType(FnSymbol* iterFn, CallExpr* iterCall,
                                     ParIterResult saOrLeader)
{
  if (iterFn->hasFlag(FLAG_ITERATOR_FN)) {
    IteratorInfo* ii = iterFn->iteratorInfo;
    INT_ASSERT(ii);
    return ii->getValue->getReturnQualType();
  } else {
    // e.g. "proc these() return _value.these();"
    AggregateType* retType = toAggregateType(iterFn->retType);
    if (retType && retType->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
      FnSymbol* iterator = retType->iteratorInfo->iterator;
      INT_ASSERT(iterator->hasFlag(FLAG_ITERATOR_FN)); // no more recursion?
      return fsIterYieldType(iterator, iterCall, saOrLeader);
    } else {
      USR_FATAL_CONT(iterCall, "The iterable-expression resolves to a non-iterator function '%s' when looking for %s iterator", iterFn->name, saOrLeader == pSA ? "standalone" : "leader");
      USR_PRINT(iterFn, "The function '%s' is declared here", iterFn->name);
      USR_STOP();
      return (QualifiedType)0; // dummy
    }
  }
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
  ForLoop*   followBody  = new ForLoop(followIdx, followIter, loopBody, zippered);

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

  ref->insertAfter(followBlock); // otherwise it wouldn't normalize
  normalize(followBlock);
  followBlock->remove();

  followBlock->insertAtTail(new DefExpr(followIdx));

  followBlock->insertAtTail("{TYPE 'move'(%S, iteratorIndex(%S)) }", followIdx, followIter);

  followBlock->insertAtTail(followBody);
  followBlock->insertAtTail(new CallExpr("_freeIterator", followIter));

  return followBlock;
}

static SymExpr* normalizeFSIterExpr(ForallStmt* pfs, Symbol*& ieTemp,
                                    Expr* origExpr, QualifiedType origType)
{
  SymExpr* iterSE = toSymExpr(origExpr);

  // Normalize if needed.
  if (iterSE) {
    // leave origExpr alone
    iterSE = iterSE->copy();
  } else {
    ieTemp = newTemp("chpl_ieTemp");
    ieTemp->type = origType.type();
    ieTemp->qual = origType.getQual();
    iterSE = new SymExpr(ieTemp);

    pfs->insertBefore(new DefExpr(ieTemp));
    //Done in buildForallParIterCall():
    //  pfs->insertBefore("'move'(%S,%E)", ieTemp, origExpr);
  }

  if (origType.type()->getValType()->symbol->
        hasEitherFlag(FLAG_DOMAIN, FLAG_ARRAY))
  {
    // Redirect to _value. It would be simpler instead to have something
    // like this in modules:
    //   proc _domain.these(param tag) return _value.these(tag);
    //
    // However, that would result in an "unresolved call" error
    // for a domain map without the standalone iterator.

    VarSymbol* valTemp = newTemp("chpl_valueTemp");
    pfs->insertBefore(new DefExpr(valTemp));
    CallExpr*  valCall = new CallExpr("_value", gMethodToken, iterSE);
    CallExpr* moveCall = new CallExpr(PRIM_MOVE, valTemp, valCall);
    pfs->insertBefore(moveCall);

    if (FnSymbol* callee = tryResolveCall(valCall)) {
      // todo: replace somehow with resolveFnForCall(callee, valCall);
      callStack.add(valCall);
      resolveFns(callee);
      callStack.pop();
    } else {
      if (iterSE->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
        USR_FATAL(origExpr, "unable to iterate over type '%s'",
                  toString(iterSE->symbol()->type));
      } else {
        USR_FATAL(origExpr, "cannot iterate over values of type %s",
                  toString(iterSE->symbol()->type));
      }
    }
    resolveCall(moveCall);

    iterSE = new SymExpr(valTemp);
  }

  return iterSE;
}

// Replaces 'origExpr' in the tree with the result.
// If 'ieTemp' is created, origExpr will be inTree().
static CallExpr* buildForallParIterCall(ForallStmt* pfs, Expr* origExpr,
                                        Symbol*& ieTemp)
{
  CallExpr* iterCall;
  QualifiedType origType = origExpr->qualType();
  
  if (origType.type()->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
    // Our iterable expression is an iterator call.
    iterCall = toCallExpr(origExpr->copy());
    // If it is not a call, how do we convert it to a parallel iterator?
    // We do not want to resort to PRIM_TO_STANDALONE et al.
    // We'd probably need to work with the call that's assigned into iterCall.
    INT_ASSERT(iterCall);
    FnSymbol* origTarget = iterCall->isResolved();
    INT_ASSERT(origTarget);
    iterCall->baseExpr = new UnresolvedSymExpr(origTarget->name);

  } else {
    // Not an iterator call, so add a call to these().
    SymExpr* iterSE = normalizeFSIterExpr(pfs, ieTemp, origExpr, origType);
    iterCall = new CallExpr("these", gMethodToken, iterSE);
  }

  origExpr->replace(iterCall);

  // Couldn't do it in normalizeFSIterExpr() because origExpr was inTree().
  if (ieTemp)
    ieTemp->defPoint->insertAfter("'move'(%S,%E)", ieTemp, origExpr);

  return iterCall;
}

static ParIterResult findStandaloneOrLeader(ForallStmt* pfs,
                                            CallExpr* iterCall, bool inTry)
{
  bool gotParallel = false;
  ParIterResult saOrLeader = pSA;

  // We are starting with a serial-iterator call.
  // Transform it to a standalone/leader call.
  NamedExpr* tag = new NamedExpr("tag", new SymExpr(gStandaloneTag));
  iterCall->insertAtTail(tag);

  // try standalone
  if (!pfs->fZippered)
    gotParallel = tryResolveCall(iterCall);

  // try leader
  if (!gotParallel) {
    saOrLeader = pLeader;
    tag->actual->replace(new SymExpr(gLeaderTag));
    gotParallel = tryResolveCall(iterCall);
  }

  if (!gotParallel) {
    if (inTry)
      return pFailed;
    
    // Cannot USR_FATAL_CONT in general: e.g. if these() is not found,
    // we do not know the type of the index variable. Without which
    // which cannot typecheck the loop body.
    USR_FATAL(iterCall, "A standalone or leader iterator is not found for the iterable expression in this forall loop");
  }

  return saOrLeader;
}

static Expr* rebuildIterableCall(ForallStmt* pfs,
                                 CallExpr* iterCall, Expr* origExprFlw)
{
  INT_ASSERT(iterCall == pfs->fIter.head); // still here?

  if (!pfs->fZippered) {
    INT_ASSERT(!iterCall->next);
    // no tuple building here
    return origExprFlw;
  }

  int origLength = pfs->fIter.length;
  CallExpr* result = new CallExpr("_build_tuple", origExprFlw);
  while (Expr* curr = iterCall->next)
    result->insertAtTail(curr->remove());

  // todo: remove the assert and origLength
  INT_ASSERT(result->numActuals() == origLength);
  return result;
}

static void resolveParallelIteratorAndIdxVar(ForallStmt* pfs,
                                             CallExpr* iterCall,
                                             ParIterResult saOrLeader)
{
  // The iterator probably has been extendLeader()-ed for forall intents.
  FnSymbol* parIter = iterCall->isResolved();
  resolveFns(parIter);

  // Set QualifiedType of the index variable.
  QualifiedType iType = fsIterYieldType(parIter, iterCall, saOrLeader);
  VarSymbol* idxVar = forallIndexVar(pfs);
  idxVar->type = iType.type();
  idxVar->qual = iType.getQual();
}

static void buildLeaderLoopBody(ForallStmt* pfs, Expr* iterExpr) {
  bool zippered = pfs->fZippered;

  // Set up the follower call etc.

  // Extract leadIdxCopy and its type.
  VarSymbol* leadIdxCopy = forallIdxCopyVar(pfs);

  BlockStmt* resultBlock     = new BlockStmt();
  // cf in build.cpp: new ForLoop(leadIdx, leadIter, NULL, zippered)
  BlockStmt* leadForLoop     = new BlockStmt();

  VarSymbol* iterRec         = newTemp("chpl__iterLF"); // serial iter, LF case

  VarSymbol* followIdx       = newTemp("chpl__followIdx");
  VarSymbol* followIter      = newTemp("chpl__followIter");
  BlockStmt* followBlock     = NULL;

  iterRec->addFlag(FLAG_EXPR_TEMP);
  iterRec->addFlag(FLAG_CHPL__ITER);

  followIdx->addFlag(FLAG_INDEX_OF_INTEREST);

  resultBlock->insertAtTail(new DefExpr(iterRec));
  resultBlock->insertAtTail(new CallExpr(PRIM_MOVE, iterRec, iterExpr));
  Expr* toNormalize = resultBlock->body.tail;

  // loop body to refer to followIdx instead of leadIdxCopy
  for_SymbolSymExprs(se, leadIdxCopy)
    if (se->parentExpr->parentExpr != pfs->fVar) // heuristic
      se->replace(new SymExpr(followIdx));

  BlockStmt* loopBody = pfs->fBody;
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
    // from former buildForallLoopStmt()
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
  pfs->fVar->insertAtTail(leadForLoop);

  pfs->insertBefore(resultBlock);
  normalize(toNormalize); // requires inTree()
  resolveBlockStmt(resultBlock);
  resultBlock->flattenAndRemove();
}

Expr* resolveParallelIteratorAndForallIntents(Expr* origExpr, bool inTry) {
  ForallStmt* pfs = toForallStmt(origExpr->parentExpr);
  if (!pfs || !origExpr->list)
    return origExpr;

  // We only get here for origExpr==fIter.head .
  // If at that time there are any other elements in fIter, we remove them.
  INT_ASSERT(origExpr == pfs->fIter.head);

  Symbol* ieTemp = NULL;  // may be set in next line
  CallExpr* iterCall = buildForallParIterCall(pfs, origExpr, ieTemp);

  // So we know where iterCall is.
  INT_ASSERT(iterCall == pfs->fIter.head);
  // Later, use ieTemp if defined, otherwise use origExpr.
  INT_ASSERT(!ieTemp == !origExpr->inTree());

  ParIterResult saOrLeader = findStandaloneOrLeader(pfs, iterCall, inTry);
  if (saOrLeader == pFailed)
    return NULL;

  implementForallIntentsNew(pfs, iterCall);

  resolveParallelIteratorAndIdxVar(pfs, iterCall, saOrLeader);

  if (saOrLeader == pLeader) {
    Expr* origExprFlw = ieTemp ? new SymExpr(ieTemp) : origExpr;
    buildLeaderLoopBody(pfs,
                        rebuildIterableCall(pfs, iterCall, origExprFlw));
  }

  INT_ASSERT(iterCall == pfs->fIter.head); // still here?
  INT_ASSERT(iterCall == pfs->fIter.tail); // only one element in fIter

  return iterCall;
}


//
// (A) We are doing this so the generated code looks like it did before.
//     Even though arguably this should not matter.
//     Eventually - or soon - this should go away.
// (B) It is a heuristic based on the shape of the AST.
//     If it does not work, delete. (See also (A).)
//
static void move_freeIterator_call(BlockStmt* SABlock) {
  CallExpr* freeIt = toCallExpr(SABlock->body.tail);
  INT_ASSERT(freeIt && freeIt->isNamed("_freeIterator"));
  Expr *freeHere = NULL, *curr = SABlock;
  for (int i = 0; i < 15; i++) {
    curr = curr->next;
    if (!curr) break;
    if (CallExpr* currCE = toCallExpr(curr))
      if (currCE->isNamed("chpl_here_free"))
        { freeHere = curr; break; }
  }
  // freeHere==NULL for when riSpec is a user-defined/managed object
  if (freeHere)
    freeHere->insertAfter(freeIt->remove());
}  

void lowerForallStmts() {
  forv_Vec(ForallStmt, fs, gForallStmts) {
    if (!fs->inTree() || !fs->getFunction()->isResolved())
      continue;

    // Forall intents aka fs->fWith should have already been handled.

    CallExpr* parIterCall = toCallExpr(fs->fIter.head);
    INT_ASSERT(parIterCall && !parIterCall->next); // expected
    SET_LINENO(parIterCall);
    BlockStmt* SABlock = new BlockStmt();
    fs->replace(SABlock); // so we can resolve SABlock below
    // Maybe move this ->replace() down to resolveBlockStmt(SABlock) ?
    // If so, add ->remove() to fs->fIter and perhaps others.

    // From the original buildForallLoopStmt().
    VarSymbol* iterRec   = newTemp("chpl__iterSA"); // serial iter, SA case
    VarSymbol* saIter    = newTemp("chpl__saIter");
    VarSymbol* saIdx     = forallIndexVar(fs);

    iterRec->addFlag(FLAG_CHPL__ITER);
    iterRec->addFlag(FLAG_MAYBE_REF);
    iterRec->addFlag(FLAG_EXPR_TEMP);

    saIter->addFlag(FLAG_EXPR_TEMP);
    saIdx->addFlag(FLAG_INDEX_OF_INTEREST);
    saIdx->addFlag(FLAG_INDEX_VAR);

    SABlock->insertAtTail(new DefExpr(iterRec));
    SABlock->insertAtTail(new DefExpr(saIter));
    DefExpr* saIdxDef = toDefExpr(fs->fVar->body.head);
    INT_ASSERT(saIdxDef && saIdxDef->sym == saIdx);
    SABlock->insertAtTail(saIdxDef->remove());

    SABlock->insertAtTail(new CallExpr(PRIM_MOVE, iterRec, parIterCall));
    SABlock->insertAtTail("'move'(%S, _getIterator(%S))", saIter, iterRec);
    SABlock->insertAtTail("{TYPE 'move'(%S, iteratorIndex(%S)) }", saIdx, saIter);

    currentAstLoc = fs->fBody->astloc; // can't do SET_LINENO
    ForLoop* SABody = new ForLoop(saIdx, saIter, NULL, false);

    // not needed:
    //destructureIndices(SABody, indices, new SymExpr(saIdxCopy), false);

    SABlock->insertAtTail(SABody);
    SABlock->insertAtTail("_freeIterator(%S)", saIter);

    resolveBlockStmt(SABlock);
    move_freeIterator_call(SABlock);
    SABlock->flattenAndRemove();

    BlockStmt* fsLoopBody = toBlockStmt(fs->fVar->body.tail);
    fsLoopBody->flattenAndRemove(); // into fVar

    SABody->insertAtTail(fs->fVar); // fs->fVar is already resolved
    fs->fVar->flattenAndRemove();   // into SABody
  }
}

GenRet ForallStmt::codegen() {
  INT_ASSERT(false); // should not be invoked
  return (GenRet)0;
}
