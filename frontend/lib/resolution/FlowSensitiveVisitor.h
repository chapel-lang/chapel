/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_LIB_RESOLUTION_FLOWSENSITIVEVISITOR_H
#define CHPL_LIB_RESOLUTION_FLOWSENSITIVEVISITOR_H

#include "chpl/uast/all-uast.h"
#include "chpl/resolution/scope-queries.h"

namespace chpl {
namespace resolution {

struct ControlFlowInfo {
 private:
  // has the block already encountered a return or a throw?
  bool returnsOrThrows_ = false;

  // whether this frame hit a 'break'
  bool breaks_ = false;

  // whether this frame hit a 'continue'
  bool continues_ = false;

 public:
  ControlFlowInfo() = default;
  ControlFlowInfo(bool returnsOrThrows, bool breaks, bool continues)
    : returnsOrThrows_(returnsOrThrows), breaks_(breaks), continues_(continues) {}

  bool isDoneExecuting() const { return returnsOrThrows_ || breaks_ || continues_; }
  bool returnsOrThrows() const { return returnsOrThrows_; }
  bool breaks() const { return breaks_; }
  bool continues() const { return continues_; }

  void markReturnOrThrow() { if (!isDoneExecuting()) returnsOrThrows_ = true; }
  void markBreak() { if (!isDoneExecuting()) breaks_ = true; }
  void markContinue() { if (!isDoneExecuting()) continues_ = true; }

  void resetContinue() { continues_ = false; }
  void resetBreak() { breaks_ = false; }

  ControlFlowInfo& operator&=(const ControlFlowInfo& other) {
    returnsOrThrows_ &= other.returnsOrThrows_;
    breaks_ &= other.breaks_;
    continues_ &= other.continues_;
    return *this;
  }

  ControlFlowInfo operator&(const ControlFlowInfo& other) {
    auto ret = *this;
    return ret &= other;
  }

  void sequence(const ControlFlowInfo& other) {
    if (!isDoneExecuting()) {
      returnsOrThrows_ |= other.returnsOrThrows_;
      breaks_ |= other.breaks_;
      continues_ |= other.continues_;
    }
  }
};

template <typename Self>
struct BaseFrame {
  // the ast node corresponding to this frame
  const uast::AstNode* scopeAst = nullptr;

  // the scope of this frame
  const Scope* scope = nullptr;

  // control flow information
  ControlFlowInfo controlFlowInfo;

  // for conditionals/selects, does this block have a param true condition?
  bool paramTrueCond = false;

  // for conditionals/selects, is this known to be the only path?
  bool knownPath = false;

  // When processing a conditional or catch blocks,
  // instead of popping the SplitInitFrame for the then/else/catch blocks,
  // store them here, for use in flow-sensitive operations.
  std::vector<std::tuple<const uast::AstNode*, owned<Self>>> subFrames;

  BaseFrame() = default;
  BaseFrame(const uast::AstNode* ast) : scopeAst(ast) {}

  /* have we hit a statement that would prevent further execution of this block? */
  bool isDoneExecuting() {
    return controlFlowInfo.isDoneExecuting();
  }
};

struct DefaultFrame : public BaseFrame<DefaultFrame> {
  DefaultFrame() = default;
  DefaultFrame(const uast::AstNode* ast) : BaseFrame(ast) {}
};

template <typename Frame /* : BaseFrame */, typename ExtraData = std::variant<std::monostate>>
struct FlowSensitiveVisitor {
  std::vector<owned<Frame>> scopeStack;

  /** Return the current frame. This should always be safe to call
      from one of the handle* methods. */
  Frame* currentFrame() {
    CHPL_ASSERT(!scopeStack.empty());
    return scopeStack.back().get();
  }

  /** Return the parent frame or nullptr if there is none. */
  Frame* currentParentFrame() {
    Frame* parent = nullptr;
    size_t n = scopeStack.size();
    if (n >= 2) {
      parent = scopeStack[n-2].get();
    }
    return parent;
  }

