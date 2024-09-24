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

#include "Resolver.h"

#include "chpl/framework/global-strings.h"
#include "chpl/framework/query-impl.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/can-pass.h"
#include "chpl/resolution/intents.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"

#include "InitResolver.h"
#include "VarScopeVisitor.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"

#include <bitset>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

namespace {
  struct IterDetails {
    // Iterators will be resolved for each bit set in descending order.
    // Resolution stops when the first iterator with a valid yield type
    // is found. TODO: Stop the moment we find a signature instead?
    //
    // If the 'LEADER_FOLLOWER' bit is set, then the 'FOLLOWER' bit will
    // be ignored, and the 'leaderYieldType' will be computed from the
    // resolved leader signature. Any provided value for 'leaderYieldType'
    // will be overwritten.
    enum Priority {
      NONE            = 0b0000,
      STANDALONE      = 0b0001,
      LEADER_FOLLOWER = 0b0010,
      FOLLOWER        = 0b0100,
      SERIAL          = 0b1000,
    };

    // When an iter is resolved these pieces of the process will be stored.
    struct Pieces {
      bool wasCallInjected = false;
      CallResolutionResult crr;
      const TypedFnSignature* sig = crr.mostSpecific().only().fn();
    };
    Pieces standalone;
    Pieces leader;
    Pieces follower;
    Pieces serial;

    // This is the iterator that resolution stopped at if it succeeded.
    // In that case, the type in 'idxType' will be set to the yield type
    // of this iterator.
    Priority succeededAt = NONE;

    // If the 'LEADER_FOLLOWER' bit was set, then this will always be the
    // yield type of the resolved leader iterator. Otherwise, it will be
    // the type the user provided.
    QualifiedType leaderYieldType;

    // This will be set to the yield type of the first iterator to succeed.
    QualifiedType idxType;
  };
}

static const QualifiedType&
getIterKindConstantOrUnknownQuery(Context* context, UniqueString constant);

// Helper to resolve a specified iterator signature and its yield type.
static QualifiedType
resolveIterTypeWithTag(Resolver& rv,
                       IterDetails::Pieces& outIterPieces,
                       const AstNode* astForErr,
                       const AstNode* iterand,
                       UniqueString iterKindStr,
                       const QualifiedType& followThisFormal);

// Resolve iterators according to the policy set in 'mask' (see the type
// 'IterDetails::Policy'). Resolution stops the moment an iterator is
// found with a usable yield type.
static IterDetails resolveIterDetails(Resolver& rv,
                                      const AstNode* astForErr,
                                      const AstNode* iterand,
                                      const QualifiedType& leaderYieldType,
                                      int mask);

static QualifiedType::Kind qualifiedTypeKindForId(Context* context, ID id) {
  if (parsing::idIsParenlessFunction(context, id))
    return QualifiedType::PARENLESS_FUNCTION;

  auto tag = parsing::idToTag(context, id);
  if (isFunction(tag)) {
    return QualifiedType::FUNCTION;
  } else if (isModule(tag) || isInclude(tag)) {
    return QualifiedType::MODULE;
  } else if (isTypeDecl(tag)) {
    return QualifiedType::TYPE;
  } else if (asttags::isEnumElement(tag)) {
    return QualifiedType::CONST_VAR;
  }

  return QualifiedType::UNKNOWN;
}

static QualifiedType::Kind
qualifiedTypeKindForDecl(Context* context, const NamedDecl* decl) {
  if (const VarLikeDecl* vd = decl->toVarLikeDecl()) {
    Qualifier storageKind = vd->storageKind();
    return storageKind;
  }

  QualifiedType::Kind ret = qualifiedTypeKindForId(context, decl->id());
  CHPL_ASSERT(ret != QualifiedType::UNKNOWN && "case not handled");
  return ret;
}

// This class can gather up the IDs of contained fields or formals
struct GatherFieldsOrFormals {
  std::set<ID> fieldOrFormals;

  bool enter(const NamedDecl* decl) {
    // visit type declarations
    // is it a field or a formal?
    bool isField = false;
    if (auto var = decl->toVariable())
      if (var->isField())
        isField = true;

    bool isFieldOrFormal = isField || decl->isFormal();

    if (isFieldOrFormal)
      fieldOrFormals.insert(decl->id());

    return false;
  }
  void exit(const NamedDecl* decl) { }

  // go in to TupleDecl and MultiDecl
  bool enter(const TupleDecl* d) { return true; }
  void exit(const TupleDecl* d) { }
  bool enter(const MultiDecl* d) { return true; }
  void exit(const MultiDecl* d) { }

  // don't go in to anything else
  bool enter(const AstNode* ast) { return false; }
  void exit(const AstNode* ast) { }
};


Resolver::ParenlessOverloadInfo
Resolver::ParenlessOverloadInfo::fromMatchingIds(Context* context,
                                                 const MatchingIdsWithName& ids)
{
  bool anyMethodOrField = false;
  bool anyNonMethodOrField = false;

  for (auto idIt = ids.begin(); idIt != ids.end(); ++idIt) {
    if (idIt.curIdAndFlags().isMethodOrField()) {
      anyMethodOrField = true;
    } else {
      anyNonMethodOrField = true;
    }

    // TODO: should this check NOT_PARENFUL_FUNCTION ?
    if (!parsing::idIsParenlessFunction(context, idIt.curIdAndFlags().id())) {
      return {};
    }
  }
  return Resolver::ParenlessOverloadInfo(anyMethodOrField, anyNonMethodOrField);
}

Resolver
Resolver::createForModuleStmt(Context* context, const Module* mod,
                              const AstNode* modStmt,
                              ResolutionResultByPostorderID& byId) {
  auto ret = Resolver(context, mod, byId, nullptr);
  ret.curStmt = modStmt;
  ret.byPostorder.setupForSymbol(mod);
  return ret;
}

Resolver
Resolver::createForScopeResolvingModuleStmt(
                              Context* context, const Module* mod,
                              const AstNode* modStmt,
                              ResolutionResultByPostorderID& byId) {
  auto ret = Resolver(context, mod, byId, nullptr);
  ret.curStmt = modStmt;
  ret.byPostorder.setupForSymbol(mod);
  ret.scopeResolveOnly = true;
  return ret;
}

Resolver
Resolver::createForInitialSignature(Context* context, const Function* fn,
                                    ResolutionResultByPostorderID& byId)
{
  auto ret = Resolver(context, fn, byId, nullptr);
  ret.signatureOnly = true;
  ret.fnBody = fn->body();
  ret.byPostorder.setupForSignature(fn);

  if (fn->isMethod()) {
    fn->thisFormal()->traverse(ret);
    auto receiverType = ret.byPostorder.byAst(fn->thisFormal()).type();
    if (receiverType.hasTypePtr()) {
      if (auto ct = receiverType.type()->toCompositeType()) {
        ret.inCompositeType = ct;
      }
      ret.allowReceiverScopes = true;
    }
  }

  return ret;
}

Resolver
Resolver::createForInstantiatedSignature(Context* context,
                                         const Function* fn,
                                         const SubstitutionsMap& substitutions,
                                         const PoiScope* poiScope,
                                         ResolutionResultByPostorderID& byId) {
  auto ret = Resolver(context, fn, byId, poiScope);
  ret.substitutions = &substitutions;
  ret.signatureOnly = true;
  ret.fnBody = fn->body();
  ret.byPostorder.setupForSignature(fn);

  if (fn->isMethod()) {
    ret.allowReceiverScopes = true;
  }

  return ret;
}

Resolver
Resolver::createForFunction(Context* context,
                            const Function* fn,
                            const PoiScope* poiScope,
                            const TypedFnSignature* typedFnSignature,
                            ResolutionResultByPostorderID& byId) {
  auto ret = Resolver(context, fn, byId, poiScope);
  ret.typedSignature = typedFnSignature;
  ret.signatureOnly = false;
  ret.fnBody = fn->body();

  CHPL_ASSERT(typedFnSignature);
  CHPL_ASSERT(typedFnSignature->untyped());

  ret.byPostorder.setupForFunction(fn);

  // set the resolution results for the formals according to
  // the typedFnSignature
  const UntypedFnSignature* uSig = typedFnSignature->untyped();
  int nFormals = typedFnSignature->numFormals();
  for (int i = 0; i < nFormals; i++) {
    const Decl* decl = uSig->formalDecl(i);
    const auto& qt = typedFnSignature->formalType(i);

    ResolvedExpression& r = ret.byPostorder.byAst(decl);
    r.setType(qt);

    // TODO: Aren't these results already computed when we traverse formals
    // in resolution-queries?
    if (auto formal = decl->toFormal())
      ret.resolveTypeQueriesFromFormalType(formal, qt);
    if (auto formal = decl->toVarArgFormal())
      ret.resolveTypeQueriesFromFormalType(formal, qt);
    if (auto td = decl->toTupleDecl())
      ret.resolveTupleUnpackDecl(td, qt);
  }

  if (typedFnSignature->isMethod()) {
    // allow computing the receiver scope from the typedSignature.
    ret.allowReceiverScopes = true;
  }

  return ret;
}

Resolver
Resolver::createForInitializer(Context* context,
                               const uast::Function* fn,
                               const PoiScope* poiScope,
                               const TypedFnSignature* typedFnSignature,
                               ResolutionResultByPostorderID& byPostorder) {
  auto ret = createForFunction(context, fn, poiScope, typedFnSignature,
                               byPostorder);
  ret.initResolver = InitResolver::create(context, ret, fn);
  return ret;
}

Resolver
Resolver::createForScopeResolvingFunction(Context* context,
                                          const Function* fn,
                                          ResolutionResultByPostorderID& byId,
                                          owned<OuterVariables> outerVars) {
  auto ret = Resolver(context, fn, byId, nullptr);
  ret.typedSignature = nullptr; // re-set below
  ret.signatureOnly = true; // re-set below
  ret.scopeResolveOnly = true;
  ret.outerVars = std::move(outerVars);
  ret.fnBody = fn->body();

  ret.byPostorder.setupForFunction(fn);

  // scope-resolve the formal types but not the body, yet
  // (particularly relevant for computing the method receiver type
  //  if it is an identifier)
  fn->traverse(ret);

  // copy the formal types to create a TypedFnSignature
  const UntypedFnSignature* uSig = UntypedFnSignature::get(context, fn->id());
  std::vector<QualifiedType> formalTypes = ret.getFormalTypes(fn);
  auto whereTbd = TypedFnSignature::WhereClauseResult::WHERE_TBD;
  const TypedFnSignature* sig =
    TypedFnSignature::get(context, uSig,
                          std::move(formalTypes),
                          whereTbd,
                          /* needsInstantiation */ false,
                          /* instantiatedFrom */ nullptr,
                          /* parentFn */ nullptr,
                          /* formalsInstantiated */ Bitmap());

  ret.typedSignature = sig;
  ret.signatureOnly = false;

  CHPL_ASSERT(sig);
  CHPL_ASSERT(sig->untyped());

  // set the resolution results for the formals according to
  // the typedFnSignature (which just has UnknownType in it for all args
  // here)
  int nFormals = sig->numFormals();
  for (int i = 0; i < nFormals; i++) {
    const Decl* decl = uSig->formalDecl(i);
    const auto& qt = sig->formalType(i);

    ResolvedExpression& r = ret.byPostorder.byAst(decl);
    r.setType(qt);
  }

  return ret;
}

Resolver
Resolver::createForScopeResolvingField(Context* context,
                                 const uast::AggregateDecl* ad,
                                 const AstNode* fieldStmt,
                                 ResolutionResultByPostorderID& byPostorder) {
  auto ret = Resolver(context, ad, byPostorder, nullptr);
  ret.scopeResolveOnly = true;
  ret.curStmt = fieldStmt;
  ret.byPostorder.setupForSymbol(ad);

  return ret;
}

Resolver
Resolver::createForScopeResolvingEnumConstant(Context* context,
                                       const uast::Enum* ed,
                                       const uast::AstNode* fieldStmt,
                                       ResolutionResultByPostorderID& byPostorder) {
  auto ret = Resolver(context, ed, byPostorder, nullptr);
  ret.scopeResolveOnly = true;
  ret.curStmt = fieldStmt;
  ret.byPostorder.setupForSymbol(ed);

  return ret;
}


// set up Resolver to initially resolve field declaration types
Resolver
Resolver::createForInitialFieldStmt(Context* context,
                                    const AggregateDecl* decl,
                                    const AstNode* fieldStmt,
                                    const CompositeType* compositeType,
                                    ResolutionResultByPostorderID& byId,
                                    DefaultsPolicy defaultsPolicy) {
  auto ret = Resolver(context, decl, byId, nullptr);
  ret.curStmt = fieldStmt;
  ret.inCompositeType = compositeType;
  ret.defaultsPolicy = defaultsPolicy;
  ret.byPostorder.setupForSymbol(decl);
  ret.fieldTypesOnly = true;
  ret.allowReceiverScopes = true;
  return ret;
}

// set up Resolver to resolve instantiated field declaration types
Resolver
Resolver::createForInstantiatedFieldStmt(Context* context,
                                         const AggregateDecl* decl,
                                         const AstNode* fieldStmt,
                                         const CompositeType* compositeType,
                                         const PoiScope* poiScope,
                                         ResolutionResultByPostorderID& byId,
                                         DefaultsPolicy defaultsPolicy) {
  auto ret = Resolver(context, decl, byId, poiScope);
  ret.curStmt = fieldStmt;
  ret.inCompositeType = compositeType;
  ret.substitutions = &compositeType->substitutions();
  ret.defaultsPolicy = defaultsPolicy;
  ret.byPostorder.setupForSymbol(decl);
  ret.fieldTypesOnly = true;
  ret.allowReceiverScopes = true;
  return ret;
}

Resolver
Resolver::createForEnumElements(Context* context,
                                const uast::Enum* enumNode,
                                ResolutionResultByPostorderID& byPostorder) {
  auto ret = Resolver(context, enumNode, byPostorder, nullptr);
  ret.byPostorder.setupForSymbol(enumNode);
  return ret;
}

// set up Resolver to resolve instantiated field declaration types
// without knowing the CompositeType
Resolver
Resolver::createForInstantiatedSignatureFields(Context* context,
                                     const AggregateDecl* decl,
                                     const SubstitutionsMap& substitutions,
                                     const PoiScope* poiScope,
                                     ResolutionResultByPostorderID& byId) {
  auto ret = Resolver(context, decl, byId, poiScope);
  ret.substitutions = &substitutions;
  ret.defaultsPolicy = DefaultsPolicy::IGNORE_DEFAULTS;
  ret.byPostorder.setupForSymbol(decl);
  ret.fieldTypesOnly = true;
  return ret;
}


// set up Resolver to resolve a parent class type expression
Resolver
Resolver::createForParentClass(Context* context,
                               const AggregateDecl* decl,
                               const SubstitutionsMap& substitutions,
                               const PoiScope* poiScope,
                               ResolutionResultByPostorderID& byId) {
  auto ret = Resolver(context, decl, byId, poiScope);
  ret.substitutions = &substitutions;
  ret.defaultsPolicy = DefaultsPolicy::USE_DEFAULTS;
  ret.byPostorder.setupForSymbol(decl);
  return ret;
}

// set up Resolver to scope resolve a parent class type expression
Resolver
Resolver::createForParentClassScopeResolve(Context* context,
                                           const AggregateDecl* decl,
                                           ResolutionResultByPostorderID& byId)
{
  auto ret = Resolver(context, decl, byId, /* poiScope */ nullptr);
  ret.defaultsPolicy = DefaultsPolicy::USE_DEFAULTS;
  ret.byPostorder.setupForSymbol(decl);
  ret.scopeResolveOnly = true;
  return ret;
}


// set up Resolver to resolve a param loop
Resolver
Resolver::paramLoopResolver(Resolver& parent,
                            const For* loop,
                            ResolutionResultByPostorderID& bodyResults) {
  auto ret = Resolver(parent.context, loop, bodyResults, parent.poiScope);
  ret.parentResolver = &parent;
  ret.declStack = parent.declStack;
  ret.byPostorder.setupForParamLoop(loop, parent.byPostorder);
  ret.typedSignature = parent.typedSignature;

  return ret;
}

const AstNode* Resolver::nearestCalledExpression() const {
  if (callNodeStack.empty()) return nullptr;

  return callNodeStack.back()->calledExpression();
}

void Resolver::setCompositeType(const CompositeType* ct) {
  CHPL_ASSERT(this->inCompositeType == nullptr);
  this->inCompositeType = ct;
}

std::vector<types::QualifiedType>
Resolver::getFormalTypes(const Function* fn) {
  std::vector<types::QualifiedType> formalTypes;
  for (auto formal : fn->formals()) {
    QualifiedType t = byPostorder.byAst(formal).type();
    // compute concrete intent
    bool isThis = false;
    bool isInit = fn->name() == USTR("init") || fn->name() == USTR("init=");
    if (auto namedDecl = formal->toNamedDecl()) {
      isThis = namedDecl->name() == USTR("this");
    }
    Qualifier intent = resolveIntent(t, isThis, isInit);
    if (auto attributes = formal->attributeGroup()) {
      if (attributes->hasPragma(PRAGMA_INTENT_REF_MAYBE_CONST_FORMAL)) {
        intent = Qualifier::REF_MAYBE_CONST;
      }
    }
    t = QualifiedType(intent, t.type(), t.param());

    formalTypes.push_back(std::move(t));
  }
  return formalTypes;
}

types::QualifiedType Resolver::typeErr(const uast::AstNode* ast,
                                       const char* msg)
{
  context->error(ast, "%s", msg);
  auto t = QualifiedType(QualifiedType::UNKNOWN, ErroneousType::get(context));
  return t;
}

static bool isOuterVariable(Resolver* rv, ID target) {
  if (target.isEmpty()) return false;

  // E.g., a function, or a class/record/union/enum. We don't need to track
  // this, and shouldn't, because its current instantiation may not make any
  // sense in this context. As well, these things have an "infinite lifetime",
  // and are always reachable.
  if (target.isSymbolDefiningScope()) return false;


  auto parentSymbolId = target.parentSymbolId(rv->context);

  // No match if there is no parent or if the parent is the resolver symbol.
  if (parentSymbolId.isEmpty()) return false;
  if (rv->symbol && parentSymbolId == rv->symbol->id()) return false;

  switch (parsing::idToTag(rv->context, parentSymbolId)) {
    case asttags::Function: return true;

    // Module-scope variables are not considered outer-variables. However,
    // variables declared in a module initializer statement can be, e.g.,
    /**
      module M {
        if someCondition {
          var someVar = 42;
          proc f() { writeln(someVar); }
          f();
        }
      }
    */
    case asttags::Module: {
      auto targetParentId = parsing::idToParentId(rv->context, target);
      return parentSymbolId != targetParentId;
    } break;
    default: break;
  }

  return false;
}

bool Resolver::getMethodReceiver(QualifiedType* outType, ID* outId) {
  if (!scopeResolveOnly &&
      typedSignature &&
      typedSignature->untyped()->isMethod()) {
    // use type information to compute the receiver type
    if (outType) *outType = typedSignature->formalType(0);
    return true;
  } else {
    // Use scope-resolver logic to compute the receiver scopes.

    // If receiver type is specified by a simple identifier, determine it.
    // For more complicated receiver types we cannot yet gather any information.

    ID methodId;
    const Formal* thisFormal = nullptr;
    if (typedSignature && typedSignature->untyped()->isMethod()) {
      thisFormal = typedSignature->untyped()->formalDecl(0)->toFormal();
      methodId = typedSignature->id();
    }

    if (thisFormal == nullptr) {
      // if there is no typed signature, fall back to computing receiver type
      // from an identifier
      if (auto func = this->symbol->toFunction()) {
        thisFormal = func->thisFormal();
        methodId = this->symbol->id();
      }
    }

    if (thisFormal) {
      auto type = thisFormal->typeExpression();
      if (type == nullptr) {
        // `this` formals of primary methods have no type expression. They
        // are, however, in primary methods, so the method's parent is the
        // aggregate type whose scope should be used.

        if (outId) *outId = parsing::idToParentId(context, methodId);
      } else if (auto ident = type->toIdentifier()) {
        if (outId) *outId = byPostorder.byAst(ident).toId();
      }
      return true;
    } else if (auto agg = symbol->toAggregateDecl()) {
      // Lacking any more specific information, use the containing aggregate ID
      // if available.
      //
      // TODO: when enabled for 'scopeResolveOnly' we must start issuing
      // errors for cyclic class hierarchies and an inability to find
      // parent class expressions.
      if (!scopeResolveOnly) {
        if (outId) *outId = agg->id();
        return true;
      }
    }
  }

  return false;
}

const ReceiverScopeHelper* Resolver::getMethodReceiverScopeHelper() {
  if (!allowReceiverScopes) {
    // can't use receiver scopes yet
    // (e.g. we are computing the type of 'this' & otherwise that would recurse)
    return nullptr;
  }

  if (!receiverScopesComputed) {
    receiverScopeHelper = nullptr;

    auto fn = symbol->toFunction();
    if (fn && fn->isMethod()) {
      if (!scopeResolveOnly) {
        if (typedSignature != nullptr) {
          if (typedSignature->isMethod()) {
            // if we have a method typedFnSignature, use that
            receiverScopeTypedHelper =
              ReceiverScopeTypedHelper(typedSignature->id(),
                                       typedSignature->formalType(0));
            receiverScopeHelper = &receiverScopeTypedHelper;
          }
        } else {
          // use the result from byPostorder
          auto receiverType = byPostorder.byAst(fn->thisFormal()).type();
          receiverScopeTypedHelper =
            ReceiverScopeTypedHelper(fn->id(), std::move(receiverType));
          receiverScopeHelper = &receiverScopeTypedHelper;
        }

      } else {
        // scope resolve only
        receiverScopeHelper = &receiverScopeSimpleHelper;
      }
    }

    receiverScopesComputed = true;
  }

  return receiverScopeHelper;
}

const MethodLookupHelper* Resolver::getFieldDeclarationLookupHelper() {
  if (!allowReceiverScopes) {
    // can't use receiver scopes yet
    // (e.g. we are computing the parent class types)
    return nullptr;
  }

  if (!methodHelperComputed) {
    methodLookupHelper = nullptr;

    if (symbol->isTypeDecl()) {
      // the case of being in a method will be handled
      // by getMethodReceiverScopeHelper.

      if (!scopeResolveOnly) {
        if (inCompositeType) {
          // create a helper for the containing class/record type
          // to help with field/method access in field & forwarding declarations
          auto qt = methodReceiverType();
          auto helper = ReceiverScopeTypedHelper();
          methodLookupHelper = helper.methodLookupForType(context, qt);
        }
      } else {
        auto helper = ReceiverScopeSimpleHelper();
        ID typeId = symbol->id();
        methodLookupHelper = helper.methodLookupForTypeId(context, typeId);
      }
      methodHelperComputed = true;
    }
  }

  return methodLookupHelper;
}

