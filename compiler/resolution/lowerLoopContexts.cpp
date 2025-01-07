/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#include <memory>

// todo: can any of these be removed?
#include "astutil.h"
#include "AstVisitorTraverse.h"
#include "CForLoop.h"
#include "driver.h"
#include "errorHandling.h"
#include "expr.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "iterator.h"
#include "lowerLoopContexts.h"
#include "optimizations.h"
#include "passes.h"
#include "resolution.h"
#include "resolveIntents.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "view.h"
#include "wellknown.h"

#include "global-ast-vecs.h"

static bool isInUserCode(BaseAST* node) {
  if (ModuleSymbol* mod = node->getModule()) {
    return mod->modTag == MOD_USER;
  }
  return false;
}

static bool fnCanHaveContext(FnSymbol* fn) {
  // TODO make sure it is not cobegin ?
  return (fn->hasFlag(FLAG_COBEGIN_OR_COFORALL) ||
          fn->hasFlag(FLAG_ON)) &&
         fn->singleInvocation() != NULL;

}

static std::string showid(BaseAST* node, const char* endstring="") {
  std::string result;
  if (developer) result = "[" + std::to_string(node->id) + "]" + endstring;
  return result;
}
static void CONTEXT_DEBUG(int indent, std::string msg,
                          BaseAST* node, Symbol* sym = nullptr) {
  if (fReportContextAdj) {
    for (int i=0 ; i<indent ; i++) std::cout << " ";
    if (Symbol* nodeSym = toSymbol(node))
      std::cout << showid(node, "'") << nodeSym->name << "' " << msg;
    else
      std::cout << showid(node, " ") << msg;
    if (sym != nullptr) std::cout << " '" << sym->name << showid(sym) << "'";
    std::cout << std::endl;
  }
}

static int findFormalIndex(FnSymbol* fn, ArgSymbol* arg) {
  int i = 1;
  for_formals (formal, fn) {
    if (formal == arg) return i;
    i++;
  }
  return -1;
}

// This class factors out code related to handles, esp. findLoopContextHandle.
// Todo: perhaps switch its subclasses from "is a Context" to "has a Context"?
class Context {
  public:
  Symbol* localHandle_ = NULL;

  // TODO we can probably populate the bottom two together
  std::vector<CallExpr*> localHandleAutoDestroys_;
  Expr* endOfLocalHandleSetup_ = NULL;
  Expr* upEndCount_ = NULL;

  bool hasLocalHandle() {
    return localHandle_ != nullptr;
  }

  // this'll need to be differentiated between LoopContext and IteratorContext
  // when we have a proper syntax. The current implementation is more suitable
  // for IteratorContext
  void findLoopContextHandle() {
    const int debugDepth = 1;
    std::vector<DefExpr*> defExprs;
    collectDefExprs(this->node(), defExprs);

    for_vector (DefExpr, def, defExprs) {
      //CONTEXT_DEBUG(debugDepth+1, "looking at DefExpr", def);

      if (defExprIsLocalHandle(def)) {
        if (localHandle_ == NULL) {
          CONTEXT_DEBUG(debugDepth+2, "found a context handle", def->sym);
          localHandle_ = def->sym;
        }
        else {
          // For now, ignore other context handles. Slight challenge: the innermost
          // coforall_fn also contains the loop in question. So when trying to
          // find the coforall_fn's context handles, we should avoid looking at
          // loop's.

          //CONTEXT_DEBUG(debugDepth+2, "found another context handle?", def->sym);
          //INT_FATAL("found another context handle?");
        }
      }
    }
  }

  void collectLocalHandleAutoDestroys() {
    std::vector<CallExpr*> calls;
    collectCallExprs(this->node(), calls);

    for_vector(CallExpr, call, calls) {
      if (FnSymbol* fn = call->resolvedFunction()) {
        if (fn->hasFlag(FLAG_AUTO_DESTROY_FN)) {
          if (toSymExpr(call->get(1))->symbol() == localHandle_) {
            this->localHandleAutoDestroys_.push_back(call);
          }
        }
      }
    }
  }

  std::vector<CallExpr*>& getLocalHandleAutoDestroys() {
    if (localHandleAutoDestroys_.size() == 0) {
      collectLocalHandleAutoDestroys();
    }
    return localHandleAutoDestroys_;
  }

