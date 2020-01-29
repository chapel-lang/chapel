/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#include "addAutoDestroyCalls.h"



#include "astutil.h"
#include "AutoDestroyScope.h"
#include "AstVisitorTraverse.h"
#include "DeferStmt.h"
#include "errorHandling.h"
#include "expr.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "LoopStmt.h"
#include "resolution.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "wellknown.h"

#include <vector>

// last mention map:
// Maps from expressions to the list of variables to be destroy after
// that expression.
typedef std::map<Expr*, std::vector<VarSymbol*> > LastMentionMap;

static void computeLastMentionPoints(LastMentionMap& lmm, FnSymbol* fn);

static void walkBlock(FnSymbol*         fn,
                      AutoDestroyScope* parent,
                      BlockStmt*        block,
                      std::set<VarSymbol*>& ignoredVariables,
                      LastMentionMap&   lmm,
                      ForallStmt*       pfs = NULL);

static void walkBlockWithScope(AutoDestroyScope& scope,
                               FnSymbol*         fn,
                               BlockStmt*        block,
                               std::set<VarSymbol*>& ignoredVariables,
                               LastMentionMap&   lmm);

void addAutoDestroyCalls() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    std::set<VarSymbol*> ignoredVariables;
    LastMentionMap lmm;
    computeLastMentionPoints(lmm, fn);
    walkBlock(fn, NULL, fn->body, ignoredVariables, lmm);
  }

  // Finally, remove all defer statements, since they have been lowered.
  forv_Vec(DeferStmt, defer, gDeferStmts) {
    defer->remove();
  }
}

/************************************* | **************************************
*                                                                             *
* Walk the statements for a function's top-level BlockStmt                    *
*                                                                             *
*   1) Collect variables that are marked to be auto destroyed                 *
*                                                                             *
*   2) Handle recursion in to nested sub blocks                               *
*                                                                             *
*   3) Insert auto destroy calls at the end of the block                      *
*                                                                             *
*      Functions with multiple return points require additional care when     *
*      traversing a function's top-level block.                               *
*                                                                             *
*      Nested return statements are implemented as one or more PRIM_MOVEs to  *
*      the return variable followed by a GOTO to the return label. The        *
*      return value may be different at each return and so the autoDestroy    *
*      sets for the locals are handled immediately before each GOTO.          *
*                                                                             *
*      Functions that accept formals with OUT intent or INOUT intent rely on  *
*      formal temps within the function.  These formal temps are written to   *
*      the final destination using compiler generated code that appears       *
*      immediately before the return statement and will be shared by every    *
*      return path. We refer to this code as the "function epilogue".         *
*                                                                             *
*      The function epilogue is informally defined as                         *
*                                                                             *
*        a) The return label for the common exit point                        *
*        b) The first compiler-generated statement to write to a formal temp  *
*                                                                             *
*      but is not formally represented in the AST. Hence we require code to   *
*      detect the start of epilogue.                                          *
*                                                                             *
* A final complication is that the current implementation may find goto       *
* statements in the middle of a BlockStmt; this is not currently removed      *
* before this phase.  To be conservative, we arrange to ignore any such       *
* dead code.  The possibility of multiple returns makes this a little         *
* convoluted for the function's top level block statement.                    *
*                                                                             *
************************************** | *************************************/

static VarSymbol*   definesAnAutoDestroyedVariable(const Expr* stmt);
static VarSymbol* possiblyInitializesDestroyedVariable(Expr* stmt);
static LabelSymbol* findReturnLabel(FnSymbol* fn);
static bool         isReturnLabel(const Expr*        stmt,
                                  const LabelSymbol* returnLabel);
static bool         isErrorLabel(const Expr*        stmt);
static bool         isYieldStmt(const Expr* stmt);
static void         walkForallBlocks(FnSymbol* fn,
                                     AutoDestroyScope* parentScope,
                                     ForallStmt* forall,
                                     std::set<VarSymbol*>& parentIgnored,
                                     LastMentionMap& lmm);

