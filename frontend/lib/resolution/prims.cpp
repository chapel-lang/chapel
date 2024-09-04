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

#include "prims.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"
#include "chpl/framework/ErrorBase.h"
#include "chpl/resolution/can-pass.h"
#include "chpl/util/version-info.h"
#include "chpl/framework/query-impl.h"

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

static const CompositeType* toCompositeTypeActual(const QualifiedType& type,
                                                  bool shouldBeType = true) {
  if ((type.kind() == QualifiedType::TYPE) == shouldBeType) {
    if (auto t = type.type()) {
      if (auto ct = t->getCompositeType()) {
        return ct;
      }
    }
  }
  return nullptr;
}

static const ResolvedFields*
toCompositeTypeActualFields(Context* context,
                            const QualifiedType& type,
                            bool shouldBeType = true) {
  if (auto ct = toCompositeTypeActual(type, shouldBeType)) {
    auto& resolvedFields = fieldsForTypeDecl(context, ct,
                                             DefaultsPolicy::IGNORE_DEFAULTS);
    return &resolvedFields;
  }
  return nullptr;
}

static bool toParamIntActual(const QualifiedType& type, int64_t& into) {
  if (type.kind() == QualifiedType::PARAM) {
    if (auto t = type.type()) {
      if (t->isIntType()) {
        if (auto p = type.param()) {
          if (auto ip = p->toIntParam()) {
            into = ip->value();
            return true;
          }
        }
      }
    }
  }
  return false;
}

static bool paramStringBytesHelper(const QualifiedType& type,
                                   UniqueString& into, bool isString) {
  if (type.kind() == QualifiedType::PARAM) {
    if (auto t = type.type()) {
      if ((t->isBytesType() && !isString) ||
          (t->isStringType() && isString)) {
        if (auto p = type.param()) {
          if (auto sp = p->toStringParam()) {
            into = sp->value();
            return true;
          }
        }
      }
    }
  }
  return false;
}

static bool toParamBytesActual(const QualifiedType& type, UniqueString& into) {
 return paramStringBytesHelper(type, into, false);
}

static bool toParamStringActual(const QualifiedType& type, UniqueString& into) {
  return paramStringBytesHelper(type, into, true);
}

static QualifiedType makeParamBool(Context* context, bool b) {
  return { QualifiedType::PARAM, BoolType::get(context),
           BoolParam::get(context, b) };
}

static QualifiedType makeParamString(Context* context, UniqueString s) {
  return { QualifiedType::PARAM, RecordType::getStringType(context),
           StringParam::get(context, s) };
}

static QualifiedType makeParamString(Context* context, std::string s) {
  auto ustr = UniqueString::get(context, s);
  return makeParamString(context, ustr);
}

static QualifiedType primIsBound(Context* context, const CallInfo& ci) {
  auto type = QualifiedType();
  if (ci.numActuals() != 2) return type;

  auto firstActual = ci.actual(0).type();
  auto secondActual = ci.actual(1).type();
  if (auto fields = toCompositeTypeActualFields(context, firstActual)) {
    UniqueString fieldName;
    if (!toParamStringActual(secondActual, fieldName)) return type;

    for (int i = 0; i < fields->numFields(); i++) {
      if (fields->fieldName(i) != fieldName) continue;
      // Production compiler only reports a field as unbound if it's
      // generic and doesn't have a substitution. In other words,
      // a field instantiated with a generic type is "bound". Dyno
      // treats these fields as still-generic, so this primitive
      // will only return true if the field's type is concrete.
      auto isBound =
        fields->fieldType(i).genericity() == Type::Genericity::CONCRETE;
      type = makeParamBool(context, isBound);
      break;
    }
  }
  return type;
}

static QualifiedType primNumFields(Context* context, const CallInfo& ci) {
  auto type = QualifiedType();
  if (ci.numActuals() != 1) return type;

  auto firstActual = ci.actual(0).type();
  if (auto fields = toCompositeTypeActualFields(context, firstActual)) {
    int64_t numFields = fields->numFields();
    type = QualifiedType(QualifiedType::PARAM,
                         IntType::get(context, 64),
                         IntParam::get(context, numFields));
  }
  return type;
}

static QualifiedType primFieldNumToName(Context* context, const CallInfo& ci) {
  auto type = QualifiedType();
  if (ci.numActuals() != 2) return type;

  auto firstActual = ci.actual(0).type();
  auto secondActual = ci.actual(1).type();
  if (auto fields = toCompositeTypeActualFields(context, firstActual)) {
    int64_t fieldNum = 0;
    if (!toParamIntActual(secondActual, fieldNum)) return type;
    // Fields in these primitives are 1-indexed.
    if (fieldNum > fields->numFields() || fieldNum < 1) return type;

    auto fieldName = fields->fieldName(fieldNum - 1);
    type = QualifiedType(QualifiedType::PARAM,
                         RecordType::getStringType(context),
                         StringParam::get(context, fieldName));
  }
  return type;
}

static QualifiedType primFieldNameToNum(Context* context, const CallInfo& ci) {
  auto type = QualifiedType();
  if (ci.numActuals() != 2) return type;

  auto firstActual = ci.actual(0).type();
  auto secondActual = ci.actual(1).type();
  if (auto fields = toCompositeTypeActualFields(context, firstActual)) {
    UniqueString fieldName;
    if (!toParamStringActual(secondActual, fieldName)) return type;
    bool foundField = false;
    int field = 0;

    // TODO move this into a method on fields?
    for (int i = 0; i < fields->numFields(); i++) {
      if (fields->fieldName(i) == fieldName) {
        foundField = true;
        // Fields in these primitives are 1-indexed.
        field = i + 1;
        break;
      }
    }

    if (!foundField) return type;
    type = QualifiedType(QualifiedType::PARAM,
                         IntType::get(context, 64),
                         IntParam::get(context, field));
  }
  return type;
}

static QualifiedType primFieldByNum(Context* context, const CallInfo& ci) {
  if (ci.numActuals() != 2) return QualifiedType();

  auto firstActual = ci.actual(0).type();
  auto secondActual = ci.actual(1).type();
  auto fields = toCompositeTypeActualFields(context,
                                            firstActual,
                                            /* shouldBeType */ false);
  if (!fields) return QualifiedType();
  int64_t fieldNum = 0;
  if (!toParamIntActual(secondActual, fieldNum)) return QualifiedType();

  // Fields in these primitives are 1-indexed.
  if (fieldNum > fields->numFields() || fieldNum < 1) return QualifiedType();
  return fields->fieldType(fieldNum - 1);;
}

static QualifiedType primCallResolves(Context* context, const CallInfo &ci,
                                      bool forMethod, bool resolveFn,
                                      const PrimCall* call,
                                      const CallScopeInfo& inScopes) {
  if ((forMethod && ci.numActuals() < 2) ||
      (!forMethod && ci.numActuals() < 1)) {
    return QualifiedType();
  }

  size_t fnNameActual = forMethod ? 1 : 0;
  UniqueString fnName;
  if (!toParamStringActual(ci.actual(fnNameActual).type(), fnName)) {
    return QualifiedType();
  }

  bool callAndFnResolved = false;
  std::vector<CallInfoActual> actuals;
  if (forMethod) {
    actuals.push_back(CallInfoActual(ci.actual(0).type(), USTR("this")));
  }
  for (size_t i = fnNameActual + 1; i < ci.numActuals(); i++) {
    actuals.push_back(ci.actual(i));
  }
  auto callInfo = CallInfo(fnName,
                           /* calledType */ QualifiedType(),
                           /* isMethodCall */ forMethod,
                           /* hasQuestionArg */ false,
                           /* isParenless */ false,
                           std::move(actuals));
  auto callResult = context->runAndTrackErrors([&](Context* context) {
    return resolveGeneratedCall(context, call, callInfo,
                                inScopes);
  });
  const TypedFnSignature* bestCandidate = nullptr;
  for (auto candidate : callResult.result().mostSpecific()) {
    if (candidate) {
      bestCandidate = candidate.fn();
      break;
    }
  }

  if (bestCandidate != nullptr) {
    callAndFnResolved = callResult.ranWithoutErrors();

    if (resolveFn) {
      // We did find a candidate; resolve the function body.
      auto bodyResult = context->runAndTrackErrors([&](Context* context) {
        return resolveFunction(context, bestCandidate, inScopes.poiScope());
      });
      callAndFnResolved &= bodyResult.ranWithoutErrors();
    }
  }

  return QualifiedType(QualifiedType::PARAM,
                       BoolType::get(context),
                       BoolParam::get(context, callAndFnResolved));
}