  Frame* currentParamTrueFrame() {
    Frame* frame = currentFrame();
    for (auto& subFrameInfo : frame->subFrames) {
      auto subFrame = std::get<owned<Frame>>(subFrameInfo).get();
      if (!subFrame || !subFrame->knownPath) continue;
      return subFrame;
    }
    return nullptr;
  }

  /** Assuming that the current frame refers to a Conditional,
      returns the frame for the 'then' block. */
  Frame* currentThenFrame() {
    auto frame = currentFrame();
    CHPL_ASSERT(frame->scopeAst->isConditional());
    CHPL_ASSERT(frame->subFrames.size() == 1 || frame->subFrames.size() == 2);
    auto& ret = std::get<owned<Frame>>(frame->subFrames[0]);
    // ret can be nullptr if the if branch was skipped at resolution time
    // (i.e. if the condition was param false).
    return ret.get();
  }

  /** Assuming that the current frame refers to a Conditional,
      returns the frame for the 'else' block, or 'nullptr' if there was none. */
  Frame* currentElseFrame() {
    auto frame = currentFrame();
    CHPL_ASSERT(frame->scopeAst->isConditional());
    CHPL_ASSERT(frame->subFrames.size() == 1 || frame->subFrames.size() == 2);
    if (frame->subFrames.size() == 1) {
      // there was no 'else' clause
      return nullptr;
    } else {
      return std::get<owned<Frame>>(frame->subFrames[1]).get();
    }
  }

  /** Assuming that the current frame refers to a Try,
      returns the number of frames saved for Catch clauses.  */
  int currentNumCatchFrames() {
    auto frame = currentFrame();
    CHPL_ASSERT(frame->scopeAst->isTry());
    int ret = frame->subFrames.size();
    CHPL_ASSERT(frame->scopeAst->toTry()->numHandlers() == ret);
    return ret;
  }

  /** Assuming that the current frame refers to a Try,
      returns the i'th saved Catch frame. */
  Frame* currentCatchFrame(int i) {
    auto frame = currentFrame();
    CHPL_ASSERT(frame->scopeAst->isTry());
    CHPL_ASSERT(0 <= i && (size_t) i < frame->subFrames.size());
    auto& ret = std::get<owned<Frame>>(frame->subFrames[i]);
    CHPL_ASSERT(ret.get());
    return ret.get();
  }

  /** Assuming that the current frame refers to a Select,
      returns the number of frames saved for When clauses.  */
  int currentNumWhenFrames() {
    auto frame = currentFrame();
    CHPL_ASSERT(frame->scopeAst->isSelect());
    int ret = frame->subFrames.size();
    //allowance for otherwise placeholder
    CHPL_ASSERT(frame->scopeAst->toSelect()->numWhenStmts() == ret);
    return ret;
  }

  /** Assuming that the current frame refers to a Select,
      returns the i'th saved When frame.  */
  Frame* currentWhenFrame(int i) {
    auto frame = currentFrame();
    CHPL_ASSERT(frame->scopeAst->isSelect());
    CHPL_ASSERT(0 <= i && (size_t) i < frame->subFrames.size());
    auto& ret = std::get<owned<Frame>>(frame->subFrames[i]);
    return ret.get();
  }

  void createSubFrames(const uast::AstNode* ast) {
    auto& newFrame = scopeStack.back();
    if (auto c = ast->toConditional()) {
      // note thenBlock / elseBlock
      newFrame->subFrames.emplace_back(c->thenBlock(), nullptr);
      newFrame->subFrames.emplace_back(c->elseBlock(), nullptr);
    } else if (auto t = ast->toTry()) {
      // note each of the catch handlers (aka catch clauses)
      for (auto clause : t->handlers()) {
        newFrame->subFrames.emplace_back(clause, nullptr);
      }
    } else if (auto s = ast->toSelect()) {
      for (auto when : s->whenStmts()) {
        newFrame->subFrames.emplace_back(when, nullptr);
      }
    }
  }

