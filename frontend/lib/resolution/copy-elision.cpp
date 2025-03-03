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

#include "chpl/resolution/copy-elision.h"

#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/ClassType.h"
#include "chpl/uast/all-uast.h"

#include "VarScopeVisitor.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

struct FindElidedCopies : VarScopeVisitor {
  // inputs
  const std::set<ID>& allSplitInitedVars;
  const PoiScope* poiScope = nullptr; // used for checking init= calls
  const Block* fnBody = nullptr; // used for handling implicit return

  // internal variables
  std::set<ID> outOrInoutFormals;

  // result of the process
  std::set<ID> allElidedCopyFromIds;

  // methods
  FindElidedCopies(Context* context,
                   const PoiScope* poiScope,
                   const Block* fnBody,
                   const std::set<ID>& allSplitInitedVars,
                   QualifiedType fnReturnType)
    : VarScopeVisitor(context, std::move(fnReturnType)),
      allSplitInitedVars(allSplitInitedVars),
      poiScope(poiScope),
      fnBody(fnBody) {
  }

  bool hasCrossTypeInitAssignWithIn(const QualifiedType& lhsType,
                                    const QualifiedType& rhsType,
                                    const AstNode* ast,
                                    RV& rv);
  bool copyElisionAllowedForTypes(const QualifiedType& lhsType,
                                  const QualifiedType& rhsType,
                                  const AstNode* ast,
                                  RV& rv);
  static bool lastMentionIsCopy(VarFrame* frame, ID varId);
  static void gatherLastMentionIsCopyVars(VarFrame* frame, std::set<ID>& vars);
  static void addDeclaration(VarFrame* frame, const VarLikeDecl* ast);
  static void addCopyInit(VarFrame* frame, ID fromVarId, ID point);
  static void addMention(VarFrame* frame, ID varId);

  // save the copy-elided variables in frame to allElidedCopyFromIds
  void saveElidedCopies(VarFrame* frame);
  // save the copy-elided local variables in frame to allElidedCopyFromIds
  void saveLocalVarElidedCopies(VarFrame* frame);

  bool isEligibleVarInAnyFrame(ID varId);

