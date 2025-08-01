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

#include "default-functions.h"

#include "chpl/framework/global-strings.h"
#include "chpl/framework/query-impl.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/ResolutionContext.h"
#include "chpl/resolution/can-pass.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/AstTag.h"
#include "chpl/uast/all-uast.h"

#include "Resolver.h"
#include "./OverloadSelector.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

static bool isDeSerializeMethod(UniqueString name) {
  return name == USTR("serialize") || name == USTR("deserialize");
}

/**
  Return true if 'name' is the name of a compiler generated method.
*/
static bool isNameOfCompilerGeneratedMethod(UniqueString name) {
  // TODO: Update me over time.
  if (name == USTR("init")       ||
      name == USTR("deinit")     ||
      name == USTR("init=")      ||
      name == USTR("hash")) {
    return true;
  }

  if (isDeSerializeMethod(name)) {
    return true;
  }

  return false;
}

// do not look outside the defining module
static const LookupConfig IN_SCOPE_CONFIG = LOOKUP_DECLS | LOOKUP_PARENTS | LOOKUP_METHODS;

static MatchingIdsWithName getMatchingIdsInInternalModule(Context* context,
                                                          const char* modName,
                                                          UniqueString name) {
  auto mod =
    parsing::getToplevelModule(context,
                               UniqueString::get(context, modName));

  if (!mod) return MatchingIdsWithName();

  return lookupNameInScope(context, scopeForModule(context, mod->id()),
                           /* methodLookupHelper */ nullptr,
                           /* receiverScopeHelper */ nullptr,
                           name, IN_SCOPE_CONFIG);
}

static MatchingIdsWithName getMatchingIdsInDefiningScope(Context* context,
                                                         const Type* type,
                                                         UniqueString name) {
  const Scope* scopeForReceiverType = nullptr;

  if (auto compType = type->getCompositeType()) {
    scopeForReceiverType = scopeForId(context, compType->id());
  } else if (auto enumType = type->toEnumType()) {
    scopeForReceiverType = scopeForId(context, enumType->id());
  } else if (auto externType = type->toExternType()) {
    scopeForReceiverType = scopeForId(context, externType->id());
  }

  // there is no defining scope
  if (!scopeForReceiverType) return {};

  auto ids = lookupNameInScope(context, scopeForReceiverType,
                               /* methodLookupHelper */ nullptr,
                               /* receiverScopeHelper */ nullptr,
                               name, IN_SCOPE_CONFIG);

  // this ought to be solved by interfaces, but today it isn't. As a workaround
  // for some standard types having their (de)serialize methods defined in
  // ChapelIO, search that module too. Same deal with hash/ChapelHashing.
  if (ids.numIds() == 0) {
    if (isDeSerializeMethod(name)) {
      ids = getMatchingIdsInInternalModule(context, "ChapelIO", name);
    } else if (name == USTR("hash")) {
      ids = getMatchingIdsInInternalModule(context, "ChapelHashing", name);
    }
  }

  return ids;
}

static bool
areFnOverloadsPresentInDefiningScope(Context* context,
                                     const Type* type,
                                     QualifiedType::Kind kind,
                                     UniqueString name) {

  auto ids = getMatchingIdsInDefiningScope(context, type, name);

  // nothing found
  if (ids.numIds() == 0) return false;

  auto haveQt = QualifiedType(kind, type);

  // loop through IDs and see if any are methods or operators (method or
  // standalone) on the same type
  for (const auto& id : ids) {
    // TODO: is idToAst appropriate here? Might be better to use
    // UntypedFnSignature / TypedFnSignature.
    auto node = parsing::idToAst(context, id);
    CHPL_ASSERT(node);

    if (auto fn = node->toFunction()) {
      if (fn->isMethod() || fn->kind() == Function::Kind::OPERATOR) {
        ResolutionResultByPostorderID r;
        ResolutionContext rcval(context);
        auto vis = Resolver::createForInitialSignature(&rcval, fn, r);
        // use receiver for method, first formal for standalone operator
        auto checkFormal =
            (fn->isMethod() ? fn->thisFormal() : fn->formal(0));
        checkFormal->traverse(vis);
        auto receiverQualType = vis.byPostorder.byAst(checkFormal).type();

        // Return true if:
        // * the receiver type matches
        // * the receiver type is a generic type and we have an instantiation
        // * the receiver type converts via implicit borrowing
        auto result = canPassScalar(context, haveQt, receiverQualType);
        if (result.passes() &&
            (!result.converts() || result.convertsWithBorrowing())) {
          return true;
        }
      }
    }
  }

  return false;
}

static bool
areOperatorOverloadsPresentInDefiningScope(Context* context,
                                           const QualifiedType& typeForScope,
                                           const QualifiedType& lhsType,
                                           const QualifiedType& rhsType,
                                           UniqueString name) {

  CHPL_ASSERT(lhsType.type());
  auto ids = getMatchingIdsInDefiningScope(context, typeForScope.type(), name);

  // nothing found
  if (ids.numIds() == 0) return false;

  for (int i = 0; i < ids.numIds(); i++) {
    auto& idAndFlags = ids.idAndFlags(i);

    // types are not operators
    if (idAndFlags.isType()) continue;

    // fields are not operators
    if (idAndFlags.isMethodOrField() && !idAndFlags.isMethod()) continue;

    // Actually simulate a call to the function to find generic but applicable
    // instantiations and evaluate where clauses.
    ResolutionContext rcval(context);
    auto sig = typedSignatureInitialForId(&rcval, idAndFlags.id());
    std::vector<CallInfoActual> actuals;
    actuals.push_back({ lhsType, UniqueString() });
    actuals.push_back({ rhsType, UniqueString() });
    auto ci = CallInfo(name,
                       QualifiedType(),
                       /* isMethodCall */ false,
                       /* hasQuestionArg */ false,
                       /* isParenless */ false,
                       std::move(actuals));
    CHPL_ASSERT(ci.isOpCall());

    // If the function doesn't apply, this isn't a relevant operator overload.
    auto faMap = FormalActualMap(sig, ci);
    if (!isInitialTypedSignatureApplicable(context, sig, faMap, ci).success()) {
      continue;
    }

    // If this function was generic, and we couldn't instantiate it (e.g.
    // due to a 'where' clause), it's not an applicable candidate.
    if (sig->needsInstantiation()) {
      // note: this is pretty much the body of doIsCandidateApplicableInstantiating
      // in resolution queries, but that function is private, and it seemed
      // not worth it to expose it in the header.

      auto result = instantiateSignature(&rcval, sig, ci, /* poiScope */ nullptr);

      // function didn't apply after instantiation
      if (!result.success()) continue;

      // 'where' clause evaluated to false, so this doesn't apply
      if (result.candidate()->whereClauseResult() == TypedFnSignature::WHERE_FALSE)
        continue;
    }

    // found a candidate that matches!
    return true;
  }

  return false;
}

template <typename F>
const TypedFnSignature* typedSignatureFromGenerator(ResolutionContext* rc, F&& generator, const ID& generateFor) {
  auto context = rc->context();
  auto& br = generator(context, generateFor);
  auto initFn = br.topLevelExpression(0)->toFunction();
  auto uSig = getUntypedFnSignatureForFn(context, initFn, &generateFor);
  return typedSignatureInitial(rc, uSig);
}

static const TypedFnSignature*
generateInitCopySignature(ResolutionContext* rc, const CompositeType* inCompType) {
  return typedSignatureFromGenerator(rc, buildInitEquals, inCompType->id());
}

static const TypedFnSignature*
generateRecordComparisonSignature(ResolutionContext* rc, const CompositeType* lhsType) {
  return typedSignatureFromGenerator(rc, buildRecordComparison, lhsType->id());
}

static const TypedFnSignature*
generateRecordInequalityComparisonSignature(ResolutionContext* rc,
                                            const CompositeType* lhsType) {
  return typedSignatureFromGenerator(rc, buildRecordInequalityComparison, lhsType->id());
}

static const TypedFnSignature*
generateRecordCompareLtSignature(ResolutionContext* rc, const CompositeType* lhsType) {
  return typedSignatureFromGenerator(rc, buildRecordCompareLt, lhsType->id());
}

static const TypedFnSignature*
generateRecordCompareLeSignature(ResolutionContext* rc, const CompositeType* lhsType) {
  return typedSignatureFromGenerator(rc, buildRecordCompareLe, lhsType->id());
}

static const TypedFnSignature*
generateRecordCompareGtSignature(ResolutionContext* rc, const CompositeType* lhsType) {
  return typedSignatureFromGenerator(rc, buildRecordCompareGt, lhsType->id());
}

static const TypedFnSignature*
generateRecordCompareGeSignature(ResolutionContext* rc, const CompositeType* lhsType) {
  return typedSignatureFromGenerator(rc, buildRecordCompareGe, lhsType->id());
}

static const TypedFnSignature*
generateRecordAssignment(ResolutionContext* rc, const CompositeType* lhsType) {
  return typedSignatureFromGenerator(rc, buildRecordAssign, lhsType->id());
}

static const TypedFnSignature*
generateRecordHashSignature(ResolutionContext* rc, const CompositeType* lhsType) {
  return typedSignatureFromGenerator(rc, buildRecordHash, lhsType->id());
}

using EnumCastSelector = OverloadSelector<buildEnumToStringCastImpl, buildEnumToBytesCastImpl, buildStringToEnumCastImpl, buildBytesToEnumCastImpl>;

using CompositeGeneratorType = TypedFnSignature const* (*)(ResolutionContext*, const CompositeType*);
using EnumGeneratorType = TypedFnSignature const* (*)(ResolutionContext*, const EnumType*);

static CompositeGeneratorType generatorForCompilerGeneratedRecordOperator(UniqueString name) {
  if (name == USTR("==")) {
    return generateRecordComparisonSignature;
  } else if (name == USTR("!=")) {
    return generateRecordInequalityComparisonSignature;
  } else if (name == USTR("<")) {
    return generateRecordCompareLtSignature;
  } else if (name == USTR("<=")) {
    return generateRecordCompareLeSignature;
  } else if (name == USTR(">")) {
    return generateRecordCompareGtSignature;
  } else if (name == USTR(">=")) {
    return generateRecordCompareGeSignature;
  } else if (name == USTR("=")) {
    return generateRecordAssignment;
  }

  return nullptr;
}

// non-record types have == and = etc. implemented in the modules so we don't
// want to generate them
static bool isNameOfCompilerGeneratedRecordOperator(UniqueString name) {
  return generatorForCompilerGeneratedRecordOperator(name) != nullptr;
}

static const TypedFnSignature*
generateEnumToStringCastSignature(ResolutionContext* rc, const EnumType* et) {
  auto fn = EnumCastSelector::invoke<buildEnumToStringCastImpl>;
  return typedSignatureFromGenerator(rc, fn, et->id());
}

static const TypedFnSignature*
generateEnumToBytesCastSignature(ResolutionContext* rc, const EnumType* et) {
  auto fn = EnumCastSelector::invoke<buildEnumToBytesCastImpl>;
  return typedSignatureFromGenerator(rc, fn, et->id());
}

static const TypedFnSignature*
generateStringToEnumCastSignature(ResolutionContext* rc, const EnumType* et) {
  auto fn = EnumCastSelector::invoke<buildStringToEnumCastImpl>;
  return typedSignatureFromGenerator(rc, fn, et->id());
}

static const TypedFnSignature*
generateBytesToEnumCastSignature(ResolutionContext* rc, const EnumType* et) {
  auto fn = EnumCastSelector::invoke<buildBytesToEnumCastImpl>;
  return typedSignatureFromGenerator(rc, fn, et->id());
}