  virtual void doEnterScope(const uast::AstNode* ast, ExtraData extraData) {
    createSubFrames(ast);
  }

  bool enterScope(const uast::AstNode* ast, ExtraData extraData) {
    if (resolution::createsScope(ast->tag())) {
      scopeStack.emplace_back(new Frame(ast));
      doEnterScope(ast, extraData);
      return true;
    }
    return false;
  }

  bool saveSubFrame(const uast::AstNode* ast, Frame*& outParentFrame) {
    CHPL_ASSERT(!scopeStack.empty());
    size_t n = scopeStack.size();
    auto& curFrame = scopeStack[n-1];
    CHPL_ASSERT(curFrame->scopeAst == ast);
    if (n < 2) return false;

    // Are we in the situation of just finishing a Block
    // that is within a Conditional? or a Catch clause?
    // If so, save the frame in the subBlocks, to
    // be handled when processing condFrame.
    outParentFrame = scopeStack[n-2].get();
    for (auto & subBlock : outParentFrame->subFrames) {
      if (std::get<const uast::AstNode*>(subBlock) == ast) {
        std::get<owned<Frame>>(subBlock).swap(curFrame);
        return true;
      }
    }

    return false;
  }

  void reconcileFrames(Frame* parentFrame, const uast::Conditional* cond) {
    auto thenFrame = currentThenFrame();
    auto elseFrame = currentElseFrame();
    if (thenFrame && elseFrame) {
      parentFrame->controlFlowInfo.sequence(thenFrame->controlFlowInfo & elseFrame->controlFlowInfo);
    }
    if (thenFrame && thenFrame->knownPath) {
      parentFrame->controlFlowInfo.sequence(thenFrame->controlFlowInfo);
    }
    if (elseFrame && elseFrame->knownPath) {
      parentFrame->controlFlowInfo.sequence(elseFrame->controlFlowInfo);
    }
  }

  void sequenceWithParentFrame(Frame* parentFrame, ControlFlowInfo append) {
    // 'break' and 'continue' are scoped to the loop that they're in.
    // a loop that has 'continue' in its body does not itself continue
    // its parent loop.
    if (parentFrame->scopeAst->isLoop()) {
      append.resetBreak();
      append.resetContinue();
    }

    parentFrame->controlFlowInfo.sequence(append);
  }

  void reconcileFrames(Frame* parentFrame, const uast::Try* t) {
    auto tryFrame = currentFrame();
    if (tryFrame->controlFlowInfo.isDoneExecuting()) {
      int nCatchFrames = currentNumCatchFrames();
      auto accumulatedControlFlowInfo = tryFrame->controlFlowInfo;
      for (int i = 0; i < nCatchFrames; i++) {
        auto catchFrame = currentCatchFrame(i);
        accumulatedControlFlowInfo &= catchFrame->controlFlowInfo;
      }
      sequenceWithParentFrame(parentFrame, accumulatedControlFlowInfo);
    }
  }

  void reconcileFrames(Frame* parentFrame, const uast::Select* s) {
    ControlFlowInfo accumulatedControlFlowInfo(true, true, true);
    for(int i = 0; i < s->numWhenStmts(); i++) {
      auto whenFrame = currentWhenFrame(i);
      if (!whenFrame) continue;
      accumulatedControlFlowInfo &= whenFrame->controlFlowInfo;
      if (whenFrame->knownPath) {  // this is known to be the taken path
        sequenceWithParentFrame(parentFrame, whenFrame->controlFlowInfo);
        break;
      }
    }

    if (s->hasOtherwise()) {
      sequenceWithParentFrame(parentFrame, accumulatedControlFlowInfo);
    }
  }

  void reconcileFrames(Frame* parentFrame, const uast::AstNode* ast) {
    if (ast->isLoop()) {
      // don't propagate return/throw etc. out of a loop, because we can't
      // reason about its dynamic behavior. The exception are 'Param' loops.
      return;
    }
    sequenceWithParentFrame(parentFrame, currentFrame()->controlFlowInfo);
  }

