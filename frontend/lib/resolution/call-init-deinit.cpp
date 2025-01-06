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


// TODO -- a default argument can have a RHS that is a reference
//         even though it is 'in' intent. As such, it would require
//         a copy, but it's hard to associate that information
//         with a call actual (because the actual doesn't exist).
// TODO -- and, what would happen if 'init=' had default arguments?


// Resolves default-init, copy-init, assign, and deinit
// TODO: should it be renamed to include Assign?
struct CallInitDeinit : VarScopeVisitor {
  // inputs to the process
  Resolver& resolver;
  const std::set<ID>& splitInitedVars;
  const std::set<ID>& elidedCopyFromIds;

  // local state
  std::set<ID> outOrInoutFormals;

  // methods
  CallInitDeinit(Context* context,
                 Resolver& resolver,
                 const std::set<ID>& splitInitedVars,
                 const std::set<ID>& elidedCopyFromIds)
    : VarScopeVisitor(context, resolver.returnType),
      resolver(resolver),
      splitInitedVars(splitInitedVars),
      elidedCopyFromIds(elidedCopyFromIds)
  { }

  void analyzeReturnedExpr(ResolvedExpression& re,
                           const AstNode* returnOrYield,
                           bool& needsCopyOrConv,
                           bool& copyElidesFromSkip,
                           ID& skipDeinitId,
                           RV& rv);
  bool isCallProducingValue(const AstNode* rhsAst,
                            const QualifiedType& rhsType,
                            RV& rv);

  void recordInitializationOrder(VarFrame* frame, ID varId);
  void checkUseOfDeinited(const AstNode* useAst, ID varId);
  bool isInitedAnyFrame(ID varId);
  void processDeinitsForReturn(const AstNode* atAst,
                               ID skipVarId,
                               RV& rv);
  void processDeinitsAndPropagate(VarFrame* frame, VarFrame* parent, RV& rv);

  void resolveDefaultInit(const VarLikeDecl* ast, RV& rv);
  void resolveAssign(const AstNode* ast,
                     const QualifiedType& lhsType,
                     const QualifiedType& rhsType,
                     RV& rv);
  void resolveCopyInit(const AstNode* ast,
                       const AstNode* rhsAst,
                       const QualifiedType& lhsType,
                       const QualifiedType& rhsType,
                       bool forMoveInit,
                       RV& rv);
  void resolveMoveInit(const AstNode* ast,
                       const AstNode* rhsAst,
                       const QualifiedType& lhsType,
                       const QualifiedType& rhsType,
                       RV& rv);
  void processInit(VarFrame* frame,
                   const AstNode* ast,
                   const QualifiedType& lhsType,
                   const QualifiedType& rhsType,
                   RV& rv);

  void resolveDeinit(const AstNode* ast,
                     ID deinitedId,
                     const QualifiedType& type,
                     RV& rv);


