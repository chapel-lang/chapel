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
#include "chpl/resolution/can-pass.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/AstTag.h"
#include "chpl/uast/all-uast.h"

#include "Resolver.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;


/**
  Return true if 'name' is the name of a compiler generated method.
*/
static bool isNameOfCompilerGeneratedMethod(UniqueString name) {
  // TODO: Update me over time.
  if (name == USTR("init")       ||
      name == USTR("deinit")     ||
      name == USTR("init=")) {
    return true;
  }

  if (name == USTR("serialize") || name == USTR("deserialize")) {
    return true;
  }

  return false;
}

static bool
areOverloadsPresentInDefiningScope(Context* context,
                                   const Type* type,
                                   QualifiedType::Kind kind,
                                   UniqueString name) {
  const Scope* scopeForReceiverType = nullptr;

  if (auto compType = type->getCompositeType()) {
    scopeForReceiverType = scopeForId(context, compType->id());
  } else if (auto enumType = type->toEnumType()) {
    scopeForReceiverType = scopeForId(context, enumType->id());
  }

  // there is no defining scope
  if (!scopeForReceiverType) return false;

  // do not look outside the defining module
  const LookupConfig config = LOOKUP_DECLS | LOOKUP_PARENTS | LOOKUP_METHODS;

  auto ids = lookupNameInScope(context, scopeForReceiverType,
                               /* methodLookupHelper */ nullptr,
                               /* receiverScopeHelper */ nullptr,
                               name, config);

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
        auto result = canPass(context, haveQt, receiverQualType);
        if (result.passes() &&
            (!result.converts() || result.convertsWithBorrowing()) &&
            !result.promotes()) {
          return true;
        }
      }
    }
  }

  return false;
}

// non-record types have == and = implemented in the modules so we don't
// want to generate them
static bool isBuiltinTypeOperator(UniqueString name) {
  // adding "==" and "=" to list of compiler generated method names was
  // problematic for other types, like int
  return !(name == USTR("==") || name == USTR("="));
}

