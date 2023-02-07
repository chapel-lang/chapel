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

#include "Resolver.h"

#include "chpl/framework/global-strings.h"
#include "chpl/framework/query-impl.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/can-pass.h"
#include "chpl/resolution/disambiguation.h"
#include "chpl/resolution/intents.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"

#include "InitResolver.h"
#include "VarScopeVisitor.h"

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"

#include <cstdio>
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
                                          ResolutionResultByPostorderID& byId) {
  auto ret = Resolver(context, fn, byId, nullptr);
  ret.typedSignature = nullptr; // re-set below
  ret.signatureOnly = true; // re-set below
  ret.scopeResolveOnly = true;
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

  return ret;
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

/**
  Find scopes for superclasses of a class.  The passed ID should refer to a
  Class declaration node.  If not, this function will return an empty vector.

  This function is temporary and should only be used in scopeResolveOnly mode.
  */
static const std::vector<const Scope*>&
gatherParentClassScopesForScopeResolving(Context* context, ID classDeclId) {
  QUERY_BEGIN(gatherParentClassScopesForScopeResolving, context, classDeclId);

  std::vector<const Scope*> result;

  ID curId = classDeclId;

  while (!curId.isEmpty()) {
    if (auto ast = parsing::idToAst(context, curId)) {
      if (auto c = ast->toClass()) {
        if (const AstNode* parentClassExpr = c->parentClass()) {
          // Resolve the parent class type expression
          ResolutionResultByPostorderID r;
          auto visitor =
            Resolver::createForParentClassScopeResolve(context, c, r);
          parentClassExpr->traverse(visitor);

          ResolvedExpression& re = r.byAst(parentClassExpr);
          if (re.toId().isEmpty()) {
            context->error(parentClassExpr, "invalid parent class expression");
          } else {
            result.push_back(scopeForId(context, re.toId()));
            curId = re.toId();
            continue; // keep going with parent classes of that class
          }
        }
      }
    }
    break; // stop if we get to this point
  }

  return QUERY_END(result);
}



Resolver::ReceiverScopesVec
Resolver::gatherReceiverAndParentScopesForDeclId(Context* context,
                                                 ID aggregateDeclId) {
  ReceiverScopesVec scopes;

  if (aggregateDeclId.isEmpty()) {
    return scopes;
  }

  // use temporary code to scope resolve the parent class Identifiers
  scopes.push_back(scopeForId(context, aggregateDeclId));
  // if it's a class type, also gather the parent class scopes
  auto tag = parsing::idToTag(context, aggregateDeclId);
  if (asttags::isClass(tag)) {
    const std::vector<const Scope*>& v =
      gatherParentClassScopesForScopeResolving(context, aggregateDeclId);

    for (auto elt : v) {
      scopes.push_back(elt);
    }
  }

  return scopes;
}

Resolver::ReceiverScopesVec
Resolver::gatherReceiverAndParentScopesForType(Context* context,
                                               const types::Type* thisType) {
  ReceiverScopesVec scopes;

  if (thisType != nullptr) {
    // use the regular type system to scope resolve parent classes
    const Type* currentType = thisType->toCompositeType();
    while (currentType) {
      auto ct = currentType->toCompositeType();
      scopes.push_back(scopeForId(context, ct->id()));
      auto bct = ct->toBasicClassType();
      if (!bct) break;
      currentType = bct->parentClassType();
    }
  }

  return scopes;
}

Resolver::ReceiverScopesVec Resolver::methodReceiverScopes(bool recompute) {
  if (recompute) {
    receiverScopesComputed = false;
  }

  if (receiverScopesComputed) return savedReceiverScopes;

  if (!scopeResolveOnly &&
      typedSignature &&
      typedSignature->untyped()->isMethod()) {
    // use type information to compute the receiver type
    auto receiverType = typedSignature->formalType(0).type();
    // and use that to compute the scopes
    savedReceiverScopes =
      gatherReceiverAndParentScopesForType(context, receiverType);
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
      ID idForTypeDecl;
      if (type == nullptr) {
        // `this` formals of primary methods have no type expression. They
        // are, however, in primary methods, so the method's parent is the
        // aggregate type whose scope should be used.

        idForTypeDecl = parsing::idToParentId(context, methodId);
      } else if (auto ident = type->toIdentifier()) {
        idForTypeDecl = byPostorder.byAst(ident).toId();
      }
      savedReceiverScopes =
        gatherReceiverAndParentScopesForDeclId(context, idForTypeDecl);
    }
  }

  receiverScopesComputed = true;
  return savedReceiverScopes;
}

