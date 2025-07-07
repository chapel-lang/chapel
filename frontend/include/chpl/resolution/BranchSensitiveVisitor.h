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

#ifndef CHPL_LIB_RESOLUTION_BRANCHSENSITIVEVISITOR_H
#define CHPL_LIB_RESOLUTION_BRANCHSENSITIVEVISITOR_H

#include "chpl/uast/all-uast.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/resolution/resolution-types.h"

namespace chpl {
namespace resolution {

/**
  This struct contains information about a program's control flow state.
  Specifically, it tracks whether the program has returned, thrown, invoked 'break',
  or invoked 'continue'. This is used in various program analyses to avoid
  reasoning about code that's unreachable. It is also used in some analyses
  for other reasons (e.g. if a frame doesn't initialze a split-init'ed variable,
  but throws, that variable is still eligible for split-init).
 */
struct ControlFlowInfo {
 private:
  // has the block already encountered a return or a throw?
  bool returns_ = false;
  bool throws_ = false;

  // if this frame hit a 'break', the loop it broke out of
  const uast::Loop* loopBreaks_ = nullptr;

  // if this frame hit a 'continue', the loop it continued
  const uast::Loop* loopContinues_ = nullptr;

  // did we encounter a 'compilerError'? These terminate resolution.
  bool fatalError_ = false;

 public:
  ControlFlowInfo() = default;

  /* have we hit a statement that would prevent further execution of a block? */
  bool isDoneExecuting() const { return returns_ || loopBreaks_ || loopContinues_ || fatalError_; }
  bool returnsOrThrows() const { return returns_ || throws_; }
  bool throws() const { return throws_; }
  bool breaks() const { return loopBreaks_ != nullptr; }
  bool continues() const { return loopContinues_ != nullptr; }
  bool fatalError() const { return fatalError_; }

  // The below mark functions check if the block is already done executing.
  // This is useful because in cases like 'return; continue', we don't want
  // to treat the code as if it continues. If we already stopped executing,
  // further changes to control flow are not recorded.

  void markReturn() { if (!isDoneExecuting()) returns_ = true; }
  void markThrow() { if (!isDoneExecuting()) throws_ = true; }
  void markBreak(const uast::Loop* markWith) { if (!isDoneExecuting()) loopBreaks_ = markWith; }
  void markContinue(const uast::Loop* markWith) { if (!isDoneExecuting()) loopContinues_ = markWith; }
  void markFatalError() { if (!isDoneExecuting()) fatalError_ = true; }

  // resetting break and continue is useful for when we are handling 'param' for loops.
  // If we hit a continue, we might want to move on to the next iteration of
  // resolving the body, but if we hit a return, we should stop.

  void resetBreak(const uast::Loop* clearing) {  if (loopBreaks_ == clearing) loopBreaks_ = nullptr; }
  void resetContinue(const uast::Loop* clearing) {  if (loopContinues_ == clearing) loopContinues_ = nullptr; }

  /* mutably combine control flow information from two branches (e.g., then/else) */
  ControlFlowInfo& operator&=(const ControlFlowInfo& other) {
    returns_ &= other.returns_;
    throws_ &= other.throws_;

    if (other.loopBreaks_ == loopBreaks_) {
      // both break out of the same loop (or neither breaks), do nothing
    } else {
      // incompatible breaks, can't tell what will happen for sure
      loopBreaks_ = nullptr;
    }

    if (other.loopContinues_ == loopContinues_) {
      // both continue the same loop (or neither continue), do nothing
    } else {
      // incompatible continues, can't tell what will happen for sure
      loopContinues_ = nullptr;
    }

    // errors in one branch are sufficient to infect all branches
    fatalError_ |= other.fatalError_;

    return *this;
  }


  /* immutably combine control flow information from two branches (e.g., then/else) */
  ControlFlowInfo operator&(const ControlFlowInfo& other) {
    auto ret = *this;
    return ret &= other;
  }

  /* supposing 'this' represents a block's current control flow state,
     update it with changes from 'other' that represent a new statement having
     been executed. */
  void sequence(const ControlFlowInfo& other) {
    if (!isDoneExecuting()) {
      returns_ |= other.returns_;
      throws_ |= other.throws_;
      loopBreaks_ = other.loopBreaks_;
      loopContinues_ = other.loopContinues_;
      fatalError_ |= other.fatalError_;
    }
  }