static void
setupGeneratedEnumCastFormals(Context* context,
                              const EnumType* enumType,
                              std::vector<UntypedFnSignature::FormalDetail>& outUfsFormals,
                              std::vector<QualifiedType>& outFormalTypes,
                              bool isFromCast /* otherwise, it's a "to" cast */) {
  const Type* fromType;
  const Type* toType;

  if (isFromCast) {
    fromType = enumType;
    toType = AnyIntegralType::get(context);
  } else {
    fromType = AnyIntegralType::get(context);
    toType = enumType;
  }

  auto fromQt = QualifiedType(QualifiedType::DEFAULT_INTENT, fromType);
  auto toQt = QualifiedType(QualifiedType::TYPE, toType);

  auto ufsFrom =
      UntypedFnSignature::FormalDetail(UniqueString::get(context, "from"),
                                       UntypedFnSignature::DK_NO_DEFAULT, nullptr);
  outUfsFormals.push_back(std::move(ufsFrom));
  auto ufsTo =
      UntypedFnSignature::FormalDetail(UniqueString::get(context, "to"),
                                       UntypedFnSignature::DK_NO_DEFAULT, nullptr);
  outUfsFormals.push_back(std::move(ufsTo));

  outFormalTypes.push_back(fromQt);
  outFormalTypes.push_back(toQt);
}

template <bool isFromCast> /* otherwise, it's a "to" cast */
static const TypedFnSignature*
generateIntegralToOrFromCastForEnum(ResolutionContext* rc,
                                    const EnumType* enumType) {
  auto context = rc->context();

  std::vector<UntypedFnSignature::FormalDetail> ufsFormals;
  std::vector<QualifiedType> formalTypes;

  if (enumType->isAbstract()) return nullptr;

  setupGeneratedEnumCastFormals(context, enumType, ufsFormals, formalTypes,
                                isFromCast);

  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ enumType->id(),
                        /*name*/ USTR(":"),
                        /*isMethod*/ false,
                        /*isTypeConstructor*/ false,
                        /*isCompilerGenerated*/ true,
                        /*throws*/ false,
                        /*idTag*/ parsing::idToTag(context, enumType->id()),
                        /*kind*/ uast::Function::Kind::OPERATOR,
                        /*formals*/ std::move(ufsFormals),
                        /*whereClause*/ nullptr);

  auto ret = TypedFnSignature::get(context,
                                   ufs,
                                   std::move(formalTypes),
                                   TypedFnSignature::WHERE_NONE,
                                   /* needsInstantiation */ true,
                                   /* instantiatedFrom */ nullptr,
                                   /* parentFn */ nullptr,
                                   /* formalsInstantiated */ Bitmap(),
                                   /* outerVariables */ {});
  return ret;
}


static EnumGeneratorType generatorForCompilerGeneratedEnumOperator(UniqueString name, const QualifiedType& lhs, const QualifiedType& rhs, const EnumType*& outEt) {
  auto lhsT = lhs.type();
  auto rhsT = rhs.type();
  CHPL_ASSERT(lhsT && rhsT);

  if (name == USTR(":")) {
    if ((outEt = lhsT->toEnumType())) {
      if (rhsT->isStringType()) return generateEnumToStringCastSignature;
      if (rhsT->isBytesType()) return generateEnumToBytesCastSignature;
      if (rhsT->isIntegralType()) return generateIntegralToOrFromCastForEnum</* isFromCast */ true>;
    } else if ((outEt = rhsT->toEnumType())) {
      if (lhsT->isIntegralType()) return generateIntegralToOrFromCastForEnum</* isFromCast */ false>;
      if (lhsT->isStringType()) return generateStringToEnumCastSignature;
      if (lhsT->isBytesType()) return generateBytesToEnumCastSignature;
    }
  }
  return nullptr;
}

static bool isCompilerGeneratedEnumOperator(UniqueString name, const QualifiedType& lhs, const QualifiedType& rhs, QualifiedType& outTypeForScope) {
  const EnumType* et = nullptr;
  if (generatorForCompilerGeneratedEnumOperator(name, lhs, rhs, et) != nullptr) {
    outTypeForScope = et == lhs.type() ? lhs : rhs;
    return true;
  }
  return false;
}

bool
needCompilerGeneratedMethod(Context* context, const Type* type,
                            UniqueString name, bool parenless) {
  if (type == nullptr) return false;

  if (type->isNothingType()) return false;

  bool isAggregate = type->getCompositeType() || type->isRecordLike();
  if ((isAggregate && isNameOfCompilerGeneratedMethod(name))) {
    bool userDefinedExists =
      areFnOverloadsPresentInDefiningScope(context, type, QualifiedType::INIT_RECEIVER, name);

    if (name == USTR("hash")) {
      if (parenless) return false;

      auto qt = QualifiedType(QualifiedType::CONST_VAR, type);
      userDefinedExists |=
        areOperatorOverloadsPresentInDefiningScope(context, /* typeForScope */ qt, qt, qt, USTR("==")) ||
        areOperatorOverloadsPresentInDefiningScope(context, /* typeForScope */ qt, qt, qt, USTR("!="));
    }

    if (!userDefinedExists) {
      return true;
    }
  }

  if (type->isPtrType()) {
    if (name == "eltType") {
      return true;
    } else if (type->isHeapBufferType() && name == "this") {
      return true;
    }
  } else if (type->isEnumType()) {
    if (name == "size") {
      return true;
    }
  } else if (type->isIteratorType()) {
    if (name == "_shape_") {
      return true;
    }
  }

  return false;
}

bool needCompilerGeneratedBinaryOp(Context* context,
                                   const types::QualifiedType& lhs,
                                   const types::QualifiedType& rhs,
                                   UniqueString name) {
  auto lhsT = lhs.type();
  auto rhsT = rhs.type();

  if (lhsT == nullptr || rhsT == nullptr) return false;

  if (lhsT->isNothingType() || rhsT->isNothingType()) return false;

  if (lhsT->isExternType() && name == USTR("=")) {
    // use the simpler mechanism for extern operators
    // TODO: should we promote these areOperatorOverloads...?
    if (!areFnOverloadsPresentInDefiningScope(context, lhs.type(), QualifiedType::INIT_RECEIVER, name)) {
      return true;
    }
  }

  auto lhsRec = lhsT->toRecordType(),
       rhsRec = rhsT->toRecordType();
  if (lhsRec && rhsRec && isNameOfCompilerGeneratedRecordOperator(name)) {
    // record operators are only generated for same-record pairs
    // (e.g., R1 == R2, not R1 == R2 where R1 != R2). Moreover, other overloads
    // (like R1 == R2) do not affect the auto-generation of default comparison.

    if (lhsRec != rhsRec) return false;

    if (!areOperatorOverloadsPresentInDefiningScope(context, /* typeForScope */ lhs, lhs, rhs, name)) {
      return true;
    }
  }

  QualifiedType enumType;
  if (isCompilerGeneratedEnumOperator(name, lhs, rhs, enumType)) {
    if (!areOperatorOverloadsPresentInDefiningScope(context, /* typeForScope */ enumType, lhs, rhs, name)) {
      return true;
    }
  }

  return false;
}

static bool initHelper(Context* context,
                       Builder* builder,
                       const AggregateDecl* typeDecl,
                       const Location& dummyLoc,
                       AstList& formals, AstList& superArgs, AstList& stmts) {
  // Return 'true' if a super.init call is necessary
  bool addSuperInit = false;

  // Check if we need a super.init() call. If the parent has a default
  // initializer, add arguments to the super.init() and formals to the
  // current initializer.
  if (auto cls = typeDecl->toClass()) {
    auto t = initialTypeForTypeDecl(context, cls->id());
    auto bct = t->getCompositeType()->toBasicClassType();
    auto pct = bct->parentClassType();

    if (pct && !pct->isObjectType()) {
      addSuperInit = true;

      const Type* manager = nullptr;
      auto borrowedNonnilDecor =
          ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
      auto parentReceiver =
        ClassType::get(context, pct->toBasicClassType(), manager, borrowedNonnilDecor);
      auto userDefinedExists = areFnOverloadsPresentInDefiningScope(context,
                                 parentReceiver,
                                 QualifiedType::INIT_RECEIVER,
                                 USTR("init"));

      if (!userDefinedExists) {
        auto& br = buildInitializer(context, pct->id());
        auto fn = br.topLevelExpression(0)->toFunction();

        // Add formals and super.init() arguments
        for (auto formal : fn->formals()) {
          if (auto named = formal->toNamedDecl();
              named && named->name() != USTR("this")) {
            formals.push_back(formal->copy());

            owned<AstNode> arg = Identifier::build(builder, dummyLoc, named->name());
            superArgs.push_back(std::move(arg));
          }
        }
      }
    }
  }

  // Note: Using lambdas here to avoid threading several arguments
  auto addFormal = [&](const VarLikeDecl* field,
                       const AstNode* typeExpr,
                       const AstNode* initExpr) -> void {
    Formal::Intent kind;
    // for types & param, use the field kind, for values use 'in' intent
    if (field->storageKind() == Qualifier::TYPE ||
        field->storageKind() == Qualifier::PARAM) {
      kind = (Formal::Intent)field->storageKind();
    } else {
      kind = Formal::Intent::IN;
    }

    owned<AstNode> formal = Formal::build(builder, dummyLoc,
                                          /*attributeGroup=*/nullptr,
                                          field->name(), kind,
                                          typeExpr ? typeExpr->copy() : nullptr,
                                          initExpr ? initExpr->copy() : nullptr);

    // Create 'this.field = arg;' statement
    owned<AstNode> lhs = Dot::build(builder, dummyLoc,
                                    Identifier::build(builder, dummyLoc, USTR("this")),
                                    field->name());
    owned<AstNode> rhs = Identifier::build(builder, dummyLoc, field->name());
    owned<AstNode> assign = OpCall::build(builder, dummyLoc, USTR("="),
                                          std::move(lhs), std::move(rhs));
    stmts.push_back(std::move(assign));

    formals.push_back(std::move(formal));
  };

  std::function<void (const AstNode*)> processDecl = [&addFormal, &processDecl](auto ast) {
    if (auto var = ast->toVarLikeDecl()) {
      addFormal(var, var->typeExpression(), var->initExpression());
    } else if (auto multi = ast->toMultiDecl()) {
      auto it = multi->decls().begin();
      auto groupBegin = it;
      const AstNode* curTypeExpr = nullptr;
      const AstNode* curInitExpr = nullptr;

      // Iterate over decls until we find one with either a type-expression or
      // init-expression. Once found, the inner loop propagates the
      // type/init-expressions to previous decls without such expressions.
      //
      // For example, in a multi-decl like this:
      //
      //   var a, b: string, c, d = 0;
      //
      // The outer loop would do nothing for variables, 'a' and 'c'. Then,
      // when 'b' and 'd' are examined, their expressions will be propagated
      // back to 'a' and 'c' during the inner loop.
      while (it != multi->decls().end()) {
        const VarLikeDecl* curDecl = (*it)->toVarLikeDecl();

        // TupleDecls currently not supported as fields
        CHPL_ASSERT(curDecl != nullptr);

        curTypeExpr = curDecl->typeExpression();
        curInitExpr = curDecl->initExpression();

        // Now, propagate to the previous decls forming the current 'group'.
        // We process this group in a 'forward' direction, rather than a
        // 'backward' direction, so that we preserve field declaration order.
        if (curTypeExpr || curInitExpr) {
          auto groupEnd = std::next(it);
          auto groupIt = groupBegin;
          while (groupIt != groupEnd) {
            const VarLikeDecl* d = (*groupIt)->toVarLikeDecl();
            addFormal(d, curTypeExpr, curInitExpr);
            groupIt++;
          }

          groupBegin = groupEnd;
        }

        ++it;
      }
    } else if (auto tup = ast->toTupleDecl()) {
      CHPL_ASSERT(false && "tuple decls cannot currently be used as fields");
    } else if (auto fwd = ast->toForwardingDecl()) {
      if (fwd->isDecl()) {
        processDecl(fwd->expr());
      }
    }
  };

  for (auto d : typeDecl->decls()) {
    processDecl(d);
  }

  return addSuperInit;
}

