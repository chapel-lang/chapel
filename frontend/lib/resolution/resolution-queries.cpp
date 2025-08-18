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
#include "chpl/util/assertions.h"
#include "chpl/util/hash.h"

#include "Resolver.h"
#include "call-init-deinit.h"
#include "default-functions.h"
#include "extern-blocks.h"
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

namespace {
  struct EvaluatedCandidates {
    chpl::resolution::CandidatesAndForwardingInfo matching;
    std::vector<chpl::resolution::ApplicabilityResult> rejected;
    std::vector<chpl::resolution::ApplicabilityResult> rejectedIteratorsMissingTag;
    bool evaluatedAnyNestedFunction = false;

    bool operator==(const EvaluatedCandidates& rhs) const {
      return this->matching == rhs.matching &&
        this->rejected == rhs.rejected &&
        this->rejectedIteratorsMissingTag == rhs.rejectedIteratorsMissingTag &&
        this->evaluatedAnyNestedFunction == rhs.evaluatedAnyNestedFunction;
    }
    bool operator!=(const EvaluatedCandidates& rhs) const {
      return !(*this == rhs);
    }
    void swap(EvaluatedCandidates& rhs) {
      matching.swap(rhs.matching);
      std::swap(rejected, rhs.rejected);
      std::swap(evaluatedAnyNestedFunction, rhs.evaluatedAnyNestedFunction);
      std::swap(rejectedIteratorsMissingTag, rhs.rejectedIteratorsMissingTag);
    }
    static bool update(EvaluatedCandidates& keep, EvaluatedCandidates& addin) {
      return chpl::defaultUpdate(keep, addin);
    }
    void mark(chpl::Context* context) const {
      matching.mark(context);
      chpl::mark<decltype(rejected)>{}(context, rejected);
      chpl::mark<decltype(rejectedIteratorsMissingTag)>{}(context, rejectedIteratorsMissingTag);
    }
    size_t hash() const {
      return chpl::hash(matching, rejected, rejectedIteratorsMissingTag, evaluatedAnyNestedFunction);
    }
  };
}

namespace std {
  template <> struct hash<EvaluatedCandidates> {
    size_t operator()(const EvaluatedCandidates& key) const {
      return key.hash();
    }
  };
} // end namespace std

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

  CHPL_ASSERT(!id.isSymbolDefiningScope());

  ResolutionResultByPostorderID result;
  ResolutionContext rcval(context);
  auto rc = &rcval;

  ID moduleId = parsing::idToParentId(context, id);
  auto moduleAst = parsing::idToAst(context, moduleId);
  if (const Module* mod = moduleAst->toModule()) {
    // Resolve just the requested statement
    auto modStmt = parsing::idToAst(context, id);
    auto visitor = Resolver::createForModuleStmt(rc, mod, modStmt, result);
    modStmt->traverse(visitor);

    // There will be no further calls (it's a top-level statement), so if the
    // diagnostics wanted to be higher up the call stack, too bad.
    for (auto& diagnostic : visitor.userDiagnostics) {
      visitor.emitUserDiagnostic(diagnostic, modStmt);
    }

    if (auto rec = context->recoverFromSelfRecursion()) {
      CHPL_REPORT(context, RecursionModuleStmt, modStmt, mod, std::move(*rec));
    }
  }

  return QUERY_END(result);
}

