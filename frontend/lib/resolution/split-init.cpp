/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/resolution/split-init.h"

#include "Resolver.h"

#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/uast/all-uast.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

struct SplitInitFrame;

struct ControlFlowSubBlock {
  const AstNode* block = nullptr; // then block / else block / catch block
  owned<SplitInitFrame> frame;
  ControlFlowSubBlock(const AstNode* block) : block(block) { }
};

struct SplitInitFrame {
  const AstNode* scopeAst = nullptr; // for debugging

  // Which variables are declared in this scope without
  // an initialization expression?
  std::set<ID> declaredEligibleVars;

  // Which variables are initialized in this scope before
  // being mentioned, throwing or returning?
  std::set<ID> initedVars;

  // Same as initedVars but preserves order
  std::vector<std::pair<ID, QualifiedType>> initedVarsVec;

  // Which variables are mentioned in this scope before
  // being mentioned, throwing or returning?
  std::set<ID> mentionedVars;

  // has the block already encountered a return?
  bool returns = false;

  // has the block already encountered a throw?
  bool throws = false;

  // When processing a conditional or catch blocks,
  // instead of popping the SplitInitFrame for the if/else/catch blocks,
  // store them here, for use in handleExitScope(Conditional or Try).
  std::vector<ControlFlowSubBlock> subBlocks;

  SplitInitFrame(const AstNode* scopeAst) : scopeAst(scopeAst) { }

  void addMention(ID dstId);
  void addInit(ID dstId, QualifiedType rhsType);
};

void SplitInitFrame::addMention(ID dstId) {
  mentionedVars.insert(dstId);
}
void SplitInitFrame::addInit(ID dstId, QualifiedType rhsType) {
  auto pair = initedVars.insert(dstId);
  if (pair.second) {
    // if it was inserted into the set, also record order and type
    initedVarsVec.emplace_back(dstId, rhsType);
  }
}

struct FindSplitInits {
  using RV = ResolvedVisitor<FindSplitInits>;

  // inputs to the process
  Context* context = nullptr;

  // result of the process
  std::set<ID> allSplitInitedVars;

  // internal variables
  std::vector<owned<SplitInitFrame>> scopeStack;

  // main entry point to this code
  static void process(Resolver& resolver, std::set<ID>& splitInitedVars);

  FindSplitInits(Context* context);

  void handleMention(ID dstId);
  void handleInitOrAssign(ID dstId, QualifiedType rhsType, RV& rv);
  void handleExitConditional(const uast::Conditional* cond);
  void handleExitTry(const uast::Try* t);
  void handleExitScope(const uast::AstNode* ast);

  void enterScope(const uast::AstNode* ast);
  void exitScope(const uast::AstNode* ast);

  bool enter(const VarLikeDecl* ast, RV& rv);
  void exit(const VarLikeDecl* ast, RV& rv);

  bool enter(const OpCall* ast, RV& rv);
  void exit(const OpCall* ast, RV& rv);

  bool enter(const FnCall* ast, RV& rv);
  void exit(const FnCall* ast, RV& rv);

  bool enter(const Return* ast, RV& rv);
  void exit(const Return* ast, RV& rv);

  bool enter(const Throw* ast, RV& rv);
  void exit(const Throw* ast, RV& rv);

  bool enter(const Identifier* ast, RV& rv);
  void exit(const Identifier* ast, RV& rv);

  bool enter(const uast::AstNode* node, RV& rv);
  void exit(const uast::AstNode* node, RV& rv);
};

FindSplitInits::FindSplitInits(Context* context)
  : context(context)
{
}

