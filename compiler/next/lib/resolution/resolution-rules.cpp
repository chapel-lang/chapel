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

#include "chpl/resolution/resolution-rules.h"

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

CanPassResult CanPassResult::canParamCoerce(const QualifiedType& actualQT,
                                            const QualifiedType& formalQT) {
  const Type* actualT = actualQT.type();
  const Type* formalT = formalQT.type();

  if (actualT->isBoolType() && formalT->isBoolType())
    return convert(PARAM);

  if (auto formalIntT = formalT->toIntType()) {
    if (actualT->isBoolType())
      return convert(PARAM);

    if (auto actualIntT = actualT->toIntType())
      if (actualIntT->bitwidth() < formalIntT->bitwidth())
        return convert(PARAM);

    if (auto actualUintT = actualT->toUintType())
      if (actualUintT->bitwidth() < formalIntT->bitwidth())
        return convert(PARAM);

    //
    // For smaller integer types, if the argument is a param, does it
    // store a value that's small enough that it could dispatch to
    // this argument?
    //
    if (formalIntT->bitwidth() < 64)
      if (auto actualP = actualQT.param())
        if (paramFitsInInt(formalIntT->bitwidth(), actualP))
          return convert(PARAM_NARROWING);
  }

  if (auto formalUintT = formalT->toUintType()) {
    if (actualT->isBoolType())
      return convert(PARAM);

    if (auto actualUintT = actualT->toUintType())
      if (actualUintT->bitwidth() < formalUintT->bitwidth())
        return convert(PARAM);

    if (auto actualP = actualQT.param())
      if (paramFitsInUint(formalUintT->bitwidth(), actualP))
        return convert(PARAM_NARROWING);
  }

  // param strings can coerce between string and c_string
  if (actualQT.hasParam())
    if ((formalT->isStringType() || formalT->isCStringType()) &&
        (actualT->isStringType() || actualT->isCStringType()))
      return convert(PARAM);

  // coerce fully representable integers into real / real part of complex
  if (auto formalRealT = formalT->toRealType()) {
    int mantissaW = 0;
    int exponentW = 0;
    getMantissaExponentWidth(formalRealT, mantissaW, exponentW);

    // don't coerce bools to reals (per spec: "unintended by programmer")

    // coerce any integer type to maximum width real
    if (actualT->isIntegralType() && formalRealT->bitwidth() == 64)
      return convert(PARAM);

    // coerce integer types that are exactly representable
    if (auto actualIntT = actualT->toIntType())
      if (actualIntT->bitwidth() < mantissaW)
        return convert(PARAM);

    if (auto actualUintT = actualT->toUintType())
      if (actualUintT->bitwidth() < mantissaW)
        return convert(PARAM);

    // coerce real from smaller size
    if (auto actualRealT = actualT->toRealType())
      if (actualRealT->bitwidth() < formalRealT->bitwidth())
        return convert(PARAM);

    // coerce literal/param ints that are exactly representable
    if (auto actualP = actualQT.param()) {
      if (actualT->isIntegralType())
        if (fitsInMantissa(mantissaW, actualP))
          return convert(PARAM_NARROWING);

      if (actualT->isRealType())
        if (fitsInMantissaExponent(mantissaW, exponentW, actualP, true))
          return convert(PARAM_NARROWING);
    }
  }

  if (auto formalImagT = formalT->toImagType()) {
    int mantissaW = 0;
    int exponentW = 0;
    getMantissaExponentWidth(formalImagT, mantissaW, exponentW);

    // coerce literal/param imag that are exactly representable
    if (auto actualP = actualQT.param())
      if (actualT->isImagType())
        if (fitsInMantissaExponent(mantissaW, exponentW, actualP, false))
          return convert(PARAM_NARROWING);
  }

  if (auto formalComplexT = formalT->toComplexType()) {
    int mantissaW = 0;
    int exponentW = 0;
    getMantissaExponentWidth(formalComplexT, mantissaW, exponentW);

    // don't coerce bools to complexes (per spec: "unintended by programmer")

    // coerce any integer type to maximum width complex
    if (actualT->isIntegralType() && formalComplexT->bitwidth() == 128)
      return convert(PARAM);

    // coerce integer types that are exactly representable
    if (auto actualIntT = actualT->toIntType())
      if (actualIntT->bitwidth() < mantissaW)
        return convert(PARAM);
    if (auto actualUintT = actualT->toUintType())
      if (actualUintT->bitwidth() < mantissaW)
        return convert(PARAM);

    // coerce real/imag from smaller size
    if (auto actualRealT = actualT->toRealType())
      if (actualRealT->bitwidth() <= formalComplexT->bitwidth()/2)
        return convert(PARAM);
    if (auto actualImagT = actualT->toImagType())
      if (actualImagT->bitwidth() <= formalComplexT->bitwidth()/2)
        return convert(PARAM);

    // coerce smaller complex types
    if (auto actualComplexT = actualT->toComplexType())
      if (actualComplexT->bitwidth() < formalComplexT->bitwidth())
        return convert(PARAM);

    // coerce literal/param complexes that are exactly representable
    if (auto actualP = actualQT.param()) {
      if (actualT->isIntegralType())
        if (fitsInMantissa(mantissaW, actualP))
          return convert(PARAM_NARROWING);
      if (actualT->isRealType())
        if (fitsInMantissaExponent(mantissaW, exponentW, actualP, true))
          return convert(PARAM_NARROWING);
      if (actualT->isImagType())
        if (fitsInMantissaExponent(mantissaW, exponentW, actualP, false))
          return convert(PARAM_NARROWING);
      if (actualT->isComplexType()) {
        if (fitsInMantissaExponent(mantissaW, exponentW, actualP, true) &&
            fitsInMantissaExponent(mantissaW, exponentW, actualP, false))
          return convert(PARAM_NARROWING);
      }
    }
  }

  return fail();
}