static const ResolutionResultByPostorderID&
scopeResolveModuleStmt(Context* context, ID id) {
  QUERY_BEGIN(scopeResolveModuleStmt, context, id);

  CHPL_ASSERT(!id.isSymbolDefiningScope());

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

static void checkImplementationPoint(ResolutionContext* rc, const ImplementationPoint* implPoint) {
  if (getTypeGenericity(rc->context(), implPoint->interface()) == types::Type::CONCRETE) {
    auto inScope = scopeForId(rc->context(), implPoint->id());
    auto inScopes = CallScopeInfo::forNormalCall(inScope, nullptr);
    std::ignore = checkInterfaceConstraints(rc, implPoint->interface(), implPoint->id(), inScopes);
    // checkInterfaceConstraints emits an error already, nothing to do.
  }
}

static const std::map<ID, std::vector<const ImplementationPoint*>>&
collectImplementationPointsInModule(Context* context,
                                    const Module* mod);


const ResolvedExpression& resolveNameInModule(Context* context,
                                              ID modId,
                                              UniqueString name) {
  QUERY_BEGIN(resolveNameInModule, context, modId, name);

  ResolutionResultByPostorderID ignored;
  ResolutionContext rcval(context);
  const AstNode* ast = parsing::idToAst(context, modId);
  CHPL_ASSERT(ast != nullptr && ast->isModule());
  auto res = Resolver::createForModuleStmt(&rcval, ast->toModule(), /* modStmt */ nullptr, ignored);
  res.allowLocalSearch = false;
  auto result = res.resolveNameInModule(name);

  return QUERY_END(result);
}

const ResolutionResultByPostorderID& resolveModule(Context* context, ID id) {
  QUERY_BEGIN(resolveModule, context, id);

  const AstNode* ast = parsing::idToAst(context, id);
  CHPL_ASSERT(ast != nullptr);

  ResolutionResultByPostorderID result;
  ResolutionContext rcval(context);
  auto rc = &rcval;

  if (ast != nullptr) {
    if (const Module* mod = ast->toModule()) {
      // check for multiply-defined symbols within the module
      auto modScope = scopeForId(context, mod->id());
      emitMultipleDefinedSymbolErrors(context, modScope);

      auto r = Resolver::createForModuleStmt(rc, mod, nullptr, result);

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
      checkThrows(rc, result, mod);
      callInitDeinit(r);

      // check interface implementations in this module
      auto implPoints = collectImplementationPointsInModule(context, mod);
      for (const auto& pair : implPoints) {
        for (const auto implPoint : pair.second) {
          checkImplementationPoint(rc, implPoint);
        }
      }
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

  if (!id.isSymbolDefiningScope()) {
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
  } else if (name == USTR("chpl_INFINITY")) {
    double infinity = std::numeric_limits<double>::infinity();
    result = QualifiedType(QualifiedType::PARAM,
                           RealType::get(context, 0),
                           RealParam::get(context, infinity));
  } else if (name == USTR("chpl_NAN")) {
    double nan = std::numeric_limits<double>::quiet_NaN();
    result = QualifiedType(QualifiedType::PARAM,
                           RealType::get(context, 0),
                           RealParam::get(context, nan));
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
  if (substitutions != nullptr && formalDecl) {
    if (auto vld = formalDecl->toVarLikeDecl();
        vld && vld->name() == USTR("this") &&
        vld->storageKind() != Qualifier::TYPE) {
      // 'this' does not support partial instantiations, so don't bother
      // looking at substitutions.
    } else if (substitutions->count(formalDecl->id())) {
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
    if (untypedSig->isOperator() && untypedSig->formalName(i) == USTR("this")) {
      // the 'this' formals of operators aren't really used (lhs and rhs are
      // passed in as the first two formals), do don't check 'this'.
    } else if (formalNeedsInstantiation(context, qt, untypedSig->formalDecl(i),
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

static void
checkForParenlessMethodFieldRedefinition(Resolver& rv, const Function* fn) {
  Context* context = rv.context;

  if (fn->isMethod() && fn->isParenless()) {
    bool allowNonLocal = false;
    if (auto receiverInfo = rv.closestMethodReceiverInfo(allowNonLocal)) {
      auto compositeId = std::get<0>(*receiverInfo);
      auto receiverType = std::get<1>(*receiverInfo);

      if (receiverType.type()) {
        // use the type information, if it is present
        if (auto ct = receiverType.type()->getCompositeType()) {
          compositeId = ct->id();
        }
      }

      // Forcibly use scope-resolution to look up the composite if needed.
      if (parsing::idIsFunction(context, compositeId)) {
        compositeId = rv.scopeResolveCompositeIdFromMethodReceiver();
      }

      if (compositeId) {
        if (parsing::idContainsFieldWithName(context, compositeId,
                                             fn->name())) {
          context->error(fn, "parenless proc redeclares the field '%s'",
                         fn->name().c_str());
        }
      }
    }
  }
}

// Returns 'true' and emits an error if parent frames are not in the RC.
static bool errorIfParentFramesNotPresent(ResolutionContext* rc,
                                          const UntypedFnSignature* usig) {
  Context* context = rc->context();

  // TODO: More specifically, check if parent frames exist.
  bool ret = rc->isEmpty();

  // TODO: Becomes a structural issue after we pass in the parent.
  if (ret) {
    const ID& id = usig->id();
    context->error(id, "stack frames for the parent of '%s' are not "
                       "present, so outer variables in its signature "
                       "cannot be typed",
                       usig->name().c_str());
  }

  return ret;
}

static const TypedFnSignature*
typedSignatureInitialImpl(ResolutionContext* rc,
                          const UntypedFnSignature* untypedSig,
                          bool usePlaceholders) {
  Context* context = rc->context();
  const TypedFnSignature* result = nullptr;

  if (untypedSig->idIsExternBlockFunction()) {
    auto functionId = untypedSig->id();
    auto name = untypedSig->name();
    auto externBlockId = functionId.parentSymbolId(context);
    return externBlockSigForFn(context, externBlockId, name);
  }

  const AstNode* ast = parsing::idToAst(context, untypedSig->id());
  const Function* fn = ast->toFunction();

  if (fn == nullptr) return nullptr;

  // Construct the parent function's signature.
  // If the input is a nested function, then a frame for the parent signature
  // may be present in the 'ResolutionContext'. If no frame is present, then
  // attempt to call 'typedSignatureInitial' on the parent. This will give
  // up if any parent is generic or if any parent contains outer variables.
  const TypedFnSignature* parentSignature = nullptr;
  if (ID parentFnId = parsing::idToParentFunctionId(context, fn->id())) {
    if (auto frame = rc->findFrameWithId(parentFnId)) {
      if (auto sig = frame->signature()) {
        CHPL_ASSERT(sig->id() && sig->id().contains(fn->id()));
        parentSignature = sig;
      }
    } else {
      // TODO: Have the user pass the parent signature to make this explicit.
      auto parentShape = UntypedFnSignature::get(context, parentFnId);
      parentSignature = typedSignatureInitial(rc, parentShape);
    }

    // The parent signature must exist.
    if (!parentSignature) return nullptr;

    // TODO: Change this from a warning to a 'return nullptr' once we pass
    // in the parent function signature - in that case it is on the caller
    // to make sure they are concrete.
    for (auto up = parentSignature; up; up = up->parentFn()) {
      if (up->needsInstantiation()) {
        const ID& id = untypedSig->id();
        context->error(id, "One or more parent functions was inferred "
                           "to be generic while constructing the "
                           "initial signature of '%s'",
                           untypedSig->name().c_str());
        break;
      }
    }
  }

  ResolutionResultByPostorderID r;
  auto visitor = Resolver::createForInitialSignature(rc, fn, r);
  visitor.usePlaceholders = usePlaceholders;

  // visit the formals, but not the return type or body
  for (auto formal : fn->formals()) formal->traverse(visitor);

  if (!visitor.outerVariables.isEmpty()) {
    // outer variables can come from a parent function or from
    // an interface containing the function.
    CHPL_ASSERT(parentSignature || parsing::idToParentInterfaceId(context, fn->id()));

    // Outer variables can't be typed without stack frames, so give up.
    if (errorIfParentFramesNotPresent(rc, untypedSig)) return nullptr;
  }

  // now, construct a TypedFnSignature from the result
  std::vector<types::QualifiedType> formalTypes = visitor.getFormalTypes(fn);
  bool needsInstantiation = anyFormalNeedsInstantiation(context, formalTypes,
                                                        untypedSig,
                                                        nullptr);

  // visit the where clause, unless it needs to be instantiated, in
  // which case we will visit the where clause when that happens
  auto whereResult = TypedFnSignature::WHERE_NONE;
  if (auto whereClause = fn->whereClause()) {
    if (needsInstantiation) {
      // Visit the where clause for generic nested functions just to collect
      // outer variables. TODO: Is this OK or could POI muck with this?
      if (parentSignature) whereClause->traverse(visitor);
      whereResult = TypedFnSignature::WHERE_TBD;
    } else {
      whereClause->traverse(visitor);
      whereResult = whereClauseResult(context, fn, r, needsInstantiation);
    }
  }

  if (!visitor.outerVariables.isEmpty()) {
    CHPL_ASSERT(parentSignature || parsing::idToParentInterfaceId(context, fn->id()));

    // Outer variables can't be typed without stack frames, so give up.
    if (errorIfParentFramesNotPresent(rc, untypedSig)) return nullptr;
  }

  checkForParenlessMethodFieldRedefinition(visitor, fn);

  result = TypedFnSignature::get(context,
                                 untypedSig,
                                 std::move(formalTypes),
                                 whereResult,
                                 needsInstantiation,
                                 /* instantiatedFrom */ nullptr,
                                 /* parentFn */ parentSignature,
                                 /* formalsInstantiated */ Bitmap(),
                                 std::move(visitor.outerVariables));

  // also check the signature at this point if it is concrete
  if (result != nullptr && !result->needsInstantiation()) {
    checkSignature(context, result);
  }

  return result;
}

const TypedFnSignature* const&
typedSignatureInitial(ResolutionContext* rc,
                      const UntypedFnSignature* untypedSig) {
  CHPL_RESOLUTION_QUERY_BEGIN(typedSignatureInitial, rc, untypedSig);
  auto ret = typedSignatureInitialImpl(rc, untypedSig, /* usePlaceholders */ false);
  return CHPL_RESOLUTION_QUERY_END(ret);
}

static const TypedFnSignature* const&
typedSignatureInitialForIdQuery(ResolutionContext* rc, ID id) {
  CHPL_RESOLUTION_QUERY_BEGIN(typedSignatureInitialForIdQuery, rc, id);
  Context* context = rc->context();
  const UntypedFnSignature* uSig = UntypedFnSignature::get(context, id);
  const TypedFnSignature* ret = uSig ? typedSignatureInitial(rc, uSig)
                                     : nullptr;
  return CHPL_RESOLUTION_QUERY_END(ret);
}

const TypedFnSignature*
typedSignatureInitialForId(ResolutionContext* rc, ID id) {
  return typedSignatureInitialForIdQuery(rc, std::move(id));
}

static const TypedFnSignature* const&
typedSignatureTemplateForIdQuery(ResolutionContext* rc, ID id) {
  CHPL_RESOLUTION_QUERY_BEGIN(typedSignatureTemplateForIdQuery, rc, id);
  Context* context = rc->context();
  const UntypedFnSignature* uSig = UntypedFnSignature::get(context, id);
  const TypedFnSignature* ret = uSig ? typedSignatureInitialImpl(rc, uSig, /* usePlaceholders */ true)
                                     : nullptr;
  return CHPL_RESOLUTION_QUERY_END(ret);
}

const TypedFnSignature*
typedSignatureTemplateForId(ResolutionContext* rc, ID id) {
  return typedSignatureTemplateForIdQuery(rc, id);
}

// initedInParent is true if the decl variable is inited due to a parent
// uast node.  This comes up for TupleDecls.
static void helpSetFieldTypes(const CompositeType* ct,
                              const AstNode* ast,
                              ResolutionResultByPostorderID& r,
                              bool initedInParent,
                              ResolvedFields& fields,
                              bool syntaxOnly) {

  if (auto var = ast->toVarLikeDecl()) {
    bool hasDefaultValue = initedInParent || var->initExpression() != nullptr;

    auto fieldType = QualifiedType();
    if (!syntaxOnly) {
      const ResolvedExpression& e = r.byAst(var);
      fieldType = e.type();
    } else {
      auto& subs = ct->substitutions();
      if (auto it = subs.find(ast->id()); it != subs.end()) {
        fieldType = it->second;
      }
    }
    fields.addField(var->name(), hasDefaultValue, var->id(), fieldType);
  } else if (auto mult = ast->toMultiDecl()) {
    for (auto decl : mult->decls()) {
      helpSetFieldTypes(ct, decl, r, initedInParent, fields, syntaxOnly);
    }
  } else if (auto tup = ast->toTupleDecl()) {
    bool hasInit = initedInParent || tup->initExpression() != nullptr;
    for (auto decl : tup->decls()) {
      helpSetFieldTypes(ct, decl, r, hasInit, fields, syntaxOnly);
    }
  } else if (auto fwd = ast->toForwardingDecl()) {
    if (auto fwdTo = fwd->expr()) {
      if (fwdTo->isDecl()) {
        helpSetFieldTypes(ct, fwd->expr(), r, initedInParent, fields, syntaxOnly);
      }
      // If it's a visibility clause, use the type of the symbol rather than
      // the whole clause.
      auto fwdToSymbol = fwdTo->isVisibilityClause()
                             ? fwdTo->toVisibilityClause()->symbol()
                             : fwdTo;
      fields.addForwarding(fwd->id(), r.byAst(fwdToSymbol).type());
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

static const Type* const&
initialTypeForInterfaceQuery(Context* context, ID declId) {
  QUERY_BEGIN(initialTypeForInterfaceQuery, context, declId);
  const Type* result = nullptr;
  auto ast = parsing::idToAst(context, declId);
  if (auto itf = ast->toInterface()) {
    result = InterfaceType::get(context, itf->id(), itf->name(), /* subs */ {});
  }
  return QUERY_END(result);
}

const Type* initialTypeForInterface(Context* context, ID declId) {
  return initialTypeForInterfaceQuery(context, declId);
}

const ResolvedFields& resolveFieldDecl(ResolutionContext* rc,
                                       const CompositeType* ct,
                                       ID fieldId,
                                       DefaultsPolicy defaultsPolicy,
                                       bool syntaxOnly) {
  CHPL_RESOLUTION_QUERY_BEGIN(resolveFieldDecl, rc, ct, fieldId, defaultsPolicy, syntaxOnly);
  auto context = rc->context();

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

      if (!syntaxOnly) {
        auto visitor =
          Resolver::createForInitialFieldStmt(rc, ad, fieldAst,
                                              ct, r, defaultsPolicy);

        // resolve the field types and set them in 'result'
        fieldAst->traverse(visitor);
      }

      helpSetFieldTypes(ct, fieldAst, r, /* initedInParent */ false, result, syntaxOnly);
    } else {
      // handle resolving an instantiated type
      ResolutionResultByPostorderID r;

      if (!syntaxOnly) {
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

      helpSetFieldTypes(ct, fieldAst, r, /* initedInParent */ false, result, syntaxOnly);
    }
  }

  if (!syntaxOnly) result.validateFieldGenericity(context, ct);

  return CHPL_RESOLUTION_QUERY_END(result);
}

static
const ResolvedFields& fieldsForTypeDeclQuery(ResolutionContext* rc,
                                             const CompositeType* ct,
                                             DefaultsPolicy defaultsPolicy,
                                             bool syntaxOnly) {
  CHPL_RESOLUTION_QUERY_BEGIN(fieldsForTypeDeclQuery, rc, ct, defaultsPolicy, syntaxOnly);
  auto context = rc->context();

  CHPL_RESOLUTION_QUERY_REGISTER_TRACER(
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
  if (isObjectType) {
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
          resolveFieldDecl(rc, ct, child->id(), defaultsPolicy, syntaxOnly);
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
    result.finalizeFields(context, syntaxOnly);
  }

  return CHPL_RESOLUTION_QUERY_END(result);
}

const ResolvedFields& fieldsForTypeDecl(ResolutionContext* rc,
                                        const CompositeType* ct,
                                        DefaultsPolicy defaultsPolicy,
                                        bool syntaxOnly) {
  // The defaults policy only matters if type resolution is in play. If it
  // isn't, always set defaults policy to IGNORE_DEFAULTS to avoid memoizing
  // the same result multiple times.
  if (syntaxOnly) {
    return fieldsForTypeDeclQuery(rc, ct, DefaultsPolicy::IGNORE_DEFAULTS,
                                  /* syntaxOnly */ true);
  }

  if (defaultsPolicy == DefaultsPolicy::IGNORE_DEFAULTS){
    return fieldsForTypeDeclQuery(rc, ct, DefaultsPolicy::IGNORE_DEFAULTS,
                                  /* syntaxOnly */ false);
  }

  // try first with defaultsPolicy=FOR_OTHER_FIELDS
  const auto& f = fieldsForTypeDeclQuery(rc, ct,
                                         DefaultsPolicy::USE_DEFAULTS_OTHER_FIELDS,
                                         /* syntaxOnly */ false);

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
    return fieldsForTypeDeclQuery(rc, ct, finalDefaultsPolicy,
                                  /* syntaxOnly */ false);
  }

  // Otherwise, use the value we just computed.
  return f;
}

// Resolve all statements like 'forwarding _value;' in 'ct'
static
const ResolvedFields& resolveForwardingExprs(ResolutionContext* rc,
                                             const CompositeType* ct) {
  CHPL_RESOLUTION_QUERY_BEGIN(resolveForwardingExprs, rc, ct);
  auto context = rc->context();

  ResolvedFields result;

  CHPL_ASSERT(ct);
  result.setType(ct);

  bool isObjectType = false;
  if (auto bct = ct->toBasicClassType()) {
    isObjectType = bct->isObjectType();
  }
  if (isObjectType) {
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
          resolveFieldDecl(rc, ct, child->id(), DefaultsPolicy::USE_DEFAULTS);
        result.addForwarding(resolvedFields);
      }
    }
  }

  return CHPL_RESOLUTION_QUERY_END(result);
}

static bool typeUsesForwarding(Context* context, const Type* receiverType) {
  if (auto ct = receiverType->getCompositeType()) {
    if (ct->isBasicClassType() || ct->isRecordType() || ct->isDomainType() ||
        ct->isArrayType() || ct->isUnionType()) {
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
checkForwardingCycles(ResolutionContext* rc,
                      const CompositeType* ct,
                      llvm::SmallPtrSet<const CompositeType*, 8>& visited) {

  auto context = rc->context();
  if (typeUsesForwarding(context, ct)) {
    auto pair = visited.insert(ct);
    if (pair.second == false) {
      // it was already in the visited set
      context->error(ct->id(), "forwarding cycle detected");
      return true;
    }

    const ResolvedFields& r = fieldsForTypeDecl(rc, ct,
                                                DefaultsPolicy::USE_DEFAULTS);

    // Check for cycles. If a cycle is detected, emit an error
    // and return 'true'.
    int n = r.numForwards();
    for (int i = 0; i < n; i++) {
      auto qt = r.forwardingToType(i);
      if (auto t = qt.type()) {
        if (auto forwardingCt = t->getCompositeType()) {
          bool cyc = checkForwardingCycles(rc, forwardingCt, visited);
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
forwardingCycleCheckQuery(ResolutionContext* rc, const CompositeType* ct) {
  CHPL_RESOLUTION_QUERY_BEGIN(forwardingCycleCheckQuery, rc, ct);

  bool result = false;
  llvm::SmallPtrSet<const CompositeType*, 8> visited;

  result = checkForwardingCycles(rc, ct, visited);

  return CHPL_RESOLUTION_QUERY_END(result);
}

// returns 'true' if a forwarding cycle was detected & error emitted
static bool
emitErrorForForwardingCycles(ResolutionContext* rc, const CompositeType* ct) {
  bool cycleFound = false;
  if (typeUsesForwarding(rc->context(), ct)) {
    // check for cycles
    cycleFound = forwardingCycleCheckQuery(rc, ct);
  }

  return cycleFound;
}

static const CompositeType* getTypeWithDefaults(ResolutionContext* rc,
                                                const CompositeType* ct) {
  // resolve the fields with DefaultsPolicy=FOR_OTHER_FIELDS
  const ResolvedFields& g = fieldsForTypeDecl(rc, ct,
                                              DefaultsPolicy::USE_DEFAULTS_OTHER_FIELDS);
  if (!g.isGenericWithDefaults()) {
    return ct;
  }

  // and with DefaultsPolicy=USE
  const ResolvedFields& r = fieldsForTypeDecl(rc, ct,
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

  auto context = rc->context();
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
const CompositeType* const& getTypeWithDefaultsQuery(ResolutionContext* rc,
                                                     const CompositeType* ct) {
  CHPL_RESOLUTION_QUERY_BEGIN(getTypeWithDefaultsQuery, rc, ct);

  auto result = getTypeWithDefaults(rc, ct);

  return CHPL_RESOLUTION_QUERY_END(result);
}

const types::QualifiedType typeWithDefaults(ResolutionContext* rc,
                                            types::QualifiedType t) {
  if (t.type()) {
    if (auto clst = t.type()->toClassType()) {
      if (auto bct = clst->basicClassType()) {
        auto got = getTypeWithDefaultsQuery(rc, bct);
        CHPL_ASSERT(got->isBasicClassType());
        bct = got->toBasicClassType();

        auto r = ClassType::get(rc->context(), bct, clst->manager(), clst->decorator());
        return QualifiedType(t.kind(), r, t.param());
      }
    } else if (auto ct = t.type()->toCompositeType()) {
      auto got = getTypeWithDefaultsQuery(rc, ct);
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

  DefaultsPolicy defaultsPolicy = DefaultsPolicy::USE_DEFAULTS_OTHER_FIELDS;
  // NOTE: Assuming that syntaxOnly means we don't need proper RC from elsewhere
  ResolutionContext rc(context);
  const ResolvedFields& f = fieldsForTypeDecl(&rc, ct,
                                              defaultsPolicy,
                                              /* syntaxOnly */ true);

  if (f.isGenericWithDefaults() &&
      (g == Type::CONCRETE || g == Type::GENERIC_WITH_DEFAULTS))
    return Type::GENERIC_WITH_DEFAULTS;

  if (f.isGeneric())
    return Type::GENERIC;

  return g;
}

static Type::Genericity getInterfaceActualsGenericity(Context* context,
                                                      const InterfaceType* ift,
                                                      std::set<const Type*>& ignore) {
  // add the current type to the ignore set, and stop now
  // if it is already in the ignore set.
  auto it = ignore.insert(ift);
  if (it.second == false) {
    // set already contained ct, so stop & consider it concrete
    return Type::CONCRETE;
  }

  if (ift->substitutions().empty()) return Type::GENERIC;

  auto itf = parsing::idToAst(context, ift->id())->toInterface();
  CHPL_ASSERT(itf);
  for (auto formal : itf->formals()) {
    // if the substitutions aren't empty, expect substitutions for all types
    auto& qt = ift->substitutions().at(formal->id());
    if (getTypeGenericityIgnoring(context, qt.type(), ignore) != Type::CONCRETE) {
      return Type::GENERIC;
    }
  }

  return Type::CONCRETE;
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

  if (auto pt = t->toPtrType()) {
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
  CHPL_ASSERT(t->isCompositeType() || t->isClassType() || t->isInterfaceType());

  if (auto ift = t->toInterfaceType()) {
    return getInterfaceActualsGenericity(context, ift, ignore);
  }

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

static const Type::Genericity& getTypeGenericityViaPtrQuery(Context* context, const Type* t) {
  QUERY_BEGIN(getTypeGenericityViaPtrQuery, context, t);

  std::set<const Type*> ignore;
  auto result = getTypeGenericityIgnoring(context, t, ignore);

  return QUERY_END(result);
}

Type::Genericity getTypeGenericity(Context* context, const Type* t) {
  return getTypeGenericityViaPtrQuery(context, t);
}

static const Type::Genericity& getTypeGenericityViaQualifiedTypeQuery(Context* context, QualifiedType qt) {
  QUERY_BEGIN(getTypeGenericityViaQualifiedTypeQuery, context, qt);

  std::set<const Type*> ignore;
  auto result = getTypeGenericityIgnoring(context, qt, ignore);

  return QUERY_END(result);
}

Type::Genericity getTypeGenericity(Context* context, QualifiedType qt) {
  return getTypeGenericityViaQualifiedTypeQuery(context, qt);
}

static bool callHasQuestionMark(const FnCall* call) {
  for (auto actual : call->actuals()) {
    if (auto ident = actual->toIdentifier()) {
      if (ident->name() == "?") {
        return true;
      }
    }
  }

  return false;
}

static const AstNode* unwrapClassCall(const Call* call, bool& outConcreteManagement) {
  const AstNode* unwrapped = call;
  outConcreteManagement = false;

  if (auto fnCall = call->toFnCall()) {
    if (parsing::isCallToClassManager(fnCall)) {
      if (call->numActuals() == 1) {
        unwrapped = call->actual(0);
        outConcreteManagement = true;
      }
    }
  }

  if (unwrapped) {
    if (auto opCall = unwrapped->toOpCall()) {
      if (opCall->numActuals() == 1 && opCall->op() == "?") {
        unwrapped = opCall->actual(0);
      }
    }
  }

  return unwrapped;
}

static bool isScopeResolvedExprGeneric(Context* context,
                                       ResolutionResultByPostorderID& rr,
                                       const AstNode* expr,
                                       std::set<const Type*>& ignore) {
  bool isConcreteManagement = false;
  if (auto call = expr->toCall()) {
    expr = unwrapClassCall(call, isConcreteManagement);
  }

  auto ident = expr->toIdentifier();
  auto call = expr->toFnCall();
  if (call) {
    ident = call->calledExpression()->toIdentifier();
  }

  if (call && callHasQuestionMark(call)) {
    return true;
  }

  if (ident) {
    if (isNameBuiltinGenericType(context, ident->name())) {
      return true;
    }

    auto& re = rr.byAst(ident);
    if (re.toId().isEmpty()) {
      // If we can't find this, treat it as concrete.
    } else {
      auto& toId = re.toId();
      auto toTag = parsing::idToTag(context, toId);
      const CompositeType* initialType = nullptr;

      if (asttags::isAggregateDecl(toTag)) {
        initialType = initialTypeForTypeDecl(context, toId)->getCompositeType();
      }

      if (asttags::isClass(toTag) && !isConcreteManagement &&
          ignore.find(initialType) == ignore.end()) {
        // classes without 'shared' or 'owned' are generic (generic management),
        // regardless if whether the class' fields are generic or not.
        //
        // except for things marked with `pragma "data class"`, which are
        // magical and don't get managed.
        if (!initialType->hasPragma(context, PRAGMA_DATA_CLASS)) {
          return true;
        }
      }

      // if the type is generic, then the resolved expression that points to
      // this type is generic too, _unless_ it's used as part of a call to
      // a type constructor to create a concrete instance of the type (in
      // which case there is a 'call').
      if (asttags::isAggregateDecl(toTag) && !call) {
        if (getTypeGenericityIgnoring(context, initialType, ignore) != Type::CONCRETE) {
          return true;
        }
      }
    }
  }

  // at this point, if we have a call it's got 0+ actuals, none of which are '?',
  // and the base expression is either a record or a managed class type.
  // Check if any of the actuals are generic, and take that to assume the
  // whole call is generic.
  if (call) {
    for (auto actual : call->actuals()) {
      if (isScopeResolvedExprGeneric(context, rr, actual, ignore)) {
        return true;
      }
    }
  }

  return false;
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
                                              types::QualifiedType* outFormalType,
                                              std::set<const Type*>& ignore) {
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

  auto aggregateDeclId = var->id().parentSymbolId(context);
  auto aggregateDecl = parsing::idToAst(context, aggregateDeclId)->toAggregateDecl();
  CHPL_ASSERT(aggregateDecl);

  // Performance: this scope resolution could be put behind a query if it
  //              impacts performance too much.
  ResolutionResultByPostorderID rr;
  auto visitor =
    Resolver::createForScopeResolvingField(context, aggregateDecl,
                                           var, rr);
  var->traverse(visitor);

  outIsGeneric = isScopeResolvedExprGeneric(context, rr, var->typeExpression(), ignore);
  return true;
}

bool isFieldSyntacticallyGenericIgnoring(Context* context,
                                         const ID& fieldId,
                                         types::QualifiedType* formalType,
                                         std::set<const Type*>& typeGenericities) {
  // compare with AggregateType::fieldIsGeneric

  auto var = parsing::idToAst(context, fieldId)->toVariable();
  CHPL_ASSERT(var);

  bool isGeneric = false;
  if (isVariableDeclWithClearGenericity(context, var, isGeneric, formalType, typeGenericities)) {
    return isGeneric;
  }

  // Today, in situations when the genericity is not clear, without further
  // information we assume the field is generic.
  CHPL_ASSERT(isGeneric == true);

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

      if (isVariableDeclWithClearGenericity(context, neighborVar, isGeneric, formalType, typeGenericities)) {
        break;
      }
    }
  }

  return isGeneric;
}

bool isFieldSyntacticallyGeneric(Context* context,
                                 const ID& fieldId,
                                 types::QualifiedType* formalType) {
  // Performance: this might be made into a query, why not?
  std::set<const Type*> typeGenericities;
  return isFieldSyntacticallyGenericIgnoring(context, fieldId, formalType, typeGenericities);
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
  if (auto ct = t->getCompositeType()) {
    id = ct->id();
  } else if (auto ift = t->toInterfaceType()) {
    id = ift->id();
  } else {
    CHPL_ASSERT(false && "invalid argument to typeConstructorInitialQuery");
  }

  UniqueString name;
  std::vector<UntypedFnSignature::FormalDetail> formals;

  auto& br = buildTypeConstructor(context, id);

  if (br.numTopLevelExpressions() != 0) {
    auto typeCtor = br.topLevelExpression(0)->toFunction();

    // Build the UntypedFnSignature formals
    for (auto decl : typeCtor->formals()) {
      auto formal = decl->toFormal();
      bool hasDefault = formal->initExpression() != nullptr;
      auto defaultKind = hasDefault ? UntypedFnSignature::DK_DEFAULT
                                    : UntypedFnSignature::DK_NO_DEFAULT;

      auto d = UntypedFnSignature::FormalDetail(formal->name(), defaultKind, decl, decl->isVarArgFormal());
      formals.push_back(d);
    }

    auto untyped = UntypedFnSignature::get(context,
                                           typeCtor->id(), name,
                                           /* isMethod */ false,
                                           /* isTypeConstructor */ true,
                                           /* isCompilerGenerated */ true,
                                           /* throws */ false,
                                           asttags::Function,
                                           Function::PROC,
                                           std::move(formals),
                                           /* whereClause */ nullptr,
                                           id);

    ResolutionContext rcval(context);
    result = typedSignatureInitial(&rcval, untyped);
  }

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
  CHPL_ASSERT(canPass(context, actualType, formalType).instantiates() ||
              formalType.isType());

  // there are some tricky cases around '_owned' (record) and our internal 'owned'
  // representation (e.g.). 'owned' -> `_owned' cannot create conversions, but
  // the other way could (if the actual is a manager record, we turn it into
  // the internal 'owned' representation, then borrow it). Do this wrapping
  // now, if needed.
  tryConvertClassTypeOutOfManagerRecordIfNeeded(context, actualT, formalT);

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
      const BasicClassType* bct = nullptr;
      auto formalBct = formalCt->basicClassType();
      if (formalBct && getTypeGenericity(context, formalBct) == Type::CONCRETE) {
        bct = formalBct;
      } else if (!formalBct) {
        bct = actualCt->basicClassType();
      } else {
        // search for a common parent. in the case of Parent(X=?) being instantiated
        // with Child(X, Y), need to get Parent(X).

        auto actualBct = actualCt->basicClassType();
        while (actualBct) {
          if (actualBct->id() == formalBct->id()) {
            bct = actualBct;
            break;
          }

          actualBct = actualBct->parentClassType();
        }

        if (!bct) {
          // no common parent found, so use the actual's basic class type
          bct = actualCt->basicClassType();
        }
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

std::pair<optional<QualifiedType>, RequiredSignedness> const&
initialNumericValueOfEnumElement(Context* context, ID elementId) {
  QUERY_BEGIN(initialNumericValueOfEnumElement, context, elementId);

  auto declId = elementId.parentSymbolId(context);
  auto elem = parsing::idToAst(context, elementId)->toEnumElement();
  auto decl = parsing::idToAst(context, declId)->toEnum();

  auto one = QualifiedType::makeParamInt(context, 1);

  ResolutionResultByPostorderID byPostorder;
  Resolver res = Resolver::createForEnumElements(context, decl, byPostorder);

  const EnumElement* prevElem = nullptr;
  auto enumType = initialTypeForTypeDecl(context, declId);
  for (auto elem : decl->enumElements()) {
    if (elem->id() == elementId) {
      break;
    }

    // The resolver will try using "local results" (i.e., consult
    // byPostorder) for the enum elements, so give them the type they ought
    // to have.
    auto paramVal =
      EnumParam::EnumValue(elem->id(), elem->name().str());
    auto param =
        QualifiedType(QualifiedType::PARAM, enumType,
                      EnumParam::get(context, paramVal));
    byPostorder.byAst(elem).setType(param);

    prevElem = elem;
  }

  // Found an initialization expression; use its type.
  CHPL_ASSERT(elem);
  optional<QualifiedType> value = empty;
  if (elem->initExpression()) {
    elem->traverse(res);
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
    if (prevElem == nullptr) {
      // we're the first value, without an init expression we're abstract.
      value = empty;
    } else {
      auto& prevValue = initialNumericValueOfEnumElement(context, prevElem->id());
      if (!prevValue.first) {
        // the previous value is abstract, which means so are we.
        value = empty;
      } else {
        auto& lastQt = *prevValue.first;
        if (lastQt.isParam()) {
          // Previous value was valid, so add one to it.
          value = Param::fold(context, elem, chpl::uast::PRIM_ADD, lastQt, one);
        } else {
          // Previous value was unknown, so we can't add one to it.
          value = QualifiedType();
        }
      }
    }
  }

  RequiredSignedness signedness = RS_NONE;
  if (!value || !value->param()) {
    // Do nothing.
  } else if (auto intParam = value->param()->toIntParam()) {
    signedness = intParam->value() < 0 ? RS_SIGNED : RS_NONE;
  } else if (auto uintParam = value->param()->toUintParam()) {
    signedness = uintParam->value() > INT64_MAX ? RS_UNSIGNED : RS_NONE;
  }

  auto result = std::make_pair(std::move(value), signedness);
  return QUERY_END(result);
}

const std::map<ID, QualifiedType>&
computeNumericValuesOfEnumElements(Context* context, ID node) {
  QUERY_BEGIN(computeNumericValuesOfEnumElements, context, node);

  std::map<ID, types::QualifiedType> result;
  auto ast = parsing::idToAst(context, node);
  if (!ast) return QUERY_END(result);
  auto enumNode = ast->toEnum();
  if (!enumNode) return QUERY_END(result);

  // First collect all the values, no matter what types they are.
  using ValueVector = std::vector<std::tuple<QualifiedType,
                                             RequiredSignedness,
                                             const EnumElement*>>;
  ValueVector valuesAndAsts;
  for (auto elem : enumNode->enumElements()) {
    auto [maybeValue, signedness] =
        initialNumericValueOfEnumElement(context, elem->id());
    auto value = maybeValue ? *maybeValue : QualifiedType();
    valuesAndAsts.emplace_back(value, signedness, maybeValue ? elem : nullptr);
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
        resultType = QualifiedType::makeParamInt(context, *signedValue);
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

const std::pair<ID, bool>&
scopeResolveEnumElement(Context* context,
                        const Enum* enumAst,
                        UniqueString elementName,
                        const AstNode* nodeForErr) {
  QUERY_BEGIN(scopeResolveEnumElement, context, enumAst, elementName, nodeForErr);
  bool ambiguous = false;
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
    ambiguous = true;
  } else {
    auto ret = std::make_pair(ids.firstId(), ambiguous);
    return QUERY_END(ret);
  }

  auto ret = std::make_pair(ID(), ambiguous);
  return QUERY_END(ret);
}

QualifiedType
typeForScopeResolvedEnumElement(Context* context,
                                const EnumType* enumType,
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
typeForScopeResolvedEnumElement(Context* context,
                                const ID& enumTypeId,
                                const ID& refersToId,
                                bool ambiguous) {
  auto type = initialTypeForTypeDecl(context, enumTypeId);
  CHPL_ASSERT(type && type->isEnumType());
  return typeForScopeResolvedEnumElement(context, type->toEnumType(),
                                         refersToId, ambiguous);
}

const QualifiedType&
typeForEnumElement(Context* context,
                   const EnumType* enumType,
                   UniqueString elementName,
                   const AstNode* nodeForErr) {
  QUERY_BEGIN(typeForEnumElement, context, enumType, elementName, nodeForErr);
  auto enumAst = parsing::idToAst(context, enumType->id())->toEnum();
  CHPL_ASSERT(enumAst != nullptr);
  auto& [refersToId, ambiguous] =
    scopeResolveEnumElement(context, enumAst, elementName, nodeForErr);
  auto qt = typeForScopeResolvedEnumElement(context, enumType, refersToId, ambiguous);
  if (refersToId.isEmpty() && !ambiguous) {
    // scopeResolveEnumElement doesn't report a "not found" error because
    // not being able to find an enum element isn't always an error. Here,
    // though, we are specifically interested in an element, so report
    // the error.
    CHPL_REPORT(context, UnknownEnumElem, nodeForErr, elementName, enumAst);
  }
  return QUERY_END(qt);
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

static Resolver createResolverForAst(ResolutionContext* rc,
                                     const Function* fn,
                                     const AggregateDecl* ad,
                                     const Enum* ed,
                                     const SubstitutionsMap& substitutions,
                                     const PoiScope* poiScope,
                                     ResolutionResultByPostorderID& r) {
  if (fn != nullptr) {
    return Resolver::createForInstantiatedSignature(rc, fn, substitutions,
                                                    poiScope, r);
  } else if (ad != nullptr) {
    return Resolver::createForInstantiatedSignatureFields(rc->context(), ad,
                                                          substitutions,
                                                          poiScope, r);
  } else {
    CHPL_ASSERT(ed != nullptr);
    return Resolver::createForEnumElements(rc->context(), ed, r);
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

// if the signature is a function and has an AST, fn should be that AST.
// This is used to do some other checking, like observing return intents.
static bool allowPromotionForSig(const TypedFnSignature* sig,
                                 const uast::Function* fn) {
  // Functions that return 'param' or 'type' cannot be promoted
  if (fn && (fn->returnIntent() == uast::Function::TYPE ||
             fn->returnIntent() == uast::Function::PARAM)) {
    return false;
  }

  const UntypedFnSignature* untypedSignature = sig->untyped();
  return
    untypedSignature->name() != USTR("these") &&
    untypedSignature->name() != USTR("=");
}

static bool instantiateAcrossManagerRecordConversion(Context* context,
                                                     const QualifiedType& formal,
                                                     const QualifiedType& actual,
                                                     QualifiedType& outInstType) {
  auto formalT = formal.type();
  auto actualT = actual.type();
  if (tryConvertClassTypeIntoManagerRecordIfNeeded(context, formalT, actualT)) {
    outInstType = QualifiedType(formal.kind(), actualT);
    return true;
  } else if (tryConvertClassTypeOutOfManagerRecordIfNeeded(context, actualT, formalT)) {
    outInstType = QualifiedType(formal.kind(), actualT);
    return true;
  }
  return false;
}

// TODO: We could remove the 'ResolutionContext' argument if we figure out
// a different way/decide not to resolve initializer bodies down below.
static ApplicabilityResult
instantiateSignatureImpl(ResolutionContext* rc,
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

  Context* context = rc->context();
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

  auto canPassFn = allowPromotionForSig(sig, fn) ? canPass : canPassScalar;

  // If we are instantiating a nested function, then its parents should
  // already be fully instantiated, in order for assumptions made during
  // resolution to hold (the expectation is that the bodies of nested
  // functions are fully resolved from the outermost parent down).
  const TypedFnSignature* parentSignature = sig->parentFn();
  if (parentSignature) {
    for (auto up = parentSignature; up; up = up->parentFn()) {
      if (up->needsInstantiation()) {
        CHPL_UNIMPL("parent function needs instantiation");
        return ApplicabilityResult::failure(sig, FAIL_CANDIDATE_OTHER);
      }
    }
  }

  auto faMap = FormalActualMap(sig, call);
  if (!faMap.isValid()) {
    return ApplicabilityResult::failure(sig, faMap.reason());
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
  auto visitor = createResolverForAst(rc, fn, ad, ed, substitutions,
                                      poiScope, r);

  // TODO: Stop copying these back in.
  visitor.outerVariables = sig->outerVariables();

  QualifiedType varArgType;
  for (const FormalActual& entry : faMap.byFormals()) {
    // Do not ignore substitutions initially
    visitor.ignoreSubstitutionFor = nullptr;
    visitor.skipTypeQueries = false;

    bool addSub = false;
    bool isInstantiated = false;
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

      // We allow UnknownType elements (e.g., didn't have enough information
      // to invoke the type constructor in a vararg type expression). However,
      // in such cases, their intent will be UNKNOWN, which makes code below
      // think we know less than we do. So, reset the intent to the var arg
      // declaration's intent.
      if (formalType.kind() == QualifiedType::UNKNOWN) {
        formalType = QualifiedType(entry.formalType().kind(), formalType.type(), formalType.param());
      }
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
      auto got = canPassFn(context, actualType, formalType);
      if (!got.passes()) {
        // Including past type information made this instantiation fail.
        return ApplicabilityResult::failure(sig, got.reason(), entry.formalIdx());
      }

      // If promotion was involved, figure out the scalar type. We want to
      // work with the scalar function, so we need the scalar type.
      auto scalarType = actualType;
      if (got.promotes()) {
        scalarType = getPromotionType(context, actualType);
      }

      if (got.instantiates() || formalType.isType()) {
        addSub = true;
        // add a substitution for a valid value
        //
        // if no conversion took place, then we can use the actual type. One
        // exception is the case when we transparently converted a
        // 'owned C' into a '_owned(C)'. This does not count as a conversion by
        // the canPass function (since, notionally these are two representations
        // of the same type), but we want to instantiate with whatever the
        // formal type was (e.g., if we're the formal is `_owned(C)`, instantiate
        // with `_owned(C)`).
        if (!got.converts()) {
          if (instantiateAcrossManagerRecordConversion(context, formalType, actualType, useType)) {
            // useType was set as an out parameter of the call in the condition.
            isInstantiated = true;
          } else if (formalType.isType() && !got.instantiates()) {
            // When a type formal's type expression is a specific instantiation,
            // e.g., `type T : R(int)`, we want to add a substitution but
            // should not consider it instantiated.
            useType = scalarType;
          } else {
            // use the actual type since no conversion/promotion was needed
            isInstantiated = true;
            useType = scalarType;
          }
        } else {
          isInstantiated = true;
          // get instantiation type
          useType = getInstantiationType(context,
                                         scalarType,
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

          auto got = canPassFn(context, scalarType, useTypeConcrete);
          if (!got.passes()) {
            return ApplicabilityResult::failure(sig, got.reason(), entry.formalIdx());
          }
        }
      }
    }

    // If the formal wasn't instantiated then use whatever type was computed.
    if (!addSub) useType = formalType;

    if (entry.isVarArgEntry()) {
      // If any formal needs instantiating then we need to instantiate all
      // the VarArgs
      instantiateVarArgs = instantiateVarArgs || addSub;

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

        if (isInstantiated) {
          formalsInstantiated.setBit(formalIdx, true);
        }
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

      // We've set up the type queries and re-traversed the formal AST to
      // compute the type using these queries. If the formal type is still
      // unknown at this point, we couldn't extract the type queries, which
      // means the call is ill-formed.
      if (qFormalType.isUnknownKindOrType()) {
        if (entry.hasActual()) {
          return ApplicabilityResult::failure(sig, FAIL_CANNOT_INSTANTIATE, entry.formalIdx());
        } else {
          // Something else has gone wrong. Use this failure kind to avoid
          // situations down the line where someone wants to know which
          // actual the failure corresponds to.
          return ApplicabilityResult::failure(sig, FAIL_UNKNOWN_FORMAL_TYPE, entry.formalIdx());
        }
      }

      auto checkType = !useType.isUnknown() ? useType : formalType;
      // With the type and query-aware type known, make sure that they're compatible
      auto passResult = canPassFn(context, checkType, qFormalType);
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
        visitor.setCompositeType(formalType.type()->getCompositeType());
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
      const TupleType* t = TupleType::getQualifiedTuple(context, varargsTypes,
                                                        /*isVarArgTuple=*/true);
      auto formal = faMap.byFormalIdx(varArgIdx).formal()->toVarArgFormal();
      QualifiedType vat = QualifiedType(formal->storageKind(), t);
      substitutions.insert({formal->id(), vat});
      r.byAst(formal).setType(vat);
      visitor.resolveVarArgSizeQuery(formal, t->numElements());

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
    if (!sig->isInitializer()) {
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
          return ApplicabilityResult::failure(sig, FAIL_VARARG_MISMATCH);
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

    // do not visit the field declarations directly; only visit those
    // that are relevant for computing the types of the formals. This
    // happens below.

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
      fieldDecl->traverse(visitor);
      const ResolvedExpression& e = r.byAst(fieldDecl);
      QualifiedType fieldType = e.type();
      QualifiedType sigType = sig->formalType(formalIdx);

      // use the same kind as the old formal type but update the type, param
      // to reflect how instantiation occurred.
      formalTypes.push_back(QualifiedType(sigType.kind(),
                                          fieldType.type(),
                                          fieldType.param()));

      if (isFieldSyntacticallyGeneric(context, fieldDecl->id())){
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
                                      /* parentFn */ parentSignature,
                                      std::move(formalsInstantiated),
                                      sig->outerVariables());

  // For initializers, may need to resolve the body to compute final TFS.
  // Don't do that here because disambiguation might discard the candidate.
  // Body resolution will be done by MostSpecificCandidate when constructed.

  return ApplicabilityResult::success(result);
}

static ApplicabilityResult const&
instantiateSignatureQuery(ResolutionContext* rc,
                          const TypedFnSignature* sig,
                          const CallInfo& call,
                          const PoiScope* poiScope) {
  CHPL_RESOLUTION_QUERY_BEGIN(instantiateSignatureQuery, rc, sig, call, poiScope);
  auto ret = instantiateSignatureImpl(rc, sig, call, poiScope);
  return CHPL_RESOLUTION_QUERY_END(ret);
}

ApplicabilityResult instantiateSignature(ResolutionContext* rc,
                                         const TypedFnSignature* sig,
                                         const CallInfo& call,
                                         const PoiScope* poiScope) {
  return instantiateSignatureQuery(rc, sig, call, poiScope);
}

// This implements the body of 'resolveFunctionByInfo'. It returns a new
// 'ResolvedFunction' and does not directly set any queries.
//
// It is possible for the signature recorded in the returned function to
// be different than the input signature. Right now this happens for
// initializers.
static owned<ResolvedFunction>
resolveFunctionByInfoImpl(ResolutionContext* rc, const TypedFnSignature* sig,
                          PoiInfo poiInfo) {
  Context* context = rc->context();
  const UntypedFnSignature* untypedSignature = sig->untyped();
  auto& id = untypedSignature->id();
  const AstNode* ast = parsing::idToAst(context, id);
  const Function* fn = ast->toFunction();
  const PoiScope* poiScope = poiInfo.poiScope();
  PoiInfo resolvedPoiInfo;
  ResolutionResultByPostorderID rr;

  // TODO: Make sure the ID is an extern function specifically.
  bool canResolveWithoutAst = parsing::idIsExtern(context, sig->id()) ||
                              sig->isInitializer();
  if (!fn && !canResolveWithoutAst) {
    CHPL_ASSERT(false && "Unexpected input to 'resolveFunction'!");
    return nullptr;
  }

  auto visitor = sig->isInitializer()
    ? Resolver::createForInitializer(rc, fn, poiScope, sig, rr)
    : Resolver::createForFunction(rc, fn, poiScope, sig, rr);

  if (sig->isInitializer()) {
    CHPL_ASSERT(visitor.initResolver.get());
    auto qt = QualifiedType(QualifiedType::VAR, VoidType::get(context));
    visitor.returnType = std::move(qt);
  }

  // Walk the body to resolve it.
  if (fn->body()) fn->body()->traverse(visitor);

  // There were outer variables we could not type, so give up.
  if (!visitor.outerVariables.isEmpty() && rc->isEmpty()) {
    return nullptr;
  }

  const TypedFnSignature* finalSig = sig;

  // then, compute the return type if it is not an initializer
  if (!sig->isInitializer()) {
    computeReturnType(visitor);

  // else, potentially write out a new initializer signature
  } else {
    finalSig = visitor.initResolver->finalize();
  }

  // then, resolve '=' and add any copy init/deinit calls as needed
  callInitDeinit(visitor);

  // then, handle return intent overloads and maybe-const formals
  adjustReturnIntentOverloadsAndMaybeConstRefs(visitor);

  // check that throws are handled or forwarded
  // TODO: Call for initializers as well, and remove checks in the resolver.
  if (!sig->isInitializer()) checkThrows(rc, rr, fn);

  // TODO: can this be encapsulated in a method?
  resolvedPoiInfo.swap(visitor.poiInfo);
  resolvedPoiInfo.setResolved(true);
  resolvedPoiInfo.setPoiScope(nullptr);

  CHPL_ASSERT(sig == finalSig || sig->isInitializer());

  auto ret = toOwned(new ResolvedFunction(finalSig,
                                  fn->returnIntent(),
                                  std::move(rr),
                                  std::move(resolvedPoiInfo),
                                  std::move(visitor.returnType),
                                  std::move(visitor.userDiagnostics),
                                  std::move(visitor.poiTraceToChild),
                                  std::move(visitor.sigAndInfoToChildPtr)));
  return ret;
}

// Forward declaration so that that query storage actions can be specialized.
static const ResolvedFunction* const&
resolveFunctionByInfoQuery(ResolutionContext* rc,
                           const TypedFnSignature* sig,
                           PoiInfo poiInfo);

static const ResolvedFunction* const&
resolveFunctionByResolvedInfoQuery(Context* context,
                                   const TypedFnSignature* sig,
                                   PoiInfo poiInfo);

static const owned<ResolvedFunction>&
resolveFunctionByPoisQuery(ResolutionContext* rc, PoiInfo::Trace poiTrace);

// TODO: This can be used later to implement the "rewinding" necessary to
// resolve an exterior call to a nested function. Right now it is not
// clear if that will ever be possible outside of the API, so it remains
// unimplemented (but this specialization will stay as a reminder).
//
// Right now, this specialization just performs the default behavior.
//
template <typename Args> struct
ResolutionContext::GlobalComputeSetup<resolveFunctionByInfoQuery, Args> {
  bool enter(ResolutionContext* rc, const Args& args) {
    return false;
  }

  void leave(ResolutionContext* rc, const Args& args) {}
};

// Implement unstable caching for 'resolveFunctionByInfoQuery' by looking
// for an appropriate PoiInfo/function stored within the parent's Resolver.
template <typename T, typename Args> struct
ResolutionContext::UnstableCache<resolveFunctionByInfoQuery, T, Args> {

  const T* fetchOrNull(ResolutionContext* rc, const Args& args) {
    static_assert(std::tuple_size_v<Args> == 2);
    auto sig = std::get<0>(args);

    if (auto rv = rc->findParentResolverFor(sig)) {
      auto& m = rv->sigAndInfoToChildPtr;
      ResolvedFunction::SigAndInfo k = { sig, std::get<1>(args) };
      auto it = m.find(k);
      if (it == m.end()) return nullptr;
      return &it->second;
    }

    return nullptr;
  }

  const T& store(ResolutionContext* rc, T x, const Args& args) {
    static_assert(std::tuple_size_v<Args> == 2);
    auto sig = std::get<0>(args);

    if (auto rv = rc->findParentResolverFor(sig)) {
      auto& m = rv->sigAndInfoToChildPtr;
      ResolvedFunction::SigAndInfo k = { sig, std::get<1>(args) };
      const ResolvedFunction* v = x;
      auto check = m.insert(std::make_pair(std::move(k), v));
      CHPL_ASSERT(check.second);
      return check.first->second;
    }

    CHPL_ASSERT(false && "Should not reach here!");
    auto& f = x != nullptr ? rc->lastFrameOrBaseMutable() : rc->baseFrame_;
    return f.cache(std::move(x));
  }
};

// Implement unstable caching for 'resolveFunctionByPoisQuery' by looking
// for an appropriate POI trace/function within the parent's Resolver.
template <typename T, typename Args> struct
ResolutionContext::UnstableCache<resolveFunctionByPoisQuery, T, Args> {

  const T* fetchOrNull(ResolutionContext* rc, const Args& args) {
    static_assert(std::tuple_size_v<Args> == 1);
    auto& poiTrace = std::get<0>(args);
    auto sig = std::get<0>(poiTrace);

    if (auto rv = rc->findParentResolverFor(sig)) {
      auto& m = rv->poiTraceToChild;
      auto it = m.find(poiTrace);
      if (it != m.end()) return &it->second;
    }

    return nullptr;
  }

  const T& store(ResolutionContext* rc, T x, const Args& args) {
    static_assert(std::tuple_size_v<Args> == 1);
    auto& poiTrace = std::get<0>(args);
    auto sig = std::get<0>(poiTrace);

    if (auto rv = rc->findParentResolverFor(sig)) {
      auto& m = rv->poiTraceToChild;
      auto p = std::make_pair(poiTrace, std::move(x));
      auto check = m.insert(std::move(p));
      CHPL_ASSERT(check.second);
      return check.first->second;
    }

    CHPL_ASSERT(false && "Should not reach here!");
    auto& f = x != nullptr ? rc->lastFrameOrBaseMutable() : rc->baseFrame_;
    return f.cache(std::move(x));
  }
};

static const owned<ResolvedFunction>&
resolveFunctionByPoisQuery(ResolutionContext* rc, PoiInfo::Trace poiTrace) {
  CHPL_RESOLUTION_QUERY_BEGIN(resolveFunctionByPoisQuery, rc, poiTrace);

  auto& rcq = CHPL_RESOLUTION_REF_TO_CURRENT_QUERY_HANDLE();
  if (rcq.canUseGlobalCache()) {
    // We should not have made it this far if we can recall from the global
    // context query cache. The query guard should have returned early.
    //
    // The actual value is set in 'resolveFunctionByInfoQuery' after it is
    // computed, because resolving the function's body generates the POI
    // trace, which is the key for this query.
    CHPL_ASSERT(false && "Should be set in 'resolveFunctionByInfoQuery'!");
  }

  // Just return an empty pointer, otherwise.
  owned<ResolvedFunction> ret;
  return CHPL_RESOLUTION_QUERY_END(ret);
}

// Why does this query exist in addition to resolveFunctionByInfoQuery?
//
// Overall, the goal is to enable generic instantiation caching. There are,
// in fact, two kinds of PoiInfo: resolved and unresolved ones, which
// correspond loosely to "before instantiation" and "after instantiation".
//
// Unresolved PoI infos simply contain a PoI scope in which a function's
// body should be resolved. Caching these queries would prevent having
// to re-resolve a function called mutliple times from the same scope, but
// it will not do anything clever about PoI compatibility (see see PR #16261, e.g.).
//
// Resolved PoI infos don't contain the scope in which they were resolved; instead,
// they contain a trace of all functions that used PoI. By the logic described
// in PR #16261, the same instantiation can be re-used in another PoI scope if
// all PoI calls in its trace are visible from that other scope. As a special
// case, a function that didn't use any PoI calls can be re-used in any scope.
//
// Resolved PoI infos have two important properties:
// 2. They can only be constructed after resolving a function instantiation.
//    Pragmatically, this means that they can't serve as a-priori inputs
//    to "resolveFunction" (otherwise, we'd need to resolve the function
//    to resolve the function).
//
//    This hints at the need for two queries, one that gets called with the
//    "initial" PoI scope, and one that gets set after the function is resolved.
//
// 1. Importantly, by removing the PoI scope and replacing it with the trace,
//    resolved PoiInfos actually describe _sets of PoI scopes_ that are compatible
//    with a given instantiation for the purposes of reuse. The compatibility
//    check is encoded in PoiInfo::canReuse. In fact, to fit into the query framework,
//    the '==' operator is relaxed to "compatibility": uInfo == rInfo is true
//    if the unresolved info uInfo is compatible with the resolved info rInfo.
//
//    This essentially forces a query that operates on "resolved" infos to
//    not have any dependencies, because it represents a related set of
//    resolutions, each of which would presumably have different dependencies.
//    Having incompatible dependencies would wreak havoc on the query system.
//    Instead, we have '...ByInfoQuery', which includes all the dependencies
//    of resolving the function, and '...ByResolvedInfoQuery', which is
//    set and used when possible.
static const ResolvedFunction* const&
resolveFunctionByResolvedInfoQuery(Context* context,
                                   const TypedFnSignature* sig,
                                   PoiInfo poiInfo) {
  QUERY_BEGIN_EXTERNALLY_SET(resolveFunctionByResolvedInfoQuery, context, sig, poiInfo);

  // should only ever be SET by 'resolveFunctionByInfoQuery'. Since we got
  // here, something is wrong; return a bogus result.
  const ResolvedFunction* ret = nullptr;

  return QUERY_END(ret);
}

static const ResolvedFunction* const&
resolveFunctionByInfoQuery(ResolutionContext* rc,
                           const TypedFnSignature* sig,
                           PoiInfo poiInfo) {
  CHPL_RESOLUTION_QUERY_BEGIN(resolveFunctionByInfoQuery, rc, sig, poiInfo);

  const ResolvedFunction* ret = nullptr;
  // note: this lookup searches for "compatible" post-resolution POI infos
  //       because we're feeding it an unresolved PoI info, and its
  //       entries are always stored from resolved PoI infos. Operator ==
  //       decides compatibility in that case. See comment on
  //       resolveFunctionByResolvedInfoQuery, and the implementation
  //       of PoiInfo::operator==.
  if (rc->context()->hasCurrentResultForQuery(resolveFunctionByResolvedInfoQuery,
                                              std::make_tuple(sig, poiInfo))) {
    ret = resolveFunctionByResolvedInfoQuery(rc->context(), sig, poiInfo);
  } else {
    // Call the implementation which resolves the function body.
    auto resolved = resolveFunctionByInfoImpl(rc, sig, poiInfo);

    // The final signature should only differ for initializers.
    auto finalSig = resolved->signature();
    CHPL_ASSERT(finalSig == sig || sig->isInitializer());

    // Make a POI trace for use with the generic cache.
    auto resolvedPoiTrace = resolved->poiInfo().createTraceFor(sig);

    // Try to store in the generic cache.
    CHPL_RESOLUTION_QUERY_UNSAFE_STORE_RESULT(resolveFunctionByPoisQuery, rc,
        std::move(resolved),
        resolvedPoiTrace);

    // Initializer signatures can potentially require their body to be
    // resolved in order to complete instantiation. This means that the
    // 'ResolvedFunction' can have a different signature than what you
    // started with (the receiver or a dependent type could instantiate).
    //
    // The user could pass in the fully resolved initializer signature
    // in a subsequent call, and we don't want to resolve the function
    // body again. So record a second entry for 'resolveFunctionByInfoQuery'
    // which links the fully resolved signature to the result.
    if (finalSig != sig) {
      auto& saved = resolveFunctionByPoisQuery(rc, resolvedPoiTrace);
      CHPL_RESOLUTION_QUERY_UNSAFE_STORE_RESULT(resolveFunctionByInfoQuery, rc,
          saved.get(),
          finalSig,
          poiInfo);
    }

    // Return the unique result from the query (that might have been saved
    // above - if it was not saved, then we are reusing a cached result,
    // which means the input and final signatures can differ.
    auto& saved = resolveFunctionByPoisQuery(rc, resolvedPoiTrace);
    ret = saved.get();

    QUERY_STORE_RESULT(resolveFunctionByResolvedInfoQuery, rc->context(),
                       ret, sig, saved->poiInfo());
  }

  return CHPL_RESOLUTION_QUERY_END(ret);
}

static const ResolvedFunction*
helpResolveFunction(ResolutionContext* rc, const TypedFnSignature* sig,
                    const PoiScope* poiScope,
                    bool skipIfRunning) {
  // Forget about any inferred signature (to avoid resolving the
  // same function twice when working with inferred 'out' formals)
  sig = sig->inferredFrom();

  // Signature should be concrete by now, except in the case of an initializer
  // or type constructor in which case we may still have generic formals.
  // For example, range(?) will reach this point.
  if (!sig->isInitializer() && !sig->untyped()->isTypeConstructor() &&
      sig->needsInstantiation()) {
    CHPL_ASSERT(false &&
                "Should only be called on concrete or fully "
                "instantiated functions");
    return nullptr;
  } else if (sig->untyped()->idIsExternBlockFunction()) {
    CHPL_ASSERT(false && "Should not be called on functions in extern blocks");
  }

  // construct the PoiInfo for this case
  auto poiInfo = PoiInfo(poiScope);

  if (skipIfRunning) {
    constexpr auto f = resolveFunctionByInfoQuery;
    if (CHPL_RESOLUTION_IS_GLOBAL_QUERY_RUNNING(f, rc, sig, poiInfo)) {
      return nullptr;
    }

    // unstable resolver frames mean no query is running, but we could
    // still be recursively processing the function. This is still
    // considered "running".
    for (auto& frame : rc->frames()) {
      if (frame.signature() == sig) {
        return nullptr;
      }
    }
  }

  // lookup in the map using this PoiInfo
  return resolveFunctionByInfoQuery(rc, sig, std::move(poiInfo));
}

const TypedFnSignature*
inferRefMaybeConstFormals(ResolutionContext* rc,
                          const TypedFnSignature* sig,
                          const PoiScope* poiScope) {
  Context* context = rc->context();
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

  const bool skipIfRunning = true;
  auto rFn = helpResolveFunction(rc, sig, poiScope, skipIfRunning);

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

static std::vector<std::tuple<const Decl*, QualifiedType>>
collectGenericFormals(Context* context, const TypedFnSignature* tfs) {
  std::vector<std::tuple<const Decl*, QualifiedType>> ret;

  // Skip the 'this' formal since it will always be generic if one of the
  // "real" formals is generic.
  int formalIdx = 0;
  if (tfs->isMethod()) {
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

bool checkUninstantiatedFormals(Context* context, const AstNode* astForErr, const TypedFnSignature* sig) {
  if (!sig->needsInstantiation()) return false;

  CHPL_REPORT(context, MissingFormalInstantiation,
              astForErr,
              collectGenericFormals(context, sig));
  return true;
}

const ResolvedFunction* resolveFunction(ResolutionContext* rc,
                                        const TypedFnSignature* sig,
                                        const PoiScope* poiScope,
                                        bool skipIfRunning) {
  return helpResolveFunction(rc, sig, poiScope, skipIfRunning);
}

static const ImplementationPoint* const&
resolveImplementsStmtQuery(Context* context, ID id) {
  QUERY_BEGIN(resolveImplementsStmtQuery, context, id);
  const ImplementationPoint* result = nullptr;

  auto byPostorder = resolveModuleStmt(context, id);
  auto ast = parsing::idToAst(context, id);
  CHPL_ASSERT(ast->isImplements());
  auto impl = ast->toImplements();

  auto interfaceExpr = impl->interfaceExpr();
  QualifiedType interfaceQt;
  if (auto interfaceIdent = interfaceExpr->toIdentifier()) {
    interfaceQt = byPostorder.byAst(interfaceIdent).type();
  } else if (auto interfaceCall = interfaceExpr->toFnCall()) {
    interfaceQt = byPostorder.byAst(interfaceCall->calledExpression()).type();
  }

  if (!interfaceQt.isType() || interfaceQt.isUnknown() ||
      !interfaceQt.type()->isInterfaceType()) {
    CHPL_REPORT(context, InvalidImplementsInterface, impl, interfaceQt);
  } else {
    auto genericIft = interfaceQt.type()->toInterfaceType();
    std::vector<QualifiedType> actuals;

    auto addActual = [&byPostorder, &actuals, context, impl](const AstNode* actual) {
      auto& actualType = byPostorder.byAst(actual).type();
      if (actualType.isUnknownOrErroneous()) {
        return false;
      } else if (!actualType.isType()) {
        CHPL_REPORT(context, InvalidImplementsActual, impl, actual, actualType);
        return false;
      }
      actuals.push_back(actualType);
      return true;
    };

    bool addPoint = true;
    if (auto typeIdent = impl->typeIdent()) {
      addPoint = addActual(typeIdent);
    }
    if (auto interfaceCall = impl->interfaceExpr()->toFnCall()) {
      for (auto actual : interfaceCall->actuals()) {
        if (!addPoint) break; // already found a broken actual
        addPoint &= addActual(actual);
      }
    }

    if (addPoint) {
      auto ift = InterfaceType::withTypes(context, genericIft, actuals);
      if (!ift) {
        CHPL_REPORT(context, InvalidImplementsArity, impl, genericIft, actuals);
      } else {
        result = ImplementationPoint::get(context, ift, id);
      }
    }
  }

  return QUERY_END(result);
}

const ImplementationPoint* resolveImplementsStmt(Context* context,
                                                 ID id) {
  return resolveImplementsStmtQuery(context, id);
}

static const std::map<ID, std::vector<const ImplementationPoint*>>&
collectImplementationPointsInModule(Context* context,
                                    const uast::Module* module) {
  QUERY_BEGIN(collectImplementationPointsInModule, context, module);
  std::map<ID, std::vector<const ImplementationPoint*>> byInterfaceId;

  for (auto stmt : module->stmts()) {
    if (auto ad = stmt->toAggregateDecl()) {
      auto& implPoints = getImplementedInterfaces(context, ad);
      for (auto implPoint : implPoints) {
        byInterfaceId[implPoint->interface()->id()].push_back(implPoint);
      }
    } else if (auto implements = stmt->toImplements()) {
      auto implPoint = resolveImplementsStmt(context, implements->id());
      if (implPoint) {
        byInterfaceId[implPoint->interface()->id()].push_back(implPoint);
      }
    }
  }

  return QUERY_END(byInterfaceId);
}

static const std::map<ID, std::vector<const ImplementationPoint*>>&
collectImplementationPointsInScope(Context* context,
                                   const Scope* scope) {
  QUERY_BEGIN(collectImplementationPointsInScope, context, scope);
  CHPL_ASSERT(scope->moduleScope() == scope);

  auto module = parsing::idToAst(context, scope->id())->toModule();
  auto& result = collectImplementationPointsInModule(context, module);

  return QUERY_END(result);
}

static void
helpCollectVisibileImplementationPoints(Context* context,
                                        const Scope* scope,
                                        std::unordered_set<const Scope*>& seen,
                                        std::map<ID, std::vector<const ImplementationPoint*>>& into) {
  auto insertResult = seen.insert(scope);
  if (!insertResult.second) return;

  auto& inScope = collectImplementationPointsInScope(context, scope);
  for (auto& pointsForScope : inScope) {
    auto& copyInto = into[pointsForScope.first];
    for (auto implPoint : pointsForScope.second) {
      copyInto.push_back(implPoint);
    }
  }

  if (auto visStmts = resolveVisibilityStmts(context, scope)) {
    for (auto visClause : visStmts->visibilityClauses()) {
      auto nextScope = visClause.scope();
      if (nextScope && asttags::isModule(nextScope->tag())) {
        helpCollectVisibileImplementationPoints(context, nextScope, seen, into);
      }
    }
  }
}

static const std::map<ID, std::vector<const ImplementationPoint*>>&
visibleImplementationPoints(Context* context,
                            const Scope* scope,
                            const PoiScope* poiScope) {
  QUERY_BEGIN(visibleImplementationPoints, context, scope, poiScope);
  std::map<ID, std::vector<const ImplementationPoint*>> result;
  std::unordered_set<const Scope*> seen;
  helpCollectVisibileImplementationPoints(context, scope->moduleScope(), seen, result);
  for (; poiScope; poiScope = poiScope->inFnPoi()) {
    auto inScope = poiScope->inScope()->moduleScope();
    helpCollectVisibileImplementationPoints(context, inScope, seen, result);
  }
  return QUERY_END(result);
}

const std::vector<const ImplementationPoint*>*
visibileImplementationPointsForInterface(Context* context,
                                         const Scope* scope,
                                         const PoiScope* poiScope,
                                         ID id) {
  auto& allInstantiationPoints = visibleImplementationPoints(context, scope, poiScope);
  auto it = allInstantiationPoints.find(id);
  if (it != allInstantiationPoints.end()) {
    return &it->second;
  }

  return nullptr;
}

const ResolvedFunction* resolveConcreteFunction(Context* context, ID id) {
  if (id.isEmpty())
    return nullptr;

  ResolutionContext rcval(context);
  auto rc = &rcval;
  const UntypedFnSignature* uSig = UntypedFnSignature::get(context, id);
  const TypedFnSignature* sig = typedSignatureInitial(rc, uSig);

  if (sig == nullptr || sig->needsInstantiation()) {
    return nullptr;
  }

  auto whereFalse = TypedFnSignature::WhereClauseResult::WHERE_FALSE;
  if (sig->whereClauseResult() == whereFalse) {
    return nullptr;
  }

  const PoiScope* poiScope = nullptr;
  auto ret = resolveFunction(&rcval, sig, poiScope);
  return ret;
}

static owned<ResolvedFunction>
scopeResolveFunctionQueryBody(Context* context, ID id) {
  const AstNode* ast = parsing::idToAst(context, id);
  const Function* fn = ast->toFunction();

  ResolutionResultByPostorderID resolutionById;
  const TypedFnSignature* sig = nullptr;
  owned<ResolvedFunction> result;

  if (fn) {
    auto visitor =
      Resolver::createForScopeResolvingFunction(context, fn, resolutionById);

    // visit the children of fn to scope resolve
    // (visiting the children because visiting a function will not
    //  cause it to be scope resolved).
    for (auto child: fn->children()) {
      child->traverse(visitor);
    }

    checkForParenlessMethodFieldRedefinition(visitor, fn);

    sig = visitor.typedSignature;
  }

  result = toOwned(new ResolvedFunction(sig, fn->returnIntent(),
                                        std::move(resolutionById),
                                        PoiInfo(),
                                        QualifiedType(),
                                        {}, {}, {}));
  return result;
}

static const owned<ResolvedFunction>&
scopeResolveFunctionQuery(Context* context, ID id) {
  QUERY_BEGIN(scopeResolveFunctionQuery, context, id);
  auto ret = scopeResolveFunctionQueryBody(context, id);
  return QUERY_END(ret);
}

const ResolvedFunction* scopeResolveFunction(Context* context, ID id) {
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
      auto res = Resolver::createForParentClassScopeResolve(context, ad, inheritExpr, result);
      inheritExpr->traverse(res);
    }

    // TODO: Use some kind of "ad->fields()" iterator
    for (auto child : ad->children()) {
      if (child->isVarLikeDecl() ||
          child->isMultiDecl() ||
          child->isTupleDecl() ||
          child->isForwardingDecl()) {
        auto res = Resolver::createForScopeResolvingField(context, ad, child, result);

        if (child->isForwardingDecl()) {
          res.allowReceiverScopes = true;
        }

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

static optional<CandidateFailureReason>
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
    return faMap.reason();
  }

  // TODO: more to check for method-ness?
  // TODO: better reason failed in this case
  if (!ci.isOpCall() && ci.isMethodCall() != ufs->isMethod()) {
    return FAIL_CANDIDATE_OTHER;
  }

  return empty;
}

// given a typed function signature, determine if it applies to a call
ApplicabilityResult
isInitialTypedSignatureApplicable(Context* context,
                                  const TypedFnSignature* tfs,
                                  const FormalActualMap& faMap,
                                  const CallInfo& ci) {
  if (auto reasonFailed = isUntypedSignatureApplicable(context, tfs->untyped(), faMap, ci)) {
    return ApplicabilityResult::failure(tfs, *reasonFailed);
  }

  const uast::Function* fn = nullptr;
  if (!tfs->untyped()->id().isEmpty()) {
    auto ast = parsing::idToAst(context, tfs->untyped()->id());
    if (ast) fn = ast->toFunction();
  }
  auto canPassFn = allowPromotionForSig(tfs, fn) ? canPass : canPassScalar;

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

        got = canPassFn(context, actualType, getVarArgTupleElemType(formalType));
      } else {
        got = canPassFn(context, actualType, formalType);
      }
      if (got.passes()) {
        // fine, continue to next entry
      } else if (got.reason() == FAIL_DID_NOT_INSTANTIATE) {
        // A conversion is possible, but the formal type is generic and
        // the actual doesn't have enough info to instantiate. However,
        // this is only the "initial" check -- previous formals might contribute
        // information that makes the formal concrete. Allow this for now.
      } else {
        CHPL_ASSERT(!got.passes());
        return ApplicabilityResult::failure(tfs, got.reason(), entry.formalIdx());
      }
    }
  }

  if (!varArgType.isUnknown()) {
    const TupleType* tup = varArgType.type()->toTupleType();
    if (tup != nullptr && tup->isVarArgTuple() &&
        tup->isKnownSize() && numVarArgActuals != tup->numElements()) {
      return ApplicabilityResult::failure(tfs, FAIL_VARARG_MISMATCH);
    }
  }

  // check that the where clause applies
  auto whereResult = tfs->whereClauseResult();
  if (whereResult == TypedFnSignature::WHERE_FALSE) {
    return ApplicabilityResult::failure(tfs, FAIL_WHERE_CLAUSE);
  }

  return ApplicabilityResult::success(tfs);
}

// returns nullptr if the candidate is not applicable,
// or the result of typedSignatureInitial if it is.
static ApplicabilityResult
doIsCandidateApplicableInitial(ResolutionContext* rc,
                               const IdAndFlags& candidate,
                               const CallInfo& ci) {
  Context* context = rc->context();
  bool isParenlessFn = !candidate.isParenfulFunction();
  bool isField = candidate.isMethodOrField() && !candidate.isMethod();
  const ID& candidateId = candidate.id();

  // if it's a paren-less call, only consider parenless routines
  // (including generated field accessors) but not types/outer variables/
  // calls with parens.
  if (ci.isParenless()) {
    if (isParenlessFn || isField) {
      // OK
    } else {
      return ApplicabilityResult::failure(candidateId, FAIL_PARENLESS_MISMATCH);
    }
  }

  if (candidate.isType() && !ci.isParenless()) {
    // calling a type - i.e. type construction
    const Type* t = initialTypeForTypeDecl(context, candidateId);
    return ApplicabilityResult::success(typeConstructorInitial(context, t));
  }

  // quickly rule out formals as candidates for method calls
  // by checking that, if it's a method call, the candidate is a method or field
  if (ci.isMethodCall() && !candidate.isMethodOrField()) {
    return ApplicabilityResult::failure(candidateId, /* TODO */ FAIL_CANDIDATE_OTHER);
  }

  if (isField) {
    if (ci.isParenless() && ci.isMethodCall() && ci.numActuals() == 1) {
      // calling a field accessor
      //
      // TODO: This doesn't have anything to do with this candidate. Shouldn't
      // we be handling this somewhere else?
      auto qt = ci.actual(0).type();

      // the field might not be present on the current type T, but on the
      // element type of T for promotion. Keep trying to find the field by
      // looking at the element type of the current type if we fail.
      //
      // note: in `canPass` and elsewhere, we do not consider transitive promotion.
      // So, stop after a single call to getPromotionType. This code is still
      // written as a loop in case we want to change that in the future.
      // See other calls to getPromotionType for other places where we decide
      // on transitive promotion.
      bool inPromotion = false;
      while (!qt.isUnknownOrErroneous()) {
        auto containingType = isNameOfField(context, ci.name(), qt.type());
        if (!containingType) {
          // help handle the case where we're calling a field accessor on a manger.
          // while resolving the body of a method on owned to evaluate the applicability,
          // we need to be able to resolve the field accessor on the manager.
          if (auto classType = qt.type()->toClassType()) {
            if (auto managerType = classType->managerRecordType(context)) {
              containingType = isNameOfField(context, ci.name(), managerType);
            }
          }
        }

        if (containingType &&
            containingType->id() == candidateId.parentSymbolId(context)) {
          auto ret = fieldAccessor(context, containingType, ci.name());
          return ApplicabilityResult::success(ret);
        }

        if (inPromotion) break;

        qt = getPromotionType(context, qt);
        inPromotion = true;
      }
    }
    // not a candidate
    return ApplicabilityResult::failure(candidateId, /* TODO */ FAIL_CANDIDATE_OTHER);
  }

  if (!candidate.isParenfulFunction()) {
    AstTag tag = asttags::AST_TAG_UNKNOWN;
    if (!candidateId.isEmpty()) {
      tag = parsing::idToTag(context, candidateId);
    }
    if (!isFunction(tag)) {
      context->error(candidateId, "Found non-function where function was expected");
      return ApplicabilityResult::failure(candidateId, /* TODO */ FAIL_CANDIDATE_OTHER);
    }
  }

  if (ci.isMethodCall() && (ci.name() == "init" || ci.name() == "init=")) {
    // TODO: test when record has defaults for type/param fields
    auto recv = ci.methodReceiverType();
    auto fn = parsing::idToAst(context, candidateId)->toFunction();
    ResolutionResultByPostorderID r;
    auto vis = Resolver::createForInitialSignature(rc, fn, r);
    fn->thisFormal()->traverse(vis);
    auto res = vis.byPostorder.byAst(fn->thisFormal());

    auto got = canPassScalar(context, recv, res.type());
    // Allow passing directly or via implicit borrowing only.
    if (!got.passes() || (got.converts() && !got.convertsWithBorrowing())) {
      return ApplicabilityResult::failure(candidateId,
                                          /* TODO */ FAIL_CANDIDATE_OTHER);
    }
  }

  // For now, ignore any candidates that are not part of the 2.0 edition
  // Going forward, we'll need to teach Dyno's resolver to respect the
  // --edition flag.
  //
  // Developed by @benharsh
  if (!candidateId.isEmpty()) {
    if (auto ast = parsing::idToAst(context, candidateId);
        ast && ast->isFunction()) {
      auto fn = ast->toFunction();
      if (auto attr = fn->attributeGroup();
          attr && attr->hasEdition()) {
        auto first = attr->firstEdition();
        auto last = attr->lastEdition();
        auto two = UniqueString::get(context, "2.0");
        if ((!first.isEmpty() && first != two) ||
            (!last.isEmpty() && last != two)) {
          return ApplicabilityResult::failure(candidateId,
                                              /* TODO */ FAIL_CANDIDATE_OTHER);
        }
      }
    }
  }

  auto ret = typedSignatureInitialForId(rc, candidateId);
  auto ufs = ret ? ret->untyped() : nullptr;

  if (!ret) {
    // TODO: Can we reliably return something more specific here?
    if (parsing::idIsNestedFunction(context, candidateId) && rc->isEmpty()) {
      return ApplicabilityResult::failure(candidateId, /* TODO */ FAIL_CANDIDATE_OTHER);
    }

    return ApplicabilityResult::failure(candidateId, /* TODO */ FAIL_CANDIDATE_OTHER);
  }

  auto faMap = FormalActualMap(ufs, ci);
  return isInitialTypedSignatureApplicable(context, ret, faMap, ci);
}

// returns nullptr if the candidate is not applicable,
// or the result of an instantiated typedSignature if it is.
static ApplicabilityResult
doIsCandidateApplicableInstantiating(ResolutionContext* rc,
                                     const TypedFnSignature* typedSignature,
                                     const CallInfo& call,
                                     const PoiScope* poiScope) {
  auto instantiated = instantiateSignature(rc, typedSignature, call, poiScope);

  if (!instantiated.success())
    return instantiated;

  // check that the where clause applies
  if (instantiated.candidate()->whereClauseResult() == TypedFnSignature::WHERE_FALSE)
    return ApplicabilityResult::failure(typedSignature, FAIL_WHERE_CLAUSE);

  return instantiated;
}

static const EvaluatedCandidates
filterCandidatesInitialGatherRejectedImpl(ResolutionContext* rc,
                                          const MatchingIdsWithName& lst,
                                          const CallInfo& call,
                                          bool gatherRejected) {
  Context* context = rc->context();
  EvaluatedCandidates ret;

  for (auto cur = lst.begin(); cur != lst.end(); ++cur) {
    auto& idv = cur.curIdAndFlags();
    bool isNestedCandidate = parsing::idIsNestedFunction(context, idv.id());

    if (isNestedCandidate) ret.evaluatedAnyNestedFunction = true;
    auto s = doIsCandidateApplicableInitial(rc, idv, call);

    if (s.success()) {
      ret.matching.addCandidate(s.candidate());
    } else if (s.reason() == FAIL_FORMAL_ACTUAL_MISMATCH_ITERATOR_API) {
      ret.rejectedIteratorsMissingTag.push_back(s);
    } else if (gatherRejected) {
      ret.rejected.push_back(s);
    }
  }

  return ret;
}

static const EvaluatedCandidates&
filterCandidatesInitialGatherRejectedQuery(ResolutionContext* rc,
                                           MatchingIdsWithName lst,
                                           CallInfo call,
                                           bool gatherRejected) {
  // TODO: The predicate always loops over the list (best case 2 passes).
  CHPL_RESOLUTION_QUERY_BEGIN(filterCandidatesInitialGatherRejectedQuery, rc,
                              lst, call, gatherRejected);
  auto ret = filterCandidatesInitialGatherRejectedImpl(rc, lst, call,
                                                       gatherRejected);
  return CHPL_RESOLUTION_QUERY_END(ret);
}

const CandidatesAndForwardingInfo&
filterCandidatesInitial(ResolutionContext* rc,
                        MatchingIdsWithName lst,
                        CallInfo call) {
  constexpr auto query = filterCandidatesInitialGatherRejectedQuery;
  bool gatherRejected = false;
  auto& result = query(rc, std::move(lst), call, gatherRejected);
  return result.matching;
}

void
filterCandidatesInstantiating(ResolutionContext* rc,
                              const CandidatesAndForwardingInfo& lst,
                              const CallInfo& call,
                              const Scope* inScope,
                              const PoiScope* inPoiScope,
                              CandidatesAndForwardingInfo& result,
                              std::vector<ApplicabilityResult>* rejected) {
  Context* context = rc->context();

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
        doIsCandidateApplicableInstantiating(rc, typedSignature, call,
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
void accumulatePoisUsedByResolvingBody(ResolutionContext* rc,
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
  bool skipIfRunning = true;
  auto r = helpResolveFunction(rc, signature, poiScope, skipIfRunning);
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

    const Type* ret = nullptr;
    if (name == USTR("owned")) {
      ret = AnyOwnedType::get(context);
    } else if (name == USTR("shared")) {
      ret = AnySharedType::get(context);
    } else if (name == USTR("unmanaged")) {
      ret = ClassType::get(context, AnyClassType::get(context), nullptr, ClassTypeDecorator(ClassTypeDecorator::UNMANAGED));
    } else if (name == USTR("borrowed")) {
      ret = ClassType::get(context, AnyClassType::get(context), nullptr, ClassTypeDecorator(ClassTypeDecorator::BORROWED));
    } else {
      // case not handled in here
      return nullptr;
    }

    if (ret != nullptr && ci.numActuals() != 0) {
      context->error(astForErr, "invalid class type construction");
      return ErroneousType::get(context);
    } else {
      return ret;
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
      return ErroneousType::get(context);
    } else {
      return UnknownType::get(context);
    }
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

  bool namedCall =
      name == USTR("int") || name == USTR("uint") || name == USTR("bool") ||
      name == USTR("real") || name == USTR("imag") || name == USTR("complex");


  bool calledType = false;
  if (auto ct = ci.calledType().type()) {
    calledType = ct->isNumericOrBoolType();
  }

  if (namedCall || calledType) {
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

static const Type* getHeapBufferType(Context* context,
                               const AstNode* astForErr,
                               const CallInfo& ci) {
  UniqueString name = ci.name();

  auto called = ci.calledType();
  if (!(called.hasTypePtr() && called.type()->isHeapBufferType())) {
    return nullptr;
  }
  if (name != USTR("_ddata")) {
    return nullptr;
  }

  bool useGenericType = false;
  // There should be 0 or 1 actuals depending on if it is ?
  if (ci.hasQuestionArg()) {
    if (ci.numActuals() != 0) {
      context->error(astForErr, "invalid %s type construction", name.c_str());
      return ErroneousType::get(context);
    }
    useGenericType = true;
  } else {
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
    return HeapBufferType::get(context);
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
    context->error(astForErr, "invalid %s type construction", name.c_str());
    return ErroneousType::get(context);
  } else {
    return HeapBufferType::get(context, t);
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
      auto num = first.param()->toIntParam()->value();
      std::vector<const Type*> eltTypes(num, second.type());
      return TupleType::getValueTuple(context, eltTypes);
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

  if (auto t = getHeapBufferType(context, astForErr, ci)) {
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

static bool extractParamIndexingValueForTuple(const QualifiedType& qt,
                                              int64_t& outVal) {
  if (!qt.isParam() || !qt.hasParamPtr()) return false;

  if (qt.type()->isIntType()) {
    outVal = qt.param()->toIntParam()->value();
    return true;
  } else if (qt.type()->isUintType()) {
    uint64_t uval = qt.param()->toUintParam()->value();

    // if uint is too big, just set it to int max. Tuple can't be that
    // big anyway so we will report an error later.
    if (uval > std::numeric_limits<int64_t>::max()) {
      outVal = std::numeric_limits<int64_t>::max();
    } else {
      outVal = static_cast<int64_t>(uval);
    }
    return true;
  }
  return false;
}

// Resolving calls for certain compiler-supported patterns
// without requiring module implementations exist at all.
static bool resolveFnCallSpecial(Context* context,
                                 const AstNode* astForErr,
                                 const CallInfo& ci,
                                 QualifiedType& exprTypeOut) {
  // TODO: .borrow()
  // TODO: chpl__coerceCopy

  // Sometimes, actual types can be unknown since we are checking for 'out'
  // intent. No special functions here use the 'out' intent, so in this case,
  // return false.
  for (auto& actual : ci.actuals()) {
    if (actual.type().isUnknown()) {
      return false;
    }
  }

  // special casts including explicit param casts are resolved here
  if (ci.isOpCall() && ci.name() == USTR(":")) {
    auto srcQt = ci.actual(0).type();
    auto dstQt = ci.actual(1).type();
    auto srcTy = srcQt.type();
    auto dstTy = dstQt.type();

    if (!srcQt.isType() && srcTy == dstTy) {
      // TODO: insert copy for unnecessary casts involving records
      exprTypeOut = srcQt;
      return true;
    }

    auto targetParamGuess = Param::tryGuessParamTagFromType(dstQt.type());
    if (srcQt.isParam() && !targetParamGuess) {
      // We're casting a param value, but the destination type can't be
      // a param. This should be treated as a non-special cast.
      return false;
    }

    bool isDstType = dstQt.kind() == QualifiedType::TYPE;
    bool isParamTypeCast = srcQt.kind() == QualifiedType::PARAM && isDstType;

    if (isParamTypeCast) {
        auto srcQtEnumType = srcTy->toEnumType();
        auto dstQtEnumType = dstTy->toEnumType();

        if (srcQtEnumType && dstTy->isStringType()) {
          std::ostringstream oss;
          srcQt.param()->stringify(oss, chpl::StringifyKind::CHPL_SYNTAX);
          exprTypeOut = QualifiedType::makeParamString(context, oss.str());
          return true;
        } else if (dstQtEnumType && srcTy->isStringType()) {
          CHPL_ASSERT(srcQt.param()->isStringParam());

          auto enumName = srcQt.param()->toStringParam()->value();
          auto path = UniqueString();
          auto lastDot = findLastDot(enumName.c_str());

          if (lastDot != -1) {
            path = UniqueString::get(context, enumName.c_str(),lastDot);
            enumName = UniqueString::get(context, enumName.c_str() + lastDot + 1);
          }

          if (!dstQtEnumType->id().symbolPath().endsWith(path)) {
            context->error(astForErr,
                           "invalid qualified name for enum element '%s' in cast to %s",
                           enumName.c_str(), dstQtEnumType->id().symbolPath().c_str());
            exprTypeOut = QualifiedType(QualifiedType::UNKNOWN,
                                        ErroneousType::get(context));
          } else {
            exprTypeOut = typeForEnumElement(context, dstQtEnumType,
                                             enumName,
                                             astForErr);
          }
          return true;
        }

        if (srcQtEnumType && srcQtEnumType->isAbstract()) {
          exprTypeOut = CHPL_TYPE_ERROR(context, EnumAbstract, astForErr, "from", srcQtEnumType, dstTy);
          return true;
        } else if (dstQtEnumType && dstQtEnumType->isAbstract()) {
          exprTypeOut = CHPL_TYPE_ERROR(context, EnumAbstract, astForErr, "to", dstQtEnumType, srcTy);
          return true;
        } else if (srcQtEnumType && dstTy->toNothingType()) {
          auto fromName = tagToString(srcTy->tag());
          context->error(astForErr, "illegal cast from %s to nothing", fromName);
          exprTypeOut = QualifiedType(QualifiedType::UNKNOWN,
                                      ErroneousType::get(context));
          return true;
        }

        if (Param::castAllowed(context, srcQt, dstQt)) {
          exprTypeOut = Param::fold(context, astForErr,
                                    uast::PrimitiveTag::PRIM_CAST, srcQt, dstQt);
          return true;
        }

        // fall through, param cast isn't possible but maybe there's a runtime
        // cast that normal function resolution can handle.
    } else if (srcQt.isType() && dstQt.hasTypePtr() && dstTy->isStringType()) {
      // handle casting a type name to a string
      std::ostringstream oss;
      srcTy->stringify(oss, chpl::StringifyKind::CHPL_SYNTAX);
      exprTypeOut = QualifiedType::makeParamString(context, oss.str());
      return true;
    } else if (srcTy->isClassType() && dstTy->isClassType()) {
      // cast (borrowed class) : unmanaged,
      // and (unmanaged class) : borrowed
      auto srcClass = srcTy->toClassType();
      auto dstClass = dstTy->toClassType();
      bool isValidDst = dstClass->manageableType()->isAnyClassType() &&
                        (dstClass->decorator().isUnmanaged() ||
                         dstClass->decorator().isBorrowed());
      bool isValidSrc = srcClass->decorator().isBorrowed() ||
                        srcClass->decorator().isUnmanaged();
      if (isValidDst && isValidSrc) {
        auto management = ClassTypeDecorator::removeNilableFromDecorator(dstClass->decorator().val());
        auto decorator = ClassTypeDecorator(management);
        decorator = decorator.copyNilabilityFrom(srcClass->decorator());
        auto outTy = ClassType::get(context, srcClass->manageableType(),
                                    nullptr, decorator);
        exprTypeOut = QualifiedType(srcQt.kind(), outTy, srcQt.param());
        return true;
      }
    } else if (!srcQt.isParam() &&
               srcTy->isStringType() && isDstType &&
               dstTy->isStringType()) {
      // supported non-param casts to string
      exprTypeOut =
          QualifiedType(QualifiedType::VAR, RecordType::getStringType(context));
      return true;
    } else if (!isDstType) {
      // trying to cast to something that's not a type
      auto toName = tagToString(dstTy->tag());
      auto fromName = tagToString(srcTy->tag());
      context->error(astForErr, "illegal cast from %s to %s", fromName, toName);
      exprTypeOut = QualifiedType(QualifiedType::UNKNOWN,
                                  ErroneousType::get(context));
      return true;
    }
  }

  if ((ci.name() == USTR("==") || ci.name() == USTR("!="))) {
    if (ci.numActuals() == 2) {
      auto lhs = ci.actual(0).type();
      auto rhs = ci.actual(1).type();

      bool bothType = lhs.kind() == QualifiedType::TYPE &&
                      rhs.kind() == QualifiedType::TYPE;
      bool bothParam = lhs.kind() == QualifiedType::PARAM &&
                       rhs.kind() == QualifiedType::PARAM;
      if (bothType || bothParam) {
        bool result = lhs == rhs;
        result = ci.name() == USTR("==") ? result : !result;
        exprTypeOut = QualifiedType::makeParamBool(context, result);
        return true;
      }
    } else if (ci.numActuals() == 1 && ci.hasQuestionArg()) {
      // support type and param comparisons with '?'
      // TODO: will likely need adjustment once we are able to compare a
      // partially-instantiated type's fields with '?'
      auto arg = ci.actual(0).type();
      bool result = false;
      bool haveResult = true;
      if (arg.isType()) {
        result = arg.type()->isAnyType();
      } else if (arg.isParam()) {
        result = arg.param() == nullptr;
      } else {
        haveResult = false;
      }
      result = ci.name() == USTR("==") ? result : !result;
      if (haveResult) {
        exprTypeOut = QualifiedType::makeParamBool(context, result);
        return true;
      }
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

  if (ci.name() == USTR("borrow") && ci.numActuals() == 1 && ci.isMethodCall()) {
    // this is the equivalent of the production compiler's `resolveClassBorrowMethod`.
    // A call to `isClassLike` there rejects handling `owned` and `shared`,
    // so only handle undecorated class types here.

    auto receiver = ci.methodReceiverType();
    const ManageableType* receiverBct = nullptr;
    const ClassType* receiverCt = nullptr;
    bool handle =
      (receiverBct = receiver.type()->toBasicClassType()) ||
      ((receiverCt = receiver.type()->toClassType()) &&
       !receiverCt->decorator().isManaged());

    if (handle) {
      auto finalBct = receiverBct ? receiverBct : receiverCt->manageableType();

      auto decorator = ClassTypeDecorator(ClassTypeDecorator::BORROWED);
      if (receiverCt) {
        decorator = decorator.copyNilabilityFrom(receiverCt->decorator());
      }

      auto outTy = ClassType::get(context, finalBct, nullptr, decorator);
      exprTypeOut = QualifiedType(QualifiedType::VAR, outTy);
      return true;
    }
  }

  if (ci.name() == USTR("isCoercible")) {
    if (ci.numActuals() != 2) {
      if (!ci.isMethodCall()) {
        context->error(astForErr, "bad call to %s", ci.name().c_str());
        exprTypeOut = QualifiedType(QualifiedType::UNKNOWN,
                                    ErroneousType::get(context));
        return true;
      } else {
        return false;
      }
    }
    auto got = canPassScalar(context, ci.actual(0).type(), ci.actual(1).type());
    bool result = got.passes();
    exprTypeOut = QualifiedType::makeParamBool(context, result);
    return true;
  }

  if (ci.name() == USTR("this") && ci.numActuals() == 2) {
    // compiler-defined 'this' operator for param-indexed tuples
    auto thisType = ci.actual(0).type();
    auto second = ci.actual(1).type();
    int64_t val;
    if (thisType.hasTypePtr() && thisType.type()->isTupleType() &&
        extractParamIndexingValueForTuple(second, val)) {
      auto tup = thisType.type()->toTupleType();
      if (val < 0 || val >= tup->numElements()) {
        CHPL_REPORT(context, TupleIndexOOB, astForErr->toCall(), tup, val);
        exprTypeOut = QualifiedType(QualifiedType::UNKNOWN, ErroneousType::get(context));
      } else {
        auto member = tup->elementType(val);

        // adjust kind ensure that tupleType(idx) is also a type.
        if (thisType.isType()) {
          member = QualifiedType(thisType.kind(), member.type());
        }

        exprTypeOut = member;
      }
      return true;
    }
  }

  // In Dyno, we don't have a _ref wrapper type, so we can't distinguish
  // between 'ref' and non-'ref' type formals. Moreover, this function is
  // filled in by the compiler in production.
  if (!ci.isMethodCall() && (ci.name() == USTR("_build_tuple") ||
                             ci.name() == USTR("_build_tuple_noref") ||
                             ci.name() == USTR("_build_tuple_always_allow_ref"))) {
    bool removeRefs = ci.name() == USTR("_build_tuple_noref");
    auto intent = ci.name() == USTR("_build_tuple_always_allow_ref") ?
                  QualifiedType::CONST_VAR : QualifiedType::TYPE;
    std::vector<QualifiedType> components;
    for (size_t i = 0; i < ci.numActuals(); i++) {
      auto actual = ci.actual(i).type();
      if (removeRefs) {
        CHPL_ASSERT(actual.param() == nullptr);
        actual = QualifiedType(KindProperties::removeRef(actual.kind()), actual.type());
      }
      components.push_back(std::move(actual));
    }

    auto resultTuple = TupleType::getQualifiedTuple(context, components);
    exprTypeOut = QualifiedType(intent, resultTuple);
    return true;
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

  ResolutionContext rcval(context);
  auto rc = &rcval;

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
  if (ci.name() == "atomic") {
    auto newName = UniqueString::get(context, "chpl__atomicType");
    auto ctorCall = CallInfo::copyAndRename(ci, newName);
    result = resolveCall(rc, call, ctorCall, inScopes);
    return true;
  }

  return false;
}

static bool resolveMethodCallSpecial(Context* context,
                                     const AstNode* astContext,
                                     const CallInfo& ci,
                                     const CallScopeInfo& inScopes,
                                     CallResolutionResult& result) {
  if (!ci.isMethodCall()) {
    return false;
  }

  ResolutionContext rcval(context);
  auto rc = &rcval;

  // Methods that require resolving some kind of helper method to build
  // the type.

  if (ci.name() == USTR("domain") && ci.isParenless()) {
    auto newName = UniqueString::get(context, "_dom");
    auto ctorCall = CallInfo::copyAndRename(ci, newName);
    result = resolveGeneratedCall(rc, astContext, ctorCall, inScopes);
    return true;
  }

  if (ci.name() == USTR("bytes") && !ci.isParenless()) {
    auto newName = UniqueString::get(context, "chpl_bytes");
    auto ctorCall = CallInfo::copyAndRename(ci, newName);
    result = resolveGeneratedCall(rc, astContext, ctorCall, inScopes);
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
  ResolutionContext rcval(context);
  auto rc = &rcval;

  CHPL_ASSERT(ci.calledType().type() != nullptr);
  CHPL_ASSERT(!ci.calledType().type()->isUnknownType());

  auto initial = typeConstructorInitial(context, ci.calledType().type());
  initialCandidates.addCandidate(initial);

  // TODO: do something for partial instantiation

  filterCandidatesInstantiating(rc, initialCandidates, ci, inScope,
                                inPoiScope,
                                candidates,
                                /* rejected */ nullptr);


  // find most specific candidates / disambiguate
  // Note: at present there can only be one candidate here
  MostSpecificCandidates mostSpecific =
      findMostSpecificCandidates(rc, candidates, ci, inScope, inPoiScope);

  return mostSpecific;
}

static const TypedFnSignature*
considerCompilerGeneratedMethods(ResolutionContext* rc,
                                 const CallInfo& ci,
                                 CandidatesAndForwardingInfo& candidates) {
  // only consider compiler-generated methods and opcalls, for now
  if (!ci.isMethodCall()) return nullptr;

  CHPL_ASSERT(ci.numActuals() >= 1);
  auto receiverType = ci.methodReceiverType();

  // if we don't need the method, nothing to do.
  if (!needCompilerGeneratedMethod(rc->context(), receiverType.type(), ci.name(),
                                   ci.isParenless())) {
    return nullptr;
  }

  return getCompilerGeneratedMethod(rc, receiverType, ci.name(),
                                    ci.isParenless());
}

static const TypedFnSignature*
considerCompilerGeneratedFunctions(ResolutionContext* rc,
                                   const CallInfo& ci,
                                   CandidatesAndForwardingInfo& candidates) {
  // methods and op calls considered elsewhere
  if (ci.isMethodCall() || ci.isOpCall()) return nullptr;

  return getCompilerGeneratedFunction(rc, ci);
}

// not all compiler-generated procs are method. For instance, the compiler
// generates to-and-from integral casts for enums. In the to-casts, the
// receiver (or lhs) is an integral, not an enum.
//
// This helper serves to consider compiler-generated functions that can't
// be guessed based on the first argument.
static std::vector<const TypedFnSignature*> const&
considerCompilerGeneratedOperators(ResolutionContext* rc,
                                   const CallInfo& ci,
                                   CandidatesAndForwardingInfo& candidates) {
  static const std::vector<const TypedFnSignature*> empty;
  if (!ci.isOpCall() || ci.numActuals() != 2) return empty;

  CHPL_ASSERT(ci.numActuals() >= 1);
  auto& lhs = ci.actual(0).type();
  auto& rhs = ci.actual(1).type();

  // if we don't need the operator, nothing to do.
  if (!needCompilerGeneratedBinaryOp(rc->context(), lhs, rhs, ci.name())) {
    return empty;
  }

  return getCompilerGeneratedBinaryOp(rc, lhs, rhs, ci.name());
}

static void
considerCompilerGeneratedCandidates(ResolutionContext* rc,
                                    const AstNode* astForErr,
                                    const CallInfo& ci,
                                    CandidatesAndForwardingInfo& candidates,
                                    std::vector<ApplicabilityResult>* rejected) {
  std::vector<const TypedFnSignature*> tfss;

  for (auto tfs : considerCompilerGeneratedOperators(rc, ci, candidates)){
    tfss.push_back(tfs);
  }
  if (tfss.empty()) {
    if (auto tfs = considerCompilerGeneratedMethods(rc, ci, candidates))
      tfss.push_back(tfs);
  }
  if (tfss.empty()) {
    if (auto tfs = considerCompilerGeneratedFunctions(rc, ci, candidates))
      tfss.push_back(tfs);
  }

  if (tfss.empty()) return;

  for (auto tfs : tfss) {
    // check if the initial signature matches
    auto faMap = FormalActualMap(tfs->untyped(), ci);
    if (!isInitialTypedSignatureApplicable(rc->context(), tfs, faMap, ci).success()) {
      return;
    }

    // OK, already concrete, store and return
    if (!tfs->needsInstantiation()) {
      candidates.addCandidate(tfs);
      return;
    }

    // need to instantiate before storing
    auto instantiated = doIsCandidateApplicableInstantiating(rc,
                                                             tfs,
                                                             ci,
                                                             /* POI */ nullptr);
    if (!instantiated.success()) {
      // failed when instantiating, likely due to dependent types.
      if (rejected) rejected->push_back(instantiated);
      return;
    }

    if (!instantiated.candidate()->isInitializer() &&
        checkUninstantiatedFormals(rc->context(), astForErr, instantiated.candidate())) {
      return; // do not push invalid candidate into list
    }

    candidates.addCandidate(instantiated.candidate());
  }
}

static const types::QualifiedType& getPromotionTypeQuery(Context* context, types::QualifiedType qt);

static bool shouldSkipScalarTypes(Context* context,
                                  QualifiedType receiverType) {
  if (receiverType.isUnknownOrErroneous()) return true;
  if (receiverType.isParam() || receiverType.isType()) return false;

  // adjust the intent since we do that prior to invoking getPromotionTypeQuery
  receiverType = QualifiedType(QualifiedType::CONST_VAR, receiverType.type());

  // if we're currently computing the promotion (scalar) type, we can't use said
  // promotion type for computing receivers.
  return context->isQueryRunning(getPromotionTypeQuery, std::make_tuple(receiverType));
}

static MatchingIdsWithName lookupCalledExprImpl(Context* context,
                                                const Scope* scope,
                                                UniqueString name,
                                                const QualifiedType& receiverType,
                                                LookupConfig config,
                                                CheckedScopes& visited) {
  ReceiverScopeTypedHelper typedHelper;

  bool checkScalarTypes = !shouldSkipScalarTypes(context, receiverType);
  auto lookupHelper = typedHelper.methodLookupForType(context, receiverType, checkScalarTypes);
  auto ret = lookupNameInScopeWithSet(context, scope,
                                      lookupHelper,
                                      /* receiverScopeHelper */ nullptr,
                                      name, config, visited);

  // At this point, we don't allow "switching gears" to 'proc this'-based
  // resolution of non-function names. The calling code is expected to
  // have set up the CallInfo with 'name = "this"' if that's what it wanted.
  // So, rule out non-functional IDs.
  //
  // This relies on the fact that under LOOKUP_STOP_NON_FN, the non-functional
  // IDs are last. (except in the case of ambiguity, but that should've been
  // ruled out by calling code as well).
  if (config & LOOKUP_STOP_NON_FN) {
    for (int i = 0; i < ret.numIds(); i++) {
      auto& idv = ret.idAndFlags(i);
      if (!idv.isFunctionLike() && !idv.isType()) ret.truncate(i);
    }
  }

  return ret;
}

static MatchingIdsWithName const& cachedCandidates(Context* context,
                                                   const Scope* scope,
                                                   UniqueString name,
                                                   QualifiedType receiverType,
                                                   LookupConfig config) {
  QUERY_BEGIN(cachedCandidates, context, scope, name, receiverType, config);

  CheckedScopes visited;
  auto ret =
    lookupCalledExprImpl(context, scope, name, receiverType, config, visited);

  return QUERY_END(ret);
}

static MatchingIdsWithName
lookupCalledExpr(Context* context,
                 const Scope* scope,
                 const CallInfo& ci,
                 CheckedScopes& visited) {

  auto receiverType = QualifiedType();

  // For method calls, also consider the receiver scope.
  if (ci.isMethodCall() || ci.isOpCall()) {
    // TODO: should types of all arguments be considered for an op call?
    CHPL_ASSERT(ci.numActuals() >= 1);
    receiverType = ci.actual(0).type();
  }

  LookupConfig config = LOOKUP_DECLS | LOOKUP_IMPORT_AND_USE | LOOKUP_PARENTS |
                        LOOKUP_EXTERN_BLOCKS;

  // For parenless non-method calls, only find the innermost match
  if (ci.isParenless() && !ci.isMethodCall()) {
    config |= LOOKUP_INNERMOST;
  } else {
    config |= LOOKUP_STOP_NON_FN;
  }

  if (ci.isMethodCall()) {
    config |= LOOKUP_ONLY_METHODS_FIELDS;
  }

  if (ci.isOpCall()) {
    config |= LOOKUP_METHODS;
  }

  UniqueString name = ci.name();

  // If this is a cacheable lookup (i.e., one that doesn't depend on
  // prior state), use cachedCandidates, which is the same as invoking
  // lookupCalledExprImpl except that it doesn't populate visited
  // and caches the result. This is intended to optimize cases where
  // the same function is re-resolved with different types, but expressions
  // in its body always refer to the same thing.
  bool tryCache = visited.size() == 0;
  if (tryCache) {
    return cachedCandidates(context, scope, name, receiverType, config);
  }

  return lookupCalledExprImpl(context, scope, name, receiverType, config,
                           visited);
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

static void filterGatheredCandidates(ResolutionContext* rc,
                                     MatchingIdsWithName& v,
                                     const CallInfo& ci,
                                     const CallScopeInfo& inScopes,
                                     std::vector<ApplicabilityResult>* rejected,
                                     CandidatesAndForwardingInfo& into,
                                     CandidatesAndForwardingInfo& lastResort) {
  // filter without instantiating yet
  const auto& initialCandidates =
    filterCandidatesInitial(rc, std::move(v), ci);

  // find candidates, doing instantiation if necessary
  CandidatesAndForwardingInfo candidatesWithInstantiations;
  filterCandidatesInstantiating(rc,
      initialCandidates,
      ci,
      inScopes.callScope(),
      inScopes.poiScope(),
      candidatesWithInstantiations,
      rejected);

  // filter out last resort candidates
  filterCandidatesLastResort(rc->context(), candidatesWithInstantiations,
      into, lastResort);
}

// If the receiver type is promoted, then we might be trying to call
// a method on its scalar type, and promote that call. Search for fields in
// scalar types to enable this pattern.
static void
gatherAndFilterScalarFields(ResolutionContext* rc,
                            const CallInfo& ci,
                            const CallScopeInfo& inScopes,
                            CheckedScopes& visited,
                            CandidatesAndForwardingInfo& candidates) {
  if (!ci.isMethodCall() || !ci.isParenless()) return;
  if (ci.name() == "chpl__promotionType") return;
  CHPL_ASSERT(ci.numActuals() >= 1);

  ReceiverScopeTypedHelper typedHelper;
  auto receiverType = ci.actual(0).type();

  // Limited version of config from lookupCalledExpr
  LookupConfig config = LOOKUP_DECLS | LOOKUP_ONLY_METHODS_FIELDS | LOOKUP_STOP_NON_FN;

  MatchingIdsWithName toReturn;

  while (true) {
    receiverType = getPromotionType(rc->context(), receiverType, /* skipIfRunning */ true);
    if (receiverType.isUnknownOrErroneous()) break;

    auto lookupHelper = typedHelper.methodLookupForType(rc->context(), receiverType);
    auto ret = lookupNameInScopeWithSet(rc->context(),
                                        /* scope */nullptr,
                                        lookupHelper,
                                        /* receiverScopeHelper */ nullptr,
                                        ci.name(), config, visited);

    for (int i = 0; i < ret.numIds(); i++) {
      auto& idv = ret.idAndFlags(i);
      if (idv.isMethodOrField() && !idv.isMethod()) toReturn.append(idv);
    }

    // note: in `canPass` and elsewhere, we do not consider transitive promotion.
    // So, stop after a single call to getPromotionType. This code is still
    // written as a loop in case we want to change that in the future.
    break;
  }

  CandidatesAndForwardingInfo lastResort;
  filterGatheredCandidates(rc, toReturn, ci, inScopes,
                           /* rejected */ nullptr, candidates, lastResort);
}

// this function gathers candidates not from POI and candidates
// from POI into separate vectors.
// For each of these vectors, the corresponding forwardingTo vector
// will have an element for each of the returned candidates &
// indicates the actual type that is passed as the method receiver
// when using forwarding.
static void
gatherAndFilterCandidatesForwarding(ResolutionContext* rc,
                                    const AstNode* astContext,
                                    const Call* call,
                                    const CallInfo& ci,
                                    const CallScopeInfo& inScopes,
                                    CandidatesAndForwardingInfo& nonPoiCandidates,
                                    CandidatesAndForwardingInfo& poiCandidates,
                                    LastResortCandidateGroups& lrcGroups,
                                    std::vector<ApplicabilityResult>* rejected) {
  Context* context = rc->context();
  const Type* receiverType = ci.actual(0).type().type();

  // Resolve the forwarding expression's types & decide if we
  // want to consider forwarding.
  ResolvedFields forwards;
  UniqueString name = ci.name();
  if (name == USTR("init") || name == USTR("init=") || name == USTR("deinit") ||
      name == USTR("postfix!")) {
    // these are exempt from forwarding
  } else if (auto ct = receiverType->getCompositeType()) {
    auto useDefaults = DefaultsPolicy::USE_DEFAULTS;
    const ResolvedFields& fields = fieldsForTypeDecl(rc, ct,
                                                     useDefaults);

    if (CHPL_RESOLUTION_IS_GLOBAL_QUERY_RUNNING(resolveForwardingExprs, rc, ct)) {
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
      const ResolvedFields& exprs = resolveForwardingExprs(rc, ct);
      if (fields.numForwards() > 0 ||
          exprs.numForwards() > 0) {
        // and check for cycles
        bool cycleFound = emitErrorForForwardingCycles(rc, ct);
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
                                       QualifiedType(),
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
      considerCompilerGeneratedCandidates(rc, astContext, fci,
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

        filterGatheredCandidates(rc, v, fci, inScopes, rejected,
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

        filterGatheredCandidates(rc, v, fci, inScopes, rejected,
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
            gatherAndFilterCandidatesForwarding(rc, astContext, call, fci,
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

static void doGatherCandidates(ResolutionContext* rc,
                               CandidatesAndForwardingInfo& outCandidates,
                               LastResortCandidateGroups& outLrcGroups,
                               CheckedScopes& outVisited,
                               size_t& outFirstPoiCandidateIdx,
                               bool& outRejectedPossibleIteratorCandidates,
                               const Call* call,
                               const CallInfo& ci,
                               const CallScopeInfo& inScopes,
                               std::vector<ApplicabilityResult>* rejected,
                               const PoiScope* usePoiScope) {
  Context* context = rc->context();
  auto lookupScope = usePoiScope ? usePoiScope->inScope()
                                 : inScopes.lookupScope();

  // First, lookup possible candidates in the relevant scope.
  auto v = lookupCalledExpr(context, lookupScope, ci, outVisited);
  bool gatherRejections = (rejected != nullptr);

  // Next, filter candidates against their initial type signatures.
  constexpr auto filter = filterCandidatesInitialGatherRejectedQuery;
  auto& initial = filter(rc, v, ci, gatherRejections);
  const auto& initialCandidates = initial.matching;
  const auto& initialRejections = initial.rejected;
  outRejectedPossibleIteratorCandidates =
    !initial.rejectedIteratorsMissingTag.empty();

  if (rejected != nullptr) {
    rejected->insert(rejected->end(),
                     initialRejections.begin(),
                     initialRejections.end());
  }

  // Finally, filter with instantiation as necessary.
  CandidatesAndForwardingInfo candidatesWithInstantiations;
  filterCandidatesInstantiating(rc,
                                initialCandidates,
                                ci,
                                inScopes.callScope(),
                                inScopes.poiScope(),
                                candidatesWithInstantiations,
                                rejected);

  // filter out last resort candidates
  CandidatesAndForwardingInfo lrcGroup;
  filterCandidatesLastResort(context, candidatesWithInstantiations,
                             outCandidates, lrcGroup);
  if (usePoiScope) {
    outLrcGroups.addPoiCandidates(std::move(lrcGroup));
  } else {
    outLrcGroups.addNonPoiCandidates(std::move(lrcGroup));
  }
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
gatherAndFilterCandidates(ResolutionContext* rc,
                          const AstNode* astContext,
                          const Call* call,
                          const CallInfo& ci,
                          const CallScopeInfo& inScopes,
                          size_t& firstPoiCandidate,
                          bool& outRejectedPossibleIteratorCandidates,
                          std::vector<ApplicabilityResult>* rejected,
                          bool skipForwarding = false) {
  Context* context = rc->context();
  CandidatesAndForwardingInfo candidates;
  LastResortCandidateGroups lrcGroups;
  CheckedScopes visited;
  firstPoiCandidate = 0;

  // inject compiler-generated candidates in a manner similar to below
  // (note that any added candidates are already fully instantiated &
  //  the poiInfo from these is not gathered, because such methods should
  //  always be available in any scope that can refer to the type & are
  //  considered part of the custom type)
  considerCompilerGeneratedCandidates(rc, astContext, ci,
                                      candidates,
                                      rejected);

  // don't worry about last resort for compiler generated candidates

  // look for candidates without using POI.
  doGatherCandidates(rc, candidates, lrcGroups, visited,
                     firstPoiCandidate, outRejectedPossibleIteratorCandidates,
                     call, ci, inScopes, rejected, nullptr);

  // next, look for candidates using POIs
  firstPoiCandidate = candidates.size();
  for (const PoiScope* curPoi = inScopes.poiScope();
       curPoi != nullptr;
       curPoi = curPoi->inFnPoi()) {

    // stop if any candidate has been found.
    if (candidates.empty() == false) {
      break;
    }

    doGatherCandidates(rc, candidates, lrcGroups, visited,
                       firstPoiCandidate, outRejectedPossibleIteratorCandidates,
                       call, ci, inScopes, rejected, curPoi);
  }

  // If no candidates were found and it's a method, try forwarding
  if (candidates.empty() && ci.isMethodCall() && ci.numActuals() >= 1 &&
      !skipForwarding) {
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

    // don't look for chpl__promotionType using forwarding,
    // This prevents an infinite dependency of:
    //
    // get scopes
    //   -> get scopes for scalar types
    //   -> resolve chpl__promotionType to get scalar types
    //   -> resolve fields for the purposes of forwarding
    //   -> resolve method calls in fields
    //   -> get scopes
    //
    // This dependency is technically broken anyway by uses of isQueryRunning,
    // but I'm keeping this workaround since isQueryRunning technically violates
    // the "no side effects" rule of queries, so the fewer queries end up
    // invoking it, the better. -D.F.
    bool skipForwarding = (ci.name() == "chpl__promotionType");

    if (receiverType && typeUsesForwarding(context, receiverType) &&
        !skipForwarding &&
        !isInsideForwarding(context, call)) {
      CandidatesAndForwardingInfo nonPoiCandidates;
      CandidatesAndForwardingInfo poiCandidates;

      gatherAndFilterCandidatesForwarding(
          rc, astContext, call, ci, inScopes, nonPoiCandidates,
          poiCandidates, lrcGroups.getForwardingGroups(),
          rejected);

      // append candidates from forwarding
      candidates.takeFromOther(nonPoiCandidates);
      candidates.takeFromOther(poiCandidates);
    }

    if (candidates.empty()) {
      // Though it's not "true" forwarding, now is the time to look for references
      // to fields of scalar types. (e.g., we're resolving A.x where A is an array
      // of records that have field x).
      CandidatesAndForwardingInfo fieldCandidates;
      gatherAndFilterScalarFields(rc, ci, inScopes, visited, fieldCandidates);
      candidates.takeFromOther(fieldCandidates);
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
findMostSpecificAndCheck(ResolutionContext* rc,
                         const CandidatesAndForwardingInfo& candidates,
                         size_t firstPoiCandidate,
                         const AstNode* astContext,
                         const Call* call,
                         const CallInfo& ci,
                         const Scope* inScope,
                         const PoiScope* inPoiScope,
                         PoiInfo& poiInfo) {

  // When checking visibility distance, we need to choose either the callScope
  // or the poiScope. Currently the poiScope is favored if both are available.
  //
  // If the poiScope is passed, but was not used to find candidates, then
  // visibility distance could be wrong if candidates are equally visible from
  // the poiScope, but not equally visible from the callScope.
  //
  // If there were no POI candidates found, then the poiScope should not be
  // used. See 'testDistance' in 'testDisambiguation'.
  auto poiScopeToUse = firstPoiCandidate >= candidates.size() ? nullptr :
                       inPoiScope;

  // find most specific candidates / disambiguate
  MostSpecificCandidates mostSpecific =
      findMostSpecificCandidates(rc, candidates, ci, inScope, poiScopeToUse);

  // perform fn signature checking for any instantiated candidates that are used
  for (const MostSpecificCandidate& candidate : mostSpecific) {
    if (!candidate) continue;

    if (candidate.fn()->instantiatedFrom()) {
      checkSignature(rc->context(), candidate.fn());
    }

    // Initializers haven't yet been checked for uninstantiated formals,
    // because prior to getting a MostSpecificCandidate we didn't resolve
    // their bodies. Now we have, so we have a "final" TFS to check.
    //
    // Note: this check "normally" only fires for compiler-generated cases,
    // so match that here. See other calls to checkUninstantiatedFormal.
    if (candidate.fn()->isInitializer() && candidate.fn()->isCompilerGenerated()) {
      checkUninstantiatedFormals(rc->context(), astContext, candidate.fn());
    }
  }

  // note any most-specific candidates from POI in poiInfo.
  auto id =
      call != nullptr ? call->id() :
      astContext != nullptr ? astContext->id() :
      ID();
  size_t n = candidates.size();
  for (size_t i = firstPoiCandidate; i < n; i++) {
    for (const MostSpecificCandidate& candidate : mostSpecific) {
      if (candidate.fn() == candidates.get(i)) {
        poiInfo.addIds(id, candidate.fn()->id());
      }
    }
  }

  return mostSpecific;
}


static MostSpecificCandidates
resolveFnCallFilterAndFindMostSpecific(ResolutionContext* rc,
                                       const AstNode* astContext,
                                       const Call* call,
                                       const CallInfo& ci,
                                       const CallScopeInfo& inScopes,
                                       PoiInfo& poiInfo,
                                       bool& outRejectedPossibleIteratorCandidates,
                                       std::vector<ApplicabilityResult>* rejected,
                                       bool skipForwarding = false) {
  // search for candidates at each POI until we have found candidate(s)
  size_t firstPoiCandidate = 0;
  auto candidates = gatherAndFilterCandidates(rc, astContext, call, ci,
                                              inScopes,
                                              firstPoiCandidate,
                                              outRejectedPossibleIteratorCandidates,
                                              rejected,
                                              skipForwarding);
  // * find most specific candidates / disambiguate
  // * check signatures
  // * gather POI info
  auto mostSpecific =
    findMostSpecificAndCheck(rc, candidates, firstPoiCandidate,
                             astContext, call, ci, inScopes.callScope(),
                             inScopes.poiScope(), poiInfo);

  return mostSpecific;
}

static const PoiScope*
instantiationPoiScopeForMostSpecificCandidates(Context* context,
                                               MostSpecificCandidates& mostSpecific,
                                               const CallScopeInfo& inScopes) {
  const PoiScope* instantiationPoiScope = nullptr;
  for (const MostSpecificCandidate& candidate : mostSpecific) {
    if (candidate && candidate.fn()) {
      instantiationPoiScope =
          Resolver::poiScopeOrNull(context, candidate.fn(),
                                   inScopes.callScope(),
                                   inScopes.poiScope());
      if (instantiationPoiScope) break;
    }
    if (instantiationPoiScope) break;
  }
  return instantiationPoiScope;
}

static void
accumulatePoiInfoForMostSpecificCandidates(ResolutionContext* rc,
                                           MostSpecificCandidates& mostSpecific,
                                           PoiInfo& poiInfo,
                                           const PoiScope* instantiationPoiScope) {
  if (instantiationPoiScope) {
    poiInfo.setPoiScope(instantiationPoiScope);
    for (const MostSpecificCandidate& candidate : mostSpecific) {
      if (candidate) {
        if (candidate.fn()->untyped()->idIsFunction()) {
          // note: following call returns early if candidate not instantiated
          accumulatePoisUsedByResolvingBody(rc, candidate.fn(),
                                            instantiationPoiScope,
                                            poiInfo);
        }
      }
    }
  }
}

static CallResolutionResult
resolutionResultFromMostSpecificCandidate(ResolutionContext* rc,
                                          const MostSpecificCandidate& msc,
                                          const CallScopeInfo& inScopes) {
  auto mscs = MostSpecificCandidates::getOnly(msc);

  PoiInfo poiInfo;
  auto instantiationPoiScope =
    instantiationPoiScopeForMostSpecificCandidates(rc->context(), mscs, inScopes);
  accumulatePoiInfoForMostSpecificCandidates(rc, mscs, poiInfo,
      instantiationPoiScope);
  QualifiedType exprType;
  QualifiedType yieldedType;
  if (msc.fn()) {
    auto& yieldAndRet = returnTypes(rc, msc.fn(), instantiationPoiScope);
    exprType = yieldAndRet.second;

    if (!msc.promotedFormals().empty()) {
      yieldedType = exprType;
      exprType = QualifiedType(
          exprType.kind(),
          PromotionIteratorType::get(rc->context(),
                                     instantiationPoiScope,
                                     msc.fn(),
                                     exprType,
                                     msc.promotedFormals()));
    }

    if (msc.fn()->isIterator() && yieldedType.isUnknown()) {
      yieldedType = yieldAndRet.first;
    }
  }

  bool rejectedPossibleIteratorCandidates = false;
  return CallResolutionResult(mscs, rejectedPossibleIteratorCandidates,
                              exprType, yieldedType, poiInfo);
}

static bool handleReflectionFunction(ResolutionContext* rc,
                                     const TypedFnSignature* sig,
                                     const Call* call,
                                     QualifiedType& result) {
  if (sig->isCompilerGenerated()) return false;

  auto context = rc->context();
  auto fn = parsing::idToAst(context, sig->id())->toFunction();
  if (fn && fn->attributeGroup()) {
    auto attr = fn->attributeGroup();
    if (attr->hasPragma(pragmatags::PRAGMA_GET_LINE_NUMBER)) {
      auto loc = parsing::locateId(context, call->id());
      result = QualifiedType::makeParamInt(context, loc.firstLine());
      return true;
    } else if (attr->hasPragma(pragmatags::PRAGMA_GET_FILE_NAME)) {
      auto loc = parsing::locateId(context, call->id());
      result = QualifiedType::makeParamString(context, loc.path());
      return true;
    } else if (attr->hasPragma(pragmatags::PRAGMA_GET_FUNCTION_NAME)) {
      auto func = parsing::idToParentFunctionId(context, call->id());
      if (!func.isEmpty()) {
        auto name = func.symbolName(context);
        result = QualifiedType::makeParamString(context, name);
      } else {
        // For compatibility with production
        // TODO: What *should* happen here?
        auto mod = parsing::idToParentModule(context, call->id());
        auto name = mod.symbolName(context);
        auto initFn = UniqueString::get(context, "chpl__init_" + name.str());
        result = QualifiedType::makeParamString(context, initFn);
      }
      return true;
    } else if (attr->hasPragma(pragmatags::PRAGMA_GET_MODULE_NAME)) {
      auto mod = parsing::idToParentModule(context, call->id());
      auto name = mod.symbolName(context);
      result = QualifiedType::makeParamString(context, name);
      return true;
    }
  }

  return false;
}

// handle field access constness and pragmas like REF_TO_CONST_WHEN_CONST_THIS
static void adjustConstnessBasedOnReceiver(Context* context,
                                           const MostSpecificCandidate& candidate,
                                           const CallInfo& ci,
                                           QualifiedType& rt) {
  bool adjustConst = false;
  if (candidate.fn()->untyped()->idIsFunction() && !candidate.fn()->isCompilerGenerated()) {
    auto fnAst = parsing::idToAst(context, candidate.fn()->id());
    if (auto ag = fnAst->attributeGroup()) {
      if (ag->hasPragma(pragmatags::PRAGMA_REF_TO_CONST_WHEN_CONST_THIS)) {
        adjustConst = true;
      }
    }
  } else if (candidate.fn()->untyped()->idIsField()) {
    // the return type computation for fields already takes care of making
    // it ref where necessary, so just adjust constness for refs.
    //
    // Don't do this for classes, though: a 'const shared C' can still
    // have its fields accessed mutably.

    if (ci.methodReceiverType().type() &&
        ci.methodReceiverType().type()->isClassType()) {
      // don't adjust
    } else {
      adjustConst = rt.isRef();
    }
  }

  if (adjustConst) {
    auto kp = KindProperties::fromKind(rt.kind());
    kp.setConst(ci.methodReceiverType().isConst());
    rt = QualifiedType(kp.toKind(), rt.type());
  }
}

// call can be nullptr. in that event ci.name() will be used to find
// what is called.
static CallResolutionResult
resolveFnCall(ResolutionContext* rc,
              const AstNode* astContext,
              const Call* call,
              const CallInfo& ci,
              const CallScopeInfo& inScopes,
              std::vector<ApplicabilityResult>* rejected,
              bool skipForwarding = false) {
  Context* context = rc->context();
  PoiInfo poiInfo;
  MostSpecificCandidates mostSpecific;
  bool rejectedPossibleIteratorCandidates = false;

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
    mostSpecific =
      resolveFnCallFilterAndFindMostSpecific(rc, astContext,
                                             call, ci,
                                             inScopes,
                                             poiInfo,
                                             rejectedPossibleIteratorCandidates,
                                             rejected,
                                             skipForwarding);
  }

  // fully resolve each candidate function and gather poiScopesUsed.

  // figure out the poiScope to use
  const PoiScope* instantiationPoiScope =
    instantiationPoiScopeForMostSpecificCandidates(context, mostSpecific, inScopes);

  accumulatePoiInfoForMostSpecificCandidates(rc, mostSpecific,
                                             poiInfo, instantiationPoiScope);


  // infer types of generic 'out' formals from function bodies
  mostSpecific.inferOutFormals(rc, instantiationPoiScope);

  // compute the return types
  optional<QualifiedType> retType;
  optional<QualifiedType> yieldedType;
  for (const MostSpecificCandidate& candidate : mostSpecific) {
    if (candidate.fn() == nullptr) continue;

    QualifiedType rt;
    if (handleReflectionFunction(rc, candidate.fn(), call, rt)) {
      CHPL_ASSERT(rt.isParam() && rt.hasParamPtr());
      CHPL_ASSERT(mostSpecific.numBest() == 1);
      CHPL_ASSERT(candidate.fn()->numFormals() == 0);
      retType = rt;
      break;
    }

    bool isIterator = candidate.fn()->isIterator();
    auto& yieldAndRet =
      returnTypes(rc, candidate.fn(), instantiationPoiScope);
    rt = yieldAndRet.second;


    if (ci.isMethodCall()) {
      adjustConstnessBasedOnReceiver(context, candidate, ci, rt);
    }

    QualifiedType yt;

    if (!candidate.promotedFormals().empty()) {
      // this is actually a promotion; construct a promotion type instead.
      yt = rt;
      rt = QualifiedType(rt.kind(), PromotionIteratorType::get(context, instantiationPoiScope, candidate.fn(), rt, candidate.promotedFormals()));
    }

    if (retType && retType->type() != rt.type()) {
      // The actual iterator type for each overload will be different
      // since it includes the TypedFnSignature, and different overloads
      // are different TypedFnSignatures. Don't error, though.
      //
      // TODO: how do iterators + iterator groups work with return intent
      // overloading?
      if (!isIterator) {
        context->error(candidate.fn(),
                       nullptr,
                       "return intent overload type does not match");
      }
    } else if (!retType) {
      retType = rt;
    }

    if (isIterator || !yt.isUnknown()) {
      if (yt.isUnknown()) {
        yt = yieldAndRet.first;
      }

      if (yieldedType && yieldedType->type() != yt.type()) {
        context->error(candidate.fn(),
                       nullptr,
                       "return intent overload type does not match");
      } else if (!yieldedType) {
        yieldedType = yt;
      }
    }
  }

  return CallResolutionResult(mostSpecific,
                              rejectedPossibleIteratorCandidates,
                              ((bool) retType) ? *retType : QualifiedType(),
                              ((bool) yieldedType) ? *yieldedType : QualifiedType(),
                              std::move(poiInfo));
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
    return CallResolutionResult(QualifiedType());
  }

  // otherwise, construct the tuple type
  std::vector<const Type*> eltTypes;

  for (const auto& actual : ci.actuals()) {
    QualifiedType q = actual.type();
    const Type* t = q.type();
    eltTypes.push_back(t);
  }

  const Type* t = nullptr;
  QualifiedType::Kind kind;
  if (allType) {
    kind = QualifiedType::TYPE;
    t = TupleType::getValueTuple(context, std::move(eltTypes));
  } else if (allValue) {
    kind = QualifiedType::CONST_REF;
    t = TupleType::getReferentialTuple(context, std::move(eltTypes));
    // Use CONST_VAR intent for tuple expressions containing no references
    if (t->toTupleType()->toValueTuple(context) == t->toTupleType()) {
      kind = QualifiedType::CONST_VAR;
    }
  } else {
    context->error(tuple, "Mix of value and type tuple elements in tuple expr");
    kind = QualifiedType::UNKNOWN;
    t = ErroneousType::get(context);
  }

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

CallResolutionResult resolveCall(ResolutionContext* rc,
                                 const Call* call,
                                 const CallInfo& ci,
                                 const CallScopeInfo& inScopes,
                                 std::vector<ApplicabilityResult>* rejected,
                                 bool skipForwarding) {
  Context* context = rc->context();
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

    if (resolveMethodCallSpecial(context, call, ci, inScopes, keywordRes)) {
      return keywordRes;
    }

    // otherwise do regular call resolution
    return resolveFnCall(rc, call, call, ci, inScopes, rejected, skipForwarding);
  } else if (auto prim = call->toPrimCall()) {
    return resolvePrimCall(rc, prim, ci, inScopes.callScope(), inScopes.poiScope());
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
  return CallResolutionResult(emptyCandidates,
                              /* rejectedPossibleIteratorCandidates */ false,
                              emptyType, emptyType, emptyPoi);
}

CallResolutionResult
resolveCallInMethod(ResolutionContext* rc,
                    const Call* call,
                    const CallInfo& ci,
                    const CallScopeInfo& inScopes,
                    QualifiedType implicitReceiver,
                    std::vector<ApplicabilityResult>* rejected) {

  CallResolutionResult asFunction = resolveCall(rc,call,ci,inScopes,rejected);

  CallResolutionResult asMethod;
  if (shouldAttemptImplicitReceiver(ci, implicitReceiver)) {
    auto methodCi = CallInfo::createWithReceiver(ci, implicitReceiver);
    bool skipForwarding = asFunction.mostSpecific().foundCandidates();
    asMethod = resolveCall(rc, call, methodCi, inScopes, rejected, skipForwarding);
  }
  if (asMethod.mostSpecific().foundCandidates() && asFunction.mostSpecific().foundCandidates()) {
    ID methodId = asMethod.mostSpecific().only().fn()->id();
    ID functionId = asFunction.mostSpecific().only().fn()->id();
    CHPL_REPORT(rc->context(), AmbiguousCall, call, methodId, functionId);
  }
  if (asMethod.mostSpecific().foundCandidates()) {
    return asMethod;
  } else {
    return asFunction;
  }
}

CallResolutionResult resolveGeneratedCall(ResolutionContext* rc,
                                          const AstNode* astContext,
                                          const CallInfo& ci,
                                          const CallScopeInfo& inScopes,
                                          std::vector<ApplicabilityResult>* rejected) {
  QualifiedType tmpRetType;

  // see if the call is handled directly by the compiler
  if (resolveFnCallSpecial(rc->context(), astContext, ci, tmpRetType)) {
    return CallResolutionResult(std::move(tmpRetType));
  }

  CallResolutionResult keywordRes;
  if (resolveMethodCallSpecial(rc->context(), astContext, ci, inScopes, keywordRes)) {
    return keywordRes;
  }

  // otherwise do regular call resolution
  const Call* call = nullptr;
  return resolveFnCall(rc, astContext, call, ci, inScopes, rejected);
}

CallResolutionResult
resolveGeneratedCallInMethod(ResolutionContext* rc,
                             const AstNode* astContext,
                             const CallInfo& ci,
                             const CallScopeInfo& inScopes,
                             QualifiedType implicitReceiver) {
  // If there is an implicit receiver and ci isn't written as a method,
  // construct a method call and use that instead. If that resolves,
  // it takes precedence over functions.
  if (shouldAttemptImplicitReceiver(ci, implicitReceiver)) {
    auto methodCi = CallInfo::createWithReceiver(ci, implicitReceiver);
    auto ret = resolveGeneratedCall(rc, astContext, methodCi, inScopes);
    if (ret.mostSpecific().foundCandidates()) {
      return ret;
    }
  }

  // otherwise, resolve a regular function call
  return resolveGeneratedCall(rc, astContext, ci, inScopes);
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
                     /* calledType */ QualifiedType(),
                     /* isMethodCall */ true,
                     /* hasQuestionArg */ false,
                     /* isParenless */ false, actuals);

  const Scope* scope = nullptr;
  if (astForScopeOrErr) scope = scopeForId(context, astForScopeOrErr->id());

  auto rc = createDummyRC(context);
  auto c = resolveGeneratedCall(&rc, astForScopeOrErr, ci,
                                CallScopeInfo::forNormalCall(scope, poiScope));
  return c.mostSpecific().only().fn();
}

bool addExistingSubstitutionsAsActuals(Context* context,
                                       const types::Type* type,
                                       std::vector<CallInfoActual>& outActuals,
                                       std::vector<const uast::AstNode*>& outActualAsts) {
  bool addedSubs = false;
  while (auto ct = type->getCompositeType()) {
    if (!ct->instantiatedFromCompositeType()) break;

    for (auto& [id, qt] : ct->substitutions()) {
      auto fieldAst = parsing::idToAst(context, id)->toVarLikeDecl();
      if (fieldAst->storageKind() == QualifiedType::TYPE ||
          fieldAst->storageKind() == QualifiedType::PARAM) {
        addedSubs = true;
        outActuals.emplace_back(qt, fieldAst->name());
        outActualAsts.push_back(nullptr);
      }
    }

    if (auto clt = ct->toBasicClassType()) {
      type = clt->parentClassType();
    } else {
      break;
    }
  }

  return addedSubs;
}

const TypedFnSignature* tryResolveZeroArgInit(Context* context,
                                              const AstNode* astForScopeOrErr,
                                              const types::Type* toInit,
                                              const PoiScope* poiScope) {
  if (!toInit->getCompositeType()) return nullptr;

  QualifiedType toInitQt(QualifiedType::INIT_RECEIVER, toInit);

  std::vector<CallInfoActual> actuals;
  std::vector<const uast::AstNode*> ignoredActualAsts;
  actuals.push_back(CallInfoActual(toInitQt, USTR("this")));
  addExistingSubstitutionsAsActuals(context, toInit, actuals, ignoredActualAsts);
  auto ci = CallInfo(/* name */ USTR("init"),
                     /* calledType */ QualifiedType(),
                     /* isMethodCall */ true,
                     /* hasQuestionArg */ false,
                     /* isParenless */ false, actuals);

  const Scope* scope = nullptr;
  if (astForScopeOrErr) scope = scopeForId(context, astForScopeOrErr->id());

  auto rc = createDummyRC(context);
  auto c = resolveGeneratedCall(&rc, astForScopeOrErr, ci,
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
                     /* calledType */ QualifiedType(),
                     /* isMethodCall */ true,
                     /* hasQuestionArg */ false,
                     /* isParenless */ false,
                     actuals);

  const Scope* scope = nullptr;
  if (astForScopeOrErr) scope = scopeForId(context, astForScopeOrErr->id());

  auto rc = createDummyRC(context);
  auto c = resolveGeneratedCall(&rc, astForScopeOrErr, ci,
                                CallScopeInfo::forNormalCall(scope, poiScope));
  return c.mostSpecific().only().fn();
}

static bool
matchImplementationPoint(ResolutionContext* rc,
                         const InterfaceType* ift,
                         const ImplementationPoint* implPoint,
                         bool& outIsGeneric) {
  if (ift->id() != implPoint->interface()->id()) return false;

  // Use 'const var' so that canPass doesn't say that a type instantiates
  // itself.
  auto actualT = QualifiedType(QualifiedType::CONST_VAR, ift);
  auto formalT = QualifiedType(QualifiedType::CONST_VAR, implPoint->interface());
  auto got = canPass(rc->context(), actualT, formalT);

  outIsGeneric = got.instantiates();
  return got.passes();
}

const ImplementationWitness* findMatchingImplementationPoint(ResolutionContext* rc,
                                                            const types::InterfaceType* ift,
                                                            const CallScopeInfo& inScopes) {
  auto implPoints =
    visibileImplementationPointsForInterface(rc->context(), inScopes.lookupScope(), inScopes.poiScope(), ift->id());

  // TODO: this matches production, in which the first matching generic
  // implementation is used if no concrete one is found. It's probably
  // better to use the same disambiguation rules as functions, though.
  // I don't see a particularly nice way to do that, though.
  const ImplementationPoint* generic = nullptr;
  if (implPoints) {
    for (auto implPoint : *implPoints) {
      bool isGeneric = false;
      if (matchImplementationPoint(rc, ift, implPoint, isGeneric)) {
        if (isGeneric && generic == nullptr) {
          generic = implPoint;
        } else if (!isGeneric) {
          // For a concrete instantiation point, the current search scope
          // is irrelevant; use the point's scope for the search.

          auto implScope = scopeForId(rc->context(), implPoint->id());
          auto checkScope = CallScopeInfo::forNormalCall(implScope, nullptr);
          if (auto witness = checkInterfaceConstraints(rc, ift, implPoint->id(), checkScope)) {
            return witness;
          }
        }
      }
    }
  }

  if (generic) {
    // For a generic instantiation point, construct a new PoI scope from
    // the current search scope, and use the point's scope for the search.

    auto implScope = scopeForId(rc->context(), generic->id());
    auto poiScope = pointOfInstantiationScope(rc->context(), inScopes.callScope(), inScopes.poiScope());
    auto checkScope = CallScopeInfo::forNormalCall(implScope, poiScope);
    if (auto witness = checkInterfaceConstraints(rc, ift, generic->id(), checkScope)) {
      return witness;
    }
  }

  return nullptr;
}

struct InterfaceCheckHelper {
  ResolutionContext* rc;
  const Interface* itf;
  const InterfaceType* ift;
  const ID& implPointId;
  const CallScopeInfo& inScopes;
  const ImplementationWitness* witness;
  PlaceholderMap* allPlaceholders;
  bool allGenerated;

  // special case behavior fields below.
  // if not null, we're trying to infer the return type for contexts (storing into this map)
  ImplementationWitness::AssociatedTypeMap* inferContextReturnType;
  // When encountering 'IFC_ANY_RETURN_INTENT', we will save all overloads we found.
  optional<MostSpecificCandidates> returnIntentOverloads;

  optional<CallInfo> setupCallForTemplate(const Function* templateFn,
                                          const TypedFnSignature* templateSig) {
    std::vector<CallInfoActual> actuals;
    for (int i = 0; i < templateSig->numFormals(); i++) {
      auto decl = templateSig->untyped()->formalDecl(i);
      auto name = UniqueString();
      if (auto formal = decl->toFormal()) {
        name = formal->name();
      } else if (decl->isVarArgFormal()) {
        CHPL_UNIMPL("vararg formals in interface function requirements");
        return empty;
      }
      actuals.emplace_back(templateSig->formalType(i), name);
    }

    CallInfo ci {
      templateSig->untyped()->name(),
        /* calledType */ QualifiedType(),
        /* isMethodCall */ templateFn->isMethod(),
        /* hasQuestionArg */ false,
        /* isParenless */ templateFn->isParenless(),
        std::move(actuals)
    };

    return ci;
  }

  const TypedFnSignature*
  findBestCandidateForTemplate(const CallInfo& ci,
                               const CallResolutionResult& c,
                               const Function* templateFn,
                               const TypedFnSignature* templateSigWithSubs) {
    std::vector<ApplicabilityResult> rejected;
    std::vector<const TypedFnSignature*> ambiguous;

    bool failed = c.exprType().isUnknownOrErroneous();
    if (failed && c.mostSpecific().isAmbiguous()) {
      // TODO: no way at this time to collected candidates rejected due to
      //       ambiguity, so just report an empty list.
      CHPL_REPORT(rc->context(), InterfaceAmbiguousFn, ift, implPointId,
                  templateFn, std::move(ambiguous));
      return nullptr;
    } else if (failed) {
      // Failed to find a call, not due to ambiguity. Re-run call and gather
      // rejected candidates.
      resolveGeneratedCall(rc, templateFn, ci, inScopes, &rejected);
      CHPL_REPORT(rc->context(), InterfaceMissingFn, ift, implPointId,
                  templateSigWithSubs, ci, std::move(rejected));
      return nullptr;
    }

    CHPL_ASSERT(!failed);

    const TypedFnSignature* foundFn = nullptr;
    if (c.mostSpecific().numBest() > 1) {

      // For when encountering return intent overloading, we would normally
      // pick a "best overload". However, IFC_ANY_RETURN_INTENT is meant
      // to allow us to defer selecting a specific overload (one use case
      // is the contextManager, where we pick different overloads depending
      // on different intents).
      if (auto ag = templateFn->attributeGroup()) {
        if (ag->hasPragma(uast::pragmatags::PRAGMA_IFC_ANY_RETURN_INTENT)) {
          returnIntentOverloads = c.mostSpecific();
          for (const auto& candidate : c.mostSpecific()) {
            if (!candidate) continue;
            return candidate.fn();
          }
          CHPL_ASSERT(false && "numBest() > 1 but no candidate found");
        }
      }

      CHPL_UNIMPL("return intent overloading in interface constraint checking");
      return nullptr;
    } else {
      // There's only one function
      foundFn = c.mostSpecific().only().fn();
    }

    return foundFn;
  }

  bool validateFormalOrderForTemplate(const CallInfo& ci,
                                      const TypedFnSignature* templateSigWithSubs,
                                      const TypedFnSignature* candidateSig) {
    // Validate that the formal names are in the right order. We could do this
    // by resolving a generated call with every actual being named, but this
    // will miss some cases, like when the template is:
    //
    //      proc f(x: int, y: int)
    //
    //  and the actual function is:
    //
    //      proc f(y: int, x: int):
    //
    // such calls would resolve match with and without named actuals, but the
    // actuals are not in the right order compared to the template.
    FormalActualMap faMap(candidateSig, ci);

    int lastActualPosition = -1;
    for (auto& formalActual : faMap.byFormals()) {
      if (formalActual.actualIdx() == -1) {
        // Allow defaulted formals to be skipped vs. a template, so that a template
        //
        //    proc foo();
        //
        // Can match:
        //
        //    proc foo(x: int = 10) {}
        continue;
      }

      if (formalActual.actualIdx() <= lastActualPosition) {
        // the actuals in the call (which are formals from the template)
        // are re-ordered compared to the foundFn. This is an error.
        CHPL_REPORT(rc->context(), InterfaceReorderedFnFormals, ift,
                    implPointId, templateSigWithSubs, candidateSig);
        return false;
      }
      lastActualPosition = formalActual.actualIdx();
    }

    return true;
  }

  bool validateReturnTypeForTemplate(const CallInfo& ci,
                                     const CallResolutionResult& c,
                                     const Function* templateFn,
                                     const TypedFnSignature* templateSig,
                                     const TypedFnSignature* candidateSig) {
    CHPL_ASSERT(!c.exprType().isUnknownOrErroneous());

    if (inferContextReturnType && templateFn->name() == USTR("enterContext")) {
      // We were asked to infer the return type using this function. Nothing
      // to validate, but pull the return type from the call and store it in
      // the map.
      auto retType = c.exprType();
      for (auto child : itf->stmts()) {
        auto vd = child->toVariable();
        if (!vd) continue;

        if (vd->name() != "contextReturnType") continue;

        inferContextReturnType->emplace(vd->id(), retType.type());
        return true;
      }

      CHPL_ASSERT(false && "did not find associated type to be inferred.");
    }

    // for interface functions in particular, no return type type means
    // "void" (where it normally means "infer from body"). 'returnType'
    // does not check for this case because it's unusual and because checking
    // for IDs being inside an interface is relatively slow.
    QualifiedType templateQT;

    if (templateFn->returnType()) {
      // Create a resolver for the interface, which pushes the current interface
      // as a frame for the resolver and thus provides 'witness' (which contains
      // associated types) to function resolution.
      ResolutionResultByPostorderID byPostorder;
      auto resolver = Resolver::createForInterfaceStmt(rc, itf, ift, witness, templateFn, byPostorder);

      // Note: want to use the signature without substitutes here, because
      // if we instantiate the signature with the "real" Self types, we
      // get receiver scopes from Self when resolving the return type. However,
      // we want the return type of the function to be determined entirely
      // from its declaration within the interface, without the need to search
      // instantiated scopes. So, use the signature with placeholders to
      // perform return type resolution, _then_ replace the placeholders
      // with associated types etc. to get the final return type.
      templateQT =
        returnType(rc, templateSig, c.poiInfo().poiScope())
          .substitute(rc->context(), *allPlaceholders);
    } else {
      templateQT = QualifiedType(QualifiedType::CONST_VAR, VoidType::get(rc->context()));
    }

    if (templateQT.isUnknownOrErroneous()) return false;

    auto actualKind = c.exprType().kind();
    auto templateKind = templateQT.kind();
    auto templateT = templateQT.type();

    if (templateFn->kind() != candidateSig->untyped()->kind()) {
      // TODO: special error message about function kind mismatch
      rc->context()->error(implPointId, "function kind mismatch");
      return false;
    }


    // For iterable things, we need to compare yield types (while there,
    // figure out yield intent)
    bool checkedYield = false;
    if (templateFn->kind() == Function::ITER) {
      CHPL_ASSERT(candidateSig->untyped()->kind() == Function::ITER);
      CHPL_ASSERT(templateT->isFnIteratorType());
      auto templateYieldT = yieldTypeForIterator(rc, templateT->toFnIteratorType()).type();
      actualKind = c.yieldedType().kind();
      auto yieldT = c.yieldedType().type();

      if (templateYieldT != yieldT) {
        // TODO: error message (expecting exactly matching yield types)
        rc->context()->error(implPointId, "yield type mismatch");
        return false;
      }

      checkedYield = true;
    }

    bool validIntent = actualKind == templateKind;
    if (actualKind != templateKind) {
      if (templateKind == QualifiedType::CONST_VAR &&
          (actualKind == QualifiedType::REF ||
           actualKind == QualifiedType::CONST_REF ||
           actualKind == QualifiedType::PARAM)) {
        // we can turn [const] references into values, so allow this intent mismatch.
        validIntent = true;
      } else if (templateKind == QualifiedType::CONST_REF &&
                 actualKind == QualifiedType::REF) {
        // we can turn references into const references, so allow this intent mismatch.
        validIntent = true;
      }
    }

    if (!validIntent) {
      bool silenceReturnIntentError = false;
      if (auto ag = templateFn->attributeGroup()) {
        if (ag->hasPragma(uast::pragmatags::PRAGMA_IFC_ANY_RETURN_INTENT)) {
          silenceReturnIntentError = true;
        }
      }

      if (!silenceReturnIntentError) {
        CHPL_REPORT(rc->context(), InterfaceInvalidIntent, ift,
                    implPointId, templateSig, candidateSig);
        return false;
      }
    }

    if (checkedYield) {
      // we checked the return type above, no more checks needed here.
    } else if (templateT != c.exprType().type()) {
      // with the exception of promotion, we expect an exact return type match
      if (!c.exprType().type()->isPromotionIteratorType()) {
        rc->context()->error(implPointId, "return type mismatch");
        return false;
      }

      // we're promoting; we don't expect an exact match, but we only allow
      // promotion for void- and nothing-returning functions. Check that.
      CHPL_ASSERT(!c.yieldedType().isUnknownOrErroneous());
      auto yieldT = c.yieldedType().type();
      if (!templateT->isVoidType() && !templateT->isNothingType()) {
        // TODO: special error message about promotion w.r.t. return type
        rc->context()->error(implPointId, "promotion used but not supported for non-void interface functions");
        return false;
      }

      if (!yieldT->isVoidType() && !yieldT->isNothingType()) {
        // TODO: special error message about promotion w.r.t. return type
        rc->context()->error(implPointId, "promotion used but scalar function has non-void return type");
        return false;
      }

      // ok: we expect a void function, we're promoting a void function to
      // satisfy the constraint.
    }

    return true;
  }

  const TypedFnSignature* searchFunctionByTemplate(const Function* templateFn,
                                                   const TypedFnSignature* templateSig) {
    CHPL_ASSERT(allPlaceholders);
    auto templateSigWithSubs = templateSig->substitute(rc->context(), *allPlaceholders);

    returnIntentOverloads.reset();

    auto ci = setupCallForTemplate(templateFn, templateSigWithSubs);
    if (!ci) return  nullptr;

    // TODO: how to note this?
    auto c =
      resolveGeneratedCall(rc, templateFn, *ci, inScopes);

    if (c.exprType().isUnknownOrErroneous() && templateFn->body()) {
      // template has a default implementation; return it, we're good.
      return templateSigWithSubs;
    }

    auto foundFn = findBestCandidateForTemplate(*ci, c, templateFn,
                                                templateSigWithSubs);
    if (!foundFn) return nullptr;

    if (!validateFormalOrderForTemplate(*ci, templateSigWithSubs, foundFn)) {
      return nullptr;
    }

    if (!validateReturnTypeForTemplate(*ci, c, templateFn, templateSig, foundFn)) {
      return nullptr;
    }

    // Found the function. Is it compiler-generated?
    if (!foundFn->isCompilerGenerated()) {
      allGenerated = false;
    }

    // ordering is fine, so foundFn is good to go.
    return foundFn;
  }

  QualifiedType searchForAssociatedType(const QualifiedType& receiverType,
                                        const Variable* td,
                                        bool& outInferType) {
    // Set up a parenless type-proc call to compute associated type
    auto ci = CallInfo(
      td->name(),
      /* calledType */ QualifiedType(),
      /* isMethodCall */ true,
      /* hasQuestionArg */ false,
      /* isParenless */ true,
      /* actuals */ { { receiverType, USTR("this") } }
    );

    // TODO: how to note this?
    auto c =
      resolveGeneratedCall(rc, td, ci, inScopes);

    std::vector<ApplicabilityResult> rejected;
    bool failed = c.exprType().isUnknownOrErroneous();
    bool notType = false;
    if (!failed) {
      notType = !c.exprType().isType();
      if (notType) {
        rejected.push_back(
            ApplicabilityResult::failure(c.mostSpecific().only().fn(),
                                         FAIL_INTERFACE_NOT_TYPE_INTENT));
      }
    } else {
      resolveGeneratedCall(rc, td, ci, inScopes, &rejected);
    }

    if (!failed && !notType) {
      // Ok, we found a type. Note whether it was generated or user-supplied.
      if (!c.mostSpecific().only().fn()->isCompilerGenerated()) {
        allGenerated = false;
      }
    } else {
      // Special case: for context managers, to ease migration, infer
      // the context return type. Allow it to be missing here.

      if (parsing::idIsInBundledModule(rc->context(), td->id()) &&
          ift->id().symbolPath() == "ChapelContext.contextManager" &&
          td->name() == "contextReturnType") {
        // No error message; we allow inferring this type.
        outInferType = true;
      } else {
        // No, sorry, we don't infer this type, and we couldn't find it.
        CHPL_REPORT(rc->context(), InterfaceMissingAssociatedType, ift,
                    implPointId, td, ci, std::move(rejected));
      }

      return QualifiedType();
    }

    return c.exprType();
  }
};

static const ImplementationWitness* const&
checkInterfaceConstraintsQuery(ResolutionContext* rc,
                               const InterfaceType* ift,
                               const ID& implPointIdForErr,
                               const Scope* inScope,
                               const PoiScope* inPoiScope) {
  CHPL_RESOLUTION_QUERY_BEGIN(checkInterfaceConstraintsQuery, rc, ift, implPointIdForErr, inScope, inPoiScope);

  auto inScopes = CallScopeInfo::forNormalCall(inScope, inPoiScope);

  const ImplementationWitness* result = nullptr;
  auto itf = parsing::idToAst(rc->context(), ift->id())->toInterface();

  // First, process any associated constraints, and create a "phase 1"
  // implementation witness with this information.
  // TODO: not used in production today, so not implemented,
  ImplementationWitness::ConstraintMap associatedConstraints;
  bool allGenerated = true;
  bool anyWitnesses = false;
  auto witness1 =
    ImplementationWitness::get(rc->context(), associatedConstraints,
                               {}, {}, {}, allGenerated);

  // Next, process all the associated types, and create a "phase 2"
  // implementation witness.

  // Here, interface formals aren't 'outer variables' since they live in the
  // same symbol (the interface), so insert them into 'byPostorderForAssociatedTypes'
  // as a shortcut for 'resolveNamedDecl' given ift->subs().
  ResolutionResultByPostorderID byPostorderForAssociatedTypes;
  byPostorderForAssociatedTypes.setupForSymbol(itf);
  for (auto& sub : ift->substitutions()) {
    byPostorderForAssociatedTypes.byId(sub.first).setType(sub.second);
  }
  auto associatedReceiverType = QualifiedType(); // cached across iterations
  ImplementationWitness::AssociatedTypeMap associatedTypes;
  InterfaceCheckHelper helper {
    rc, itf, ift, implPointIdForErr, inScopes, witness1, nullptr, allGenerated, nullptr,
  };
  for (auto stmt : itf->stmts()) {
    auto td = stmt->toVariable();
    if (!td) continue;

    anyWitnesses = true;

    // Only associated type are valid declarations in this position
    CHPL_ASSERT(td->storageKind() == QualifiedType::TYPE);

    ResolutionResultByPostorderID byPostorder;
    auto resolver = Resolver::createForInterfaceStmt(rc, itf, ift, witness1, stmt, byPostorder);
    td->traverse(resolver);
    if (associatedReceiverType.kind() != QualifiedType::TYPE) {
      // for associated types of multi-type interfaces, resolve the call
      // on a tuple.
      if (itf->numFormals() > 1) {
        std::vector<const Type*> formalTypes;
        for (auto formal : itf->formals()) {
          formalTypes.push_back(ift->substitutions().at(formal->id()).type());
        }

        associatedReceiverType =
          QualifiedType(QualifiedType::TYPE,
                        TupleType::getValueTuple(rc->context(), formalTypes));
      } else {
        associatedReceiverType = ift->substitutions().at(itf->formal(0)->id());
      }
    }

    bool inferType = false;
    auto foundQt = helper.searchForAssociatedType(associatedReceiverType, td, inferType);
    if (foundQt.isUnknownOrErroneous()) {

      // Right now, this is only hit as a special case, so we know we're inferring
      // the context return type. In the future, if we support inference more
      // generally, note what type is being inferred and how it should be
      // computed.
      if (inferType) {
        helper.inferContextReturnType = &associatedTypes;
        continue;
      }

      result = nullptr;
      return CHPL_RESOLUTION_QUERY_END(result);
    } else {
      associatedTypes.emplace(td->id(), foundQt.type());
    }
  }
  auto witness2 =
    ImplementationWitness::get(rc->context(), associatedConstraints,
                               associatedTypes, {}, {},
                               anyWitnesses && helper.allGenerated);

  // Next, process all the functions; if all of these are found, we can construct
  // a final witness with all the required information.
  PlaceholderMap allPlaceholders;
  for (auto& [id, t] : associatedTypes) allPlaceholders.emplace(id, t);
  for (auto& [id, qt] : ift->substitutions()) allPlaceholders.emplace(id, qt.type());
  ImplementationWitness::FunctionMap functions;
  ImplementationWitness::OverloadMap returnIntentOverloads;
  helper.witness = witness2;
  helper.allPlaceholders = &allPlaceholders;
  for (auto stmt : itf->stmts()) {
    auto fn = stmt->toFunction();
    if (!fn) continue;

    if (auto ag = fn->attributeGroup()) {
      if (ag->hasPragma(PRAGMA_DOCS_ONLY)) continue;
    }

    anyWitnesses = true;

    // Note: construct a resolver with the witness above, which pushes
    // an interface frame onto the ResolutionContext. This is required for
    // resolving typed signatures in the interface.
    ResolutionResultByPostorderID byPostorder;
    auto resolver = Resolver::createForInterfaceStmt(rc, itf, ift, witness2, stmt, byPostorder);


    // Construct an initial typed signature, which will have opaque placeholder
    // types for 'Self', associated types, etc. Then, replace the placeholders
    // with the substitutions we've determined, so that we may proceed to
    // type checking.
    auto tfs = typedSignatureTemplateForId(rc, fn->id());
    auto foundFn = helper.searchFunctionByTemplate(fn, tfs);

    if (!foundFn) {
      result = nullptr;
      return CHPL_RESOLUTION_QUERY_END(result);
    } else {
      functions.emplace(fn->id(), foundFn);
      if (helper.returnIntentOverloads) {
        returnIntentOverloads.emplace(fn->id(), std::move(*helper.returnIntentOverloads));
      }
    }
  }

  result =
    ImplementationWitness::get(rc->context(), associatedConstraints,
                               std::move(associatedTypes),
                               std::move(functions),
                               std::move(returnIntentOverloads),
                               anyWitnesses && helper.allGenerated);
  return CHPL_RESOLUTION_QUERY_END(result);
}

const ImplementationWitness*
checkInterfaceConstraints(ResolutionContext* rc,
                          const InterfaceType* ift,
                          const ID& implPointId,
                          const CallScopeInfo& inScopes) {
  return checkInterfaceConstraintsQuery(rc, ift, implPointId,
                                        inScopes.callScope(),
                                        inScopes.poiScope());
}

const ImplementationWitness* findOrImplementInterface(ResolutionContext* rc,
                                                      const types::InterfaceType* ift,
                                                      const types::Type* forType,
                                                      const CallScopeInfo& inScopes,
                                                      const ID& implPointId,
                                                      bool& outFoundExisting) {
  outFoundExisting = false;

  auto instantiatedIft =
    InterfaceType::withTypes(rc->context(), ift,
                             { QualifiedType(QualifiedType::TYPE, forType) });
  if (!instantiatedIft) return nullptr;

  auto witness =
    findMatchingImplementationPoint(rc, instantiatedIft, inScopes);

  if (witness) {
    outFoundExisting = true;
    return witness;
  }

  // try automatically satisfy the interface if it's in the standard modules.
  if (parsing::idIsInBundledModule(rc->context(), ift->id())) {
    auto runResult = rc->context()->runAndDetectErrors([&](Context* context) {
      return checkInterfaceConstraints(rc, instantiatedIft, implPointId, inScopes);
    });
    witness = runResult.result();
  }

  return witness;
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
                     /* calledType */ QualifiedType(),
                     /* isMethodCall */ asMethod,
                     /* hasQuestionArg */ false,
                     /* isParenless */ false,
                     actuals);
  const Scope* scope = nullptr;
  if (astForScopeOrErr) scope = scopeForId(context, astForScopeOrErr->id());
  auto rc = createDummyRC(context);
  auto c = resolveGeneratedCall(&rc, astForScopeOrErr, ci,
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
  ID ignoredId;
  return parsing::findFieldIdInDeclaration(ast, name, ignoredId);
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

void getCopyOrAssignableInfo(ResolutionContext* rc, const Type* t,
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
    ResolutionContext* rc, const CompositeType* ct, bool checkCopyable) {
  CHPL_RESOLUTION_QUERY_BEGIN(getCopyOrAssignableInfoQuery, rc, ct, checkCopyable);
  auto context = rc->context();


  CopyableAssignableInfo result = CopyableAssignableInfo::fromNone();

  // Inspect type for either kind of copyability/assignability.
  auto genericity = getTypeGenericity(context, ct);
  if (genericity == Type::GENERIC || genericity == Type::MAYBE_GENERIC) {
    // generic composite types cannot be copied or assigned
    result = CopyableAssignableInfo::fromNone();
  } else if (auto at = ct->toArrayType()) {
    if (auto eltType = at->eltType().type()) {
      // Arrays are copyable/assignable if their elements are
      result = getCopyOrAssignableInfo(rc, eltType, checkCopyable);
    }
  } else if (auto tt = ct->toTupleType()) {
    // Tuples have the minimum copyable/assignable-ness of their elements
    result = CopyableAssignableInfo::fromConst();
    // TODO: add iterator for TupleType element types and use a range-based for
    for (int i = 0; i < tt->numElements(); i++) {
      result.intersectWith(getCopyOrAssignableInfo(
          rc, tt->elementType(i).type(), checkCopyable));
      if (tt->isStarTuple()) break;
    }
  } else {
    auto ast = parsing::idToAst(context, ct->id());
    const AttributeGroup* attrs = nullptr;
    if (ast) attrs = ast->attributeGroup();
    if (checkCopyable && attrs &&
        (attrs->hasPragma(PRAGMA_SYNC))) {
      // Syncs are copyable
      // This is a special case to preserve deprecated behavior before
      // sync implicit reads are removed. 12/8/23
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
              fieldsForTypeDecl(rc, ct, DefaultsPolicy::USE_DEFAULTS);
          for (int i = 0; i < resolvedFields.numFields(); i++) {
            auto fieldType = resolvedFields.fieldType(i).type();
            if (auto classTy = fieldType->toClassType()) {
              result.intersectWith(getClassTypeCopyOrAssignable(classTy));
            } else if (auto rt = fieldType->toRecordType()) {
              // check record fields recursively
              result.intersectWith(
                  getCopyOrAssignableInfo(rc, rt, checkCopyable));
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

  return CHPL_RESOLUTION_QUERY_END(result);
}

CopyableAssignableInfo getCopyOrAssignableInfo(ResolutionContext* rc, const Type* t,
                                               bool checkCopyable) {
  CopyableAssignableInfo result;

  if (auto ct = t->toCompositeType()) {
    // Use query to cache results only for composite types, others are trivial
    result = getCopyOrAssignableInfoQuery(rc, ct, checkCopyable);
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
  return QualifiedType::makeParamBool(context, value);
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

const Decl* findFieldByName(Context* context,
                            const AggregateDecl* ad,
                            const CompositeType* ct,
                            UniqueString name) {
  const Decl* ret = nullptr;

  for (auto decl : ad->children()) {
    if (auto named = decl->toVarLikeDecl()) {
      if (named->name() == name) {
        ret = named;
        break;
      }
    } else if (auto named = decl->toMultiDecl()) {
      for (auto md : named->children()) {
        auto nmd = md->toNamedDecl();
        if (nmd->name() == name) {
          ret = nmd;
          break;
        }
      }
    } else if (auto fwd = decl->toForwardingDecl()) {
      if (auto var = fwd->expr()) {
        auto n = var->toNamedDecl();
        if (n && n->name() == name) {
          ret = n;
          break;
        }
      }
    }
  }

  if (ret == nullptr && ct != nullptr) {
    if (auto bct = ct->toBasicClassType()) {
      if (auto parent = bct->parentClassType()) {
        if (parent->isObjectType() == false) {
          auto parentAD = parsing::idToAst(context, parent->id())->toAggregateDecl();
          ret = findFieldByName(context, parentAD, parent, name);
        }
      }
    }
  }

  return ret;
}

static UniqueString iterKindToUniqueString(Context* context,
                                           Function::IteratorKind kind) {
  switch (kind) {
    case Function::IteratorKind::SERIAL:
      return UniqueString();
    case Function::IteratorKind::STANDALONE:
      return USTR("standalone");
    case Function::IteratorKind::FOLLOWER:
      return USTR("follower");
    case Function::IteratorKind::LEADER:
      return USTR("leader");
  }
  CHPL_ASSERT(false && "unhandled iterator kind");
  return UniqueString();
}

const QualifiedType&
getIterKindConstantOrUnknown(Context* context, Function::IteratorKind iterKind) {
  QUERY_BEGIN(getIterKindConstantOrUnknown, context, iterKind);

  QualifiedType ret = { QualifiedType::UNKNOWN, UnknownType::get(context) };

  auto constant = iterKindToUniqueString(context, iterKind);
  if (!constant.isEmpty()) {
    auto ik = EnumType::getIterKindType(context);
    if (auto m = EnumType::getParamConstantsMapOrNull(context, ik)) {
      auto it = m->find(constant);
      if (it != m->end()) ret = it->second;
    }
  }

  return QUERY_END(ret);
}

static const MostSpecificCandidate&
findTaggedIterator(ResolutionContext* rc,
                   UniqueString name,
                   bool isMethod,
                   std::vector<QualifiedType> argTypes,
                   Function::IteratorKind tag,
                   const Scope* callScope,
                   const Scope* iteratorScope,
                   const PoiScope* poiScope) {
  CHPL_RESOLUTION_QUERY_BEGIN(findTaggedIterator, rc, name, isMethod, argTypes, tag, callScope, iteratorScope, poiScope);

  auto scopeInfo = CallScopeInfo::forIteratorOverloadSearch(callScope, iteratorScope, poiScope);

  CHPL_ASSERT(!isMethod || (argTypes.size() >= 1 && !argTypes[0].isUnknownOrErroneous()));

  auto followThisType = QualifiedType();
  bool isFollower = tag == Function::FOLLOWER;
  bool isSerial = tag == Function::SERIAL;
  if (isFollower) {
    auto candidate = findTaggedIterator(rc, name, isMethod, argTypes,
                                        Function::LEADER, callScope, iteratorScope, poiScope);
    if (candidate && candidate.fn()->isIterator()) {
      followThisType = yieldType(rc, candidate.fn(), poiScope);
    }
  }

  if (isFollower && followThisType.isUnknownOrErroneous()) {
    auto ret = MostSpecificCandidate();
    return CHPL_RESOLUTION_QUERY_END(ret);
  }

  auto iterKindType = EnumType::getIterKindType(rc->context());

  std::vector<CallInfoActual> actuals;
  int idx = -1;
  for (auto argType : argTypes) {
    idx++;

    // We explicitly insert the tag below.
    if (argType.type() == iterKindType) {
      CHPL_ASSERT(!isMethod || idx > 0);
      continue;
    }

    // Pass the 'this' formal by name.
    auto name = UniqueString();
    if (idx == 0 && isMethod) {
      name = USTR("this");
    }
    actuals.push_back(CallInfoActual(argType, name));
  }
  if (!isSerial) {
    auto iterKind = getIterKindConstantOrUnknown(rc->context(), tag);
    if (iterKind.isUnknownOrErroneous()) {
      auto ret = MostSpecificCandidate();
      return CHPL_RESOLUTION_QUERY_END(ret);
    }

    actuals.push_back(CallInfoActual(iterKind, USTR("tag")));
  }

  if (isFollower) {
    actuals.push_back(CallInfoActual(followThisType, USTR("followThis")));
  }

  auto ci = CallInfo(name, QualifiedType(),
                     /* isMethodCall */ isMethod,
                     /* hasQuestionArg */ false,
                     /* isParenless */ false,
                     actuals);

  auto c = resolveGeneratedCall(rc, parsing::idToAst(rc->context(), iteratorScope->id()), ci, scopeInfo);
  auto ret = c.mostSpecific().only();
  return CHPL_RESOLUTION_QUERY_END(ret);
}

static CallScopeInfo callScopeInfoForIterator(Context* context,
                                              const IteratorType* iter,
                                              const Scope* overrideScope) {
  // The ID of the scope to lookup the other overloads in.
  ID id;
  if (auto fnIter = iter->toFnIteratorType()) {
    id = fnIter->iteratorFn()->id();
  } else if (auto loopIter = iter->toLoopExprIteratorType()) {
    id = loopIter->sourceLocation();
  } else {
    CHPL_ASSERT(iter->isPromotionIteratorType());
    id = iter->toPromotionIteratorType()->scalarFn()->id();
  }
  auto iteratorScope = scopeForId(context, id);
  if (overrideScope) {
    iteratorScope = overrideScope;
  }
  auto callScope = iteratorScope;
  auto poiScope = iter->poiScope();

  // If the function needs a PoI scope, this scope will capture functions
  // at the iterator's own point of instantiation; we don't want to include
  // this scope in the overload search (lookup scope), because that would make
  // it possible to introduce new overloads of the iterator via PoI, which we do not
  // want to allow.
  //
  // However, we do want to include the functions available at instantiation time
  // when resolving the bodies of the other overloads, if applicable. So,
  // change to callScope to unwrap one level from the PoI scope.
  //
  // Loop expressions do not create new PoI scopes (should they?) so they
  // are exempt from this.
  //
  // See the comment on CallScopeInfo for details on why three scopes are
  // necessary for resolving functions.
  if (poiScope && !iter->isLoopExprIteratorType()) {
    callScope = poiScope->inScope();
    poiScope = poiScope->inFnPoi();
  }

  return CallScopeInfo::forIteratorOverloadSearch(callScope, iteratorScope, poiScope);
}

const MostSpecificCandidate&
findTaggedIteratorForType(ResolutionContext* rc,
                          const FnIteratorType* fnIter,
                          Function::IteratorKind iterKind,
                          const Scope* overrideScope) {
  CHPL_RESOLUTION_QUERY_BEGIN(findTaggedIteratorForType, rc, fnIter, iterKind, overrideScope);

  auto fn = fnIter->iteratorFn();
  auto name = fnIter->iteratorFn()->untyped()->name();
  std::vector<QualifiedType> argTypes;
  for (int i = 0; i < fn->numFormals(); i++) {
    if (fn->untyped()->formalIsVarArgs(i)) {
      auto varArgs = fn->formalType(i);
      CHPL_ASSERT(!varArgs.isUnknownOrErroneous());
      auto tup = varArgs.type()->toTupleType();
      CHPL_ASSERT(tup);
      for (int j = 0; j < tup->numElements(); j++) {
        argTypes.push_back(tup->elementType(j));
      }
    } else {
      argTypes.push_back(fn->formalType(i));
    }
  }
  auto inScopes = callScopeInfoForIterator(rc->context(), fnIter, overrideScope);

  auto ret = findTaggedIterator(rc, name, fn->isMethod(), argTypes, iterKind,
                                inScopes.callScope(), inScopes.lookupScope(), inScopes.poiScope());
  return CHPL_RESOLUTION_QUERY_END(ret);
}

const types::QualifiedType&
taggedYieldTypeForType(ResolutionContext* rc,
                       const types::FnIteratorType* fnIter,
                       uast::Function::IteratorKind iterKind,
                       const Scope* overrideScope) {
  CHPL_RESOLUTION_QUERY_BEGIN(taggedYieldTypeForType, rc, fnIter, iterKind, overrideScope);

  // building a call resolution result takes care of setting up the
  // PoI scope and doing type inference using that PoI scope, so do it here.
  auto msc = findTaggedIteratorForType(rc, fnIter, iterKind, overrideScope);
  auto inScopes = callScopeInfoForIterator(rc->context(), fnIter, overrideScope);
  auto c = resolutionResultFromMostSpecificCandidate(rc, msc, inScopes);

  QualifiedType ret;
  if (!c.yieldedType().isUnknownOrErroneous()) {
    ret = c.yieldedType();
  }

  return CHPL_RESOLUTION_QUERY_END(ret);
}

const types::QualifiedType&
yieldTypeForIterator(ResolutionContext* rc,
                     const types::IteratorType* iter) {
  CHPL_RESOLUTION_QUERY_BEGIN(yieldTypeForIterator, rc, iter);

  QualifiedType ret;
  if (auto fnIter = iter->toFnIteratorType()) {
    ret = fnIter->yieldType();
  } else if (auto loopIter = iter->toLoopExprIteratorType()) {
    ret = loopIter->yieldType();
  } else {
    CHPL_ASSERT(iter->isPromotionIteratorType());
    auto promoIter = iter->toPromotionIteratorType();
    ret = promoIter->yieldType();
  }

  return CHPL_RESOLUTION_QUERY_END(ret);
}

static const CallResolutionResult&
resolveIteratorShapeComputation(Context* context,
                                const IteratorType* iter,
                                QualifiedType qt) {

  QUERY_BEGIN(resolveIteratorShapeComputation, context, iter, qt);
  std::vector<CallInfoActual> actuals;
  actuals.emplace_back(qt, UniqueString());

  auto ci = CallInfo(UniqueString::get(context, "chpl_computeIteratorShape"),
                     QualifiedType(),
                     /* isMethodCall */ false,
                     /* hasQuestionArg */ false,
                     /* isParenless */ false,
                     /* actuals */ std::move(actuals));
  auto inScopes = callScopeInfoForIterator(context, iter, /* overrideScopes */ nullptr);

  auto rc = createDummyRC(context);
  auto c = resolveGeneratedCall(&rc, /* astForScopeOrErr */ nullptr, ci, inScopes);
  return QUERY_END(c);
}

static const Type* const&
shapeForIteratorQuery(Context* context,
                      const types::IteratorType* iter) {
  QUERY_BEGIN(shapeForIteratorQuery, context, iter);
  // shape is determined by the leader / standalone. In some cases,
  // the iterator doesn't have the notion of a leader, so we will early-return.
  // Otherwise, we will fall through and work on leader below.
  QualifiedType leaderType;

  if (auto loopIter = iter->toLoopExprIteratorType()) {
    // The first iterand in the zip, if any, is a leader. Otherwise, there's
    // only one iterand, and that's the leader.

    CHPL_ASSERT(!loopIter->iterand().isUnknownOrErroneous());
    if (loopIter->isZippered()) {
      auto iteratedTuple = loopIter->iterand().type()->toTupleType();
      CHPL_ASSERT(iteratedTuple);

      leaderType = iteratedTuple->elementType(0);
    } else {
      leaderType = loopIter->iterand();
    }
  } else if (iter->isFnIteratorType()) {
    // `iter` subroutines can have arbitrary behavior, and thus don't
    // have a shape.

    const Type* result = nullptr;
    return QUERY_END(result);
  } else if (auto promoIterator = iter->toPromotionIteratorType()) {
    auto scalarFn = promoIterator->scalarFn();
    auto& formalMap = promoIterator->promotedFormals();
    for (int i = 0; i < scalarFn->numFormals(); i++) {
      auto promotedEntry = formalMap.find(i);
      if (promotedEntry != formalMap.end()) {
        leaderType = promotedEntry->second;
        break;
      }
    }
  }

  CHPL_ASSERT(!leaderType.isUnknownOrErroneous());
  const Type* result = nullptr;
  if (auto leaderIter = leaderType.type()->toIteratorType()) {
    return QUERY_END(shapeForIteratorQuery(context, leaderIter));
  } else {
    // Resolve this using  call, and make that call a query because typed
    // conversion might eventually require access to it for the purposes
    // of runtime types. Some additional wrangling will be needed to avoid
    // re-extracing leaderType (could we have a set-only query that computes
    // the CallResolutionResult from iter, that we set right here?) but I
    // leave that to you, O brave future implementer.

    auto cr = resolveIteratorShapeComputation(context, iter, leaderType);
    if (!cr.exprType().isUnknownOrErroneous()) {
      result = cr.exprType().type();
    } else {
      // At the time of writing, it should not be possible for us to
      // not find a call, because we have a catch-all overload. However,
      // just in case, handle the case gracefully, simply returning "no shape".
      result = nullptr;
    }
  }
  return QUERY_END(result);
}

const Type* shapeForIterator(Context* context,
                             const types::IteratorType* iter) {
  return shapeForIteratorQuery(context, iter);
}

static TheseResolutionResult
callResolutionResultToTheseResolutionResult(CallResolutionResult cr, QualifiedType iterandType) {
  if (cr.exprType().isUnknownOrErroneous() ||
      !cr.exprType().type()->isIteratorType()) {
    auto reason = TheseResolutionResult::THESE_FAIL_NO_ITERATOR_WITH_TAG;
    return TheseResolutionResult::failure(reason, std::move(iterandType), std::move(cr));
  }

  return TheseResolutionResult::success(cr, iterandType);
}

static TheseResolutionResult
resolveTheseCallForFnIterator(ResolutionContext* rc,
                              const FnIteratorType* fnIt,
                              uast::Function::IteratorKind iterKind,
                              const types::QualifiedType& followThis) {
  auto& msc = findTaggedIteratorForType(rc, fnIt, iterKind);
  auto iterandType = QualifiedType(QualifiedType::CONST_VAR, fnIt);

  if (msc && iterKind == Function::FOLLOWER) {
    // Additionally check that the follower type matches the expected
    // follower type.
    for (int i = 0; i < msc.fn()->numFormals(); i++){
      if (msc.fn()->formalName(i) == USTR("followThis")) {
        auto formalType = msc.fn()->formalType(i);
        auto got = canPassScalar(rc->context(), followThis, formalType);
        if (!got.passes()) {
          auto reason = TheseResolutionResult::THESE_FAIL_LEADER_FOLLOWER_MISMATCH;
          return TheseResolutionResult::failure(reason, std::move(iterandType));
        }
        break;
      }
    }
  }

  auto inScopes = callScopeInfoForIterator(rc->context(), fnIt, nullptr);
  auto cr = resolutionResultFromMostSpecificCandidate(rc, msc, inScopes);

  return callResolutionResultToTheseResolutionResult(cr, std::move(iterandType));
}

// Helper with shared logic between loop expression iterators and promotion,
// in which several iterables (zippered args to loop, multiple promoted args),
// are all part of the 'these' call.
static TheseResolutionResult
resolveTheseCallForZipperedArguments(ResolutionContext* rc,
                                     const AstNode* astContext,
                                     const IteratorType* iterType,
                                     std::vector<QualifiedType> zippered,
                                     uast::Function::IteratorKind iterKind,
                                     const types::QualifiedType& followThis,
                                     std::vector<QualifiedType>* outYieldTypes = nullptr) {
  bool leaderOnly = iterKind == Function::LEADER;
  bool standalone = iterKind == Function::STANDALONE;
  bool serial = iterKind == Function::SERIAL;
  auto iterandType = QualifiedType(QualifiedType::CONST_VAR, iterType);

  // Loop expressions and promotion only support standalone iterators
  // when there's a single iterand.
  if (standalone && zippered.size() > 1) {
    auto reason = iterType->isLoopExprIteratorType() ?
      TheseResolutionResult::THESE_FAIL_NO_LOOP_EXPR_STANDALONE :
      TheseResolutionResult::THESE_FAIL_NO_PROMO_STANDALONE;
    return TheseResolutionResult::failure(reason, std::move(iterandType));
  }

  bool supportsParallel = !iterType->isLoopExprIteratorType() ||
                          iterType->toLoopExprIteratorType()->supportsParallel();

  // the loop was written as a serial loop expression, so no parallel
  // 'these' calls are allowed.
  if (!serial && !supportsParallel) {
    auto reason = TheseResolutionResult::THESE_FAIL_SERIAL_LOOP_EXPR;
    return TheseResolutionResult::failure(reason, std::move(iterandType));
  }

  auto inScopes = callScopeInfoForIterator(rc->context(), iterType, nullptr);
  std::unique_ptr<TheseResolutionResult> failedZipResult = nullptr;
  int index = -1;
  for (auto receiverType : zippered) {
    index++;

    auto tr = resolveTheseCall(rc, astContext, receiverType,
                               iterKind, followThis, inScopes);

    if (!tr) {
      failedZipResult = std::make_unique<TheseResolutionResult>(std::move(tr));
      break;
    }

    if (outYieldTypes) {
      outYieldTypes->push_back(tr.yieldedType());
    }

    if (leaderOnly) return tr;
  }
  CHPL_ASSERT(index != -1);

  if (failedZipResult) {
    return TheseResolutionResult::failure(std::move(failedZipResult), index, std::move(iterandType));
  }

  if (auto loopIt = iterType->toLoopExprIteratorType()) {
    auto cr = CallResolutionResult(QualifiedType(QualifiedType::CONST_VAR, loopIt),
                                   loopIt->yieldType());
    return TheseResolutionResult::success(cr, std::move(iterandType));
  } else {
    CHPL_ASSERT(iterType->isPromotionIteratorType());
    auto promoIt = iterType->toPromotionIteratorType();

    auto scalarReturn = promoIt->yieldType();
    auto cr = CallResolutionResult(QualifiedType(QualifiedType::CONST_VAR, promoIt),
                                   scalarReturn);
    return TheseResolutionResult::success(cr, std::move(iterandType));
  }
}

static TheseResolutionResult
resolveTheseCallForLoopIterator(ResolutionContext* rc,
                                const AstNode* astContext,
                                const LoopExprIteratorType* loopIt,
                                uast::Function::IteratorKind iterKind,
                                const types::QualifiedType& followThis) {
  // When resolving the leader iterator of a zippered loop expression,
  // we only resolve the leader of its first iterand. On the other hand,
  // we resolve all follower iterators of the loop expression.

  std::vector<QualifiedType> receiverTypes;
  if (loopIt->isZippered()) {
    auto receiverQt = loopIt->iterand();
    CHPL_ASSERT(receiverQt.type()->toTupleType());
    auto tupleType = receiverQt.type()->toTupleType();

    for (int i = 0; i < tupleType->numElements(); i++) {
      receiverTypes.push_back(tupleType->elementType(i));
    }
  } else {
    receiverTypes.push_back(loopIt->iterand());
  }

  return resolveTheseCallForZipperedArguments(rc, astContext, loopIt,
                                              receiverTypes, iterKind, followThis);
}

static TheseResolutionResult
resolveTheseCallForPromotionIterator(ResolutionContext* rc,
                                     const AstNode* astContext,
                                     const PromotionIteratorType* promoIt,
                                     uast::Function::IteratorKind iterKind,
                                     const types::QualifiedType& followThis) {
  std::vector<QualifiedType> receiverTypes;
  std::vector<QualifiedType> scalarFormalTypes;
  std::vector<QualifiedType> yieldTypes;

  auto typedScalarFn = promoIt->scalarFn();
  auto untypedScalarFn = typedScalarFn->untyped();
  int numFormals = untypedScalarFn->numFormals();
  for (int i = 0; i < numFormals; i++) {
    auto promotionType = promoIt->promotedFormals().find(i);
    if (promotionType != promoIt->promotedFormals().end()) {
      receiverTypes.push_back(promotionType->second);
    }
  }

  auto tr = resolveTheseCallForZipperedArguments(rc, astContext, promoIt,
                                                 receiverTypes, iterKind, followThis, &yieldTypes);

  if (!tr) return tr;

  // Skip validating yield types for leader iterators.
  if (iterKind == Function::LEADER) return tr;

  // Validate that yield types match the promotion types.
  int index = 0;
  for (auto ri = receiverTypes.begin(), yi = yieldTypes.begin();
       ri != receiverTypes.end() && yi != yieldTypes.end(); ++ri, ++yi, ++index) {
    auto promotionType = getPromotionType(rc->context(), *ri);
    if (promotionType.type() != yi->type()) {
      auto reason = TheseResolutionResult::THESE_FAIL_PROMOTION_TYPE_YIELD_MISMATCH;
      auto failureForIndex = TheseResolutionResult::failure(reason, *ri);
      auto failureForIndexPtr =
        std::make_unique<TheseResolutionResult>(std::move(failureForIndex));

      auto zipperedFailure =
        TheseResolutionResult::failure(std::move(failureForIndexPtr), index,
                                       QualifiedType(QualifiedType::CONST_VAR,
                                         promoIt));
      return zipperedFailure;
    }
  }

  return tr;
}

TheseResolutionResult resolveTheseCall(ResolutionContext* rc,
                                       const uast::AstNode* astContext,
                                       const types::QualifiedType& receiverType,
                                       uast::Function::IteratorKind iterKind,
                                       const types::QualifiedType& followThis,
                                       const CallScopeInfo& inScopes) {
  // Handle 'these' on various iterator types, circumventing the normal
  // process (since we do not generate 'these' methods).
  if (receiverType.type()) {
    if (auto fnIt = receiverType.type()->toFnIteratorType()) {
      return resolveTheseCallForFnIterator(rc, fnIt, iterKind, followThis);
    } else if (auto loopIt = receiverType.type()->toLoopExprIteratorType()) {
      return resolveTheseCallForLoopIterator(rc, astContext, loopIt, iterKind, followThis);
    } else if (auto promoIt = receiverType.type()->toPromotionIteratorType()) {
      return resolveTheseCallForPromotionIterator(rc, astContext, promoIt, iterKind, followThis);
    }
  }

  // Otherwise, just generate a normal 'these' call.

  std::vector<CallInfoActual> actuals;
  actuals.push_back(CallInfoActual(receiverType, USTR("this")));

  auto iterKindActual = getIterKindConstantOrUnknown(rc->context(), iterKind);
  CHPL_ASSERT(iterKind == Function::SERIAL || !iterKindActual.isUnknown());
  if (!iterKindActual.isUnknown()) {
    actuals.emplace_back(iterKindActual, USTR("tag"));
  }

  if (!followThis.isUnknown()) {
    actuals.emplace_back(followThis, USTR("followThis"));
  }

  auto ci = CallInfo(USTR("these"),
                     QualifiedType(),
                     /* isMethodCall */ true,
                     /* hasQuestionArg */ false,
                     /* isParenless */ false,
                     /* actuals */ std::move(actuals));
  auto cr = resolveGeneratedCall(rc, astContext, ci, inScopes);
  return callResolutionResultToTheseResolutionResult(cr, receiverType);
}

static const types::QualifiedType& getPromotionTypeQuery(Context* context, types::QualifiedType qt) {
  QUERY_BEGIN(getPromotionTypeQuery, context, qt);

  auto ret = QualifiedType();
  if (qt.isUnknownOrErroneous()) {
    /* do nothing */
  } else if (auto loopIt = qt.type()->toLoopExprIteratorType()) {
    ret = loopIt->yieldType();
  } else if (auto fnIt = qt.type()->toFnIteratorType()) {
    ret = fnIt->yieldType();
  } else if (auto promoIt = qt.type()->toPromotionIteratorType()) {
    ret = promoIt->yieldType();
  } else {
    std::vector<CallInfoActual> actuals;
    actuals.push_back(CallInfoActual(qt, USTR("this")));
    auto ci = CallInfo(UniqueString::get(context, "chpl__promotionType"),
        QualifiedType(),
        /* isMethodCall */ true,
        /* hasQuestionArg */ false,
        /* isParenless */ false,
        /* actuals */ std::move(actuals));

    auto t = qt.type();
    const AstNode* astContext = nullptr;
    if (t) {
      if (auto ct = t->getCompositeType()) {
        if (!ct->id().isEmpty()) {
          astContext = parsing::idToAst(context, ct->id());
        }
      }
    }

    // only the receiver type in the call info should be used for search
    auto scopes = CallScopeInfo::forNormalCall(nullptr, nullptr);
    auto rc = createDummyRC(context);
    auto c = resolveGeneratedCall(&rc, astContext, ci, scopes);

    ret = c.exprType();

    // Fix the intent to match original actual
    ret = QualifiedType(qt.kind(), ret.type());

  }

  return QUERY_END(ret);
}

const types::QualifiedType& getPromotionType(Context* context, types::QualifiedType qt, bool skipIfRunning) {
  // for non-type, non-param intents, normalize to const var.
  if (!qt.isParam() && !qt.isType()) {
    qt = QualifiedType(QualifiedType::CONST_VAR, qt.type());
  }

  if (skipIfRunning && context->isQueryRunning(getPromotionTypeQuery, std::make_tuple(qt))) {
    static auto empty = QualifiedType();
    return empty;
  }

  return getPromotionTypeQuery(context, qt);
}

static const types::RuntimeType* const& getRuntimeTypeQuery(Context* context, const types::CompositeType* ct) {
  QUERY_BEGIN(getRuntimeTypeQuery, context, ct);

  CHPL_ASSERT(ct->isArrayType() || ct->isDomainType());

  auto ci = CallInfo(UniqueString::get(context, "chpl__convertValueToRuntimeType"),
    QualifiedType(),
    /* isMethodCall */ false,
    /* hasQuestionArg */ false,
    /* isParenless */ false,
    /* actuals */ { CallInfoActual(QualifiedType(QualifiedType::VAR, ct), UniqueString()) });

  auto ctAst = parsing::idToAst(context, ct->id());
  auto scopes = CallScopeInfo::forNormalCall(scopeForId(context, ct->id()), nullptr);
  auto rc = createDummyRC(context);
  auto c = resolveGeneratedCall(&rc, ctAst, ci, scopes);

  const RuntimeType* ret = nullptr;
  if (!c.exprType().isUnknownOrErroneous() && c.mostSpecific().numBest() == 1) {
    // the conversion function's body should just be one 'return', which
    // is the builder. We could be smarter about it (handling control flow,
    // param conditionals, etc.), but why?

    auto fn = parsing::idToAst(context, c.mostSpecific().only().fn()->id())->toFunction();
    CHPL_ASSERT(fn);
    auto body = fn->stmt(0);
    CHPL_ASSERT(body && body->isReturn() && body->toReturn()->value());

    ResolutionContext rcval(context);
    auto rFn = resolveFunction(&rcval, c.mostSpecific().only().fn(), c.poiInfo().poiScope());
    if (rFn) {
      auto returnedRE = rFn->resolutionById().byAstOrNull(body->toReturn()->value());
      if (returnedRE && returnedRE->mostSpecific().only()) {
        auto calledBuilder = returnedRE->mostSpecific().only().fn();
        ret = RuntimeType::get(context, calledBuilder);
      }
    }
  }
  CHPL_ASSERT(ret);

  return QUERY_END(ret);
}

const types::RuntimeType* getRuntimeType(Context* context, const types::CompositeType* ct) {
  return getRuntimeTypeQuery(context, ct);
}

Access accessForQualifier(Qualifier q) {
  if (q == Qualifier::REF ||
      q == Qualifier::OUT ||
      q == Qualifier::INOUT) {
    return REF;
  }

  if (q == Qualifier::CONST_REF) {
    return CONST_REF;
  }

  if (q == Qualifier::REF_MAYBE_CONST) {
    return REF_MAYBE_CONST;
  }

  return VALUE; // including IN at least
}

// leaves outReturnKind unchanged if no return intent overload selection
// took place.
const MostSpecificCandidate*
determineBestReturnIntentOverload(const MostSpecificCandidates& candidates,
                                  Access access,
                                  QualifiedType::Kind& outReturnKind,
                                  bool& outAmbiguity) {
  outAmbiguity = false;
  const MostSpecificCandidate* best = nullptr;
  if (candidates.numBest() > 1) {
    auto& bestRef = candidates.bestRef();
    auto& bestConstRef = candidates.bestConstRef();
    auto& bestValue = candidates.bestValue();
    if (access == REF) {
      if (bestRef) best = &bestRef;
      else if (bestConstRef) best = &bestConstRef;
      else best = &bestValue;
    } else if (access == CONST_REF) {
      if (bestConstRef) best = &bestConstRef;
      else if (bestValue) best = &bestValue;
      else best = &bestRef;
    } else if (access == REF_MAYBE_CONST) {
      // raise an error
      outAmbiguity = true;
      if (bestConstRef) best = &bestConstRef;
      else if (bestValue) best = &bestValue;
      else best = &bestRef;
    } else { // access == VALUE
      if (bestValue) best = &bestValue;
      else if (bestConstRef) best = &bestConstRef;
      else best = &bestRef;
    }

    // set the determined return intent.
    if (best == &bestRef) outReturnKind = QualifiedType::REF;
    else if (best == &bestConstRef) outReturnKind = QualifiedType::CONST_REF;
    else if (best == &bestValue) outReturnKind = QualifiedType::CONST_VAR;

  } else if (candidates.numBest() == 1) {
    best = &candidates.only();
  }
  return best;
}

bool const& noteErrorMessage(Context* context, UniqueString message) {
  QUERY_BEGIN(noteErrorMessage, context, message);
  return QUERY_END(true);
}

bool const& noteWarningMessage(Context* context, UniqueString message) {
  QUERY_BEGIN(noteWarningMessage, context, message);
  return QUERY_END(true);
}


} // end namespace resolution
} // end namespace chpl