void FindSplitInits::handleMention(ID dstId) {
  if (scopeStack.empty()) return;

  bool foundReturnsThrows = false;
  bool foundInitedVar = false;
  bool foundMentionedVar = false;
  bool foundDeclaredVar = false;
  for (ssize_t i = scopeStack.size() - 1; i >= 0; i--) {
    SplitInitFrame* frame = scopeStack[i].get();
    if (frame->returns || frame->throws) {
      foundReturnsThrows = true;
    }
    if (frame->initedVars.count(dstId) > 0) {
      foundInitedVar = true;
    }
    if (frame->mentionedVars.count(dstId) > 0) {
      foundMentionedVar = true;
    }
    if (frame->declaredEligibleVars.count(dstId) > 0) {
      foundDeclaredVar = true;
      break;
    }
  }

  if (!foundDeclaredVar) return;

  // if it wasn't already initialized or mentioned, we mark it mentioned
  // in this frame.
  if (!foundReturnsThrows && !foundInitedVar && !foundMentionedVar) {
    SplitInitFrame* frame = scopeStack.back().get();
    frame->mentionedVars.insert(dstId);
  }
}

void FindSplitInits::handleInitOrAssign(ID dstId,
                                        QualifiedType rhsType,
                                        RV& rv) {
  if (scopeStack.empty()) return;

  bool foundReturnsThrows = false;
  bool foundInitedVar = false;
  bool foundMentionedVar = false;
  bool foundDeclaredVar = false;
  for (ssize_t i = scopeStack.size() - 1; i >= 0; i--) {
    SplitInitFrame* frame = scopeStack[i].get();
    if (frame->returns || frame->throws) {
      foundReturnsThrows = true;
    }
    if (frame->initedVars.count(dstId) > 0) {
      foundInitedVar = true;
    }
    if (frame->mentionedVars.count(dstId) > 0) {
      foundMentionedVar = true;
    }
    if (frame->declaredEligibleVars.count(dstId) > 0) {
      foundDeclaredVar = true;
      break;
    }
  }

  if (!foundDeclaredVar) return;

  // if it wasn't already initialized or mentioned, we mark it initialized
  // in this frame.
  if (!foundReturnsThrows && !foundInitedVar && !foundMentionedVar) {
    // normalize the type -- only consider param value for dstId being param.
    QualifiedType::Kind kind = QualifiedType::VAR;
    if (rv.hasId(dstId)) {
      kind = rv.byId(dstId).type().kind();
    }

    const Param* p = rhsType.param();
    if (kind != QualifiedType::PARAM) {
      p = nullptr;
    }
    auto useType = QualifiedType(kind, rhsType.type(), p);

    // add the split init entry to the current frame
    SplitInitFrame* frame = scopeStack.back().get();
    frame->addInit(dstId, useType);
  }
}

static bool allowsSplitInit(const uast::AstNode* ast) {
  return ast->isBlock() ||
         ast->isConditional() ||
         ast->isLocal() ||
         ast->isSerial() ||
         ast->isTry();
}