static QualifiedType computeDomainType(Context* context, const CallInfo& ci) {
  if (ci.numActuals() == 3) {
    auto type = DomainType::getRectangularType(context,
                                          ci.actual(0).type(),
                                          ci.actual(1).type(),
                                          ci.actual(2).type());
    return QualifiedType(QualifiedType::TYPE, type);
  } else if (ci.numActuals() == 2) {
    auto type = DomainType::getAssociativeType(context,
                                               ci.actual(0).type(),
                                               ci.actual(1).type());
    return QualifiedType(QualifiedType::TYPE, type);
  } else {
    CHPL_ASSERT(false && "unhandled domain type?");
  }
  return QualifiedType();
}

static QualifiedType primAddrOf(Context* context, const CallInfo& ci) {
  if (ci.numActuals() != 1) return QualifiedType();

  auto actualQt = ci.actual(0).type();
  auto kp = KindProperties::fromKind(actualQt.kind());

  // Combine the properties of the argument's kind with those of the 'REF'
  // kind. This should inherit const-ness, throw off param-ness, and result in
  // errors if the argument is a TYPE.
  kp.combineWithJoin(KindProperties::fromKind(QualifiedType::REF));
  if (!kp.valid()) return QualifiedType();

  // 'combineWith' actually disables ref-ness if either argument is non-ref.
  // Insist that we do really want a reference.
  kp.setRef(true);
  return QualifiedType(kp.toKind(), actualQt.type());
}

static QualifiedType primGpuAllocShared(Context* context, const CallInfo& ci) {
  if (ci.numActuals() != 1) return QualifiedType();

  auto firstActualType = ci.actual(0).type();
  if (!firstActualType.type() || !firstActualType.type()->isIntType() ||
      firstActualType.kind() != QualifiedType::PARAM || !firstActualType.param()) {
    return QualifiedType();
  }

  return QualifiedType(QualifiedType::CONST_VAR, CPtrType::getCVoidPtrType(context));
}

static QualifiedType primGpuSetBlockSize(Context* context, const CallInfo& ci) {
  if (ci.numActuals() != 1) return QualifiedType();

  auto firstActualType = ci.actual(0).type();
  if (!firstActualType.type() || !firstActualType.type()->isIntegralType()) {
    return QualifiedType();
  }

  return QualifiedType(QualifiedType::CONST_VAR, VoidType::get(context));
}

static QualifiedType primAssertOnGpu(Context* context, const CallInfo& ci) {
  if (ci.numActuals() != 1) return QualifiedType();

  auto firstActualType = ci.actual(0).type();
  if (!firstActualType.type() || !firstActualType.type()->isBoolType() ||
      firstActualType.kind() != QualifiedType::PARAM || !firstActualType.param()) {
    return QualifiedType();
  }

  return QualifiedType(QualifiedType::CONST_VAR, VoidType::get(context));
}

static QualifiedType primTypeof(Context* context, PrimitiveTag prim, const CallInfo& ci) {
  if (ci.numActuals() != 1) return QualifiedType();

  auto actualQt = ci.actual(0).type();

  if ((actualQt.isType() && prim == PRIM_TYPEOF)) {
    // PRIM_TYPEOF (intended to behave like .type) is only allowed on types.
    // On the other hand, PRIM_STATIC_TYPEOF is applied to types on many occasions
    // in the compiler code, so it does not cause an error here.
    return QualifiedType();
  }

  auto typePtr = actualQt.type();
  if (!typePtr) return QualifiedType();

  if (auto tupleType = typePtr->toTupleType()) {
    typePtr = tupleType->toValueTuple(context);
  }

  return QualifiedType(QualifiedType::TYPE, typePtr);
}

static QualifiedType primGetSvecMember(Context* context, PrimitiveTag prim,
                                       const CallInfo& ci) {
  CHPL_ASSERT(prim == PRIM_GET_SVEC_MEMBER ||
              prim == PRIM_GET_SVEC_MEMBER_VALUE);

  if (ci.numActuals() == 2 && ci.actual(0).type().hasTypePtr()) {
    auto index = ci.actual(1).type();
    if (index.hasTypePtr() && index.type()->isIntegralType()) {
      auto act = ci.actual(0).type();
      if (auto tup = act.type()->toTupleType()) {
        if (tup->isStarTuple()) {
          auto eltType = tup->starType();
          QualifiedType::Kind retKind = eltType.kind();
          if (prim == PRIM_GET_SVEC_MEMBER_VALUE) {
            // Return as value for _VALUE variant, maintaining constness.
            retKind =
                KindProperties::combineKindsMeet(retKind, QualifiedType::PARAM);
          }
          return QualifiedType(retKind, eltType.type());
        }
      }
    }
  }

  return QualifiedType();
}

static QualifiedType staticFieldType(Context* context, const CallInfo& ci) {
  // Note: this is slightly different semantically from the primitive in
  // production. In production owned(X) is a type of its own (aliasing _owned(X)),
  // so it would have the fields of _owned accessed by the primitive. Meanwhile,
  // in Dyno, an owned(X) will have the fields of X accessed by this primitive.

  if (ci.numActuals() != 2) return QualifiedType();

  auto typeActualQt = ci.actual(0).type();
  auto fieldActualQt = ci.actual(1).type();

  if (!typeActualQt.type() ||
      !fieldActualQt.isParam() || !fieldActualQt.type() || !fieldActualQt.param()) {
    return QualifiedType();
  }

  auto compositeType = typeActualQt.type()->getCompositeType();
  auto fieldNameParam = fieldActualQt.param()->toStringParam();

  if (!compositeType || !fieldNameParam) {
    return QualifiedType();
  }

  auto& fields = fieldsForTypeDecl(context, compositeType, DefaultsPolicy::IGNORE_DEFAULTS);
  for (int i = 0; fields.numFields(); i++) {
    if (fields.fieldName(i) == fieldNameParam->value()) {
      auto returnType = fields.fieldType(i).type();
      return QualifiedType(QualifiedType::TYPE, returnType);
    }
  }
  return QualifiedType();
}

struct TestFunctionFinder {
  // Inputs
  Context* context;
  const QualifiedType& testType;

  // Outputs
  std::vector<const Function*> fns;

  TestFunctionFinder(Context* context, const QualifiedType& testType)
    : context(context), testType(testType) {}

  bool enter(const Function* fn) {
    // We're looking for a non-method function that throws and accepts
    // a single argument (which we will soon validate to be of the testType).
    if (!fn->throws() || fn->isMethod() || fn->numFormals() != 1) return false;
    // Skip non-user functions.
    if (parsing::idIsInBundledModule(context, fn->id())) return false;

    // The function also needs to be concrete.
    const UntypedFnSignature* uSig = UntypedFnSignature::get(context, fn);
    const TypedFnSignature* sig = typedSignatureInitial(context, uSig);
    if (sig == nullptr || sig->needsInstantiation()) return false;

    if (canPass(context, testType, sig->formalType(0)).passes()) {
      // One formal of 'testType', which constitutes a test.
      fns.push_back(fn);
    }

    return false;
  }
  void exit(const Function* fn) {}

  bool enter(const AstNode* node) { return true; }
  void exit(const AstNode* node) {}
};

static const std::vector<const Function*>& gatheredTestsQuery(Context* context) {
  QUERY_BEGIN_INPUT(gatheredTestsQuery, context);
  std::vector<const Function*> toReturn;
  return QUERY_END(toReturn);
}

const std::vector<const uast::Function*>& getTestsGatheredViaPrimitive(Context* context) {
  return gatheredTestsQuery(context);
}

