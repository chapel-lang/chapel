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

  if (isNameOfCompilerGeneratedMethod(name) ||
      (type->isRecordType() && !isBuiltinTypeOperator(name))) {
    if (!areOverloadsPresentInDefiningScope(context, type, QualifiedType::INIT_RECEIVER, name)) {
      return true;
    }
  }

  // Some basic getter methods for domain properties
  //
  // TODO: We can eventually replace these for calls on a domain *value* by
  // looking at the property from the _instance implementation. But that won't
  // work if we want to support these methods on a domain type-expression.
  //
  // TODO: calling these within a method doesn't work
  if (type->isDomainType()) {
    if (parenless) {
      if (name == "idxType" || name == "rank" || name == "stridable" ||
          name == "parSafe") {
        return true;
      }
    } else {
      if (name == "isRectangular" || name == "isAssociative") {
        return true;
      }
    }
  } else if (type->isArrayType()) {
    if (name == "domain" || name == "eltType") {
      return true;
    }
  } else if (type->isCPtrType()) {
    if (name == "eltType") {
      return true;
    }
  } else if (type->isEnumType()) {
    if (name == "size") {
      return true;
    }
  }

  return false;
}

// generates the untyped function signature and typed function
// signature formal entries for the 'this' method receiver for
// some kind of 'init' or 'init='
static void
generateInitParts(Context* context,
                  const CompositeType* inCompType,
                  const CompositeType*& compType,
                  std::vector<UntypedFnSignature::FormalDetail>& ufsFormals,
                  std::vector<QualifiedType>& formalTypes,
                  bool useGeneric) {
  // adjust to refer to fully generic signature if needed
  auto genericCompType = inCompType->instantiatedFromCompositeType();
  if (useGeneric && genericCompType != nullptr) {
    compType = genericCompType;
  } else {
    compType = inCompType;
  }

  // start by adding a formal for the receiver
  auto ufsReceiver =
    UntypedFnSignature::FormalDetail(USTR("this"),
                                     UntypedFnSignature::DK_NO_DEFAULT,
                                     nullptr);
  ufsFormals.push_back(std::move(ufsReceiver));

  // Determine the receiver type and intent.
  QualifiedType qtReceiver;

  // If the receiver is a record type, just give it the 'ref' intent.
  if (compType->isRecordType() || compType->isUnionType()) {
    qtReceiver = QualifiedType(QualifiedType::REF, compType);

  // If the receiver is a basic class C, use 'const in x: borrowed C'.
  } else if (auto basic = compType->toBasicClassType()) {
    const Type* manager = nullptr;
    auto borrowedNonnilDecor =
        ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
    auto receiverType =
        ClassType::get(context, basic, manager, borrowedNonnilDecor);
    CHPL_ASSERT(receiverType);
    qtReceiver = QualifiedType(QualifiedType::CONST_IN, receiverType);

  } else {
    CHPL_ASSERT(false && "Not possible!");
  }

  formalTypes.push_back(std::move(qtReceiver));
}

