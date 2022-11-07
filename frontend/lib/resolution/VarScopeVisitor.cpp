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
                         const ResolutionResultByPostorderID& byPostorder) {
  ResolvedVisitor<VarScopeVisitor> rv(context,
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
      enterScope(body);

      // traverse the formals
      for (auto formal : fn->formals()) {
        formal->traverse(rv);
      }

      // traverse the real body
      body->traverse(rv);

      exitScope(body);
    }
  } else {
    symbol->traverse(rv);
  }
}

VarFrame* VarScopeVisitor::currentThenFrame() {
  VarFrame* frame = currentFrame();
  assert(frame->scopeAst->isConditional());
  assert(frame->subBlocks.size() == 1 || frame->subBlocks.size() == 2);
  VarFrame* ret = frame->subBlocks[0].frame.get();
  assert(ret);
  return ret;
}
VarFrame* VarScopeVisitor::currentElseFrame() {
  VarFrame* frame = currentFrame();
  assert(frame->scopeAst->isConditional());
  assert(frame->subBlocks.size() == 1 || frame->subBlocks.size() == 2);
  if (frame->subBlocks.size() == 1) {
    // there was no 'else' clause
    return nullptr;
  } else {
    return frame->subBlocks[1].frame.get();
  }
}

int VarScopeVisitor::currentNumCatchFrames() {
  VarFrame* frame = currentFrame();
  assert(frame->scopeAst->isTry());
  int ret = frame->subBlocks.size();
  assert(frame->scopeAst->toTry()->numHandlers() == ret);
  return ret;
}
VarFrame* VarScopeVisitor::currentCatchFrame(int i) {
  VarFrame* frame = currentFrame();
  assert(frame->scopeAst->isTry());
  assert(0 <= i && (size_t) i < frame->subBlocks.size());
  VarFrame* ret = frame->subBlocks[i].frame.get();
  assert(ret);
  return ret;
}

ID VarScopeVisitor::refersToId(const AstNode* ast, RV& rv) {
  ID toId;
  if (ast != nullptr && rv.hasAst(ast)) {
    toId = rv.byAst(ast).toId();
  }
  return toId;
}

void VarScopeVisitor::handleMentions(const AstNode* ast, RV& rv) {
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
      handleMentions(child, rv);
    }
  }
}

void VarScopeVisitor::enterScope(const AstNode* ast) {
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
void VarScopeVisitor::exitScope(const AstNode* ast) {
  if (createsScope(ast->tag())) {
    assert(!scopeStack.empty());
    size_t n = scopeStack.size();
    owned<VarFrame>& curFrame = scopeStack[n-1];
    assert(curFrame->scopeAst == ast);
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
      assert(parentFrame->scopeAst->isConditional() ||
             parentFrame->scopeAst->isTry());
    } else if (auto cond = ast->toConditional()) {
      handleConditional(cond);
    } else if (auto t = ast->toTry()) {
      handleTry(t);
    } else {
      handleScope(ast);
    }
    scopeStack.pop_back();
  }
}


bool VarScopeVisitor::enter(const VarLikeDecl* ast, RV& rv) {
  enterScope(ast);

  return true;
}
void VarScopeVisitor::exit(const VarLikeDecl* ast, RV& rv) {
  assert(!scopeStack.empty());
  if (!scopeStack.empty()) {
    handleDeclaration(ast, rv);
  }

  exitScope(ast);
}


bool VarScopeVisitor::enter(const OpCall* ast, RV& rv) {

  if (ast->op() == USTR("=")) {
    // What is the RHS and LHS of the '=' call?
    auto lhsAst = ast->actual(0);
    auto rhsAst = ast->actual(1);

    // visit the RHS first
    rhsAst->traverse(rv);

    // now consider the LHS
    ID toId = rv.byAst(lhsAst).toId();

    if (!toId.isEmpty()) {
      handleVarAssign(ast, toId, rv);
    } else {
      // visit the LHS to check for mentions
      lhsAst->traverse(rv);
    }
  }

  return false;
}

void VarScopeVisitor::exit(const OpCall* ast, RV& rv) {
}