static QualifiedType primGatherTests(Context* context, const CallInfo& ci) {
  if (ci.numActuals() != 1) return QualifiedType();

  auto testType = ci.actual(0).type();
  if (testType.isUnknown() || testType.isErroneousType())
    return QualifiedType();

  TestFunctionFinder finder(context, testType);
  auto topLevelAsts = parsing::introspectParsedTopLevelExpressions(context);
  for (auto ast : topLevelAsts) {
    ast->traverse(finder);
  }

  QUERY_STORE_INPUT_RESULT(gatheredTestsQuery, context, finder.fns);
  auto numFoundFns = (int) gatheredTestsQuery(context).size();

  return QualifiedType(QualifiedType::PARAM,
                       IntType::get(context, 0),
                       IntParam::get(context, numFoundFns));
}

static QualifiedType primIsTuple(Context* context,
                                 const CallInfo& ci) {
  if (ci.numActuals() != 1) return QualifiedType();
  auto& actualType = ci.actual(0).type();
  if (actualType.kind() != QualifiedType::TYPE) return QualifiedType();

  bool isTupleType = actualType.type() && actualType.type()->isTupleType();
  return QualifiedType(QualifiedType::PARAM,
                       BoolType::get(context),
                       BoolParam::get(context, isTupleType));
}

static QualifiedType primCast(Context* context,
                              const CallInfo& ci) {
  if (ci.numActuals() != 2) return QualifiedType();

  auto& castTo = ci.actual(0).type();
  auto& castFrom = ci.actual(1).type();

  if (!castTo.isType()) return QualifiedType();

  // Note: the production compiler also handles wide classes here (if
  // cast-from is a wide class, turn cast-to into a wide class). We don't
  // currently track wide classes in Dyno so this logic is omitted here.

  return QualifiedType(castFrom.kind(), castTo.type(), castFrom.param());
}

static QualifiedType convertClassToDecorator(Context* context,
                                             const PrimCall* call,
                                             const CallInfo& ci,
                                             ClassTypeDecorator::ClassTypeDecoratorEnum cde,
                                             bool checked) {
  if (ci.numActuals() != 1) return QualifiedType();

  auto& actualType = ci.actual(0).type();
  auto typePtr = actualType.type();
  if (!typePtr) return QualifiedType();

  // The checked primitives only allow a 'type' as the actual.
  if (!actualType.isType() && checked) {
    return CHPL_TYPE_ERROR(context, InvalidClassCast, call, actualType);
  }

  const ManageableType* manageableType = nullptr;
  auto decorator = ClassTypeDecorator(cde);

  if (auto ct = typePtr->toClassType()) {
    manageableType = ct->manageableType();
    decorator = decorator.copyNilabilityFrom(ct->decorator());
  } else if (auto mt = typePtr->toManageableType()) {
    manageableType = mt;
  } else if (typePtr->isAnySharedType() || typePtr->isAnyOwnedType()) {
    // 'shared' and 'owned' are just thrown out if we convert to a decorator.
    manageableType = AnyClassType::get(context);
  }


  if (manageableType) {
    return QualifiedType(actualType.kind(),
                         ClassType::get(context, manageableType, nullptr, decorator));
  } else if (checked) {
    // It wasn't a class-like type, which is also not allowed by the checked
    // primitives.
    return CHPL_TYPE_ERROR(context, InvalidClassCast, call, actualType);
  }
  return actualType;
}

static QualifiedType primToUnmanagedClass(Context* context,
                                          const PrimCall* call,
                                          const CallInfo& ci,
                                          bool checked) {
  return convertClassToDecorator(context, call, ci,
                                 ClassTypeDecorator::UNMANAGED, checked);
}

static QualifiedType primToUndecoratedClass(Context* context,
                                            const PrimCall* call,
                                            const CallInfo& ci,
                                            bool checked) {
  return convertClassToDecorator(context, call, ci,
                                 ClassTypeDecorator::GENERIC, checked);
}

static QualifiedType primToBorrowedClass(Context* context,
                                         const PrimCall* call,
                                         const CallInfo& ci,
                                         bool checked) {
  return convertClassToDecorator(context, call, ci,
                                 ClassTypeDecorator::BORROWED, checked);
}

static QualifiedType setClassNilability(Context* context,
                                        const PrimCall* call,
                                        const CallInfo& ci,
                                        bool nilability,
                                        bool checked) {
  if (ci.numActuals() != 1) return QualifiedType();

  auto& actualType = ci.actual(0).type();
  auto typePtr = actualType.type();
  if (!typePtr) return QualifiedType();

  // The checked primitives only allow a 'type' as the actual.
  if (!actualType.isType() && checked) {
    return CHPL_TYPE_ERROR(context, InvalidClassCast, call, actualType);
  }

  const ManageableType* manageableType = nullptr;
  const Type* manager = nullptr;
  optional<ClassTypeDecorator::ClassTypeDecoratorEnum> cde;

  if (auto ct = typePtr->toClassType()) {
    cde = ct->decorator().val();
    manager = ct->manager();
    manageableType = ct->manageableType();
  } else if (auto mt = typePtr->toManageableType()) {
    cde = ClassTypeDecorator::BORROWED;
    manageableType = mt;
  } else if (typePtr->isAnySharedType() || typePtr->isAnyOwnedType()) {
    // convert 'shared' into 'shared class?', which has the desired semantics.
    cde = ClassTypeDecorator::MANAGED;
    manager = typePtr;
    manageableType = AnyClassType::get(context);
  }

  if (cde) {
    auto decorator = ClassTypeDecorator(*cde);
    auto newDecorator = nilability ? decorator.addNilable() : decorator.addNonNil();
    const Type* newType = ClassType::get(context, manageableType, manager, newDecorator);
    return QualifiedType(actualType.kind(), newType);
  } else if (checked) {
    // It wasn't a class-like type, which is also not allowed by the checked
    // primitives.
    return CHPL_TYPE_ERROR(context, InvalidClassCast, call, actualType);
  }

  return actualType;
}

static QualifiedType primFamilyIsSubtype(Context* context,
                                         const PrimCall* call,
                                         const CallInfo& ci) {
  if (ci.numActuals() != 2) return QualifiedType();

  auto prim = call->prim();
  auto& parentQT = ci.actual(0).type();
  auto& subQT = ci.actual(1).type();

  bool parentIsType = parentQT.isType();
  bool subIsType = subQT.isType();

  // TODO: more specific error reporting
  if (prim == PRIM_IS_INSTANTIATION_ALLOW_VALUES) {
    // At least one type needed for PRIM_IS_INSTANTIATION_ALLOW_VALUES.
    if (!parentIsType && !subIsType) return QualifiedType();
  } else {
    // Both need to be types for other primitives in this family.
    if (!parentIsType || !subIsType) return QualifiedType();
  }

  // Note: omitted here is the special logic for distributions
  // (if parent is a distribution class, retrieve the child distribution's
  // _instance). It's unclear if we need this logic in Dyno.

  auto newParentQT = QualifiedType(QualifiedType::TYPE, parentQT.type());
  auto newSubQT = QualifiedType(QualifiedType::TYPE, subQT.type());

  auto cpr = canPass(context, newSubQT, newParentQT);
  bool result = false;
  if (prim == PRIM_IS_INSTANTIATION_ALLOW_VALUES) {
    // We allow the type to be the same OR require instantiation; thus,
    // it's sufficient to check if no conversion occurs (both instantiates()
    // and !instantiates() are allowed).
    result = cpr.passes() && !cpr.converts() && !cpr.promotes();
  } else {
    // TODO: Don't count borrowing conversion as implying subtype, since that's
    // not what the spec does.
    bool isSubType = cpr.passes() &&
                     (cpr.conversionKind() == CanPassResult::NONE ||
                      cpr.conversionKind() == CanPassResult::SUBTYPE ||
                      cpr.conversionKind() == CanPassResult::BORROWS ||
                      cpr.conversionKind() == CanPassResult::BORROWS_SUBTYPE);
    if (prim == PRIM_IS_SUBTYPE) {
      result = isSubType;
    } else {
      CHPL_ASSERT(prim == PRIM_IS_PROPER_SUBTYPE);
      // TODO: Analogous to the above, don't count a non-subtype borrowing
      // conversion as a proper subtype.
      result = isSubType && newSubQT != newParentQT;
    }
  }

  return QualifiedType(QualifiedType::PARAM, BoolType::get(context),
                       BoolParam::get(context, result));
}

static QualifiedType primToNilableClass(Context* context,
                                        const PrimCall* call,
                                        const CallInfo& ci,
                                        bool checked) {
  return setClassNilability(context, call, ci, /* nilability */ true, checked);
}

