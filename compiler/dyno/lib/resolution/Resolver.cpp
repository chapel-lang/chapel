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

#include "Resolver.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/queries/global-strings.h"
#include "chpl/queries/query-impl.h"
#include "chpl/resolution/can-pass.h"
#include "chpl/resolution/disambiguation.h"
#include "chpl/resolution/intents.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"

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


static QualifiedType::Kind qualifiedTypeKindForDecl(const NamedDecl* decl) {
  if (decl->isFunction()) {
    return QualifiedType::FUNCTION;
  } else if (decl->isModule()) {
    return QualifiedType::MODULE;
  } else if (decl->isTypeDecl()) {
    return QualifiedType::TYPE;
  } else if (const VarLikeDecl* vd = decl->toVarLikeDecl()) {
    IntentList storageKind = vd->storageKind();
    return storageKind;
    assert(false && "case not handled");
  }
  assert(false && "case not handled");
  return QualifiedType::UNKNOWN;
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
  bool enter(const ASTNode* ast) { return false; }
  void exit(const ASTNode* ast) { }
};

Resolver
Resolver::moduleResolver(Context* context, const Module* mod,
                         ResolutionResultByPostorderID& byId) {
  auto ret = Resolver(context, mod, byId, nullptr);
  ret.byPostorder.setupForSymbol(mod);
  return ret;
}

Resolver
Resolver::initialSignatureResolver(Context* context, const Function* fn,
                                   ResolutionResultByPostorderID& byId)
{
  auto ret = Resolver(context, fn, byId, nullptr);
  ret.signatureOnly = true;
  ret.fnBody = fn->body();
  ret.byPostorder.setupForSignature(fn);
  return ret;
}

Resolver
Resolver::instantiatedSignatureResolver(Context* context,
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
Resolver::functionResolver(Context* context,
                           const Function* fn,
                           const PoiScope* poiScope,
                           const TypedFnSignature* typedFnSignature,
                           ResolutionResultByPostorderID& byId) {
  auto ret = Resolver(context, fn, byId, poiScope);
  ret.typedSignature = typedFnSignature;
  ret.signatureOnly = false;
  ret.fnBody = fn->body();

  assert(typedFnSignature);
  assert(typedFnSignature->untyped());

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

    if (auto formal = decl->toFormal())
      ret.resolveTypeQueriesFromFormalType(formal, qt);
  }

  return ret;
}

// set up Resolver to initially resolve field declaration types
Resolver
Resolver::initialFieldsResolver(Context* context,
                                const AggregateDecl* decl,
                                ResolutionResultByPostorderID& byId,
                                bool useGenericFormalDefaults) {
  auto ret = Resolver(context, decl, byId, nullptr);
  ret.useGenericFormalDefaults = useGenericFormalDefaults;
  ret.byPostorder.setupForSymbol(decl);
  return ret;
}

