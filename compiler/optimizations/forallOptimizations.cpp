/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

#include <algorithm>

#include "astutil.h"
#include "build.h"
#include "ForallStmt.h"
#include "LoopStmt.h"
#include "passes.h"
#include "preFold.h"
#include "forallOptimizations.h"
#include "resolution.h"
#include "stlUtil.h"
#include "view.h"

// This file contains analysis and transformation logic that need to happen
// before normalization. These transformations help the following optimizations:
//
// - symbolic fast follower check: Avoids dynamic costs for checking for fast
//                                followers in cases where the follower can be
//                                proven to be aligned with the leader. e.g.
//                                `zip(A, A.domain)`
// - automatic local access: Use `localAccess` instead of `this` for array
//                           accesses that can be proven to be local
//
// - automatic aggregation: Use aggregation instead of regular assignments for
//                          applicable last statements within `forall` bodies

static int curLogDepth = 0;
static void LOG_ALA(int depth, const char *msg, BaseAST *node);
static void LOGLN_ALA(BaseAST *node);

static void LOG_AA(int depth, const char *msg, BaseAST *node);
static void LOGLN_AA(BaseAST *node);

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
static bool loopHasValidInductionVariables(ForallStmt *forall);
static Symbol *canDetermineLoopDomainStatically(ForallStmt *forall);
static Symbol *getCallBaseSymIfSuitable(CallExpr *call, ForallStmt *forall,
                                        bool checkArgs);
static Symbol *getCallBase(CallExpr *call);
static void generateDynamicCheckForAccess(CallExpr *access,
                                          ForallStmt *forall,
                                          CallExpr *&allChecks);
static Symbol *generateStaticCheckForAccess(CallExpr *access,
                                            ForallStmt *forall,
                                            Expr *&allChecks);