static QualifiedType primToNonNilableClass(Context* context,
                                           const PrimCall* call,
                                           const CallInfo& ci,
                                           bool checked) {
  return setClassNilability(context, call, ci, /* nilability */ false, checked);
}

template <typename FromParam, typename ToParam,
          typename FromCppType, typename ToCppType>
QualifiedType primNumericTransmute(Context* context,
                                   const PrimCall* call,
                                   const CallInfo& ci,
                                   const Type* sourceType,
                                   const Type* targetType) {
  static_assert(sizeof(FromCppType) == sizeof(ToCppType),
                "types for transmutation must have the same size");
  if (call->numActuals() != 1) return QualifiedType();

  auto& fromQt = ci.actual(0).type();
  if (fromQt.type() != sourceType) return QualifiedType();

  const Param* param = nullptr;
  if (auto fromParam = fromQt.param()) {
    // Note: assuming that the param matches the type here, because we can't
    // call a toWhateverParam. Perform an "unsafe" cast, which should be
    // safe if the param QT is well-formed.
    FromCppType fromVal = (FromCppType) ((const FromParam*) fromParam)->value();
    ToCppType toVal;
    memcpy(&toVal, &fromVal, sizeof(ToCppType));
    param = ToParam::get(context, (ToCppType) toVal);
  }

  return QualifiedType(fromQt.kind(), targetType, param);
}

static QualifiedType primRealToInt(Context* context, const CallInfo& ci) {
  if (ci.numActuals() != 1) return QualifiedType();

  auto argType = ci.actual(0).type();
  auto argTypePtr = argType.type();

  if (!argTypePtr->isRealType()) return QualifiedType();

  return QualifiedType(argType.kind(), IntType::get(context, 64));
}

static QualifiedType primObjectToInt(Context* context, const CallInfo& ci) {
  if (ci.numActuals() != 1) return QualifiedType();

  auto argType = ci.actual(0).type();
  auto argTypePtr = argType.type();

  if (!argTypePtr->isClassType() && !argTypePtr->isBasicClassType())
    return QualifiedType();

  return QualifiedType(argType.kind(), IntType::get(context, 64));
}

/*
  for get real/imag primitives
*/
static QualifiedType
primComplexGetComponent(Context* context, const CallInfo& ci) {
  QualifiedType ret = QualifiedType();

  if (ci.numActuals() != 1) return ret;

  if (auto comp = ci.actual(0).type().type()->toComplexType()) {
    int w = comp->componentBitwidth();
    ret = QualifiedType(QualifiedType::REF, RealType::get(context, w));
  }
  return ret;
}

/* for abs */
static QualifiedType
primAbsGetType(Context* context, const CallInfo& ci) {
  QualifiedType ret = QualifiedType();

  if (ci.numActuals() != 1) return ret;

  // Note: the PRIM_ABS primitive is only actually used for params,
  // so this code should probably assert that.
  QualifiedType actualType = ci.actual(0).type();
  if (auto comp = actualType.type()->toComplexType()) {
    int w = comp->componentBitwidth();
    ret = QualifiedType(QualifiedType::CONST_VAR, RealType::get(context, w));
  } else if (auto img = actualType.type()->toImagType()) {
    int w = img->bitwidth();
    ret = QualifiedType(QualifiedType::CONST_VAR, RealType::get(context, w));
  } else {
    // otherwise just use the original type
    ret = QualifiedType(QualifiedType::CONST_VAR, actualType.type());
  }
  return ret;
}

static QualifiedType primFamilyCopyableAssignable(Context* context,
                                                  const CallInfo& ci,
                                                  const PrimitiveTag prim) {
  const bool checkCopyable =
      (prim == PRIM_IS_COPYABLE || prim == PRIM_IS_CONST_COPYABLE);
  const bool checkAssignable =
      (prim == PRIM_IS_ASSIGNABLE || prim == PRIM_IS_CONST_ASSIGNABLE);
  CHPL_ASSERT((checkCopyable || checkAssignable) &&
              "incorrect primitive for this handler");

  if (ci.numActuals() != 1) return QualifiedType();
  auto t = ci.actual(0).type().type();

  auto info = getCopyOrAssignableInfo(context, t, checkCopyable);

  // copyable/assignable from const is stricter than from ref
  const bool isFromRefOk =
      (prim == PRIM_IS_COPYABLE || prim == PRIM_IS_ASSIGNABLE);
  const bool isCopyableOrAssignable =
      info.isFromConst() || (info.isFromRef() && isFromRefOk);

  return QualifiedType(QualifiedType::PARAM, BoolType::get(context),
                       BoolParam::get(context, isCopyableOrAssignable));
}

// TODO: What should be done for 'MAYBE_GENERIC', if anything?
static QualifiedType primIsGenericType(Context* context, const CallInfo& ci) {
  if (ci.numActuals() < 1) return QualifiedType();
  bool eval = false;
  auto qt = ci.actual(0).type();
  if (auto t = qt.type()) {
    auto g = t->genericity();

    if (g == Type::MAYBE_GENERIC) {
      QualifiedType qtEval = qt;
      auto ct = t->toCompositeType();
      if (auto cls = t->toClassType()) ct = cls->basicClassType();

      // A generic type with defaults will be computed to have default subs
      // at this point, so calling 'getTypeGenericity' on it will produce
      // 'CONCRETE' instead of 'GENERIC_WITH_DEFAULTS'. Back up and use the
      // uninstantiated type to produce the latter.
      if (ct) {
        if (auto ctGeneric = ct->instantiatedFromCompositeType()) {
          qtEval = QualifiedType(qt.kind(), ctGeneric);
        }
      }
      g = getTypeGenericity(context, qtEval);
    }

    // Both cases are considered 'generic' for this primitive.
    eval = (g == Type::GENERIC || g == Type::GENERIC_WITH_DEFAULTS);
  }
  return makeParamBool(context, eval);
}


static QualifiedType primIsClassType(Context* context, const CallInfo& ci) {
  if (ci.numActuals() < 1) return QualifiedType();
  bool eval = false;
  if (auto t = ci.actual(0).type().type()) {
    const bool isClassLike =
        t->isClassType()      ||  // E.g., 'owned C' or 'owned class'.
        t->isBasicClassType() ||  // TODO: This probably shouldn't appear.
        t->isAnyClassType();      // E.g., 'class'.
    // TODO: No to extern types.
    const bool isExtern = false;
    const bool isDdata = t->hasPragma(context, PRAGMA_DATA_CLASS);
    eval = isClassLike && !isExtern && !isDdata;
  }
  return makeParamBool(context, eval);
}

template <typename F>
static QualifiedType
actualTypeHasProperty(Context* context, const CallInfo& ci, F&& f) {
  if (ci.numActuals() < 1) return QualifiedType();
  bool eval = false;
  if (auto t = ci.actual(0).type().type()) eval = f(t);
  return makeParamBool(context, eval);
}

static QualifiedType
primIsNilableClassType(Context* context, const CallInfo& ci) {
  return actualTypeHasProperty(context, ci, [](auto t) {
    auto mt = t->toClassType();
    return mt ? mt->decorator().isNilable() : false;
  });
}

static QualifiedType
primIsNonNilableClassType(Context* context, const CallInfo& ci) {
  return actualTypeHasProperty(context, ci, [](auto t) {
    auto mt = t->toClassType();
    return mt ? mt->decorator().isNonNilable() : false;
  });
}

static QualifiedType primIsRecordType(Context* context, const CallInfo& ci) {
  return actualTypeHasProperty(context, ci, [](auto t) {
    return t->isUserRecordType();
  });
}

static QualifiedType primIsFcfType(Context* context, const CallInfo& ci) {
  CHPL_UNIMPL("PRIM_IS_FCF_TYPE");
  return makeParamBool(context, false);
}

static QualifiedType primIsUnionType(Context* context, const CallInfo& ci) {
  return actualTypeHasProperty(context, ci, [](auto t) {
    return t->isUnionType();
  });
}

static QualifiedType primIsExternType(Context* context, const CallInfo& ci) {
  return actualTypeHasProperty(context, ci, [=](auto t) {
    if (t->isExternType()) return true;
    auto ct = t->getCompositeType();
    return ct ? parsing::idIsExtern(context, ct->id()) : false;
  });
}

