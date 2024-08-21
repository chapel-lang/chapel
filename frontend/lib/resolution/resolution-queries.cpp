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

#include "chpl/resolution/resolution-queries.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/framework/compiler-configuration.h"
#include "chpl/framework/ErrorMessage.h"
#include "chpl/framework/UniqueString.h"
#include "chpl/framework/global-strings.h"
#include "chpl/framework/query-impl.h"
#include "chpl/resolution/can-pass.h"
#include "chpl/resolution/disambiguation.h"
#include "chpl/resolution/intents.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"
#include "chpl/util/hash.h"

#include "Resolver.h"
#include "call-init-deinit.h"
#include "default-functions.h"
#include "maybe-const.h"
#include "prims.h"
#include "return-type-inference.h"
#include "signature-checks.h"
#include "try-catch-analysis.h"

#include <cstdio>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include <iterator>

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

const ResolutionResultByPostorderID& resolveModuleStmt(Context* context,
                                                       ID id) {
  QUERY_BEGIN(resolveModuleStmt, context, id);
  QUERY_REGISTER_TRACER(
    return std::make_pair(std::get<0>(args), "resolving this module statement");
  );

  CHPL_ASSERT(id.postOrderId() >= 0);

  ResolutionResultByPostorderID result;

  ID moduleId = parsing::idToParentId(context, id);
  auto moduleAst = parsing::idToAst(context, moduleId);
  if (const Module* mod = moduleAst->toModule()) {
    // Resolve just the requested statement
    auto modStmt = parsing::idToAst(context, id);
    auto visitor = Resolver::createForModuleStmt(context, mod, modStmt, result);
    modStmt->traverse(visitor);

    if (auto rec = context->recoverFromSelfRecursion()) {
      CHPL_REPORT(context, RecursionModuleStmt, modStmt, mod, std::move(*rec));
    }
  }

  return QUERY_END(result);
}

static const ResolutionResultByPostorderID&
scopeResolveModuleStmt(Context* context, ID id) {
  QUERY_BEGIN(scopeResolveModuleStmt, context, id);

  CHPL_ASSERT(id.postOrderId() >= 0);

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
    auto visitor =
      Resolver::createForScopeResolvingModuleStmt(context, mod, modStmt,
                                                  result);
    modStmt->traverse(visitor);
  }

  return QUERY_END(result);
}

static void updateTypeForModuleLevelSplitInit(Context* context, ID id,
                                              ResolvedExpression& lhs,
                                              const ResolvedExpression& rhs) {
  const QualifiedType lhsType = lhs.type();
  const QualifiedType rhsType = rhs.type();

  if (!lhsType.needsSplitInitTypeInfo(context)) return;

  const Param* p = rhsType.param();
  if (lhsType.kind() != QualifiedType::PARAM) {
    p = nullptr;
  }
  const auto useType = QualifiedType(lhsType.kind(), rhsType.type(), p);

  lhs.setType(useType);
}

const ResolutionResultByPostorderID& resolveModule(Context* context, ID id) {
  QUERY_BEGIN(resolveModule, context, id);

  const AstNode* ast = parsing::idToAst(context, id);
  CHPL_ASSERT(ast != nullptr);

  ResolutionResultByPostorderID result;

  if (ast != nullptr) {
    if (const Module* mod = ast->toModule()) {
      // check for multiply-defined symbols within the module
      auto modScope = scopeForId(context, mod->id());
      emitMultipleDefinedSymbolErrors(context, modScope);

      auto r = Resolver::createForModuleStmt(context, mod, nullptr, result);

      for (auto child: mod->children()) {
        if (child->isComment() ||
            child->isTypeDecl() ||
            child->isFunction() ||
            child->isModule() ||
            child->isExternBlock()) {
            // Resolve use/import to find deprecation/unstable warnings.
            // child->isUse() ||
            // child->isImport()) {
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
            ResolvedExpression& re = result.byId(exprId);
            if (auto reToCopy = resolved.byIdOrNull(exprId)) {
              re = *reToCopy;
            }
          }
          // copy results for split-inited vars
          for (int i = 0; i < firstId; i++) {
            ID exprId(stmtId.symbolPath(), i, 0);
            ResolvedExpression& re = result.byId(exprId);
            if (auto reToCopy = resolved.byIdOrNull(exprId)) {
              updateTypeForModuleLevelSplitInit(context, exprId, re, *reToCopy);
            }
          }
        }
      }
      checkThrows(context, result, mod);
      callInitDeinit(r);
    }
  }

  return QUERY_END(result);
}

const ResolutionResultByPostorderID&
scopeResolveModule(Context* context, ID id) {
  QUERY_BEGIN(scopeResolveModule, context, id);

  const AstNode* ast = parsing::idToAst(context, id);
  CHPL_ASSERT(ast != nullptr);

  ResolutionResultByPostorderID result;

  if (ast != nullptr) {
    if (const Module* mod = ast->toModule()) {
      // check for multiply-defined symbols within the module
      auto modScope = scopeForId(context, mod->id());
      emitMultipleDefinedSymbolErrors(context, modScope);

      result.setupForSymbol(mod);
      for (auto child: mod->children()) {
        if (child->isComment() ||
            child->isTypeDecl() ||
            child->isFunction() ||
            child->isModule() ||
            child->isInterface() ||
            child->isExternBlock()) {
            // Resolve use/import to find deprecation/unstable warnings.
            // child->isUse() ||
            // child->isImport()) {
          // ignore this statement since it is not relevant to
          // the resolution of module initializers and module-level
          // variables.
        } else {
          ID stmtId = child->id();
          // resolve the statement
          const ResolutionResultByPostorderID& resolved =
            scopeResolveModuleStmt(context, stmtId);

          // copy results for children and the node itself
          int firstId = stmtId.postOrderId() - stmtId.numContainedChildren();
          int lastId = firstId + stmtId.numContainedChildren();
          for (int i = firstId; i <= lastId; i++) {
            ID exprId(stmtId.symbolPath(), i, 0);
            ResolvedExpression& re = result.byId(exprId);
            if (auto reToCopy = resolved.byIdOrNull(exprId)) {
              re = *reToCopy;
            }
          }
        }
      }
    }
  }

  return QUERY_END(result);

}

const QualifiedType& typeForModuleLevelSymbol(Context* context, ID id,
                                              bool isCurrentModule) {
  QUERY_BEGIN(typeForModuleLevelSymbol, context, id, isCurrentModule);

  QualifiedType result;

  int postOrderId = id.postOrderId();
  if (postOrderId >= 0) {
    // 'typeForModuleLevelSymbol' can be called with an 'id' corresponding to a
    // variable declared as part of a MultiDecl or TupleDecl. Using that 'id'
    // with 'resolveModuleStmt' will return a bogus result because that 'id'
    // is not at the statement level.
    auto stmtId = parsing::idToContainingMultiDeclId(context, id);
    const auto& resolvedStmt = resolveModuleStmt(context, stmtId);
    if (resolvedStmt.hasId(id)) {
      result = resolvedStmt.byId(id).type();
      if (result.needsSplitInitTypeInfo(context) && !isCurrentModule) {
        ID moduleId = parsing::idToParentId(context, stmtId);
        const auto& resolvedModule = resolveModule(context, moduleId);
        assert(resolvedModule.hasId(id));
        result = resolvedModule.byId(id).type();
      }
    } else {
      // fall back to default value
      result = QualifiedType();
    }
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
      } else if (asttags::isInterface(tag)) {
        //TODO: kind = QualifiedType::INTERFACE;
        CHPL_UNIMPL("interfaces");
      } else {
        CHPL_ASSERT(false && "case not handled");
      }
    } else {
      CHPL_ASSERT(false && "case not handled");
    }

    result = QualifiedType(kind, t);
  }

  return QUERY_END(result);
}

const QualifiedType& typeForBuiltin(Context* context,
                                    UniqueString name) {
  QUERY_BEGIN(typeForBuiltin, context, name);

  QualifiedType result;

  std::unordered_map<UniqueString,const Type*> typeMap;
  Type::gatherBuiltins(context, typeMap);
  auto& globalMap = getCompilerGeneratedGlobals(context);

  auto searchTypes = typeMap.find(name);
  auto searchGlobals = globalMap.find(name);
  if (searchTypes != typeMap.end()) {
    const Type* t = searchTypes->second;
    CHPL_ASSERT(t);

    if (auto bct = t->toManageableType()) {
      auto d = ClassTypeDecorator(ClassTypeDecorator::GENERIC_NONNIL);
      t = ClassType::get(context, bct, /*manager*/ nullptr, d);
    }

    result = QualifiedType(QualifiedType::TYPE, t);
  } else if (searchGlobals != globalMap.end()) {
    result = searchGlobals->second;
  } else if (name == USTR("nil")) {
    result = QualifiedType(QualifiedType::CONST_VAR,
                           NilType::get(context));
  } else if (name == USTR("none")) {
    result = QualifiedType(QualifiedType::PARAM,
                           NothingType::get(context),
                           NoneParam::get(context, /* NoneValue */ {}));
  } else {
    // Could be a non-type builtin like 'index'
    result = QualifiedType();
  }

  return QUERY_END(result);
}

const QualifiedType& typeForSysCType(Context* context, UniqueString name) {
  QUERY_BEGIN(typeForSysCType, context, name);

  QualifiedType result;

  UniqueString modName = UniqueString::get(context, "ChapelSysCTypes");
  if (auto mod = parsing::getToplevelModule(context, modName)) {
    for (auto stmt : mod->children()) {
      auto decl = stmt->toNamedDecl();
      if (decl && decl->name() == name) {
        auto res = resolveModuleStmt(context, stmt->id());
        result = res.byId(stmt->id()).type();
      }
    }
  }

  return QUERY_END(result);
}

QualifiedType typeForLiteral(Context* context, const Literal* literal) {
  const Type* typePtr = nullptr;
  const Param* paramPtr = nullptr;

  switch (literal->tag()) {
    case asttags::BoolLiteral:
      typePtr = BoolType::get(context);
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
      typePtr = CompositeType::getBytesType(context);
      break;
    case asttags::CStringLiteral:
      typePtr = CStringType::get(context);
      break;
    case asttags::StringLiteral:
      typePtr = CompositeType::getStringType(context);
      break;
    default:
      CHPL_ASSERT(false && "case not handled");
  }
  paramPtr = literal->param();

  return QualifiedType(QualifiedType::PARAM, typePtr, paramPtr);
}


/////// function resolution

static bool
formalNeedsInstantiation(Context* context,
                         const QualifiedType& formalType,
                         const Decl* formalDecl,
                         const SubstitutionsMap* substitutions) {
  if (formalType.isUnknown()) {
    return true;
  }

  bool considerGenericity = true;
  if (substitutions != nullptr) {
    if (formalDecl && substitutions->count(formalDecl->id())) {
      // don't consider it needing a substitution - e.g. when passing
      // a generic type into a type argument.
      considerGenericity = false;
    }
  }

  if (considerGenericity) {
    auto g = getTypeGenericity(context, formalType);
    if (g != Type::CONCRETE) {
      return true;
    }
  }

  return false;
}

