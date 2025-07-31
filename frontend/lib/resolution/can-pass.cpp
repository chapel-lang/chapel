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

#include "chpl/resolution/can-pass.h"

#include "chpl/resolution/resolution-queries.h"
#include "chpl/parsing/parsing-queries.h"
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
      return false;
    case Type::GENERIC_WITH_DEFAULTS:
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

    if (auto actualIntT = actualT->toIntType()) {
      if (actualIntT->bitwidth() <= formalUintT->bitwidth()) {
        // int can coerce to uint
        return true;
      }
    }
  }

  if (auto formalRealT = formalT->toRealType()) {
    // don't convert bools to reals (per spec: "unintended by programmer")

    // coerce any integer type to any width real
    if (actualT->isIntegralType())
      return true;

    // convert real from smaller size
    if (auto actualRealT = actualT->toRealType())
      if (actualRealT->bitwidth() < formalRealT->bitwidth())
        return true;

  }

  if (auto formalImagT = formalT->toImagType()) {
    // convert imag from smaller size
    if (auto actualImagT = actualT->toImagType())
      if (actualImagT->bitwidth() < formalImagT->bitwidth())
        return true;
  }

  if (auto formalComplexT = formalT->toComplexType()) {
    // don't convert bools to complexes (per spec: "unintended by programmer")

    // coerce any integer type to any width complex
    if (actualT->isIntegralType())
      return true;

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

  }

  return false;
}