static QualifiedType
primIsExternUnionType(Context* context, const CallInfo& ci) {
  auto qt1 = primIsExternType(context, ci);
  auto qt2 = primIsUnionType(context, ci);
  const bool eval = qt1.isParamTrue() && qt2.isParamTrue();
  return makeParamBool(context, eval);
}

static QualifiedType
primIsAtomicType(Context* context, const CallInfo& ci) {
  return actualTypeHasProperty(context, ci, [=](auto t) {
    return t->hasPragma(context, PRAGMA_ATOMIC_TYPE);
  });
}

static QualifiedType
primIsRefIterType(Context* context, const CallInfo& ci) {
  CHPL_UNIMPL("PRIM_IS_REF_ITER_TYPE");
  return QualifiedType();
}

static QualifiedType
primIsBorrowedClassType(Context* context, const CallInfo& ci) {
  return actualTypeHasProperty(context, ci, [](auto t) {
    auto mt = t->toClassType();
    return mt ? mt->decorator().isBorrowed() : false;
  });
}

static QualifiedType
primIsAbsEnumType(Context* context, const CallInfo& ci) {
  return actualTypeHasProperty(context, ci, [](auto t) {
    auto et = t->toEnumType();
    return et ? et->isAbstract() : false;
  });
}

static QualifiedType primIsPod(Context* context, const CallInfo& ci) {
  return actualTypeHasProperty(context, ci, [=](auto t) {
    return Type::isPod(context, t);
  });
}

static QualifiedType
primIsCoercible(Context* context, const CallInfo& ci) {
  if (ci.numActuals() < 2) return QualifiedType();
  auto qtFrom = ci.actual(0).type();
  auto qtTo = ci.actual(1).type();
  auto canPass = CanPassResult::canPass(context, qtFrom, qtTo);
  bool eval = canPass.passes() &&
              (canPass.instantiates() || canPass.converts()) &&
              !canPass.promotes();
  return makeParamBool(context, eval);
}

static std::string typeToString(Context* context, const Type* t);

static void
doRegularCompositeTypeToString(Context* context, const CompositeType* ct,
                               const ClassType* asClassType,
                               std::stringstream& ss) {
  bool shouldAddNilableQuestionMark = false;

  // Handle class management and check for nilability.
  if (auto cls = asClassType) {
    auto d = cls->decorator();
    shouldAddNilableQuestionMark = d.isNilable();
    if (d.isBorrowed()) {
      ss << "borrowed ";
    } else if (d.isUnmanaged()) {
      ss << "unmanaged ";
    } else if (d.isManaged()) {
      auto m = cls->manager();
      CHPL_ASSERT(m);
      // Just print the manager type name (they're all builtins for now).
      ss << typeToString(context, cls->manager()) << " ";
    }
  }

  ss << ct->name().c_str();

  if (shouldAddNilableQuestionMark) ss << "?";

  // Recursively print substitutions.
  if (!ct->substitutions().empty()) {
    auto sortedSubs = ct->sortedSubstitutions();
    ss << "(";
    for (size_t i = 0; i < sortedSubs.size(); i++) {
      auto& qt = sortedSubs[i].second;
      ss << typeToString(context, qt.type());
      const bool last = (i+1) == sortedSubs.size();
      if (!last) ss << ", ";
    }
    ss << ")";
  }
}

static std::string typeToString(Context* context, const Type* t) {
  std::stringstream ss;

  // TODO: Should stringify be able to handle everything?
  const bool canSimpleStringify = t->isPrimitiveType() ||
                                  t->isBuiltinType() ||
                                  t->isStringType() ||
                                  t->isBytesType() ||
                                  t->isEnumType() ||
                                  t->isAnyType();

  if (canSimpleStringify) {
    t->stringify(ss, CHPL_SYNTAX);

  } else if (t->isRecordType() && !t->isUserRecordType()) {
    CHPL_UNIMPL("Record-wrapped language types");

  } else if (auto ct = t->getCompositeType()) {
    doRegularCompositeTypeToString(context, ct, t->toClassType(), ss);

  } else {
    CHPL_UNIMPL("Unimplemented type-to-string");
  }

  auto ret = ss.str();
  return ret;
}

static QualifiedType
primTypeToString(Context* context, const CallInfo& ci) {
  if (ci.numActuals() < 1) return QualifiedType();
  std::string eval;
  if (auto t = ci.actual(0).type().type()) {
    eval = typeToString(context, t);
  }
  return makeParamString(context, eval);
}

