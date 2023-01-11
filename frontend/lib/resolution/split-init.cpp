/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/resolution/scope-queries.h"
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
  void handleConditional(const Conditional* cond, RV& rv) override;
  void handleTry(const Try* t, RV& rv) override;
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
      if (ast->storageKind() == IntentList::OUT) {
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
  for (auto id : outFormals) {
    frame->mentionedVars.insert(id);
  }
}

void FindSplitInits::handleThrow(const uast::Throw* ast, RV& rv) {
  // no action needed
}

void FindSplitInits::handleYield(const uast::Yield* ast, RV& rv) {
  // no action needed
}

// updates the back frame with the combined result from
// the then/else blocks from the conditional
void FindSplitInits::handleConditional(const Conditional* cond, RV& rv) {
  VarFrame* frame = currentFrame();
  VarFrame* thenFrame = currentThenFrame();
  VarFrame* elseFrame = currentElseFrame();

  // save results for vars declared in then/else
  // gather the set of variables to consider
  std::set<ID> locInitedVars;
  for (auto id : thenFrame->initedVars) {
    if (thenFrame->eligibleVars.count(id) > 0) {
      // variable declared in this scope, so save the result
      allSplitInitedVars.insert(id);
    } else if (elseFrame == nullptr ||
               elseFrame->mentionedVars.count(id) == 0) {
      // variable inited in 'then' but not mentioned in 'else'
      locInitedVars.insert(id);
    }
  }
  if (elseFrame != nullptr) {
    for (auto id : elseFrame->initedVars) {
      if (elseFrame->eligibleVars.count(id) > 0) {
        // variable declared in this scope, so save the result
        allSplitInitedVars.insert(id);
      } else if (thenFrame->mentionedVars.count(id) == 0) {
        // variable inited in 'else' and not mentioned in 'then'
        locInitedVars.insert(id);
      }
    }
  }

  bool thenReturnsThrows = thenFrame->returnsOrThrows;
  bool elseReturnsThrows = false;
  if (elseFrame != nullptr) {
    elseReturnsThrows = elseFrame->returnsOrThrows;
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

    if (thenInits && elseInits) {
      locSplitInitedVars.insert(id);
    } else if ((thenInits         && elseReturnsThrows) ||
               (thenReturnsThrows && elseInits)) {
      // one branch returns or throws and the other inits
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
      addInit(frame, id, rhsType); // propagate
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
        addInit(frame, id, rhsType); // propagate
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
  // these asserts should always be true because the 'both'
  // set was computed first; and because the vectors are
  // just the ordering for things in the set.
  CHPL_ASSERT(splitInitedBothElseIds.size() == size);
  CHPL_ASSERT(splitInitedBothThenTypes.size() == size);
  CHPL_ASSERT(splitInitedBothElseTypes.size() == size);
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
    /* Why do we insist that the types match for split init?
       Say we are resolving this:
       {
          var x;

          if cond {
            x = 5;
            compilerWarning('in if branch, x.type is ' + x.type:string);
          } else {
            x = 12.0;
            compilerWarning('in else branch, x.type is ' + x.type:string);
          }
        }

        If it succeeds at compiling, the type of 'x' will seem to change
        as it compiles. It's OK to be temporarily wrong about a type
        like this if we are going to issue an error, but to do so
        for correct code seems problematic.
     */
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
    if (thenFrame->declaredVars.count(id) == 0) {
      frame->mentionedVars.insert(id);
    }
  }
  if (elseFrame != nullptr) {
    for (auto id : elseFrame->mentionedVars) {
      if (elseFrame->declaredVars.count(id) == 0) {
        frame->mentionedVars.insert(id);
      }
    }
  }

  handleScope(cond, rv);
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
  for (auto id : tryFrame->initedVars) {
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
  for (auto pair : tryFrame->initedVarsVec) {
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
         ast->isTry();
}

void FindSplitInits::handleScope(const AstNode* ast, RV& rv) {
  VarFrame* frame = currentFrame();
  VarFrame* parent = currentParentFrame();

  if (allowsSplitInit(ast)) {
    // a scope that allows split init (e.g. a regular { } block)

    // propagate initedVars and update global result
    for (auto pair : frame->initedVarsVec) {
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
      for (auto id : frame->mentionedVars) {
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
      for (auto id : frame->initedVars) {
        if (frame->eligibleVars.count(id) == 0) {
          parent->mentionedVars.insert(id);
        }
      }
      for (auto id : frame->mentionedVars) {
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