  void processReturnThrowYield(const uast::AstNode* ast, RV& rv);

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


// When varId is initialized, record that fact in the
// localsAndDefers/initedOuterVars vectors.
// Does not update any sets
void CallInitDeinit::recordInitializationOrder(VarFrame* frame, ID varId) {
  CHPL_ASSERT(!varId.isEmpty()); // caller error
  if (frame->declaredVars.count(varId) > 0) {
    // it is declared in the local scope
    frame->localsAndDefers.push_back(varId);
  } else {
    // it is declared in an outer scope
    frame->initedOuterVars.push_back(varId);
  }
}

static bool isValue(QualifiedType::Kind kind) {
  return (kind == QualifiedType::VAR ||
          kind == QualifiedType::CONST_VAR ||
          kind == QualifiedType::IN ||
          kind == QualifiedType::CONST_IN ||
          kind == QualifiedType::OUT ||
          kind == QualifiedType::INOUT ||
          kind == QualifiedType::INIT_RECEIVER);
}
static bool isValueOrParam(QualifiedType::Kind kind) {
  return isValue(kind) || kind == QualifiedType::PARAM;
}
static bool isRef(QualifiedType::Kind kind) {
  return (kind == QualifiedType::CONST_REF ||
          kind == QualifiedType::REF);
}
static bool isTypeParam(QualifiedType::Kind kind) {
  return (kind == QualifiedType::PARAM ||
          kind == QualifiedType::TYPE);
}

bool CallInitDeinit::isCallProducingValue(const AstNode* rhsAst,
                                          const QualifiedType& rhsType,
                                          RV& rv) {
  return rv.byAst(rhsAst).toId().isEmpty() && !isRef(rhsType.kind());
}

void CallInitDeinit::analyzeReturnedExpr(ResolvedExpression& re,
                                         const AstNode* returnOrYield,
                                         bool& needsCopyOrConv,
                                         bool& copyElidesFromSkip,
                                         ID& skipDeinitId,
                                         RV& rv) {
  bool fnReturnsRegularValue = false;
  if (resolver.symbol) {
    if (auto inFn = resolver.symbol->toFunction()) {
      switch (inFn->returnIntent()) {
        case Function::DEFAULT_RETURN_INTENT:
        case Function::OUT:
        case Function::CONST:
          fnReturnsRegularValue = true;
          break;
        case Function::CONST_REF:
        case Function::REF:
        case Function::PARAM:
        case Function::TYPE:
          // leave returnsByValue false
          break;
      }
    }
  }

  if (fnReturnsRegularValue) {
    ID toId = re.toId(); // what variable was returned/yielded?
    if (!toId.isEmpty()) {
      if (resolver.symbol->id().contains(toId)) { // is it a local variable?
        if (isValue(re.type().kind())) {
          if (returnOrYield->isYield()) {
            // for a yield, it depends on if the yield was copy elided
            if (elidedCopyFromIds.count(returnOrYield->id()) > 0) {
              // from last mention of a variable
              skipDeinitId = toId;
              copyElidesFromSkip = true;
            } else {
              // yield of a variable mentioned again needs to be a copy
              // (since iterator will come back and continue)
              needsCopyOrConv = true;
            }
          } else {
            // for a return of a local by value, always move to return it,
            // but no need to mark it dead since the return will end the block!
            skipDeinitId = toId;
          }
        } else {
          // returning a local reference by value
          needsCopyOrConv = true;
        }
      } else {
        // returning e.g. a module-scope variable by value
        needsCopyOrConv = true;
      }
    } else {
      // it wasn't a simple variable
      // consider the type of the returned expression.
      auto kind = re.type().kind();
      if (isValue(kind)) {
        // no action required to return a value expression by value
        // e.g. return makeSomeRecord();
      } else if (isRef(kind)) {
        // need to copy if we are returning a reference by value
        // e.g. return someReference;
        needsCopyOrConv = true;
      } else {
        // do nothing for other types (PARAM, TYPE, FUNCTION, etc)
      }
    }
  }
}

void CallInitDeinit::checkUseOfDeinited(const AstNode* useAst, ID varId) {
  // check that the variable is not dead
  ssize_t n = scopeStack.size();
  for (ssize_t i = n - 1; i >= 0; i--) {
    VarFrame* frame = scopeStack[i].get();
    if (frame->deinitedVars.count(varId) > 0) {
      // For vars dead after a call, don't error for uses within that call.
      if (frame->deinitedVars[varId] ==
          parsing::idToParentId(context, useAst->id())) {
        continue;
      }

      // TODO: fix this error
      context->error(useAst, "use of dead / already deinited variable");
      break;
    }
  }
}

bool CallInitDeinit::isInitedAnyFrame(ID varId) {
  ssize_t n = scopeStack.size();
  for (ssize_t i = n - 1; i >= 0; i--) {
    VarFrame* frame = scopeStack[i].get();
    if (frame->initedVars.count(varId) > 0) {
      return true;
    }
  }
  return false;
}


void CallInitDeinit::processDeinitsForReturn(const AstNode* atAst,
                                             ID skipVarId,
                                             RV& rv) {
  std::set<ID> initedAnyFrame;
  std::unordered_map<ID, ID> deinitedAnyFrame;

  ssize_t n = scopeStack.size();

  // compute initedAnyFrame and deinitedAnyFrame
  // * don't deinit if it hasn't been inited yet
  // * don't deinit if it was moved from so already deinited
  // * don't deinit out formals
  for (ssize_t i = 0; i < n; i++) {
    VarFrame* frame = scopeStack[i].get();
    initedAnyFrame.insert(frame->initedVars.begin(),
                          frame->initedVars.end());
    deinitedAnyFrame.insert(frame->deinitedVars.begin(),
                            frame->deinitedVars.end());
  }

  // also count outOrInoutFormals as already deinited
  for (const auto& id : outOrInoutFormals) {
    deinitedAnyFrame.emplace(id, ID());
  }

  for (ssize_t i = n - 1; i >= 0; i--) {
    VarFrame* frame = scopeStack[i].get();
    ssize_t nv = frame->localsAndDefers.size();
    for (ssize_t j = nv - 1; j >= 0; j--) {
      ID varOrDeferId = frame->localsAndDefers[j];

      if (varOrDeferId != skipVarId &&
          initedAnyFrame.count(varOrDeferId) > 0 &&
          deinitedAnyFrame.count(varOrDeferId) == 0) {

        ResolvedExpression& re = rv.byId(varOrDeferId);
        QualifiedType type = re.type();

        // don't deinit reference variables
        if (isValue(type.kind())) {
          resolveDeinit(atAst, varOrDeferId, type, rv);
        }

        deinitedAnyFrame.emplace(varOrDeferId, ID());
      }
    }
  }
}

void CallInitDeinit::processDeinitsAndPropagate(VarFrame* frame,
                                                VarFrame* parent,
                                                RV& rv) {
  ssize_t n = frame->localsAndDefers.size();
  for (ssize_t i = n - 1; i >= 0; i--) {
    ID varOrDeferId = frame->localsAndDefers[i];
    if (outOrInoutFormals.count(varOrDeferId) > 0) {
      // don't deinit out formals
    } else if (frame->deinitedVars.count(varOrDeferId) > 0) {
      // don't deinit it if it was already destroyed by moving from it
    } else {
      ResolvedExpression& re = rv.byId(varOrDeferId);
      QualifiedType type = re.type();
      // don't deinit reference variables
      if (isValue(type.kind())) {
        resolveDeinit(frame->scopeAst, varOrDeferId, type, rv);
      }
    }
  }

  if (parent != nullptr) {
    for (const auto& id : frame->initedOuterVars) {
      if (parent->addToInitedVars(id)) {
        recordInitializationOrder(parent, id);
      }
    }
    for (const auto& [declId, lastStmtId] : frame->deinitedVars) {
      if (frame->declaredVars.count(declId) == 0) {
        parent->deinitedVars.emplace(declId, lastStmtId);
      }
    }
  }
}

static bool isRecordLike(const Type* t) {
  if (auto ct = t->toClassType()) {
    auto decorator = ct->decorator();
    // no action needed for 'borrowed' or 'unmanaged'
    // (these should just default initialized to 'nil',
    //  so nothing else needs to be resolved)
    if (! (decorator.isBorrowed() || decorator.isUnmanaged() ||
          decorator.isUnknownManagement())) {
      return true;
    }
  } else if (t->isRecordType() || t->isUnionType()) {
    return true;
  }
  // TODO: tuples?

  return false;
}

static bool typeNeedsInitDeinitCall(const Type* t) {
  if (t == nullptr || t->isUnknownType() || t->isErroneousType()) {
    // can't do anything with these
    return false;
  } else if (t->isPrimitiveType() || t->isBuiltinType() || t->isCStringType() ||
             t->isNilType() || t->isNothingType() || t->isVoidType()) {
    // OK, we can always default initialize primitive numeric types,
    // and as well we assume that for the non-generic builtin types
    // e.g. TaskIdType.
    // No need to resolve anything additional now.
    return false;
  } else if (t->isEnumType()) {
    // OK, can default-initialize enums to first element
    return false;
  } else if (t->isFunctionType()) {
    return false;
  }

  return isRecordLike(t);
}


void CallInitDeinit::resolveDefaultInit(const VarLikeDecl* ast, RV& rv) {
  // Type variables do not need default init.
  if (ast->storageKind() == Qualifier::TYPE) return;

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

  if (auto ct = vt->toClassType()) {
    auto decorator = ct->decorator();
    // check that the class is a nilable class type
    if (!decorator.isNilable()) {
      // TODO: improve this error
      context->error(ast, "cannot default initialize variable using non-nilable class type");
      return;
    }
  }

  if (!typeNeedsInitDeinitCall(vt)) {
    // nothing to do here
  } else if (vt->isTupleType()) {
    // TODO: probably need to do something here, at least in some cases
  } else if (auto ct = vt->toClassType()) {
    // otherwise, need to resolve an 'init' e.g. shared.init
    classType = ct;
    CHPL_ASSERT(ct->manager());
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
      // when default-initializing a shared C? or owned C?,
      // call e.g. shared.init(chpl_t=borrowed C?).
      //
      // Safe to use basicClassType() here because the type would otherwise
      // be generic, but we know it's concrete.
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
      for (const auto& pair : subs) {
        const ID& id = pair.first;
        const QualifiedType& qt = pair.second;
        auto fieldAst = parsing::idToAst(context, id)->toVarLikeDecl();
        if (fieldAst->storageKind() == QualifiedType::TYPE ||
            fieldAst->storageKind() == QualifiedType::PARAM) {
          actuals.push_back(CallInfoActual(qt, fieldAst->name()));
        }
      }
    }

    // Get the 'root' instantiation
    const CompositeType* calledCT = compositeType;
    while (auto insn = calledCT->instantiatedFromCompositeType()) {
      calledCT = insn;
    }
    auto calledType = QualifiedType(QualifiedType::VAR, calledCT);

    auto ci = CallInfo (/* name */ USTR("init"),
                        /* calledType */ calledType,
                        /* isMethodCall */ true,
                        /* hasQuestionArg */ false,
                        /* isParenless */ false,
                        std::move(actuals));
    const Scope* scope = scopeForId(context, ast->id());
    auto inScopes = CallScopeInfo::forNormalCall(scope, resolver.poiScope);
    auto c = resolveGeneratedCall(context, ast, ci, inScopes);
    ResolvedExpression& opR = rv.byAst(ast);
    resolver.handleResolvedCall(opR, ast, ci, c,
                                { { AssociatedAction::DEFAULT_INIT, ast->id() } });
  }
}