static void buildInitArgs(Context* context,
                          const CompositeType* compType,
                          const ResolvedFields& rf,
                          std::vector<UntypedFnSignature::FormalDetail>& ufsFormals,
                          std::vector<QualifiedType>& formalTypes) {

  // TODO: super fields and invoking super
  if (auto basic = compType->toBasicClassType()) {
    if (auto parent = basic->parentClassType()) {
      if (!parent->isObjectType()) {
        const Type* manager = nullptr;
        auto borrowedNonnilDecor =
            ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
        auto parentReceiver =
          ClassType::get(context, parent, manager, borrowedNonnilDecor);

        // Do not add args if the parent has a user-defined initializer
        // TODO: It would be nice to be able to generate a nice error message
        //   for the user if they try and pass arguments for the parent in
        //   this case.
        if (!areOverloadsPresentInDefiningScope(context, parentReceiver, QualifiedType::INIT_RECEIVER, USTR("init"))) {
          const DefaultsPolicy defaultsPolicy = DefaultsPolicy::IGNORE_DEFAULTS;
          auto& rf = fieldsForTypeDecl(context, parent, defaultsPolicy);
          buildInitArgs(context, parent, rf, ufsFormals, formalTypes);
        }
      }
    }
  }

  // push all fields -> formals in order
  for (int i = 0; i < rf.numFields(); i++) {
    auto fieldQt = rf.fieldType(i);
    auto formalName = rf.fieldName(i);
    bool formalHasDefault = rf.fieldHasDefaultValue(i);

    const uast::Decl* formalAst =
      parsing::idToAst(context, rf.fieldDeclId(i))->toDecl();

    // A field may not have a default value. If it is default-initializable
    // then the formal should still take a default value (in this case the
    // default value is for the type, e.g., '0' for 'int'.
    // TODO: If this isn't granular enough, we can introduce a 'DefaultValue'
    // type that can be used as a sentinel.
    formalHasDefault |= isTypeDefaultInitializable(context, fieldQt.type());

    UntypedFnSignature::DefaultKind defaultKind;
    if (formalHasDefault) {
      defaultKind = UntypedFnSignature::DK_DEFAULT;
    } else if (rf.isGeneric()) {
      defaultKind = UntypedFnSignature::DK_MAYBE_DEFAULT;
    } else {
      defaultKind = UntypedFnSignature::DK_NO_DEFAULT;
    }

    auto fd = UntypedFnSignature::FormalDetail(formalName, defaultKind,
                                               formalAst);
    ufsFormals.push_back(std::move(fd));

    // for types & param, use the field kind, for values use 'in' intent
    if (fieldQt.isType() || fieldQt.isParam()) {
      formalTypes.push_back(fieldQt);
    } else {
      auto qt = QualifiedType(QualifiedType::IN, fieldQt.type());
      formalTypes.push_back(std::move(qt));
    }
  }
}

static void collectFields(const AstNode* ast,
                          std::vector<const VarLikeDecl*>& fields) {
  if (auto var = ast->toVarLikeDecl()) {
    fields.push_back(var);
  } else if (auto multi = ast->toMultiDecl()) {
    for (auto d : multi->decls()) {
      collectFields(d, fields);
    }
  } else if (auto tup = ast->toTupleDecl()) {
    for (auto d : tup->decls()) {
      collectFields(d, fields);
    }
  } else if (auto fwd = ast->toForwardingDecl()) {
    if (fwd->isDecl()) {
      collectFields(fwd->expr(), fields);
    }
  }
}

static void initHelper(Context* context,
                       Builder* builder,
                       const AggregateDecl* typeDecl,
                       const Location& dummyLoc,
                       AstList& formals, AstList& superArgs, AstList& stmts,
                       bool isChild = true) {
  if (auto cls = typeDecl->toClass()) {
    if (cls->numInheritExprs() == 1) {
      ResolutionResultByPostorderID r;
      auto visitor = Resolver::createForParentClass(context, typeDecl,
                                                    {}, nullptr, r);
      cls->inheritExpr(0)->traverse(visitor);
      auto res = r.byAst(cls->inheritExpr(0));
      if (auto parentType = res.type().type()) {
        if (auto pct = parentType->getCompositeType()) {
          const Type* manager = nullptr;
          auto borrowedNonnilDecor =
              ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
          auto parentReceiver =
            ClassType::get(context, pct->toBasicClassType(), manager, borrowedNonnilDecor);

          // Do not add formals if the parent has a user-defined initializer
          // TODO: It would be nice to be able to generate a nice error message
          //   for the user if they try and pass arguments for the parent in
          //   this case.
          if (!areOverloadsPresentInDefiningScope(context, parentReceiver, QualifiedType::INIT_RECEIVER, USTR("init"))) {
            auto parentAst = parsing::idToAst(context, pct->id());
            if (auto parentDecl = parentAst->toAggregateDecl()) {
              initHelper(context, builder, parentDecl, dummyLoc,
                         formals, superArgs, stmts, /*isChild=*/false);
            }
          }
        }
      }
    }
  }

  std::vector<const VarLikeDecl*> fields;
  for (auto d : typeDecl->decls()) {
    collectFields(d, fields);
  }

  for (auto field : fields) {
    Formal::Intent kind;
    // for types & param, use the field kind, for values use 'in' intent
    if (field->storageKind() == Qualifier::TYPE ||
        field->storageKind() == Qualifier::PARAM) {
      kind = (Formal::Intent)field->storageKind();
    } else {
      kind = Formal::Intent::IN;
    }

    auto typeExpr = field->typeExpression();
    auto initExpr = field->initExpression();

    owned<AstNode> formal = Formal::build(builder, dummyLoc,
                                          /*attributeGroup=*/nullptr,
                                          field->name(), kind,
                                          typeExpr ? typeExpr->copy() : nullptr,
                                          initExpr ? initExpr->copy() : nullptr);

    if (isChild) {
      // Create 'this.field = arg;' statement
      owned<AstNode> lhs = Dot::build(builder, dummyLoc,
                                      Identifier::build(builder, dummyLoc, USTR("this")),
                                      field->name());
      owned<AstNode> rhs = Identifier::build(builder, dummyLoc, field->name());
      owned<AstNode> assign = OpCall::build(builder, dummyLoc, USTR("="),
                                            std::move(lhs), std::move(rhs));
      stmts.push_back(std::move(assign));
    } else {
      // collect arguments for super.init(...)
      owned<AstNode> arg = Identifier::build(builder, dummyLoc, field->name());
      superArgs.push_back(std::move(arg));
    }

    formals.push_back(std::move(formal));
  }
}