const BuilderResult& buildInitializer(Context* context, ID typeID) {
  QUERY_BEGIN(buildInitializer, context, typeID);

  auto typeDecl = parsing::idToAst(context, typeID)->toAggregateDecl();
  auto bld = Builder::createForGeneratedCode(context, typeID);
  auto builder = bld.get();
  auto dummyLoc = parsing::locateId(context, typeID);

  auto thisFormal = Formal::build(builder, dummyLoc, nullptr,
                                  USTR("this"), Formal::DEFAULT_INTENT,
                                  nullptr, nullptr);

  AstList formals;
  AstList stmts;
  AstList superArgs;
  bool addSuperInit = initHelper(context, builder, typeDecl, dummyLoc,
                                 formals, superArgs, stmts);

  if (addSuperInit) {
    owned<AstNode> dot = Dot::build(builder, dummyLoc,
                                    Identifier::build(builder, dummyLoc,
                                                      USTR("super")),
                                    USTR("init"));
    owned<AstNode> call = FnCall::build(builder, dummyLoc,
                                        std::move(dot), std::move(superArgs),
                                        /*callUsedSquareBrackets*/false);
    stmts.insert(stmts.begin(), std::move(call));
  }

  auto body = Block::build(builder, dummyLoc, std::move(stmts));
  auto genFn = Function::build(builder,
                               dummyLoc, {},
                               Decl::Visibility::PUBLIC,
                               Decl::Linkage::DEFAULT_LINKAGE,
                               /*linkageName=*/{},
                               USTR("init"),
                               /*inline=*/false, /*override=*/false,
                               Function::Kind::PROC,
                               /*receiver=*/std::move(thisFormal),
                               Function::ReturnIntent::DEFAULT_RETURN_INTENT,
                               // throws, primaryMethod, parenless
                               false, true, false,
                               std::move(formals),
                               // returnType, where, lifetime, body
                               {}, {}, {}, std::move(body));

  builder->noteChildrenLocations(genFn.get(), dummyLoc);
  builder->addToplevelExpression(std::move(genFn));

  auto result = builder->result();
  return QUERY_END(result);
}

static std::vector<UntypedFnSignature::FormalDetail>
buildInitUfsFormals(const uast::Function* initFn) {
  // compute the FormalDetails manually so that we can set the default-kind
  // appropriately.
  //
  // TODO: give the Formals proper init-expressions so that we can rely on
  // pre-existing code to create the UntypedFnSignature.
  std::vector<UntypedFnSignature::FormalDetail> formals;
  for (auto decl : initFn->formals()) {
    UniqueString name;
    bool hasDefault = false;

    if (auto formal = decl->toFormal()) {
      name = formal->name();

      hasDefault = formal->initExpression() != nullptr;
      if (decl != initFn->thisFormal()) {
        if (formal->intent() != Formal::Intent::TYPE &&
            formal->intent() != Formal::Intent::PARAM) {
          if (formal->typeExpression() != nullptr) {
            hasDefault = true;
          }
        }
      }
    }

    auto defaultKind = hasDefault ? UntypedFnSignature::DK_DEFAULT
                                  : UntypedFnSignature::DK_NO_DEFAULT;
    auto fd = UntypedFnSignature::FormalDetail(name, defaultKind,
                                               decl, decl->isVarArgFormal());
    formals.push_back(fd);
  }

  return formals;
}

static const TypedFnSignature*
generateInitSignature(ResolutionContext* rc, const CompositeType* inCompType) {
  auto context = rc->context();
  if (auto ct = inCompType->getCompositeType()->toBasicClassType()) {
    if (ct->isObjectType()) {
      return nullptr;
    }
  }

  auto& br = buildInitializer(context, inCompType->id());

  auto initFn = br.topLevelExpression(0)->toFunction();
  auto formals = buildInitUfsFormals(initFn);

  // find the unique-ified untyped signature
  auto uSig = UntypedFnSignature::get(context, initFn->id(), initFn->name(),
                                   true,
                                   /* isTypeConstructor */ false,
                                   /* isCompilerGenerated */ true,
                                   /* throws */ false,
                                   /* idTag */ asttags::Function,
                                   uast::Function::Kind::PROC,
                                   std::move(formals), nullptr,
                                   inCompType->id());

  return typedSignatureInitial(rc, uSig);
}

struct FnBuilder {
  Context* context_;
  ID typeID_;
  const TypeDecl* typeDecl_;
  UniqueString name_;
  Function::Kind kind_;
  bool throws_;
  bool inline_ = false;

  owned<Builder> builder_;
  Location dummyLoc_;

  AstList stmts_;

  FnBuilder(Context* context, ID typeID, UniqueString name,
           Function::Kind kind, bool throws = false,
           optional<int> overloadOffset = empty)
    : context_(context), typeID_(std::move(typeID)), name_(name), kind_(kind),
      throws_(throws) {
    builder_ = Builder::createForGeneratedCode(context_, typeID_, overloadOffset);
    dummyLoc_ = parsing::locateId(context_, typeID_);

    typeDecl_ = parsing::idToAst(context, typeID_)->toTypeDecl();
    CHPL_ASSERT(typeDecl_);
  }

  Context* context() const { return context_; }

  Builder* builder() { return builder_.get(); }

  AstList& stmts() { return stmts_; }

  owned<StringLiteral> stringLit(const char* str) {
    return StringLiteral::build(builder(), dummyLoc_, str, StringLikeLiteral::QuoteStyle::DOUBLE);
  }

  owned<BytesLiteral> bytesLit(const char* str) {
    return BytesLiteral::build(builder(), dummyLoc_, str, StringLikeLiteral::QuoteStyle::DOUBLE);
  }

  owned<BoolLiteral> boolLit(bool value) {
    return BoolLiteral::build(builder(), dummyLoc_, value);
  }

  owned<IntLiteral> intLit(int64_t value) {
    return IntLiteral::build(builder(), dummyLoc_, value, UniqueString::get(context_, std::to_string(value)));
  }

  owned<Identifier> identifier(UniqueString name) {
    return Identifier::build(builder(), dummyLoc_, name);
  }

  owned<Dot> dot(owned<AstNode> lhs, UniqueString name) {
    return Dot::build(builder(), dummyLoc_, std::move(lhs), name);
  }

  owned<OpCall> op(owned<AstNode> lhs, UniqueString op, owned<AstNode> rhs) {
    return OpCall::build(builder(), dummyLoc_, op,
                         std::move(lhs), std::move(rhs));
  }

  template <typename ... Ts>
  owned<FnCall> call(owned<AstNode> calledExpr, Ts&& ... varArgs) {
    AstList args;
    (args.push_back(std::move(varArgs)),...);
    return FnCall::build(builder(), dummyLoc_, std::move(calledExpr), std::move(args), /* callUsedSquareBrackets */ false);
  }

  template <typename ... Ts>
  owned<FnCall> call(UniqueString fn, Ts&& ... varArgs) {
    return call(identifier(fn), std::forward<Ts>(varArgs)...);
  }

  owned<Return> ret(owned<AstNode> result) {
    return Return::build(builder(), dummyLoc_, std::move(result));
  }

  owned<AstNode> conditional(owned<AstNode> condition,
                             AstList thenStmts,
                             optional<AstList> elseStmts = empty) {
    auto thenBlock = Block::build(builder(), dummyLoc_, std::move(thenStmts));
    auto elseBlock = elseStmts ? Block::build(builder(), dummyLoc_, std::move(*elseStmts)) : nullptr;
    auto cond = Conditional::build(builder(), dummyLoc_,
                                   std::move(condition),
                                   BlockStyle::IMPLICIT,
                                   std::move(thenBlock),
                                   BlockStyle::IMPLICIT,
                                   std::move(elseBlock),
                                   /* isExpressionLevel */ false);
    return cond;
  }

  owned<AstNode> earlyReturn(owned<AstNode> condition, owned<AstNode> result) {
    auto retStmt = ret(std::move(result));
    AstList stmts;
    stmts.push_back(std::move(retStmt));
    auto cond = Conditional::build(builder(), dummyLoc_,
                                   std::move(condition),
                                   BlockStyle::IMPLICIT,
                                   Block::build(builder(), dummyLoc_, std::move(stmts)),
                                   /* isExpressionLevel */ false);
    return cond;
  }

  template <typename F>
  void eachField(F&& callable) {
    auto ct = initialTypeForTypeDecl(context_, typeID_)->getCompositeType();

    // attempt to resolve the fields
    auto defaultsPolicy = DefaultsPolicy::IGNORE_DEFAULTS;
    auto rc = createDummyRC(context_);
    auto& rf = fieldsForTypeDecl(&rc, ct,
                                                 defaultsPolicy,
                                                 /* syntaxOnly */ true);
    for (int i = 0; i < rf.numFields(); i++) {
      auto fieldDecl = parsing::idToAst(context_, rf.fieldDeclId(i));
      CHPL_ASSERT(fieldDecl && fieldDecl->isVariable());
      callable(fieldDecl->toVariable());
    }
  }

  BuilderResult finalize(owned<Formal> receiver,
                         AstList otherFormals,
                         owned<Block> body) {
    auto genFn = Function::build(builder(),
                                 dummyLoc_, {},
                                 Decl::Visibility::PUBLIC,
                                 Decl::Linkage::DEFAULT_LINKAGE,
                                 /*linkageName=*/{},
                                 name_,
                                 inline_, /*override=*/false,
                                 kind_,
                                 /*receiver=*/std::move(receiver),
                                 Function::ReturnIntent::DEFAULT_RETURN_INTENT,
                                 // throws, primaryMethod, parenless
                                 throws_, false, false,
                                 std::move(otherFormals),
                                 // returnType, where, lifetime, body
                                 {}, {}, {}, std::move(body));

    builder_->noteChildrenLocations(genFn.get(), dummyLoc_);
    builder_->addToplevelExpression(std::move(genFn));

    return builder_->result();
  }
};

struct BinaryFnBuilder : public FnBuilder {
  Formal::Intent lhsIntent_;
  Formal::Intent rhsIntent_;

  owned<Formal> lhsFormal_;
  owned<Formal> rhsFormal_;

  BinaryFnBuilder(Context* context, ID typeID, UniqueString name,
                 Function::Kind kind,
                 Formal::Intent lhsIntent = Formal::DEFAULT_INTENT,
                 Formal::Intent rhsIntent = Formal::DEFAULT_INTENT,
                 bool throws = false,
                 optional<int> overloadOffset = empty)
    : FnBuilder(context, std::move(typeID), name, kind, throws, std::move(overloadOffset)),
      lhsIntent_(lhsIntent), rhsIntent_(rhsIntent) {
  }

  virtual owned<AstNode> lhsFormalTypeExpr() = 0;
  owned<Formal>& lhsFormal() {
    if (lhsFormal_) return lhsFormal_;

    // As a special rule in Chapel, operator declarations inside a type ignore
    // the 'this' formal, and instead have an 'lhs' and 'rhs' formal. Handle
    // that here by constructing an LHS formal that's not called 'this'. When
    // finalizing, we'll insert a dummy 'this' formal.
    UniqueString nameToUse = USTR("this");
    if (kind_ == Function::Kind::OPERATOR) {
      nameToUse = UniqueString::get(context_, "lhs");
    }
    lhsFormal_ = Formal::build(builder(), dummyLoc_, nullptr,
                               nameToUse, lhsIntent_,
                               lhsFormalTypeExpr(), nullptr);
    return lhsFormal_;
  }