void CallInitDeinit::resolveAssign(const AstNode* ast,
                                   const QualifiedType& lhsType,
                                   const QualifiedType& rhsType,
                                   RV& rv) {
  if (lhsType.isUnknown() || lhsType.isErroneousType() ||
      rhsType.isUnknown() || rhsType.isErroneousType()) {
    return;
  }

  if (auto call = ast->toOpCall()) {
    if (call->op() == "=" && call->child(0)->isTuple()) {
      // Tuple unpacking assignment, handled directly by Resolver
      // (a, b, c) = foo();
      return;
    }
  }

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
  auto inScopes = CallScopeInfo::forNormalCall(scope, resolver.poiScope);
  auto c = resolveGeneratedCall(context, ast, ci, inScopes);
  ResolvedExpression& opR = rv.byAst(ast);

  auto op = ast->toOpCall();
  if (op != nullptr && op->op() == USTR("=")) {
    // if the syntax shows a '=' call, resolve that into the assign
    resolver.handleResolvedCall(opR, ast, ci, c);
  } else {
    // otherwise, add an associated action
    resolver.handleResolvedCall(opR, ast, ci, c,
                                { { AssociatedAction::ASSIGN, ast->id() } });
  }
}

void CallInitDeinit::resolveCopyInit(const AstNode* ast,
                                     const AstNode* rhsAst,
                                     const QualifiedType& lhsType,
                                     const QualifiedType& rhsType,
                                     bool forMoveInit,
                                     RV& rv) {
  if (!typeNeedsInitDeinitCall(lhsType.type())) {
    // TODO: we could resolve it anyway
    return;
  }

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
  auto inScopes = CallScopeInfo::forNormalCall(scope, resolver.poiScope);
  auto c = resolveGeneratedCall(context, ast, ci, inScopes);

  std::vector<const AstNode*> actualAsts;
  actualAsts.push_back(ast);
  actualAsts.push_back(ast);
  std::vector<Qualifier> intents;
  std::vector<QualifiedType> formalTypes;

  computeActualFormalIntents(context, c.mostSpecific(), ci, actualAsts,
                             intents, formalTypes);

  bool formalUsesInIntent = false;
  CHPL_ASSERT(intents.size() >= 1);
  if (intents.size() >= 1 &&
      (intents[1] == Qualifier::IN || intents[1] == Qualifier::CONST_IN)) {
    formalUsesInIntent = true;
  }

  if (formalUsesInIntent && !forMoveInit &&
      formalTypes[0].type() != formalTypes[1].type()) {
    // for init= with mixed type and 'in' intent:
    // also resolve the same-type copy initializer and add a note to call that.
    // Do this before recording so that the actions are in the order they
    // would need to be taken.
    // The idea is to copy-initialize and then move that copy into the
    // mixed-type init= 'in' formal.
    resolveCopyInit(ast, rhsAst,
                    formalTypes[1], rhsType,
                    /* forMoveInit */ true,
                    rv);
  }

  ResolvedExpression& opR = rv.byAst(ast);

  auto action = AssociatedAction::COPY_INIT;
  if (lhsType.type() != rhsType.type()) {
    action = AssociatedAction::INIT_OTHER;
  }
  resolver.handleResolvedCall(opR, ast, ci, c, { { action, ast->id() } });

  // If we were trying to move, but had to run an init= to change types,
  // and that init= did not accept its argument by 'in' intent, we need
  // to deinit the temporary created.
  if (forMoveInit && !formalUsesInIntent) {
    resolveDeinit(ast, rhsAst->id(), rhsType, rv);
  }
}

