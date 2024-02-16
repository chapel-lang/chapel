/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/framework/ErrorBase.h"
#include "chpl/uast/all-uast.h"

#include "VarScopeVisitor.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

struct SplitInitVarStatus;

struct FindSplitInits : VarScopeVisitor {
  // result of the process
  std::set<ID> allSplitInitedVars;

  // internal variables
  std::set<ID> outFormals;

  // methods
  FindSplitInits(Context* context)
    : VarScopeVisitor(context, QualifiedType()),
      allSplitInitedVars() {
  }

  SplitInitVarStatus findVarStatus(ID varId);
  static void addInit(VarFrame* frame, ID varId, QualifiedType rhsType);
  void handleInitOrAssign(ID varId, QualifiedType rhsType, RV& rv);

  std::map<ID,QualifiedType> verifyInitOrderAndType(const AstNode * node, 
                                                    const std::vector<VarFrame*>& frames,
                                                    const std::set<ID>& splitInitedVars);

  // overrides
  void handleDeclaration(const VarLikeDecl* ast, RV& rv) override;
  void handleMention(const Identifier* ast, ID varId, RV& rv) override;
  void handleAssign(const OpCall* ast, RV& rv) override;
  void handleOutFormal(const FnCall* ast, const AstNode* actual,
                       const QualifiedType& formalType,
                       RV& rv) override;
  void handleInFormal(const FnCall* ast, const AstNode* actual,
                      const QualifiedType& formalType,
                      RV& rv) override;
  void handleInoutFormal(const FnCall* ast, const AstNode* actual,
                         const QualifiedType& formalType,
                         RV& rv) override;

  void handleReturn(const uast::Return* ast, RV& rv) override;
  void handleThrow(const uast::Throw* ast, RV& rv) override;
  void handleYield(const uast::Yield* ast, RV& rv) override;
  void handleTry(const Try* t, RV& rv) override;

  void handleDisjunction(const AstNode * node, 
                         VarFrame * currentFrame,
                         const std::vector<VarFrame*>& frames, 
                         bool total, RV& rv) override;

  void handleScope(const AstNode* ast, RV& rv) override;
};

struct SplitInitVarStatus {
  bool foundReturnsThrows = false;
  bool foundInitedVar = false;
  bool foundMentionedVar = false;
  bool foundDeclaredVar = false;
};

SplitInitVarStatus FindSplitInits::findVarStatus(ID varId) {
  SplitInitVarStatus status;

  if (scopeStack.empty()) return status;

  for (ssize_t i = scopeStack.size() - 1; i >= 0; i--) {
    VarFrame* frame = scopeStack[i].get();
    if (frame->returnsOrThrows) {
      status.foundReturnsThrows = true;
    }
    if (frame->initedVars.count(varId) > 0) {
      status.foundInitedVar = true;
    }
    if (frame->mentionedVars.count(varId) > 0) {
      status.foundMentionedVar = true;
    }
    if (frame->declaredVars.count(varId) > 0) {
      if (frame->eligibleVars.count(varId) > 0) {
        status.foundDeclaredVar = true;
      }
      break;
    }
  }

  return status;
}

void FindSplitInits::addInit(VarFrame* frame,
                             ID varId, QualifiedType rhsType) {
  bool inserted = frame->addToInitedVars(varId);
  if (inserted) {
    // if it was inserted into the set, also record order and type
    frame->initedVarsVec.emplace_back(varId, rhsType);
  }
}

void FindSplitInits::handleInitOrAssign(ID varId,
                                        QualifiedType rhsType,
                                        RV& rv) {
  VarFrame* frame = currentFrame();
  SplitInitVarStatus s = findVarStatus(varId);

  if (!s.foundDeclaredVar) return;

  // if it wasn't already initialized or mentioned, we mark it initialized
  // in this frame.
  if (!s.foundReturnsThrows && !s.foundInitedVar && !s.foundMentionedVar) {
    // normalize the type -- only consider param value for varId being param.
    QualifiedType::Kind kind = QualifiedType::VAR;
    if (rv.hasId(varId)) {
      kind = rv.byId(varId).type().kind();
    }

    const Param* p = rhsType.param();
    if (kind != QualifiedType::PARAM) {
      p = nullptr;
    }
    auto useType = QualifiedType(kind, rhsType.type(), p);

    // add the split init entry to the current frame
    addInit(frame, varId, useType);
  }
}