bool
needCompilerGeneratedMethod(Context* context, const Type* type,
                            UniqueString name, bool parenless) {
  if (type == nullptr) return false;

  if (type->isNothingType()) return false;

  if (isNameOfCompilerGeneratedMethod(name) ||
      (type->isRecordType() && !isBuiltinTypeOperator(name))) {
    if (!areOverloadsPresentInDefiningScope(context, type, QualifiedType::INIT_RECEIVER, name)) {
      return true;
    }
  }

  if (type->isArrayType()) {
    if (name == "domain" || name == "eltType") {
      return true;
    }
  } else if (type->isPtrType()) {
    if (name == "eltType") {
      return true;
    } else if (type->isHeapBufferType() && name == "this") {
      return true;
    }
  } else if (type->isEnumType()) {
    if (name == "size") {
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
      auto userDefinedExists = areOverloadsPresentInDefiningScope(context,
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

  auto thisType = Identifier::build(builder, dummyLoc, typeDecl->name());
  auto thisFormal = Formal::build(builder, dummyLoc, nullptr,
                                  USTR("this"), Formal::DEFAULT_INTENT,
                                  std::move(thisType), nullptr);

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
                               false, false, false,
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

      // Check if formal should have a default, which is never true for init=
      if (initFn->name() != USTR("init=")) {
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
generateInitSignature(Context* context, const CompositeType* inCompType) {
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

  ResolutionContext rcval(context);
  return typedSignatureInitial(&rcval, uSig);
}

const BuilderResult& buildInitEquals(Context* context, ID typeID) {
  QUERY_BEGIN(buildInitEquals, context, typeID);

  auto typeDecl = parsing::idToAst(context, typeID)->toAggregateDecl();
  auto bld = Builder::createForGeneratedCode(context, typeID);
  auto builder = bld.get();
  auto dummyLoc = parsing::locateId(context, typeID);

  auto thisType = Identifier::build(builder, dummyLoc, typeDecl->name());
  auto thisFormal = Formal::build(builder, dummyLoc, nullptr,
                                  USTR("this"), Formal::DEFAULT_INTENT,
                                  std::move(thisType), nullptr);

  // TODO: constrain type to be same as 'typeID', possibly through 'this.type'?
  auto otherName = UniqueString::get(context, "other");
  auto otherFormal = Formal::build(builder, dummyLoc, nullptr,
                                   otherName, Formal::DEFAULT_INTENT,
                                   nullptr, nullptr);
  AstList formals;
  formals.push_back(std::move(otherFormal));

  AstList stmts;

  auto ct = initialTypeForTypeDecl(context, typeID)->getCompositeType();

  // attempt to resolve the fields
  DefaultsPolicy defaultsPolicy = DefaultsPolicy::IGNORE_DEFAULTS;
  const ResolvedFields& rf = fieldsForTypeDecl(context, ct,
                                               defaultsPolicy,
                                               /* syntaxOnly */ true);
  for (int i = 0; i < rf.numFields(); i++) {
    auto name = rf.fieldName(i);
    // Create 'this.field = other.field;' statement
    owned<AstNode> lhs = Dot::build(builder, dummyLoc,
                                    Identifier::build(builder, dummyLoc, USTR("this")),
                                    name);
    owned<AstNode> rhs = Dot::build(builder, dummyLoc,
                                    Identifier::build(builder, dummyLoc, otherName),
                                    name);
    owned<AstNode> assign = OpCall::build(builder, dummyLoc, USTR("="),
                                          std::move(lhs), std::move(rhs));
    stmts.push_back(std::move(assign));
  }

  auto body = Block::build(builder, dummyLoc, std::move(stmts));
  auto genFn = Function::build(builder,
                               dummyLoc, {},
                               Decl::Visibility::PUBLIC,
                               Decl::Linkage::DEFAULT_LINKAGE,
                               /*linkageName=*/{},
                               USTR("init="),
                               /*inline=*/false, /*override=*/false,
                               Function::Kind::PROC,
                               /*receiver=*/std::move(thisFormal),
                               Function::ReturnIntent::DEFAULT_RETURN_INTENT,
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
generateInitCopySignature(Context* context, const CompositeType* inCompType) {
  auto& br = buildInitEquals(context, inCompType->id());
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

  ResolutionContext rcval(context);
  return typedSignatureInitial(&rcval, uSig);
}

const BuilderResult& buildDeinit(Context* context, ID typeID) {
  QUERY_BEGIN(buildDeinit, context, typeID);

  auto bld = Builder::createForGeneratedCode(context, typeID);
  auto builder = bld.get();
  auto dummyLoc = parsing::locateId(context, typeID);

  auto thisType = Identifier::build(builder, dummyLoc, typeID.symbolName(context));
  auto thisFormal = Formal::build(builder, dummyLoc, nullptr,
                                  USTR("this"), Formal::DEFAULT_INTENT,
                                  std::move(thisType), nullptr);

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
generateDeinitSignature(Context* context, const CompositeType* inCompType) {
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

  ResolutionContext rcval(context);
  return typedSignatureInitial(&rcval, ufs);
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
                                  std::move(thisType), nullptr);

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
                               true, false, false,
                               std::move(formals),
                               // returnType, where, lifetime, body
                               {}, {}, {}, std::move(body));

  builder->noteChildrenLocations(genFn.get(), dummyLoc);
  builder->addToplevelExpression(std::move(genFn));

  auto result = builder->result();
  return QUERY_END(result);
}

static const TypedFnSignature*
generateDeSerialize(Context* context, const CompositeType* compType,
                    UniqueString name, std::string channel,
                    std::string deSerializer) {
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
                                       genFn->formal(0)));
  ufsFormals.push_back(
      UntypedFnSignature::FormalDetail(UniqueString::get(context, deSerializer),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       genFn->formal(1)));

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

  ResolutionContext rcval(context);
  return typedSignatureInitial(&rcval, ufs);
}

static const TypedFnSignature*
generateArrayMethod(Context* context,
                    const ArrayType* at,
                    UniqueString name) {
  // Build a basic function signature for methods on an array
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
                        /*idTag*/ parsing::idToTag(context, at->id()),
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
                             const CompositeType*& compType,
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


// builds the formal entries for the operator methods, including the 'this'
// method receiver and the lhs argument. Specify the
// QualifiedType::Kind for each of these.
static void
generateUnaryOperatorMethodParts(Context* context,
                                  const CompositeType* inCompType,
                                  const CompositeType*& compType,
                                  std::vector<UntypedFnSignature::FormalDetail>& ufsFormals,
                                  std::vector<QualifiedType>& formalTypes,
                                  QualifiedType::Kind thisKind,
                                  QualifiedType::Kind lhsKind) {
  // adjust to refer to fully generic signature if needed
  auto genericCompType = inCompType->instantiatedFromCompositeType();
  compType = genericCompType ? genericCompType : inCompType;

  // make sure the receiver is a record type
  CHPL_ASSERT(compType->isRecordType() && "Only RecordType supported for now");

  // start by adding a formal for the receiver, 'this'
  generateOperatorFormalDetail(USTR("this"), compType, ufsFormals, formalTypes,
                               thisKind);

  // add a formal for the 'lhs' argument
  generateOperatorFormalDetail(UniqueString::get(context,"lhs"),
                               compType, ufsFormals, formalTypes,
                               lhsKind);
  CHPL_ASSERT(formalTypes.size() == 2);
  CHPL_ASSERT(ufsFormals.size() == 2);
}

// builds the formal entries for the operator methods, including the 'this'
// method receiver and the lhs and rhs arguments. Specify the
// QualifiedType::Kind for each of these.
static void
generateBinaryOperatorMethodParts(Context* context,
                                  const CompositeType* inCompType,
                                  const CompositeType*& compType,
                                  std::vector<UntypedFnSignature::FormalDetail>& ufsFormals,
                                  std::vector<QualifiedType>& formalTypes,
                                  QualifiedType::Kind thisKind,
                                  QualifiedType::Kind lhsKind,
                                  QualifiedType::Kind rhsKind) {
  // add formals for the 'this' receiver and 'lhs' argument
  generateUnaryOperatorMethodParts(context, inCompType, compType, ufsFormals,
                                   formalTypes, thisKind, lhsKind);

  // add a formal for the 'rhs' argument
  generateOperatorFormalDetail(UniqueString::get(context,"rhs"),
                               compType, ufsFormals, formalTypes,
                               rhsKind);

  CHPL_ASSERT(formalTypes.size() == 3);
  CHPL_ASSERT(ufsFormals.size() == 3);
}

/*
generate a TypedFnSignature and UntypedFnSignature with formal details for a
record operator method. The operator is specified by the UniqueString op.
*/
static const TypedFnSignature*
generateRecordBinaryOperator(Context* context, UniqueString op,
                             const CompositeType* lhsType,
                             QualifiedType::Kind thisKind,
                             QualifiedType::Kind lhsKind,
                             QualifiedType::Kind rhsKind) {
  const CompositeType* compType = nullptr;
  std::vector<UntypedFnSignature::FormalDetail> ufsFormals;
  std::vector<QualifiedType> formalTypes;

  // build the formal details
  generateBinaryOperatorMethodParts(context, lhsType, compType, ufsFormals,
                                  formalTypes, thisKind, lhsKind, rhsKind);
  // build the untyped signature
  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ compType->id(),
                        /*name*/ op,
                        /*isMethod*/ true,
                        /*isTypeConstructor*/ false,
                        /*isCompilerGenerated*/ true,
                        /*throws*/ false,
                        /*idTag*/ parsing::idToTag(context, compType->id()),
                        /*kind*/ uast::Function::Kind::OPERATOR,
                        /*formals*/ std::move(ufsFormals),
                        /*whereClause*/ nullptr);

  // now build the other pieces of the typed signature
  auto g = getTypeGenericity(context, lhsType);
  bool needsInstantiation = (g == Type::GENERIC ||
                             g == Type::GENERIC_WITH_DEFAULTS);

  auto ret = TypedFnSignature::get(context,
                                   ufs,
                                   std::move(formalTypes),
                                   TypedFnSignature::WHERE_NONE,
                                   needsInstantiation,
                                   /* instantiatedFrom */ nullptr,
                                   /* parentFn */ nullptr,
                                   /* formalsInstantiated */ Bitmap(),
                                   /* outerVariables */ {});

  return ret;
}

static const TypedFnSignature*
generateRecordAssignment(Context* context, const CompositeType* lhsType) {
  return generateRecordBinaryOperator(context, USTR("="), lhsType,
                                      /*this*/ QualifiedType::CONST_REF,
                                      /*lhs*/  QualifiedType::CONST_REF,
                                      /*rhs*/  QualifiedType::CONST_REF);
}

static const TypedFnSignature*
generateRecordComparison(Context* context, const CompositeType* lhsType) {
  return generateRecordBinaryOperator(context, USTR("=="), lhsType,
                                      /*this*/ QualifiedType::REF,
                                      /*lhs*/  QualifiedType::REF,
                                      /*rhs*/  QualifiedType::CONST_REF);
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
generateEnumMethod(Context* context,
                   const EnumType* et,
                   UniqueString name) {
  const TypedFnSignature* result = nullptr;
  if (name == USTR("size") &&
      !areOverloadsPresentInDefiningScope(context, et, QualifiedType::TYPE, name)) {
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

  return result;
}

static const TypedFnSignature* const&
getCompilerGeneratedMethodQuery(Context* context, QualifiedType receiverType,
                                UniqueString name, bool parenless) {
  QUERY_BEGIN(getCompilerGeneratedMethodQuery, context, receiverType, name, parenless);

  const Type* type = receiverType.type();

  const TypedFnSignature* result = nullptr;

  if (needCompilerGeneratedMethod(context, type, name, parenless)) {
    auto compType = type->getCompositeType();
    CHPL_ASSERT(compType || type->isPtrType() || type->isEnumType());

    if (name == USTR("init")) {
      result = generateInitSignature(context, compType);
    } else if (name == USTR("init=")) {
      result = generateInitCopySignature(context, compType);
    } else if (name == USTR("deinit")) {
      result = generateDeinitSignature(context, compType);
    } else if (name == USTR("serialize")) {
      result = generateDeSerialize(context, compType, name, "writer", "serializer");
    } else if (name == USTR("deserialize")) {
      result = generateDeSerialize(context, compType, name, "reader", "deserializer");
    } else if (auto arrayType = type->toArrayType()) {
      result = generateArrayMethod(context, arrayType, name);
    } else if (auto tupleType = type->toTupleType()) {
      result = generateTupleMethod(context, tupleType, name);
    } else if (auto recordType = type->toRecordType()) {
      if (name == USTR("==")) {
        result = generateRecordComparison(context, recordType);
      } else if (name == USTR("=")) {
        result = generateRecordAssignment(context, recordType);
      } else {
        CHPL_UNIMPL("record method not implemented yet!");
      }
    } else if (type->isPtrType()) {
      result = generatePtrMethod(context, receiverType, name);
    } else if (auto enumType = type->toEnumType()) {
      result = generateEnumMethod(context, enumType, name);
    } else {
      CHPL_UNIMPL("should not be reachable");
    }
  }

  CHPL_ASSERT(result == nullptr || result->untyped()->name() == name);

  return QUERY_END(result);
}

static void
setupGeneratedEnumCastFormals(Context* context,
                              const EnumType* enumType,
                              std::vector<UntypedFnSignature::FormalDetail>& ufsFormals,
                              std::vector<QualifiedType>& formalTypes,
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
  ufsFormals.push_back(std::move(ufsFrom));
  auto ufsTo =
      UntypedFnSignature::FormalDetail(UniqueString::get(context, "to"),
                                       UntypedFnSignature::DK_NO_DEFAULT, nullptr);
  ufsFormals.push_back(std::move(ufsTo));

  formalTypes.push_back(fromQt);
  formalTypes.push_back(toQt);
}

static const TypedFnSignature*
generateToOrFromCastForEnum(Context* context,
                            const types::QualifiedType& lhs,
                            const types::QualifiedType& rhs,
                            bool isFromCast /* otherwise, it's a "to" cast */) {
  std::vector<UntypedFnSignature::FormalDetail> ufsFormals;
  std::vector<QualifiedType> formalTypes;

  auto enumType = isFromCast ? lhs.type()->toEnumType() : rhs.type()->toEnumType();

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

static const TypedFnSignature*
generateCastFromEnum(Context* context,
                     const types::QualifiedType& lhs,
                     const types::QualifiedType& rhs) {
  return generateToOrFromCastForEnum(context, lhs, rhs, /*isFromCast*/ true);
}

static const TypedFnSignature*
generateCastToEnum(Context* context,
                   const types::QualifiedType& lhs,
                   const types::QualifiedType& rhs) {
  return generateToOrFromCastForEnum(context, lhs, rhs, /*isFromCast*/ false);
}

/**
  Given a type and a UniqueString representing the name of a method,
  determine if the type needs a method with such a name to be
  generated for it, and if so, generates and returns a
  TypedFnSignature representing the generated method.

  If no method was generated, returns nullptr.
*/
const TypedFnSignature*
getCompilerGeneratedMethod(Context* context, const QualifiedType receiverType,
                           UniqueString name, bool parenless) {
  // Normalize receiverType to allow TYPE methods on c_ptr and _ddata, and to
  // otherwise use the VAR Kind. The Param* value is also stripped away to
  // reduce queries.
  auto qt = receiverType;
  bool isPtr = qt.hasTypePtr() ? qt.type()->isPtrType() : false;
  if (!(qt.isType() && isPtr)) {
    qt = QualifiedType(QualifiedType::VAR, qt.type());
  }
  return getCompilerGeneratedMethodQuery(context, qt, name, parenless);
}

static const TypedFnSignature* const&
getOrderToEnumFunction(Context* context, bool paramVersion, const EnumType* et) {
  QUERY_BEGIN(getOrderToEnumFunction, context, paramVersion, et);

  std::vector<UntypedFnSignature::FormalDetail> ufsFormals;
  std::vector<QualifiedType> formalTypes;

  ufsFormals.push_back(
      UntypedFnSignature::FormalDetail(UniqueString::get(context, "i"),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       nullptr));
  formalTypes.push_back(QualifiedType(paramVersion ?
                                      QualifiedType::PARAM :
                                      QualifiedType::DEFAULT_INTENT,
                                      AnyIntegralType::get(context)));

  ufsFormals.push_back(
      UntypedFnSignature::FormalDetail(UniqueString::get(context, "et"),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       nullptr));
  formalTypes.push_back(QualifiedType(QualifiedType::TYPE, et));

  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ et->id(),
                        /*name*/ UniqueString::get(context, "chpl__orderToEnum"),
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
getEnumToOrderFunction(Context* context, bool paramVersion, const EnumType* et) {
  QUERY_BEGIN(getEnumToOrderFunction, context, paramVersion, et);

  std::vector<UntypedFnSignature::FormalDetail> ufsFormals;
  std::vector<QualifiedType> formalTypes;

  ufsFormals.push_back(
      UntypedFnSignature::FormalDetail(UniqueString::get(context, "e"),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       nullptr));
  formalTypes.push_back(QualifiedType(paramVersion ?
                                      QualifiedType::PARAM :
                                      QualifiedType::DEFAULT_INTENT,
                                      et));

  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ et->id(),
                        /*name*/ UniqueString::get(context, "chpl__enumToOrder"),
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
                                   /* needsInstantiation */ paramVersion,
                                   /* instantiatedFrom */ nullptr,
                                   /* parentFn */ nullptr,
                                   /* formalsInstantiated */ Bitmap(),
                                   /* outerVariables */ {});

  return QUERY_END(ret);
}

const TypedFnSignature*
getCompilerGeneratedFunction(Context* context,
                             const CallInfo& ci) {
  if (ci.name() == "chpl__orderToEnum") {
    if (ci.numActuals() == 2) {
      auto& firstQt = ci.actual(0).type();
      auto& secondQt = ci.actual(1).type();

      auto secondType = secondQt.type();
      if (secondType && secondType->isEnumType()) {
        bool paramVersion = firstQt.isParam();
        return getOrderToEnumFunction(context, paramVersion, secondType->toEnumType());
      }
    }
  } else if (ci.name() == "chpl__enumToOrder") {
    if (ci.numActuals() == 1) {
      auto& firstQt = ci.actual(0).type();

      auto firstType = firstQt.type();
      if (firstType && firstType->isEnumType()) {
        bool paramVersion = firstQt.isParam();
        return getEnumToOrderFunction(context, paramVersion, firstType->toEnumType());
      }
    }
  }
  return nullptr;
}

static const TypedFnSignature* const&
getCompilerGeneratedBinaryOpQuery(Context* context,
                                  types::QualifiedType lhs,
                                  types::QualifiedType rhs,
                                  UniqueString name) {
  QUERY_BEGIN(getCompilerGeneratedBinaryOpQuery, context, lhs, rhs, name);

  const TypedFnSignature* result = nullptr;
  if (name == USTR(":")) {
    if (lhs.type() && lhs.type()->isEnumType()) {
      result = generateCastFromEnum(context, lhs, rhs);
    } else if (rhs.type() && rhs.type()->isEnumType()) {
      result = generateCastToEnum(context, lhs, rhs);
    }
  }

  return QUERY_END(result);
}

const TypedFnSignature*
getCompilerGeneratedBinaryOp(Context* context,
                             const types::QualifiedType lhs,
                             const types::QualifiedType rhs,
                             UniqueString name) {
  return getCompilerGeneratedBinaryOpQuery(context, lhs, rhs, name);
}

const BuilderResult&
buildTypeConstructor(Context* context, ID typeID) {
  QUERY_BEGIN(buildTypeConstructor, context, typeID);

  auto bld = Builder::createForGeneratedCode(context, typeID);
  auto builder = bld.get();
  auto dummyLoc = parsing::locateId(context, typeID);

  AstList formals;
  auto ct = initialTypeForTypeDecl(context, typeID)->getCompositeType();

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
  const ResolvedFields& f = fieldsForTypeDecl(context, ct,
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
      owned<AstNode> formal = Formal::build(builder, dummyLoc,
                                            /*attributeGroup=*/nullptr,
                                            fieldDecl->name(),
                                            (Formal::Intent)kind,
                                            typeExpr ? typeExpr->copy() : nullptr,
                                            initExpr ? initExpr->copy() : nullptr);
      formals.push_back(std::move(formal));
    }
  }

  auto genFn = Function::build(builder, dummyLoc, {},
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

  builder->noteChildrenLocations(genFn.get(), dummyLoc);
  builder->addToplevelExpression(std::move(genFn));

  // Finalize the uAST and obtain the BuilderResult
  auto res = builder->result();

  return QUERY_END(res);
}

const BuilderResult*
builderResultForDefaultFunction(Context* context,
                                UniqueString typePath,
                                UniqueString name) {
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
  } else if (typeID.symbolName(context) == name) {
    return &buildTypeConstructor(context, typeID);
  }

  return nullptr;
}


} // end namespace resolution
} // end namespace chpl
