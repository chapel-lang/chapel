/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "chpl/resolution/can-pass.h"

#include "chpl/resolution/resolution-queries.h"
#include "chpl/types/all-types.h"

#include <cmath>
#include <cinttypes>

namespace chpl {
namespace resolution {

using namespace uast;
using namespace types;

static bool fitsInIntHelper(int width, int64_t val) {
  switch (width) {
    default: CHPL_ASSERT(false && "bad width in fits_in_int_helper");
    case 1:
      return (val == 0 || val == 1);
    case 8:
      return (INT8_MIN <= val && val <= INT8_MAX);
    case 16:
      return (INT16_MIN <= val && val <= INT16_MAX);
    case 32:
      return (INT32_MIN <= val && val <= INT32_MAX);
    case 64:
      // As an int64_t will always fit within a 64 bit int.
      return true;
  }
}

static bool paramFitsInInt(int width, const Param* p) {
  if (auto ip = p->toIntParam()) {
    int64_t i = ip->value();
    return fitsInIntHelper(width, i);
  } else if (auto up = p->toUintParam()) {
    uint64_t u = up->value();
    if (u > INT64_MAX)
      return false;
    return fitsInIntHelper(width, (int64_t)u);
  }
  return false;
}

static bool fitsInUintHelper(int width, uint64_t val) {
  switch (width) {
  default: CHPL_ASSERT(false && "bad width in fits_in_uint_helper");
  case 8:
    return (val <= UINT8_MAX);
  case 16:
    return (val <= UINT16_MAX);
  case 32:
    return (val <= UINT32_MAX);
  case 64:
    // As a uint64_t will always fit inside a 64 bit uint.
    return true;
  }
}

static bool paramFitsInUint(int width, const Param* p) {
  if (auto ip = p->toIntParam()) {
    int64_t i = ip->value();
    if (i < 0)
      return false;
    return fitsInUintHelper(width, (uint64_t)i);
  } else if (auto up = p->toUintParam()) {
    uint64_t u = up->value();
    return fitsInUintHelper(width, u);
  }
  return false;
}

// numbers between -2**mantissa .. 2**mantissa
// will fit exactly in a floating-point representation.
static void getMantissaExponentWidth(const Type *t,
                                     int& mantissa,
                                     int& exponent) {
  // a 16 bit float would have 11 bits of precision
  // a 16 bit float would have 5 bits of exponent
  const int mantissa_64 = 53; // mantissa for 64-bit float
  const int exponent_64 = 15; // exponent bits for 64-bit float
  const int mantissa_32 = 24; // mantissa for 32-bit float
  const int exponent_32 = 8; // exponent bits for 32-bit float

  if (auto rt = t->toRealType()) {
    if (rt->bitwidth() == 32) {
      mantissa = mantissa_32;
      exponent = exponent_32;
      return;
    }
    if (rt->bitwidth() == 64) {
      mantissa = mantissa_64;
      exponent = exponent_64;
      return;
    }
  }
  if (auto it = t->toImagType()) {
    if (it->bitwidth() == 32) {
      mantissa = mantissa_32;
      exponent = exponent_32;
      return;
    }
    if (it->bitwidth() == 64) {
      mantissa = mantissa_64;
      exponent = exponent_64;
      return;
    }
  }
  if (auto ct = t->toComplexType()) {
    if (ct->bitwidth() == 64) {
      mantissa = mantissa_32;
      exponent = exponent_32;
      return;
    }
    if (ct->bitwidth() == 128) {
      mantissa = mantissa_64;
      exponent = exponent_64;
      return;
    }
  }
  CHPL_ASSERT(false && "unknown mantissa width");
  mantissa = 0;
  exponent = 0;
}

static bool fitsInBitsNoSign(int width, int64_t i) {
  // is it between -2**width .. 2**width, inclusive?
  int64_t p = 1;
  p <<= width; // now p is 2**width

  return -p <= i && i <= p;
}

static bool fitsInTwosComplement(int width, int64_t i) {
  // would it fit in a width-bit 2's complement representation?

  CHPL_ASSERT(width < 64);

  int64_t max_pos = 1;
  max_pos <<= width-1;
  max_pos--;

  int64_t min_neg = 1+max_pos;
  return -min_neg <= i && i <= max_pos;
}

// Does the integer in p fit in a floating point format with 'mantissaWidth'
// bits of mantissa?
static bool fitsInMantissa(int mantissaWidth, const Param* p) {
  // is it between -2**mantissaWidth .. 2**mantissaWidth, inclusive?

  if (auto ip = p->toIntParam()) {
    int64_t i = ip->value();
    return fitsInBitsNoSign(mantissaWidth, i);
  } else if (auto up = p->toUintParam()) {
    uint64_t u = up->value();
    if (u > INT64_MAX)
      return false;
    return fitsInBitsNoSign(mantissaWidth, (int64_t)u);
  }

  return false;
}

// realPart indicates which part to check for a complex number
// (and it is ignored for real/imag).
static bool fitsInMantissaExponent(int mantissaWidth,
                                   int exponentWidth,
                                   const Param* p,
                                   bool realPart) {
  double v = 0.0;

  if (auto rp = p->toRealParam()) {
    v = rp->value();
  } else if (auto cp = p->toComplexParam()) {
    if (realPart)
      v = cp->value().re;
    else
      v = cp->value().im;
  } else {
    CHPL_ASSERT(false && "unsupported number kind");
  }

  double frac = 0.0;
  int exp = 0;

  frac = frexp(v, &exp);

  int64_t intpart = 2*frac;

  if (fitsInBitsNoSign(mantissaWidth, intpart) &&
      fitsInTwosComplement(exponentWidth, exp))
    return true;

  return false;
}

static bool isConsideredGeneric(Type::Genericity g) {
  switch (g) {
    case Type::CONCRETE:
    case Type::GENERIC_WITH_DEFAULTS:
      // argument passing calculations think of generic with defaults
      // as the same as concrete.
      return false;
    case Type::GENERIC:
      return true;
    case Type::MAYBE_GENERIC:
      CHPL_ASSERT(false && "Should not be reachable");
  }

  return false;
}

bool CanPassResult::isTypeGeneric(Context* context, const QualifiedType& qt) {
  auto g = getTypeGenericity(context, qt);
  return isConsideredGeneric(g);
}
bool CanPassResult::isTypeGeneric(Context* context, const Type* t) {
  auto g = getTypeGenericity(context, t);
  return isConsideredGeneric(g);
}

// can we do an implicit numeric conversion?
// does not check for param narrowing conversions
bool CanPassResult::canConvertNumeric(Context* context,
                                      const Type* actualT,
                                      const Type* formalT) {
  // Return early if the types involved are not numeric/bool
  if (!actualT->isNumericOrBoolType() || !formalT->isNumericOrBoolType())
    return false;

  // any bool type can convert to any other bool type
  if (actualT->isBoolType() && formalT->isBoolType())
    return true;

  if (auto formalIntT = formalT->toIntType()) {
    // convert any bool to int
    if (actualT->isBoolType())
      return true;

    // convert from int/uint of smaller size
    if (auto actualIntT = actualT->toIntType())
      if (actualIntT->bitwidth() < formalIntT->bitwidth())
        return true;

    if (auto actualUintT = actualT->toUintType())
      if (actualUintT->bitwidth() < formalIntT->bitwidth())
        return true;
  }

  if (auto formalUintT = formalT->toUintType()) {
    // convert any bool to uint
    if (actualT->isBoolType())
      return true;

    // convert from uint of smaller size
    if (auto actualUintT = actualT->toUintType())
      if (actualUintT->bitwidth() < formalUintT->bitwidth())
        return true;
  }

  if (auto formalRealT = formalT->toRealType()) {
    // don't convert bools to reals (per spec: "unintended by programmer")

    // convert real from smaller size
    if (auto actualRealT = actualT->toRealType())
      if (actualRealT->bitwidth() < formalRealT->bitwidth())
        return true;

    if (actualT->isIntegralType()) {
      // convert any integer type to maximum width real
      if (formalRealT->bitwidth() == 64)
        return true;

      int mantissaW = 0;
      int exponentW = 0;
      getMantissaExponentWidth(formalRealT, mantissaW, exponentW);

      // convert integer types that are exactly representable
      if (auto actualIntT = actualT->toIntType())
        if (actualIntT->bitwidth() < mantissaW)
          return true;

      if (auto actualUintT = actualT->toUintType())
        if (actualUintT->bitwidth() < mantissaW)
          return true;
    }
  }

  if (auto formalImagT = formalT->toImagType()) {
    // convert imag from smaller size
    if (auto actualImagT = actualT->toImagType())
      if (actualImagT->bitwidth() < formalImagT->bitwidth())
        return true;
  }

  if (auto formalComplexT = formalT->toComplexType()) {
    // don't convert bools to complexes (per spec: "unintended by programmer")

    // convert smaller complex types
    if (auto actualComplexT = actualT->toComplexType())
      if (actualComplexT->bitwidth() < formalComplexT->bitwidth())
        return true;

    // convert real/imag from smaller size
    if (auto actualRealT = actualT->toRealType())
      if (actualRealT->bitwidth() <= formalComplexT->bitwidth()/2)
        return true;
    if (auto actualImagT = actualT->toImagType())
      if (actualImagT->bitwidth() <= formalComplexT->bitwidth()/2)
        return true;

    if (actualT->isIntegralType()) {
      // convert any integer type to maximum width complex
      if (formalComplexT->bitwidth() == 128)
        return true;

      int mantissaW = 0;
      int exponentW = 0;
      getMantissaExponentWidth(formalComplexT, mantissaW, exponentW);

      // convert integer types that are exactly representable
      if (auto actualIntT = actualT->toIntType())
        if (actualIntT->bitwidth() < mantissaW)
          return true;
      if (auto actualUintT = actualT->toUintType())
        if (actualUintT->bitwidth() < mantissaW)
          return true;
    }
  }

  return false;
}

bool
CanPassResult::canConvertParamNarrowing(Context* context,
                                        const QualifiedType& actualQT,
                                        const QualifiedType& formalQT) {
  const Type* actualT = actualQT.type();
  const Param* actualP = actualQT.param();
  const Type* formalT = formalQT.type();

  // return early if actual is not a param
  if (actualP == nullptr)
    return false;

  // return early if formal is not numeric or stringlike
  if (!formalT->isNumericType() && !formalT->isStringLikeType())
    return false;

  if (auto formalIntT = formalT->toIntType()) {
    //
    // For smaller integer types, if the argument is a param, does it
    // store a value that's small enough that it could dispatch to
    // this argument?
    //
    if (formalIntT->bitwidth() < 64)
      if (paramFitsInInt(formalIntT->bitwidth(), actualP))
        return true;
  }

  if (auto formalUintT = formalT->toUintType()) {
    if (paramFitsInUint(formalUintT->bitwidth(), actualP))
      return true;
  }

  // param strings can convert between string and c_string
  if ((formalT->isStringType() || formalT->isCStringType()) &&
      (actualT->isStringType() || actualT->isCStringType()))
    return true;

  if (auto formalRealT = formalT->toRealType()) {
    int mantissaW = 0;
    int exponentW = 0;
    getMantissaExponentWidth(formalRealT, mantissaW, exponentW);

    // convert literal/param ints that are exactly representable
    if (actualT->isIntegralType())
      if (fitsInMantissa(mantissaW, actualP))
        return true;

    if (actualT->isRealType())
      if (fitsInMantissaExponent(mantissaW, exponentW, actualP, true))
        return true;
  }

  if (auto formalImagT = formalT->toImagType()) {
    int mantissaW = 0;
    int exponentW = 0;
    getMantissaExponentWidth(formalImagT, mantissaW, exponentW);

    // convert literal/param imag that are exactly representable
    if (actualT->isImagType())
      if (fitsInMantissaExponent(mantissaW, exponentW, actualP, false))
        return true;
  }

  if (auto formalComplexT = formalT->toComplexType()) {
    int mantissaW = 0;
    int exponentW = 0;
    getMantissaExponentWidth(formalComplexT, mantissaW, exponentW);

    // convert literal/param complexes that are exactly representable
    if (auto actualP = actualQT.param()) {
      if (actualT->isIntegralType())
        if (fitsInMantissa(mantissaW, actualP))
          return true;
      if (actualT->isRealType())
        if (fitsInMantissaExponent(mantissaW, exponentW, actualP, true))
          return true;
      if (actualT->isImagType())
        if (fitsInMantissaExponent(mantissaW, exponentW, actualP, false))
          return true;
      if (actualT->isComplexType()) {
        if (fitsInMantissaExponent(mantissaW, exponentW, actualP, true) &&
            fitsInMantissaExponent(mantissaW, exponentW, actualP, false))
          return true;
      }
    }
  }

  return false;
}

CanPassResult CanPassResult::canPassDecorators(Context* context,
                                               ClassTypeDecorator actual,
                                               ClassTypeDecorator formal) {
  if (actual == formal) {
    return passAsIs();
  }

  bool instantiates = false;
  bool converts = false;
  bool fails = false;

  ClassTypeDecorator actualNily = actual.toBorrowed();
  ClassTypeDecorator formalNily = formal.toBorrowed();

  ClassTypeDecorator actualMgmt = actual.removeNilable();
  ClassTypeDecorator formalMgmt = formal.removeNilable();

  // consider nilability.
  if (actualNily != formalNily) {
    if (formalNily.isUnknownNilability())
      instantiates = true; // instantiating with passed nilability
    else if (actualNily.isNonNilable() && formalNily.isNilable())
      converts = true; // non-nil to nil conversion
    else
      fails = true; // all other nilability cases
  }

  // consider management.
  if (actualMgmt != formalMgmt) {
    if (formalMgmt.isUnknownManagement())
      instantiates = true; // instantiating with passed management
    else if (formalMgmt.isBorrowed())
      converts = true; // management can convert to borrowed
    else
      fails = true;
  }

  if (fails)
    return fail();

  // all class conversions are subtype conversions
  ConversionKind conversion = converts ? SUBTYPE : NONE;

  return CanPassResult(/*passes*/ true,
                       instantiates,
                       /*promotes*/ false,
                       conversion);
}

CanPassResult CanPassResult::canPassClassTypes(Context* context,
                                               const ClassType* actualCt,
                                               const ClassType* formalCt) {
  // owned Child -> owned Parent
  // owned Child -> owned Parent?
  // ditto borrowed, etc

  // check decorators allow passing
  CanPassResult decResult = canPassDecorators(context,
                                              actualCt->decorator(),
                                              formalCt->decorator());

  if (!decResult.passes())
    return fail();

  if (actualCt->decorator().isManaged() &&
      formalCt->decorator().isManaged() &&
      actualCt->manager() != formalCt->manager()) {
    // disallow e.g. owned C -> shared C
    return fail();
  }

  auto actualBct = actualCt->basicClassType();
  auto formalBct = formalCt->basicClassType();

  // code below assumes this
  CHPL_ASSERT(decResult.passes_);
  CHPL_ASSERT(decResult.conversionKind_ == NONE ||
         decResult.conversionKind_ == SUBTYPE);
  CHPL_ASSERT(!decResult.promotes_);

  bool converts = decResult.conversionKind_ != NONE;
  bool instantiates = decResult.instantiates_;

  if (actualBct->isSubtypeOf(formalBct, converts, instantiates)) {
    // the basic class types are the same
    // or there was a subclass relationship
    // or there was instantiation

    // all class conversions are subtype conversions
    ConversionKind conversion = converts ? SUBTYPE : NONE;

    return CanPassResult(/*passes*/ true,
                         instantiates,
                         /*promotes*/ false,
                         conversion);
  }

  return fail();
}


// The compiler considers many patterns of "subtyping" as things that require
// implicit conversions (they often require implicit conversions in the
// generated C).  However not all implicit conversions are created equal.
// Some of them are implementing subtyping.
//
// Here we consider an implicit conversion to be implementing "subtyping" if,
// in an ideal implementation, the actual could be passed to a `const ref`
// argument of the formal type.
//
// This function returns CanPassResult which always has conversion
// kind of NONE or SUBTYPE.
// It's returning CanPassResult in order to also reflect if instantiation
// was necessary.
CanPassResult CanPassResult::canPassSubtype(Context* context,
                                            const Type* actualT,
                                            const Type* formalT) {
  // nil -> pointers and class types
  if (actualT->isNilType() && formalT->isNilablePtrType() &&
      !formalT->isCStringType())
    return convert(SUBTYPE);

  if (auto actualCt = actualT->toClassType()) {
    if (auto formalCt = formalT->toClassType()) {
      CanPassResult result = canPassClassTypes(context, actualCt, formalCt);
      if (result.passes_ && (result.conversionKind_ == NONE ||
                             result.conversionKind_ == SUBTYPE)) {
        return result;
      }
    }
  }

  // TODO: c_ptr -> c_void_ptr
  // TODO: c_array -> c_void_ptr, c_array(t) -> c_ptr(t)

  return fail();
}

CanPassResult CanPassResult::canConvertTuples(Context* context,
                                              const TupleType* aT,
                                              const TupleType* fT) {

  // passing to generic tuple type
  if (fT == TupleType::getGenericTupleType(context)) {
    return instantiate();
  }

  if (aT->numElements() != fT->numElements()) {
    // Number of fields differs, so not convertible.
    return fail();
  }

  if (aT->isStarTuple() != fT->isStarTuple()) {
    // Star-tuple-ness differs, so not convertible.
    return fail();
  }

  int n = aT->numElements();
  if (aT->isStarTuple())
    n = 1; // only need to consider one type

  bool instantiates = false;
  bool converts = false;

  for (int i = 0; i < n; i++) {
    // check to see if the types are not the same and can't convert
    QualifiedType aElt = aT->elementType(i);
    QualifiedType fElt = fT->elementType(i);

    if (aElt != fElt) {
      auto got = canPass(context, aElt, fElt);
      if (!got.passes() || got.promotes()) {
        return fail();
      } else {
        instantiates = instantiates || got.instantiates();
        converts = converts || got.converts();
      }
    }
  }

  if (converts == true && instantiates == true) {
    return convertAndInstantiate(OTHER);
  } else if (converts == true && instantiates == false) {
    return convert(OTHER);
  } else if (converts == false && instantiates == true) {
    return instantiate();
  } else { // (converts == false && instantiates == false)
    return passAsIs();
  }
}

CanPassResult CanPassResult::canConvert(Context* context,
                                        const QualifiedType& actualQT,
                                        const QualifiedType& formalQT) {
  const Type* actualT = actualQT.type();
  const Type* formalT = formalQT.type();

  // can we convert with a subtype conversion, including class subtyping?
  {
    auto got = canPassSubtype(context, actualT, formalT);
    if (got.passes()) {
      // canPassSubtype should always return NONE or SUBTYPE conversion.
      CHPL_ASSERT(got.conversionKind_ == NONE || got.conversionKind_ == SUBTYPE);
      return got;
    }
  }

  // can we convert with a numeric conversion?
  if (canConvertNumeric(context, actualT, formalT))
    return convert(NUMERIC);

  // can we convert with param narrowing?
  if (canConvertParamNarrowing(context, actualQT, formalQT))
    return convert(PARAM_NARROWING);

  // can we convert tuples?
  if (actualQT.type()->isTupleType() && formalQT.type()->isTupleType()) {
    auto aT = actualQT.type()->toTupleType();
    auto fT = formalQT.type()->toTupleType();
    auto got = canConvertTuples(context, aT, fT);
    if (got.passes()) {
      return got;
    }
  }

  // TODO: check for conversion to copy type
  // (relevant for array slices and iterator records)
  // TODO: port canCoerceToCopyType

  return fail();
}

// handles formalT being a builtin generic type like integral
bool CanPassResult::canInstantiateBuiltin(Context* context,
                                          const Type* actualT,
                                          const Type* formalT) {
  if (formalT->isAnyType())
    return true;

  if (formalT->isAnyBoolType() && actualT->isBoolType())
      return true;

  if (formalT->isAnyBorrowedNilableType())
    if (auto ct = actualT->toClassType())
      if (ct->decorator().val() == ClassTypeDecorator::BORROWED_NILABLE)
        return true;

  if (formalT->isAnyBorrowedNonNilableType())
    if (auto ct = actualT->toClassType())
      if (ct->decorator().val() == ClassTypeDecorator::BORROWED_NONNIL)
        return true;

  if (formalT->isAnyBorrowedType())
    if (auto ct = actualT->toClassType())
      if (ct->decorator().isBorrowed())
        return true;

  if (formalT->isAnyComplexType() && actualT->isComplexType())
    return true;

  if (formalT->isAnyEnumType() && actualT->isEnumType())
    return true;

  if (formalT->isAnyImagType() && actualT->isImagType())
    return true;

  if (formalT->isAnyIntType() && actualT->isIntType())
    return true;

  if (formalT->isAnyIntegralType() && actualT->isIntegralType())
    return true;

  if (formalT->isAnyIteratorClassType())
    CHPL_ASSERT(false && "Not implemented yet"); // TODO: represent iterators

  if (formalT->isAnyIteratorRecordType())
    CHPL_ASSERT(false && "Not implemented yet"); // TODO: represent iterators

  if (formalT->isAnyManagementAnyNilableType())
    if (actualT->isClassType())
      return true;

  if (formalT->isAnyManagementNilableType())
    if (auto ct = actualT->toClassType())
      if (ct->decorator().isNilable())
        return true;

  if (formalT->isAnyManagementNonNilableType())
    if (auto ct = actualT->toClassType())
      if (ct->decorator().isNonNilable())
        return true;

  if (formalT->isAnyNumericType() && actualT->isNumericType())
    return true;

  if (formalT->isAnyOwnedType())
    if (auto ct = actualT->toClassType())
      if (ct->decorator().isManaged())
        if (auto manager = ct->manager())
          if (manager->isAnyOwnedType())
            return true;

  if (formalT->isAnyPodType())
    CHPL_ASSERT(false && "Not implemented yet"); // TODO: compute POD-ness

  if (formalT->isAnyRealType() && actualT->isRealType())
    return true;

  if (formalT->isAnyRecordType() && actualT->isUserRecordType())
    return true;

  if (formalT->isAnySharedType())
    if (auto ct = actualT->toClassType())
      if (ct->decorator().isManaged())
        if (auto manager = ct->manager())
          if (manager->isAnySharedType())
            return true;

  if (formalT->isAnyUintType() && actualT->isUintType())
    return true;

  if (formalT->isAnyUninstantiatedType()) {
    if (isTypeGeneric(context, actualT))
      return true;
  }


  if (formalT->isAnyUnionType() && actualT->isUnionType())
    return true;

  if (formalT->isAnyUnmanagedNilableType())
    if (auto ct = actualT->toClassType())
      if (ct->decorator().val() == ClassTypeDecorator::UNMANAGED_NILABLE)
        return true;

  if (formalT->isAnyUnmanagedNonNilableType())
    if (auto ct = actualT->toClassType())
      if (ct->decorator().val() == ClassTypeDecorator::UNMANAGED_NONNIL)
        return true;

  if (formalT->isAnyUnmanagedType())
    if (auto ct = actualT->toClassType())
      if (ct->decorator().isUnmanaged())
        return true;

  return false;
}

CanPassResult CanPassResult::canInstantiate(Context* context,
                                            const QualifiedType& actualQT,
                                            const QualifiedType& formalQT) {
  // Should we proceed with instantiation?
  // Further checking will occur after the instantiation occurs,
  // so checking here just rules out predictable situations.

  const Type* actualT = actualQT.type();
  const Type* formalT = formalQT.type();
  CHPL_ASSERT(actualT && formalT);

  // check for builtin generic types
  if (canInstantiateBuiltin(context, actualT, formalT)) {
    return instantiate();
  }

  // TODO: check for constrained generic types

  if (auto actualCt = actualT->toClassType()) {
    // check for instantiating classes
    if (auto formalCt = formalT->toClassType()) {
      CanPassResult got = canPassClassTypes(context, actualCt, formalCt);
      if (got.passes() && got.instantiates()) {
        return got;
      }
    }
  } else if (auto actualCt = actualT->toCompositeType()) {
    // check for instantiating records/unions/tuples
    if (auto formalCt = formalT->toCompositeType()) {
      if (actualCt->isInstantiationOf(context, formalCt)) {
        return instantiate();
      }
    }
  }

  return fail();
}

CanPassResult CanPassResult::canPass(Context* context,
                                     const QualifiedType& actualQT,
                                     const QualifiedType& formalQT) {

  const Type* actualT = actualQT.type();
  const Type* formalT = formalQT.type();
  CHPL_ASSERT(actualT && formalT);

  // if the formal type is unknown, allow passing
  // this can come up with e.g.
  //   proc f(a: int(?w), b: int(2*w))
  // when computing an initial candidate, 'b' is unknown
  // but we should allow passing an argument to it.
  if (formalT->isUnknownType() && !actualQT.isType()) {
    return passAsIs();
  }

  // allow unknown qualifier for any type actuals
  // so other code doesn't have to decide between param
  // and type for type queries
  bool typeQueryActual = (actualQT.kind() == QualifiedType::TYPE_QUERY);

  // check that the kinds are compatible

  // type formal, type actual -> OK
  // non-type formal, non-type actual -> OK
  // type formal, non-type actual -> error, can't pass value to type
  // non-type formal, type actual -> error, can't pass type to value
  if (formalQT.isType() != actualQT.isType() && !typeQueryActual)
    return fail();

  // param actuals can pass to non-param formals
  if (formalQT.isParam() && !actualQT.isParam() && !typeQueryActual)
    return fail();

  // check params
  const Param* actualParam = actualQT.param();
  const Param* formalParam = formalQT.param();
  if (actualParam && formalParam) {
    if (actualParam != formalParam) {
      // passing different param values won't do
      return fail();
    } // otherwise continue with type information
  } else if (formalParam && !actualParam) {
    // this case doesn't make sense
    CHPL_ASSERT(false && "case not expected");
    return fail();
  }

  // Type-query Kinds should always pass
  if (actualT == formalT || typeQueryActual) {
    if (formalQT.kind() == QualifiedType::PARAM &&
        formalQT.param() == nullptr) {
      // if the formal parameter value is unknown, we need to instantiate
      return instantiate();
    }

    // Passing in a type to another type requires instantiation.
    // Note: we might encounter this situation for a type method on a
    //   generic type. I.e., passing 'R(?)' to 'R(?)' for the 'this' formal.
    //   This case should instantiate so that code looking for a substitution
    //   will find one, rather than just seeing a generic type and guessing
    //   that it wasn't instantiated.
    //
    // 'AnyType' has special meaning elsewhere, so it doesn't count as
    // instantiation here.
    if (formalQT.kind() == QualifiedType::TYPE && !formalT->isAnyType()) {
      return instantiate();
    }

    // otherwise we can pass as-is
    return passAsIs();
  }

  if (formalQT.kind() == QualifiedType::OUT) {
    // 'out' intent is unusual:
    // type information for out intent will be inferred from the called function
    return passAsIs();
  }

  if (actualQT.isUnknown() && !typeQueryActual) {
    return fail(); // actual type not established
  }

  if (formalQT.isUnknownKindOrType()) {
    return fail(); // unknown formal type, can't resolve
  }

  if (isTypeGeneric(context, formalQT)) {
    // Check to see if we should proceed with instantiation.
    // Further checking will occur after the instantiation occurs,
    // so checking here just rules out predictable situations.

    if (formalQT.kind() != QualifiedType::TYPE &&
        isTypeGeneric(context, actualQT))
      return fail(); // generic types can only be passed to type actuals

    return canInstantiate(context, actualQT, formalQT);

  } else {
    // if the formal type is concrete, do additional checking
    // (if it is generic, we will do this checking after instantiation)
    switch (formalQT.kind()) {
      case QualifiedType::UNKNOWN:
      case QualifiedType::FUNCTION:
      case QualifiedType::PARENLESS_FUNCTION:
      case QualifiedType::MODULE:
      case QualifiedType::TYPE_QUERY:
      case QualifiedType::INDEX:
      case QualifiedType::DEFAULT_INTENT:
      case QualifiedType::CONST_INTENT:
      case QualifiedType::OUT: // handled above
        // no additional checking for these
        break;

      case QualifiedType::REF:
        return fail(); // ref type requires same time which is ruled out above

      case QualifiedType::CONST_REF:
      case QualifiedType::REF_MAYBE_CONST:
      case QualifiedType::TYPE:
        {
          auto got = canPassSubtype(context, actualT, formalT);
          if (got.passes()) {
            return got;
          }
          break;
        }

      case QualifiedType::PARAM:
        {
          auto got = canConvert(context, actualQT, formalQT);
          if (got.passes()) {
            // if the formal parameter value is unknown, we need
            // to instantiate as well.
            if (formalQT.param() == nullptr) {
              got.instantiates_ = true;
            }
            return got;
          }
          break;
        }

      case QualifiedType::IN:
      case QualifiedType::CONST_IN:
      case QualifiedType::INOUT:
      case QualifiedType::VAR:       // var/const var don't really make sense
      case QualifiedType::CONST_VAR: // as formals but we allow it for testing
        {
          auto got = canConvert(context, actualQT, formalQT);
          if (got.passes())
            return got;
          break;
        }
    }
  }

  // can we promote?
  // TODO: implement promotion check

  return fail();
}

// When trying to combine two kinds, you can't just pick one.
// For instance, if any type in the list is a value, the result
// should be a value, and if any type in the list is const, the
// result should be const. Thus, combining `const ref` and `var`
// should result in `const var`.
//
// This class is used to describe the "mixing rules" of various kinds.
// To this end, it breaks them down into their properties (const-ness,
// ref-ness, etc) each of which are processed independently from
// the others.
class KindProperties {
 private:
  bool isConst = false;
  bool isRef = false;
  bool isType = false;
  bool isParam = false;
  bool isValid = false;