void FindSplitInits::handleDeclaration(const VarLikeDecl* ast, RV& rv) {
  VarFrame* frame = currentFrame();
  bool inserted = frame->addToDeclaredVars(ast->id());
  if (inserted) {
    if (ast->initExpression() == nullptr) {
      frame->eligibleVars.insert(ast->id());
    }
    if (ast->isFormal() || ast->isVarArgFormal()) {
      if (ast->storageKind() == Qualifier::OUT) {
        outFormals.insert(ast->id());
      }
    }
  }
}

void FindSplitInits::handleMention(const Identifier* ast, ID varId, RV& rv) {
  VarFrame* frame = currentFrame();
  SplitInitVarStatus s = findVarStatus(varId);

  if (!s.foundDeclaredVar) return;

  // if it wasn't already initialized or mentioned, we mark it mentioned
  // in this frame.
  if (!s.foundReturnsThrows && !s.foundInitedVar && !s.foundMentionedVar) {
    frame->mentionedVars.insert(varId);
  }
}

void FindSplitInits::handleAssign(const OpCall* ast, RV& rv) {
  auto lhsAst = ast->actual(0);
  auto rhsAst = ast->actual(1);

  ID lhsVarId = refersToId(lhsAst, rv);
  if (!lhsVarId.isEmpty()) {
    // get the type for the rhs
    QualifiedType rhsType = rv.byAst(rhsAst).type();
    handleInitOrAssign(lhsVarId, rhsType, rv);
  } else {
    // visit the LHS to check for mentions
    lhsAst->traverse(rv);
  }
}

void FindSplitInits::handleOutFormal(const FnCall* ast, const AstNode* actual,
                                     const QualifiedType& formalType, RV& rv) {
  ID toId = refersToId(actual, rv);
  if (!toId.isEmpty()) {
    handleInitOrAssign(toId, formalType, rv);
  } else {
    // gather any mentions used in the actual
    processMentions(actual, rv);
  }
}

void FindSplitInits::handleInFormal(const FnCall* ast, const AstNode* actual,
                                    const QualifiedType& formalType,
                                    RV& rv) {
  processMentions(actual, rv);
}

void FindSplitInits::handleInoutFormal(const FnCall* ast, const AstNode* actual,
                                       const QualifiedType& formalType,
                                       RV& rv) {
  processMentions(actual, rv);
}

void FindSplitInits::handleReturn(const uast::Return* ast, RV& rv) {
  // consider all 'out' formals to be mentioned by the return
  VarFrame* frame = currentFrame();
  for (const auto& id : outFormals) {
    frame->mentionedVars.insert(id);
  }
}

void FindSplitInits::handleThrow(const uast::Throw* ast, RV& rv) {
  // no action needed
}

void FindSplitInits::handleYield(const uast::Yield* ast, RV& rv) {
  // no action needed
}

static void propagateMentions(VarFrame * parentFrame, VarFrame* frame) {
  for (const auto& id : frame->mentionedVars) {
    if (frame->declaredVars.count(id) == 0) {
      parentFrame->mentionedVars.insert(id);
    }
  }
}


std::map<ID,QualifiedType> FindSplitInits::verifyInitOrderAndType(const AstNode * node, 
                                                                  const std::vector<VarFrame*>& frames,
                                                                  const std::set<ID>& splitInitedVars) {

  std::vector<std::pair<ID,QualifiedType>> referenceInitOrder;
  std::map<ID, QualifiedType> referenceInitTypes;
  std::map<ID, int> initTypeBranchIdxs;
  int frameCount = 0; // needed to track branches for error reporting
  for (auto frame : frames) {
    
    size_t idx = 0;

    for (auto & pair : frame->initedVarsVec) {
      auto& id = pair.first;
      auto& qt = pair.second;

      // only consider the variables that are being split-inited
      if (splitInitedVars.count(id) == 0) continue;

      //check types for all frames
      if (referenceInitTypes.count(id) == 0) {
        referenceInitTypes[id] = qt;
        initTypeBranchIdxs[id] = frameCount;
      } else if (referenceInitTypes[id] != qt) {
        CHPL_REPORT(context, SplitInitMismatchedConditionalTypes,
                    parsing::idToAst(context, id)->toVariable(), node,
                    qt, referenceInitTypes[id], 
                    frameCount, initTypeBranchIdxs[id]);
        return std::map<ID,QualifiedType>();
      }

      // only check initialization order if the branch 
      // does not unconditionally return
      if (frame->returnsOrThrows) continue;

      if (idx >= referenceInitOrder.size()) {
        referenceInitOrder.emplace_back(id, qt);
      } else if (referenceInitOrder[idx].first != id) {
          context->error(node, 
                       "initialization order does not match between branches");
          return std::map<ID,QualifiedType>();
      }
      idx++;
    }
    frameCount++;
  }
  return referenceInitTypes;
}

