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

#ifndef VAR_SCOPE_VISITOR_H
#define VAR_SCOPE_VISITOR_H

#include "chpl/framework/ID.h"
#include "chpl/resolution/ResolvedVisitor.h"

namespace chpl {
namespace uast {
  class AstNode;
  class Conditional;
  class FnCall;
  class Identifier;
  class OpCall;
  class Return;
  class Throw;
  class Try;
}
namespace types {
  class QualifiedType;
}

namespace resolution {

struct VarFrame;
struct ControlFlowSubBlock;

/** Helper visitor for traversals that work with variable
    init and deinit. This class is intended to factor out code
    common between 3 analysis:
     * split init
     * copy elision
     * computing where copies and deinits occur

    Since it is only used internally for these cases,
    the interface covers whatever these analyses need. */
class VarScopeVisitor {
 protected:
  using RV = MutatingResolvedVisitor<VarScopeVisitor>;

  // ----- inputs to the process
  Context* context = nullptr;
  types::QualifiedType fnReturnType;

  // ----- internal variables
  std::vector<owned<VarFrame>> scopeStack;
  std::vector<const AstNode*> inAstStack;

  // ----- methods to be implemented by specific analysis subclass

  /** Called for a variable declaration */
  virtual void handleDeclaration(const uast::VarLikeDecl* ast, RV& rv) = 0;
  /** Called for an Identifier not used in one of the below cases */
  virtual void handleMention(const uast::Identifier* ast, ID varId, RV& rv) = 0;
  /** Called for <expr> = <expr> assignment pattern */
  virtual void handleAssign(const uast::OpCall* ast, RV& rv) = 0;
  /** Called for an actual passed to an 'out' formal */
  virtual void handleOutFormal(const uast::FnCall* ast,
                               const uast::AstNode* actual,
                               const types::QualifiedType& formalType, RV& rv) = 0;
  /** Called for an actual passed to an 'in' formal */
  virtual void handleInFormal(const uast::FnCall* ast,
                              const uast::AstNode* actual,
                              const types::QualifiedType& formalType,
                              RV& rv) = 0;
  /** Called for an actual passed to an 'out' formal */
  virtual void handleInoutFormal(const uast::FnCall* ast,
                                 const uast::AstNode* actual,
                                 const types::QualifiedType& formalType,
                                 RV& rv) = 0;

  /** Called for a 'return' */
  virtual void handleReturn(const uast::Return* ast, RV& rv) = 0;

  /** Called for a 'throw' */
  virtual void handleThrow(const uast::Throw* ast, RV& rv) = 0;

  /** Called for a 'yield' */
  virtual void handleYield(const uast::Yield* ast, RV& rv) = 0;

  /** Called to process a Conditional after handling its contents --
      should update currentFrame() which is the frame for the Conditional.
      The then/else frames are sitting in currentFrame().subBlocks. */
  virtual void handleConditional(const uast::Conditional* cond, RV& rv);
  /** Called to process a Try after handling its contents --
      should update currentFrame() which is the frame for the Try.
      The catch clause frames are sitting in currentFrame().subBlocks. */
  virtual void handleTry(const uast::Try* t, RV& rv) = 0;
  /** Called to process a Select after handling its contents --
      should update currentFrame() which is the frame for the Select.
      The when frames are sitting in currentFrame().subBlocks. */
  virtual void handleSelect(const uast::Select* cond, RV& rv);
  /** Generalizes processing Conditional and Select nodes after handling 
      their contents. Updates currentFrame based on the frames of the 
      possible branching targets stored in frames. total indicates whether
      a branch is taken no matter the input. */
  virtual void handleDisjunction(const uast::AstNode * node, 
                                 VarFrame* currentFrame, 
                                 const std::vector<VarFrame*>& frames, 
                                 bool total,
                                 RV& rv) = 0;
  /** Called to process any other Scope after handling its contents --
      should update scopeStack.back() which is the frame for the Try.
      Not called for Conditional or Try. */
  virtual void handleScope(const uast::AstNode* ast, RV& rv) = 0;


  // ----- methods for use by specific analysis subclasses

  VarScopeVisitor(Context* context, types::QualifiedType fnReturnType)
    : context(context), fnReturnType(std::move(fnReturnType)) { }