QualifiedType Resolver::methodReceiverType() {
  if (typedSignature && typedSignature->untyped()->isMethod()) {
    return typedSignature->formalType(0);
  } else if (inCompositeType != nullptr) {
    // compute the method receiver type in case it is needed
    // to find fields or methods
    // TODO: do we want this to be more focused? It might compute
    // the receiver type in too many cases.
    const Type* t = inCompositeType;
    if (auto bct = t->toBasicClassType()) {
      auto b = ClassTypeDecorator::BORROWED_NONNIL;
      t = ClassType::get(context, bct, /* manager */ nullptr,
                         ClassTypeDecorator(b));
    }
    return QualifiedType(QualifiedType::VAR, t);
  }

  return QualifiedType();
}

bool Resolver::isPotentialSuper(const Identifier* ident, QualifiedType* outType) {
  if (ident->name() == USTR("super")) {
    return getMethodReceiver(outType);
  }
  return false;
}

bool Resolver::shouldUseUnknownTypeForGeneric(const ID& id) {
  // make sure the set of IDs for fields and formals is computed
  if (!fieldOrFormalsComputed) {
    auto visitor = GatherFieldsOrFormals();
    symbol->traverse(visitor);
    fieldOrFormals.swap(visitor.fieldOrFormals);

    // also compute instantiatedFieldOrFormals
    if (typedSignature != nullptr) {
      auto untyped = typedSignature->untyped();
      int nFormals = untyped->numFormals();
      for (int i = 0; i < nFormals; i++) {
        if (typedSignature->formalIsInstantiated(i)) {
          CHPL_ASSERT(!untyped->formalDecl(i)->id().isEmpty());
          instantiatedFieldOrFormals.insert(untyped->formalDecl(i)->id());
        }
      }
    }

    fieldOrFormalsComputed = true;
  }

  bool isFieldOrFormal = fieldOrFormals.count(id) > 0;
  bool isSubstituted = false;
  bool isFormalInstantiated = false;

  if (substitutions != nullptr) {
    auto search = substitutions->find(id);
    if (search != substitutions->end()) {
      isSubstituted = true;
    }
  }

  // check also instantiated formals from typedSignature
  if (isFieldOrFormal) {
    isFormalInstantiated = instantiatedFieldOrFormals.count(id) > 0;
  }

  return isFieldOrFormal && !isSubstituted && !isFormalInstantiated;
}

// is it a call to int / uint / etc?
static bool isCallToIntEtc(const AstNode* formalTypeExpr) {
  if (auto call = formalTypeExpr->toFnCall()) {
    if (auto calledAst = call->calledExpression()) {
      if (auto calledIdent = calledAst->toIdentifier()) {
        UniqueString n = calledIdent->name();
        if (n == USTR("int") || n == USTR("uint") || n == USTR("bool") ||
            n == USTR("real") || n == USTR("imag") || n == USTR("complex")) {
          return true;
        }
      }
    }
  }

  return false;
}

static bool isCallToCPtr(const AstNode* formalTypeExpr) {
  if (auto call = formalTypeExpr->toFnCall()) {
    if (auto calledAst = call->calledExpression()) {
      if (auto calledIdent = calledAst->toIdentifier()) {
        UniqueString n = calledIdent->name();
        if (n == USTR("c_ptr") || n == USTR("c_ptrConst")) {
          return true;
        }
      }
    }
  }

  return false;
}

static void varArgTypeQueryError(Context* context,
                                 const AstNode* node,
                                 ResolvedExpression& result) {
  context->error(node, "Cannot query type of variable arguments formal when types are not homogeneous");
  auto errType = QualifiedType(QualifiedType::TYPE,
                               ErroneousType::get(context));
  result.setType(errType);
}

static bool isCallToClassManager(const FnCall* call) {
  auto ident = call->calledExpression()->toIdentifier();
  if (!ident) return false;
  auto name = ident->name();
  return name == USTR("owned") || name == USTR("shared") ||
         name == USTR("unmanaged") || name == USTR("borrowed");
}

// helper for resolveTypeQueriesFromFormalType
void Resolver::resolveTypeQueries(const AstNode* formalTypeExpr,
                                  const QualifiedType& actualType,
                                  bool isNonStarVarArg,
                                  bool isTopLevel) {

  auto actualTypePtr = actualType.type();
  // Give up if the type is nullptr or UnknownType or AnyType
  if (actualTypePtr == nullptr ||
      actualTypePtr->isUnknownType() ||
      actualTypePtr->isAnyType())
    return;

  CHPL_ASSERT(formalTypeExpr != nullptr);

  // Give up if typeExpr is an Identifier
  if (formalTypeExpr->isIdentifier())
    return;

  if (formalTypeExpr->isTypeQuery()) {
    ResolvedExpression& result = byPostorder.byAst(formalTypeExpr);

    if (isNonStarVarArg) {
      varArgTypeQueryError(context, formalTypeExpr, result);
    } else {
      // Set the type that we know (since it was passed in)
      if (isTopLevel) {
        // Queries at the top level only capture type. e.g., param x: ?t
        result.setType(QualifiedType(QualifiedType::TYPE, actualTypePtr));
      } else {
        result.setType(actualType);
      }
    }
  }

  // Make recursive calls as needed to handle any TypeQuery nodes
  // nested within typeExpr.
  if (auto call = formalTypeExpr->toFnCall()) {
    if (isCallToIntEtc(formalTypeExpr)) {
      // If it is e.g. int(TypeQuery), resolve the type query to the width
      // Set the type that we know (since it was passed in)
      if (call->numActuals() == 1) {
        if (auto tq = call->actual(0)->toTypeQuery()) {
          if (auto pt = actualTypePtr->toPrimitiveType()) {
            ResolvedExpression& resolvedWidth = byPostorder.byAst(tq);
            if (isNonStarVarArg) {
              varArgTypeQueryError(context, call->actual(0), resolvedWidth);
            } else {
              auto p = IntParam::get(context, pt->bitwidth());
              auto it = IntType::get(context, 0);
              auto qt = QualifiedType(QualifiedType::PARAM, it, p);
              resolvedWidth.setType(qt);
            }
          }
        }
      }
    } else if (isCallToCPtr(formalTypeExpr)) {
      // If it is e.g. c_ptr(TypeQuery), resolve the type query to the eltType
      // Set the type that we know (since it was passed in)
      if (call->numActuals() == 1) {
        if (auto tq = call->actual(0)->toTypeQuery()) {
          if (auto pt = actualTypePtr->toCPtrType()) {
            ResolvedExpression& resolvedElt = byPostorder.byAst(tq);
            if (isNonStarVarArg) {
              varArgTypeQueryError(context, call->actual(0), resolvedElt);
            } else {
              resolvedElt.setType(QualifiedType(QualifiedType::TYPE,
                                                pt->eltType()));
            }
          }
        }
      }
    } else if (isCallToClassManager(call) &&
               call->numActuals() == 1 &&
               actualTypePtr->isClassType()) {
      // Strip the owned/shared/etc. for both the formal and the type
      // This works since the recursive case expects the actual type to
      // be a composite type, which manageable types are.

      auto classArg = call->actual(0);
      auto actualMt = actualTypePtr->toClassType()->manageableType();
      resolveTypeQueries(classArg, QualifiedType(actualType.kind(), actualMt),
                         isNonStarVarArg,
                         /* isTopLevel */ false);
    } else {
      // Error if it is not calling a type constructor
      auto actualCt = actualTypePtr->toCompositeType();

      if (actualCt == nullptr) {
        context->error(formalTypeExpr, "Type construction call expected");
        return;
      } else if (!actualCt->instantiatedFromCompositeType()) {
        context->error(formalTypeExpr, "Instantiated type expected");
        return;
      }

      // TODO: need to implement type queries for domain type expressions
      if (actualCt->isDomainType()) return;

      auto baseCt = actualCt->instantiatedFromCompositeType();
      auto sig = typeConstructorInitial(context, baseCt);

      // Generate a simple CallInfo for the call
      auto callInfo = CallInfo::createSimple(call);
      // generate a FormalActualMap
      auto faMap = FormalActualMap(sig, callInfo);

      // Now, consider the formals
      int nActuals = call->numActuals();
      for (int i = 0; i < nActuals; i++) {
        // ignore actuals like ?
        // since these aren't type queries & don't match a formal
        if (isQuestionMark(call->actual(i)))
          continue;

        const FormalActual* fa = faMap.byActualIdx(i);
        CHPL_ASSERT(fa != nullptr && fa->formal() != nullptr);

        // get the substitution for that field from the CompositeType
        // and recurse with the result to set types for nested TypeQuery nodes
        auto formal = fa->formal()->toNamedDecl();
        const SubstitutionsMap& subs = actualCt->substitutions();
        auto ad = parsing::idToAst(context, actualCt->id())->toAggregateDecl();
        auto field = findFieldByName(context, ad, actualCt, formal->name());
        auto search = subs.find(field->id());
        if (search != subs.end()) {
          QualifiedType fieldType = search->second;
          auto actual = call->actual(i);
          resolveTypeQueries(actual, fieldType,
                             isNonStarVarArg,
                             /* isTopLevel */ false);
        }
      }
    }
  }
}

void Resolver::resolveTypeQueriesFromFormalType(const VarLikeDecl* formal,
                                                QualifiedType formalType) {
  if (auto varargs = formal->toVarArgFormal()) {
    const TupleType* tuple = formalType.type()->toTupleType();

    // args...?n
    if (auto countQuery = varargs->count()) {
      auto intType = IntType::get(context, 0);
      auto val = IntParam::get(context, tuple->numElements());
      ResolvedExpression& result = byPostorder.byAst(countQuery);
      result.setType(QualifiedType(QualifiedType::PARAM, intType, val));
    }

    if (auto typeExpr = formal->typeExpression()) {
      QualifiedType useType = tuple->elementType(0);
      resolveTypeQueries(typeExpr, useType, !tuple->isStarTuple());
    }
  } else if (auto typeExpr = formal->typeExpression()) {
    resolveTypeQueries(typeExpr, formalType);
  }
}

bool Resolver::checkForKindError(const AstNode* typeForErr,
                                 const AstNode* initForErr,
                                 QualifiedType::Kind declKind,
                                 QualifiedType declaredType,
                                 QualifiedType initExprType) {
  // check that the resolution of the type expression is a type
  if (declaredType.hasTypePtr() &&
      declaredType.kind() != QualifiedType::UNKNOWN &&
      declaredType.kind() != QualifiedType::TYPE_QUERY) {
    if (declaredType.kind() != QualifiedType::TYPE) {
      CHPL_REPORT(context, ValueUsedAsType, typeForErr, declaredType);
      return true;
    }
  }

  // check that the init expression has compatible kind
  if (initExprType.hasTypePtr() &&
      !initExprType.type()->isUnknownType() &&
      initExprType.kind() != QualifiedType::UNKNOWN) {
    if (declKind == QualifiedType::TYPE &&
        initExprType.kind() != QualifiedType::TYPE) {
      CHPL_REPORT(context, IncompatibleKinds, declKind, initForErr, initExprType);
      return true;
    } else if (declKind != QualifiedType::TYPE &&
               initExprType.kind() == QualifiedType::TYPE) {
      CHPL_REPORT(context, IncompatibleKinds, declKind, initForErr, initExprType);
      return true;
    } else if (declKind == QualifiedType::PARAM &&
               initExprType.kind() != QualifiedType::PARAM) {
      CHPL_REPORT(context, IncompatibleKinds, declKind, initForErr, initExprType);
      return true;
    }
  }

  return false; // no error
}


static const CompositeType*
getTypeWithCustomInfer(Context* context, const Type* type) {
  if (auto rec = type->getCompositeType()) {
    if (rec->id().isEmpty() == false) {
      // TODO 2023-05: This might return null for a type like 'string' which
      // we force to exist whether the module is present or not. Is this
      // necessary in the long-term?
      if (const auto node = parsing::idToAst(context, rec->id())) {
        if (const auto attr = node->attributeGroup()) {
          if (attr->hasPragma(PRAGMA_INFER_CUSTOM_TYPE)) {
            return rec;
          }
        }
      }
    }
  }

  return nullptr;
}

const Type*
Resolver::computeCustomInferType(const AstNode* decl,
                                 const CompositeType* ct) {
  QualifiedType ret;
  auto name = UniqueString::get(context, "chpl__inferCopyType");
  QualifiedType calledType = QualifiedType(QualifiedType::CONST_REF, ct);
  auto receiver = CallInfoActual(calledType, USTR("this"));
  std::vector<CallInfoActual> actuals = {std::move(receiver)};

  auto ci = CallInfo(/* name */ name,
                     /* calledType */ calledType,
                     /* isMethodCall */ true,
                     /* hasQuestionArg */ false,
                     /* isParenless */ false,
                     std::move(actuals));
  auto inScopes = CallScopeInfo::forNormalCall(scopeStack.back(), poiScope);
  auto rr = resolveGeneratedCall(context, nullptr, ci, inScopes);
  if (rr.mostSpecific().only()) {
    ret = rr.exprType();
    handleResolvedCall(byPostorder.byAst(decl), decl, ci, rr,
                       { { AssociatedAction::INFER_TYPE, decl->id() } });
  } else {
    context->error(ct->id(), "'chpl__inferCopyType' is unimplemented");
  }

  return ret.type();
}

QualifiedType Resolver::getTypeForDecl(const AstNode* declForErr,
                                       const AstNode* typeForErr,
                                       const AstNode* initForErr,
                                       QualifiedType::Kind declKind,
                                       QualifiedType declaredType,
                                       QualifiedType initExprType) {

  const Type* typePtr = nullptr;
  const Param* paramPtr = nullptr;

  if (typeForErr == nullptr)
    typeForErr = declForErr;
  if (initForErr == nullptr)
    initForErr = declForErr;

  bool inferParam = (declKind == QualifiedType::PARAM &&
                     initExprType.kind() == QualifiedType::PARAM);

  // check that the resolution of the type expression is a type
  if (checkForKindError(typeForErr, initForErr, declKind,
                        declaredType, initExprType)) {
    // error already issued in checkForKindError
    typePtr = ErroneousType::get(context);
  } else if (!declaredType.hasTypePtr() && !initExprType.hasTypePtr()) {
    // Can't establish the type for now
    typePtr = UnknownType::get(context);
  } else if (declaredType.hasTypePtr() && !initExprType.hasTypePtr()) {
    // declared type but no init, so use declared type
    typePtr = declaredType.type();
  } else if (!declaredType.hasTypePtr() && initExprType.hasTypePtr()) {
    // Check if this type requires custom type inference
    if (auto rec = getTypeWithCustomInfer(context, initExprType.type())) {
      typePtr = computeCustomInferType(declForErr, rec);
    } else {
      // init but no declared type, so use init type
      typePtr = initExprType.type();
      if (inferParam) {
        paramPtr = initExprType.param();
      }
    }
  } else {
    // otherwise both declaredType and initExprType are provided.
    // check that they are compatible
    auto fullDeclType = QualifiedType(declKind, declaredType.type());
    auto got = canPass(context, initExprType, fullDeclType);
    if (!got.passes()) {
      if (declaredType.type()->isExternType()) {
        auto varDT = QualifiedType(QualifiedType::VAR, declaredType.type());
        // We allow for default-init-then-assign for extern types
        std::vector<CallInfoActual> actuals;
        actuals.push_back(CallInfoActual(varDT, UniqueString()));
        actuals.push_back(CallInfoActual(initExprType, UniqueString()));
        auto ci = CallInfo (/* name */ USTR("="),
                            /* calledType */ QualifiedType(),
                            /* isMethodCall */ false,
                            /* hasQuestionArg */ false,
                            /* isParenless */ false,
                            actuals);

        // TODO: store an associated action?
        const Scope* scope = scopeStack.back();
        auto inScopes = CallScopeInfo::forNormalCall(scope, poiScope);
        auto c = resolveGeneratedCall(context, declForErr, ci, inScopes);
        if (!c.mostSpecific().isEmpty()) {
          typePtr = declaredType.type();
        } else {
          CHPL_REPORT(context, IncompatibleTypeAndInit, declForErr, typeForErr,
                      initForErr, declaredType.type(), initExprType.type());
          typePtr = ErroneousType::get(context);
        }
      } else {
          // For a record/union, check for an init= from the provided type
        const bool isRecordOrUnion = (declaredType.type()->isRecordType() ||
                                      declaredType.type()->isUnionType());
        const TypedFnSignature* initEq = nullptr;
        if (isRecordOrUnion) {
          // Note: This code assumes that this init= will be added as an
          // associated action by ``CallInitDeinit::resolveCopyInit``
          initEq = tryResolveInitEq(context, declForErr, declaredType.type(),
                                    initExprType.type(), poiScope);
        }
        if (initEq == nullptr) {
          CHPL_REPORT(context, IncompatibleTypeAndInit, declForErr, typeForErr,
                      initForErr, declaredType.type(), initExprType.type());
          typePtr = ErroneousType::get(context);
        } else {
          typePtr = initEq->formalType(0).type();
        }
      }
    } else if (!got.instantiates()) {
      // use the declared type since no conversion/promotion was needed
      typePtr = declaredType.type();
    } else {
      // instantiation is needed
      if (!got.converts() && !got.promotes()) {
        // use the init expr type since no conversion/promotion was needed
        typePtr = initExprType.type();
        if (inferParam) {
          paramPtr = initExprType.param();
        }
      } else {
        // get instantiation type
        auto t = getInstantiationType(context, initExprType, fullDeclType);
        typePtr = t.type();
        if (inferParam) {
          paramPtr = t.param();
        }
      }
    }
  }

  CHPL_ASSERT(typePtr != nullptr); // should always be set above.
  return QualifiedType(declKind, typePtr, paramPtr);
}

static bool isValidVarArgCount(QualifiedType paramSize) {
  if (paramSize.type() == nullptr ||
      paramSize.type()->isErroneousType()) {
    return false;
  } else if (paramSize.isParam() == false) {
    return false;
  } else {
    const Param* p = paramSize.param();
    if (p == nullptr) {
      // param n : int, args...n
      return true;
    } else if (p->isIntParam()) {
      int64_t val = p->toIntParam()->value();
      return val > 0;
    } else if (p->isUintParam()) {
      uint64_t val = p->toUintParam()->value();
      return val > 0;
    } else {
      return false;
    }
  }

  return true;
}


//
// This function is called in the case that there is no substitution. When
// resolveNamedDecl processes such a case, it will compute the kind/type as if
// for a normal formal. This function will attempt to use that kind/type as
// the star-type for a VarArgTuple.
//
static const Type* computeVarArgTuple(Resolver& resolver,
                                      const VarArgFormal* varArgs,
                                      QualifiedType::Kind qtKind,
                                      const Type* typePtr) {
  Context* context = resolver.context;
  auto& byPostorder = resolver.byPostorder;
  auto tuple = typePtr->toTupleType();
  bool isVarArgTuple = (tuple != nullptr && tuple->isVarArgTuple());
  if (!isVarArgTuple) {
    QualifiedType paramSize;
    bool invalid = false;
    if (auto count = varArgs->count()) {
      if (count->isTypeQuery() == false &&
          isQuestionMark(count) == false) {
        ResolvedExpression& countVal = byPostorder.byAst(count);
        paramSize = countVal.type();
        invalid = !isValidVarArgCount(paramSize);
      }
    }

    if (invalid) {
      typePtr = ErroneousType::get(context);
    } else {
      auto newKind = resolveIntent(QualifiedType(qtKind, typePtr),
                                   /* isThis */ false, /* isInit */ false);
      QualifiedType elt = QualifiedType(newKind, typePtr);
      typePtr = TupleType::getStarTuple(context, paramSize, elt);
    }
  }

  return typePtr;
}

/* If the type is generic with defaults, computes the defaults of a type.
   Returns the original type if instantiating with defaults isn't necessary. */
static QualifiedType computeTypeDefaults(Resolver& resolver,
                                         const QualifiedType& type) {
  if (auto t = type.type()) {
    if (auto ct = t->getCompositeType()) {
      // test if that type is generic
      auto g = getTypeGenericity(resolver.context, ct);
      if (g == Type::GENERIC_WITH_DEFAULTS) {
        // fill in the defaults
        return typeWithDefaults(resolver.context, type);
      }
    }
  }
  return type;
}