  virtual owned<AstNode> rhsFormalTypeExpr() = 0;
  owned<Formal>& rhsFormal() {
    if (rhsFormal_) return rhsFormal_;


    auto otherName = UniqueString::get(context_, "rhs");
    rhsFormal_ = Formal::build(builder(), dummyLoc_, nullptr,
                                otherName, rhsIntent_,
                                rhsFormalTypeExpr(), nullptr);
    return rhsFormal_;
  }

  BuilderResult finalize() {
    auto body = Block::build(builder(), dummyLoc_, std::move(stmts_));
    AstList otherFormals;

    // See comment in constructor. For operators, the left hand side formal is not really
    // "this", and "this" is a dummy formal. Insert the "lhs" formal as a normal
    // formal, and create the dummy.
    if (kind_ == Function::Kind::OPERATOR) {
      otherFormals.push_back(std::move(lhsFormal()));
      lhsFormal_ = Formal::build(builder(), dummyLoc_, nullptr,
                                      USTR("this"), Formal::DEFAULT_INTENT,
                                      identifier(typeID_.symbolName(context_)), nullptr);
    }
    otherFormals.push_back(std::move(rhsFormal()));
    return FnBuilder::finalize(std::move(lhsFormal()), std::move(otherFormals), std::move(body));
  }
};

struct FieldFnBuilder : BinaryFnBuilder {
  FieldFnBuilder(Context* context, ID typeID, UniqueString name,
                 Function::Kind kind,
                 Formal::Intent lhsIntent = Formal::DEFAULT_INTENT,
                 Formal::Intent rhsIntent = Formal::DEFAULT_INTENT)
    : BinaryFnBuilder(context, typeID, name, kind, lhsIntent, rhsIntent) {}

  owned<AstNode> lhsFormalTypeExpr() override {
    // If it's a generic type with defaults, we use the type name with a '?'.
    // Otherwise, typedSignatureInitial will use the defaulted type.
    if (auto agg = typeDecl_->toAggregateDecl()) {
      bool hasGenericDefaults = false;
      for (auto decl : agg->decls()) {
        if (auto var = decl->toVarLikeDecl()) {
          if (var->storageKind() == Qualifier::TYPE ||
              var->storageKind() == Qualifier::PARAM) {
            if (var->initExpression() != nullptr) {
              hasGenericDefaults = true;
              break;
            }
          }
        }
      }

      if (hasGenericDefaults) {
        return call(agg->name(), identifier(USTR("?")));
      } else {
        return identifier(agg->name());
      }
    } else {
      return identifier(typeDecl_->name());
    }
  }

  owned<AstNode> rhsFormalTypeExpr() override {
    auto thisDotType = dot(identifier(USTR("this")), USTR("type"));
    return thisDotType;
  }

  owned<Dot> lhsField(const VarLikeDecl* fieldDecl) {
    return dot(identifier(lhsFormal()->name()), fieldDecl->name());
  }

  owned<Dot> rhsField(const VarLikeDecl* fieldDecl) {
    return dot(identifier(rhsFormal()->name()), fieldDecl->name());
  }
};

const BuilderResult& buildInitEquals(Context* context, ID typeID) {
  QUERY_BEGIN(buildInitEquals, context, typeID);

  FieldFnBuilder bld(context, typeID, USTR("init="), Function::Kind::PROC);

  bld.eachField([&bld](const Variable* decl) {
    bld.stmts().push_back(bld.op(bld.lhsField(decl),
                                 USTR("="),
                                 bld.rhsField(decl)));
  });

  auto result = bld.finalize();
  return QUERY_END(result);
}

const BuilderResult& buildRecordComparison(Context* context, ID typeID) {
  QUERY_BEGIN(buildRecordComparison, context, typeID);

  FieldFnBuilder bld(context, typeID, USTR("=="), Function::Kind::OPERATOR,
                     Formal::CONST_REF,
                     Formal::CONST_REF);

  bld.eachField([&bld](const Variable* decl) {
    if (decl->kind() == Variable::TYPE) return;

    auto neqCall =
      bld.call(UniqueString::get(bld.context(), "chpl_field_neq"),
               bld.lhsField(decl), bld.rhsField(decl));
    bld.stmts().push_back(bld.earlyReturn(std::move(neqCall), bld.boolLit(false)));
  });

  bld.stmts().push_back(bld.ret(bld.boolLit(true)));

  auto result = bld.finalize();
  return QUERY_END(result);
}

const BuilderResult& buildRecordInequalityComparison(Context* context, ID typeID) {
  QUERY_BEGIN(buildRecordInequalityComparison, context, typeID);

  FieldFnBuilder bld(context, typeID, USTR("!="), Function::Kind::OPERATOR,
                     Formal::CONST_REF,
                     Formal::CONST_REF);

  bld.eachField([&bld](const Variable* decl) {
    if (decl->kind() == Variable::TYPE) return;

    bld.stmts().push_back(
        bld.call(UniqueString::get(bld.context(), "chpl_field_neq"),
                 bld.lhsField(decl), bld.rhsField(decl)));
  });

  owned<AstNode> wholeNeq = bld.boolLit(false);
  for (auto it = bld.stmts().rbegin(); it != bld.stmts().rend(); ++it) {
    wholeNeq = bld.op(std::move(wholeNeq), USTR("||"), std::move(*it));
  }

  bld.stmts().clear();
  bld.stmts().push_back(bld.ret(std::move(wholeNeq)));

  auto result = bld.finalize();
  return QUERY_END(result);
}

static const BuilderResult buildOrderedComparison(Context* context, ID typeID,
                                                  UniqueString fnName,
                                                  UniqueString strictCompFn,
                                                  UniqueString strictCompReversedFn,
                                                  bool allowEqual) {

  FieldFnBuilder bld(context, typeID, fnName, Function::Kind::OPERATOR,
                     Formal::CONST_REF,
                     Formal::CONST_REF);

  // the general pattern is:
  // if (x[0] < y[0]) return true;
  // if (x[0] > y[0]) return false;
  //
  // otherwise, first field is equal, so move on to second field.
  // This implements dictionary ordering.

  bld.eachField([&, strictCompFn](const Variable* decl) {
    if (decl->kind() == Variable::TYPE) return;

    bld.stmts().push_back(
      bld.earlyReturn(bld.call(strictCompFn,
                               bld.lhsField(decl),
                               bld.rhsField(decl)),
                      bld.boolLit(true))
    );
    bld.stmts().push_back(
      bld.earlyReturn(bld.call(strictCompReversedFn,
                               bld.lhsField(decl),
                               bld.rhsField(decl)),
                      bld.boolLit(false))
    );
  });

  // if we get here, all fields are equal. If allowEqual is true, return
  // true, otherwise return false.
  bld.stmts().push_back(bld.ret(bld.boolLit(allowEqual)));

  return bld.finalize();
}

const uast::BuilderResult& buildRecordCompareLt(Context* context, ID typeID) {
  QUERY_BEGIN(buildRecordCompareLt, context, typeID);
  auto result = buildOrderedComparison(context, typeID, USTR("<"),
                                       UniqueString::get(context, "chpl_field_lt"),
                                       UniqueString::get(context, "chpl_field_gt"),
                                       false);
  return QUERY_END(result);
}

const uast::BuilderResult& buildRecordCompareLe(Context* context, ID typeID) {
  QUERY_BEGIN(buildRecordCompareLe, context, typeID);
  auto result = buildOrderedComparison(context, typeID, USTR("<="),
                                       UniqueString::get(context, "chpl_field_lt"),
                                       UniqueString::get(context, "chpl_field_gt"),
                                       true);
  return QUERY_END(result);
}

const uast::BuilderResult& buildRecordCompareGt(Context* context, ID typeID) {
  QUERY_BEGIN(buildRecordCompareGt, context, typeID);
  auto result = buildOrderedComparison(context, typeID, USTR(">"),
                                       UniqueString::get(context, "chpl_field_gt"),
                                       UniqueString::get(context, "chpl_field_lt"),
                                       false);
  return QUERY_END(result);
}

const uast::BuilderResult& buildRecordCompareGe(Context* context, ID typeID) {
  QUERY_BEGIN(buildRecordCompareGe, context, typeID);
  auto result = buildOrderedComparison(context, typeID, USTR(">="),
                                       UniqueString::get(context, "chpl_field_gt"),
                                       UniqueString::get(context, "chpl_field_lt"),
                                       true);
  return QUERY_END(result);
}

const BuilderResult& buildRecordAssign(Context* context, ID typeID) {
  QUERY_BEGIN(buildRecordAssign, context, typeID);

  FieldFnBuilder bld(context, typeID, USTR("="), Function::Kind::OPERATOR,
                     Formal::REF, Formal::CONST_REF);

  bld.eachField([&bld](const Variable* decl) {
    if (decl->kind() == Variable::TYPE ||
        decl->kind() == Variable::PARAM) return;

    bld.stmts().push_back(
        bld.op(bld.lhsField(decl), USTR("="), bld.rhsField(decl)));
  });

  auto result = bld.finalize();
  return QUERY_END(result);
}

struct HashFnBuilder : public FnBuilder {
  owned<Formal> thisFormal_;
  bool isInline_ = false;

  HashFnBuilder(Context* context, ID typeID)
    : FnBuilder(context, typeID, USTR("hash"), Function::PROC, false, chpl::empty) {
    thisFormal_ = Formal::build(builder(), dummyLoc_, nullptr,
                                USTR("this"), Formal::DEFAULT_INTENT,
                                nullptr, nullptr);
  }

  void accumulateHash() {
    owned<AstNode> hashExpr = nullptr;
    int fieldIdx = 1;
    eachField([&hashExpr, &fieldIdx, this](const Variable* decl) {
      if (decl->kind() == Variable::TYPE ||
          decl->kind() == Variable::PARAM) return;

      auto fieldAccess = this->dot(this->identifier(USTR("this")), decl->name());
      if (!hashExpr) {
        hashExpr = this->call(UniqueString::get(this->context(), "chpl__defaultHashWrapperInner"),
                              std::move(fieldAccess));
      } else {
        auto hashCall = this->call(this->dot(std::move(fieldAccess), USTR("hash")));
        hashExpr = this->call(UniqueString::get(this->context(), "chpl__defaultHashCombine"),
                              std::move(hashCall), std::move(hashExpr),
                              this->intLit(fieldIdx++));
      }
    });

    if (!hashExpr) {
      // no fields, use a constant hash value
      // Dyno has no "uint literals", so perform a cast to uint from int.
      hashExpr = this->op(this->intLit(0), USTR(":"), this->identifier(USTR("uint")));
      isInline_ = true;
    }

    stmts().push_back(this->ret(std::move(hashExpr)));
  }

  BuilderResult finalize() {
    return FnBuilder::finalize(std::move(thisFormal_),
                               AstList{},
                               Block::build(builder(), dummyLoc_, std::move(stmts_)));
  }
};

const uast::BuilderResult& buildRecordHash(Context* context, ID typeID) {
  QUERY_BEGIN(buildRecordHash, context, typeID);

  HashFnBuilder bld(context, typeID);
  bld.accumulateHash();
  auto result = bld.finalize();

  return QUERY_END(result);
}

