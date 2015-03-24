/*
 * Copyright 2004-2015 Cray Inc.
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

/*
This module provides wrappers for <cmath> (math.h) numerical constants and
routines.  Its symbols are provided by default; an explicit 'use' statement
is not necessary.

The C Math library is part of the C Language Standard (ISO/IEC 9899), as
described in Section 7.12.  Please consult that standard for an
authoritative description of the expected properties of those constants and
routines.

In general, where the C math library provides a *double* and a *float* version
of a function, the float version has a suffix 'f'.  In the Chapel interface,
the suffix is dropped, and the type of the operand determines which version is
called -- according to the usual function overloading and resolution rules.
Normally, the result has the same precision
as the argument(s).  Please consult the C standard for specifics.

Rounding -- The rounding mode for floating-point addition (subtraction) is
implementation-defined.

Error Handling -- At present, Chapel does not provide control over error
handling in the Math module.  The default behavior is as if the macro
``math_errhandling`` is set to 0: Given erroneous input at run-time,
all math functions will return an implementation-defined value; no
exception will be generated.

*/
module Math {

  //////////////////////////////////////////////////////////////////////////
  // Helper constants and functions (not included in chpldocs).
  //
  pragma "no doc"
  extern proc chpl_macro_INFINITY():real(64);
  pragma "no doc"
  extern proc chpl_macro_NAN():real(64);

  pragma "no doc"
  extern proc chpl_macro_double_isinf(x: real(64)): c_int;
  pragma "no doc"
  extern proc chpl_macro_float_isinf(x: real(32)): c_int;
  pragma "no doc"
  extern proc chpl_macro_double_isfinite(x: real(64)): c_int;
  pragma "no doc"
  extern proc chpl_macro_float_isfinite(x: real(32)): c_int;
  pragma "no doc"
  extern proc chpl_macro_double_isnan(x: real(64)): c_int;
  pragma "no doc"
  extern proc chpl_macro_float_isnan(x: real(32)): c_int;

  pragma "no doc"
  proc _logBasePow2Help(in val, baseLog2) {
    var result = -1;
    while (val != 0) {
      val >>= baseLog2;
      result += 1;
    }
    return result;
  }

  pragma "no doc"
  proc logBasePow2(in val: int(?w), baseLog2) {
    if (val < 1) {
      halt("Can't take the log() of a non-positive integer");
    }
    return _logBasePow2Help(val, baseLog2);
  }

  pragma "no doc"
  proc logBasePow2(in val: uint(?w), baseLog2) {
    if (val < 1) {
      halt("Can't take the log() of a non-positive integer");
    }
    return _logBasePow2Help(val, baseLog2);
  }

  pragma "no doc"
  inline proc mod_help(m: integral, n: integral) {
    const temp = m % n;

    // eliminate some run-time tests if input(s) is(are) unsigned
    return
      if isNonnegative(n) then
        if isUintType(m.type)
        then temp
        else ( if temp >= 0 then temp else temp + n )
      else
        // n < 0
        ( if temp <= 0 then temp else temp + n );
  }

  pragma "no doc"
  inline proc mod_help(x: real(?w), y: real(w)): real(w) {
    // This codes up the standard definition, according to Wikipedia.
    // Is there a more efficient implementation for reals?
    return x - y*floor(x/y);
  }

  pragma "no doc"
  inline proc sgn_help(i : int(?w)): int(8)
    return ((i > 0) : int(8) - (i < 0) : int(8)) : int(8);

  pragma "no doc"
  inline proc sgn_help(i : uint(?w)): uint(8)
    return (i > 0) : uint(8);

  pragma "no doc"
  inline proc sgn_help(x : real(?w)): int(8)
    return ((x > 0.0) : int(8) - (x < 0.0) : int(8)) : int(8);
  //
  //////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////
  //
  // Public interface (included in chpldocs).
  //
  // (The entries below are alphabetized (case-insensitively) because chpldocs
  // presents them in declaration order.)
  //


  /* Returns the absolute value of the integer param argument `i`. */
  proc abs(param i : integral) param return if i < 0 then -i else i;