 public:
  void process(const uast::AstNode* symbol,
               ResolutionResultByPostorderID& byPostorder);

 protected:

  /** Return the current frame. This should always be safe to call
      from one of the handle* methods. */
  VarFrame* currentFrame() {
    CHPL_ASSERT(!scopeStack.empty());
    return scopeStack.back().get();
  }

  /** Returns the current statement being traversed */
  const AstNode* currentStatement();

  /** Return the parent frame or nullptr if there is none. */
  VarFrame* currentParentFrame() {
    VarFrame* parent = nullptr;
    size_t n = scopeStack.size();
    if (n >= 2) {
      parent = scopeStack[n-2].get();
    }
    return parent;
  }

  /** Assuming that the current frame refers to a Conditional,
      returns the frame for the 'then' block. */
  VarFrame* currentThenFrame();
  /** Assuming that the current frame refers to a Conditional,
      returns the frame for the 'else' block, or 'nullptr' if there was none. */
  VarFrame* currentElseFrame();

  /** Assuming that the current frame refers to a Try,
      returns the number of frames saved for Catch clauses.  */
  int currentNumCatchFrames();
  /** Assuming that the current frame refers to a Try,
      returns the i'th saved Catch frame. */
  VarFrame* currentCatchFrame(int i);

  /** Assuming that the current frame refers to a Select,
      returns the number of frames saved for When clauses.  */
  int currentNumWhenFrames();
  /** Assuming that the current frame refers to a Select,
      returns the i'th saved When frame.  */
  VarFrame * currentWhenFrame(int i);

  /** If ast is an Identifier that refers to a VarLikeDecl, return the
      Id of the VarLikeDecl. Otherwise, return an empty ID. */
  ID refersToId(const AstNode* ast, RV& rv);

  /** Call handleMention for any Identifiers contained in this ast node.
      Only appropriate for expressions (not for Loops) */
  void processMentions(const AstNode* ast, RV& rv);

  /** Update initedVars if an assignment represents a split-init.
      Returns true if it was a split init. */
  bool processSplitInitAssign(const OpCall* ast,
                              const std::set<ID>& allSplitInitedVars,
                              RV& rv);

  /** Update initedVars if a call with at 'out' formal
      represents a split-init. Returns true if it was a split init. */
  bool processSplitInitOut(const FnCall* ast,
                           const AstNode* actual,
                           const std::set<ID>& allSplitInitedVars,
                           RV& rv);

  /** Update initedVars for a declaration with an initExpression. */
  bool processDeclarationInit(const VarLikeDecl* ast, RV& rv);

  /** Returns the return or yield type of the function being processed */
  const types::QualifiedType& returnOrYieldType();

 public:
  // ----- visitor implementation
  void enterScope(const uast::AstNode* ast, RV& rv);
  void exitScope(const uast::AstNode* ast, RV& rv);

  void enterAst(const uast::AstNode* ast);
  void exitAst(const uast::AstNode* ast);

  bool enter(const NamedDecl* ast, RV& rv);
  void exit(const NamedDecl* ast, RV& rv);

  bool enter(const TupleDecl* ast, RV& rv);
  void exit(const TupleDecl* ast, RV& rv);

  bool enter(const OpCall* ast, RV& rv);
  void exit(const OpCall* ast, RV& rv);

  bool enter(const FnCall* ast, RV& rv);
  void exit(const FnCall* ast, RV& rv);

  bool enter(const Return* ast, RV& rv);
  void exit(const Return* ast, RV& rv);

  bool enter(const Throw* ast, RV& rv);
  void exit(const Throw* ast, RV& rv);

  bool enter(const Yield* ast, RV& rv);
  void exit(const Yield* ast, RV& rv);

  bool enter(const Identifier* ast, RV& rv);
  void exit(const Identifier* ast, RV& rv);

  bool enter(const uast::Conditional* node, RV& rv);
  void exit(const uast::Conditional* node, RV& rv);

  bool enter(const uast::Select* node, RV& rv);
  void exit(const uast::Select* node, RV& rv);