void FindSplitInits::handleDisjunction(const AstNode * node, 
                                       VarFrame * currentFrame, 
                                       const std::vector<VarFrame*>& frames, 
                                       bool total, RV& rv) {

  // first, if the branch is known at compile time, just process that one.
  for(auto frame: frames) {
    if (!frame->paramTrueCond) continue;

    for (auto pair : frame->initedVarsVec) {
      const auto & id = pair.first;
      const auto & rhsType = pair.second;
      if (frame->eligibleVars.count(id) > 0) {
        // variable is local to this frame's scope; save the result.
        allSplitInitedVars.insert(id);
      } else {
        // variable was declared in an outer scope, and this is only
        // possible path, so its definitely a valid split init
        addInit(currentFrame, id, rhsType);
      }
    }

    propagateMentions(currentFrame, frame);
    return;
  }

  // gather the set of variables inited in any of the branches
  std::set<ID> locallyInitedVars;
  for(auto frame : frames) {
    for (const auto& id : frame->initedVars) {
      if (frame->eligibleVars.count(id) > 0) {
        // variable declared and inited locally. save the result
        allSplitInitedVars.insert(id);
      } else {
        // it was declared in an outer scope
        locallyInitedVars.insert(id);
      }
    }
  }

  // remove variables mentioned in other branches, as that means they
  // were not initialized in that branch. 
  for(auto frame: frames) {
    for (const auto& id : frame->mentionedVars) {
      locallyInitedVars.erase(id);
    }
  }

  // calculate the set of variables that are split initialized in at least 
  // one branch. A variable is split inited if all branches either 
  // return/throw or initialize the variable.
  std::set<ID> locallySplitInitedVars;
  for (auto id : locallyInitedVars) {
    bool allFramesInitReturnOrThrow = true;
    for(auto frame : frames) {
      bool thisFrameInits = frame->initedVars.count(id) > 0;
      allFramesInitReturnOrThrow &= thisFrameInits || frame->returnsOrThrows;
    }
    if (allFramesInitReturnOrThrow) {
      locallySplitInitedVars.insert(id);
    } else {
      currentFrame->mentionedVars.insert(id);
    }
  }

  if (!total) {
    for (auto frame: frames) {
      propagateMentions(currentFrame, frame);
    } 
    for (const auto & id: locallySplitInitedVars) {
      currentFrame->mentionedVars.insert(id);
    }
    return;
  }
  
  auto verifiedInits = verifyInitOrderAndType(node, frames, 
                                              locallySplitInitedVars);
  for (auto& pair : verifiedInits) {
    addInit(currentFrame, pair.first, pair.second);
  }

  for (auto frame: frames) {
    propagateMentions(currentFrame, frame);
  }
}

