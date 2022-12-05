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

  void recordInitializationOrder(VarFrame* frame, ID varId);
  void checkUseOfDeinited(const AstNode* useAst, ID varId);
  void processDeinitsAndPropagate(VarFrame* frame, VarFrame* parent);

  void resolveDefaultInit(const VarLikeDecl* ast, RV& rv);
  void resolveAssign(const AstNode* ast,
                     const QualifiedType& lhsType,
                     const QualifiedType& rhsType,
                     RV& rv);
  void resolveCopyInit(const AstNode* ast,
                       const QualifiedType& lhsType,
                       const QualifiedType& rhsType,
                       RV& rv);
  void resolveMoveInit(const AstNode* ast,
                       const QualifiedType& lhsType,
                       const QualifiedType& rhsType,
                       RV& rv);
  void resolveDeinit(const AstNode* ast,
                     const QualifiedType& type,
                     RV& rv);


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

// When varId is initialized, record that fact in the
// localsAndDefers/initedOuterVars vectors.
// Does not update any sets
void CallInitDeinit::recordInitializationOrder(VarFrame* frame, ID varId) {
  assert(!varId.isEmpty()); // caller error
  if (frame->declaredVars.count(varId) > 0) {
    // it is declared in the local scope
    frame->localsAndDefers.push_back(varId);
  } else {
    // it is declared in an outer scope
    frame->initedOuterVars.push_back(varId);
  }
}

void CallInitDeinit::checkUseOfDeinited(const AstNode* useAst, ID varId) {
  // check that the variable is not dead
  ssize_t n = scopeStack.size();
  for (ssize_t i = n - 1; i >= 0; i--) {
    VarFrame* frame = scopeStack[i].get();
    if (frame->deinitedVars.count(varId) > 0) {
      // TODO: fix this error
      context->error(useAst, "use of dead / already deinited variable");
      break;
    }
  }
}

void CallInitDeinit::processDeinitsAndPropagate(VarFrame* frame,
                                                VarFrame* parent) {
  ssize_t n = frame->localsAndDefers.size();
  for (ssize_t i = n - 1; i >= 0; i--) {
    ID varOrDeferId = frame->localsAndDefers[i]; 
    // TODO find the uAST node to associate the deinits & call it here
    // It could just be the last child node, since there must
    // be more than one if we are deiniting something, right?
    printf("DEINIT %s\n", varOrDeferId.str().c_str());
    // TODO: actually call it here
  }

  if (parent != nullptr) {
    for (auto id : frame->initedOuterVars) {
      if (parent->addToInitedVars(id)) {
        recordInitializationOrder(parent, id);
      }
    }
    for (auto id : frame->deinitedVars) {
      if (parent->declaredVars.count(id) == 0) {
        parent->deinitedVars.insert(id);
      }
    }
  }
}