static CallExpr *replaceCandidate(CallExpr *candidate,
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

static const char *getForallCloneTypeStr(Symbol *aggMarker);
static CallExpr *getAggGenCallForChild(CallExpr *child, bool srcAggregation);
static bool assignmentSuitableForAggregation(CallExpr *call, ForallStmt *forall);
static void insertAggCandidate(CallExpr *call, ForallStmt *forall);
static bool handleYieldedArrayElementsInAssignment(CallExpr *call,
                                                   ForallStmt *forall);
static void findAndUpdateMaybeAggAssign(CallExpr *call, bool confirmed);
static CallExpr *findMaybeAggAssignInBlock(BlockStmt *block);
static void removeAggregatorFromMaybeAggAssign(CallExpr *call, int argIndex);
static void autoAggregation(ForallStmt *forall);

void doPreNormalizeArrayOptimizations() {
  const bool anyAnalysisNeeded = fAutoLocalAccess ||
                                 fAutoAggregation ||
                                 !fNoFastFollowers;
  if (anyAnalysisNeeded) {
    forv_Vec(ForallStmt, forall, gForallStmts) {
      if (!fNoFastFollowers) {
        symbolicFastFollowerAnalysis(forall);
      }

      if (fAutoLocalAccess) {
        autoLocalAccess(forall);
      }

      if (fAutoAggregation) {
        autoAggregation(forall);
      }
    }
  }
}

Expr *preFoldMaybeLocalArrElem(CallExpr *call) {

  // (call "may be local arr elem" elem, arr, paramFlag)
  // we don't really care about the second argument here, it is used elsewhere

  SymExpr *maybeArrElemSymExpr = toSymExpr(call->get(1));
  INT_ASSERT(maybeArrElemSymExpr);

  SymExpr *controlSymExp = toSymExpr(call->get(3));
  INT_ASSERT(controlSymExp);

  bool confirmed = (controlSymExp->symbol() == gTrue);

  if (fAutoAggregation) {
    findAndUpdateMaybeAggAssign(call, confirmed);
  }

  return maybeArrElemSymExpr->remove();
}

Expr *preFoldMaybeLocalThis(CallExpr *call) {
  Expr *ret = NULL;
  bool confirmed = false;

  // PRIM_MAYBE_LOCAL_THIS looks like
  //
  //  (call "may be local access" arrSymbol, idxSym0, ... ,idxSymN,
  //                              paramControlFlag, paramStaticallyDetermined)
  //
  // we need to check the second argument from last to determine whether we
  // are confirming this to be a local access or not
  if (SymExpr *controlSE = toSymExpr(call->get(call->argList.length-1))) {
    if (controlSE->symbol() == gTrue) {
      confirmed = true;
    }
  }
  else {
    INT_FATAL("Misconfigured PRIM_MAYBE_LOCAL_THIS");
  }

  if (fAutoLocalAccess) {
    ret = confirmed ? confirmAccess(call) : revertAccess(call);
  }
  else {
    // maybe we have automatic aggregation but no automatic local access? In
    // that case we may have generated one of this primitive, for aggregation
    // detection, now we just remove it silently
    ret = revertAccess(call);
  }

  INT_ASSERT(ret);

  if (fAutoAggregation) {
    findAndUpdateMaybeAggAssign(call, confirmed);
  }

  return ret;
}

// called during LICM to restructure a conditional aggregation to direct
// aggregation
void transformConditionalAggregation(CondStmt *cond) {
  // move the aggregation call before the conditional (at this point in
  // compilation it must be inlined)
  for_alist(expr, cond->elseStmt->body) {
    cond->insertBefore(expr->remove());
  }
  
  // remove the defpoint of the aggregation marker
  SymExpr *condExpr = toSymExpr(cond->condExpr);
  INT_ASSERT(condExpr);

  if (fReportAutoAggregation) {
    std::stringstream message;
    message << "Replaced assignment with aggregation";
    if (condExpr->symbol()->hasFlag(FLAG_AGG_IN_STATIC_AND_DYNAMIC_CLONE)) {
      message << " [static and dynamic ALA clone] ";
    }
    else if (condExpr->symbol()->hasFlag(FLAG_AGG_IN_STATIC_ONLY_CLONE)) {
      message << " [static only ALA clone] ";
    }
    LOG_AA(0, message.str().c_str(), condExpr);
  }

  condExpr->symbol()->defPoint->remove();

  // remove the conditional
  cond->remove();
}

// called after unordered forall optimization to remove aggregation code (by
// choosing non-aggregated version)
void cleanupRemainingAggCondStmts() {
  forv_Vec(CondStmt, condStmt, gCondStmts) {
    if (condStmt->inTree()) {
      if (SymExpr *condSymExpr = toSymExpr(condStmt->condExpr)) {
        if (condSymExpr->symbol()->hasFlag(FLAG_AGG_MARKER)) {
          // this is a conditional that wasn't modified by the unordered
          // optimization, so we need to remove the effects of the
          // forall intent adding the aggregator
          
          // find the aggregator symbol within the else block:
          // note that the else block will be inlined and will be a big mess, we
          // need to find the compiler-generated aggregator in that mess
          std::vector<SymExpr *> symExprs;
          collectSymExprs(condStmt->elseStmt, symExprs);

          Symbol *aggregatorToRemove = NULL;
          FnSymbol *parentFn = NULL;
          for_vector(SymExpr, symExpr, symExprs) {
            Symbol *sym = symExpr->symbol();
            if(sym->hasFlag(FLAG_COMPILER_ADDED_AGGREGATOR)) {
              aggregatorToRemove = sym;
              parentFn = toFnSymbol(symExpr->parentSymbol);
              break;
            }
          }

          INT_ASSERT(aggregatorToRemove != NULL);
          INT_ASSERT(parentFn != NULL);

          // put the nodes in the then block right before the conditional
          for_alist(expr, condStmt->thenStmt->body) {
            condStmt->insertBefore(expr->remove());
          }

          // remove the remaining conditional
          condStmt->remove();

          // remove the defPoint of the aggregator
          aggregatorToRemove->defPoint->remove();

          // search for the aggregator in the parent function and scrub it clean.
          // We expect 3 occurances of the symbol after the else block is gone
          symExprs.clear();
          collectSymExprs(parentFn->body, symExprs);

          for_vector(SymExpr, symExpr, symExprs) {
            Symbol *sym = symExpr->symbol();
            if (sym == aggregatorToRemove) {
              CallExpr *parentCall = toCallExpr(symExpr->parentExpr);
              INT_ASSERT(parentCall != NULL);

              if (parentCall->isPrimitive(PRIM_MOVE)) {
                /* Here's what we expect to see in the AST with associated
                   variable names in the following snippet

                  (def aggregator)
                  (def rhsSym)
                  (def prevRhsSym)
                  (call chpl_srcAggregatorForArr someArrSym, prevRhsSym)  `aggGenCall`
                  (call move call_tmp prevRhsSym)      `prevCall`
                  (call move aggregator rhsSym)   `parentCall`

                  At this point we only have a handle on the last expression. We
                  walk back up from that.
                */
                INT_ASSERT(toSymExpr(parentCall->get(1))->symbol() == aggregatorToRemove);

                Symbol *rhsSym = toSymExpr(parentCall->get(2))->symbol();
                CallExpr *prevCall = toCallExpr(parentCall->prev);
                INT_ASSERT(prevCall);
                INT_ASSERT(toSymExpr(prevCall->get(1))->symbol() == rhsSym);

                Symbol *prevRhsSym = toSymExpr(prevCall->get(2))->symbol();
                CallExpr *aggGenCall = toCallExpr(prevCall->prev);
                INT_ASSERT(aggGenCall);
                INT_ASSERT(aggGenCall->theFnSymbol()->hasFlag(FLAG_AGG_GENERATOR));
                INT_ASSERT(toSymExpr(aggGenCall->get(2))->symbol() == prevRhsSym);

                parentCall->remove();
                prevCall->remove();
                aggGenCall->remove();
                rhsSym->defPoint->remove();
                prevRhsSym->defPoint->remove();
              }
              else if (parentCall->isPrimitive(PRIM_SET_REFERENCE)) {
                /* Here's what we expect to see in the AST with associated
                   variable names in the following snippet

                   (def lhsSym)
                   (move lhsSym (`set reference` aggregator))  `moveCall`
                   (deinit lhsSym)

                  At this point we only have a handle on the `set reference` call.
                  We detect other expressions starting from that.
                */
                CallExpr *moveCall = toCallExpr(parentCall->parentExpr);
                INT_ASSERT(moveCall);
                INT_ASSERT(moveCall->isPrimitive(PRIM_MOVE));

                Symbol *lhsSym = toSymExpr(moveCall->get(1))->symbol();
                INT_ASSERT(lhsSym->defPoint == moveCall->prev);
                
                CallExpr *deinitCall = toCallExpr(moveCall->next);
                INT_ASSERT(deinitCall);
                INT_ASSERT(deinitCall->theFnSymbol()->hasFlag(FLAG_DESTRUCTOR));
                INT_ASSERT(toSymExpr(deinitCall->get(1))->symbol() == lhsSym);

                moveCall->remove();
                lhsSym->defPoint->remove();
                deinitCall->remove();
              }
              else {
                INT_FATAL("Auto-aggregator is in an unexpected expression");
              }
            }
          }
        }
      }
    }
  }
}
//
// logging support for --report-auto-local-access and --report-auto-aggregation
// during the first analysis phase depth is used roughly in the following way
// 0: entering/exiting forall
// 1: information about the forall
// 2: entering/exiting call
// 3: information about call
//
// during resolution, the output is much more straightforward, and depth 0 is
// used always
static void LOG_help(int depth, const char *msg, BaseAST *node,
                     bool forallDetails, bool flag) {
  if (flag) {
    bool verbose = (node->getModule()->modTag != MOD_INTERNAL &&
                    node->getModule()->modTag != MOD_STANDARD);

    const bool veryVerbose = false;
    if (verbose) {
      curLogDepth = depth;
      if (curLogDepth > 0) {
        std::cout << "|";
      }
      
      for (int i = 0 ; i < depth; i++) {
        std::cout << " ";
      }
      std::cout << msg;
      if (forallDetails) {
        if (ForallStmt *forall = toForallStmt(node)) {
          switch (forall->optInfo.cloneType) {
            case STATIC_AND_DYNAMIC:
              std::cout << " [static and dynamic ALA clone] ";
              break;
            case STATIC_ONLY:
              std::cout << " [static only ALA clone] ";
              break;
            case NO_OPTIMIZATION:
              std::cout << " [no ALA clone] ";
              break;
            default:
              break;
          }
        }
      }
      if (node != NULL) {
        std::cout << " (" << node->stringLoc() << ")";
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

static void LOGLN_help(BaseAST *node, bool flag) {
  if (flag) {
    bool verbose = (node->getModule()->modTag != MOD_INTERNAL &&
                    node->getModule()->modTag != MOD_STANDARD);
    if (verbose) {
      if (curLogDepth > 0) {
        std::cout << "|";
      }
      std::cout << std::endl;
    }
  }
}

static void LOG_AA(int depth, const char *msg, BaseAST *node) {
  LOG_help(depth, msg, node, /*forallDetails=*/true,
           fAutoAggregation && fReportAutoAggregation);
}

static void LOGLN_AA(BaseAST *node) {
  LOGLN_help(node, fAutoAggregation && fReportAutoAggregation);
}

static void LOG_ALA(int depth, const char *msg, BaseAST *node) {
  LOG_help(depth, msg, node, /*forallDetails=*/false,
           fAutoLocalAccess && fReportAutoLocalAccess);
}

static void LOGLN_ALA(BaseAST *node) {
  LOGLN_help(node, fAutoLocalAccess && fReportAutoLocalAccess);
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

static Expr *getDomainExprFromArrayType(CallExpr* ceInner){
  if (ceInner->isNamed("chpl__ensureDomainExpr")) {
    return ceInner->get(1);
  }
  return NULL;
}

// get the domain part of the expression from `[D] int` or `[?D] int`
static Expr *getDomExprFromTypeExprOrQuery(Expr *e) {
  if (CallExpr *ce = toCallExpr(e)) {
    if (ce->isNamed("chpl__buildArrayRuntimeType")) {
      if (CallExpr *ceInner = toCallExpr(ce->get(1))) {
        if (Expr* e = getDomainExprFromArrayType(ceInner)){
          return e;
        }
      } else if (DefExpr *queryDef = toDefExpr(ce->get(1))) {
        return queryDef;
      }
    }
  } else if (SymExpr* se = toSymExpr(e)) {
    if(se->symbol()->hasFlag(FLAG_TYPE_VARIABLE) &&
        se->symbol()->hasFlag(FLAG_TEMP)) {
      //if type var then check type expression
      if(DefExpr* defExpr = toDefExpr(se->symbol()->defPoint)){
        if (CallExpr *ce = toCallExpr(defExpr->init)) {
          if (ce->isNamed("chpl__buildArrayRuntimeType")) {
            if (CallExpr *ceInner = toCallExpr(ce->get(1))) {
              if (Expr* e = getDomainExprFromArrayType(ceInner)){
                return e;
              }
            }
          }
        }
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

  // find the check_tuple_var_decl and get the size of the tuple
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
    LOG_ALA(1, "Can't recognize loop's index symbols", baseSym);
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
    }
  }
  // it might be in the form `A.domain` where A is used in the loop body
  else if (Symbol *dotDomBaseSym = getDotDomBaseSym(iterExprs.head)) {
    forall->optInfo.dotDomIterExpr = iterExprs.head;
    forall->optInfo.dotDomIterSym = dotDomBaseSym;
    forall->optInfo.dotDomIterSymDom = getDomSym(forall->optInfo.dotDomIterSym);
  }
  else if (CallExpr *iterCall = toCallExpr(iterExprs.head)) {
    // not sure how or why we should support forall i in zip((...tup))
    if (!iterCall->isPrimitive(PRIM_TUPLE_EXPAND)) {
      if(Symbol *iterCallTmp = earlyNormalizeForallIterand(iterCall, forall)) {
        forall->optInfo.iterCall = iterCall;
        forall->optInfo.iterCallTmp = iterCallTmp;
      }
    }
  }

  if (forall->optInfo.iterSym != NULL ||
      forall->optInfo.dotDomIterSym != NULL ||
      forall->optInfo.iterCall != NULL) {
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
  forall->optInfo.infoGathered = true;
}

static bool loopHasValidInductionVariables(ForallStmt *forall) {
  // if we understand the induction variables, then we can still take a look at
  // the loop body for some calls that we can optimize dynamically
  return forall->optInfo.multiDIndices.size() > 0;
}

static Symbol *canDetermineLoopDomainStatically(ForallStmt *forall) {
  // a forall is suitable for static optimization only if it iterates over a
  // symbol (with the hopes that that symbol is a domain), or a foo.domain
  if (forall->optInfo.iterSym != NULL ){
    return forall->optInfo.iterSym;
  }
  else if (forall->optInfo.dotDomIterSym != NULL) {
    return forall->optInfo.dotDomIterSym;
  }
  return NULL;
}

// Bunch of checks to see if `call` is a candidate for optimization within
// `forall`. Returns the symbol of the `baseExpr` of the `call` if it is
// suitable. NULL otherwise
static Symbol *getCallBaseSymIfSuitable(CallExpr *call, ForallStmt *forall,
                                        bool checkArgs) {
  
  // TODO see if you can use getCallBase
  SymExpr *baseSE = toSymExpr(call->baseExpr);

  if (baseSE != NULL) {
    Symbol *accBaseSym = baseSE->symbol();

    // Prevent making changes to `new C[i]`
    if (CallExpr *parentCall = toCallExpr(call->parentExpr)) {
      if (parentCall->isPrimitive(PRIM_NEW)) { return NULL; } 
    }

    // give up if the access uses a different symbol
    if (checkArgs && !callHasSymArguments(call, forall->optInfo.multiDIndices)) { return NULL; }

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
  else if (optInfo.iterCallTmp != NULL) {
    currentCheck->insertAtTail(new SymExpr(optInfo.iterCallTmp));
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
    else if (optInfo.iterCallTmp != NULL) {
      checkCall->insertAtTail(new SymExpr(optInfo.iterCallTmp));
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
static CallExpr* replaceCandidate(CallExpr *candidate,
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

  return repl;
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
    if (!doStatic) {
      forall->optInfo.staticCheckSymsForDynamicCandidates.push_back(checkSym);

      generateDynamicCheckForAccess(candidate,
                                    forall,
                                    dynamicCond);
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
// if (staticCheck1 || staticCheck2 || ... || staticCheckN) {
//   
//   const dynamicCheck = (!staticCheckX || dynamicCheck(arrX, loopDomain)) &&
//                        (!staticCheckY || dynamicCheck(arrY, loopDomain)) &&
//                        ...
//                        (!staticCheckZ || dynamicCheck(arrZ, loopDomain));
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
//        `--dynamic-auto-local-access` flag.
//
// staticCheckX and staticCheckZ are static checks added for dynamic
// candidates. OR'ed static checks in two `if`s are added so that we can fold
// out unnecessary loops during resolution.
// 
// Note that the static checks are param flags and during resolution we'll
// definitely lose either loop0 or the bigger branch. In other words, there can
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
  noOpt->optInfo.cloneType = NO_OPTIMIZATION;

  if (sOptCandidates.size() > 0) {
    // change potential static accesses in loop1
    optimizeLoop(forall, staticCond, dynamicCond, /* isStatic= */ true);
    forall->optInfo.cloneType = STATIC_ONLY;
  }

  if (dOptCandidates.size() > 0) {
    // copy the forall to have: `noDyn` == loop1, `forall` == loop2
    noDyn = cloneLoop(forall);
    noDyn->optInfo.cloneType = STATIC_ONLY;
    
    // change potential dynamic accesses in loop2
    optimizeLoop(forall, staticCond, dynamicCond, /* isStatic= */ false);
    forall->optInfo.cloneType = STATIC_AND_DYNAMIC;
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

  LOGLN_ALA(forall);
  LOG_ALA(0, "Start analyzing forall", forall);

  gatherForallInfo(forall);

  if (!loopHasValidInductionVariables(forall)) {
    LOG_ALA(1, "Can't optimize this forall: invalid induction variables", forall);
    return;
  }

  Symbol *loopDomain = canDetermineLoopDomainStatically(forall);
  bool staticLoopDomain = loopDomain != NULL;
  if (staticLoopDomain) {
    LOG_ALA(1, "Found loop domain", loopDomain);
    LOG_ALA(1, "Will attempt static and dynamic optimizations", forall);
    LOGLN_ALA(forall);
  }
  else {
    LOG_ALA(1, "Couldn't determine loop domain: will attempt dynamic optimizations only", forall);
    LOGLN_ALA(forall);
  }

  std::vector<CallExpr *> allCallExprs;
  collectCallExprs(forall->loopBody(), allCallExprs);

  for_vector(CallExpr, call, allCallExprs) {
    Symbol *accBaseSym = getCallBaseSymIfSuitable(call, forall, /*checkArgs=*/true);

    if (accBaseSym == NULL) {
      continue;
    }

    LOG_ALA(2, "Start analyzing call", call);

    bool canOptimizeStatically = false;

    if (staticLoopDomain) {
      
      // forall i in A.domain do ... A[i] ...
      if (forall->optInfo.dotDomIterSym == accBaseSym) {
        canOptimizeStatically = true;

        LOG_ALA(3, "Can optimize: Access base is the iterator's base", call);
      }
      else {
        Symbol *domSym = getDomSym(accBaseSym);

        if (domSym != NULL) {  //  I can find the domain of the array
          LOG_ALA(3, "Found the domain of the access base", domSym);
          
          // forall i in A.domain do ... B[i] ... where B and A share domain
          if (forall->optInfo.dotDomIterSymDom == domSym) {
            canOptimizeStatically = true;

            LOG_ALA(3, "Can optimize: Access base has the same domain as iterator's base", call);
          }
         
          // forall i in D do ... A[i] ... where D is A's domain
          else if (forall->optInfo.iterSym == domSym) {
            canOptimizeStatically = true;

            LOG_ALA(3, "Can optimize: Access base's domain is the iterator", call);
          }
        }
        else {
          LOG_ALA(3, "Can't determine the domain of access base", accBaseSym);
        }
      }

      if (canOptimizeStatically) {
        LOG_ALA(2, "This call is a static optimization candidate", call);
        LOGLN_ALA(call);
        forall->optInfo.staticCandidates.push_back(call);
      }
    }

    if (fDynamicAutoLocalAccess && !canOptimizeStatically) {
      // two things might have happened:
      //
      // (1) I couldn't find a domain symbol for this array, but it can
      // still be a candidate for optimization based on analysis at
      // runtime
      //
      // (2) the loop wasn't suitable for static optimization (i.e. I can't
      // determine the symbol of the loop domain. But this call that I am
      // looking at still has potential because it is `A(i)` where `i` is
      // the loop index. So, add this call to dynamic candidates
      LOG_ALA(2, "This call is a dynamic optimization candidate", call);
      LOGLN_ALA(call);
      forall->optInfo.dynamicCandidates.push_back(call);
    }
  }

  generateOptimizedLoops(forall);

  LOG_ALA(0, "End analyzing forall", forall);
  LOGLN_ALA(forall);
}



//
// Resolution support for auto-local-access
//
static CallExpr *revertAccess(CallExpr *call) {
  LOG_ALA(0, "Static check failed. Reverting optimization", call);

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
    LOG_ALA(0, "Static check successful. Using localAccess", call);
  }
  else {
    LOG_ALA(0, "Static check successful. Using localAccess with dynamic check", call);
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
  forall->optInfo.confirmedFastFollower = confirm;
}

//
// Support for automatic aggregation

static void autoAggregation(ForallStmt *forall) {

  // if auto-local-access was on (that's the default case), we don't need to
  // look into the loop in detail, but if that optimization was off, we still
  // need to check what is local and what is not for aggregation.
  if (forall->optInfo.autoLocalAccessChecked == false) {
    autoLocalAccess(forall);
  }

  LOG_AA(0, "Start analyzing forall for automatic aggregation", forall);

  if (CallExpr *lastCall = toCallExpr(forall->loopBody()->body.last())) {

    if (lastCall->isNamed("=")) {
      // no need to do anything if it is array access
      if (assignmentSuitableForAggregation(lastCall, forall)) {
        LOG_AA(1, "Found an aggregation candidate", lastCall);

        insertAggCandidate(lastCall, forall);
      }
      // we need special handling if it is a symbol that is an array element
      else if (handleYieldedArrayElementsInAssignment(lastCall, forall)) {
        LOG_AA(1, "Found an aggregation candidate", lastCall);

        insertAggCandidate(lastCall, forall);
      }
    }
  }
  LOG_AA(0, "End analyzing forall for automatic aggregation", forall);
  LOGLN_AA(forall);
}

AggregationCandidateInfo::AggregationCandidateInfo(CallExpr *candidate,
                                                   ForallStmt *forall):
  candidate(candidate),
  forall(forall),
  lhsLocalityInfo(UNKNOWN),
  rhsLocalityInfo(UNKNOWN),
  lhsLogicalChild(NULL),
  rhsLogicalChild(NULL),
  srcAggregator(NULL),
  dstAggregator(NULL) { }

static CondStmt *createAggCond(CallExpr *noOptAssign, Symbol *aggregator, SymExpr *aggMarkerSE) {
  INT_ASSERT(aggregator);
  INT_ASSERT(aggMarkerSE);

  // the candidate assignment must have been normalized, so, we expect symexpr
  // on both sides
  SymExpr *lhsSE = toSymExpr(noOptAssign->get(1));
  SymExpr *rhsSE = toSymExpr(noOptAssign->get(2));
  INT_ASSERT(lhsSE != NULL && rhsSE != NULL);

  SET_LINENO(noOptAssign);

  // generate the aggregated call
  Expr *callBase = buildDotExpr(aggregator, "copy");
  CallExpr *aggCall = new CallExpr(callBase, lhsSE->copy(), rhsSE->copy());

  // create the conditional with regular assignment on the then block
  // and the aggregated call is on the else block
  BlockStmt *thenBlock = new BlockStmt();
  BlockStmt *elseBlock = new BlockStmt();

  CondStmt *aggCond = new CondStmt(aggMarkerSE, thenBlock, elseBlock);

  thenBlock->insertAtTail(noOptAssign);
  elseBlock->insertAtTail(aggCall);

  return aggCond;
}

// currently we only support one aggregator at normalize time, however, we
// should relax that limitation a bit. Normalize should be able to add both
// aggregators for
//
// forall i in a.domain {
//   a[i] = b[i];
// }
//
// Because: b[i] is a local access candidate, and both sides of = will be
// visited as such. Currently, that's not handled properly
void AggregationCandidateInfo::addAggregators() {
  if (srcAggregator != NULL && dstAggregator != NULL) { // assert?
    return; // we have already enough
  }

  // we have a lhs that waits analysis, and a rhs that we can't know about
  if (srcAggregator == NULL &&
      lhsLocalityInfo == PENDING &&
      rhsLogicalChild != NULL) {
    if (CallExpr *genCall = getAggGenCallForChild(rhsLogicalChild, true)) {
      SET_LINENO(this->forall);

      UnresolvedSymExpr *aggTmp = new UnresolvedSymExpr("chpl_src_auto_agg");
      ShadowVarSymbol *aggregator = ShadowVarSymbol::buildForPrefix(SVP_VAR,
                                                                    aggTmp,
                                                                    NULL, //type
                                                                    genCall);
      aggregator->addFlag(FLAG_COMPILER_ADDED_AGGREGATOR);
      forall->shadowVariables().insertAtTail(aggregator->defPoint);

      LOG_AA(2, "Potential source aggregation", this->candidate);

      this->srcAggregator = aggregator;
    }
  }
  
  // we have a rhs that waits analysis
  if (dstAggregator == NULL &&
      rhsLocalityInfo == PENDING &&
      lhsLogicalChild != NULL) {
    if (CallExpr *genCall = getAggGenCallForChild(lhsLogicalChild, false)) {
      SET_LINENO(this->forall);

      UnresolvedSymExpr *aggTmp = new UnresolvedSymExpr("chpl_dst_auto_agg");
      ShadowVarSymbol *aggregator = ShadowVarSymbol::buildForPrefix(SVP_VAR,
                                                                    aggTmp,
                                                                    NULL, //type
                                                                    genCall);
      aggregator->addFlag(FLAG_COMPILER_ADDED_AGGREGATOR);
      forall->shadowVariables().insertAtTail(aggregator->defPoint);

      LOG_AA(2, "Potential destination aggregation", this->candidate);

      this->dstAggregator = aggregator;
    }
  }

}

static const char *getForallCloneTypeStr(Symbol *aggMarker) {
  if (aggMarker->hasFlag(FLAG_AGG_IN_STATIC_AND_DYNAMIC_CLONE)) {
    return "[static and dynamic ALA clone]";
  }
  if (aggMarker->hasFlag(FLAG_AGG_IN_STATIC_ONLY_CLONE)) {
    return "[static only ALA clone]";
  }
  return "";
}

static CallExpr *getAggGenCallForChild(CallExpr *child, bool srcAggregation) {
  const char *aggFnName = srcAggregation ? "chpl_srcAggregatorForArr" :
                                           "chpl_dstAggregatorForArr";
  SET_LINENO(child);

  if (child->isPrimitive(PRIM_MAYBE_LOCAL_THIS)) {
    if (SymExpr *arrSymExpr = toSymExpr(child->get(1))) {
      return new CallExpr(aggFnName, new SymExpr(arrSymExpr->symbol()));
    }
  }
  else if (child->isPrimitive(PRIM_MAYBE_LOCAL_ARR_ELEM)) {
    if (SymExpr *arrSymExpr = toSymExpr(child->get(2))) {
      return new CallExpr(aggFnName, new SymExpr(arrSymExpr->symbol()));
    }
  }
  else {
    if (SymExpr *arrSymExpr = toSymExpr(child->baseExpr)) {
      return new CallExpr(aggFnName, new SymExpr(arrSymExpr->symbol()));
    }
  }
  return NULL;
}

// currently we want both sides to be calls, but we need to relax these to
// accept symexprs to support foralls over arrays
static bool assignmentSuitableForAggregation(CallExpr *call, ForallStmt *forall) {
  INT_ASSERT(call->isNamed("="));

  if (CallExpr *leftCall = toCallExpr(call->get(1))) {
    if (CallExpr *rightCall = toCallExpr(call->get(2))) {
      // we want either side to be PRIM_MAYBE_LOCAL_THIS
      if (leftCall->isPrimitive(PRIM_MAYBE_LOCAL_THIS) ||
          rightCall->isPrimitive(PRIM_MAYBE_LOCAL_THIS)) {
        // we want the side that's not to have a baseExpr that's a SymExpr
        // this avoid function calls
        if (!leftCall->isPrimitive(PRIM_MAYBE_LOCAL_THIS)) {
          return getCallBaseSymIfSuitable(leftCall, forall,
                                          /*checkArgs=*/false) != NULL;
        }
        else if (!rightCall->isPrimitive(PRIM_MAYBE_LOCAL_THIS)) {
          return getCallBaseSymIfSuitable(rightCall, forall,
                                          /*checkArgs=*/false) != NULL;
        }
      }
    }
  }
  return false;
}

Expr *preFoldMaybeAggregateAssign(CallExpr *call) {
  INT_ASSERT(call->isPrimitive(PRIM_MAYBE_AGGREGATE_ASSIGN));

  Expr *rhs = call->get(2)->remove();
  Expr *lhs = call->get(1)->remove();

  CallExpr *assign = new CallExpr("=", lhs, rhs);

  SymExpr *srcAggregatorSE = toSymExpr(call->get(2)->remove());
  INT_ASSERT(srcAggregatorSE);
  SymExpr *dstAggregatorSE = toSymExpr(call->get(1)->remove());
  INT_ASSERT(dstAggregatorSE);

  Symbol *srcAggregator = srcAggregatorSE->symbol();
  Symbol *dstAggregator = dstAggregatorSE->symbol();

  SymExpr *rhsLocalSE = toSymExpr(call->get(2)->remove());
  INT_ASSERT(rhsLocalSE);
  SymExpr *lhsLocalSE = toSymExpr(call->get(1)->remove());
  INT_ASSERT(lhsLocalSE);

  bool rhsLocal = (rhsLocalSE->symbol() == gTrue);
  bool lhsLocal = (lhsLocalSE->symbol() == gTrue);

  SymExpr *aggMarkerSE = toSymExpr(call->get(1)->remove());
  INT_ASSERT(aggMarkerSE);

  Expr *replacement = NULL;

  if (lhsLocal || rhsLocal) {
    INT_ASSERT(lhsLocal != rhsLocal);

    Symbol *aggregator = NULL;

    std::stringstream message;

    // aggregator can be nil in two cases:
    // (1) we couldn't determine what the code looks like statically on one side of
    //     the assignment, in which case we set this argument to `gNil`.
    // (2) we may have called an aggregator generator on an unsupported type,
    //     which returns `nil` in the module code.
    if (lhsLocal && (srcAggregator->type != dtNil)) {
      if (fReportAutoAggregation) {
        message << "LHS is local, RHS is nonlocal. Will use source aggregation ";
      }
      aggregator = srcAggregator;
    }
    else if (rhsLocal && (dstAggregator->type != dtNil)) {
      if (fReportAutoAggregation) {
        message << "LHS is nonlocal, RHS is local. Will use destination aggregation ";
      }
      aggregator = dstAggregator;
    }

    if (aggregator != NULL) {
      if (fReportAutoAggregation) {
        message << getForallCloneTypeStr(aggMarkerSE->symbol());
        LOG_AA(0, message.str().c_str(), call);
      }

      replacement = createAggCond(assign, aggregator, aggMarkerSE);
    }
  }
  
  if (replacement == NULL) {
    aggMarkerSE->symbol()->defPoint->remove();
    if (srcAggregator != gNil) {
      srcAggregator->defPoint->remove();
    }
    if (dstAggregator != gNil) {
      dstAggregator->defPoint->remove();
    }
    replacement = assign;
  }

  return replacement;
}

void AggregationCandidateInfo::transformCandidate() {
  SET_LINENO(this->candidate);
  Expr *rhs = this->candidate->get(2)->remove();
  Expr *lhs = this->candidate->get(1)->remove();
  CallExpr *repl = new CallExpr(PRIM_MAYBE_AGGREGATE_ASSIGN,
                                lhs,
                                rhs);

  repl->insertAtTail(this->dstAggregator ? this->dstAggregator : gNil);
  repl->insertAtTail(this->srcAggregator ? this->srcAggregator : gNil);

  repl->insertAtTail(new SymExpr(gFalse)); // lhs local?
  repl->insertAtTail(new SymExpr(gFalse)); // rhs local?

  Symbol *aggMarker = newTemp("aggMarker", dtBool);
  aggMarker->addFlag(FLAG_AGG_MARKER);
  switch (this->forall->optInfo.cloneType) {
    case STATIC_AND_DYNAMIC:
      aggMarker->addFlag(FLAG_AGG_IN_STATIC_AND_DYNAMIC_CLONE);
      break;
    case STATIC_ONLY:
      aggMarker->addFlag(FLAG_AGG_IN_STATIC_ONLY_CLONE);
      break;
    default:
      break;
  }
  this->candidate->insertBefore(new DefExpr(aggMarker));

  repl->insertAtTail(new SymExpr(aggMarker));
  
  this->candidate->replace(repl);
}

static void insertAggCandidate(CallExpr *call, ForallStmt *forall) {
  AggregationCandidateInfo *info = new AggregationCandidateInfo(call, forall);

  // establish connection between PRIM_MAYBE_LOCAL_THIS and their parent
  CallExpr *lhsCall = toCallExpr(call->get(1));
  INT_ASSERT(lhsCall); // enforced by callSuitableForAggregation
  if (lhsCall->isPrimitive(PRIM_MAYBE_LOCAL_THIS) ||
      lhsCall->isPrimitive(PRIM_MAYBE_LOCAL_ARR_ELEM)) {
    info->lhsLocalityInfo = PENDING;
  }
  info->lhsLogicalChild = lhsCall;

  CallExpr *rhsCall = toCallExpr(call->get(2));
  INT_ASSERT(rhsCall); // enforced by callSuitableForAggregation
  if (rhsCall->isPrimitive(PRIM_MAYBE_LOCAL_THIS) ||
      rhsCall->isPrimitive(PRIM_MAYBE_LOCAL_ARR_ELEM)) {
    info->rhsLocalityInfo = PENDING;
  }
  info->rhsLogicalChild = rhsCall;

  info->addAggregators();

  info->transformCandidate();
}

static bool handleYieldedArrayElementsInAssignment(CallExpr *call,
                                                   ForallStmt *forall) {
  SET_LINENO(call);

  INT_ASSERT(call->isNamed("="));

  if (!forall->optInfo.infoGathered) {
    gatherForallInfo(forall);
  }

  Symbol *maybeArrSym = forall->optInfo.iterSym;
  Symbol *maybeArrElemSym = NULL;
  if (forall->optInfo.multiDIndices.size() > 0) {
    maybeArrElemSym = forall->optInfo.multiDIndices[0];
  }

  // stop here if you don't know what I am talking about
  if (maybeArrSym == NULL || maybeArrElemSym == NULL) return false;

  bool lhsMaybeArrSym = false;
  bool rhsMaybeArrSym = false;

  SymExpr *lhsSymExpr = toSymExpr(call->get(1));
  SymExpr *rhsSymExpr = toSymExpr(call->get(2));

  if (lhsSymExpr) {
    lhsMaybeArrSym = (lhsSymExpr->symbol() == maybeArrElemSym);
  }
  if (rhsSymExpr) {
    rhsMaybeArrSym = (rhsSymExpr->symbol() == maybeArrElemSym);
  }

  // stop if neither can be an array element symbol
  if (!lhsMaybeArrSym && !rhsMaybeArrSym) return false;

  // just to be sure, stop if someone's doing `a=a`;
  if (lhsMaybeArrSym && rhsMaybeArrSym) return false;

  Expr *otherChild = lhsMaybeArrSym ? call->get(2) : call->get(1);

  bool otherChildIsSuitable = false;
  if (CallExpr *otherCall = toCallExpr(otherChild)) {
    if (otherCall->isPrimitive(PRIM_MAYBE_LOCAL_THIS)) {
      otherChildIsSuitable = true;
    }
    else {
      otherChildIsSuitable = (getCallBaseSymIfSuitable(otherCall,
                                                       forall,
                                                       /*checkArgs=*/false) != NULL);
    }
  }

  if (!otherChildIsSuitable) return false;

  SymExpr *symExprToReplace = lhsMaybeArrSym ? lhsSymExpr : rhsSymExpr;

  // add the check symbol
  VarSymbol *checkSym = new VarSymbol("chpl__yieldedArrayElemIsAligned");
  checkSym->addFlag(FLAG_PARAM);

  CallExpr *checkCall = new CallExpr("chpl__arrayIteratorYieldsLocalElements");
  checkCall->insertAtTail(maybeArrSym);

  DefExpr *checkSymDef = new DefExpr(checkSym, checkCall);
  forall->insertBefore(checkSymDef);

  // replace the call with PRIM_MAYBE_LOCAL_ARR_ELEM
  CallExpr *primCall = new CallExpr(PRIM_MAYBE_LOCAL_ARR_ELEM,
                                    new SymExpr(maybeArrElemSym),
                                    new SymExpr(maybeArrSym),
                                    new SymExpr(checkSym));
  symExprToReplace->replace(primCall);

  return true;
}

static CallExpr *findMaybeAggAssignInBlock(BlockStmt *block) {
  Expr *cur = block->body.last();
  
  // at this point, only skippable call seems like PRIM_END_OF_STATEMENT, so
  // just skip that and give up after.
  if (CallExpr *curCall = toCallExpr(cur)) {
    if (curCall->isPrimitive(PRIM_END_OF_STATEMENT)) {
      cur = cur->prev;
    }
  }

  if (CallExpr *curCall = toCallExpr(cur)) {
    if (curCall->isPrimitive(PRIM_MAYBE_AGGREGATE_ASSIGN)) {
      return curCall;
    }
  }

  return NULL;
}

static void removeAggregatorFromMaybeAggAssign(CallExpr *call, int argIndex) {
  INT_ASSERT(call->isPrimitive(PRIM_MAYBE_AGGREGATE_ASSIGN));

  SymExpr *aggregatorSE = toSymExpr(call->get(argIndex));
  INT_ASSERT(aggregatorSE);
  Symbol *aggregator = aggregatorSE->symbol();
  if (aggregator != gNil) {
    // remove the definition
    aggregator->defPoint->remove();

    // replace the SymExpr in call
    aggregatorSE->replace(new SymExpr(gNil));

    // find and remove other SymExprs within the function
    std::vector<SymExpr *> symExprsToRemove;
    Symbol *parentSym = call->parentSymbol;
    collectSymExprsFor(parentSym, aggregator, symExprsToRemove);
    for_vector(SymExpr, se, symExprsToRemove) {
      se->remove();
    }
  }
}

static void findAndUpdateMaybeAggAssign(CallExpr *call, bool confirmed) {
  bool unaggregatable = false;
  if (call->isPrimitive(PRIM_MAYBE_LOCAL_ARR_ELEM)) {
    if (!confirmed) {
      unaggregatable = true;
    }
  }

  bool lhsOfMaybeAggAssign = false;
  bool rhsOfMaybeAggAssign = false;
  Symbol *tmpSym = NULL;
  if (CallExpr *parentCall = toCallExpr(call->parentExpr)) {
    if (parentCall->isPrimitive(PRIM_MOVE)) {
      SymExpr *lhsSE = toSymExpr(parentCall->get(1));
      INT_ASSERT(lhsSE);

      Symbol *lhsSym = lhsSE->symbol();
      if (lhsSym->hasFlag(FLAG_EXPR_TEMP)) {
        tmpSym = lhsSym;
      }
    }
  }

  if (tmpSym) {
    if (CallExpr *maybeAggAssign = findMaybeAggAssignInBlock(call->getScopeBlock())) {


      if (fReportAutoAggregation) {
        std::stringstream message;

        SymExpr *aggMarkerSE = toSymExpr(maybeAggAssign->get(7));
        INT_ASSERT(aggMarkerSE);

        message << "Aggregation candidate ";
        message << "has ";
        message << (confirmed ? "confirmed" : "reverted");
        message << " local child ";
        message << getForallCloneTypeStr(aggMarkerSE->symbol());
        LOG_AA(0, message.str().c_str(), call);
      }

      if (unaggregatable) {
        removeAggregatorFromMaybeAggAssign(maybeAggAssign, 3);
        removeAggregatorFromMaybeAggAssign(maybeAggAssign, 4);
      }
      else {
        SymExpr *lhsSE = toSymExpr(maybeAggAssign->get(1));
        INT_ASSERT(lhsSE);

        SymExpr *rhsSE = toSymExpr(maybeAggAssign->get(2));
        INT_ASSERT(rhsSE);

        lhsOfMaybeAggAssign = (lhsSE->symbol() == tmpSym);
        rhsOfMaybeAggAssign = (rhsSE->symbol() == tmpSym);

        if (lhsOfMaybeAggAssign || rhsOfMaybeAggAssign) {
          // at most one can be true
          INT_ASSERT(lhsOfMaybeAggAssign != rhsOfMaybeAggAssign);

          int flagIndex = lhsOfMaybeAggAssign ? 5 : 6;
          SymExpr *controlFlag = new SymExpr(confirmed ? gTrue : gFalse);
          maybeAggAssign->get(flagIndex)->replace(controlFlag);

          int aggToRemoveIndex = lhsOfMaybeAggAssign ? 3 : 4;
          removeAggregatorFromMaybeAggAssign(maybeAggAssign, aggToRemoveIndex);
        }
      }
    }
  }
}