  /* same as 'sequence', except treats the 'other' as an iteration of a loop
     body rather than a statement in the current block. For this reason,
     does not propagate 'continue' and 'break' for the given loop, since a
     loop whose body continues does not continue its parent loop. */
  void sequenceIteration(const ControlFlowInfo& other, const uast::Loop* loop) {
    if (!isDoneExecuting()) {
      returns_ |= other.returns_;
      throws_ |= other.throws_;
      if (other.loopBreaks_ != loop) loopBreaks_ = other.loopBreaks_;
      if (other.loopContinues_ != loop) loopContinues_ = other.loopContinues_;
      fatalError_ |= other.fatalError_;
    }
  }
};

/*
  This is a base class for frames in the BranchSensitiveVisitor. It uses
  CRTP, so the Self template parameter should be the class inheriting from
  BaseFrame.

  At base, this struct stores information about control flow (see
  comment on ControlFlowInfo for when this is useful). It also stores the current
  scope and AST node. Finally, it stores information about 'param'-known
  paths; if a 'then' branch of a loop is known to be taken, we will incorporate
  that information into the analysis and not traverse the 'else' branch.

  The 'paramTrueCond' and 'knownPath' are subtly different. A frame
  can have a true condition without being known to execute at compile time.
  This can happen (e.g.) if a preceding 'when' clause has a non-'param'
  condition, or if the preceding clause has a 'param true' condition and
  thus is evaluated instead.

  Visitors deriving from BranchSensitiveVisitor can extend this frame
  class with additional information. For example, VarScopeVisitor
  stores split-init, copy elision, etc. information in its frames. It is not
  included here to avoid adding unnecessary overhead to other visitors.
*/

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

/* Since you can't instantiate a template like BaseFrame with itself
   (you'd need BaseFrame<BaseFrame<...>>), this class exists to provide
   only the fields from BaseFrame to visitors that need nothing else.
 */
struct DefaultFrame : public BaseFrame<DefaultFrame> {
  DefaultFrame() = default;
  DefaultFrame(const uast::AstNode* ast) : BaseFrame(ast) {}
};

/**

  This struct is intended as a base class for visitors that need to be
  sensitive to branching and control flow. For example, Resolver relies
  on this to avoid evaluating code that exists in dead branches.
  The VarScopeVisitor and its family of child classes do the same, _and_
  use control flow information for other parts of their analyses -- see
  the comment on ControlFlowInfo above.

  The visitor is parameterized by a Frame type, which should be a subclass
  of BaseFrame. See the comment on BaseFrame for more information about
  what it does.

  Normally, this visitor (via its 'enterScope' and 'exitScope' methods)
  pushes Frames onto its stack when entering an AST node with a scope,
  and pops them when exiting. For certain types of control flow, however,
  rather than popping the frame, it saves it in the parent frame's
  'subFrames' field. This is done to enable processing multi-branch statements
  that have nontrivial behaviors (e.g., 'select' picks the first branch
  that matches, and if that condition is 'param', other branches are considered
  dead; this affects what the whole 'select' statement does w.r.t. control flow
  and other analyses). The 'doEnterScope' and 'doExitScope' methods are
  intended to be overridden by visitors that need to do something when
  entering or exiting a scope. They are executed after the frame is pushed
  and before it is popped, so that the visitor can access the current frame
  when processing the scope.

  This visitor additionally consolidates logic for deciding which branch
  of a conditional or select to traverse. To be most general, this is done
  via the 'determineWhenCaseValue' and 'determineIfValue' methods. For
  Resolver, which actually does the work of evaluating expressions, these
  are intended to perform that work. For other visitors which operates on
  results of the resolution process, these methods are intended to simply
  access that (pre-computed) information. Finally, 'traverseNode' is used
  when "comitting" to a 'param'-known path.

  To call into the conditional/select logic, use 'branchSensitivelyTraverse'.

  The ExtraData template parameter is used for visitors whose recursive
  traversals (via 'traverse') require that additional data. This is true
  concretely for ResolvedVisitor-based analyses, which have a mutually recursive
  'traverse' function.
*/
template <typename Frame /* : BaseFrame */, typename ExtraData = std::variant<std::monostate>>
struct BranchSensitiveVisitor {
 public:
  std::vector<owned<Frame>> scopeStack;