static void gatherIgnoredVariablesForYield(
    Expr* stmt,
    std::set<VarSymbol*>& ignoredVariables);

//
// A ForallStmt index variable does not have a DefExprs in the loop body.
// Yet, it needs to be autoDestroyed at the end of the block.
// For that to happen, we add it to 'scope'.
//
static void addForallIndexVarToScope(AutoDestroyScope* scope,
                                     ForallStmt* forall)
{
  VarSymbol* idx = parIdxVar(forall);
  if (isAutoDestroyedVariable(idx)) {
    INT_ASSERT(!idx->isRef()); // no destruction for ref iterators
    scope->variableAdd(idx);
    scope->addInitialization(idx);
  }
}

static void walkBlockScopelessBlock(AutoDestroyScope& scope,
                                    FnSymbol*         fn,
                                    LabelSymbol*      retLabel,
                                    bool              isDeadCode,
                                    BlockStmt*        block,
                                    std::set<VarSymbol*>& ignoredVariables,
                                    LastMentionMap&   lmm);

static void checkSplitInitOrder(CondStmt* cond,
                                std::vector<VarSymbol*>& thenOrder,
                                std::vector<VarSymbol*>& elseOrder);

// Returns the next statement to traverse
static Expr* walkBlockStmt(FnSymbol*         fn,
                           AutoDestroyScope& scope,
                           LabelSymbol*      retLabel,
                           bool              isDeadCode,
                           bool              inScopelessBlock,
                           Expr*             stmt,
                           std::set<VarSymbol*>& ignoredVariables,
                           LastMentionMap&   lmm) {

  Expr* next = stmt->next;

  //
  // Handle the current statement
  //

  // TODO -- maybe we need to handle breakLabel and continueLabel here?

  // Once a variable is yielded, it should no longer be auto-destroyed,
  // since such destruction is the responsibility of
  // the calling loop.
  if (isYieldStmt(stmt)) {
    gatherIgnoredVariablesForYield(stmt, ignoredVariables);

  // AutoDestroy locals at the start of the error-handling label
  // (when exiting a try block without error)
  } else if (isErrorLabel(stmt) == true) {
    INT_ASSERT(!inScopelessBlock); // situation leads to miscompiles/leaks

    scope.insertAutoDestroys(fn, stmt, ignoredVariables);

    std::vector<VarSymbol*> outerInits = scope.getInitedOuterVars();
    scope.forgetOuterVariableInitializations();

    // Consider the catch blocks now
    for (Expr* cur = stmt->next; cur != NULL; cur = next) {
      next = walkBlockStmt(fn, scope, retLabel, false, false, cur,
                           ignoredVariables, lmm);
    }

    // Now once again consider the outer variables initialized
    for_vector(VarSymbol, var, outerInits) {
      scope.addInitialization(var);
    }

  // AutoDestroy primary locals at start of function epilogue (1)
  } else if (isReturnLabel(stmt, retLabel) == true) {
    scope.insertAutoDestroys(fn, stmt, ignoredVariables);

  // Be conservative about unreachable code before the epilogue
  } else if (isDeadCode == false) {
    // Collect variables that should be autoDestroyed
    if (VarSymbol* var = definesAnAutoDestroyedVariable(stmt)) {
      scope.variableAdd(var);

    // Collect defer statements to run during cleanup
    } else if (DeferStmt* defer = toDeferStmt(stmt)) {
      scope.deferAdd(defer);

    // AutoDestroy primary locals at start of function epilogue (2)
    } else if (scope.handlingFormalTemps(stmt) == true) {
      scope.insertAutoDestroys(fn, stmt, ignoredVariables);

    } else if (VarSymbol* var = possiblyInitializesDestroyedVariable(stmt)) {
      // note that this case will run also when setting the variable
      // after the 1st initialization. That should be OK though because
      // once a variable is initialized, it stays initialized, until
      // it is destroyed.

      // TODO: fix test/errhandling/ferguson/loopexprs-caught.chpl
      if (isCheckErrorStmt(stmt->next)) {
        // Visit the check-error block now - do not consider
        // the variable initialized when running that check-error block.
        next = walkBlockStmt(fn, scope, retLabel, false, false, stmt->next,
                             ignoredVariables, lmm);
      }

      scope.addInitialization(var);

    // Recurse in to a BlockStmt (or sub-classes of BlockStmt e.g. a loop)
    } else if (BlockStmt* subBlock = toBlockStmt(stmt)) {
      // ignore scopeless blocks for deciding where to destroy
      if ((subBlock->blockTag & BLOCK_SCOPELESS))
        walkBlockScopelessBlock(scope, fn, retLabel, isDeadCode,
                                subBlock, ignoredVariables, lmm);
      else
        walkBlock(fn, &scope, subBlock, ignoredVariables, lmm);

    // Recurse in to a ForallStmt
    } else if (ForallStmt* forall = toForallStmt(stmt)) {
      walkForallBlocks(fn, &scope, forall, ignoredVariables, lmm);

    // Recurse in to the BlockStmt(s) of a CondStmt
    } else if (CondStmt*  cond     = toCondStmt(stmt))  {

      //std::set<VarSymbol*> toIgnore(ignoredVariables);

      if (isCheckErrorStmt(cond)) {
        INT_ASSERT(!cond->elseStmt);
        walkBlock(fn, &scope, cond->thenStmt, ignoredVariables, lmm);

      } else if (cond->elseStmt == NULL) {
        // Traverse into the if branch only
        walkBlock(fn, &scope, cond->thenStmt, ignoredVariables, lmm);
      } else {
        // includes an if and an else. Split init is possible within these,
        // and we need to ensure that the order of initializations matches.

        // visit the then block
        AutoDestroyScope thenScope(&scope, cond->thenStmt);
        walkBlockWithScope(thenScope, fn, cond->thenStmt, ignoredVariables, lmm);
        // Gather the inited outer vars from the then block
        std::vector<VarSymbol*> thenOrder = thenScope.getInitedOuterVars();
        // When we visit the else block, outer variables that were initialized
        // in the then block shouldn't be considered initialized.
        thenScope.forgetOuterVariableInitializations();

        // visit the else block
        AutoDestroyScope elseScope(&scope, cond->elseStmt);
        walkBlockWithScope(elseScope, fn, cond->elseStmt, ignoredVariables, lmm);
        std::vector<VarSymbol*> elseOrder = elseScope.getInitedOuterVars();

        // check that the outer variables are initialized in the same order.
        checkSplitInitOrder(cond, thenOrder, elseOrder);
      }
    }
  }

  // Destroy the variable after this statement if it's the last mention
  // Since this adds the destroy immediately after this statement,
  // it ends up destroying multiple variables to be destroyed here
  // in the reverse order of the vector - i.e. reverse initialization order.
  LastMentionMap::const_iterator lmmIt = lmm.find(stmt);
  if (lmmIt != lmm.end()) {
    const std::vector<VarSymbol*>& vars = lmmIt->second;
    for_vector(VarSymbol, var, vars) {
      scope.destroyVariable(stmt, var, ignoredVariables);

      // Needs a better strategy if we move last mention points within
      // conditionals
      ignoredVariables.insert(var);
    }
  }

  return next;
}