void CallInitDeinit::resolveMoveInit(const AstNode* ast,
                                     const AstNode* rhsAst,
                                     const QualifiedType& lhsType,
                                     const QualifiedType& rhsType,
                                     RV& rv) {
  if (isTypeParam(lhsType.kind())) {
    // OK, nothing else to do
  } else if (isValue(lhsType.kind()) && isValueOrParam(rhsType.kind())) {
    // Accept if we can pass with only a subtype conversion
    // (for passing non-nilable to nilable).
    auto canPassResult = canPass(context, rhsType, lhsType);
    if (canPassResult.passes() &&
        (!canPassResult.converts() ||
         canPassResult.conversionKind() ==
             CanPassResult::ConversionKind::SUBTYPE)) {
      // Future TODO: might need to call something provided by the record
      // author to be a hook for move initialization across locales
      // (see issue #15676).

      // Otherwise, no need to resolve anything else.
    } else {
      bool lhsGenUnk = lhsType.isUnknown() ||
                       getTypeGenericity(context, lhsType) != Type::CONCRETE;
      bool rhsGenUnk = rhsType.isUnknown() ||
                       getTypeGenericity(context, rhsType) != Type::CONCRETE;
      // resolve a copy init and a deinit to deinit the temporary
      if (lhsGenUnk || rhsGenUnk) {
        CHPL_ASSERT(false && "should not be reached");
      } else {
        resolveCopyInit(ast, rhsAst, lhsType, rhsType,
                        /* forMoveInit */ true,
                        rv);
      }
    }
  } else {
    CHPL_UNIMPL("value = copy init from ref");
  }
}

