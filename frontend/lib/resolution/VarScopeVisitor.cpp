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

#include "VarScopeVisitor.h"

#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/uast/all-uast.h"

#include "Resolver.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;


bool VarFrame::addToDeclaredVars(ID varId) {
  auto pair = declaredVars.insert(std::move(varId));
  return pair.second;
}
bool VarFrame::addToInitedVars(ID varId) {
  auto pair = initedVars.insert(std::move(varId));
  return pair.second;
}

void
VarScopeVisitor::process(const uast::AstNode* symbol,
                         ResolutionResultByPostorderID& byPostorder) {
  MutatingResolvedVisitor<VarScopeVisitor> rv(context,
                                              symbol,
                                              *this,
                                              byPostorder);

  // Traverse formals and then the body. This is done here rather
  // than in enter(Function) because nested functions will have
  // 'process' called on them separately.
  if (auto fn = symbol->toFunction()) {
    // traverse formals and then traverse the body
    if (auto body = fn->body()) {
      // make a pretend scope for the formals
      enterScope(body, rv);

      // traverse the formals
      for (auto formal : fn->formals()) {
        formal->traverse(rv);
      }

      // traverse the real body
      body->traverse(rv);

      exitScope(body, rv);
    }
  } else {
    symbol->traverse(rv);
  }
}

const AstNode* VarScopeVisitor::currentStatement() {
  if (inAstStack.empty()) {
    return nullptr;
  }

  for (ssize_t i = inAstStack.size() - 1; i >= 0; i--) {
    const AstNode* ast = inAstStack[i];
    const AstNode* parent = nullptr;
    if (i > 0) {
      parent = inAstStack[i-1];
    }

    if (ast->isInherentlyStatement()) {
      return ast;
    }

    if (parent && parent->isSimpleBlockLike()) {
      return ast;
    }
  }

  return nullptr;
}

VarFrame* VarScopeVisitor::currentThenFrame() {
  VarFrame* frame = currentFrame();
  CHPL_ASSERT(frame->scopeAst->isConditional());
  CHPL_ASSERT(frame->subBlocks.size() == 1 || frame->subBlocks.size() == 2);
  VarFrame* ret = frame->subBlocks[0].frame.get();
  // ret can be nullptr if the if branch was skipped at resolution time
  // (i.e. if the condition was param false).
  return ret;
}
VarFrame* VarScopeVisitor::currentElseFrame() {
  VarFrame* frame = currentFrame();
  CHPL_ASSERT(frame->scopeAst->isConditional());
  CHPL_ASSERT(frame->subBlocks.size() == 1 || frame->subBlocks.size() == 2);
  if (frame->subBlocks.size() == 1) {
    // there was no 'else' clause
    return nullptr;
  } else {
    return frame->subBlocks[1].frame.get();
  }
}

int VarScopeVisitor::currentNumCatchFrames() {
  VarFrame* frame = currentFrame();
  CHPL_ASSERT(frame->scopeAst->isTry());
  int ret = frame->subBlocks.size();
  CHPL_ASSERT(frame->scopeAst->toTry()->numHandlers() == ret);
  return ret;
}
VarFrame* VarScopeVisitor::currentCatchFrame(int i) {
  VarFrame* frame = currentFrame();
  CHPL_ASSERT(frame->scopeAst->isTry());
  CHPL_ASSERT(0 <= i && (size_t) i < frame->subBlocks.size());
  VarFrame* ret = frame->subBlocks[i].frame.get();
  CHPL_ASSERT(ret);
  return ret;
}

ID VarScopeVisitor::refersToId(const AstNode* ast, RV& rv) {
  ID toId;
  if (ast != nullptr) {
    toId = rv.byAst(ast).toId();
  }
  return toId;
}

void VarScopeVisitor::processMentions(const AstNode* ast, RV& rv) {
  // This could be its own ResolvedVisitor if it needs to handle
  // more complex forms. For now, this simple implementation should suffice
  // for expressions used as actuals etc.
  if (auto ident = ast->toIdentifier()) {
    ID toId = refersToId(ident, rv);
    if (!toId.isEmpty()) {
      handleMention(ident, toId, rv);
    }
  } else {
    for (auto child : ast->children()) {
      processMentions(child, rv);
    }
  }
}

bool
VarScopeVisitor::processSplitInitAssign(const OpCall* ast,
                                        const std::set<ID>& allSplitInitedVars,
                                        RV& rv) {
  bool inserted = false;
  VarFrame* frame = currentFrame();
  auto lhsAst = ast->actual(0);
  ID lhsVarId = refersToId(lhsAst, rv);
  if (!lhsVarId.isEmpty() && allSplitInitedVars.count(lhsVarId) > 0) {
    inserted = frame->addToInitedVars(lhsVarId);
  }
  return inserted;
}