static void walkBlockScopelessBlock(AutoDestroyScope& scope,
                                    FnSymbol*         fn,
                                    LabelSymbol*      retLabel,
                                    bool              isDeadCode,
                                    BlockStmt*        block,
                                    std::set<VarSymbol*>& ignoredVariables,
                                    LastMentionMap&   lmm) {
  Expr* next = NULL;
  for (Expr* stmt = block->body.first(); stmt != NULL; stmt = next) {
    next = walkBlockStmt(fn, scope, retLabel, isDeadCode, true, stmt,
                         ignoredVariables, lmm);
  }
}

static void checkSplitInitOrder(CondStmt* cond,
                                std::vector<VarSymbol*>& thenOrder,
                                std::vector<VarSymbol*>& elseOrder) {
  // normalizer should prevent these two from initializing different
  // variables.
  bool ok = true;

  if (thenOrder.size() == elseOrder.size()) {
    // common case
    size_t size = thenOrder.size();
    for (size_t i = 0; i < size; i++) {
      if (thenOrder[i] != elseOrder[i]) {
        ok = false;
        break;
      }
    }
  } else {
    // one of the branches might have returned early,
    // so only consider variables that are initialized in both.
    std::vector<VarSymbol*> thenOrder2;
    std::vector<VarSymbol*> elseOrder2;
    std::set<VarSymbol*> thenSet;
    std::set<VarSymbol*> elseSet;

    for_vector(VarSymbol, var, thenOrder) {
      thenSet.insert(var);
    }
    for_vector(VarSymbol, var, elseOrder) {
      elseSet.insert(var);
    }
    for_vector(VarSymbol, var, thenOrder) {
      if (elseSet.count(var) != 0)
        thenOrder2.push_back(var);
    }
    for_vector(VarSymbol, var, elseOrder) {
      if (thenSet.count(var) != 0)
        elseOrder2.push_back(var);
    }
    INT_ASSERT(thenOrder2.size() == elseOrder2.size());
    size_t size = thenOrder2.size();
    for (size_t i = 0; i < size; i++) {
      if (thenOrder2[i] != elseOrder2[i]) {
        ok = false;
        break;
      }
    }
  }

  if (!ok) {
    USR_FATAL_CONT(cond, "Initialization order in conditional does not match");
    std::string thenMsg = "then initializes:";
    for_vector(VarSymbol, var, thenOrder) {
      thenMsg += " ";
      thenMsg += var->name;
      if (developer) {
        thenMsg += "[";
        thenMsg += istr(var->id);
        thenMsg += "]";
      }
    }
    std::string elseMsg = "else initializes:";
    for_vector(VarSymbol, var, elseOrder) {
      elseMsg += " ";
      elseMsg += var->name;
      if (developer) {
        elseMsg += "[";
        elseMsg += istr(var->id);
        elseMsg += "]";
      }
    }
    USR_PRINT(cond->thenStmt, thenMsg.c_str());
    USR_PRINT(cond->elseStmt, elseMsg.c_str());
  }
}