void CallInitDeinit::processInit(VarFrame* frame,
                                 const AstNode* ast,
                                 const QualifiedType& lhsType,
                                 const QualifiedType& rhsType,
                                 RV& rv) {
  if (lhsType.type() == rhsType.type() &&
      !typeNeedsInitDeinitCall(lhsType.type())) {
    // TODO: we should resolve it anyway
    return;
  }

  // ast should be:
  //  * a '=' call
  //  * a VarLikeDecl
  //  * an actual passed by 'in' intent
  //  * a Return or Yield
  const AstNode* rhsAst = nullptr;
  auto op = ast->toOpCall();
  if (op != nullptr && op->op() == USTR("=")) {
    rhsAst = op->actual(1);
  } else if (auto vd = ast->toVarLikeDecl()) {
    rhsAst = vd->initExpression();
  } else if (auto r = ast->toReturn()) {
    rhsAst = r->value();
  } else if (auto y = ast->toYield()) {
    rhsAst = y->value();
  } else {
    rhsAst = ast;
  }

  if (lhsType.isType() || lhsType.isParam()) {
    // these are basically 'move' initialization
    resolveMoveInit(ast, rhsAst, lhsType, rhsType, rv);
  } else {
    if (isRef(lhsType.kind())) {
      // e.g. ref x = localVariable;
      //  or  ref y = returnAValue();
      if (isCallProducingValue(rhsAst, rhsType, rv)) {
        resolveDeinit(ast, rhsAst->id(), rhsType, rv);
      }
    } else if (elidedCopyFromIds.count(ast->id()) > 0) {
      // it is move initialization
      resolveMoveInit(ast, rhsAst, lhsType, rhsType, rv);

      // The RHS must represent a variable that is now dead,
      // so note that in deinitedVars.
      ID rhsDeclId = refersToId(rhsAst, rv);
      // copy elision with '=' should only apply to myVar = myOtherVar
      CHPL_ASSERT(!rhsDeclId.isEmpty());
      frame->deinitedVars.emplace(rhsDeclId, ast->id());
    } else if (isCallProducingValue(rhsAst, rhsType, rv)) {
      // e.g. var x; x = callReturningValue();
      resolveMoveInit(ast, rhsAst, lhsType, rhsType, rv);
    } else {
      // it is copy initialization, so use init= for records
      // and assign for other stuff
      if (lhsType.type() != nullptr && isRecordLike(lhsType.type())) {
        resolveCopyInit(ast, rhsAst,
                        lhsType, rhsType,
                        /* forMoveInit */ false,
                        rv);
      } else {
        resolveAssign(ast, lhsType, rhsType, rv);
      }
    }
  }
}