  Expr* getEndOfLocalHandleSetup() {
    if (endOfLocalHandleSetup_ == NULL) {
      // probably can iterate over the body
      std::vector<CallExpr*> calls;
      collectCallExprs(this->node(), calls);

      for_vector (CallExpr, call, calls) {
        if (call->isPrimitive(PRIM_MOVE)) {
          if (Symbol* lhs = toSymExpr(call->get(1))->symbol()) {
            if (lhs == localHandle_) {
              endOfLocalHandleSetup_ = call;
              break;
            }
          }
        }
      }
    }

    return endOfLocalHandleSetup_;
  }

  Expr* getUpEndCount() {
    if (upEndCount_ == NULL) {
      std::vector<CallExpr*> calls;
      collectCallExprs(this->node(), calls);

      for_vector (CallExpr, call, calls) {
        if (call->isNamed("_upEndCount")) {
          upEndCount_ = call;
          break;
        }
      }
    }
    return upEndCount_;
  }

  virtual BaseAST* node() = 0;

  bool defExprIsLocalHandle(DefExpr* def) {
    if (isArgSymbol(def->sym)) return false;

    //todo: the check for FLAG_CONTEXT_TYPE should suffice
    //      and perhaps FLAG_TEMP - ??
    return !def->sym->hasFlag(FLAG_TEMP) &&
           !isLabelSymbol(def->sym) &&
           !def->sym->hasFlag(FLAG_INDEX_VAR) && // avoid re-finding loop's
           !def->sym->hasFlag(FLAG_EPILOGUE_LABEL) && // same as !isLabelSymbol?
           def->sym->getValType()->symbol->hasFlag(FLAG_CONTEXT_TYPE);
  }

}; // class Context

class LoopContext: public Context {
  public:
  CForLoop* loop_;
  LoopContext(CForLoop* loop): loop_(loop) {}

  BaseAST* node() override { return loop_; };
};

class CoforallOnContext;
class VectorizedLoopContext;

class IteratorContext: public Context {
 protected:
  enum class Kind {
    CoforallOn,
    VectorizedLoop,
  };

  /** Entry-point information for the context inside this one. */
  struct {
    /**
     If the inner context is a coforall loop, it's represented as a call to
     a coforall_fn inside of a plain for-loop. This represents the for-loop.
     */
    CForLoop* innerLoop_ = NULL;

    /**
     If the inner context is a coforall loop or an on statement, it is represented
     using a call to some function, either coforall_fn or on_fn. This represents
     the call to that function.
     */
    CallExpr* callToInner_ = NULL;

    /**
      If inner context is the initial serial for-loop that triggered the context
      lowering, this represents that loop.
     */
    CForLoop* innermostLoop_ = NULL;
  };

  /** What kind of context this is, for casting. */
  Kind kind_;

  IteratorContext(Kind kind) : kind_(kind) {}

 public:
  void setInnerLoop(CForLoop* loop) { innerLoop_ = loop; }
  void setCallToInner(CallExpr* expr) { callToInner_ = expr; }
  void setInnermostLoop(CForLoop* loop) { innermostLoop_ = loop; }

  CoforallOnContext* toCoforallOnContext() {
    if (kind_ == Kind::CoforallOn) return (CoforallOnContext*) this;
    return nullptr;
  }

  VectorizedLoopContext* toVectorizedLoopContext() {
    if (kind_ == Kind::VectorizedLoop) return (VectorizedLoopContext*) this;
    return nullptr;
  }

  /**
    Determine the point right before the inner context, which hoisting should
    use as an anchor.
   */
  Expr* getInsertBeforeCallToInnerAnchor() const {
    if (innerLoop_) {
      return innerLoop_;
    } else if (callToInner_) {
      return callToInner_;
    } else {
      return innermostLoop_;
    }
  }

  /**
    If a new symbol has been introduced outside, and if the inner context
    is accessed via a call, add a new actual to the call to make that symbol
    available there. This also modifies the function being called to
    introduce the formal.

    Returns the symbol made available within the inner context.
   */
  Symbol* insertActualForOuterSymbol(Symbol* sym) {
    if (callToInner_) {
      callToInner_->insertAtTail(new SymExpr(sym));
      // TODO this is probably the right intent for now. But maybe we want
      // `const ref`?
      auto formal = new ArgSymbol(INTENT_CONST_IN, sym->name, sym->getValType());
      callToInner_->resolvedFunction()->insertFormalAtTail(formal);
      return formal;
    }
    // TODO: what do here
    return nullptr;
  }