// useType will be used to set the type if it is not nullptr
void Resolver::resolveNamedDecl(const NamedDecl* decl, const Type* useType) {
  if (scopeResolveOnly)
    return;

  // Figure out the Kind of the declaration
  auto qtKind = qualifiedTypeKindForDecl(context, decl);

  // Figure out the Type of the declaration
  // Nested Identifiers and Expressions should already be resolved
  const Type* typePtr = nullptr;

  // Figure out the param value, if any
  const Param* paramPtr = nullptr;

  bool isField = false;
  bool isFormal = false;
  bool isFormalThis = false;
  bool isFieldOrFormal = false;
  bool isVarArgs = decl->isVarArgFormal();

  if (auto var = decl->toVarLikeDecl()) {
    // Figure out variable type based upon:
    //  * the type in the variable declaration
    //  * the initialization expression in the variable declaration
    //  * the initialization expression from split-init

    auto typeExpr = var->typeExpression();
    auto initExpr = var->initExpression();

    if (auto var = decl->toVariable())
      if (var->isField())
        isField = true;

    isFormal = decl->isFormal() || isVarArgs;
    isFormalThis = isFormal && decl->name() == USTR("this");
    isFieldOrFormal = isField || isFormal;

    bool foundSubstitution = false;
    bool foundSubstitutionDefaultHint = false;

    QualifiedType typeExprT;
    QualifiedType initExprT;

    if (isFieldOrFormal) {
      // use substitutions computed for fields and formals
      if (substitutions != nullptr) {
        auto search = substitutions->find(decl->id());
        if (search != substitutions->end()) {
          const QualifiedType& t = search->second;
          if (t.kind() == QualifiedType::PARAM) {
            typeExprT = t; // keep kind, typePtr, paramPtr
          } else {
            // set kind to TYPE and forget about paramPtr
            typeExprT = QualifiedType(QualifiedType::TYPE, t.type());
          }
          if (t.type() == nullptr && t.kind() == QualifiedType::UNKNOWN)
            foundSubstitutionDefaultHint = true;
          else
            foundSubstitution = true;
        }
      }
    }

    // use the type from the type expression; if this is a `this` formal
    // for a primary method, there's no type expression, but we should
    // act as if there was one.
    if (!foundSubstitution || ignoreSubstitutionFor == decl) {
      if (typeExpr) {
        // get the type we should have already computed postorder
        ResolvedExpression& r = byPostorder.byAst(typeExpr);
        typeExprT = r.type();
        // otherwise, typeExprT can be empty/null
      } else if (isFormalThis) {
        // We're a primary method `this` formal (which do not have type
        // expressions). This means we don't have to go through searching
        // scopes for the ID the formal refers to: it's the ID of the
        // enclosing record or class.
        auto functionId = parsing::idToParentId(context, decl->id());
        auto aggregateId = parsing::idToParentId(context, functionId);
        auto parentType = typeForId(aggregateId, /* localGenericToUnknown */ true);
        typeExprT = parentType;
      }

      // for 'this' formals of class type, adjust them to be borrowed, so
      // e.g. proc C.foo() { } has 'this' of type 'borrowed C'.
      // This should not apply to parenthesized expressions.
      //
      // An exception to this is when we are looking at a type method or
      // a parenless, type- or param returning method. In that case, the
      // decorator is 'anymanaged any-nilable', not borrowed nonnil.
      bool identOrNoTypeExpr = !typeExpr || typeExpr->isIdentifier();
      bool isClassType = typeExprT.type() && typeExprT.type()->isClassType();
      if (isFormalThis && isClassType && identOrNoTypeExpr) {
        auto dec = ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
        bool useFullyGenericDecorator = false;

        if (auto varLikeDecl = decl->toVarLikeDecl()) {
          useFullyGenericDecorator |=
            varLikeDecl->storageKind() == QualifiedType::TYPE;
        }

        if (symbol && symbol->isFunction()) {
          auto fn = symbol->toFunction();
          useFullyGenericDecorator |=
            (fn->returnIntent() == Function::TYPE ||
             fn->returnIntent() == Function::PARAM) &&
            fn->isParenless();
        }

        if (useFullyGenericDecorator) {
          dec = ClassTypeDecorator(ClassTypeDecorator::GENERIC);
        }

        auto ct = typeExprT.type()->toClassType();
        typeExprT = QualifiedType(typeExprT.kind(),
                                  ct->withDecorator(context, dec),
                                  typeExprT.param());
      }
    }

    if (initExpr && !foundSubstitution) {
      // compute the type based upon the init expression
      ResolvedExpression& r = byPostorder.byAst(initExpr);
      initExprT = r.type();
    }

    if (typeExpr == nullptr && initExpr == nullptr &&
        var->linkage() == uast::Decl::EXTERN &&
        var->storageKind() == QualifiedType::TYPE) {
      // creating an opaque external type.

      CHPL_ASSERT(var->isVariable());
      auto linkageNameNode = var->linkageName();
      CHPL_ASSERT(linkageNameNode == nullptr || linkageNameNode->isStringLiteral());

      UniqueString linkageName = var->name();
      if (linkageNameNode) {
        linkageName = linkageNameNode->toStringLiteral()->value();
      }
      qtKind = QualifiedType::TYPE;
      typePtr = ExternType::get(context, linkageName, var->id());
      paramPtr = nullptr;
    } else if (!typeExprT.hasTypePtr() && useType != nullptr) {
      // use type from argument to resolveNamedDecl
      typeExprT = QualifiedType(QualifiedType::TYPE, useType);
      typePtr = typeExprT.type();
    } else if (foundSubstitution && ignoreSubstitutionFor != decl) {
      // if we are working with a substitution, just use that
      // without doing lots of kinds checking
      typePtr = typeExprT.type();
      if (qtKind == QualifiedType::PARAM)
        paramPtr = typeExprT.param();
    } else {
      if (isFieldOrFormal &&
          typeExpr == nullptr && !isFormalThis &&
          initExpr == nullptr) {
        // Lack of initializer for a field/formal means the Any type.
        //
        // However, a `this` formal lacks a type expression if it belongs to a
        // primary method. This does not, however, mean that its type should be
        // AnyType; it is not adjusted here.

        typeExprT = QualifiedType(QualifiedType::TYPE, AnyType::get(context));
      } else if (isFieldOrFormal) {
        // figure out if we should potentially infer the type from the init expr
        // (we do so if it's not a field or a formal)
        bool isTypeOrParam = qtKind == QualifiedType::TYPE ||
                             qtKind == QualifiedType::PARAM;
        // infer the type of the variable from its initialization expr?
        bool inferFromInit = foundSubstitutionDefaultHint ||
                             defaultsPolicy == DefaultsPolicy::USE_DEFAULTS;
        // in addition, always infer from init for a concrete type.
        // the non-concrete cases are like this, e.g.:
        //    type t = int;
        //    var x:GenericRecord = f()
        if (inferFromInit == false && !isTypeOrParam)  {
          // check also for a generic type as the type expression
          auto g = getTypeGenericity(context, typeExprT);
          if (g != Type::GENERIC) {
            inferFromInit = true;
          }
        }

        if (!inferFromInit) {
          // if we aren't inferring from the init expr, clear initExprT
          // so it is not used below.
          initExprT = QualifiedType();

          if (isTypeOrParam && isFieldOrFormal && typeExprT == QualifiedType()) {
            // a type or param field with initExpr is still generic, if
            // it doesn't already have a type-expr, e.g.
            // record R { type t = int; }
            // if that behavior is requested with defaultsPolicy == IGNORE_DEFAULTS
            typeExprT = QualifiedType(QualifiedType::TYPE,
                                      AnyType::get(context));
          }
        }
      }

      // for an initializer expression that is the default value of a formal,
      // check if we have the formal type already and then use that to
      // compute the qtKind now rather than trying to getTypeForDecl first,
      // which will fail in cases where an implicit conversion is needed
      if (!isVarArgs && typeExprT.hasTypePtr() &&
          (isFormal || (signatureOnly && isField))) {
        // update qtKind with the result of resolving the intent
        if (!typeExprT.type()->isTupleType()) {
          // skip for tuple types, which are handled along with ref/value below
          computeFormalIntent(decl, qtKind, typeExprT.type(), typeExprT.param());
        }
      }
      // Check that the initExpr type is compatible with declared type
      // Check kinds are OK
      // Handle any implicit conversions / instantiations
      auto qt = getTypeForDecl(decl, typeExpr, initExpr,
                               qtKind, typeExprT, initExprT);
      typePtr = qt.type();
      paramPtr = qt.param();
    }
  }

  if (typePtr == nullptr) {
    if (qtKind == QualifiedType::FUNCTION ||
        qtKind == QualifiedType::MODULE) {
      // OK, type can be null for now
    } else {
      // typePtr should not be null; it should use UnknownType instead.
      // it can be UnknownType for a variable that is split inited.
      typePtr = UnknownType::get(context);
    }
  }

  // param value should not be set if the QualifiedType is not param
  CHPL_ASSERT(qtKind == QualifiedType::PARAM || paramPtr == nullptr);

  auto declaredKind = qtKind;

  if (isVarArgs) {
    typePtr = computeVarArgTuple(*this, decl->toVarArgFormal(),
                                 qtKind, typePtr);
  }

  // adjust tuple declarations for value / referential tuples
  if (typePtr != nullptr && decl->isVarArgFormal() == false) {
    if (auto tupleType = typePtr->toTupleType()) {
      if (declaredKind == QualifiedType::DEFAULT_INTENT) {
        typePtr = tupleType->toReferentialTuple(context);
        qtKind = QualifiedType::CONST_REF;
      } else if (declaredKind == QualifiedType::CONST_INTENT) {
        typePtr = tupleType->toReferentialTuple(context, /* makeConst */ true);
        qtKind = QualifiedType::CONST_REF;
      } else if (qtKind == QualifiedType::CONST_IN ||
                 qtKind == QualifiedType::CONST_REF) {
        typePtr = tupleType->toValueTuple(context, /* makeConst */ true);
      } else if (qtKind == QualifiedType::VAR ||
                 qtKind == QualifiedType::CONST_VAR ||
                 qtKind == QualifiedType::REF ||
                 qtKind == QualifiedType::IN ||
                 qtKind == QualifiedType::OUT ||
                 qtKind == QualifiedType::INOUT ||
                 qtKind == QualifiedType::TYPE) {
        typePtr = tupleType->toValueTuple(context);
      }
    }
  }

  ResolvedExpression& result = byPostorder.byAst(decl);
  result.setType(QualifiedType(qtKind, typePtr, paramPtr));
}

void Resolver::computeFormalIntent(const uast::NamedDecl *decl,
                                   types::QualifiedType::Kind &qtKind,
                                   const types::Type *typePtr,
                                   const types::Param *paramPtr) {
  // compute the intent for formals (including type constructor formals)
  bool isThis = decl->name() == USTR("this");
  bool isInit = false;
  if (symbol) {
    if (auto named = symbol->toNamedDecl()) {
      isInit = named->name() == USTR("init") ||
               named->name() == USTR("init=");
    }
  }
  auto formalQt = QualifiedType(qtKind, typePtr, paramPtr);
  // update qtKind with the result of resolving the intent
  qtKind = resolveIntent(formalQt, isThis, isInit);
}

void
Resolver::issueErrorForFailedCallResolution(const uast::AstNode* astForErr,
                                            const CallInfo& ci,
                                            const CallResolutionResult& c) {
  if (c.mostSpecific().isEmpty()) {
    // if the call resolution result is empty, we need to issue an error
    if (c.mostSpecific().isAmbiguous()) {
      // ambiguity between candidates
      context->error(astForErr, "Cannot resolve call to '%s': ambiguity",
                     ci.name().c_str());
    } else {
      // could not find a most specific candidate
      std::vector<ApplicabilityResult> rejected;
      CHPL_REPORT(context, NoMatchingCandidates, astForErr, ci, rejected);
    }
  } else {
    context->error(astForErr, "Cannot establish type for call expression");

    // expecting call site to check for hasTypePtr.
    CHPL_ASSERT(!c.exprType().hasTypePtr());
  }
}

void Resolver::issueErrorForFailedModuleDot(const Dot* dot,
                                            ID moduleId,
                                            LookupConfig config) {
  LookupConfig configWithPrivate = config & ~LOOKUP_SKIP_PRIVATE_VIS;
  bool thereButPrivate = false;
  // No need to do the search if we already did search for private symbols
  if (configWithPrivate != config) {
    auto modScope = scopeForModule(context, moduleId);
    auto ids = lookupNameInScope(context, modScope,
                                 /* methodLookupHelper */ nullptr,
                                 /* receiverScopeHelper */ nullptr,
                                 dot->field(), configWithPrivate);
    for (auto& id : ids) {
      // Only report "bla is private" if it's originally declared in the
      // given module (i.e., don't do so if the found ID is imported from
      // elsewhere)
      auto modId = parsing::idToParentModule(context, id);
      if (modId != moduleId) continue;

      thereButPrivate = true;
    }
  }

  // figure out what name was used for the module in the Dot expression
  auto modName = moduleId.symbolName(context);
  auto dotModName = modName;
  ID renameClauseId;
  if (auto dotLeftPart = dot->receiver()) {
    if (auto leftIdent = dotLeftPart->toIdentifier()) {
      dotModName = leftIdent->name();
    }
  }
  if (modName != dotModName) {
    // get a trace for where the module was renamed so that
    // the error can show line numbers
    CHPL_ASSERT(scopeStack.size() > 0);
    const Scope* scope = scopeStack.back();
    std::vector<ResultVisibilityTrace> trace;
    lookupNameInScopeTracing(context, scope,
                             /* methodLookupHelper */ nullptr,
                             /* receiverScopeHelper */ nullptr,
                             dotModName,
                             LOOKUP_DECLS | LOOKUP_IMPORT_AND_USE |
                             LOOKUP_PARENTS | LOOKUP_INNERMOST |
                             LOOKUP_EXTERN_BLOCKS,
                             trace);
    // find the last rename in the trace
    for (const auto& t : trace) {
      for (const auto& elt : t.visibleThrough) {
        if (elt.fromUseImport && elt.renameFrom != dotModName) {
          renameClauseId = elt.visibilityClauseId;
        }
      }
    }
  }

  CHPL_REPORT(context, NotInModule, dot, moduleId, modName, renameClauseId, thereButPrivate);

}

bool Resolver::handleResolvedCallWithoutError(ResolvedExpression& r,
                                              const uast::AstNode* astForErr,
                                              const CallInfo& ci,
                                              const CallResolutionResult& c,
                                              optional<ActionAndId> actionAndId) {

  if (!c.exprType().hasTypePtr()) {
    if (!actionAndId) {
      // Only set the type to erroneous if we're handling an actual user call,
      // and not an associated action.
      r.setType(QualifiedType(r.type().kind(), ErroneousType::get(context)));
      r.setMostSpecific(c.mostSpecific());
    }

    // If the call was specially handled, assume special-case logic has already
    // issued its own error, so we shouldn't emit a general error.
    return !c.speciallyHandled();
  } else {
    if (actionAndId) {
      // save candidates as associated functions
      for (auto& sig : c.mostSpecific()) {
        if (sig) {
          r.addAssociatedAction(std::get<0>(*actionAndId), sig.fn(),
                                std::get<1>(*actionAndId));
        }
      }
    } else {
      r.setPoiScope(c.poiInfo().poiScope());
      r.setType(c.exprType());
      validateAndSetMostSpecific(r, astForErr, c.mostSpecific());
    }
    // gather the poi scopes used when resolving the call
    poiInfo.accumulate(c.poiInfo());
  }
  return false;
}

void Resolver::handleResolvedCall(ResolvedExpression& r,
                                  const uast::AstNode* astForErr,
                                  const CallInfo& ci,
                                  const CallResolutionResult& c,
                                  optional<ActionAndId> actionAndId) {
  if (handleResolvedCallWithoutError(r, astForErr, ci, c, std::move(actionAndId))) {
    issueErrorForFailedCallResolution(astForErr, ci, c);
  }
}

void Resolver::handleResolvedCallPrintCandidates(ResolvedExpression& r,
                                                 const uast::Call* call,
                                                 const CallInfo& ci,
                                                 const CallScopeInfo& inScopes,
                                                 const QualifiedType& receiverType,
                                                 const CallResolutionResult& c,
                                                 optional<ActionAndId> actionAndId) {
  bool wasCallGenerated = (bool) actionAndId;
  CHPL_ASSERT(!wasCallGenerated || receiverType.isUnknown());
  if (handleResolvedCallWithoutError(r, call, ci, c, std::move(actionAndId))) {
    if (c.mostSpecific().isEmpty() &&
        !c.mostSpecific().isAmbiguous()) {
      // The call isn't ambiguous; it might be that we rejected all the candidates
      // that we encountered. Re-run resolution, providing a 'rejected' vector
      // this time to preserve the list of rejected candidates.

      std::vector<ApplicabilityResult> rejected;

      if (wasCallGenerated) {
        std::ignore = resolveGeneratedCall(context, call, ci, inScopes, &rejected);
      } else {
        std::ignore = resolveCallInMethod(context, call, ci, inScopes,
                                          receiverType, &rejected);
      }

      if (!rejected.empty()) {
        // There were candidates but we threw them out. We can issue a nicer
        // error explaining why each candidate was rejected.
        CHPL_REPORT(context, NoMatchingCandidates, call, ci, rejected);
        return;
      }
    }

    // Fall through to the more general error handling.
    issueErrorForFailedCallResolution(call, ci, c);
  }
}

void Resolver::adjustTypesForSplitInit(ID id,
                                       const QualifiedType& rhsType,
                                       const AstNode* lhsExprAst,
                                       const AstNode* astForError) {
  if (id.isEmpty()) {
    return;
  }

  // what variable does the LHS refer to? is it within the purview of this
  // Resolver?
  bool useLocalResult = (id.symbolPath() == symbol->id().symbolPath() &&
                         !id.isSymbolDefiningScope());

  // if the the lhs variable has not been initialized and
  // the type of LHS is generic or unknown, update it based on the RHS type.

  if (!useLocalResult) {
    return;
  }

  ResolvedExpression& lhs = byPostorder.byId(id);
  QualifiedType lhsType = lhs.type();

  if (!lhsType.needsSplitInitTypeInfo(context)) return;

  const Param* p = rhsType.param();
  auto useKind = lhsType.kind();
  if (p && symbol->isModule()) {
    // This is a white lie since if we are in a module-level statement, the
    // kind we see here will always be UNKNOWN, whether this is a PARAM or not.
    // This is because module statements are resolved separately from each
    // other.
    // Set it to PARAM here so we are allowed to store a param value for
    // it, else we get a complaint about storing a param value on a non-param
    // kind. The param kind will only get preserved later (at module-level
    // resolution) if it actually is a PARAM decl.
    useKind = QualifiedType::PARAM;
  }
  if (useKind != QualifiedType::PARAM) p = nullptr;
  const auto useType = QualifiedType(useKind, rhsType.type(), p);

  lhs.setType(useType);
  if (lhsExprAst != nullptr) {
    ResolvedExpression& lhsExpr = byPostorder.byAst(lhsExprAst);
    lhsExpr.setType(useType);
  }
}

// handle adjusting a variable's type to support split init
void Resolver::adjustTypesOnAssign(const AstNode* lhsAst,
                                   const AstNode* rhsAst) {

  ResolvedExpression& lhsExpr = byPostorder.byAst(lhsAst);

  ID id = lhsExpr.toId();
  if (id.isEmpty()) {
    return;
  }

  QualifiedType rhsType = byPostorder.byAst(rhsAst).type();

  adjustTypesForSplitInit(id, rhsType, lhsAst, rhsAst);
}

// Update a variable's type if it is generic/unknown
// and the variable is initialized by an 'out' formal
void
Resolver::adjustTypesForOutFormals(const CallInfo& ci,
                                   const std::vector<const AstNode*>& asts,
                                   const MostSpecificCandidates& fns) {

  std::vector<QualifiedType> actualFormalTypes;
  std::vector<Qualifier> actualIntents;
  computeActualFormalIntents(context, fns, ci, asts,
                             actualIntents, actualFormalTypes);

  int actualIdx = 0;
  for (const auto& actual : ci.actuals()) {
    (void) actual; // avoid compilation error about unused variable

    // find an actual referring to an ID that is passed to an 'out' formal
    ID id;
    const AstNode* actualAst = asts[actualIdx];
    if (actualAst != nullptr && byPostorder.hasAst(actualAst)) {
      id = byPostorder.byAst(actualAst).toId();
    }
    if (actualIntents[actualIdx] == Qualifier::OUT && !id.isEmpty()) {
      QualifiedType formalType = actualFormalTypes[actualIdx];
      adjustTypesForSplitInit(id, formalType, actualAst, actualAst);
    }
    actualIdx++;
  }
}

void Resolver::resolveTupleUnpackAssign(ResolvedExpression& r,
                                        const uast::AstNode* astForErr,
                                        const Tuple* lhsTuple,
                                        QualifiedType lhsType,
                                        QualifiedType rhsType) {
  // Check that lhsType = rhsType can work

  if (!lhsType.hasTypePtr()) {
    context->error(lhsTuple, "Unknown lhs tuple type in split tuple assign");
    return;
  }
  if (!rhsType.hasTypePtr()) {
    context->error(lhsTuple, "Unknown rhs tuple type in split tuple assign");
    return;
  }

  // First, check that lhsType and rhsType are tuples
  const TupleType* lhsT = lhsType.type()->toTupleType();
  const TupleType* rhsT = rhsType.type()->toTupleType();

  if (lhsT == nullptr) {
    context->error(lhsTuple, "lhs type is not tuple in split tuple assign");
    return;
  }
  if (rhsT == nullptr) {
    context->error(lhsTuple, "rhs type is not tuple in split tuple assign");
    return;
  }

  // Then, check that they have the same size
  if (lhsTuple->numActuals() != rhsT->numElements()) {
    context->error(lhsTuple, "tuple size mismatch in split tuple assign");
    return;
  }
  if (lhsT->numElements() != rhsT->numElements()) {
    context->error(lhsTuple, "tuple size mismatch in split tuple assign");
    return;
  }

  CHPL_ASSERT(scopeStack.size() > 0);
  const Scope* scope = scopeStack.back();
  auto inScopes = CallScopeInfo::forNormalCall(scope, poiScope);

  // Finally, try to resolve = between the elements
  int i = 0;
  for (auto actual : lhsTuple->actuals()) {
    QualifiedType lhsEltType = lhsT->elementType(i);
    QualifiedType rhsEltType = rhsT->elementType(i);
    if (auto innerTuple = actual->toTuple()) {
      resolveTupleUnpackAssign(r, astForErr, innerTuple, lhsEltType, rhsEltType);
    } else {
      std::vector<CallInfoActual> actuals;
      actuals.push_back(CallInfoActual(lhsEltType, UniqueString()));
      actuals.push_back(CallInfoActual(rhsEltType, UniqueString()));
      auto ci = CallInfo (/* name */ USTR("="),
                          /* calledType */ QualifiedType(),
                          /* isMethodCall */ false,
                          /* hasQuestionArg */ false,
                          /* isParenless */ false,
                          actuals);

      auto c = resolveGeneratedCall(context, actual, ci, inScopes);
      handleResolvedCall(r, astForErr, ci, c,
                         { { AssociatedAction::ASSIGN, lhsTuple->id() } });
    }
    i++;
  }
}