  bool enter(const uast::AstNode* node, RV& rv);
  void exit(const uast::AstNode* node, RV& rv);
};

/** Collects information about a Conditional's then/else blocks
    or a Try's Catch blocks. */
struct ControlFlowSubBlock {
  const AstNode* block = nullptr; // then block / else block / catch block
  owned<VarFrame> frame;
  ControlFlowSubBlock(const AstNode* block) : block(block) { }
};

/** Per-variable state used by the copy elision implementation */
struct CopyElisionState {
  bool lastIsCopy = false; // is the last mention (so far) a copy?
  std::set<ID> points; // uAST IDs of the potentially elided copies?
};

/** Collects information about a variable declaration frame / scope.
    Note that some of the fields here will only be used by a single subclass.
    Keeping them declared here keeps things simple. */
struct VarFrame {
  // ----- variables used by VarScopeVisitor
  const AstNode* scopeAst = nullptr;

  // Which variables are declared in this scope?
  // For split init, only variables without init expressions.
  std::set<ID> declaredVars;

  // Which variables are initialized in this scope?
  // This includes both locally declared and outer variables.
  std::set<ID> initedVars;

  // has the block already encountered a return or a throw?
  bool returnsOrThrows = false;

  // for conditionals/selects, does this block have a param true condition?
  bool paramTrueCond = false;

  // for conditionals/selects, is this known to be the only path?
  bool knownPath = false;

  // When processing a conditional or catch blocks,
  // instead of popping the SplitInitFrame for the then/else/catch blocks,
  // store them here, for use in handleExitScope(Conditional or Try).
  std::vector<ControlFlowSubBlock> subBlocks;


  // ----- variables declared here for use in particular subclasses

  // TODO: These probably can't store ID because, for a variable
  // declared within a param for loop, we want to have a different
  // variable for each iteration, rather than considering them all the same.

  // for split init and copy elision:
  // which variables are declared here in a way that allows split init?
  std::set<ID> eligibleVars;

  // for split init:
  // same as initedVars but preserves order & saves types
  std::vector<std::pair<ID, types::QualifiedType>> initedVarsVec;
  // Which variables are mentioned in this scope before
  // being initialized, throwing or returning?
  std::set<ID> mentionedVars;

  // for copy elision:
  // Is the last mention of the variable a copy?
  // What are the copy points?
  std::unordered_map<ID,CopyElisionState> copyElisionState;

  // for call init deinit:
  // localsAndDefers contains both VarSymbol and DeferStmt in
  // order to create a single stack for cleanup operations to be executed.
  // In particular, the ordering between defer blocks and locals matters,
  // in addition to the ordering within each group.
  // It stores variables in initialization order. It does not store
  // declared but not-yet-initialized variables.
  std::vector<ID> localsAndDefers;

  // Which outer variables have been initialized in this scope?
  // This vector lists them in initialization order.
  std::vector<ID> initedOuterVars;

  // Which variables have been deinitialized early in this scope?
  // Map of (ID of decl) -> (ID of call after which it is deinited)
  std::unordered_map<ID, ID> deinitedVars;

  VarFrame(const AstNode* scopeAst) : scopeAst(scopeAst) { }

  // returns 'true' if it was inserted
  bool addToDeclaredVars(ID varId);
  // returns 'true' if it was inserted
  bool addToInitedVars(ID varId);
};

/**
  Compute a vector indicating which actuals are passed to an 'out'/'in'/'inout'
  formal in all return intent overloads. For each actual 'i',
  actualFormalIntent[i] will be set to one of the following:
   * uast::Qualifier::OUT if it is passed to an 'out' formal
   * uast::Qualifier::IN if it is passed to an 'in' or 'const in' formal
   * uast::Qualifier::INOUT if it is passed to an 'inout' formal
   * uast::Qualifier::UNKNOWN otherwise

  actualFormalTypes will be set so that for actual 'i', if it is passed
  to an 'out'/'in'/'inout' formal, actualFormalTypes[i] will be set to the
  type of the 'out'/'in'/'inout' formal.

  If either of the above computed values do not match among
  the return intent overloads, this function will issue an error
  in the current query.
 */
void
computeActualFormalIntents(Context* context,
                           const MostSpecificCandidates& candidates,
                           const CallInfo& ci,
                           const std::vector<const AstNode*>& actualAsts,
                           std::vector<uast::Qualifier>& actualFrmlIntents,
                           std::vector<types::QualifiedType>& actualFrmlTypes);

} // end namespace resolution
} // end namespace chpl

#endif