static void walkBlock(FnSymbol*         fn,
                      AutoDestroyScope* parent,
                      BlockStmt*        block,
                      std::set<VarSymbol*>& ignoredVariables,
                      LastMentionMap&   lmm,
                      ForallStmt*       pfs) {
  AutoDestroyScope scope(parent, block);

  if (pfs != NULL)
    addForallIndexVarToScope(&scope, pfs);

  walkBlockWithScope(scope, fn, block, ignoredVariables, lmm);
}

static void walkBlockWithScope(AutoDestroyScope& scope,
                               FnSymbol*         fn,
                               BlockStmt*        block,
                               std::set<VarSymbol*>& ignoredVariables,
                               LastMentionMap&   lmm) {
  AutoDestroyScope* parent    = scope.getParentScope();
  LabelSymbol*     retLabel   = (parent == NULL) ? findReturnLabel(fn) : NULL;
  bool             isDeadCode = false;

  Expr* next = NULL;
  for (Expr* stmt = block->body.first(); stmt != NULL; stmt = next) {

    //
    // Handle the current statement
    //
    next = walkBlockStmt(fn, scope, retLabel, isDeadCode, false, stmt,
                         ignoredVariables, lmm);

    //
    // Handle the end of a block
    // For the purposes of this pass, a block ends either
    // with a GotoStmt or when we run out of next statements.
    //
    GotoStmt* gotoStmt = toGotoStmt(stmt);
    if (gotoStmt != NULL || next == NULL) {

      // Don't visit any later code in this block
      // (don't add variable definitions, etc, above).
      isDeadCode = true;

      // The main block for a function or a simple sub-block
      if (parent == NULL || gotoStmt == NULL) {
        scope.insertAutoDestroys(fn, stmt, ignoredVariables);

      // Currently unprepared for a nested scope that ends in a goto
      } else {
        switch (gotoStmt->gotoTag) {
          case GOTO_RETURN:
          case GOTO_CONTINUE:
          case GOTO_BREAK:
          case GOTO_ERROR_HANDLING:
          case GOTO_BREAK_ERROR_HANDLING:
            scope.insertAutoDestroys(fn, stmt, ignoredVariables);
            break;

          case GOTO_NORMAL:
          case GOTO_GETITER_END:
          case GOTO_ITER_RESUME:
          case GOTO_ITER_END:
           // MDN 2016/03/18 Need to revisit these cases
           break;
        }
      }
    }
  }
}