// updates the back frame with the combined result from
// the then/else blocks from the conditional
void FindSplitInits::handleExitConditional(const uast::Conditional* cond) {
  SplitInitFrame* frame = scopeStack.back().get();
  assert(frame->subBlocks.size() == 1 || frame->subBlocks.size() == 2);

  SplitInitFrame* thenFrame = frame->subBlocks[0].frame.get();
  SplitInitFrame* elseFrame = nullptr;
  if (frame->subBlocks.size() == 2) {
    elseFrame = frame->subBlocks[1].frame.get();
  }

  // save results for vars declared in then/else
  // gather the set of variables to consider
  std::set<ID> locInitedVars;
  for (auto id : thenFrame->initedVars) {
    if (thenFrame->declaredEligibleVars.count(id) > 0) {
      // variable declared in this scope, so save the result
      allSplitInitedVars.insert(id);
    } else {
      locInitedVars.insert(id);
    }
  }
  if (elseFrame != nullptr) {
    for (auto id : elseFrame->initedVars) {
      if (elseFrame->declaredEligibleVars.count(id) > 0) {
        // variable declared in this scope, so save the result
        allSplitInitedVars.insert(id);
      } else {
        locInitedVars.insert(id);
      }
    }
  }

  bool thenReturns = thenFrame->returns;
  bool thenThrows = thenFrame->throws;
  bool thenReturnsThrows = thenReturns || thenThrows;
  bool elseReturns = false;
  bool elseThrows = false;
  bool elseReturnsThrows = false;
  if (elseFrame != nullptr) {
    elseReturns = elseFrame->returns;
    elseThrows = elseFrame->throws;
    elseReturnsThrows = elseReturns || elseThrows;
  }

  std::set<ID> locSplitInitedVars;

  // now consider the initialized 'vars' && propagate them
  // split init is OK if:
  //   both then & else blocks initialize it before mentioning it
  //   one initializes it and the other returns
  for (auto id : locInitedVars) {
    bool thenInits = thenFrame->initedVars.count(id) > 0;
    bool elseInits = false;
    if (elseFrame) {
      elseInits = elseFrame->initedVars.count(id) > 0;
    }

    if ((thenInits         && elseInits) ||
        (thenInits         && elseReturnsThrows) ||
        (thenReturnsThrows && elseInits)) {
      locSplitInitedVars.insert(id);
    } else {
      frame->mentionedVars.insert(id);
    }
  }

  // compute set of variables initialized in both branches
  // (different from locSplitInitedVars because one branch might have
  //  returned early)
  // this will be used for error checking
  std::set<ID> splitInitedInBoth;
  for (auto id : thenFrame->initedVars) {
    if (elseFrame && elseFrame->initedVars.count(id) > 0) {
      splitInitedInBoth.insert(id);
    }
  }

  // Compute the then and else orders and RHS types for error checking
  // Propagate the split-inited variables
  std::vector<ID> splitInitedBothThenIds;
  std::vector<ID> splitInitedBothElseIds;
  std::vector<QualifiedType> splitInitedBothThenTypes;
  std::vector<QualifiedType> splitInitedBothElseTypes;

  for (auto pair: thenFrame->initedVarsVec) {
    ID id = pair.first;
    QualifiedType rhsType = pair.second;
    if (locSplitInitedVars.count(id) > 0) {
      frame->addInit(id, rhsType); // propagate
      if (splitInitedInBoth.count(id) > 0) {
        splitInitedBothThenIds.push_back(id);
        splitInitedBothThenTypes.push_back(rhsType);
      }
    }
  }
  if (elseFrame) {
    for (auto pair: elseFrame->initedVarsVec) {
      ID id = pair.first;
      QualifiedType rhsType = pair.second;
      if (locSplitInitedVars.count(id) > 0) {
        frame->addInit(id, rhsType); // propagate
        if (splitInitedInBoth.count(id) > 0) {
          splitInitedBothElseIds.push_back(id);
          splitInitedBothElseTypes.push_back(rhsType);
        }
      }
    }
  }

  // do error checking
  // * split-init variables are initialized in the same order
  //   in the 'then' and 'else' blocks
  // * split-init variables are initialized with the same type
  //   in the 'then' and 'else' blocks
  size_t size = splitInitedBothThenIds.size();
  assert(splitInitedBothElseIds.size() == size);
  assert(splitInitedBothThenTypes.size() == size);
  assert(splitInitedBothElseTypes.size() == size);
  bool orderOk = true;
  // first, check the order
  for (size_t i = 0; i < size; i++) {
    ID thenId = splitInitedBothThenIds[i];
    ID elseId = splitInitedBothElseIds[i];

    if (thenId != elseId) {
      orderOk = false;
      context->error(cond,
                     "initialization order in conditional does not match");
      break;
    }
  }
  // then, check the types
  if (orderOk) {
    for (size_t i = 0; i < size; i++) {
      QualifiedType thenRhsType = splitInitedBothThenTypes[i];
      QualifiedType elseRhsType = splitInitedBothElseTypes[i];
      if (thenRhsType != elseRhsType) {
        context->error(cond,
                       "types do not match in conditional split init");
      }
    }
  }

  // propagate the mentioned variables
  for (auto id : thenFrame->mentionedVars) {
    if (thenFrame->declaredEligibleVars.count(id) == 0) {
      frame->mentionedVars.insert(id);
    }
  }
  if (elseFrame != nullptr) {
    for (auto id : elseFrame->mentionedVars) {
      if (elseFrame->declaredEligibleVars.count(id) == 0) {
        frame->mentionedVars.insert(id);
      }
    }
  }
}

