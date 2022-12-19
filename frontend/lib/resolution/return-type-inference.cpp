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

#include "return-type-inference.h"

#include "chpl/framework/ErrorMessage.h"
#include "chpl/framework/UniqueString.h"
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

#include "Resolver.h"
#include "call-init-deinit.h"
#include "default-functions.h"
#include "prims.h"
#include "signature-checks.h"

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


// Get a Type for an AggregateDecl
// poiScope, instantiatedFrom are nullptr if not instantiating
const CompositeType* helpGetTypeForDecl(Context* context,
                                        const AggregateDecl* ad,
                                        const SubstitutionsMap& substitutions,
                                        const PoiScope* poiScope,
                                        const Type* instantiatedFrom) {

  CHPL_ASSERT(ad);

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
        Resolver::createForParentClass(context, c,
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
        CHPL_ASSERT(false && "unexpected instantiatedFrom type");
    }


    if (!parentClassType->isObjectType() && !substitutions.empty()) {
      // recompute the parent class type with substitutions
      auto parentAst = parsing::idToAst(context, parentClassType->id());
      CHPL_ASSERT(parentAst);
      auto parentAd = parentAst->toAggregateDecl();
      CHPL_ASSERT(parentAd);
      auto got = helpGetTypeForDecl(context,
                                    parentAd,
                                    substitutions,
                                    poiScope,
                                    parentClassType);
      auto gotBct = got->toBasicClassType();
      CHPL_ASSERT(gotBct);
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
        CHPL_ASSERT(false && "unexpected instantiatedFrom type");
    }

    ret = RecordType::get(context, r->id(), r->name(),
                          insnFromRec, std::move(filteredSubs));

  } else if (auto u = ad->toUnion()) {
    const UnionType* insnFromUni = nullptr;
    if (instantiatedFrom != nullptr) {
      if (auto uni = instantiatedFrom->toUnionType())
        insnFromUni = uni;
      else
        CHPL_ASSERT(false && "unexpected instantiatedFrom type");
    }

    ret = UnionType::get(context, u->id(), u->name(),
                         insnFromUni, std::move(filteredSubs));

  } else {
    CHPL_ASSERT(false && "case not handled");
  }

  return ret;
}

struct ReturnTypeInferrer {
  // input
  Context* context;
  const AstNode* astForErr;
  Function::ReturnIntent returnIntent;
  const ResolutionResultByPostorderID& resolutionById;

  // output
  std::vector<QualifiedType> returnedTypes;

