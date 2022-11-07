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

#include "call-init-deinit.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/can-pass.h"
#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/resolution/copy-elision.h"
#include "chpl/resolution/split-init.h"
#include "chpl/uast/all-uast.h"

#include "Resolver.h"
#include "VarScopeVisitor.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;


// TODO -- figure out where to store copy (associatedFns?)
//         and where to store deinit (associatedFns not so good).
//         For now it just prints these.
//
// TODO -- a default argument can have a RHS that is a reference
//         even though it is 'in' intent. As such, it would require
//         a copy, but it's hard to associate that information
//         with a call actual (because the actual doesn't exist).

/*
struct Action {
  enum ActionKind {
    COPY_INIT, // for 'in'
    WRITE_BACK, // for 'out' and 'inout'
    DEINIT,
  };
  ActionKind action; // which action?
  ID id;             // which ID?
  Action(ActionKind action, ID id) : action(action), id(id) { }
};
*/

// Resolves default-init, copy-init, assign, and deinit
// TODO: should it be renamed to include Assign?
struct CallInitDeinit : VarScopeVisitor {
  // inputs to the process
  Resolver& resolver;
  const std::set<ID>& splitInitedVars;
  const std::set<ID>& elidedCopyFromIds;

  // methods
  CallInitDeinit(Context* context,
                 Resolver& resolver,
                 const std::set<ID>& splitInitedVars,
                 const std::set<ID>& elidedCopyFromIds)
    : VarScopeVisitor(context),
      resolver(resolver),
      splitInitedVars(splitInitedVars),
      elidedCopyFromIds(elidedCopyFromIds)
  { }

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
  void handleReturnOrThrow(const uast::AstNode* ast, RV& rv) override;
  void handleConditional(const Conditional* cond) override;
  void handleTry(const Try* t) override;
  void handleScope(const AstNode* ast) override;
};

/*
void CallInitDeinit::printActions(const std::vector<Action>& actions) {
  for (auto act : actions) {
    switch (act.action) {
      case Action::COPY_INIT:
        printf("copy-init %s\n", act.id.str().c_str());
        break;
      case Action::WRITE_BACK:
        printf("writeback %s\n", act.id.str().c_str());
        break;
      case Action::DEINIT:
        printf("deinit %s\n", act.id.str().c_str());
        break;
    }
  }
}*/