void Resolver::resolveTupleUnpackDecl(const TupleDecl* lhsTuple,
                                      QualifiedType rhsType) {
  if (!rhsType.hasTypePtr()) {
    CHPL_REPORT(context, TupleDeclUnknownType, lhsTuple);
    return;
  }

  const TupleType* rhsT = rhsType.type()->toTupleType();
  std::vector<QualifiedType> eltTypes;

  if (rhsT == nullptr) {
    if (rhsType.isUnknown() ||
        rhsType.isErroneousType() ||
        (rhsType.hasTypePtr() && rhsType.type()->isAnyType())) {
      // Fill element types with the unknown/generic QualifiedType, so that we
      // at least get the 'kind' right
      eltTypes = std::vector<QualifiedType>(lhsTuple->numDecls(), rhsType);
    } else {
      CHPL_REPORT(context, TupleDeclNotTuple, lhsTuple, rhsType.type());
      return;
    }
  } else if (lhsTuple->numDecls() != rhsT->numElements()) {
    // Then, check that they have the same size
    CHPL_REPORT(context, TupleDeclMismatchedElems, lhsTuple, rhsT);
    return;
  } else {
    for (int i = 0; i < rhsT->numElements(); i++) {
      eltTypes.push_back(rhsT->elementType(i));
    }
  }

  // Finally, try to resolve the types of the elements
  int i = 0;
  for (auto actual : lhsTuple->decls()) {
    QualifiedType rhsEltType = eltTypes[i];
    if (auto innerTuple = actual->toTupleDecl()) {
      resolveTupleUnpackDecl(innerTuple, rhsEltType);
    } else if (auto namedDecl = actual->toNamedDecl()) {
      resolveNamedDecl(namedDecl, rhsEltType.type());
    } else {
      CHPL_ASSERT(false && "case not handled");
    }
    i++;
  }
}

void Resolver::resolveTupleDecl(const TupleDecl* td,
                                const Type* useType) {
  if (scopeResolveOnly) {
    return;
  }

  QualifiedType::Kind declKind = (Qualifier) td->intentOrKind();
  QualifiedType useT;

  // Non-default intents are currently not allowed for tuple-grouped formals.
  //
  // Note: This doesn't apply to nested TupleDecls like ``(a, b)`` in the
  // formal ``((a, b), c)``. Such nested formals should be handled by
  // ``resolveTupleUnpackDecl``.
  if (td->isTupleDeclFormal() && declKind != QualifiedType::DEFAULT_INTENT) {
    useT = QualifiedType(declKind, ErroneousType::get(context));
    ResolvedExpression& result = byPostorder.byAst(td);
    result.setType(useT);
    return;
  }

  // Figure out the type to use for this tuple
  if (useType != nullptr) {
    useT = QualifiedType(declKind, useType);
  } else if (substitutions != nullptr &&
             substitutions->count(td->id()) != 0) {
    auto sub = substitutions->find(td->id())->second;
    auto subTup = sub.hasTypePtr() ? sub.type()->toTupleType() : nullptr;

    if (subTup == nullptr) {
      // Rely on hasTypePtr check below to recognize error
      useT = QualifiedType();
    } else {
      useT = sub;
    }
  } else {
    QualifiedType typeExprT;
    QualifiedType initExprT;

    auto typeExpr = td->typeExpression();
    auto initExpr = td->initExpression();

    if (typeExpr == nullptr && initExpr == nullptr) {
      // Note: we seem to rely on tuple components being 'var', and relying on
      // the tuple's kind instead. Without this, the current instantiation
      // logic won't allow, for example, passing (1, 2, 3) to (?, ?, ?).
      auto anyType = QualifiedType(QualifiedType::VAR, AnyType::get(context));
      std::vector<QualifiedType> eltTypes(td->numDecls(), anyType);
      auto tup = TupleType::getQualifiedTuple(context, eltTypes);
      useT = QualifiedType(declKind, tup);
    } else {
      if (typeExpr != nullptr) {
        ResolvedExpression& result = byPostorder.byAst(typeExpr);
        typeExprT = result.type();
      }
      if (initExpr != nullptr) {
        ResolvedExpression& result = byPostorder.byAst(initExpr);
        initExprT = result.type();
      }

      useT = getTypeForDecl(td, typeExpr, initExpr,
                            declKind, typeExprT, initExprT);

    }
  }

  if (!useT.hasTypePtr()) {
    context->error(td, "Cannot establish type for tuple decl");
    useT = QualifiedType(declKind, ErroneousType::get(context));
  } else if (useT.type()->isTupleType()) {
    useT = QualifiedType(useT.kind(),
                         useT.type()->toTupleType()->toReferentialTuple(context));
  }

  // save the type in byPostorder
  ResolvedExpression& result = byPostorder.byAst(td);
  result.setType(useT);
  // resolve the types of the tuple elements
  resolveTupleUnpackDecl(td, useT);
}

bool Resolver::resolveSpecialNewCall(const Call* call) {
  if (!call->calledExpression() ||
      !call->calledExpression()->isNew()) {
    return false;
  }

  auto newExpr = call->calledExpression()->toNew();
  auto& re = byPostorder.byAst(call);
  auto& reNewExpr = byPostorder.byAst(newExpr);
  auto qtNewExpr = reNewExpr.type();


  // exit immediately if the 'new' failed to resolve
  if (qtNewExpr.isUnknown() || qtNewExpr.isErroneousType()) {
    return true;
  }

  // Remove nilability from e.g., 'new C?()' for the init call (or else it
  // will not resolve because the receiver formal is 'nonnil borrowed').
  const Type* initReceiverType = qtNewExpr.type();
  if (auto clsType = qtNewExpr.type()->toClassType()) {
    auto oldDecor = clsType->decorator();
    auto newDecor = oldDecor.addNonNil();
    initReceiverType = clsType->withDecorator(context, newDecor);
    CHPL_ASSERT(initReceiverType);
  }

  // The 'new' will produce an 'init' call as a side effect.
  UniqueString name = USTR("init");

  /*
  auto cls = qtNewExpr.type()->toClassType();
  CHPL_ASSERT(cls);
  auto calledIntent = cls->manager() ? QualifiedType::REF
                                     : QualifiedType::CONST_IN;
  */
  // TODO: Unclear to me whether we should use this type or the above.
  auto calledType = QualifiedType(qtNewExpr.kind(), initReceiverType);
  bool isMethodCall = true;
  const AstNode* questionArg = nullptr;
  std::vector<CallInfoActual> actuals;

  // Prepare receiver.
  auto receiverInfo = CallInfoActual(calledType, USTR("this"));
  actuals.push_back(std::move(receiverInfo));

  // Remaining actuals.
  if (call->numActuals()) {
    prepareCallInfoActuals(call, actuals, questionArg);
    CHPL_ASSERT(!questionArg);
  }

  auto ci = CallInfo(name, calledType, isMethodCall,
                     /* hasQuestionArg */ questionArg != nullptr,
                     /* isParenless */ false,
                     std::move(actuals));
  auto inScope = scopeStack.back();
  auto inPoiScope = poiScope;
  auto inScopes = CallScopeInfo::forNormalCall(inScope, inPoiScope);

  // note: the resolution machinery will get compiler generated candidates
  auto crr = resolveGeneratedCall(context, call, ci, inScopes);
  handleResolvedCallPrintCandidates(re, call, ci, inScopes, QualifiedType(), crr,
                                    { { AssociatedAction::NEW_INIT, call->id() } });


  // there should be one or zero applicable candidates
  CHPL_ASSERT(crr.mostSpecific().numBest() <= 1);
  if (auto initMsc = crr.mostSpecific().only()) {
    auto initTfs = initMsc.fn();

    // Set the final output type based on the result of the 'new' call.
    auto qtInitReceiver = initTfs->formalType(0);
    auto type = qtInitReceiver.type();

    // Preserve original management if receiver is a class.
    if (auto cls = type->toClassType()) {
      auto newCls = qtNewExpr.type()->toClassType();
      CHPL_ASSERT(newCls);
      CHPL_ASSERT(!cls->manager() && cls->decorator().isNonNilable());
      CHPL_ASSERT(cls->decorator().isBorrowed());
      CHPL_ASSERT(cls->basicClassType());
      type = ClassType::get(context, cls->basicClassType(),
                            newCls->manager(),
                            newCls->decorator());
    }

    auto qt = QualifiedType(QualifiedType::VAR, type);
    re.setType(qt);
  }

  return true;
}

bool Resolver::resolveSpecialOpCall(const Call* call) {
  if (!call->isOpCall()) return false;

  auto op = call->toOpCall();

  if (op->op() == USTR("=")) {
    if (op->numActuals() == 2) {
      if (auto lhsTuple = op->actual(0)->toTuple()) {

        ResolvedExpression& r = byPostorder.byAst(op);
        QualifiedType lhsType = byPostorder.byAst(op->actual(0)).type();
        QualifiedType rhsType = byPostorder.byAst(op->actual(1)).type();

        resolveTupleUnpackAssign(r, call,
                                 lhsTuple, lhsType, rhsType);
        return true;
      }

      // Update a generic/unknown type when split-init is used.
      adjustTypesOnAssign(op->actual(0), op->actual(1));
    }
  } else if (op->op() == USTR("...")) {
    // just leave it unknown -- tuple expansion only makes sense
    // in the argument list for another call.
    return true;
  }

  return false;
}

bool Resolver::resolveSpecialPrimitiveCall(const Call* call) {
  auto primCall = call->toPrimCall();
  if (!primCall) return false;

  if (primCall->prim() == PRIM_RESOLVES) {
    QualifiedType result;

    if (primCall->numActuals() != 1) {
      result = typeErr(primCall, "invalid call to \"resolves\" primitive");
    } else {
      auto resultAndErrors = context->runAndTrackErrors([&](Context* context) {
        primCall->actual(0)->traverse(*this);
        return byPostorder.byAst(primCall->actual(0)).type();
      });

      bool resultBool = true;
      if (!resultAndErrors.ranWithoutErrors()) {
        // Errors were emitted, so even if we have a type, it doesn't
        // "resolve".
        resultBool = false;
      } else if (resultAndErrors.result().isErroneousType() ||
                 resultAndErrors.result().isUnknown()) {
        // We got an erroneous or unknown type, so it didn't resolve.
        resultBool = false;
      }

      result = QualifiedType(QualifiedType::PARAM, BoolType::get(context),
                             BoolParam::get(context, resultBool));
    }

    byPostorder.byAst(primCall).setType(result);
    return true;
  }

  return false;
}

bool Resolver::resolveSpecialKeywordCall(const Call* call) {
  if (!call->isFnCall()) return false;

  auto fnCall = call->toFnCall();
  if (!fnCall->calledExpression()->isIdentifier()) return false;

  auto fnName = fnCall->calledExpression()->toIdentifier()->name();
  if (fnName == "index") {
    auto runResult = context->runAndTrackErrors([&](Context* ctx) {
      auto ci = CallInfo::create(context, call, byPostorder,
                                 /* raiseErrors */ true,
                                 /* actualAsts */ nullptr,
                                 /* moduleScopeId */ nullptr,
                                 /* rename */ UniqueString::get(context, "chpl__buildIndexType"));
      auto scope = scopeStack.back();
      auto inScopes = CallScopeInfo::forNormalCall(scope, poiScope);
      auto result = resolveGeneratedCall(context, call, ci, inScopes);

      auto& r = byPostorder.byAst(call);
      handleResolvedCall(r, call, ci, result);
      return result;
    });

    if (!runResult.ranWithoutErrors()) {
      auto firstActual = QualifiedType();
      if (call->numActuals() > 0) {
        firstActual = byPostorder.byAst(call->actual(0)).type();
      }
      CHPL_REPORT(context, InvalidIndexCall, fnCall, firstActual);
    }
    return true;
  }

  return false;
}

bool Resolver::resolveSpecialCall(const Call* call) {
  if (resolveSpecialOpCall(call)) {
    return true;
  } else if (resolveSpecialPrimitiveCall(call)) {
    return true;
  } else if (resolveSpecialNewCall(call)) {
    return true;
  } else if (resolveSpecialKeywordCall(call)) {
    return true;
  }

  return false;
}

QualifiedType Resolver::typeForId(const ID& id, bool localGenericToUnknown) {
  if (scopeResolveOnly) {
    auto kind = qualifiedTypeKindForId(context, id);
    const Type* type = nullptr;
    return QualifiedType(kind, type);
  }

  // Intercept the standard library `c_ptr` and `c_ptrConst` and turn them into
  // the builtin type.
  if (id == CPtrType::getId(context)) {
    return QualifiedType(QualifiedType::TYPE, CPtrType::get(context));
  } else if (id == CPtrType::getConstId(context)) {
    return QualifiedType(QualifiedType::TYPE, CPtrType::getConst(context));
  }

  // if the id is contained within this symbol,
  // get the type information from the resolution result.
  //
  // when resolving a module statement, the resolution result only
  // contains things within that statement.
  if (parentResolver != nullptr) {
    const Scope* idScope = scopeForId(context, id);
    bool local = false;
    for (auto sc : scopeStack) {
      if (sc == idScope) {
        local = true;
      }
    }

    if (!local) {
      return parentResolver->typeForId(id, localGenericToUnknown);
    }
  }

  bool useLocalResult = (id.symbolPath() == symbol->id().symbolPath() &&
                         !id.isSymbolDefiningScope());
  bool error = false;
  if (useLocalResult && curStmt != nullptr) {
    if (curStmt->id().contains(id)) {
      // OK, proceed using local result
    } else {
      useLocalResult = false;
      // attempting to get a type for a value that has a later post-order ID
      // than curStmt should result in an error since we want resolution to
      // behave as though things are resolved in order.
      if (id.postOrderId() > curStmt->id().postOrderId()) {
        error = true;
      }
    }
  }

  if (error) {
    CHPL_REPORT(context, UseOfLaterVariable, curStmt, id);
    auto unknownType = UnknownType::get(context);
    return QualifiedType(QualifiedType::UNKNOWN, unknownType);
  }

  if (useLocalResult) {
    QualifiedType ret = byPostorder.byId(id).type();
    auto g = Type::MAYBE_GENERIC;
    if (ret.hasTypePtr()) {
      g = getTypeGenericity(context, ret.type());
    }

    if (g != Type::CONCRETE && shouldUseUnknownTypeForGeneric(id)) {
      // if id refers to a field or formal that needs to be instantiated,
      // replace the type with UnknownType since we can't compute
      // the type of anything using this type (since it will change
      // on instantiation).
      auto unknownType = UnknownType::get(context);
      ret = QualifiedType(ret.kind(), unknownType);
    }

    return ret;
  }

  // Otherwise, use a query to try to look it up.

  // Look up class/record types (used for primary method receivers)
  auto tag = parsing::idToTag(context, id);
  if (asttags::isAggregateDecl(tag)) {
    const Type* t = initialTypeForTypeDecl(context, id);
    return QualifiedType(QualifiedType::TYPE, t);
  } else if (asttags::isModule(tag)) {
    return QualifiedType(QualifiedType::MODULE, nullptr);
  }

  if (asttags::isFunction(tag)) {
    // TODO: use real function types
    auto kind = qualifiedTypeKindForId(context, id);
    const Type* type = nullptr;
    return QualifiedType(kind, type);
  }

  if (id.isFabricatedId()) {
    switch (id.fabricatedIdKind()) {
      case ID::ExternBlockElement: {
        // TODO: resolve types for extern block
        // (will need the Identifier name for that)
        auto unknownType = UnknownType::get(context);
        return QualifiedType(QualifiedType::UNKNOWN, unknownType);
      }
      case ID::Generated: break;
    }
  }

  // Figure out what ID is contained within so we can use the
  // appropriate query.
  ID parentId = id.parentSymbolId(context);
  auto parentTag = asttags::AST_TAG_UNKNOWN;
  if (!parentId.isEmpty()) {
    parentTag = parsing::idToTag(context, parentId);
  }

  if (asttags::isModule(parentTag)) {
    // If the id is contained within a module, use typeForModuleLevelSymbol.
    bool isCurrentModule =
        parsing::idToAst(context, parentId)->toModule() == symbol->toModule();
    return typeForModuleLevelSymbol(context, id, isCurrentModule);
  }

  if (asttags::isEnum(parentTag) && asttags::isEnumElement(tag)) {
    return typeForScopeResolvedEnumElement(parentId, id, /* ambiguous */ false);
  }

  // If the id is contained within a class/record/union that we are resolving,
  // get the resolved field.
  const CompositeType* ct = nullptr;
  if (parentId == symbol->id() && inCompositeType) {
    ct = inCompositeType;
  } else {
    // TODO: this needs to consider the possibility
    // that we are working with a nested method
    if (auto rt = methodReceiverType().type()) {
      if (auto comprt = rt->getCompositeType()) {
        if (comprt->id() == parentId) {
          ct = comprt; // handle record, class with field
        } else if (auto bct = comprt->toBasicClassType()) {
          // if it's a class, check for parent classes to decide
          // which type corresponds to the uAST ID parentId
          while (bct != nullptr) {
            if (bct->id() == parentId) {
              // found the matching type
              ct = bct;
              break;
            }
            // otherwise, try the parent class type
            bct = bct->parentClassType();
          }
        }
      }
    }
  }

  if (ct) {
    auto newDefaultsPolicy = defaultsPolicy;
    if (defaultsPolicy == DefaultsPolicy::USE_DEFAULTS_OTHER_FIELDS &&
        ct == inCompositeType) {
      // The USE_DEFAULTS_OTHER_FIELDS policy is supposed to make
      // the Resolver act as if it was running with IGNORE_DEFAULTS
      // at first, but then switch to USE_DEFAULTS for all other fields
      // of the type being resolved. This branch implements the switch:
      // if we're moving on to resolving another field, and if this
      // field is from the current type, we resolve that field with
      // USE_DEFAULTS.
      newDefaultsPolicy = DefaultsPolicy::USE_DEFAULTS;
    }
    // if it is recursive within the current class/record, we can
    // call resolveField.
    const ResolvedFields& resolvedFields =
      resolveFieldDecl(context, ct, id, newDefaultsPolicy);
    // find the field that matches
    int nFields = resolvedFields.numFields();
    for (int i = 0; i < nFields; i++) {
      if (resolvedFields.fieldDeclId(i) == id) {
        return resolvedFields.fieldType(i);
      }
    }

    CHPL_ASSERT(false && "could not find resolved field");
  }

  // Otherwise it is a case not handled yet
  // TODO: handle outer function variables
  CHPL_UNIMPL("not yet handled");
  auto unknownType = UnknownType::get(context);
  return QualifiedType(QualifiedType::UNKNOWN, unknownType);
}

void Resolver::enterScope(const AstNode* ast) {
  if (createsScope(ast->tag())) {
    scopeStack.push_back(scopeForId(context, ast->id()));
  }
  if (auto d = ast->toDecl()) {
    declStack.push_back(d);
  }
  tagTracker[ast->tag()] += 1;
  if (ast->isLoop()) {
    tagTracker[AstTag::START_Loop] += 1;
  }
}
void Resolver::exitScope(const AstNode* ast) {
  if (createsScope(ast->tag())) {
    CHPL_ASSERT(!scopeStack.empty());
    scopeStack.pop_back();
  }
  if (ast->isDecl()) {
    CHPL_ASSERT(!declStack.empty());
    declStack.pop_back();
  }
  tagTracker[ast->tag()] -= 1;
  if (ast->isLoop()) {
    tagTracker[AstTag::START_Loop] -= 1;
  }
}

bool Resolver::isInsideTag(uast::asttags::AstTag tag) const {
  return tagTracker[tag] > 0;
}

bool Resolver::enter(const uast::Conditional* cond) {
  auto& r = byPostorder.byAst(cond);

  // Try short-circuiting. Visit the condition to see if it is a param
  cond->condition()->traverse(*this);

  // Make sure the 'if-var' is a class type, if it exists.
  if (auto var = cond->condition()->toVariable()) {
    auto& reVar = byPostorder.byAst(var);
    if (!reVar.type().isUnknown()) {
      auto t = reVar.type().type();

      // Resolve as non-nil borrowed class
      if (auto ct = t->toClassType()) {
        if (auto basicClass = ct->basicClassType()) {
          auto newClassType = ClassType::get(context,
              basicClass,
              /* no manager for borrowed class */ nullptr,
              ct->decorator().toBorrowed().addNonNil());
          reVar.setType(QualifiedType(reVar.type().kind(), newClassType));
        }
      } else {
        CHPL_REPORT(context, IfVarNonClassType, cond, reVar.type());
      }
    }
  }

  auto& condType = byPostorder.byAst(cond->condition()).type();
  if (!scopeResolveOnly) {
    // Only do short-circuiting for regular resolution, not scope-resolution

    if (condType.isParamTrue()) {
      // condition is param true, might as well only resolve `then` branch
      cond->thenBlock()->traverse(*this);
      if (cond->isExpressionLevel()) {
        auto& thenType = byPostorder.byAst(cond->thenStmt(0)).type();
        r.setType(thenType);
      }
      // No need to visit children again, or visit `else` branch.
      return false;
    } else if (condType.isParamFalse()) {
      auto elseBlock = cond->elseBlock();
      if (elseBlock == nullptr) {
        // no else branch. leave the type unknown.
        return false;
      }
      elseBlock->traverse(*this);
      if (cond->isExpressionLevel()) {
        auto& elseType = byPostorder.byAst(elseBlock->stmt(0)).type();
        r.setType(elseType);
      }
      // No need to visit children again, especially `then` branch.
      return false;
    }
  }

  // We might as well visit the rest of the children here,
  // since returning `true` at this point would cause a second visit
  // to `cond->condition()`.
  auto thenBlock = cond->thenBlock();
  auto elseBlock = cond->elseBlock();

  owned<InitResolver> thenInit;
  owned<InitResolver> elseInit;

  if (initResolver) {
    thenInit = initResolver->fork();
  }
  initResolver.swap(thenInit);
  thenBlock->traverse(*this);
  initResolver.swap(thenInit);

  if (elseBlock) {
    if (initResolver) elseInit = initResolver->fork();

    initResolver.swap(elseInit);
    elseBlock->traverse(*this);
    initResolver.swap(elseInit);
  }

  if (initResolver) {
    initResolver->merge(thenInit, elseInit);
  }

  if (cond->isExpressionLevel() && !scopeResolveOnly) {
    std::vector<QualifiedType> returnTypes;
    returnTypes.push_back(byPostorder.byAst(thenBlock->stmt(0)).type());
    if (elseBlock != nullptr) {
      returnTypes.push_back(byPostorder.byAst(elseBlock->stmt(0)).type());
    }
    // with useRequiredKind = false, the QualifiedType::Kind argument
    // is ignored. Just pick a dummy value.
    auto ifType = commonType(context, returnTypes);
    if (!ifType && !condType.isUnknown()) {
      // do not error if the condition type is unknown
      r.setType(CHPL_TYPE_ERROR(context, IncompatibleIfBranches, cond,
                                returnTypes[0], returnTypes[1]));
    } else if (ifType) {
      r.setType(*ifType);
    }
  }
  return false;
}
void Resolver::exit(const uast::Conditional* cond) {
}