void FindSplitInits::handleExitTry(const uast::Try* t) {
  SplitInitFrame* tryFrame = scopeStack.back().get();

  // if there are no catch clauses, there is nothing to do
  if (tryFrame->subBlocks.size() == 0) {
    return;
  }

  // otherwise, consider the catch clauses

  // do all of the catch clauses always throw or return?
  bool allThrowOrReturn = true;
  for (const auto& sub : tryFrame->subBlocks) {
    SplitInitFrame* subFrame = sub.frame.get();
    if (!subFrame->returns && !subFrame->throws) {
      allThrowOrReturn = false;
    }
  }

  // collect variables to tell parent frame we are split initing here
  std::set<ID> trySplitInitVars;
  std::set<ID> tryMentionedVars = tryFrame->mentionedVars;

  // consider the variables we wish to init.
  // Are they mentioned in any catch clauses? Including with assignment?
  // If they are not eligible for split-init, move them to mentionedVars.
  for (auto id : tryFrame->initedVars) {
    if (tryFrame->declaredEligibleVars.count(id) > 0) {
      // variable declared in the Try scope, so save the result
      allSplitInitedVars.insert(id);
    } else {
      // gather variables to be split-inited for parent frames
      // into trySplitInitVars
      bool allowSplitInit = false;
      if (allThrowOrReturn) {
        bool mentionedOrInitedInCatch = false;
        for (const auto& sub : tryFrame->subBlocks) {
          SplitInitFrame* subFrame = sub.frame.get();
          if (subFrame->initedVars.count(id) > 0 ||
              subFrame->mentionedVars.count(id) > 0) {
            mentionedOrInitedInCatch = true;
            break;
          }
        }
        // if no catch clauses mention the variable, it is a split init
        allowSplitInit = !mentionedOrInitedInCatch;
      }
      if (allowSplitInit) {
        trySplitInitVars.insert(id);
      } else {
        tryMentionedVars.insert(id);
      }
    }
  }

  // compute the inited vars types and ordering
  std::vector<std::pair<ID, QualifiedType>> tryInitedVarsVec;
  for (auto pair : tryFrame->initedVarsVec) {
    ID id = pair.first;
    QualifiedType rhsType = pair.second;
    if (trySplitInitVars.count(id) > 0) {
      tryInitedVarsVec.emplace_back(id, rhsType);
    }
  }

  // now, update mentionedVars with anything at all mentioned in a catch
  for (const auto& sub : tryFrame->subBlocks) {
    SplitInitFrame* subFrame = sub.frame.get();
    tryMentionedVars.insert(subFrame->initedVars.begin(),
                            subFrame->initedVars.end());
    tryMentionedVars.insert(subFrame->mentionedVars.begin(),
                            subFrame->mentionedVars.end());
  }

  // swap trySplitInitVars / tryMentionedVars into place
  // for use in handleExitScope
  tryFrame->initedVars.swap(trySplitInitVars);
  tryFrame->initedVarsVec.swap(tryInitedVarsVec);
  tryFrame->mentionedVars.swap(tryMentionedVars);
}