  /**
    If a symbol is being hoisted past the inner context, and the context is
    accessed via a call, adds a new actual to the call to make the symbol available
    inside. This also modifies the function being called to introduce the formal.

    Returns the symbol made available within the inner context.
   */
  Symbol* insertActualForHoistedSymbol(Symbol* outerSym, const char* name, Type* type) {
    if (CallExpr* toAdjust = callToInner_) {
      toAdjust->insertAtTail(new SymExpr(outerSym));

      auto formal = new ArgSymbol(INTENT_REF, name, type);
      toAdjust->resolvedFunction()->insertFormalAtTail(formal);
      return formal;
    }
    return outerSym;
  }
 public:
  virtual ~IteratorContext() = default;

  virtual void dump(int depth) const = 0;
  virtual void recomputeActualSymbol(int& actualIdx, Symbol*& actual) const = 0;
}; // class IteratorContext

class CoforallOnContext : public IteratorContext {
 private:
  FnSymbol* fn_;
 public:
  CoforallOnContext(FnSymbol* fn) :
    IteratorContext(Kind::CoforallOn), fn_(fn) {}

  BaseAST* node() override { return fn_; };

  void dump(int depth) const override {
    std::string msg = "";
    if (fn_->hasFlag(FLAG_ON)) {
      msg += "on function with handle ";
    }
    else if (fn_->hasFlag(FLAG_COBEGIN_OR_COFORALL)) {
      msg += "coforall function with handle ";
    }
    msg += localHandle_->name;
    if (developer) {
      msg += "[" + std::to_string(localHandle_->id) + "]";
    }
    CONTEXT_DEBUG(depth, msg, fn_);
  }

  void recomputeActualSymbol(int& actualIdx, Symbol*& actual) const override {
    auto callToFn = fn_->singleInvocation();
    // what is the symbol that was passed to this function?
    actual = toSymExpr(callToFn->get(actualIdx))->symbol();

    // is it also an argument here?
    if (ArgSymbol* curFormal = toArgSymbol(actual)) {
      auto parentFn = toFnSymbol(callToFn->parentSymbol);
      INT_ASSERT(parentFn);
      int formalIdx = findFormalIndex(parentFn, curFormal);
      INT_ASSERT(formalIdx >= 0);
      actualIdx = formalIdx; // update to this function's formal idx
    } else {
      actualIdx = -1; // signal that this variable is local here
    }
  }
}; // class CoforallOnContext

class VectorizedLoopContext : public IteratorContext {
 private:
  CForLoop* loop_;

 public:
  VectorizedLoopContext(CForLoop* loop) :
    IteratorContext(Kind::VectorizedLoop), loop_(loop) {}

  BaseAST* node() override { return loop_; }

  void dump(int depth) const override {
    std::string msg = "vectorized loop with handle ";
    msg += localHandle_->name;
    if (developer) {
      msg += "[" + std::to_string(localHandle_->id) + "]";
    }
    CONTEXT_DEBUG(depth, msg, loop_);
  }

  void recomputeActualSymbol(int& actualIdx, Symbol*& actual) const override {
    // Do nothing, because vectorized loops aren't implemented as functions.
  }
}; // class VectorizedLoopContext

using IterContextPtr = std::unique_ptr<IteratorContext>;

class ContextHandler {
  public:
  LoopContext loopCtx_;
  std::vector<IterContextPtr> contextStack_;

  // map between any handle used within user's loop body and the indices to
  // contexts within contextStack
  std::map<Symbol*, int> handleMap_;

  ContextHandler(CForLoop* loop): loopCtx_(loop) {}

  void collectHandleAndOuterContexts() {
    const int debugDepth = 1;
    CForLoop* loop = loopCtx_.loop_;

    CONTEXT_DEBUG(debugDepth, "looking for the handle", loop);
    loopCtx_.findLoopContextHandle();

    if (loopCtx_.hasLocalHandle()) {
      CONTEXT_DEBUG(debugDepth+1, "found loop context handle",
                    loopCtx_.localHandle_);

      CONTEXT_DEBUG(debugDepth, "collecting context chain", loop);

      this->collectOuterContexts();
    }
    else {
      CONTEXT_DEBUG(debugDepth+1, "couldn't find context handle", loop);
    }
    dumpOuterContexts();
  }