const BuilderResult& buildDeinit(Context* context, ID typeID) {
  QUERY_BEGIN(buildDeinit, context, typeID);

  auto bld = Builder::createForGeneratedCode(context, typeID);
  auto builder = bld.get();
  auto dummyLoc = parsing::locateId(context, typeID);

  auto thisFormal = Formal::build(builder, dummyLoc, nullptr,
                                  USTR("this"), Formal::DEFAULT_INTENT,
                                  nullptr, nullptr);

  AstList formals;

  // Empty body. Easier to let other parts of resolution infer the contents.
  // TODO: Insert call to parent deinit
  AstList stmts;
  auto body = Block::build(builder, dummyLoc, std::move(stmts));

  auto genFn = Function::build(builder,
                               dummyLoc, {},
                               Decl::Visibility::PUBLIC,
                               Decl::Linkage::DEFAULT_LINKAGE,
                               /*linkageName=*/{},
                               USTR("deinit"),
                               /*inline=*/false, /*override=*/false,
                               Function::Kind::PROC,
                               /*receiver=*/std::move(thisFormal),
                               Function::ReturnIntent::DEFAULT_RETURN_INTENT,
                               // throws, primaryMethod, parenless
                               false, true, false,
                               std::move(formals),
                               // returnType, where, lifetime, body
                               {}, {}, {}, std::move(body));

  builder->noteChildrenLocations(genFn.get(), dummyLoc);
  builder->addToplevelExpression(std::move(genFn));

  auto result = builder->result();
  return QUERY_END(result);
}

static const TypedFnSignature*
generateDeinitSignature(ResolutionContext* rc, const CompositeType* inCompType) {
  auto context = rc->context();
  std::vector<UntypedFnSignature::FormalDetail> ufsFormals;
  auto& br = buildDeinit(context, inCompType->id());
  auto deinitFn = br.topLevelExpression(0)->toFunction();

  auto fd = UntypedFnSignature::FormalDetail(USTR("this"),
                                           UntypedFnSignature::DK_NO_DEFAULT,
                                           deinitFn->thisFormal(), false);
  ufsFormals.push_back(fd);

  // build the untyped signature
  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ deinitFn->id(),
                        /*name*/ USTR("deinit"),
                        /*isMethod*/ true,
                        /*isTypeConstructor*/ false,
                        /*isCompilerGenerated*/ true,
                        /*throws*/ false,
                        /*idTag*/ asttags::Function,
                        /*kind*/ uast::Function::Kind::PROC,
                        /*formals*/ std::move(ufsFormals),
                        /*whereClause*/ nullptr);

  return typedSignatureInitial(rc, ufs);
}

const BuilderResult& buildDeSerialize(Context* context, ID typeID, bool isSerializer) {
  QUERY_BEGIN(buildDeSerialize, context, typeID, isSerializer);

  // TODO:
  // - use types for formals
  // - add calls to (de)serializeDefaultImpl

  auto bld = Builder::createForGeneratedCode(context, typeID);
  auto builder = bld.get();
  auto dummyLoc = parsing::locateId(context, typeID);

  auto thisType = Identifier::build(builder, dummyLoc, typeID.symbolName(context));
  auto thisFormal = Formal::build(builder, dummyLoc, nullptr,
                                  USTR("this"), Formal::DEFAULT_INTENT,
                                  nullptr, nullptr);

  auto writerArg = Formal::build(builder, dummyLoc, nullptr, UniqueString::get(context, "writer"),
                                 Formal::DEFAULT_INTENT, {}, nullptr);
  auto serializerArg = Formal::build(builder, dummyLoc, nullptr, UniqueString::get(context, "serializer"),
                                 Formal::DEFAULT_INTENT, {}, nullptr);
  AstList formals;
  formals.push_back(std::move(writerArg));
  formals.push_back(std::move(serializerArg));

  AstList stmts;
  auto body = Block::build(builder, dummyLoc, std::move(stmts));

  auto genFn = Function::build(builder,
                               dummyLoc, {},
                               Decl::Visibility::PUBLIC,
                               Decl::Linkage::DEFAULT_LINKAGE,
                               /*linkageName=*/{},
                               USTR("serialize"),
                               /*inline=*/false, /*override=*/false,
                               Function::Kind::PROC,
                               /*receiver=*/std::move(thisFormal),
                               Function::ReturnIntent::DEFAULT_RETURN_INTENT,
                               // throws, primaryMethod, parenless
                               true, true, false,
                               std::move(formals),
                               // returnType, where, lifetime, body
                               {}, {}, {}, std::move(body));

  builder->noteChildrenLocations(genFn.get(), dummyLoc);
  builder->addToplevelExpression(std::move(genFn));

  auto result = builder->result();
  return QUERY_END(result);
}

static void buildWhenStmts(Context* context, Builder* builder,
                           Location dummyLoc, const Enum* type,
                           AstList& whenStmts, bool enumToOrder) {
  int count = 0;
  for(auto elem : type->enumElements()) {
    owned<AstNode> elemIdent = Identifier::build(builder, dummyLoc,
                                                 elem->name());
    auto str = UniqueString::get(context, std::to_string(count));
    owned<AstNode> intLit = IntLiteral::build(builder, dummyLoc, count, str);

    AstList cases;
    cases.push_back(std::move(enumToOrder ? elemIdent : intLit));

    AstList stmtList;
    auto ret = Return::build(builder, dummyLoc,
                             std::move(enumToOrder ? intLit : elemIdent));
    stmtList.push_back(std::move(ret));

    auto when = When::build(builder, dummyLoc,
                            std::move(cases),
                            BlockStyle::IMPLICIT,
                            std::move(stmtList));
    whenStmts.push_back(std::move(when));

    count += 1;
  }

  {
    // TODO: Use PRIM_RT_ERROR for now to avoid AST-verification failure in
    // production, which complains about control-flow and return-symbol
    // initialization. A PRIM_RT_ERROR avoids that check.
    //
    // Eventually, this should turn into a 'halt'.
    auto prim = PrimCall::build(builder, dummyLoc,
                                uast::primtags::PRIM_RT_ERROR, {});

    AstList stmtList;
    stmtList.push_back(std::move(prim));

    auto when = When::build(builder, dummyLoc,
                            {}, BlockStyle::IMPLICIT, std::move(stmtList));
    whenStmts.push_back(std::move(when));
  }
}

const BuilderResult& buildEnumToOrder(Context* context, ID typeID) {
  QUERY_BEGIN(buildEnumToOrder, context, typeID);

  auto bld = Builder::createForGeneratedCode(context, typeID);
  auto builder = bld.get();
  auto dummyLoc = parsing::locateId(context, typeID);

  auto argType = Identifier::build(builder, dummyLoc, typeID.symbolName(context));
  auto typeIdent = argType->copy(); // make a copy for the 'use <type>' stmt
  auto argName = UniqueString::get(context, "e");
  auto argFormal = Formal::build(builder, dummyLoc, nullptr,
                                  argName, Formal::DEFAULT_INTENT,
                                  std::move(argType), nullptr);

  AstList formals;
  formals.push_back(std::move(argFormal));

  AstList stmts;
  auto type = parsing::idToAst(context, typeID)->toEnum();

  AstList useList;
  auto visClause = VisibilityClause::build(builder, dummyLoc,
                                           std::move(typeIdent));
  useList.push_back(std::move(visClause));

  // In production, the new function gets inserted into the ChapelBase module.
  // In dyno, we can't do this, since generated code has to be in a particular
  // location (at this time, logically "inside" the type declaration it's generated
  // for). To ensure we have the necessary code, `use` ChapelBase` in the body.
  auto baseIdent = Identifier::build(builder, dummyLoc,
                                     UniqueString::get(context, "ChapelBase"));
  auto baseVisClause = VisibilityClause::build(builder, dummyLoc,
                                               std::move(baseIdent));
  useList.push_back(std::move(baseVisClause));

  auto use = Use::build(builder, dummyLoc, Decl::Visibility::DEFAULT_VISIBILITY, std::move(useList));
  stmts.push_back(std::move(use));


  // build up when-stmts

  AstList whenStmts;
  buildWhenStmts(context, builder, dummyLoc,
                 type, whenStmts, /*enumToOrder=*/true);

  auto expr = Identifier::build(builder, dummyLoc, argName);
  auto select = Select::build(builder, dummyLoc, std::move(expr), std::move(whenStmts));
  stmts.push_back(std::move(select));

  auto body = Block::build(builder, dummyLoc, std::move(stmts));

  auto returnIntent = Function::ReturnIntent::DEFAULT_RETURN_INTENT;
  auto genFn = Function::build(builder,
                               dummyLoc, {},
                               Decl::Visibility::PUBLIC,
                               Decl::Linkage::DEFAULT_LINKAGE,
                               /*linkageName=*/{},
                               USTR("chpl__enumToOrder"),
                               /*inline=*/false, /*override=*/false,
                               Function::Kind::PROC,
                               /*receiver=*/nullptr,
                               returnIntent,
                               // throws, primaryMethod, parenless
                               false, false, false,
                               std::move(formals),
                               // returnType, where, lifetime, body
                               {}, {}, {}, std::move(body));

  builder->noteChildrenLocations(genFn.get(), dummyLoc);
  builder->addToplevelExpression(std::move(genFn));

  auto result = builder->result();
  return QUERY_END(result);
}

struct EnumCastBuilder : BinaryFnBuilder {
  UniqueString otherType_;
  bool castFromEnum_;

  // when clauses with which to populate into a Select statement.
  AstList selectWhens_;

  // what to return if none of the 'when' clauses matched
  owned<AstNode> fallback_;

  EnumCastBuilder(Context* context,
                  ID enumId,
                  UniqueString otherType,
                  bool castFromEnum,
                  int overloadIdx)
    : BinaryFnBuilder(context, enumId, USTR(":"),
                      Function::Kind::OPERATOR,
                      Formal::DEFAULT_INTENT,
                      Formal::TYPE,
                      /* throws */ false,
                      overloadIdx != 0 ? optional<int>(overloadIdx) : empty),
      otherType_(otherType),
      castFromEnum_(castFromEnum) {
  }

  owned<AstNode> lhsFormalTypeExpr() override {
    return identifier(castFromEnum_ ? typeDecl_->name() : otherType_);
  }

  owned<AstNode> rhsFormalTypeExpr() override {
    return identifier(castFromEnum_ ? otherType_ : typeDecl_->name());
  }
  owned<AstNode> enumElt(UniqueString name) {
    return dot(identifier(typeDecl_->name()), name);
  }

  void addWhenClause(owned<AstNode> whenValue,
                     owned<AstNode> toReturn) {
    AstList caseExprs;
    caseExprs.push_back(std::move(whenValue));
    AstList stmts;
    stmts.push_back(ret(std::move(toReturn)));
    auto whenClause = When::build(builder(), dummyLoc_,
                                  std::move(caseExprs),
                                  BlockStyle::IMPLICIT,
                                  std::move(stmts));
    selectWhens_.push_back(std::move(whenClause));
  }

  void setFallback(owned<AstNode> fallback) {
    fallback_ = std::move(fallback);
  }

  BuilderResult finalize() {
    auto select = Select::build(builder(), dummyLoc_,
                                identifier(lhsFormal()->name()),
                                std::move(selectWhens_));
    stmts().push_back(std::move(select));

    if (fallback_) {
      stmts().push_back(ret(std::move(fallback_)));
    } else {
      // inserting a throwing call to chpl_enum_cast_error, and then
      // return the first element.
      throws_ = true;
      CHPL_ASSERT(!castFromEnum_);
      stmts().push_back(call(UniqueString::get(context(), "chpl_enum_cast_error"),
                             identifier(lhsFormal()->name()),
                             stringLit(typeDecl_->name().c_str())));

      // return the first element
      auto ed = typeDecl_->toEnum();
      CHPL_ASSERT(ed && ed->numElements() > 0);
      stmts().push_back(ret(enumElt(ed->enumElements().begin()->name())));
    }

    return BinaryFnBuilder::finalize();
  }
};

