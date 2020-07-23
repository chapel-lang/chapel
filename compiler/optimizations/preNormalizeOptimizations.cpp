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

#include "astutil.h"
#include "ForallStmt.h"
#include "LoopStmt.h"
#include "preNormalizeOptimizations.h"
#include "resolution.h"
#include "stlUtil.h"
#include "view.h"

// This file contains analysis and transformation logic that need to happen
// before normalization. There are two non-static functions:
//
//   - doPreNormalizeOptimizations
//
//     - Does an analysis on foralls to override dynamic checking for fast
//       followers.
//     - Does an analysis on foralls to check if regular array accesses can be
//       replaced with localAccess
//
//   - preFoldMaybeLocalThis
//
//     - Replaces `PRIM_MAYBE_LOCAL_THIS` with a regular array access or
//     localAccess during resolution


static void LOG(const char *msg, BaseAST *node);
static bool callHasSymArguments(CallExpr *ce, const std::vector<Symbol *> &syms);
static Symbol *getDotDomBaseSym(Expr *expr);
static Expr *getDomExprFromTypeExprOrQuery(Expr *e);
static Symbol *getDomSym(Symbol *arrSym);
static Symbol *getDomSymFromDomExpr(Expr *domExpr, bool allowQuery);
static Stmt *getLocalityDominator(CallExpr* ce);
static bool isSubIndexAssignment(Expr *expr,
                                 Symbol *subIndex,
                                 int indexIndex,
                                 Symbol *indexBundle);
static std::vector<Symbol *> getLoopIndexSymbols(ForallStmt *forall,
                                                 Symbol *baseSym);
static void gatherForallInfo(ForallStmt *forall);
static bool checkLoopSuitableForOpt(ForallStmt *forall);
static Symbol *getCallBaseSymIfSuitable(CallExpr *call, ForallStmt *forall);
static Symbol *getCallBase(CallExpr *call);
static void generateDynamicCheckForAccess(CallExpr *access,
                                          ForallStmt *forall,
                                          CallExpr *&allChecks);
static Symbol *generateStaticCheckForAccess(CallExpr *access,
                                            ForallStmt *forall,
                                            Expr *&allChecks);
static void replaceCandidate(CallExpr *candidate,
                             Symbol *staticCheckSym,
                             bool doStatic);
static void optimizeLoop(ForallStmt *forall,
                         Expr *&staticCond, CallExpr *&dynamicCond,
                         bool doStatic);
static CallExpr *addStaticCheckSymsToDynamicCond(ForallStmt *forall,
                                     CallExpr *dynamicCond,
                                     std::vector<Symbol *> &staticCheckSyms);
static ForallStmt *cloneLoop(ForallStmt *forall);
static void constructCondStmtFromLoops(Expr *condExpr,
                                       ForallStmt *optimized,
                                       ForallStmt *unoptimized);

static void generateOptimizedLoops(ForallStmt *forall);
static void autoLocalAccess(ForallStmt *forall);
static CallExpr *revertAccess(CallExpr *call);
static CallExpr *confirmAccess(CallExpr *call);

static void symbolicFastFollowerAnalysis(ForallStmt *forall);

void doPreNormalizeArrayOptimizations() {
  const bool anyAnalysisNeeded = fAutoLocalAccess ||
                                 !fNoFastFollowers;

  if (anyAnalysisNeeded) {
    forv_Vec(ForallStmt, forall, gForallStmts) {
      if (!fNoFastFollowers) {
        symbolicFastFollowerAnalysis(forall);
      }

      if (fAutoLocalAccess) {
        autoLocalAccess(forall);
      }
    }
  }
}

Expr *preFoldMaybeLocalThis(CallExpr *call) {
  if (fAutoLocalAccess) {
    // PRIM_MAYBE_LOCAL_THIS looks like
    //
    //  (call "may be local access" arrSymbol, idxSym0, ... ,idxSymN,
    //                              paramControlFlag, paramStaticallyDetermined)
    //
    // we need to check the second argument from last to determine whether we
    // are confirming this to be a local access or not
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
  }
  return NULL;
}