void FindSplitInits::handleExitScope(const uast::AstNode* ast) {
  size_t n = scopeStack.size();
  SplitInitFrame* frame = scopeStack.back().get();
  SplitInitFrame* parent = nullptr;
  if (n >= 2) {
    parent = scopeStack[n-2].get();
  }

  if (allowsSplitInit(ast)) {
    // a scope that allows split init (e.g. a regular { } block)

    // update 'frame' with control-flow logic
    if (auto cond = ast->toConditional()) {
      // process then/else blocks to merge into Conditional's frame
      handleExitConditional(cond);
    }
    if (auto t = ast->toTry()) {
      // process catch blocks to merge into Try's frame
      handleExitTry(t);
    }

    // propagate initedVars and update global result
    for (auto pair : frame->initedVarsVec) {
      ID id = pair.first;
      QualifiedType rhsType = pair.second;
      if (frame->declaredEligibleVars.count(id) > 0) {
        // variable declared in this scope, so save the result
        allSplitInitedVars.insert(id);
      } else if (parent != nullptr) {
        // variable declared in a parent scope, so update parent scope
        parent->addInit(id, rhsType);
      }
    }

    if (parent != nullptr) {
      // propagate mentionedVars
      for (auto id : frame->mentionedVars) {
        if (frame->declaredEligibleVars.count(id) == 0) {
          parent->mentionedVars.insert(id);
        }
      }
      // propagate return/throw-ness
      if (frame->returns) {
        parent->returns = true;
      }
      if (frame->throws) {
        parent->throws = true;
      }
    }
  } else {
    // some kind of scope that does not allow split init
    // (e.g., a loop)
    SplitInitFrame* frame = scopeStack.back().get();
    if (parent != nullptr) {
      // propagate initedVars and mentionedVars
      // to mentionedVars in the parent
      for (auto id : frame->initedVars) {
        if (frame->declaredEligibleVars.count(id) == 0) {
          parent->mentionedVars.insert(id);
        }
      }
      for (auto id : frame->mentionedVars) {
        if (frame->declaredEligibleVars.count(id) == 0) {
          parent->mentionedVars.insert(id);
        }
      }
    }
  }
}

void FindSplitInits::enterScope(const AstNode* ast) {
  if (createsScope(ast->tag())) {
    scopeStack.push_back(toOwned(new SplitInitFrame(ast)));
  }
  if (auto c = ast->toConditional()) {
    // note thenBlock / elseBlock
    SplitInitFrame* condFrame = scopeStack.back().get();
    condFrame->subBlocks.push_back(ControlFlowSubBlock(c->thenBlock()));
    condFrame->subBlocks.push_back(ControlFlowSubBlock(c->elseBlock()));
  } else if (auto t = ast->toTry()) {
    // note each of the catch handlers (aka catch clauses)
    SplitInitFrame* tryFrame = scopeStack.back().get();
    for (auto clause : t->handlers()) {
      tryFrame->subBlocks.push_back(ControlFlowSubBlock(clause));
    }
  }
}
void FindSplitInits::exitScope(const AstNode* ast) {
  if (createsScope(ast->tag())) {
    assert(!scopeStack.empty());
    size_t n = scopeStack.size();
    owned<SplitInitFrame>& curFrame = scopeStack[n-1];
    assert(curFrame->scopeAst == ast);
    SplitInitFrame* parentFrame = nullptr; // Conditional or Try
    bool savedSubBlock = false;
    if (n >= 2) {
      // Are we in the situation of just finishing a Block
      // that is within a Conditional? or a Catch clause?
      // If so, save the frame in the subBlocks, to
      // be handled when processing condFrame.
      parentFrame = scopeStack[n-2].get();
      for (auto & subBlock : parentFrame->subBlocks) {
        if (subBlock.block == ast) {
          subBlock.frame.swap(curFrame);
          savedSubBlock = true;
        }
      }
    }
    if (!savedSubBlock) {
      handleExitScope(ast);
    }
    scopeStack.pop_back();
  }
}

bool FindSplitInits::enter(const VarLikeDecl* ast, RV& rv) {

  enterScope(ast);

  return true;
}
void FindSplitInits::exit(const VarLikeDecl* ast, RV& rv) {
  assert(!scopeStack.empty());
  if (!scopeStack.empty()) {
    if (ast->initExpression() == nullptr) {
      SplitInitFrame* frame = scopeStack.back().get();
      frame->declaredEligibleVars.insert(ast->id());
    }
  }

  exitScope(ast);
}