// Is this a DefExpr that defines a variable that might be autoDestroyed?
static VarSymbol* definesAnAutoDestroyedVariable(const Expr* stmt) {
  VarSymbol* retval = NULL;

  if (const DefExpr* expr = toConstDefExpr(stmt)) {
    if (VarSymbol* var = toVarSymbol(expr->sym))
      retval = (isAutoDestroyedVariable(var) == true) ? var : NULL;
  }

  return retval;
}

// Is this a CallExpr that initializes a variable that might be destroyed?
// If so, return the VarSymbol initialized. Otherwise, return NULL.
//
// Note, this must identify the first initialization, but it can also
// return a variable for other calls setting the variable (since the
// variable remains initialized).
static VarSymbol* possiblyInitializesDestroyedVariable(Expr* stmt) {

  if (CallExpr* call = toCallExpr(stmt)) {
    // case 1: PRIM_MOVE/PRIM_ASSIGN into a variable
    if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN))
      if (SymExpr* se = toSymExpr(call->get(1)))
        if (VarSymbol* var = toVarSymbol(se->symbol()))
          if (isAutoDestroyedVariable(var))
            return var;

    if (FnSymbol* calledFn = call->resolvedOrVirtualFunction()) {
      // case 2: init or init=
      if (calledFn->isMethod() &&
          (calledFn->name == astrInit || calledFn->name == astrInitEquals)) {
        SymExpr* se = toSymExpr(call->get(1));
        if (VarSymbol* var = toVarSymbol(se->symbol()))
          if (isAutoDestroyedVariable(var))
            return var;

      // case 3: return through ret-arg
      } else if (calledFn->hasFlag(FLAG_FN_RETARG)) {
        ArgSymbol* retArg = toArgSymbol(toDefExpr(calledFn->formals.tail)->sym);
        INT_ASSERT(retArg && retArg->hasFlag(FLAG_RETARG));
        // Find the corresponding actual, which is the last actual
        if (SymExpr* lastActual = toSymExpr(call->argList.tail))
          if (VarSymbol* var = toVarSymbol(lastActual->symbol()))
            if (isAutoDestroyedVariable(var))
              return var;
      }
    }
  }

  return NULL;
}

//
// The return label is one of the logical markers for the start of
// the function epilogue but is not currently well marked.
//
// Identify it by inspecting any goto statements.  If there are
// multiple jumps to the return label then verify they are all
// the same.
//

static LabelSymbol* findReturnLabel(FnSymbol* fn) {
  std::vector<GotoStmt*> gotoStmts;
  BlockStmt*             block  = fn->body;
  LabelSymbol*           retval = NULL;

  collectGotoStmts(block, gotoStmts);

  for_vector(GotoStmt, gotoStmt, gotoStmts) {
    if (gotoStmt->isGotoReturn() == true) {
      LabelSymbol* target = gotoStmt->gotoTarget();

      if (retval == NULL)
        retval = target;
      else
        INT_ASSERT(retval == target);
    }
  }

  return retval;
}

// Is this the definition of the return label?
static bool isReturnLabel(const Expr* stmt, const LabelSymbol* returnLabel) {
  bool retval = false;

  if (returnLabel != NULL) {
    if (const DefExpr*  expr = toConstDefExpr(stmt)) {
      if (LabelSymbol* label = toLabelSymbol(expr->sym)) {
        retval = (label == returnLabel) ? true : false;
      }
    }
  }

  return retval;
}

