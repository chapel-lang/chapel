/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include "LoopStmt.h"
#include "view.h"
#include "ForallStmt.h"
#include "stlUtil.h"
#include "astutil.h"
#include "resolution.h"

static void analyzeArrLog(const char *msg, BaseAST *node) {
  if (fAutoLocalAccessLog) {
    const bool verbose = (node->getModule()->modTag != MOD_INTERNAL &&
                          node->getModule()->modTag != MOD_STANDARD);

    const bool veryVerbose = false;
    if (verbose) {
      std::cout << msg << std::endl;
      if (node != NULL) {
        std::cout << "\t" << node->stringLoc() << std::endl;
        if (veryVerbose) {
          nprint_view(node);
        }
        else {
          std::cout << std::endl;
        }
      }
    }
  }
}

static bool callHasSymArguments(CallExpr *ce, const std::vector<Symbol *> &syms) {
  if (ce->argList.length != syms.size()) return false;
  for (int i = 0 ; i < ce->argList.length ; i++) {
    if (SymExpr *arg = toSymExpr(ce->get(i+1))) {
      if (arg->symbol() != syms[i]) {
        return false;
      }
    }
    else {
      return false;
    }
  }
  return true;
}

static Symbol *getDotDomBaseSym(Expr *expr) {
  if (CallExpr *ce = toCallExpr(expr)) {
    if (ce->isNamedAstr(astrSdot)) {
      if (SymExpr *se = toSymExpr(ce->get(2))) {
        if (VarSymbol *var = toVarSymbol(se->symbol())) {
          if (var->immediate->const_kind == CONST_KIND_STRING) {
            if (strcmp(var->immediate->v_string, "_dom") == 0) {
              if (SymExpr *se = toSymExpr(ce->get(1))) {
                return se->symbol();
              }
            }
          }
        }
      }
    }
  }
  return NULL;
}

static Symbol *getDomSym(Symbol *arrSym) {
  Symbol *ret = NULL;
  if(DefExpr *def = arrSym->defPoint) {  // TODO: what happens if ArgSymbol?
    // check the most basic idiom `var A: [D] int`
    if (def->exprType != NULL) {
      if (CallExpr *ceOuter = toCallExpr(def->exprType)) {
        if (ceOuter->isNamed("chpl__buildArrayRuntimeType")) {
          if (CallExpr *ceInner = toCallExpr(ceOuter->get(1))) {
            if (ceInner->isNamed("chpl__ensureDomainExpr")) {
              Expr *arg = ceInner->get(1);
              if (SymExpr *domSE = toSymExpr(arg)) {
                ret = domSE->symbol();
              }
              else if(Symbol *dotDomBaseSym = getDotDomBaseSym(arg)) {
                ret = getDomSym(dotDomBaseSym); // recurse
              }
            }
          }
        }
      }
    }
    // check if the array variable was created with a call `var A = foo()`
    else { // def->exprType == NULL
      analyzeArrLog("There is no type expression in definition, or it is not a call", def);
    }
  }
  if (ret == NULL) {
      analyzeArrLog("Regular domain symbol was not found", arrSym);
  }
  return ret;
}

// return the closest parent of `ce` that can impact locality
static Stmt *getLocalityDominator(CallExpr* ce) {
  Expr *cur = ce->parentExpr;
  while (cur != NULL) {
    if (BlockStmt *block = toBlockStmt(cur)) {
      if (!block->isRealBlockStmt()) { // check for on statement
        return block;
      }
    }
    if (ForallStmt *forall = toForallStmt(cur)) {
      return forall;
    }
    cur = cur->parentExpr;
  }
  return NULL;
}