// The compiler considers many patterns of "subtyping" as things
// that require coercions (they often require coercions in the generated C).
// However not all coercions are created equal. Some of them are implementing
// subtyping.
// Here we consider a coercion to be implementing "subtyping" and return
// true for this call if, in an ideal implementation, the actual could
// be passed to a `const ref` argument of the formal type.
CanPassResult CanPassResult::canSubtypeCoerce(const QualifiedType& actualQT,
                                              const QualifiedType& formalQT) {
  const Type* actualT = actualQT.type();
  const Type* formalT = formalQT.type();

  if (actualT->isNilType() && formalT->isAnyPtrType())
    return convert(SUBTYPE);


  if (actualT->isClassType() || formalT->isClassType()) {
    // see canCoerceAsSubtype
    assert(false && "not implemented yet");
  }

  // TODO: implement c_ptr, c_array conversions

  return fail();
}

CanPassResult CanPassResult::canCoerce(const QualifiedType& actualQT,
                                       const QualifiedType& formalQT) {
  CanPassResult ret;

  // can we "param coerce" ?
  ret = canParamCoerce(actualQT, formalQT);
  if (ret.passes())
    return ret;

  // can we "subtype coerce" ?
  ret = canSubtypeCoerce(actualQT, formalQT);
  if (ret.passes())
    return ret;

  // can we coerce tuples?
  if (actualQT.type()->isTupleType() && formalQT.type()->isTupleType()) {
    assert(false && "not implemented yet");
    // see canCoerceTuples
  }

  // Check for other class subtyping
  // Class subtyping needs coercions in order to generate C code.

  // TODO: check for coercion to copy type
  // (relevant for array slices and iterator records)
  // see canCoerceToCopyType

  return fail();
}

CanPassResult CanPassResult::canPass(const QualifiedType& actualQT,
                                     const QualifiedType& formalQT) {

  assert(actualQT.type() && formalQT.type());

  // check params
  if (formalQT.hasParam()) {
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
    // otherwise the param passing is OK
  }

  if (actualQT.type() == formalQT.type()) {
    return passAsIs();
  }

  CanPassResult ret;

  // can we "param coerce" "subtype coerce" or other "coerce" ?
  ret = canCoerce(actualQT, formalQT);
  if (ret.passes())
    return ret;

  // can we promote?
  // TODO: implement promotion check
  return fail();
}


} // end namespace resolution
} // end namespace chpl
