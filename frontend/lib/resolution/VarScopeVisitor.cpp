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
  ResolutionContext rcval(context);
  MutatingResolvedVisitor<VarScopeVisitor> rv(&rcval,
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
  } else if (auto mod = symbol->toModule()) {
    // Process module initialization code, similarly to a function body

    enterScope(mod, rv);

    for (auto child : mod->children()) {
      // Skip functions and nested modules as they are handled elsewhere
      if (!(child->isFunction() || child->isModule())) {
        child->traverse(rv);
      }
    }

    exitScope(mod, rv);
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
  auto frame = currentFrame();
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
  auto frame = currentFrame();
  ID actualVarId = refersToId(actual, rv);
  if (!actualVarId.isEmpty() && allSplitInitedVars.count(actualVarId) > 0) {
    inserted = frame->addToInitedVars(actualVarId);
  }
  return inserted;
}

bool VarScopeVisitor::processDeclarationInit(const VarLikeDecl* ast, RV& rv) {
  auto frame = currentFrame();
  bool inserted = false;
  if (ast->initExpression() != nullptr) {
    inserted = frame->addToInitedVars(ast->id());
  }
  return inserted;
}

const QualifiedType& VarScopeVisitor::returnOrYieldType() {
  return fnReturnType;
}

void VarScopeVisitor::doEnterScope(const uast::AstNode* ast, RV& rv) {
  FlowSensitiveVisitor::doEnterScope(ast, rv);
}

void VarScopeVisitor::doExitScope(const uast::AstNode* ast, RV& rv) {
  FlowSensitiveVisitor::doExitScope(ast, rv);

  // we moved the frame into the parent frame's subBlocks
  if (scopeStack.back() == nullptr) return;

  switch (ast->tag()) {
    case chpl::uast::asttags::Conditional:
      handleConditional(ast->toConditional(), rv);
      break;
    case chpl::uast::asttags::Try:
      handleTry(ast->toTry(), rv);
      break;
    case chpl::uast::asttags::Select:
      handleSelect(ast->toSelect(), rv);
      break;
    default:
      handleScope(ast, rv);
      break;
  }
}

static const types::Param* determineParamValue(const ResolvedExpression& rr) {
  if (rr.type().kind() == QualifiedType::PARAM) return rr.type().param();
  return nullptr;
}

const types::Param* VarScopeVisitor::determineWhenCaseValue(const uast::AstNode* ast, RV& extraData) {
  return determineParamValue(extraData.byAst(ast));
}

const types::Param* VarScopeVisitor::determineIfValue(const uast::AstNode* ast, RV& extraData) {
  return determineParamValue(extraData.byAst(ast));
}

void VarScopeVisitor::traverseNode(const uast::AstNode* ast, RV& rv) {
  ast->traverse(rv);
}

void VarScopeVisitor::handleConditional(const Conditional* cond, RV& rv) {
  auto frame = currentFrame();
  auto thenFrame = currentThenFrame();
  auto elseFrame = currentElseFrame();

  std::vector<VarFrame*> frames;
  if (thenFrame) frames.push_back(thenFrame);
  if (elseFrame) frames.push_back(elseFrame);
  handleDisjunction(cond, frame, frames, elseFrame != nullptr, rv);
  handleScope(cond, rv);
}

void VarScopeVisitor::handleSelect(const Select* sel, RV& rv) {
  auto frame = currentFrame();

  std::vector<VarFrame*> frames;
  bool total = sel->hasOtherwise();
  for(int i = 0; i < sel->numWhenStmts(); i++) {
    auto whenFrame = currentWhenFrame(i);
    if (!whenFrame) continue;
    frames.push_back(whenFrame);
    total |= whenFrame->paramTrueCond;
  }
  handleDisjunction(sel, frame, frames, total, rv);
  handleScope(sel, rv);
}

void VarScopeVisitor::enterAst(const uast::AstNode* ast) {
  inAstStack.push_back(ast);
}
void VarScopeVisitor::exitAst(const uast::AstNode* ast) {
  CHPL_ASSERT(!inAstStack.empty() && ast == inAstStack.back());
  inAstStack.pop_back();
}

bool VarScopeVisitor::enter(const TupleDecl* ast, RV& rv) {
  enterAst(ast);
  enterScope(ast, rv);

  // TODO: handle tuple decls
  return false;
}
void VarScopeVisitor::exit(const TupleDecl* ast, RV& rv) {
  exitScope(ast, rv);
  exitAst(ast);

  return;
}

bool VarScopeVisitor::enter(const NamedDecl* ast, RV& rv) {

  if (ast->id().isSymbolDefiningScope()) {
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
  if (ast->id().isSymbolDefiningScope()) {
    // It's a symbol with a different path, e.g. a Function.
    // Don't try to resolve it now in this
    // traversal. Instead, resolve it e.g. when the function is called.
    return;
  }

  // Loop index variables don't need default-initialization and aren't
  // subject to split init etc., so skip them.
  //
  // TODO: I think it's fine to skip this for all users of VarScopeVisitor;
  //       is there an analysis that does need to handle loop indices?
  bool skipDecl = false;
  if (inAstStack.size() > 1) {
    auto parentAst = inAstStack[inAstStack.size() - 2];
    if (auto indexableLoop = parentAst->toIndexableLoop()) {
      if (ast == indexableLoop->index()) {
        skipDecl = true;
      }
    }
  }

  CHPL_ASSERT(!scopeStack.empty());
  if (!scopeStack.empty() && !skipDecl) {
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
    bool isMethod = false;
    for (const MostSpecificCandidate& candidate : candidates) {
      if (candidate) {
        auto fn = candidate.fn();
        if (fn->untyped()->isMethod()) isMethod = true;

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
      //
      // TODO: Should we store the resolved CallInfo so we don't need to build
      // it back up here?
      std::vector<const AstNode*> actualAsts;
      auto ci = CallInfo::create(context, callAst, rv.byPostorder(),
                                 /* raiseErrors */ false,
                                 &actualAsts);

      if (isMethod && ci.isMethodCall() == false) {
        // Create a dummy 'this' actual
        ci = ci.createWithReceiver(ci, QualifiedType());
        actualAsts.insert(actualAsts.begin(), nullptr);
      }

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
        if (actualAst == nullptr) {
          CHPL_ASSERT(ci.isMethodCall() && actualIdx == 0);
        } else if (kind == Qualifier::OUT) {
          handleOutFormal(callAst, actualAst,
                          actualFormalTypes[actualIdx], rv);
        } else if ((kind == Qualifier::IN || kind == Qualifier::CONST_IN) &&
                   !(ci.name() == "init" && actualIdx == 0)) {
          // don't do this for the 'this' argument to 'init', because it
          // is not getting copied.
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
  return flowSensitivelyTraverse(cond, rv);
}

void VarScopeVisitor::exit(const Conditional* cond, RV& rv) {
  exitScope(cond, rv);
  exitAst(cond);
}

bool VarScopeVisitor::enter(const Select* sel, RV& rv) {
  enterAst(sel);
  enterScope(sel, rv);
  return flowSensitivelyTraverse(sel, rv);
}

void VarScopeVisitor::exit(const Select* ast, RV& rv) {
  exitScope(ast, rv);
  exitAst(ast);
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
  for (const MostSpecificCandidate& candidate : candidates) {
    if (candidate) {
      auto& formalActualMap = candidate.formalActualMap();
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