static const BuilderResult& buildInitializer(Context* context, ID typeID) {
  auto typeDecl = parsing::idToAst(context, typeID)->toAggregateDecl();
  auto parentMod = parsing::idToParentModule(context, typeID);
  auto modName = "chpl__generated_" + parentMod.symbolName(context).str() + "_" + typeDecl->name().str() + "_init";
  auto bld = Builder::createForGeneratedCode(context, modName.c_str(), parentMod, parentMod.symbolPath());
  auto builder = bld.get();
  auto dummyLoc = parsing::locateId(context, typeID);

  auto thisType = Identifier::build(builder, dummyLoc, typeDecl->name());
  auto thisFormal = Formal::build(builder, dummyLoc, nullptr,
                                  USTR("this"), Formal::DEFAULT_INTENT,
                                  std::move(thisType), nullptr);

  AstList formals;
  AstList stmts;
  AstList superArgs;
  initHelper(context, builder, typeDecl, dummyLoc, formals, superArgs, stmts);

  if (auto cls = typeDecl->toClass()) {
    if (cls->numInheritExprs() > 0) {
      owned<AstNode> dot = Dot::build(builder, dummyLoc, Identifier::build(builder, dummyLoc, USTR("super")), USTR("init"));
      owned<AstNode> call = FnCall::build(builder, dummyLoc, std::move(dot), std::move(superArgs), false);
      stmts.insert(stmts.begin(), std::move(call));
    }
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

  auto modPath = result.topLevelExpression(0)->id().symbolPath();
  parsing::setCompilerGeneratedBuilder(context, modPath, std::move(result));

  auto& br = parsing::getCompilerGeneratedBuilder(context, modPath);

  return br;
}

static const TypedFnSignature*
generateInitSignature(Context* context, const CompositeType* inCompType) {
  const TypedFnSignature* result;

  if (auto ct = inCompType->getCompositeType()->toBasicClassType()) {
    if (ct->isObjectType()) {
      return nullptr;
    }
  }

  // Old-style default function placeholder for certain types when the standard
  // library isn't available.
  if (CompositeType::isMissingBundledType(context, inCompType->id())) {
    const CompositeType* compType = nullptr;
    std::vector<UntypedFnSignature::FormalDetail> ufsFormals;
    std::vector<QualifiedType> formalTypes;
    AstList formals;

    generateInitParts(context, inCompType, compType,
                      ufsFormals, formalTypes, /*useGeneric*/ true);
    // consult the fields to build up the remaining untyped formals
    const DefaultsPolicy defaultsPolicy = DefaultsPolicy::IGNORE_DEFAULTS;
    auto& rf = fieldsForTypeDecl(context, compType, defaultsPolicy);

    // Add field-based arguments to initializer, including those of parent class
    // if present.
    buildInitArgs(context, compType, rf, ufsFormals, formalTypes);

    // build the untyped signature
    auto ufs = UntypedFnSignature::get(context,
                          /*id*/ inCompType->id(),
                          /*name*/ USTR("init"),
                          /*isMethod*/ true,
                          /*isTypeConstructor*/ false,
                          /*isCompilerGenerated*/ true,
                          /*throws*/ false,
                          /*idTag*/ parsing::idToTag(context, compType->id()),
                          /*kind*/ uast::Function::Kind::PROC,
                          /*formals*/ std::move(ufsFormals),
                          /*whereClause*/ nullptr,
                          /*compilerGeneratedOrigin=*/compType->id());

    // now build the other pieces of the typed signature
    bool needsInstantiation = rf.isGeneric();

    result = TypedFnSignature::get(context,
                                   ufs,
                                   std::move(formalTypes),
                                   TypedFnSignature::WHERE_NONE,
                                   needsInstantiation,
                                   /* instantiatedFrom */ nullptr,
                                   /* parentFn */ nullptr,
                                   /* formalsInstantiated */ Bitmap(),
                                   /* outerVariables */ {});
  } else {
    auto& br = buildInitializer(context, inCompType->id());

    const Module* genMod = br.topLevelExpression(0)->toModule();
    auto initFn = genMod->child(genMod->numChildren()-1)->toFunction();

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
    result = typedSignatureInitial(&rcval, uSig);
  }

  return result;
}

static const TypedFnSignature*
generateInitCopySignature(Context* context, const CompositeType* inCompType) {
  const CompositeType* compType = nullptr;
  std::vector<UntypedFnSignature::FormalDetail> ufsFormals;
  std::vector<QualifiedType> formalTypes;

  generateInitParts(context, inCompType, compType,
                    ufsFormals, formalTypes, /*useGeneric*/ false);

  // add a formal for the 'other' argument
  auto name = UniqueString::get(context, "other");
  auto defaultKind = UntypedFnSignature::DK_NO_DEFAULT;
  const uast::Decl* node = nullptr;

  auto fd = UntypedFnSignature::FormalDetail(name, defaultKind, node);
  ufsFormals.push_back(std::move(fd));

  CHPL_ASSERT(formalTypes.size() == 1);
  auto otherType = QualifiedType(QualifiedType::CONST_REF,
                                 formalTypes[0].type());
  formalTypes.push_back(std::move(otherType));

  // build the untyped signature
  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ compType->id(),
                        /*name*/ USTR("init="),
                        /*isMethod*/ true,
                        /*isTypeConstructor*/ false,
                        /*isCompilerGenerated*/ true,
                        /*throws*/ false,
                        /*idTag*/ parsing::idToTag(context, compType->id()),
                        /*kind*/ uast::Function::Kind::PROC,
                        /*formals*/ std::move(ufsFormals),
                        /*whereClause*/ nullptr);

  // now build the other pieces of the typed signature
  auto ret = TypedFnSignature::get(context,
                                   ufs,
                                   std::move(formalTypes),
                                   TypedFnSignature::WHERE_NONE,
                                   /*needsInstantiation*/ false,
                                   /* instantiatedFrom */ nullptr,
                                   /* parentFn */ nullptr,
                                   /* formalsInstantiated */ Bitmap(),
                                   /* outerVariables */ {});

  return ret;
}