// Is this the definition of an error handling label?
static bool isErrorLabel(const Expr* stmt) {
  if (const DefExpr*  expr = toConstDefExpr(stmt)) {
    if (LabelSymbol* label = toLabelSymbol(expr->sym)) {
      if (label->hasFlag(FLAG_ERROR_LABEL))
        return true;
    }
  }

  return false;
}

static bool isYieldStmt(const Expr* stmt) {
  if (const CallExpr* call = toConstCallExpr(stmt))
    if (call->isPrimitive(PRIM_YIELD))
      return true;

  return false;
}

// Helper for walkBlock() to walk everything for a ForallStmt.
static void walkForallBlocks(FnSymbol* fn,
                             AutoDestroyScope* parentScope,
                             ForallStmt* forall,
                             std::set<VarSymbol*>& parentIgnored,
                             LastMentionMap& lmm)
{
  std::set<VarSymbol*> toIgnoreLB(parentIgnored);
  walkBlock(fn, parentScope, forall->loopBody(), toIgnoreLB, lmm, forall);

  for_shadow_vars(svar, temp, forall)
    if (!svar->initBlock()->body.empty() || !svar->deinitBlock()->body.empty())
      {
        // I am unsure about these recursive walkBlock() calls, specifically
        //  * should 'toIgnoreSV' start out with 'parentIgnored'?
        //  * is it appropriate to reference 'fn' ?  -vass 1/2018
        std::set<VarSymbol*> toIgnoreSV(parentIgnored);
        walkBlock(fn, parentScope, svar->initBlock(), toIgnoreSV, lmm);
        walkBlock(fn, parentScope, svar->deinitBlock(), toIgnoreSV, lmm);
      }
}

static void gatherIgnoredVariablesForYield(
    Expr* yieldStmt,
    std::set<VarSymbol*>& ignoredVariables)
{
  CallExpr* yield = toCallExpr(yieldStmt);
  SymExpr* yieldedSe = toSymExpr(yield->get(1));
  INT_ASSERT(yieldedSe);
  VarSymbol* yieldedVar = toVarSymbol(yieldedSe->symbol());
  INT_ASSERT(yieldedVar);
  QualifiedType t = yieldedVar->qualType();

  if (typeNeedsCopyInitDeinit(t.type()) && ! t.isRef()) {

    SymExpr* foundSe = findSourceOfYield(yield);
    VarSymbol* var = toVarSymbol(foundSe->symbol());

    if (var &&
        var->hasEitherFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW,
                           FLAG_EXPR_TEMP))
      ignoredVariables.insert(var);
  }
}

class ComputeLastSymExpr : public AstVisitorTraverse
{
  public:
    std::vector<VarSymbol*>& inited;
    std::set<VarSymbol*> initedSet;
    std::map<VarSymbol*, Expr*>& last;
    ComputeLastSymExpr(std::vector<VarSymbol*>& inited,
                       std::map<VarSymbol*, Expr*>& last)
      : inited(inited), last(last) { }
    virtual bool enterCallExpr(CallExpr* node);
    virtual void visitSymExpr(SymExpr* node);
    virtual void exitForallStmt(ForallStmt* node);
};

static Expr* findLastExprInStatement(Expr* e, VarSymbol* v);

static void computeLastMentionPoints(LastMentionMap& lmm, FnSymbol* fn) {

  std::vector<VarSymbol*> inited;
  std::map<VarSymbol*, Expr*> last;

  // Use a traversal to compute the last SymExpr mentioning each
  ComputeLastSymExpr visitor(inited, last);
  fn->body->accept(&visitor);

  // Store the gathered DefExprs in the appropriate place in the inverse
  // map. The map creates vectors in order of initialization.
  for_vector(VarSymbol, var, inited) {
    std::map<VarSymbol*, Expr*>::iterator it = last.find(var);
    if (it != last.end()) {
      Expr* point = it->second;
      // find the end of statement
      point = findLastExprInStatement(point, var);
      if (point != NULL) {
        // store in the inverse map
        lmm[point].push_back(var);
      }
    }
  }
}