bool VarScopeVisitor::enter(const FnCall* callAst, RV& rv) {

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
      auto calledExprAst = callAst->calledExpression();
      if (rv.hasAst(calledExprAst)) {
        std::vector<const AstNode*> actualAsts;
        auto ci = CallInfo::create(context, callAst, rv.byPostorder(),
                                   /* raiseErrors */ false,
                                   &actualAsts);

        // compute a vector indicating which actuals are passed to
        // an 'out' formal in all return intent overloads
        std::vector<QualifiedType> actualFormalTypes;
        std::vector<IntentList> actualFormalIntents;
        computeActualFormalIntents(context, candidates, ci, actualAsts,
                                   actualFormalIntents, actualFormalTypes);

        int actualIdx = 0;
        for (auto actual : ci.actuals()) {
          (void) actual; // avoid compilation error about unused variable

          const AstNode* actualAst = actualAsts[actualIdx];
          IntentList kind = actualFormalIntents[actualIdx];

          // handle an actual that is passed to an 'out'/'in'/'inout' formal
          if (kind == IntentList::OUT) {
            handleOutFormal(callAst, actualAst,
                            actualFormalTypes[actualIdx], rv);
          } else if (kind == IntentList::IN || kind == IntentList::CONST_IN) {
            handleInFormal(callAst, actualAst,
                           actualFormalTypes[actualIdx], rv);
          } else if (kind == IntentList::INOUT) {
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
  }

  return false;
}

void VarScopeVisitor::exit(const FnCall* ast, RV& rv) {
}


bool VarScopeVisitor::enter(const Return* ast, RV& rv) {
  return true;
}
void VarScopeVisitor::exit(const Return* ast, RV& rv) {
  if (!scopeStack.empty()) {
    VarFrame* frame = scopeStack.back().get();
    frame->returns = true;
  }
}


bool VarScopeVisitor::enter(const Throw* ast, RV& rv) {
  return true;
}
void VarScopeVisitor::exit(const Throw* ast, RV& rv) {
  if (!scopeStack.empty()) {
    VarFrame* frame = scopeStack.back().get();
    frame->throws = true;
  }
}


bool VarScopeVisitor::enter(const Identifier* ast, RV& rv) {
  return true;
}
void VarScopeVisitor::exit(const Identifier* ast, RV& rv) {
  if (!scopeStack.empty()) {
    ID toId;
    if (rv.hasAst(ast)) {
      toId = rv.byAst(ast).toId();
    }
    if (!toId.isEmpty()) {
      handleMention(ast, toId, rv);
    }
  }
}

bool VarScopeVisitor::enter(const AstNode* ast, RV& rv) {
  enterScope(ast);

  return true;
}
void VarScopeVisitor::exit(const AstNode* ast, RV& rv) {
  exitScope(ast);
}


static IntentList normalizeFormalIntent(IntentList intent) {
  switch (intent) {
    case IntentList::OUT:
      return IntentList::OUT;

    case IntentList::IN:
    case IntentList::CONST_IN:
      return IntentList::IN;

    case IntentList::INOUT:
      return IntentList::INOUT;

    default:
      return IntentList::UNKNOWN;
  }
}

void
computeActualFormalIntents(Context* context,
                           const MostSpecificCandidates& candidates,
                           const CallInfo& ci,
                           const std::vector<const AstNode*>& actualAsts,
                           std::vector<IntentList>& actualFormalIntents,
                           std::vector<QualifiedType>& actualFormalTypes) {

  int nActuals = ci.numActuals();
  actualFormalIntents.clear();
  actualFormalIntents.resize(nActuals);
  actualFormalTypes.clear();
  actualFormalTypes.resize(nActuals);

  int nFns = candidates.numBest();
  if (nFns == 0) {
    // return early if there are no actual candidates
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
          if (intent != IntentList::UNKNOWN) {
            aft = fa->formalType();
          }
        } else {
          // check that the intent and types match
          if (actualFormalIntents[actualIdx] != intent) {
            // TODO: fix this error once return intent overloading implemented.
            context->error(actualAsts[actualIdx],
                  "return intent overloading but intent does not match");
            actualFormalIntents[actualIdx] = IntentList::UNKNOWN;
          } else if (intent != IntentList::UNKNOWN && aft != fa->formalType()) {
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