static const TypedFnSignature*
generateDeinitSignature(Context* context, const CompositeType* inCompType) {
  const CompositeType* compType = nullptr;
  std::vector<UntypedFnSignature::FormalDetail> ufsFormals;
  std::vector<QualifiedType> formalTypes;

  generateInitParts(context, inCompType, compType,
                    ufsFormals, formalTypes, /*useGeneric*/ false);

  // build the untyped signature
  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ compType->id(),
                        /*name*/ USTR("deinit"),
                        /*isMethod*/ true,
                        /*isTypeConstructor*/ false,
                        /*isCompilerGenerated*/ true,
                        /*throws*/ false,
                        /*idTag*/ parsing::idToTag(context, compType->id()),
                        /*kind*/ uast::Function::Kind::PROC,
                        /*formals*/ std::move(ufsFormals),
                        /*whereClause*/ nullptr);

  // now build the other pieces of the typed signature
  auto ret = TypedFnSignature::get(context,
                                   ufs,
                                   std::move(formalTypes),
                                   TypedFnSignature::WHERE_NONE,
                                   /*needsInstantiation*/ false,
                                   /* instantiatedFrom */ nullptr,
                                   /* parentFn */ nullptr,
                                   /* formalsInstantiated */ Bitmap(),
                                   /* outerVariables */ {});

  return ret;
}

