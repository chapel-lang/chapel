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

//
// Normalize support
//

// Return true if `ce`'s arguments are exactly identical to `syms`
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

// Return the symbol `A` from an expression if it is in the form `A.domain`
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

// If `arrSym` is an array symbol, try to return its domain's symbol. return
// NULL if we can't find a domain symbol statically
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

// Return the closest parent of `ce` that can impact locality (forall or on)
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

// baseSym must be the tuple tmp that represents `(i,j)` in `forall (i,j) in D`
static std::vector<Symbol *> getLoopIndexSymbols(ForallStmt *forall,
                                                 Symbol *baseSym) {
  std::vector<Symbol *> indexSymbols;
  int indexVarCount = -1;
  int bodyExprCount = 1;

  AList &bodyExprs = forall->loopBody()->body;

  // fund the check_typle_var_decl and get the size of the tuple
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

    // push every element (subindex) of the index tuple to the vector
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

  // we must have pushed the same number of indexes to the vector as we
  // initially found. Otherwise, clear the vector, which denotes an unknown
  // pattern.
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
        forall->optInfo.iterSym = iterSE->symbol();
        analyzeArrLog("Iterated symbol", forall->optInfo.iterSym);
      }
      else {
        analyzeArrLog("Iterated Expr is unresolved", iterExprs.head);
      }
    }
    // it might be in the form `A.domain` where A is used in the loop body
    else if (Symbol *dotDomBaseSym = getDotDomBaseSym(iterExprs.head)) {
      forall->optInfo.dotDomIterExpr = iterExprs.head;
      forall->optInfo.dotDomIterSym = dotDomBaseSym;
      forall->optInfo.dotDomIterSymDom = getDomSym(forall->optInfo.dotDomIterSym);
      analyzeArrLog("Iterated over .domain of", forall->optInfo.dotDomIterSym);
      if (forall->optInfo.dotDomIterSymDom != NULL) {
        analyzeArrLog("where its domain is", forall->optInfo.dotDomIterSymDom);
      }
      else {
        analyzeArrLog("Whose domain cannot be determined statically", forall->optInfo.dotDomIterSym);
      }
    }

    if (forall->optInfo.iterSym != NULL || forall->optInfo.dotDomIterSym != NULL) {
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
        forall->optInfo.multiDIndices = getLoopIndexSymbols(forall, loopIdxSym);
      }
      else {
        forall->optInfo.multiDIndices.push_back(loopIdxSym);
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
  if (forall->optInfo.multiDIndices.size() == 0) {
    return false;
  }

  INT_ASSERT(forall->optInfo.iterSym != NULL ||
             forall->optInfo.dotDomIterSym != NULL);
  return true;
}