bool
VarScopeVisitor::processSplitInitOut(const FnCall* ast,
                                     const AstNode* actual,
                                     const std::set<ID>& allSplitInitedVars,
                                     RV& rv) {
  bool inserted = false;
  VarFrame* frame = currentFrame();
  ID actualVarId = refersToId(actual, rv);
  if (!actualVarId.isEmpty() && allSplitInitedVars.count(actualVarId) > 0) {
    inserted = frame->addToInitedVars(actualVarId);
  }
  return inserted;
}

bool VarScopeVisitor::processDeclarationInit(const VarLikeDecl* ast, RV& rv) {
  VarFrame* frame = currentFrame();
  bool inserted = false;
  if (ast->initExpression() != nullptr) {
    inserted = frame->addToInitedVars(ast->id());
  }
  return inserted;
}

const QualifiedType& VarScopeVisitor::returnOrYieldType() {
  return fnReturnType;
}

void VarScopeVisitor::enterScope(const AstNode* ast, RV& rv) {
  if (createsScope(ast->tag())) {
    scopeStack.push_back(toOwned(new VarFrame(ast)));
  }
  if (auto c = ast->toConditional()) {
    // note thenBlock / elseBlock
    VarFrame* condFrame = scopeStack.back().get();
    condFrame->subBlocks.push_back(ControlFlowSubBlock(c->thenBlock()));
    condFrame->subBlocks.push_back(ControlFlowSubBlock(c->elseBlock()));
  } else if (auto t = ast->toTry()) {
    // note each of the catch handlers (aka catch clauses)
    VarFrame* tryFrame = scopeStack.back().get();
    for (auto clause : t->handlers()) {
      tryFrame->subBlocks.push_back(ControlFlowSubBlock(clause));
    }
  }
}
void VarScopeVisitor::exitScope(const AstNode* ast, RV& rv) {
  if (createsScope(ast->tag())) {
    CHPL_ASSERT(!scopeStack.empty());
    size_t n = scopeStack.size();
    owned<VarFrame>& curFrame = scopeStack[n-1];
    CHPL_ASSERT(curFrame->scopeAst == ast);
    VarFrame* parentFrame = nullptr; // Conditional or Try
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
    if (savedSubBlock) {
      // frame will be processed with parent block
      CHPL_ASSERT(parentFrame->scopeAst->isConditional() ||
             parentFrame->scopeAst->isTry());
    } else if (auto cond = ast->toConditional()) {
      handleConditional(cond, rv);
      if (parentFrame != nullptr) {
        // if both branches return or throw, update the parent frame.
        VarFrame* thenFrame = currentThenFrame();
        VarFrame* elseFrame = currentElseFrame();
        if (thenFrame && elseFrame &&
            thenFrame->returnsOrThrows && elseFrame->returnsOrThrows) {
          parentFrame->returnsOrThrows = true;
        }
      }
    } else if (auto t = ast->toTry()) {
      handleTry(t, rv);
      // if the try returns/throws
      // and any catch clauses also return/throws, update the parent frame
      if (parentFrame != nullptr) {
        VarFrame* tryFrame = currentFrame();
        if (tryFrame->returnsOrThrows) {
          int nCatchFrames = currentNumCatchFrames();
          bool allCatchReturnThrow = true;
          for (int i = 0; i < nCatchFrames; i++) {
            VarFrame* catchFrame = currentCatchFrame(i);
            if (!catchFrame->returnsOrThrows) {
              allCatchReturnThrow = false;
              break;
            }
          }
          if (allCatchReturnThrow) {
            parentFrame->returnsOrThrows = true;
          }
        }
      }
    } else {
      handleScope(ast, rv);
      // update the parent frame with the returns/throws status
      if (parentFrame != nullptr) {
        if (!ast->isLoop()) {
          // don't propagate return/throw out of a loop,
          // because it could be loop { break; return; } e.g.
          VarFrame* frame = currentFrame();
          if (frame->returnsOrThrows) {
            parentFrame->returnsOrThrows = true;
          }
        }
      }
    }
    scopeStack.pop_back();
  }
}

void VarScopeVisitor::enterAst(const uast::AstNode* ast) {
  inAstStack.push_back(ast);
}
void VarScopeVisitor::exitAst(const uast::AstNode* ast) {
  CHPL_ASSERT(!inAstStack.empty() && ast == inAstStack.back());
  inAstStack.pop_back();
}

