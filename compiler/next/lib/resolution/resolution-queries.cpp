/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#include "chpl/resolution/resolution-queries.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/queries/ErrorMessage.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/queries/query-impl.h"
#include "chpl/resolution/can-pass.h"
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

struct Resolver;


static QualifiedType::Kind resolveIntent(const QualifiedType& t);


const QualifiedType& typeForBuiltin(Context* context,
                                    UniqueString name) {
  QUERY_BEGIN(typeForBuiltin, context, name);

  QualifiedType result;

  std::unordered_map<UniqueString,const Type*> map;
  Type::gatherBuiltins(context, map);

  auto search = map.find(name);
  if (search != map.end()) {
    result = QualifiedType(QualifiedType::TYPE, search->second);
  } else {
    assert(false && "Should not be reachable");
  }

  return QUERY_END(result);
}

static QualifiedType typeForLiteral(Context* context, const Literal* literal) {
  const Type* typePtr = nullptr;
  const Param* paramPtr = nullptr;

  switch (literal->tag()) {
    case asttags::BoolLiteral:
      typePtr = BoolType::get(context, 0);
      break;
    case asttags::ImagLiteral:
      typePtr = ImagType::get(context, 0);
      break;
    case asttags::IntLiteral:
      typePtr = IntType::get(context, 0);
      break;
    case asttags::RealLiteral:
      typePtr = RealType::get(context, 0);
      break;
    case asttags::UintLiteral:
      typePtr = UintType::get(context, 0);
      break;
    case asttags::BytesLiteral:
      typePtr = BytesType::get(context);
      break;
    case asttags::CStringLiteral:
      typePtr = CStringType::get(context);
      break;
    case asttags::StringLiteral:
      typePtr = StringType::get(context);
      break;
    default:
      assert(false && "case not handled");
  }
  paramPtr = literal->param();

  return QualifiedType(QualifiedType::PARAM, typePtr, paramPtr);
}

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

struct Resolver {
  // inputs to the resolution process
  Context* context = nullptr;
  const ASTNode* symbol = nullptr;
  const PoiScope* poiScope = nullptr;
  const SubstitutionsMap* substitutions = nullptr;
  bool useGenericFormalDefaults = false;

  // internal variables
  std::vector<const Scope*> scopeStack;
  bool signatureOnly = false;
  const Block* fnBody = nullptr;

  // results of the resolution process

  // the resolution results for the contained Expressions
  ResolutionResultByPostorderID& byPostorder;
  // the set of POI scopes from which POI functions were used --
  // these are gathered here during resolution in order to
  // allow accurate caching and reuse of instantiations
  PoiInfo poiInfo;

  // set up Resolver to resolve a Module
  Resolver(Context* context,
           const Module* mod,
           ResolutionResultByPostorderID& byPostorder)
    : context(context),
      symbol(mod),
      byPostorder(byPostorder) {

    byPostorder.setupForSymbol(mod);
    enterScope(symbol);
  }

  // set up Resolver to resolve a potentially generic Function signature
  Resolver(Context* context,
           const Function* fn,
           ResolutionResultByPostorderID& byPostorder)
    : context(context),
      symbol(fn),
      signatureOnly(true),
      fnBody(fn->body()),
      byPostorder(byPostorder) {

    byPostorder.setupForSignature(fn);
    enterScope(symbol);
  }

  // set up Resolver to resolve an instantiation of a Function signature
  Resolver(Context* context,
           const Function* fn,
           const SubstitutionsMap& substitutions,
           const PoiScope* poiScope,
           ResolutionResultByPostorderID& byPostorder)
    : context(context),
      symbol(fn),
      poiScope(poiScope),
      substitutions(&substitutions),
      signatureOnly(true),
      fnBody(fn->body()),
      byPostorder(byPostorder),
      poiInfo(poiScope) {

    byPostorder.setupForSignature(fn);
    enterScope(symbol);
  }

  // set up Resolver to resolve a Function body
  Resolver(Context* context,
           const Function* fn,
           const PoiScope* poiScope,
           const TypedFnSignature* typedFnSignature,
           ResolutionResultByPostorderID& byPostorder)
    : context(context),
      symbol(fn),
      poiScope(poiScope),
      signatureOnly(false),
      fnBody(fn->body()),
      byPostorder(byPostorder),
      poiInfo(poiScope) {

    assert(typedFnSignature);
    assert(typedFnSignature->untyped());

    byPostorder.setupForFunction(fn);
    enterScope(symbol);

    // set the resolution results for the formals according to
    // the typedFnSignature
    const UntypedFnSignature* uSig = typedFnSignature->untyped();
    int nFormals = typedFnSignature->numFormals();
    for (int i = 0; i < nFormals; i++) {
      const Decl* decl = uSig->formalDecl(i);
      const auto& qt = typedFnSignature->formalType(i);

      ResolvedExpression& r = byPostorder.byAst(decl);
      r.setType(qt);
    }
  }

  // set up Resolver to initially resolve field declaration types
  Resolver(Context* context,
           const AggregateDecl* decl,
           ResolutionResultByPostorderID& byPostorder,
           bool useGenericFormalDefaults)
    : context(context),
      symbol(decl),
      useGenericFormalDefaults(useGenericFormalDefaults),
      byPostorder(byPostorder) {

    byPostorder.setupForSymbol(decl);
    enterScope(symbol);
  }

  // set up Resolver to resolve instantiated field declaration types
  Resolver(Context* context,
           const AggregateDecl* decl,
           const SubstitutionsMap& substitutions,
           const PoiScope* poiScope,
           ResolutionResultByPostorderID& byPostorder,
           bool useGenericFormalDefaults)
    : context(context),
      symbol(decl),
      poiScope(poiScope),
      substitutions(&substitutions),
      useGenericFormalDefaults(useGenericFormalDefaults),
      byPostorder(byPostorder),
      poiInfo(poiScope) {

    byPostorder.setupForSymbol(decl);
    enterScope(symbol);
  }


  /* When resolving a generic record or a generic function,
     there might be generic types that we don't know yet.
     E.g.

       proc f(type t, arg: t)

     before instantiating, we should conclude that:
       * t has type AnyType
       * arg has type UnknownType (and in particular, not AnyType)
   */
  bool shouldUseUnknownTypeForGeneric(const ID& id) {
    bool isFieldOrFormal = false;
    bool isSubstituted = false;
    // TODO: should we compute this some other way
    // (e.g. when setting up the traversal)
    // given that it's within the Symbol we are visiting?
    auto ast = parsing::idToAst(context, id);
    if (ast) {
      if (auto decl = ast->toDecl()) {
        bool isField = false;
        if (auto var = decl->toVariable())
          if (var->isField())
            isField = true;

        isFieldOrFormal = isField || decl->isFormal();

        if (substitutions != nullptr) {
          auto search = substitutions->find(decl);
          if (search != substitutions->end()) {
            isSubstituted = true;
          }
        }
      }
    }

    return isFieldOrFormal && !isSubstituted;
  }