bool FindSplitInits::enter(const OpCall* ast, RV& rv) {

  if (ast->op() == USTR("=")) {
    // What is the RHS and LHS of the '=' call?
    auto lhsAst = ast->actual(0);
    auto rhsAst = ast->actual(1);

    // visit the RHS first
    rhsAst->traverse(rv);

    QualifiedType rhsType = rv.byAst(rhsAst).type();

    // now consider the LHS
    ID toId = rv.byAst(lhsAst).toId();

    if (!toId.isEmpty()) {
      handleInitOrAssign(toId, rhsType, rv);
    } else {
      // visit the LHS to check for mentions
      lhsAst->traverse(rv);
    }
  }

  return false;
}

void FindSplitInits::exit(const OpCall* ast, RV& rv) {
}

bool FindSplitInits::enter(const FnCall* callAst, RV& rv) {

  // variables can be split-inited if they are
  // directly passed to an 'out' intent formal

  if (rv.hasAst(callAst)) {
    // Do all of the return-intent-overloads use 'out' intent at all?
    // This filter is intended as an optimization.
    const MostSpecificCandidates& candidates = rv.byAst(callAst).mostSpecific();
    bool eachCandidateHasAnOutFormal = !candidates.isEmpty();
    for (const TypedFnSignature* fn : candidates) {
      if (fn != nullptr) {
        int n = fn->numFormals();
        bool candidateHasAnOutFormal = false;
        for (int i = 0; i < n; i++) {
          const QualifiedType& formalQt = fn->formalType(i);
          if (formalQt.kind() == QualifiedType::OUT) {
            candidateHasAnOutFormal = true;
            break;
          }
        }
        if (!candidateHasAnOutFormal) {
          eachCandidateHasAnOutFormal = false;
          break;
        }
      }
    }

    if (!eachCandidateHasAnOutFormal) {
      // visit the actuals to gather mentions
      for (auto actualAst : callAst->actuals()) {
        actualAst->traverse(rv);
      }
    } else {
      // Use FormalActualMap to figure out which variable ID
      // is passed to a formal with out intent for all return intent overloads
      // and then record it with handleInitOrAssign.

      auto calledExprAst = callAst->calledExpression();
      if (rv.hasAst(calledExprAst)) {
        std::vector<const AstNode*> actualAsts;
        auto ci = CallInfo::create(context, callAst, rv.byPostorder(),
                                   /* raiseErrors */ false,
                                   &actualAsts);

        // compute a vector indicating which actuals are passed to
        // an 'out' formal in all return intent overloads
        std::vector<QualifiedType> actualFormalTypes;
        std::vector<int8_t> actualIdxToOut;
        computeOutFormals(context, candidates, ci, actualAsts,
                          actualIdxToOut, actualFormalTypes);

        int actualIdx = 0;
        for (auto actual : ci.actuals()) {
          (void) actual; // avoid compilation error about unused variable

          // find an actual referring to an ID that is passed to an 'out' formal
          ID toId;
          const AstNode* actualAst = actualAsts[actualIdx];
          if (actualAst != nullptr && rv.hasAst(actualAst)) {
            toId = rv.byAst(actualAst).toId();
          }
          if (actualIdxToOut[actualIdx] && !toId.isEmpty()) {
            // it is like an assignment to the variable with ID toID
            handleInitOrAssign(toId, actualFormalTypes[actualIdx], rv);
          } else {
            // otherwise, visit the actuals to gather mentions
            actualAst->traverse(rv);
          }

          actualIdx++;
        }
      }
    }
  }

  return false;
}

void FindSplitInits::exit(const FnCall* ast, RV& rv) {
}


bool FindSplitInits::enter(const Return* ast, RV& rv) {
  return true;
}
void FindSplitInits::exit(const Return* ast, RV& rv) {
  if (!scopeStack.empty()) {
    SplitInitFrame* frame = scopeStack.back().get();
    frame->returns = true;
  }
}