void CallInitDeinit::resolveDeinit(const AstNode* ast,
                                   ID deinitedId,
                                   const QualifiedType& type,
                                   RV& rv) {

  // nothing to do for 'int' etc
  if (!typeNeedsInitDeinitCall(type.type())) {
    return;
  } else if (type.type()->isTupleType()) {
    // TODO: probably need to do something here, at least in some cases
    printf("Warning: omitting tuple deinit");
    return;
  }

  QualifiedType deinitType = type;

  // Deinit nilable class types as the corresponding non-nilable type, since we
  // will have a runtime check to not call deinit on nil.
  if (auto ct = type.type()->toClassType()) {
    auto decorator = ct->decorator();
    if (decorator.isNilable()) {
      deinitType = QualifiedType(
          type.kind(), ct->withDecorator(context, decorator.addNonNil()));
    }
  }

  std::vector<CallInfoActual> actuals;
  actuals.push_back(CallInfoActual(deinitType, USTR("this")));
  auto ci = CallInfo (/* name */ USTR("deinit"),
                      /* calledType */ deinitType,
                      /* isMethodCall */ true,
                      /* hasQuestionArg */ false,
                      /* isParenless */ false,
                      actuals);
  const Scope* scope = scopeForId(context, ast->id());
  auto inScopes = CallScopeInfo::forNormalCall(scope, resolver.poiScope);
  auto c = resolveGeneratedCall(context, ast, ci, inScopes);

  // Should we associate it with the current statement or the current block?
  const AstNode* assocAst = currentStatement();
  if (assocAst == nullptr || assocAst->isVarLikeDecl()) {
    assocAst = currentFrame()->scopeAst;
  }

  ResolvedExpression& opR = rv.byAst(assocAst);
  resolver.handleResolvedCall(opR, assocAst, ci, c,
                              { { AssociatedAction::DEINIT, deinitedId } });
}