// set up Resolver to resolve instantiated field declaration types
Resolver
Resolver::instantiatedFieldsResolver(Context* context,
                                     const AggregateDecl* decl,
                                     const SubstitutionsMap& substitutions,
                                     const PoiScope* poiScope,
                                     ResolutionResultByPostorderID& byId,
                                     bool useGenericFormalDefaults) {
  auto ret = Resolver(context, decl, byId, poiScope);
  ret.substitutions = &substitutions;
  ret.useGenericFormalDefaults = useGenericFormalDefaults;
  ret.byPostorder.setupForSymbol(decl);
  return ret;
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
          assert(!untyped->formalDecl(i)->id().isEmpty());
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

// helper for resolveTypeQueriesFromFormalType
void Resolver::resolveTypeQueries(const Expression* formalTypeExpr,
                                  const Type* actualType) {

  // Give up if the type is nullptr or UnknownType or AnyType
  if (actualType == nullptr ||
      actualType->isUnknownType() ||
      actualType->isAnyType())
    return;

  assert(formalTypeExpr != nullptr);

  // Give up if typeExpr is an Identifier
  if (formalTypeExpr->isIdentifier())
    return;

  // Set the type that we know (since it was passed in)
  ResolvedExpression& result = byPostorder.byAst(formalTypeExpr);
  result.setType(QualifiedType(QualifiedType::TYPE, actualType));

  // Make recursive calls as needed to handle any TypeQuery nodes
  // nested within typeExpr.
  if (auto call = formalTypeExpr->toFnCall()) {
    // Error if it is not calling a type constructor
    auto actualCt = actualType->toCompositeType();

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
    auto callInfo = CallInfo(call);
    // generate a FormalActualMap
    auto faMap = FormalActualMap(sig, callInfo);

    // Now, consider the formals
    int nActuals = call->numActuals();
    for (int i = 0; i < nActuals; i++) {
      // ignore actuals like ?
      // since these aren't type queries & don't match a formal
      if (auto id = call->actual(i)->toIdentifier())
        if (id->name() == USTR("?"))
          continue;

      const FormalActual* fa = faMap.byActualIdx(i);
      assert(fa != nullptr && fa->formal() != nullptr);

      // get the substitution for that field from the CompositeType
      // and recurse with the result to set types for nested TypeQuery nodes
      const uast::Decl* field = fa->formal();
      const SubstitutionsMap& subs = actualCt->substitutions();
      auto search = subs.find(field->id());
      if (search != subs.end()) {
        QualifiedType fieldType = search->second;
        auto actual = call->actual(i);
        resolveTypeQueries(actual, fieldType.type());
      }
    }
  }
}

void Resolver::resolveTypeQueriesFromFormalType(const Formal* formal,
                                                QualifiedType formalType) {
  if (auto typeExpr = formal->typeExpression()) {
    resolveTypeQueries(typeExpr, formalType.type());
  }
}

QualifiedType Resolver::typeForId(const ID& id, bool localGenericToUnknown) {
  // if the id is contained within this symbol,
  // get the type information from the resolution result.
  if (id.symbolPath() == symbol->id().symbolPath() && id.postOrderId() >= 0) {
    QualifiedType ret = byPostorder.byId(id).type();
    auto g = getTypeGenericity(context, ret.type());
    if (g != Type::CONCRETE) {
      if (shouldUseUnknownTypeForGeneric(id)) {
        // if id refers to a field or formal that needs to be instantiated,
        // replace the type with UnknownType since we can't compute
        // the type of anything using this type (since it will change
        // on instantiation).
        auto unknownType = UnknownType::get(context);
        ret = QualifiedType(ret.kind(), unknownType, nullptr);
      }
    }

    return ret;
  }

  // TODO: handle outer function variables

  // otherwise, use a query to try to look it up top-level.
  return typeForModuleLevelSymbol(context, id);
}

void Resolver::enterScope(const ASTNode* ast) {
  if (createsScope(ast->tag())) {
    scopeStack.push_back(scopeForId(context, ast->id()));
  }
  if (auto d = ast->toDecl()) {
    declStack.push_back(d);
  }
}
void Resolver::exitScope(const ASTNode* ast) {
  if (createsScope(ast->tag())) {
    assert(!scopeStack.empty());
    scopeStack.pop_back();
  }
  if (ast->isDecl()) {
    assert(!declStack.empty());
    declStack.pop_back();
  }
}

bool Resolver::enter(const Literal* literal) {
  ResolvedExpression& result = byPostorder.byAst(literal);
  result.setType(typeForLiteral(context, literal));
  return false;
}
void Resolver::exit(const Literal* literal) {
}

bool Resolver::enter(const Identifier* ident) {
  assert(scopeStack.size() > 0);
  const Scope* scope = scopeStack.back();
  ResolvedExpression& result = byPostorder.byAst(ident);

  // for 'proc f(arg:?)' need to set 'arg' to have type AnyType
  assert(declStack.size() > 0);
  const Decl* inDecl = declStack.back();
  if (inDecl->isVarLikeDecl() && ident->name() == USTR("?")) {
    result.setType(QualifiedType(QualifiedType::TYPE, AnyType::get(context)));
    return false;
  }

  LookupConfig config = LOOKUP_DECLS |
                        LOOKUP_IMPORT_AND_USE |
                        LOOKUP_PARENTS |
                        LOOKUP_INNERMOST;

  auto vec = lookupInScope(context, scope, ident, config);
  if (vec.size() == 0) {
    result.setType(QualifiedType());
  } else if (vec.size() > 1 || vec[0].numIds() > 1) {
    // can't establish the type. If this is in a function
    // call, we'll establish it later anyway.
  } else {
    // vec.size() == 1 and vec[0].numIds() <= 1
    const ID& id = vec[0].id(0);
    QualifiedType type;
    if (id.isEmpty()) {
      // empty IDs from the scope resolution process are builtins
      type = typeForBuiltin(context, ident->name());
    } else {
      // use the type established at declaration/initialization,
      // but for things with generic type, use unknown.
      type = typeForId(id, /*localGenericToUnknown*/ true);
    }
    result.setToId(id);
    result.setType(type);
    // if there are multiple ids we should have gotten
    // a multiple definition error at the declarations.
  }
  return false;
}
void Resolver::exit(const Identifier* ident) {
}

bool Resolver::enter(const TypeQuery* tq) {
  // Consider 'proc f(arg:?t)'
  //   * if there is no substitution for 'arg', 't' should be AnyType
  //   * if there is a substitution for 'arg', 't' should be computed from it

  // Find the parent Formal and check for a substitution for that Formal
  const Formal* formal = nullptr;
  bool foundFormalSubstitution = false;
  QualifiedType foundFormalType;
  for (auto it = declStack.rbegin(); it != declStack.rend(); ++it) {
    const Decl* d = *it;
    if (auto fml = d->toFormal()) {
      formal = fml;
      break;
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
    result.setType(QualifiedType(QualifiedType::TYPE, AnyType::get(context)));
  } else {

    if (result.type().kind() != QualifiedType::UNKNOWN &&
        result.type().type() != nullptr) {
      // Looks like we already computed it, so do nothing else
    } else {
      // Found a substitution after instantiating, so gather the components
      // of the type. We do this in a way that handles all TypeQuery
      // nodes within the Formal uAST node.
      resolveTypeQueriesFromFormalType(formal, foundFormalType);
    }
  }

  return false;
}
void Resolver::exit(const TypeQuery* tq) {
}

bool Resolver::enter(const NamedDecl* decl) {
  assert(scopeStack.size() > 0);
  const Scope* scope = scopeStack.back();

  bool canOverload = false;
  if (const Function* fn = decl->toFunction()) {
    if (fn->isParenless() == false) {
      canOverload = true;
    }
  }

  if (canOverload == false) {
    // check for multiple definitions
    LookupConfig config = LOOKUP_DECLS;
    auto vec = lookupNameInScope(context, scope, decl->name(), config);

    if (vec.size() > 0) {
      const BorrowedIdsWithName& m = vec[0];
      if (m.id(0) == decl->id() && m.numIds() > 1) {
        Location loc = parsing::locateId(context, decl->id());
        auto error =
          ErrorMessage::build(decl->id(), loc, ErrorMessage::ERROR,
                              "'%s' has multiple definitions",
                              decl->name().c_str());
        for (const ID& id : m) {
          if (id != decl->id()) {
            Location curLoc = parsing::locateId(context, id);
            error.addDetail(ErrorMessage::build(id, curLoc,
                                                ErrorMessage::ERROR,
                                                "redefined here"));
          }
        }
        context->report(error);
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
    // It's a scoped symbol. Don't try to resolve it now in this
    // traversal. Instead, resolve it e.g. when the function is called.

  } else {
    // Figure out the Kind of the declaration
    auto qtKind = qualifiedTypeKindForDecl(decl);

    // Figure out the Type of the declaration
    // Nested Identifiers and Expressions should already be resolved
    const Type* typePtr = nullptr;

    // Figure out the param value, if any
    const Param* paramPtr = nullptr;

    bool isField = false;
    bool isFormal = false;
    bool isFieldOrFormal = false;

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

      isFormal = decl->isFormal();
      isFieldOrFormal = isField || isFormal;

      bool foundSubstitution = false;
      bool foundSubstitutionDefaultHint = false;

      if (isFieldOrFormal) {
        // use substitutions computed for fields and formals
        if (substitutions != nullptr) {
          auto search = substitutions->find(decl->id());
          if (search != substitutions->end()) {
            const QualifiedType& t = search->second;
            typePtr = t.type();
            paramPtr = t.param();
            if (typePtr == nullptr && t.kind() == QualifiedType::UNKNOWN)
              foundSubstitutionDefaultHint = true;
            else
              foundSubstitution = true;
          }
        }
      }

      if (typeExpr && !foundSubstitution) {
        // get the type we should have already computed postorder
        ResolvedExpression& r = byPostorder.byAst(typeExpr);
        // check that the resolution of that expression is a type
        auto kind = r.type().kind();
        if (kind == QualifiedType::TYPE) {
          typePtr = r.type().type();
        } else if (kind != QualifiedType::UNKNOWN) {
          typePtr = ErroneousType::get(context);
          context->error(typeExpr, "Value provided where type expected");
        }
        // otherwise, typePtr can remain nullptr.
      }

      if (initExpr && !foundSubstitution) {
        // compute the type based upon the init expression
        ResolvedExpression& r = byPostorder.byAst(initExpr);
        const QualifiedType& initType = r.type();

        // check that the init expression has compatible kind
        if (qtKind == QualifiedType::TYPE &&
            initType.kind() != QualifiedType::TYPE) {
          context->error(initExpr, "Cannot initialize type with value");
        } else if (qtKind != QualifiedType::TYPE &&
                   initType.kind() == QualifiedType::TYPE) {
          context->error(initExpr, "Cannot initialize value with type");
        } else if (qtKind == QualifiedType::PARAM &&
                   initType.kind() != QualifiedType::PARAM) {
          context->error(initExpr, "Cannot initialize param with non-param");
        }
        // check that the initExpr type is compatible with declared type
        if (typePtr != nullptr && initType.type() != typePtr) {
          context->error(typeExpr, "Cannot initialize this type with that");
          // TODO: better error
          // TODO: implicit conversions and instantiations
        }

        bool isGenericField = isField &&
                              (qtKind == QualifiedType::TYPE ||
                               qtKind == QualifiedType::PARAM);

        // infer the type of the variable from its initialization expr?
        bool inferFromInit = !isGenericField ||
                              useGenericFormalDefaults ||
                              foundSubstitutionDefaultHint;

        if (inferFromInit) {
          // Infer the param value from the initialization expr
          if (qtKind == QualifiedType::PARAM &&
              initType.kind() == QualifiedType::PARAM) {
            paramPtr = initType.param();
          }
          // Infer the type from the initialization expr
          typePtr = initType.type();
        } else if (isGenericField) {
          // a type or param field with initExpr is still generic, e.g.
          // record R { type t = int; }
          // if that behavior is requested with useGenericFormalDefaults
          typePtr = AnyType::get(context);
        }
      }

      if (isFieldOrFormal && !foundSubstitution) {
        // Lack of initializer for a formal means the Any type
        if (typeExpr == nullptr && initExpr == nullptr) {
          typePtr = AnyType::get(context);
        }
      }

      // TODO: handle split init
      // TODO: handle generic & instantiated formal arguments

      if (typePtr == nullptr) {
        context->error(var, "Cannot establish type for %s",
                             var->name().c_str());
        typePtr = ErroneousType::get(context);
      }
    }

    if (typePtr == nullptr) {
      if (qtKind == QualifiedType::FUNCTION ||
          qtKind == QualifiedType::MODULE) {
        // OK, type can be null for now
      } else {
        // type should have been established above
        context->error(decl, "Cannot establish type");
        typePtr = ErroneousType::get(context);
      }
    }

    // forget the param value if the QualifiedType is not param
    if (qtKind != QualifiedType::PARAM)
      paramPtr = nullptr;

    auto declaredKind = qtKind;

    // compute the intent for formals (including type constructor formals)
    if (isFormal || (signatureOnly && isField)) {
      bool isThis = decl->name() == USTR("this");
      auto formalQt = QualifiedType(qtKind, typePtr, paramPtr);
      // update qtKind with the result of resolving the intent
      qtKind = resolveIntent(formalQt, isThis);
    }

    // adjust tuple declarations for value / referential tuples
    if (typePtr != nullptr) {
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

  exitScope(decl);
}

bool Resolver::enter(const Call* call) {
  return true;
}
void Resolver::exit(const Call* call) {
  assert(scopeStack.size() > 0);
  const Scope* scope = scopeStack.back();

  // TODO should we move this to a class method that takes in the
  // context and call?
  // Generate a CallInfo for the call
  UniqueString name;

  if (auto op = call->toOpCall()) {
    name = op->op();
  } else if (auto called = call->calledExpression()) {
    if (auto calledIdent = called->toIdentifier()) {
      name = calledIdent->name();
    } else {
      assert(false && "TODO: method calls with Dot called");
    }
  }

  const FnCall* fnCall = call->toFnCall();
  bool hasQuestionArg = false;
  std::vector<CallInfoActual> actuals;

  int i = 0;
  for (auto actual : call->actuals()) {
    bool isQuestionMark = false;
    if (auto id = actual->toIdentifier())
      if (id->name() == USTR("?"))
        isQuestionMark = true;

    if (isQuestionMark) {
      if (hasQuestionArg) {
        context->error(actual, "Cannot have ? more than once in a call");
      }
      hasQuestionArg = true;
    } else {
      ResolvedExpression& r = byPostorder.byAst(actual);
      UniqueString byName;
      if (fnCall && fnCall->isNamedActual(i)) {
        byName = fnCall->actualName(i);
      }
      actuals.push_back(CallInfoActual(r.type(), byName));
      i++;
    }
  }

  QualifiedType calledType;
  if (auto calledExpr = call->calledExpression()) {
    ResolvedExpression& r = byPostorder.byAst(calledExpr);
    calledType = r.type();
  }

  CallInfo ci(name, calledType, hasQuestionArg, actuals);
  CallResolutionResult c = resolveCall(context, call, ci, scope, poiScope);

  // save the most specific candidates in the resolution result for the id
  ResolvedExpression& r = byPostorder.byAst(call);
  r.setMostSpecific(c.mostSpecific());
  r.setPoiScope(c.poiInfo().poiScope());
  r.setType(c.exprType());

  if (r.type().type() != nullptr) {
    // assume it is OK even if mostSpecific is empty
    // (e.g. it could be a primitive or other builtin operation)
  } else {

    if (c.mostSpecific().isEmpty()) {
      // if the call resolution result is empty, we need to issue an error
      if (c.mostSpecific().isAmbiguous()) {
        // ambiguity between candidates
        context->error(call, "Cannot resolve call: ambiguity");
      } else {
        // could not find a most specific candidate
        context->error(call, "Cannot resolve call: no matching candidates");
      }
    } else {
      context->error(call, "Cannot establish type for call expression");
    }
    r.setType(QualifiedType(r.type().kind(), ErroneousType::get(context)));
  }

  // gather the poi scopes used when resolving the call
  poiInfo.accumulate(c.poiInfo());
}

bool Resolver::enter(const Dot* dot) {
  return true;
}
void Resolver::exit(const Dot* dot) {
  if (dot->field() == USTR("type")) {
    ResolvedExpression& receiver = byPostorder.byAst(dot->receiver());
    const Type* receiverType;
    ResolvedExpression& r = byPostorder.byAst(dot);

    if (receiver.type().type() != nullptr) {
      receiverType = receiver.type().type();
    } else {
      receiverType = ErroneousType::get(context);
    }
    r.setType(QualifiedType(QualifiedType::TYPE, receiverType));
  }

  // TODO: resolve field accessors / parenless methods
}

bool Resolver::enter(const ASTNode* ast) {
  enterScope(ast);

  bool skipChildren = signatureOnly && ast == fnBody;
  return !skipChildren;
}
void Resolver::exit(const ASTNode* ast) {
  exitScope(ast);
}


} // end namespace resolution
} // end namespace chpl