static const uast::BuilderResult& buildEnumToStringOrBytesCastImpl(Context* context, ID typeID, bool isString, bool castFromEnum, int overloadIdx) {
  QUERY_BEGIN(buildEnumToStringOrBytesCastImpl, context, typeID, isString, castFromEnum, overloadIdx);

  EnumCastBuilder builder(context, typeID, isString ? USTR("string") : USTR("bytes"), castFromEnum, overloadIdx);
  auto buildLiteral = [&](UniqueString str) -> owned<AstNode> {
    if (isString) {
      return builder.stringLit(str.c_str());
    } else {
      return builder.bytesLit(str.c_str());
    }
  };

  auto enumDecl = parsing::idToAst(context, typeID)->toEnum();
  CHPL_ASSERT(enumDecl);
  for (auto elt : enumDecl->enumElements()) {
    if (castFromEnum) {
      builder.addWhenClause(builder.enumElt(elt->name()),
                            buildLiteral(elt->name()));
    } else {
      builder.addWhenClause(buildLiteral(elt->name()),
                            builder.enumElt(elt->name()));
      builder.addWhenClause(buildLiteral(UniqueString::getConcat(context, enumDecl->name().c_str(), ".", elt->name().c_str())),
                            builder.enumElt(elt->name()));
    }
  }
  // if we're casting from an enum, the cast should be exhaustive, but add
  // a dummy return just in case.
  if (castFromEnum) {
    builder.setFallback(buildLiteral(UniqueString()));
  }

  auto result = builder.finalize();
  return QUERY_END(result);
}

const uast::BuilderResult& buildEnumToStringCastImpl(Context* context, ID typeID, int overloadIdx) {
  return buildEnumToStringOrBytesCastImpl(context, typeID, true, /* castFromEnum */ true, overloadIdx);
}
const uast::BuilderResult& buildEnumToBytesCastImpl(Context* context, ID typeID, int overloadIdx) {
  return buildEnumToStringOrBytesCastImpl(context, typeID, false, /* castFromEnum */ true, overloadIdx);
}

const uast::BuilderResult& buildStringToEnumCastImpl(Context* context, ID typeID, int overloadIdx) {
  return buildEnumToStringOrBytesCastImpl(context, typeID, true, /* castFromEnum */ false, overloadIdx);
}
const uast::BuilderResult& buildBytesToEnumCastImpl(Context* context, ID typeID, int overloadIdx) {
  return buildEnumToStringOrBytesCastImpl(context, typeID, false, /* castFromEnum */ false, overloadIdx);
}

const BuilderResult& buildOrderToEnum(Context* context, ID typeID) {
  QUERY_BEGIN(buildOrderToEnum, context, typeID);

  auto bld = Builder::createForGeneratedCode(context, typeID);
  auto builder = bld.get();
  auto dummyLoc = parsing::locateId(context, typeID);

  auto intType = Identifier::build(builder, dummyLoc, USTR("int"));
  auto intName = UniqueString::get(context, "i");
  auto intFormal = Formal::build(builder, dummyLoc, nullptr,
                                 intName, Formal::DEFAULT_INTENT,
                                 std::move(intType), nullptr);

  auto argType = Identifier::build(builder, dummyLoc, typeID.symbolName(context));
  auto typeIdent = argType->copy(); // make a copy for the 'use <type>' stmt
  auto typeArgName = UniqueString::get(context, "et");
  auto argFormal = Formal::build(builder, dummyLoc, nullptr,
                                  typeArgName, Formal::TYPE,
                                  std::move(argType), nullptr);

  AstList formals;
  formals.push_back(std::move(intFormal));
  formals.push_back(std::move(argFormal));

  AstList stmts;
  auto type = parsing::idToAst(context, typeID)->toEnum();

  AstList useList;
  auto visClause = VisibilityClause::build(builder, dummyLoc,
                                           std::move(typeIdent));
  useList.push_back(std::move(visClause));
  auto use = Use::build(builder, dummyLoc, Decl::Visibility::DEFAULT_VISIBILITY, std::move(useList));
  stmts.push_back(std::move(use));

  // build up when-stmts

  AstList whenStmts;
  buildWhenStmts(context, builder, dummyLoc,
                 type, whenStmts, /*enumToOrder=*/false);

  auto expr = Identifier::build(builder, dummyLoc, intName);
  auto select = Select::build(builder, dummyLoc, std::move(expr), std::move(whenStmts));
  stmts.push_back(std::move(select));

  auto body = Block::build(builder, dummyLoc, std::move(stmts));

  auto returnIntent = Function::ReturnIntent::DEFAULT_RETURN_INTENT;
  auto genFn = Function::build(builder,
                               dummyLoc, {},
                               Decl::Visibility::PUBLIC,
                               Decl::Linkage::DEFAULT_LINKAGE,
                               /*linkageName=*/{},
                               USTR("chpl__orderToEnum"),
                               /*inline=*/false, /*override=*/false,
                               Function::Kind::PROC,
                               /*receiver=*/nullptr,
                               returnIntent,
                               // throws, primaryMethod, parenless
                               false, false, false,
                               std::move(formals),
                               // returnType, where, lifetime, body
                               {}, {}, {}, std::move(body));

  builder->noteChildrenLocations(genFn.get(), dummyLoc);
  builder->addToplevelExpression(std::move(genFn));

  auto result = builder->result();
  return QUERY_END(result);
}

static const TypedFnSignature*
generateDeSerialize(ResolutionContext* rc, const CompositeType* compType,
                    UniqueString name, std::string channel,
                    std::string deSerializer) {
  auto context = rc->context();
  std::vector<UntypedFnSignature::FormalDetail> ufsFormals;
  std::vector<QualifiedType> formalTypes;

  auto& br = buildDeSerialize(context, compType->id(), name == USTR("serialize"));
  auto genFn = br.topLevelExpression(0)->toFunction();

  ufsFormals.push_back(
      UntypedFnSignature::FormalDetail(USTR("this"),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       genFn->thisFormal()));
  ufsFormals.push_back(
      UntypedFnSignature::FormalDetail(UniqueString::get(context, channel),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       genFn->formal(1)));
  ufsFormals.push_back(
      UntypedFnSignature::FormalDetail(UniqueString::get(context, deSerializer),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       genFn->formal(2)));

  // build the untyped signature
  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ genFn->id(),
                        /*name*/ name,
                        /*isMethod*/ true,
                        /*isTypeConstructor*/ false,
                        /*isCompilerGenerated*/ true,
                        /*throws*/ true,
                        /*idTag*/ asttags::Function,
                        /*kind*/ uast::Function::Kind::PROC,
                        /*formals*/ std::move(ufsFormals),
                        /*whereClause*/ nullptr);

  return typedSignatureInitial(rc, ufs);
}

static const TypedFnSignature*
generateTupleMethod(Context* context,
                    const TupleType* at,
                    UniqueString name) {
  // TODO: we should really have a way to just set the return type here
  const TypedFnSignature* result = nullptr;
  std::vector<UntypedFnSignature::FormalDetail> formals;
  std::vector<QualifiedType> formalTypes;

  formals.push_back(
      UntypedFnSignature::FormalDetail(USTR("this"),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       nullptr));
  formalTypes.push_back(QualifiedType(QualifiedType::CONST_REF, at));

  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ at->id(),
                        /*name*/ name,
                        /*isMethod*/ true,
                        /*isTypeConstructor*/ false,
                        /*isCompilerGenerated*/ true,
                        /*throws*/ false,
                        /*idTag*/ asttags::Tuple,
                        /*kind*/ uast::Function::Kind::PROC,
                        /*formals*/ std::move(formals),
                        /*whereClause*/ nullptr);

  // now build the other pieces of the typed signature
  result = TypedFnSignature::get(context, ufs, std::move(formalTypes),
                                 TypedFnSignature::WHERE_NONE,
                                 /* needsInstantiation */ false,
                                 /* instantiatedFrom */ nullptr,
                                 /* parentFn */ nullptr,
                                 /* formalsInstantiated */ Bitmap(),
                                 /* outerVariables */ {});

  return result;
}

static const TypedFnSignature* const&
fieldAccessorQuery(Context* context,
                   const types::CompositeType* compType,
                   UniqueString fieldName) {
  QUERY_BEGIN(fieldAccessorQuery, context, compType, fieldName);

  const TypedFnSignature* result = nullptr;
  std::vector<UntypedFnSignature::FormalDetail> ufsFormals;
  std::vector<QualifiedType> formalTypes;

  // start by adding a formal for the receiver
  auto ufsReceiver =
    UntypedFnSignature::FormalDetail(USTR("this"),
                                     UntypedFnSignature::DK_NO_DEFAULT,
                                     nullptr);
  ufsFormals.push_back(std::move(ufsReceiver));

  const Type* thisType = compType;
  if (auto bct = thisType->toBasicClassType()) {
    auto dec = ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
    thisType = ClassType::get(context, bct, /*manager*/ nullptr, dec);
  }

  // receiver is ref-maybe-const to allow mutation
  formalTypes.push_back(
      QualifiedType(QualifiedType::REF_MAYBE_CONST, thisType));

  ID fieldId = parsing::fieldIdWithName(context, compType->id(), fieldName);

  // build the untyped signature
  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ fieldId,
                        /*name*/ fieldName,
                        /*isMethod*/ true,
                        /*isTypeConstructor*/ false,
                        /*isCompilerGenerated*/ true,
                        /*throws*/ false,
                        /*idTag*/ parsing::idToTag(context, fieldId),
                        /*kind*/ uast::Function::Kind::PROC,
                        /*formals*/ std::move(ufsFormals),
                        /*whereClause*/ nullptr);

  // now build the other pieces of the typed signature
  result = TypedFnSignature::get(context, ufs, std::move(formalTypes),
                                 TypedFnSignature::WHERE_NONE,
                                 /* needsInstantiation */ false,
                                 /* instantiatedFrom */ nullptr,
                                 /* parentFn */ nullptr,
                                 /* formalsInstantiated */ Bitmap(),
                                 /* outerVariables */ {});

  return QUERY_END(result);
}

const TypedFnSignature* fieldAccessor(Context* context,
                                      const types::CompositeType* compType,
                                      UniqueString fieldName) {
  if (compType == nullptr) {
    return nullptr;
  }

  return fieldAccessorQuery(context, compType, fieldName);
}

// generate formal detail and formal type
static void
generateOperatorFormalDetail(const UniqueString name,
                             const Type* compType,
                             std::vector<UntypedFnSignature::FormalDetail>& ufsFormals,
                             std::vector<QualifiedType>& formalTypes,
                             QualifiedType::Kind qtKind,
                             bool hasDefault = false,
                             const uast::Decl* decl = nullptr) {
  auto defaultKind = hasDefault ? UntypedFnSignature::DK_DEFAULT
                                : UntypedFnSignature::DK_NO_DEFAULT;

  auto fd = UntypedFnSignature::FormalDetail(name, defaultKind, decl);
  ufsFormals.push_back(std::move(fd));

  auto qtFd = QualifiedType(qtKind, compType);
  formalTypes.push_back(std::move(qtFd));
}

static const TypedFnSignature*
generatePtrMethod(Context* context, QualifiedType receiverType,
                   UniqueString name) {
  // Build a basic function signature for methods on a PtrType
  // TODO: we should really have a way to just set the return type here
  const PtrType* pt = receiverType.type()->toPtrType();
  const TypedFnSignature* result = nullptr;
  std::vector<UntypedFnSignature::FormalDetail> formals;
  std::vector<QualifiedType> formalTypes;

  formals.push_back(
      UntypedFnSignature::FormalDetail(USTR("this"),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       nullptr));
  // Allow calling 'eltType' on either a type or value
  auto qual = receiverType.isType() ? QualifiedType::TYPE : QualifiedType::CONST_REF;
  formalTypes.push_back(QualifiedType(qual, pt));

  if (name == "this") {
    formals.push_back(UntypedFnSignature::FormalDetail(
        UniqueString::get(context, "i"), UntypedFnSignature::DK_NO_DEFAULT,
        nullptr));
    formalTypes.push_back(
        QualifiedType(QualifiedType::VAR, AnyIntegralType::get(context)));
  }

  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ pt->id(context),
                        /*name*/ name,
                        /*isMethod*/ true,
                        /*isTypeConstructor*/ false,
                        /*isCompilerGenerated*/ true,
                        /*throws*/ false,
                        /*idTag*/ asttags::Class,
                        /*kind*/ uast::Function::Kind::PROC,
                        /*formals*/ std::move(formals),
                        /*whereClause*/ nullptr);

  // now build the other pieces of the typed signature
  result = TypedFnSignature::get(context, ufs, std::move(formalTypes),
                                 TypedFnSignature::WHERE_NONE,
                                 /* needsInstantiation */ false,
                                 /* instantiatedFrom */ nullptr,
                                 /* parentFn */ nullptr,
                                 /* formalsInstantiated */ Bitmap(),
                                 /* outerVariables */ {});

  return result;
}