void CallInitDeinit::handleDeclaration(const VarLikeDecl* ast, RV& rv) {
  VarFrame* frame = currentFrame();

  // check for use of deinited variables in type or init exprs
  if (auto init = ast->initExpression()) {
    processMentions(init, rv);
  }
  if (auto init = ast->initExpression()) {
    processMentions(init, rv);
  }

  bool inited = processDeclarationInit(ast, rv);
  bool splitInited = (splitInitedVars.count(ast->id()) > 0);

  bool handledFormal = false;
  bool isCatchVariable = false;

  if (ast->isFormal() || ast->isVarArgFormal()) {

    // consider the formal's intent
    ResolvedExpression& formalRe = rv.byAst(ast);
    QualifiedType formalType = formalRe.type();
    auto intent = formalType.kind();
    if (intent == QualifiedType::IN || intent == QualifiedType::CONST_IN) {
      // note that the variable is now initialized
      ID id = ast->id();
      frame->addToInitedVars(id);
      frame->localsAndDefers.push_back(id);
      handledFormal = true;
    } else if (intent == QualifiedType::OUT) {
      // treat it like any other variable declaration
      handledFormal = false;
      outOrInoutFormals.insert(ast->id());
    } else if (intent == QualifiedType::INOUT) {
      // note that the variable is now initialized
      ID id = ast->id();
      frame->addToInitedVars(id);
      frame->localsAndDefers.push_back(id);
      handledFormal = true;
      // also note it as in inout or out formal (for return handling)
      outOrInoutFormals.insert(ast->id());
    } else {
      handledFormal = true;
    }
  }

  // Errors in Catch statements will be instantiated by the throwing function
  // in the Try block
  auto parent = parsing::parentAst(context, ast);
  if (parent && parent->isCatch()) {
    auto catchNode = parent->toCatch();
    CHPL_ASSERT(ast == catchNode->error());
    isCatchVariable = true;
  }

  if (handledFormal) {
    // already handled above
  } else if (splitInited) {
    // Will be inited later, don't default init,
    // and also don't try to deinit it on e.g. a return before that point

  } else if (inited) {
    auto lhsAst = ast;
    auto rhsAst = ast->initExpression();

    ResolvedExpression& lhsRe = rv.byAst(lhsAst);
    QualifiedType lhsType = lhsRe.type();

    ResolvedExpression& rhsRe = rv.byAst(rhsAst);
    QualifiedType rhsType = rhsRe.type();

    processInit(frame, ast, lhsType, rhsType, rv);
    // note that the variable is now initialized
    ID id = ast->id();
    frame->addToInitedVars(id);
    frame->localsAndDefers.push_back(id);
  } else if (isCatchVariable) {
    // initialized from the throw that activates this Catch
    ID id = ast->id();
    frame->addToInitedVars(id);
    frame->localsAndDefers.push_back(id);
  } else {
    // default init it
    // not inited here and not split-inited, so default-initialize it
    resolveDefaultInit(ast, rv);
    // note that the variable is now initialized
    ID id = ast->id();
    frame->addToInitedVars(id);
    frame->localsAndDefers.push_back(id);
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
    // if initializing a variable, update localsAndDefers or initedOuterVars
    ID lhsId = refersToId(lhsAst, rv);
    recordInitializationOrder(frame, lhsId);
  }

  // check for use of deinited variables
  processMentions(ast, rv);

  bool isInit = splitInited;
  isInit |= resolver.initResolver && resolver.initResolver->isInitPoint(ast);

  if (lhsType.isType() || lhsType.isParam()) {
    // these are basically 'move' initialization
    resolveMoveInit(ast, rhsAst, lhsType, rhsType, rv);
  } else if (isInit) {
    processInit(frame, ast, lhsType, rhsType, rv);
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
    // if initializing a variable, update localsAndDefers or initedOuterVars
    ID actualId = refersToId(actual, rv);
    recordInitializationOrder(frame, actualId);

    // we can skip the copy if the types match
    resolveMoveInit(actual, actual, actualType, formalType, rv);
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

  if (auto t = formalType.type()) {
    if (auto ct = t->toClassType()) {
      if (ct->decorator().isBorrowed()) return;
    }
  }

  // compute the actual type
  QualifiedType actualType = rv.byAst(actual).type();

  // is the copy for 'in' elided?
  if (elidedCopyFromIds.count(actual->id()) > 0 &&
      isValue(actualType.kind()) &&
      typeNeedsInitDeinitCall(actualType.type())) {
    // it is move initialization
    resolveMoveInit(actual, actual, formalType, actualType, rv);

    // The RHS must represent a variable that is now dead,
    // so note that in deinitedVars.
    ID actualId = refersToId(actual, rv);
    // copy elision should only apply to copies from variables
    CHPL_ASSERT(!actualId.isEmpty());
    frame->deinitedVars.emplace(actualId, ast->id());
  } else {
    processInit(frame, actual, formalType, actualType, rv);
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

void CallInitDeinit::processReturnThrowYield(const uast::AstNode* ast, RV& rv) {
  // check for use of deinited variables
  processMentions(ast, rv);

  const AstNode* retValue = nullptr;
  if (auto rtn = ast->toReturn()) {
    retValue = rtn->value();
  } else if (auto y = ast->toYield()) {
    retValue = y->value();
  }

  ID skipDeinitId;

  if (retValue) {
    bool needsCopyOrConv = false;
    bool copyElidesFromSkip = false;
    ResolvedExpression& re = rv.byAst(retValue);

    // decide what needs to happen for this return
    analyzeReturnedExpr(re, ast,
                        needsCopyOrConv, copyElidesFromSkip, skipDeinitId,
                        rv);

    if (copyElidesFromSkip) {
      // if it's a yield, we need to also mark the rhs ID variable dead
      VarFrame* frame = currentFrame();
      frame->deinitedVars.emplace(skipDeinitId, ID());

    } else if (needsCopyOrConv) {
      QualifiedType fnRetType = returnOrYieldType();

      if (!fnRetType.isUnknown() && !fnRetType.isErroneousType()) {
        // init the return value from the return expression
        processInit(currentFrame(), ast, fnRetType, re.type(), rv);
      }
    }
  }

  if (!ast->isYield()) {
    processDeinitsForReturn(ast, skipDeinitId, rv);
  }
}

void CallInitDeinit::handleReturn(const uast::Return* ast, RV& rv) {
  processReturnThrowYield(ast, rv);
}

void CallInitDeinit::handleThrow(const uast::Throw* ast, RV& rv) {
  processReturnThrowYield(ast, rv);
}

void CallInitDeinit::handleYield(const uast::Yield* ast, RV& rv) {
  processReturnThrowYield(ast, rv);
}

void CallInitDeinit::handleTry(const Try* t, RV& rv) {
  VarFrame* frame = currentFrame();
  VarFrame* parent = currentParentFrame();

  int nCatch = currentNumCatchFrames();
  for (int i = 0; i < nCatch; i++) {
    VarFrame* catchFrame = currentCatchFrame(i);
    if (!catchFrame->returnsOrThrows) {
      processDeinitsAndPropagate(catchFrame, frame, rv);
    }
  }

  // propagate information out of the Try itself
  processDeinitsAndPropagate(frame, parent, rv);
}

void CallInitDeinit::handleDisjunction(const uast::AstNode * node, 
                                 VarFrame* currentFrame, 
                                 const std::vector<VarFrame*>& frames, 
                                 bool total, RV& rv) {
  
  for (auto frame : frames) {
    if(!frame->returnsOrThrows) {
      processDeinitsAndPropagate(frame, currentFrame, rv);
    }
  }

  //propagate out of the disjunction itself
  processDeinitsAndPropagate(currentFrame, currentParentFrame(), rv);
}

void CallInitDeinit::handleScope(const AstNode* ast, RV& rv) {
  VarFrame* frame = currentFrame();
  VarFrame* parent = currentParentFrame();
  processDeinitsAndPropagate(frame, parent, rv);
}

void callInitDeinit(Resolver& resolver) {
  std::set<ID> splitInitedVars = computeSplitInits(resolver.context,
                                                   resolver.symbol,
                                                   resolver.byPostorder);

  std::set<ID> elidedCopyFromIds = computeElidedCopies(resolver.context,
                                                       resolver.symbol,
                                                       resolver.byPostorder,
                                                       resolver.poiScope,
                                                       splitInitedVars,
                                                       resolver.returnType);

  auto symName = UniqueString::get(resolver.context, "unknown");
  if (auto nd = resolver.symbol->toNamedDecl()) {
    symName = nd->name();
  }

  // TODO: Run this for module initializer code as well. Currently if enabled,
  // it breaks a large number of dyno tests that have module-initializer code
  // containing things we can't resolve default-init for yet, such as
  // fully-defaulted generic types. Either adjust the tests to expect the errors
  // for unsupported code, or add the support, then enable this on modules.
  if (!resolver.symbol->isModule()) {
    CallInitDeinit uv(resolver.context, resolver,
                      splitInitedVars, elidedCopyFromIds);
    uv.process(resolver.symbol, resolver.byPostorder);
  }
}


} // end namespace resolution
} // end namespace chpl