static QualifiedType
primSimpleTypeName(Context* context, const CallInfo& ci) {
  if (ci.numActuals() < 1) return QualifiedType();
  std::string eval;
  if (auto t = ci.actual(0).type().type()) {
    const Type* root = t;
    if (auto ct = t->toCompositeType()) {
      if (auto ins = ct->instantiatedFromCompositeType()) root = ins;
    }
    eval = typeToString(context, root);
  }
  return makeParamString(context, eval);
}

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

  // handle param folding
  auto prim = call->prim();
  if (Param::isParamOpFoldable(prim) && allParam) {
    if (ci.numActuals() == 2) {
      type = Param::fold(context, call, prim, ci.actual(0).type(), ci.actual(1).type());
    } else if (ci.numActuals() == 1) {
      type = Param::fold(context, call, prim, ci.actual(0).type(), QualifiedType());
    } else {
      CHPL_ASSERT(false && "unsupported param folding");
    }
    return CallResolutionResult(candidates, type, poi, /* specially handled */ true);
  }

  // otherwise, handle each primitive individually
  using namespace uast::primtags;
  switch (prim) {
    /* resolution queries */
    case PRIM_IS_SUBTYPE:
    case PRIM_IS_INSTANTIATION_ALLOW_VALUES:
    case PRIM_IS_PROPER_SUBTYPE:
      type = primFamilyIsSubtype(context, call, ci);
      break;

    case PRIM_IS_BOUND:
      type = primIsBound(context, ci);
      break;

    case PRIM_IS_COERCIBLE:
      type = primIsCoercible(context, ci);
      break;

    case PRIM_TYPE_TO_STRING:
      type = primTypeToString(context, ci);
      break;

    case PRIM_HAS_LEADER:
      CHPL_UNIMPL("PRIM_HAS_LEADER");
      break;

    case PRIM_IS_TUPLE_TYPE:
      type = primIsTuple(context, ci);
      break;

    case PRIM_SIMPLE_TYPE_NAME:
      type = primSimpleTypeName(context, ci);
      break;

    case PRIM_NUM_FIELDS:
      type = primNumFields(context, ci);
      break;

    case PRIM_FIELD_NUM_TO_NAME:
      type = primFieldNumToName(context, ci);
      break;

    case PRIM_FIELD_NAME_TO_NUM:
      type = primFieldNameToNum(context, ci);
      break;

    case PRIM_FIELD_BY_NUM:
      type = primFieldByNum(context, ci);
      break;

    case PRIM_IS_COPYABLE:
    case PRIM_IS_CONST_COPYABLE:
    case PRIM_IS_ASSIGNABLE:
    case PRIM_IS_CONST_ASSIGNABLE:
      type = primFamilyCopyableAssignable(context, ci, prim);
      break;

    case PRIM_ITERATOR_RECORD_FIELD_VALUE_BY_FORMAL:
      CHPL_UNIMPL("PRIM_ITERATOR_RECORD_FIELD_VALUE_BY_FORMAL");
      break;

    case PRIM_IS_GENERIC_TYPE:
      type = primIsGenericType(context, ci);
      break;

    case PRIM_IS_CLASS_TYPE:
      type = primIsClassType(context, ci);
      break;

    case PRIM_IS_NILABLE_CLASS_TYPE:
      type = primIsNilableClassType(context, ci);
      break;

    case PRIM_IS_NON_NILABLE_CLASS_TYPE:
      type = primIsNonNilableClassType(context, ci);
      break;

    case PRIM_IS_RECORD_TYPE:
      type = primIsRecordType(context, ci);
      break;

    case PRIM_IS_FCF_TYPE:
      type = primIsFcfType(context, ci);
      break;

    case PRIM_IS_UNION_TYPE:
      type = primIsUnionType(context, ci);
      break;

    case PRIM_IS_EXTERN_UNION_TYPE:
      type = primIsExternUnionType(context, ci);
      break;

    case PRIM_IS_ATOMIC_TYPE:
      type = primIsAtomicType(context, ci);
      break;

    case PRIM_IS_REF_ITER_TYPE:
      type = primIsRefIterType(context, ci);
      break;

    case PRIM_IS_EXTERN_TYPE:
      type = primIsExternType(context, ci);
      break;

    case PRIM_IS_BORROWED_CLASS_TYPE:
      type = primIsBorrowedClassType(context, ci);
      break;

    case PRIM_IS_ABS_ENUM_TYPE:
      type = primIsAbsEnumType(context, ci);
      break;

    case PRIM_IS_POD:
      type = primIsPod(context, ci);
      break;

    case PRIM_HAS_DEFAULT_VALUE:
    case PRIM_NEEDS_AUTO_DESTROY:
      CHPL_UNIMPL("various primitives");
      break;

    case PRIM_CALL_RESOLVES:
    case PRIM_CALL_AND_FN_RESOLVES:
    case PRIM_METHOD_CALL_AND_FN_RESOLVES:
    case PRIM_METHOD_CALL_RESOLVES: {
        bool forMethod = prim == PRIM_METHOD_CALL_RESOLVES ||
                         prim == PRIM_METHOD_CALL_AND_FN_RESOLVES;
        bool resolveFn = prim == PRIM_CALL_AND_FN_RESOLVES ||
                         prim == PRIM_METHOD_CALL_AND_FN_RESOLVES;

        auto inScopes = CallScopeInfo::forNormalCall(inScope, inPoiScope);
        type = primCallResolves(context, ci, forMethod, resolveFn, call, inScopes);
      }
      break;

    case PRIM_RESOLVES:
    case PRIM_IMPLEMENTS_INTERFACE:
      CHPL_UNIMPL("various primitives");
      break;

    case PRIM_IS_STAR_TUPLE_TYPE:
      if (ci.numActuals() == 1) {
        bool result = false;
        if (auto t = ci.actual(0).type().type())
          if (auto tt = t->toTupleType())
            result = tt->isStarTuple();

        type = QualifiedType(QualifiedType::PARAM,
                             BoolType::get(context),
                             BoolParam::get(context, result));
      }
      break;

    /* cast-like things */
    case PRIM_CAST:
    case PRIM_DYNAMIC_CAST:
      type = primCast(context, ci);
      break;

    case PRIM_TO_LEADER:
    case PRIM_TO_FOLLOWER:
    case PRIM_TO_STANDALONE:
      CHPL_UNIMPL("iterator casting  primitives");
      break;

    case PRIM_CAST_TO_VOID_STAR:
      if (ci.numActuals() == 1) {
        type = QualifiedType(ci.actual(0).type().kind(),
                             CPtrType::getCVoidPtrType(context));
      }
      break;

    case PRIM_CAST_TO_TYPE:
      // Note: the implementation isn't the same in production, but seems to
      // match what we do in Dyno for primCast.
      type = primCast(context, ci);
      break;

    case PRIM_REAL_TO_INT:
      type = primRealToInt(context, ci);
      break;

    case PRIM_OBJECT_TO_INT:
      type = primObjectToInt(context, ci);

    case PRIM_COERCE:
      CHPL_UNIMPL("coerce primitive");
      break;

    case PRIM_TO_UNMANAGED_CLASS_CHECKED:
    case PRIM_TO_UNMANAGED_CLASS:
      type = primToUnmanagedClass(context, call, ci,
                                  /* checked */ prim == PRIM_TO_UNMANAGED_CLASS_CHECKED);
      break;

    case PRIM_TO_BORROWED_CLASS_CHECKED:
    case PRIM_TO_BORROWED_CLASS:
      type = primToBorrowedClass(context, call, ci,
                                 /* checked */ prim == PRIM_TO_BORROWED_CLASS_CHECKED);
      break;

    case PRIM_TO_UNDECORATED_CLASS:
      type = primToUndecoratedClass(context, call, ci, /* checked */ false);
      break;

    case PRIM_TO_NILABLE_CLASS_CHECKED:
    case PRIM_TO_NILABLE_CLASS:
      type = primToNilableClass(context, call, ci,
                                /* checked */ prim == PRIM_TO_NILABLE_CLASS_CHECKED);
      break;

    case PRIM_TO_NON_NILABLE_CLASS:
      type = primToNonNilableClass(context, call, ci, /* checked */ false);
      break;

    case PRIM_UINT32_AS_REAL32:
      type = primNumericTransmute<UintParam, RealParam, uint32_t, float>(
          context, call, ci, UintType::get(context, 32), RealType::get(context, 32));
      break;

    case PRIM_UINT64_AS_REAL64:
      type = primNumericTransmute<UintParam, RealParam, uint64_t, double>(
          context, call, ci, UintType::get(context, 64), RealType::get(context, 64));
      break;

    case PRIM_REAL32_AS_UINT32:
      type = primNumericTransmute<RealParam, UintParam, float, uint32_t>(
          context, call, ci, RealType::get(context, 32), UintType::get(context, 32));
      break;

    case PRIM_REAL64_AS_UINT64:
      type = primNumericTransmute<RealParam, UintParam, double, uint64_t>(
          context, call, ci, RealType::get(context, 64), UintType::get(context, 64));
      break;

    /* string operations */
    case PRIM_STRING_COMPARE:
    case PRIM_STRING_CONTAINS:
    case PRIM_STRING_LENGTH_BYTES:
    {
      if (ci.numActuals() > 0) {
        UniqueString sParam;
        auto& actualType = ci.actual(0).type();
        if (toParamStringActual(actualType, sParam)||
            toParamBytesActual(actualType, sParam)) {
          const size_t s = sParam.length();
          type = QualifiedType(QualifiedType::PARAM,
                               IntType::get(context, 0),
                               IntParam::get(context, s));
          break;
        } else if (actualType.type()->isStringType() ||
                   actualType.type()->isBytesType() ||
                   actualType.type()->isCStringType() ||
                   actualType.type()->isCPtrType()) {
          // for non-param string/bytes, the return type is just a default int
          type = QualifiedType(QualifiedType::CONST_VAR,
                               IntType::get(context, 0));
          break;
        }
      }
    }
    case PRIM_STRING_CONCAT:
    {
      if (ci.numActuals() == 2) {
        auto lhs = ci.actual(0).type();
        auto rhs = ci.actual(1).type();

        if (lhs.type() == rhs.type() &&
            lhs.isParam() && rhs.isParam() &&
            (lhs.type()->isStringType() || lhs.type()->isBytesType())) {
          auto lstr = lhs.param()->toStringParam()->value();
          auto rstr = rhs.param()->toStringParam()->value();
          auto concat = UniqueString::getConcat(context, lstr.c_str(), rstr.c_str());
          type = QualifiedType(QualifiedType::PARAM, lhs.type(), StringParam::get(context, concat));
        }
      }
      break;
    }
    case PRIM_STRING_LENGTH_CODEPOINTS:
    case PRIM_ASCII:
    case PRIM_STRING_ITEM:
    case PRIM_BYTES_ITEM:
    case PRIM_STRING_INDEX:
    case PRIM_STRING_COPY:
    case PRIM_STRING_SELECT:

    /* primitives that always return bool */
    case PRIM_EQUAL:
      if (ci.actual(0).type().isType() && ci.actual(1).type().isType()) {
        bool isEqual = ci.actual(0).type().type() == ci.actual(1).type().type();
        type = QualifiedType(QualifiedType::PARAM,
                             BoolType::get(context),
                             BoolParam::get(context, isEqual));
        break;
      }
    case PRIM_IS_WIDE_PTR:
    case PRIM_NOTEQUAL:
    case PRIM_LESSOREQUAL:
    case PRIM_GREATEROREQUAL:
    case PRIM_LESS:
    case PRIM_GREATER:
    case PRIM_UNARY_LNOT:
    case PRIM_TESTCID:
    case PRIM_LOGICAL_AND:
    case PRIM_LOGICAL_OR:
    case PRIM_GET_SERIAL:
    case PRIM_PTR_EQUAL:
    case PRIM_PTR_NOTEQUAL:
    case PRIM_BLOCK_C_FOR_LOOP:
      type = QualifiedType(QualifiedType::CONST_VAR,
                           BoolType::get(context));
      break;

    /* Primitives that return the type of the 1st operand.
       Could use a more complex rule (e.g. compute numeric
       size that can contain both actuals) but using the type
       of the 1st operand is sufficient if they are only ever
       called from within Chapel wrappers. */
    case PRIM_RETURN:
    case PRIM_THROW:
    case PRIM_TRY_EXPR:
    case PRIM_TRYBANG_EXPR:
    case PRIM_YIELD:
    case PRIM_UNARY_MINUS:
    case PRIM_UNARY_PLUS:
    case PRIM_UNARY_NOT:
    case PRIM_ADD:
    case PRIM_SUBTRACT:
    case PRIM_MULT:
    case PRIM_DIV:
    case PRIM_MOD:
    case PRIM_FMA:
    case PRIM_LSH:
    case PRIM_RSH:
    case PRIM_AND:
    case PRIM_OR:
    case PRIM_XOR:
    case PRIM_POW:
    case PRIM_MIN:
    case PRIM_MAX:
    case PRIM_STEAL:
    case PRIM_SQRT:
      if (ci.numActuals() > 0) {
        type = QualifiedType(QualifiedType::CONST_VAR,
                             ci.actual(0).type().type());
      }
      // Note: PRIM_SQRT primitive is only actually used for params,
      // so this code should probably assert that
      break;
    /* primitives that return default int */
    case PRIM_GET_UNION_ID:
    case PRIM_GET_REQUESTED_SUBLOC:
    case PRIM_GET_USER_LINE:
      type = QualifiedType(QualifiedType::CONST_VAR,
                           IntType::get(context, 0));
      break;
    /* primitives that return an int32 */
    case PRIM_GETCID:
    case PRIM_GET_USER_FILE:
    case PRIM_GPU_THREADIDX_X:
    case PRIM_GPU_THREADIDX_Y:
    case PRIM_GPU_THREADIDX_Z:
    case PRIM_GPU_BLOCKIDX_X:
    case PRIM_GPU_BLOCKIDX_Y:
    case PRIM_GPU_BLOCKIDX_Z:
    case PRIM_GPU_BLOCKDIM_X:
    case PRIM_GPU_BLOCKDIM_Y:
    case PRIM_GPU_BLOCKDIM_Z:
    case PRIM_GPU_GRIDDIM_X:
    case PRIM_GPU_GRIDDIM_Y:
    case PRIM_GPU_GRIDDIM_Z:
      type = QualifiedType(QualifiedType::CONST_VAR,
                           IntType::get(context, 32));
      break;
    /* primitives that return void */
    case PRIM_NOOP:
    case PRIM_MOVE:
    case PRIM_DEFAULT_INIT_VAR:
    case PRIM_NOINIT_INIT_VAR:
    case PRIM_INIT_FIELD:
    case PRIM_INIT_VAR:
    case PRIM_INIT_VAR_SPLIT_DECL:
    case PRIM_INIT_VAR_SPLIT_INIT:
    case PRIM_INIT_REF_DECL:
    case PRIM_INIT_DONE:
    case PRIM_REDUCE:
    case PRIM_ASSIGN:
    case PRIM_ASSIGN_ELIDED_COPY:
    case PRIM_UNORDERED_ASSIGN:
    case PRIM_ADD_ASSIGN:
    case PRIM_SUBTRACT_ASSIGN:
    case PRIM_MULT_ASSIGN:
    case PRIM_DIV_ASSIGN:
    case PRIM_MOD_ASSIGN:
    case PRIM_LSH_ASSIGN:
    case PRIM_RSH_ASSIGN:
    case PRIM_AND_ASSIGN:
    case PRIM_OR_ASSIGN:
    case PRIM_XOR_ASSIGN:
    case PRIM_REDUCE_ASSIGN:
    case PRIM_SETCID:
    case PRIM_SET_UNION_ID:
    case PRIM_SET_MEMBER:
    case PRIM_CHECK_NIL:
    case PRIM_IF_VAR:
    case PRIM_LOCAL_CHECK:
    case PRIM_SET_END_COUNT:
    case PRIM_SET_DYNAMIC_END_COUNT:
    case PRIM_SET_SERIAL:
    case PRIM_ERROR:
    case PRIM_WARNING:
    case PRIM_WHEN:
    case PRIM_CALL_DESTRUCTOR:
    case PRIM_INT_ERROR:
    case PRIM_SLEEP:
    case PRIM_CHPL_EXIT_ANY:
    case PRIM_RT_ERROR:
    case PRIM_RT_WARNING:
    case PRIM_NEW_PRIV_CLASS:
    case PRIM_SET_SVEC_MEMBER:
    case PRIM_ITERATOR_RECORD_SET_SHAPE:
    case PRIM_START_RMEM_FENCE:
    case PRIM_FINISH_RMEM_FENCE:
    case PRIM_ZERO_VARIABLE:
    case PRIM_REQUIRE:
    case PRIM_CHECK_ERROR:
    case PRIM_SET_ALIASING_ARRAY_ON_TYPE:
    case PRIM_INVARIANT_START:
    case PRIM_GET_TEST_BY_NAME:
    case PRIM_GET_TEST_BY_INDEX:
    case PRIM_ARRAY_SET:
    case PRIM_ARRAY_SET_FIRST:
    case PRIM_INIT_FIELDS:
    case PRIM_CHPL_COMM_GET:
    case PRIM_CHPL_COMM_PUT:
    case PRIM_CHPL_COMM_ARRAY_GET:
    case PRIM_CHPL_COMM_ARRAY_PUT:
    case PRIM_CHPL_COMM_REMOTE_PREFETCH:
    case PRIM_CHPL_COMM_GET_STRD:
    case PRIM_CHPL_COMM_PUT_STRD:
    case PRIM_ARRAY_SHIFT_BASE_POINTER:
    case PRIM_AUTO_DESTROY_RUNTIME_TYPE:
    case PRIM_CREATE_FN_TYPE:
    case PRIM_GPU_KERNEL_LAUNCH:
    case PRIM_GPU_SYNC_THREADS:
    case PRIM_GPU_ELIGIBLE:
    case PRIM_GPU_DEINIT_KERNEL_CFG:
    case PRIM_GPU_ARG:
    case PRIM_GPU_PID_OFFLOAD:
    case PRIM_GPU_ATTRIBUTE_BLOCK:
    case PRIM_GPU_PRIMITIVE_BLOCK:
    case PRIM_GPU_BLOCK_REDUCE:
    case PRIM_GPU_REDUCE_WRAPPER:
      type = QualifiedType(QualifiedType::CONST_VAR,
                           VoidType::get(context));
      break;

    case PRIM_STATIC_DOMAIN_TYPE:
      type = computeDomainType(context, ci);
      break;

    case PRIM_GET_COMPILER_VAR: {
        auto chplenv = context->getChplEnv();
        auto varName = ci.actual(0).type().param()->toStringParam()->value().str();
        auto it = chplenv->find(varName);
        auto ret = (it != chplenv->end()) ? it->second : "";

        auto st = CompositeType::getStringType(context);
        auto sp = StringParam::get(context, UniqueString::get(context, ret));
        type = QualifiedType(QualifiedType::PARAM, st, sp);
      }
      break;
    /* primitives that return real parts from a complex */
    case PRIM_GET_REAL:
    case PRIM_GET_IMAG:
      // TODO: get the real/imag component from a param complex
      type = primComplexGetComponent(context, ci);
      break;
    /* other math primitives */
    case PRIM_ABS:
      type = primAbsGetType(context, ci);
      break;
    /* Getting the wide pointer address */
    case PRIM_WIDE_GET_ADDR:
      type = QualifiedType(QualifiedType::CONST_VAR,
                           CPtrType::getCVoidPtrType(context));
      break;
    /* primitives that return a c_string*/
    case PRIM_LOOKUP_FILENAME:
    case PRIM_CLASS_NAME_BY_ID:
    case PRIM_REF_TO_STRING:{
      type = QualifiedType(QualifiedType::CONST_VAR,
                           CStringType::get(context));
      break;
    }
    /* primitives that are not yet handled in dyno */
    case PRIM_ACTUALS_LIST:
    case PRIM_GET_MEMBER:
    case PRIM_GET_MEMBER_VALUE:
    case PRIM_NEW:
    case PRIM_NEW_WITH_ALLOCATOR:
    case PRIM_QUERY:
    case PRIM_QUERY_PARAM_FIELD:
    case PRIM_QUERY_TYPE_FIELD:
      CHPL_UNIMPL("misc primitives");
      break;

    case PRIM_ADDR_OF:
      type = primAddrOf(context, ci);
      break;

    case PRIM_DEREF:
    case PRIM_SET_REFERENCE:
    case PRIM_GET_END_COUNT:
    case PRIM_GET_DYNAMIC_END_COUNT:
      CHPL_UNIMPL("misc primitives");
      break;

    case PRIM_GPU_ALLOC_SHARED:
      type = primGpuAllocShared(context, ci);
      break;

    case PRIM_GPU_SET_BLOCKSIZE:
      type = primGpuSetBlockSize(context, ci);
      break;

    case PRIM_ASSERT_ON_GPU:
      type = primAssertOnGpu(context, ci);
      break;

    case PRIM_ASSERT_GPU_ELIGIBLE:
      type = QualifiedType(QualifiedType::CONST_VAR, VoidType::get(context));
      break;

    case PRIM_GPU_INIT_KERNEL_CFG:
    case PRIM_GPU_INIT_KERNEL_CFG_3D:
      type = QualifiedType(QualifiedType::CONST_VAR, CPtrType::getCVoidPtrType(context));
      break;

    case PRIM_SIZEOF_BUNDLE:
    case PRIM_SIZEOF_DDATA_ELEMENT:
      type = QualifiedType(QualifiedType::CONST_VAR,
                           IntType::get(context, 64));
      break;

    case PRIM_LIFETIME_OF:
      CHPL_UNIMPL("misc primitives");
      break;

    case PRIM_TYPEOF:
    case PRIM_STATIC_TYPEOF:
      type = primTypeof(context, prim, ci);
      break;

    case PRIM_SCALAR_PROMOTION_TYPE:
      CHPL_UNIMPL("misc primitives");
      break;
    case PRIM_STATIC_FIELD_TYPE:
      type = staticFieldType(context, ci);
      break;
    case PRIM_WIDE_GET_NODE: // NodeIdType is defined as int32
      type = QualifiedType(QualifiedType::CONST_VAR,
                           IntType::get(context, 32));
      break;
    case PRIM_ON_LOCALE_NUM:
      type = QualifiedType(QualifiedType::CONST_VAR,
                           CompositeType::getLocaleIDType(context));
      break;

    case PRIM_ARRAY_GET: {
        if (ci.numActuals() == 2 && ci.actual(0).type().hasTypePtr()) {
          auto index = ci.actual(1).type();
          if (index.hasTypePtr() && index.type()->isIntegralType()) {
            auto act = ci.actual(0).type();
            if (auto ptr = act.type()->toCPtrType()) {
              type = QualifiedType(QualifiedType::REF, ptr->eltType());
            }
          } else {
            context->error(call, "bad call to primitive \"%s\": second argument must be an integral type", primTagToName(prim));
          }
        }
      }
      break;

    case PRIM_GET_SVEC_MEMBER:
    case PRIM_GET_SVEC_MEMBER_VALUE:
        return primGetSvecMember(context, prim, ci);
        break;

    case PRIM_USED_MODULES_LIST:
    case PRIM_REFERENCED_MODULES_LIST:
    case PRIM_TUPLE_EXPAND:
    case PRIM_MAYBE_LOCAL_THIS:
    case PRIM_MAYBE_LOCAL_ARR_ELEM:
    case PRIM_MAYBE_AGGREGATE_ASSIGN:
    case PRIM_PROTO_SLICE_ASSIGN:
    case PRIM_BLOCK_PARAM_LOOP:
    case PRIM_BLOCK_WHILEDO_LOOP:
    case PRIM_BLOCK_DOWHILE_LOOP:
    case PRIM_BLOCK_FOR_LOOP:
    case PRIM_BLOCK_BEGIN:
    case PRIM_BLOCK_COBEGIN:
    case PRIM_BLOCK_COFORALL:
    case PRIM_BLOCK_ON:
    case PRIM_BLOCK_ELIDED_ON:
    case PRIM_BLOCK_BEGIN_ON:
    case PRIM_BLOCK_COBEGIN_ON:
    case PRIM_BLOCK_COFORALL_ON:
    case PRIM_BLOCK_LOCAL:
    case PRIM_BLOCK_UNLOCAL:
    case PRIM_LOGICAL_FOLDER:
    case PRIM_WIDE_MAKE:
    case PRIM_WIDE_GET_LOCALE:
    case PRIM_REGISTER_GLOBAL_VAR:
    case PRIM_BROADCAST_GLOBAL_VARS:
    case PRIM_PRIVATE_BROADCAST:
    case PRIM_CAPTURE_FN:
    case PRIM_CAPTURE_FN_TO_CLASS:
    case PRIM_RESOLUTION_POINT:
    case PRIM_FTABLE_CALL:
    case PRIM_VIRTUAL_METHOD_CALL:
    case PRIM_END_OF_STATEMENT:
    case PRIM_CURRENT_ERROR:
    case PRIM_GET_RUNTIME_TYPE_FIELD:
    case PRIM_GET_VISIBLE_SYMBOLS:
    case PRIM_STACK_ALLOCATE_CLASS:
    case PRIM_ZIP:
    case PRIM_NO_ALIAS_SET:
    case PRIM_COPIES_NO_ALIAS_SET:
    case PRIM_OPTIMIZATION_INFO:
    case PRIM_CREATE_THUNK:
    case PRIM_THUNK_RESULT:
    case PRIM_FORCE_THUNK:
    case PRIM_THUNK_RESULT_TYPE:
      CHPL_UNIMPL("misc primitives");
      break;

    case PRIM_GATHER_TESTS:
      type = primGatherTests(context, ci);
      break;

    case PRIM_VERSION_MAJOR:
      type = QualifiedType(QualifiedType::PARAM, IntType::get(context, 0),
                           IntParam::get(context, getMajorVersion()));
      break;

    case PRIM_VERSION_MINOR:
      type = QualifiedType(QualifiedType::PARAM, IntType::get(context, 0),
                           IntParam::get(context, getMinorVersion()));
      break;

    case PRIM_VERSION_UPDATE:
      type = QualifiedType(QualifiedType::PARAM, IntType::get(context, 0),
                           IntParam::get(context, getUpdateVersion()));
      break;

    case PRIM_VERSION_SHA: {
      UniqueString versionHash;
      if (!getIsOfficialRelease()) {
        versionHash = UniqueString::get(context, getCommitHash());
      }

      type = QualifiedType(QualifiedType::PARAM, RecordType::getStringType(context),
                           StringParam::get(context, versionHash));
      break;
    }

    case PRIM_REF_DESERIALIZE:
    case PRIM_UNKNOWN:
    case PRIM_INNERMOST_CONTEXT:
    case PRIM_OUTER_CONTEXT:
    case PRIM_HOIST_TO_CONTEXT:
    case PRIM_STATIC_FUNCTION_VAR:
    case PRIM_STATIC_FUNCTION_VAR_VALIDATE_TYPE:
    case PRIM_STATIC_FUNCTION_VAR_WRAPPER:
    case NUM_KNOWN_PRIMS:
    case PRIM_BREAKPOINT:
    case PRIM_CONST_ARG_HASH:
    case PRIM_CHECK_CONST_ARG_HASH:
    case PRIM_TASK_PRIVATE_SVAR_CAPTURE:
      CHPL_UNIMPL("misc primitives");

    // no default to get a warning when new primitives are added
  }

  if (type.kind() == QualifiedType::UNKNOWN && !type.isErroneousType()) {
    context->error(call, "bad call to primitive \"%s\"", primTagToName(prim));
    type = QualifiedType(QualifiedType::UNKNOWN, ErroneousType::get(context));
  }

  return CallResolutionResult(candidates, type, poi, /* specially handled */ true);
}


} // end namespace resolution
} // end namespace chpl