  CForLoop* findInnermostLoop(Expr* curParent) {
    // TODO: this will find loops that aren't coforall lowering.
    //       how to fix?

    while (curParent) {
      if (CForLoop* loop = toCForLoop(curParent)) {
        return loop;
      }
      curParent = curParent->parentExpr;
    }
    return nullptr;
  }

  void collectOuterContexts() {
    const int debugDepth = 2;

    // If the current context was implemented using a plain loop, that loop.
    CForLoop* plainLoop = this->loop();
    // If the current context was implemented using an fn, the call to that fn.
    CallExpr* callToCurCtx = NULL;

    Expr* cur = plainLoop;
    do {
      if (cur->parentExpr) {
        if (CForLoop* cfl = toCForLoop(cur)) {
          if (cfl->isOrderIndependent()) {
            // Found a vectorization context.
            CONTEXT_DEBUG(debugDepth, "found a candidate vectorized loop", cfl);

            auto outerCtx = std::make_unique<VectorizedLoopContext>(cfl);
            outerCtx->setInnermostLoop(plainLoop);
            outerCtx->setCallToInner(callToCurCtx);
            outerCtx->setInnerLoop(findInnermostLoop(callToCurCtx));
            outerCtx->findLoopContextHandle();
            if (!outerCtx->hasLocalHandle()) break;

            contextStack_.push_back(std::move(outerCtx));
            cur = cur->parentExpr;
            callToCurCtx = NULL; // vectorized loops aren't impl'ed as fns.
            plainLoop = cfl;

            continue;
          }
        }

        // Not any expression we currently care about; keep looking upwards.
        cur = cur->parentExpr;
        continue;
      } else if (auto parentFn = toFnSymbol(cur->parentSymbol)) {
        // Functions that aren't coforall or on functions don't fit the
        // pattern. Stop building chain.
        if (!fnCanHaveContext(parentFn)) break;

        // Found a coforall/on context.
        CONTEXT_DEBUG(debugDepth, "found a candidate parent fn", parentFn);

        auto outerCtx = std::make_unique<CoforallOnContext>(parentFn);
        outerCtx->setInnermostLoop(plainLoop);
        outerCtx->setCallToInner(callToCurCtx);
        outerCtx->setInnerLoop(findInnermostLoop(callToCurCtx));
        outerCtx->findLoopContextHandle();
        if (!outerCtx->hasLocalHandle()) break;

        contextStack_.push_back(std::move(outerCtx));
        cur = callToCurCtx = parentFn->singleInvocation();
        plainLoop = NULL;
      } else {
        // Nothing fits the pattern, done searching.
        break;
      }
    } while (true);
  }

  void dumpOuterContexts() {
    int debugDepth = 2;
    if (contextStack_.size() == 0) {
      CONTEXT_DEBUG(debugDepth, "couldn't find any context chain", this->loop());
    } else {
      CONTEXT_DEBUG(debugDepth, "found the following context chain:", this->loop());
      for (auto& i : contextStack_) {
        i->dump(++debugDepth);
      }
    }
  }

  CForLoop* loop() { return toCForLoop(this->loopCtx_.loop_); }
  Symbol* loopHandle() { return this->loopCtx_.localHandle_; }

  void removeHoistToContextCall(CallExpr* call) {
    call->remove();
  }

  void removeOuterContextCallAndInitShadowHandle(CallExpr* call,
                                                 Symbol* symToSet) {
    INT_ASSERT(symToSet);

    if (CallExpr* parent = toCallExpr(call->parentExpr)) {
      // TODO this may be a little too reckless. At least do some INT_ASSERTS
      Symbol* lhs = toSymExpr(parent->get(1))->symbol();

      parent->remove();
      std::vector<SymExpr*> symExprs;
      collectSymExprsFor(loop(), lhs, symExprs);

      for_vector (SymExpr, symExpr, symExprs) {
        if (CallExpr* call = toCallExpr(symExpr->parentExpr)) {
          if (call->isNamed(astrInitEquals) || // do we need init=?
              call->isPrimitive(PRIM_MOVE)) {
            symExpr->replace(new SymExpr(symToSet));
            continue;
          }
        }
        symExpr->parentExpr->remove();
      }

      lhs->defPoint->remove();
    }
    call->remove();
  }