void CallInitDeinit::handleDeclaration(const VarLikeDecl* ast, RV& rv) {
  processDeclarationInit(ast, rv);
}
void CallInitDeinit::handleMention(const Identifier* ast, ID varId, RV& rv) {
  // TODO
}
void CallInitDeinit::handleAssign(const OpCall* ast, RV& rv) {
  // What is the RHS and LHS of the '=' call?
  auto lhsAst = ast->actual(0);
  auto rhsAst = ast->actual(1);

  ResolvedExpression& lhsRe = rv.byAst(lhsAst);
  QualifiedType lhsType = lhsRe.type();

  ResolvedExpression& rhsRe = rv.byAst(rhsAst);
  QualifiedType rhsType = rhsRe.type();

  bool resolveAssign = false;
  bool resolveInitAssign = false;

  // update initedVars if it is initializing a variable
  bool splitInited = processSplitInitAssign(ast, splitInitedVars, rv);

  if (lhsType.isType() || lhsType.isParam()) {
    // these are basically 'move' initialization
  } else if (splitInited) {
    if (elidedCopyFromIds.count(ast->id())) {
      // it is move initialization

      // Future TODO: might need to call something provided by the record
      // author to be a hook for move initialization across locales
      // (see issue #15676).

      // Otherwise, no need to resolve anything else.
    } else {
      // it is copy initialization, so use init= for records
      // TODO: and tuples?
      if (lhsType.type() != nullptr && lhsType.type()->isRecordType()) {
        resolveInitAssign = true;
      } else {
        resolveAssign = true;
      }
    }
  } else {
    // it is assignment, so resolve the '=' call
    resolveAssign = true;
  }

  if (resolveAssign) {
    std::vector<CallInfoActual> actuals;
    actuals.push_back(CallInfoActual(lhsType, UniqueString()));
    actuals.push_back(CallInfoActual(rhsType, UniqueString()));
    auto ci = CallInfo (/* name */ USTR("="),
                        /* calledType */ QualifiedType(),
                        /* isMethodCall */ false,
                        /* hasQuestionArg */ false,
                        /* isParenless */ false,
                        actuals);
    const Scope* scope = scopeForId(context, ast->id());
    auto c = resolveGeneratedCall(context, ast, ci, scope,
                                  resolver.poiScope);
    ResolvedExpression& opR = rv.byAst(ast);
    resolver.handleResolvedAssociatedCall(opR, ast, ci, c);
  } else if (resolveInitAssign) {
    std::vector<CallInfoActual> actuals;
    actuals.push_back(CallInfoActual(lhsType, USTR("this")));
    actuals.push_back(CallInfoActual(rhsType, UniqueString()));
    auto ci = CallInfo (/* name */ USTR("init="),
                        /* calledType */ QualifiedType(),
                        /* isMethodCall */ true,
                        /* hasQuestionArg */ false,
                        /* isParenless */ false,
                        actuals);
    const Scope* scope = scopeForId(context, ast->id());
    auto c = resolveGeneratedCall(context, ast, ci, scope,
                                  resolver.poiScope);
    ResolvedExpression& opR = rv.byAst(ast);
    resolver.handleResolvedAssociatedCall(opR, ast, ci, c);
  } else {
    // if it's move initialization, check that the types are compatable
    if (!canPass(context, rhsType, lhsType).passes()) {
      context->error(ast, "types not compatable for move-init");
    }
  }
}
void CallInitDeinit::handleOutFormal(const FnCall* ast,
                                     const AstNode* actual,
                                     const QualifiedType& formalType,
                                     RV& rv) {
  // update initedVars if it is initializing a variable
  processSplitInitOut(ast, actual, splitInitedVars, rv);
}
void CallInitDeinit::handleInFormal(const FnCall* ast, const AstNode* actual,
                                      const QualifiedType& formalType,
                                      RV& rv) {
  // TODO
}
void CallInitDeinit::handleInoutFormal(const FnCall* ast,
                                         const AstNode* actual,
                                         const QualifiedType& formalType,
                                         RV& rv) {
  // TODO
}
void CallInitDeinit::handleReturnOrThrow(const uast::AstNode* ast, RV& rv) {
  // TODO
}
void CallInitDeinit::handleConditional(const Conditional* cond) {
  // TODO
}
void CallInitDeinit::handleTry(const Try* t) {
  // TODO
}
void CallInitDeinit::handleScope(const AstNode* ast) {
  VarFrame* frame = currentFrame();
  VarFrame* parent = currentParentFrame();

  // propagate inited vars
  if (parent != nullptr) {
    parent->initedVars.insert(frame->initedVars.begin(),
                              frame->initedVars.end());
  }
}

void callInitDeinit(Resolver& resolver) {
  std::set<ID> splitInitedVars = computeSplitInits(resolver.context,
                                                   resolver.symbol,
                                                   resolver.byPostorder);
  std::set<ID> elidedCopyFromIds = computeElidedCopies(resolver.context,
                                                       resolver.symbol,
                                                       resolver.byPostorder,
                                                       splitInitedVars);

  printf("\nSplit Init Report:\n");
  for (auto varId : splitInitedVars) {
    auto ast = parsing::idToAst(resolver.context, varId);
    if (ast) {
      if (auto vd = ast->toVarLikeDecl()) {
        printf("  Split initing '%s' with ID %s\n",
               vd->name().c_str(), vd->id().str().c_str());
      }
    }
  }
  printf("\n");

  printf("\nCopy Elision Report:\n");
  for (auto id : elidedCopyFromIds) {
    printf("  Copy eliding ID %s\n",
           id.str().c_str());
  }
  printf("\n");


  CallInitDeinit uv(resolver.context, resolver,
                    splitInitedVars, elidedCopyFromIds);
  uv.process(resolver.symbol, resolver.byPostorder);
}


} // end namespace resolution
} // end namespace chpl