bool VarScopeVisitor::enter(const NamedDecl* ast, RV& rv) {

  if (ast->id().postOrderId() < 0) {
    // It's a symbol with a different path, e.g. a Function.
    // Don't try to resolve it now in this
    // traversal. Instead, resolve it e.g. when the function is called.
    return false;
  }

  enterAst(ast);
  enterScope(ast, rv);

  return true;
}
void VarScopeVisitor::exit(const NamedDecl* ast, RV& rv) {
  if (ast->id().postOrderId() < 0) {
    // It's a symbol with a different path, e.g. a Function.
    // Don't try to resolve it now in this
    // traversal. Instead, resolve it e.g. when the function is called.
    return;
  }

  CHPL_ASSERT(!scopeStack.empty());
  if (!scopeStack.empty()) {
    if (auto vld = ast->toVarLikeDecl()) {
      handleDeclaration(vld, rv);
    }
  }

  exitScope(ast, rv);
  exitAst(ast);
}


bool VarScopeVisitor::enter(const OpCall* ast, RV& rv) {
  enterAst(ast);

  if (ast->op() == USTR("=")) {
    // What is the RHS of the '=' call?
    auto rhsAst = ast->actual(1);

    // visit the RHS first
    rhsAst->traverse(rv);

    handleAssign(ast, rv);
  }

  return false;
}

void VarScopeVisitor::exit(const OpCall* ast, RV& rv) {
  exitAst(ast);
}


bool VarScopeVisitor::enter(const FnCall* callAst, RV& rv) {
  enterAst(callAst);

  if (rv.hasAst(callAst)) {
    // Does any return-intent-overload use 'in', 'out', or 'inout'?
    // This filter is intended as an optimization.
    const MostSpecificCandidates& candidates = rv.byAst(callAst).mostSpecific();
    bool anyInOutInout = false;
    for (const TypedFnSignature* fn : candidates) {
      if (fn != nullptr) {
        int n = fn->numFormals();
        for (int i = 0; i < n; i++) {
          const QualifiedType& formalQt = fn->formalType(i);
          auto kind = formalQt.kind();
          if (kind == QualifiedType::OUT ||
              kind == QualifiedType::IN || kind == QualifiedType::CONST_IN ||
              kind == QualifiedType::INOUT) {
            anyInOutInout = true;
            break;
          }
        }
        if (anyInOutInout) {
          break;
        }
      }
    }

    if (!anyInOutInout) {
      // visit the actuals to gather mentions
      for (auto actualAst : callAst->actuals()) {
        actualAst->traverse(rv);
      }
    } else {
      // Use FormalActualMap to figure out which variable ID
      // is passed to a formal with out/in/inout intent.
      // Issue an error if it does not match among return intent overloads.
      std::vector<const AstNode*> actualAsts;
      auto ci = CallInfo::create(context, callAst, rv.byPostorder(),
                                 /* raiseErrors */ false,
                                 &actualAsts);

      // compute a vector indicating which actuals are passed to
      // an 'out' formal in all return intent overloads
      std::vector<QualifiedType> actualFormalTypes;
      std::vector<Qualifier> actualFormalIntents;
      computeActualFormalIntents(context, candidates, ci, actualAsts,
                                 actualFormalIntents, actualFormalTypes);

      int actualIdx = 0;
      for (auto actual : ci.actuals()) {
        (void) actual; // avoid compilation error about unused variable

        const AstNode* actualAst = actualAsts[actualIdx];
        Qualifier kind = actualFormalIntents[actualIdx];

        // handle an actual that is passed to an 'out'/'in'/'inout' formal
        if (kind == Qualifier::OUT) {
          handleOutFormal(callAst, actualAst,
                          actualFormalTypes[actualIdx], rv);
        } else if (kind == Qualifier::IN || kind == Qualifier::CONST_IN) {
          handleInFormal(callAst, actualAst,
                         actualFormalTypes[actualIdx], rv);
        } else if (kind == Qualifier::INOUT) {
          handleInoutFormal(callAst, actualAst,
                            actualFormalTypes[actualIdx], rv);
        } else {
          // otherwise, visit the actuals to gather mentions
          actualAst->traverse(rv);
        }

        actualIdx++;
      }
    }
  }

  return false;
}

void VarScopeVisitor::exit(const FnCall* ast, RV& rv) {
  exitAst(ast);
}


bool VarScopeVisitor::enter(const Return* ast, RV& rv) {
  enterAst(ast);
  return true;
}
void VarScopeVisitor::exit(const Return* ast, RV& rv) {
  if (!scopeStack.empty()) {
    VarFrame* frame = scopeStack.back().get();
    frame->returnsOrThrows = true;
    handleReturn(ast, rv);
  }
  exitAst(ast);
}