  Symbol* handleOuterContextCall(CallExpr* call) {
    const int debugDepth = 4;

    Symbol* outerCtxHandle = NULL;
    //
    // TODO better/less fragile pattern matching?
    CallExpr* parentCall = toCallExpr(call->parentExpr);
    INT_ASSERT(parentCall);
    INT_ASSERT(parentCall->isPrimitive(PRIM_MOVE));

    SymExpr* lhsSe = toSymExpr(parentCall->get(1));
    Symbol* lhs = lhsSe->symbol();

    DefExpr* nextDef = toDefExpr(parentCall->next);
    INT_ASSERT(nextDef);

    outerCtxHandle = nextDef->sym;
    if (call->numActuals() == 1) {
      INT_ASSERT(outerCtxHandle->getValType() ==
                 this->loopCtx_.localHandle_->getValType());
    }
    else {
      INT_ASSERT(call->numActuals() == 2);
      INT_ASSERT(outerCtxHandle->getValType() == call->get(1)->getValType());
    }

    CallExpr* callAfterDef = toCallExpr(nextDef->next);
    INT_ASSERT(callAfterDef);
    INT_ASSERT(callAfterDef->isNamed(astrInitEquals) ||
               callAfterDef->isPrimitive(PRIM_MOVE));
    INT_ASSERT(toSymExpr(callAfterDef->get(1))->symbol() == outerCtxHandle);
    INT_ASSERT(toSymExpr(callAfterDef->get(2))->symbol() == lhs);

    CONTEXT_DEBUG(debugDepth, "found an outer context handle", outerCtxHandle);
    INT_ASSERT(handleMap_.count(outerCtxHandle) == 0);

    int outerCtxIdx = -1;
    Symbol* argToCall = toSymExpr(call->argList.last())->symbol();

    if (argToCall == this->loopHandle()) {
      // immediate outer context
      outerCtxIdx = 0;
    }
    else {
      // farther away context
      int j=0;
      for (auto i = contextStack_.begin(); i != contextStack_.end(); i++, j++) {
        if (this->handleMap_[argToCall] == j) {
          outerCtxIdx = j+1;
          break;
        }
      }
    }

    if (outerCtxIdx >= (int)contextStack_.size()) {
      USR_FATAL(call, "could not find the %d-th outer context"
                " in the iterator for the enclosing loop", outerCtxIdx+1);
    }

    handleMap_[outerCtxHandle] = outerCtxIdx;

    CONTEXT_DEBUG(debugDepth+1, "mapped to", outerCtxHandle,
                  contextStack_[outerCtxIdx]->localHandle_);

    // TODO refactor this into a common helper
    SET_LINENO(nextDef);
    int curAdjustmentIdx = outerCtxIdx;
    Symbol* handle = contextStack_[curAdjustmentIdx]->localHandle_;

    while (auto newHandle = contextStack_[curAdjustmentIdx]->insertActualForOuterSymbol(handle)) {
      curAdjustmentIdx--;
      handle = newHandle;
    }

    removeOuterContextCallAndInitShadowHandle(call, handle);

    return outerCtxHandle;
  }

  enum class HoistingKind {
    Array, CArray, Barrier, Other
  };