bool FindSplitInits::enter(const Throw* ast, RV& rv) {
  return true;
}
void FindSplitInits::exit(const Throw* ast, RV& rv) {
  if (!scopeStack.empty()) {
    SplitInitFrame* frame = scopeStack.back().get();
    frame->throws = true;
  }
}

bool FindSplitInits::enter(const Identifier* ast, RV& rv) {
  return true;
}
void FindSplitInits::exit(const Identifier* ast, RV& rv) {
  if (!scopeStack.empty()) {
    ID toId;
    if (rv.hasAst(ast)) {
      toId = rv.byAst(ast).toId();
    }
    if (!toId.isEmpty()) {
      handleMention(toId);
    }
  }
}



bool FindSplitInits::enter(const AstNode* ast, RV& rv) {
  enterScope(ast);

  return true;
}
void FindSplitInits::exit(const AstNode* ast, RV& rv) {
  exitScope(ast);
}

std::set<ID>
computeSplitInits(Context* context,
                  const uast::AstNode* symbol,
                  const ResolutionResultByPostorderID& byPostorder) {
  std::set<ID> splitInitedVars;
  FindSplitInits uv(context);

  ResolvedVisitor<FindSplitInits> rv(context,
                                     symbol,
                                     uv,
                                     byPostorder);

  if (auto fn = symbol->toFunction()) {
    // traverse formals and then traverse the body
    if (auto body = fn->body()) {
      // make a pretend scope for the formals
      uv.enterScope(body);

      // traverse the formals
      for (auto formal : fn->formals()) {
        formal->traverse(rv);
      }

      // traverse the real body
      body->traverse(rv);

      uv.exitScope(body);
    }
  } else {
    symbol->traverse(rv);
  }

  // swap the result into place
  splitInitedVars.swap(uv.allSplitInitedVars);

  return splitInitedVars;
}

void
computeOutFormals(Context* context,
                  const MostSpecificCandidates& candidates,
                  const CallInfo& ci,
                  const std::vector<const AstNode*>& actualAsts,
                  std::vector<int8_t>& actualIsOut,
                  std::vector<QualifiedType>& actualFormalTypes) {

  int nActuals = ci.numActuals();
  actualIsOut.clear();
  actualIsOut.resize(nActuals);

  int nFns = candidates.numBest();
  if (nFns == 0) {
    // return early if there are no actual candidates
    return;
  }

  for (const TypedFnSignature* fn : candidates) {
    if (fn != nullptr) {
      auto formalActualMap = FormalActualMap(fn, ci);
      for (int actualIdx = 0; actualIdx < nActuals; actualIdx++) {
        const FormalActual* fa = formalActualMap.byActualIdx(actualIdx);
        int8_t isOutFormal = fa->formalType().kind() == QualifiedType::OUT;
        actualIsOut[actualIdx] += isOutFormal;

        if (isOutFormal) {
          if ((size_t) actualIdx >= actualFormalTypes.size()) {
            actualFormalTypes.resize(actualIdx+1);
          }
          QualifiedType& aft = actualFormalTypes[actualIdx];
          if (aft.type() == nullptr && fa->formalType().type() != nullptr) {
            // no type was yet gathered, so update the type
            aft = fa->formalType();
          } else {
            // check that the type matches
            if (aft != fa->formalType()) {
              context->error(actualAsts[actualIdx],
                  "using return intent overloads with 'out' formals "
                  "but the return intent overloads do not have matching "
                  "'out' formal types");
            }
          }
        }
      }
    }
  }

  for (int actualIdx = 0; actualIdx < nActuals; actualIdx++) {
    if (actualIsOut[actualIdx] == nFns) {
      actualIsOut[actualIdx] = 1;
    } else {
      actualIsOut[actualIdx] = 0;
    }
  }
}



} // end namespace resolution
} // end namespace chpl