bool
CanPassResult::canConvertCPtr(Context* context,
                              const Type* actualT,
                              const Type* formalT) {
  if (auto actualPtr = actualT->toCPtrType()) {
    if (auto formalPtr = formalT->toCPtrType()) {
      // Allow constness casts: an int* can be a const int*.
      // In Chapel lingo, `c_ptr(int)` is passable to `c_ptrConst(int)`.
      if (formalPtr->isConst() && !actualPtr->isConst() &&
          formalPtr->eltType() == actualPtr->eltType())
        return true;

      return formalPtr->isVoidPtr();
    } else {
      // Check for old c_void_ptr behavior.
      return formalT->isCVoidPtrType();
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
    // If the argument is a param, does it
    // store a value that's small enough that it could dispatch to
    // this argument?
    //
    return paramFitsInInt(formalIntT->bitwidth(), actualP);
  }

  if (auto formalUintT = formalT->toUintType()) {
    return paramFitsInUint(formalUintT->bitwidth(), actualP);
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
    if (actual.isNilable() == formal.isNilable()) {
      if (formal.isUnknownManagement()) {
        // Account for the case when passing an unmanaged class type to
        // an any-managed type formal, which is technically an instantiation.
        return CanPassResult(/* no fail reason, passes */ {},
                             /*instantiates*/ true,
                             /*promotes*/ false,
                             /*conversion*/ NONE);
      }
    }
    return passAsIs();
  }

  bool instantiates = false;
  ConversionKind conversion = NONE;
  optional<PassingFailureReason> fails = {};

  ClassTypeDecorator actualNily = actual.toBorrowed();
  ClassTypeDecorator formalNily = formal.toBorrowed();

  ClassTypeDecorator actualMgmt = actual.removeNilable();
  ClassTypeDecorator formalMgmt = formal.removeNilable();

  // consider nilability.
  if (actualNily != formalNily) {
    if (formalNily.isUnknownNilability())
      instantiates = true; // instantiating with passed nilability
    else if (actualNily.isNonNilable() && formalNily.isNilable())
      conversion = SUBTYPE;  // non-nil to nil conversion
    else
      fails = FAIL_INCOMPATIBLE_NILABILITY; // all other nilability cases
  }

  // consider management.
  if (actualMgmt != formalMgmt) {
    if (formalMgmt.isUnknownManagement())
      instantiates = true;  // instantiating with passed management
    else if (formalMgmt.isBorrowed()) {
      // management can convert to borrowed
      if (conversion == NONE)
        conversion = BORROWS;
      else if (conversion == SUBTYPE)
        conversion = BORROWS_SUBTYPE;
      else
        CHPL_ASSERT(false && "should be unreachable");
    } else
      fails = FAIL_INCOMPATIBLE_MGMT;
  }

  if (fails)
    return fail(*fails);

  return CanPassResult(/* no fail reason, passes */ {},
                       instantiates,
                       /*promotes*/ false,
                       conversion);
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
CanPassResult CanPassResult::canPassSubtypeNonBorrowing(Context* context,
                                            const Type* actualT,
                                            const Type* formalT) {
  // nil -> pointers
  if (actualT->isNilType() && formalT->isNilablePtrType() &&
      !formalT->isCStringType()) {
    return CanPassResult(/* no fail reason */ {},
                         /* instantiates */ false,
                         /*promotes*/ false,
                         /*conversion*/ SUBTYPE);
  }

  // class types
  if (auto actualCt = actualT->toClassType()) {
    if (auto formalCt = formalT->toClassType()) {
      // owned Child -> owned Parent
      // owned Child -> owned Parent?
      // owned Child? -> owned Parent?

      // check decorators allow passing
      CanPassResult decResult = canPassDecorators(context,
                                                  actualCt->decorator(),
                                                  formalCt->decorator());
      if (!decResult.passes()) return decResult;
      CHPL_ASSERT(!decResult.promotes_);

      if (actualCt->decorator().isManaged() &&
          formalCt->decorator().isManaged() &&
          actualCt->manager() != formalCt->manager()) {
        // disallow e.g. owned C -> shared C
        return fail(FAIL_INCOMPATIBLE_MGR);
      }

      auto actualBct = actualCt->basicClassType();
      auto formalBct = formalCt->basicClassType();

      // Only consider subtype conversions here.
      bool converts = (decResult.conversionKind_ == SUBTYPE ||
                       decResult.conversionKind_ == BORROWS_SUBTYPE);
      bool instantiates = decResult.instantiates_;

      bool pass = false;
      if (formalCt->manageableType()->isAnyClassType()) {
        // Formal is the generic `class`. This is an instantiation since
        // that's always generic, but it might also be a conversion if
        // nilability is involved.
        instantiates = true;
        pass = true;
      } else if (actualCt->manageableType()->isAnyClassType()) {
        // We might encounter this case when relying on 'canPass' to implement
        // the various 'subtype' primitives. Types like 'borrowed class' are
        // valid arguments in production, and can be found in the where-clauses
        // of some basic operator implementations.

        // From the previous conditional, we know the formal isn't another
        // 'any' class, so this actual cannot be passed.
        pass = false;
      } else if (actualBct->isSubtypeOf(context, formalBct, converts, instantiates)) {
        // the basic class types are the same
        // or there was a subclass relationship
        // or there was instantiation
        pass = true;
      }

      if (pass) {
        ConversionKind conversion = converts ? SUBTYPE : NONE;
        return CanPassResult(/* no fail reason */ {},
                             instantiates,
                             /*promotes*/ false,
                             conversion);
      }
    }
  }

  // TODO: c_ptr(t) -> c_ptr(void)
  // TODO: c_array -> c_ptr(void), c_array(t) -> c_ptr(t)

  return fail(FAIL_EXPECTED_SUBTYPE);
}

// Like canPassSubtypeNonBorrowing, but considers subtyping conversions and/or implicit
// borrowing conversions.
// This function returns CanPassResult which always has
// conversion kind of NONE, SUBTYPE, BORROWS, or BORROWS_SUBTYPE.
CanPassResult CanPassResult::canPassSubtypeOrBorrowing(Context* context,
                                                       const Type* actualT,
                                                       const Type* formalT) {
  // First check if we can pass directly or with subtype conversion, ignoring
  // any borrowing that may be necessary.
  auto result = canPassSubtypeNonBorrowing(context, actualT, formalT);
  if (result.passes()) {
    const ConversionKind subtypingConversion = result.conversionKind_;
    CHPL_ASSERT(subtypingConversion == NONE || subtypingConversion == SUBTYPE);

    // Check if borrowing is required for the conversion.
    ConversionKind borrowingConversion = NONE;
    if (auto actualCt = actualT->toClassType()) {
      if (auto formalCt = formalT->toClassType()) {
        CanPassResult decResult = canPassDecorators(
            context, actualCt->decorator(), formalCt->decorator());
        CHPL_ASSERT(decResult.passes() && "expected types known to pass");
        // Extract borrowing conversion info only.
        borrowingConversion = decResult.conversionKind();
        if (borrowingConversion == SUBTYPE) borrowingConversion = NONE;
      }
    }

    // Adjust subtyping component of conversion to reflect necessary borrowing.
    ConversionKind adjustedConversion;
    if (borrowingConversion == NONE) {
      // no adjustment needed
      adjustedConversion = subtypingConversion;
    } else if (borrowingConversion == BORROWS) {
      if (subtypingConversion == NONE) {
        adjustedConversion = BORROWS;
      } else {
        adjustedConversion = BORROWS_SUBTYPE;
      }
    } else {
      CHPL_ASSERT(borrowingConversion == BORROWS_SUBTYPE);
      CHPL_ASSERT(subtypingConversion == SUBTYPE);
      adjustedConversion = BORROWS_SUBTYPE;
    }

    result.conversionKind_ = adjustedConversion;
    return result;
  } else {
    return fail(FAIL_EXPECTED_SUBTYPE);
  }
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
    return fail(FAIL_INCOMPATIBLE_TUPLE_SIZE);
  }

  if (aT->isStarTuple() != fT->isStarTuple()) {
    // Star-tuple-ness differs, so not convertible.
    return fail(FAIL_INCOMPATIBLE_TUPLE_STAR);
  }

  if (aT->toReferentialTuple(context) == fT) {
    return convert(TO_REFERENTIAL_TUPLE);
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
      auto got = canPassScalar(context, aElt, fElt);
      if (!got.passes()){
        // TODO: figure out how to propagate this information.
        return fail(FAIL_FORMAL_OTHER);
      } else if (got.promotes()) {
        return fail(FAIL_FORMAL_OTHER);
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

  // can we convert with a subtype conversion (including class subtyping) and/or
  // a borrowing conversion?
  {
    auto got = canPassSubtypeOrBorrowing(context, actualT, formalT);
    if (got.passes()) {
      CHPL_ASSERT(got.conversionKind_ == NONE ||
                  got.conversionKind_ == SUBTYPE ||
                  got.conversionKind_ == BORROWS ||
                  got.conversionKind_ == BORROWS_SUBTYPE);
      return got;
    }
  }

  // can we convert with a numeric conversion?
  if (canConvertNumeric(context, actualT, formalT))
    return convert(NUMERIC);

  // can we convert with param narrowing?
  if (canConvertParamNarrowing(context, actualQT, formalQT))
    return convert(PARAM_NARROWING);

  if (canConvertCPtr(context, actualT, formalT)) {
    return convert(OTHER);
  }

  if (actualQT.isParam() && actualT->isStringType() && formalT->isCPtrType()) {
    auto ptr = formalT->toCPtrType();
    auto charType = typeForSysCType(context, USTR("c_char"));
    if (ptr->isConst() && ptr->eltType() == charType.type()) {
      return convert(OTHER);
    }
  }

  // can we convert tuples?
  if (actualQT.type()->isTupleType() && formalQT.type()->isTupleType()) {
    auto aT = actualQT.type()->toTupleType();
    auto fT = formalQT.type()->toTupleType();

    // TODO: for now, return the result from the tuple conversion no matter
    // what it is. This is better than falling through to the catch-all
    // fail(...) below because it propagates the error.
    return canConvertTuples(context, aT, fT);
  }

  // TODO: check for conversion to copy type
  // (relevant for array slices and iterator records)
  // TODO: port canCoerceToCopyType

  return fail(FAIL_CANNOT_CONVERT);
}

// handles formalT being a builtin generic type like integral
bool CanPassResult::canInstantiateBuiltin(Context* context,
                                          const Type* actualT,
                                          const Type* formalT) {
  if (formalT->isAnyType())
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

  if (formalT->isAnyIteratorClassType()) {
    // TODO: represent iterators
    return false;
  }

  if (formalT->isAnyIteratorRecordType()) {
    return actualT->isIteratorType();
  }

  if (formalT->isAnyNumericType() && actualT->isNumericType())
    return true;

  if (formalT->isAnyOwnedType())
    if (auto ct = actualT->toClassType())
      if (ct->decorator().isManaged())
        if (auto manager = ct->manager())
          if (manager->isAnyOwnedType())
            return true;

  if (formalT->isAnyPodType()) {
    CHPL_UNIMPL("POD types"); // TODO: compute POD-ness
    return false;
  }

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

  return false;
}

static bool classTypeIsManagedAndDecorated(Context* context,
                                           const ClassType* ct) {
  if (ct && ct->manager() && ct->decorator().isManaged()) {
    return true;
  }
  return false;
}

bool
tryConvertClassTypeIntoManagerRecordIfNeeded(Context* context,
                                             const Type* const & mightBeManagerRecord,
                                             const Type*& mightBeClass) {
  if (!mightBeManagerRecord || !mightBeClass) return false;

  auto mr = mightBeManagerRecord->toRecordType();
  auto ct = mightBeClass->toClassType();
  auto aot = mightBeClass->toAnyOwnedType();
  auto ast = mightBeClass->toAnySharedType();

  bool isCtManagedAndDecorated = classTypeIsManagedAndDecorated(context, ct);
  // if mightBeManagerRecord is a record type and (mightBeClass is a class type
  // that is managed and decorated or mightBeClass is the generic 'owned' or 
  // 'shared' type) we can continue
  if (!mr || (!isCtManagedAndDecorated && !aot && !ast )) return false;

  if (!parsing::idIsInBundledModule(context, mr->id())) return false;

  auto ag = parsing::idToAttributeGroup(context, mr->id());
  if (!ag || !ag->hasPragma(pragmatags::PragmaTag::PRAGMA_MANAGED_POINTER)) {
    return false;
  }

  // Override the class type to the manager record type
  // mightBeClass used to be `owned` of type ClassType,
  // now it's `_owned` of type RecordType
  if (aot) {
    mightBeClass = CompositeType::getOwnedRecordType(context, /*bct*/ nullptr);
  } else if (ast) {
    mightBeClass = CompositeType::getSharedRecordType(context, /*bct*/ nullptr);
  } else {
    mightBeClass = ct->managerRecordType(context);
  }

  return true;
}

static optional<std::pair<const RecordType*, const RecordType*>>
shouldConvertClassTypeIntoManagerRecord(Context* context,
                                        const Type* actualT,
                                        const Type* formalT) {
  if (tryConvertClassTypeIntoManagerRecordIfNeeded(context, formalT, actualT) ||
      tryConvertClassTypeIntoManagerRecordIfNeeded(context, actualT, formalT)) {
    CHPL_ASSERT(formalT->isRecordType() && actualT->isRecordType());
    return std::make_pair(actualT->toRecordType(), formalT->toRecordType());
  }

  return empty;
}

CanPassResult CanPassResult::ensureSubtypeConversionInstantiates(CanPassResult got) {
  if (got.instantiates()) {
    return got;
  }

  // The type passed, but didn't actually instantiate. This is odd
  // since we are trying to instantiate a generic formal. This suggests
  // the actual is generic, too, which typically doesn't make sense.
  // Explicitly set the fail reason but keep the rest of the properties
  // intact, so that that the caller can dismiss this error if
  // generic actuals are allowed.
  got.failReason_ = FAIL_DID_NOT_INSTANTIATE;
  return got;
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
  //
  // note: 'class' generic handling is below, because it shares
  // some logic for owned -> shared and nilable -> nonnilable conversions (etc.).
  // It might need to be copied / moved up here, but for now, it's identical to
  // regular parent / child logic, so leave it there.
  if (canInstantiateBuiltin(context, actualT, formalT)) {
    return instantiate();
  }

  // this is to allow instantiating 'class?' type with nil
  if (actualT->isNilType()) {
    auto got = canPassSubtypeOrBorrowing(context, actualT, formalT);
    if (got.passes()) {
      return ensureSubtypeConversionInstantiates(got);
    }
  }

  // TODO: check for constrained generic types

  if (auto actualIt = actualT->toInterfaceType()) {
    if (auto formalIt = formalT->toInterfaceType()) {
      if (actualIt->isInstantiationOf(context, formalIt)) {
        return instantiate();
      }
    }
  }

  if (auto actualCt = actualT->toClassType()) {
    // check for instantiating classes
    if (auto formalCt = formalT->toClassType()) {
      CanPassResult got = canPassSubtypeOrBorrowing(context, actualCt, formalCt);
      if (got.passes()) {
        return ensureSubtypeConversionInstantiates(got);
      }
    }
  } else if (auto actualCt = actualT->toCompositeType()) {
    // check for instantiating records/unions/tuples
    if (auto formalCt = formalT->toCompositeType()) {
      // Quick check to disallow passing tuples of mismatching sizes when the
      // sizes are known.
      if (actualCt->isTupleType() && formalCt->isTupleType()) {
        auto at = actualCt->toTupleType();
        auto ft = formalCt->toTupleType();
        if (at->isKnownSize() && ft->isKnownSize() &&
            at->numElements() != ft->numElements()) {
          return fail(FAIL_INCOMPATIBLE_TUPLE_SIZE);
        }
      }

      if (actualCt->isInstantiationOf(context, formalCt)) {
        return instantiate();
      }
    }
  } else if (auto actualPt = actualT->toPtrType()) {
    if (auto formalPt = formalT->toPtrType()) {
      // Check first if they're direct instantiations (c_ptr(int(?w)) <- c_ptr(int)).
      if (actualPt->isInstantiationOf(context, formalPt)) {
        return instantiate();
      }

      if (auto actualCPt = actualPt->toCPtrType()) {
        if (auto formalCPt = formalPt->toCPtrType()) {
          // Instantiation might still be possible, together with a coercion, if
          // the formal is const but the actual isn't.
          formalCPt = formalCPt->withoutConst(context);

          if (actualCPt->isInstantiationOf(context, formalCPt)) {
            return CanPassResult(/* no fail reason, passes */ {},
                                 /* instantiates */ true,
                                 /* promotes */ false,
                                 /* converts */ ConversionKind::OTHER);
          }
        }
      }
    }
  }

  return fail(FAIL_CANNOT_INSTANTIATE);
}

CanPassResult CanPassResult::canPassScalar(Context* context,
                                           const QualifiedType& actualQtIn,
                                           const QualifiedType& formalQtIn) {
  auto actualQT = actualQtIn;
  auto formalQT = formalQtIn;

  const Type* actualT = actualQT.type();
  const Type* formalT = formalQT.type();
  CHPL_ASSERT(actualT && formalT);
  if (auto managerRecordPair =
           shouldConvertClassTypeIntoManagerRecord(context, actualT, formalT)) {
    actualT = managerRecordPair->first;
    formalT = managerRecordPair->second;
    actualQT = QualifiedType(actualQT.kind(), actualT, actualQT.param());
    formalQT = QualifiedType(formalQT.kind(), formalT, formalQT.param());
  }

  // if the formal type is unknown, allow passing
  // this can come up with e.g.
  //   proc f(a: int(?w), b: int(2*w))
  // when computing an initial candidate, 'b' is unknown
  // but we should allow passing an argument to it.
  if (formalT->isUnknownType()) {
    return instantiate();
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
    return fail(FAIL_TYPE_VS_NONTYPE);

  // param actuals can pass to non-param formals
  if (formalQT.isParam() && !actualQT.isParam() && !typeQueryActual)
    return fail(FAIL_NOT_PARAM);

  // check params
  const Param* actualParam = actualQT.param();
  const Param* formalParam = formalQT.param();
  if (actualParam && formalParam) {
    // Note: Comparing 'actualParam' and 'formalParam' here isn't quite enough.
    // We could have squeezed an int(64) '0' into a uint(64) formal, which
    // would then be instantiated as a uint(64) '0'.
    //
    // Passing a param with a value to another such param isn't valid in Chapel,
    // so the caller should be responsible for catching that case and dealing
    // with it based on the context of the two arguments.
  } else if (formalParam && !actualParam) {
    // this case doesn't make sense
    CHPL_ASSERT(false && "case not expected");
    return fail(FAIL_FORMAL_OTHER);
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
      if (isTypeGeneric(context, formalQT)) {
        return instantiate();
      } else {
        return passAsIs();
      }
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
    return fail(FAIL_UNKNOWN_ACTUAL_TYPE); // actual type not established
  }

  if (formalQT.isUnknownKindOrType()) {
    return fail(FAIL_UNKNOWN_FORMAL_TYPE); // unknown formal type, can't resolve
  }

  if (isTypeGeneric(context, formalQT)) {
    // Check to see if we should proceed with instantiation.
    // Further checking will occur after the instantiation occurs,
    // so checking here just rules out predictable situations.

    bool canAcceptGenericActuals =
      formalQT.kind() == QualifiedType::TYPE ||
      actualQT.kind() == QualifiedType::INIT_RECEIVER;

    if (isTypeGeneric(context, actualQT) && !canAcceptGenericActuals)
      return fail(FAIL_GENERIC_TO_NONTYPE); // generic types can only be passed to type actuals

    auto got = canInstantiate(context, actualQT, formalQT);
    if (!got.passes() &&
         got.reason() == FAIL_DID_NOT_INSTANTIATE && canAcceptGenericActuals) {
      // No instantiation occurred, but the actual isn't incompatible with
      // the formal. This suggests a generic actual being passed to the
      // formal; this is typically not allowed, but in this case
      // (canAcceptGenericActuals) it is. So, it's not an error.
      got.failReason_ = chpl::optional<PassingFailureReason>();
      return got;
    }
    if (!got.passes() && formalQT.isParam()) {
      // 'isTypeGeneric' will return 'true' if there is not a param value
      // for the given QualifiedType, which is usually the case for a param
      // formal despite the presence of a type expression.
      //
      // Fall through to the checks below.
    } else {
      return got;
    }
  }

  // if the formal type doesn't need instantiation, do additional checking
  // (if it is generic, we will do this checking after instantiation)
  switch (formalQT.kind()) {
    case QualifiedType::UNKNOWN:
    case QualifiedType::FUNCTION:
    case QualifiedType::PARENLESS_FUNCTION:
    case QualifiedType::MODULE:
    case QualifiedType::LOOP:
    case QualifiedType::TYPE_QUERY:
    case QualifiedType::INDEX:
    case QualifiedType::DEFAULT_INTENT:
    case QualifiedType::CONST_INTENT:
    case QualifiedType::OUT: // handled above
      // no additional checking for these
      break;

    case QualifiedType::REF:
      // ref type requires same type which is ruled out above
      return fail(FAIL_NOT_EXACT_MATCH);

    case QualifiedType::CONST_REF:
    case QualifiedType::REF_MAYBE_CONST:
    case QualifiedType::TYPE:
      {
        auto actualTup = actualT->toTupleType();
        if (actualTup != nullptr  && formalT->isTupleType()) {
          if (formalQT.kind() == QualifiedType::TYPE &&
                actualTup->toValueTuple(context) == formalT) {
            return passAsIs();
          } else if (formalQT.kind() != QualifiedType::TYPE) {
            auto got = canConvert(context, actualQT, formalQT);
            if (got.passes()) {
              return got;
            }
          }
        }
        return canPassSubtypeNonBorrowing(context, actualT, formalT);
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
        }
        return got;
      }

    case QualifiedType::IN:
    case QualifiedType::CONST_IN:
    case QualifiedType::INOUT:
    case QualifiedType::VAR:       // var/const var don't really make sense
    case QualifiedType::CONST_VAR: // as formals but we allow it for testing
    case QualifiedType::INIT_RECEIVER:
      {
        return canConvert(context, actualQT, formalQT);
      }
  }

  return fail(FAIL_FORMAL_OTHER);
}

CanPassResult CanPassResult::canPass(Context* context,
                                     const QualifiedType& actualQtIn,
                                     const QualifiedType& formalQtIn) {
  auto got = canPassScalar(context, actualQtIn, formalQtIn);
  if (got.passes()) {
    return got;
  }

  // Scalar passing failing, but promotion may be possible.
  if (formalQtIn.kind() != QualifiedType::TYPE &&
      formalQtIn.kind() != QualifiedType::PARAM) {
    auto promotionType = getPromotionType(context, actualQtIn, /*skipIfRunning */ true);

    if (!promotionType.isUnknownOrErroneous()) {
      auto got = canPassScalar(context, promotionType, formalQtIn);

      if (got.passes()) {
        return CanPassResult(/* no fail reason */ {},
                             /* instantiates */ got.instantiates(),
                             /* promotes */ true,
                             /* converts */ got.conversionKind());
      }
    }
  }

  return got;
}

bool canInstantiateSubstitutions(Context* context,
                                 const SubstitutionsMap& instances,
                                 const SubstitutionsMap& generics,
                                 bool allowMissing) {
  // Check to see if the substitutions in `instances` are all instantiations
  // of the substitutions in `generics`
  //
  // check, for each substitution in mySubs, that it matches
  // or is an instantiation of pSubs.

  for (const auto& mySubPair : instances) {
    ID mySubId = mySubPair.first;
    QualifiedType mySubType = mySubPair.second;

    // look for a substitution in pSubs with the same ID
    auto pSearch = generics.find(mySubId);
    if (pSearch != generics.end()) {
      QualifiedType pSubType = pSearch->second;
      // check the types
      auto r = canPass(context, mySubType, pSubType);
      if (r.passes() && !r.promotes() && !r.converts()) {
        // instantiation and same-type passing are allowed here
        //
        // canPass doesn't check param values for equivalence to allow handling
        // coercions and narrowing, so explicitly check them here.
        if (pSubType.isParam()) {
          bool compatible = pSubType.param() == nullptr ||
                            mySubType.param() == pSubType.param();
          if (!compatible) {
            return false;
          }
        }
      } else {
        // it was not an instantiation
        return false;
      }
    } else if (!allowMissing) {
      // If the ID isn't found, then that means the generic component doesn't
      // exist in the other type, which means this cannot be an instantiation
      // of the other type.
      //
      // How could we reach this condition? One path here involves passing a
      // tuple to a tuple formal with a fewer number of elements. For example,
      // passing "(1, 2, 3)" to "(int, ?)".
      return false;
    } else {
      // A substitution is missing in the partial type, but we have one.
      // For a composite type, that might just mean that we are foo(X, Y),
      // while the partial is foo(X, ?) -- partially generic. So, this is
      // fine, don't return false.
    }
  }

  return true;

}

void KindProperties::invalidate() {
  isRef = isConst = isType = isParam = isValid = false;
}

KindProperties KindProperties::fromKind(QualifiedType::Kind kind) {
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

void KindProperties::setRef(bool isRef) {
  this->isRef = isRef;
}

void KindProperties::setParam(bool isParam) {
  this->isParam = isParam;
}

void KindProperties::setConst(bool isConst) {
  this->isConst = isConst;
}

bool KindProperties::checkValidCombine(const KindProperties& other) const {
  if (!isValid || !other.isValid) {
    return false;
  }
  if (isType != other.isType) {
    // Can't mix types and non-types.
    return false;
  }

  return true;
}

void KindProperties::combineWithJoin(const KindProperties& other) {
  if (!checkValidCombine(other)) {
    invalidate();
    return;
  }

  isConst = isConst || other.isConst;
  isRef = isRef && other.isRef;
  isParam = isParam && other.isParam;
}

void KindProperties::combineWithMeet(const KindProperties& other) {
  bool bothConst = isConst && other.isConst;
  combineWithJoin(other);
  isConst = bothConst;
}

void KindProperties::strictCombineWith(const KindProperties& other) {
  if (!checkValidCombine(other)) {
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

types::QualifiedType::Kind KindProperties::combineKindsMeet(
    types::QualifiedType::Kind kind1, types::QualifiedType::Kind kind2) {
  auto kp1 = KindProperties::fromKind(kind1);
  auto kp2 = KindProperties::fromKind(kind2);
  kp1.combineWithMeet(kp2);
  return kp1.toKind();
}

QualifiedType::Kind
KindProperties::addConstness(QualifiedType::Kind kind) {
  auto kp = KindProperties::fromKind(kind);
  kp.setConst(true);
  return kp.toKind();
}

QualifiedType::Kind
KindProperties::addRefness(QualifiedType::Kind kind) {
  auto kp = KindProperties::fromKind(kind);
  kp.setRef(true);
  return kp.toKind();
}

QualifiedType::Kind KindProperties::toKind() const {
  if (!isValid) return QualifiedType::UNKNOWN;
  if (isType) return QualifiedType::TYPE;
  if (isParam) return QualifiedType::PARAM;
  if (isConst) {
    return isRef ? QualifiedType::CONST_REF : QualifiedType::CONST_VAR ;
  } else {
    return isRef ? QualifiedType::REF : QualifiedType::VAR ;
  }
}

types::QualifiedType::Kind KindProperties::makeConst(types::QualifiedType::Kind kind) {
  auto props = KindProperties::fromKind(kind);
  props.setConst(true);
  return props.toKind();
}

types::QualifiedType::Kind KindProperties::removeRef(types::QualifiedType::Kind kind) {
  auto props = KindProperties::fromKind(kind);
  props.setRef(false);
  return props.toKind();
}

// Try finding a common ancestor type between class types
static optional<QualifiedType> findByAncestor(
    Context* context, const std::vector<QualifiedType>& types,
    const KindRequirement& requiredKind) {
  // disallow subtype conversions for ref intents
  if (requiredKind && isRefQualifier(*requiredKind)) return chpl::empty;

  std::vector<QualifiedType> parentTypes;
  for (const auto& type : types) {
    auto ct = type.type()->toClassType();
    if (!ct) return chpl::empty;
    auto bct = ct->basicClassType();
    if (!bct) return chpl::empty;
    auto pct = bct->parentClassType();
    // don't consider the root of the class hierarchy for a common type
    if (!pct || pct->isObjectType()) return chpl::empty;

    parentTypes.emplace_back(QualifiedType(
        type.kind(),
        ClassType::get(context, pct, ct->manager(), ct->decorator())));
  }

  if (parentTypes.empty()) return chpl::empty;

  return commonType(context, parentTypes, requiredKind);
}

static optional<QualifiedType>
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
  return chpl::empty;
}

optional<QualifiedType>
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
    properties.combineWithJoin(typeProperties);
  }

  if (requiredKind) {
    // The caller enforces a particular kind on us. Make sure that the
    // computed properties line up with the kind.
    auto requiredProperties = KindProperties::fromKind(*requiredKind);
    requiredProperties.strictCombineWith(properties);
    properties = requiredProperties;
  }

  // We can't reconcile the intents. Return with error.
  if (!properties.valid()) return chpl::empty;
  auto bestKind = properties.toKind();

  // Check param values of each type. If a param is required and the values
  // do not match then there is no common type.
  //
  // If a param is not required and there is a mismatch continue on without
  // param-ness.
  bool paramRequired = requiredKind &&
                       *requiredKind == QualifiedType::PARAM;
  if (bestKind == QualifiedType::PARAM || paramRequired) {
    auto firstParam = types.front().param();
    bool mismatch = false;
    for (auto& type : types) {
      mismatch = mismatch || type.param() != firstParam;
    }
    if (mismatch) {
      if (paramRequired) {
        return chpl::empty;
      } else {
        properties.setParam(false);
        bestKind = properties.toKind();
      }
    }
  }

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
  if (auto commonType = findByPassing(context, adjustedTypes))
    return commonType;

  if (auto commonType = findByAncestor(context, adjustedTypes, requiredKind))
    return commonType;

  return chpl::empty;
}

} // end namespace resolution
} // end namespace chpl