bool Resolver::enter(const uast::Select* sel) {
  sel->expr()->traverse(*this);

  auto exprType = byPostorder.byAst(sel->expr()).type();

  enterScope(sel);

  const Scope* scope = scopeStack.back();
  auto inScopes = CallScopeInfo::forNormalCall(scope, poiScope);
  bool foundParamTrue = false;
  int otherwise = -1;

  for (int i = 0; i < sel->numWhenStmts(); i++) {
    auto when = sel->whenStmt(i);
    bool allParamFalse = true;
    bool anyParamTrue = false;

    if (when->isOtherwise()) {
      CHPL_ASSERT(otherwise == -1);
      otherwise = i;
      continue;
    }

    // Resolve all the when-cases
    for (auto caseExpr : when->caseExprs()) {
      caseExpr->traverse(*this);

      if (!scopeResolveOnly) {
        auto caseResult = byPostorder.byAst(caseExpr);
        auto caseType = caseResult.type();
        std::vector<CallInfoActual> actuals;
        actuals.push_back(CallInfoActual(exprType, UniqueString()));
        actuals.push_back(CallInfoActual(caseType, UniqueString()));
        auto ci = CallInfo (/* name */ USTR("=="),
                            /* calledType */ QualifiedType(),
                            /* isMethodCall */ false,
                            /* hasQuestionArg */ false,
                            /* isParenless */ false,
                            actuals);
        auto c = resolveGeneratedCall(context, caseExpr, ci, inScopes);
        handleResolvedCall(caseResult, caseExpr, ci, c,
                           { { AssociatedAction::COMPARE, caseExpr->id() } });

        auto type = c.exprType();
        anyParamTrue = anyParamTrue || type.isParamTrue();
        allParamFalse = allParamFalse && type.isParamFalse();
        byPostorder.byAst(caseExpr).setType(type);
      }
    }

    if (!scopeResolveOnly && allParamFalse) {
      // case will never be true, so do not resolve the statement
      continue;
    } else if (when->body()->numChildren() > 0) {
      // Otherwise, resolve the body.
      when->body()->traverse(*this);
    }

    // Current behavior is to ignore when-stmts following a param-true
    // condition.
    //
    // TODO: Should we keep resolving when-cases and warn for param-true
    // cases further down the line?
    if (anyParamTrue) {
      foundParamTrue = true;
      break;
    }
  }

  // If one of the when-stmts is statically true, we should not resolve the
  // 'otherwise' statement, should one exist.
  if (foundParamTrue == false && otherwise != -1) {
    auto other = sel->whenStmt(otherwise);
    if (other->body()->numChildren() > 0) {
      other->body()->traverse(*this);
    }
  }

  exitScope(sel);

  return false;
}

void Resolver::exit(const uast::Select* sel) {
}

bool Resolver::enter(const Literal* literal) {
  ResolvedExpression& result = byPostorder.byAst(literal);
  result.setType(typeForLiteral(context, literal));
  return false;
}
void Resolver::exit(const Literal* literal) {
}

// This class can count Identifiers with a particular name
struct CountIdentifiersWithName {
  UniqueString name;
  int count = 0;

  bool enter(const Identifier* d) {
    if (d->name() == name) count++;
    return false;
  }
  void exit(const Identifier* d) { }

  // traverse everything
  bool enter(const AstNode* ast) { return true; }
  void exit(const AstNode* ast) { }
};

// TODO: this is not quite accurate, since there might be another
// Identifier that does resolve but has the same name (e.g. with shadowing).
// One alternative would be to keep a map of Identifiers that did not
// resolve and then emit errors about them when concluding the resolution
// of a function (but.. would it be a problem if errors related to
// an identifier having an unknown type appear before we say the identifier
// was not found)?
bool Resolver::identHasMoreMentions(const Identifier* ident) {
  if (symbol) {
    CountIdentifiersWithName visitor;
    visitor.name = ident->name();
    symbol->traverse(visitor);
    if (visitor.count > 1) {
      return true;
    }
  }

  return false;
}

void Resolver::issueAmbiguityErrorIfNeeded(const Identifier* ident,
                                           const Scope* scope,
                                           LookupConfig prevConfig) {

  auto pair = namesWithErrorsEmitted.insert(ident->name());
  if (pair.second) {
    // insertion took place so emit the error
    bool printFirstMention = identHasMoreMentions(ident);

    auto fHelper = getFieldDeclarationLookupHelper();
    auto helper = getMethodReceiverScopeHelper();
    std::vector<ResultVisibilityTrace> traceResult;
    auto vec = lookupNameInScopeTracing(
                        context, scope,
                        /* methodLookupHelper */ fHelper,
                        /* receiverScopeHelper */ helper,
                        ident->name(), prevConfig,
                        traceResult);

    // emit an ambiguity error if this is not resolving a called ident
    CHPL_REPORT(context, AmbiguousIdentifier,
                ident, printFirstMention, vec, traceResult);
  }
}

MatchingIdsWithName Resolver::lookupIdentifier(
      const Identifier* ident,
      ParenlessOverloadInfo& outParenlessOverloadInfo) {
  CHPL_ASSERT(scopeStack.size() > 0);
  const Scope* scope = scopeStack.back();
  outParenlessOverloadInfo = ParenlessOverloadInfo();

  if (ident->name() == USTR("super")) {
    // Super is a keyword, and should't be looked up in scopes. Return
    // an empty ID to indicate that this identifier points to something,
    // but that something has a special meaning.
    return MatchingIdsWithName::createWithIdAndFlags(
                                      IdAndFlags::createForBuiltinVar());
  }

  bool resolvingCalledIdent = nearestCalledExpression() == ident;
  LookupConfig config = IDENTIFIER_LOOKUP_CONFIG;
  if (!resolvingCalledIdent) config |= LOOKUP_INNERMOST;

  auto fHelper = getFieldDeclarationLookupHelper();
  auto helper = getMethodReceiverScopeHelper();

  auto m = lookupNameInScopeWithWarnings(
                     context, scope,
                     /* methodLookupHelper */ fHelper,
                     /* receiverScopeHelper */ helper,
                     ident->name(), config, ident->id());

  if (!m.isEmpty()) {
    // We might be ambiguous, but due to having found multiple parenless procs.
    // It's not certain that this is an error; in particular, some parenless
    // procs can be ruled out if their 'where' clauses are false. If even
    // one identifier is not a parenless proc, there's an ambiguity.
    //
    // outParenlessOverloadInfo will be falsey if we found non-parenless-proc
    // IDs. In that case we may emit an ambiguity error later, after filtering
    // out incorrect receivers.
    outParenlessOverloadInfo =
        ParenlessOverloadInfo::fromMatchingIds(context, m);
  }

  return m;
}

void Resolver::validateAndSetToId(ResolvedExpression& r,
                                  const AstNode* node,
                                  const ID& toId) {
  r.setToId(toId);
  if (toId.isEmpty()) return;
  if (toId.isFabricatedId()) return;

  // Validate the newly set to ID.
  auto idTag = parsing::idToTag(context, toId);

  // It shouldn't refer to a module unless the node is an identifier in one of
  // the places where module references are allowed (e.g. imports).
  if (asttags::isModule(idTag)) {
    auto parentId = parsing::idToParentId(context, node->id());
    if (!parentId.isEmpty()) {
      auto parentTag = parsing::idToTag(context, parentId);
      if (asttags::isUse(parentTag) || asttags::isImport(parentTag) ||
          asttags::isAs(parentTag) || asttags::isVisibilityClause(parentTag) ||
          asttags::isDot(parentTag)) {
        // OK
      } else {
        auto toAst = parsing::idToAst(context, toId);
        auto mod = toAst->toModule();
        auto parentAst = parsing::idToAst(context, parentId);
        CHPL_REPORT(context, ModuleAsVariable, node, parentAst, mod);
        r.setToId(ID()); // clear
      }
    }
  }

  // If we're in a nested class, it shouldn't refer to an outer class' field.
  auto parentId =
    Builder::astTagIndicatesNewIdScope(idTag) ? toId : toId.parentSymbolId(context);
  auto parentTag = parsing::idToTag(context, parentId);
  if (asttags::isAggregateDecl(parentTag) &&
      parentId.contains(node->id()) &&
      parentId != toId /* It's okay to refer to the record itself */) {
    // Referring to a field of a class that's surrounding the current node.
    // Loop upwards looking for a composite type.
    auto searchId = node->id().parentSymbolId(context);
    while (!searchId.isEmpty()) {
      if (searchId == parentId) {
        // We found the aggregate type in which the to-ID is declared,
        // so there's no nested class issues.
        break;
      } else if (asttags::isTypeDecl(parsing::idToTag(context, searchId))) {
        auto parentAst = parsing::idToAst(context, parentId);
        auto searchAst = parsing::idToAst(context, searchId);
        auto searchAD = searchAst->toTypeDecl();
        // It's an error!
        CHPL_REPORT(context, NestedClassFieldRef, parentAst->toTypeDecl(),
                    searchAD, node, toId);
        break;
      }

      // Move on to the surrounding ID.
      searchId = searchId.parentSymbolId(context);
    }
  }
}

void Resolver::validateAndSetMostSpecific(ResolvedExpression& r,
                                          const uast::AstNode* expr,
                                          const MostSpecificCandidates& mostSpecific) {
  if (auto only = mostSpecific.only()) {
    // A single candidate was selected, either immediately or after return
    // intent overloading. Now, if calling this candidate requires a subtype
    // coercion for a 'const ref' formal, we need to emit an error: this
    // is allowed by the spec, but can't be compiled in the C backend due
    // to C's aliasing rules.

    if (only.hasConstRefCoercion()) {
      r.setType(CHPL_TYPE_ERROR(context, ConstRefCoercion, expr, only));
    }
  }

  r.setMostSpecific(mostSpecific);
}

static void maybeEmitWarningsForId(Resolver* rv, QualifiedType qt,
                                   const AstNode* astMention,
                                   ID idTarget) {
  if (astMention == nullptr || idTarget.isEmpty()) return;

  // Emit deprecation or unstable warnings if needed. Note that components
  // of use/import statements are handled entirely by the scope resolver
  // and will never be reached here. Overloaded functions are skipped via
  // the previous branch, and warnings will not be emitted until the parent
  // call is resolved. For consistency, have all calls emit errors at the
  // same place. Emit errors for other identifiers here.
  //
  // TODO: If we adjust production to emit errors for unambiguous calls
  // (and to skip doing so under '--dyno'), we can remove this branch.
  // TODO: We can skip all parenless functions using the below check, but
  // I'm not sure we can write something similar for functions, since it's
  // possible for function names to appear in other places besides calls.

  if (qt.kind() == QualifiedType::PARENLESS_FUNCTION) return;

  bool emitUnstableAndDeprecationWarnings = true;
  if (rv->nearestCalledExpression() == astMention) {
    // Note: the above conditional assumes that the astMention is
    // currently the thing being traversed (which is what makes
    // nearestCalledExpression() sufficient).
    //
    // For functions, do not warn, since call resolution will take
    // care of that.  However, if we're referring to other symbol
    // kinds, we know right now that a deprecation warning should be
    // emitted.
    emitUnstableAndDeprecationWarnings =
      !asttags::isFunction(parsing::idToTag(rv->context, idTarget));
  }

  if (emitUnstableAndDeprecationWarnings) {
    ID idMention = astMention->id();
    Context* context = rv->context;
    parsing::reportDeprecationWarningForId(context, idMention, idTarget);
    parsing::reportUnstableWarningForId(context, idMention, idTarget);
  }
}

QualifiedType Resolver::getSuperType(Context* context,
                                     const QualifiedType& sub,
                                     const Identifier* identForError) {
  // Early return: if we don't know the child type, we can't figure out the
  // parent type either.
  if (sub.isUnknownKindOrType()) {
    return QualifiedType();
  }

  if (auto classType = sub.type()->toClassType()) {
    if (auto basicClass = classType->basicClassType()) {
      auto basicParentClass = basicClass->parentClassType();
      auto newClassType = ClassType::get(context,
          basicParentClass,
          /* no manager for borrowed class */ nullptr,
          classType->decorator().toBorrowed());
      return QualifiedType(sub.kind(), newClassType);
    }
  }
  CHPL_REPORT(context, InvalidSuper, identForError, sub);
  return QualifiedType();
}

void Resolver::tryResolveParenlessCall(const ParenlessOverloadInfo& info,
                                       const Identifier* ident) {

  ResolvedExpression& r = byPostorder.byAst(ident);
  // resolve a parenless call
  std::vector<CallInfoActual> actuals;
  auto ci = CallInfo (/* name */ ident->name(),
                      /* calledType */ QualifiedType(),
                      /* isMethodCall */ false,
                      /* hasQuestionArg */ false,
                      /* isParenless */ true,
                      actuals);
  CHPL_ASSERT(!scopeStack.empty());
  auto inScope = scopeStack.back();

  // Note: this is only ever used from resolveIdentifier, so no qualifiers
  // are needed; resolve as an unqualified call.
  auto inScopes = CallScopeInfo::forNormalCall(inScope, poiScope);

  // If some IDs were methods and some weren't, we have to resolve two
  // calls: one with the (implicit) receiver, and one without.
  if (info.hasMethodCandidates() && info.hasNonMethodCandidates()) {
    auto cMethod = resolveGeneratedCallInMethod(context, ident, ci,
                                               inScopes,
                                               methodReceiverType());
    auto cNonMethod = resolveGeneratedCall(context, ident, ci,
                                           inScopes);

    if (!cMethod.mostSpecific().isEmpty() &&
        !cNonMethod.mostSpecific().foundCandidates()) {
      // Only found a valid method call.
      handleResolvedCall(r, ident, ci, cMethod);
    } else if (!cNonMethod.mostSpecific().isEmpty() &&
               !cMethod.mostSpecific().foundCandidates()) {
      // Only found a valid non-method call.
      handleResolvedCall(r, ident, ci, cNonMethod);
    } else if (cMethod.mostSpecific().isEmpty() && cNonMethod.mostSpecific().isEmpty()) {
      // Found neither; lots of candidates, but none worked! Use handleResolvedCall
      // on cMethod to issue an error and record the result.
      handleResolvedCall(r, ident, ci, cMethod);
    } else {
      // Found both, it's an ambiguity after all. Issue the ambiguity error
      // late, for which we need to recover some context.

      LookupConfig config = IDENTIFIER_LOOKUP_CONFIG;
      bool resolvingCalledIdent = nearestCalledExpression() == ident;
      if (!resolvingCalledIdent) config |= LOOKUP_INNERMOST;

      issueAmbiguityErrorIfNeeded(ident, inScope, config);
      auto& rr = byPostorder.byAst(ident);
      rr.setType(QualifiedType(QualifiedType::UNKNOWN,
                               ErroneousType::get(context)));

    }
  } else if (info.hasMethodCandidates()) {
    auto c = resolveGeneratedCallInMethod(context, ident, ci,
                                          inScopes,
                                          methodReceiverType());
    // save the most specific candidates in the resolution result
    handleResolvedCall(r, ident, ci, c);
  } else {
    CHPL_ASSERT(info.hasNonMethodCandidates());

    // as above, but don't consider method scopes
    auto c = resolveGeneratedCall(context, ident, ci,
                                  inScopes);
    // save the most specific candidates in the resolution result
    handleResolvedCall(r, ident, ci, c);
  }
}

void Resolver::resolveIdentifier(const Identifier* ident) {
  ResolvedExpression& result = byPostorder.byAst(ident);

  // for 'proc f(arg:?)' need to set 'arg' to have type AnyType
  CHPL_ASSERT(declStack.size() > 0);
  const Decl* inDecl = declStack.back();
  if (inDecl->isVarLikeDecl() && ident->name() == USTR("?")) {
    result.setType(QualifiedType(QualifiedType::TYPE, AnyType::get(context)));
    return;
  }

  // lookupIdentifier reports any errors that are needed
  auto parenlessInfo = ParenlessOverloadInfo();
  auto ids = lookupIdentifier(ident, parenlessInfo);

  bool resolvingCalledIdent = nearestCalledExpression() == ident;
  // TODO: these errors should be enabled for scope resolution
  // but for now, they are off, as a temporary measure to enable
  // the production compiler handle these cases. To enable this,
  // we will have to adjust the dyno scope resolver to handle 'domain',
  // and probably a few other features.
  bool emitLookupErrors = !resolvingCalledIdent && !scopeResolveOnly;

  if (parenlessInfo.areCandidatesOnlyParenlessProcs() && !scopeResolveOnly) {
    // Ambiguous, but a special case: there are many parenless functions.
    // This might be fine, if their 'where' clauses leave only one.
    //
    // Call resolution will issue an error if the overload selection fails.
    tryResolveParenlessCall(parenlessInfo, ident);
  } else if (ids.numIds() == 0) {
    if (emitLookupErrors) {
      auto pair = namesWithErrorsEmitted.insert(ident->name());
      if (pair.second) {
        // insertion took place so emit the error
        bool mentionedMoreThanOnce = identHasMoreMentions(ident);
        CHPL_REPORT(context, UnknownIdentifier, ident, mentionedMoreThanOnce);
      }
    }

    result.setType(QualifiedType());
  } else if (ids.numIds() > 1) {
    // Ambiguous. Check if we can break ambiguity by performing function resolution with
    // an implicit 'this' receiver, to filter based on receiver type.
    QualifiedType receiverType;
    ID receiverId;
    if (getMethodReceiver(&receiverType, &receiverId) && receiverType.type()) {
      std::vector<CallInfoActual> actuals;
      actuals.push_back(CallInfoActual(receiverType, USTR("this")));
      auto ci = CallInfo(/* name */ ident->name(),
                         /* calledType */ QualifiedType(),
                         /* isMethodCall */ true,
                         /* hasQuestionArg */ false,
                         /* isParenless */ true, actuals);
      auto inScope = scopeStack.back();
      auto inScopes = CallScopeInfo::forNormalCall(inScope, poiScope);
      auto c = resolveGeneratedCall(context, ident, ci, inScopes);
      // Ensure we error out for redeclarations within the method itself,
      // which resolution with an implicit 'this' currently does not catch.
      MatchingIdsWithName redeclarations;
      inScope->lookupInScope(ident->name(), redeclarations, IdAndFlags::Flags(),
                             IdAndFlags::FlagSet());
      if (!redeclarations.isEmpty()) {
        bool resolvingCalledIdent = nearestCalledExpression() == ident;
        LookupConfig config = IDENTIFIER_LOOKUP_CONFIG;
        if (!resolvingCalledIdent) config |= LOOKUP_INNERMOST;
        issueAmbiguityErrorIfNeeded(ident, inScope, config);
      } else {
        // Save result if successful
        if (handleResolvedCallWithoutError(result, ident, ci, c) &&
            emitLookupErrors) {
          issueErrorForFailedCallResolution(ident, ci, c);
        }
      }
    } else {
      // Can't establish the type. If this is in a function
      // call, we'll establish it later anyway.
      result.setType(QualifiedType());
    }
  } else {
    // just a single match
    const ID& id = ids.firstId();
    QualifiedType type;

    // empty IDs from the scope resolution process are builtins or super
    if (id.isEmpty() && isPotentialSuper(ident, &type)) {
      result.setType(getSuperType(context, type, ident));
      return;
    } else if (id.isEmpty()) {
      type = typeForBuiltin(context, ident->name());
      result.setIsBuiltin(true);

      // Some builtin types have a more useful ID than an empty ID
      ID builtinId = id;
      if (type.hasTypePtr()) {
        if (auto cl = type.type()->toClassType()) {
          if (auto basic = cl->basicClassType()) {
            builtinId = basic->id();
          }
        } else if (auto ct = type.type()->toCompositeType()) {
          builtinId = ct->id();
        }
      }
      validateAndSetToId(result, ident, builtinId);

      result.setType(type);
      return;
    }

    // use the type established at declaration/initialization,
    // but for things with generic type, use unknown.
    type = typeForId(id, /*localGenericToUnknown*/ true);

    maybeEmitWarningsForId(this, type, ident, id);

    // Record uses of outer variables.
    if (isOuterVariable(this, id) && outerVars) {
      const ID& mention = ident->id();
      const ID& var = id;
      outerVars->add(context, mention, var);
    }

    if (type.kind() == QualifiedType::TYPE) {
      // now, for a type that is generic with defaults,
      // compute the default version when needed. e.g.
      //   record R { type t = int; }
      //   var x: R; // should refer to R(int)
      bool computeDefaults = true;
      bool resolvingCalledIdent = nearestCalledExpression() == ident;

      // For calls like
      //
      //   type myType = anotherType(int)
      //
      // Use the generic version of anotherType to feed as receiver.
      if (resolvingCalledIdent) {
        computeDefaults = false;
      }

      // Other special exceptions like 'r' in:
      //
      //  proc r.init() { ... }
      //
      if (!genericReceiverOverrideStack.empty()) {
        auto& topEntry = genericReceiverOverrideStack.back();
        if ((topEntry.first.isEmpty() || topEntry.first == ident->name()) &&
            topEntry.second == parsing::parentAst(context, ident)) {
          computeDefaults = false;
        }
      }

      if (computeDefaults) {
        type = computeTypeDefaults(*this, type);
      }
    // Do not resolve function calls under 'scopeResolveOnly'
    } else if (type.kind() == QualifiedType::PARENLESS_FUNCTION) {
      CHPL_ASSERT(scopeResolveOnly && "resolution of parenless functions should've happened above");

      // Possibly a "compatibility hack" with production: we haven't checked
      // whether the call is valid, but the production scope resolver doesn't
      // care and assumes `ident` points to this parenless function. Setting
      // the toId also helps determine if this is a method call and should
      // have `this` inserted, as well as whether or not to turn this
      // into a parenless call.

      // Fall through to validateAndSetToId
    } else if (scopeResolveOnly &&
               type.kind() == QualifiedType::FUNCTION) {
      // Possibly a "compatibility hack" with production: we haven't checked
      // whether the call is valid, but the production scope resolver doesn't
      // care and assumes `ident` points to this function. Setting
      // the toId also helps determine if this is a method call

      // Fall through to validateAndSetToId
    }

    validateAndSetToId(result, ident, id);
    result.setType(type);
    // if there are multiple ids we should have gotten
    // a multiple definition error at the declarations.
  }
}

bool Resolver::enter(const Identifier* ident) {
  if (initResolver && initResolver->handleResolvingFieldAccess(ident)) {
    std::ignore = initResolver->handleUseOfField(ident);
    return false;
  } else {
    resolveIdentifier(ident);
    return false;
  }
}

void Resolver::exit(const Identifier* ident) {
}

bool Resolver::enter(const uast::Init* init) {
  return true;
}