  void handleHoistToCoforallOnContextCall(CallExpr* call,
                                          Symbol* toHoist,
                                          CoforallOnContext* context,
                                          Symbol* usedHandle,
                                          Symbol* targetHandle,
                                          int targetCtxIdx,
                                          HoistingKind kind) {
    const int debugDepth = 3;
    CONTEXT_DEBUG(debugDepth, "will hoist to", call, context->localHandle_);

    if (kind == HoistingKind::CArray) {
      // The early GPU transformations would've added a block here with
      // additional code to support CArray hoisting to GPU. But it's not
      // being hoisted to the GPU, so remove that block.
      auto block = toBlockStmt(call->prev);
      INT_ASSERT(block && "Expected a generated block before the hoist");
      CONTEXT_DEBUG(debugDepth, "removing this block", block);
      block->remove();
    }

    std::vector<CallExpr*>& autoDestroyAnchors =
        context->getLocalHandleAutoDestroys();

    // TODO cache this stuff somewhere, but we remove some below. Is that a problem?
    std::vector<CallExpr*> callsInLoop;
    collectCallExprs(this->loop(), callsInLoop);
    std::map<Symbol*, std::vector<CallExpr*>> autoDestroysInLoop;
    for_vector (CallExpr, call, callsInLoop) {
      if (FnSymbol* fn = call->resolvedFunction()) {
        if (fn->hasFlag(FLAG_AUTO_DESTROY_FN)) {
          Symbol* sym = toSymExpr(call->get(1))->symbol();
          autoDestroysInLoop[sym].push_back(call);
        }
      }
    }

    DefExpr* def = toHoist->defPoint;
    Expr* defPrev = def->prev;

    // this is only needed for `if hoist then __primitive("hoist"....)`
    // when that if param-folds, we leave the block in the AST. This is probably
    // irrelevant when this feature is production-ready
    if (BlockStmt* parentBlock = toBlockStmt(call->parentExpr)) {
      if (parentBlock->length() == 1) {
        parentBlock->flattenAndRemove();
      }
      else if (parentBlock->prev == def) {
        parentBlock->flattenAndRemove();
      }
    }

    SET_LINENO(def);

    // if we are using local shadows of target contexts' handle, we'll need to
    // update the symbol to use what's local in the target
    SymbolMap handleUpdateMap;
    handleUpdateMap.put(usedHandle, targetHandle);

    CallExpr* mulCall = NULL; // only meaningful if isBarrier
    Expr* cur = call->prev;
    FnSymbol* parentFn = toFnSymbol(call->parentSymbol);
    INT_ASSERT(parentFn);

    std::map<ArgSymbol*, int> outerActuals;

    while (cur != defPrev) {
      CONTEXT_DEBUG(debugDepth+1, "hoisting", cur);

      // if we cross any defExpr, hoist/remove its autoDestroys
      if (DefExpr* defExpr = toDefExpr(cur)) {
        Symbol* sym = defExpr->sym;
        std::vector<CallExpr*>& symsAutoDestroys = autoDestroysInLoop[sym];

        if (symsAutoDestroys.size() > 0) {
          // put auto destroys in all the right places
          for_vector (CallExpr, anchor, autoDestroyAnchors) {
            CONTEXT_DEBUG(debugDepth+2, "inserting new autoDestroy before",
                          anchor);

            anchor->insertBefore(symsAutoDestroys[0]->copy());
          }
          // remove all the existing ones
          for_vector (CallExpr, autoDestroy, symsAutoDestroys) {
            CONTEXT_DEBUG(debugDepth+2, "removing autoDestroy", autoDestroy);

            autoDestroy->remove();
          }
        }
      }

      // replace shadow handles with actual local handles
      update_symbols(cur, &handleUpdateMap);

      // keep track of indices of ArgSymbols here that we are hoisting. They'll
      // need to be updated in that given context. Or we need to error if the
      // user hoisted things way too far.
      std::vector<SymExpr*> symExprsInCur; // why can't I just collect symbols?
      collectSymExprs(cur, symExprsInCur);
      for_vector (SymExpr, symExpr, symExprsInCur) {
        Symbol* sym = symExpr->symbol();

        if (ArgSymbol* argSym = toArgSymbol(sym)) {
          if (outerActuals.count(argSym) == 1) continue;

          int formalIdx = findFormalIndex(parentFn, argSym);
          INT_ASSERT(formalIdx >= 0);
          CONTEXT_DEBUG(debugDepth+1,
                        "found a formal at idx " + std::to_string(formalIdx),
                        argSym);
          outerActuals[argSym] = formalIdx;
        }
      }

      // if we are hoisting a barrier, try to find its multiply call
      if (kind == HoistingKind::Barrier) {
        if (CallExpr* call = findMultiplyCallForBarrier(cur, toHoist)) {
          CONTEXT_DEBUG(debugDepth+1, "found multiply call", call);
          if (mulCall != NULL) {
            CONTEXT_DEBUG(debugDepth+2, "WARNING: there was another one", mulCall);
          }
          mulCall = call;
        }
      }

      targetHandle->defPoint->insertAfter(cur->remove());

      cur = call->prev;
    }

    // we have some symbols declared elsewhere in the block that we are hoisting
    // walk up the context stack to the target to find where they are
    SymbolMap newContextUpdateMap;
    for (auto& outerActualToIdx : outerActuals) {
      int actualIdx = outerActualToIdx.second;
      Symbol* curActual = NULL;

      for (int i = 0; i < targetCtxIdx ; i++) {
        if (actualIdx == -1) {
          // we are currently looking for a symbol that was defined in an inner
          // context. IOW, the user wants to hoist a block that has a symbol,
          // to a block where the symbol wasn't defined yet. This is a user
          // error.
          USR_FATAL(call, "Attempt to hoist symbols from an inner context to an outer context");
        }

        contextStack_[i]->recomputeActualSymbol(actualIdx, curActual);
      }

      newContextUpdateMap.put(outerActualToIdx.first, curActual);
    }

    const char* hoistedName = astr("hoisted_", toHoist->name);
    Symbol* refToSym = new VarSymbol(hoistedName, toHoist->getRefType());
    DefExpr* refToSymDef = new DefExpr(refToSym);
    CallExpr* setRef = new CallExpr(PRIM_MOVE, refToSym, new CallExpr(PRIM_ADDR_OF, toHoist));

    context->getInsertBeforeCallToInnerAnchor()->insertBefore(refToSymDef);
    context->getInsertBeforeCallToInnerAnchor()->insertBefore(setRef);

    for (int curCtx = targetCtxIdx ; curCtx >= 0 ; curCtx--) {
      auto& ctx = contextStack_[curCtx];

      auto innerSym = ctx->insertActualForHoistedSymbol(refToSym, hoistedName,
                                                        toHoist->getRefType());

      if (kind == HoistingKind::Barrier) {
        if (Expr* upEndCount = ctx->getUpEndCount()) {
          CONTEXT_DEBUG(debugDepth+1, "multiply block will be inserted after here", upEndCount);
          Symbol* numTasks = toSymExpr(toCallExpr(upEndCount)->get(2))->symbol();

          INT_ASSERT(mulCall);
          CallExpr* newMulCall = mulCall->copy();
          newMulCall->get(1)->replace(new SymExpr(refToSym));
          newMulCall->get(2)->replace(new SymExpr(numTasks));

          ctx->getInsertBeforeCallToInnerAnchor()->insertBefore(newMulCall);
        }
      }

      refToSym = innerSym;
    }

    removeHoistToContextCall(call);
    if (mulCall) {
      mulCall->remove();
    }

    // we want to use the last added symbol after the loop to adjust the loop body
    SymbolMap updateMap;
    updateMap.put(toHoist, refToSym);

    update_symbols(loop(), &updateMap);
    update_symbols(context->node(), &newContextUpdateMap);
  }