  /* Returns the magnitude (absolute value) of the argument `x`.

     :rtype: The type of `x`.
  */
  inline proc abs(x) where isNumericalType(x.type) {
    if isComplex(x) {
      return sqrt(x.re * x.re + x.im * -x.im);
    } else if isReal(x) {
      if numBits(x) == 64 {
        extern proc fabs(x: real(64)): real(64);
        return fabs(x);
      } else if numBits(x) == 32 {
        extern proc fabsf(x: real(32)): real(32);
        return fabsf(x);
      } else {
        compilerError("abs() -- Invalid real argument width: ", numBits(x));
      }
    } else if isImaginary(x) {
      if numBits(x) == 64 {
        extern proc fabs(x: real(64)): real(64);
        return fabs(_i2r(x));
      } else if numBits(x) == 32 {
        extern proc fabsf(x: real(32)): real(32);
        return fabsf(_i2r(x));
      } else {
        compilerError("abs() -- Invalid imaginary argument width ", numBits(x));
      }
    } else if isIntegral(x) {
      if (isSigned(x)) then
        return if i < 0 then -i else i;
      else
        return if x == 0 then 0 else i;
    } else{
      compilerError("abs() -- Invalid argument type: ", typeToString(x.type));
    }
  }


  /* Returns the arc cosine of the real argument `x`.

     It is an error if `x` is less than -1 or greater than 1.
  */
  inline proc acos(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc acos(x: real(64)): real(64);
      return acos(x);
    } else if w == 32 {
      extern proc acosf(x: real(32)): real(32);
      return acosf(x);
    } else {
      compilerError("acos() -- Invalid argument width ", w);
    }
  }


  /* Returns the inverse hyperbolic cosine of the argument `x`.

     It is an error if `x` is less than 1.
  */
  inline proc acosh(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc acosh(x: real(64)): real(64);
      return acosh(x);
    } else if w == 32 {
      extern proc acoshf(x: real(32)): real(32);
      return acoshf(x);
    } else {
      compilerError("acosh() -- Invalid argument width ", w);
    }
  }


  /* Returns the arc sine of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1.
  */
  inline proc asin(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc asin(x: real(64)): real(64);
      return asin(x);
    } else if w == 32 {
      extern proc asinf(x: real(32)): real(32);
      return asinf(x);
    } else {
      compilerError("asin() -- Invalid argument width ", w);
    }
  }


  /* Returns the inverse hyperbolic sine of the argument `x`. */
  inline proc asinh(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc asinh(x: real(64)): real(64);
      return asinh(x);
    } else if w == 32 {
      extern proc asinyf(x: real(32)): real(32);
      return asinhf(x);
    } else {
      compilerError("asinh() -- Invalid argument width ", w);
    }
  }


  /* Returns the arc tangent of the argument `x`. */
  inline proc atan(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc atan(x: real(64)): real(64);
      return atan(x);
    } else if w == 32 {
      extern proc atanf(x: real(32)): real(32);
      return atanf(x);
    } else {
      compilerError("atan() -- Invalid argument width ", w);
    }
  }


  /* Returns the arc tangent of the ratio of the two arguments.

     This is equivalent to
     the arc tangent of `y` / `x` except that the signs of `y`
     and `x` are used to determine the quadrant of the result.
  */
  inline proc atan2(y: real(?w), x: real(w)) : real(w) {
    if w == 64 {
      extern proc atan2(y: real(64), x: real(64)): real(64);
      return atan2(y, x);
    } else if w == 32 {
      extern proc atan2f(y: real(32), x: real(32)): real(32);
      return atan2f(y, x);
    } else {
      compilerError("atan2() -- Invalid argument width ", w);
    }
  }


  /* Returns the inverse hyperbolic tangent of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1.
  */
  inline proc atanh(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc atanh(x: real(64)): real(64);
      return atanh(x);
    } else if w == 32 {
      extern proc atanhf(x: real(32)): real(32);
      return atanhf(x);
    } else {
      compilerError("atanh() -- Invalid argument width ", w);
    }
  }


  /* Returns the cube root of the argument `x`. */
  inline proc cbrt(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc cbrt(x: real(64)): real(64);
      return cbrt(x);
    } else if w == 32 {
      extern proc cbrtf(x: real(32)): real(32);
      return cbrtf(x);
    } else {
      compilerError("cbrt() -- Invalid argument width ", w);
    }
  }


  /* Returns the value of the argument `x` rounded up to the nearest integer. */
  inline proc ceil(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc ceil(x: real(64)): real(64);
      return ceil(x);
    } else if w == 32 {
      extern proc ceilf(x: real(32)): real(32);
      return ceilf(x);
    } else {
      compilerError("ceil() -- Invalid argument width ", w);
    }
  }


  /* Returns the complex conjugate of the argument `x`. */
  inline proc conjg(x: complex(?w)) : complex(w) return (x.re, -x.im):complex(w);


  /* Returns the cosine of the argument `x`. */
  inline proc cos(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc cos(x: real(64)): real(64);
      return cos(x);
    } else if w == 32 {
      extern proc cosf(x: real(32)): real(32);
      return cosf(x);
    } else {
      compilerError("cos() -- Invalid argument width ", w);
    }
  }


  /* Returns the hyperbolic cosine of the argument `x`. */
  inline proc cosh(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc cosh(x: real(64)): real(64);
      return cosh(x);
    } else if w == 32 {
      extern proc coshf(x: real(32)): real(32);
      return coshf(x);
    } else {
      compilerError("cosh() -- Invalid argument width ", w);
    }
  }


  /* Returns :proc:`ceil`\(`m`/`n`),
     i.e., the fraction `m`/`n` rounded up to the nearest integer.

     If the arguments are of unsigned type, then
     fewer condititionals will be evaluated at run time.
  */
  proc divceil(param m: integral, param n: integral)
    param
    return
    if isNonnegative(m) then
      if isNonnegative(n) then (m + n - 1) / n
      else                     m / n
    else
      if isNonnegative(n) then m / n
      else                     (m + n + 1) / n;



  /* Returns :proc:`ceil`\(`m`/`n`),
     i.e., the fraction `m`/`n` rounded up to the nearest integer.

     If the arguments are of unsigned type, then
     fewer condititionals will be evaluated at run time.
  */
  proc divceil(m: integral, n: integral) return
    if isNonnegative(m) then
      if isNonnegative(n) then (m + n - 1) / n
      else                     m / n
    else
      if isNonnegative(n) then m / n
      else                     (m + n + 1) / n;


  /* Returns :proc:`floor`\(`m`/`n`),
     i.e., the fraction `m`/`n` rounded down to the nearest integer.

     If the arguments are of unsigned type, then
     fewer condititionals will be evaluated at run time.
  */
  proc divfloor(param m: integral, param n: integral) param return
    if isNonnegative(m) then
      if isNonnegative(n) then m / n
      else                     (m - n - 1) / n
    else
      if isNonnegative(n) then (m - n + 1) / n
      else                     m / n;


  /* Returns :proc:`floor`\(`m`/`n`),
     i.e., the fraction `m`/`n` rounded down to the nearest integer.

     If the arguments are of unsigned type, then
     fewer condititionals will be evaluated at run time.
  */
  proc divfloor(m: integral, n: integral) return
    if isNonnegative(m) then
      if isNonnegative(n) then m / n
      else                     (m - n - 1) / n
    else
      if isNonnegative(n) then (m - n + 1) / n
      else                     m / n;


  /* Returns the error function of the argument `x`. */
  inline proc erf(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc erf(x: real(64)): real(64);
      return erf(x);
    } else if w == 32 {
      extern proc erff(x: real(32)): real(32);
      return erff(x);
    } else {
      compilerError("erf() -- Invalid argument width ", w);
    }
  }


  /* Returns the complementary error function of the argument.
     This is equivalent to 1.0 - :proc:`erf`\(`x`).
  */
  inline proc erfc(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc erfc(x: real(64)): real(64);
      return erfc(x);
    } else if w == 32 {
      extern proc erfcf(x: real(32)): real(32);
      return erfcf(x);
    } else {
      compilerError("erfc() -- Invalid argument width ", w);
    }
  }


  /* Returns the value of the Napierien `e` raised to the power of the argument `x`. */
  inline proc exp(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc exp(x: real(64)): real(64);
      return exp(x);
    } else if w == 32 {
      extern proc expf(x: real(32)): real(32);
      return expf(x);
    } else {
      compilerError("exp() -- Invalid argument width ", w);
    }
  }


  /* Returns the value of `2` raised to the power of the argument `x`. */
  inline proc exp2(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc exp2(x: real(64)): real(64);
      return exp2(x);
    } else if w == 32 {
      extern proc exp2f(x: real(32)): real(32);
      return exp2f(x);
    } else {
      compilerError("exp2() -- Invalid argument width ", w);
    }
  }


  /* Returns one less than the value of the Napierien `e` raised to the power
     of the argument `x`.
  */
  inline proc expm1(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc expm1(x: real(64)): real(64);
      return expm1(x);
    } else if w == 32 {
      extern proc expm1f(x: real(32)): real(32);
      return expm1f(x);
    } else {
      compilerError("expm1() -- Invalid argument width ", w);
    }
  }


  /* Returns the value of the argument `x` rounded down to the nearest integer. */
  inline proc floor(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc floor(x: real(64)): real(64);
      return floor(x);
    } else if w == 32 {
      extern proc floorf(x: real(32)): real(32);
      return floorf(x);
    } else {
      compilerError("floor() -- Invalid argument width ", w);
    }
  }


  /* Returns a value for which :proc:`isinf` will return `true`. */
  inline proc INFINITY: real(64) return chpl_macro_INFINITY();


  /* Returns `true` if the argument `x` is a representation of a finite value;
     `false` otherwise.
  */
  inline proc isfinite(x: real(?w)) : real(w) {
    if w == 64 {
      return chpl_macro_double_isfinite(x);
    } else if w == 32 {
      return chpl_macro_float_isfinite(x);
    } else {
      compilerError("isfinite() -- Invalid argument width ", w);
    }
  }


  /* Returns `true` if the argument `x` is a representation of *infinity*;
     `false` otherwise. */
  inline proc isinf(x: real(?w)) : real(w) {
    if w == 64 {
      return chpl_macro_double_isinf(x);
    } else if w == 32 {
      return chpl_macro_float_isinf(x);
    } else {
      compilerError("isinf() -- Invalid argument width ", w);
    }
  }


  /* Returns `true` if the argument `x` does not represent a valid number;
     `false` otherwise. */
  inline proc isnan(x: real(?w)) : real(w) {
    if w == 64 {
      return chpl_macro_double_isnan(x);
    } else if w == 32 {
      return chpl_macro_float_isnan(x);
    } else {
      compilerError("isnan() -- Invalid argument width ", w);
    }
  }


  /* Returns the natural logarithm of the absolute value
     of the gamma function of the argument `x`.
  */
  inline proc lgamma(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc lgamma(x: real(64)): real(64);
      return lgamma(x);
    } else if w == 32 {
      extern proc lgammaf(x: real(32)): real(32);
      return lgammaf(x);
    } else {
      compilerError("lgamma() -- Invalid argument width ", w);
    }
  }


  /* Returns the natural logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc log(x: real(64)): real(64);
      return log(x);
    } else if w == 32 {
      extern proc logf(x: real(32)): real(32);
      return logf(x);
    } else {
      compilerError("log() -- Invalid argument width ", w);
    }
  }


  /* Returns the base 10 logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log10(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc log10(x: real(64)): real(64);
      return log10(x);
    } else if w == 32 {
      extern proc log10f(x: real(32)): real(32);
      return log10f(x);
    } else {
      compilerError("log10() -- Invalid argument width ", w);
    }
  }


  /* Returns the natural logarithm of `x` + 1.

     It is an error if `x` is less than or equal to -1.
  */
  inline proc log1p(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc log1p(x: real(64)): real(64);
      return log1p(x);
    } else if w == 32 {
      extern proc log1pf(x: real(32)): real(32);
      return log1pf(x);
    } else {
      compilerError("log1p() -- Invalid argument width ", w);
    }
  }


  /* Returns the base 2 logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log2(x) : x.type where isNumeric(x) {
    if isReal(x) {
      if w == 64 {
        extern proc log2(x: real(64)): real(64);
        return log2(x);
      } else if w == 32 {
        extern proc log2f(x: real(32)): real(32);
        return log2f(x);
      } else {
        compilerError("log2() -- Invalid real argument width ", w);
      }
    } else if isIntegral(x) {
      return logBasePow2(val, 1);
    } else {
      compilerError("log2() -- Invalid numeric argument type: ",
                    typeToString(x.type));
    }
  }


  /* Computes the mod operator on the two integer param arguments, defined as
     ``mod(x,y) = x - y * floor(x / y)``.
  */
  proc mod(param m: integral, param n: integral) param {
    param temp = m % n;

    // verbatim copy from the other 'mod', to simplify maintenance
    return
      if isNonnegative(n) then
        if isUintType(m.type)
        then temp
        else ( if temp >= 0 then temp else temp + n )
      else
        // n < 0
        ( if temp <= 0 then temp else temp + n );
  }

  /* Computes the mod operator on the two arguments, defined as
     ``mod(x,y) = x - y * floor(x / y)``.

     If the arguments are of unsigned type, then
     fewer condititionals will be evaluated at run time.
  */
  inline proc mod(m:numeric, n:numeric) {
    return mod_help(m, n);
  }

  /* Returns a value for which :proc:`isnan` will return `true`. */
  inline proc NAN : real(64) return chpl_macro_NAN();


  /* Returns the rounded integral value of the argument `x` determined by the
     current rounding direction.  :proc:`nearbyint` will not raise the "inexact"
     floating-point exception.
  */
  inline proc nearbyint(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc nearbyint(x: real(64)): real(64);
      return nearbyint(x);
    } else if w == 32 {
      extern proc nearbyintf(x: real(32)): real(32);
      return nearbyintf(x);
    } else {
      compilerError("nearbyint() -- Invalid argument width ", w);
    }
  }


  /* Returns the rounded integral value of the argument `x` determined by the
     current rounding direction.  :proc:`rint` may raise the "inexact" floating-point
     exception.
  */
  inline proc rint(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc rint(x: real(64)): real(64);
      return rint(x);
    } else if w == 32 {
      extern proc rintf(x: real(32)): real(32);
      return rintf(x);
    } else {
      compilerError("rint() -- Invalid argument width ", w);
    }
  }


  /* Returns the rounded integral value of the argument `x`. */
  inline proc round(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc round(x: real(64)): real(64);
      return round(x);
    } else if w == 32 {
      extern proc roundf(x: real(32)): real(32);
      return roundf(x);
    } else {
      compilerError("round() -- Invalid argument width ", w);
    }
  }


  /* Returns the signum function of the integer param argument `i`:
     1 if positive, -1 if negative, 0 if zero.
  */
  proc sgn(param i : integral) param
    return if i > 0 then 1 else if i == 0 then 0 else -1;

  /* Returns the signum function of the argument `x`:
     1 if positive, -1 if negative, 0 if zero.
  */
  inline proc sgn(x: numeric) return sgn_help(x);


  /* Returns the sine of the argument `x`. */
  inline proc sin(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc sin(x: real(64)): real(64);
      return sin(x);
    } else if w == 32 {
      extern proc sinf(x: real(32)): real(32);
      return sinf(x);
    } else {
      compilerError("sin() -- Invalid argument width ", w);
    }
  }


  /* Returns the hyperbolic sine of the argument `x`. */
  inline proc sinh(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc sinh(x: real(64)): real(64);
      return sinh(x);
    } else if w == 32 {
      extern proc sinhf(x: real(32)): real(32);
      return sinhf(x);
    } else {
      compilerError("sinh() -- Invalid argument width ", w);
    }
  }


  /* Returns the square root of the argument `x`.

     It is an error if the `x` is less than zero.
  */
  inline proc sqrt(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc sqrt(x: real(64)): real(64);
      return sqrt(x);
    } else if w == 32 {
      extern proc sqrtf(x: real(32)): real(32);
      return sqrtf(x);
    } else {
      compilerError("sqrt() -- Invalid argument width ", w);
    }
  }


  /* Returns the tangent of the argument `x`. */
  inline proc tan(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc tan(x: real(64)): real(64);
      return tan(x);
    } else if w == 32 {
      extern proc tanf(x: real(32)): real(32);
      return tanf(x);
    } else {
      compilerError("tan() -- Invalid argument width ", w);
    }
  }


  /* Returns the hyperbolic tangent of the argument `x`. */
  inline proc tanh(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc tanh(x: real(64)): real(64);
      return tanh(x);
    } else if w == 32 {
      extern proc tanhf(x: real(32)): real(32);
      return tanhf(x);
    } else {
      compilerError("tanh() -- Invalid argument width ", w);
    }
  }


  /* Returns the absolute value of the gamma function of the argument `x`. */
  inline proc tgamma(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc tgamma(x: real(64)): real(64);
      return tgamma(x);
    } else if w == 32 {
      extern proc tgammaf(x: real(32)): real(32);
      return tgammaf(x);
    } else {
      compilerError("tgamma() -- Invalid argument width ", w);
    }
  }


  /* Returns the nearest integral value to the argument `x` that is not larger
     than `x` in absolute value. */
  inline proc trunc(x: real(?w)) : real(w) {
    if w == 64 {
      extern proc trunc(x: real(64)): real(64);
      return trunc(x);
    } else if w == 32 {
      extern proc truncf(x: real(32)): real(32);
      return truncf(x);
    } else {
      compilerError("trunc() -- Invalid argument width ", w);
    }
  }


} // end of module Math