  ReturnTypeInferrer(Context* context,
                     const AstNode* astForErr,
                     const ResolvedFunction& resolvedFn)
    : context(context),
      astForErr(astForErr),
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
    returnedTypes.push_back(std::move(qt));
  }

  QualifiedType returnedType() {
    if (returnedTypes.size() == 0) {
      return QualifiedType(QualifiedType::CONST_VAR, VoidType::get(context));
    } else {
      auto retType = commonType(context, returnedTypes,
                                (QualifiedType::Kind) returnIntent);
      if (!retType) {
        // Couldn't find common type, so return type is incorrect.
        context->error(astForErr, "could not determine return type for function");
        retType = QualifiedType(QualifiedType::UNKNOWN, ErroneousType::get(context));
      }
      return retType.getValue();
    }
  }

  bool enter(const Conditional* cond) {
    auto condition = cond->condition();
    CHPL_ASSERT(condition != nullptr);
    const ResolvedExpression& r = resolutionById.byAst(condition);
    if (r.type().isParamTrue()) {
      auto then = cond->thenBlock();
      CHPL_ASSERT(then != nullptr);
      then->traverse(*this);
      return false;
    } else if (r.type().isParamFalse()) {
      auto else_ = cond->elseBlock();
      if (else_) {
        else_->traverse(*this);
      }
      return false;
    }
    return true;
  }
  void exit(const Conditional* cond){
  }

  bool enter(const Return* ret) {
    if (const AstNode* expr = ret->value()) {
      noteReturnType(expr, ret);
      if (const Function* fn = astForErr->toFunction()) {
        if (fn->name() == "init" && fn->isMethod()) {
          context->error(ret, "initializers can only return 'void'");
        }
      }
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
      CHPL_ASSERT(t);

      // ignore decorators etc for finding instantiatedFrom
      if (auto ct = t->toClassType())
        t = ct->basicClassType();

      instantiatedFrom = t->toCompositeType();
      CHPL_ASSERT(instantiatedFrom);
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
    CHPL_ASSERT(false && "case not handled");
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
                                          DefaultsPolicy::IGNORE_DEFAULTS);
    int n = fields.numFields();
    for (int i = 0; i < n; i++) {
      if (fields.fieldDeclId(i) == fieldId) {
        return fields.fieldType(i);
      }
    }
  }

  CHPL_ASSERT(false && "should not be reachable");
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
    CHPL_ASSERT(fn);
    if (const AstNode* retType = fn->returnType()) {
      // resolve the return type
      ResolutionResultByPostorderID resolutionById;
      auto visitor = Resolver::createForFunction(context, fn, poiScope, sig,
                                                 resolutionById);
      retType->traverse(visitor);
      result = resolutionById.byAst(retType).type();
    } else {
      // resolve the function body
      const ResolvedFunction* rFn = resolveFunction(context, sig, poiScope);
      // infer the return type
      ReturnTypeInferrer visitor(context, fn, *rFn);
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
      CHPL_ASSERT(tt == vt); // this should already be done in return type inference
      result = QualifiedType(kind, vt);
    }

  } else if (untyped->isTypeConstructor()) {
    const Type* t = returnTypeForTypeCtorQuery(context, sig, poiScope);

    // for a 'class C' declaration, the above query returns a BasicClassType,
    // but 'C' normally means a generic-management non-nil C
    // so adjust the result.
    if (untyped->idIsClass()) {
      auto bct = t->toBasicClassType();
      CHPL_ASSERT(bct);
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
      CHPL_ASSERT(false && "unhandled compiler-generated method");
    }
  } else {
    CHPL_ASSERT(false && "case not handled");
  }

  return QUERY_END(result);
}

static const TypedFnSignature* const&
inferOutFormalsQuery(Context* context,
                     const TypedFnSignature* sig,
                     const PoiScope* instantiationPoiScope) {
  QUERY_BEGIN(inferOutFormalsQuery, context, sig, instantiationPoiScope);

  const UntypedFnSignature* untyped = sig->untyped();

  std::vector<types::QualifiedType> formalTypes;

  // resolve the function body
  const ResolvedFunction* rFn = resolveFunction(context, sig,
                                                instantiationPoiScope);
  const ResolutionResultByPostorderID& rr = rFn->resolutionById();

  int numFormals = sig->numFormals();
  for (int i = 0; i < numFormals; i++) {
    const types::QualifiedType& ft = sig->formalType(i);
    if (ft.kind() == QualifiedType::OUT && ft.isGenericOrUnknown()) {
      formalTypes.push_back(rr.byAst(untyped->formalDecl(i)).type());
    } else {
      formalTypes.push_back(ft);
    }
  }

  const TypedFnSignature* result = nullptr;
  result = TypedFnSignature::getInferred(context,
                                         std::move(formalTypes),
                                         sig);

  return QUERY_END(result);
}

const TypedFnSignature* inferOutFormals(Context* context,
                                        const TypedFnSignature* sig,
                                        const PoiScope* instantiationPoiScope) {
  if (sig == nullptr) {
    return nullptr;
  }

  bool anyGenericOutFormals = false;
  int numFormals = sig->numFormals();
  for (int i = 0; i < numFormals; i++) {
    const types::QualifiedType& ft = sig->formalType(i);
    if (ft.kind() == QualifiedType::OUT && ft.isGenericOrUnknown()) {
      anyGenericOutFormals = true;
      break;
    }
  }

  // if there are no 'out' formals with generic type, just return 'sig'.
  // also just return 'sig' if the function needs instantiation;
  // in that case, we can't infer the 'out' formals by resolving the body.
  if (anyGenericOutFormals && !sig->needsInstantiation()) {
    return inferOutFormalsQuery(context, sig, instantiationPoiScope);
  } else {
    return sig;
  }
}


} // end namespace resolution
} // end namespace chpl