  QualifiedType typeForId(const ID& id, bool localGenericToUnknown) {
    // if the id is contained within this symbol,
    // get the type information from the resolution result.
    if (id.symbolPath() == symbol->id().symbolPath()) {
      QualifiedType ret = byPostorder.byId(id).type();
      if (ret.isGenericType()) {
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

  void enterScope(const ASTNode* ast) {
    if (createsScope(ast->tag())) {
      scopeStack.push_back(scopeForId(context, ast->id()));
    }
  }
  void exitScope(const ASTNode* ast) {
    if (createsScope(ast->tag())) {
      scopeStack.pop_back();
    }
  }

  bool enter(const Literal* literal) {
    ResolvedExpression& result = byPostorder.byAst(literal);
    result.setType(typeForLiteral(context, literal));
    return false;
  }
  void exit(const Literal* literal) {
  }

  bool enter(const Identifier* ident) {
    assert(scopeStack.size() > 0);
    const Scope* scope = scopeStack.back();
    ResolvedExpression& result = byPostorder.byAst(ident);

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
  void exit(const Identifier* ident) {
  }

  bool enter(const NamedDecl* decl) {
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
            ErrorMessage::build(loc, "'%s' has multiple definitions",
                                decl->name().c_str());
          for (const ID& id : m) {
            if (id != decl->id()) {
              Location curLoc = parsing::locateId(context, id);
              error.addDetail(ErrorMessage::build(curLoc, "redefined here"));
            }
          }
          context->error(error);
        }
      }
    }

    // don't visit e.g. nested functions - these will be resolved
    // when calling them.
    bool visitChildren = !Builder::astTagIndicatesNewIdScope(decl->tag());

    enterScope(decl);

    return visitChildren;
  }

  void exit(const NamedDecl* decl) {
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

      if (auto var = decl->toVarLikeDecl()) {
        // Figure out variable type based upon:
        //  * the type in the variable declaration
        //  * the initialization expression in the variable declaration
        //  * the initialization expression from split-init

        auto typeExpr = var->typeExpression();
        auto initExpr = var->initExpression();

        bool isField = false;
        if (auto var = decl->toVariable())
          if (var->isField())
            isField = true;

        bool isFieldOrFormal = isField || decl->isFormal();

        bool foundSubstitution = false;
        bool foundSubstitutionDefaultHint = false;

        if (isFieldOrFormal) {
          // use substitutions computed for fields and formals
          if (substitutions != nullptr) {
            auto search = substitutions->find(decl);
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

      ResolvedExpression& result = byPostorder.byAst(decl);
      result.setType(QualifiedType(qtKind, typePtr, paramPtr));
    }

    exitScope(decl);
  }

  bool enter(const Call* call) {
    return true;
  }
  void exit(const Call* call) {
    assert(scopeStack.size() > 0);
    const Scope* scope = scopeStack.back();

    // TODO should we move this to a class method that takes in the context and call?
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
        if (id->name() == "?")
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

    CallInfo ci(name, hasQuestionArg, actuals);
    CallResolutionResult c = resolveCall(context, call, ci, scope, poiScope);

    // save the most specific candidates in the resolution result for the id
    ResolvedExpression& r = byPostorder.byAst(call);
    r.setMostSpecific(c.mostSpecific());
    r.setPoiScope(c.poiInfo().poiScope());
    r.setType(c.exprType());

    if (r.type().type() == nullptr) {
      context->error(call, "Cannot establish type for call expression");
      r.setType(QualifiedType(r.type().kind(), ErroneousType::get(context)));
    }

    // gather the poi scopes used when resolving the call
    poiInfo.accumulate(c.poiInfo());
  }

  bool enter(const ASTNode* ast) {
    enterScope(ast);

    bool skipChildren = signatureOnly && ast == fnBody;
    return !skipChildren;
  }
  void exit(const ASTNode* ast) {
    exitScope(ast);
  }
};

// Given an AggregateDecl, do initial resolution of the fields
// Note that useGenericFormalDefaults is moot for most types
// and so code calling this query can optimize by calling with 'false'
// first and then adjusting if it is not moot.
static const ResolutionResultByPostorderID&
resolveFieldsInitial(Context* context, ID id, bool useGenericFormalDefaults) {
  QUERY_BEGIN(resolveFieldsInitial, context, id, useGenericFormalDefaults);

  ResolutionResultByPostorderID& partialResult = QUERY_CURRENT_RESULT;

  auto ast = parsing::idToAst(context, id);
  if (const AggregateDecl* ad = ast->toAggregateDecl()) {
    Resolver visitor(context, ad, partialResult, useGenericFormalDefaults);
    // visit the field declarations
    // TODO: handle MultiDecl and TupleDecl
    for (auto child: ad->children()) {
      if (auto var = child->toVariable()) {
        if (var->isField()) {
          child->traverse(visitor);
        }
      }
    }
  } else {
    assert(false && "case not handled");
  }

  return QUERY_END_CURRENT_RESULT();
}


const ResolutionResultByPostorderID& resolveModule(Context* context, ID id) {
  QUERY_BEGIN(resolveModule, context, id);

  ResolutionResultByPostorderID& partialResult = QUERY_CURRENT_RESULT;

  auto ast = parsing::idToAst(context, id);
  if (const Module* mod = ast->toModule()) {
    Resolver visitor(context, mod, partialResult);
    for (auto child: mod->children()) {
      child->traverse(visitor);
    }
  } else {
    assert(false && "case not handled");
  }

  return QUERY_END_CURRENT_RESULT();
}

static
const ResolutionResultByPostorderID& partiallyResolvedModule(Context* context,
                                                             ID id) {

  // check for a partial result from a running query
  const ResolutionResultByPostorderID* r =
    QUERY_RUNNING_PARTIAL_RESULT(resolveModule, context, id);
  // if there was a partial result, return it
  if (r != nullptr) {
    return *r;
  }

  // otherwise, run the query to compute the full result
  return resolveModule(context, id);
}

const QualifiedType& typeForModuleLevelSymbol(Context* context, ID id) {
  QUERY_BEGIN(typeForModuleLevelSymbol, context, id);

  QualifiedType result;

  int postOrderId = id.postOrderId();
  if (postOrderId >= 0) {
    // Find the parent scope for the ID - i.e. where the id is declared
    ID parentSymbolId = id.parentSymbolId(context);
    ASTTag parentTag = parsing::idToTag(context, parentSymbolId);
    if (asttags::isModule(parentTag)) {
      auto& partial = partiallyResolvedModule(context, parentSymbolId);
      result = partial.byId(id).type();
    }
  } else {
    QualifiedType::Kind kind = QualifiedType::UNKNOWN;
    const Type* t = nullptr;

    auto ast = parsing::idToAst(context, id);
    if (auto* decl = ast->toNamedDecl()) {
      kind = qualifiedTypeKindForDecl(decl);
      if (auto td = decl->toTypeDecl()) {
        t = typeForTypeDecl(context, td, /* useGenericFormalDefaults */ true);
        assert(kind == QualifiedType::TYPE);
      } else if (decl->isModule() || decl->isFunction()) {
        // OK, don't try to establish the types of these right now
      } else {
        assert(false && "case not handled");
      }
    } else {
      assert(false && "case not handled");
    }

    result = QualifiedType(kind, t);
  }

  return QUERY_END(result);
}


/////// function resolution

static const owned<TypedFnSignature>&
typedSignatureQuery(Context* context,
                    const UntypedFnSignature* untypedSignature,
                    std::vector<types::QualifiedType> formalTypes,
                    TypedFnSignature::WhereClauseResult whereClauseResult,
                    bool needsInstantiation,
                    const TypedFnSignature* instantiatedFrom,
                    const TypedFnSignature* parentFn) {
  QUERY_BEGIN(typedSignatureQuery, context,
              untypedSignature, formalTypes, whereClauseResult,
              needsInstantiation, instantiatedFrom, parentFn);

  auto result = toOwned(new TypedFnSignature(untypedSignature,
                                             std::move(formalTypes),
                                             whereClauseResult,
                                             needsInstantiation,
                                             instantiatedFrom,
                                             parentFn));

  return QUERY_END(result);
}

static std::vector<types::QualifiedType>
getFormalTypes(const Function* fn,
               const ResolutionResultByPostorderID& r) {
  std::vector<types::QualifiedType> formalTypes;
  for (auto formal : fn->formals()) {
    QualifiedType t = r.byAst(formal).type();
    // compute concrete intent
    t = QualifiedType(resolveIntent(t), t.type(), t.param());

    formalTypes.push_back(std::move(t));
  }
  return formalTypes;
}

static bool
anyFormalNeedsInstantiation(const std::vector<types::QualifiedType>& formalTs) {
  bool genericOrUnknown = false;
  for (const auto& qt : formalTs) {
    if (qt.isGenericOrUnknown()) {
      genericOrUnknown = true;
      break;
    }
  }
  return genericOrUnknown;
}

static TypedFnSignature::WhereClauseResult whereClauseResult(
                                     Context* context,
                                     const Function* fn,
                                     const ResolutionResultByPostorderID& r,
                                     bool needsInstantiation) {
  auto whereClauseResult = TypedFnSignature::WHERE_TBD;
  if (const Expression* where = fn->whereClause()) {
    const QualifiedType& qt = r.byAst(where).type();
    if (qt.isParam() && qt.type()->isBoolType()) {
      // OK, we know the result of the where clause
      // TODO: handle Immediate
      if (qt.param() != 0) {
        whereClauseResult = TypedFnSignature::WHERE_TRUE;
      } else {
        whereClauseResult = TypedFnSignature::WHERE_FALSE;
      }
    } else if (needsInstantiation) {
      // it's OK, need to establish the value of the where clause later
      whereClauseResult = TypedFnSignature::WHERE_TBD;
    } else {
      // where clause should be resolved by now
      context->error(where,
                     "where clause does not result in a param bool value");
    }
  } else {
    whereClauseResult = TypedFnSignature::WHERE_NONE;
  }

  return whereClauseResult;
}

// Finds a parent function from a function ID
// Returns that parent function, or an empty ID if there was none.
static ID parentFunctionId(Context* context, ID functionId) {
  ID parentSymId = functionId.parentSymbolId(context);
  const Scope* parentScope = scopeForId(context, parentSymId);
  for (const Scope* s = parentScope; s != nullptr; s = s->parentScope()) {
    if (s->tag() == asttags::Function) {
      return s->id();
    }
  }

  return ID();
}

const TypedFnSignature*
typedSignatureInitial(Context* context,
                      const UntypedFnSignature* untypedSig) {

  const ASTNode* ast = parsing::idToAst(context, untypedSig->id());
  const Function* fn = ast->toFunction();

  if (fn == nullptr) {
    return nullptr;
  }

  // look at the parent scopes to find the parent function, if any
  const UntypedFnSignature* parentFnUntyped = nullptr;
  const TypedFnSignature* parentFnTyped = nullptr;
  ID parentFnId = parentFunctionId(context, fn->id());
  if (!parentFnId.isEmpty()) {
    auto parentAst = parsing::idToAst(context, parentFnId);
    auto parentFn = parentAst->toFunction();
    parentFnUntyped = UntypedFnSignature::get(context, parentFn);
    parentFnTyped = typedSignatureInitial(context, parentFnUntyped);
  }

  ResolutionResultByPostorderID r;
  Resolver visitor(context, fn, r);
  // visit the formals, return type, where clause
  for (auto child: fn->children()) {
    child->traverse(visitor);
  }

  // now, construct a TypedFnSignature from the result
  std::vector<types::QualifiedType> formalTypes = getFormalTypes(fn, r);
  bool needsInstantiation = anyFormalNeedsInstantiation(formalTypes);
  auto whereResult = whereClauseResult(context, fn, r, needsInstantiation);
  // use an empty poiFnIdsUsed since this is never an instantiation
  std::set<std::pair<ID, ID>> poiFnIdsUsed;

  const auto& result = typedSignatureQuery(context,
                                           untypedSig,
                                           std::move(formalTypes),
                                           whereResult,
                                           needsInstantiation,
                                           /* instantiatedFrom */ nullptr,
                                           /* parentFn */ parentFnTyped);
  return result.get();
}


static const Type* const&
typeForTypeDeclQuery(Context* context,
                     ID declId,
                     bool useGenericFormalDefaults) {
  QUERY_BEGIN(typeForTypeDeclQuery, context, declId, useGenericFormalDefaults);

  const Type* result = nullptr;

  auto ast = parsing::idToAst(context, declId);
  if (auto td = ast->toTypeDecl()) {
    if (td->isEnum())
      assert(false && "case not handled");
    if (auto ad = td->toAggregateDecl()) {
      // do 1st pass resolution of the field types
      const ResolutionResultByPostorderID& r =
        resolveFieldsInitial(context, declId, useGenericFormalDefaults);

      // now pull out the field types
      // TODO: handle MultiDecl and TupleDecl
      std::vector<CompositeType::FieldDetail> fields;
      for (auto child: ad->children()) {
        if (auto var = child->toVariable()) {
          if (var->isField()) {
            const ResolvedExpression& e = r.byAst(child);
            bool hasDefault = var->initExpression() != nullptr;
            fields.push_back(CompositeType::FieldDetail(var->name(),
                                                        hasDefault,
                                                        var,
                                                        e.type()));
          }
        }
      }

      if (auto rec = ad->toRecord()) {
        result = RecordType::get(context, rec->id(), rec->name(),
                                 std::move(fields));
      }
      if (auto cls = ad->toClass()) {
        auto t = BasicClassType::get(context, cls->id(), cls->name(),
                                     std::move(fields));
        auto d = ClassTypeDecorator(ClassTypeDecorator::GENERIC_NONNIL);
        result = ClassType::get(context, t, nullptr, d);
      }
      if (auto uni = ad->toUnion()) {
        result = UnionType::get(context, uni->id(), uni->name(),
                                std::move(fields));
      }
    }
  }

  return QUERY_END(result);
}

const Type* typeForTypeDecl(Context* context,
                            const TypeDecl* d,
                            bool useGenericFormalDefaults) {
  assert(d);
  const Type* t = typeForTypeDeclQuery(context, d->id(),
                                       /* useGenericFormalDefaults */ false);

  // If useGenericFormalDefaults was requested and the type
  // is generic with defaults, compute the type again.
  // We do it this way so that we are more likely to be able to reuse the
  // result of the above query in most cases since most types
  // are not generic record/class with defaults.
  if (useGenericFormalDefaults) {
    if (auto ct = t->getCompositeType()) {
      if (ct->isGenericWithDefaults()) {
        t = typeForTypeDeclQuery(context, d->id(),
                                 /* useGenericFormalDefaults */ true);
      }
    }
  }

  return t;
}


// Returns true if the field should be included in the type constructor.
// In that event, also sets formalType to the type the formal should use.
static
bool shouldIncludeFieldInTypeConstructor(Context* context,
                                         const Decl* fieldDecl,
                                         const QualifiedType& fieldType,
                                         QualifiedType& formalType) {
  // compare with AggregateType::fieldIsGeneric

  // fields with concrete types don't need to be in type constructor
  if (!fieldType.isGenericOrUnknown()) {
    return false;
  }

  // fields that are 'type' or 'param' are generic
  // and we can use the same type/param intent for the type constructor
  if ((fieldType.isParam() && !fieldType.hasParamPtr()) ||
      fieldType.isType()) {
    formalType = fieldType;
    return true;
  }

  if (const VarLikeDecl* var = fieldDecl->toVarLikeDecl()) {
    // non-type/param fields with an init expression aren't generic
    if (var->initExpression())
      return false;

    // non-type/param fields that have no declared type and no initializer
    // are generic and these need a type variable for the argument with AnyType.
    if (var->typeExpression() == nullptr) {
      formalType = QualifiedType(QualifiedType::TYPE, AnyType::get(context));
      return true;
    }

    // otherwise, the field may or may not be generic.
    // it is generic if the field type is generic.
    // for this check we make some simplifying assumptions:
    //  * generic-with-defaults means concrete, unless ? is used in the type
    //  * unknown type means it depends on a previous generic field
    //    (and when previous generic fields are set, they will be concrete)
    const Type* t = fieldType.type();
    if (t && !t->isUnknownType()) {
      bool fieldIsGeneric = t->isGeneric();
      if (const CompositeType* ct = t->getCompositeType()) {
        if (ct->isGenericWithDefaults())
          fieldIsGeneric = false;
      }
      if (fieldIsGeneric) {
        formalType = QualifiedType(QualifiedType::TYPE, t);
        return true;
      }
    }
  }

  // otherwise it does not need to go into the type constructor
  return false;
}

static const owned<TypedFnSignature>&
typeConstructorInitialQuery(Context* context, const Type* t)
{
  QUERY_BEGIN(typeConstructorInitialQuery, context, t);

  owned<TypedFnSignature> result;

  ID id;
  UniqueString name;
  std::vector<UntypedFnSignature::FormalDetail> formals;
  std::vector<types::QualifiedType> formalTypes;

  if (auto ct = t->getCompositeType()) {
    id = ct->id();
    name = ct->name();

    // find the generic fields from the type and add
    // these as type constructor arguments.
    int nFields = ct->numFields();
    for (int i = 0; i < nFields; i++) {
      const Decl* fieldDecl = ct->fieldDecl(i);
      QualifiedType fieldType = ct->fieldType(i);
      QualifiedType formalType;
      if (shouldIncludeFieldInTypeConstructor(context, fieldDecl, fieldType,
                                              formalType)) {

        auto d = UntypedFnSignature::FormalDetail(ct->fieldName(i),
                                                  ct->fieldHasDefaultValue(i),
                                                  fieldDecl);
        formals.push_back(d);
        // formalType should have been set above
        assert(formalType.kind() != QualifiedType::UNKNOWN);
        formalTypes.push_back(formalType);
      }
    }
  } else {
    assert(false && "case not handled");
  }

  auto untyped = UntypedFnSignature::get(context,
                                         id, name,
                                         /* isMethod */ false,
                                         /* idIsFunction */ false,
                                         /* isTypeConstructor */ true,
                                         Function::PROC,
                                         std::move(formals),
                                         /* whereClause */ nullptr);

  auto sig = new TypedFnSignature(untyped,
                                  std::move(formalTypes),
                                  TypedFnSignature::WHERE_NONE,
                                  /* needsInstantiation */ true,
                                  /* instantiatedFrom */ nullptr,
                                  /* parentFn */ nullptr);
  result = toOwned(sig);

  return QUERY_END(result);
}

const TypedFnSignature* typeConstructorInitial(Context* context,
                                               const types::Type* t) {
  return typeConstructorInitialQuery(context, t).get();
}

const TypedFnSignature* instantiateSignature(Context* context,
                                             const TypedFnSignature* sig,
                                             const CallInfo& call,
                                             const PoiScope* poiScope) {

  // Performance: Should this query use a similar approach to
  // resolveFunctionByInfoQuery, where the PoiInfo and visibility
  // are consulted?
  //
  // It does not impact correctness, because typedSignatureQuery
  // will arrange to construct a unique TypedFnSignature by
  // its contents.

  assert(sig->needsInstantiation());

  const UntypedFnSignature* untypedSignature = sig->untyped();
  const ASTNode* ast = nullptr;
  const Function* fn = nullptr;
  const AggregateDecl* ad = nullptr;

  if (!untypedSignature->id().isEmpty()) {
    ast = parsing::idToAst(context, untypedSignature->id());
    fn = ast->toFunction();
    ad = ast->toAggregateDecl();
  }

  const TypedFnSignature* parentFnTyped = nullptr;
  if (sig->parentFn()) {
    assert(false && "generic child functions not yet supported");
    // TODO: how to compute parentFn for the instantiation?
    // Does the parent function need to be instantiated in some case?
    // Set parentFnTyped somehow.
  }

  auto faMap = FormalActualMap(sig, call);
  if (!faMap.isValid()) {
    return nullptr;
  }

  // compute the substitutions
  SubstitutionsMap substitutions;
  for (const FormalActual& entry : faMap.byFormalIdx()) {
    // note: entry.actualType can have type()==nullptr and UNKNOWN.
    // in that case, resolver code should treat it as a hint to
    // use the default value. Unless the call used a ? argument.
    if (call.hasQuestionArg() &&
        entry.actualType.kind() == QualifiedType::UNKNOWN &&
        entry.actualType.type() == nullptr) {
      // don't add any substitution
    } else {
      // add a substitution that is either valid or a
      // "use the default" hint.
      substitutions.insert({entry.formal, entry.actualType});
    }
  }

  std::vector<types::QualifiedType> formalTypes;
  bool needsInstantiation = false;
  TypedFnSignature::WhereClauseResult where = TypedFnSignature::WHERE_NONE;

  if (fn != nullptr) {
    // visit the formals, return type, where clause
    ResolutionResultByPostorderID r;
    Resolver visitor(context, fn, substitutions, poiScope, r);
    for (auto child: fn->children()) {
      child->traverse(visitor);
    }

    auto tmp = getFormalTypes(fn, r);
    formalTypes.swap(tmp);
    needsInstantiation = anyFormalNeedsInstantiation(formalTypes);
    where = whereClauseResult(context, fn, r, needsInstantiation);
  } else if (ad != nullptr) {
    // visit the fields
    ResolutionResultByPostorderID r;
    Resolver visitor(context, ad, substitutions, poiScope, r,
                     /* useGenericFormalDefaults */ false);
    // TODO: handle MultiDecl and TupleDecl
    for (auto child: ad->children()) {
      if (auto var = child->toVariable()) {
        if (var->isField()) {
          child->traverse(visitor);
        }
      }
    }

    // now pull out the field types
    int nFormals = sig->numFormals();
    for (int i = 0; i < nFormals; i++) {
      const Decl* fieldDecl = untypedSignature->formalDecl(i);
      const ResolvedExpression& e = r.byAst(fieldDecl);
      QualifiedType fieldType = e.type();
      QualifiedType sigType = sig->formalType(i);

      // use the same kind as the old formal type but update the type, param
      // to reflect how instantiation occured.
      formalTypes.push_back(QualifiedType(sigType.kind(),
                                          fieldType.type(),
                                          fieldType.param()));
    }
    needsInstantiation = anyFormalNeedsInstantiation(formalTypes);
  } else {
    assert(false && "case not handled");
  }

  // now, construct a TypedFnSignature from the result
  const auto& result = typedSignatureQuery(context,
                                           untypedSignature,
                                           std::move(formalTypes),
                                           where,
                                           needsInstantiation,
                                           /* instantiatedFrom */ sig,
                                           /* parentFn */ parentFnTyped);
  return result.get();
}

static const owned<ResolvedFunction>&
resolveFunctionByPoisQuery(Context* context,
                           const TypedFnSignature* sig,
                           std::set<std::pair<ID, ID>> poiFnIdsUsed) {
  QUERY_BEGIN(resolveFunctionByPoisQuery, context, sig, poiFnIdsUsed);

  owned<ResolvedFunction> result;
  // the actual value is set in resolveFunctionByInfoQuery after it is
  // computed because computing it generates the poiFnIdsUsed which is
  // part of the key for this query.
  assert(false && "should not be reached");

  return QUERY_END(result);
}

static const ResolvedFunction* const&
resolveFunctionByInfoQuery(Context* context,
                            const TypedFnSignature* sig,
                            PoiInfo poiInfo) {
  QUERY_BEGIN(resolveFunctionByInfoQuery, context, sig, poiInfo);

  const UntypedFnSignature* untypedSignature = sig->untyped();
  const ASTNode* ast = parsing::idToAst(context, untypedSignature->id());
  const Function* fn = ast->toFunction();

  const PoiScope* poiScope = poiInfo.poiScope();

  PoiInfo resolvedPoiInfo;

  if (fn) {
    ResolutionResultByPostorderID resolutionById;
    Resolver visitor(context, fn, poiScope, sig, resolutionById);

    // visit the body
    fn->body()->traverse(visitor);

    resolvedPoiInfo.swap(visitor.poiInfo);
    // TODO can this be encapsulated in a method?
    resolvedPoiInfo.setResolved(true);
    resolvedPoiInfo.setPoiScope(nullptr);

    owned<ResolvedFunction> resolved = toOwned(new ResolvedFunction(sig, fn->returnIntent(), resolutionById, resolvedPoiInfo));

    // Store the result in the query under the POIs used.
    // If there was already a value for this revision, this
    // call will not update it. (If it did, that could lead to
    // memory errors).
    QUERY_STORE_RESULT(resolveFunctionByPoisQuery,
                       context,
                       resolved,
                       sig,
                       resolvedPoiInfo.poiFnIdsUsed());
  } else {
    assert(false && "this query should be called on Functions");
  }

  // Return the unique result from the query (that might have been saved above)
  const owned<ResolvedFunction>& resolved =
    resolveFunctionByPoisQuery(context, sig, resolvedPoiInfo.poiFnIdsUsed());

  const ResolvedFunction* result = resolved.get();

  return QUERY_END(result);
}

const ResolvedFunction* resolveFunction(Context* context,
                                         const TypedFnSignature* sig,
                                         const PoiScope* poiScope) {
  // this should only be applied to concrete fns or instantiations
  assert(!sig->needsInstantiation());

  // construct the PoiInfo for this case
  auto poiInfo = PoiInfo(poiScope);

  // lookup in the map using this PoiInfo
  return resolveFunctionByInfoQuery(context, sig, std::move(poiInfo));
}

const ResolvedFunction* resolveConcreteFunction(Context* context, ID id) {
  auto func = parsing::idToAst(context, id)->toFunction();
  if (func == nullptr)
    return nullptr;

  const UntypedFnSignature* uSig = UntypedFnSignature::get(context, func);
  const TypedFnSignature* sig = typedSignatureInitial(context, uSig);
  if (sig->needsInstantiation())
    return nullptr;

  const ResolvedFunction* ret = resolveFunction(context, sig, nullptr);
  return ret;
}

const ResolvedFunction* resolveOnlyCandidate(Context* context,
                                              const ResolvedExpression& r) {
  const TypedFnSignature* sig = r.mostSpecific().only();
  const PoiScope* poiScope = r.poiScope();

  if (sig == nullptr)
    return nullptr;

  return resolveFunction(context, sig, poiScope);
}

struct ReturnTypeInferer {
  // input
  Context* context;
  Function::ReturnIntent returnIntent;
  const ResolutionResultByPostorderID& resolutionById;

  // output
  std::vector<QualifiedType> returnedTypes;

  ReturnTypeInferer(Context* context,
                    const ResolvedFunction& resolvedFn)
    : context(context),
      returnIntent(resolvedFn.returnIntent()),
      resolutionById(resolvedFn.resolutionById()) {
  }

  bool enter(const Function* fn) {
    return false;
  }
  void exit(const Function* fn) {
  }

  void checkReturn(const Expression* inExpr, const QualifiedType& qt) {
    if (qt.type()->isVoidType()) {
      if (returnIntent == Function::REF) {
        context->error(inExpr, "Cannot return void with ref return intent");
      } else if (returnIntent == Function::PARAM) {
        context->error(inExpr, "Cannot return void with param return intent");
      } else if (returnIntent == Function::TYPE) {
        context->error(inExpr, "Cannot return void with type return intent");
      }
    } else {
      bool ok = true;
      if ((qt.isType() || qt.isParam()) &&
          (returnIntent == Function::CONST_REF ||
           returnIntent == Function::REF)) {
        ok = false;
      } else if (returnIntent == Function::TYPE && !qt.isType()) {
        ok = false;
      } else if (returnIntent == Function::PARAM && !qt.isParam()) {
        ok = false;
      }
      if (!ok) {
        context->error(inExpr, "cannot return it with provided return intent");
      }
    }
  }

  void noteVoidReturnType(const Expression* inExpr) {
    auto voidType = QualifiedType(QualifiedType::CONST_VAR, VoidType::get(context));
    returnedTypes.push_back(voidType);

    checkReturn(inExpr, voidType);
  }
  void noteReturnType(const Expression* expr, const Expression* inExpr) {
    const QualifiedType& qt = resolutionById.byAst(expr).type();

    QualifiedType::Kind kind = qt.kind();
    const Type* type = qt.type();

    checkReturn(inExpr, qt);

    kind = (QualifiedType::Kind) returnIntent;

    returnedTypes.push_back(QualifiedType(kind, type));
  }

  QualifiedType returnedType() {
    if (returnedTypes.size() == 0) {
      return QualifiedType(QualifiedType::CONST_VAR, VoidType::get(context));
    } else if (returnedTypes.size() == 1) {
      return returnedTypes[0];
    } else {
      assert(false && "TODO");
      QualifiedType ret;
      return ret;
    }
  }

  bool enter(const Return* ret) {
    if (const Expression* expr = ret->value()) {
      noteReturnType(expr, ret);
    } else {
      noteVoidReturnType(ret);
    }
    return false;
  }
  void exit(const Return* ret) {
  }

  bool enter(const Yield* ret) {
    noteReturnType(ret->value(), ret);
    return false;
  }
  void exit(const Yield* ret) {
  }

  bool enter(const ASTNode* ast) {
    return true;
  }
  void exit(const ASTNode* ast) {
  }
};

const QualifiedType& returnType(Context* context,
                                const TypedFnSignature* sig,
                                const PoiScope* poiScope) {
  QUERY_BEGIN(returnType, context, sig, poiScope);


  const UntypedFnSignature* untyped = sig->untyped();

  QualifiedType result;

  if (untyped->idIsFunction()) {
    // this should only be applied to concrete fns or instantiations
    assert(!sig->needsInstantiation());

    const ASTNode* ast = parsing::idToAst(context, untyped->id());
    const Function* fn = ast->toFunction();
    assert(fn);
    if (const Expression* retType = fn->returnType()) {
      // resolve the return type
      ResolutionResultByPostorderID resolutionById;
      Resolver visitor(context, fn, poiScope, sig, resolutionById);
      retType->traverse(visitor);
      result = resolutionById.byAst(retType).type();
    } else {
      // resolve the function body
      const ResolvedFunction* rFn = resolveFunction(context, sig, poiScope);
      // infer the return type
      ReturnTypeInferer visitor(context, *rFn);
      fn->body()->traverse(visitor);
      result = visitor.returnedType();
    }
  } else if (untyped->isTypeConstructor()) {
    // handle type construction
    const AggregateDecl* ad = nullptr;
    if (!untyped->id().isEmpty())
      if (auto ast = parsing::idToAst(context, untyped->id()))
        ad = ast->toAggregateDecl();

    if (ad) {
      // compute the substitutions
      SubstitutionsMap substitutions;

      int nFormals = sig->numFormals();
      for (int i = 0; i < nFormals; i++) {
        const Decl* formalDecl = untyped->formalDecl(i);
        const QualifiedType& formalType = sig->formalType(i);
        substitutions.insert({formalDecl, formalType});
      }

      std::vector<CompositeType::FieldDetail> fields;

      // visit the fields, computing types once again
      ResolutionResultByPostorderID r;
      Resolver visitor(context, ad, substitutions, poiScope, r,
                       /* useGenericFormalDefaults */ false);
      // TODO: handle MultiDecl and TupleDecl
      for (auto child: ad->children()) {
        if (auto var = child->toVariable()) {
          if (var->isField()) {
            var->traverse(visitor);

            UniqueString name = var->name();
            bool hasDefault = var->initExpression() != nullptr;
            QualifiedType type = r.byAst(var).type();

            fields.push_back(
              CompositeType::FieldDetail(name, hasDefault, var, type));
          }
        }
      }

      const Type* t = nullptr;
      if (auto rec = ad->toRecord()) {
        t = RecordType::get(context, rec->id(), rec->name(),
                            std::move(fields));
      }
      if (auto cls = ad->toClass()) {
        auto bct = BasicClassType::get(context, cls->id(), cls->name(),
                                       std::move(fields));
        auto d = ClassTypeDecorator(ClassTypeDecorator::GENERIC_NONNIL);
        t = ClassType::get(context, bct, nullptr, d);
      }
      if (auto uni = ad->toUnion()) {
        t = UnionType::get(context, uni->id(), uni->name(),
                           std::move(fields));
      }
      result = QualifiedType(QualifiedType::TYPE, t);

    } else {
      // built-in type construction should be handled
      // by resolveFnCallSpecialType and not reach this point.
      assert(false && "case not handled");
    }
  } else {
    assert(false && "case not handled");
  }

  return QUERY_END(result);
}

// TODO move these to a core logic of resolution file
static QualifiedType::Kind resolveIntent(const QualifiedType& t) {
  if (t.type()->isPrimitiveType()) {
    auto kind = t.kind();
    if (kind == QualifiedType::UNKNOWN ||
        kind == QualifiedType::DEFAULT_INTENT ||
        kind == QualifiedType::CONST_INTENT)
      return QualifiedType::CONST_IN;
  } else if (t.isGenericOrUnknown()) {
    return QualifiedType::UNKNOWN;
  } else {
    assert(false && "TODO");
  }
  return t.kind();
}

static bool canPassInitial(const QualifiedType& actualType,
                           const QualifiedType& formalType) {
  // TODO: use Any type vs. Unknown for formals without a type?
  if (formalType.kind() == QualifiedType::UNKNOWN) return true;

  auto result = canPass(actualType, formalType);
  return result.passes();
}

static bool canPassAfterInstantiating(const QualifiedType& actualType,
                                      const QualifiedType& formalType) {
  // TODO: Any type handling should be in canPass
  if (formalType.type()->isAnyType()) return true;

  auto result = canPass(actualType, formalType);
  return result.passes();
}

// returns nullptr if the candidate is not applicable,
// or the result of typedSignatureInitial if it is.
static const TypedFnSignature*
doIsCandidateApplicableInitial(Context* context,
                               const ID& candidateId,
                               const CallInfo& call) {

  const ASTNode* ast = nullptr;
  const Function* fn = nullptr;

  if (!candidateId.isEmpty()) {
    ast = parsing::idToAst(context, candidateId);
    fn = ast->toFunction();
  }

  if (ast == nullptr || ast->isTypeDecl()) {
    // calling a builtin type or a declared type
    const TypeDecl* td = nullptr;
    if (ast)
      td = ast->toTypeDecl();

    if (td != nullptr) {
      const Type* t = typeForTypeDecl(context, td,
                                      /* useGenericFormalDefaults */ false);
      return typeConstructorInitial(context, t);
    } else {
      assert(false && "case not handled");
    }
  }

  assert(fn);
  auto uSig = UntypedFnSignature::get(context, fn);
  // First, check that the untyped properties allow a match:
  //  * number of arguments
  //  * names of arguments
  //  * method-ness
  //  * ref-ness

  auto faMap = FormalActualMap(uSig, call);
  if (!faMap.isValid()) {
    return nullptr;
  }

  // TODO: check method-ness
  // TODO: reason failed

  auto initialTypedSignature = typedSignatureInitial(context, uSig);
  // Next, check that the types are compatible
  int formalIdx = 0;
  for (const FormalActual& entry : faMap.byFormalIdx()) {
    const auto& actualType = entry.actualType;
    const auto& formalType = initialTypedSignature->formalType(formalIdx);
    bool ok = canPassInitial(actualType, formalType);
    if (!ok) {
      return nullptr;
    }

    formalIdx++;
  }

  // check that the where clause applies
  auto whereResult = initialTypedSignature->whereClauseResult();
  if (whereResult == TypedFnSignature::WHERE_FALSE) {
    return nullptr;
  }

  return initialTypedSignature;
}

// returns nullptr if the candidate is not applicable,
// or the result of an instantiated typedSignature if it is.
static const TypedFnSignature*
doIsCandidateApplicableInstantiating(Context* context,
                                     const TypedFnSignature* typedSignature,
                                     const CallInfo& call,
                                     const PoiScope* poiScope) {

  const TypedFnSignature* instantiated =
    instantiateSignature(context, typedSignature, call, poiScope);

  if (instantiated == nullptr)
    return nullptr;

  // Next, check that the types are compatible
  size_t nActuals = call.numActuals();
  for (size_t i = 0; i < nActuals; i++) {
    const QualifiedType& actualType = call.actuals(i).type();
    const QualifiedType& formalType = instantiated->formalType(i);
    bool ok = canPassAfterInstantiating(actualType, formalType);
    if (!ok)
      return nullptr;
  }

  // check that the where clause applies
  if (instantiated->whereClauseResult() == TypedFnSignature::WHERE_FALSE)
    return nullptr;

  return instantiated;
}

static const TypedFnSignature* const&
isCandidateApplicableInitialQuery(Context* context,
                                  ID candidateId,
                                  CallInfo call) {

  QUERY_BEGIN(isCandidateApplicableInitialQuery, context, candidateId, call);

  const TypedFnSignature* result =
    doIsCandidateApplicableInitial(context, candidateId, call);

  return QUERY_END(result);
}

const std::vector<const TypedFnSignature*>&
filterCandidatesInitial(Context* context,
                        std::vector<BorrowedIdsWithName> lst,
                        CallInfo call) {
  QUERY_BEGIN(filterCandidatesInitial, context, lst, call);

  std::vector<const TypedFnSignature*> result;

  for (const BorrowedIdsWithName& ids : lst) {
    for (const ID& id : ids) {
      const TypedFnSignature* s =
        isCandidateApplicableInitialQuery(context, id, call);
      if (s != nullptr) {
        result.push_back(s);
      }
    }
  }

  return QUERY_END(result);
}

void
filterCandidatesInstantiating(Context* context,
                              const std::vector<const TypedFnSignature*>& lst,
                              const CallInfo& call,
                              const Scope* inScope,
                              const PoiScope* inPoiScope,
                              std::vector<const TypedFnSignature*>& result) {

  // Performance: Would it help to make this a query?
  // (I left it not as a query since it runs some other queries
  //  and seems like it might have limited ability for reuse).
  const PoiScope* instantiationPoiScope = nullptr;

  for (const TypedFnSignature* typedSignature : lst) {
    if (typedSignature->needsInstantiation()) {
      if (instantiationPoiScope == nullptr) {
        instantiationPoiScope =
          pointOfInstantiationScope(context, inScope, inPoiScope);
      }

      const TypedFnSignature* instantiated =
        doIsCandidateApplicableInstantiating(context,
                                             typedSignature,
                                             call,
                                             instantiationPoiScope);
      if (instantiated != nullptr) {
        result.push_back(instantiated);
      }
    } else {
      // if it's already concrete, we already know it is a candidate.
      result.push_back(typedSignature);
    }
  }
}

const MostSpecificCandidates&
findMostSpecificCandidates(Context* context,
                           std::vector<const TypedFnSignature*> lst,
                           CallInfo call) {
  QUERY_BEGIN(findMostSpecificCandidates, context, lst, call);

  MostSpecificCandidates result;

  if (lst.size() > 1) {

    // TODO: find most specific -- pull over disambiguation code
    // TODO: handle return intent overloading
    // TODO: this is demo code
    if (call.numActuals() > 1) {
      if (call.actuals(1).type().type()->isIntType()) {
        result.setBestRef(lst[0]);
      } else {
        result.setBestRef(lst[lst.size()-1]);
      }
    } else {
      result.setBestRef(lst[0]);
    }
  }
  if (lst.size() == 1) {
    result.setBestRef(lst[0]);
  }

  return QUERY_END(result);
}

static std::vector<BorrowedIdsWithName>
lookupCalledExpr(Context* context,
                 const Scope* scope,
                 const Call* call,
                 std::unordered_set<const Scope*>& visited) {

  std::vector<BorrowedIdsWithName> ret;

  LookupConfig config = LOOKUP_DECLS |
                        LOOKUP_IMPORT_AND_USE |
                        LOOKUP_PARENTS;

  if (auto op = call->toOpCall()) {
    auto vec = lookupNameInScopeWithSet(context, scope, op->op(), config,
                                        visited);
    ret.swap(vec);
  } else if (const Expression* called = call->calledExpression()) {
    auto vec = lookupInScopeWithSet(context, scope, called, config,
                                    visited);
    ret.swap(vec);
  }

  return ret;
}


static
void accumulatePoisUsedByResolvingBody(Context* context,
                                       const TypedFnSignature* signature,
                                       const PoiScope* poiScope,
                                       PoiInfo& poiInfo) {

  if (signature == nullptr) {
    return;
  }

  assert(!signature->needsInstantiation());

  if (signature->instantiatedFrom() == nullptr) {
    // if it's not an instantiation, no need to gather POIs
    return;
  }

  // resolve the body
  const ResolvedFunction* r = resolveFunction(context, signature, poiScope);

  // gather the POI scopes from instantiating the function body
  poiInfo.accumulate(r->poiInfo());
}

// if the call's name matches a class management type construction,
// return the result or ErroneousType.
// returns nullptr if the class type is not handled here.
static const Type* getManagedClassType(Context* context,
                                       const Call* call,
                                       const CallInfo& ci) {
  UniqueString name = ci.name();

  if (ci.hasQuestionArg()) {
    if (ci.numActuals() != 0) {
      context->error(call, "invalid class type construction");
      return ErroneousType::get(context);
    } else if (name == "owned") {
      return AnyOwnedType::get(context);
    } else if (name == "shared") {
      return AnySharedType::get(context);
    } else if (name == "unmanaged") {
      return AnyUnmanagedType::get(context);
    } else if (name == "borrowed") {
      return AnyBorrowedType::get(context);
    } else {
      // case not handled in here
      return nullptr;
    }
  }

  ClassTypeDecorator::ClassTypeDecoratorEnum de;
  const Type* manager = nullptr;

  if (name == "owned") {
    de = ClassTypeDecorator::MANAGED;
    manager = AnyOwnedType::get(context);
  } else if (name == "shared") {
    de = ClassTypeDecorator::MANAGED;
    manager = AnySharedType::get(context);
  } else if (name == "unmanaged") {
    de = ClassTypeDecorator::UNMANAGED;
    manager = nullptr;
  } else if (name == "borrowed") {
    de = ClassTypeDecorator::BORROWED;
    manager = nullptr;
  } else {
    // case not handled in here
    return nullptr;
  }

  auto d = ClassTypeDecorator(de);

  const Type* t = nullptr;
  if (ci.numActuals() > 0)
    t = ci.actuals(0).type().type();

  if (t == nullptr || !(t->isBasicClassType() || t->isClassType())) {
    context->error(call, "invalid class type construction");
    return ErroneousType::get(context);
  }

  const BasicClassType* bct = nullptr;
  if (auto ct = t->toClassType()) {
    bct = ct->basicClassType();
    // get nilability from ct
    if (ct->decorator().isNilable())
      d = d.addNilable();
    if (ct->decorator().isNonNilable())
      d = d.addNonNil();
  } else {
    bct = t->toBasicClassType();
  }

  assert(bct);
  return ClassType::get(context, bct, manager, d);
}

static const Type* getNumericType(Context* context,
                                  const Call* call,
                                  const CallInfo& ci) {
  UniqueString name = ci.name();

  if (ci.hasQuestionArg()) {
    if (ci.numActuals() != 0) {
      context->error(call, "invalid numeric type construction");
      return ErroneousType::get(context);
    } else if (name == "int") {
      return AnyIntType::get(context);
    } else if (name == "uint") {
      return AnyUintType::get(context);
    } else if (name == "bool") {
      return AnyBoolType::get(context);
    } else if (name == "real") {
      return AnyRealType::get(context);
    } else if (name == "imag") {
      return AnyImagType::get(context);
    } else if (name == "complex") {
      return AnyComplexType::get(context);
    } else {
      // case not handled in here
      return nullptr;
    }
  }

  if (name == "int" || name == "uint" || name == "bool" ||
      name == "real" || name == "imag" || name == "complex") {

    QualifiedType qt;
    if (ci.numActuals() > 0)
      qt = ci.actuals(0).type();

    if (qt.type() == nullptr || !qt.type()->isIntType() ||
        qt.param() == nullptr || !qt.param()->isIntParam() ||
        ci.numActuals() != 1) {
      context->error(call, "invalid numeric type construction");
      return ErroneousType::get(context);
    }

    const Type* ret = nullptr;
    auto ip = qt.param()->toIntParam();
    auto value = ip->value();
    if (0 <= value && value <= 128) {
      ret = PrimitiveType::getWithNameAndWidth(context, name, value);
    }

    if (ret == nullptr) {
      context->error(call, "invalid numeric type construction");
      return ErroneousType::get(context);
    }

    return ret;
  }

  return nullptr;
}

// Resolving compiler-supported type-returning patterns
// 'call' and 'inPoiScope' are used for the location for error reporting.
static const Type* resolveFnCallSpecialType(Context* context,
                                            const Call* call,
                                            const CallInfo& ci) {

  if (ci.name() == "?") {
    if (ci.numActuals() > 0) {
      if (const Type* t = ci.actuals(0).type().type()) {
        const ClassType* ct = nullptr;

        if (auto bct = t->toBasicClassType()) {
          auto d = ClassTypeDecorator(ClassTypeDecorator::GENERIC_NONNIL);
          ct = ClassType::get(context, bct, nullptr, d);
        } else {
          ct = t->toClassType();
        }

        if (ct) {
          // get the nilable version of the class type
          ClassTypeDecorator d = ct->decorator().addNilable();
          auto nilable = ct->withDecorator(context, d);
          return nilable;
        }
      }
    }
  }

  if (auto t = getManagedClassType(context, call, ci)) {
    return t;
  }

  if (auto t = getNumericType(context, call, ci)) {
    return t;
  }

  return nullptr;
}


// Resolving calls for certain compiler-supported patterns
// without requiring module implementations exist at all.
// 'call' and 'inPoiScope' are used for the location for error reporting.
static bool resolveFnCallSpecial(Context* context,
                                 const Call* call,
                                 const CallInfo& ci,
                                 QualifiedType& exprTypeOut) {
  // TODO: type comparisons
  // TODO: cast
  // TODO: .borrow()
  // TODO: chpl__coerceCopy

  if (const Type* t = resolveFnCallSpecialType(context, call, ci)) {
    exprTypeOut = QualifiedType(QualifiedType::TYPE, t);
    return true;
  }

  return false;
}


static
CallResolutionResult resolveFnCall(Context* context,
                                   const Call* call,
                                   const CallInfo& ci,
                                   const Scope* inScope,
                                   const PoiScope* inPoiScope) {
  // search for candidates at each POI until we have found a candidate
  std::vector<const TypedFnSignature*> candidates;
  size_t firstPoiCandidate = 0;
  std::unordered_set<const Scope*> visited;
  PoiInfo poiInfo;

  // first, look for candidates without using POI.

  {
    // compute the potential functions that it could resolve to
    auto v = lookupCalledExpr(context, inScope, call, visited);

    // filter without instantiating yet
    const auto& initialCandidates = filterCandidatesInitial(context, v, ci);

    // find candidates, doing instantiation if necessary
    filterCandidatesInstantiating(context,
                                  initialCandidates,
                                  ci,
                                  inScope,
                                  inPoiScope,
                                  candidates);

    firstPoiCandidate = candidates.size();
  }

  // next, look for candidates using POI
  for (const PoiScope* curPoi = inPoiScope;
       curPoi != nullptr;
       curPoi = curPoi->inFnPoi()) {

    // stop if any candidate has been found.
    if (candidates.empty() == false) {
      break;
    }

    // compute the potential functions that it could resolve to
    auto v = lookupCalledExpr(context, curPoi->inScope(), call, visited);

    // filter without instantiating yet
    const auto& initialCandidates = filterCandidatesInitial(context, v, ci);

    // find candidates, doing instantiation if necessary
    filterCandidatesInstantiating(context,
                                  initialCandidates,
                                  ci,
                                  inScope,
                                  inPoiScope,
                                  candidates);
  }

  // find most specific candidates / disambiguate
  MostSpecificCandidates mostSpecific = findMostSpecificCandidates(context,
                                                                   candidates,
                                                                   ci);

  // note any most specific candidates from POI in poiFnIdsUsed.
  {
    size_t n = candidates.size();
    for (size_t i = firstPoiCandidate; i < n; i++) {
      for (const TypedFnSignature* candidate : mostSpecific) {
        if (candidate != nullptr) {
          poiInfo.addIds(call->id(), candidate->id());
        }
      }
    }
  }

  // fully resolve each candidate function and gather poiScopesUsed.

  // figure out the poiScope to use
  const PoiScope* instantiationPoiScope = nullptr;
  bool anyInstantiated = false;

  for (const TypedFnSignature* candidate : mostSpecific) {
    if (candidate != nullptr && candidate->instantiatedFrom() != nullptr) {
      anyInstantiated = true;
      break;
    }
  }

  if (anyInstantiated) {
    instantiationPoiScope =
      pointOfInstantiationScope(context, inScope, inPoiScope);
    poiInfo.setPoiScope(instantiationPoiScope);
  }

  for (const TypedFnSignature* candidate : mostSpecific) {
    if (candidate != nullptr) {
      if (candidate->untyped()->idIsFunction()) {
        accumulatePoisUsedByResolvingBody(context, candidate,
                                          instantiationPoiScope, poiInfo);
      }
    }
  }

  // compute the return types
  QualifiedType retType;
  bool retTypeSet = false;
  for (const TypedFnSignature* candidate : mostSpecific) {
    if (candidate != nullptr) {
      QualifiedType t = returnType(context, candidate, instantiationPoiScope);
      if (retTypeSet && retType.type() != t.type()) {
        context->error(candidate,
                       nullptr,
                       "return intent overload type does not match");
      }
      retType = t;
      retTypeSet = true;
    }
  }

  return CallResolutionResult(mostSpecific, retType, std::move(poiInfo));
}

static
CallResolutionResult resolvePrimCall(Context* context,
                                     const PrimCall* call,
                                     const CallInfo& ci,
                                     const Scope* inScope,
                                     const PoiScope* inPoiScope) {

  bool allParam = true;
  for (const CallInfoActual& actual : ci.actuals()) {
    if (!actual.type().hasParamPtr()) {
      allParam = false;
      break;
    }
  }

  MostSpecificCandidates candidates;
  QualifiedType type;
  PoiInfo poi;

  // start with a non-param result type based on the 1st argument
  // TODO: do something more intelligent with a table of params
  if (ci.numActuals() > 0) {
    type = QualifiedType(QualifiedType::CONST_VAR, ci.actuals(0).type().type());
  }

  // handle param folding
  auto prim = call->prim();
  if (Param::isParamOpFoldable(prim)) {
    if (allParam && ci.numActuals() == 2) {
      type = Param::fold(context, prim, ci.actuals(0).type(), ci.actuals(1).type());
    }
  }

  return CallResolutionResult(candidates, type, poi);
}

CallResolutionResult resolveCall(Context* context,
                                 const Call* call,
                                 const CallInfo& ci,
                                 const Scope* inScope,
                                 const PoiScope* inPoiScope) {
  if (call->isFnCall() || call->isOpCall()) {
    // see if the call is handled directly by the compiler
    QualifiedType tmpRetType;
    if (resolveFnCallSpecial(context, call, ci, tmpRetType)) {
      return CallResolutionResult(std::move(tmpRetType));
    }
    // otherwise do regular call resolution
    return resolveFnCall(context, call, ci, inScope, inPoiScope);
  } else if (auto prim = call->toPrimCall()) {
    return resolvePrimCall(context, prim, ci, inScope, inPoiScope);
  }

  assert(false && "should not be reached");
  MostSpecificCandidates emptyCandidates;
  QualifiedType emptyType;
  PoiInfo emptyPoi;
  return CallResolutionResult(emptyCandidates, emptyType, emptyPoi);
}


} // end namespace resolution
} // end namespace chpl
