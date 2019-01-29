/*
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
    case TFI_DEFAULT:       return "default";
    case TFI_CONST:         return "const";
    case TFI_IN_PARENT:     return "parent-in";
    case TFI_IN:            return "in";
    case TFI_CONST_IN:      return "const in";
    case TFI_REF:           return "ref";
    case TFI_CONST_REF:     return "const ref";
    case TFI_REDUCE:        return "reduce";
    case TFI_REDUCE_OP:        return "reduce-Op";
    case TFI_REDUCE_PARENT_AS: return "parent-reduce-AS";
    case TFI_REDUCE_PARENT_OP: return "parent-reduce-Op";
    case TFI_TASK_PRIVATE:  return "task-private";
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
      USR_FATAL_CONT(ref, "%s is not supported in a 'with' clause",
                          intentDescrString(intent));
      return TFI_DEFAULT;
  }
  INT_ASSERT(false); // unexpected IntentTag; 'intent' contains garbage?
  return TFI_DEFAULT; // dummy
}

static ShadowVarSymbol* buildShadowVariable(ShadowVarPrefix prefix,
                                            const char* name, Expr* ovar)
{
  ForallIntentTag intent = TFI_DEFAULT; // dummy
  switch (prefix)
  {
    case SVP_CONST:     intent = TFI_CONST;     break;
    case SVP_IN:        intent = TFI_IN;        break;
    case SVP_CONST_IN:  intent = TFI_CONST_IN;  break;
    case SVP_REF:       intent = TFI_REF;       break;
    case SVP_CONST_REF: intent = TFI_CONST_REF; break;
    case SVP_VAR:
      // This keyword is for a TPV.
      // Whereas the user provided neither a type nor an init.
      USR_FATAL_CONT(ovar, "a task private variable '%s'"
                     "requires a type and/or initializing expression", name);
      break;
  }

  ShadowVarSymbol* result = new ShadowVarSymbol(intent, name, NULL);
  new DefExpr(result); // set result->defPoint
  return result;
}

static ShadowVarSymbol* buildTaskPrivateVariable(ShadowVarPrefix prefix,
            const char* nameString, Expr* nameExpr, Expr* type, Expr* init)
{

  // TPV - task-private variable, as we have a type and/or an initializer.
  ShadowVarSymbol* result = new ShadowVarSymbol(TFI_TASK_PRIVATE,
                                                nameString, NULL);

  switch (prefix)
  {
    case SVP_VAR:        result->qual = QUAL_VAL;       break;

    case SVP_CONST:      result->qual = QUAL_CONST_VAL;
                         result->addFlag(FLAG_CONST);   break;

    case SVP_REF:        result->qual = QUAL_REF;
                         result->addFlag(FLAG_REF_VAR); break;

    case SVP_CONST_REF:  result->qual = QUAL_CONST_REF;
                         result->addFlag(FLAG_CONST);
                         result->addFlag(FLAG_REF_VAR); break;

    case SVP_IN:
    case SVP_CONST_IN:                                  break; // error below
  }

  // Check for type or init in a wrong place.
  switch (prefix)
  {
    // One or both are fine.
    case SVP_CONST:
    case SVP_VAR:
      break;

    // Ref tpvs must have init and not type.
    case SVP_CONST_REF:
    case SVP_REF:
      if (init == NULL)
        USR_FATAL_CONT(nameExpr, "a 'ref' or 'const ref' task-private variable"
                     " '%s' must have an initializing expression", nameString);
      if (type != NULL)
        USR_FATAL_CONT(nameExpr, "a 'ref' or 'const ref' task-private variable"
                       " '%s' cannot have a type", nameString);
      break;

    // This keyword combination is not for a TPV.
    case SVP_IN:
    case SVP_CONST_IN:
      USR_FATAL_CONT(nameExpr, "an 'in' or 'const in' intent for '%s' "
                     "does not allow a type or an initializing expression",
                     nameString);
      USR_PRINT(nameExpr, "if you mean to declare a task-private variable,"
                " use 'var' or 'const'");
      break;
  }

  // We will call autoDestroy from deinitBlock() explicitly.
  result->addFlag(FLAG_NO_AUTO_DESTROY);

  new DefExpr(result, init, type); // set result->defPoint

  return result;
}

//
// The returned ShadowVarSymbol comes with a DefExpr in its defPoint.
//
ShadowVarSymbol* ShadowVarSymbol::buildForPrefix(ShadowVarPrefix prefix,
                                    Expr* nameExp, Expr* type, Expr* init)
{
  const char* nameString = toUnresolvedSymExpr(nameExp)->unresolved;

  if (type == NULL && init == NULL)
    // non-TPV forall intent
    return buildShadowVariable(prefix, nameString, nameExp);
  else
    return buildTaskPrivateVariable(prefix, nameString, nameExp, type, init);
}


ShadowVarSymbol* ShadowVarSymbol::buildFromReduceIntent(Expr* ovar,
                                                        Expr* riExpr)
{
  INT_ASSERT(riExpr != NULL);
  const char* name = toUnresolvedSymExpr(ovar)->unresolved;
  ShadowVarSymbol* result = new ShadowVarSymbol(TFI_REDUCE, name, NULL, riExpr);
  new DefExpr(result); // set result->defPoint
  return result;
}

// old style
void addForallIntent(ForallIntents* fi, Expr* var, IntentTag intent, Expr* ri) {
  ForallIntentTag tfi = ri ? TFI_REDUCE : argIntentToForallIntent(var, intent);
  fi->fiVars.push_back(var);
  fi->fIntents.push_back(tfi);
  fi->riSpecs.push_back(ri);
}

// new style
void addForallIntent(CallExpr* call, ShadowVarSymbol* svar) {
  call->insertAtTail(svar->defPoint);
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


/////////////////////////////////////////////////////
//                                                 //
// ForallStmt pre-lowering: resolveForallHeader()  //
//                                                 //
/////////////////////////////////////////////////////

// resolveForallHeader() resolves key parts of ForallStmt:
//
//  * find the target parallel iterator (standalone or leader) and resolve it
//  * issue an error, if neither is found
//  * handle forall intents, using setupAndResolveShadowVars()
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
      case TFI_IN_PARENT:
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

      case TFI_REDUCE_OP:
      case TFI_REDUCE_PARENT_AS:
      case TFI_REDUCE_PARENT_OP:
        // The error should have been issued above upon TFI_REDUCE.
        break;

      case TFI_TASK_PRIVATE:
        // task-private variables are TODO too.
        USR_FATAL_CONT(svar,
          "Task-private variables are currently not implemented"
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


static bool isIteratorRecord(Symbol* sym) {
  return sym->type->symbol->hasFlag(FLAG_ITERATOR_RECORD);
}


static bool acceptUnmodifiedIterCall(ForallStmt* pfs, CallExpr* iterCall)
{
  return pfs->createdFromForLoop() ||
         pfs->requireSerialIterator();
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

  // followIdx has a defPoint in the non-fast case
  // and no defPoint in the fast case i.e. for fastFollowIdx.
  if (followIdx->defPoint == NULL) {
    followBlock->insertAtTail(new DefExpr(followIdx));
  } else {
    followBlock->insertAtTail(followIdx->defPoint);
  }

  followBlock->insertAtTail("{TYPE 'move'(%S, iteratorIndex(%S)) }", followIdx, followIter);

  followBlock->insertAtTail(followBody);

  return followBlock;
}

// Returns true for: .=( se, "_shape_", whatever)
static bool isSettingShape(SymExpr* se) {
  if (CallExpr* parent = toCallExpr(se->parentExpr))
    if (parent->isPrimitive(PRIM_SET_MEMBER))
      if (SymExpr* field = toSymExpr(parent->get(2)))
        if (!strcmp(field->symbol()->name, "_shape_"))
          return true;
  return false;
}

// Returns true for: iteratorIndexType(se)
static bool isIITcall(SymExpr* se) {
  if (CallExpr* parent = toCallExpr(se->parentExpr))
    if (parent->isNamed("iteratorIndexType")    ||
        parent->isNamed("iteratorIndexTypeZip") )
      return true;
  return false;
}

// The respective temp may not be needed any longer. Remove it.
static void removeOrigIterCall(SymExpr* origSE)
{
  INT_ASSERT(!origSE->inTree());

  Symbol* origSym = origSE->symbol();
  INT_ASSERT(origSym->hasFlag(FLAG_TEMP));

  // If the temp is used only to set its shape, remove it. BTW there may be
  // up to 3 shape-settings, due to a ref/value/constRef ContextCall.
  //
  // Or, the temp can be passed to iteratorIndexType/Zip() to determine
  // the input type for a reduce expr. If so, keep it.
  // Ex. associative/ferguson/plus-reduce-assoc.chpl
  //     associative/bharshbarg/domains/reduceArrOfAssocDom.chpl
  //
  // If there is another scenario, the compiler will hit INT_ASSERT() below.
  // This will alert us to look at it to make sure it is legit.

  SymExpr* defSE = origSym->getSingleDef();
  bool otherUses = false;

  for_SymbolSymExprs(se1, origSym)
    if (se1 != defSE && ! isSettingShape(se1))
      {
        INT_ASSERT(isIITcall(se1));
        otherUses = true;
      }

  if (otherUses) {
    return;  // Keep the temp.
  }

  // The temp is not needed, indeed. Remove it.

  INT_ASSERT(toCallExpr(defSE->parentExpr)->isPrimitive(PRIM_MOVE));
  defSE->parentExpr->remove();

  for_SymbolSymExprs(se2, origSym) {
    INT_ASSERT(isSettingShape(se2));
    se2->parentExpr->remove();
  }

  origSym->defPoint->remove();
}

// Replaces 'origSE' in the tree with the result.
static CallExpr* buildForallParIterCall(ForallStmt* pfs, SymExpr* origSE,
                                        FnSymbol*& origTargetRef)
{
  CallExpr* iterCall = NULL;

  if (origSE->symbol()->type->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
    // Our iterable expression is an iterator call.

    if (ArgSymbol* origArg = toArgSymbol(origSE->symbol())) {
      FnSymbol* iterator = getTheIteratorFn(origArg->type);
      USR_FATAL_CONT(origSE, "a forall loop over a formal argument corresponding to a for/forall/promoted expression or an iterator call is not implemented");
      USR_PRINT(iterator, "the actual argument is here");
      USR_STOP();
    }

    CallExpr* origIterCall = toCallExpr(getDefOfTemp(origSE));
    // What to do if we do not find it?
    // For example, if the forall is over a formal that is an IR.
    INT_ASSERT(origIterCall);

    FnSymbol* origTarget = origIterCall->resolvedFunction();
    INT_ASSERT(origTarget);

    const char* targetName = origTarget->name;
    const int forallExprNameLen = strlen(astr_forallexpr);
    if (!strncmp(targetName, astr_forallexpr, forallExprNameLen)) {
      // a forall loop over a (possibly zippered) forall expression, ex.:
      //  test/reductions/deitz/test_maxloc_reduce_wmikanik_bug2.chpl
      targetName = astr(astr_loopexpr_iter, targetName + forallExprNameLen);

      // Alternatively, find the function that origTarget redirects to.
      origTarget = getTheIteratorFn(origTarget->retType);
      INT_ASSERT(origTarget->name == targetName);
    }

    if (acceptUnmodifiedIterCall(pfs, origIterCall)) {
      iterCall = origIterCall;
      iterCall->remove();
    } else {
      iterCall = origIterCall->copy();
      iterCall->baseExpr = new UnresolvedSymExpr(targetName);
      origTargetRef = origTarget;
    }

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

static bool findStandaloneOrLeader(ForallStmt* pfs, CallExpr* iterCall,
                                   SymExpr* origSE, FnSymbol* origTarget)
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

  // try serial
  if (!gotParallel && pfs->allowSerialIterator()) {
    gotSA = true;
    tag->remove();
    if (origTarget != NULL) {
      gotParallel = true;
      iterCall->baseExpr->replace(new SymExpr(origTarget));
    } else {
      // Iterating over a variable that does not have parallel .these() iters.
      INT_ASSERT(! isIteratorRecord(origSE->symbol()));
      gotParallel = tryResolveCall(iterCall);
    }
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
  if (gotSA) {
    // No need to restructure anything. Leaving it as-is for simplicity.

    VarSymbol* parIdx = parIdxVar(fs);

    // FLAG_INDEX_OF_INTEREST is needed in setConstFlagsAndCheckUponMove():
    parIdx->addFlag(FLAG_INDEX_OF_INTEREST);
    parIdx->addFlag(FLAG_INDEX_VAR);

    return;
  }

  // Keep the user loop as its own BlockStmt.
  // Make it the last thing in the new fs->loopBody().
  BlockStmt* userLoopBody = fs->loopBody();
  BlockStmt* newLoopBody = new BlockStmt();
  userLoopBody->replace(newLoopBody);
  newLoopBody->insertAtTail(userLoopBody);

  // The induction variable of the parallel loop.
  VarSymbol* parIdx = newTemp("chpl_followThis");

  // If there is only one follower, we are tempted to use
  // the original forall's induction variable as the
  // the induction variable of the follower loop.
  // Alas, this results in the autoDestroy for that variable
  // to be inserted outside the loop, and more trouble from that.
  // Ex. test/functions/ferguson/ref-pair/iterating-over-arrays.chpl

  // The induction variable of the follower loop.
  VarSymbol* followIdx = newTemp("chpl__followIdx");
  userLoopBody->insertBefore(new DefExpr(followIdx));

  AList& indvars = fs->inductionVariables();
  int idx = indvars.length;

  if (idx == 1) {
    // If only one induction var, treat as non-zippered.
    fs->setNotZippered();
    userLoopBody->insertAtHead("'move'(%S,%S)",
                               toDefExpr(indvars.head)->sym, followIdx);

  }
  else {
    for_alist_backward(def, indvars)
      userLoopBody->insertAtHead("'move'(%S,%S(%S))", toDefExpr(def)->sym,
                                 followIdx, new_IntSymbol(idx--));
  }

  // Move induction variables' DefExprs to the loop body.
  // That's where their scope is; ex. deinit them at end of each iteration.
  // Do it now, before the loop body gets cloned for and dissolves into
  // the scaffolding for fast-followers.
  //
  for_alist_backward(def, indvars)
    userLoopBody->insertAtHead(def->remove());

  // parIdx to be the index variable of the parallel loop.
  // Cf. if gotSA, the original forall's induction variable remains that.
  indvars.insertAtHead(new DefExpr(parIdx));

  // FLAG_INDEX_OF_INTEREST is needed in setConstFlagsAndCheckUponMove():
  parIdx->addFlag(FLAG_INDEX_OF_INTEREST);
  parIdx->addFlag(FLAG_INSERT_AUTO_DESTROY);

  followIdx->addFlag(FLAG_INDEX_OF_INTEREST);
  followIdx->addFlag(FLAG_INDEX_VAR);
  //followIdx->addFlag(FLAG_INSERT_AUTO_DESTROY);

  INT_ASSERT(fs->numInductionVars() == 1);
}

static void checkForNonIterator(IteratorGroup* igroup, bool gotSA,
                                CallExpr* parCall)
{
  if (gotSA ? igroup->noniterSA : igroup->noniterL) {
    FnSymbol* dest = parCall->resolvedFunction();
    USR_FATAL_CONT(parCall, "The iterable-expression resolves to a non-iterator function '%s' when looking for a parallel iterator", dest->name);
    USR_PRINT(dest, "The function '%s' is declared here", dest->name);
    USR_STOP();
  }
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
  if (idxVar->id == breakOnResolveID) gdbShouldBreakHere();

  idxVar->type = iType.type();
  idxVar->qual = iType.getQual();
}

static Expr* rebuildIterableCall(ForallStmt* pfs,
                                 CallExpr* iterCall,
                                 Expr* origExprFlw)
{
  INT_ASSERT(iterCall == pfs->firstIteratedExpr()); // still here?

  int origLength = pfs->iteratedExpressions().length;
  if (origLength == 1) {
    INT_ASSERT(!pfs->zippered());
    // no tuple building here
    return origExprFlw;
  }

  CallExpr* result = new CallExpr("_build_tuple", origExprFlw);
  while (Expr* curr = iterCall->next)
    result->insertAtTail(curr->remove());

  // todo: remove the assert and origLength
  INT_ASSERT(result->numActuals() == origLength);
  return result;
}

static void buildLeaderLoopBody(ForallStmt* pfs, Expr* iterExpr) {
  VarSymbol* leadIdxCopy = parIdxVar(pfs);
  bool       zippered    = false;
  if (CallExpr* buildTup = toCallExpr(iterExpr)) {
    INT_ASSERT(buildTup->isNamed("_build_tuple"));
    if (buildTup->numActuals() > 1)
      zippered = true;
  }

  DefExpr*  followIdxDef = toDefExpr(pfs->loopBody()->body.head->remove());
  VarSymbol*   followIdx = toVarSymbol(followIdxDef->sym);
  BlockStmt*    userBody = toBlockStmt(pfs->loopBody()->body.tail->remove());
  INT_ASSERT(pfs->loopBody()->body.empty());

  BlockStmt* preFS           = new BlockStmt(BLOCK_SCOPELESS);
  // cf in build.cpp: new ForLoop(leadIdx, leadIter, NULL, zippered)
  BlockStmt* leadForLoop     = pfs->loopBody();

  VarSymbol* iterRec         = newTemp("chpl__iterLF"); // serial iter, LF case
  VarSymbol* followIter      = newTemp("chpl__followIter");
  BlockStmt* followBlock     = NULL;

  iterRec->addFlag(FLAG_NO_COPY);
  iterRec->addFlag(FLAG_EXPR_TEMP);
  iterRec->addFlag(FLAG_CHPL__ITER);
  iterRec->addFlag(FLAG_CHPL__ITER_NEWSTYLE);

  preFS->insertAtTail(new DefExpr(iterRec));
  preFS->insertAtTail(new CallExpr(PRIM_MOVE, iterRec, iterExpr));
  Expr* toNormalize = preFS->body.tail;

  followBlock = buildFollowLoop(iterRec,
                                leadIdxCopy,
                                followIter,
                                followIdx,
                                userBody,
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
    BlockStmt* userBodyForFast = userBody->copy(&map);

    fastFollowBlock = buildFollowLoop(iterRec,
                                      leadIdxCopy,
                                      fastFollowIter,
                                      fastFollowIdx,
                                      userBodyForFast,
                                      pfs,
                                      true,
                                      zippered);

    leadForLoop->insertAtTail(new CondStmt(new SymExpr(T2), fastFollowBlock, followBlock));
  } else {
    leadForLoop->insertAtTail(followBlock);
  }

  pfs->insertBefore(preFS);
  normalize(toNormalize); // requires inTree()
  resolveBlockStmt(preFS);
  preFS->flattenAndRemove();
}

// see also comments above
CallExpr* resolveForallHeader(ForallStmt* pfs, SymExpr* origSE)
{
  CallExpr* retval = NULL;

  if (pfs->id == breakOnResolveID) gdbShouldBreakHere();

  // We only get here for origSE==firstIteratedExpr() .
  // If at that time there are other elements in iterExprs(), we remove them.
  INT_ASSERT(origSE == pfs->firstIteratedExpr());

  FnSymbol* origTarget = NULL; //for assertions
  CallExpr* iterCall = buildForallParIterCall(pfs, origSE, origTarget);

  // So we know where iterCall is.
  INT_ASSERT(iterCall         == pfs->firstIteratedExpr());
  INT_ASSERT(origSE->inTree() == false);

  // gotSA=true when we end up with standalone, original, or serial iter. 
  bool gotSA = acceptUnmodifiedIterCall(pfs, iterCall) ||
               findStandaloneOrLeader(pfs, iterCall, origSE, origTarget);
  resolveCallAndCallee(iterCall, false);

  FnSymbol* origIterFn = iterCall->resolvedFunction();

  if (!tryFailure && origTarget) {
    IteratorGroup* igroup = origTarget->iteratorGroup;
    checkForNonIterator(igroup, gotSA, iterCall);

    if (origTarget == origIterFn) {
      INT_ASSERT(gotSA);
      INT_ASSERT(pfs->allowSerialIterator());
      INT_ASSERT(origIterFn == igroup->serial);
    } else if (gotSA) {
      INT_ASSERT(origIterFn == igroup->standalone);
    } else {
      INT_ASSERT(origIterFn == igroup->leader);
    }
  }

  // ex. resolving the par iter failed and 'pfs' is under "if chpl__tryToken"
  if (tryFailure == false)
  {
    addParIdxVarsAndRestruct(pfs, gotSA);

    resolveParallelIteratorAndIdxVar(pfs, iterCall, origIterFn, gotSA);

    setupAndResolveShadowVars(pfs);

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
//   ForallStmt lowering 1   //
//                           //
///////////////////////////////

// The fRecIter* fields:
//   fRecIterIRdef, fRecIterICdef, fRecIterGetIterator, fRecIterFreeIterator
// are used to fall back to the "old" lowering of ForallStmts
// based on the iterator record/iterator class.
// We set them up here so that calls like _getIterator can be resolved.
//
// This fallback is used only when the parallel iterator is recursive.
// Otherwise the iterator is simply inlined by
//   lowerForallStmtsInline() / lowerOneForallStmt()
//
// Recursive-ness is detected later by
//   computeRecursiveIteratorSet() / find_recursive_caller().
// Since we do not know it here, we do the work
// even in the (common) case where it will not be needed.
//
void static setupRecIterFields(ForallStmt* fs)
{
  CallExpr* parIterCall = toCallExpr(fs->firstIteratedExpr());
  INT_ASSERT(parIterCall && !parIterCall->next); // expected
  SET_LINENO(parIterCall);

  // From the original buildStandaloneForallLoopStmt(), with "sa" -> "par".
  VarSymbol* iterRec = newTemp("chpl__iterPAR"); // serial iter, PAR case
  VarSymbol* parIter = newTemp("chpl__parIter");
  VarSymbol* parIdx  = parIdxVar(fs);

  iterRec->addFlag(FLAG_NO_COPY);
  iterRec->addFlag(FLAG_CHPL__ITER);
  iterRec->addFlag(FLAG_CHPL__ITER_NEWSTYLE);
  iterRec->addFlag(FLAG_MAYBE_REF);
  iterRec->addFlag(FLAG_EXPR_TEMP);

  parIter->addFlag(FLAG_EXPR_TEMP);
  // Too late to do it here - it's needed in setConstFlagsAndCheckUponMove().
  //parIdx->addFlag(FLAG_INDEX_OF_INTEREST);
  parIdx->addFlag(FLAG_INDEX_VAR);

  BlockStmt* holder = new BlockStmt();
  fs->insertBefore(holder);  // so we can resolve it

  DefExpr*   recIterIRdef = new DefExpr(iterRec);
  DefExpr*   recIterICdef = new DefExpr(parIter);
  CallExpr*  recIterGetIterator  = new CallExpr("_getIterator", iterRec);
  CallExpr*  recIterFreeIterator = new CallExpr("_freeIterator", parIter);
  
  CallExpr* initIterRec = new CallExpr(PRIM_MOVE, iterRec, parIterCall->copy());
  CallExpr* initParIter = new CallExpr(PRIM_MOVE, parIter, recIterGetIterator);

  holder->insertAtTail(recIterIRdef);
  holder->insertAtTail(recIterICdef);
  holder->insertAtTail(initIterRec);
  holder->insertAtTail(initParIter);
  holder->insertAtTail(recIterFreeIterator);

  // This may not resolve if postponed until lowerIterators.
  resolveBlockStmt(holder);

  fs->fRecIterIRdef        = recIterIRdef;
  fs->fRecIterICdef        = recIterICdef;
  fs->fRecIterGetIterator  = recIterGetIterator;
  fs->fRecIterFreeIterator = recIterFreeIterator;

  Symbol* PS = fs->parentSymbol;
  recIterIRdef       ->remove();  insert_help(recIterIRdef,        fs, PS);
  recIterICdef       ->remove();  insert_help(recIterICdef,        fs, PS);
  recIterGetIterator ->remove();  insert_help(recIterGetIterator,  fs, PS);
  recIterFreeIterator->remove();  insert_help(recIterFreeIterator, fs, PS);

  initParIter->remove();
  // This call messes up doNotTransformForForall() in callDestructors.
  // Remove it until we need it, if at all.
  initIterRec->remove();

  INT_ASSERT(holder->body.empty());
  holder->remove();
}

void resolveForallStmts1() {
  forv_Vec(ForallStmt, fs, gForallStmts) {
    if (!fs->inTree() || !fs->getFunction()->isResolved())
      continue;
    setupRecIterFields(fs);
  }
}


///////////////////////////////
//                           //
//   ForallStmt lowering 2   //
//                           //
///////////////////////////////

// These actuals have been added to handle outer variables in LoopExpr's body.
// The leader iterator neither accepts nor handles them. So drop them.
static void removeOuterVarArgs(CallExpr* iterCall, FnSymbol* oldCallee,
                               FnSymbol* newCallee) {
  int numFormals = newCallee->numFormals();
  int numActuals = iterCall->numActuals();
  INT_ASSERT(numActuals == oldCallee->numFormals());

  if (numFormals == numActuals)
    return; // there were no outer variables, nothing to do

  std::vector<Symbol*> symbols;
  if (fVerify) collectSymbols(oldCallee->body, symbols);

  for (int xtraIdx = numFormals + 1; xtraIdx <= numActuals; xtraIdx++) {
    // Remove the next extra actual.
    iterCall->get(numFormals+1)->remove();

    if (fVerify) {
      // Ensure oldCallee did not use it.
      Symbol* xtraFormal = oldCallee->getFormal(xtraIdx);
      for_vector(Symbol, sym, symbols) INT_ASSERT(sym != xtraFormal);
    }
  }
}

//
// Handle the case where the leader iterator is chpl__loopexpr_iter.
// Not doing so confuses ReturnByRef and lowering of ForallStmts.
//
// Tests:
//   library/packages/Collection/CollectionCounter.chpl
//   library/standard/Random/deitz/test1D2D.chpl
//   reductions/deitz/test_maxloc_reduce_wmikanik_bug2.chpl
//
static void convertIteratorForLoopexpr(ForallStmt* fs) {
  if (CallExpr* iterCall = toCallExpr(fs->iteratedExpressions().head))
    if (SymExpr* calleeSE = toSymExpr(iterCall->baseExpr))
      if (FnSymbol* calleeFn = toFnSymbol(calleeSE->symbol()))
       if (! calleeFn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD))
        if (isLoopExprFun(calleeFn)) {
          // In this case, we have a _toLeader call and no side effects.
          // Just use the iterator corresponding to the iterator record.
          FnSymbol* iterator = getTheIteratorFn(calleeFn->retType);
          SET_LINENO(calleeSE);
          calleeSE->replace(new SymExpr(iterator));
          if (calleeFn->firstSymExpr() == NULL)
            calleeFn->defPoint->remove(); // not needed any more
          removeOuterVarArgs(iterCall, calleeFn, iterator);
          // Adds coercions as needed.
          if (iterCall->get(1)->getValType() != iterator->getFormal(1)->getValType())
            resolveCall(iterCall);
        }
}

// Todo: can we merge this with resolveForallStmts1() ?
void resolveForallStmts2() {
  forv_Vec(ForallStmt, fs, gForallStmts) {
    if (!fs->inTree() || !fs->getFunction()->isResolved())
      continue;

    if (fs->fromReduce()) continue; // not an error

    // formerly nonLeaderParCheckInt()
    FnSymbol* parent = fs->getFunction();
    // If isTaskFun(parent), error is still reported in nonLeaderParCheckInt.
    if (parent->isIterator() && !parent->hasFlag(FLAG_INLINE_ITERATOR))
      USR_FATAL_CONT(fs, "invalid use of parallel construct in serial iterator");
    
    convertIteratorForLoopexpr(fs);
  }
}


///////////////////////////////
//                           //
//   PRIM_REDUCE lowering    //
//                           //
///////////////////////////////

// Insert a call temp. It is simpler than the full-blown normalize()
// and the caller can reference the temp.
static SymExpr* normalizeIITR(Expr* ref, Expr* iitr) {
  VarSymbol* temp = newTemp("iitr_temp");
  temp->addFlag(FLAG_TYPE_VARIABLE);
  ref->insertBefore(new DefExpr(temp));
  ref->insertBefore("'move'(%S,%E)", temp, iitr);
  return new SymExpr(temp);
}

// Given a reduce expression like "op reduce data", return op(inputType).
// inputType is the type of things being reduced - when iterating over 'data'.
// This matches the case where inputType is provided by the user.
static Expr* lowerReduceOp(Expr* ref, SymExpr* opSE, SymExpr* dataSE,
                           bool zippered)
{
  CallExpr* iit = NULL;
  if (zippered) {
    // Cf. destructZipperedIterables. 'zipcall' will be removed there.
    CallExpr* zipcall = toCallExpr(getDefOfTemp(dataSE)->copy());
    INT_ASSERT(zipcall->isPrimitive(PRIM_ZIP));
    iit = new CallExpr("iteratorIndexTypeZip");
    for_actuals(actual, zipcall)
      iit->insertAtTail(toSymExpr(actual)->symbol());
  } else {
    iit = new CallExpr("iteratorIndexType", dataSE->symbol());
  }

  ref->insertBefore(iit);
  Expr* iitR = resolveExpr(iit)->remove();
  if (!isSymExpr(iitR)) iitR = normalizeIITR(ref, iitR);

  return new CallExpr(opSE, iitR);
}

// Within resolveBlockStmt / for_exprs_postorder framework,
// we need to lower PRIM_REDUCE prior to the resolveCall()
// that gets invoked from resolveExpr(). That way the
// ForallStmt plus scaffolding that lowerPrimReduce() injects
// can come after 'retval'. resolveCall() does not support that.

void lowerPrimReduce(CallExpr* call, Expr*& retval) {
  if (call->id == breakOnResolveID) gdbShouldBreakHere();

  Expr*   callStmt = call->getStmtExpr();
  CallExpr*   noop = new CallExpr(PRIM_NOOP);
  callStmt->insertBefore(noop);

  SymExpr*   opSE = toSymExpr(call->get(1)->remove());           // 1st arg
  SymExpr* dataSE = toSymExpr(call->get(1)->remove());           // 2nd arg
  bool   zippered = toSymExpr(call->get(1))->symbol() == gTrue;  // 3rd arg
  bool  reqSerial = false; // We may need it for #11819, otherwise remove it.

  Expr* opExpr = lowerReduceOp(callStmt, opSE, dataSE, zippered);

  VarSymbol* result = newTemp("chpl_redResult");
  callStmt->insertBefore(new DefExpr(result));

  VarSymbol*       idx  = newTemp("chpl_redIdx");
  ShadowVarSymbol* svar = new ShadowVarSymbol(TFI_REDUCE, "chpl_redSVar",
                                              new SymExpr(result), opExpr);
  ForallStmt*      fs   = ForallStmt::fromReduceExpr(idx, dataSE, svar,
                                                     zippered, reqSerial);
  callStmt->insertBefore(fs);
  call->replace(new SymExpr(result));
  retval = noop;
}