bool VarScopeVisitor::enter(const Throw* ast, RV& rv) {
  enterAst(ast);
  return true;
}
void VarScopeVisitor::exit(const Throw* ast, RV& rv) {
  if (!scopeStack.empty()) {
    VarFrame* frame = scopeStack.back().get();
    frame->returnsOrThrows = true;
    handleThrow(ast, rv);
  }
  exitAst(ast);
}

bool VarScopeVisitor::enter(const Yield* ast, RV& rv) {
  enterAst(ast);
  return true;
}
void VarScopeVisitor::exit(const Yield* ast, RV& rv) {
  if (!scopeStack.empty()) {
    // does not set returnsOrThrows because iterators
    // are continued after the yield.
    handleYield(ast, rv);
  }
  exitAst(ast);
}

bool VarScopeVisitor::enter(const Identifier* ast, RV& rv) {
  enterAst(ast);
  return true;
}
void VarScopeVisitor::exit(const Identifier* ast, RV& rv) {
  if (!scopeStack.empty()) {
    ID toId = rv.byAst(ast).toId();
    if (!toId.isEmpty()) {
      handleMention(ast, toId, rv);
    }
  }
  exitAst(ast);
}

bool VarScopeVisitor::enter(const Conditional* cond, RV& rv) {
  enterAst(cond);
  enterScope(cond, rv);

  auto condRE = rv.byAst(cond->condition());
  if (condRE.type().isParamTrue()) {
    // Don't need to process the false branch.
    cond->thenBlock()->traverse(rv);
    return false;
  } else if (condRE.type().isParamFalse()) {
    if (auto elseBlock = cond->elseBlock()) {
      elseBlock->traverse(rv);
    }
    return false;
  }

  // Not param-known condition; visit both branches as normal.
  return true;
}

void VarScopeVisitor::exit(const Conditional* cond, RV& rv) {
  exitScope(cond, rv);
  exitAst(cond);
}

bool VarScopeVisitor::enter(const AstNode* ast, RV& rv) {
  enterAst(ast);
  enterScope(ast, rv);

  return true;
}
void VarScopeVisitor::exit(const AstNode* ast, RV& rv) {
  exitScope(ast, rv);
  exitAst(ast);
}


static Qualifier normalizeFormalIntent(Qualifier intent) {
  switch (intent) {
    case Qualifier::OUT:
      return Qualifier::OUT;

    case Qualifier::IN:
    case Qualifier::CONST_IN:
      return Qualifier::IN;

    case Qualifier::INOUT:
      return Qualifier::INOUT;

    default:
      return Qualifier::UNKNOWN;
  }
}

void
computeActualFormalIntents(Context* context,
                           const MostSpecificCandidates& candidates,
                           const CallInfo& ci,
                           const std::vector<const AstNode*>& actualAsts,
                           std::vector<Qualifier>& actualFormalIntents,
                           std::vector<QualifiedType>& actualFormalTypes) {

  int nActuals = ci.numActuals();
  actualFormalIntents.clear();
  actualFormalIntents.resize(nActuals);
  actualFormalTypes.clear();
  actualFormalTypes.resize(nActuals);

  int nFns = candidates.numBest();
  if (nFns == 0) {
    // return early if there are no candidates
    return;
  }

  bool firstCandidate = true;
  for (const TypedFnSignature* fn : candidates) {
    if (fn != nullptr) {
      auto formalActualMap = FormalActualMap(fn, ci);
      for (int actualIdx = 0; actualIdx < nActuals; actualIdx++) {
        const FormalActual* fa = formalActualMap.byActualIdx(actualIdx);
        auto intent  = normalizeFormalIntent(fa->formalType().kind());
        QualifiedType& aft = actualFormalTypes[actualIdx];

        if (firstCandidate) {
          actualFormalIntents[actualIdx] = intent;
          if (intent != Qualifier::UNKNOWN) {
            aft = fa->formalType();
          }
        } else {
          // check that the intent and types match
          if (actualFormalIntents[actualIdx] != intent) {
            // TODO: fix this error once return intent overloading implemented.
            context->error(actualAsts[actualIdx],
                  "return intent overloading but intent does not match");
            actualFormalIntents[actualIdx] = Qualifier::UNKNOWN;
          } else if (intent != Qualifier::UNKNOWN && aft != fa->formalType()) {
            // TODO: fix this error once return intent overloading implemented.
            context->error(actualAsts[actualIdx],
                "using return intent overloads but the return "
                "intent overloads do not have matching formal types");
          }
        }
      }
      firstCandidate = false;
    }
  }
}



} // end namespace resolution
} // end namespace chpl