  void noteMentionsForOutFormals(VarFrame* frame);

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

static bool kindAllowsCopyElision(Qualifier kind) {
  return (kind == Qualifier::VAR ||
          kind == Qualifier::CONST_VAR ||
          kind == Qualifier::IN ||
          kind == Qualifier::CONST_IN ||
          kind == Qualifier::OUT ||
          kind == Qualifier::INOUT);
}

// if this ends up representing any significant overhead,
// we can cache the result in the ResolvedExpression,
// or try to build up a map from type to type indicating convertibility.
bool FindElidedCopies::hasCrossTypeInitAssignWithIn(
                                           const QualifiedType& lhsType,
                                           const QualifiedType& rhsType,
                                           const AstNode* ast,
                                           RV& rv) {
  std::vector<CallInfoActual> actuals;
  actuals.push_back(CallInfoActual(lhsType, USTR("this")));
  actuals.push_back(CallInfoActual(rhsType, UniqueString()));
  auto ci = CallInfo (/* name */ USTR("init="),
                      /* calledType */ lhsType,
                      /* isMethodCall */ true,
                      /* hasQuestionArg */ false,
                      /* isParenless */ false,
                      actuals);
  const Scope* scope = scopeForId(context, ast->id());
  auto inScopes = CallScopeInfo::forNormalCall(scope, poiScope);
  auto rc = createDummyRC(context);
  auto c = resolveGeneratedCall(&rc, ast, ci, inScopes);
  const MostSpecificCandidates& fns = c.mostSpecific();
  // return intent overloading should not be possible with an init=
  CHPL_ASSERT(fns.numBest() <= 1);
  if (auto msc = fns.only()) {
    auto fn = msc.fn();
    // check for 'in' intent on the 'other' formal
    if (fn->numFormals() >= 2) {
      auto intent = fn->formalType(1).kind();
      if (isInQualifier(intent)) {
        return true;
      }
    }
  }

  return false;
}

static bool isRecordLike(const Type* t) {
  if (auto ct = t->toClassType()) {
    auto decorator = ct->decorator();
    if (! (decorator.isBorrowed() || decorator.isUnmanaged())) {
      return true;
    }
  } else if (t->isRecordType() || t->isUnionType()) {
    return true;
  }
  // TODO: tuples?

  return false;
}

bool
FindElidedCopies::copyElisionAllowedForTypes(const QualifiedType& lhsType,
                                             const QualifiedType& rhsType,
                                             const AstNode* ast,
                                             RV& rv) {
  if (kindAllowsCopyElision(lhsType.kind()) &&
      kindAllowsCopyElision(rhsType.kind())) {
    if (lhsType.type() == rhsType.type()) {
      return true;
    } else if (lhsType.isUnknown() || rhsType.isUnknown()) {
      return false;
    } else if (isRecordLike(lhsType.type())) {
      // check to see if an there is an init= to initialize
      // lhsType from rhsType but that uses the 'in' intent on the
      // formal.
      return hasCrossTypeInitAssignWithIn(lhsType, rhsType, ast, rv);
    }
  }

  return false;
}

bool FindElidedCopies::lastMentionIsCopy(VarFrame* frame, ID varId) {
  auto it = frame->copyElisionState.find(varId);
  if (it != frame->copyElisionState.end()) {
    const CopyElisionState& state = it->second;
    return state.lastIsCopy;
  }

  return false;
}
void FindElidedCopies::gatherLastMentionIsCopyVars(VarFrame* frame,
                                                   std::set<ID>& vars) {
  for (const auto& pair : frame->copyElisionState) {
    if (pair.second.lastIsCopy) {
      vars.insert(pair.first);
    }
  }
}

void FindElidedCopies::addDeclaration(VarFrame* frame, const VarLikeDecl* ast) {
  bool inserted = frame->addToDeclaredVars(ast->id());
  if (inserted) {
    auto kind = ast->storageKind();
    if (kindAllowsCopyElision(kind)) {
      frame->eligibleVars.insert(ast->id());
    }
  }
}

// caller should guarantee types are OK and fromVarId is eligible
// in this frame or a parent frame.
void FindElidedCopies::addCopyInit(VarFrame* frame, ID fromVarId, ID point) {
  // get the map entry, default-initializing it if there was none
  CopyElisionState& state = frame->copyElisionState[fromVarId];
  state.lastIsCopy = true;
  state.points.clear();
  state.points.insert(point);
}
void FindElidedCopies::addMention(VarFrame* frame, ID varId) {
  CopyElisionState& state = frame->copyElisionState[varId];
  state.lastIsCopy = false;
  state.points.clear();
}

void FindElidedCopies::saveElidedCopies(VarFrame* frame) {
  for (const auto& pair : frame->copyElisionState) {
    const CopyElisionState& state = pair.second;
    if (state.lastIsCopy) {
      allElidedCopyFromIds.insert(state.points.begin(), state.points.end());
    }
  }
}

void FindElidedCopies::saveLocalVarElidedCopies(VarFrame* frame) {
  for (const auto& id : frame->eligibleVars) {
    CHPL_ASSERT(frame->declaredVars.count(id) > 0);
    if (lastMentionIsCopy(frame, id)) {
      const CopyElisionState& state = frame->copyElisionState[id];
      allElidedCopyFromIds.insert(state.points.begin(), state.points.end());
    }
  }
}

bool FindElidedCopies::isEligibleVarInAnyFrame(ID varId) {
  if (scopeStack.empty()) return false;

  for (ssize_t i = scopeStack.size() - 1; i >= 0; i--) {
    VarFrame* frame = scopeStack[i].get();
    if (frame->eligibleVars.count(varId) > 0) {
      return true;
    }
  }

  return false;
}

void FindElidedCopies::noteMentionsForOutFormals(VarFrame* frame) {
  // consider all 'out' or 'inout' variables to be mentioned by a 'return'
  // (since they will be communicated back to the call site)
  for (const auto& id : outOrInoutFormals) {
    addMention(frame, id);
  }
}

void FindElidedCopies::handleDeclaration(const VarLikeDecl* ast, RV& rv) {
  addDeclaration(currentFrame(), ast);
  processDeclarationInit(ast, rv);

  if (auto initExpr = ast->initExpression()) {
    VarFrame* frame = currentFrame();
    ID lhsVarId = ast->id();
    ID rhsVarId = refersToId(initExpr, rv);
    if (!rhsVarId.isEmpty() && isEligibleVarInAnyFrame(rhsVarId)) {
      // check that the types are the same
      if (rv.hasId(lhsVarId) && rv.hasId(rhsVarId)) {
        QualifiedType lhsType = rv.byId(lhsVarId).type();
        QualifiedType rhsType = rv.byId(rhsVarId).type();
        if (copyElisionAllowedForTypes(lhsType, rhsType, ast, rv)) {
          addCopyInit(frame, rhsVarId, ast->id());
        }
      }
    }
  }

  if (ast->isFormal() || ast->isVarArgFormal()) {
    if (ast->storageKind() == Qualifier::OUT ||
        ast->storageKind() == Qualifier::INOUT) {
      outOrInoutFormals.insert(ast->id());
    }
  }
}
void FindElidedCopies::handleMention(const Identifier* ast, ID varId, RV& rv) {
  VarFrame* frame = currentFrame();
  addMention(frame, varId);
}
void FindElidedCopies::handleAssign(const OpCall* ast, RV& rv) {
  auto lhsAst = ast->actual(0);
  auto rhsAst = ast->actual(1);
  bool splitInit = processSplitInitAssign(ast, allSplitInitedVars, rv);
  if (splitInit) {
    VarFrame* frame = currentFrame();

    // if it was inserted in the current frame, it was a split init,
    // so the RHS here could be a copy & might be elided
    ID lhsVarId = refersToId(lhsAst, rv);
    ID rhsVarId = refersToId(rhsAst, rv);
    if (!rhsVarId.isEmpty() && isEligibleVarInAnyFrame(rhsVarId)) {
      // check that the types are the same
      if (rv.hasId(lhsVarId) && rv.hasId(rhsVarId)) {
        QualifiedType lhsType = rv.byId(lhsVarId).type();
        QualifiedType rhsType = rv.byId(rhsVarId).type();
        if (copyElisionAllowedForTypes(lhsType, rhsType, ast, rv)) {
          addCopyInit(frame, rhsVarId, ast->id());
        }
      }
    }
  } else {
    processMentions(lhsAst, rv);
  }
}
void FindElidedCopies::handleOutFormal(const FnCall* ast,
                                       const AstNode* actual,
                                       const QualifiedType& formalType,
                                       RV& rv) {
  // 'out' can't be the RHS for an elided copy
  processMentions(actual, rv);

  // updated initedVars for split init
  processSplitInitOut(ast, actual, allSplitInitedVars, rv);
}
void FindElidedCopies::handleInFormal(const FnCall* ast, const AstNode* actual,
                                      const QualifiedType& formalType,
                                      RV& rv) {
  // 'in' formal can be copied from another variable, and that
  // copy might need to be elided
  VarFrame* frame = currentFrame();
  ID actualToId = refersToId(actual, rv);
  bool elide = false;
  if (!actualToId.isEmpty() && isEligibleVarInAnyFrame(actualToId)) {
    // check that the types are the same
    if (rv.hasId(actualToId)) {
      QualifiedType actualType = rv.byId(actualToId).type();
      if (copyElisionAllowedForTypes(formalType, actualType, actual, rv)) {
        elide = true;
      }
    }
  }

  if (elide) {
    addCopyInit(frame, actualToId, actual->id());
  } else {
    processMentions(actual, rv);
  }
}
void FindElidedCopies::handleInoutFormal(const FnCall* ast,
                                         const AstNode* actual,
                                         const QualifiedType& formalType,
                                         RV& rv) {
  // 'inout' can't be the RHS for an elided copy
  processMentions(actual, rv);
}

void FindElidedCopies::handleReturn(const uast::Return* ast, RV& rv) {
  VarFrame* frame = currentFrame();

  noteMentionsForOutFormals(frame);
  saveElidedCopies(frame);
}

void FindElidedCopies::handleThrow(const uast::Throw* ast, RV& rv) {
  VarFrame* frame = currentFrame();
  saveElidedCopies(frame);
}

void FindElidedCopies::handleYield(const uast::Yield* ast, RV& rv) {
  // if a variable is yielded by value & the variable is not used again,
  // the copy can be elided
  VarFrame* frame = currentFrame();
  ID yieldedToId = refersToId(ast->value(), rv);
  bool elide = false;
  if (!yieldedToId.isEmpty() && isEligibleVarInAnyFrame(yieldedToId)) {
    // check that the types are the same
    if (rv.hasId(yieldedToId)) {
      QualifiedType actualType = rv.byId(yieldedToId).type();
      const QualifiedType& yieldType = returnOrYieldType();
      if (copyElisionAllowedForTypes(yieldType, actualType, ast, rv)) {
        elide = true;
      }
    }
  }

  if (elide) {
    addCopyInit(frame, yieldedToId, ast->id());
  } else {
    processMentions(ast, rv);
  }

}

static void propagateMentionsAndInits(VarFrame* parentFrame, VarFrame* childFrame) {
  parentFrame->initedVars.insert(childFrame->initedVars.begin(),
                                 childFrame->initedVars.end());
  for(auto pair : childFrame->copyElisionState) {
    auto id = pair.first;
    //if there's a non-copy use, update the parent frame
    if(!pair.second.lastIsCopy) {
      CopyElisionState& parentState = parentFrame->copyElisionState[id];
      parentState.lastIsCopy = false;
      parentState.points.clear();
    }
  }
}


void FindElidedCopies::handleDisjunction(const AstNode * node,
                                         VarFrame * currentFrame, 
                                         const std::vector<VarFrame*>& frames, 
                                         bool total, RV& rv) {
  // if any frame is 'param true' the rest of the frames do not matter
  for (auto frame : frames) {
    if (!frame->knownPath) continue;
    // yielding handled during return/throw
    if (frame->returnsOrThrows) return;

    //yield the local variables
    saveLocalVarElidedCopies(frame);

    //promote outer variable access
    for (const auto& entry : frame->copyElisionState) {
      ID id = entry.first;
      const CopyElisionState& state = entry.second;
      if (state.lastIsCopy && frame->eligibleVars.count(id) == 0) {
        CopyElisionState& s = currentFrame->copyElisionState[id];
        s.lastIsCopy = true;
        s.points.insert(state.points.begin(), state.points.end());
      }
    }

    propagateMentionsAndInits(currentFrame, frame);

    return;
  }

  //propagate mentions to the parent
  for(auto frame: frames) {
    propagateMentionsAndInits(currentFrame, frame);
  }

  std::vector<VarFrame*> nonReturningFrames;
  for(auto frame: frames) {
    if (frame->returnsOrThrows) continue;
    saveLocalVarElidedCopies(frame);
    nonReturningFrames.push_back(frame);
  }

  if (!total) return;

  // Now, note all variables that are elided in all non-returning branches.
  std::unordered_map<ID, size_t> idCounts;
  for (auto frame : nonReturningFrames) {
    for(auto pair : frame->copyElisionState) {
      if (pair.second.lastIsCopy) idCounts[pair.first] += 1;
    }
  }

  for (auto pair : idCounts) {
    // Variable did not occur in all frames, it does not get promoted.
    if (pair.second != nonReturningFrames.size()) continue;

    // Populate the parent frame
    CopyElisionState& parentState = currentFrame->copyElisionState[pair.first];
    parentState.lastIsCopy = true;
    for (auto frame : nonReturningFrames) {
      CopyElisionState& frameState = frame->copyElisionState.at(pair.first);
      parentState.points.insert(frameState.points.begin(), frameState.points.end());
    }
  }
}

void FindElidedCopies::handleTry(const Try* t, RV& rv) {

  VarFrame* tryFrame = currentFrame();

  int nCatchFrames = currentNumCatchFrames();
  // if there are no catch clauses, treat it like any other scope
  if (nCatchFrames == 0) {
    // will handle variables local to the try
    handleScope(t, rv);
    return;
  }

  // otherwise, consider the catch clauses

  // don't copy elide:
  // when the copy statement is in a try or try! which
  //  * has catch clauses that mention the variable
  //  * has catch clauses that do not always throw or return

  // don't copy elide if any catch clauses mention the variable
  // don't copy elide if any catch clause doesn't throw/return
  std::unordered_map<ID,CopyElisionState> updatedState;
  std::set<ID> catchMentions;
  bool allThrowOrReturn = true;

  for (int i = 0; i < nCatchFrames; i++) {
    VarFrame* catchFrame = currentCatchFrame(i);
    if (!catchFrame->returnsOrThrows) {
      allThrowOrReturn = false;
    }
    for (const auto& pair : catchFrame->copyElisionState) {
      catchMentions.insert(pair.first);
    }
  }

  if (allThrowOrReturn) {
    for (const auto& pair : tryFrame->copyElisionState) {
      const CopyElisionState& tryState = pair.second;
      if (tryState.lastIsCopy) {
        ID id = pair.first;
        // do any catch clauses mention the variable,
        // including in a way we thought could be copy elided?
        if (catchMentions.count(id) == 0) {
          CopyElisionState& s = updatedState[id];
          s.lastIsCopy = true;
          s.points.insert(tryState.points.begin(), tryState.points.end());
        }
      }
    }
  }

  tryFrame->copyElisionState.swap(updatedState);

  //propagate mentions in the catch clauses
  for (int i = 0; i < nCatchFrames; i++) {
    VarFrame* catchFrame = currentCatchFrame(i);
    if (catchFrame) {
      propagateMentionsAndInits(tryFrame, catchFrame);
    }
  }
  handleScope(t, rv);
}

static bool allowsCopyElision(const AstNode* ast) {
  return ast->isBlock() ||
         ast->isConditional() ||
         ast->isSelect() ||
         ast->isLocal() ||
         ast->isSerial() ||
         ast->isTry();
}

void FindElidedCopies::handleScope(const AstNode* ast, RV& rv) {
  VarFrame* frame = currentFrame();
  VarFrame* parent = currentParentFrame();

  // if it's the function's body block, consider out/inout formals mentioned
  // (since they will be used by the implicit return)
  if (frame->scopeAst == fnBody) {
    noteMentionsForOutFormals(frame);
  }

  // handle any local variables
  saveLocalVarElidedCopies(frame);

  if (parent != nullptr) {
    // propagate any non-local variables
    if (allowsCopyElision(ast) && parent != nullptr) {
      for (const auto& pair : frame->copyElisionState) {
        ID id = pair.first;
        const CopyElisionState& state = pair.second;
        if (state.lastIsCopy && frame->eligibleVars.count(id) == 0) {
          CopyElisionState& parentState = parent->copyElisionState[id];
          parentState.lastIsCopy = true;
          parentState.points.insert(state.points.begin(), state.points.end());
        } else if (frame->eligibleVars.count(id) == 0) {
          CopyElisionState& parentState = parent->copyElisionState[id];
          parentState.lastIsCopy = false;
          parentState.points.clear();
        }
      }
    }

    // propagate inited vars
    parent->initedVars.insert(frame->initedVars.begin(),
                              frame->initedVars.end());
  }
}

std::set<ID>
computeElidedCopies(Context* context,
                    const uast::AstNode* symbol,
                    const ResolutionResultByPostorderID& byPostorder,
                    const PoiScope* poiScope,
                    const std::set<ID>& allSplitInitedVars,
                    QualifiedType fnYieldedType) {
  std::set<ID> elidedCopyFromIds;

  auto fn = symbol->toFunction();

  if (fn == nullptr) {
    // assume this is running on a Module

    // module-scope variables can't be copy elided, so don't run
    // the analysis on them.
    return elidedCopyFromIds;
  }

  FindElidedCopies uv(context, poiScope, fn->body(), allSplitInitedVars,
                      std::move(fnYieldedType));

  uv.process(symbol,
             // cast here allows VarScopeVisitor to be simple
             // by always using the mutating visitor
             const_cast<ResolutionResultByPostorderID&>(byPostorder));

  // swap the result into place
  elidedCopyFromIds.swap(uv.allElidedCopyFromIds);

  return elidedCopyFromIds;
}

} // end namespace resolution
} // end namespace chpl