static const TypedFnSignature*
generateEnumMethod(ResolutionContext* rc,
                   const EnumType* et,
                   UniqueString name) {
  const TypedFnSignature* result = nullptr;
  auto context = rc->context();
  if (name == USTR("size")) {
    if (!areFnOverloadsPresentInDefiningScope(context, et, QualifiedType::TYPE, name)) {
      // TODO: we should really have a way to just set the return type here
      std::vector<UntypedFnSignature::FormalDetail> formals;
      std::vector<QualifiedType> formalTypes;

      formals.push_back(
          UntypedFnSignature::FormalDetail(USTR("this"),
            UntypedFnSignature::DK_NO_DEFAULT,
            nullptr));
      formalTypes.push_back(QualifiedType(QualifiedType::TYPE, et));

      auto ufs = UntypedFnSignature::get(context,
          /*id*/ et->id(),
          /*name*/ name,
          /*isMethod*/ true,
          /*isTypeConstructor*/ false,
          /*isCompilerGenerated*/ true,
          /*throws*/ false,
          /*idTag*/ parsing::idToTag(context, et->id()),
          /*kind*/ uast::Function::Kind::PROC,
          /*formals*/ std::move(formals),
          /*whereClause*/ nullptr);

      // now build the other pieces of the typed signature
      result = TypedFnSignature::get(context, ufs, std::move(formalTypes),
          TypedFnSignature::WHERE_NONE,
          /* needsInstantiation */ false,
          /* instantiatedFrom */ nullptr,
          /* parentFn */ nullptr,
          /* formalsInstantiated */ Bitmap(),
          /* outerVariables */ {});
    }
  }

  return result;
}

static const TypedFnSignature*
generateIteratorMethod(Context* context,
                       const IteratorType* it,
                       UniqueString name) {

  const TypedFnSignature* result = nullptr;
  if (name == "_shape_" &&
      shapeForIterator(context, it)) {
    std::vector<UntypedFnSignature::FormalDetail> formals;
    std::vector<QualifiedType> formalTypes;

    formals.push_back(
        UntypedFnSignature::FormalDetail(USTR("this"),
          UntypedFnSignature::DK_NO_DEFAULT,
          nullptr));
    formalTypes.push_back(QualifiedType(QualifiedType::CONST_REF, it));

    // It's a little scary to compute the ID for the function in this way
    // here because for the FnIterator and PromotionIterator cases, it will
    // conflict with the underlying function. Maybe that's okay?
    ID id;
    uast::asttags::AstTag tag = uast::asttags::Variable;
    if (auto loopExprIt = it->toLoopExprIteratorType()) {
      id = loopExprIt->sourceLocation();
    } else if (auto fnIt = it->toFnIteratorType()) {
      id = fnIt->iteratorFn()->id();
    } else if (auto promoIt = it->toPromotionIteratorType()) {
      id = promoIt->scalarFn()->id();
    } else {
      CHPL_ASSERT(false && "case not handled");
    }

    auto ufs = UntypedFnSignature::get(context,
        /*id*/ id,
        /*name*/ name,
        /*isMethod*/ true,
        /*isTypeConstructor*/ false,
        /*isCompilerGenerated*/ true,
        /*throws*/ false,
        /*idTag*/ tag,
        /*kind*/ uast::Function::Kind::PROC,
        /*formals*/ std::move(formals),
        /*whereClause*/ nullptr);

    // now build the other pieces of the typed signature
    result = TypedFnSignature::get(context, ufs, std::move(formalTypes),
        TypedFnSignature::WHERE_NONE,
        /* needsInstantiation */ false,
        /* instantiatedFrom */ nullptr,
        /* parentFn */ nullptr,
        /* formalsInstantiated */ Bitmap(),
        /* outerVariables */ {});
  }
  return result;
}

// Note: Generating uAST for extern assignment isn't possible at the moment
// because types like ``extern type my_struct_t;`` don't have a symbol path
// we can use. It's just something like ``MyMod@42``, which we can't use to
// make , e.g., ``MyMod@42.=``. We could generate a function like for
// extern records like ``extern record R { var x : int; }``, but it seems
// simpler to handle both in the same way.
static const TypedFnSignature*
generateExternAssignment(ResolutionContext* rc, const ExternType* type) {
  auto context = rc->context();
  const TypedFnSignature* result = nullptr;
  std::vector<UntypedFnSignature::FormalDetail> ufsFormals;
  std::vector<QualifiedType> formalTypes;

  generateOperatorFormalDetail(UniqueString::get(context,"lhs"),
                               type, ufsFormals, formalTypes,
                               QualifiedType::REF);
  generateOperatorFormalDetail(UniqueString::get(context,"rhs"),
                               type, ufsFormals, formalTypes,
                               QualifiedType::CONST_INTENT);

  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ type->id(),
                        /*name*/ USTR("="),
                        /*isMethod*/ true,
                        /*isTypeConstructor*/ false,
                        /*isCompilerGenerated*/ true,
                        /*throws*/ false,
                        /*idTag*/ asttags::Tuple,
                        /*kind*/ uast::Function::Kind::PROC,
                        /*formals*/ std::move(ufsFormals),
                        /*whereClause*/ nullptr);

  // now build the other pieces of the typed signature
  result = TypedFnSignature::get(context, ufs, std::move(formalTypes),
                                 TypedFnSignature::WHERE_NONE,
                                 /* needsInstantiation */ false,
                                 /* instantiatedFrom */ nullptr,
                                 /* parentFn */ nullptr,
                                 /* formalsInstantiated */ Bitmap(),
                                 /* outerVariables */ {});

  return result;
}

static const TypedFnSignature* const&
getCompilerGeneratedMethodQuery(ResolutionContext* rc, QualifiedType receiverType,
                                UniqueString name, bool parenless) {
  CHPL_RESOLUTION_QUERY_BEGIN(getCompilerGeneratedMethodQuery, rc, receiverType, name, parenless);
  auto context = rc->context();

  const Type* type = receiverType.type();

  const TypedFnSignature* result = nullptr;

  if (needCompilerGeneratedMethod(context, type, name, parenless)) {
    auto compType = type->getCompositeType();
    CHPL_ASSERT(compType || type->isPtrType() || type->isEnumType() ||
                type->isIteratorType() || type->isExternType());

    if (name == USTR("init")) {
      result = generateInitSignature(rc, compType);
    } else if (name == USTR("init=")) {
      result = generateInitCopySignature(rc, compType);
    } else if (name == USTR("deinit")) {
      result = generateDeinitSignature(rc, compType);
    } else if (name == USTR("serialize")) {
      result = generateDeSerialize(rc, compType, name, "writer", "serializer");
    } else if (name == USTR("deserialize")) {
      result = generateDeSerialize(rc, compType, name, "reader", "deserializer");
    } else if (name == USTR("hash")) {
      result = generateRecordHashSignature(rc, compType);
    } else if (auto tupleType = type->toTupleType()) {
      result = generateTupleMethod(context, tupleType, name);
    } else if (type->isPtrType()) {
      result = generatePtrMethod(context, receiverType, name);
    } else if (auto enumType = type->toEnumType()) {
      result = generateEnumMethod(rc, enumType, name);
    } else if (auto iterType = type->toIteratorType()) {
      result = generateIteratorMethod(context, iterType, name);
    } else {
      CHPL_UNIMPL("should not be reachable");
    }
  }

  CHPL_ASSERT(result == nullptr || result->untyped()->name() == name);

  return CHPL_RESOLUTION_QUERY_END(result);
}

static const TypedFnSignature* const&
getCompilerGeneratedBinaryOpQuery(ResolutionContext* rc,
                                  QualifiedType lhsType,
                                  QualifiedType rhsType,
                                  UniqueString name) {
  CHPL_RESOLUTION_QUERY_BEGIN(getCompilerGeneratedBinaryOpQuery, rc, lhsType, rhsType, name);
  auto context = rc->context();

  const TypedFnSignature* result = nullptr;
  auto lhsT = lhsType.type();

  if (needCompilerGeneratedBinaryOp(context, lhsType, rhsType, name)) {
    if (const EnumType* enumType = nullptr;
        auto generator = generatorForCompilerGeneratedEnumOperator(name, lhsType, rhsType, enumType)) {
      result = generator(rc, enumType);
    } else if (auto recordType = lhsT->toRecordType()) {
      if (auto generator = generatorForCompilerGeneratedRecordOperator(name)) {
        result = generator(rc, recordType);
      } else {
        CHPL_UNIMPL("record method not implemented yet!");
      }
    } else if (lhsT->isExternType() && name == USTR("=")) {
      result = generateExternAssignment(rc, lhsT->toExternType());
    } else {
      CHPL_UNIMPL("should not be reachable");
    }
  }

  CHPL_ASSERT(result == nullptr || result->untyped()->name() == name);

  return CHPL_RESOLUTION_QUERY_END(result);
}

/**
  Given a type and a UniqueString representing the name of a method,
  determine if the type needs a method with such a name to be
  generated for it, and if so, generates and returns a
  TypedFnSignature representing the generated method.

  If no method was generated, returns nullptr.
*/
const TypedFnSignature*
getCompilerGeneratedMethod(ResolutionContext* rc, const QualifiedType receiverType,
                           UniqueString name, bool parenless) {
  // Normalize receiverType to allow TYPE methods on c_ptr and _ddata, and to
  // otherwise use the VAR Kind. The Param* value is also stripped away to
  // reduce queries.
  auto qt = receiverType;
  bool isPtr = qt.hasTypePtr() ? qt.type()->isPtrType() : false;
  if (!(qt.isType() && isPtr)) {
    qt = QualifiedType(QualifiedType::VAR, qt.type());
  }
  return getCompilerGeneratedMethodQuery(rc, qt, name, parenless);
}

const TypedFnSignature*
getCompilerGeneratedBinaryOp(ResolutionContext* rc,
                             const types::QualifiedType lhsType,
                             const types::QualifiedType rhsType,
                             UniqueString name) {

  return getCompilerGeneratedBinaryOpQuery(rc, lhsType, rhsType, name);
}

static const TypedFnSignature* const&
getParamOrderToEnum(Context* context, const EnumType* et) {
  QUERY_BEGIN(getParamOrderToEnum, context, et);

  std::vector<UntypedFnSignature::FormalDetail> ufsFormals;
  std::vector<QualifiedType> formalTypes;

  ufsFormals.push_back(
      UntypedFnSignature::FormalDetail(UniqueString::get(context, "i"),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       nullptr));
  formalTypes.push_back(QualifiedType(QualifiedType::PARAM,
                                      AnyIntegralType::get(context)));

  ufsFormals.push_back(
      UntypedFnSignature::FormalDetail(UniqueString::get(context, "et"),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       nullptr));
  formalTypes.push_back(QualifiedType(QualifiedType::TYPE, et));

  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ et->id(),
                        /*name*/ USTR("chpl__orderToEnum"),
                        /*isMethod*/ false,
                        /*isTypeConstructor*/ false,
                        /*isCompilerGenerated*/ true,
                        /*throws*/ false,
                        /*idTag*/ parsing::idToTag(context, et->id()),
                        /*kind*/ uast::Function::Kind::PROC,
                        /*formals*/ std::move(ufsFormals),
                        /*whereClause*/ nullptr);

  auto ret = TypedFnSignature::get(context,
                                   ufs,
                                   std::move(formalTypes),
                                   TypedFnSignature::WHERE_NONE,
                                   /* needsInstantiation */ true,
                                   /* instantiatedFrom */ nullptr,
                                   /* parentFn */ nullptr,
                                   /* formalsInstantiated */ Bitmap(),
                                   /* outerVariables */ {});

  return QUERY_END(ret);
}