// check whether an expression is used to assign a subindex. e.g.
//
//   forall (i, j) in D
//
// will have in the beginning of its body assignments to i and j from an
// "indexOfInterest" that represents that tuple in a single symbol
static bool isSubIndexAssignment(Expr *expr,
                                 Symbol *subIndex,
                                 int indexIndex,
                                 Symbol *indexBundle) {
  if (CallExpr *moveCE = toCallExpr(expr)) {
    if (moveCE->isPrimitive(PRIM_MOVE)) {
      if (SymExpr *moveDstSE = toSymExpr(moveCE->get(1))) {
        if (CallExpr *moveSrcCE = toCallExpr(moveCE->get(2))) {
          if (SymExpr *moveSrcBaseSE = toSymExpr(moveSrcCE->baseExpr)) {
            if (SymExpr *moveSrcIdxSE = toSymExpr(moveSrcCE->get(1))) {
              if (VarSymbol *moveSrcIdxVS = toVarSymbol(moveSrcIdxSE->symbol())) {
                return (moveDstSE->symbol() == subIndex) &&
                       ( moveSrcBaseSE->symbol() == indexBundle &&
                         moveSrcIdxVS->immediate->int_value() == indexIndex );
              }
            }
          }
        }
      }
    }
  }
  return false;
}

static std::vector<Symbol *> getLoopIndexSymbols(ForallStmt *forall,
                                                 Symbol *baseSym) {
  std::vector<Symbol *> indexSymbols;
  int indexVarCount = -1;
  int bodyExprCount = 1;

  AList &bodyExprs = forall->loopBody()->body;

  if (CallExpr *firstCall = toCallExpr(bodyExprs.get(bodyExprCount++))) {
    if (firstCall->isNamed("_check_tuple_var_decl")) {
      if (SymExpr *firstArgSE = toSymExpr(firstCall->get(1))) {
        if (firstArgSE->symbol() == baseSym) {
          if (SymExpr *secondArgSE = toSymExpr(firstCall->get(2))) {
            if (VarSymbol *vs = toVarSymbol(secondArgSE->symbol())) {
              indexVarCount = vs->immediate->int_value();
            }
          }
        }
      }
    }
  }

  if (indexVarCount != -1) {
    INT_ASSERT(indexVarCount > 0);

    for (int i = 0 ; i < indexVarCount ; i++) {
      if (DefExpr *indexDE = toDefExpr(bodyExprs.get(bodyExprCount++))) {
        Symbol *indexSym = indexDE->sym;
        if(isSubIndexAssignment(bodyExprs.get(bodyExprCount++),
                                indexSym, i, baseSym)) {
          indexSymbols.push_back(indexSym);
        }
      }
    }
  }

  if (indexVarCount == -1 || indexVarCount != indexSymbols.size()) {
    analyzeArrLog("Can't recognize loop's index symbols", baseSym);
    indexSymbols.clear();
  }

  return indexSymbols;
}

static void gatherForallInfo(ForallStmt *forall) {

  Symbol *loopIdxSym = NULL;

  AList &iterExprs = forall->iteratedExpressions();
  AList &indexVars = forall->inductionVariables();

  if (iterExprs.length == 1 && indexVars.length == 1) {  // limit to 1 for now
    if (isUnresolvedSymExpr(iterExprs.head) || isSymExpr(iterExprs.head)) {
      if (SymExpr *iterSE = toSymExpr(iterExprs.head)) {
        forall->loopInfo.iterSym = iterSE->symbol();
        analyzeArrLog("Iterated symbol", forall->loopInfo.iterSym);
      }
      else {
        analyzeArrLog("Iterated Expr is unresolved", iterExprs.head);
      }
    }
    // it might be in the form `A.domain` where A is used in the loop body
    else if (Symbol *dotDomBaseSym = getDotDomBaseSym(iterExprs.head)) {
      forall->loopInfo.dotDomIterExpr = iterExprs.head;
      forall->loopInfo.dotDomIterSym = dotDomBaseSym;
      forall->loopInfo.dotDomIterSymDom = getDomSym(forall->loopInfo.dotDomIterSym);
      analyzeArrLog("Iterated over .domain of", forall->loopInfo.dotDomIterSym);
      if (forall->loopInfo.dotDomIterSymDom != NULL) {
        analyzeArrLog("where its domain is", forall->loopInfo.dotDomIterSymDom);
      }
      else {
        analyzeArrLog("Whose domain cannot be determined statically", forall->loopInfo.dotDomIterSym);
      }
    }

    if (forall->loopInfo.iterSym != NULL || forall->loopInfo.dotDomIterSym != NULL) {
      // the iterator is something we can optimize
      // now check the induction variables
      if (SymExpr* se = toSymExpr(indexVars.head)) {
        loopIdxSym = se->symbol();
      }
      else if (DefExpr* de = toDefExpr(indexVars.head)) {
        loopIdxSym = de->sym;
      }
      else {
        analyzeArrLog("Unrecognized index symbol", indexVars.head);
      }

      if (loopIdxSym->hasFlag(FLAG_INDEX_OF_INTEREST)) {
        forall->loopInfo.multiDIndices = getLoopIndexSymbols(forall, loopIdxSym);
      }
      else {
        forall->loopInfo.multiDIndices.push_back(loopIdxSym);
      }
    }
  }
}