  void handleHoistToVectorContextCall(CallExpr* call,
                                      Symbol* toHoist,
                                      VectorizedLoopContext* context,
                                      HoistingKind kind) {
     if (kind == HoistingKind::CArray) {
      // The ref-ified version of the array will not need autodestroys;
      // get rid of them.
      std::vector<CallExpr*> callsInLoop;
      collectCallExprs(this->loop(), callsInLoop);
      for_vector (CallExpr, call, callsInLoop) {
        if (FnSymbol* fn = call->resolvedFunction()) {
          if (fn->hasFlag(FLAG_AUTO_DESTROY_FN) &&
              toSymExpr(call->get(1))->symbol() == toHoist) {
            call->remove();
          }
        }
      }
      // Remove the initializer.
      call->prev->prev->remove();
      // Unpack the pre-generated initialization code for the shared array
      auto replacementBlock = toBlockStmt(call->prev);
      auto replacementDefExpr = toDefExpr(replacementBlock->getFirstExpr());
      replacementBlock->flattenAndRemove();
      // Replace the old, non-shared array with the new shared array.
      SymbolMap newArrayUpdateMap;
      newArrayUpdateMap.put(toHoist, replacementDefExpr->sym);
      toHoist->defPoint->remove();
      update_symbols(loop(), &newArrayUpdateMap);
      // Remove the call-to-context call.
      call->remove();
    } else {
      INT_FATAL("currently only array hoisting to vector contexts is supported");
    }
  }