void Resolver::exit(const uast::Init* init) {
  if (initResolver) {
    std::ignore = initResolver->handleInitStatement(init);
  }
}

bool Resolver::enter(const TypeQuery* tq) {
  if (skipTypeQueries) {
    return false;
  }

  // Consider 'proc f(arg:?t)'
  //   * if there is no substitution for 'arg', 't' should be AnyType
  //   * if there is a substitution for 'arg', 't' should be computed from it

  // Find the parent Formal and check for a substitution for that Formal
  const VarLikeDecl* formal = nullptr;
  bool foundFormalSubstitution = false;
  QualifiedType foundFormalType;
  for (auto it = declStack.rbegin(); it != declStack.rend(); ++it) {
    const Decl* d = *it;
    if (auto fml = d->toFormal()) {
      formal = fml;
      break;
    } else if (auto varargs = d->toVarArgFormal()) {
      formal = varargs;
    }
  }
  if (formal != nullptr) {
    if (substitutions != nullptr) {
      auto search = substitutions->find(formal->id());
      if (search != substitutions->end()) {
        foundFormalSubstitution = true;
        foundFormalType = search->second;
      }
    }
  }

  ResolvedExpression& result = byPostorder.byAst(tq);

  if (!foundFormalSubstitution) {
    // No substitution (i.e. initial signature) so use AnyType
    if (!callNodeStack.empty() && isCallToIntEtc(callNodeStack.back())) {
      auto defaultInt = IntType::get(context, 0);
      result.setType(QualifiedType(QualifiedType::PARAM, defaultInt));
    } else {
      // the type query could refer to a param or to a type,
      // so use the TYPE_QUERY kind
      result.setType(QualifiedType(QualifiedType::TYPE_QUERY,
                                   AnyType::get(context)));
    }
  } else {
    // Found a substitution after instantiating, so gather the components
    // of the type. We do this in a way that handles all TypeQuery
    // nodes within the Formal uAST node.
    resolveTypeQueriesFromFormalType(formal, foundFormalType);
  }

  return false;
}
void Resolver::exit(const TypeQuery* tq) {
}

// Treat receiver types specially in terms of generic resolution. That is,
// when resolving the following initializer when r is generic with defaults,
//
//   proc r.init() {}
//
// Make sure r's defaults aren't used so that the most general receiver is
// constructed. On the other hand, defaults _should_ be used for more
// complicated expressions:
//
//   proc (someTypeFn(r)).init() {}
//
static bool shouldUseGenericTypeForTypeExpr(const NamedDecl* decl) {
 return decl->isFormal() && decl->name() == USTR("this");
}

bool Resolver::enter(const NamedDecl* decl) {

  if (decl->id().isSymbolDefiningScope()) {
    // It's a symbol with a different path, e.g. a Function.
    // Don't try to resolve it now in this
    // traversal. Instead, resolve it e.g. when the function is called.
    return false;
  }

  CHPL_ASSERT(scopeStack.size() > 0);
  const Scope* scope = scopeStack.back();

  // Silence redefinition warnings for enum elements because we have
  // a special error (DuplicateEnumElement) for those.
  if (!decl->isEnumElement()) {
    emitMultipleDefinedSymbolErrors(context, scope);
  }

  enterScope(decl);

  if (shouldUseGenericTypeForTypeExpr(decl)) {
    // Empty string indicates that all identifiers should be treated as
    // non-defaulted. Using 'decl' means that only the top-level identifiers
    // will be resolved this way.
    genericReceiverOverrideStack.emplace_back(UniqueString(), decl);
  }

  // This logic exists to prioritize the field's type expression when
  // resolving a field's type. If the type expression is concrete, then we
  // do not need to resolve the init-expression. This is beneficial in cases
  // where the init-expression would otherwise result in recursive resolution.
  //
  // TODO: Initializers will need to check the compatibility of the type
  // expression and initialization expression.
  if (!scopeResolveOnly && fieldTypesOnly && decl == curStmt) {
    auto field = decl->toVarLikeDecl();
    if (field->typeExpression() != nullptr &&
        field->initExpression() != nullptr &&
        field->storageKind() != Qualifier::TYPE &&
        field->storageKind() != Qualifier::PARAM) {
      field->typeExpression()->traverse(*this);
      auto res = byPostorder.byAst(field->typeExpression());
      auto g = getTypeGenericity(context, res.type().type());
      if (g != Type::CONCRETE) {
        field->initExpression()->traverse(*this);
      }
      return false;
    } else {
      return true;
    }
  } else {
    return true;
  }
}

void Resolver::exit(const NamedDecl* decl) {
  if (shouldUseGenericTypeForTypeExpr(decl)) {
    genericReceiverOverrideStack.pop_back();
  }

  // We are resolving a symbol with a different path (e.g., a Function or
  // a CompositeType declaration). In most cases we do not try to resolve
  // in this traversal. However, if we are a nested function and the child
  // is also a nested function, we need to check and potentially propagate
  // their outer variable set into our own.
  auto idChild = decl->id();
  if (idChild.isSymbolDefiningScope()) {
    if (this->symbol != nullptr &&
        parsing::idIsNestedFunction(context, this->symbol->id()) &&
        parsing::idIsNestedFunction(context, idChild) &&
        outerVars.get()) {
      if (auto ovs = computeOuterVariables(context, idChild)) {
        for (int i = 0; i < ovs->numVariables(); i++) {

          // If the variable is reaching in the child function, it means it
          // was defined in one of _our_ parent(s). So we need to track it.
          if (ovs->isReachingVariable(i)) {
            ID var = ovs->variable(i);

            // Mentions from child functions are not recorded in the parent
            // function's info, so just use the first (as a convenience).
            ID mention = ovs->firstMention(var);
            outerVars->add(context, mention, var);
          }
        }
      }
    }
    return;
  }

  resolveNamedDecl(decl, /* useType */ nullptr);

  if (decl->isFormal() && decl->name() == USTR("this")) {
    allowReceiverScopes = true;
    // when needed, the receiver scopes can be computed, now that
    // 'this' has been resolved.
  }

  exitScope(decl);
}

static void getVarLikeOrTupleTypeInit(const AstNode* ast,
                                      const AstNode*& typeExpr,
                                      const AstNode*& initExpr) {
  typeExpr = nullptr;
  initExpr = nullptr;
  if (auto v = ast->toVarLikeDecl()) {
    if (auto t = v->typeExpression()) {
      typeExpr = t;
    }
    if (auto e = v->initExpression()) {
      initExpr = e;
    }
  } else if (auto td = ast->toTupleDecl()) {
    if (auto t = td->typeExpression()) {
      typeExpr = t;
    }
    if (auto e = td->initExpression()) {
      initExpr = e;
    }
  }
}

bool Resolver::enter(const MultiDecl* decl) {
  enterScope(decl);

  // Establish the type or init expressions within
  // by visiting those nodes
  for (auto d : decl->decls()) {
    enterScope(d);

    const AstNode* typeExpr = nullptr;
    const AstNode* initExpr = nullptr;
    getVarLikeOrTupleTypeInit(d, typeExpr, initExpr);

    if (typeExpr != nullptr) {
      typeExpr->traverse(*this);
    }
    if (initExpr != nullptr) {
      initExpr->traverse(*this);
    }

    exitScope(d);
  }

  return false;
}
void Resolver::exit(const MultiDecl* decl) {
  if (scopeResolveOnly)
    return;

  // Visit the named decls in reverse order
  // setting the type/init.
  auto begin = decl->declOrComments().begin();
  auto it = decl->declOrComments().end();
  const Type* lastType = nullptr;
  while (it != begin) {
    --it;

    auto d = it->toDecl();
    const AstNode* typeExpr = nullptr;
    const AstNode* initExpr = nullptr;
    getVarLikeOrTupleTypeInit(d, typeExpr, initExpr);

    // if it has neither init nor type, use the type from the
    // variable to the right.
    // e.g., in
    //    var a, b: int
    // a is of type int
    const Type* t = nullptr;
    if (typeExpr == nullptr && initExpr == nullptr) {
      if (lastType == nullptr) {
        // this could be split init
        t = UnknownType::get(context);
      } else {
        t = lastType;
      }
    }

    // for the functions called in these conditionals:
    //  * if t is nullptr, just resolve it like usual
    //  * update the type of d in byPostorder
    if (auto v = d->toVarLikeDecl()) {
      resolveNamedDecl(v, t);
    } else if (auto td = d->toTupleDecl()) {
      resolveTupleDecl(td, t);
    }

    // update lastType
    ResolvedExpression& result = byPostorder.byAst(d);
    lastType = result.type().type();
  }

  exitScope(decl);
}

bool Resolver::enter(const TupleDecl* decl) {
  enterScope(decl);

  // TODO: Can we just do this every time we 'enterScope'?
  CHPL_ASSERT(scopeStack.size() > 0);
  const Scope* scope = scopeStack.back();
  emitMultipleDefinedSymbolErrors(context, scope);

  // Establish the type of the type expr / init expr within
  if (auto t = decl->typeExpression()) {
    t->traverse(*this);
  }
  if (auto e = decl->initExpression()) {
    e->traverse(*this);
  }
  return false;
}

void Resolver::exit(const TupleDecl* decl) {
  resolveTupleDecl(decl, /* useType */ nullptr);
  exitScope(decl);
}

bool Resolver::enter(const Range* range) {
  return true;
}
void Resolver::exit(const Range* range) {
  if (scopeResolveOnly) {
    return;
  }

  const RecordType* rangeType = CompositeType::getRangeType(context);
  if (CompositeType::isMissingBundledRecordType(context, rangeType->id())) {
    // The range record is part of the standard library, but
    // it's possible to invoke the resolver without the stdlib.
    // In this case, mark ranges as UnknownType, but do not error.
    return;
  }

  // Encode the type of the range as two bits: bounded below, bounded above.
  int boundType = (range->lowerBound() != nullptr) << 1 |
                  (range->upperBound() != nullptr);

  // Decide which Chapel function to call for this.
  static const char* functions[] = {
    "chpl_build_unbounded_range",
    "chpl_build_high_bounded_range",
    "chpl_build_low_bounded_range",
    "chpl_build_bounded_range"
  };
  const char* function = functions[boundType];

  std::vector<CallInfoActual> actuals;
  if (range->lowerBound()) {
    actuals.emplace_back(/* type */ byPostorder.byAst(range->lowerBound()).type(),
                         /* byName */ UniqueString());
  }
  if (range->upperBound()) {
    actuals.emplace_back(/* type */ byPostorder.byAst(range->upperBound()).type(),
                         /* byName */ UniqueString());
  }
  auto ci = CallInfo(/* name */ UniqueString::get(context, function),
                     /* calledType */ QualifiedType(),
                     /* isMethodCall */ false,
                     /* hasQuestionArg */ false,
                     /* isParenless */ false, actuals);
  auto scope = scopeStack.back();
  auto inScopes = CallScopeInfo::forNormalCall(scope, poiScope);
  auto c = resolveGeneratedCall(context, range, ci, inScopes);
  handleResolvedCall(byPostorder.byAst(range), range, ci, c);
}

bool Resolver::enter(const uast::Domain* decl) {
  return true;
}

void Resolver::exit(const uast::Domain* decl) {
  if (decl->numExprs() == 0) {
    auto& re = byPostorder.byAst(decl);
    auto dt = QualifiedType(QualifiedType::CONST_VAR, DomainType::getGenericDomainType(context));
    re.setType(dt);
  }
}

types::QualifiedType Resolver::typeForBooleanOp(const uast::OpCall* op) {
  if (op->numActuals() != 2) {
    return typeErr(op, "invalid op call");
  }

  bool isAnd = op->op() == USTR("&&");
  // visit the LHS
  op->actual(0)->traverse(*this);
  // look at the LHS type. Is it param?
  const QualifiedType& lhs = byPostorder.byAst(op->actual(0)).type();
  // can we short circuit? e.g., is this false && x, or true || y?
  bool shortCircuit = isAnd ? lhs.isParamFalse() : lhs.isParamTrue();
  if (shortCircuit) {
    // short circuit, no need to evaluate RHS.
    return lhs;
  }

  // go ahead and evaluate the RHS
  op->actual(1)->traverse(*this);
  // look at the RHS type.
  const QualifiedType& rhs = byPostorder.byAst(op->actual(1)).type();

  // are we looking at true && true or false || false?
  bool bothIdentity = isAnd
    ? (lhs.isParamTrue() && rhs.isParamTrue())
    : (lhs.isParamFalse() && rhs.isParamFalse());
  if (bothIdentity) {
    // true && true == true, false || false == false.
    // return lhs type.
    return lhs;
  } else if (lhs.isUnknown() || rhs.isUnknown()) {
    // if one is unknown, return unknown
    return QualifiedType(QualifiedType::CONST_VAR,
                         UnknownType::get(context));
  } else {
    CHPL_ASSERT(rhs.type()->isBoolType() && lhs.type()->isBoolType());
    if (rhs.isParam() && lhs.isParam()) {
      // preserve param-ness
      // this case is only hit when the result is false (for &&)
      // or when the result is true (for ||), so return !isAnd.
      return QualifiedType(QualifiedType::PARAM,
                             BoolType::get(context),
                             BoolParam::get(context, !isAnd));
    } else {
      // otherwise just return a Bool value
      return QualifiedType(QualifiedType::CONST_VAR,
                             BoolType::get(context));
    }
  }
}

bool Resolver::enter(const Call* call) {
  callNodeStack.push_back(call);
  auto op = call->toOpCall();

  if (op && initResolver) {
    initResolver->doDetectPossibleAssignmentToField(op);
  }

  // Scope resolve without short-circuiting if we're only scope resolving.
  if (scopeResolveOnly) {
    return true;
  }

  // handle && and || to not bother to evaluate the RHS
  // if the LHS is param and false/true, respectively.
  if (op && (op->op() == USTR("&&") || op->op() == USTR("||"))) {
    QualifiedType result = typeForBooleanOp(op);
    // Update the type of the && call
    byPostorder.byAst(op).setType(result);
    // Don't visit the children since we already did
    return false;
  }

  // Do not descend into children for 'prim resolves' since it accepts a single
  // _expression_ (not a value!) and checks if it resolves.
  if (auto primCall = call->toPrimCall()) {
    if (primCall->prim() == PRIM_RESOLVES) {
      return false;
    }
  }

  return true;
}

void Resolver::prepareCallInfoActuals(const Call* call,
                                      std::vector<CallInfoActual>& actuals,
                                      const AstNode*& questionArg) {
  CallInfo::prepareActuals(context, call, byPostorder,
                           /* raiseErrors */ true,
                           actuals, questionArg,
                           /* actualAsts */ nullptr);
}

static const Type* getGenericType(Context* context, const Type* recv) {
  const Type* gen = nullptr;
  if (auto cur = recv->toCompositeType()) {
    gen = cur->instantiatedFromCompositeType();
    if (gen == nullptr) gen = cur;
  } else if (auto cur = recv->toClassType()) {
    auto m = getGenericType(context, cur->manageableType());
    gen = ClassType::get(context, m->toManageableType(),
                         cur->manager(), cur->decorator());
  }
  return gen;
}

void Resolver::handleCallExpr(const uast::Call* call) {
  if (scopeResolveOnly) {
    return;
  }

  if (initResolver && initResolver->handleResolvingCall(call))
    return;

  auto op = call->toOpCall();
  if (op && (op->op() == USTR("&&") || op->op() == USTR("||"))) {
    if (!scopeResolveOnly) {
      // these are handled in 'enter' to do param folding
      return;
    }
  }

  CHPL_ASSERT(scopeStack.size() > 0);
  const Scope* scope = scopeStack.back();

  // try to resolve it as a special call (e.g. Tuple assignment)
  if (resolveSpecialCall(call)) {
    return;
  }

  std::vector<const uast::AstNode*> actualAsts;
  ID moduleScopeId;
  auto ci = CallInfo::create(context, call, byPostorder,
                             /* raiseErrors */ true,
                             &actualAsts,
                             &moduleScopeId);
  auto inScopes =
    moduleScopeId.isEmpty() ?
    CallScopeInfo::forNormalCall(scope, poiScope) :
    CallScopeInfo::forQualifiedCall(context, moduleScopeId, scope, poiScope);

  // With some exceptions (see below), don't try to resolve a call that accepts:
  SkipCallResolutionReason skip = NONE;
  // EXCEPT, to handle split-init with an 'out' formal,
  // the actual argument can have unknown / generic type if it
  // refers directly to a particular variable.
  // EXCEPT, type construction can work with unknown or generic types
  // EXCEPT, tuple type construction also works with unknown/generic types

  if (!call->isTuple()) {
    int actualIdx = 0;
    for (const auto& actual : ci.actuals()) {
      ID toId; // does the actual refer directly to a particular variable?
      const AstNode* actualAst = actualAsts[actualIdx];
      if (actualAst != nullptr && byPostorder.hasAst(actualAst)) {
        toId = byPostorder.byAst(actualAst).toId();
      }
      QualifiedType qt = actual.type();
      const Type* t = qt.type();

      const AstNode* formalAst = toId.isEmpty() ? nullptr : parsing::idToAst(context, toId);
      bool isNonOutFormal = formalAst != nullptr &&
                            formalAst->isFormal() &&
                            formalAst->toFormal()->intent() != Formal::Intent::OUT;

      if (t != nullptr && t->isErroneousType()) {
        // always skip if there is an ErroneousType
        skip = ERRONEOUS_ACT;
      } else if (!toId.isEmpty() && !isNonOutFormal &&
                 qt.kind() != QualifiedType::PARAM &&
                 qt.kind() != QualifiedType::TYPE &&
                 qt.isRef() == false) {
        // don't skip because it could be initialized with 'out' intent,
        // but not for non-out formals because they can't be split-initialized.
      } else if (actualAst->isTypeQuery() && ci.calledType().isType()) {
        // don't skip for type queries in type constructors
      } else {
        if (qt.isParam() && qt.param() == nullptr) {
          skip = UNKNOWN_PARAM;
        } else if (qt.isUnknown()) {
          skip = UNKNOWN_ACT;
        } else if (t != nullptr && qt.kind() != QualifiedType::INIT_RECEIVER) {
          // For initializer calls, allow generic formals using the above
          // condition; this way, 'this.init(..)' while 'this' is generic
          // should be fine.

          auto g = getTypeGenericity(context, t);
          bool isBuiltinGeneric = (g == Type::GENERIC &&
                                   (t->isAnyType() || t->isBuiltinType()));
          if (qt.isType() && isBuiltinGeneric && substitutions == nullptr) {
            skip = GENERIC_TYPE;
          } else if (!qt.isType() && g != Type::CONCRETE) {
            skip = GENERIC_VALUE;
          }
        }
      }

      // Don't skip for type constructors, except due to unknown params.
      if (skip != UNKNOWN_PARAM && ci.calledType().isType()) {
        skip = NONE;
      }

      // Do not skip primitive calls that accept a generic type, since they
      // may be valid.
      if (skip == GENERIC_TYPE && call->toPrimCall()) {
        skip = NONE;
      }

      if (skip) {
        break;
      }
      actualIdx++;
    }
  }
  // Don't try to resolve calls to '=' until later
  if (ci.isOpCall() && ci.name() == USTR("=")) {
    skip = OTHER_REASON;
  }

  if (!skip) {
    QualifiedType receiverType = methodReceiverType();

    // Calling initializer without qualifier, e.g., `init(a, b, c);``
    //
    // If the user has mistakenly instantiated a field of the type before
    // calling ``init``, then the receiver type will either be fully or
    // partially instantiated. This will cause a failure to resolve the
    // ``init`` call, and result in a confusing and unhelpful error message.
    //
    // Instead, whenever we see this kind of 'init' call, we will set the
    // receiver type to be fully-generic as if the user had not instantiated
    // any fields. This matches the behavior when calling ``this.init(...)``,
    // where ``this`` is treated as fully-generic.
    if (initResolver &&
        ci.name() == USTR("init") && ci.isMethodCall() == false) {
      auto gen = getGenericType(context, receiverType.type());
      receiverType = QualifiedType(QualifiedType::INIT_RECEIVER, gen);
    }

    CallResolutionResult c
      = resolveCallInMethod(context, call, ci, inScopes, receiverType);

    // save the most specific candidates in the resolution result for the id
    ResolvedExpression& r = byPostorder.byAst(call);
    handleResolvedCallPrintCandidates(r, call, ci, inScopes, receiverType, c);

    // handle type inference for variables split-inited by 'out' formals
    adjustTypesForOutFormals(ci, actualAsts, c.mostSpecific());

    if (initResolver) {
      initResolver->handleResolvedCall(call, &c);
    }
  } else {
    // We're skipping the call, but explicitly store the 'unknown type'
    // in the map.
    ResolvedExpression& r = byPostorder.byAst(call);
    r.setType(QualifiedType());

    if (initResolver) {
      initResolver->handleResolvedCall(call, /* call resolution result */ nullptr);
    }
  }
}

void Resolver::exit(const Call* call) {
  handleCallExpr(call);

  // Always remove the call from the stack to make sure it's properly set.
  callNodeStack.pop_back();
}

bool Resolver::enter(const Dot* dot) {
  if (initResolver && initResolver->handleResolvingFieldAccess(dot))
    return false;
  return true;
}

ID Resolver::scopeResolveEnumElement(const Enum* enumAst,
                                     UniqueString elementName,
                                     const AstNode* nodeForErr,
                                     bool& outAmbiguous) {
  outAmbiguous = false;
  LookupConfig config = LOOKUP_DECLS | LOOKUP_INNERMOST;
  auto enumScope = scopeForId(context, enumAst->id());
  auto ids = lookupNameInScope(context, enumScope,
                               /* methodLookupHelper */ nullptr,
                               /* receiverScopeHelper */ nullptr,
                               elementName, config);
  if (ids.numIds() == 0) {
    // Do not report the error here, because it might not be an error.
    // In particular, we could be in a parenless method call. Callers
    // will decide whether or not to emit the error.
  } else if (ids.numIds() > 1) {
    // multiple candidates. report an error, but the expression most likely has a type given by the enum.
    std::vector<ID> redefinedIds(ids.numIds());
    std::copy(ids.begin(), ids.end(), redefinedIds.begin());
    CHPL_REPORT(context, MultipleEnumElems, nodeForErr, elementName, enumAst,
                std::move(redefinedIds));
    outAmbiguous = true;
  } else {
    return ids.firstId();
  }

  return ID();
}