static bool hasReduceIntentShadowVars(ForallStmt *forall) {
  // We never seem to use `temp`, why is it in the interface of this macro?
  for_shadow_vars(svar, temp, forall) {
    if (svar->isReduce()) {
      return true;
    }
  }
  return false;
}

static bool checkLoopSuitableForOpt(ForallStmt *forall) {

  // reduce-intent variables expect some special AST form that this optimization
  // somehow breaks
  if (hasReduceIntentShadowVars(forall)) {
    return false;
  }
  if (forall->loopInfo.multiDIndices.size() == 0) {
    return false;
  }

  INT_ASSERT(forall->loopInfo.iterSym != NULL ||
             forall->loopInfo.dotDomIterSym != NULL);
  return true;
}

static Symbol *getCallBaseSymIfSuitable(CallExpr *call, ForallStmt *forall) {
  
  // TODO see if you can use getCallBase
  SymExpr *baseSE = toSymExpr(call->baseExpr);

  if (baseSE != NULL) {
    Symbol *accBaseSym = baseSE->symbol();

    // (i,j) in forall (i,j) in bla is a tuple that is
    // index-by-index accessed in loop body that throw off this
    // analysis
    if (accBaseSym->hasFlag(FLAG_INDEX_OF_INTEREST)) { return NULL; }

    // give up if the symbol we are looking to optimize is defined
    // inside the loop itself
    if (forall->loopBody()->contains(accBaseSym->defPoint)) { return NULL; }

    // give up if the access uses a different symbol
    if (!callHasSymArguments(call, forall->loopInfo.multiDIndices)) { return NULL; }

    // this call has another tighter-enclosing stmt that may change
    // locality, don't optimize
    if (forall != getLocalityDominator(call)) { return NULL; }

    return accBaseSym;
  }
  return NULL;
}

static Symbol *getCallBase(CallExpr *call) {

  SymExpr *baseSE = toSymExpr(call->baseExpr);
  if (baseSE != NULL) {
    return baseSE->symbol();
  }
  return NULL;
}

// access CallExprs for each check added
std::map<CallExpr *, std::vector<CallExpr *>> accessForStaticCheckMap;
std::map<CallExpr *, std::vector<CallExpr *>> accessForDynamicCheckMap;


static void generateCheckForAccess(CallExpr *access,
                                   ForallStmt *forall,
                                   CallExpr *&allChecks,
                                   bool isStatic) {

  ForallOptimizationInfo &loopInfo = forall->loopInfo;

  Symbol *baseSym = getCallBase(access);
  INT_ASSERT(baseSym);

  CallExpr *currentCheck = NULL;
  if (isStatic) {
    if (loopInfo.staticCheckForSymMap.count(baseSym) == 0) {
      currentCheck = new CallExpr("chpl__staticAutoLocalCheck");
      loopInfo.staticCheckForSymMap[baseSym] = currentCheck;
    }
    else {
      return;
    }
  }
  else {
    if (loopInfo.dynamicCheckForSymMap.count(baseSym) == 0) {
      currentCheck = new CallExpr("chpl__dynamicAutoLocalCheck");
      loopInfo.dynamicCheckForSymMap[baseSym] = currentCheck;
    }
    else {
      return;
    }
  }


  currentCheck->insertAtTail(baseSym);
  if (loopInfo.iterSym != NULL) {
    currentCheck->insertAtTail(new SymExpr(loopInfo.iterSym));
  }
  else if (loopInfo.dotDomIterExpr != NULL) {
    currentCheck->insertAtTail(loopInfo.dotDomIterExpr->copy());
  }
  else {
    INT_FATAL("loopInfo didn't have enough information");
  }

  if (allChecks == NULL) {
    allChecks = currentCheck;
  }
  else {
    allChecks = new CallExpr(PRIM_AND, currentCheck, allChecks);
  }
}