static const TypedFnSignature* const&
getOrderToEnum(ResolutionContext* rc, const EnumType* et) {
  auto context = rc->context();
  std::vector<UntypedFnSignature::FormalDetail> ufsFormals;

  auto& br = buildOrderToEnum(context, et->id());
  auto genFn = br.topLevelExpression(0)->toFunction();

  ufsFormals.push_back(
      UntypedFnSignature::FormalDetail(UniqueString::get(context, "i"),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       genFn->formal(0)));
  ufsFormals.push_back(
      UntypedFnSignature::FormalDetail(UniqueString::get(context, "et"),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       genFn->formal(1)));

  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ genFn->id(),
                        /*name*/ genFn->name(),
                        /*isMethod*/ false,
                        /*isTypeConstructor*/ false,
                        /*isCompilerGenerated*/ true,
                        /*throws*/ false,
                        /*idTag*/ asttags::Function,
                        /*kind*/ uast::Function::Kind::PROC,
                        /*formals*/ std::move(ufsFormals),
                        /*whereClause*/ nullptr);

  return typedSignatureInitial(rc, ufs);
}

static const TypedFnSignature* const&
getParamEnumToOrder(Context* context, const EnumType* et) {
  QUERY_BEGIN(getParamEnumToOrder, context, et);

  std::vector<UntypedFnSignature::FormalDetail> ufsFormals;
  std::vector<QualifiedType> formalTypes;

  ufsFormals.push_back(
      UntypedFnSignature::FormalDetail(UniqueString::get(context, "e"),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       nullptr));
  formalTypes.push_back(QualifiedType(QualifiedType::PARAM,
                                      et));

  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ et->id(),
                        /*name*/ USTR("chpl__enumToOrder"),
                        /*isMethod*/ false,
                        /*isTypeConstructor*/ false,
                        /*isCompilerGenerated*/ true,
                        /*throws*/ false,
                        /*idTag*/ parsing::idToTag(context, et->id()),
                        /*kind*/ uast::Function::Kind::PROC,
                        /*formals*/ std::move(ufsFormals),
                        /*whereClause*/ nullptr);

  auto formalsInstantiated = Bitmap();
  auto ret = TypedFnSignature::get(context,
                                   ufs,
                                   std::move(formalTypes),
                                   TypedFnSignature::WHERE_NONE,
                                   /* needsInstantiation */ true,
                                   /* instantiatedFrom */ nullptr,
                                   /* parentFn */ nullptr,
                                   /* formalsInstantiated */ Bitmap(),
                                   /* outerVariables */ {});

  return QUERY_END(ret);
}

static const TypedFnSignature* const&
getEnumToOrder(ResolutionContext* rc, const EnumType* et) {
  auto context = rc->context();
  std::vector<UntypedFnSignature::FormalDetail> ufsFormals;

  auto& br = buildEnumToOrder(context, et->id());
  auto genFn = br.topLevelExpression(0)->toFunction();

  ufsFormals.push_back(
      UntypedFnSignature::FormalDetail(UniqueString::get(context, "e"),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       genFn->formal(0)));

  // build the untyped signature
  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ genFn->id(),
                        /*name*/ genFn->name(),
                        /*isMethod*/ false,
                        /*isTypeConstructor*/ false,
                        /*isCompilerGenerated*/ true,
                        /*throws*/ false,
                        /*idTag*/ asttags::Function,
                        /*kind*/ uast::Function::Kind::PROC,
                        /*formals*/ std::move(ufsFormals),
                        /*whereClause*/ nullptr);

  return typedSignatureInitial(rc, ufs);
}

const TypedFnSignature*
getCompilerGeneratedFunction(ResolutionContext* rc,
                             const CallInfo& ci) {
  if (ci.name() == USTR("chpl__orderToEnum")) {
    if (ci.numActuals() == 2) {
      auto& firstQt = ci.actual(0).type();
      auto& secondQt = ci.actual(1).type();

      auto secondType = secondQt.type();
      if (secondType && secondType->isEnumType()) {
        if (firstQt.isParam()) {
          return getParamOrderToEnum(rc->context(), secondType->toEnumType());
        } else {
          return getOrderToEnum(rc, secondType->toEnumType());
        }
      }
    }
  } else if (ci.name() == USTR("chpl__enumToOrder")) {
    if (ci.numActuals() == 1) {
      auto& firstQt = ci.actual(0).type();

      auto firstType = firstQt.type();
      if (firstType && firstType->isEnumType()) {
        if (firstQt.isParam()) {
          return getParamEnumToOrder(rc->context(), firstType->toEnumType());
        } else {
          return getEnumToOrder(rc, firstType->toEnumType());
        }
      }
    }
  }
  return nullptr;
}

static owned<Function> typeConstructorFnForInterface(Context* context,
                                                     const Interface* itf,
                                                     Builder* builder,
                                                     Location fnLoc) {
  AstList formals;
  for (auto formal : itf->formals()) {
    formals.push_back(formal->copy());
  }

  auto genFn = Function::build(builder, fnLoc, {},
                               Decl::Visibility::PUBLIC,
                               Decl::Linkage::DEFAULT_LINKAGE,
                               /*linkageName=*/{},
                               itf->name(),
                               /*inline=*/false, /*override=*/false,
                               Function::Kind::PROC,
                               /*receiver=*/{},
                               Function::ReturnIntent::DEFAULT_RETURN_INTENT,
                               // throws, primaryMethod, parenless
                               false, false, false,
                               std::move(formals),
                               // returnType, where, lifetime, body
                               {}, {}, {}, {});


  return genFn;
}

static owned<Function> typeConstructorFnForComposite(Context* context,
                                                     const CompositeType* ct,
                                                     Builder* builder,
                                                     Location fnLoc) {
  AstList formals;

  if (auto bct = ct->toBasicClassType()) {
    auto parent = bct->parentClassType();
    if (parent && !parent->isObjectType()) {
      auto& br = buildTypeConstructor(context, parent->id());
      auto fn = br.topLevelExpression(0)->toFunction();

      for (auto formal : fn->formals()) {
        formals.push_back(formal->copy());
      }
    }
  }

  // attempt to resolve the fields
  DefaultsPolicy defaultsPolicy = DefaultsPolicy::IGNORE_DEFAULTS;
  ResolutionContext rc(context);
  const ResolvedFields& f = fieldsForTypeDecl(&rc, ct,
                                              defaultsPolicy,
                                              /* syntaxOnly */ true);

  // find the generic fields from the type and add
  // these as type constructor arguments.
  int nFields = f.numFields();
  for (int i = 0; i < nFields; i++) {
    auto declId = f.fieldDeclId(i);
    auto declAst = parsing::idToAst(context, declId);
    CHPL_ASSERT(declAst);
    auto fieldDecl = declAst->toVariable();
    CHPL_ASSERT(fieldDecl);
    QualifiedType formalType;
    if (isFieldSyntacticallyGeneric(context, declId, nullptr)) {

      auto typeExpr = fieldDecl->typeExpression();
      auto initExpr = fieldDecl->initExpression();
      auto kind = fieldDecl->kind() == Variable::PARAM ? Variable::PARAM : Variable::TYPE;
      owned<AstNode> formal = Formal::build(builder, fnLoc,
                                            /*attributeGroup=*/nullptr,
                                            fieldDecl->name(),
                                            (Formal::Intent)kind,
                                            typeExpr ? typeExpr->copy() : nullptr,
                                            initExpr ? initExpr->copy() : nullptr);
      formals.push_back(std::move(formal));
    }
  }

  auto genFn = Function::build(builder, fnLoc, {},
                               Decl::Visibility::PUBLIC,
                               Decl::Linkage::DEFAULT_LINKAGE,
                               /*linkageName=*/{},
                               ct->name(),
                               /*inline=*/false, /*override=*/false,
                               Function::Kind::PROC,
                               /*receiver=*/{},
                               Function::ReturnIntent::DEFAULT_RETURN_INTENT,
                               // throws, primaryMethod, parenless
                               false, false, false,
                               std::move(formals),
                               // returnType, where, lifetime, body
                               {}, {}, {}, {});
  return genFn;
}

const BuilderResult&
buildTypeConstructor(Context* context, ID typeID) {
  QUERY_BEGIN(buildTypeConstructor, context, typeID);

  auto bld = Builder::createForGeneratedCode(context, typeID);
  auto builder = bld.get();
  auto dummyLoc = parsing::locateId(context, typeID);

  auto tag = parsing::idToTag(context, typeID);
  owned<Function> genFn;
  if (asttags::isInterface(tag)) {
    auto itf = parsing::idToAst(context, typeID)->toInterface();
    genFn = typeConstructorFnForInterface(context, itf, builder, dummyLoc);
  } else {
    auto ct = initialTypeForTypeDecl(context, typeID)->getCompositeType();
    genFn = typeConstructorFnForComposite(context, ct, builder, dummyLoc);
  }

  builder->noteChildrenLocations(genFn.get(), dummyLoc);
  builder->addToplevelExpression(std::move(genFn));

  // Finalize the uAST and obtain the BuilderResult
  auto res = builder->result();

  return QUERY_END(res);
}

const BuilderResult*
builderResultForDefaultFunction(Context* context,
                                UniqueString typePath,
                                UniqueString name,
                                UniqueString overloadPart) {
  auto typeID = ID(typePath);

  if (name == USTR("init")) {
    return &buildInitializer(context, typeID);
  } else if (name == USTR("init=")) {
    return &buildInitEquals(context, typeID);
  } else if (name == USTR("deinit")) {
    return &buildDeinit(context, typeID);
  } else if (name == USTR("serialize")) {
    return &buildDeSerialize(context, typeID, true);
  } else if (name == USTR("deserialize")) {
    return &buildDeSerialize(context, typeID, false);
  } else if (name == USTR("hash")) {
    return &buildRecordHash(context, typeID);
  } else if (name == USTR("==")) {
    return &buildRecordComparison(context, typeID);
  } else if (name == USTR("!=")) {
    return &buildRecordInequalityComparison(context, typeID);
  } else if (name == USTR("<")) {
    return &buildRecordCompareLt(context, typeID);
  } else if (name == USTR("<=")) {
    return &buildRecordCompareLe(context, typeID);
  } else if (name == USTR(">")) {
    return &buildRecordCompareGt(context, typeID);
  } else if (name == USTR(">=")) {
    return &buildRecordCompareGe(context, typeID);
  } else if (name == USTR("=")) {
    return &buildRecordAssign(context, typeID);
  } else if (name == USTR("chpl__enumToOrder")) {
    return &buildEnumToOrder(context, typeID);
  } else if (name == USTR("chpl__orderToEnum")) {
    return &buildOrderToEnum(context, typeID);
  } else if (name == USTR(":")) {
    return &EnumCastSelector::select(context, typeID, overloadPart);
  } else if (typeID.symbolName(context) == name) {
    return &buildTypeConstructor(context, typeID);
  }

  return nullptr;
}


} // end namespace resolution
} // end namespace chpl