const CompositeType* Resolver::methodReceiverType() {
  if (typedSignature && typedSignature->untyped()->isMethod()) {
    if (auto receiverType = typedSignature->formalType(0).type()) {
      if (auto ret = receiverType->getCompositeType()) {
        return ret;
      }
    }
  }
  return nullptr;
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

static void varArgTypeQueryError(Context* context,
                                 const AstNode* node,
                                 ResolvedExpression& result) {
  context->error(node, "Cannot query type of variable arguments formal when types are not homogeneous");
  auto errType = QualifiedType(QualifiedType::TYPE,
                               ErroneousType::get(context));
  result.setType(errType);
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
        const uast::Decl* field = fa->formal();
        const SubstitutionsMap& subs = actualCt->substitutions();
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


const Type* Resolver::tryResolveCrossTypeInitEq(const AstNode* ast,
                                                QualifiedType lhsType,
                                                QualifiedType rhsType) {

  const Type* t = lhsType.type();
  if (t->isRecordType() || t->isUnionType()) {
    // use the regular VAR kind for this query
    // (don't want a type-expr lhsType to be considered a TYPE here)
    lhsType = QualifiedType(QualifiedType::VAR, lhsType.type());
    rhsType = QualifiedType(QualifiedType::VAR, rhsType.type());

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
    auto c = resolveGeneratedCall(context, ast, ci, scope, poiScope);
    if (c.mostSpecific().isEmpty()) {
      return nullptr;
    } else {
      return lhsType.type(); // TODO: this might need to be an instantiation
    }
  }

  return nullptr;
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
    // init but no declared type, so use init type
    typePtr = initExprType.type();
    if (inferParam) {
      paramPtr = initExprType.param();
    }
  } else {
    // otherwise both declaredType and initExprType are provided.
    // check that they are compatible
    auto got = canPass(context, initExprType,
                       QualifiedType(declKind, declaredType.type()));
    if (!got.passes()) {
      // For a record/union, check for an init= from the provided type
      const Type* foundInitEqResultType =
        tryResolveCrossTypeInitEq(declForErr, declaredType, initExprType);

      if (!foundInitEqResultType) {
        CHPL_REPORT(context, IncompatibleTypeAndInit, declForErr, typeForErr,
                    initForErr, declaredType.type(), initExprType.type());
        typePtr = ErroneousType::get(context);
      } else {
        typePtr = foundInitEqResultType;
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
        auto t = getInstantiationType(context, initExprType, declaredType);
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
      typePtr = TupleType::getVarArgTuple(context, paramSize, elt);
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
        typeExprT = computeTypeDefaults(*this, parentType);
      }

      // for 'this' formals of class type, adjust them to be borrowed, so
      // e.g. proc C.foo() { } has 'this' of type 'borrowed C'.
      // This should not apply to parenthesized expressions.
      if (isFormalThis &&
          typeExprT.type() != nullptr && typeExprT.type()->isClassType() &&
          typeExpr->isIdentifier()) {
        auto ct = typeExprT.type()->toClassType();
        auto dec = ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
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

    if (!typeExprT.hasTypePtr() && useType != nullptr) {
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
          if (isTypeOrParam && isField) {
            // a type or param field with initExpr is still generic, e.g.
            // record R { type t = int; }
            // if that behavior is requested with defaultsPolicy == IGNORE_DEFAULTS
            typeExprT = QualifiedType(QualifiedType::TYPE,
                                      AnyType::get(context));
          }
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
  } else if (isFormal || (signatureOnly && isField)) {
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

  // adjust tuple declarations for value / referential tuples
  if (typePtr != nullptr && decl->isVarArgFormal() == false) {
    if (auto tupleType = typePtr->toTupleType()) {
      if (declaredKind == QualifiedType::DEFAULT_INTENT ||
          declaredKind == QualifiedType::CONST_INTENT) {
        typePtr = tupleType->toReferentialTuple(context);
      } else if (qtKind == QualifiedType::VAR ||
                 qtKind == QualifiedType::CONST_VAR ||
                 qtKind == QualifiedType::CONST_REF ||
                 qtKind == QualifiedType::REF ||
                 qtKind == QualifiedType::IN ||
                 qtKind == QualifiedType::CONST_IN ||
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
      context->error(astForErr,
                     "Cannot resolve call to '%s': no matching candidates",
                     ci.name().c_str());
    }
  } else {
    context->error(astForErr, "Cannot establish type for call expression");

    // expecting call site to check for hasTypePtr.
    CHPL_ASSERT(!c.exprType().hasTypePtr());
  }
}

void Resolver::handleResolvedCall(ResolvedExpression& r,
                                  const uast::AstNode* astForErr,
                                  const CallInfo& ci,
                                  const CallResolutionResult& c) {

  if (!c.exprType().hasTypePtr()) {
    issueErrorForFailedCallResolution(astForErr, ci, c);
    r.setType(QualifiedType(r.type().kind(), ErroneousType::get(context)));
  } else {
    r.setMostSpecific(c.mostSpecific());
    r.setPoiScope(c.poiInfo().poiScope());
    r.setType(c.exprType());
    // gather the poi scopes used when resolving the call
    poiInfo.accumulate(c.poiInfo());
  }
}

void Resolver::handleResolvedAssociatedCall(ResolvedExpression& r,
                                            const uast::AstNode* astForErr,
                                            const CallInfo& ci,
                                            const CallResolutionResult& c,
                                            AssociatedAction::Action action,
                                            ID id) {
  if (!c.exprType().hasTypePtr()) {
    issueErrorForFailedCallResolution(astForErr, ci, c);
  } else {
    // save candidates as associated functions
    for (auto sig : c.mostSpecific()) {
      if (sig != nullptr) {
        r.addAssociatedAction(action, sig, id);
      }
    }
    // gather the poi scopes used when resolving the call
    poiInfo.accumulate(c.poiInfo());
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
                         id.postOrderId() >= 0);

  // if the the lhs variable has not been initialized and
  // the type of LHS is generic or unknown, update it based on the RHS type.

  if (!useLocalResult) {
    return;
  }

  ResolvedExpression& lhs = byPostorder.byId(id);
  QualifiedType lhsType = lhs.type();

  // check to see if it is generic/unknown
  // (otherwise we do not need to infer anything)
  auto g = Type::MAYBE_GENERIC;
  if (lhsType.isUnknownKindOrType()) {
     // includes nullptr type, UnknownType, and param with unknown value
     g = Type::GENERIC;
  } else {
    CHPL_ASSERT(lhsType.type()); // should not be nullptr b/c of isUnknownKindOrType
    g = getTypeGenericity(context, lhsType.type());
  }

  // return if there's nothing to do
  if (g != Type::GENERIC) {
    return;
  }

  const Param* p = rhsType.param();
  if (lhsType.kind() != QualifiedType::PARAM) {
    p = nullptr;
  }
  auto useType = QualifiedType(lhsType.kind(), rhsType.type(), p);

  // set the type for the 1st split init only
  // a later traversal will check the type of subsequent split inits
  // (in the other branch of a conditional, say)
  auto pair = splitInitTypeInferredVariables.insert(id);
  if (pair.second) {
    // insertion took place, so update the type
    lhs.setType(useType);

    if (lhsExprAst != nullptr) {
      ResolvedExpression& lhsExpr = byPostorder.byAst(lhsExprAst);
      lhsExpr.setType(useType);
    }
  } else {
    // insertion did not take place, so check that the type matches exactly,
    // and issue an error if not.
    // (we cannot unify the types for split init without causing resolution
    //  to either go out of order or to produce results that change within
    //  a function even when there are no errors).

    if (lhsType != useType) {
      context->error(astForError, "split-init type does not match");
    }
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
  for (auto actual : ci.actuals()) {
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

      auto c = resolveGeneratedCall(context, actual, ci, scope, poiScope);
      handleResolvedAssociatedCall(r, astForErr, ci, c,
                                   AssociatedAction::ASSIGN,
                                   lhsTuple->id());
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

  if (rhsT == nullptr) {
    CHPL_REPORT(context, TupleDeclNotTuple, lhsTuple, rhsType.type());
    return;
  }

  // Then, check that they have the same size
  if (lhsTuple->numDecls() != rhsT->numElements()) {
    CHPL_REPORT(context, TupleDeclMismatchedElems, lhsTuple, rhsT);
    return;
  }

  // Finally, try to resolve the types of the elements
  int i = 0;
  for (auto actual : lhsTuple->decls()) {
    QualifiedType rhsEltType = rhsT->elementType(i);
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

  // Figure out the type to use for this tuple
  if (useType != nullptr) {
    useT = QualifiedType(declKind, useType);
  } else {
    QualifiedType typeExprT;
    QualifiedType initExprT;

    auto typeExpr = td->typeExpression();
    auto initExpr = td->initExpression();

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

  if (!useT.hasTypePtr()) {
    context->error(td, "Cannot establish type for tuple decl");
    useT = QualifiedType(declKind, ErroneousType::get(context));
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

  // note: the resolution machinery will get compiler generated candidates
  auto crr = resolveGeneratedCall(context, call, ci, inScope, inPoiScope);

  CHPL_ASSERT(crr.mostSpecific().numBest() <= 1);

  // there should be one or zero applicable candidates
  if (auto initTfs = crr.mostSpecific().only()) {
    handleResolvedAssociatedCall(re, call, ci, crr,
                                 AssociatedAction::NEW_INIT,
                                 call->id());

    // Set the final output type based on the result of the 'new' call.
    auto qtInitReceiver = initTfs->formalType(0);
    auto type = qtInitReceiver.type();

    // Preserve original management if receiver is a class.
    if (auto cls = type->toClassType()) {
      auto newCls = qtNewExpr.type()->toClassType();
      CHPL_ASSERT(newCls);
      CHPL_ASSERT(!cls->manager() && cls->decorator().isNonNilable());
      CHPL_ASSERT(cls->decorator().isBorrowed());
      type = ClassType::get(context, cls->basicClassType(),
                            newCls->manager(),
                            newCls->decorator());
    }

    auto qt = QualifiedType(QualifiedType::VAR, type);
    re.setType(qt);

  } else {
    issueErrorForFailedCallResolution(call, ci, crr);
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

bool Resolver::resolveSpecialCall(const Call* call) {
  if (resolveSpecialOpCall(call)) {
    return true;
  } else if (resolveSpecialNewCall(call)) {
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
                         id.postOrderId() >= 0);
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
  // Figure out what ID is contained within so we can use the
  // appropriate query.
  ID parentId = id.parentSymbolId(context);
  auto parentTag = asttags::Module;
  if (!parentId.isEmpty()) {
    parentTag = parsing::idToTag(context, parentId);
  }
  const Scope* mReceiverScopes = nullptr;
  auto receiverScopess = methodReceiverScopes();
  if (!receiverScopess.empty()) {
    mReceiverScopes = receiverScopess.front();
  }

  if (asttags::isModule(parentTag)) {
    // If the id is contained within a module, use typeForModuleLevelSymbol.
    return typeForModuleLevelSymbol(context, id);
  } else if (asttags::isAggregateDecl(parentTag) || mReceiverScopes) {
    // If the id is contained within a class/record/union, get the
    // resolved field.
    const CompositeType* ct = nullptr;
    if (parentId == symbol->id()) {
      ct = inCompositeType;
    } else if (mReceiverScopes) {
      // TODO: in this case, we should look for parenless methods.
      ct = methodReceiverType();
    } else {
      CHPL_ASSERT(false && "case not handled");
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
  }

  // Otherwise it is a case not handled yet
  // TODO: handle outer function variables
  CHPL_ASSERT(false && "not yet handled");
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
      bool ok = t->isClassType() || t->isBasicClassType();
      if (!ok) CHPL_REPORT(context, IfVarNonClassType, cond, reVar.type());
    }
  }

  auto& condType = byPostorder.byAst(cond->condition()).type();
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

  // We might as well visit the rest of the children here,
  // since returning `true` at this point would cause a second visit
  // to `cond->condition()`.
  auto thenBlock = cond->thenBlock();
  auto elseBlock = cond->elseBlock();
  thenBlock->traverse(*this);
  if (elseBlock) elseBlock->traverse(*this);

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
      r.setType(ifType.getValue());
    }
  }
  return false;
}
void Resolver::exit(const uast::Conditional* cond) {
}

bool Resolver::enter(const Literal* literal) {
  ResolvedExpression& result = byPostorder.byAst(literal);
  result.setType(typeForLiteral(context, literal));
  return false;
}
void Resolver::exit(const Literal* literal) {
}

std::vector<BorrowedIdsWithName>
Resolver::lookupIdentifier(const Identifier* ident,
                           llvm::ArrayRef<const Scope*> receiverScopes) {
  CHPL_ASSERT(scopeStack.size() > 0);
  const Scope* scope = scopeStack.back();

  bool resolvingCalledIdent = (inLeafCall &&
                               ident == inLeafCall->calledExpression());

  LookupConfig config = LOOKUP_DECLS |
                        LOOKUP_IMPORT_AND_USE |
                        LOOKUP_PARENTS;

  if (!resolvingCalledIdent) config |= LOOKUP_INNERMOST;

  auto vec =
      lookupNameInScope(context, scope, receiverScopes, ident->name(), config);

  return vec;
}

void Resolver::validateAndSetToId(ResolvedExpression& r,
                                  const AstNode* node,
                                  const ID& id) {
  r.setToId(id);
  if (id.isEmpty()) return;

  // Validate the newly set to ID.

  // It shouldn't refer to a module unless the node is an identifier in one of
  // the places where module references are allowed (e.g. imports).
  auto toAst = parsing::idToAst(context, id);
  if (toAst != nullptr) {
    if (auto mod = toAst->toModule()) {
      auto parentId = parsing::idToParentId(context, node->id());
      if (!parentId.isEmpty()) {
        auto parentAst = parsing::idToAst(context, parentId);
        if (!parentAst->isUse() && !parentAst->isImport() &&
            !parentAst->isAs() && !parentAst->isVisibilityClause() &&
            !parentAst->isDot()) {
          CHPL_REPORT(context, ModuleAsVariable, node, parentAst, mod);
        }
      }
    }
  }

  // If we're in a nested class, it shouldn't refer to an outer class' field.
  auto scope = scopeForId(context, id);
  auto parentId = scope->id();
  auto parentAst = parsing::idToAst(context, parentId);
  if (parentAst && parentAst->isAggregateDecl() &&
      parentId.contains(node->id())) {
    // Referring to a field of a class that's surrounding the current node.
    // Loop upwards looking for a composite type.
    auto searchId = parsing::idToParentId(context, node->id());
    while (!searchId.isEmpty()) {
      auto searchAst = parsing::idToAst(context, searchId);
      if (searchAst == parentAst) {
        // We found the aggregate type in which the to-ID is declared,
        // so there's no nested class issues.
        break;
      } else if (auto searchAD = searchAst->toAggregateDecl()) {
        // It's an error!
        CHPL_REPORT(context, NestedClassFieldRef, parentAst->toAggregateDecl(),
                    searchAD, node, id);
        break;
      }

      // Move on to the surrounding ID.
      searchId = parsing::idToParentId(context, searchId);
    }
  }
}

static bool isCalledExpression(Resolver* rv, const AstNode* ast) {
  if (!ast) return false;

  auto p = parsing::parentAst(rv->context, ast);
  if (!p) return false;

  if (auto call = p->toCall())
    if (auto ce = call->calledExpression())
      return ce == ast;

  return false;
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
  if (qt.kind() != QualifiedType::PARENLESS_FUNCTION &&
      !isCalledExpression(rv, astMention)) {
    ID idMention = astMention->id();
    Context* context = rv->context;
    parsing::reportDeprecationWarningForId(context, idMention, idTarget);
    parsing::reportUnstableWarningForId(context, idMention, idTarget);
  }
}

bool Resolver::resolveIdentifier(const Identifier* ident,
                                 llvm::ArrayRef<const Scope*> receiverScopes) {
  ResolvedExpression& result = byPostorder.byAst(ident);

  // for 'proc f(arg:?)' need to set 'arg' to have type AnyType
  CHPL_ASSERT(declStack.size() > 0);
  const Decl* inDecl = declStack.back();
  if (inDecl->isVarLikeDecl() && ident->name() == USTR("?")) {
    result.setType(QualifiedType(QualifiedType::TYPE, AnyType::get(context)));
    return false;
  }

  auto vec = lookupIdentifier(ident, receiverScopes);

  if (vec.size() == 0) {
    result.setType(QualifiedType());
  } else if (vec.size() > 1 || vec[0].numIds() > 1) {
    // can't establish the type. If this is in a function
    // call, we'll establish it later anyway.
  } else {
    // vec.size() == 1 and vec[0].numIds() <= 1
    const ID& id = vec[0].firstId();
    QualifiedType type;

    // empty IDs from the scope resolution process are builtins
    if (id.isEmpty()) {
      type = typeForBuiltin(context, ident->name());
      result.setToId(id);
      result.setType(type);
      return false;
    }

    // use the type established at declaration/initialization,
    // but for things with generic type, use unknown.
    type = typeForId(id, /*localGenericToUnknown*/ true);

    maybeEmitWarningsForId(this, type, ident, id);

    if (type.kind() == QualifiedType::TYPE) {
      // now, for a type that is generic with defaults,
      // compute the default version when needed. e.g.
      //   record R { type t = int; }
      //   var x: R; // should refer to R(int)
      bool computeDefaults = true;
      bool resolvingCalledIdent = (inLeafCall &&
                                   ident == inLeafCall->calledExpression());
      if (resolvingCalledIdent) {
        computeDefaults = false;
      }
      if (computeDefaults) {
        type = computeTypeDefaults(*this, type);
      }
    // Do not resolve function calls under 'scopeResolveOnly'
    } else if (type.kind() == QualifiedType::PARENLESS_FUNCTION) {
      if (!scopeResolveOnly) {
        // resolve a parenless call
        std::vector<CallInfoActual> actuals;
        auto ci = CallInfo (/* name */ ident->name(),
                            /* calledType */ QualifiedType(),
                            /* isMethodCall */ false,
                            /* hasQuestionArg */ false,
                            /* isParenless */ true,
                            actuals);
        auto inScope = scopeStack.back();
        auto c = resolveGeneratedCall(context, ident, ci, inScope, poiScope);
        // save the most specific candidates in the resolution result for the id
        ResolvedExpression& r = byPostorder.byAst(ident);
        handleResolvedCall(r, ident, ci, c);
      }
      return false;
    } else if (scopeResolveOnly &&
               type.kind() == QualifiedType::FUNCTION) {
      return false;
    }

    validateAndSetToId(result, ident, id);
    result.setType(type);
    // if there are multiple ids we should have gotten
    // a multiple definition error at the declarations.
  }
  return false;
}

bool Resolver::enter(const Identifier* ident) {
  if (initResolver && initResolver->handleResolvingFieldAccess(ident)) {
    std::ignore = initResolver->handleUseOfField(ident);
    return false;
  } else {
    auto ret = resolveIdentifier(ident, methodReceiverScopes());
    return ret;
  }
}

void Resolver::exit(const Identifier* ident) {
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
    if (inLeafCall && isCallToIntEtc(inLeafCall)) {
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

bool Resolver::enter(const NamedDecl* decl) {
  CHPL_ASSERT(scopeStack.size() > 0);
  const Scope* scope = scopeStack.back();

  // All functions can be overloaded, even parenless ones (via return
  // intent overloading).
  bool canOverload = decl->isFunction();

  if (canOverload == false) {
    // check for multiple definitions
    LookupConfig config = LOOKUP_DECLS;
    auto vec = lookupNameInScope(context, scope,
                                 /* receiverScopes */ {},
                                 decl->name(), config);

    if (vec.size() > 0) {
      const BorrowedIdsWithName& m = vec[0];
      if (m.firstId() == decl->id()) {
        // Checks if the given ID refers to a declaration conflicting
        // with this one. Functions don't conflict.
        auto isConflictingId = [&](auto decl) {
          auto ast = parsing::idToAst(context, decl);
          return !ast->isFunction();
        };

        std::vector<ID> redefinedIds;
        std::copy_if(m.begin(), m.end(),
                     std::back_inserter(redefinedIds), isConflictingId);
        if (redefinedIds.size() > 1) {
          // The first one is the ID we're looking at itself.
          CHPL_REPORT(context, Redefinition, decl, redefinedIds);
        }
      }
    }
  }

  // don't visit e.g. nested functions - these will be resolved
  // when calling them.
  bool visitChildren = !Builder::astTagIndicatesNewIdScope(decl->tag());

  enterScope(decl);

  return visitChildren;
}

void Resolver::exit(const NamedDecl* decl) {
  if (decl->id().postOrderId() < 0) {
    // It's a symbol with a different path, e.g. a Function.
    // Don't try to resolve it now in this
    // traversal. Instead, resolve it e.g. when the function is called.

  } else {
    resolveNamedDecl(decl, /* useType */ nullptr);
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

  // For the time being, we're resolving ranges by manually finding the record
  // and instantiating it appropriately. However, long-term, range literals
  // should be equivalent to a call to chpl_build_bounded_range. The resolver
  // cannot handle this right now, but in the future, the below implementation
  // should be replaced with one that resolves the call.

  const RecordType* rangeType = RecordType::getRangeType(context);
  auto rangeAst = parsing::idToAst(context, rangeType->id());
  if (!rangeAst) {
    // The range record is part of the standard library, but
    // it's possible to invoke the resolver without the stdlib.
    // In this case, mark ranges as UnknownType, but do not error.
    return;
  }

  ResolvedExpression& re = byPostorder.byAst(range);

  // fetch default fields for `stridable` and `idxType`
  const ResolvedFields& resolvedFields = fieldsForTypeDecl(context, rangeType,
      DefaultsPolicy::USE_DEFAULTS);
  CHPL_ASSERT(resolvedFields.fieldName(0) == "idxType");
  CHPL_ASSERT(resolvedFields.fieldName(1) == "boundedType");
  CHPL_ASSERT(resolvedFields.fieldName(2) == "stridable");

  // Determine index type, either via inference or by using the default.
  QualifiedType idxType;
  if (range->lowerBound() || range->upperBound()) {
    // We have bounds. Try to infer type from them
    std::vector<QualifiedType> suppliedTypes;
    if (auto lower = range->lowerBound()) {
      suppliedTypes.push_back(byPostorder.byAst(lower).type());
    }
    if (auto upper = range->upperBound()) {
      suppliedTypes.push_back(byPostorder.byAst(upper).type());
    }
    auto idxTypeResult = commonType(context, suppliedTypes);
    if (!idxTypeResult) {
      re.setType(CHPL_TYPE_ERROR(context, IncompatibleRangeBounds, range,
                                 suppliedTypes[0], suppliedTypes[1]));
      return;
    } else {
      idxType = idxTypeResult.getValue();
    }
  } else {
    // No bounds. Use default.
    idxType = resolvedFields.fieldType(0);
  }

  // Determine the value for boundedType.
  ID refersToId; // Needed for out parameter of typeForEnumElement
  const char* rangeTypeName;
  if (range->lowerBound() && range->upperBound()) {
    rangeTypeName = "bounded";
  } else if (range->lowerBound()) {
    rangeTypeName = "boundedLow";
  } else if (range->upperBound()) {
    rangeTypeName = "boundedHigh";
  } else {
    rangeTypeName = "boundedNone";
  }
  auto boundedRangeTypeType = EnumType::getBoundedRangeTypeType(context);
  auto boundedType = typeForEnumElement(boundedRangeTypeType,
                                        UniqueString::get(context, rangeTypeName),
                                        range,
                                        refersToId);

  auto subMap = SubstitutionsMap();
  subMap.insert({resolvedFields.fieldDeclId(0), std::move(idxType)});
  subMap.insert({resolvedFields.fieldDeclId(1), std::move(boundedType)});
  subMap.insert({resolvedFields.fieldDeclId(2), resolvedFields.fieldType(2)});

  const RecordType* rangeTypeInst =
      RecordType::get(context, rangeType->id(), rangeType->name(),
                      rangeType, std::move(subMap));
  re.setType(QualifiedType(QualifiedType::CONST_VAR, rangeTypeInst));
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
                             BoolType::get(context, 0),
                             BoolParam::get(context, !isAnd));
    } else {
      // otherwise just return a Bool value
      return QualifiedType(QualifiedType::CONST_VAR,
                             BoolType::get(context, 0));
    }
  }
}

bool Resolver::enter(const Call* call) {
  inLeafCall = call;
  auto op = call->toOpCall();

  if (op && initResolver) {
    initResolver->doDetectPossibleAssignmentToField(op);
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

QualifiedType Resolver::typeForTypeOperator(const OpCall* op,
    const QualifiedType& lt, const QualifiedType& rt) {
  if (op->op() == USTR("==") || op->op() == USTR("!=")) {
    bool opNotEqual = op->op() == USTR("!=");
    bool compareResult = lt == rt;
    return QualifiedType(QualifiedType::PARAM,
                         BoolType::get(context, 0),
                         BoolParam::get(context, opNotEqual ^ compareResult));
  }
  CHPL_ASSERT(false && "not implemented!");
  return QualifiedType();
}

void Resolver::exit(const Call* call) {
  if (scopeResolveOnly)
    return;

  if (initResolver && initResolver->handleResolvingCall(call))
    return;

  auto op = call->toOpCall();
  if (op && (op->op() == USTR("&&") || op->op() == USTR("||"))) {
    // these are handled in 'enter' to do param folding
    return;
  }

  if (op && (op->op() == USTR("==") || op->op() == USTR("!=") ||
        op->op() == USTR("<") || op->op() == USTR("<="))) {
    if (op->numActuals() != 2) {
      byPostorder.byAst(op).setType(typeErr(op, "invalid op call"));
      return;
    }
    ResolvedExpression& leftR = byPostorder.byAst(op->child(0));
    ResolvedExpression& rightR = byPostorder.byAst(op->child(1));
    if ((leftR.type().isType() && rightR.type().isType()) ||
        (leftR.type().isParam() && rightR.type().isParam())) {
      auto resultType = typeForTypeOperator(op, leftR.type(), rightR.type());
      byPostorder.byAst(op).setType(std::move(resultType));
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
  auto ci = CallInfo::create(context, call, byPostorder,
                             /* raiseErrors */ true,
                             &actualAsts);

  // With two exceptions (see below), don't try to resolve a call that accepts:
  //  * an unknown param
  //  * a type that is a generic type unless there are substitutions
  //  * a value of generic type
  //  * UnknownType, ErroneousType
  // EXCEPT, to handle split-init with an 'out' formal,
  // the actual argument can have unknown / generic type if it
  // refers directly to a particular variable.
  // EXCEPT, type construction can work with unknown or generic types

  bool skip = false;
  if (!ci.calledType().isType()) {
    int actualIdx = 0;
    for (auto actual : ci.actuals()) {
      ID toId; // does the actual refer directly to a particular variable?
      const AstNode* actualAst = actualAsts[actualIdx];
      if (actualAst != nullptr && byPostorder.hasAst(actualAst)) {
        toId = byPostorder.byAst(actualAst).toId();
      }
      QualifiedType qt = actual.type();
      const Type* t = qt.type();
      if (t != nullptr && t->isErroneousType()) {
        // always skip if there is an ErroneousType
        skip = true;
      } else if (!toId.isEmpty()) {
        // don't skip because it could be initialized with 'out' intent
      } else {
        if (qt.isParam() && qt.param() == nullptr) {
          skip = true;
        } else if (qt.isUnknown()) {
          skip = true;
        } else if (t != nullptr) {
          auto g = getTypeGenericity(context, t);
          bool isBuiltinGeneric = (g == Type::GENERIC &&
                                   (t->isAnyType() || t->isBuiltinType()));
          if (qt.isType() && isBuiltinGeneric && substitutions == nullptr) {
            skip = true;
          } else if (!qt.isType() && g != Type::CONCRETE) {
            skip = true;
          }
        }
      }
      if (skip) {
        break;
      }
      actualIdx++;
    }
  }
  // Don't try to resolve calls to '=' until later
  if (ci.isOpCall() && ci.name() == USTR("=")) {
    skip = true;
  }

  if (!skip) {
    CallResolutionResult c = resolveCall(context, call, ci, scope, poiScope);

    // save the most specific candidates in the resolution result for the id
    ResolvedExpression& r = byPostorder.byAst(call);
    handleResolvedCall(r, call, ci, c);

    // handle type inference for variables split-inited by 'out' formals
    adjustTypesForOutFormals(ci, actualAsts, c.mostSpecific());
  }

  inLeafCall = nullptr;
}

bool Resolver::enter(const Dot* dot) {
  if (initResolver && initResolver->handleResolvingFieldAccess(dot))
    return false;
  return true;
}

QualifiedType Resolver::typeForEnumElement(const EnumType* enumType,
                                           UniqueString elementName,
                                           const AstNode* nodeForErr,
                                           ID& outElemId) {
    LookupConfig config = LOOKUP_DECLS | LOOKUP_INNERMOST;
    auto enumScope = scopeForId(context, enumType->id());
    auto vec = lookupNameInScope(context, enumScope,
                                 /* receiverScopes */ {},
                                 elementName, config);
    if (vec.size() == 0) {
      return CHPL_TYPE_ERROR(context, UnknownEnumElem, nodeForErr,
                             elementName, enumType);
    } else if (vec.size() > 1 || vec[0].numIds() > 1) {
      auto& ids = vec[0];
      // multiple candidates. report a type error, but the
      // expression most likely has a type given by the enum.
      std::vector<ID> redefinedIds(ids.numIds());
      std::copy(ids.begin(), ids.end(), redefinedIds.begin());
      CHPL_REPORT(context, MultipleEnumElems, nodeForErr, elementName, enumType, std::move(redefinedIds));
      return QualifiedType(QualifiedType::CONST_VAR, enumType);
    } else {
      auto id = vec[0].firstId();
      auto newParam = EnumParam::get(context, id);
      outElemId = id;
      return QualifiedType(QualifiedType::PARAM, enumType, newParam);
    }
}

void Resolver::exit(const Dot* dot) {
  if (initResolver && initResolver->handleUseOfField(dot)) return;

  ResolvedExpression& receiver = byPostorder.byAst(dot->receiver());

  bool resolvingCalledDot = (inLeafCall &&
                             dot == inLeafCall->calledExpression());
  if (resolvingCalledDot) {
    // we will handle it when resolving the FnCall
    return;
  }

  if (dot->field() == USTR("type")) {
    const Type* receiverType;
    ResolvedExpression& r = byPostorder.byAst(dot);

    if (receiver.type().type() != nullptr) {
      receiverType = receiver.type().type();
    } else {
      receiverType = ErroneousType::get(context);
    }
    r.setType(QualifiedType(QualifiedType::TYPE, receiverType));
    return;
  }

  if (receiver.type().kind() == QualifiedType::MODULE &&
      !receiver.toId().isEmpty()) {
    // resolve e.g. M.x where M is a module
    LookupConfig config = LOOKUP_DECLS |
                          LOOKUP_IMPORT_AND_USE;

    auto modScope = scopeForModule(context, receiver.toId());
    auto vec = lookupNameInScope(context, modScope,
                                 /* receiverScopes */ {},
                                 dot->field(), config);
    ResolvedExpression& r = byPostorder.byAst(dot);
    if (vec.size() == 0) {
      r.setType(QualifiedType());
    } else if (vec.size() > 1 || vec[0].numIds() > 1) {
      // can't establish the type. If this is in a function
      // call, we'll establish it later anyway.
    } else {
      // vec.size() == 1 and vec[0].numIds() <= 1
      const ID& id = vec[0].firstId();
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
    return;
  }

  if (receiver.type().kind() == QualifiedType::TYPE &&
      receiver.type().type() != nullptr &&
      receiver.type().type()->isEnumType()) {
    // resolve E.x where E is an enum.
    const EnumType* enumType = receiver.type().type()->toEnumType();
    CHPL_ASSERT(enumType != nullptr);
    CHPL_ASSERT(receiver.toId().isEmpty() == false);

    ResolvedExpression& r = byPostorder.byAst(dot);
    ID elemId = r.toId(); // store the original in case we don't get a new one
    auto qt = typeForEnumElement(enumType, dot->field(), dot, elemId);
    validateAndSetToId(r, dot, elemId);
    r.setType(qt);
    maybeEmitWarningsForId(this, qt, dot, elemId);
    return;
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

  if (scopeResolveOnly)
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
  auto c = resolveGeneratedCall(context, dot, ci, inScope, poiScope);
  // save the most specific candidates in the resolution result for the id
  ResolvedExpression& r = byPostorder.byAst(dot);
  handleResolvedCall(r, dot, ci, c);
}

bool Resolver::enter(const New* node) {
  return true;
}

// TODO: How do we wire this up with 'getManagedClassType'? Is it possible?
// TODO: How to handle nilability, e.g. new owned C'?'
static const ClassType*
getDecoratedClassForNew(Context* context, const New* node,
                        const ClassType* classType) {
  auto basic = classType->basicClassType();
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

// TODO: Emit warning about 'new borrowed' being unstable.
// TODO: How do we handle '?'.
void Resolver::resolveNewForClass(const New* node,
                                  const ClassType* classType) {
  ResolvedExpression& re = byPostorder.byAst(node);

  // TODO: Verify initial class type?
  auto cls = getDecoratedClassForNew(context, node, classType);
  auto qt = QualifiedType(QualifiedType::VAR, cls);
  re.setType(qt);
}

void Resolver::resolveNewForRecord(const New* node,
                                   const RecordType* recordType) {
  ResolvedExpression& re = byPostorder.byAst(node);

  if (node->management() != New::DEFAULT_MANAGEMENT) {
    CHPL_REPORT(context, MemManagementNonClass, node, recordType);
  } else {
    auto qt = QualifiedType(QualifiedType::VAR, recordType);
    re.setType(qt);
  }
}

void Resolver::exit(const New* node) {
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
    resolveNewForClass(node, classType);

  } else if (auto recordType = qtTypeExpr.type()->toRecordType()) {
    resolveNewForRecord(node, recordType);

  } else {
    if (node->management() != New::DEFAULT_MANAGEMENT) {
      CHPL_REPORT(context, MemManagementNonClass, node, qtTypeExpr.type());
    }

    CHPL_REPORT(context, InvalidNewTarget, node, qtTypeExpr);
  }
}

static QualifiedType resolveSerialIterType(Resolver& resolver,
                                           const IndexableLoop* loop) {
  Context* context = resolver.context;
  const AstNode* iterand = loop->iterand();
  iterand->traverse(resolver);
  ResolvedExpression& iterandRE = resolver.byPostorder.byAst(iterand);

  if (resolver.scopeResolveOnly) {
    return QualifiedType(QualifiedType::UNKNOWN,
                         UnknownType::get(context));
  }

  auto& MSC = iterandRE.mostSpecific();
  bool isIter = MSC.isEmpty() == false &&
                MSC.numBest() == 1 &&
                MSC.only()->untyped()->kind() == uast::Function::Kind::ITER;

  bool wasResolved = iterandRE.type().isUnknown() == false &&
                     iterandRE.type().isErroneousType() == false;

  QualifiedType idxType;

  if (isIter) {
    idxType = iterandRE.type();
  } else if (wasResolved) {
    //
    // Resolve "iterand.these()"
    //
    std::vector<CallInfoActual> actuals;
    actuals.push_back(CallInfoActual(iterandRE.type(), USTR("this")));
    auto ci = CallInfo (/* name */ USTR("these"),
                        /* calledType */ iterandRE.type(),
                        /* isMethodCall */ true,
                        /* hasQuestionArg */ false,
                        /* isParenless */ false,
                        actuals);
    auto inScope = resolver.scopeStack.back();
    auto c = resolveGeneratedCall(context, iterand, ci, inScope, resolver.poiScope);

    if (c.mostSpecific().only() != nullptr) {
      idxType = c.exprType();
      resolver.handleResolvedAssociatedCall(iterandRE, loop, ci, c,
                                            AssociatedAction::ITERATE,
                                            iterand->id());
    } else {
      idxType = CHPL_TYPE_ERROR(context, NonIterable, loop, iterand, iterandRE.type());
    }
  } else {
    idxType = QualifiedType(QualifiedType::UNKNOWN,
                            ErroneousType::get(context));
  }

  return idxType;
}

bool Resolver::enter(const IndexableLoop* loop) {

  auto forLoop = loop->toFor();
  bool isParamLoop = forLoop != nullptr && forLoop->isParam();

  if (isParamLoop) {
    const AstNode* iterand = loop->iterand();
    iterand->traverse(*this);

    if (scopeResolveOnly) {
      enterScope(loop);
      return true;
    }

    if (iterand->isRange() == false) {
      context->error(loop, "param loops may only iterate over range literals");
    } else {
      // TODO: ranges with strides, '#', and '<'
      const Range* rng = iterand->toRange();
      ResolvedExpression& lowRE = byPostorder.byAst(rng->lowerBound());
      ResolvedExpression& hiRE = byPostorder.byAst(rng->upperBound());
      auto low = lowRE.type().param()->toIntParam();
      auto hi = hiRE.type().param()->toIntParam();

      if (low == nullptr || hi == nullptr) {
        context->error(loop, "param loops may only iterate over range literals with integer bounds");
      }

      std::vector<ResolutionResultByPostorderID> loopResults;
      for (int64_t i = low->value(); i <= hi->value(); i++) {
        ResolutionResultByPostorderID bodyResults;
        auto cur = Resolver::paramLoopResolver(*this, forLoop, bodyResults);

        cur.enterScope(loop);

        ResolvedExpression& idx = cur.byPostorder.byAst(loop->index());
        QualifiedType qt = QualifiedType(QualifiedType::PARAM, lowRE.type().type(), IntParam::get(context, i));
        idx.setType(qt);
        loop->body()->traverse(cur);

        cur.exitScope(loop);

        loopResults.push_back(std::move(cur.byPostorder));
      }

      auto paramLoop = new ResolvedParamLoop(forLoop);
      paramLoop->setLoopBodies(loopResults);
      auto& resolvedLoopExpr = byPostorder.byAst(loop);
      resolvedLoopExpr.setParamLoop(paramLoop);
    }

    return false;
  } else {
    QualifiedType idxType = resolveSerialIterType(*this, loop);

    enterScope(loop);

    if (const Decl* idx = loop->index()) {
      ResolvedExpression& re = byPostorder.byAst(idx);
      re.setType(idxType);
    }

    if (auto with = loop->withClause()) {
      with->traverse(*this);
    }
    loop->body()->traverse(*this);
  }

  return false;
}

void Resolver::exit(const IndexableLoop* loop) {
  // Param loops handle scope differently
  auto forLoop = loop->toFor();
  bool isParamLoop = forLoop != nullptr && forLoop->isParam();

  if (isParamLoop == false || scopeResolveOnly) {
    exitScope(loop);
  }
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

  auto receiverScopes = resolver.methodReceiverScopes();

  auto vec = lookupNameInScope(resolver.context, scope, receiverScopes,
                               intent->name(), config);

  if (vec.size() == 1) {
    resolvedId = vec[0].firstId();
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
  } else if (!scopeResolveOnly) {
    context->error(reduce, "Unable to find declaration of \"%s\" for reduction", reduce->name().c_str());
  }

  // TODO: Resolve reduce-op with shadowed type
  // E.g. "+ reduce x" --> "SumReduceOp(x.type)"
  reduce->op()->traverse(*this);

  return false;
}

void Resolver::exit(const ReduceIntent* reduce) {
}

bool Resolver::enter(const TaskVar* taskVar) {
  const bool isTaskIntent = taskVar->typeExpression() == nullptr &&
                            taskVar->initExpression() == nullptr;
  if (isTaskIntent) {
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
  const bool isTaskIntent = taskVar->typeExpression() == nullptr &&
                            taskVar->initExpression() == nullptr;
  if (isTaskIntent == false) {
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
  exitScope(node);
}

// Do not visit children. Un-ambiguous symbols will have warnings emitted
// for them in scope resolve.
bool Resolver::enter(const Use* node) {
  const Scope* scope = scopeStack.back();
  CHPL_ASSERT(scope);
  resolveUsesAndImportsInScope(context, scope);
  return false;
}

void Resolver::exit(const Use* node) {}

// Ditto the above.
bool Resolver::enter(const Import* node) {
  const Scope* scope = scopeStack.back();
  CHPL_ASSERT(scope);
  resolveUsesAndImportsInScope(context, scope);
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
