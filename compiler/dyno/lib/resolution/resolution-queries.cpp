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

#include "chpl/resolution/resolution-queries.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/queries/ErrorMessage.h"
#include "chpl/queries/UniqueString.h"
#include "chpl/queries/global-strings.h"
#include "chpl/queries/query-impl.h"
#include "chpl/resolution/can-pass.h"
#include "chpl/resolution/disambiguation.h"
#include "chpl/resolution/intents.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"

#include "Resolver.h"
#include "default-functions.h"
#include "prims.h"

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


const ResolutionResultByPostorderID& resolveModuleStmt(Context* context,
                                                       ID id) {
  QUERY_BEGIN(resolveModuleStmt, context, id);

  assert(id.postOrderId() >= 0);

  // TODO: can we save space better here by having
  // the ResolutionResultByPostorderID have a different offset
  // (so it can contain only ids within the requested stmt) or
  // maybe we can make it sparse with a hashtable or something?
  ResolutionResultByPostorderID result;

  ID moduleId = parsing::idToParentId(context, id);
  auto moduleAst = parsing::idToAst(context, moduleId);
  if (const Module* mod = moduleAst->toModule()) {
    // Resolve just the requested statement
    auto modStmt = parsing::idToAst(context, id);
    auto visitor = Resolver::moduleStmtResolver(context, mod, modStmt, result);
    modStmt->traverse(visitor);
  }

  return QUERY_END(result);
}


const ResolutionResultByPostorderID& resolveModule(Context* context, ID id) {
  QUERY_BEGIN(resolveModule, context, id);

  const AstNode* ast = parsing::idToAst(context, id);
  assert(ast != nullptr);

  ResolutionResultByPostorderID result;

  if (ast != nullptr) {
    if (const Module* mod = ast->toModule()) {
      result.setupForSymbol(mod);
      for (auto child: mod->children()) {
        if (child->isComment() ||
            child->isTypeDecl() ||
            child->isFunction() ||
            child->isUse() ||
            child->isImport()) {
          // ignore this statement since it is not relevant to
          // the resolution of module initializers and module-level
          // variables.
        } else {
          ID stmtId = child->id();
          // resolve the statement
          const ResolutionResultByPostorderID& resolved =
            resolveModuleStmt(context, stmtId);

          // copy results for children and the node itself
          int firstId = stmtId.postOrderId() - stmtId.numContainedChildren();
          int lastId = firstId + stmtId.numContainedChildren();
          for (int i = firstId; i <= lastId; i++) {
            ID exprId(stmtId.symbolPath(), i, 0);
            result.byIdExpanding(exprId) = resolved.byId(exprId);
          }
        }
      }
    }
  }

  return QUERY_END(result);
}