static bool shouldDestroyOnLastMention(VarSymbol* var) {
  return var->hasFlag(FLAG_DEAD_LAST_MENTION) && // dead at last mention
         isAutoDestroyedVariable(var) &&
         // forall statement exception avoids certain variables
         // within forall statements such as fRecIterIRdef.
         !isForallStmt(var->defPoint->parentExpr);
}

bool ComputeLastSymExpr::enterCallExpr(CallExpr* node) {
  if (VarSymbol* var = possiblyInitializesDestroyedVariable(node)) {
    if (shouldDestroyOnLastMention(var)) {
      if (initedSet.insert(var).second) {
        // the first potential initialization
        inited.push_back(var);
      }
    }
  }

  return true;
}

void ComputeLastSymExpr::visitSymExpr(SymExpr* node) {
  if (VarSymbol* var = toVarSymbol(node->symbol()))
    if (isFnSymbol(node->parentSymbol))
      if (shouldDestroyOnLastMention(var))
        last[var] = node;
}

void ComputeLastSymExpr::exitForallStmt(ForallStmt* node) {
  // visit the shadow variables and record them as uses at the forall
  for_alist(elt, node->shadowVariables()) {
    if (DefExpr* def = toDefExpr(elt))
      if (ShadowVarSymbol* svar = toShadowVarSymbol(def->sym))
        if (VarSymbol* var = toVarSymbol(svar->outerVarSym()))
          if (shouldDestroyOnLastMention(var))
            last[var] = node; // make outer variable's last the forall itself
  }
}

// Returns true for statements that are expressions when
// their type would normally be a statement,
// e.g. if-expr, for-expr, forall-expr
// Also returns 'true' for IfExprs implementing error handling
//  (handling control flow after an error is thrown by a call)
static bool stmtIsExpr(Expr* e) {
  if (CondStmt* cond = toCondStmt(e))
    return cond->isIfExpr() || isCheckErrorStmt(e);
  if (ForLoop* forLoop = toForLoop(e))
    return forLoop->isForExpr();
  if (ForallStmt* forall = toForallStmt(e))
    return forall->isForallExpr();

  return false;
}

static Expr* findLastExprInStatement(Expr* e, VarSymbol* v) {

  Expr* stmt = e->getStmtExpr();

  // make it "end of block" if it's returned
  if (CallExpr* call = toCallExpr(stmt))
    if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN))
      if (SymExpr* lhsSe = toSymExpr(call->get(1)))
        if (SymExpr* rhsSe = toSymExpr(call->get(2)))
          if (lhsSe->symbol()->hasFlag(FLAG_RVV))
            if (rhsSe->symbol() == v)
              return NULL;

  // Go up in any complex nested statements until we reach the same level
  // as the DefExpr.
  // Note, forall index vars are just in a ForallStmt (not a block)
  Expr* defParent = v->defPoint->parentExpr;
  INT_ASSERT(defParent);

  for (Expr* cur = stmt;
       cur != NULL && cur != defParent;
       cur = cur->parentExpr) {
    stmt = cur;
  }

  // Scroll forward (starting at stmt) until we reach:
  //  * the last stmt expr before label or end of block
  //  * a PRIM_END_OF_STATEMENT or chpl_statementLevelSymbol
  //  * a task function call (these are already statements)
  //  * a block, conditional, or loop (these are already statements)
  for (Expr* cur = stmt; cur != NULL; cur = cur->next) {
    if (DefExpr* def = toDefExpr(cur))
      if (isLabelSymbol(def->sym))
        break; // label statement reached - use last stmt

    stmt = cur;

    if (CallExpr* call = toCallExpr(cur)) {
      if (call->isPrimitive(PRIM_END_OF_STATEMENT))
        break;
      if (call->isNamedAstr(astr_chpl_statementLevelSymbol))
        break;
      if (FnSymbol* fn = call->resolvedFunction())
        if (isTaskFun(fn))
          break;
    }
    if (stmtIsExpr(cur)) {
      // e.g. an if-expr; keep going
    } else if (isBlockStmt(cur) || isCondStmt(cur) ||
               isLoopStmt(cur) || isForallStmt(cur)) {
      break; // full statement reached (but see fixups below)
    }
  }

  // Fix up for extra AST outside of loop statements
  if (isForallStmt(stmt)) {
    // if it's a forall statement, the end of the variable lifetime
    // is after any number of chpl_after_forall_fence calls.
    for (Expr* e = stmt->next; e != NULL; e = e->next) {
      CallExpr* call = toCallExpr(e);
      FnSymbol* calledFn = NULL;
      if (call != NULL) {
        calledFn = call->resolvedFunction();
      }
      if (calledFn == gChplAfterForallFence)
        stmt = call;
      else
        break;
    }
  } else if (LoopStmt* loop = toLoopStmt(stmt)) {
    // If it's a LoopStmt with a break label, the end of the variable
    // lifetime is after the break label DefExpr.
    if (LabelSymbol* breakLabel = loop->breakLabelGet()) {
      if (DefExpr* breakDef = breakLabel->defPoint) {
        stmt = breakDef;
      }
    }
  }

  // Check if the early deinit point is the same as the the
  // usual (end of block) deinit point. If it is, treat the variable
  // as end-of-block to simplify matters.
  GotoStmt* gotoStmt = toGotoStmt(stmt);
  if (gotoStmt != NULL || stmt->next == NULL) {
    // it's the end of something... is the end of of the block for the DefExpr?
    if (defParent == stmt->parentExpr)
      return NULL;
  }

  // Check if the call is a yield or return. In that event, treat the
  // variable as end-of-block to simplify matters.
  if (CallExpr* call = toCallExpr(stmt))
    if (call->isPrimitive(PRIM_YIELD) || call->isPrimitive(PRIM_RETURN))
      return NULL;

  return stmt; // last statement in the end of *some* block
}