QualifiedType
Resolver::typeForScopeResolvedEnumElement(const EnumType* enumType,
                                          const ID& refersToId,
                                          bool ambiguous) {
  if (!refersToId.isEmpty()) {
    // Found a single enum element, the type can be a param.
    auto newParam = Param::getEnumParam(context, refersToId);
    return QualifiedType(QualifiedType::PARAM, enumType, newParam);
  } else if (ambiguous) {
    // multiple candidates. but the expression most likely has a type given by
    // the enum.
    return QualifiedType(QualifiedType::CONST_VAR, enumType);
  } else {
    return QualifiedType(QualifiedType::UNKNOWN, ErroneousType::get(context));
  }
}

QualifiedType
Resolver::typeForScopeResolvedEnumElement(const ID& enumTypeId,
                                          const ID& refersToId,
                                          bool ambiguous) {
  auto type = initialTypeForTypeDecl(context, enumTypeId);
  CHPL_ASSERT(type && type->isEnumType());
  return typeForScopeResolvedEnumElement(type->toEnumType(), refersToId,
                                         ambiguous);
}

QualifiedType Resolver::typeForEnumElement(const EnumType* enumType,
                                           UniqueString elementName,
                                           const AstNode* nodeForErr) {
  auto enumAst = parsing::idToAst(context, enumType->id())->toEnum();
  CHPL_ASSERT(enumAst != nullptr);
  bool ambiguous;
  auto refersToId = scopeResolveEnumElement(enumAst, elementName,
                                            nodeForErr, ambiguous);
  auto qt = typeForScopeResolvedEnumElement(enumType, refersToId, ambiguous);
  if (refersToId.isEmpty() && !ambiguous) {
    // scopeResolveEnumElement doesn't report a "not found" error because
    // not being able to find an enum element isn't always an error. Here,
    // though, we are specifically interested in an element, so report
    // the error.
    CHPL_REPORT(context, UnknownEnumElem, nodeForErr, elementName, enumAst);
  }
  return qt;
}

void Resolver::exit(const Dot* dot) {
  if (initResolver && initResolver->handleUseOfField(dot)) return;

  ResolvedExpression& receiver = byPostorder.byAst(dot->receiver());

  bool deferToFunctionResolution = false;
  bool resolvingCalledDot = nearestCalledExpression() == dot;
  if (resolvingCalledDot && !scopeResolveOnly) {
    // When resolving `a.b()`, we likely want to perform call resolution,
    // as b(this=a). This happens when the parent function call expression is
    // processed. So, we can skip the work here. However, before we do skip,
    // handle other cases, such as `M.b()` where `M.b` is a value being
    // called using `proc this`.
    deferToFunctionResolution = true;

    // Try to resolve a it as a field/parenless proc so we can resolve 'this' on
    // it later if needed.
    if (!receiver.type().isUnknown() && receiver.type().type() &&
        receiver.type().type()->isCompositeType() &&
        dot->field() != "init") {
      std::vector<CallInfoActual> actuals;
      actuals.push_back(CallInfoActual(receiver.type(), USTR("this")));
      auto ci = CallInfo(/* name */ dot->field(),
                         /* calledType */ QualifiedType(),
                         /* isMethodCall */ true,
                         /* hasQuestionArg */ false,
                         /* isParenless */ true, actuals);
      auto inScope = scopeStack.back();
      auto inScopes = CallScopeInfo::forNormalCall(inScope, poiScope);
      auto c = resolveGeneratedCall(context, dot, ci, inScopes);
      if (!c.mostSpecific().isEmpty()) {
        // save the most specific candidates in the resolution result for the id
        ResolvedExpression& r = byPostorder.byAst(dot);
        handleResolvedCall(r, dot, ci, c);
      }
      return;
    }
  }

  if (dot->field() == USTR("type")) {
    const Type* receiverType;
    ResolvedExpression& r = byPostorder.byAst(dot);

    if (receiver.type().type() != nullptr) {
      receiverType = receiver.type().type();
    } else {
      receiverType = ErroneousType::get(context);
    }

    if (!receiver.type().isType()) {
      r.setType(QualifiedType(QualifiedType::TYPE, receiverType));
    } else {
      r.setType(CHPL_TYPE_ERROR(context, DotTypeOnType, dot, receiverType,
                                receiver.toId()));
    }
    return;
  }

  if (receiver.type().kind() == QualifiedType::MODULE &&
      !receiver.toId().isEmpty()) {
    ID moduleId = receiver.toId();

    // resolve e.g. M.x where M is a module
    LookupConfig config = LOOKUP_DECLS |
                          LOOKUP_IMPORT_AND_USE |
                          LOOKUP_EXTERN_BLOCKS;

    if (!moduleId.contains(dot->id())) {
      config |= LOOKUP_SKIP_PRIVATE_VIS;
    }

    auto modScope = scopeForModule(context, moduleId);
    auto ids = lookupNameInScope(context, modScope,
                                 /* methodLookupHelper */ nullptr,
                                 /* receiverScopeHelper */ nullptr,
                                 dot->field(), config);
    ResolvedExpression& r = byPostorder.byAst(dot);
    if (ids.numIds() == 0) {
      // emit a "can't find that thing" error
      issueErrorForFailedModuleDot(dot, moduleId, config);
      r.setType(QualifiedType());
    } else if (ids.numIds() > 1) {
      // can't establish the type. If this is in a function
      // call, we'll establish it later anyway.
    } else {
      // ids.numIds == 1
      const ID& id = ids.firstId();

      // TODO: handle extern blocks correctly.
      // This conditional is a workaround to leave extern block resolution
      // to the production scope resolver.
      if (!isExternBlock(parsing::idToTag(context, id))) {
        QualifiedType type;
        if (id.isEmpty()) {
          // empty IDs from the scope resolution process are builtins
          CHPL_ASSERT(false && "Not handled yet!");
        } else {
          // use the type established at declaration/initialization,
          // but for things with generic type, use unknown.
          type = typeForId(id, /*localGenericToUnknown*/ true);
        }
        maybeEmitWarningsForId(this, type, dot, id);
        validateAndSetToId(r, dot, id);
        r.setType(type);
      }
    }
    return;
  }

  if (receiver.type().kind() == QualifiedType::TYPE &&
      asttags::isEnum(parsing::idToTag(context, receiver.toId()))) {
    // resolve E.x where E is an enum.
    auto enumAst = parsing::idToAst(context, receiver.toId())->toEnum();
    CHPL_ASSERT(enumAst != nullptr);
    ResolvedExpression& r = byPostorder.byAst(dot);

    bool ambiguous;
    auto elemId = scopeResolveEnumElement(enumAst, dot->field(), dot, ambiguous);
    validateAndSetToId(r, dot, elemId);

    if (!scopeResolveOnly &&
        receiver.type().type() != nullptr &&
        receiver.type().type()->isEnumType()) {
      const EnumType* enumType = receiver.type().type()->toEnumType();
      CHPL_ASSERT(enumType != nullptr);
      auto qt = typeForScopeResolvedEnumElement(enumType, elemId, ambiguous);
      r.setType(qt);
      maybeEmitWarningsForId(this, qt, dot, elemId);
    }

    if (!elemId.isEmpty() || ambiguous) {
      // Found something in the enum, no need to check for parenless methods.
      return;
    } else {
      // Continue on to parenless methods, maybe it was a parenless call.
    }
  }

  // Handle null, unknown, or erroneous receiver type
  if (receiver.type().type() == nullptr ||
      receiver.type().type()->isUnknownType()) {
    ResolvedExpression& r = byPostorder.byAst(dot);
    r.setType(QualifiedType(QualifiedType::VAR, UnknownType::get(context)));
    return;
  }
  if (receiver.type().type()->isErroneousType()) {
    ResolvedExpression& r = byPostorder.byAst(dot);
    r.setType(QualifiedType(QualifiedType::VAR, ErroneousType::get(context)));
    return;
  }

  if (scopeResolveOnly || deferToFunctionResolution)
    return;

  // resolve a.x where a is a record/class and x is a field or parenless method
  std::vector<CallInfoActual> actuals;
  actuals.push_back(CallInfoActual(receiver.type(), USTR("this")));
  auto ci = CallInfo (/* name */ dot->field(),
                      /* calledType */ QualifiedType(),
                      /* isMethodCall */ true,
                      /* hasQuestionArg */ false,
                      /* isParenless */ true,
                      actuals);
  auto inScope = scopeStack.back();
  auto inScopes = CallScopeInfo::forNormalCall(inScope, poiScope);
  auto c = resolveGeneratedCall(context, dot, ci, inScopes);
  // save the most specific candidates in the resolution result for the id
  ResolvedExpression& r = byPostorder.byAst(dot);
  handleResolvedCall(r, dot, ci, c);
}

bool Resolver::enter(const New* node) {
  if (auto ident = node->typeExpression()->toIdentifier()) {
    genericReceiverOverrideStack.emplace_back(ident->name(), node);
  }

  return true;
}

// TODO: How do we wire this up with 'getManagedClassType'? Is it possible?
// TODO: How to handle nilability, e.g. new owned C'?'
static const ClassType*
getDecoratedClassForNew(Context* context, const New* node,
                        const ClassType* classType) {
  auto basic = classType->basicClassType();

  if (!basic) {
    context->error(node, "attempt to 'new' the generic 'class' type");
    return nullptr;
  }

  auto decorator = classType->decorator();
  const Type* manager = nullptr;

  switch (node->management()) {
    case New::DEFAULT_MANAGEMENT:
    case New::OWNED:
      decorator = ClassTypeDecorator(ClassTypeDecorator::MANAGED);
      manager = AnyOwnedType::get(context);
      break;
    case New::SHARED:
      decorator = ClassTypeDecorator(ClassTypeDecorator::MANAGED);
      manager = AnySharedType::get(context);
      break;
    case New::BORROWED:
      decorator = ClassTypeDecorator(ClassTypeDecorator::BORROWED);
      break;
    case New::UNMANAGED:
      decorator = ClassTypeDecorator(ClassTypeDecorator::UNMANAGED);
      break;
  }

  // Combine the decorators to preserve e.g. nilability.
  auto combine = decorator.combine(classType->decorator());

  // TODO: How to get outer '?' from type expression?
  if (combine.isUnknownNilability()) {
    combine = combine.addNonNil();
  }

  auto ret = ClassType::get(context, basic, manager, combine);
  return ret;
}

static void resolveNewForClass(Resolver& rv, const New* node,
                               const ClassType* classType) {
  ResolvedExpression& re = rv.byPostorder.byAst(node);
  auto cls = getDecoratedClassForNew(rv.context, node, classType);
  auto qt = QualifiedType(QualifiedType::INIT_RECEIVER, cls);
  re.setType(qt);
}

static void resolveNewForRecord(Resolver& rv, const New* node,
                                const RecordType* recordType) {
  ResolvedExpression& re = rv.byPostorder.byAst(node);

  if (node->management() != New::DEFAULT_MANAGEMENT) {
    CHPL_REPORT(rv.context, MemManagementNonClass, node, recordType);
  } else {
    auto qt = QualifiedType(QualifiedType::INIT_RECEIVER, recordType);
    re.setType(qt);
  }
}

static void resolveNewForUnion(Resolver& rv, const New* node,
                               const UnionType* unionType) {
  ResolvedExpression& re = rv.byPostorder.byAst(node);

  if (node->management() != New::DEFAULT_MANAGEMENT) {
    CHPL_REPORT(rv.context, MemManagementNonClass, node, unionType);
  } else {
    auto qt = QualifiedType(QualifiedType::INIT_RECEIVER, unionType);
    re.setType(qt);
  }
}

void Resolver::exit(const New* node) {
  if (node->typeExpression()->isIdentifier()) {
    genericReceiverOverrideStack.pop_back();
  }

  if (scopeResolveOnly)
    return;

  // Fetch the pieces of the type expression.
  const AstNode* typeExpr = node->typeExpression();
  ResolvedExpression& reTypeExpr = byPostorder.byAst(typeExpr);
  auto& qtTypeExpr = reTypeExpr.type();

  // Propagate up type expression before doing further work.
  ResolvedExpression& re = byPostorder.byAst(node);
  re.setType(qtTypeExpr);

  // Check and exit on obvious error cases.
  if (qtTypeExpr.isUnknown()) {
    context->error(node, "Attempt to 'new' a function or undefined symbol");
    return;
  } else if (qtTypeExpr.kind() != QualifiedType::TYPE) {
    context->error(node, "'new' must be followed by a type expression");
    return;
  }

  if (qtTypeExpr.type()->isBasicClassType()) {
    CHPL_ASSERT(false && "Expected fully decorated class type");

  } else if (auto classType = qtTypeExpr.type()->toClassType()) {
    resolveNewForClass(*this, node, classType);

  } else if (auto recordType = qtTypeExpr.type()->toRecordType()) {
    resolveNewForRecord(*this, node, recordType);

  } else if (auto unionType = qtTypeExpr.type()->toUnionType()) {
    resolveNewForUnion(*this, node, unionType);

  } else {
    if (node->management() != New::DEFAULT_MANAGEMENT) {
      CHPL_REPORT(context, MemManagementNonClass, node, qtTypeExpr.type());
    }

    CHPL_REPORT(context, InvalidNewTarget, node, qtTypeExpr);
  }
}

static const QualifiedType&
getIterKindConstantOrUnknownQuery(Context* context, UniqueString constant) {
  QUERY_BEGIN(getIterKindConstantOrUnknownQuery, context, constant);

  QualifiedType ret = { QualifiedType::UNKNOWN, UnknownType::get(context) };

  if (!constant.isEmpty()) {
    auto ik = EnumType::getIterKindType(context);
    if (auto m = EnumType::getParamConstantsMapOrNull(context, ik)) {
      auto it = m->find(constant);
      if (it != m->end()) ret = it->second;
    }
  }

  return QUERY_END(ret);
}

// This helper resolves by priority order as described in 'IterDetails'.
static IterDetails
resolveIterDetailsInPriorityOrder(Resolver& rv,
                                  bool& outWasIterSigResolved,
                                  const AstNode* astForErr,
                                  const AstNode* iterand,
                                  const QualifiedType& leaderYieldType,
                                  int mask) {
  IterDetails ret;
  bool computedLeaderYieldType = false;
  if (mask & IterDetails::STANDALONE) {
    ret.idxType = resolveIterTypeWithTag(rv, ret.standalone, astForErr,
                                         iterand, USTR("standalone"), {});
    outWasIterSigResolved = (ret.standalone.sig != nullptr);
    if (!ret.idxType.isUnknownOrErroneous()) {
      ret.succeededAt = IterDetails::STANDALONE;
      return ret;
    }
  }

  if (mask & IterDetails::LEADER_FOLLOWER) {
    ret.leaderYieldType = resolveIterTypeWithTag(rv, ret.leader, astForErr,
                                                 iterand, USTR("leader"),
                                                 {});
    computedLeaderYieldType = true;
  } else if (mask & IterDetails::FOLLOWER) {
    ret.leaderYieldType = leaderYieldType;
  }

  if (mask & IterDetails::LEADER_FOLLOWER ||
      mask & IterDetails::FOLLOWER) {
    if (!ret.leaderYieldType.isUnknownOrErroneous()) {
      ret.idxType = resolveIterTypeWithTag(rv, ret.follower, astForErr,
                                           iterand, USTR("follower"),
                                           ret.leaderYieldType);
      outWasIterSigResolved = (ret.follower.sig != nullptr);
      if (!ret.idxType.isUnknownOrErroneous()) {
        ret.succeededAt = computedLeaderYieldType
            ? IterDetails::LEADER_FOLLOWER
            : IterDetails::FOLLOWER;
        return ret;
      }
    }
  }

  if (mask & IterDetails::SERIAL) {
    ret.idxType = resolveIterTypeWithTag(rv, ret.serial, astForErr,
                                         iterand, {}, {});
    outWasIterSigResolved = (ret.serial.sig != nullptr);
    if (!ret.idxType.isUnknownOrErroneous()) {
      ret.succeededAt = IterDetails::SERIAL;
    }
  }

  return ret;
}

static IterDetails resolveIterDetails(Resolver& rv,
                                      const AstNode* astForErr,
                                      const AstNode* iterand,
                                      const QualifiedType& leaderYieldType,
                                      int mask) {
  Context* context = rv.context;

  if (mask == IterDetails::NONE || rv.scopeResolveOnly) {
    // Resolve the iterand as much as possible even if there is nothing to do.
    iterand->traverse(rv);
    return {};
  }

  // Resolve the iterand but suppress errors for now. We'll reissue them
  // next, possibly suppressing a "NoMatchingCandidates" for the iterand if
  // our injected call is successful.
  auto runResult = context->runAndTrackErrors([&](Context* context) {
    iterand->traverse(rv);
    return nullptr;
  });

  // Resolve iterators, stopping immediately when we get a valid yield type.
  bool wasIterSigResolved = false;
  auto ret = resolveIterDetailsInPriorityOrder(rv, wasIterSigResolved,
                                               astForErr, iterand,
                                               leaderYieldType,
                                               mask);

  // Only issue a "not iterable" error if the iterand has a type. If it was
  // not typed then earlier resolution of the iterand will have spit out an
  // approriate error for us already.
  bool skipNoCandidatesError = true;
  if (!wasIterSigResolved) {
    auto& iterandRE = rv.byPostorder.byAst(iterand);
    if (!iterandRE.type().isUnknownOrErroneous()) {
      ret.idxType = CHPL_TYPE_ERROR(context, NonIterable, astForErr, iterand,
                                    iterandRE.type());
    } else {
      skipNoCandidatesError = false;
    }
  }

  // Reissue the errors.
  for (auto& e : runResult.errors()) {
    if (e->type() == NoMatchingCandidates) {
      auto nmc = static_cast<ErrorNoMatchingCandidates*>(e.get());
      auto& f = std::get<0>(nmc->info());
      if (skipNoCandidatesError && f == iterand) continue;
    }
    context->report(std::move(e));
  }

  return ret;
}

static QualifiedType
resolveIterTypeWithTag(Resolver& rv,
                       IterDetails::Pieces& outIterPieces,
                       const AstNode* astForErr,
                       const AstNode* iterand,
                       UniqueString iterKindStr,
                       const QualifiedType& followThisFormal) {
  Context* context = rv.context;
  QualifiedType unknown(QualifiedType::UNKNOWN, UnknownType::get(context));
  QualifiedType error(QualifiedType::UNKNOWN, ErroneousType::get(context));

  auto iterKindFormal = getIterKindConstantOrUnknownQuery(context, iterKindStr);
  bool needStandalone = iterKindStr == USTR("standalone");
  bool needLeader = iterKindStr == USTR("leader");
  bool needFollower = iterKindStr == USTR("follower");
  bool needSerial = iterKindStr.isEmpty();

  // Exit early if we need a parallel iterator and don't have the enum.
  if (!needSerial && iterKindFormal.isUnknown()) return error;

  auto iterKindType = EnumType::getIterKindType(context);
  CHPL_ASSERT(needSerial || (iterKindFormal.type() == iterKindType &&
                             iterKindFormal.hasParamPtr()));

  // Inspect the resolution result to determine what should be done next.
  auto& iterandRE = rv.byPostorder.byAst(iterand);
  auto& MSC = iterandRE.mostSpecific();
  auto fn = MSC.only() ? MSC.only().fn() : nullptr;
  bool wasIterandTypeResolved = !iterandRE.type().isUnknownOrErroneous();
  bool wasIterResolved = fn && fn->isIterator();
  bool wasMatchingIterResolved = wasIterResolved &&
    ((fn->isParallelStandaloneIterator(context) && needStandalone) ||
     (fn->isParallelLeaderIterator(context) && needLeader) ||
     (fn->isParallelFollowerIterator(context) && needFollower) ||
     (fn->isSerialIterator(context) && needSerial));

  QualifiedType ret = error;

  // We resolved the iterator we need right off the bat, so use it.
  if (wasMatchingIterResolved) {
    ret = iterandRE.type();
    outIterPieces = { false, {}, fn };

  // We cannot inject e.g., the 'tag' actual in this case, so error out.
  } else if (needSerial && wasIterResolved) {
    return error;

  // There's nothing to do in this case, so error out.
  } else if (needSerial && !wasIterandTypeResolved) {
    return error;

  // In this branch we prepare a generated iterator call. It could be a call
  // to 'these()' on the iterand type, or it could be a redirect of the
  // existing call with 'iterKind' and (optionally) 'followThis' arguments
  // tacked onto the end.
  } else {
    bool shouldCreateTheseCall = !wasIterResolved && wasIterandTypeResolved;

    // We need to fill in the following pieces to construct a 'CallInfo'.
    UniqueString callName;
    types::QualifiedType callCalledType;
    bool callIsMethodCall = false;
    bool callHasQuestionArg = false;
    bool callIsParenless = false;
    std::vector<CallInfoActual> callActuals;

    // If we are constructing a new 'these()' call then add a receiver.
    if (shouldCreateTheseCall) {
      callName = USTR("these");
      callCalledType = iterandRE.type();
      callIsMethodCall = true;
      callActuals.push_back(CallInfoActual(iterandRE.type(), USTR("this")));

    // The iterand is an unresolved call, or it is a resolved iterator but
    // not the one that we need. Regather existing actuals and reuse the
    // receiver if it is present.
    } else if (auto call = iterand->toCall()) {

      bool raiseErrors = false;
      auto tmp = CallInfo::create(context, call, rv.byPostorder, raiseErrors);

      callName = tmp.name();
      callCalledType = tmp.calledType();
      callIsMethodCall = tmp.isMethodCall();
      callIsParenless = tmp.isParenless();
      for (auto& a : tmp.actuals()) callActuals.push_back(a);
    } else {
      CHPL_UNIMPL("unknown iterand");
      return error;
    }

    if (!needSerial) {
      callActuals.push_back(CallInfoActual(iterKindFormal, USTR("tag")));
    }

    if (needFollower) {
      auto x = CallInfoActual(followThisFormal, USTR("followThis"));
      callActuals.push_back(std::move(x));
    }

    auto ci = CallInfo(std::move(callName),
                       std::move(callCalledType),
                       std::move(callIsMethodCall),
                       std::move(callHasQuestionArg),
                       std::move(callIsParenless),
                       std::move(callActuals));
    auto inScope = rv.scopeStack.back();
    auto inScopes = CallScopeInfo::forNormalCall(inScope, rv.poiScope);
    auto c = resolveGeneratedCall(context, iterand, ci, inScopes);

    outIterPieces = { true, c, c.mostSpecific().only().fn() };
    ret = c.exprType();

    if (!ret.isUnknownOrErroneous()) {
      rv.handleResolvedCall(iterandRE, astForErr, ci, c,
                            { { AssociatedAction::ITERATE, iterand->id() } });
    }
  }

  return ret;
}