void FindSplitInits::handleTry(const Try* t, RV& rv) {
  VarFrame* tryFrame = currentFrame();
  int nCatchFrames = currentNumCatchFrames();
  // if there are no catch clauses, treat it like any other scope
  if (nCatchFrames == 0) {
    handleScope(t, rv);
    return;
  }

  // otherwise, consider the catch clauses

  // do all of the catch clauses always throw or return?
  bool allThrowOrReturn = true;
  for (int i = 0; i < nCatchFrames; i++) {
    VarFrame* catchFrame = currentCatchFrame(i);
    if (!catchFrame->returnsOrThrows) {
      allThrowOrReturn = false;
    }
  }

  // collect variables to tell parent frame we are split initing here
  std::set<ID> trySplitInitVars;
  std::set<ID> tryMentionedVars = tryFrame->mentionedVars;

  // consider the variables we wish to init.
  // Are they mentioned in any catch clauses? Including with assignment?
  // If they are not eligible for split-init, move them to mentionedVars.
  for (const auto& id : tryFrame->initedVars) {
    if (tryFrame->eligibleVars.count(id) > 0) {
      // variable declared in the Try scope, so save the result
      allSplitInitedVars.insert(id);
    } else {
      // gather variables to be split-inited for parent frames
      // into trySplitInitVars
      bool allowSplitInit = false;
      if (allThrowOrReturn) {
        bool mentionedOrInitedInCatch = false;
        for (int i = 0; i < nCatchFrames; i++) {
          VarFrame* catchFrame = currentCatchFrame(i);
          if (catchFrame->initedVars.count(id) > 0 ||
              catchFrame->mentionedVars.count(id) > 0) {
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
  for (const auto& pair : tryFrame->initedVarsVec) {
    ID id = pair.first;
    QualifiedType rhsType = pair.second;
    if (trySplitInitVars.count(id) > 0) {
      tryInitedVarsVec.emplace_back(id, rhsType);
    }
  }

  // now, update mentionedVars with anything at all mentioned in a catch
  for (int i = 0; i < nCatchFrames; i++) {
    VarFrame* catchFrame = currentCatchFrame(i);
    tryMentionedVars.insert(catchFrame->initedVars.begin(),
                            catchFrame->initedVars.end());
    tryMentionedVars.insert(catchFrame->mentionedVars.begin(),
                            catchFrame->mentionedVars.end());
  }

  // swap trySplitInitVars / tryMentionedVars into place
  // for use in handleScope
  tryFrame->initedVars.swap(trySplitInitVars);
  tryFrame->initedVarsVec.swap(tryInitedVarsVec);
  tryFrame->mentionedVars.swap(tryMentionedVars);

  handleScope(t, rv);
}

static bool allowsSplitInit(const AstNode* ast) {
  return ast->isBlock() ||
         ast->isConditional() ||
         ast->isLocal() ||
         ast->isSerial() ||
         ast->isSelect() ||
         ast->isTry();
}

void FindSplitInits::handleScope(const AstNode* ast, RV& rv) {
  VarFrame* frame = currentFrame();
  VarFrame* parent = currentParentFrame();

  if (allowsSplitInit(ast)) {
    // a scope that allows split init (e.g. a regular { } block)

    // propagate initedVars and update global result
    for (const auto& pair : frame->initedVarsVec) {
      ID id = pair.first;
      QualifiedType rhsType = pair.second;
      if (frame->eligibleVars.count(id) > 0) {
        // variable declared in this scope, so save the result
        allSplitInitedVars.insert(id);
      } else if (parent != nullptr) {
        // variable declared in a parent scope, so update parent scope
        addInit(parent, id, rhsType);
      }
    }

    if (parent != nullptr) {
      // propagate mentionedVars
      for (const auto& id : frame->mentionedVars) {
        if (frame->eligibleVars.count(id) == 0) {
          parent->mentionedVars.insert(id);
        }
      }
    }
  } else {
    // some kind of scope that does not allow split init
    // (e.g., a loop)
    if (parent != nullptr) {
      // propagate initedVars and mentionedVars
      // to mentionedVars in the parent
      for (const auto& id : frame->initedVars) {
        if (frame->eligibleVars.count(id) == 0) {
          parent->mentionedVars.insert(id);
        }
      }
      for (const auto& id : frame->mentionedVars) {
        if (frame->eligibleVars.count(id) == 0) {
          parent->mentionedVars.insert(id);
        }
      }
    }
  }
}

std::set<ID>
computeSplitInits(Context* context,
                  const AstNode* symbol,
                  const ResolutionResultByPostorderID& byPostorder) {
  std::set<ID> splitInitedVars;
  FindSplitInits uv(context);

  uv.process(symbol,
             // cast here allows VarScopeVisitor to be simple
             // by always using the mutating visitor
             const_cast<ResolutionResultByPostorderID&>(byPostorder));

  splitInitedVars.swap(uv.allSplitInitedVars);
  return splitInitedVars;
}


} // end namespace resolution
} // end namespace chpl