/************************************* | **************************************
*                                                                             *
* A shared utility function for resolution.                                   *
*                                                                             *
************************************** | *************************************/

bool isAutoDestroyedVariable(Symbol* sym) {
  bool retval = false;

  if (VarSymbol* var = toVarSymbol(sym)) {
    if (var->hasFlag(FLAG_NO_AUTO_DESTROY)     == false &&
        (var->hasFlag(FLAG_INSERT_AUTO_DESTROY) == true ||
         var->hasFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW) == true)) {
      if (!var->isParameter() && !var->isType()) {

        retval = (autoDestroyMap.get(var->type) != NULL);
      }
    }
  }

  return retval;
}

// For a yield of a variable, such as iter f() { var x=...; yield x; },
// unlike returning, the yield will result in a copy. This function helps
// to identify the source variable in such cases, even as there might be
// various compiler temporaries in between. It returns the SymExpr referring
// to the yielded variable that was used in the last PRIM_MOVE reading it.
SymExpr* findSourceOfYield(CallExpr* yield) {

  SymExpr* yieldedSe = toSymExpr(yield->get(1));
  Symbol* yieldedSym = yieldedSe->symbol();

  Expr* expr = yield;
  Symbol* needle = yieldedSym;
  SymExpr* foundSe = yieldedSe;

  // Walk backwards to figure out where the yielded symbol came
  // from and to find the spot at which we should add an
  // autoCopy call.
  while (expr != NULL && needle != NULL) {
    if (CallExpr* move = toCallExpr(expr)) {
      if (move->isPrimitive(PRIM_MOVE) == true) {
        SymExpr*   lhs    = toSymExpr(move->get(1));
        VarSymbol* lhsVar = toVarSymbol(lhs->symbol());

        if (needle == lhsVar) {
          if (SymExpr* rhs = toSymExpr(move->get(2))) {
            VarSymbol* rhsVar = toVarSymbol(rhs->symbol());

            if (rhsVar) {
              needle = rhsVar;

              if (rhsVar->hasFlag(FLAG_INSERT_AUTO_DESTROY) ||
                  rhsVar->hasFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW)) {
                foundSe = rhs;
                break;
              }
            }
          } else {
            needle = NULL;
          }
        }
      }
    }

    expr = expr->prev;
  }

  return foundSe;
}