// Bunch of checks to see if `call` is a candidate for optimization within
// `forall`. Returns the symbol of the `baseExpr` of the `call` if it is
// suitable. NULL otherwise
static Symbol *getCallBaseSymIfSuitable(CallExpr *call, ForallStmt *forall) {
  
  // TODO see if you can use getCallBase
  SymExpr *baseSE = toSymExpr(call->baseExpr);

  if (baseSE != NULL) {
    Symbol *accBaseSym = baseSE->symbol();

    // Prevent making changes to `new C[i]`
    if (CallExpr *parentCall = toCallExpr(call->parentExpr)) {
      if (parentCall->isPrimitive(PRIM_NEW)) { return NULL; } 
    }

    // give up if the access uses a different symbol
    if (!callHasSymArguments(call, forall->optInfo.multiDIndices)) { return NULL; }

    // (i,j) in forall (i,j) in bla is a tuple that is index-by-index accessed
    // in loop body that throw off this analysis
    if (accBaseSym->hasFlag(FLAG_INDEX_OF_INTEREST)) { return NULL; }

    // give up if the symbol we are looking to optimize is defined inside the
    // loop itself
    if (forall->loopBody()->contains(accBaseSym->defPoint)) { return NULL; }

    // similarly, give up if the base symbol is a shadow variable
    if (isShadowVarSymbol(accBaseSym)) { return NULL; }

    // this call has another tighter-enclosing stmt that may change locality,
    // don't optimize
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

// for a call like `A[i]`, this will create something like
//
// chpl_dynamicAutoLocalCheck(A, loopDomain)
//
// right before the `forall`. Multiple dynamic checks are &&'ed. `allChecks` is
// that PRIM_AND call, or NULL if this was the first time we are adding a
// dynamic check
static void generateDynamicCheckForAccess(CallExpr *access,
                                          ForallStmt *forall,
                                          CallExpr *&allChecks) {
  ForallOptimizationInfo &optInfo = forall->optInfo;
  Symbol *baseSym = getCallBase(access);
  INT_ASSERT(baseSym);

  SET_LINENO(forall);

  CallExpr *currentCheck = NULL;
  if (optInfo.dynamicCheckForSymMap.count(baseSym) == 0) {
    currentCheck = new CallExpr("chpl__dynamicAutoLocalCheck");
    optInfo.dynamicCheckForSymMap[baseSym] = currentCheck;
  }
  else {
    return;
  }
  currentCheck->insertAtTail(baseSym);

  if (optInfo.iterSym != NULL) {
    currentCheck->insertAtTail(new SymExpr(optInfo.iterSym));
  }
  else if (optInfo.dotDomIterExpr != NULL) {
    currentCheck->insertAtTail(optInfo.dotDomIterExpr->copy());
  }
  else {
    INT_FATAL("optInfo didn't have enough information");
  }

  if (allChecks == NULL) {
    allChecks = currentCheck;
  }
  else {
    allChecks = new CallExpr(PRIM_AND, currentCheck, allChecks);
  }
}

// for a call like `A[i]`, this will create something like
//
// param staticAutoLocalCheckSym = chpl_staticAutoLocalCheck(A, loopDomain)
//
// right before the `forall` and will return the symbol declared. If a check was
// added for `A` before, it'll just return the symbol (that was created before)
static Symbol *generateStaticCheckForAccess(CallExpr *access,
                                            ForallStmt *forall) {
                                          
  ForallOptimizationInfo &optInfo = forall->optInfo;
  Symbol *baseSym = getCallBase(access);
  INT_ASSERT(baseSym);

  if (optInfo.staticCheckSymForSymMap.count(baseSym) == 0) {
    SET_LINENO(forall);

    VarSymbol *checkSym = new VarSymbol("chpl__staticAutoLocalCheckSym");
    checkSym->addFlag(FLAG_PARAM);
    optInfo.staticCheckSymForSymMap[baseSym] = checkSym;

    CallExpr *checkCall = new CallExpr("chpl__staticAutoLocalCheck");
    checkCall->insertAtTail(baseSym);

    if (optInfo.iterSym != NULL) {
      checkCall->insertAtTail(new SymExpr(optInfo.iterSym));
    }
    else if (optInfo.dotDomIterExpr != NULL) {
      checkCall->insertAtTail(optInfo.dotDomIterExpr->copy());
    }
    else {
      INT_FATAL("optInfo didn't have enough information");
    }

    DefExpr *checkSymDef = new DefExpr(checkSym, checkCall);
    forall->insertBefore(checkSymDef);
    return checkSym;
  }
  else {
    return optInfo.staticCheckSymForSymMap[baseSym];
  }
}

static void generateOptimizedLoops(ForallStmt *forall) {

  std::vector<CallExpr *> &sOptCandidates = forall->optInfo.staticCandidates;
  std::vector<CallExpr *> &dOptCandidates = forall->optInfo.dynamicCandidates;

  const int totalNumCandidates = sOptCandidates.size() + dOptCandidates.size();
  if (totalNumCandidates == 0) return;

  for_vector(CallExpr, sOptCandidate, sOptCandidates) {
    Symbol *checkSym = generateStaticCheckForAccess(sOptCandidate, forall);

    analyzeArrLog("\tReplacing", sOptCandidate);

    SET_LINENO(sOptCandidate);
    Symbol *baseSym = toSymExpr(sOptCandidate->baseExpr)->symbol();
    INT_ASSERT(baseSym);

    CallExpr *repl = new CallExpr(PRIM_MAYBE_LOCAL_THIS, new SymExpr(baseSym));
    for (int i = 1 ; i <= sOptCandidate->argList.length ; i++) {
      Symbol *argSym = toSymExpr(sOptCandidate->get(i))->symbol();
      INT_ASSERT(argSym);

      repl->insertAtTail(new SymExpr(argSym));
    }
    repl->insertAtTail(new SymExpr(checkSym));

    // mark statically-determined access. Today, this is only used for more
    // accurate logging
    repl->insertAtTail(new SymExpr(gTrue));

    sOptCandidate->replace(repl);
  }

  if (dOptCandidates.size() > 0) {
    SET_LINENO(forall);

    ForallStmt *staticOnly = forall->copy();
    staticOnly->optInfo.autoLocalAccessChecked = true;
    
    CallExpr *dynamicCond = NULL;

    for_vector(CallExpr, dOptCandidate, dOptCandidates) {
      Symbol *callBase = getCallBase(dOptCandidate);
      Symbol *checkSym = generateStaticCheckForAccess(dOptCandidate,
                                                       forall);
      generateDynamicCheckForAccess(dOptCandidate, forall, dynamicCond);

      analyzeArrLog("\tMarking for dynamic analysis", dOptCandidate);
      SET_LINENO(dOptCandidate);

      CallExpr *repl = new CallExpr(PRIM_MAYBE_LOCAL_THIS, new SymExpr(callBase));
      for (int i = 1 ; i <= dOptCandidate->argList.length ; i++) {
        Symbol *argSym = toSymExpr(dOptCandidate->get(i))->symbol();
        INT_ASSERT(argSym);

        repl->insertAtTail(new SymExpr(argSym));
      }
      repl->insertAtTail(checkSym);

      // mark dynamically-determined access. Today, this is only used for more
      // accurate logging
      repl->insertAtTail(new SymExpr(gFalse));

      dOptCandidate->replace(repl);
    }

    BlockStmt *thenBlock = new BlockStmt();
    BlockStmt *elseBlock = new BlockStmt();
    CondStmt *dynamicCheck = new CondStmt(dynamicCond, thenBlock, elseBlock);

    forall->insertAfter(dynamicCheck);
    thenBlock->insertAtTail(forall->remove());
    elseBlock->insertAtTail(staticOnly);
  }
}

void analyzeArrays() {
  forv_Vec(ForallStmt, forall, gForallStmts) {
    if (forall->optInfo.autoLocalAccessChecked) {
      continue;
    }
    forall->optInfo.autoLocalAccessChecked = true;

    analyzeArrLog("**** Start forall ****", forall);
    gatherForallInfo(forall);

    if (checkLoopSuitableForOpt(forall)) {
      analyzeArrLog("Loop is suitable for further analysis", forall);

      std::vector<CallExpr *> allCallExprs;
      collectCallExprs(forall->loopBody(), allCallExprs);

      for_vector(CallExpr, call, allCallExprs) {
        if (Symbol *accBaseSym = getCallBaseSymIfSuitable(call, forall)) {
                                                    
          analyzeArrLog("Potential access", call);

          bool canOptimize = false;
          // check for different patterns
          // forall i in A.domain do ... A[i] ...
          if (forall->optInfo.dotDomIterSym != NULL &&
              forall->optInfo.dotDomIterSym == accBaseSym) {
            canOptimize = true;
            analyzeArrLog("Access base is the same as iterator's base",
                call);
          }

          // if that didn't work...
          if (!canOptimize) {
            Symbol *domSym = getDomSym(accBaseSym);

            if (domSym != NULL) {  //  I can find the domain of the array
              // forall i in A.domain do ... B[i] ... where B and A share domain
              if (forall->optInfo.dotDomIterSymDom != NULL &&
                  forall->optInfo.dotDomIterSymDom == domSym) {
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
                if (forall->optInfo.iterSym != NULL &&
                    forall->optInfo.iterSym == domSym) {
                  canOptimize = true;
                  analyzeArrLog("Access base's domain is the iterator", call);
                }
              }
            }
            else {
              // I couldn't find a domain symbol for this array, but it can
              // still be a candidate for optimization based on analysis at
              // runtime
              forall->optInfo.dynamicCandidates.push_back(call);
            }
          }

          if (canOptimize) {
            forall->optInfo.staticCandidates.push_back(call);
          }
        }
      }

      generateOptimizedLoops(forall);
    }
    analyzeArrLog("**** End forall ****", forall);
  }
}



//
// Resolution support
//
static CallExpr *revertAccess(CallExpr *call) {
  analyzeArrLog("Reverting optimization (can't statically confirm)",
                call);

  CallExpr *repl = new CallExpr(new UnresolvedSymExpr("this"),
                                gMethodToken);

  // Don't take the last two args; they are the static control symbol, and flag
  // that tells whether this is a statically-determined access
  for (int i = 1 ; i < call->argList.length-1 ; i++) {
    Symbol *argSym = toSymExpr(call->get(i))->symbol();
    repl->insertAtTail(new SymExpr(argSym));
  }

  return repl;
}

static CallExpr *confirmAccess(CallExpr *call) {

  if (toSymExpr(call->get(call->argList.length))->symbol() == gTrue) {
    analyzeArrLog("Statically confirmed optimization, using localAccess",
                  call);
  }
  else {
    analyzeArrLog("Replacing with runtime checks", call);
  }

  //Expr *parent = call->parentExpr;
  CallExpr *repl = new CallExpr(new UnresolvedSymExpr("localAccess"),
                                gMethodToken);

  // Don't take the last two args; they are the static control symbol, and flag
  // that tells whether this is a statically-determined access
  for (int i = 1 ; i < call->argList.length-1 ; i++) {
    Symbol *argSym = toSymExpr(call->get(i))->symbol();
    repl->insertAtTail(new SymExpr(argSym));
  }

  return repl;
}

Expr *preFoldMaybeLocalThis(CallExpr *call) {
  // PRIM_MAYBE_LOCAL_THIS looks like
  //
  //   (call "may be local access" arrSymbol, idxSym0, ... ,idxSymN,
  //                               paramControlFlag, paramStaticallyDetermined)
  //
  // we need to check the second argument from last to determine whether we are
  // confirming this to be a local access or not
  if (SymExpr *controlSE = toSymExpr(call->get(call->argList.length-1))) {
    if (controlSE->symbol() == gTrue) {
      return confirmAccess(call);
    }
    else {
      return revertAccess(call);
    }
  }
  else {
    INT_FATAL("Misconfigured PRIM_MAYBE_LOCAL_THIS");
  }

  return NULL;
}