void CallInitDeinit::resolveDefaultInit(const VarLikeDecl* ast, RV& rv) {

  /*auto formal = ast->toFormal();
  if (formal != nullptr && formal->intent() != Formal::OUT) {
    // don't try to default init formal values unless they are 'out'.
    return;
  }*/

  ResolvedExpression& varRes = rv.byAst(ast);
  QualifiedType varType = varRes.type();

  if (varType.isErroneousType()) {
    // ignore it -- should have already issued an error
    return;
  }
  if (varType.isUnknownKindOrType()) {
    context->error(ast, "cannot default initialize variable using generic or unknown type");
    return;
  }
  // check genericity
  auto g = getTypeGenericity(context, varType);
  if (g != Type::CONCRETE) {
    context->error(ast, "cannot default initialize variable using generic or unknown type");
    return;
  }

  // these will be set below
  const ClassType* classType = nullptr;
  const CompositeType* compositeType = nullptr;

  const Type* vt = varType.type();

  if (vt->isPrimitiveType() || vt->isBuiltinType() || vt->isCStringType() ||
      vt->isNilType() || vt->isNothingType() || vt->isVoidType()) {
    // OK, we can always default initialize primitive numeric types,
    // and as well we assume that for the non-generic builtin types
    // e.g. TaskIdType.
    // No need to resolve anything additional now.
    return;
  } else if (vt->isEnumType()) {
    // OK, can default-initialize enums to first element
    return;
  } else if (vt->isFunctionType()) {
    // TODO: any action needed here?
    return;
  } else if (vt->isTupleType()) {
    // TODO: probably need to do something here, at least in some cases
  } else if (auto ct = vt->toClassType()) {
    auto decorator = ct->decorator();
    // check that the class is a nilable class type
    if (!decorator.isNilable()) {
      // TODO: improve this error
      context->error(ast, "cannot default initialize variable using non-nilable class type");
      return;
    }
    // no action needed for 'borrowed' or 'unmanaged'
    // (these should just default initialized to 'nil',
    //  so nothing else needs to be resolved)
    if (decorator.isBorrowed() || decorator.isUnmanaged()) {
      return;
    }

    // otherwise, need to resolve an 'init' e.g. shared.init
    classType = ct;
    compositeType = ct->manager()->toCompositeType();
  } else if (auto ct = vt->toCompositeType()) {
    compositeType = ct;
  }

  if (compositeType != nullptr) {
    // try to resolve 'init'
    // TODO: handle instantiations passing field types
    std::vector<CallInfoActual> actuals;
    actuals.push_back(CallInfoActual(varType, USTR("this")));
    if (classType != nullptr && classType->manager() != nullptr) {
      // always pass chpl_t=borrowed class type
      auto dec = classType->decorator().toBorrowed();
      auto t = ClassType::get(context,
                              classType->basicClassType(),
                              /* manager */ nullptr,
                              dec);
      auto chpl_t = UniqueString::get(context, "chpl_t");
      actuals.push_back(
          CallInfoActual(QualifiedType(QualifiedType::TYPE, t), chpl_t));
    } else if (compositeType != nullptr &&
               compositeType->instantiatedFromCompositeType() != nullptr) {
      // pass generic type and param fields by the name
      auto subs = compositeType->sortedSubstitutions();
      for (auto pair : subs) {
        const ID& id = pair.first;
        const QualifiedType& qt = pair.second;
        UniqueString fname = parsing::fieldIdToName(context, id);
        actuals.push_back(CallInfoActual(qt, fname));
      }
    }
    auto ci = CallInfo (/* name */ USTR("init"),
                        /* calledType */ QualifiedType(),
                        /* isMethodCall */ true,
                        /* hasQuestionArg */ false,
                        /* isParenless */ false,
                        std::move(actuals));
    const Scope* scope = scopeForId(context, ast->id());
    auto c = resolveGeneratedCall(context, ast, ci, scope,
                                  resolver.poiScope);
    ResolvedExpression& opR = rv.byAst(ast);
    resolver.handleResolvedAssociatedCall(opR, ast, ci, c);
  }
}

void CallInitDeinit::resolveAssign(const AstNode* ast,
                                   const QualifiedType& lhsType,
                                   const QualifiedType& rhsType,
                                   RV& rv) {
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
}

void CallInitDeinit::resolveCopyInit(const AstNode* ast,
                                     const QualifiedType& lhsType,
                                     const QualifiedType& rhsType,
                                     RV& rv) {
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
}

static bool isValue(QualifiedType::Kind kind) {
  return (kind == QualifiedType::VAR ||
          kind == QualifiedType::CONST_VAR ||
          kind == QualifiedType::IN ||
          kind == QualifiedType::CONST_IN ||
          kind == QualifiedType::OUT ||
          kind == QualifiedType::INOUT);
}
static bool isRef(QualifiedType::Kind kind) {
  return (kind == QualifiedType::CONST_REF ||
          kind == QualifiedType::REF);
}
static bool isTypeParam(QualifiedType::Kind kind) {
  return (kind == QualifiedType::PARAM ||
          kind == QualifiedType::TYPE);
}

void CallInitDeinit::resolveMoveInit(const AstNode* ast,
                                     const QualifiedType& lhsType,
                                     const QualifiedType& rhsType,
                                     RV& rv) {
  if (isTypeParam(lhsType.kind())) {
    // OK, nothing else to do
  } else if (isValue(lhsType.kind()) && isValue(rhsType.kind())) {
    if (lhsType.type() == rhsType.type()) {
      // Future TODO: might need to call something provided by the record
      // author to be a hook for move initialization across locales
      // (see issue #15676).

      // Otherwise, no need to resolve anything else.
    } else {
      // resolve a copy init and a deinit to deinit the temporary
      if (lhsType.isGenericOrUnknown() || rhsType.isGenericOrUnknown()) {
        // TODO: this should not happen
        printf("Warning: should not be reached\n");
      } else {
        resolveCopyInit(ast, lhsType, rhsType, rv);
        resolveDeinit(ast, rhsType, rv);
      }
    }
  } else {
    assert(false && "TODO"); // e.g. value = copy init from ref
  }
}