bool adjustAutoLocalAccessDynamic(CallExpr *call) {
  if (call->maybeLocalAccess) {
    analyzeArrLog("Replacing with runtime checks", call);
    call->baseExpr->replace(new UnresolvedSymExpr("localAccess"));
    call->maybeLocalAccess = false; // no longer a "maybe"
    return true;
  }
  else {
    return false;
  }
}

static void revertAccess(CallExpr *call) {
  analyzeArrLog("Reverting optimization (can't statically confirm)",
                call);

  call->baseExpr->replace(new UnresolvedSymExpr("this"));
  resolveCall(call);
}

static void confirmAccess(CallExpr *call) {
  analyzeArrLog("Statically confirmed optimization, using localAccess",
                call);

  call->baseExpr->replace(new UnresolvedSymExpr("localAccess"));
  resolveCall(call);
}

static bool adjustAutoLocalAccessesBasedOnStaticCheck(CallExpr *check,
                      std::vector<CallExpr *> knownCallExprs,
                      bool confirmed) {

  bool madeAdjustments = false;

  if (knownCallExprs.size() > 0) {
    for_vector(CallExpr, knownCallExpr, knownCallExprs) {
      if(CallExpr *callToAdjust = toCallExpr(knownCallExpr->baseExpr)) {
        if (confirmed) {
          // can only confirm a static access
          if (callToAdjust->isNamed("chpl_maybeLocalAccessStatic")) {
            confirmAccess(callToAdjust);
            madeAdjustments = true;
          }
        }
        else {
          revertAccess(callToAdjust);
          madeAdjustments = true;
        }
      }
    }
  }
  else { // this may be inside an instantitation of a generic function

    // find the first conditional that comes after this check
    if (CallExpr *parentCall = toCallExpr(check->parentExpr)) {
      if (parentCall->isPrimitive(PRIM_MOVE)) {
        Expr *curExpr = parentCall->next;
        CondStmt *optCond = NULL;
        while(curExpr != NULL && optCond == NULL) {
          optCond = toCondStmt(curExpr);
          curExpr = curExpr->next;
        }

        if (optCond != NULL) {
          std::vector<CallExpr *> callExprs;
          collectCallExprs(optCond->thenStmt, callExprs);

          SymExpr *checkSE = toSymExpr(check->get(1));
          Symbol *checkSym = checkSE->symbol();
          // maybe assert here

          // remove statically-determined accesses based on this check
          for_vector(CallExpr, parentCall, callExprs) {
            if(CallExpr *callToAdjust = toCallExpr(parentCall->baseExpr)) {
              if (callToAdjust->isNamed("chpl_maybeLocalAccessStatic")) {
                // check the second argument; 1st is methodToken
                if (SymExpr *argSE = toSymExpr(callToAdjust->get(2)) ) {
                  if (argSE->symbol() == checkSym) {
                    if (confirmed) {
                      confirmAccess(callToAdjust);
                      madeAdjustments = true;
                    }
                    else {
                      revertAccess(callToAdjust);
                      madeAdjustments = true;
                    }
                  }
                }
              }
              else if (callToAdjust->isNamed("chpl_maybeLocalAccessDynamic")) { 
                // can only revert for dynamic accesses
                if (!confirmed) {
                  // check the second argument; 1st is methodToken
                  if (SymExpr *argSE = toSymExpr(callToAdjust->get(2))) {
                    if(argSE->symbol() == checkSym) {
                      revertAccess(callToAdjust);
                      madeAdjustments = true;
                    }
                  }
                }
              }
              else if (!confirmed && parentCall->isPrimitive(PRIM_MOVE)) {
                if (CallExpr *callToAdjust = toCallExpr(parentCall->get(2))) {
                  if (callToAdjust->isNamed("chpl_dynamicAutoLocalCheck")) {
                    if (SymExpr *argSE = toSymExpr(callToAdjust->get(1))) {
                      if(argSE->symbol() == checkSym) {
                        callToAdjust->replace(new SymExpr(gTrue));
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  return madeAdjustments;
}

// This is the part of the "public" interface for resolution
bool adjustAutoLocalAccessStatic(CallExpr *call, Immediate *imm) {
  bool madeAdjustments = false;
  if (call->isNamed("chpl__staticAutoLocalCheck")) {
    bool retval = imm->bool_value();

    madeAdjustments =  adjustAutoLocalAccessesBasedOnStaticCheck(call,
                                                  accessForStaticCheckMap[call],
                                                  retval);

    if (retval == false && madeAdjustments == false) {
      analyzeArrLog("Something went wrong with this check", call);
      // maybe assert here: if this was a static check that was reverted, there
      // must be something associated with it that required adjustment
    }
    
  }
  return madeAdjustments;
}

static void generateDynamicCheckForAccess(CallExpr *access,
                                          ForallStmt *forall,
                                          CallExpr *&allChecks) {
  generateCheckForAccess(access, forall, allChecks, false);
}

static void generateStaticCheckForAccess(CallExpr *access,
                                         ForallStmt *forall,
                                         CallExpr *&allChecks) {
  generateCheckForAccess(access, forall, allChecks, true);
}


static void buildLocalAccessLoops(ForallStmt *forall,
                                  std::vector<CallExpr *> &sOptCandidates,
                                  std::vector<CallExpr *> &dOptCandidates) {

  ForallOptimizationInfo &loopInfo = forall->loopInfo;

  const int totalNumCandidates = sOptCandidates.size() + dOptCandidates.size();
  if (totalNumCandidates == 0) return;

  SET_LINENO(forall);


  // ENGIN: Below, there are two anonymous blocks to keep scopes for static and
  // dynamic checking separate. They are similar but not similar enough to
  // refactor, IMO So, I use same-named variables in separate blocks,
  // technically we shouldn't need them, but I believe it is good practice.

  { //  all static optimization related stuff is here
  
    CallExpr *staticCond = NULL;
    for_vector(CallExpr, sOptCandidate, sOptCandidates) {
      generateStaticCheckForAccess(sOptCandidate, forall, staticCond);
    }

    // we need static checks for dynamic candidates, too
    for_vector(CallExpr, dOptCandidate, dOptCandidates) {
      generateStaticCheckForAccess(dOptCandidate, forall, staticCond);
    }

    ForallStmt *forallUnopt = forall->copy();
    forallUnopt->autoLocalAccessChecked = true; // am I being paranoid?

    BlockStmt *thenBlock = new BlockStmt();
    BlockStmt *elseBlock = new BlockStmt();
    CondStmt *staticCheck = new CondStmt(staticCond, thenBlock,
                                                     elseBlock);
    forall->insertAfter(staticCheck);
    thenBlock->insertAtTail(forall->remove());
    elseBlock->insertAtTail(forallUnopt);

    // this modifies calls in `forall`
    for_vector(CallExpr, sOptCandidate, sOptCandidates) {
      SET_LINENO(sOptCandidate);
      analyzeArrLog("\tReplacing", sOptCandidate);

      Symbol *baseSym = toSymExpr(sOptCandidate->baseExpr)->symbol();
      INT_ASSERT(baseSym);

      CallExpr *base = new CallExpr(".", new SymExpr(baseSym),
          new UnresolvedSymExpr("chpl_maybeLocalAccessStatic"));
      CallExpr *repl = new CallExpr(base);
      for (int i = 1 ; i <= sOptCandidate->argList.length ; i++) {
        Symbol *argSym = toSymExpr(sOptCandidate->get(i))->symbol();
        INT_ASSERT(argSym);

        repl->insertAtTail(new SymExpr(argSym));
      }
      sOptCandidate->replace(repl);
      accessForStaticCheckMap[loopInfo.staticCheckForSymMap[baseSym]].push_back(repl);
    }
  }

  { // all dynamic optimization related stuff is here
    if (dOptCandidates.size() > 0) {

      CallExpr *dynamicCond = NULL;

      ForallStmt *forallUnopt = forall->copy();
      forallUnopt->autoLocalAccessChecked = true;

      // this marks calls in `forall`
      for_vector(CallExpr, dOptCandidate, dOptCandidates) {
        SET_LINENO(dOptCandidate);
        Symbol *callBase = getCallBase(dOptCandidate);
        generateDynamicCheckForAccess(dOptCandidate, forall, dynamicCond);
        analyzeArrLog("\tMarking for dynamic analysis", dOptCandidate);
        CallExpr *base = new CallExpr(".", new SymExpr(callBase),
            new UnresolvedSymExpr("chpl_maybeLocalAccessDynamic"));
        CallExpr *repl = new CallExpr(base);
        for (int i = 1 ; i <= dOptCandidate->argList.length ; i++) {
          Symbol *argSym = toSymExpr(dOptCandidate->get(i))->symbol();
          INT_ASSERT(argSym);

          repl->insertAtTail(new SymExpr(argSym));
        }
        dOptCandidate->replace(repl);
        accessForDynamicCheckMap[loopInfo.dynamicCheckForSymMap[callBase]].push_back(repl);
        accessForStaticCheckMap[loopInfo.staticCheckForSymMap[callBase]].push_back(repl);
      }

      BlockStmt *thenBlock = new BlockStmt();
      BlockStmt *elseBlock = new BlockStmt();
      CondStmt *dynamicCheck = new CondStmt(dynamicCond, thenBlock,
                                                         elseBlock);
      forall->insertAfter(dynamicCheck);
      thenBlock->insertAtTail(forall->remove());
      elseBlock->insertAtTail(forallUnopt);
    }
  }
}

void analyzeArrays() {
  const bool limitToTestFile = false;
  forv_Vec(ForallStmt, forall, gForallStmts) {
    if (forall->autoLocalAccessChecked) {
      continue;
    }
    else {
      forall->autoLocalAccessChecked = true;
    }

    const bool fileCheck = strncmp(forall->astloc.filename,
           "/Users/ekayraklio/code/chapel/versions/f03/chapel/arrayTest.chpl",
           64) == 0;
    if ((!limitToTestFile) || fileCheck) {
      analyzeArrLog("**** Start forall ****", forall);
      gatherForallInfo(forall);

      if (!checkLoopSuitableForOpt(forall)) {
        analyzeArrLog("**** End forall ****", forall);
        continue;
      }
      else {
        analyzeArrLog("Loop is suitable for further analysis", forall);
      }

      std::vector<CallExpr *> allCallExprs;
      collectCallExprs(forall->loopBody(), allCallExprs);

      // TODO these two can be replaced by the maps
      std::vector<CallExpr *> staticOptimizationCandidates;
      std::vector<CallExpr *> dynamicOptimizationCandidates;

      for_vector(CallExpr, call, allCallExprs) {
        if (Symbol *accBaseSym = getCallBaseSymIfSuitable(call, forall)) {
                                                    
          analyzeArrLog("Potential access", call);

          bool canOptimize = false;
          // check for different patterns
          // forall i in A.domain do ... A[i] ...
          if (forall->loopInfo.dotDomIterSym != NULL &&
              forall->loopInfo.dotDomIterSym == accBaseSym) {
            canOptimize = true;
            analyzeArrLog("Access base is the same as iterator's base",
                call);
          }

          // if that didn't work...
          if (!canOptimize) {
            Symbol *domSym = getDomSym(accBaseSym);

            if (domSym != NULL) {  //  I can find the domain of the array
              // forall i in A.domain do ... B[i] ... where B and A share domain
              if (forall->loopInfo.dotDomIterSymDom != NULL &&
                  forall->loopInfo.dotDomIterSymDom == domSym) {
                canOptimize = true;
                analyzeArrLog("Access base share the domain with iterator's base",
                    call);
              }
              // forall i in D do ... A[i] ... where D is A's domain
              else {
                analyzeArrLog("\twith DefExpr", accBaseSym->defPoint);
                if (domSym != NULL) {
                  analyzeArrLog("\twith domain defined at", domSym);
                }
                if (forall->loopInfo.iterSym != NULL &&
                    forall->loopInfo.iterSym == domSym) {
                  canOptimize = true;
                  analyzeArrLog("Access base's domain is the iterator", call);
                }
              }
            }
            else {
              // I couldn't find a domain symbol for this array, but it can
              // still be a candidate for optimization based on analysis at
              // runtime
              dynamicOptimizationCandidates.push_back(call);
            }
          }

          if (canOptimize) {
            staticOptimizationCandidates.push_back(call);
          }
        } // end canOptimize
      }

      buildLocalAccessLoops(forall,
                            staticOptimizationCandidates,
                            dynamicOptimizationCandidates);

      analyzeArrLog("**** End forall ****", forall);
    }
  }
}