static bool
anyFormalNeedsInstantiation(Context* context,
                            const std::vector<types::QualifiedType>& formalTs,
                            const UntypedFnSignature* untypedSig,
                            SubstitutionsMap* substitutions) {
  bool genericOrUnknown = false;
  int i = 0;
  for (const auto& qt : formalTs) {
    if (formalNeedsInstantiation(context, qt, untypedSig->formalDecl(i),
                                 substitutions)) {
      genericOrUnknown = true;
      break;
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

static void checkForParenlessMethodFieldRedefinition(Context* context,
                                                     const Function* fn,
                                                     Resolver& visitor) {

  if (fn->isMethod() && fn->isParenless()) {
    QualifiedType receiverType;
    ID receiverId;
    if (visitor.getMethodReceiver(&receiverType, &receiverId)) {
      if (receiverType.type()) {
        // use the type information, if it is present
        if (auto ct = receiverType.type()->getCompositeType()) {
          receiverId = ct->id();
        }
      }
      if (!receiverId.isEmpty()) {
        if (parsing::idContainsFieldWithName(context, receiverId, fn->name())) {
          context->error(fn, "parenless proc redeclares the field '%s'",
                         fn->name().c_str());
        }
      }
    }
  }
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
    auto visitor = Resolver::createForInitialSignature(context, fn, r);
    // visit the formals
    for (auto formal : fn->formals()) {
      formal->traverse(visitor);
    }
    // do not visit the return type or function body

    // now, construct a TypedFnSignature from the result
    std::vector<types::QualifiedType> formalTypes = visitor.getFormalTypes(fn);
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

    checkForParenlessMethodFieldRedefinition(context, fn, visitor);

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

  auto ret = typedSignatureInitialQuery(context, untypedSig);
  // also check the signature at this point if it is concrete
  if (ret != nullptr && !ret->needsInstantiation()) {
    checkSignature(context, ret);
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
  } else if (auto fwd = ast->toForwardingDecl()) {
    if (auto fwdTo = fwd->expr()) {
      if (fwdTo->isDecl()) {
        helpSetFieldTypes(fwd->expr(), r, initedInParent, fields);
      }
      fields.addForwarding(fwd->id(), r.byAst(fwdTo).type());
    }
  }

  // no action needed for other types of Decls since they aren't fields
  // and can't contain fields
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
  } else if (auto td = ast->toEnum()) {
    result = EnumType::get(context, td->id(), td->name());
  }

  return QUERY_END(result);
}

const Type* initialTypeForTypeDecl(Context* context, ID declId) {
  return initialTypeForTypeDeclQuery(context, declId);
}

const ResolvedFields& resolveFieldDecl(Context* context,
                                       const CompositeType* ct,
                                       ID fieldId,
                                       DefaultsPolicy defaultsPolicy,
                                       bool ignoreTypes) {
  QUERY_BEGIN(resolveFieldDecl, context, ct, fieldId, defaultsPolicy, ignoreTypes);

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
    CHPL_ASSERT(typeAst && typeAst->isAggregateDecl());
    auto ad = typeAst->toAggregateDecl();

    // normalize ids in the case they are contained within another decl
    // this is so we don't try to resolve the type of an individual element without
    // the context of its container
    // TODO: This could become a performance concern as it gets called for each
    // element of a multiDecl and the way multiDecl elements are resolved involves
    // resolving all of the elements of the multiDecl each time, resulting in
    // quadratic time complexity.  We should consider a more efficient way to
    // resolve elements of a multiDecl
    auto stmtId = parsing::idToContainingMultiDeclId(context, fieldId);
    auto fieldAst = parsing::idToAst(context, stmtId);
    CHPL_ASSERT(fieldAst);

    if (ct->instantiatedFromCompositeType() == nullptr) {
      // handle resolving a not-yet-instantiated type
      ResolutionResultByPostorderID r;

      if (!ignoreTypes) {
        auto visitor =
          Resolver::createForInitialFieldStmt(context, ad, fieldAst,
                                              ct, r, defaultsPolicy);

        // resolve the field types and set them in 'result'
        fieldAst->traverse(visitor);
      }

      helpSetFieldTypes(fieldAst, r, /* initedInParent */ false, result);
    } else {
      // handle resolving an instantiated type
      ResolutionResultByPostorderID r;

      if (!ignoreTypes) {
        // use nullptr for POI scope because POI is not considered
        // when resolving the fields when constructing a type..
        const PoiScope* poiScope = nullptr;
        auto visitor =
          Resolver::createForInstantiatedFieldStmt(context, ad, fieldAst, ct,
                                                   poiScope, r,
                                                   defaultsPolicy);

        // resolve the field types and set them in 'result'
        fieldAst->traverse(visitor);
      }

      helpSetFieldTypes(fieldAst, r, /* initedInParent */ false, result);
    }
  }


  return QUERY_END(result);
}

static
const ResolvedFields& fieldsForTypeDeclQuery(Context* context,
                                             const CompositeType* ct,
                                             DefaultsPolicy defaultsPolicy,
                                             bool ignoreTypes) {
  QUERY_BEGIN(fieldsForTypeDeclQuery, context, ct, defaultsPolicy, ignoreTypes);

  QUERY_REGISTER_TRACER(
    auto& id = std::get<0>(args)->id();
    auto typeName = std::get<0>(args)->name();
    auto traceText = std::string("resolving the fields of type '")
      + typeName.c_str()
      + "'";
    return std::make_pair(id, traceText);
  );

  ResolvedFields result;

  CHPL_ASSERT(ct);
  result.setType(ct);

  bool isObjectType = false;
  if (auto bct = ct->toBasicClassType()) {
    isObjectType = bct->isObjectType();
  }
  bool isMissingBundledType =
    CompositeType::isMissingBundledType(context, ct->id());

  if (isObjectType || isMissingBundledType) {
    // no need to try to resolve the fields for the object type,
    // which doesn't have a real uAST ID.
    // for built-in types like Errors when we didn't parse the standard library
    // don't try to resolve the fields
  } else {
    auto ast = parsing::idToAst(context, ct->id());
    CHPL_ASSERT(ast && ast->isAggregateDecl());
    auto ad = ast->toAggregateDecl();

    for (auto child: ad->children()) {
      // Ignore everything other than VarLikeDecl, MultiDecl, TupleDecl
      bool isForwardingField = child->isForwardingDecl() &&
                               child->toForwardingDecl()->expr()->isDecl();
      if (child->isVarLikeDecl() ||
          child->isMultiDecl() ||
          child->isTupleDecl() ||
          isForwardingField) {
        const ResolvedFields& resolvedFields =
          resolveFieldDecl(context, ct, child->id(), defaultsPolicy, ignoreTypes);
        // Copy resolvedFields into result
        int n = resolvedFields.numFields();
        for (int i = 0; i < n; i++) {
          result.addField(resolvedFields.fieldName(i),
                          resolvedFields.fieldHasDefaultValue(i),
                          resolvedFields.fieldDeclId(i),
                          resolvedFields.fieldType(i));
        }
        result.addForwarding(resolvedFields);
      }

      if (auto rec = context->recoverFromSelfRecursion()) {
        CHPL_REPORT(context, RecursionFieldDecl, child, ad, ct, std::move(*rec));
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
                                        DefaultsPolicy defaultsPolicy,
                                        bool ignoreTypes) {
  // The defaults policy only matters if type resolution is in play. If it
  // isn't, always set defaults policy to IGNORE_DEFAULTS to avoid memoizing
  // the same result multiple times.
  if (ignoreTypes) {
    return fieldsForTypeDeclQuery(context, ct, DefaultsPolicy::IGNORE_DEFAULTS,
                                  /* ignoreTypes */ true);
  }

  if (defaultsPolicy == DefaultsPolicy::IGNORE_DEFAULTS){
    return fieldsForTypeDeclQuery(context, ct, DefaultsPolicy::IGNORE_DEFAULTS,
                                  /* ignoreTypes */ false);
  }

  // try first with defaultsPolicy=FOR_OTHER_FIELDS
  const auto& f = fieldsForTypeDeclQuery(context, ct,
                                         DefaultsPolicy::USE_DEFAULTS_OTHER_FIELDS,
                                         /* ignoreTypes */ false);

  // If defaultsPolicy=USE was requested and the type
  // is generic with defaults, compute the type again.
  // We do it this way so that we are more likely to be able to reuse the
  // result of the above query in most cases since most types
  // are not generic record/class with defaults.
  if (defaultsPolicy == DefaultsPolicy::USE_DEFAULTS) {
    // if record is not generic with defaults, return its
    // uninstantiated fields. Otherwise, instantiate.
    auto finalDefaultsPolicy = f.isGenericWithDefaults() ?
      DefaultsPolicy::USE_DEFAULTS :
      DefaultsPolicy::IGNORE_DEFAULTS;
    return fieldsForTypeDeclQuery(context, ct, finalDefaultsPolicy,
                                  /* ignoreTypes */ false);
  }

  // Otherwise, use the value we just computed.
  return f;
}

// Resolve all statements like 'forwarding _value;' in 'ct'
static
const ResolvedFields& resolveForwardingExprs(Context* context,
                                             const CompositeType* ct) {
  QUERY_BEGIN(resolveForwardingExprs, context, ct);

  ResolvedFields result;

  CHPL_ASSERT(ct);
  result.setType(ct);

  bool isObjectType = false;
  if (auto bct = ct->toBasicClassType()) {
    isObjectType = bct->isObjectType();
  }
  bool isMissingBundledType =
    CompositeType::isMissingBundledType(context, ct->id());

  if (isObjectType || isMissingBundledType) {
    // no need to try to resolve the fields for the object type,
    // which doesn't have a real uAST ID.
    // for built-in types like Errors when we didn't parse the standard library
    // don't try to resolve the fields
  } else {
    auto ast = parsing::idToAst(context, ct->id());
    CHPL_ASSERT(ast && ast->isAggregateDecl());
    auto ad = ast->toAggregateDecl();

    // TODO: don't rely on 'ResolvedFields' or 'resolveFieldDecl' here...
    for (auto child: ad->children()) {
      if (child->isForwardingDecl() &&
          !child->toForwardingDecl()->expr()->isDecl()) {
        const ResolvedFields& resolvedFields =
          resolveFieldDecl(context, ct, child->id(), DefaultsPolicy::USE_DEFAULTS);
        result.addForwarding(resolvedFields);
      }
    }
  }

  return QUERY_END(result);
}

static bool typeUsesForwarding(Context* context, const Type* receiverType) {
  if (auto ct = receiverType->getCompositeType()) {
    if (ct->isBasicClassType() || ct->isRecordType() || ct->isUnionType()) {
      ID ctId = ct->id();
      if (!ctId.isEmpty()) {
        return parsing::aggregateUsesForwarding(context, ctId);
      }
    }
  }

  return false;
}

// returns 'true' if a cycle was detected
static bool
checkForwardingCycles(Context* context,
                      const CompositeType* ct,
                      llvm::SmallPtrSet<const CompositeType*, 8>& visited) {

  if (typeUsesForwarding(context, ct)) {
    auto pair = visited.insert(ct);
    if (pair.second == false) {
      // it was already in the visited set
      context->error(ct->id(), "forwarding cycle detected");
      return true;
    }

    const ResolvedFields& r = fieldsForTypeDecl(context, ct,
                                                DefaultsPolicy::USE_DEFAULTS);

    // Check for cycles. If a cycle is detected, emit an error
    // and return 'true'.
    int n = r.numForwards();
    for (int i = 0; i < n; i++) {
      auto qt = r.forwardingToType(i);
      if (auto t = qt.type()) {
        if (auto forwardingCt = t->getCompositeType()) {
          bool cyc = checkForwardingCycles(context, forwardingCt, visited);
          if (cyc) {
            return true;
          }
        }
      }
    }
  }
  return false;
}


// returns a 'true' if there was a cycle and reports an error in that case.
// otherwise, returns 'false'.
static const bool&
forwardingCycleCheckQuery(Context* context, const CompositeType* ct) {
  QUERY_BEGIN(forwardingCycleCheckQuery, context, ct);

  bool result = false;
  llvm::SmallPtrSet<const CompositeType*, 8> visited;

  result = checkForwardingCycles(context, ct, visited);

  return QUERY_END(result);
}

// returns 'true' if a forwarding cycle was detected & error emitted
static bool
emitErrorForForwardingCycles(Context* context, const CompositeType* ct) {
  bool cycleFound = false;
  if (typeUsesForwarding(context, ct)) {
    // check for cycles
    cycleFound = forwardingCycleCheckQuery(context, ct);
  }

  return cycleFound;
}

static const CompositeType* getTypeWithDefaults(Context* context,
                                                const CompositeType* ct) {
  // resolve the fields with DefaultsPolicy=FOR_OTHER_FIELDS
  const ResolvedFields& g = fieldsForTypeDecl(context, ct,
                                              DefaultsPolicy::USE_DEFAULTS_OTHER_FIELDS);
  if (!g.isGenericWithDefaults()) {
    return ct;
  }

  // and with DefaultsPolicy=USE
  const ResolvedFields& r = fieldsForTypeDecl(context, ct,
                                              DefaultsPolicy::USE_DEFAULTS);

  // for any field that has a different type in r than in g, add
  // a substitution, and get the type with those substitutions.
  SubstitutionsMap substitutions;
  int nFields = g.numFields();
  CHPL_ASSERT(r.numFields() == nFields);
  for (int i = 0; i < nFields; i++) {
    CHPL_ASSERT(g.fieldName(i) == r.fieldName(i));
    CHPL_ASSERT(g.fieldDeclId(i) == r.fieldDeclId(i));
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
  CHPL_ASSERT(ast && ast->isAggregateDecl());
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
      if (auto bct = clst->basicClassType()) {
        auto got = getTypeWithDefaultsQuery(context, bct);
        CHPL_ASSERT(got->isBasicClassType());
        bct = got->toBasicClassType();

        auto r = ClassType::get(context, bct, clst->manager(), clst->decorator());
        return QualifiedType(t.kind(), r, t.param());
      }
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
      if (g == Type::GENERIC || g == Type::MAYBE_GENERIC) {
        combined = g;
      } else if (g == Type::GENERIC_WITH_DEFAULTS &&
                 combined == Type::CONCRETE) {
        combined = g;
      }
    }
    return combined;
  } else if (auto dt = ct->toDomainType()) {
    Type::Genericity combined = Type::CONCRETE;

    // Allows for instantiation of things like 'arg: domain'
    // TODO: currently partially generic domains are not supported
    if (dt->kind() == DomainType::Kind::Unknown) {
      combined = Type::GENERIC;
    }

    return combined;
  } else if (auto at = ct->toArrayType()) {
    auto dt = getTypeGenericityIgnoring(context, at->domainType(), ignore);
    auto et = getTypeGenericityIgnoring(context, at->eltType(), ignore);

    if (dt != Type::CONCRETE || et != Type::CONCRETE) {
      return Type::GENERIC;
    } else {
      return Type::CONCRETE;
    }
  }

  // Some testing code creates CompositeType with empty IDs.
  // Assume these are concrete.
  // Also 'object' has an empty ID and is concrete.
  if (ct->id().isEmpty())
    return Type::CONCRETE;

  Type::Genericity g = Type::CONCRETE;

  if (auto bct = ct->toBasicClassType()) {
    g = getFieldsGenericity(context, bct->parentClassType(), ignore);
    CHPL_ASSERT(g != Type::MAYBE_GENERIC);
    if (g == Type::GENERIC)
      return Type::GENERIC;
  }

  if (context->isQueryRunning(fieldsForTypeDeclQuery,
                              std::make_tuple(ct, DefaultsPolicy::IGNORE_DEFAULTS, false)) ||
      context->isQueryRunning(fieldsForTypeDeclQuery,
                              std::make_tuple(ct, DefaultsPolicy::USE_DEFAULTS, false)) ||
      context->isQueryRunning(fieldsForTypeDeclQuery,
                              std::make_tuple(ct, DefaultsPolicy::USE_DEFAULTS_OTHER_FIELDS, false))) {
    // TODO: is there a better way to avoid problems with recursion here?
    return Type::CONCRETE;
  }

  // we only care about whether or not each field is generic on its own
  // merit, as only these fields need defaults. Thus, we allow defaults
  // for fields other than the one we are checking. In this way, we prevent
  // some field (a) that depends on the value of field (b) from being
  // marked generic just because (b) is generic.
  DefaultsPolicy defaultsPolicy = DefaultsPolicy::USE_DEFAULTS_OTHER_FIELDS;
  const ResolvedFields& f = fieldsForTypeDecl(context, ct,
                                              defaultsPolicy);

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

  if (auto pt = t->toCPtrType()) {
    // Mimics the fields logic: if any field is non-concrete, the whole
    // type is generic. Logically, the c_ptr has a single field, the element
    // type.
    if (getTypeGenericityIgnoring(context, pt->eltType(), ignore) == Type::CONCRETE) {
      return Type::CONCRETE;
    } else {
      return Type::GENERIC;
    }
  }

  // MAYBE_GENERIC should only be returned for CompositeType /
  // ClassType right now.
  CHPL_ASSERT(t->isCompositeType() || t->isClassType());

  // the tuple type that isn't an instantiation is a generic type
  if (auto tt = t->toTupleType()) {
    if (tt->instantiatedFromCompositeType() == nullptr)
      return Type::GENERIC;
    if (tt->isKnownSize() == false) {
      return Type::GENERIC;
    }
  }

  // string and bytes types are never generic
  if (t->isStringType() || t->isBytesType()) {
    return Type::CONCRETE;
  }

  if (auto classType = t->toClassType()) {
    // should be handled in BasicClassType::isGeneric
    // so this code should only be called if the management is concrete
    CHPL_ASSERT(!classType->decorator().isUnknownManagement());
    CHPL_ASSERT(!classType->decorator().isUnknownNilability());

    auto mt = classType->manageableType();
    if (auto bct = mt->toBasicClassType()) {
      return getFieldsGenericity(context, bct, ignore);
    } else {
      CHPL_ASSERT(mt->isAnyClassType());
      return Type::GENERIC;
    }
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

/**
  Written primarily to support multi-decls, though the logic is the same
  as for single declarations. Sets 'outIsGeneric' with the genericity of the
  variable; however, if the genericity might be affected by the neighbors
  of this variable in a MultiDecl, returns 'false'. Thus, calling this function
  on the neighbors of the variable until it returns 'true' should help determine
  the genericity in a multi-decl.
 */
static bool isVariableDeclWithClearGenericity(Context* context,
                                              const VarLikeDecl* var,
                                              bool &outIsGeneric,
                                              types::QualifiedType* outFormalType) {
  // fields that are 'type' or 'param' are generic
  // and we can use the same type/param intent for the type constructor
  if (var->storageKind() == QualifiedType::TYPE ||
      var->storageKind() == QualifiedType::PARAM) {
    if (outFormalType)
      *outFormalType = QualifiedType(var->storageKind(), AnyType::get(context));
    outIsGeneric = true;
    return true;
  }

  // non-type/param fields with an init expression aren't generic
  if (var->initExpression() != nullptr) {
    outIsGeneric = false;
    return true;
  }

  // non-type/param fields that have no declared type and no initializer
  // are generic and these need a type variable for the argument with AnyType.
  // Except if they are part of a multi-decl, in which case they can
  // be inheriting their value or type from their neighbor.
  if (var->typeExpression() == nullptr) {
    if (outFormalType)
      *outFormalType = QualifiedType(QualifiedType::TYPE, AnyType::get(context));
    outIsGeneric = true;
    return false;
  }

  // Otherwise, it has a type expression and no init expression. The form
  // of the type expression determines if we guess it to be generic or
  // concrete. The only "generic" form is a call with a "(?)" actual.

  if (auto call = var->typeExpression()->toFnCall()) {
    for (auto actual : call->actuals()) {
      if (auto ident = actual->toIdentifier()) {
        if (ident->name() == "?") {
          outIsGeneric = true;
          return true;
        }
      }
    }
  }

  outIsGeneric = false;
  return true;
}

bool isFieldSyntacticallyGeneric(Context* context,
                                 const ID& fieldId,
                                 types::QualifiedType* formalType) {
  // compare with AggregateType::fieldIsGeneric

  if (asttags::isVarLikeDecl(parsing::idToTag(context, fieldId))) {
    auto var = parsing::idToAst(context, fieldId)->toVarLikeDecl();

    bool isGeneric;
    if (isVariableDeclWithClearGenericity(context, var, isGeneric, formalType)) {
      return isGeneric;
    }

    // Genericity isn't clear; if we're in a multi-decl, try searching
    // to the right.
    auto parentId = parsing::idToParentId(context, fieldId);
    if (parsing::idToTag(context, parentId) == asttags::MultiDecl) {
      auto md = parsing::idToAst(context, parentId)->toMultiDecl();

      // First, seek to the right until we find the field we're looking for.
      auto declIterPair = md->decls();
      auto declIter = declIterPair.begin();
      for(; declIter != declIterPair.end(); declIter++) {
        if (*declIter == var) {
          break;
        }
      }

      // Then, go through and look for a neighbor with a clear genericity.
      for (declIter++; declIter != declIterPair.end(); declIter++) {
        auto neighborVar = declIter->toVarLikeDecl();
        if (!neighborVar) {
          // Only VarLikeDecls neighbors share genericity; TupleDecls, for instance,
          // interrupt the sharing of types and initializers in a multi-decl.
          break;
        }

        if (isVariableDeclWithClearGenericity(context, neighborVar, isGeneric, formalType)) {
          break;
        }
      }
    }

    return isGeneric;
  }

  // otherwise it does not need to go into the type constructor
  return false;
}

bool shouldIncludeFieldInTypeConstructor(Context* context,
                                         const ID& fieldId,
                                         const QualifiedType& fieldType,
                                         QualifiedType* formalType) {
  // compare with AggregateType::fieldIsGeneric

  // fields with concrete types don't need to be in type constructor
  if (!fieldType.isGenericOrUnknown()) {
    return false;
  }

  // fields that are 'type' or 'param' are generic
  // and we can use the same type/param intent for the type constructor
  if ((fieldType.isParam() && !fieldType.hasParamPtr()) ||
      fieldType.isType()) {
    if (formalType) *formalType = fieldType;
    return true;
  }

  if (asttags::isVarLikeDecl(parsing::idToTag(context, fieldId))) {
    auto var = parsing::idToAst(context, fieldId)->toVarLikeDecl();

    // non-type/param fields with an init expression aren't generic
    if (var->initExpression())
      return false;

    // non-type/param fields that have no declared type and no initializer
    // are generic and these need a type variable for the argument with AnyType.
    if (var->typeExpression() == nullptr) {
      if (formalType)
        *formalType = QualifiedType(QualifiedType::TYPE, AnyType::get(context));
      return true;
    }

    // otherwise, the field may or may not be generic.
    // it is generic if the field type is generic.
    // for this check we make some simplifying assumptions:
    //  * generic-with-defaults means concrete, unless ? is used in the type
    //  * unknown type means it depends on a previous generic field
    //    (and when previous generic fields are set, they will be concrete)
    const Type* t = fieldType.type();
    // a 'var' field of 'AnyType' isn't itself generic, it just depends on
    // another field that's 'AnyType'. In that case, treat it as unknown.
    bool isVarOfAnyType = fieldType.kind() != QualifiedType::TYPE &&
                          t && t->isAnyType();
    if (t && !isVarOfAnyType && !t->isUnknownType()) {
      Type::Genericity g = getTypeGenericity(context, t);
      if (g == Type::GENERIC) { // and not GENERIC_WITH_DEFAULTS
        if (formalType) *formalType = QualifiedType(QualifiedType::TYPE, t);
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
    DefaultsPolicy defaultsPolicy = DefaultsPolicy::IGNORE_DEFAULTS;
    const ResolvedFields& f = fieldsForTypeDecl(context, ct,
                                                defaultsPolicy,
                                                /* ignoreTypes */ true);

    // find the generic fields from the type and add
    // these as type constructor arguments.
    int nFields = f.numFields();
    for (int i = 0; i < nFields; i++) {
      auto declId = f.fieldDeclId(i);
      auto declAst = parsing::idToAst(context, declId);
      CHPL_ASSERT(declAst);
      const Decl* fieldDecl = declAst->toDecl();
      CHPL_ASSERT(fieldDecl);
      QualifiedType formalType;
      if (isFieldSyntacticallyGeneric(context, declId, &formalType)) {

        auto defaultKind = f.fieldHasDefaultValue(i) ?
                           UntypedFnSignature::DK_DEFAULT :
                           UntypedFnSignature::DK_NO_DEFAULT;
        auto d = UntypedFnSignature::FormalDetail(f.fieldName(i),
                                                  defaultKind,
                                                  fieldDecl,
                                                  fieldDecl->isVarArgFormal());
        formals.push_back(d);
        // formalType should have been set above
        CHPL_ASSERT(formalType.kind() != QualifiedType::UNKNOWN);
        formalTypes.push_back(formalType);
      }
    }

    if (t->isBasicClassType() || t->isClassType()) {
      idTag = uast::asttags::Class;
    } else if (t->isRecordType()) {
      idTag = uast::asttags::Record;
    } else if (t->isUnionType()) {
      idTag = uast::asttags::Union;
    }
  } else {
    CHPL_ASSERT(false && "case not handled");
  }

  auto untyped = UntypedFnSignature::get(context,
                                         id, name,
                                         /* isMethod */ false,
                                         /* isTypeConstructor */ true,
                                         /* isCompilerGenerated */ true,
                                         /* throws */ false,
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

  CHPL_ASSERT(actualT != nullptr);
  CHPL_ASSERT(formalT != nullptr);

  // this function should only be called when instantiation is required
  CHPL_ASSERT(canPass(context, actualType, formalType).passes());
  CHPL_ASSERT(canPass(context, actualType, formalType).instantiates());

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
        CHPL_ASSERT(actualCt->decorator().isManaged() && actualCt->manager());
        manager = actualCt->manager();
      }

      // which BasicClassType to use?
      const BasicClassType* bct;
      if (auto formalBct = formalCt->basicClassType()) {
        bct = formalBct;
      } else {
        CHPL_ASSERT(formalCt->manageableType()->toManageableType());
        bct = actualCt->basicClassType();
      }
      auto g = getTypeGenericity(context, bct);
      if (g != Type::CONCRETE) {
        CHPL_UNIMPL("instantiate generic class formal");
      }

      // now construct the ClassType
      auto ct = ClassType::get(context, bct, manager, dec);
      return QualifiedType(formalType.kind(), ct);
    }

    // handle decorated class passed to special built-in type
    auto classBuiltinTypeDec = ClassTypeDecorator::GENERIC;
    bool foundClassyBuiltinType = true;

    if (formalT->isAnyOwnedType() &&
               actualCt->decorator().isManaged() &&
               actualCt->manager()->isAnyOwnedType()) {
      classBuiltinTypeDec = ClassTypeDecorator::MANAGED;
    } else if (formalT->isAnySharedType() &&
               actualCt->decorator().isManaged() &&
               actualCt->manager()->isAnySharedType()) {
      classBuiltinTypeDec = ClassTypeDecorator::MANAGED;
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
  } else if (auto actualPt = actualT->toCPtrType()) {
    if (auto formalPt = formalT->toCPtrType()) {
      // The only reason we should need an instantiation type is if a constness
      // coercion was applied, which is only possible for const formal, non-const
      // actual.
      CHPL_ASSERT(formalPt->isConst() && !actualPt->isConst());

      auto pt = CPtrType::getConst(context, actualPt->eltType());
      return QualifiedType(formalType.kind(), pt);
    }
  } else if (actualType.isParam() && formalType.isParam() &&
             formalType.hasParamPtr() == false ) {
    return Param::fold(context, nullptr, PrimitiveTag::PRIM_CAST, actualType, formalType);
  }

  // TODO: sync type -> value type?
  CHPL_ASSERT(false && "case not handled");
  return QualifiedType();
}

const std::map<ID, QualifiedType>&
computeNumericValuesOfEnumElements(Context* context, ID node) {
  QUERY_BEGIN(computeNumericValuesOfEnumElements, context, node);

  std::map<ID, types::QualifiedType> result;
  auto ast = parsing::idToAst(context, node);
  if (!ast) return QUERY_END(result);
  auto enumNode = ast->toEnum();
  if (!enumNode) return QUERY_END(result);

  ResolutionResultByPostorderID byPostorder;
  Resolver res = Resolver::createForEnumElements(context, enumNode, byPostorder);

  // The constant 'one' for adding
  auto one = QualifiedType(QualifiedType::PARAM,
                           IntType::get(context, 0),
                           IntParam::get(context, 1));

  // A type to track what kind of signedness a value needs.
  enum RequiredSignedness {
    RS_NONE,
    RS_SIGNED,
    RS_UNSIGNED,
  };

  // First collect all the values, no matter what types they are.
  using ValueVector = std::vector<std::tuple<QualifiedType,
                                             RequiredSignedness,
                                             const EnumElement*>>;
  ValueVector valuesAndAsts;
  for (auto elem : enumNode->enumElements()) {
    elem->traverse(res);
    QualifiedType value = {};

    // Found an initialization expression; use its type.
    if (elem->initExpression()) {
      auto qt = byPostorder.byAst(elem->initExpression()).type();
      auto type = qt.type();

      if (qt.isErroneousType() || qt.isGenericOrUnknown()) {
        // Don't propagate errors if they're unrelated; leave value unknown.
      } else if (!type->isIntType() && !type->isUintType()) {
        value = CHPL_TYPE_ERROR(context, EnumInitializerNotInteger, elem, qt);
      } else if (!qt.isParam()) {
        value = CHPL_TYPE_ERROR(context, EnumInitializerNotParam, elem, qt);
      } else {
        value = qt;
      }
    } else {
      if (valuesAndAsts.empty() || std::get<2>(valuesAndAsts.back()) == nullptr) {
        // we're either the first value, or all the previous values have
        // been abstract. We're abstract too -- encode this using a 'null'
        // elem.
        elem = nullptr;
      } else {
        auto& lastValueInfo = valuesAndAsts.back();
        auto lastQt = std::get<0>(lastValueInfo);
        if (lastQt.isParam()) {
          // Previous value was valid, so add one to it.
          value = Param::fold(context, elem, chpl::uast::PRIM_ADD, lastQt, one);
        } else {
          // Previous value was unknown, so we can't add one to it.
        }
      }
    }

    RequiredSignedness signedness = RS_NONE;
    if (!value.param()) {
      // Do nothing.
    } else if (auto intParam = value.param()->toIntParam()) {
      signedness = intParam->value() < 0 ? RS_SIGNED : RS_NONE;
    } else if (auto uintParam = value.param()->toUintParam()) {
      signedness = uintParam->value() > INT64_MAX ? RS_UNSIGNED : RS_NONE;
    }

    valuesAndAsts.emplace_back(value, signedness, elem);
  }

  const EnumElement* needsSigned = nullptr;
  QualifiedType valueNeedsSigned;
  const EnumElement* needsUnsigned = nullptr;
  QualifiedType valueNeedsUnsigned;

  for (auto& valueInfo : valuesAndAsts) {
    auto& qt = std::get<0>(valueInfo);
    auto signedness = std::get<1>(valueInfo);
    auto elem = std::get<2>(valueInfo);
    if (elem == nullptr) {
      // abstract value; skip it.
      continue;
    }

    if (signedness == RS_SIGNED) {
      if (!needsSigned) {
        needsSigned = elem;
        valueNeedsSigned = qt;
      }
    } else if (signedness == RS_UNSIGNED) {
      if (!needsUnsigned) {
        needsUnsigned = elem;
        valueNeedsUnsigned = qt;
      }
    }
  }

  if (needsSigned && needsUnsigned) {
    CHPL_REPORT(context, NoTypeForEnumElem, enumNode,
                needsSigned, valueNeedsSigned, needsUnsigned, valueNeedsUnsigned);

    // Though not all elements of the enum fit in the one type that we'll
    // pick (which will be unsigned), we'll proceed on a best-effort basis,
    // and store all results that don't need unsigned values. This way,
    // an enum with two elements, one negative and one too big to fit in int(64),
    // will be determined to have at least one properly-computed constant.
    // This will help provide more resolution information to the user.
  }

  // Use unsigned if any value needed it; otherwise, use signed.

  // We've now picked what type we're going to use. Convert the non-abstract values to
  // that type if they can be converted, and leave them unknown if they can't.
  for (auto& valueInfo : valuesAndAsts) {
    auto qt = std::get<0>(valueInfo);
    auto signedness = std::get<1>(valueInfo);
    auto elem = std::get<2>(valueInfo);

    if (!elem) {
      // abstract value; don't store it in the map.
      continue;
    }

    auto resultType = QualifiedType();
    optional<int64_t> signedValue = {};
    if (needsUnsigned && signedness == RS_SIGNED)  {
      // This value was known before, but it doesn't fit in the type.
      // We'll mark it with 'erroneous type'. The error has already been
      // issued above.
      resultType = QualifiedType(QualifiedType::UNKNOWN,
                                 ErroneousType::get(context));
    } else if (!qt.param()) {
      // The value is already unknown, so preserve it.
      resultType = qt;
    } else if (auto intParam = qt.param()->toIntParam()) {
      signedValue = intParam->value();
    } else if (auto uintParam = qt.param()->toUintParam()) {
      signedValue = (int64_t) uintParam->value();
    }

    if (signedValue) {
      if (needsUnsigned) {
        resultType = QualifiedType(QualifiedType::PARAM,
                                   UintType::get(context, 0),
                                   UintParam::get(context, (uint64_t) *signedValue));
      } else {
        resultType = QualifiedType(QualifiedType::PARAM,
                                   IntType::get(context, 0),
                                   IntParam::get(context, *signedValue));
      }
    }

    result[elem->id()] = resultType;
  }

  return QUERY_END(result);
}

const chpl::optional<QualifiedType>& computeUnderlyingTypeOfEnum(Context* context, ID element) {
  QUERY_BEGIN(computeUnderlyingTypeOfEnum, context, element);

  chpl::optional<QualifiedType> result;
  auto numericValues = computeNumericValuesOfEnumElements(context, element);

  // Find the first non-unknown value, and return its type. As a fallback,
  // return either the default unknown value or, if we've encountered an error,
  // the erroneous type.
  for (auto& pair : numericValues) {
    if (pair.second.isParam()) {
      result = QualifiedType(QualifiedType::TYPE, pair.second.type());
      break;
    } else if (!result) {
      result = pair.second;
    }
  }

  return QUERY_END(result);
}

const chpl::optional<QualifiedType>&
computeNumericValueOfEnumElement(Context* context, ID node) {
  QUERY_BEGIN(computeNumericValueOfEnumElement, context, node);
  auto nodeTag = parsing::idToTag(context, node);
  chpl::optional<QualifiedType> result = {};

  if (nodeTag != uast::asttags::EnumElement) {
    return QUERY_END(result);
  }

  auto parentId = parsing::idToParentId(context, node);
  auto parentTag = parsing::idToTag(context, parentId);
  if (parentTag != uast::asttags::Enum) {
    return QUERY_END(result);
  }

  auto& numericValues = computeNumericValuesOfEnumElements(context, parentId);
  auto it = numericValues.find(node);
  if (it != numericValues.end()) {
    result = it->second;
  }

  return QUERY_END(result);
}

ID lookupEnumElementByNumericValue(Context* context,
                                   const ID& node,
                                   const QualifiedType& value) {
  // Maps don't store the order of insertion, so we can't rely on iterating
  // the numeric value map. Instead, iterate the enum constants and
  // try find their numeric values.
  auto ast = parsing::idToAst(context, node);
  if (!ast) return ID();
  auto enumAst = ast->toEnum();
  if (!enumAst) return ID();

  auto& numericValues = computeNumericValuesOfEnumElements(context, node);

  for (auto elt : enumAst->enumElements()) {
    auto it = numericValues.find(elt->id());
    if (it != numericValues.end() && it->second == value) {
      return elt->id();
    }
  }

  return ID();
}

static bool varArgCountMatch(const VarArgFormal* formal,
                             ResolutionResultByPostorderID& r) {
  QualifiedType formalType = r.byAst(formal).type();
  auto tupleType = formalType.type()->toTupleType();

  if (formal->count() != nullptr) {
    const ResolvedExpression& count = r.byAst(formal->count());
    QualifiedType ct = count.type();
    if (ct.isParam() && ct.param() != nullptr) {
      auto numElements = tupleType->numElements();
      if (auto ip = ct.param()->toIntParam()) {
        return numElements == ip->value();
      } else if (auto up = ct.param()->toUintParam()) {
        return (uint64_t)numElements == up->value();
      } else {
        // TODO: Error message about coercing non-integrals in the
        // count-expression.
        return false;
      }
    }
  }

  return true;
}

static QualifiedType getVarArgTupleElemType(const QualifiedType& varArgType) {
  // If the type is a VarArgTuple then we should use its 'star' type
  // with 'canPass'.
  //
  // Note: Unless there was an error resolving the type, this tuple
  // should be a VarArgTuple
  //
  // TODO: Should we update 'canPass' to reason about VarArgTuples?
  const TupleType* tup = varArgType.type()->toTupleType();
  if (tup != nullptr && tup->isVarArgTuple()) {
    return tup->starType();
  } else {
    return varArgType;
  }
}

static Resolver createResolverForAst(Context* context,
                                     const Function* fn,
                                     const AggregateDecl* ad,
                                     const Enum* ed,
                                     const SubstitutionsMap& substitutions,
                                     const PoiScope* poiScope,
                                     ResolutionResultByPostorderID& r) {
  if (fn != nullptr) {
    return Resolver::createForInstantiatedSignature(context, fn, substitutions,
                                                    poiScope, r);
  } else if (ad != nullptr) {
    return Resolver::createForInstantiatedSignatureFields(context, ad,
                                                          substitutions,
                                                          poiScope, r);
  } else {
    CHPL_ASSERT(ed != nullptr);
    return Resolver::createForEnumElements(context, ed, r);
  }
}

static QualifiedType getProperFormalType(const ResolutionResultByPostorderID& r,
                                         const FormalActual& entry,
                                         const AggregateDecl* ad,
                                         const AstNode* typeFor) {
  auto type = r.byAst(typeFor).type();
  if (ad != nullptr) {
    // generic var fields from a type are type fields in its type constructor.
    // so, make sure the kind is correct.
    type = QualifiedType(entry.formalType().kind(),
                         type.type(),
                         type.param());
  }
  return type;
}

static bool isCallInfoForInitializer(const CallInfo& ci) {
  if (ci.name() == USTR("init") || ci.name() == USTR("init="))
    if (ci.isMethodCall())
      return true;
  return false;
}

// TODO: Move these to the 'InitResolver' visitor.
static bool isTfsForInitializer(const TypedFnSignature* tfs) {
  if (tfs->untyped()->name() == USTR("init") ||
      tfs->untyped()->name() == USTR("init="))
    if (tfs->untyped()->isMethod())
      return true;
  return false;
}

static bool ensureBodyIsResolved(Context* context, const CallInfo& ci,
                                 const TypedFnSignature* tfs) {
  if (tfs->untyped()->isCompilerGenerated()) return false;
  if (isTfsForInitializer(tfs)) return true;
  return false;
}

ApplicabilityResult instantiateSignature(Context* context,
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

  CHPL_ASSERT(sig->needsInstantiation());

  const UntypedFnSignature* untypedSignature = sig->untyped();
  const AstNode* ast = nullptr;
  const Function* fn = nullptr;
  const AggregateDecl* ad = nullptr;
  const Enum* ed = nullptr;

  if (!untypedSignature->id().isEmpty()) {
    ast = parsing::idToAst(context, untypedSignature->id());
    fn = ast->toFunction();
    ad = ast->toAggregateDecl();
    ed = ast->toEnum();
  }

  const TypedFnSignature* parentFnTyped = nullptr;
  if (sig->parentFn()) {
    if (nullptr != computeOuterVariables(context, sig->id())) {
      CHPL_UNIMPL("generic child functions that refer to outer variables "
                  "are not yet supported");
      return ApplicabilityResult::failure(sig->id(), FAIL_CANDIDATE_OTHER);
    }
  }

  auto faMap = FormalActualMap(sig, call);
  if (!faMap.isValid()) {
    return ApplicabilityResult::failure(sig->id(), FAIL_FORMAL_ACTUAL_MISMATCH);
  }

  // compute the substitutions
  SubstitutionsMap substitutions;
  Bitmap formalsInstantiated;
  int formalIdx = 0;

  // this vector is used when creating a typed signature below, after checking
  // instantiations. For functions and aggregates, it's populated from
  // substitutions after they are computed formal-by-formal below.
  //
  // For compiler-generated candidates that aren't tied to functions or records,
  // we don't have an AST, so we can't store anything into substitutions.
  // Instead, we'll populate the formal types right away.
  std::vector<types::QualifiedType> formalTypes;

  bool instantiateVarArgs = false;
  std::vector<QualifiedType> varargsTypes;
  int varArgIdx = -1;

  ResolutionResultByPostorderID r;
  auto visitor = createResolverForAst(context, fn, ad, ed, substitutions,
                                      poiScope, r);

  QualifiedType varArgType;
  for (const FormalActual& entry : faMap.byFormals()) {
    // Do not ignore substitutions initially
    visitor.ignoreSubstitutionFor = nullptr;
    visitor.skipTypeQueries = false;

    bool addSub = false;
    QualifiedType useType;
    const auto formal = untypedSignature->formalDecl(entry.formalIdx());
    const auto& actualType = entry.actualType();

    // Re-compute the formal type using substitutions if needed.
    // Performance: we can start doing this only after the first substitution
    //              is created
    QualifiedType formalType;
    if (entry.isVarArgEntry()) {
      if (varArgType.isUnknown()) {
        // We haven't yet re-computed the vararg tuple type.
        formal->traverse(visitor);
        varArgType = r.byAst(formal).type();
      }
      formalType = getVarArgTupleElemType(varArgType);
    } else if (formal) {
      formal->traverse(visitor);
      formalType = getProperFormalType(r, entry, ad, formal);
    } else {
      // without a formal AST, assume that the originally provided formal type
      // is right.
      formalType = entry.formalType();
    }

    // note: entry.actualType can have type()==nullptr and UNKNOWN.
    // in that case, resolver code should treat it as a hint to
    // use the default value. Unless the call used a ? argument.
    if (actualType.kind() == QualifiedType::UNKNOWN &&
        actualType.type() == nullptr) {
      if (call.hasQuestionArg()) {
        // don't add any substitution
      } else {
        // add a "use the default" hint substitution.
        addSub = true;
        useType = actualType;
      }
    } else {
      auto got = canPass(context, actualType, formalType);
      if (!got.passes()) {
        // Including past type information made this instantiation fail.
        return ApplicabilityResult::failure(sig, got.reason(), entry.formalIdx());
      }
      if (got.instantiates()) {
        // add a substitution for a valid value
        if (!got.converts() && !got.promotes()) {
          // use the actual type since no conversion/promotion was needed
          addSub = true;
          useType = actualType;
        } else {
          // get instantiation type
          addSub = true;
          useType = getInstantiationType(context,
                                         actualType,
                                         formalType);

          // Verify that the 'instantiation type' still accepts the actual.
          // This might not be the case based on legal argument mapping rules.
          //
          // For instance, we can successfully instantiate 'ref x: Parent'
          // with 'shared Child', leading to a 'ref x: shared Parent'
          // useType. However, we cannot pass a 'shared Child' to a
          // 'ref x: shared Parent' formal, because 'ref' requires the types
          // to match exactly, and rules out subtype conversions.

          auto kind = resolveIntent(useType, /* isThis */ false, /* isInit */ false);
          auto useTypeConcrete = QualifiedType(kind, useType.type(), useType.param());

          auto got = canPass(context, actualType, useTypeConcrete);
          if (!got.passes()) {
            return ApplicabilityResult::failure(sig, got.reason(), entry.formalIdx());
          }
        }
      }
    }

    if (entry.isVarArgEntry()) {
      // If any formal needs instantiating then we need to instantiate all
      // the VarArgs
      instantiateVarArgs = instantiateVarArgs || addSub;

      // If the formal wasn't instantiated then use whatever type was computed.
      if (!addSub) useType = formalType;

      QualifiedType::Kind qtKind = formalType.kind();
      auto tempQT = QualifiedType(qtKind, useType.type());
      auto newKind = resolveIntent(tempQT, /*isThis*/ false, /*isInit*/false);

      auto param = formalType.isParam() ? useType.param() : nullptr;
      useType = QualifiedType(newKind, useType.type(), param);

      varargsTypes.push_back(useType);

      // Grab the index and formal when first encountering a VarArgFormal.
      // Also increment the formalIdx once to stay aligned.
      if (varArgIdx < 0) {
        varArgIdx = formalIdx;
        formalIdx += 1;
      }
    } else {
      // add the substitution if we identified that we need to
      if (addSub) {
        if (formal) {
          // add it to the substitutions map
          substitutions.insert({entry.formal()->id(), useType});
          // Explicitly override the type in the resolver to make it available
          // to later fields without re-visiting and re-constructing the resolver.
          // TODO: is this too hacky?
          r.byAst(entry.formal()).setType(useType);
        }

        // note that a substitution was used here
        if ((size_t) formalIdx >= formalsInstantiated.size()) {
          formalsInstantiated.resize(sig->numFormals());
        }
        formalsInstantiated.setBit(formalIdx, true);
      }

      formalIdx++;
    }

    if (!formal && ed) {
      // we're in an enum-based generated method; note the type in
      // formalTypes.
      formalTypes.push_back(useType);
    }

    // At this point, we have computed the instantiated type for this
    // formal. However, what we're still missing some information,
    // and furthermore, we have not enforced type query constraints.

    if (entry.isVarArgEntry()) {
      // Vararg entries don't get substitutions at this point, so
      // manually update type queries.
      if (auto vld = formal->toVarLikeDecl()) {
        if (auto te = vld->typeExpression()) {
            visitor.resolveTypeQueries(te, useType);
        }
      }
    } else if (formal) {
      // Substitutions have been updated; re-run resolution to get better
      // intents, vararg info, and to extract type query info.
      formal->traverse(visitor);
      formalType = getProperFormalType(r, entry, ad, formal);
    }

    // Type queries have now been computed. We need to verify that type
    // query constraints are matched. To do this, instruct resolver to avoid
    // using substitutions, and to preserve previously computed type query
    // info. This way, we'll get as output the type expression's QualifiedType
    // which incorporates type query info.
    if (formal) {
      if (auto vld = formal->toVarLikeDecl()) {
        if (vld->typeExpression()) {
          visitor.ignoreSubstitutionFor = formal;
          visitor.skipTypeQueries = true;
        }
      }
      formal->traverse(visitor);
      auto qFormalType = getProperFormalType(r, entry, ad, formal);
      if (entry.isVarArgEntry()) {
        // We only need to canPass the tuple element types.
        qFormalType = getVarArgTupleElemType(qFormalType);
      } else {
        // Explicitly override the type in the resolver to what we have found it
        // to be before the type-query-aware traversal.
        r.byAst(entry.formal()).setType(formalType);
      }

      auto checkType = !useType.isUnknown() ? useType : formalType;
      // With the type and query-aware type known, make sure that they're compatible
      auto passResult = canPass(context, checkType, qFormalType);
      if (!passResult.passes()) {
        // Type query constraints were not satisfied
        return ApplicabilityResult::failure(sig, passResult.reason(), entry.formalIdx());
      }

      if (fn != nullptr && fn->isMethod() && fn->thisFormal() == formal) {
        // Set the visitor's 'inCompositeType' property to the final
        // instantiation of 'this' so that we can correctly resolve methods.
        //
        // Also recompute receiver scopes based on the instantiated type so
        // that we correctly resolve the types of field identifiers.
        visitor.setCompositeType(formalType.type()->toCompositeType());
        visitor.receiverScopesComputed = false;
      }
    }
  }

  // instantiate the VarArg formal if necessary
  if (varargsTypes.size() > 0) {
    const TupleType* tup = sig->formalType(varArgIdx).type()->toTupleType();
    if (tup->isKnownSize() == false) {
      instantiateVarArgs = true;
    }

    if (instantiateVarArgs) {
      const TupleType* t = TupleType::getQualifiedTuple(context, varargsTypes);
      auto formal = faMap.byFormalIdx(varArgIdx).formal()->toVarArgFormal();
      QualifiedType vat = QualifiedType(formal->storageKind(), t);
      substitutions.insert({formal->id(), vat});
      r.byAst(formal).setType(vat);

      // note that a substitution was used here
      if ((size_t) varArgIdx >= formalsInstantiated.size()) {
        formalsInstantiated.resize(sig->numFormals());
      }
      formalsInstantiated.setBit(varArgIdx, true);
    }
  }

  // use the existing signature if there were no substitutions
  if (substitutions.size() == 0 && formalTypes.size() == 0) {
    // Even if no instantiations occurred due to formals, an initializer
    // might end up creating substitutions when we resolve its body
    // and process assignments like `this.someType = bla`. So, do not
    // short-circuit in that case.
    if (!isTfsForInitializer(sig)) {
      return ApplicabilityResult::success(sig);
    } else {
      // normally we do this when we add a substitution, but we haven't
      // added any substitutions yet.
      formalsInstantiated.resize(sig->numFormals());
    }
  }

  bool needsInstantiation = false;
  TypedFnSignature::WhereClauseResult where = TypedFnSignature::WHERE_NONE;

  if (fn != nullptr) {
    for (auto formal : fn->formals()) {
      if (auto varArgFormal = formal->toVarArgFormal()) {
        if (!varArgCountMatch(varArgFormal, r)) {
          return ApplicabilityResult::failure(sig->id(), FAIL_VARARG_MISMATCH);
        }
      }
    }

    // visit the where clause
    if (auto whereClause = fn->whereClause()) {
      whereClause->traverse(visitor);
    }
    // do not visit the return type or function body

    auto tmp = visitor.getFormalTypes(fn);
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
      Resolver::createForInstantiatedSignatureFields(context, ad, substitutions,
                                                     poiScope, r);
    // visit the parent type
    if (auto cls = ad->toClass()) {
      for (int i = 0; i < cls->numInheritExprs(); i++) {
        cls->inheritExpr(i)->traverse(visitor);
      }
    }

    // visit the field declarations
    for (auto child: ad->children()) {
      if (child->isVariable() ||
          child->isMultiDecl() ||
          child->isTupleDecl() ||
          child->isForwardingDecl()) {
        child->traverse(visitor);
      }
    }

    // add formals according to the parent class type

    // now pull out the field types
    CHPL_ASSERT(formalTypes.empty());
    int nFormals = sig->numFormals();
    int formalIdx = 0;

    // The "default value" hints are set in substitutions, but at this
    // point, we want to use the actual type that was computed. So,
    // rebuild the substitutions.
    SubstitutionsMap newSubstitutions;

    if (!sig->untyped()->isTypeConstructor()) {
      // Compiler-generated initializer has an initial 'this' formal,
      // skip it for now and insert a placeholder.
      formalIdx++;
      formalTypes.push_back(QualifiedType());
    }

    for (; formalIdx < nFormals; formalIdx++) {
      const Decl* fieldDecl = untypedSignature->formalDecl(formalIdx);
      const ResolvedExpression& e = r.byAst(fieldDecl);
      QualifiedType fieldType = e.type();
      QualifiedType sigType = sig->formalType(formalIdx);

      // use the same kind as the old formal type but update the type, param
      // to reflect how instantiation occurred.
      formalTypes.push_back(QualifiedType(sigType.kind(),
                                          fieldType.type(),
                                          fieldType.param()));

      if (isFieldSyntacticallyGeneric(context, fieldDecl->id())){
        // TODO
        newSubstitutions.insert({fieldDecl->id(), fieldType});
      }
    }

    if (!sig->untyped()->isTypeConstructor()) {
      // We've visited the rest of the formals and figured out their types.
      // Time to backfill the 'this' formal.
      const Type* newType = helpGetTypeForDecl(context, ad, newSubstitutions,
                                               poiScope, sig->formalType(0).type());

      // If the original formal is a class type (with management etc.), ensure
      // that the management etc. is preserved.
      if (auto sigCt = sig->formalType(0).type()->toClassType()) {
        if (auto mt = newType->toManageableType()) {
          newType = ClassType::get(context, mt, sigCt->manager(), sigCt->decorator());
        }
      }

      formalTypes[0] = QualifiedType(sig->formalType(0).kind(), newType);
    }

    needsInstantiation = anyFormalNeedsInstantiation(context, formalTypes,
                                                     untypedSignature,
                                                     &newSubstitutions);
  } else if (ed) {
    // Fine; formal types were stored into formalTypes earlier since we're
    // considering a compiler-generated candidate on an enum.
  } else {
    CHPL_ASSERT(false && "case not handled");
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

  // May need to resolve the body at this point to compute final TFS.
  if (ensureBodyIsResolved(context, call, result)) {
    if (!result->untyped()->isCompilerGenerated()) {
      if (isTfsForInitializer(result)) {
        auto resolvedFn = resolveInitializer(context, result, poiScope);
        auto newTfs = resolvedFn->signature();
        if (newTfs->needsInstantiation()) {
          context->error(newTfs->id(), "Failure to resolve initializer");
        } else {
          result = newTfs;
        }
      } else {
        CHPL_ASSERT(false && "Not handled yet!");
        std::ignore = resolveFunction(context, result, poiScope);
      }
    }
  }

  return ApplicabilityResult::success(result);
}

static const owned<ResolvedFunction>&
resolveFunctionByPoisQuery(Context* context,
                           const TypedFnSignature* sig,
                           PoiCallIdFnIds poiFnIdsUsed,
                           PoiRecursiveCalls recursiveFnsUsed) {
  QUERY_BEGIN(resolveFunctionByPoisQuery,
              context, sig, poiFnIdsUsed, recursiveFnsUsed);

  owned<ResolvedFunction> result;
  // the actual value is set in resolveFunctionByInfoQuery after it is
  // computed because computing it generates the poiFnIdsUsed which is
  // part of the key for this query.
  CHPL_ASSERT(false && "should not be reached");

  return QUERY_END(result);
}

// TODO: remove this workaround now that the build uses
// -Wno-dangling-reference
static const owned<ResolvedFunction>&
resolveFunctionByPoisQueryWrapper(Context* context,
                                  const TypedFnSignature* sig,
                                  const PoiInfo& poiInfo) {
  auto poiFnIdsUsedCopy = poiInfo.poiFnIdsUsed();
  auto recursiveFnsUsedCopy = poiInfo.recursiveFnsUsed();

  return resolveFunctionByPoisQuery(context, sig,
                                    std::move(poiFnIdsUsedCopy),
                                    std::move(recursiveFnsUsedCopy));
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

  // Note that in this case the AST for the function can be nullptr.
  if (isTfsForInitializer(sig)) {
    auto retType = QualifiedType(QualifiedType::VAR, VoidType::get(context));

    ResolutionResultByPostorderID resolutionById;
    auto visitor = Resolver::createForInitializer(context, fn, poiScope,
                                                  sig,
                                                  resolutionById);
    CHPL_ASSERT(visitor.initResolver.get());
    if (fn) {
      fn->body()->traverse(visitor);
      // then, set the return type
      visitor.returnType = retType;
      // then, resolve '=' and add any copy init/deinit calls as needed
      callInitDeinit(visitor);
      // then, handle return intent overloads and maybe-const formals
      adjustReturnIntentOverloadsAndMaybeConstRefs(visitor);
    }

    auto newTfsForInitializer = visitor.initResolver->finalize();

    // TODO: can this be encapsulated in a method?
    resolvedPoiInfo.swap(visitor.poiInfo);
    resolvedPoiInfo.setResolved(true);
    resolvedPoiInfo.setPoiScope(nullptr);

    // If we resolved an initializer, then we started with a function
    // signature that might have needed instantiation for the receiver.
    // We need to communicate to the query framework that the new TFS
    // does not need to have its corresponding function resolved.
    if (newTfsForInitializer != sig) {
      auto resolutionByIdCopy = resolutionById;
      auto resolvedInit = toOwned(new ResolvedFunction(newTfsForInitializer,
                                  fn->returnIntent(),
                                  std::move(resolutionByIdCopy),
                                  resolvedPoiInfo,
                                  visitor.returnType));
      QUERY_STORE_RESULT(resolveFunctionByPoisQuery,
                         context,
                         resolvedInit,
                         newTfsForInitializer,
                         resolvedPoiInfo.poiFnIdsUsed(),
                         resolvedPoiInfo.recursiveFnsUsed());
      auto& saved =
        resolveFunctionByPoisQueryWrapper(context, newTfsForInitializer,
                                          resolvedPoiInfo);
      const ResolvedFunction* resultInit = saved.get();
      QUERY_STORE_RESULT(resolveFunctionByInfoQuery,
                         context,
                         resultInit,
                         newTfsForInitializer,
                         poiInfo);
    }

    // If we resolved an initializer, the result should point to the
    // final, fully instantiated TFS that was created (if there is
    // one). In other cases, we just use the input signature.
    auto finalTfs = newTfsForInitializer ? newTfsForInitializer : sig;

    owned<ResolvedFunction> resolved
        = toOwned(new ResolvedFunction(finalTfs, fn->returnIntent(),
                  std::move(resolutionById),
                  resolvedPoiInfo,
                  visitor.returnType));

    // Store the result in the query under the POIs used.
    // If there was already a value for this revision, this
    // call will not update it. (If it did, that could lead to
    // memory errors).
    QUERY_STORE_RESULT(resolveFunctionByPoisQuery,
                       context,
                       resolved,
                       sig,
                       resolvedPoiInfo.poiFnIdsUsed(),
                       resolvedPoiInfo.recursiveFnsUsed());

  // On this path we are just resolving a normal function.
  } else if (fn) {
    ResolutionResultByPostorderID resolutionById;
    auto visitor = Resolver::createForFunction(context, fn, poiScope, sig,
                                               resolutionById);

    if (fn->body()) {
      fn->body()->traverse(visitor);
    }

    // then, compute the return type
    computeReturnType(visitor);

    // then, resolve '=' and add any copy init/deinit calls as needed
    callInitDeinit(visitor);

    // then, handle return intent overloads and maybe-const formals
    adjustReturnIntentOverloadsAndMaybeConstRefs(visitor);

    // check that throws are handled or forwarded
    checkThrows(context, resolutionById, fn);

    // TODO: can this be encapsulated in a method?
    resolvedPoiInfo.swap(visitor.poiInfo);
    resolvedPoiInfo.setResolved(true);
    resolvedPoiInfo.setPoiScope(nullptr);

    owned<ResolvedFunction> resolved
        = toOwned(new ResolvedFunction(sig, fn->returnIntent(),
                  std::move(resolutionById),
                  resolvedPoiInfo,
                  visitor.returnType));

    // Store the result in the query under the POIs used.
    // If there was already a value for this revision, this
    // call will not update it. (If it did, that could lead to
    // memory errors).
    QUERY_STORE_RESULT(resolveFunctionByPoisQuery,
                       context,
                       resolved,
                       sig,
                       resolvedPoiInfo.poiFnIdsUsed(),
                       resolvedPoiInfo.recursiveFnsUsed());

  } else {
    CHPL_ASSERT(false && "this query should be called on Functions");
  }

  // Return the unique result from the query (that might have been saved above)
  const owned<ResolvedFunction>& resolved =
    resolveFunctionByPoisQueryWrapper(context, sig, resolvedPoiInfo);

  const ResolvedFunction* result = resolved.get();

  return QUERY_END(result);
}

const ResolvedFunction* resolveInitializer(Context* context,
                                           const TypedFnSignature* sig,
                                           const PoiScope* poiScope) {
  bool isAcceptable = isTfsForInitializer(sig);
  if (!isAcceptable) {
    CHPL_ASSERT(false && "Should only be called for initializers");
  }

  // construct the PoiInfo for this case
  auto poiInfo = PoiInfo(poiScope);

  // lookup in the map using this PoiInfo
  return resolveFunctionByInfoQuery(context, sig, std::move(poiInfo));
}

static const ResolvedFunction* helpResolveFunction(Context* context,
                                                   const TypedFnSignature* sig,
                                                   const PoiScope* poiScope,
                                                   bool skipIfRunning) {

  // Forget about any inferred signature (to avoid resolving the
  // same function twice when working with inferred 'out' formals)
  sig = sig->inferredFrom();

  // this should only be applied to concrete fns or instantiations
  CHPL_ASSERT(!sig->needsInstantiation());

  // construct the PoiInfo for this case
  auto poiInfo = PoiInfo(poiScope);

  if (skipIfRunning) {
    if (context->isQueryRunning(resolveFunctionByInfoQuery,
                                std::make_tuple(sig, poiInfo))) {
      return nullptr;
    }
  }

  // lookup in the map using this PoiInfo
  return resolveFunctionByInfoQuery(context, sig, std::move(poiInfo));
}

const TypedFnSignature* inferRefMaybeConstFormals(Context* context,
                                                  const TypedFnSignature* sig,
                                                  const PoiScope* poiScope) {
  if (sig == nullptr) {
    return nullptr;
  }

  bool anyRefMaybeConstFormals = false;
  int numFormals = sig->numFormals();
  for (int i = 0; i < numFormals; i++) {
    const types::QualifiedType& ft = sig->formalType(i);
    if (ft.kind() == QualifiedType::REF_MAYBE_CONST) {
      anyRefMaybeConstFormals = true;
      break;
    }
  }

  if (anyRefMaybeConstFormals == false) {
    // nothing else to do here
    return sig;
  }

  // otherwise, try to resolve the body of the function
  const ResolvedFunction* rFn =
    helpResolveFunction(context, sig, poiScope, /* skipIfRunning */ true);

  if (rFn == nullptr)
    return nullptr; // give up if it would be a recursive query invocation

  // resolve the function body
  const UntypedFnSignature* untyped = sig->untyped();
  const ResolutionResultByPostorderID& rr = rFn->resolutionById();
  std::vector<types::QualifiedType> formalTypes;
  for (int i = 0; i < numFormals; i++) {
    const types::QualifiedType& ft = sig->formalType(i);
    if (ft.kind() == QualifiedType::REF_MAYBE_CONST) {
      formalTypes.push_back(rr.byAst(untyped->formalDecl(i)).type());
    } else {
      formalTypes.push_back(ft);
    }
  }

  const TypedFnSignature* result = nullptr;
  result = TypedFnSignature::getInferred(context,
                                         std::move(formalTypes),
                                         sig);
  return result;
}

const ResolvedFunction* resolveFunction(Context* context,
                                        const TypedFnSignature* sig,
                                        const PoiScope* poiScope) {
  // If there were outer variables, then do not bother trying to resolve.
  if (computeOuterVariables(context, sig->id())) return nullptr;
  return helpResolveFunction(context, sig, poiScope, /* skipIfRunning */ false);
}

const ResolvedFunction* resolveConcreteFunction(Context* context, ID id) {
  if (id.isEmpty())
    return nullptr;

  // If there were outer variables, then do not bother trying to resolve.
  if (computeOuterVariables(context, id)) return nullptr;

  const UntypedFnSignature* uSig = UntypedFnSignature::get(context, id);
  const TypedFnSignature* sig = typedSignatureInitial(context, uSig);

  if (sig == nullptr || sig->needsInstantiation()) {
    return nullptr;
  }

  auto whereFalse =
    resolution::TypedFnSignature::WhereClauseResult::WHERE_FALSE;
  if (sig->whereClauseResult() == whereFalse) {
    return nullptr;
  }

  const ResolvedFunction* ret = resolveFunction(context, sig, nullptr);
  return ret;
}

// This should be set when performing 'scopeResolveFunction', below.
static const owned<OuterVariables>&
computeOuterVariablesQuery(Context* context, ID id) {
  QUERY_BEGIN(computeOuterVariablesQuery, context, id);
  owned<OuterVariables> ret;
  CHPL_ASSERT(false && "Should not be called directly!");
  return QUERY_END(ret);
}

static owned<ResolvedFunction>
scopeResolveFunctionQueryBody(Context* context, ID id) {
  const AstNode* ast = parsing::idToAst(context, id);
  const Function* fn = ast->toFunction();

  ResolutionResultByPostorderID resolutionById;
  const TypedFnSignature* sig = nullptr;
  owned<ResolvedFunction> result;
  owned<OuterVariables> outerVars = toOwned(new OuterVariables(context, id));

  if (fn) {
    auto visitor =
      Resolver::createForScopeResolvingFunction(context, fn, resolutionById,
                                                std::move(outerVars));

    // visit the children of fn to scope resolve
    // (visiting the children because visiting a function will not
    //  cause it to be scope resolved).
    for (auto child: fn->children()) {
      child->traverse(visitor);

      // Recompute the method receiver after the 'this' formal is
      // scope-resolved, when we might be able to gather some information
      // about the type on which the method is declared.
      if (fn->isMethod() && child == fn->thisFormal()) {
        visitor.methodReceiverScopes(/*recompute=*/true);
      }
    }

    checkForParenlessMethodFieldRedefinition(context, fn, visitor);

    sig = visitor.typedSignature;

    if (!visitor.outerVars->isEmpty()) {
      std::swap(outerVars, visitor.outerVars);
    }
  }

  QUERY_STORE_RESULT(computeOuterVariablesQuery,
                     context,
                     std::move(outerVars),
                     id);

  result = toOwned(new ResolvedFunction(sig, fn->returnIntent(),
                                        std::move(resolutionById),
                                        PoiInfo(),
                                        QualifiedType()));
  return result;
}

static const owned<ResolvedFunction>&
scopeResolveFunctionQuery(Context* context, ID id) {
  QUERY_BEGIN(scopeResolveFunctionQuery, context, id);
  auto ret = scopeResolveFunctionQueryBody(context, id);
  return QUERY_END(ret);
}

const OuterVariables* computeOuterVariables(Context* context, ID id) {

  // For now, preemptively return 'nullptr' if 'id' is not a function.
  if (!parsing::idIsNestedFunction(context, id)) return nullptr;

  if (!context->hasCurrentResultForQuery(computeOuterVariablesQuery, { id })) {
    if (!context->isQueryRunning(scopeResolveFunctionQuery, { id })) {

      // The 'computeOuterVariablesQuery' is set as a side effect of
      // performing scope resolution, since both require a traversal.
      std::ignore = scopeResolveFunction(context, id);
    } else {

      // Just return 'nullptr', we have no results to use, yet. The caller
      // can only be the Resolver set up for scope-resolve if this branch
      // is happening.
      return nullptr;
    }
  }

  // We should have a result at this point.
  return computeOuterVariablesQuery(context, id).get();
}

const ResolvedFunction* scopeResolveFunction(Context* context,
                                                     ID id) {
  if (id.isEmpty())
    return nullptr;

  const owned<ResolvedFunction>& result =
    scopeResolveFunctionQuery(context, id);

  return result.get();
}

const ResolutionResultByPostorderID& scopeResolveAggregate(Context* context,
                                                           ID id) {
  QUERY_BEGIN(scopeResolveAggregate, context, id);

  const AggregateDecl* ad = parsing::idToAst(context, id)->toAggregateDecl();
  ResolutionResultByPostorderID result;

  if (ad) {
    // These are children, but are skipped by the is*Decl conditions below.
    for (auto inheritExpr : ad->inheritExprs()) {
      auto res = Resolver::createForScopeResolvingField(context, ad, inheritExpr, result);
      inheritExpr->traverse(res);
    }

    // TODO: Use some kind of "ad->fields()" iterator
    for (auto child : ad->children()) {
      if (child->isVarLikeDecl() ||
          child->isMultiDecl() ||
          child->isTupleDecl() ||
          child->isForwardingDecl()) {
        auto res = Resolver::createForScopeResolvingField(context, ad, child, result);
        child->traverse(res);
      }
    }
  }

  return QUERY_END(result);
}

const ResolutionResultByPostorderID& scopeResolveEnum(Context* context,
                                                      ID id) {
  QUERY_BEGIN(scopeResolveEnum, context, id);

  auto ed = parsing::idToAst(context, id)->toEnum();
  ResolutionResultByPostorderID result;

  if (ed) {
    for (auto child : ed->enumElements()) {
      auto res = Resolver::createForScopeResolvingEnumConstant(context, ed, child, result);
      child->traverse(res);
    }
  }

  return QUERY_END(result);
}


const ResolvedFunction* resolveOnlyCandidate(Context* context,
                                             const ResolvedExpression& r) {
  auto msc = r.mostSpecific().only();
  if (!msc) return nullptr;

  const TypedFnSignature* sig = msc.fn();
  const PoiScope* poiScope = r.poiScope();

  return resolveFunction(context, sig, poiScope);
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
  if (!ci.isOpCall() && ci.isMethodCall() != ufs->isMethod()) {
    return false;
  }

  // TODO: reason failed

  return true;
}

// given a typed function signature, determine if it applies to a call
static ApplicabilityResult
isInitialTypedSignatureApplicable(Context* context,
                                  const TypedFnSignature* tfs,
                                  const FormalActualMap& faMap,
                                  const CallInfo& ci) {
  if (!isUntypedSignatureApplicable(context, tfs->untyped(), faMap, ci)) {
    return ApplicabilityResult::failure(tfs->id(), /* TODO */ FAIL_CANDIDATE_OTHER);
  }

  // Next, check that the types are compatible
  int numVarArgActuals = 0;
  QualifiedType varArgType;
  for (const FormalActual& entry : faMap.byFormals()) {
    const auto& actualType = entry.actualType();

    // note: entry.actualType can have type()==nullptr and UNKNOWN.
    // in that case, resolver code should treat it as a hint to
    // use the default value. Unless the call used a ? argument.
    //
    // TODO: set a flag in the entry rather than relying on some encoded
    // property via QualifiedType.
    if (actualType.kind() == QualifiedType::UNKNOWN &&
        actualType.type() == nullptr &&
        !ci.hasQuestionArg()) {
      // use the default value - no need to check it matches formal
    } else {
      const auto& formalType = tfs->formalType(entry.formalIdx());
      CanPassResult got;
      if (entry.isVarArgEntry()) {
        if (varArgType.isUnknown()) {
          varArgType = formalType;
        }
        numVarArgActuals += 1;

        got = canPass(context, actualType, getVarArgTupleElemType(formalType));
      } else {
        got = canPass(context, actualType, formalType);
      }
      if (!got.passes()) {
        return ApplicabilityResult::failure(tfs, got.reason(), entry.formalIdx());
      }
    }
  }

  if (!varArgType.isUnknown()) {
    const TupleType* tup = varArgType.type()->toTupleType();
    if (tup != nullptr && tup->isVarArgTuple() &&
        tup->isKnownSize() && numVarArgActuals != tup->numElements()) {
      return ApplicabilityResult::failure(tfs->id(), FAIL_VARARG_MISMATCH);
    }
  }

  // check that the where clause applies
  auto whereResult = tfs->whereClauseResult();
  if (whereResult == TypedFnSignature::WHERE_FALSE) {
    return ApplicabilityResult::failure(tfs->id(), FAIL_WHERE_CLAUSE);
  }

  return ApplicabilityResult::success(tfs);
}

// returns nullptr if the candidate is not applicable,
// or the result of typedSignatureInitial if it is.
static ApplicabilityResult
doIsCandidateApplicableInitial(Context* context,
                               const ID& candidateId,
                               const CallInfo& ci) {
  AstTag tag = asttags::AST_TAG_UNKNOWN;

  if (!candidateId.isEmpty()) {
    tag = parsing::idToTag(context, candidateId);
  }

  // if it's a paren-less call, only consider parenless routines
  // (including generated field accessors) but not types/outer variables/
  // calls with parens.
  if (ci.isParenless()) {
    if (parsing::idIsParenlessFunction(context, candidateId) ||
        parsing::idIsField(context, candidateId)) {
      // OK
    } else {
      return ApplicabilityResult::failure(candidateId, FAIL_PARENLESS_MISMATCH);
    }
  }

  if (isTypeDecl(tag)) {
    // calling a type - i.e. type construction
    const Type* t = initialTypeForTypeDecl(context, candidateId);
    return ApplicabilityResult::success(typeConstructorInitial(context, t));
  }

  // not a candidate
  if (ci.isMethodCall() && isFormal(tag)) {
    return ApplicabilityResult::failure(candidateId, /* TODO */ FAIL_CANDIDATE_OTHER);
  }

  if (isVariable(tag)) {
    if (ci.isParenless() && ci.isMethodCall() && ci.numActuals() == 1) {
      // calling a field accessor
      //
      // TODO: This doesn't have anything to do with this candidate. Shouldn't
      // we be handling this somewhere else?
      if (auto ct = ci.actual(0).type().type()->getCompositeType()) {
        if (auto containingType = isNameOfField(context, ci.name(), ct)) {
          auto ret = fieldAccessor(context, containingType, ci.name());
          return ApplicabilityResult::success(ret);
        }
      }
    }
    // not a candidate
    return ApplicabilityResult::failure(candidateId, /* TODO */ FAIL_CANDIDATE_OTHER);
  }

  if (!isFunction(tag)) {
    context->error(candidateId, "Found non-function where function was expected");
    return ApplicabilityResult::failure(candidateId, /* TODO */ FAIL_CANDIDATE_OTHER);
  }

  if (ci.isMethodCall() && (ci.name() == "init" || ci.name() == "init=")) {
    // TODO: test when record has defaults for type/param fields
    auto recv = ci.calledType();
    auto fn = parsing::idToAst(context, candidateId)->toFunction();
    ResolutionResultByPostorderID r;
    auto vis = Resolver::createForInitialSignature(context, fn, r);
    fn->thisFormal()->traverse(vis);
    auto res = vis.byPostorder.byAst(fn->thisFormal());

    auto got = canPass(context, recv, res.type());
    // Allow passing directly or via implicit borrowing only.
    if (!got.passes() || (got.converts() && !got.convertsWithBorrowing())) {
      return ApplicabilityResult::failure(candidateId,
                                          /* TODO */ FAIL_CANDIDATE_OTHER);
    }
  }

  auto ufs = UntypedFnSignature::get(context, candidateId);
  auto faMap = FormalActualMap(ufs, ci);
  auto ret = typedSignatureInitial(context, ufs);

  if (!ret) {
    return ApplicabilityResult::failure(candidateId, /* TODO */ FAIL_CANDIDATE_OTHER);
  }

  return isInitialTypedSignatureApplicable(context, ret, faMap, ci);
}

// returns nullptr if the candidate is not applicable,
// or the result of an instantiated typedSignature if it is.
static ApplicabilityResult
doIsCandidateApplicableInstantiating(Context* context,
                                     const TypedFnSignature* typedSignature,
                                     const CallInfo& call,
                                     const PoiScope* poiScope) {

  auto instantiated =
    instantiateSignature(context, typedSignature, call, poiScope);

  if (!instantiated.success())
    return instantiated;

  // check that the where clause applies
  if (instantiated.candidate()->whereClauseResult() == TypedFnSignature::WHERE_FALSE)
    return ApplicabilityResult::failure(typedSignature->id(), FAIL_WHERE_CLAUSE);

  return instantiated;
}

static ApplicabilityResult const&
isCandidateApplicableInitialQuery(Context* context,
                                  ID candidateId,
                                  CallInfo call) {

  QUERY_BEGIN(isCandidateApplicableInitialQuery, context, candidateId, call);

  auto result =
    doIsCandidateApplicableInitial(context, candidateId, call);

  return QUERY_END(result);
}

static const std::pair<CandidatesAndForwardingInfo,
                       std::vector<ApplicabilityResult>>&
filterCandidatesInitialGatherRejected(Context* context,
                                      std::vector<BorrowedIdsWithName> lst,
                                      CallInfo call, bool gatherRejected) {
  QUERY_BEGIN(filterCandidatesInitialGatherRejected, context, lst, call, gatherRejected);

  CandidatesAndForwardingInfo matching;
  std::vector<ApplicabilityResult> rejected;

  for (const BorrowedIdsWithName& ids : lst) {
    for (const ID& id : ids) {
      auto s = isCandidateApplicableInitialQuery(context, id, call);
      if (s.success()) {
        matching.addCandidate(s.candidate());
      } else if (gatherRejected) {
        rejected.push_back(s);
      }
    }
  }

  auto result = std::make_pair(std::move(matching), std::move(rejected));
  return QUERY_END(result);
}

const CandidatesAndForwardingInfo&
filterCandidatesInitial(Context* context,
                        std::vector<BorrowedIdsWithName> lst,
                        CallInfo call) {
  auto& result = filterCandidatesInitialGatherRejected(context, std::move(lst),
                                                       call, /* gatherRejected */ false);
  return result.first;
}

void
filterCandidatesInstantiating(Context* context,
                              const CandidatesAndForwardingInfo& lst,
                              const CallInfo& call,
                              const Scope* inScope,
                              const PoiScope* inPoiScope,
                              CandidatesAndForwardingInfo& result,
                              std::vector<ApplicabilityResult>* rejected) {

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

      auto instantiated =
        doIsCandidateApplicableInstantiating(context,
                                             typedSignature,
                                             call,
                                             instantiationPoiScope);
      if (instantiated.success()) {
        result.addCandidate(instantiated.candidate());
      } if (rejected) {
        rejected->push_back(std::move(instantiated));
      }
    } else {
      // if it's already concrete, we already know it is a candidate.
      result.addCandidate(typedSignature);
    }
  }
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

  // resolve the body, if it is not already being resolved
  const ResolvedFunction* r = helpResolveFunction(context, signature, poiScope,
                                                  /* skipIfRunning */ true);
  if (r == nullptr) {
    // If it's a recursive call, track it in the PoiInfo
    poiInfo.accumulateRecursive(signature, poiScope);
  } else {
    // gather the POI scopes from instantiating the function body
    poiInfo.accumulate(r->poiInfo());
  }
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
      return ClassType::get(context, AnyClassType::get(context), nullptr, ClassTypeDecorator(ClassTypeDecorator::UNMANAGED));
    } else if (name == USTR("borrowed")) {
      return ClassType::get(context, AnyClassType::get(context), nullptr, ClassTypeDecorator(ClassTypeDecorator::BORROWED));
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

  if (t == nullptr || !(t->isManageableType() || t->isClassType())) {
    if (t != nullptr && !t->isUnknownType()) {
      context->error(astForErr, "invalid class type construction");
    }
    return ErroneousType::get(context);
  }

  const ManageableType* mt = nullptr;
  if (auto ct = t->toClassType()) {
    mt = ct->manageableType();
    // get nilability from ct
    if (ct->decorator().isNilable())
      d = d.addNilable();
    if (ct->decorator().isNonNilable())
      d = d.addNonNil();
  } else {
    mt = t->toManageableType();
  }

  CHPL_ASSERT(mt);
  return ClassType::get(context, mt, manager, d);
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
      } else if (name == USTR("real")) {
        return AnyRealType::get(context);
      } else if (name == USTR("imag")) {
        return AnyImagType::get(context);
      } else if (name == USTR("complex")) {
        return AnyComplexType::get(context);
      } else {
        CHPL_ASSERT(false && "should not be reachable");
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

/*
  gets either a c_ptr or c_ptrConst type depending on the name in the CallInfo
*/
static const Type* getCPtrType(Context* context,
                               const AstNode* astForErr,
                               const CallInfo& ci) {
  UniqueString name = ci.name();
  bool isConst;

  // 'typeForId' should have prepared this for us if 'CTypes' was in scope.
  auto called = ci.calledType();
  if (!(called.hasTypePtr() && called.type()->isCPtrType())) {
    return nullptr;
  }

  if (name == USTR("c_ptr")) {
    isConst = false;
  } else if (name == USTR("c_ptrConst")) {
    isConst = true;
  } else {
    return nullptr;
  }
  // Should we compute the generic version of the type (e.g. c_ptr(?)/c_ptrConst(?)
  bool useGenericType = false;

  // There should be 0 or 1 actuals depending on if it is ?
  if (ci.hasQuestionArg()) {
    // handle c_ptr(?)/c_ptrConst(?)
    if (ci.numActuals() != 0) {
      context->error(astForErr, "invalid %s type construction", name.c_str());
      return ErroneousType::get(context);
    }
    useGenericType = true;
  } else {
    // handle c_ptr(?t) or c_ptr(eltT)/c_ptrConst(?t) or c_ptrConst(eltT)
    if (ci.numActuals() != 1) {
      context->error(astForErr,"invalid %s type construction", name.c_str());
      return ErroneousType::get(context);
    }

    QualifiedType qt = ci.actual(0).type();
    if (qt.type() && qt.type()->isAnyType()) {
      useGenericType = true;
    }
  }

  if (useGenericType) {
    return isConst ? CPtrType::getConst(context) : CPtrType::get(context);
  }

  QualifiedType qt;
  CHPL_ASSERT(ci.numActuals() > 0);
  qt = ci.actual(0).type();

  const Type* t = qt.type();
  if (t == nullptr) {
    // Details not yet known so return UnknownType
    return UnknownType::get(context);
  } else if (t->isUnknownType() || t->isErroneousType()) {
    // Just propagate the Unknown / Erroneous type
    // without raising any errors
    return t;
  }

  if (!qt.isType()) {
    // raise an error b/c of type mismatch
    context->error(astForErr,"invalid %s type construction", name.c_str());
    return ErroneousType::get(context);
  } else {
    return isConst ? CPtrType::getConst(context, t) :
                     CPtrType::get(context, t);
  }
}

static const Type*
convertClassTypeToNilable(Context* context, const Type* t) {
  const ClassType* ct = nullptr;

  if (auto bct = t->toManageableType()) {
    auto d = ClassTypeDecorator(ClassTypeDecorator::GENERIC_NONNIL);
    ct = ClassType::get(context, bct, nullptr, d);
  } else {
    ct = t->toClassType();
  }

  if (ct) {
    // get the nilable version of the class type
    ClassTypeDecorator d = ct->decorator().addNilable();
    auto ret= ct->withDecorator(context, d);
    return ret;
  }

  return nullptr;
}

// Resolving compiler-supported type-returning patterns
// 'call' and 'inPoiScope' are used for the location for error reporting.
static const Type* resolveBuiltinTypeCtor(Context* context,
                                          const AstNode* astForErr,
                                          const CallInfo& ci) {
  // none of the special type function calls are methods; we can stop here.
  if (ci.isMethodCall()) return nullptr;

  if (ci.name() == USTR("?")) {
    if (ci.numActuals() > 0) {
      if (const Type* t = ci.actual(0).type().type()) {
        if (auto nilable = convertClassTypeToNilable(context, t)) {
          return nilable;
        }
      }
    }
  }

  if (ci.name() == USTR("*") && ci.numActuals() == 2) {
    auto first = ci.actual(0).type();
    auto second = ci.actual(1).type();
    if (first.isParam() && first.type()->isIntType() &&
        second.isType()) {
      return TupleType::getStarTuple(context, first, second);
    }
  }

  if (auto t = getManagedClassType(context, astForErr, ci)) {
    return t;
  }

  if (auto t = getNumericType(context, astForErr, ci)) {
    return t;
  }

  if (auto t = getCPtrType(context, astForErr, ci)) {
    return t;
  }

  return nullptr;
}

static
bool resolvePostfixNilableAppliedToNew(Context* context, const Call* call,
                                       const CallInfo& ci,
                                       QualifiedType& exprTypeOut) {

  // First, pattern match to find something like 'new C()?'...
  if (!call || !call->isOpCall()) return false;

  auto opCall = call->toOpCall();
  if (opCall->op() != USTR("?") || opCall->numActuals() != 1) return false;

  auto newCall = opCall->actual(0)->toFnCall();
  if (!newCall || !newCall->calledExpression() ||
      !newCall->calledExpression()->isNew()) {
    return false;
  }

  // Now, adjust the type to be nilable, but not the kind.
  auto qtNewCall = ci.actual(0).type();

  if (qtNewCall.isUnknown() || qtNewCall.isErroneousType()) {
    exprTypeOut = qtNewCall;
  }

  auto convToNilable = convertClassTypeToNilable(context, qtNewCall.type());
  auto outType = convToNilable ? convToNilable : qtNewCall.type();

  exprTypeOut = QualifiedType(qtNewCall.kind(), outType);

  return true;
}

// Resolving calls for certain compiler-supported patterns
// without requiring module implementations exist at all.
static bool resolveFnCallSpecial(Context* context,
                                 const AstNode* astForErr,
                                 const CallInfo& ci,
                                 QualifiedType& exprTypeOut) {
  // TODO: .borrow()
  // TODO: chpl__coerceCopy

  // explicit param casts are resolved here
  if (ci.isOpCall() && ci.name() == USTR(":")) {
    auto src = ci.actual(0).type();
    auto dst = ci.actual(1).type();

    auto targetParamGuess = Param::tryGuessParamTagFromType(dst.type());
    if (!targetParamGuess) {
      // We're casting a param value, but the destination type can't be
      // a param. This should be treated as a non-special cast.
      return false;
    }

    bool isDstType = dst.kind() == QualifiedType::TYPE;
    bool isParamTypeCast = src.kind() == QualifiedType::PARAM && isDstType;

    if (isParamTypeCast) {
        auto srcEnumType = src.type()->toEnumType();
        auto dstEnumType = dst.type()->toEnumType();
        if (srcEnumType && srcEnumType->isAbstract()) {
          exprTypeOut = CHPL_TYPE_ERROR(context, EnumAbstract, astForErr, "from", srcEnumType, dst.type());
          return true;
        } else if (dstEnumType && dstEnumType->isAbstract()) {
          exprTypeOut = CHPL_TYPE_ERROR(context, EnumAbstract, astForErr, "to", dstEnumType, src.type());
          return true;
        } else if (srcEnumType && dst.type()->toNothingType()) {
          auto fromName = tagToString(src.type()->tag());
          context->error(astForErr, "illegal cast from %s to nothing", fromName);
          exprTypeOut = QualifiedType(QualifiedType::UNKNOWN,
                                      ErroneousType::get(context));
          return true;
        }

        exprTypeOut = Param::fold(context, astForErr,
                                  uast::PrimitiveTag::PRIM_CAST, src, dst);
        return true;
    } else if (src.isType() && dst.hasTypePtr() && dst.type()->isStringType()) {
      // handle casting a type name to a string
      std::ostringstream oss;
      src.type()->stringify(oss, chpl::StringifyKind::CHPL_SYNTAX);
      auto ustr = UniqueString::get(context, oss.str());
      exprTypeOut = QualifiedType(QualifiedType::PARAM,
                                  RecordType::getStringType(context),
                                  StringParam::get(context, ustr));
      return true;
    } else if (!isDstType) {
      // trying to cast to something that's not a type
      auto toName = tagToString(dst.type()->tag());
      auto fromName = tagToString(src.type()->tag());
      context->error(astForErr, "illegal cast from %s to %s", fromName, toName);
      exprTypeOut = QualifiedType(QualifiedType::UNKNOWN,
                                  ErroneousType::get(context));
      return true;
    }
  }

  if ((ci.name() == USTR("==") || ci.name() == USTR("!=")) &&
      ci.numActuals() == 2) {
    auto lhs = ci.actual(0).type();
    auto rhs = ci.actual(1).type();

    bool bothType = lhs.kind() == QualifiedType::TYPE &&
                    rhs.kind() == QualifiedType::TYPE;
    bool bothParam = lhs.kind() == QualifiedType::PARAM &&
                     rhs.kind() == QualifiedType::PARAM;
    if (bothType || bothParam) {
      bool result = lhs == rhs;
      result = ci.name() == USTR("==") ? result : !result;
      exprTypeOut = QualifiedType(QualifiedType::PARAM, BoolType::get(context),
                                  BoolParam::get(context, result));
      return true;
    }
  }

  if (ci.isOpCall() && ci.name() == USTR("!") && ci.numActuals() == 1) {
    auto qt = ci.actual(0).type();
    if (qt.kind() == QualifiedType::PARAM && qt.hasParamPtr() &&
        qt.hasTypePtr() && qt.type()->isBoolType()) {
      exprTypeOut = qt.param()->fold(context, astForErr,
                                     chpl::uast::PrimitiveTag::PRIM_UNARY_LNOT,
                                     qt, QualifiedType());
      return true;
    }
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
    exprTypeOut = QualifiedType(QualifiedType::PARAM, BoolType::get(context),
                                BoolParam::get(context, result));
    return true;
  }

  if (ci.name() == USTR("this") && ci.numActuals() == 2) {
    // compiler-defined 'this' operator for param-indexed tuples
    auto thisType = ci.actual(0).type();
    auto second = ci.actual(1).type();
    if (thisType.hasTypePtr() && thisType.type()->isTupleType() &&
        second.isParam() && second.hasParamPtr() &&
        second.type()->isIntType()) {
      auto tup = thisType.type()->toTupleType();
      auto val = second.param()->toIntParam()->value();
      if (val < 0 || val >= tup->numElements()) {
        CHPL_REPORT(context, TupleIndexOOB, astForErr->toCall(), tup, val);
        exprTypeOut = QualifiedType(QualifiedType::UNKNOWN, ErroneousType::get(context));
      } else {
        auto member = tup->elementType(val);
        exprTypeOut = member;
      }
      return true;
    }
  }

  return false;
}

static bool resolveFnCallSpecialType(Context* context,
                                     const Call* call,
                                     const CallInfo& ci,
                                     const CallScopeInfo& inScopes,
                                     CallResolutionResult& result) {
  if (ci.isMethodCall()) {
    return false;
  }

  // Types that can be computed without resolving other calls
  if (const Type* t = resolveBuiltinTypeCtor(context, call, ci)) {
    auto exprTypeOut = QualifiedType(QualifiedType::TYPE, t);
    result = CallResolutionResult(exprTypeOut);
    return true;
  }

  // Types that require resolving some kind of helper function to build
  // the type.
  //
  // TODO: sync, single
  if (ci.name() == "domain") {
    // TODO: a compiler-generated type constructor would be simpler, but we
    // don't support default values on compiler-generated methods because the
    // default values require existing AST.

    // Note: 'dmapped' is treated like a binary operator at the moment, so
    // we don't need to worry about distribution type for 'domain(...)' exprs.

    // Transform domain type expressions like `domain(arg1, ...)` into:
    //   _domain.static_type(arg1, ...)
    auto genericDom = DomainType::getGenericDomainType(context);
    auto recv = QualifiedType(QualifiedType::TYPE, genericDom);
    auto typeCtorName = UniqueString::get(context, "static_type");
    auto ctorCall = CallInfo::createWithReceiver(ci, recv, typeCtorName);

    result = resolveCall(context, call, ctorCall, inScopes);
    return true;
  } else if (ci.name() == "atomic") {
    auto newName = UniqueString::get(context, "chpl__atomicType");
    auto ctorCall = CallInfo::copyAndRename(ci, newName);
    result = resolveCall(context, call, ctorCall, inScopes);
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

  CandidatesAndForwardingInfo initialCandidates;
  CandidatesAndForwardingInfo candidates;

  CHPL_ASSERT(ci.calledType().type() != nullptr);
  CHPL_ASSERT(!ci.calledType().type()->isUnknownType());

  auto initial = typeConstructorInitial(context, ci.calledType().type());
  initialCandidates.addCandidate(initial);

  // TODO: do something for partial instantiation

  filterCandidatesInstantiating(context,
                                initialCandidates,
                                ci,
                                inScope,
                                inPoiScope,
                                candidates,
                                /* rejected */ nullptr);


  // find most specific candidates / disambiguate
  // Note: at present there can only be one candidate here
  MostSpecificCandidates mostSpecific =
      findMostSpecificCandidates(context, candidates, ci, inScope, inPoiScope);

  return mostSpecific;
}

static const TypedFnSignature*
considerCompilerGeneratedMethods(Context* context,
                                 const CallInfo& ci,
                                 const Scope* inScope,
                                 const PoiScope* inPoiScope,
                                 CandidatesAndForwardingInfo& candidates) {
  // only consider compiler-generated methods and opcalls, for now
  if (!ci.isMethodCall() && !ci.isOpCall()) return nullptr;

  // fetch the receiver type info
  CHPL_ASSERT(ci.numActuals() >= 1);
  auto& receiver = ci.actual(0);
  auto receiverType = receiver.type();

  // if not compiler-generated, then nothing to do
  if (!needCompilerGeneratedMethod(context, receiverType.type(), ci.name(),
                                   ci.isParenless())) {
    return nullptr;
  }

  // get the compiler-generated function, may be generic
  auto tfs = getCompilerGeneratedMethod(context, receiverType, ci.name(),
                                        ci.isParenless());
  return tfs;
}

// not all compiler-generated procs are method. For instance, the compiler
// generates to-and-from integral casts for enums. In the to-casts, the
// receiver (or lhs) is an integral, not an enum.
//
// This helper serves to consider compiler-generated functions that can't
// be guessed based on the first argument.
static const TypedFnSignature*
considerCompilerGeneratedOperators(Context* context,
                                   const CallInfo& ci,
                                   const Scope* inScope,
                                   const PoiScope* inPoiScope,
                                   CandidatesAndForwardingInfo& candidates) {
  if (!ci.isOpCall()) return nullptr;

  // Avoid invoking the query if we don't need a binary operation here.
  if (ci.name() != USTR(":") || ci.numActuals() != 2) return nullptr;

  auto lhsType = ci.actual(0).type();
  auto rhsType = ci.actual(1).type();
  if (!(lhsType.type() && lhsType.type()->isEnumType()) &&
      !(rhsType.type() && rhsType.type()->isEnumType())) {
    return nullptr;
  }

  auto tfs = getCompilerGeneratedBinaryOp(context, lhsType, rhsType, ci.name());
  return tfs;
}

static std::vector<std::tuple<const Decl*, QualifiedType>>
collectGenericFormals(Context* context, const TypedFnSignature* tfs) {
  std::vector<std::tuple<const Decl*, QualifiedType>> ret;

  // Skip the 'this' formal since it will always be generic if one of the
  // "real" formals is generic.
  int formalIdx = 0;
  if (tfs->untyped()->formalName(0) == USTR("this")) {
    formalIdx++;
  }

  for (; formalIdx < tfs->numFormals(); formalIdx++) {
    auto formalType = tfs->formalType(formalIdx);
    auto formalDecl = tfs->untyped()->formalDecl(formalIdx);
    if (formalNeedsInstantiation(context, formalType, formalDecl, /* substitutions */ nullptr)) {
      ret.push_back(std::make_tuple(formalDecl, formalType));
    }
  }
  return ret;
}

static void
considerCompilerGeneratedCandidates(Context* context,
                                   const AstNode* astForErr,
                                   const CallInfo& ci,
                                   const Scope* inScope,
                                   const PoiScope* inPoiScope,
                                   CandidatesAndForwardingInfo& candidates,
                                   std::vector<ApplicabilityResult>* rejected) {
  const TypedFnSignature* tfs = nullptr;

  tfs = considerCompilerGeneratedMethods(context, ci, inScope, inPoiScope, candidates);
  if (tfs == nullptr) {
    tfs = considerCompilerGeneratedOperators(context, ci, inScope, inPoiScope, candidates);
  }

  if (!tfs) return;

  // check if the initial signature matches
  auto faMap = FormalActualMap(tfs->untyped(), ci);
  if (!isInitialTypedSignatureApplicable(context, tfs, faMap, ci).success()) {
    return;
  }

  // OK, already concrete, store and return
  if (!tfs->needsInstantiation()) {
    candidates.addCandidate(tfs);
    return;
  }

  // need to instantiate before storing
  auto poi = pointOfInstantiationScope(context, inScope, inPoiScope);
  auto instantiated = doIsCandidateApplicableInstantiating(context,
                                                           tfs,
                                                           ci,
                                                           poi);
  if (!instantiated.success()) {
    // failed when instantiating, likely due to dependent types.
    if (rejected) rejected->push_back(instantiated);
    return;
  }

  if (instantiated.candidate()->needsInstantiation()) {
    CHPL_REPORT(context, MissingFormalInstantiation,
                astForErr,
                collectGenericFormals(context, instantiated.candidate()));
    return; // do not push invalid candidate into list
  }

  candidates.addCandidate(instantiated.candidate());
}

static std::vector<BorrowedIdsWithName>
lookupCalledExpr(Context* context,
                 const Scope* scope,
                 const CallInfo& ci,
                 CheckedScopes& visited) {

  Resolver::ReceiverScopesVec receiverScopes;

  // For method calls, also consider the receiver scope.
  if (ci.isMethodCall() || ci.isOpCall()) {
    // TODO: should types of all arguments be considered for an op call?
    CHPL_ASSERT(ci.numActuals() >= 1);
    auto& qtReceiver = ci.actual(0).type();
    if (auto t = qtReceiver.type()) {
      if (auto compType = t->getCompositeType()) {
        receiverScopes =
          Resolver::gatherReceiverAndParentScopesForType(context, compType);
      } else if (auto cptr = t->toCPtrType()) {
        receiverScopes.push_back(scopeForId(context, cptr->id(context)));
      } else if (const ExternType* et = t->toExternType()) {
        receiverScopes.push_back(scopeForId(context, et->id()));
      }
    }
  }

  LookupConfig config = LOOKUP_DECLS | LOOKUP_IMPORT_AND_USE | LOOKUP_PARENTS;

  // For parenless non-method calls, only find the innermost match
  if (ci.isParenless() && !ci.isMethodCall()) {
    config |= LOOKUP_INNERMOST;
  }

  if (ci.isMethodCall()) {
    config |= LOOKUP_ONLY_METHODS_FIELDS;
  }

  if (ci.isOpCall()) {
    config |= LOOKUP_METHODS;
  }

  UniqueString name = ci.name();

  auto ret = lookupNameInScopeWithSet(context, scope, receiverScopes, name,
                                      config, visited);

  return ret;
}

// Container for ordering groups of last resort candidates by resolution
// preference.
struct LastResortCandidateGroups {

  // Combine another set of groups into this one, consuming the other one.
  // For use with candidates from multiple potential forwarding types.
  void mergeWithGroups(LastResortCandidateGroups other) {
    // merge non-poi candidate group and forwarding info
    if (other.nonPoi) {
      if (nonPoi) {
        nonPoi->takeFromOther(*other.nonPoi);
      } else {
        nonPoi = std::move(other.nonPoi);
      }
    }

    // merge poi candidate groups and forwarding info at corresponding indexes
    for (size_t i = 0; i < std::max(this->poi.size(), other.poi.size());
         i++) {
      if (i < this->poi.size() && i < other.poi.size()) {
        // both have a poi candidates group at this index
        this->poi[i].takeFromOther(other.poi[i]);
      } else if (i < this->poi.size()) {
        // only this one has a group here, nothing to do
      } else {
        // only the other has a group
        this->poi.push_back(std::move(other.poi[i]));
      }
    }

    // recurse into other's forwarding groups
    if (forwardingCandidateGroups) {
      forwardingCandidateGroups->mergeWithGroups(
          std::move(other.getForwardingGroups()));
    } else {
      forwardingCandidateGroups = std::move(other.forwardingCandidateGroups);
    }
  }

  // Get the LastResortCandidateGroups for forwarded-to candidates,
  // creating it first if it does not exist.
  LastResortCandidateGroups& getForwardingGroups() {
    if (!forwardingCandidateGroups) {
      forwardingCandidateGroups = std::make_unique<LastResortCandidateGroups>();
    }
    return *forwardingCandidateGroups;
  }

  // Get the most preferred candidates group that has candidates, if there is
  // one. Otherwise, return an empty candidates group.
  // Out-params:
  // - firstPoiCandidate: Index of the first poi candidate. Presumed 0 to start
  //   with, and set to the end of the non-poi candidates if they are returned.
  // - forwardingInfo: Forwarding info for the returned group, set if it comes
  //   from forwarding.
  const CandidatesAndForwardingInfo firstNonEmptyCandidatesGroup(
      size_t* firstPoiCandidate) const {
    if (nonPoi && !nonPoi->empty()) {
      *firstPoiCandidate = nonPoi->size();
      return *nonPoi;
    }
    for (size_t i = 0; i < poi.size(); i++) {
      if (!poi[i].empty()) {
        return poi[i];
      }
    }
    if (forwardingCandidateGroups) {
      return forwardingCandidateGroups->firstNonEmptyCandidatesGroup(
          firstPoiCandidate);
    }
    return CandidatesAndForwardingInfo();
  }

  void addNonPoiCandidates(CandidatesAndForwardingInfo&& group) {
    CHPL_ASSERT(!nonPoi && "non poi candidates already set");
    this->nonPoi = std::move(group);
  }

  void addPoiCandidates(CandidatesAndForwardingInfo&& group) {
    CHPL_ASSERT(nonPoi && "setting poi candidates before non poi");
    this->poi.push_back(std::move(group));
  }

 private:
  // Non-poi candidates (most preferred).
  chpl::optional<CandidatesAndForwardingInfo> nonPoi;
  // Poi candidates from innermost (more preferred) to outermost scope.
  std::vector<CandidatesAndForwardingInfo> poi;

  // A LastResortCandidateGroups for candidates found via forwarding from the
  // site of the current group. This is effectively a linked list due to the
  // possibility of forwards-to-forwards.
  owned<LastResortCandidateGroups> forwardingCandidateGroups = nullptr;
};

// Returns candidates with last resort candidates removed and saved in a
// separate list.
static void filterCandidatesLastResort(
    Context* context, const CandidatesAndForwardingInfo& list, CandidatesAndForwardingInfo& result,
    CandidatesAndForwardingInfo& lastResort) {
  for (auto& candidate : list) {
    auto attrs =
        parsing::idToAttributeGroup(context, candidate->untyped()->id());
    if (attrs && attrs->hasPragma(PRAGMA_LAST_RESORT)) {
      lastResort.addCandidate(candidate);
    } else {
      result.addCandidate(candidate);
    }
  }
}

// this function gathers candidates not from POI and candidates
// from POI into separate vectors.
// For each of these vectors, the corresponding forwardingTo vector
// will have an element for each of the returned candidates &
// indicates the actual type that is passed as the method receiver
// when using forwarding.
static void
gatherAndFilterCandidatesForwarding(Context* context,
                                    const AstNode* astForErr,
                                    const Call* call,
                                    const CallInfo& ci,
                                    const CallScopeInfo& inScopes,
                                    CandidatesAndForwardingInfo& nonPoiCandidates,
                                    CandidatesAndForwardingInfo& poiCandidates,
                                    LastResortCandidateGroups& lrcGroups,
                                    std::vector<ApplicabilityResult>* rejected) {

  const Type* receiverType = ci.actual(0).type().type();

  // Resolve the forwarding expression's types & decide if we
  // want to consider forwarding.
  ResolvedFields forwards;
  UniqueString name = ci.name();
  if (name == USTR("init") || name == USTR("init=") || name == USTR("deinit")) {
    // these are exempt from forwarding
  } else if (auto ct = receiverType->getCompositeType()) {
    auto useDefaults = DefaultsPolicy::USE_DEFAULTS;
    const ResolvedFields& fields = fieldsForTypeDecl(context, ct,
                                                     useDefaults);

    if (context->isQueryRunning(resolveForwardingExprs, std::make_tuple(ct))) {
      // If we are trying to resolve a method call while collecting forwarding
      // candidates, do not try to use forwarding to resolve that method.
      //
      // TODO: this may not be sufficient. For instance, we may have multiple
      // forwarding statements in succession that bring in necessary methods:
      //
      //     forwarding var hasSomething: SomeType;
      //     forwarding methodFromHasSomething();
      //
      // A more robust approach might split forwarding statement resolution
      // into individual queries, and skip the code below only if the current
      // forwarding statement is being used to resolve itself.
      //
      // https://github.com/chapel-lang/chapel/issues/24709
    } else {
      const ResolvedFields& exprs = resolveForwardingExprs(context, ct);
      if (fields.numForwards() > 0 ||
          exprs.numForwards() > 0) {
        // and check for cycles
        bool cycleFound = emitErrorForForwardingCycles(context, ct);
        if (cycleFound == false) {
          forwards.addForwarding(fields);
          forwards.addForwarding(exprs);
        }
      }
    }
  }

  if (forwards.numForwards() > 0) {
    // Construct CallInfos with the receiver replaced for each
    // of the forwarded-to types.
    std::vector<CallInfo> forwardingCis;

    int numForwards = forwards.numForwards();
    for (int i = 0; i < numForwards; i++) {
      QualifiedType forwardType = forwards.forwardingToType(i);

      // an error occurred, skip it
      if (forwardType.isUnknown() || forwardType.hasTypePtr() == false)
        continue;

      std::vector<CallInfoActual> actuals;
      // compute the actuals
      // first, the method receiver (from the forwarded type)
      actuals.push_back(CallInfoActual(forwardType, USTR("this")));

      // Check that ci was consistent with passing the receiver by name
      // If it turns out to not always be the case for a good reason,
      // this assert can be removed.
      CHPL_ASSERT(ci.actual(0).byName() == USTR("this"));

      // then, the other actuals, if any, from ci
      int numActuals = ci.numActuals();
      for (int j = 1; j < numActuals; j++) {
        actuals.push_back(ci.actual(j));
      }

      forwardingCis.push_back(CallInfo(ci.name(),
                                       forwardType,
                                       ci.isMethodCall(),
                                       ci.hasQuestionArg(),
                                       ci.isParenless(),
                                       std::move(actuals)));
    }

    // Now, consider each of the forwarding calls to gather and filter
    // candidates.
    // This should follow the same process as gatherAndFilterCandidates but:
    // * in each stage, consider all of the forwarded method receiver types
    //   equally as sources of candidates
    // * do not consider forwarding (since we are considering it now!)

    std::vector<CheckedScopes> visited;
    visited.resize(numForwards);

    for (const auto& fci : forwardingCis) {
      size_t start = nonPoiCandidates.size();
      // consider compiler-generated candidates
      considerCompilerGeneratedCandidates(context, astForErr, fci,
                                          inScopes.callScope(), inScopes.poiScope(),
                                          nonPoiCandidates,
                                          rejected);
      // update forwardingTo
      nonPoiCandidates.helpComputeForwardingTo(fci, start);

      // don't worry about last resort for compiler generated candidates
    }

    // next, look for candidates without using POI.
    {
      int i = 0;
      CandidatesAndForwardingInfo newLrcGroup;
      for (const auto& fci : forwardingCis) {
        size_t start = nonPoiCandidates.size();
        // compute the potential functions that it could resolve to
        auto v = lookupCalledExpr(context, inScopes.lookupScope(), fci, visited[i]);

        // filter without instantiating yet
        const auto& initialCandidates =
          filterCandidatesInitial(context, std::move(v), fci);

        // find candidates, doing instantiation if necessary
        CandidatesAndForwardingInfo candidatesWithInstantiations;
        filterCandidatesInstantiating(context,
                                      initialCandidates,
                                      fci,
                                      inScopes.callScope(),
                                      inScopes.poiScope(),
                                      candidatesWithInstantiations,
                                      rejected);

        // filter out last resort candidates
        filterCandidatesLastResort(context, candidatesWithInstantiations,
                                   nonPoiCandidates, newLrcGroup);

        // update forwardingTo (for candidates and last resort candidates)
        nonPoiCandidates.helpComputeForwardingTo(fci, start);
        newLrcGroup.helpComputeForwardingTo(fci, start);
        i++;
      }
      lrcGroups.addNonPoiCandidates(std::move(newLrcGroup));
    }

    // next, look for candidates using POI
    for (const PoiScope* curPoi = inScopes.poiScope();
         curPoi != nullptr;
         curPoi = curPoi->inFnPoi()) {

      // stop if any candidate has been found.
      if (nonPoiCandidates.empty() == false || poiCandidates.empty() == false) {
        break;
      }


      int i = 0;
      CandidatesAndForwardingInfo newLrcGroup;
      for (const auto& fci : forwardingCis) {
        size_t start = poiCandidates.size();

        // compute the potential functions that it could resolve to
        auto v = lookupCalledExpr(context, curPoi->inScope(), fci, visited[i]);

        // filter without instantiating yet
        auto& initialCandidates =
          filterCandidatesInitial(context, std::move(v), fci);

        // find candidates, doing instantiation if necessary
        CandidatesAndForwardingInfo candidatesWithInstantiations;
        filterCandidatesInstantiating(context,
                                      initialCandidates,
                                      fci,
                                      inScopes.callScope(),
                                      inScopes.poiScope(),
                                      candidatesWithInstantiations,
                                      rejected);

        // filter out last resort candidates
        filterCandidatesLastResort(context, candidatesWithInstantiations,
                                   poiCandidates, newLrcGroup);

        // update forwardingTo (for candidates and last resort candidates)
        poiCandidates.helpComputeForwardingTo(fci, start);
        newLrcGroup.helpComputeForwardingTo(fci, start);
        i++;
      }
      lrcGroups.addPoiCandidates(std::move(newLrcGroup));
    }

    // If no candidates were found and it's a method, try forwarding
    // This supports the forwarding-to-forwarding case.
    if (nonPoiCandidates.empty() && poiCandidates.empty()) {
      for (const auto& fci : forwardingCis) {
        LastResortCandidateGroups thisForwardingLrcGroups;
        if (fci.isMethodCall() && fci.numActuals() >= 1) {
          const Type* receiverType = fci.actual(0).type().type();
          if (typeUsesForwarding(context, receiverType)) {
            gatherAndFilterCandidatesForwarding(context,
                                                astForErr, call, fci,
                                                inScopes,
                                                nonPoiCandidates,
                                                poiCandidates,
                                                thisForwardingLrcGroups,
                                                rejected);
          }
        }
        lrcGroups.getForwardingGroups().mergeWithGroups(
            std::move(thisForwardingLrcGroups));
      }
    }
  }

  // No need to gather up last resort candidates here, gatherAndFilterCandidates
  // above us will handle it.
}

// TODO: Could/should this be a parsing query?
static bool isInsideForwarding(Context* context, const Call* call) {
  bool insideForwarding = false;
  if (call != nullptr) {
    auto p = parsing::parentAst(context, call);
    while (p != nullptr) {
      // If we encounter an aggregate or function, we're definitely not in
      // a forwarding statement.
      if (p->isAggregateDecl() || p->isFunction()) break;

      if (p->isForwardingDecl()) {
        insideForwarding = true;
        break;
      }

      p = parsing::parentAst(context, p);
    }
  }

  return insideForwarding;
}

// Returns candidates (including instantiating candidates)
// for resolving CallInfo 'ci'.
//
// call can be nullptr. in that event, ci.name() will be used
// to find the call with that name.
//
// forwardingTo is a vector that will be empty unless forwarding
// is used for some candidates.
//
// If forwarding is used, it will have an element for each of the returned
// candidates and will indicate the actual type that is passed
// to the 'this' receiver formal.
static CandidatesAndForwardingInfo
gatherAndFilterCandidates(Context* context,
                          const AstNode* astForErr,
                          const Call* call,
                          const CallInfo& ci,
                          const CallScopeInfo& inScopes,
                          size_t& firstPoiCandidate,
                          std::vector<ApplicabilityResult>* rejected) {
  CandidatesAndForwardingInfo candidates;
  LastResortCandidateGroups lrcGroups;
  CheckedScopes visited;
  firstPoiCandidate = 0;

  // inject compiler-generated candidates in a manner similar to below
  // (note that any added candidates are already fully instantiated &
  //  the poiInfo from these is not gathered, because such methods should
  //  always be available in any scope that can refer to the type & are
  //  considered part of the custom type)
  considerCompilerGeneratedCandidates(context, astForErr, ci,
                                      inScopes.callScope(),
                                      inScopes.poiScope(),
                                      candidates,
                                      rejected);

  // don't worry about last resort for compiler generated candidates

  // next, look for candidates without using POI.
  {
    // compute the potential functions that it could resolve to
    auto v = lookupCalledExpr(context, inScopes.lookupScope(), ci, visited);

    // filter without instantiating yet
    const auto& initialCandidatesAndRejections =
      filterCandidatesInitialGatherRejected(context, std::move(v), ci, rejected != nullptr);
    const auto& initialCandidates = initialCandidatesAndRejections.first;
    const auto& initialRejections = initialCandidatesAndRejections.second;

    if (rejected != nullptr) {
      rejected->insert(rejected->end(),
                       initialRejections.begin(),
                       initialRejections.end());
    }

    // find candidates, doing instantiation if necessary
    CandidatesAndForwardingInfo candidatesWithInstantiations;
    filterCandidatesInstantiating(context,
                                  initialCandidates,
                                  ci,
                                  inScopes.callScope(),
                                  inScopes.poiScope(),
                                  candidatesWithInstantiations,
                                  rejected);

    // filter out last resort candidates
    CandidatesAndForwardingInfo lrcGroup;
    filterCandidatesLastResort(context, candidatesWithInstantiations,
                               candidates, lrcGroup);
    lrcGroups.addNonPoiCandidates(std::move(lrcGroup));
  }

  // next, look for candidates using POI
  firstPoiCandidate = candidates.size();
  for (const PoiScope* curPoi = inScopes.poiScope();
       curPoi != nullptr;
       curPoi = curPoi->inFnPoi()) {

    // stop if any candidate has been found.
    if (candidates.empty() == false) {
      break;
    }

    // compute the potential functions that it could resolve to
    auto v = lookupCalledExpr(context, curPoi->inScope(), ci, visited);

    // filter without instantiating yet
    const auto& initialCandidatesAndRejections =
      filterCandidatesInitialGatherRejected(context, std::move(v), ci, rejected != nullptr);
    const auto& initialCandidates = initialCandidatesAndRejections.first;
    const auto& initialRejections = initialCandidatesAndRejections.second;

    if (rejected != nullptr) {
      rejected->insert(rejected->end(),
                       initialRejections.begin(),
                       initialRejections.end());
    }

    // find candidates, doing instantiation if necessary
    CandidatesAndForwardingInfo candidatesWithInstantiations;
    filterCandidatesInstantiating(context,
                                  initialCandidates,
                                  ci,
                                  inScopes.callScope(),
                                  inScopes.poiScope(),
                                  candidatesWithInstantiations,
                                  rejected);

    // filter out last resort candidates
    CandidatesAndForwardingInfo lrcGroup;
    filterCandidatesLastResort(context, candidatesWithInstantiations,
                               candidates, lrcGroup);
    lrcGroups.addPoiCandidates(std::move(lrcGroup));
  }

  // If no candidates were found and it's a method, try forwarding
  if (candidates.empty() && ci.isMethodCall() && ci.numActuals() >= 1) {
    const Type* receiverType = ci.actual(0).type().type();

    // TODO: Should this information come as a boolean argument set by the
    // Resolver? It would be less expensive to set a boolean on Resolver once
    // we encounter a ForwardingDecl.
    //
    // Possible recursion here when resolving a function call in a forwarding
    // statement:
    //     record R { forwarding foo(); }
    // We need to try resolving 'foo()' as a method on 'R', which eventually
    // leads us back to this path here.
    //
    // By skipping the gathering of forwarding candidates below, we also
    // prevent forwarding statements from containing expressions that
    // themselves require forwarding. For example, if you had a couple of
    // forwarding statements like:
    //     forwarding b;
    //     forwarding bar();
    // The 'isInsideForwarding' check below would prevent resolving a method
    // 'bar()' on 'b'.

    if (typeUsesForwarding(context, receiverType) &&
        !isInsideForwarding(context, call)) {
      CandidatesAndForwardingInfo nonPoiCandidates;
      CandidatesAndForwardingInfo poiCandidates;

      gatherAndFilterCandidatesForwarding(
          context, astForErr, call, ci, inScopes, nonPoiCandidates,
          poiCandidates, lrcGroups.getForwardingGroups(),
          rejected);

      // append candidates from forwarding
      candidates.takeFromOther(nonPoiCandidates);
      candidates.takeFromOther(poiCandidates);
    }
  }

  // If no candidates have been found, consider last resort candidates.
  if (candidates.empty()) {
    candidates = lrcGroups.firstNonEmptyCandidatesGroup(&firstPoiCandidate);
  }

  return candidates;
}

// * find the best candidates (disambiguate)
// * check signatures of selected candidates
// * gather POI info from any instantiations
static MostSpecificCandidates
findMostSpecificAndCheck(Context* context,
                         const CandidatesAndForwardingInfo& candidates,
                         size_t firstPoiCandidate,
                         const Call* call,
                         const CallInfo& ci,
                         const Scope* inScope,
                         const PoiScope* inPoiScope,
                         PoiInfo& poiInfo) {

  // find most specific candidates / disambiguate
  MostSpecificCandidates mostSpecific =
      findMostSpecificCandidates(context, candidates, ci, inScope, inPoiScope);

  // perform fn signature checking for any instantiated candidates that are used
  for (const MostSpecificCandidate& candidate : mostSpecific) {
    if (candidate && candidate.fn()->instantiatedFrom()) {
      checkSignature(context, candidate.fn());
    }
  }

  // note any most-specific candidates from POI in poiInfo.
  // TODO: This can be the case for generated calls, but is skipping the POI
  // accumulation safe?
  if (call != nullptr) {
    size_t n = candidates.size();
    for (size_t i = firstPoiCandidate; i < n; i++) {
      for (const MostSpecificCandidate& candidate : mostSpecific) {
        if (candidate.fn() == candidates.get(i)) {
          poiInfo.addIds(call->id(), candidate.fn()->id());
        }
      }
    }
  }

  return mostSpecific;
}


static MostSpecificCandidates
resolveFnCallFilterAndFindMostSpecific(Context* context,
                                       const AstNode* astForErr,
                                       const Call* call,
                                       const CallInfo& ci,
                                       const CallScopeInfo& inScopes,
                                       PoiInfo& poiInfo,
                                       std::vector<ApplicabilityResult>* rejected) {

  // search for candidates at each POI until we have found candidate(s)
  size_t firstPoiCandidate = 0;
  CandidatesAndForwardingInfo candidates = gatherAndFilterCandidates(
      context, astForErr, call, ci, inScopes, firstPoiCandidate, rejected);

  // * find most specific candidates / disambiguate
  // * check signatures
  // * gather POI info

  MostSpecificCandidates mostSpecific =
      findMostSpecificAndCheck(context, candidates, firstPoiCandidate, call, ci,
                               inScopes.callScope(), inScopes.poiScope(),
                               poiInfo);

  return mostSpecific;
}

// call can be nullptr. in that event ci.name() will be used to find
// what is called.
static
CallResolutionResult resolveFnCall(Context* context,
                                   const AstNode* astForErr,
                                   const Call* call,
                                   const CallInfo& ci,
                                   const CallScopeInfo& inScopes,
                                   std::vector<ApplicabilityResult>* rejected) {
  PoiInfo poiInfo;
  MostSpecificCandidates mostSpecific;

  // Note: currently type constructors are not implemented as methods
  if (ci.calledType().kind() == QualifiedType::TYPE &&
      ci.isMethodCall() == false) {
    // handle invocation of a type constructor from a type
    // (note that we might have the type through a type alias)
    mostSpecific = resolveFnCallForTypeCtor(context, ci,
                                            inScopes.callScope(),
                                            inScopes.poiScope(),
                                            poiInfo);
  } else {
    // * search for candidates at each POI until we have found a candidate
    // * filter and instantiate
    // * disambiguate
    // * note any most specific candidates from POI in poiInfo.
    mostSpecific = resolveFnCallFilterAndFindMostSpecific(context, astForErr, call, ci,
                                                          inScopes,
                                                          poiInfo, rejected);
  }

  // fully resolve each candidate function and gather poiScopesUsed.

  // figure out the poiScope to use
  const PoiScope* instantiationPoiScope = nullptr;
  bool anyInstantiated = false;

  for (const MostSpecificCandidate& candidate : mostSpecific) {
    if (candidate && candidate.fn()->instantiatedFrom() != nullptr) {
      anyInstantiated = true;
      break;
    }
  }

  if (anyInstantiated) {
    instantiationPoiScope =
      pointOfInstantiationScope(context, inScopes.callScope(), inScopes.poiScope());
    poiInfo.setPoiScope(instantiationPoiScope);

    for (const MostSpecificCandidate& candidate : mostSpecific) {
      if (candidate) {
        if (candidate.fn()->untyped()->idIsFunction()) {
          // note: following call returns early if candidate not instantiated
          accumulatePoisUsedByResolvingBody(context, candidate.fn(),
                                            instantiationPoiScope, poiInfo);
        }
      }
    }
  }

  // infer types of generic 'out' formals from function bodies
  mostSpecific.inferOutFormals(context, instantiationPoiScope);

  // Make sure that we are resolving initializer bodies even when the
  // signature is concrete, because there are semantic checks.
  if (isCallInfoForInitializer(ci) && mostSpecific.numBest() == 1) {
    auto candidateFn = mostSpecific.only().fn();
    CHPL_ASSERT(isTfsForInitializer(candidateFn));

    // TODO: Can we move this into the 'InitVisitor'?
    // Note: resolveInitializer is already called during instantiation
    if (!candidateFn->untyped()->isCompilerGenerated() &&
        candidateFn->instantiatedFrom() == nullptr) {
      std::ignore = resolveInitializer(context, candidateFn, inScopes.poiScope());
    }
  }

  // compute the return types
  QualifiedType retType;
  bool retTypeSet = false;
  for (const MostSpecificCandidate& candidate : mostSpecific) {
    if (candidate.fn() != nullptr) {
      QualifiedType t = returnType(context, candidate.fn(), instantiationPoiScope);
      if (retTypeSet && retType.type() != t.type()) {
        context->error(candidate.fn(),
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
  for (const auto& actual : ci.actuals()) {
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

  for (const auto& actual : ci.actuals()) {
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

static bool shouldAttemptImplicitReceiver(const CallInfo& ci,
                                          QualifiedType implicitReceiver) {
  return !ci.isMethodCall() &&
         !ci.isOpCall() &&
         implicitReceiver.type() != nullptr &&
         // Assuming ci.name().isEmpty()==true implies a primitive call.
         // TODO: Add some kind of 'isPrimitive()' to CallInfo
         !ci.name().isEmpty() &&
         ci.name() != USTR("?") &&
         ci.name() != USTR("owned") &&
         ci.name() != USTR("shared") &&
         ci.name() != USTR("borrowed") &&
         ci.name() != USTR("unmanaged");
}

CallResolutionResult resolveCall(Context* context,
                                 const Call* call,
                                 const CallInfo& ci,
                                 const CallScopeInfo& inScopes,
                                 std::vector<ApplicabilityResult>* rejected) {
  if (call->isFnCall() || call->isOpCall()) {
    // see if the call is handled directly by the compiler
    QualifiedType tmpRetType;
    if (resolvePostfixNilableAppliedToNew(context, call, ci, tmpRetType)) {
      return CallResolutionResult(std::move(tmpRetType));
    }
    if (resolveFnCallSpecial(context, call, ci, tmpRetType)) {
      return CallResolutionResult(std::move(tmpRetType));
    }

    CallResolutionResult keywordRes;
    if (resolveFnCallSpecialType(context, call, ci, inScopes, keywordRes)) {
      return keywordRes;
    }

    // otherwise do regular call resolution
    return resolveFnCall(context, call, call, ci, inScopes, rejected);
  } else if (auto prim = call->toPrimCall()) {
    return resolvePrimCall(context, prim, ci, inScopes.callScope(), inScopes.poiScope());
  } else if (auto tuple = call->toTuple()) {
    return resolveTupleExpr(context, tuple, ci, inScopes.callScope(), inScopes.poiScope());
  }

  if (call) {
    std::string msg = "resolveCall cannot handle tag: ";
    msg += asttags::tagToString(call->tag());
    CHPL_UNIMPL(msg.c_str());
  } else {
    CHPL_UNIMPL("resolveCall with null Call*");
  }
  MostSpecificCandidates emptyCandidates;
  QualifiedType emptyType;
  PoiInfo emptyPoi;
  return CallResolutionResult(emptyCandidates, emptyType, emptyPoi);
}

CallResolutionResult resolveCallInMethod(Context* context,
                                         const Call* call,
                                         const CallInfo& ci,
                                         const CallScopeInfo& inScopes,
                                         QualifiedType implicitReceiver,
                                         std::vector<ApplicabilityResult>* rejected) {

  // If there is an implicit receiver and ci isn't written as a method,
  // construct a method call and use that instead. If that resolves,
  // it takes precedence over functions.
  if (shouldAttemptImplicitReceiver(ci, implicitReceiver)) {
    auto methodCi = CallInfo::createWithReceiver(ci, implicitReceiver);
    auto ret = resolveCall(context, call, methodCi, inScopes, rejected);
    if (ret.mostSpecific().foundCandidates()) {
      return ret;
    }
  }

  // otherwise, use normal resolution
  return resolveCall(context, call, ci, inScopes, rejected);
}

CallResolutionResult resolveGeneratedCall(Context* context,
                                          const AstNode* astForErr,
                                          const CallInfo& ci,
                                          const CallScopeInfo& inScopes,
                                          std::vector<ApplicabilityResult>* rejected) {
  // see if the call is handled directly by the compiler
  QualifiedType tmpRetType;
  if (resolveFnCallSpecial(context, astForErr, ci, tmpRetType)) {
    return CallResolutionResult(std::move(tmpRetType));
  }
  // otherwise do regular call resolution
  return resolveFnCall(context, astForErr, /* call */ nullptr, ci, inScopes, rejected);
}

CallResolutionResult
resolveGeneratedCallInMethod(Context* context,
                             const AstNode* astForErr,
                             const CallInfo& ci,
                             const CallScopeInfo& inScopes,
                             QualifiedType implicitReceiver) {
  // If there is an implicit receiver and ci isn't written as a method,
  // construct a method call and use that instead. If that resolves,
  // it takes precedence over functions.
  if (shouldAttemptImplicitReceiver(ci, implicitReceiver)) {
    auto methodCi = CallInfo::createWithReceiver(ci, implicitReceiver);
    auto ret = resolveGeneratedCall(context, astForErr, methodCi, inScopes);
    if (ret.mostSpecific().foundCandidates()) {
      return ret;
    }
  }

  // otherwise, resolve a regular function call
  return resolveGeneratedCall(context, astForErr, ci, inScopes);
}

const TypedFnSignature* tryResolveInitEq(Context* context,
                                         const AstNode* astForScopeOrErr,
                                         const types::Type* lhsType,
                                         const types::Type* rhsType,
                                         const PoiScope* poiScope) {
  if (!lhsType->getCompositeType()) return nullptr;

  // use the regular VAR kind for this query
  // (don't want a type-expr lhsType to be considered a TYPE here)
  QualifiedType lhsQt(QualifiedType::VAR, lhsType);
  QualifiedType rhsQt(QualifiedType::VAR, rhsType);

  std::vector<CallInfoActual> actuals;
  actuals.push_back(CallInfoActual(lhsQt, USTR("this")));
  actuals.push_back(CallInfoActual(rhsQt, UniqueString()));
  auto ci = CallInfo(/* name */ USTR("init="),
                     /* calledType */ lhsQt,
                     /* isMethodCall */ true,
                     /* hasQuestionArg */ false,
                     /* isParenless */ false, actuals);

  const Scope* scope = nullptr;
  if (astForScopeOrErr) scope = scopeForId(context, astForScopeOrErr->id());

  auto c = resolveGeneratedCall(context, astForScopeOrErr, ci,
                                CallScopeInfo::forNormalCall(scope, poiScope));
  return c.mostSpecific().only().fn();
}

const TypedFnSignature* tryResolveDeinit(Context* context,
                                         const AstNode* astForScopeOrErr,
                                         const types::Type* t,
                                         const PoiScope* poiScope) {
  if (!t->getCompositeType()) return nullptr;

  QualifiedType qt(QualifiedType::VAR, t);

  std::vector<CallInfoActual> actuals;
  actuals.push_back(CallInfoActual(qt, USTR("this")));
  auto ci = CallInfo(/* name */ USTR("deinit"),
                     /* calledType */ qt,
                     /* isMethodCall */ true,
                     /* hasQuestionArg */ false,
                     /* isParenless */ false,
                     actuals);

  const Scope* scope = nullptr;
  if (astForScopeOrErr) scope = scopeForId(context, astForScopeOrErr->id());

  auto c = resolveGeneratedCall(context, astForScopeOrErr, ci,
                                CallScopeInfo::forNormalCall(scope, poiScope));
  return c.mostSpecific().only().fn();
}

static const TypedFnSignature*
tryResolveAssignHelper(Context* context,
                       const uast::AstNode* astForScopeOrErr,
                       const types::Type* lhsType,
                       const types::Type* rhsType,
                       bool asMethod) {
  // Use 'var' here since actual types don't really matter, and some
  // assignment operators (e.g., for 'owned') will mutate the RHS.
  auto qtLhs = QualifiedType(QualifiedType::VAR, lhsType);
  auto qtRhs = QualifiedType(QualifiedType::VAR, rhsType);

  std::vector<CallInfoActual> actuals;
  if (asMethod) {
    actuals.push_back(CallInfoActual(qtLhs, USTR("this")));
  }
  actuals.push_back(CallInfoActual(qtLhs, UniqueString()));
  actuals.push_back(CallInfoActual(qtRhs, UniqueString()));
  auto ci = CallInfo(/* name */ USTR("="),
                     /* calledType */ qtLhs,
                     /* isMethodCall */ asMethod,
                     /* hasQuestionArg */ false,
                     /* isParenless */ false,
                     actuals);
  const Scope* scope = nullptr;
  if (astForScopeOrErr) scope = scopeForId(context, astForScopeOrErr->id());
  auto c = resolveGeneratedCall(context, astForScopeOrErr, ci,
                                CallScopeInfo::forNormalCall(scope, /* poiScope */ nullptr));
  return c.mostSpecific().only().fn();
}

// Tries to resolve an = that assigns a type from itself, first as a method and
// then as a standalone operator.
const TypedFnSignature*
tryResolveAssign(Context* context,
                 const uast::AstNode* astForScopeOrErr,
                 const types::Type* lhsType,
                 const types::Type* rhsType,
                 const PoiScope* poiScope) {
  auto res = tryResolveAssignHelper(context, astForScopeOrErr, lhsType,
                                    rhsType,
                                    /* asMethod */ true);
  if (!res) {
    res = tryResolveAssignHelper(context, astForScopeOrErr, lhsType,
                                 rhsType,
                                 /* asMethod */ false);
  }
  return res;
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
  } else if (auto fwd = ast->toForwardingDecl()) {
    if (auto fwdVar = fwd->expr()->toVariable()) {
      return fwdVar->name() == name;
    }
  }
  return false;
}

static const CompositeType* const&
isNameOfFieldQuery(Context* context,
                   UniqueString name, const CompositeType* ct) {
  QUERY_BEGIN(isNameOfFieldQuery, context, name, ct);

  const CompositeType* result = nullptr;
  auto ast = parsing::idToAst(context, ct->id());
  CHPL_ASSERT(ast && ast->isAggregateDecl());
  auto ad = ast->toAggregateDecl();

  for (auto child: ad->children()) {
    // Ignore everything other than VarLikeDecl, MultiDecl, TupleDecl
    if (child->isVarLikeDecl() ||
        child->isMultiDecl() ||
        child->isTupleDecl() ||
        child->isForwardingDecl()) {
      bool found = helpFieldNameCheck(child, name);
      if (found) {
        result = ct;
        break;
      }
    }
  }

  if (result == nullptr) {
    // check also superclass fields
    if (auto bct = ct->toBasicClassType()) {
      const CompositeType* found =
        isNameOfField(context, name, bct->parentClassType());
      if (found != nullptr) {
        result = found;
      }
    }
  }

  return QUERY_END(result);
}

const CompositeType* isNameOfField(Context* context,
                                   UniqueString name,
                                   const Type* t) {

  if (t == nullptr) {
    return nullptr;
  }

  const CompositeType* ct = t->getCompositeType();

  if (ct == nullptr) {
    return nullptr;
  }

  if (auto bct = ct->toBasicClassType()) {
    if (bct->isObjectType()) {
      return nullptr;
    }
  }

  return isNameOfFieldQuery(context, name, ct);
}

// TODO: This is very early draft and is missing a lot, e.g.,
//    - No valid default-initializer present
//    - Instantiated generics must supply type/param arguments when
//      searching for a default-initializer
//    - Consideration of 'where' clauses
//    - Composites with compilerError'd default-initializers
//    - Mutually recursive class types
//    - Non-nil 'owned' classes
static bool
isTypeDefaultInitializableImpl(Context* context, const Type* t) {
  const auto g = t->genericity();

  switch (g) {
    case Type::CONCRETE: return true;
    case Type::GENERIC: return false;

    // For these, consider the fields.
    case Type::GENERIC_WITH_DEFAULTS:
    case Type::MAYBE_GENERIC:
      break;
  }

  CHPL_ASSERT(!t->isPrimitiveType());

  if (t->isBuiltinType()) {
    CHPL_ASSERT(false && "Not handled!");
  }

  if (auto ct = t->toCompositeType()) {
    const auto p = DefaultsPolicy::USE_DEFAULTS;
    auto& rf = fieldsForTypeDecl(context, ct, p);

    if (!rf.isGeneric()) return true;

    // TODO: Do I still need to consider field genericity, here? I.E., if
    // a field is marked 'GENERIC_WITH_DEFAULTS' is there more to do?
    // If I can tell the thing is concrete from the ResolvedFields, then
    // there's probably no need to recurse.
    if (rf.isGenericWithDefaults()) {
      for (int i = 0; i < rf.numFields(); i++) {
        auto ft = rf.fieldType(i).type();

        // TODO: Skipping avoids a recursive query but doesn't handle
        // mutually recursive classes.
        if (ft == t) continue;

        if (!isTypeDefaultInitializable(context, ft)) return false;
      }

      return true;
    }
  }

  return false;
}

static const bool&
isTypeDefaultInitializableQuery(Context* context, const Type* t) {
  QUERY_BEGIN(isTypeDefaultInitializableQuery, context, t);
  bool ret = isTypeDefaultInitializableImpl(context, t);
  return QUERY_END(ret);
}

bool isTypeDefaultInitializable(Context* context, const Type* t) {
  return isTypeDefaultInitializableQuery(context, t);
}

void getCopyOrAssignableInfo(Context* context, const Type* t,
                                    bool& fromConst, bool& fromRef,
                                    bool checkCopyable);

// Determine whether a class type is copyable or assignable, from ref and/or
// from const.
static const CopyableAssignableInfo getClassTypeCopyOrAssignable(
    const ClassType* ct) {
  CopyableAssignableInfo result;

  if (ct->decorator().isManaged() && ct->manager()->isAnyOwnedType()) {
    // Owned class types are copyable/assignable from ref iff they are nilable.
    // TODO: update if/when user-defined memory management styles are added
    if (ct->decorator().isNilable()) {
      result = CopyableAssignableInfo::fromRef();
    }
  } else {
    // Class types of other management are copyable/assignable from const.
    result = CopyableAssignableInfo::fromConst();
  }

  return result;
}

// Set checkCopyable true for copyable, false for assignable.
static const CopyableAssignableInfo& getCopyOrAssignableInfoQuery(
    Context* context, const CompositeType* ct, bool checkCopyable) {
  QUERY_BEGIN(getCopyOrAssignableInfoQuery, context, ct, checkCopyable);


  CopyableAssignableInfo result = CopyableAssignableInfo::fromNone();

  // Inspect type for either kind of copyability/assignability.
  auto genericity = getTypeGenericity(context, ct);
  if (genericity == Type::GENERIC || genericity == Type::MAYBE_GENERIC) {
    // generic composite types cannot be copied or assigned
    result = CopyableAssignableInfo::fromNone();
  } else if (auto at = ct->toArrayType()) {
    if (auto eltType = at->eltType().type()) {
      // Arrays are copyable/assignable if their elements are
      result = getCopyOrAssignableInfo(context, eltType, checkCopyable);
    }
  } else if (auto tt = ct->toTupleType()) {
    // Tuples have the minimum copyable/assignable-ness of their elements
    result = CopyableAssignableInfo::fromConst();
    // TODO: add iterator for TupleType element types and use a range-based for
    for (int i = 0; i < tt->numElements(); i++) {
      result.intersectWith(getCopyOrAssignableInfo(
          context, tt->elementType(i).type(), checkCopyable));
      if (tt->isStarTuple()) break;
    }
  } else {
    auto ast = parsing::idToAst(context, ct->id());
    const AttributeGroup* attrs = nullptr;
    if (ast) attrs = ast->attributeGroup();
    if (checkCopyable && attrs &&
        (attrs->hasPragma(PRAGMA_SYNC) || attrs->hasPragma(PRAGMA_SINGLE))) {
      // Syncs and singles are copyable
      // This is a special case to preserve deprecated behavior before
      // sync/single implicit reads are removed. 12/8/23
      result = CopyableAssignableInfo::fromConst();
    } else {
      // In general, try to resolve the type's 'init='/'=', and examine it to
      // determine copy/assignability, respectively.
      const TypedFnSignature* testResolvedSig =
          (checkCopyable ? tryResolveInitEq(context, ast, ct, ct)
                         : tryResolveAssign(context, ast, ct, ct));
      if (testResolvedSig) {
        if (testResolvedSig->untyped()->isCompilerGenerated()) {
          // Check for class fields reducing copy/assignability; otherwise it
          // is from const.

          result = CopyableAssignableInfo::fromConst();
          auto resolvedFields =
              fieldsForTypeDecl(context, ct, DefaultsPolicy::USE_DEFAULTS);
          for (int i = 0; i < resolvedFields.numFields(); i++) {
            auto fieldType = resolvedFields.fieldType(i).type();
            if (auto classTy = fieldType->toClassType()) {
              result.intersectWith(getClassTypeCopyOrAssignable(classTy));
            } else if (auto rt = fieldType->toRecordType()) {
              // check record fields recursively
              result.intersectWith(
                  getCopyOrAssignableInfo(context, rt, checkCopyable));
            }
          }
        } else {
          // Check intent of formal to copy/assign from.

          // For init=, formals are (this, other). For =, formals are (lhs,
          // rhs), unless it is a method in which case they are (this, lhs,
          // rhs). Get the index of the 'other' or 'rhs' formal.
          int otherFormalNum = 1;
          if (!checkCopyable && testResolvedSig->untyped()->isMethod()) {
            otherFormalNum = 2;
          }
          CHPL_ASSERT(testResolvedSig->numFormals() == (otherFormalNum + 1) &&
                      "unexpected formals");
          auto other = testResolvedSig->formalType(otherFormalNum);
          CHPL_ASSERT(!other.isNonConcreteIntent() &&
                      "should have resolved concrete intent by now");

          if (other.isIn() || other.isConst() ||
              other.kind() == QualifiedType::TYPE ||
              other.kind() == QualifiedType::PARAM) {
            result = CopyableAssignableInfo::fromConst();
          } else if (other.isRef()) {
            result = CopyableAssignableInfo::fromRef();
          } else {
            context->error(
                testResolvedSig->untyped()->formalDecl(otherFormalNum),
                "unexpected formal intent for special proc");
          }
        }
      }
    }
  }

  return QUERY_END(result);
}

CopyableAssignableInfo getCopyOrAssignableInfo(Context* context, const Type* t,
                                               bool checkCopyable) {
  CopyableAssignableInfo result;

  if (auto ct = t->toCompositeType()) {
    // Use query to cache results only for composite types, others are trivial
    result = getCopyOrAssignableInfoQuery(context, ct, checkCopyable);
  } else if (auto classTy = t->toClassType()) {
    result = getClassTypeCopyOrAssignable(classTy);
  } else {
    // Non-composite/class types are always copyable/assignable from const
    result = CopyableAssignableInfo::fromConst();
  }

  return result;
}

template <typename T>
QualifiedType paramTypeFromValue(Context* context, T value);

template <>
QualifiedType paramTypeFromValue<bool>(Context* context, bool value) {
  return QualifiedType(QualifiedType::PARAM,
                       BoolType::get(context),
                       BoolParam::get(context, value));
}

const std::unordered_map<UniqueString, QualifiedType>&
getCompilerGeneratedGlobals(Context* context) {
  QUERY_BEGIN(getCompilerGeneratedGlobals, context);

  auto& globals = compilerGlobals(context);
  std::unordered_map<UniqueString, QualifiedType> result;
  #define COMPILER_GLOBAL(TYPE__, IDENT__, NAME__)\
    result[UniqueString::get(context, IDENT__)] = \
      paramTypeFromValue<TYPE__>(context, globals.NAME__);
  #include "chpl/uast/compiler-globals-list.h"
  #undef COMPILER_GLOBAL

  return QUERY_END(result);
}

static const bool&
reportInvalidMultipleInheritanceImpl(Context* context,
                                     const uast::Class* node,
                                     const uast::AstNode* firstParent,
                                     const uast::AstNode* secondParent) {
  QUERY_BEGIN(reportInvalidMultipleInheritanceImpl, context, node, firstParent, secondParent);
  CHPL_REPORT(context, MultipleInheritance, node, firstParent, secondParent);
  auto result = false;
  return QUERY_END(result);
}

void
reportInvalidMultipleInheritance(Context* context,
                                 const uast::Class* node,
                                 const uast::AstNode* firstParent,
                                 const uast::AstNode* secondParent) {

  std::ignore = reportInvalidMultipleInheritanceImpl(context, node,
                                                     firstParent, secondParent);
}


} // end namespace resolution
} // end namespace chpl