const QualifiedType& typeForModuleLevelSymbol(Context* context, ID id) {
  QUERY_BEGIN(typeForModuleLevelSymbol, context, id);

  QualifiedType result;

  int postOrderId = id.postOrderId();
  if (postOrderId >= 0) {
    const auto& resolvedStmt = resolveModuleStmt(context, id);
    result = resolvedStmt.byId(id).type();
  } else {
    QualifiedType::Kind kind = QualifiedType::UNKNOWN;
    const Type* t = nullptr;

    auto tag = parsing::idToTag(context, id);

    if (asttags::isNamedDecl(tag)) {
      if (asttags::isTypeDecl(tag)) {
        kind = QualifiedType::TYPE;
        t = initialTypeForTypeDecl(context, id);
      } else if (asttags::isModule(tag)) {
        kind = QualifiedType::MODULE;
      } else if (asttags::isFunction(tag)) {
        if (parsing::idIsParenlessFunction(context, id)) {
          kind = QualifiedType::PARENLESS_FUNCTION;
        } else {
          kind = QualifiedType::FUNCTION;
        }
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

const QualifiedType& typeForBuiltin(Context* context,
                                    UniqueString name) {
  QUERY_BEGIN(typeForBuiltin, context, name);

  QualifiedType result;

  std::unordered_map<UniqueString,const Type*> map;
  Type::gatherBuiltins(context, map);

  auto search = map.find(name);
  if (search != map.end()) {
    const Type* t = search->second;
    assert(t);

    if (auto bct = t->toBasicClassType()) {
      auto d = ClassTypeDecorator(ClassTypeDecorator::GENERIC_NONNIL);
      t = ClassType::get(context, bct, /*manager*/ nullptr, d);
    }

    result = QualifiedType(QualifiedType::TYPE, t);
  } else {
    assert(false && "Should not be reachable");
  }

  return QUERY_END(result);
}

QualifiedType typeForLiteral(Context* context, const Literal* literal) {
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
      typePtr = RecordType::getBytesType(context);
      break;
    case asttags::CStringLiteral:
      typePtr = CStringType::get(context);
      break;
    case asttags::StringLiteral:
      typePtr = RecordType::getStringType(context);
      break;
    default:
      assert(false && "case not handled");
  }
  paramPtr = literal->param();

  return QualifiedType(QualifiedType::PARAM, typePtr, paramPtr);
}


/////// function resolution

static std::vector<types::QualifiedType>
getFormalTypes(const Function* fn,
               const ResolutionResultByPostorderID& r) {
  std::vector<types::QualifiedType> formalTypes;
  for (auto formal : fn->formals()) {
    QualifiedType t = r.byAst(formal).type();
    // compute concrete intent
    bool isThis = false;
    if (auto namedDecl = formal->toNamedDecl()) {
      isThis = namedDecl->name() == USTR("this");
    }
    t = QualifiedType(resolveIntent(t, isThis), t.type(), t.param());

    formalTypes.push_back(std::move(t));
  }
  return formalTypes;
}

static bool
anyFormalNeedsInstantiation(Context* context,
                            const std::vector<types::QualifiedType>& formalTs,
                            const UntypedFnSignature* untypedSig,
                            SubstitutionsMap* substitutions) {
  bool genericOrUnknown = false;
  int i = 0;
  for (const auto& qt : formalTs) {
    if (qt.isUnknown()) {
      genericOrUnknown = true;
      break;
    }

    bool considerGenericity = true;
    if (substitutions != nullptr) {
      auto formalDecl = untypedSig->formalDecl(i);
      if (substitutions->count(formalDecl->id())) {
        // don't consider it needing a substitution - e.g. when passing
        // a generic type into a type argument.
        considerGenericity = false;
      }
    }

    if (considerGenericity) {
      auto g = getTypeGenericity(context, qt);
      if (g != Type::CONCRETE) {
        genericOrUnknown = true;
        break;
      }
    }

    i++;
  }
  return genericOrUnknown;
}

static TypedFnSignature::WhereClauseResult whereClauseResult(
                                     Context* context,
                                     const Function* fn,
                                     const ResolutionResultByPostorderID& r,
                                     bool needsInstantiation) {
  auto whereClauseResult = TypedFnSignature::WHERE_TBD;
  if (const AstNode* where = fn->whereClause()) {
    const QualifiedType& qt = r.byAst(where).type();
    bool isBoolType = qt.type() && qt.type()->isBoolType();
    if (isBoolType && qt.isParamTrue()) {
      whereClauseResult = TypedFnSignature::WHERE_TRUE;
    } else if (isBoolType && qt.isParamFalse()) {
      whereClauseResult = TypedFnSignature::WHERE_FALSE;
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

static const TypedFnSignature* const&
typedSignatureInitialQuery(Context* context,
                           const UntypedFnSignature* untypedSig) {
  QUERY_BEGIN(typedSignatureInitialQuery, context, untypedSig);

  const TypedFnSignature* result = nullptr;
  const AstNode* ast = parsing::idToAst(context, untypedSig->id());
  const Function* fn = ast->toFunction();

  if (fn != nullptr) {
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
    auto visitor = Resolver::initialSignatureResolver(context, fn, r);
    // visit the formals
    for (auto formal : fn->formals()) {
      formal->traverse(visitor);
    }
    // do not visit the return type or function body

    // now, construct a TypedFnSignature from the result
    std::vector<types::QualifiedType> formalTypes = getFormalTypes(fn, r);
    bool needsInstantiation = anyFormalNeedsInstantiation(context, formalTypes,
                                                          untypedSig,
                                                          nullptr);

    // visit the where clause, unless it needs to be instantiated, in
    // which case we will visit the where clause when that happens
    TypedFnSignature::WhereClauseResult whereResult =
      TypedFnSignature::WHERE_NONE;
    if (auto whereClause = fn->whereClause()) {
      if (needsInstantiation) {
        whereResult = TypedFnSignature::WHERE_TBD;
      } else {
        whereClause->traverse(visitor);
        whereResult = whereClauseResult(context, fn, r, needsInstantiation);
      }
    }

    result = TypedFnSignature::get(context,
                                   untypedSig,
                                   std::move(formalTypes),
                                   whereResult,
                                   needsInstantiation,
                                   /* instantiatedFrom */ nullptr,
                                   /* parentFn */ parentFnTyped,
                                   /* formalsInstantiated */ Bitmap());
  }

  return QUERY_END(result);
}

const TypedFnSignature*
typedSignatureInitial(Context* context,
                      const UntypedFnSignature* untypedSig) {

  return typedSignatureInitialQuery(context, untypedSig);

}

// Get a Type for an AggregateDecl
// poiScope, instantiatedFrom are nullptr if not instantiating
static
const CompositeType* helpGetTypeForDecl(Context* context,
                                        const AggregateDecl* ad,
                                        const SubstitutionsMap& substitutions,
                                        const PoiScope* poiScope,
                                        const Type* instantiatedFrom) {

  assert(ad);

  // Filter out substitutions that aren't fields within 'ad'.
  // In particular, there might be substitutions to do with a parent class.
  SubstitutionsMap filteredSubs;
  for (auto pair : substitutions) {
    if (ad->id().contains(pair.first)) {
      filteredSubs.insert(pair);
    }
  }

  if (filteredSubs.empty()) {
    instantiatedFrom = nullptr;
  }

  const CompositeType* ret = nullptr;

  if (const Class* c = ad->toClass()) {
    const BasicClassType* parentClassType = nullptr;
    if (const AstNode* parentClassExpr = c->parentClass()) {
      // Resolve the parent class type expression
      ResolutionResultByPostorderID r;
      auto visitor =
        Resolver::parentClassResolver(context, c,
                                      substitutions,
                                      poiScope, r);
      parentClassExpr->traverse(visitor);

      QualifiedType qt = r.byAst(parentClassExpr).type();
      if (auto t = qt.type()) {
        if (auto bct = t->toBasicClassType())
          parentClassType = bct;
        else if (auto ct = t->toClassType())
          parentClassType = ct->basicClassType();
      }
      if (qt.isType() && parentClassType != nullptr) {
        // OK
      } else {
        context->error(parentClassExpr, "invalid parent class expression");
        parentClassType = BasicClassType::getObjectType(context);
      }
    } else {
      parentClassType = BasicClassType::getObjectType(context);
    }

    const BasicClassType* insnFromBct = nullptr;
    if (instantiatedFrom != nullptr) {
      if (auto bct = instantiatedFrom->toBasicClassType())
        insnFromBct = bct;
      else if (auto ct = instantiatedFrom->toClassType())
        insnFromBct = ct->basicClassType();
      else
        assert(false && "unexpected instantiatedFrom type");
    }


    if (!parentClassType->isObjectType() && !substitutions.empty()) {
      // recompute the parent class type with substitutions
      auto parentAst = parsing::idToAst(context, parentClassType->id());
      assert(parentAst);
      auto parentAd = parentAst->toAggregateDecl();
      assert(parentAd);
      auto got = helpGetTypeForDecl(context,
                                    parentAd,
                                    substitutions,
                                    poiScope,
                                    parentClassType);
      auto gotBct = got->toBasicClassType();
      assert(gotBct);
      parentClassType = gotBct;
    }

    ret = BasicClassType::get(context, c->id(), c->name(),
                              parentClassType,
                              insnFromBct, std::move(filteredSubs));

  } else if (auto r = ad->toRecord()) {
    const RecordType* insnFromRec = nullptr;
    if (instantiatedFrom != nullptr) {
      if (auto rec = instantiatedFrom->toRecordType())
        insnFromRec = rec;
      else
        assert(false && "unexpected instantiatedFrom type");
    }

    ret = RecordType::get(context, r->id(), r->name(),
                          insnFromRec, std::move(filteredSubs));

  } else if (auto u = ad->toUnion()) {
    const UnionType* insnFromUni = nullptr;
    if (instantiatedFrom != nullptr) {
      if (auto uni = instantiatedFrom->toUnionType())
        insnFromUni = uni;
      else
        assert(false && "unexpected instantiatedFrom type");
    }

    ret = UnionType::get(context, u->id(), u->name(),
                         insnFromUni, std::move(filteredSubs));

  } else {
    assert(false && "case not handled");
  }

  return ret;
}

// initedInParent is true if the decl variable is inited due to a parent
// uast node.  This comes up for TupleDecls.
static void helpSetFieldTypes(const AstNode* ast,
                              ResolutionResultByPostorderID& r,
                              bool initedInParent,
                              ResolvedFields& fields) {

  if (auto var = ast->toVarLikeDecl()) {
    bool hasDefaultValue = initedInParent || var->initExpression() != nullptr;
    const ResolvedExpression& e = r.byAst(var);
    fields.addField(var->name(), hasDefaultValue, var->id(), e.type());
  } else if (auto mult = ast->toMultiDecl()) {
    for (auto decl : mult->decls()) {
      helpSetFieldTypes(decl, r, initedInParent, fields);
    }
  } else if (auto tup = ast->toTupleDecl()) {
    bool hasInit = initedInParent || tup->initExpression() != nullptr;
    for (auto decl : tup->decls()) {
      helpSetFieldTypes(decl, r, hasInit, fields);
    }
  }

  // no action needed for other types of Decls since they aren't fields.
}

static const Type* const&
initialTypeForTypeDeclQuery(Context* context, ID declId) {
  QUERY_BEGIN(initialTypeForTypeDeclQuery, context, declId);

  const Type* result = nullptr;

  auto ast = parsing::idToAst(context, declId);
  if (auto ad = ast->toAggregateDecl()) {
    result = helpGetTypeForDecl(context, ad,
                                SubstitutionsMap(),
                                /* poiScope */ nullptr,
                                /* instantiatedFrom */ nullptr);

    if (result != nullptr) {
      if (auto bct = result->toBasicClassType()) {
        // for a 'class C' declaration, 'C' normally means a generic-management
        // non-nil C so adjust the result.
        auto dec = ClassTypeDecorator(ClassTypeDecorator::GENERIC_NONNIL);
        result = ClassType::get(context, bct, /*manager*/ nullptr, dec);
      }
    }
  }

  return QUERY_END(result);
}

const Type* initialTypeForTypeDecl(Context* context, ID declId) {
  return initialTypeForTypeDeclQuery(context, declId);
}

const ResolvedFields& resolveFieldDecl(Context* context,
                                       const CompositeType* ct,
                                       ID fieldId,
                                       bool useGenericFormalDefaults) {
  QUERY_BEGIN(resolveFieldDecl, context, ct, fieldId, useGenericFormalDefaults);

  ResolvedFields result;
  bool isObjectType = false;
  if (auto bct = ct->toBasicClassType()) {
    isObjectType = bct->isObjectType();
  }

  if (isObjectType) {
    // no need to try to resolve the fields for the object type,
    // which doesn't have a real uAST ID.

  } else {
    auto typeAst = parsing::idToAst(context, ct->id());
    assert(typeAst && typeAst->isAggregateDecl());
    auto ad = typeAst->toAggregateDecl();

    auto fieldAst = parsing::idToAst(context, fieldId);
    assert(fieldAst);

    if (ct->instantiatedFromCompositeType() == nullptr) {
      // handle resolving a not-yet-instantiated type
      ResolutionResultByPostorderID r;
      auto visitor =
        Resolver::initialFieldStmtResolver(context, ad, fieldAst,
                                           ct, r, useGenericFormalDefaults);

      // resolve the field types and set them in 'result'
      fieldAst->traverse(visitor);
      helpSetFieldTypes(fieldAst, r, /* initedInParent */ false, result);
    } else {
      // handle resolving an instantiated type

      // use nullptr for POI scope because POI is not considered
      // when resolving the fields when constructing a type..
      const PoiScope* poiScope = nullptr;
      ResolutionResultByPostorderID r;
      auto visitor =
        Resolver::instantiatedFieldStmtResolver(context, ad, fieldAst, ct,
                                                poiScope, r,
                                                useGenericFormalDefaults);

      // resolve the field types and set them in 'result'
      fieldAst->traverse(visitor);
      helpSetFieldTypes(fieldAst, r, /* initedInParent */ false, result);
    }
  }


  return QUERY_END(result);
}

static
const ResolvedFields& fieldsForTypeDeclQuery(Context* context,
                                             const CompositeType* ct,
                                             bool useGenericFormalDefaults) {
  QUERY_BEGIN(fieldsForTypeDeclQuery, context, ct, useGenericFormalDefaults);

  ResolvedFields result;

  assert(ct);
  result.setType(ct);

  bool isObjectType = false;
  if (auto bct = ct->toBasicClassType()) {
    isObjectType = bct->isObjectType();
  }

  if (isObjectType) {
    // no need to try to resolve the fields for the object type,
    // which doesn't have a real uAST ID.

  } else {
    auto ast = parsing::idToAst(context, ct->id());
    assert(ast && ast->isAggregateDecl());
    auto ad = ast->toAggregateDecl();

    for (auto child: ad->children()) {
      // Ignore everything other than VarLikeDecl, MultiDecl, TupleDecl
      if (child->isVarLikeDecl() ||
          child->isMultiDecl() ||
          child->isTupleDecl()) {
        const ResolvedFields& resolvedFields =
          resolveFieldDecl(context, ct, child->id(), useGenericFormalDefaults);
        // Copy resolvedFields into result
        int n = resolvedFields.numFields();
        for (int i = 0; i < n; i++) {
          result.addField(resolvedFields.fieldName(i),
                          resolvedFields.fieldHasDefaultValue(i),
                          resolvedFields.fieldDeclId(i),
                          resolvedFields.fieldType(i));
        }
      }
    }

    // finalize the field types to compute summary information
    // like whether any was generic
    result.finalizeFields(context);
  }

  return QUERY_END(result);
}

const ResolvedFields& fieldsForTypeDecl(Context* context,
                                        const CompositeType* ct,
                                        bool useGenericFormalDefaults) {

  // try first with useGenericFormalDefaults=false
  const auto& f = fieldsForTypeDeclQuery(context, ct, false);

  // If useGenericFormalDefaults was requested and the type
  // is generic with defaults, compute the type again.
  // We do it this way so that we are more likely to be able to reuse the
  // result of the above query in most cases since most types
  // are not generic record/class with defaults.
  if (useGenericFormalDefaults && f.isGenericWithDefaults()) {
    return fieldsForTypeDeclQuery(context, ct, true);
  }

  // Otherwise, use the value we just computed.
  return f;
}

static const CompositeType* getTypeWithDefaults(Context* context,
                                                const CompositeType* ct) {
  // resolve the fields with useGenericFormalDefaults=false
  const ResolvedFields& g = fieldsForTypeDecl(context, ct, false);
  if (!g.isGenericWithDefaults()) {
    return ct;
  }

  // and with useGenericFormalDefaults=true
  const ResolvedFields& r = fieldsForTypeDecl(context, ct, true);

  // for any field that has a different type in r than in g, add
  // a substitution, and get the type with those substitutions.
  SubstitutionsMap substitutions;
  int nFields = g.numFields();
  assert(r.numFields() == nFields);
  for (int i = 0; i < nFields; i++) {
    assert(g.fieldName(i) == r.fieldName(i));
    assert(g.fieldDeclId(i) == r.fieldDeclId(i));
    QualifiedType gType = g.fieldType(i);
    QualifiedType rType = r.fieldType(i);
    if (gType != rType) {
      // The type is different so add a substitution.
      substitutions.insert({g.fieldDeclId(i), rType});
    }
  }

  if (substitutions.size() == 0) {
    return ct;
  }

  auto ast = parsing::idToAst(context, ct->id());
  assert(ast && ast->isAggregateDecl());
  auto ad = ast->toAggregateDecl();

  // POI is not relevant here
  const PoiScope* poiScope = nullptr;
  const Type* instantiatedFrom = ct;

  // Create the composite type with those substitutions
  return helpGetTypeForDecl(context, ad, substitutions,
                            poiScope, instantiatedFrom);
}

static
const CompositeType* const& getTypeWithDefaultsQuery(Context* context,
                                                     const CompositeType* ct) {
  QUERY_BEGIN(getTypeWithDefaultsQuery, context, ct);

  auto result = getTypeWithDefaults(context, ct);

  return QUERY_END(result);
}

const types::QualifiedType typeWithDefaults(Context* context,
                                            types::QualifiedType t) {
  if (t.type()) {
    if (auto clst = t.type()->toClassType()) {
      auto bct = clst->basicClassType();
      auto got = getTypeWithDefaultsQuery(context, bct);
      assert(got->isBasicClassType());
      bct = got->toBasicClassType();

      auto r = ClassType::get(context, bct, clst->manager(), clst->decorator());
      return QualifiedType(t.kind(), r, t.param());
    } else if (auto ct = t.type()->toCompositeType()) {
      auto got = getTypeWithDefaultsQuery(context, ct);
      return QualifiedType(t.kind(), got, t.param());
    }
  }

  return t;
}


// the ignore argument is just to ignore types that we are currently
// computing the genericity of (we can assume that those are concrete).
// that is important for recursive class types (e.g. a linked list).
static Type::Genericity getFieldsGenericity(Context* context,
                                            const CompositeType* ct,
                                            std::set<const Type*>& ignore) {
  // Figure out the genericity of the type based on the genericity
  // of the fields.

  // add the current type to the ignore set, and stop now
  // if it is already in the ignore set.
  auto it = ignore.insert(ct);
  if (it.second == false) {
    // set already contained ct, so stop & consider it concrete
    return Type::CONCRETE;
  }

  // compute genericity of tuple types
  if (auto tt = ct->toTupleType()) {
    Type::Genericity combined = Type::CONCRETE;
    int n = tt->numElements();
    for (int i = 0; i < n; i++) {
      auto g = getTypeGenericityIgnoring(context, tt->elementType(i), ignore);
      assert(g != Type::MAYBE_GENERIC);
      if (g == Type::GENERIC) {
        combined = g;
      } else if (g == Type::GENERIC_WITH_DEFAULTS &&
                 combined == Type::CONCRETE) {
        combined = g;
      }
    }
    return combined;
  }

  // Some testing code creates CompositeType with empty IDs.
  // Assume these are concrete.
  // Also 'object' has an empty ID and is concrete.
  if (ct->id().isEmpty())
    return Type::CONCRETE;

  Type::Genericity g = Type::CONCRETE;

  if (auto bct = ct->toBasicClassType()) {
    g = getFieldsGenericity(context, bct->parentClassType(), ignore);
    assert(g != Type::MAYBE_GENERIC);
    if (g == Type::GENERIC)
      return Type::GENERIC;
  }

  if (context->isQueryRunning(fieldsForTypeDeclQuery,
                              std::make_tuple(ct, false)) ||
      context->isQueryRunning(fieldsForTypeDeclQuery,
                              std::make_tuple(ct, true))) {
    // TODO: is there a better way to avoid problems with recursion here?
    return Type::CONCRETE;
  }

  // this setting is irrelevant for this query since the
  // isGenericWithDefaults will be computed either way.
  bool useGenericFormalDefaults = false;
  const ResolvedFields& f = fieldsForTypeDecl(context, ct,
                                              useGenericFormalDefaults);

  if (f.isGenericWithDefaults() &&
      (g == Type::CONCRETE || g == Type::GENERIC_WITH_DEFAULTS))
    return Type::GENERIC_WITH_DEFAULTS;

  if (f.isGeneric())
    return Type::GENERIC;

  return g;
}

Type::Genericity getTypeGenericityIgnoring(Context* context, const Type* t,
                                           std::set<const Type*>& ignore) {
  if (t == nullptr)
    return Type::MAYBE_GENERIC;

  // check if the type knows the full answer
  Type::Genericity ret = t->genericity();

  if (ret != Type::MAYBE_GENERIC)
    return ret;

  if (t->isUnknownType())
    return Type::MAYBE_GENERIC;

  // MAYBE_GENERIC should only be returned for CompositeType /
  // ClassType right now.
  assert(t->isCompositeType() || t->isClassType());

  // the tuple type that isn't an instantiation is a generic type
  if (auto tt = t->toTupleType()) {
    if (tt->instantiatedFromCompositeType() == nullptr)
      return Type::GENERIC;
  }

  // string and bytes types are never generic
  if (t->isStringType() || t->isBytesType()) {
    return Type::CONCRETE;
  }

  if (auto classType = t->toClassType()) {
    // should be handled in BasicClassType::isGeneric
    // so this code should only be called if the management is concrete
    assert(!classType->decorator().isUnknownManagement());
    assert(!classType->decorator().isUnknownNilability());

    auto bct = classType->basicClassType();
    return getFieldsGenericity(context, bct, ignore);
  }

  auto compositeType = t->toCompositeType();
  return getFieldsGenericity(context, compositeType, ignore);
}

Type::Genericity getTypeGenericityIgnoring(Context* context, QualifiedType qt,
                                           std::set<const Type*>& ignore) {
   Type::Genericity g = qt.genericity();
   if (g == Type::MAYBE_GENERIC && qt.type() != nullptr ) {
     return resolution::getTypeGenericityIgnoring(context, qt.type(), ignore);
   }

   // otherwise return whatever we computed
   return g;
}

Type::Genericity getTypeGenericity(Context* context, const Type* t) {
  std::set<const Type*> ignore;
  return getTypeGenericityIgnoring(context, t, ignore);
}

Type::Genericity getTypeGenericity(Context* context, QualifiedType qt) {
  std::set<const Type*> ignore;
  return getTypeGenericityIgnoring(context, qt, ignore);
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
      Type::Genericity g = getTypeGenericity(context, t);
      if (g == Type::GENERIC) { // and not GENERIC_WITH_DEFAULTS
        formalType = QualifiedType(QualifiedType::TYPE, t);
        return true;
      }
    }
  }

  // otherwise it does not need to go into the type constructor
  return false;
}

static const TypedFnSignature* const&
typeConstructorInitialQuery(Context* context, const Type* t)
{
  QUERY_BEGIN(typeConstructorInitialQuery, context, t);

  const TypedFnSignature* result = nullptr;

  ID id;
  UniqueString name;
  std::vector<UntypedFnSignature::FormalDetail> formals;
  std::vector<types::QualifiedType> formalTypes;
  auto idTag = uast::asttags::AST_TAG_UNKNOWN;

  if (auto ct = t->getCompositeType()) {
    id = ct->id();
    name = ct->name();

    // attempt to resolve the fields
    bool useGenericFormalDefaults = false;
    const ResolvedFields& f = fieldsForTypeDecl(context, ct,
                                                useGenericFormalDefaults);

    // find the generic fields from the type and add
    // these as type constructor arguments.
    int nFields = f.numFields();
    for (int i = 0; i < nFields; i++) {
      auto declId = f.fieldDeclId(i);
      auto declAst = parsing::idToAst(context, declId);
      assert(declAst);
      const Decl* fieldDecl = declAst->toDecl();
      assert(fieldDecl);
      QualifiedType fieldType = f.fieldType(i);
      QualifiedType formalType;
      if (shouldIncludeFieldInTypeConstructor(context, fieldDecl, fieldType,
                                              formalType)) {

        auto d = UntypedFnSignature::FormalDetail(f.fieldName(i),
                                                  f.fieldHasDefaultValue(i),
                                                  fieldDecl);
        formals.push_back(d);
        // formalType should have been set above
        assert(formalType.kind() != QualifiedType::UNKNOWN);
        formalTypes.push_back(formalType);
      }
    }

    if (t->isBasicClassType() || t->isClassType()) {
      idTag = uast::asttags::Class;
    } else if (t->isRecordType()) {
      idTag = uast::asttags::Record;
    }
  } else {
    assert(false && "case not handled");
  }

  auto untyped = UntypedFnSignature::get(context,
                                         id, name,
                                         /* isMethod */ false,
                                         /* isTypeConstructor */ true,
                                         /* isCompilerGenerated */ true,
                                         idTag,
                                         Function::PROC,
                                         std::move(formals),
                                         /* whereClause */ nullptr);

  result = TypedFnSignature::get(context,
                                 untyped,
                                 std::move(formalTypes),
                                 TypedFnSignature::WHERE_NONE,
                                 /* needsInstantiation */ true,
                                 /* instantiatedFrom */ nullptr,
                                 /* parentFn */ nullptr,
                                 /* formalsInstantiated */ Bitmap());

  return QUERY_END(result);
}

const TypedFnSignature* typeConstructorInitial(Context* context,
                                               const types::Type* t) {
  return typeConstructorInitialQuery(context, t);
}

QualifiedType getInstantiationType(Context* context,
                                   QualifiedType actualType,
                                   QualifiedType formalType) {

  // The formal is generic but the actual might require a coercion
  // on the way to it. In that event, instantiate the formal type
  // using the type that the actual will coerce to.

  // E.g. a MyClass actual passed to an x:borrowed? formal
  // should instantiate with MyClass?

  const Type* actualT = actualType.type();
  const Type* formalT = formalType.type();

  assert(actualT != nullptr);
  assert(formalT != nullptr);

  // this function should only be called when instantiation is required
  assert(canPass(context, actualType, formalType).passes());
  assert(canPass(context, actualType, formalType).instantiates());

  if (auto actualCt = actualT->toClassType()) {
    // handle decorated class passed to decorated class
    if (auto formalCt = formalT->toClassType()) {
      // which decorator to use?
      auto dec = formalCt->decorator().combine(actualCt->decorator());

      // which manager to use?
      const Type* manager = nullptr;
      if (dec.isManaged()) {
        // there aren't implicit conversions from managed -> managed,
        // so we can always use the actual's manager if the combined
        // decorator indicates management.
        assert(actualCt->decorator().isManaged() && actualCt->manager());
        manager = actualCt->manager();
      }

      // which BasicClassType to use?
      const BasicClassType* bct = formalCt->basicClassType();
      auto g = getTypeGenericity(context, bct);
      if (g != Type::CONCRETE) {
        assert(false && "not implemented yet");
      }

      // now construct the ClassType
      auto ct = ClassType::get(context, bct, manager, dec);
      return QualifiedType(formalType.kind(), ct);
    }

    // handle decorated class passed to special built-in type
    auto classBuiltinTypeDec = ClassTypeDecorator::GENERIC;
    bool foundClassyBuiltinType = true;

    if (formalT->isAnyBorrowedNilableType()) {
      classBuiltinTypeDec = ClassTypeDecorator::BORROWED_NILABLE;
    } else if (formalT->isAnyBorrowedNonNilableType()) {
      classBuiltinTypeDec = ClassTypeDecorator::BORROWED_NONNIL;
    } else if (formalT->isAnyBorrowedType()) {
      classBuiltinTypeDec = ClassTypeDecorator::BORROWED;
    } else if (formalT->isAnyManagementAnyNilableType()) {
      classBuiltinTypeDec = ClassTypeDecorator::GENERIC;
    } else if (formalT->isAnyManagementNilableType()) {
      classBuiltinTypeDec = ClassTypeDecorator::GENERIC_NILABLE;
    } else if (formalT->isAnyManagementNonNilableType()) {
      classBuiltinTypeDec = ClassTypeDecorator::GENERIC_NONNIL;
    } else if (formalT->isAnyOwnedType() &&
               actualCt->decorator().isManaged() &&
               actualCt->manager()->isAnyOwnedType()) {
      classBuiltinTypeDec = ClassTypeDecorator::MANAGED;
    } else if (formalT->isAnySharedType() &&
               actualCt->decorator().isManaged() &&
               actualCt->manager()->isAnySharedType()) {
      classBuiltinTypeDec = ClassTypeDecorator::MANAGED;
    } else if (formalT->isAnyUnmanagedNilableType()) {
      classBuiltinTypeDec = ClassTypeDecorator::UNMANAGED_NILABLE;
    } else if (formalT->isAnyUnmanagedNonNilableType()) {
      classBuiltinTypeDec = ClassTypeDecorator::UNMANAGED_NONNIL;
    } else if (formalT->isAnyUnmanagedType()) {
      classBuiltinTypeDec = ClassTypeDecorator::UNMANAGED;
    } else {
      foundClassyBuiltinType = false;
    }

    if (foundClassyBuiltinType) {
      // which basic class type?
      const BasicClassType* bct = actualCt->basicClassType();

      // which decorator?
      auto formalDec = ClassTypeDecorator(classBuiltinTypeDec);
      auto dec = formalDec.combine(actualCt->decorator());

      // which manager?
      const Type* manager = nullptr;
      if (dec.isManaged())
        manager = actualCt->manager();

      // now construct the ClassType
      auto ct = ClassType::get(context, bct, manager, dec);
      return QualifiedType(formalType.kind(), ct);
    }
  } else if (actualT->isNilType()) {
    if (formalT->isAnyBorrowedNilableType() ||
        formalT->isAnyBorrowedType() ||
        formalT->isAnyManagementAnyNilableType() ||
        formalT->isAnyManagementNilableType() ||
        formalT->isAnyUnmanagedNilableType() ||
        formalT->isAnyUnmanagedType()) {
      return actualType; // instantiate with NilType for these cases
    }
  }

  // TODO: sync type -> value type?
  assert(false && "case not handled");
  return QualifiedType();
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
  const AstNode* ast = nullptr;
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
  Bitmap formalsInstantiated;
  int formalIdx = 0;

  for (const FormalActual& entry : faMap.byFormals()) {
    bool addSub = false;
    QualifiedType useType;

    // note: entry.actualType can have type()==nullptr and UNKNOWN.
    // in that case, resolver code should treat it as a hint to
    // use the default value. Unless the call used a ? argument.
    if (entry.actualType().kind() == QualifiedType::UNKNOWN &&
        entry.actualType().type() == nullptr) {
      if (call.hasQuestionArg()) {
        // don't add any substitution
      } else {
        // add a "use the default" hint substitution.
        addSub = true;
        useType = entry.actualType();
      }
    } else {
      auto got = canPass(context, entry.actualType(), entry.formalType());
      assert(got.passes()); // should not get here otherwise
      if (got.instantiates()) {
        // add a substitution for a valid value
        if (!got.converts() && !got.promotes()) {
          // use the actual type since no conversion/promotion was needed
          addSub = true;
          useType = entry.actualType();
        } else {
          // get instantiation type
          addSub = true;
          useType = getInstantiationType(context,
                                         entry.actualType(),
                                         entry.formalType());
        }
      }
    }

    // add the substitution if we identified that we need to
    if (addSub) {
      // add it to the substitutions map
      substitutions.insert({entry.formal()->id(), useType});
      // note that a substitution was used here
      if ((size_t) formalIdx >= formalsInstantiated.size()) {
        formalsInstantiated.resize(sig->numFormals());
      }
      formalsInstantiated.setBit(formalIdx, true);
    }

    formalIdx++;
  }

  // use the existing signature if there were no substitutions
  if (substitutions.size() == 0) {
    return sig;
  }

  std::vector<types::QualifiedType> formalTypes;
  bool needsInstantiation = false;
  TypedFnSignature::WhereClauseResult where = TypedFnSignature::WHERE_NONE;

  if (fn != nullptr) {
    ResolutionResultByPostorderID r;
    auto visitor = Resolver::instantiatedSignatureResolver(context, fn,
                                                           substitutions,
                                                           poiScope, r);
    // visit the formals
    for (auto formal : fn->formals()) {
      formal->traverse(visitor);
    }
    // visit the where clause
    if (auto whereClause = fn->whereClause()) {
      whereClause->traverse(visitor);
    }
    // do not visit the return type or function body

    auto tmp = getFormalTypes(fn, r);
    formalTypes.swap(tmp);
    needsInstantiation = anyFormalNeedsInstantiation(context, formalTypes,
                                                     untypedSignature,
                                                     &substitutions);
    where = whereClauseResult(context, fn, r, needsInstantiation);
  } else if (ad != nullptr) {
    // TODO: compute the class type

    // visit the fields
    ResolutionResultByPostorderID r;
    auto visitor =
      Resolver::instantiatedSignatureFieldsResolver(context, ad, substitutions,
                                                    poiScope, r);
    // visit the parent type
    if (auto cls = ad->toClass()) {
      if (auto parentClassExpr = cls->parentClass()) {
        parentClassExpr->traverse(visitor);
      }
    }

    // visit the field declarations
    for (auto child: ad->children()) {
      if (child->isVariable() ||
          child->isMultiDecl() ||
          child->isTupleDecl()) {
        child->traverse(visitor);
      }
    }

    // add formals according to the parent class type

    // now pull out the field types
    int nFormals = sig->numFormals();
    for (int i = 0; i < nFormals; i++) {
      const Decl* fieldDecl = untypedSignature->formalDecl(i);
      const ResolvedExpression& e = r.byAst(fieldDecl);
      QualifiedType fieldType = e.type();
      QualifiedType sigType = sig->formalType(i);

      // use the same kind as the old formal type but update the type, param
      // to reflect how instantiation occurred.
      formalTypes.push_back(QualifiedType(sigType.kind(),
                                          fieldType.type(),
                                          fieldType.param()));
    }
    needsInstantiation = anyFormalNeedsInstantiation(context, formalTypes,
                                                     untypedSignature,
                                                     &substitutions);
  } else {
    assert(false && "case not handled");
  }

  // now, construct a TypedFnSignature from the result
  auto result = TypedFnSignature::get(context,
                                      untypedSignature,
                                      std::move(formalTypes),
                                      where,
                                      needsInstantiation,
                                      /* instantiatedFrom */ sig,
                                      /* parentFn */ parentFnTyped,
                                      std::move(formalsInstantiated));
  return result;
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
  const AstNode* ast = parsing::idToAst(context, untypedSignature->id());
  const Function* fn = ast->toFunction();

  const PoiScope* poiScope = poiInfo.poiScope();

  PoiInfo resolvedPoiInfo;

  if (fn) {
    ResolutionResultByPostorderID resolutionById;
    auto visitor = Resolver::functionResolver(context, fn, poiScope, sig,
                                              resolutionById);

    // visit the function body
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

struct ReturnTypeInferrer {
  // input
  Context* context;
  Function::ReturnIntent returnIntent;
  const ResolutionResultByPostorderID& resolutionById;

  // output
  std::vector<QualifiedType> returnedTypes;

  ReturnTypeInferrer(Context* context,
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

  void checkReturn(const AstNode* inExpr, const QualifiedType& qt) {
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

  void noteVoidReturnType(const AstNode* inExpr) {
    auto voidType = QualifiedType(QualifiedType::CONST_VAR, VoidType::get(context));
    returnedTypes.push_back(voidType);

    checkReturn(inExpr, voidType);
  }
  void noteReturnType(const AstNode* expr, const AstNode* inExpr) {
    QualifiedType qt = resolutionById.byAst(expr).type();

    QualifiedType::Kind kind = qt.kind();
    const Type* type = qt.type();

    // Functions that return tuples need to return
    // a value tuple (for value returns and type returns)
    // or a reference to a value tuple (for ref/const ref returns)
    if (type && type->isTupleType()) {
      auto tt = type->toTupleType();
      type = tt->toValueTuple(context);
      qt = QualifiedType(kind, type);
    }

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
    if (const AstNode* expr = ret->value()) {
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

  bool enter(const AstNode* ast) {
    return true;
  }
  void exit(const AstNode* ast) {
  }
};

// For a class type construction, returns a BasicClassType
static const Type* const&
returnTypeForTypeCtorQuery(Context* context,
                           const TypedFnSignature* sig,
                           const PoiScope* poiScope) {
  QUERY_BEGIN(returnTypeForTypeCtorQuery, context, sig, poiScope);

  const UntypedFnSignature* untyped = sig->untyped();

  const Type* result = nullptr;

  // handle type construction
  const AggregateDecl* ad = nullptr;
  if (!untyped->id().isEmpty())
    if (auto ast = parsing::idToAst(context, untyped->id()))
      ad = ast->toAggregateDecl();

  if (ad) {
    // compute instantiatedFrom
    const CompositeType* instantiatedFrom = nullptr;
    if (sig->instantiatedFrom()) {
      const Type* t = initialTypeForTypeDecl(context, ad->id());
      assert(t);

      // ignore decorators etc for finding instantiatedFrom
      if (auto ct = t->toClassType())
        t = ct->basicClassType();

      instantiatedFrom = t->toCompositeType();
      assert(instantiatedFrom);
    }

    // compute the substitutions
    SubstitutionsMap subs;

    if (instantiatedFrom != nullptr) {
      int nFormals = sig->numFormals();
      for (int i = 0; i < nFormals; i++) {
        const Decl* formalDecl = untyped->formalDecl(i);
        const QualifiedType& formalType = sig->formalType(i);
        // Note that the formalDecl should already be a fieldDecl
        // based on typeConstructorInitialQuery.
        bool hasInitExpression = false;
        if (auto vd = formalDecl->toVarLikeDecl())
          if (vd->initExpression() != nullptr)
            hasInitExpression = true;

        if (formalType.type()->isAnyType() && !hasInitExpression) {
          // Ignore this substitution - easier to just leave it out
          // of the map entirely.
          // Note that we explicitly put a sub for AnyType for generics
          // with default, where the default is not used. E.g.
          //    record R { type t = int; }
          //    type RR = R(?);
          //    var x: RR;
          // is a compilation error because x has generic type.
          // In order to support that pattern, we need to be able to
          // represent that RR is a version of R where it's not behaving
          // as generic-with-default and substituting in AnyType does that.
        } else {
          subs.insert({formalDecl->id(), formalType});
        }
      }
    }

    // get a type using the substitutions
    const CompositeType* theType = helpGetTypeForDecl(context, ad,
                                                      subs,
                                                      poiScope,
                                                      instantiatedFrom);

    result = theType;

  } else {
    // built-in type construction should be handled
    // by resolveFnCallSpecialType and not reach this point.
    assert(false && "case not handled");
  }

  return QUERY_END(result);
}

static QualifiedType computeTypeOfField(Context* context,
                                        const Type* t,
                                        ID fieldId) {
  if (const CompositeType* ct = t->getCompositeType()) {
    // Figure out the parent MultiDecl / TupleDecl
    ID declId = parsing::idToContainingMultiDeclId(context, fieldId);

    // Resolve the type of that field (or MultiDecl/TupleDecl)
    const auto& fields = resolveFieldDecl(context, ct, declId,
                                          /*useGenericFormalDefaults*/ false);
    int n = fields.numFields();
    for (int i = 0; i < n; i++) {
      if (fields.fieldDeclId(i) == fieldId) {
        return fields.fieldType(i);
      }
    }
  }

  assert(false && "should not be reachable");
  return QualifiedType(QualifiedType::VAR, ErroneousType::get(context));
}

const QualifiedType& returnType(Context* context,
                                const TypedFnSignature* sig,
                                const PoiScope* poiScope) {
  QUERY_BEGIN(returnType, context, sig, poiScope);

  const UntypedFnSignature* untyped = sig->untyped();

  QualifiedType result;

  if (untyped->idIsFunction() && sig->needsInstantiation()) {
    // if it needs instantiation, we don't know the return type yet.
    result = QualifiedType(QualifiedType::UNKNOWN, UnknownType::get(context));
  } else if (untyped->idIsFunction()) {
    const AstNode* ast = parsing::idToAst(context, untyped->id());
    const Function* fn = ast->toFunction();
    assert(fn);
    if (const AstNode* retType = fn->returnType()) {
      // resolve the return type
      ResolutionResultByPostorderID resolutionById;
      auto visitor = Resolver::functionResolver(context, fn, poiScope, sig,
                                                resolutionById);
      retType->traverse(visitor);
      result = resolutionById.byAst(retType).type();
    } else {
      // resolve the function body
      const ResolvedFunction* rFn = resolveFunction(context, sig, poiScope);
      // infer the return type
      ReturnTypeInferrer visitor(context, *rFn);
      fn->body()->traverse(visitor);
      result = visitor.returnedType();
    }

    // Figure out the kind for the QualifiedType based on the return intent
    // Need to do this if the return type is declared.
    QualifiedType::Kind kind = (QualifiedType::Kind) fn->returnIntent();
    // adjust default / const return intent to 'var'
    if (kind == QualifiedType::DEFAULT_INTENT ||
        kind == QualifiedType::CONST_VAR) {
        kind = QualifiedType::VAR;
    }
    result = QualifiedType(kind, result.type(), result.param());

    // Functions that return tuples need to return
    // a value tuple (for value returns and type returns)
    // or a reference to a value tuple (for ref/const ref returns)
    if (result.type() && result.type()->isTupleType()) {
      auto tt = result.type()->toTupleType();
      auto vt = tt->toValueTuple(context);
      assert(tt == vt); // this should already be done in return type inference
      result = QualifiedType(kind, vt);
    }

  } else if (untyped->isTypeConstructor()) {
    const Type* t = returnTypeForTypeCtorQuery(context, sig, poiScope);

    // for a 'class C' declaration, the above query returns a BasicClassType,
    // but 'C' normally means a generic-management non-nil C
    // so adjust the result.
    if (untyped->idIsClass()) {
      auto bct = t->toBasicClassType();
      assert(bct);
      auto dec = ClassTypeDecorator(ClassTypeDecorator::GENERIC_NONNIL);
      t = ClassType::get(context, bct, /*manager*/ nullptr, dec);
    }

    result = QualifiedType(QualifiedType::TYPE, t);

  // if method call and the receiver points to a composite type definition,
  // then it's some sort of compiler-generated method
  } else if (untyped->isCompilerGenerated()) {
    if (untyped->name() == USTR("init")) {
      result = QualifiedType(QualifiedType::CONST_VAR,
                             VoidType::get(context));
    } else if (untyped->idIsField() && untyped->isMethod()) {
      // method accessor - compute the type of the field
      QualifiedType ft = computeTypeOfField(context,
                                            sig->formalType(0).type(),
                                            untyped->id());
      if (ft.isType() || ft.isParam()) {
        // return the type as-is (preserving param/type-ness)
        result = ft;
      } else if (ft.isConst()) {
        // return a const ref
        result = QualifiedType(QualifiedType::CONST_REF, ft.type());
      } else {
        // return a ref
        result = QualifiedType(QualifiedType::REF, ft.type());
      }
    } else {
      assert(false && "unhandled compiler-generated method");
    }
  } else {
    assert(false && "case not handled");
  }

  return QUERY_END(result);
}

static bool
isUntypedSignatureApplicable(Context* context,
                             const UntypedFnSignature* ufs,
                             const FormalActualMap& faMap,
                             const CallInfo& ci) {
  // First, check that the untyped properties allow a match:
  //  * number of arguments
  //  * names of arguments
  //  * method-ness
  //  * ref-ness

  if (!faMap.isValid()) {
    return false;
  }

  // TODO: more to check for method-ness?
  if (ci.isMethodCall() != ufs->isMethod()) {
    return false;
  }

  // TODO: reason failed

  return true;
}

// given a typed function signature, determine if it applies to a call
static bool
isInitialTypedSignatureApplicable(Context* context,
                                  const TypedFnSignature* tfs,
                                  const FormalActualMap& faMap,
                                  const CallInfo& ci) {
  if (!isUntypedSignatureApplicable(context, tfs->untyped(), faMap, ci)) {
    return false;
  }

  // Next, check that the types are compatible
  int formalIdx = 0;
  for (const FormalActual& entry : faMap.byFormals()) {
    const auto& actualType = entry.actualType();

    // note: entry.actualType can have type()==nullptr and UNKNOWN.
    // in that case, resolver code should treat it as a hint to
    // use the default value. Unless the call used a ? argument.
    if (entry.actualType().kind() == QualifiedType::UNKNOWN &&
        entry.actualType().type() == nullptr &&
        !ci.hasQuestionArg()) {
      // use the default value - no need to check it matches formal
    } else {
      const auto& formalType = tfs->formalType(formalIdx);
      auto got = canPass(context, actualType, formalType);
      if (!got.passes()) {
        return false;
      }
    }

    formalIdx++;
  }

  // check that the where clause applies
  auto whereResult = tfs->whereClauseResult();
  if (whereResult == TypedFnSignature::WHERE_FALSE) {
    return false;
  }

  return true;
}

// returns nullptr if the candidate is not applicable,
// or the result of typedSignatureInitial if it is.
static const TypedFnSignature*
doIsCandidateApplicableInitial(Context* context,
                               const ID& candidateId,
                               const CallInfo& ci) {
  AstTag tag = asttags::AST_TAG_UNKNOWN;

  if (!candidateId.isEmpty()) {
    tag = parsing::idToTag(context, candidateId);
    // could be a function, type for type construction, field
    assert(isFunction(tag) || isTypeDecl(tag) || isVariable(tag));
  } else {
    assert(false && "case not handled");
    return nullptr;
  }

  if (isTypeDecl(tag)) {
    // calling a type - i.e. type construction
    const Type* t = initialTypeForTypeDecl(context, candidateId);
    return typeConstructorInitial(context, t);
  }

  if (isVariable(tag) &&
      ci.isParenless() && ci.isMethodCall() && ci.numActuals() == 1) {
    // calling a field accessor
    auto ct = ci.actual(0).type().type()->getCompositeType();
    assert(ct);
    assert(isNameOfField(context, ci.name(), ct));
    return fieldAccessor(context, ct, ci.name());
  }

  assert(isFunction(tag) && "only fn case handled here");
  auto ufs = UntypedFnSignature::get(context, candidateId);
  auto faMap = FormalActualMap(ufs, ci);
  auto ret = typedSignatureInitial(context, ufs);

  if (!isInitialTypedSignatureApplicable(context, ret, faMap, ci)) {
    return nullptr;
  }

  return ret;
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
    const QualifiedType& actualType = call.actual(i).type();
    const QualifiedType& formalType = instantiated->formalType(i);
    auto got = canPass(context, actualType, formalType);
    if (!got.passes())
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

// call can be nullptr; in that event, the CallInfo will be consulted
// and it will search for something with 'ci.name()'.
static std::vector<BorrowedIdsWithName>
lookupCalledExpr(Context* context,
                 const Scope* scope,
                 const Call* call,
                 const CallInfo& ci,
                 std::unordered_set<const Scope*>& visited) {
  std::vector<BorrowedIdsWithName> ret;

  // TODO: it would be nice if the caller could compute the name instead
  const bool doLookupByName = (call == nullptr || call->isOpCall());
  const LookupConfig config = LOOKUP_DECLS |
                              LOOKUP_IMPORT_AND_USE |
                              LOOKUP_PARENTS;
  #ifndef NDEBUG
  if (call != nullptr) {
    if (auto op = call->toOpCall()) {
      assert(op->op() == ci.name());
    }
  }
  #endif

  // For method calls, perform an initial lookup starting at the scope for
  // the definition of the receiver type, if it can be found.
  if (ci.isMethodCall()) {
    assert(ci.numActuals() >= 1);

    auto& receiverQualType = ci.actual(0).type();
    const Scope* scopeForReceiverType = nullptr;

    // Try to fetch the scope of the receiver type's definition.
    if (auto receiverType = receiverQualType.type()) {
      if (auto compType = receiverType->getCompositeType()) {
        scopeForReceiverType = scopeForId(context, compType->id());
      }
    }

    // Lookup by name, as lookup for dot-exprs does not handle method calls.
    if (scopeForReceiverType && !visited.count(scopeForReceiverType)) {
      auto vec = lookupNameInScopeWithSet(context, scopeForReceiverType,
                                          ci.name(),
                                          config,
                                          visited);
      ret.swap(vec);
    }
  }

  std::vector<BorrowedIdsWithName> vec;
  if (doLookupByName) {
    vec = lookupNameInScopeWithSet(context, scope, ci.name(), config,
                                   visited);
  } else if (const AstNode* called = call->calledExpression()) {
    vec = lookupInScopeWithSet(context, scope, called, config,
                               visited);
  }

  if (ret.size() > 0) {
    ret.insert(ret.end(), vec.begin(), vec.end());
  } else {
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

  if (signature->instantiatedFrom() == nullptr) {
    // if it's not an instantiation, no need to gather POIs
    return;
  }

  if (signature->needsInstantiation()) {
    // if it needs instantiation, it's not time to gather POIs yet
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
                                       const AstNode* astForErr,
                                       const CallInfo& ci) {
  UniqueString name = ci.name();

  if (ci.hasQuestionArg()) {
    if (ci.numActuals() != 0) {
      context->error(astForErr, "invalid class type construction");
      return ErroneousType::get(context);
    } else if (name == USTR("owned")) {
      return AnyOwnedType::get(context);
    } else if (name == USTR("shared")) {
      return AnySharedType::get(context);
    } else if (name == USTR("unmanaged")) {
      return AnyUnmanagedType::get(context);
    } else if (name == USTR("borrowed")) {
      return AnyBorrowedType::get(context);
    } else {
      // case not handled in here
      return nullptr;
    }
  }

  ClassTypeDecorator::ClassTypeDecoratorEnum de;
  const Type* manager = nullptr;

  if (name == USTR("owned")) {
    de = ClassTypeDecorator::MANAGED;
    manager = AnyOwnedType::get(context);
  } else if (name == USTR("shared")) {
    de = ClassTypeDecorator::MANAGED;
    manager = AnySharedType::get(context);
  } else if (name == USTR("unmanaged")) {
    de = ClassTypeDecorator::UNMANAGED;
    manager = nullptr;
  } else if (name == USTR("borrowed")) {
    de = ClassTypeDecorator::BORROWED;
    manager = nullptr;
  } else {
    // case not handled in here
    return nullptr;
  }

  auto d = ClassTypeDecorator(de);

  const Type* t = nullptr;
  if (ci.numActuals() > 0)
    t = ci.actual(0).type().type();

  if (t == nullptr || !(t->isBasicClassType() || t->isClassType())) {
    context->error(astForErr, "invalid class type construction");
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
                                  const AstNode* astForErr,
                                  const CallInfo& ci) {
  UniqueString name = ci.name();

  if (name == USTR("int") || name == USTR("uint") || name == USTR("bool") ||
      name == USTR("real") || name == USTR("imag") || name == USTR("complex")) {

    // Should we compute the generic version of the type (e.g. int(?))
    bool useGenericType = false;

    // There should be 0 or 1 actuals depending on if it is ?
    if (ci.hasQuestionArg()) {
      // handle int(?)
      if (ci.numActuals() != 0) {
        context->error(astForErr, "invalid numeric type construction");
        return ErroneousType::get(context);
      }
      useGenericType = true;
    } else {
      // handle int(?t) or int(16)
      if (ci.numActuals() != 1) {
        context->error(astForErr, "invalid numeric type construction");
        return ErroneousType::get(context);
      }

      QualifiedType qt = ci.actual(0).type();
      if (qt.type() && qt.type()->isAnyType()) {
        useGenericType = true;
      } else if (qt.isParam() && qt.param() == nullptr)  {
        useGenericType = true;
      }
    }

    if (useGenericType) {
      if (name == USTR("int")) {
        return AnyIntType::get(context);
      } else if (name == USTR("uint")) {
        return AnyUintType::get(context);
      } else if (name == USTR("bool")) {
        return AnyBoolType::get(context);
      } else if (name == USTR("real")) {
        return AnyRealType::get(context);
      } else if (name == USTR("imag")) {
        return AnyImagType::get(context);
      } else if (name == USTR("complex")) {
        return AnyComplexType::get(context);
      } else {
        assert(false && "should not be reachable");
        return nullptr;
      }
    }

    QualifiedType qt;
    if (ci.numActuals() > 0)
      qt = ci.actual(0).type();

    const Type* t = qt.type();
    if (t == nullptr) {
      // Details not yet known so return UnknownType
      return UnknownType::get(context);
    }
    if (t->isUnknownType() || t->isErroneousType()) {
      // Just propagate the Unknown / Erroneous type
      // without raising any errors
      return t;
    }
    if (qt.param() == nullptr) {
      // Details not yet known so return UnknownType
      return UnknownType::get(context);
    }

    if (!t->isIntType() || !qt.param()->isIntParam()) {
      // raise an error b/c of type mismatch
      context->error(astForErr, "invalid numeric type construction");
      return ErroneousType::get(context);
    }

    const Type* ret = nullptr;
    auto ip = qt.param()->toIntParam();
    auto value = ip->value();
    if (0 <= value && value <= 128) {
      ret = PrimitiveType::getWithNameAndWidth(context, name, value);
    }

    if (ret == nullptr) {
      context->error(astForErr, "invalid numeric type construction");
      return ErroneousType::get(context);
    }

    return ret;
  }

  return nullptr;
}

// Resolving compiler-supported type-returning patterns
// 'call' and 'inPoiScope' are used for the location for error reporting.
static const Type* resolveFnCallSpecialType(Context* context,
                                            const AstNode* astForErr,
                                            const CallInfo& ci) {

  if (ci.name() == USTR("?")) {
    if (ci.numActuals() > 0) {
      if (const Type* t = ci.actual(0).type().type()) {
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

  if (auto t = getManagedClassType(context, astForErr, ci)) {
    return t;
  }

  if (auto t = getNumericType(context, astForErr, ci)) {
    return t;
  }

  return nullptr;
}


// Resolving calls for certain compiler-supported patterns
// without requiring module implementations exist at all.
static bool resolveFnCallSpecial(Context* context,
                                 const AstNode* astForErr,
                                 const CallInfo& ci,
                                 QualifiedType& exprTypeOut) {
  // TODO: type comparisons
  // TODO: cast
  // TODO: .borrow()
  // TODO: chpl__coerceCopy

  if (const Type* t = resolveFnCallSpecialType(context, astForErr, ci)) {
    exprTypeOut = QualifiedType(QualifiedType::TYPE, t);
    return true;
  }

  if (ci.name() == USTR("isCoercible")) {
    if (ci.numActuals() != 2) {
      context->error(astForErr, "bad call to %s", ci.name().c_str());
      exprTypeOut = QualifiedType(QualifiedType::UNKNOWN,
                                  ErroneousType::get(context));
      return true;
    }
    auto got = canPass(context, ci.actual(0).type(), ci.actual(1).type());
    bool result = got.passes();
    exprTypeOut = QualifiedType(QualifiedType::PARAM, BoolType::get(context, 0),
                                BoolParam::get(context, result));
    return true;
  }

  return false;
}

static MostSpecificCandidates
resolveFnCallForTypeCtor(Context* context,
                         const CallInfo& ci,
                         const Scope* inScope,
                         const PoiScope* inPoiScope,
                         PoiInfo& poiInfo) {

  std::vector<const TypedFnSignature*> initialCandidates;
  std::vector<const TypedFnSignature*> candidates;

  assert(ci.calledType().type() != nullptr);
  assert(!ci.calledType().type()->isUnknownType());

  auto initial = typeConstructorInitial(context, ci.calledType().type());
  initialCandidates.push_back(initial);

  // TODO: do something for partial instantiation

  filterCandidatesInstantiating(context,
                                initialCandidates,
                                ci,
                                inScope,
                                inPoiScope,
                                candidates);

  // find most specific candidates / disambiguate
  // Note: at present there can only be one candidate here
  MostSpecificCandidates mostSpecific = findMostSpecificCandidates(context,
                                                                   candidates,
                                                                   ci,
                                                                   inScope,
                                                                   inPoiScope);

  return mostSpecific;
}

using CandidatesVec = std::vector<const TypedFnSignature*>;

// returns true if one or more TypedFnSignature for compiler-generated
// methods were added to the candidates vector, and false otherwise
static bool
considerCompilerGeneratedCandidates(Context* context,
                                    const CallInfo& ci,
                                    const Scope* inScope,
                                    const PoiScope* inPoiScope,
                                    PoiInfo& poiInfo,
                                    CandidatesVec& candidates) {

  // only consider compiler-generated methods, for now
  if (!ci.isMethodCall()) return false;

  // fetch the receiver type info
  assert(ci.numActuals() >= 1);
  auto& receiver = ci.actual(0);
  auto receiverType = receiver.type().type();

  // if not compiler-generated, then nothing to do
  if (!needCompilerGeneratedMethod(context, receiverType, ci.name(),
                                   ci.isParenless()))
    return false;

  // get the compiler-generated function, may be generic
  auto tfs = getCompilerGeneratedMethod(context, receiverType, ci.name(),
                                        ci.isParenless());
  assert(tfs);

  // check if the initial signature matches
  auto faMap = FormalActualMap(tfs->untyped(), ci);
  if (!isInitialTypedSignatureApplicable(context, tfs, faMap, ci)) {
    return false;
  }

  // OK, already concrete, store and return
  if (!tfs->needsInstantiation()) {
    candidates.push_back(tfs);
    return true;
  }

  // need to instantiate before storing
  auto poi = pointOfInstantiationScope(context, inScope, inPoiScope);
  auto instantiated = doIsCandidateApplicableInstantiating(context,
                                                           tfs,
                                                           ci,
                                                           poi);
  assert(instantiated->untyped()->idIsFunction());
  assert(instantiated->instantiatedFrom());

  auto instantiationScope = pointOfInstantiationScope(context, inScope,
                                                      inPoiScope);
  poiInfo.setPoiScope(instantiationScope);

  accumulatePoisUsedByResolvingBody(context, instantiated,
                                    instantiationScope,
                                    poiInfo);

  candidates.push_back(instantiated);

  return true;
}

// call can be nullptr. in that event, ci.name() will be used
// to find the call with that name.
static MostSpecificCandidates
resolveFnCallFilterAndFindMostSpecific(Context* context,
                                       const Call* call,
                                       const CallInfo& ci,
                                       const Scope* inScope,
                                       const PoiScope* inPoiScope,
                                       PoiInfo& poiInfo) {
  // search for candidates at each POI until we have found a candidate
  CandidatesVec candidates;
  size_t firstPoiCandidate = 0;
  std::unordered_set<const Scope*> visited;

  // inject compiler-generated candidates in a manner similar to below
  considerCompilerGeneratedCandidates(context, ci, inScope, inPoiScope,
                                      poiInfo,
                                      candidates);

  // next, look for candidates without using POI.
  {
    // compute the potential functions that it could resolve to
    auto v = lookupCalledExpr(context, inScope, call, ci, visited);

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
    auto v = lookupCalledExpr(context, curPoi->inScope(), call, ci,
                              visited);

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
                                                                   ci,
                                                                   inScope,
                                                                   inPoiScope);

  // note any most specific candidates from POI in poiInfo.
  {
    size_t n = candidates.size();
    for (size_t i = firstPoiCandidate; i < n; i++) {
      for (const TypedFnSignature* candidate : mostSpecific) {
        if (candidate == candidates[i]) {
          poiInfo.addIds(call->id(), candidate->id());
        }
      }
    }
  }

  return mostSpecific;
}


// call can be nullptr. in that event ci.name() will be used to find
// what is called.
static
CallResolutionResult resolveFnCall(Context* context,
                                   const Call* call,
                                   const CallInfo& ci,
                                   const Scope* inScope,
                                   const PoiScope* inPoiScope) {

  PoiInfo poiInfo;
  MostSpecificCandidates mostSpecific;

  if (ci.calledType().kind() == QualifiedType::TYPE) {
    // handle invocation of a type constructor from a type
    // (note that we might have the type through a type alias)
    mostSpecific = resolveFnCallForTypeCtor(context, ci,
                                            inScope, inPoiScope,
                                            poiInfo);
  } else {
    // * search for candidates at each POI until we have found a candidate
    // * filter and instantiate
    // * disambiguate
    // * note any most specific candidates from POI in poiInfo.
    mostSpecific = resolveFnCallFilterAndFindMostSpecific(context, call, ci,
                                                          inScope, inPoiScope,
                                                          poiInfo);
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

    for (const TypedFnSignature* candidate : mostSpecific) {
      if (candidate != nullptr) {
        if (candidate->untyped()->idIsFunction()) {
          // note: following call returns early if candidate not instantiated
          accumulatePoisUsedByResolvingBody(context, candidate,
                                            instantiationPoiScope, poiInfo);
        }
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
CallResolutionResult resolveTupleExpr(Context* context,
                                      const Tuple* tuple,
                                      const CallInfo& ci,
                                      const Scope* inScope,
                                      const PoiScope* inPoiScope) {
  // resolve the tuple type from a tuple expression

  // per spec:
  // Tuple expressions are a form of referential tuple.
  // Like most other referential tuples, tuple expressions capture each
  // element based on the default argument intent of the element’s type.

  // check if the elements are all type or all value
  bool anyUnknown = false;
  bool allType = true;
  bool allValue = true;
  for (auto actual : ci.actuals()) {
    QualifiedType q = actual.type();
    const Type* t = q.type();
    if (t == nullptr || t->isUnknownType())
      anyUnknown = true;
    else if (q.kind() == QualifiedType::TYPE)
      allValue = false;
    else
      allType = false;
  }

  // if any argument is Unknown / null, return Unknown
  if (anyUnknown) {
    auto unk = UnknownType::get(context);
    return CallResolutionResult(QualifiedType(QualifiedType::CONST_VAR, unk));
  }

  // if there is a mix of value and type elements, error
  if (allType == false && allValue == false) {
    context->error(tuple, "Mix of value and type tuple elements in tuple expr");
    auto e = ErroneousType::get(context);
    return CallResolutionResult(QualifiedType(QualifiedType::CONST_VAR, e));
  }

  // otherwise, construct the tuple type
  std::vector<const Type*> eltTypes;

  QualifiedType::Kind kind = QualifiedType::UNKNOWN;
  if (allValue)
    kind = QualifiedType::CONST_VAR;
  else if (allType)
    kind = QualifiedType::TYPE;

  for (auto actual : ci.actuals()) {
    QualifiedType q = actual.type();
    const Type* t = q.type();
    eltTypes.push_back(t);
  }

  const TupleType* t = nullptr;
  if (allType)
    t = TupleType::getValueTuple(context, std::move(eltTypes));
  else
    t = TupleType::getReferentialTuple(context, std::move(eltTypes));

  return CallResolutionResult(QualifiedType(kind, t));
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
  } else if (auto tuple = call->toTuple()) {
    return resolveTupleExpr(context, tuple, ci, inScope, inPoiScope);
  }

  assert(false && "should not be reached");
  MostSpecificCandidates emptyCandidates;
  QualifiedType emptyType;
  PoiInfo emptyPoi;
  return CallResolutionResult(emptyCandidates, emptyType, emptyPoi);
}

CallResolutionResult resolveGeneratedCall(Context* context,
                                          const AstNode* astForErr,
                                          const CallInfo& ci,
                                          const Scope* inScope,
                                          const PoiScope* inPoiScope) {
  // see if the call is handled directly by the compiler
  QualifiedType tmpRetType;
  if (resolveFnCallSpecial(context, astForErr, ci, tmpRetType)) {
    return CallResolutionResult(std::move(tmpRetType));
  }
  // otherwise do regular call resolution
  return resolveFnCall(context, /* call */ nullptr, ci, inScope, inPoiScope);
}

static bool helpFieldNameCheck(const AstNode* ast,
                               UniqueString name) {
  if (auto var = ast->toVarLikeDecl()) {
    return var->name() == name;
  } else if (auto mult = ast->toMultiDecl()) {
    for (auto decl : mult->decls()) {
      bool found = helpFieldNameCheck(decl, name);
      if (found) {
        return true;
      }
    }
  } else if (auto tup = ast->toTupleDecl()) {
    for (auto decl : tup->decls()) {
      bool found = helpFieldNameCheck(decl, name);
      if (found) {
        return true;
      }
    }
  }
  return false;
}

static const bool&
isNameOfFieldQuery(Context* context,
                   UniqueString name, const CompositeType* ct) {
  QUERY_BEGIN(isNameOfFieldQuery, context, name, ct);

  bool result = false;
  auto ast = parsing::idToAst(context, ct->id());
  assert(ast && ast->isAggregateDecl());
  auto ad = ast->toAggregateDecl();

  for (auto child: ad->children()) {
    // Ignore everything other than VarLikeDecl, MultiDecl, TupleDecl
    if (child->isVarLikeDecl() ||
        child->isMultiDecl() ||
        child->isTupleDecl()) {
      bool found = helpFieldNameCheck(child, name);
      if (found) {
        result = true;
        break;
      }
    }
  }

  return QUERY_END(result);
}

bool isNameOfField(Context* context, UniqueString name, const Type* t) {
  const CompositeType* ct = t->getCompositeType();

  if (ct == nullptr) {
    return false;
  }

  if (auto bct = ct->toBasicClassType()) {
    if (bct->isObjectType()) {
      return false;
    }
  }


  return isNameOfFieldQuery(context, name, ct);
}



} // end namespace resolution
} // end namespace chpl