 private:
  /** when entering a scope, set up slots for subFrames to be stored into,
      if applicable for the current AST node type. */
  void createSubFrames(const uast::AstNode* ast) {
    auto& newFrame = scopeStack.back();
    if (auto c = ast->toConditional()) {
      // note thenBlock / elseBlock
      newFrame->subFrames.emplace_back(c->thenBlock(), nullptr);
      newFrame->subFrames.emplace_back(c->elseBlock(), nullptr);
    } else if (auto t = ast->toTry()) {
      // note the body itself to avoid clobbering the whole Try node with
      // control flow information
      newFrame->subFrames.emplace_back(t->body(), nullptr);
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

  /** when exiting frame, detect if we need to store it as a subFrame.
      We do so if an existing subFrame slot created by createSubFrames
      matches the current AST node.

      Regardless, set outParentFrame to the parent frame, if any.
   */
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

  /** Incorporate the control flow information from a child statement into
      the parent frame's control flow information. */
  void sequenceWithParentFrame(Frame* parentFrame, const ControlFlowInfo& append) {
    // 'break' and 'continue' are scoped to the loop that they're in.
    // a loop that has 'continue' in its body does not itself continue
    // its parent loop.
    if (auto loop = parentFrame->scopeAst->toLoop()) {
      parentFrame->controlFlowInfo.sequenceIteration(append, loop);
    } else {
      parentFrame->controlFlowInfo.sequence(append);
    }
  }

  /** When exiting an AST node, incorporate the information from its
      frame (assumed at the top of the stack) and sub-frames into the
      given parent frame. */
  void reconcileFrames(Frame* parentFrame, const uast::AstNode* ast) {
    if (ast->isLoop()) {
      // don't propagate return/throw etc. out of a loop, because we can't
      // reason about its dynamic behavior. The exception are 'Param' loops.
      return;
    } else if (ast->isFunction()) {
      return;
    }
    sequenceWithParentFrame(parentFrame, currentFrame()->controlFlowInfo);
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
  void reconcileFrames(Frame* parentFrame, const uast::Try* t) {
    auto mainFrame = currentTryBodyFrame();
    if (mainFrame->controlFlowInfo.isDoneExecuting()) {
      int nCatchFrames = currentNumCatchFrames();
      auto accumulatedControlFlowInfo = mainFrame->controlFlowInfo;
      for (int i = 0; i < nCatchFrames; i++) {
        auto catchFrame = currentCatchFrame(i);
        accumulatedControlFlowInfo &= catchFrame->controlFlowInfo;
      }
      sequenceWithParentFrame(parentFrame, accumulatedControlFlowInfo);
    }
  }
  void reconcileFrames(Frame* parentFrame, const uast::Select* s) {
    bool inited = false;
    ControlFlowInfo accumulatedControlFlowInfo;

    for(int i = 0; i < s->numWhenStmts(); i++) {
      auto whenFrame = currentWhenFrame(i);
      if (!whenFrame) continue;

      if (!inited) {
        accumulatedControlFlowInfo = whenFrame->controlFlowInfo;
        inited = true;
      } else {
        accumulatedControlFlowInfo &= whenFrame->controlFlowInfo;
      }

      if (whenFrame->knownPath) {  // this is known to be the taken path
        sequenceWithParentFrame(parentFrame, whenFrame->controlFlowInfo);
        break;
      }
    }

    if (s->hasOtherwise()) {
      sequenceWithParentFrame(parentFrame, accumulatedControlFlowInfo);
    }
  }

 public:

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

  /** Returns the current subFrame (regardless of what AST node is being
      used) that is known to be the path taken. */
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
    int ret = frame->scopeAst->toTry()->numHandlers();
    CHPL_ASSERT((size_t) (ret + 1) == frame->subFrames.size());
    return ret;
  }

  /** Assuming that the current frame refers to a Try,
      returns the frame for its body. */
  Frame* currentTryBodyFrame() {
    auto frame = currentFrame();
    CHPL_ASSERT(frame->scopeAst->isTry());
    CHPL_ASSERT(frame->subFrames.size() >= 1);

    // for try-catch, body will be a block with its own frame. For try expressions,
    // body will be the expression itself, and we should use the current frame.
    auto ret = std::get<owned<Frame>>(frame->subFrames[0]).get();
    if (!ret) ret = frame;
    return ret;
  }

  /** Assuming that the current frame refers to a Try,
      returns the i'th saved Catch frame. */
  Frame* currentCatchFrame(int i) {
    auto frame = currentFrame();
    CHPL_ASSERT(frame->scopeAst->isTry());
    CHPL_ASSERT(0 <= i && (size_t) (i + 1) < frame->subFrames.size());
    auto& ret = std::get<owned<Frame>>(frame->subFrames[i + 1]);
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

  /** note that the current frame, if any, has returned or thrown */
  void markReturn() {
    if (scopeStack.empty()) return;
    currentFrame()->controlFlowInfo.markReturn();
  }

  void markThrow() {
    if (scopeStack.empty()) return;
    currentFrame()->controlFlowInfo.markThrow();
  }

  /** note that the current frame, if any, has invoked 'break' */
  void markBreak(const uast::Loop* loop) {
    if (scopeStack.empty()) return;
    currentFrame()->controlFlowInfo.markBreak(loop);
  }

  /** note that the current frame, if any, has invoked 'continue' */
  void markContinue(const uast::Loop* loop) {
    if (scopeStack.empty()) return;
    currentFrame()->controlFlowInfo.markContinue(loop);
  }

  /** note that the current frame, if any, has encountered a fatal error */
  void markFatalError() {
    if (scopeStack.empty()) return;
    currentFrame()->controlFlowInfo.markFatalError();
  }

  /** have we hit a statement that would prevent further execution of the current block / frame? */
  bool isDoneExecuting() {
    if (scopeStack.empty()) return false;
    return currentFrame()->isDoneExecuting();
  }

  bool markedThrow() {
    if (scopeStack.empty()) return false;
    return currentFrame()->controlFlowInfo.throws();
  }

  /** code to run right after we entered a new scope and pushed a new frame.
      The default implementation sets up subFrame slots for handling Conditionals etc. */
  virtual void doEnterScope(const uast::AstNode* ast, ExtraData extraData) {
    createSubFrames(ast);
  }

  /** If the given AST node creates a scope, push a new Frame onto the stack
      and call doEnterScope.

      Returns whether a new scope was entered.
   */
  bool enterScope(const uast::AstNode* ast, ExtraData extraData) {
    if (resolution::createsScope(ast->tag())) {
      scopeStack.emplace_back(new Frame(ast));
      doEnterScope(ast, extraData);
      return true;
    }
    return false;
  }

  /** code to run right before we exit a scope and pop the current frame.
      The default implementation handles storing the frame into
      the parent frame's subFrames, if applicable. It also handles
      incorporating the frame's control flow information into the parent frame. */
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

  /** If the given AST node creates a scope, call doExitScope and pop the current frame.

      Returns whether a new scope was exited.
   */
  bool exitScope(const uast::AstNode* ast, ExtraData extraData) {
    if (resolution::createsScope(ast->tag())) {
      doExitScope(ast, extraData);
      scopeStack.pop_back();
      return true;
    }
    return false;
  }

  /** Overriden by subclasses to determine the value of a when case expression.
      This is used for evaluating short-circuited / 'param' known (or not) When statements. */
  virtual const types::Param* determineWhenCaseValue(const uast::AstNode* ast, ExtraData extraData) = 0;

  /** Overriden by subclasses to determine the value of an if condition.
      This is used for evaluating short-circuited / 'param known' (or not) Conditional statements. */
  virtual const types::Param* determineIfValue(const uast::AstNode* ast, ExtraData extraData) = 0;

  bool isParamTrue(const types::Param* param) { return param && param->isNonZero(); }
  bool isParamFalse(const types::Param* param) { return param && param->isZero(); }

  /** Overriden by subclasses to traverse a node using their visitor startegy.
      This is used for entering bodies of 'param'-known branches. */
  virtual void traverseNode(const uast::AstNode* ast, ExtraData extraData) = 0;

  /** Traverse the given AST node, taking into account (and traversing,
      if possible) 'param'-known branches. Returns whether the caller
      should continue traversing the node's children. */
  bool branchSensitivelyTraverse(const uast::Select* sel, ExtraData extraData) {
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
  bool branchSensitivelyTraverse(const uast::Conditional* cond, ExtraData extraData) {
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
    // Not 'param'-known condition; visit both branches as normal.
    return true;
  }
};

}  // namespace resolution
}  // namespace chpl

#endif