  KindProperties() {}

  KindProperties(bool isConst, bool isRef, bool isType,
                 bool isParam)
    : isConst(isConst), isRef(isRef), isType(isType),
      isParam(isParam), isValid(true) {}

 public:
  static KindProperties fromKind(QualifiedType::Kind kind) {
    if (kind == QualifiedType::TYPE)
      return KindProperties(false, false, true, false);
    if (kind == QualifiedType::PARAM)
      // Mark params as const to cover the case in which a
      // param decays to a const var.
      return KindProperties(true, false, false, true);
    auto isConst = isConstQualifier(kind);
    auto isRef = isRefQualifier(kind);
    return KindProperties(isConst, isRef, false, false);
  }

  void invalidate() {
    isRef = isConst = isType = isParam = isValid = false;
  }

  void setParam(bool isParam) {
    this->isParam = isParam;
  }

  void combineWith(const KindProperties& other) {
    if (!isValid) return;
    if (!other.isValid || isType != other.isType) {
      // Can't mix types and non-types.
      invalidate();
      return;
    }
    isConst = isConst || other.isConst;
    isRef = isRef && other.isRef;
    isParam = isParam && other.isParam;
  }

  void strictCombineWith(const KindProperties& other) {
    if (!isValid) return;
    if (!other.isValid || isType != other.isType) {
      // Can't mix types and non-types.
      invalidate();
      return;
    }
    if (isParam && !other.isParam) {
      // If a param is required, can't return a non-param.
      invalidate();
      return;
    }
    // Ensuring that everything can actually be made
    // into a reference and const will happen later.
    // leave isRef and isConst as specified.
    // We could do some checking now, but that might be a bit premature.
  }