static bool resolveParamForLoop(Resolver& rv, const For* forLoop) {
  const AstNode* iterand = forLoop->iterand();
  Context* context = rv.context;

  iterand->traverse(rv);

  if (rv.scopeResolveOnly) {
    rv.enterScope(forLoop);
    return true;
  }

  if (iterand->isRange() == false) {
    context->error(forLoop, "param loops may only iterate over range literals");
  } else {
    // TODO: ranges with strides, '#', and '<'
    const Range* rng = iterand->toRange();
    ResolvedExpression& lowRE = rv.byPostorder.byAst(rng->lowerBound());
    ResolvedExpression& hiRE = rv.byPostorder.byAst(rng->upperBound());
    // TODO: Simplify once we no longer use nullptr for param()
    auto lowParam = lowRE.type().param();
    auto hiParam = hiRE.type().param();
    auto low = lowParam ? lowParam->toIntParam() : nullptr;
    auto hi = hiParam ? hiParam->toIntParam() : nullptr;

    int hiVal = hi->value();
    if (rng->opKind() == Range::OPEN_HIGH) {
      // TODO: overflow issue here; if hiVal is INT_MIN, subtracting would
      // overflow.
      hiVal--;
    }

    if (low == nullptr || hi == nullptr) {
      context->error(forLoop, "param loops may only iterate over range literals with integer bounds");
      return false;
    }

    std::vector<ResolutionResultByPostorderID> loopResults;
    for (int64_t i = low->value(); i <= hiVal; i++) {
      ResolutionResultByPostorderID bodyResults;
      auto cur = Resolver::paramLoopResolver(rv, forLoop, bodyResults);

      cur.enterScope(forLoop);

      ResolvedExpression& idx = cur.byPostorder.byAst(forLoop->index());
      QualifiedType qt = QualifiedType(QualifiedType::PARAM, lowRE.type().type(), IntParam::get(context, i));
      idx.setType(qt);
      forLoop->body()->traverse(cur);

      cur.exitScope(forLoop);

      loopResults.push_back(std::move(cur.byPostorder));
    }

    auto paramLoop = new ResolvedParamLoop(forLoop);
    paramLoop->setLoopBodies(loopResults);
    auto& resolvedLoopExpr = rv.byPostorder.byAst(forLoop);
    resolvedLoopExpr.setParamLoop(paramLoop);
  }

  return false;
}

static void
backpatchArrayTypeSpecifier(Resolver& rv, const IndexableLoop* loop) {
  if (rv.scopeResolveOnly || !loop->isBracketLoop()) return;
  Context* context = rv.context;

  // Check if this is an array
  auto iterandType = rv.byPostorder.byAst(loop->iterand()).type();
  if (!iterandType.isUnknown() && iterandType.type()->isDomainType()) {
    QualifiedType eltType;

    CHPL_ASSERT(loop->isExpressionLevel() && loop->numStmts() <= 1);
    if (loop->numStmts() == 1) {
      eltType = rv.byPostorder.byAst(loop->stmt(0)).type();
    } else if (loop->numStmts() == 0) {
      eltType = QualifiedType(QualifiedType::TYPE, AnyType::get(context));
    }

    // TODO: resolve array types when the iterand is something other than
    // a domain.
    if (eltType.isType() || eltType.kind() == QualifiedType::TYPE_QUERY) {
      eltType = QualifiedType(QualifiedType::TYPE, eltType.type());
      auto arrayType = ArrayType::getArrayType(context, iterandType, eltType);

      auto& re = rv.byPostorder.byAst(loop);
      re.setType(QualifiedType(QualifiedType::TYPE, arrayType));
    }
  }
}

static QualifiedType
resolveZipExpression(Resolver& rv, const IndexableLoop* loop, const Zip* zip) {
  Context* context = rv.context;
  bool loopRequiresParallel = loop->isForall();
  bool loopPrefersParallel = loopRequiresParallel || loop->isBracketLoop();
  QualifiedType ret;

  // We build up tuple element types by resolving all the zip actuals.
  std::vector<QualifiedType> eltTypes;

  // We determine the follower policy by resolving the leader actual.
  auto followerPolicy = IterDetails::NONE;
  QualifiedType leaderYieldType;

  // Get the leader actual.
  if (auto leader = (zip->numActuals() ? zip->actual(0) : nullptr)) {

    // Set the policy mask for the leader based on the loop properties.
    int m = IterDetails::NONE;
    if (loopPrefersParallel) m |= IterDetails::LEADER_FOLLOWER;
    if (!loopRequiresParallel) m |= IterDetails::SERIAL;
    CHPL_ASSERT(m != IterDetails::NONE);

    // Resolve the leader iterator.
    auto dt = resolveIterDetails(rv, leader, leader, {}, m);

    eltTypes.push_back(dt.idxType);

    // Configure what followers should do using the iterator details.
    if (dt.succeededAt == IterDetails::LEADER_FOLLOWER) {
      followerPolicy = IterDetails::FOLLOWER;
      leaderYieldType = dt.leaderYieldType;
    } else if (dt.succeededAt == IterDetails::SERIAL) {
      followerPolicy = IterDetails::SERIAL;
    } else {
      // TODO: Emit an error here informing the user that a usable leader
      // iterator wasn't found. Might be some test failure(s) to fix.
      ret = { QualifiedType::UNKNOWN, ErroneousType::get(context) };
    }
  }

  // Resolve the follower iterator or serial iterator for all followers.
  // It is possible for the follower policy to be 'NONE', in which case
  // no iterators will be resolved, but the follower iterands will be
  // resolved.
  for (int i = 1; i < zip->numActuals(); i++) {
    auto actual = zip->actual(i);
    auto dt = resolveIterDetails(rv, actual, actual, leaderYieldType,
                                 followerPolicy);
    auto& qt = dt.idxType;
    eltTypes.push_back(qt);
  }

  CHPL_ASSERT(((int) eltTypes.size()) == zip->numActuals());

  auto kind = QualifiedType::CONST_VAR;
  for (auto& et : eltTypes) {
    if (!et.isUnknownOrErroneous() && !et.isConst()) {
      kind = QualifiedType::VAR;
      break;
    }
  }

  if (!ret.isErroneousType()) {
    // This 'TupleType' builder preserves references for index types.
    auto type = TupleType::getQualifiedTuple(context, std::move(eltTypes));
    ret = { kind, type };
  }

  auto& reZip = rv.byPostorder.byAst(zip);
  reZip.setType(ret);

  return ret;
}

bool Resolver::enter(const IndexableLoop* loop) {
  auto forLoop = loop->toFor();
  bool isParamForLoop = forLoop != nullptr && forLoop->isParam();

  // whether this is a param or regular loop, before entering its body
  // or considering its iterand, resolve expressions in the loop's attribute
  // group.
  if (auto ag = loop->attributeGroup()) {
    ag->traverse(*this);
  }

  if (isParamForLoop) return resolveParamForLoop(*this, loop->toFor());

  auto iterand = loop->iterand();
  QualifiedType idxType;

  if (iterand->isZip()) {
    idxType = resolveZipExpression(*this, loop, iterand->toZip());

  } else {
    bool loopRequiresParallel = loop->isForall();
    bool loopPrefersParallel = loopRequiresParallel || loop->isBracketLoop();

    int m = IterDetails::NONE;
    if (loopPrefersParallel) m |= IterDetails::LEADER_FOLLOWER |
                                  IterDetails::STANDALONE;
    if (!loopRequiresParallel) m |= IterDetails::SERIAL;
    CHPL_ASSERT(m != IterDetails::NONE);

    auto dt = resolveIterDetails(*this, loop, iterand, {}, m);
    idxType = dt.idxType;
  }

  enterScope(loop);

  if (const Decl* idx = loop->index()) {
    ResolvedExpression& re = byPostorder.byAst(idx);
    re.setType(idxType);
  }

  if (auto with = loop->withClause()) {
    with->traverse(*this);
  }

  loop->body()->traverse(*this);

  // TODO: Resolve the loop body first when it looks like an array type,
  // and if the body is a type, then skip resolving iterators to save time.
  backpatchArrayTypeSpecifier(*this, loop);

  return false;
}

void Resolver::exit(const IndexableLoop* loop) {
  // Param loops handle scope differently
  auto forLoop = loop->toFor();
  bool isParamForLoop = forLoop != nullptr && forLoop->isParam();

  if (isParamForLoop == false || scopeResolveOnly) {
    exitScope(loop);
  }
}

bool Resolver::enter(const DoWhile* loop) {
  enterScope(loop);

  // traversing the block directly will push its scope onto the stack, which would
  // duplicate the enter we'll do here. So we just visit the children manually.
  enterScope(loop->body());
  for (auto child : loop->body()->children()) {
    child->traverse(*this);
  }
  // traverse the condition in the same scope as the loop body.
  loop->condition()->traverse(*this);
  exitScope(loop->body());

  return false;
}

void Resolver::exit(const DoWhile* loop) {
  exitScope(loop);
}


// Returns 'true' if a single Id was scope-resolved, in which case the function
// will also return via the ID and QualifiedType formals.
static bool computeTaskIntentInfo(Resolver& resolver, const NamedDecl* intent,
                                  ID& resolvedId, QualifiedType& type) {
  auto& scopeStack = resolver.scopeStack;

  // Look at the scope before the loop-statement
  const Scope* scope = scopeStack[scopeStack.size()-2];
  LookupConfig config = LOOKUP_DECLS |
                        LOOKUP_IMPORT_AND_USE |
                        LOOKUP_PARENTS |
                        LOOKUP_INNERMOST;

  auto fHelper = resolver.getFieldDeclarationLookupHelper();
  auto helper = resolver.getMethodReceiverScopeHelper();

  auto ids = lookupNameInScope(resolver.context, scope,
                               /* methodLookupHelper */ fHelper,
                               /* receiverScopeHelper */ helper,
                               intent->name(), config);

  if (ids.numIds() == 1) {
    resolvedId = ids.firstId();
    if (resolver.scopeResolveOnly == false) {
      if (resolvedId.isEmpty()) {
        type = typeForBuiltin(resolver.context, intent->name());
      } else {
        type = resolver.typeForId(resolvedId, /*localGenericToUnknown*/ true);
      }
    }
    return true;
  } else {
    return false;
  }
}

bool Resolver::enter(const ReduceIntent* reduce) {
  ID id;
  QualifiedType type;
  ResolvedExpression& result = byPostorder.byAst(reduce);

  if (computeTaskIntentInfo(*this, reduce, id, type)) {
    validateAndSetToId(result, reduce, id);
    // set reduce intent shadow variable to a VAR with type of shadowed variable
    QualifiedType reduceIntentType =
        QualifiedType(QualifiedType::Kind::VAR, type.type());
    result.setType(reduceIntentType);
  } else if (!scopeResolveOnly) {
    context->error(reduce, "Unable to find declaration of \"%s\" for reduction", reduce->name().c_str());
  }

  // TODO: Resolve reduce->op() with shadowed type
  // E.g. "+ reduce x" --> "SumReduceOp(x.type)"

  return false;
}

void Resolver::exit(const ReduceIntent* reduce) {
}

static UniqueString identifierReduceScanOpName(Context* context,
                                               UniqueString name) {
  if (name == USTR("+")) return UniqueString::get(context, "SumReduceScanOp");
  if (name == USTR("*")) return UniqueString::get(context, "ProductReduceScanOp");
  if (name == USTR("&&")) return UniqueString::get(context, "LogicalAndReduceScanOp");
  if (name == USTR("||")) return UniqueString::get(context, "LogicalOrReduceScanOp");
  if (name == USTR("&")) return UniqueString::get(context, "BitwiseAndReduceScanOp");
  if (name == USTR("|")) return UniqueString::get(context, "BitwiseOrReduceScanOp");
  if (name == USTR("^")) return UniqueString::get(context, "BitwiseXorReduceScanOp");

  if (name == USTR("max")) return UniqueString::get(context, "MaxReduceScanOp");
  if (name == USTR("min")) return UniqueString::get(context, "MinReduceScanOp");

  return UniqueString();
}

static const ClassType *
constructReduceScanOpClass(Resolver& resolver,
                           const uast::AstNode* reduceOrScan,
                           UniqueString opName,
                           const QualifiedType& iterType) {
  auto context = resolver.context;
  auto actualType = QualifiedType(Qualifier::TYPE, iterType.type());

  std::vector<CallInfoActual> actuals;
  actuals.push_back(CallInfoActual(actualType, UniqueString()));
  auto ci = CallInfo (/* name */ opName,
                      /* calledType */ QualifiedType(),
                      /* isMethodCall */ false,
                      /* hasQuestionArg */ false,
                      /* isParenless */ false,
                      actuals);
  const Scope* scope = scopeForId(context, reduceOrScan->id());
  auto inScopes = CallScopeInfo::forNormalCall(scope, resolver.poiScope);
  auto c = resolveGeneratedCall(context, reduceOrScan, ci, inScopes);
  auto opType = c.exprType();

  // Couldn't resolve the call; is opName a valid reduction?
  if (opType.isUnknown()) {
    CHPL_REPORT(context, ReductionInvalidName, reduceOrScan, opName, iterType);
    return nullptr;
  } else {
    resolver.handleResolvedCall(resolver.byPostorder.byAst(reduceOrScan),
                                reduceOrScan, ci, c,
                                { { AssociatedAction::REDUCE_SCAN, reduceOrScan->id() } });
  }

  // We found some type; is it a subclass of ReduceScanOp?
  auto baseClass = BasicClassType::getReduceScanOpType(context);
  auto actualClass = opType.type()->toClassType();
  bool converts, instantiates;
  if (opType.kind() != QualifiedType::TYPE ||
      !actualClass ||
      !actualClass->basicClassType() ||
      !actualClass->basicClassType()->isSubtypeOf(baseClass, converts, instantiates)) {
    CHPL_REPORT(context, ReductionNotReduceScanOp, reduceOrScan, opType);
  }

  return actualClass;
}

static const ClassType* determineReduceScanOp(Resolver& resolver,
                                              const uast::AstNode* reduceOrScan,
                                              const uast::AstNode* op,
                                              const QualifiedType& iterType) {
  if (auto ident = op->toIdentifier()) {
    auto toLookUp = ident->name();
    auto opName = identifierReduceScanOpName(resolver.context, ident->name());
    if (!opName.isEmpty()) {
      // The identifier does not itself name a ReduceScanOp class, but is
      // associated with such a class. Use the associated class' name instead
      // of the identifier itself.
      toLookUp = opName;
    }
    auto scanOp = constructReduceScanOpClass(resolver, reduceOrScan, toLookUp, iterType);
    if (scanOp != nullptr) {
      // Since we found a ReduceScanOp, set the refersToId of the identifier.
      //
      // It's safe to call basicClassType since constructReduceScanOpClass
      // ensures it's a basic class type.
      resolver.validateAndSetToId(resolver.byPostorder.byAst(ident),
                                  ident, scanOp->basicClassType()->id());
    }
    // No further processing is needed; we found the operation.
    return scanOp;
  }

  // In the future, node could be a value or something else, and this function
  // would return the corresponding ClassType.

  return nullptr;
}

static QualifiedType getReduceScanOpResultType(Resolver& resolver,
                                               const uast::AstNode* reduceOrScan,
                                               const ClassType* opClass) {
  auto context = resolver.context;

  auto borrowedDecorator =
    ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
  auto borrowedClass = opClass->withDecorator(context,
                                              std::move(borrowedDecorator));
  auto thisActual = QualifiedType(Qualifier::CONST_IN, borrowedClass);

  std::vector<CallInfoActual> typeActuals;
  typeActuals.push_back(CallInfoActual(thisActual, USTR("this")));
  auto ci = CallInfo (/* name */ USTR("generate"),
                      /* calledType */ thisActual,
                      /* isMethodCall */ true,
                      /* hasQuestionArg */ false,
                      /* isParenless */ false,
                      typeActuals);
  const Scope* scope = scopeForId(context, reduceOrScan->id());
  auto inScopes = CallScopeInfo::forNormalCall(scope, resolver.poiScope);
  auto c = resolveGeneratedCall(context, reduceOrScan, ci, inScopes);
  return c.exprType();
}

static QualifiedType resolveReduceScanOp(Resolver& resolver,
                                         const AstNode* reduceOrScan,
                                         const AstNode* op,
                                         const AstNode* iterand) {
  auto dt = resolveIterDetails(resolver, reduceOrScan, iterand, {},
                               IterDetails::SERIAL);
  auto idxType = dt.idxType;
  if (idxType.isUnknown()) return QualifiedType();
  auto opClass = determineReduceScanOp(resolver, reduceOrScan, op, idxType);
  if (opClass == nullptr) return QualifiedType();

  return getReduceScanOpResultType(resolver, reduceOrScan, opClass);
}

bool Resolver::enter(const uast::Reduce* reduce) {
  auto elementType = resolveReduceScanOp(*this, reduce,
                                         reduce->op(), reduce->iterand());
  byPostorder.byAst(reduce).setType(std::move(elementType));
  return false;
}

void Resolver::exit(const uast::Reduce* reduce) {
}

// helper to determine if a TaskVar is a task intent
static bool isTaskIntent(const TaskVar* taskVar) {
  return taskVar->typeExpression() == nullptr &&
         taskVar->initExpression() == nullptr;
}

bool Resolver::enter(const TaskVar* taskVar) {
  if (isTaskIntent(taskVar)) {
    ID id;
    QualifiedType type;
    ResolvedExpression& result = byPostorder.byAst(taskVar);
    if (computeTaskIntentInfo(*this, taskVar, id, type)) {
      QualifiedType taskVarType = QualifiedType(taskVar->storageKind(),
                                                type.type());
      validateAndSetToId(result, taskVar, id);

      // TODO: Handle in-intents where type can change (e.g. array slices)
      result.setType(taskVarType);
    } else if (!scopeResolveOnly) {
      context->error(taskVar, "Unable to find declaration of \"%s\" for task intent", taskVar->name().c_str());
    }
    return false;
  } else {
    enterScope(taskVar);
    return true;
  }
}

void Resolver::exit(const TaskVar* taskVar) {
  if (!isTaskIntent(taskVar)) {
    exitScope(taskVar);
  }
}

bool Resolver::enter(const Return* ret) {
  return true;
}

void Resolver::exit(const Return* ret) {
  if (initResolver) {
    initResolver->checkEarlyReturn(ret);
  }
}

bool Resolver::enter(const Throw* node) {
  return true;
}

void Resolver::exit(const Throw* node) {
  if (initResolver) {
    context->error(node, "initializers are not yet allowed to throw errors");
  }
}

bool Resolver::enter(const Try* node) {
  enterScope(node);
  return true;
}

void Resolver::exit(const Try* node) {
  if (initResolver && node->isTryBang() && node->numHandlers() > 0) {
    context->error(node, "Only catch-less try! statements are allowed in initializers for now");
  }
  // Node inherits the type of its expression if it is not a statement.
  if (node->isExpressionLevel()) {
    auto expr = node->stmt(0);
    CHPL_ASSERT(expr);
    auto& reNode = byPostorder.byAst(node);
    auto& reExpr = byPostorder.byAst(expr);
    reNode.setType(reExpr.type());
  }
  exitScope(node);
}

bool Resolver::enter(const Catch* node) {
  enterScope(node);
  if (scopeResolveOnly) {
    return true;
  }

  if (auto errVar = node->error()) {
    CHPL_ASSERT(errVar->initExpression() == nullptr &&
      "catch variable should not have an init expression");

    const AstNode* typeExpr = errVar->typeExpression();
    if (typeExpr == nullptr) {
      // default to Error placeholder
      const ClassType* errorType = CompositeType::getErrorType(context);
      auto qt = QualifiedType(QualifiedType::VAR, errorType);
      ResolvedExpression& re = byPostorder.byAst(errVar);
      re.setType(qt);
    } else {
      errVar->traverse(*this);
    }

    ResolvedExpression& re = byPostorder.byAst(errVar);

    bool isBasicClass = false;
    if (auto ct = re.type().type()->toClassType()) {
      bool converts = false;
      bool instantiates = false;
      if (auto bct = ct->basicClassType()) {
        isBasicClass = true;
        if (!bct->isSubtypeOf(CompositeType::getErrorType(context)->basicClassType(), converts, instantiates)) {
          // get the penultimate type in the chain
          while (!bct->parentClassType()->isObjectType()) {
            bct = bct->parentClassType();
          }
          context->error(errVar, "catch variable '%s' must be a class that inherits from Error, not '%s'", errVar->name().c_str(), bct->name().c_str());
        } else {
          auto dec = ClassTypeDecorator(ClassTypeDecorator::MANAGED_NONNIL);
          auto manager = AnyOwnedType::get(context);
          auto ret = ClassType::get(context, bct, manager, dec);
          auto qt = QualifiedType(re.type().kind(), ret->withDecorator(context, dec));
          re.setType(qt); // replace type
        }
      }
    }

    if (!isBasicClass) {
      context->error(errVar, "catch variable '%s' must be a class that inherits from Error", errVar->name().c_str());
    }
  } // TODO: is there an else case to handle here for catchall without an error variable (e.g. catch {})?

  // do traverse of body
  if (auto body = node->body()) {
    body->traverse(*this);
  }

  return false;
}

void Resolver::exit(const Catch* node) {
  exitScope(node);
}

// Do not visit children. Un-ambiguous symbols will have warnings emitted
// for them in scope resolve.
bool Resolver::enter(const Use* node) {
  const Scope* scope = scopeStack.back();
  CHPL_ASSERT(scope);
  std::ignore = resolveVisibilityStmts(context, scope);
  emitMultipleDefinedSymbolErrors(context, scope);
  return false;
}

void Resolver::exit(const Use* node) {}

// Ditto the above.
bool Resolver::enter(const Import* node) {
  const Scope* scope = scopeStack.back();
  CHPL_ASSERT(scope);
  std::ignore = resolveVisibilityStmts(context, scope);
  emitMultipleDefinedSymbolErrors(context, scope);
  return false;
}

void Resolver::exit(const Import* node) {}

bool Resolver::enter(const AstNode* ast) {
  enterScope(ast);

  bool skipChildren = signatureOnly && ast == fnBody;
  return !skipChildren;
}
void Resolver::exit(const AstNode* ast) {
  exitScope(ast);
}


} // end namespace resolution
} // end namespace chpl