static const TypedFnSignature*
generateDeSerialize(Context* context, const CompositeType* compType,
                    UniqueString name, std::string channel,
                    std::string deSerializer) {
  std::vector<UntypedFnSignature::FormalDetail> ufsFormals;
  std::vector<QualifiedType> formalTypes;

  ufsFormals.push_back(
      UntypedFnSignature::FormalDetail(USTR("this"),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       nullptr));

  QualifiedType thisType;
  if (compType->isBasicClassType()) {
    const Type* manager = nullptr;
    auto borrowedNonnilDecor =
        ClassTypeDecorator(ClassTypeDecorator::BORROWED_NONNIL);
    auto receiverType =
        ClassType::get(context, compType->toBasicClassType(), manager, borrowedNonnilDecor);
    thisType = QualifiedType(QualifiedType::CONST_IN, receiverType);
  } else {
    thisType =QualifiedType(QualifiedType::CONST_REF, compType);
  }
  formalTypes.push_back(thisType);

  // TODO: Add constraints to these arguments
  ufsFormals.push_back(
      UntypedFnSignature::FormalDetail(UniqueString::get(context, channel),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       nullptr));
  formalTypes.push_back(QualifiedType(QualifiedType::CONST_REF, AnyType::get(context)));

  ufsFormals.push_back(
      UntypedFnSignature::FormalDetail(UniqueString::get(context, deSerializer),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       nullptr));
  formalTypes.push_back(QualifiedType(QualifiedType::REF, AnyType::get(context)));

  // build the untyped signature
  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ compType->id(),
                        /*name*/ name,
                        /*isMethod*/ true,
                        /*isTypeConstructor*/ false,
                        /*isCompilerGenerated*/ true,
                        /*throws*/ true,
                        /*idTag*/ parsing::idToTag(context, compType->id()),
                        /*kind*/ uast::Function::Kind::PROC,
                        /*formals*/ std::move(ufsFormals),
                        /*whereClause*/ nullptr);

  // now build the other pieces of the typed signature
  auto ret = TypedFnSignature::get(context,
                                   ufs,
                                   std::move(formalTypes),
                                   TypedFnSignature::WHERE_NONE,
                                   /*needsInstantiation*/ false,
                                   /* instantiatedFrom */ nullptr,
                                   /* parentFn */ nullptr,
                                   /* formalsInstantiated */ Bitmap(),
                                   /* outerVariables */ {});

  return ret;
}