  virtual void doExitScope(const uast::AstNode* ast, ExtraData extraData) {
    Frame* parentFrame = nullptr;

    if (saveSubFrame(ast, parentFrame)) {
      // frame will be processed with parent block
      CHPL_ASSERT(parentFrame->scopeAst->isConditional() ||
          parentFrame->scopeAst->isTry() ||
          parentFrame->scopeAst->isSelect());
      return;
    }

    if (parentFrame == nullptr) return;

    switch (ast->tag()) {
      case uast::asttags::AstTag::Conditional:
        reconcileFrames(parentFrame, ast->toConditional()); break;
      case uast::asttags::AstTag::Try:
        reconcileFrames(parentFrame, ast->toTry()); break;
      case uast::asttags::AstTag::Select:
        reconcileFrames(parentFrame, ast->toSelect()); break;
      default:
        reconcileFrames(parentFrame, ast);
    }
  }

  bool exitScope(const uast::AstNode* ast, ExtraData extraData) {
    if (resolution::createsScope(ast->tag())) {
      doExitScope(ast, extraData);
      scopeStack.pop_back();
      return true;
    }
    return false;
  }

  virtual const types::Param* determineWhenCaseValue(const uast::AstNode* ast, ExtraData extraData) = 0;
  virtual const types::Param* determineIfValue(const uast::AstNode* ast, ExtraData extraData) = 0;

  bool isParamTrue(const types::Param* param) { return param && param->isNonZero(); }
  bool isParamFalse(const types::Param* param) { return param && param->isZero(); }

  virtual void traverseNode(const uast::AstNode* ast, ExtraData extraData) = 0;

  bool flowSensitivelyTraverse(const uast::Select* sel, ExtraData extraData) {
    // have we encountered a when without param-decided conditions?
    bool anyWhenNonParam = false;

    for(int i = 0; i < sel->numWhenStmts(); i++) {
      auto whenAst = sel->whenStmt(i);

      bool anyCaseParamTrue = false;
      bool allCaseParamFalse = !whenAst->isOtherwise();
      for(auto caseExpr : whenAst->caseExprs()) {
        auto param = determineWhenCaseValue(caseExpr, extraData);
        anyCaseParamTrue |= isParamTrue(param);
        allCaseParamFalse &= isParamFalse(param);
      }

      anyWhenNonParam |= !anyCaseParamTrue && !allCaseParamFalse;

      if (!allCaseParamFalse) {
        // only traverse whens that are not param false
        traverseNode(whenAst, extraData);
        // if there is a param true case and none of the preceding whens might
        // be taken at runtime, then this is the only path we need to consider
        currentWhenFrame(i)->knownPath = anyCaseParamTrue && !anyWhenNonParam;
        currentWhenFrame(i)->paramTrueCond = anyCaseParamTrue;
      }

      if (whenAst->isOtherwise()) {
        // if we've reached this point, none of the preceding whens have a param
        // true condition, so the otherwise is paramTrue if all preceding whens
        // are param false.
        currentWhenFrame(i)->knownPath = !anyWhenNonParam;
      } else if (anyCaseParamTrue) {
        break;
      }
    }
    return false;
  }

  bool flowSensitivelyTraverse(const uast::Conditional* cond, ExtraData extraData) {
    auto param = determineIfValue(cond->condition(), extraData);
    if (isParamTrue(param)) {
      // Don't need to process the false branch.
      traverseNode(cond->thenBlock(), extraData);
      currentThenFrame()->paramTrueCond = true;
      currentThenFrame()->knownPath = true;
      return false;
    } else if (isParamFalse(param)) {
      if (auto elseBlock = cond->elseBlock()) {
        traverseNode(elseBlock, extraData);
        currentElseFrame()->paramTrueCond = true;
        currentElseFrame()->knownPath = true;
      }
      return false;
    }
    // Not param-known condition; visit both branches as normal.
    return true;
  }
};

}  // namespace resolution
}  // namespace chpl

#endif