void CallInitDeinit::resolveDeinit(const AstNode* ast,
                                   const QualifiedType& type,
                                   RV& rv) {
  std::vector<CallInfoActual> actuals;
  actuals.push_back(CallInfoActual(type, USTR("this")));
  auto ci = CallInfo (/* name */ USTR("deinit"),
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
}

void CallInitDeinit::handleDeclaration(const VarLikeDecl* ast, RV& rv) {
  VarFrame* frame = currentFrame();

  auto formal = ast->toFormal();
  if (formal != nullptr && formal->intent() != Formal::OUT) {
    // don't try to default init formal values unless they are 'out'.
  } else if (auto vaf = ast->toVarArgFormal()) {
    // don't try to default init var-arg formals
    if (vaf->intent() == Formal::OUT) {
      // at least it's not supported right now...
      context->error(vaf, "cannot have out intent varargs");
    }
  } else {
    bool inited = processDeclarationInit(ast, rv);
    bool splitInited = (splitInitedVars.count(ast->id()) > 0);

    if (!inited && !splitInited) {
      // not inited here and not split-inited, so default-initialize it
      resolveDefaultInit(ast, rv);
      ID id = ast->id();
      frame->initedVars.insert(id);
      frame->localsAndDefers.push_back(id);
    }
  }

  // record it in declaredVars
  frame->addToDeclaredVars(ast->id());
}

void CallInitDeinit::handleMention(const Identifier* ast, ID varId, RV& rv) {
  // check that the variable is not dead
  checkUseOfDeinited(ast, varId);
}

void CallInitDeinit::handleAssign(const OpCall* ast, RV& rv) {
  VarFrame* frame = currentFrame();
  // What is the RHS and LHS of the '=' call?
  auto lhsAst = ast->actual(0);
  auto rhsAst = ast->actual(1);

  ResolvedExpression& lhsRe = rv.byAst(lhsAst);
  QualifiedType lhsType = lhsRe.type();

  ResolvedExpression& rhsRe = rv.byAst(rhsAst);
  QualifiedType rhsType = rhsRe.type();


  // update initedVars if it is initializing a variable
  bool splitInited = processSplitInitAssign(ast, splitInitedVars, rv);

  if (splitInited) {
    // if initializating a variable, update localsAndDefers or initedOuterVars
    ID lhsId = refersToId(lhsAst, rv);
    recordInitializationOrder(frame, lhsId);
  }

  // check for use of deinited variables
  processMentions(ast, rv);

  if (lhsType.isType() || lhsType.isParam()) {
    // these are basically 'move' initialization
    resolveMoveInit(ast, lhsType, rhsType, rv);
  } else if (splitInited) {
    if (isRef(lhsType.kind())) {
      // e.g. ref x = returnAValue();
      assert(false && "TODO");
    } else if (elidedCopyFromIds.count(ast->id()) > 0) {
      // it is move initialization
      resolveMoveInit(ast, lhsType, rhsType, rv);

      // The RHS must represent a variable that is now dead,
      // so note that in deinitedVars.
      ID rhsId = refersToId(rhsAst, rv);
      // copy elision with '=' should only apply to myVar = myOtherVar
      assert(!rhsId.isEmpty());
      frame->deinitedVars.insert(rhsId);
    } else {
      // it is copy initialization, so use init= for records
      // TODO: and tuples?
      if (lhsType.type() != nullptr && lhsType.type()->isRecordType()) {
        resolveCopyInit(ast, lhsType, rhsType, rv);
      } else {
        resolveAssign(ast, lhsType, rhsType, rv);
      }
    }
  } else {
    // it is assignment, so resolve the '=' call
    resolveAssign(ast, lhsType, rhsType, rv);
  }
}
void CallInitDeinit::handleOutFormal(const FnCall* ast,
                                     const AstNode* actual,
                                     const QualifiedType& formalType,
                                     RV& rv) {
  VarFrame* frame = currentFrame();

  // check for use of deinited variables
  processMentions(actual, rv);

  // compute the actual type
  QualifiedType actualType = rv.byAst(actual).type();

  // update initedVars if it is initializing a variable
  bool splitInited = processSplitInitOut(ast, actual, splitInitedVars, rv);
  if (splitInited && isValue(actualType.kind())) {
    // if initializating a variable, update localsAndDefers or initedOuterVars
    ID actualId = refersToId(actual, rv);
    recordInitializationOrder(frame, actualId);

    // we can skip the copy if the types match
    resolveMoveInit(actual, actualType, formalType, rv);
  } else {
    // not initializing a variable, so just resolve the '=' call
    resolveAssign(actual, actualType, formalType, rv);
  }
}
void CallInitDeinit::handleInFormal(const FnCall* ast, const AstNode* actual,
                                    const QualifiedType& formalType,
                                    RV& rv) {
  VarFrame* frame = currentFrame();

  // check for use of deinited variables
  processMentions(actual, rv);

  // compute the actual type
  QualifiedType actualType = rv.byAst(actual).type();

  // is the copy for 'in' elided?
  if (elidedCopyFromIds.count(ast->id()) > 0 && isValue(actualType.kind())) {
    // it is move initialization
    resolveMoveInit(actual, formalType, actualType, rv);

    // The RHS must represent a variable that is now dead,
    // so note that in deinitedVars.
    ID actualId = refersToId(actual, rv);
    // copy elision should only apply to copies from variables
    assert(!actualId.isEmpty());
    frame->deinitedVars.insert(actualId);
  }
}

void CallInitDeinit::handleInoutFormal(const FnCall* ast,
                                         const AstNode* actual,
                                         const QualifiedType& formalType,
                                         RV& rv) {
  // check for use of deinited variables
  processMentions(actual, rv);

  // compute the actual type
  QualifiedType actualType = rv.byAst(actual).type();

  // resolve '=' for storing and writeback
  resolveAssign(actual, actualType, formalType, rv);
}

void CallInitDeinit::handleReturnOrThrow(const uast::AstNode* ast, RV& rv) {
  // check for use of deinited variables
  processMentions(ast, rv);
}
void CallInitDeinit::handleConditional(const Conditional* cond) {
  // Any outer variables inited in the 'then' frame can be propagated up
  VarFrame* frame = currentFrame();
  VarFrame* parent = currentParentFrame();
  VarFrame* thenFrame = currentThenFrame();
  VarFrame* elseFrame = currentElseFrame();

  // process end-of-block deinits in then/else blocks and then propagate
  processDeinitsAndPropagate(thenFrame, frame);
  if (elseFrame) {
    processDeinitsAndPropagate(elseFrame, frame);
  }

  // propagate information out of Conditional itself
  processDeinitsAndPropagate(frame, parent);
}
void CallInitDeinit::handleTry(const Try* t) {
  VarFrame* frame = currentFrame();
  VarFrame* parent = currentParentFrame();

  int nCatch = currentNumCatchFrames();
  for (int i = 0; i < nCatch; i++) {
    VarFrame* catchFrame = currentCatchFrame(i);
    processDeinitsAndPropagate(catchFrame, frame);
  }

  // propagate information out of the Try itself
  processDeinitsAndPropagate(frame, parent);
}
void CallInitDeinit::handleScope(const AstNode* ast) {
  VarFrame* frame = currentFrame();
  VarFrame* parent = currentParentFrame();
  processDeinitsAndPropagate(frame, parent);
}

void callInitDeinit(Resolver& resolver) {
  std::set<ID> splitInitedVars = computeSplitInits(resolver.context,
                                                   resolver.symbol,
                                                   resolver.byPostorder);
  std::set<ID> elidedCopyFromIds = computeElidedCopies(resolver.context,
                                                       resolver.symbol,
                                                       resolver.byPostorder,
                                                       splitInitedVars);

  auto symName = UniqueString::get(resolver.context, "unknown");
  if (auto nd = resolver.symbol->toNamedDecl()) {
    symName = nd->name();
  }

  printf("\nSplit Init Report for '%s':\n", symName.c_str());
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

  printf("\nCopy Elision Report for '%s':\n", symName.c_str());
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