// logging support for --report-auto-local-access
static void LOG(const char *msg, BaseAST *node) {
  if (fReportAutoLocalAccess) {
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
// Normalize support for --auto-local-access
//

// Return true if `ce`'s arguments are exactly identical to `syms`
static bool callHasSymArguments(CallExpr *ce, const std::vector<Symbol *> &syms) {
  if (((std::size_t)ce->argList.length) != syms.size()) return false;
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

// get the domain part of the expression from `[D] int` or `[?D] int`
static Expr *getDomExprFromTypeExprOrQuery(Expr *e) {
  if (CallExpr *ce = toCallExpr(e)) {
    if (ce->isNamed("chpl__buildArrayRuntimeType")) {
      if (CallExpr *ceInner = toCallExpr(ce->get(1))) {
        if (ceInner->isNamed("chpl__ensureDomainExpr")) {
          return ceInner->get(1);
        }
      }
      else if (DefExpr *queryDef = toDefExpr(ce->get(1))) {
        return queryDef;
      }
    }
  }
  return NULL;
}

static Symbol *getDomSym(Symbol *arrSym);

// get the domain symbol from `Dom`, `?Dom` (if allowQuery) or `arr.domain`
static Symbol *getDomSymFromDomExpr(Expr *domExpr, bool allowQuery) {
  // we try the following cases one by one:
  
  if (SymExpr *domSE = toSymExpr(domExpr)) {
    return domSE->symbol();
  }

  if (allowQuery) {
    if (DefExpr *domSE = toDefExpr(domExpr)) {
      return domSE->sym;
    }
  }

  if (Symbol *dotDomBaseSym = getDotDomBaseSym(domExpr)) {
    return getDomSym(dotDomBaseSym); // recurse
  }
  return NULL;
}

// If `arrSym` is an array symbol, try to return its domain's symbol. return
// NULL if we can't find a domain symbol statically
static Symbol *getDomSym(Symbol *arrSym) {

  Symbol *ret = NULL;

  // try to get the domain of arrays that are defined in this scope
  if(DefExpr *def = arrSym->defPoint) {
    if (def->exprType != NULL) {
      // check for pattern `var A: [D] int;`
      if (Expr *arg = getDomExprFromTypeExprOrQuery(def->exprType)) {
        ret = getDomSymFromDomExpr(arg, /* allowQuery= */ false);
      }
      // check for `B` in `var A, B: [D] int;`
      else if (CallExpr *ceOuter = toCallExpr(def->exprType)) {
        if (ceOuter->isPrimitive(PRIM_TYPEOF)) {
          if (SymExpr *typeOfSymExpr = toSymExpr(ceOuter->get(1))) {
            ret = getDomSym(typeOfSymExpr->symbol()); // recurse
          }
        }
      }
    }
  }

  if (ret == NULL) {
    // try to get the domain if the symbol was an argument
    // e.g. `a: [d] int`, `a: [?d] int`, `a: [x.domain] int`
    if (ArgSymbol *arrArgSym = toArgSymbol(arrSym)) {
      if (BlockStmt *typeBlock = toBlockStmt(arrArgSym->typeExpr)) {
        Expr *firstExpr = typeBlock->body.head;
        Expr *domExpr = getDomExprFromTypeExprOrQuery(firstExpr);

        ret = getDomSymFromDomExpr(domExpr, /* allowQuery= */ true);

      }
    }
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
  if (indexVarCount == -1 ||
      ((std::size_t)indexVarCount) != indexSymbols.size()) {
    LOG("Can't recognize loop's index symbols", baseSym);
    indexSymbols.clear();
  }

  return indexSymbols;
}

static void gatherForallInfo(ForallStmt *forall) {

  Symbol *loopIdxSym = NULL;

  AList &iterExprs = forall->iteratedExpressions();
  AList &indexVars = forall->inductionVariables();

  if (isUnresolvedSymExpr(iterExprs.head) || isSymExpr(iterExprs.head)) {
    if (SymExpr *iterSE = toSymExpr(iterExprs.head)) {
      forall->optInfo.iterSym = iterSE->symbol();

      LOG("Iterated symbol", forall->optInfo.iterSym);
    }
  }
  // it might be in the form `A.domain` where A is used in the loop body
  else if (Symbol *dotDomBaseSym = getDotDomBaseSym(iterExprs.head)) {
    forall->optInfo.dotDomIterExpr = iterExprs.head;
    forall->optInfo.dotDomIterSym = dotDomBaseSym;
    forall->optInfo.dotDomIterSymDom = getDomSym(forall->optInfo.dotDomIterSym);

    LOG("Iterated over the domain of", forall->optInfo.dotDomIterSym);
    if (forall->optInfo.dotDomIterSymDom != NULL) {
      LOG(", which is", forall->optInfo.dotDomIterSymDom);
    }
    else {
      LOG(", whose domain cannot be determined statically",
                    forall->optInfo.dotDomIterSym);
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
      INT_FATAL("Loop index cannot be extracted");
    }

    if (loopIdxSym->hasFlag(FLAG_INDEX_OF_INTEREST)) {
      forall->optInfo.multiDIndices = getLoopIndexSymbols(forall, loopIdxSym);
    }
    else {
      forall->optInfo.multiDIndices.push_back(loopIdxSym);
    }
  }
}

static bool checkLoopSuitableForOpt(ForallStmt *forall) {

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
// that "&&" call, or NULL if this was the first time we are adding a
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

  CallExpr *staticOverride = new CallExpr(PRIM_UNARY_LNOT, 
      new SymExpr(forall->optInfo.staticCheckSymForSymMap[baseSym]));
  currentCheck = new CallExpr("||", staticOverride, currentCheck);

  if (allChecks == NULL) {
    allChecks = currentCheck;
  }
  else {
    allChecks = new CallExpr("&&", currentCheck, allChecks);
  }
}

// for a call like `A[i]`, this will create something like
//
// param staticAutoLocalCheckSym = chpl_staticAutoLocalCheck(A, loopDomain)
//
// right before the `forall` and will return the symbol declared. If a check was
// added for `A` before, it'll just return the symbol (that was created before)
static Symbol *generateStaticCheckForAccess(CallExpr *access,
                                            ForallStmt *forall,
                                            Expr *&allChecks) {
                                          
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

    if (allChecks == NULL) {
      allChecks = new SymExpr(checkSym);
    }
    else {
      allChecks = new CallExpr("||", new SymExpr(checkSym), allChecks);
    }

    DefExpr *checkSymDef = new DefExpr(checkSym, checkCall);
    forall->insertBefore(checkSymDef);
    return checkSym;
  }
  else {
    return optInfo.staticCheckSymForSymMap[baseSym];
  }
}

// replace a candidate CallExpr with the corresponding PRIM_MAYBE_LOCAL_THIS
static void replaceCandidate(CallExpr *candidate,
                             Symbol *staticCheckSym,
                             bool doStatic) {
  SET_LINENO(candidate);

  Symbol *callBase = getCallBase(candidate);
  CallExpr *repl = new CallExpr(PRIM_MAYBE_LOCAL_THIS, new SymExpr(callBase));
  for (int i = 1 ; i <= candidate->argList.length ; i++) {
    Symbol *argSym = toSymExpr(candidate->get(i))->symbol();
    INT_ASSERT(argSym);

    repl->insertAtTail(new SymExpr(argSym));
  }
  repl->insertAtTail(new SymExpr(staticCheckSym));

  // mark if this access is a static candidate. Today, this is only used for
  // accurate logging
  repl->insertAtTail(new SymExpr(doStatic?gTrue:gFalse));

  candidate->replace(repl);
}

// replace all the candidates in the loop with PRIM_MAYBE_LOCAL_THIS
// while doing that, also builds up static and dynamic conditions
static void optimizeLoop(ForallStmt *forall,
                         Expr *&staticCond, CallExpr *&dynamicCond,
                         bool doStatic) {

  std::vector<CallExpr *> candidates = doStatic ?
      forall->optInfo.staticCandidates :
      forall->optInfo.dynamicCandidates;

  for_vector(CallExpr, candidate, candidates) {

    Symbol *checkSym = generateStaticCheckForAccess(candidate,
                                                    forall,
                                                    staticCond);
    if (doStatic) {
      LOG("\tMarking static candidate", candidate);
    }
    else {
      forall->optInfo.staticCheckSymsForDynamicCandidates.push_back(checkSym);

      generateDynamicCheckForAccess(candidate,
                                    forall,
                                    dynamicCond);

      LOG("\tMarking dynamic candidate", candidate);
    }

    replaceCandidate(candidate, checkSym, doStatic);
  }
}

// add the param part of the dynamicCond
static CallExpr *addStaticCheckSymsToDynamicCond(ForallStmt *forall,
                                                 CallExpr *dynamicCond,
                                                 std::vector<Symbol *> &staticCheckSyms) {
  CallExpr *ret = dynamicCond;

  if (staticCheckSyms.size() > 0) {
    SET_LINENO(forall);

    if (staticCheckSyms.size() == 1) {
      ret = new CallExpr("&&", staticCheckSyms[0], dynamicCond);
    }
    else {
      CallExpr *allChecks = new CallExpr("||", staticCheckSyms[0],
                                               staticCheckSyms[1]);

      for (size_t i = 2 ; i < staticCheckSyms.size() ; i++) {
        allChecks = new CallExpr("||", staticCheckSyms[i], allChecks);
      }
      ret = new CallExpr("&&", allChecks, dynamicCond);
    }
  }

  return ret;
}

static ForallStmt *cloneLoop(ForallStmt *forall) {
  SET_LINENO(forall);

  ForallStmt *clone = forall->copy();
  clone->optInfo.autoLocalAccessChecked = forall->optInfo.autoLocalAccessChecked;
  clone->optInfo.confirmedFastFollower = forall->optInfo.confirmedFastFollower;
  return clone;
}

// expects `optimized` to be in the AST, `unoptimized` to be not
//
// turns
//
//   optimized
//
// into
//
//   if condExpr then
//     optimized
//   else
//     unoptimized
static void constructCondStmtFromLoops(Expr *condExpr,
                                       ForallStmt *optimized,
                                       ForallStmt *unoptimized) {
  SET_LINENO(optimized);

  BlockStmt *thenBlock = new BlockStmt();
  BlockStmt *elseBlock = new BlockStmt();
  CondStmt *cond = new CondStmt(condExpr, thenBlock, elseBlock);

  optimized->insertAfter(cond);
  thenBlock->insertAtTail(optimized->remove());
  elseBlock->insertAtTail(unoptimized);
}

// Takes a forall statement and generates the following structure:
//
// param staticCheck1 = staticCheck(arr1, loopDomain)
// param staticCheck2 = staticCheck(arr2, loopDomain)
// ...
// param staticCheckN = staticCheck(arrN, loopDomain)
//
// if (staticCheck1 || statickCheck2 || ... || staticCheckN) {
//   
//   const dynamicCheck = (!staticCheck1 || dynamicCheck(arr1, loopDomain)) &&
//                        (!staticCheck2 || dynamicCheck(arr2, loopDomain)) &&
//                        ...
//                        (!staticCheckN || dynamicCheck(arrN, loopDomain));
//
//   if (staticCheckX || .. || staticCheckZ) && dynamicCheck {
//     loop2
//   }
//   else {
//     loop1
//   }
// }
// else {
//   loop0
// }
//
// loop0: This'll be the copy of the unoptimized forall loop
// loop1: This'll contain optimizations that can be decided at the compile time
//        only. If there is no such optimizations but there are some that can be
//        determined at runtime, we still create this and it'll be identical to
//        loop0. This'll be the "backup" loop for potential dynamic check
//        failure
// loop2: Optional. This is only created if there are some dynamic candidates
//        and dynamic optimizations are enabled with the
//        `--auto-local-access-dynamic` flag.
//
// staticCheckX and staticCheckZ are static checks added for dynamic
// candidates. OR'ed static checks in two `if`s are added so that we can fold
// out unnecessary loops during resolution.
// 
// Note that the static checks are param flags and during resolution we'll
// defintely lose either loop0 or the bigger branch. In other words, there can
// be duplicate loops at the end of normalize, but after resolution we expect
// them to go away.
static void generateOptimizedLoops(ForallStmt *forall) {
  std::vector<CallExpr *> &sOptCandidates = forall->optInfo.staticCandidates;
  std::vector<CallExpr *> &dOptCandidates = forall->optInfo.dynamicCandidates;

  const int totalNumCandidates = sOptCandidates.size() + dOptCandidates.size();
  if (totalNumCandidates == 0) return;

  ForallStmt *noOpt = NULL;
  ForallStmt *noDyn = NULL;
  Expr *staticCond = NULL;
  CallExpr *dynamicCond = NULL;

  // copy the forall to have: `noOpt` == loop0, `forall` == loop1
  noOpt = cloneLoop(forall);

  if (sOptCandidates.size() > 0) {
    // change potential static accesses in loop1
    optimizeLoop(forall, staticCond, dynamicCond, /* isStatic= */ true);
  }

  if (fAutoLocalAccessDynamic && dOptCandidates.size() > 0) {
    // copy the forall to have: `noDyn` == loop1, `forall` == loop2
    noDyn = cloneLoop(forall);
    
    // change potential dynamic accesses in loop2
    optimizeLoop(forall, staticCond, dynamicCond, /* isStatic= */ false);
  }

  // add `(staticChecksX || .. || staticChecksZ)` part
  dynamicCond = addStaticCheckSymsToDynamicCond(forall, dynamicCond,
          forall->optInfo.staticCheckSymsForDynamicCandidates);
  
  // we have all the parts needed, now build the structure
  if (staticCond != NULL) {  // this must be true at this point
    constructCondStmtFromLoops(staticCond, forall, noOpt);
  }
  
  if (dynamicCond != NULL) {
    constructCondStmtFromLoops(dynamicCond, forall, noDyn);
  }
}

static void autoLocalAccess(ForallStmt *forall) {

  if (forall->optInfo.autoLocalAccessChecked) {
    return;
  }
  forall->optInfo.autoLocalAccessChecked = true;

  LOG("**** Start forall ****", forall);
  gatherForallInfo(forall);

  if (checkLoopSuitableForOpt(forall)) {
    LOG("Loop is suitable for further analysis", forall);

    std::vector<CallExpr *> allCallExprs;
    collectCallExprs(forall->loopBody(), allCallExprs);

    for_vector(CallExpr, call, allCallExprs) {
      if (Symbol *accBaseSym = getCallBaseSymIfSuitable(call, forall)) {

        LOG("Potential access", call);

        bool canOptimize = false;
        // check for different patterns
        // forall i in A.domain do ... A[i] ...
        if (forall->optInfo.dotDomIterSym != NULL &&
            forall->optInfo.dotDomIterSym == accBaseSym) {
          canOptimize = true;
          LOG("\tCan optimize: Access base is the iterator's base",
              call);
        }

        // if that didn't work...
        if (!canOptimize) {
          Symbol *domSym = getDomSym(accBaseSym);
          if (domSym != NULL) {
            LOG("\twith domain defined at", domSym);
          }
          else {
            LOG("\tregular domain symbol was not found for array", accBaseSym);
          }

          if (domSym != NULL) {  //  I can find the domain of the array
            // forall i in A.domain do ... B[i] ... where B and A share domain
            if (forall->optInfo.dotDomIterSymDom != NULL &&
                forall->optInfo.dotDomIterSymDom == domSym) {
              canOptimize = true;
              LOG("\tCan optimize: Access base has the same domain as iterator's base",
                  call);
            }
            // forall i in D do ... A[i] ... where D is A's domain
            else {
              if (forall->optInfo.iterSym != NULL &&
                  forall->optInfo.iterSym == domSym) {
                canOptimize = true;
                LOG("Access base's domain is the iterator", call);
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
  LOG("**** End forall ****", forall);
}



//
// Resolution support for auto-local-access
//
static CallExpr *revertAccess(CallExpr *call) {
  LOG("Static check failed. Reverting optimization", call);

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
    LOG("Static check successful. Using localAccess", call);
  }
  else {
    LOG("Static check successful. Using localAccess with dynamic check", call);
  }

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

//
// An analysis to enable overriding dynamic checks for fast followers
//
static void symbolicFastFollowerAnalysis(ForallStmt *forall) {
  AList &iterExprs = forall->iteratedExpressions();

  if (iterExprs.length < 2) {
    return;
  }

  Symbol *commonDomSym = NULL;
  bool confirm = true;

  for_alist(iterExpr, forall->iteratedExpressions()) {
    Symbol *iterBaseSym = NULL;
    Symbol *iterBaseDomSym = NULL;

    // record it if it is a symbol
    if (isUnresolvedSymExpr(iterExpr) || isSymExpr(iterExpr)) {
      if (SymExpr *iterSE = toSymExpr(iterExpr)) {
        iterBaseSym = iterSE->symbol();
      }
    }
    // it might be in the form `A.domain`
    else if (Symbol *dotDomBaseSym = getDotDomBaseSym(iterExpr)) {
      iterBaseSym = dotDomBaseSym;
    }

    // break if we couldn't get a symbol that we can analyze further
    if (iterBaseSym == NULL) {
      confirm = false;
      break;
    }

    if (Symbol *domainSymbol = getDomSym(iterBaseSym)) {
      // found a symbol through a definition that I can recognize
      iterBaseDomSym = domainSymbol;
    }
    else {
      // for now, just roll the dice and hope that it was a domain
      iterBaseDomSym = iterBaseSym;
    }

    if (commonDomSym == NULL) {
      commonDomSym = iterBaseDomSym;
    }
    else {
      // this iterator's symbol is different then what I assumed to be the
      // common domain for all iterators. Not much I can do with this loop
      if (commonDomSym != iterBaseDomSym) {
        confirm = false;
        break;
      }
    }
  }
  if (confirm) {
    //std::cout << "Confirming forall for static fast follower check\n";
    //std::cout << forall->stringLoc() << std::endl;
  }
  forall->optInfo.confirmedFastFollower = confirm;
}