  QualifiedType::Kind toKind() const {
    if (!isValid) return QualifiedType::UNKNOWN;
    if (isType) return QualifiedType::TYPE;
    if (isParam) return QualifiedType::PARAM;
    if (isConst) {
      return isRef ? QualifiedType::CONST_REF : QualifiedType::CONST_VAR ;
    } else {
      return isRef ? QualifiedType::REF : QualifiedType::VAR ;
    }
  }

  bool valid() const { return isValid; }
};

static llvm::Optional<QualifiedType>
findByPassing(Context* context,
              const std::vector<QualifiedType>& types) {
  for (auto& type : types) {
    bool fitsOthers = true;
    for (auto& otherType : types) {
      if (otherType == type) continue;
      auto passes = canPass(context, otherType, type);
      if (!passes.passes() || passes.promotes()) {
        fitsOthers = false;
        break;
      }
    }
    if (fitsOthers) return type;
  }
  return llvm::Optional<QualifiedType>();
}

llvm::Optional<QualifiedType>
commonType(Context* context,
           const std::vector<QualifiedType>& types,
           KindRequirement requiredKind) {
  CHPL_ASSERT(types.size() > 0);

  // figure out the kind
  auto properties = KindProperties::fromKind(types.front().kind());
  for (auto& type : types) {
    if (type.isUnknown()) {
      // if any type is unknown, we can't figure out the common type,
      // but it's not an error.
      return QualifiedType();
    }
    auto kind = type.kind();
    auto typeProperties = KindProperties::fromKind(kind);
    properties.combineWith(typeProperties);
  }

  if (requiredKind) {
    // The caller enforces a particular kind on us. Make sure that the
    // computed properties line up with the kind.
    auto requiredProperties = KindProperties::fromKind(requiredKind.getValue());
    requiredProperties.strictCombineWith(properties);
    properties = requiredProperties;
  }

  // We can't reconcile the intents. Return with error.
  if (!properties.valid()) return llvm::Optional<QualifiedType>();
  auto bestKind = properties.toKind();

  // Create a new list of types with their kinds adjusted.
  std::vector<QualifiedType> adjustedTypes;
  for (const auto& type : types) {
    const Param* param = type.param();
    if (bestKind != QualifiedType::PARAM) {
      // if we've dropped from params to values, clear the params
      // from all types
      param = nullptr;
    }
    auto adjustedType = QualifiedType(bestKind, type.type(), param);
    adjustedTypes.push_back(std::move(adjustedType));
  }

  // Try apply usual coercion rules to find common type
  // Performance: if the types vector ever becomes very long,
  // it might be worth using a unique'd vector here.
  auto commonType = findByPassing(context, adjustedTypes);
  if (commonType) {
    return commonType;
  }

  bool paramRequired = requiredKind &&
    requiredKind.getValue() == QualifiedType::PARAM;
  if (bestKind == QualifiedType::PARAM && !paramRequired) {
    // We couldn't unify the types as params, but maybe if we downgrade
    // them to values, it'll work.
    properties.setParam(false);
    bestKind = properties.toKind();
    for (auto& adjustedType : adjustedTypes) {
      // adjust kind and strip param
      adjustedType = QualifiedType(bestKind, adjustedType.type());
    }

    commonType = findByPassing(context, adjustedTypes);
    if (commonType) {
      return commonType;
    }
  }
  return llvm::Optional<QualifiedType>();
}

} // end namespace resolution
} // end namespace chpl