static const TypedFnSignature*
generateDomainMethod(Context* context,
                     const DomainType* dt,
                     UniqueString name) {
  // Build a basic function signature for methods querying some aspect of
  // a domain's type.
  // TODO: we should really have a way to just set the return type here
  const TypedFnSignature* result = nullptr;
  std::vector<UntypedFnSignature::FormalDetail> formals;
  std::vector<QualifiedType> formalTypes;

  formals.push_back(
      UntypedFnSignature::FormalDetail(USTR("this"),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       nullptr));
  formalTypes.push_back(QualifiedType(QualifiedType::CONST_REF, dt));

  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ dt->id(),
                        /*name*/ name,
                        /*isMethod*/ true,
                        /*isTypeConstructor*/ false,
                        /*isCompilerGenerated*/ true,
                        /*throws*/ false,
                        /*idTag*/ parsing::idToTag(context, dt->id()),
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
generateCPtrMethod(Context* context, QualifiedType receiverType,
                   UniqueString name) {
  // Build a basic function signature for methods on a cptr
  // TODO: we should really have a way to just set the return type here
  const CPtrType* cpt = receiverType.type()->toCPtrType();
  const TypedFnSignature* result = nullptr;
  std::vector<UntypedFnSignature::FormalDetail> formals;
  std::vector<QualifiedType> formalTypes;

  formals.push_back(
      UntypedFnSignature::FormalDetail(USTR("this"),
                                       UntypedFnSignature::DK_NO_DEFAULT,
                                       nullptr));

  // Allow calling 'eltType' on either a type or value
  auto qual = receiverType.isType() ? QualifiedType::TYPE : QualifiedType::CONST_REF;
  formalTypes.push_back(QualifiedType(qual, cpt));

  auto ufs = UntypedFnSignature::get(context,
                        /*id*/ cpt->id(context),
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
    CHPL_ASSERT(compType || type->isCPtrType() || type->isEnumType());

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
    } else if (auto domainType = type->toDomainType()) {
      result = generateDomainMethod(context, domainType, name);
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
    } else if (type->isCPtrType()) {
      result = generateCPtrMethod(context, receiverType, name);
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
  // Normalize recieverType to allow TYPE methods on c_ptr, and to otherwise
  // use the VAR Kind. The Param* value is also stripped away to reduce
  // queries.
  auto qt = receiverType;
  bool isCPtr = qt.hasTypePtr() ? qt.type()->isCPtrType() : false;
  if (!(qt.isType() && isCPtr)) {
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

static const BuilderResult&
buildAssignmentOperators(Context* context,
                         QualifiedType lhs, QualifiedType rhs) {
  std::stringstream ss;
  lhs.type()->stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
  auto typeName = ss.str();

  auto modName = "chpl__generated_" + typeName + "_=";
  auto baseName = UniqueString::get(context, "ChapelBase");
  auto bld = Builder::createForGeneratedCode(context, modName.c_str(), ID(baseName, ID_GEN_START, 0), UniqueString::get(context, "ChapelBase"));
  auto builder = bld.get();
  auto dummyLoc = Location(UniqueString::get(context, "ChapelBase.="));

  auto lhsFormal = Formal::build(builder, dummyLoc, nullptr,
                                 UniqueString::get(context, "lhs"),
                                 Formal::REF, nullptr, nullptr);
  auto rhsFormal = Formal::build(builder, dummyLoc, nullptr,
                                 UniqueString::get(context, "rhs"),
                                 Formal::CONST, nullptr, nullptr);
  AstList formals;
  formals.push_back(std::move(lhsFormal));
  formals.push_back(std::move(rhsFormal));

  AstList stmts;
  auto body = Block::build(builder, dummyLoc, std::move(stmts));
  auto genFn = Function::build(builder,
                               dummyLoc, {},
                               Decl::Visibility::PUBLIC,
                               Decl::Linkage::DEFAULT_LINKAGE,
                               /*linkageName=*/{},
                               USTR("="),
                               /*inline=*/false, /*override=*/false,
                               Function::Kind::OPERATOR,
                               /*receiver=*/nullptr,
                               Function::ReturnIntent::DEFAULT_RETURN_INTENT,
                               // throws, primaryMethod, parenless
                               false, false, false,
                               std::move(formals),
                               // returnType, where, lifetime, body
                               {}, {}, {}, std::move(body));

  builder->noteChildrenLocations(genFn.get(), dummyLoc);
  builder->addToplevelExpression(std::move(genFn));

  auto result = builder->result();

  auto modPath = result.topLevelExpression(0)->id().symbolPath();
  parsing::setCompilerGeneratedBuilder(context, modPath, std::move(result));

  auto& br = parsing::getCompilerGeneratedBuilder(context, modPath);

  return br;
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
  } else if (name == USTR("=") &&
             lhs.type() == rhs.type() &&
             (lhs.type()->isPrimitiveType() ||
              lhs.type()->isTupleType() ||
              lhs.type()->isClassType())) {

    auto& br = buildAssignmentOperators(context, lhs, rhs);
    auto assignFn = br.topLevelExpression(0)->child(0)->toFunction();

    auto lhsType = QualifiedType(QualifiedType::REF, lhs.type());
    auto rhsType = QualifiedType(QualifiedType::CONST_IN, rhs.type());
    std::vector<QualifiedType> formalTypes = {lhsType, rhsType};

    auto lhsDet =
        UntypedFnSignature::FormalDetail(UniqueString::get(context, "lhs"),
                                         UntypedFnSignature::DK_NO_DEFAULT, assignFn->formal(0));
    auto rhsDet =
        UntypedFnSignature::FormalDetail(UniqueString::get(context, "rhs"),
                                       UntypedFnSignature::DK_NO_DEFAULT, assignFn->formal(1));
    std::vector<UntypedFnSignature::FormalDetail> ufsFormals = {lhsDet, rhsDet};

    auto ufs = UntypedFnSignature::get(context,
                          /*id*/ assignFn->id(),
                          /*name*/ USTR("="),
                          /*isMethod*/ false,
                          /*isTypeConstructor*/ false,
                          /*isCompilerGenerated*/ true,
                          /*throws*/ false,
                          /*idTag*/ asttags::Function,
                          /*kind*/ uast::Function::Kind::OPERATOR,
                          /*formals*/ std::move(ufsFormals),
                          /*whereClause*/ nullptr);

    result = TypedFnSignature::get(context,
                                   ufs,
                                   std::move(formalTypes),
                                   TypedFnSignature::WHERE_NONE,
                                   /* needsInstantiation */ false,
                                   /* instantiatedFrom */ nullptr,
                                   /* parentFn */ nullptr,
                                   /* formalsInstantiated */ Bitmap(),
                                   /* outerVariables */ {});
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


} // end namespace resolution
} // end namespace chpl