  void handleHoistToContextCall(CallExpr* call) {
    const int debugDepth = 3;

    Symbol* handle = toSymExpr(call->get(1))->symbol();
    Symbol* sym = toSymExpr(call->get(2))->symbol();
    int targetCtxIdx = handleMap_[handle];
    auto& target = contextStack_[targetCtxIdx];
    Symbol* targetHandle = target->localHandle_;

    bool isBarrier = astr(sym->type->symbol->name) == astr("barrier");
    bool isArray = strncmp(sym->type->symbol->name, "_array(", sizeof("_array(") - 1) == 0;
    bool isCArray = strncmp(sym->type->symbol->name, "c_array(", sizeof("c_array(") - 1) == 0;
    HoistingKind kind;
    if (isBarrier) {
      kind = HoistingKind::Barrier;
      CONTEXT_DEBUG(debugDepth, "this is a barrier", sym);
    } else if (isArray) {
      kind = HoistingKind::Array;
      CONTEXT_DEBUG(debugDepth, "this is an array", sym);
    } else if (isCArray) {
      kind = HoistingKind::CArray;
      CONTEXT_DEBUG(debugDepth, "this is a C array", sym);
    } else {
      kind = HoistingKind::Other;
      CONTEXT_DEBUG(debugDepth, "this is something else", sym);
    }

    if (auto coforallOnCtx = target->toCoforallOnContext()) {
      handleHoistToCoforallOnContextCall(call, sym, coforallOnCtx, handle,
                                         targetHandle, targetCtxIdx, kind);
    } else if (auto vectorizedLoopCtx = target->toVectorizedLoopContext()) {
      handleHoistToVectorContextCall(call, sym, vectorizedLoopCtx, kind);
    }
  }

  CallExpr* findMultiplyCallForBarrier(Expr* e, Symbol* barrierSym) {
    if (BlockStmt* block = toBlockStmt(e)) {
      for_alist(expr, block->body) {
        if (CallExpr* found = findMultiplyCallForBarrier(expr, barrierSym)) {
          return found;
        }
      }
    }
    else if (CallExpr* call = toCallExpr(e)) {
      if (call->isNamed("multiply")) {
        Symbol* callReceiver = toSymExpr(call->get(1))->symbol();
        if (callReceiver == barrierSym) {
          return call;
        }
      }
    }
    return NULL;
  }

  void handleContextUsesWithinLoopBody() {
    std::vector<Symbol*> handles;
    handles.push_back(this->loopHandle());

    std::vector<Symbol*>::size_type curIdx = 0;
    while (curIdx < handles.size()) {
      const int debugDepth = 1;
      std::vector<SymExpr*> handleUses;
      collectSymExprsFor(this->loop(), handles[curIdx], handleUses);

      for_vector (SymExpr, use, handleUses) {
        if (CallExpr* call = toCallExpr(use->parentExpr)) {
          CONTEXT_DEBUG(debugDepth+1, "found a call that uses handle", call);

          if (call->isPrimitive(PRIM_OUTER_CONTEXT)) {
            CONTEXT_DEBUG(debugDepth+2, "PRIM_OUTER_CONTEXT", call);
            if (Symbol* newCtx = handleOuterContextCall(call)) {
              handles.push_back(newCtx);
            }
          }
          else if (call->isPrimitive(PRIM_HOIST_TO_CONTEXT)) {
            CONTEXT_DEBUG(debugDepth+2, "PRIM_HOIST_TO_CONTEXT", call);

            handleHoistToContextCall(call);
          }
        }
        else {
          CONTEXT_DEBUG(debugDepth, "illegal use of context handle", use);
          USR_FATAL(use, "illegal use of context handle %s",
                    loopHandle()->name);
        }
      }

      curIdx++;
    }
  }
}; // class ContextHandler

void lowerContexts() {
  if (!fIteratorContexts) return;

  forv_Vec(FnSymbol*, fn, gFnSymbols) {
    if (!isInUserCode(fn)) continue;

    // TODO do something lighter
    std::vector<BaseAST*> asts;
    collect_asts(fn, asts);

    for_vector(BaseAST, ast, asts) {
      if (CForLoop* loop = toCForLoop(ast)) {
        CONTEXT_DEBUG(0, "encountered a C loop", loop);
        if (loop->id == breakOnResolveID) gdbShouldBreakHere();

        ContextHandler h(loop);
        h.collectHandleAndOuterContexts();
        h.handleContextUsesWithinLoopBody();
      }
    }
  }
}
