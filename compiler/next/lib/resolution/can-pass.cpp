/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#include "chpl/types/all-types.h"

#include <cmath>
#include <cinttypes>

namespace chpl {
namespace resolution {

using namespace uast;
using namespace types;

static bool fitsInIntHelper(int width, int64_t val) {
  switch (width) {
    default: assert(false && "bad width in fits_in_int_helper");
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
  default: assert(false && "bad width in fits_in_uint_helper");
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
  assert(false && "unknown mantissa width");
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

  assert(width < 64);

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
    assert(false && "unsupported number kind");
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

// can we do an implicit numeric conversion?
// does not check for param narrowing conversions
bool CanPassResult::canConvertNumeric(const types::Type* actualT,
                                      const types::Type* formalT) {
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
CanPassResult::canConvertParamNarrowing(const QualifiedType& actualQT,
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

// The compiler considers many patterns of "subtyping" as things that require
// implicit conversions (they often require implicit conversions in the
// generated C).  However not all implicit conversions are created equal.
// Some of them are implementing subtyping.
//
// Here we consider an implicit conversion to be implementing "subtyping" if,
// in an ideal implementation, the actual could be passed to a `const ref`
// argument of the formal type.
bool CanPassResult::isSubtype(const Type* actualT,
                              const Type* formalT) {
  // nil -> pointers and class types
  if (actualT->isNilType() && formalT->isAnyNilablePtrType() &&
      !formalT->isCStringType())
    return true;

  if (auto actualCt = actualT->toClassType()) {
    if (auto formalCt = formalT->toClassType()) {
      // owned Child -> owned Parent
      if (actualCt->decorator().isManaged() &&
          formalCt->decorator().isManaged() &&
          actualCt->manager() == formalCt->manager()) {
        return isSubtype(actualCt->basicClassType(),
                         formalCt->basicClassType());
      }

      // check decorators allow subtyping conversion
      auto actualDec = actualCt->decorator().val();
      auto formalDec = formalCt->decorator().val();
      bool ok = ClassTypeDecorator::canCoerceDecorators(
                                        actualDec, formalDec,
                                        /* allowNonSubtypes */ false,
                                        /* implicitBang */ false);
      if (ok) {
        auto actualBct = actualCt->basicClassType();
        auto formalBct = formalCt->basicClassType();

        // are the decorated class types the same?
        if (actualBct == formalBct)
          return true;

        // are we passing a subclass?
        // TODO: check for subclass relationship
      }
    }
  }

  // TODO: c_ptr -> c_void_ptr
  // TODO: c_array -> c_void_ptr, c_array(t) -> c_ptr(t)

  return false;
}

CanPassResult CanPassResult::canConvert(const QualifiedType& actualQT,
                                        const QualifiedType& formalQT) {
  const Type* actualT = actualQT.type();
  const Type* formalT = formalQT.type();

  if (isSubtype(actualT, formalT))
    return convert(SUBTYPE);

  // can we convert with a numeric conversion?
  if (canConvertNumeric(actualT, formalT))
    return convert(NUMERIC);

  // can we convert with param narrowing?
  if (canConvertParamNarrowing(actualQT, formalQT))
    return convert(PARAM_NARROWING);

  // can we convert tuples?
  if (actualQT.type()->isTupleType() && formalQT.type()->isTupleType()) {
    assert(false && "not implemented yet");
    // TODO: port canCoerceTuples from production compiler
  }

  // TODO: check for conversion to copy type
  // (relevant for array slices and iterator records)
  // TODO: port canCoerceToCopyType

  return fail();
}

CanPassResult CanPassResult::canPass(const QualifiedType& actualQT,
                                     const QualifiedType& formalQT) {

  const Type* actualT = actualQT.type();
  const Type* formalT = formalQT.type();
  assert(actualT && formalT);

  // check that the kinds are compatible

  // type formal, type actual -> OK
  // non-type formal, non-type actual -> OK
  // type formal, non-type actual -> error, can't pass value to type
  // non-type formal, type actual -> error, can't pass type to value
  if (formalQT.isType() != actualQT.isType())
    return fail();

  // param actuals can pass to non-param formals
  if (formalQT.isParam() && !actualQT.isParam())
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
    assert(false && "case not expected");
    return fail();
  }

  if (actualT == formalT) {
    return passAsIs();
  }

  // if the formal type is concrete, do additional checking
  // (if it is generic, we will do this checking after instantiation)
  if (!formalQT.isGenericOrUnknown()) {
    switch (formalQT.kind()) {
      case QualifiedType::UNKNOWN:
      case QualifiedType::FUNCTION:
      case QualifiedType::MODULE:
      case QualifiedType::INDEX:
      case QualifiedType::VAR:
      case QualifiedType::CONST_VAR:
      case QualifiedType::DEFAULT_INTENT:
      case QualifiedType::CONST_INTENT:
        // no additional checking for these
        break;

      case QualifiedType::REF:
        return fail(); // ref type requires same time which is ruled out above

      case QualifiedType::CONST_REF:
      case QualifiedType::TYPE:
        if (isSubtype(formalT, actualT))
          return convert(SUBTYPE);
        break;

      case QualifiedType::PARAM:
      case QualifiedType::IN:
      case QualifiedType::CONST_IN:
      case QualifiedType::INOUT:
        {
          auto got = canConvert(actualQT, formalQT);
          if (got.passes())
            return got;
          break;
        }

      case QualifiedType::OUT:
        return passAsIs();
    }
  }

  // can we promote?
  // TODO: implement promotion check

  return fail();
}


} // end namespace resolution
} // end namespace chpl
