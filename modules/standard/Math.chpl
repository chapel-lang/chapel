/*
 * Copyright 2004-2020 Cray Inc.
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
This module provides mathematical constants and functions.

.. note:: All Chapel programs automatically ``use`` this module by default.
          An explicit ``use`` statement is not necessary.

It includes wrappers for many of the constants in functions in
the C Math library, which is part of the C Language Standard (ISO/IEC 9899)
as described in Section 7.12.  Please consult that standard for an
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
  private use HaltWrappers only;
  private use SysCTypes;

  //////////////////////////////////////////////////////////////////////////
  // Constants (included in chpldocs)
  //

  /* e - exp(1) or  the base of the natural logarithm */
  param e = 2.7182818284590452354;
  /* log2(e) */
  param log2_e = 1.4426950408889634074;
  /* log10(e) */
  param log10_e = 0.43429448190325182765;
  /* log(2) (natural logarithm) */
  param ln_2 = 0.69314718055994530942;
  /* log(10) (natural logarithm) */
  param ln_10 = 2.30258509299404568402;
  /* pi - the circumference/the diameter of a circle */
  param pi = 3.14159265358979323846;
  /* pi/2 */
  param half_pi = 1.57079632679489661923;
  /* pi/4 */
  param quarter_pi = 0.78539816339744830962;
  /* 1/pi */
  param recipr_pi = 0.31830988618379067154;
  /* 2/pi */
  param twice_recipr_pi = 0.63661977236758134308;
  /* 2/sqrt(pi) */
  param twice_recipr_sqrt_pi = 1.12837916709551257390;
  /* sqrt(2) */
  param sqrt_2 = 1.41421356237309504880;
  /* 1/sqrt(2) */
  param recipr_sqrt_2 = 0.70710678118654752440;

  //////////////////////////////////////////////////////////////////////////
  // Helper constants and functions (not included in chpldocs).
  //
  pragma "fn synchronization free"
  private extern proc chpl_macro_double_isinf(x: real(64)): c_int;
  pragma "fn synchronization free"
  private extern proc chpl_macro_float_isinf(x: real(32)): c_int;
  pragma "fn synchronization free"
  private extern proc chpl_macro_double_isfinite(x: real(64)): c_int;
  pragma "fn synchronization free"
  private extern proc chpl_macro_float_isfinite(x: real(32)): c_int;
  pragma "fn synchronization free"
  private extern proc chpl_macro_double_isnan(x: real(64)): c_int;
  pragma "fn synchronization free"
  private extern proc chpl_macro_float_isnan(x: real(32)): c_int;

  pragma "fn synchronization free"
  private extern proc fabs(x: real(64)): real(64);

  private inline proc _logBasePow2Help(in val, baseLog2) {
    // These are used here to avoid including BitOps by default.
    extern proc chpl_bitops_clz_32(x: c_uint) : uint(32);
    extern proc chpl_bitops_clz_64(x: c_ulonglong) : uint(64);

    var lg2 = 0;

    if numBits(val.type) <= 32 {
      var tmp:uint(32) = val:uint(32);
      lg2 = 32 - 1 - chpl_bitops_clz_32(tmp):int;
    } else if numBits(val.type) == 64 {
      var tmp:uint(64) = val:uint(64);
      lg2 = 64 - 1 - chpl_bitops_clz_64(tmp):int;
    } else {
      compilerError("Integer width not handled in logBasePow2");
    }

    return lg2 / baseLog2;
  }

  //
  //////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////
  //
  // Public interface (included in chpldocs).
  //
  // (The entries below are alphabetized (case-insensitively) because chpldocs
  // presents them in declaration order.)
  //


  /* Returns the absolute value of the integer argument.

     :rtype: The type of `i`.
  */
  inline proc abs(i : int(?w)) return if i < 0 then -i else i;

  /* Returns the absolute value of the unsigned integer argument.

     :rtype: The type of `i`.
  */
  inline proc abs(i : uint(?w)) return i;

  /* Returns the absolute value of the integer param argument `i`. */
  proc abs(param i : integral) param return if i < 0 then -i else i;

  /* Returns the magnitude of the real argument `r`. */
  inline proc abs(r : real(64)):real(64) return fabs(r);

  /* Returns the magnitude of the real argument `x`. */
  inline proc abs(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc fabsf(x: real(32)): real(32);
    return fabsf(x);
  }

  /* Returns the real magnitude of the imaginary argument `im`. */
  inline proc abs(im : imag(64)): real(64) return fabs(_i2r(im));

  /* Returns the real magnitude of the imaginary argument `im`. */
  inline proc abs(im: imag(32)): real(32) {
    pragma "fn synchronization free"
    extern proc fabsf(x: real(32)): real(32);
    return fabsf(_i2r(im));
  }

  /* Returns the real magnitude of the complex argument `z`.

     :rtype: The type of the real component of the argument (== `w`/2).
  */
  inline proc abs(z : complex(?w)): real(w/2) {
    pragma "fn synchronization free"
    extern proc cabsf(z: complex(64)): real(32);
    pragma "fn synchronization free"
    extern proc cabs(z: complex(128)): real(64);
    if w == 64 then
      return cabsf(z);
    else
      return cabs(z);
  }


  /* Returns the real phase angle of complex argument `z`. */
  inline proc carg(z: complex(?w)): real(w/2) {
    pragma "fn synchronization free"
    extern proc cargf(z: complex(64)): real(32);
    pragma "fn synchronization free"
    extern proc carg(z: complex(128)): real(64);
    if w == 64 then
      return cargf(z);
    else
      return carg(z);
  }


  /* Returns the arc cosine of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1.
  */
  pragma "fn synchronization free"
  extern proc acos(x: real(64)): real(64);

  /* Returns the arc cosine of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1.
  */
  inline proc acos(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc acosf(x: real(32)): real(32);
    return acosf(x);
  }

  /* Returns the arc cosine of the argument `z`. */
  inline proc acos(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    extern proc cacosf(z: complex(64)): complex(64);
    return cacosf(z);
  }

  /* Returns the arc cosine of the argument `z`. */
  inline proc acos(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    extern proc cacos(z: complex(128)): complex(128);
    return cacos(z);
  }


  /* Returns the inverse hyperbolic cosine of the argument `x`.

     It is an error if `x` is less than 1.
  */
  pragma "fn synchronization free"
  extern proc acosh(x: real(64)): real(64);

  /* Returns the inverse hyperbolic cosine of the argument `x`.

     It is an error if `x` is less than 1.
  */
  inline proc acosh(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc acoshf(x: real(32)): real(32);
    return acoshf(x);
  }

  /* Returns the inverse hyperbolic cosine of the argument `z`. */
  inline proc acosh(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    extern proc cacoshf(z: complex(64)): complex(64);
    return cacoshf(z);
  }

  /* Returns the inverse hyperbolic cosine of the argument `z`. */
  inline proc acosh(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    extern proc cacosh(z: complex(128)): complex(128);
    return cacosh(z);
  }


  /* Returns the arc sine of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1.
  */
  pragma "fn synchronization free"
  extern proc asin(x: real(64)): real(64);

  /* Returns the arc sine of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1.
  */
  inline proc asin(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc asinf(x: real(32)): real(32);
    return asinf(x);
  }

  /* Returns the arc sine of the argument `z`. */
  inline proc asin(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    extern proc casinf(z: complex(64)): complex(64);
    return casinf(z);
  }

  /* Returns the arc sine of the argument `z`. */
  inline proc asin(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    extern proc casin(z: complex(128)): complex(128);
    return casin(z);
  }


  /* Returns the inverse hyperbolic sine of the argument `x`. */
  pragma "fn synchronization free"
  extern proc asinh(x: real(64)): real(64);

  /* Returns the inverse hyperbolic sine of the argument `x`. */
  inline proc asinh(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc asinhf(x: real(32)): real(32);
    return asinhf(x);
  }

  /* Returns the inverse hyperbolic sine of the argument `z`. */
  inline proc asinh(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    extern proc casinhf(z: complex(64)): complex(64);
    return casinhf(z);
  }

  /* Returns the inverse hyperbolic sine of the argument `z`. */
  inline proc asinh(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    extern proc casinh(z: complex(128)): complex(128);
    return casinh(z);
  }



  /* Returns the arc tangent of the argument `x`. */
  pragma "fn synchronization free"
  extern proc atan(x: real(64)): real(64);

  /* Returns the arc tangent of the argument `x`. */
  inline proc atan(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc atanf(x: real(32)): real(32);
    return atanf(x);
  }

  /* Returns the arc tangent of the argument `z`. */
  inline proc atan(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    extern proc catanf(z: complex(64)): complex(64);
    return catanf(z);
  }

  /* Returns the arc tangent of the argument `z`. */
  inline proc atan(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    extern proc catan(z: complex(128)): complex(128);
    return catan(z);
  }


  /* Returns the arc tangent of the ratio of the two arguments.

     This is equivalent to
     the arc tangent of `y` / `x` except that the signs of `y`
     and `x` are used to determine the quadrant of the result. */
  pragma "fn synchronization free"
  extern proc atan2(y: real(64), x: real(64)): real(64);

  /* Returns the arc tangent of the two arguments.

     This is equivalent to
     the arc tangent of `y` / `x` except that the signs of `y`
     and `x` are used to determine the quadrant of the result. */
  inline proc atan2(y : real(32), x: real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc atan2f(y: real(32), x: real(32)): real(32);
    return atan2f(y, x);
  }


  /* Returns the inverse hyperbolic tangent of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1. */
  pragma "fn synchronization free"
  extern proc atanh(x: real(64)): real(64);

  /* Returns the inverse hyperbolic tangent of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1. */
  inline proc atanh(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc atanhf(x: real(32)): real(32);
    return atanhf(x);
  }

  /* Returns the inverse hyperbolic tangent of the argument `z`. */
  inline proc atanh(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    extern proc catanhf(z: complex(64)): complex(64);
    return catanhf(z);
  }

  /* Returns the inverse hyperbolic tangent of the argument `z`. */
  inline proc atanh(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    extern proc catanh(z: complex(128)): complex(128);
    return catanh(z);
  }


  /* Returns the cube root of the argument `x`. */
  pragma "fn synchronization free"
  extern proc cbrt(x: real(64)): real(64);

  /* Returns the cube root of the argument `x`. */
  inline proc cbrt(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc cbrtf(x: real(32)): real(32);
    return cbrtf(x);
  }


  /* Returns the value of the argument `x` rounded up to the nearest integer. */
  pragma "fn synchronization free"
  extern proc ceil(x: real(64)): real(64);

  /* Returns the value of the argument `x` rounded up to the nearest integer. */
  inline proc ceil(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc ceilf(x: real(32)): real(32);
    return ceilf(x);
  }


  /* Returns the complex conjugate of the complex argument `z`.

     :rtype: A complex number of the same type as `z`.
  */
  inline proc conjg(z: complex(?w)) {
    pragma "fn synchronization free"
    extern proc conjf(z: complex(64)): complex(64);
    pragma "fn synchronization free"
    extern proc conj(z: complex(128)): complex(128);
    if w == 64 then
      return conjf(z);
    else
      return conj(z);
  }

  /* Returns the complex conjugate of the imaginary argument `z`.

     :rtype: An imaginary number of the same type as `z`.
  */
  inline proc conjg(z: imag(?w)) {
    return -z;
  }

  /* Returns the argument `z`.

     :rtype: A number that is not complex or imaginary of the same type as `z`.
  */
  inline proc conjg(z: int(?w)) {
    return z;
  }

  inline proc conjg(z: uint(?w)) {
    return z;
  }

  inline proc conjg(z: real(?w)) {
    return z;
  }

  /* Returns the projection of `z` on a Riemann sphere. */
  inline proc cproj(z: complex(?w)): real(w/2) {
    pragma "fn synchronization free"
    extern proc cprojf(z: complex(64)): real(32);
    pragma "fn synchronization free"
    extern proc cproj(z: complex(128)): real(64);
    if w == 64 then
      return cprojf(z);
    else
      return cproj(z);
  }


  /* Returns the cosine of the argument `x`. */
  pragma "fn synchronization free"
  extern proc cos(x: real(64)): real(64);

  /* Returns the cosine of the argument `x`. */
  inline proc cos(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc cosf(x: real(32)): real(32);
    return cosf(x);
  }

  /* Returns the cosine of the argument `z`. */
  inline proc cos(z : complex(64)): complex(64) {
    pragma "fn synchronization free"
    extern proc ccosf(z: complex(64)): complex(64);
    return ccosf(z);
  }

  /* Returns the cosine of the argument `z`. */
  inline proc cos(z : complex(128)): complex(128) {
    pragma "fn synchronization free"
    extern proc ccos(z: complex(128)): complex(128);
    return ccos(z);
  }


  /* Returns the hyperbolic cosine of the argument `x`. */
  pragma "fn synchronization free"
  extern proc cosh(x: real(64)): real(64);

  /* Returns the hyperbolic cosine of the argument `x`. */
  inline proc cosh(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc coshf(x: real(32)): real(32);
    return coshf(x);
  }

  /* Returns the hyperbolic cosine of the argument `z`. */
  inline proc cosh(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    extern proc ccoshf(z: complex(64)): complex(64);
    return ccoshf(z);
  }

  /* Returns the hyperbolic cosine of the argument `z`. */
  inline proc cosh(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    extern proc ccosh(z: complex(128)): complex(128);
    return ccosh(z);
  }


  /* Returns :proc:`ceil`\(`m`/`n`),
     i.e., the fraction `m`/`n` rounded up to the nearest integer.

     If the arguments are of unsigned type, then
     fewer conditionals will be evaluated at run time.
  */
  proc divceil(param m: integral, param n: integral) param return
    if isNonnegative(m) then
      if isNonnegative(n) then (m + n - 1) / n
      else                     m / n
    else
      if isNonnegative(n) then m / n
      else                     (m + n + 1) / n;

  /* Returns :proc:`ceil`\(`m`/`n`),
     i.e., the fraction `m`/`n` rounded up to the nearest integer.

     If the arguments are of unsigned type, then
     fewer conditionals will be evaluated at run time.
  */
  proc divceil(m: integral, n: integral) return
    if isNonnegative(m) then
      if isNonnegative(n) then (m + n - 1) / n
      else                     m / n
    else
      if isNonnegative(n) then m / n
      else                     (m + n + 1) / n;

  /*
    A variant of :proc:`divceil` that performs no runtime checks.
    The user must ensure that both arguments are strictly positive
    (not 0) and are of a signed integer type (not `uint`).
  */
  proc divceilpos(m: integral, n: integral) {
    if !isIntType(m.type) || !isIntType(n.type) then
      compilerError("divceilpos() accepts only arguments of signed integer types");
    return (m - 1) / n + 1;
  }


  /* Returns :proc:`floor`\(`m`/`n`),
     i.e., the fraction `m`/`n` rounded down to the nearest integer.

     If the arguments are of unsigned type, then
     fewer conditionals will be evaluated at run time.
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
     fewer conditionals will be evaluated at run time.
  */
  proc divfloor(m: integral, n: integral) return
    if isNonnegative(m) then
      if isNonnegative(n) then m / n
      else                     (m - n - 1) / n
    else
      if isNonnegative(n) then (m - n + 1) / n
      else                     m / n;

  /*
    A variant of :proc:`divfloor` that performs no runtime checks.
    The user must ensure that both arguments are strictly positive
    (not 0) and are of a signed integer type (not `uint`).
  */
  proc divfloorpos(m: integral, n: integral) {
    if !isIntType(m.type) || !isIntType(n.type) then
      compilerError("divfloorpos() accepts only arguments of signed integer types");
    return m / n;
  }


  /* Returns the error function of the argument `x`. */
  pragma "fn synchronization free"
  extern proc erf(x: real(64)): real(64);

  /* Returns the error function of the argument `x`. */
  inline proc erf(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc erff(x: real(32)): real(32);
    return erff(x);
  }


  /* Returns the complementary error function of the argument.
     This is equivalent to 1.0 - :proc:`erf`\(`x`).
  */
  pragma "fn synchronization free"
  extern proc erfc(x: real(64)): real(64);

  /* Returns the complementary error function of the argument.
     This is equivalent to 1.0 - :proc:`erf`\(`x`).
  */
  inline proc erfc(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc erfcf(x: real(32)): real(32);
    return erfcf(x);
  }


  /* Returns the value of the Napierian `e` raised to the power of the argument `x`. */
  pragma "fn synchronization free"
  extern proc exp(x: real(64)): real(64);

  /* Returns the value of the Napierian `e` raised to the power of the argument. */
  inline proc exp(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc expf(x: real(32)): real(32);
    return expf(x);
  }

  /* Returns the value of the Napierian `e` raised to the power of the argument. */
  inline proc exp(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    extern proc cexpf(z: complex(64)): complex(64);
    return cexpf(z);
  }

  /* Returns the value of the Napierian `e` raised to the power of the argument. */
  inline proc exp(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    extern proc cexp(z: complex(128)): complex(128);
    return cexp(z);
  }


  /* Returns the value of `2` raised to the power of the argument `x`. */
  pragma "fn synchronization free"
  extern proc exp2(x: real(64)): real(64);

  /* Returns the value of `2` raised to the power of the argument `x`. */
  inline proc exp2(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc exp2f(x: real(32)): real(32);
    return exp2f(x);
  }


  /* Returns one less than the value of the Napierian `e` raised to the power
     of the argument `x`. */
  pragma "fn synchronization free"
  extern proc expm1(x: real(64)): real(64);

  /* Returns one less than the value of the Napierian `e` raised to the power
     of the argument `x`. */
  inline proc expm1(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc expm1f(x: real(32)): real(32);
    return expm1f(x);
  }


  /* Returns the value of the argument `x` rounded down to the nearest integer. */
  pragma "fn synchronization free"
  extern proc floor(x: real(64)): real(64);

  /* Returns the value of the argument `x` rounded down to the nearest integer. */
  inline proc floor(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc floorf(x: real(32)): real(32);
    return floorf(x);
  }


  /* Returns a value for which :proc:`isinf` will return `true`. */
  inline proc INFINITY param : real(64) return chpl_INFINITY;


  /* Returns `true` if the argument `x` is a representation of a finite value;
     `false` otherwise. */
  inline proc isfinite(x: real(64)): bool return chpl_macro_double_isfinite(x):bool;

  /* Returns `true` if the argument `x` is a representation of a finite value;
     `false` otherwise. */
  inline proc isfinite(x: real(32)): bool return chpl_macro_float_isfinite(x):bool;


  /* Returns `true` if the argument `x` is a representation of *infinity*;
     `false` otherwise. */
  inline proc isinf(x: real(64)): bool return chpl_macro_double_isinf(x):bool;

  /* Returns `true` if the argument `x` is a representation of *infinity*;
     `false` otherwise. */
  inline proc isinf(x: real(32)): bool return chpl_macro_float_isinf(x):bool;


  /* Returns `true` if the argument `x` does not represent a valid number;
     `false` otherwise. */
  inline proc isnan(x: real(64)): bool return chpl_macro_double_isnan(x):bool;

  /* Returns `true` if the argument `x` does not represent a valid number;
     `false` otherwise. */
  inline proc isnan(x: real(32)): bool return chpl_macro_float_isnan(x):bool;

  /* Multiply by an integer power of 2.
     Returns x * 2**n.
     */
  pragma "fn synchronization free"
  extern proc ldexp(x:real(64), n:int(32)):real(64);
  inline proc ldexp(x:real(32), n:int(32)):real(32) {
    pragma "fn synchronization free"
    extern proc ldexpf(x:real(32), n:int(32)):real(32);
    return ldexpf(x, n);
  }

  /* Returns the natural logarithm of the absolute value
     of the gamma function of the argument `x`.
  */
  pragma "fn synchronization free"
  extern proc lgamma(x: real(64)): real(64);

  /* Returns the natural logarithm of the absolute value
     of the gamma function of the argument `x`.
  */
  inline proc lgamma(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc lgammaf(x: real(32)): real(32);
    return lgammaf(x);
  }


  /* Returns the natural logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  pragma "fn synchronization free"
  extern proc log(x: real(64)): real(64);

  /* Returns the natural logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc logf(x: real(32)): real(32);
    return logf(x);
  }

  /* Returns the natural logarithm of the argument `z`. */
  inline proc log(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    extern proc clogf(z: complex(64)): complex(64);
    return clogf(z);
  }

  /* Returns the natural logarithm of the argument `z`. */
  inline proc log(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    extern proc clog(z: complex(128)): complex(128);
    return clog(z);
  }


  /* Returns the base 10 logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  pragma "fn synchronization free"
  extern proc log10(x: real(64)): real(64);

  /* Returns the base 10 logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log10(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc log10f(x: real(32)): real(32);
    return log10f(x);
  }


  /* Returns the natural logarithm of `x` + 1.

     It is an error if `x` is less than or equal to -1.
  */
  pragma "fn synchronization free"
  extern proc log1p(x: real(64)): real(64);

  /* Returns the natural logarithm of `x` + 1.

     It is an error if `x` is less than or equal to -1.
  */
  inline proc log1p(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc log1pf(x: real(32)): real(32);
    return log1pf(x);
  }


  /* Returns the log to the base `2**baseLog2` of the given `in` value.
     If `baseLog2` is `1`, then returns the log to the base `2`;
     if `baseLog2` is `2`, then returns the log to the base `4`, etc.
     Any fractional part is discarded.

     :rtype: `int`
  */
  inline proc logBasePow2(val: int(?w), baseLog2) {
    if (val < 1) {
      halt("Can't take the log() of a non-positive integer");
    }
    return _logBasePow2Help(val, baseLog2);
  }

  /* Returns the log to the base `2**baseLog2` of the given `in` value.
     If `baseLog2` is `1`, then returns the log to the base `2`;
     if `baseLog2` is `2`, then returns the log to the base `4`, etc.
     Any fractional part is discarded.

     :rtype: `int`
  */
  inline proc logBasePow2(val: uint(?w), baseLog2) {
    return _logBasePow2Help(val, baseLog2);
  }


  /* Returns the base 2 logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  pragma "fn synchronization free"
  extern proc log2(x: real(64)): real(64);

  /* Returns the base 2 logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log2(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc log2f(x: real(32)): real(32);
    return log2f(x);
  }


  /* Returns the base 2 logarithm of the argument `x`,
     rounded down.

     :rtype: `int`

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log2(val: int(?w)) {
    return logBasePow2(val, 1);
  }

  /* Returns the base 2 logarithm of the argument `x`,
     rounded down.

     :rtype: `int`

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log2(val: uint(?w)) {
    return logBasePow2(val, 1);
  }


  /* Computes the mod operator on the two arguments, defined as
     ``mod(m,n) = m - n * floor(m / n)``.

     The result is always >= 0 if `n` > 0.
     It is an error if `n` == 0.
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
     ``mod(m,n) = m - n * floor(m / n)``.

     If the arguments are of unsigned type, then
     fewer conditionals will be evaluated at run time.

     The result is always >= 0 if `n` > 0.
     It is an error if `n` == 0.
  */
  proc mod(m: integral, n: integral) {
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

  /* Computes the mod operator on the two numbers, defined as
     ``mod(x,y) = x - y * floor(x / y)``.

     The return value has the same type as `x`.
  */
  proc mod(x: real(?w), y: real(w)): real(w) {
    // This codes up the standard definition, according to Wikipedia.
    // Is there a more efficient implementation for reals?
    return x - y*floor(x/y);
  }


  /* Returns a value for which :proc:`isnan` will return `true`. */
  inline proc NAN param : real(64) return chpl_NAN;


  /* Returns the rounded integral value of the argument `x` determined by the
     current rounding direction.  :proc:`nearbyint` will not raise the "inexact"
     floating-point exception.
  */
  pragma "fn synchronization free"
  extern proc nearbyint(x: real(64)): real(64);

  /* Returns the rounded integral value of the argument `x` determined by the
     current rounding direction.  :proc:`nearbyint` will not raise the "inexact"
     floating-point exception.
  */
  inline proc nearbyint(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc nearbyintf(x: real(32)): real(32);
    return nearbyintf(x);
  }


  /* Returns the rounded integral value of the argument `x` determined by the
     current rounding direction.  :proc:`rint` may raise the "inexact" floating-point
     exception.
  */
  pragma "fn synchronization free"
  extern proc rint(x: real(64)): real(64);

  /* Returns the rounded integral value of the argument `x` determined by the
     current rounding direction.  :proc:`rint` may raise the "inexact" floating-point
     exception.
  */
  inline proc rint(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc rintf(x: real(32)): real(32);
    return rintf(x);
  }


  /* Returns the rounded integral value of the argument `x`. */
  pragma "fn synchronization free"
  extern proc round(x: real(64)): real(64);

  /* Returns the rounded integral value of the argument `x`. */
  inline proc round(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc roundf(x: real(32)): real(32);
    return roundf(x);
  }


  /* Returns the signum function of the integer argument `i`:
     1 if positive, -1 if negative, 0 if zero.
  */
  inline proc sgn(i : int(?w)): int(8)
    return ((i > 0) : int(8) - (i < 0) : int(8)) : int(8);

  /* Returns the signum function of the unsigned integer argument `i`:
     1 if positive, -1 if negative, 0 if zero.
  */
  inline proc sgn(i : uint(?w)): uint(8)
    return (i > 0) : uint(8);

  /* Returns the signum function of the integer param argument `i`:
     1 if positive, -1 if negative, 0 if zero.
  */
  proc sgn(param i : integral) param
    return if i > 0 then 1 else if i == 0 then 0 else -1;

  /* Returns the signum function of the real argument `x`:
     1 if positive, -1 if negative, 0 if zero.
  */
  inline proc sgn(x : real(?w)): int(8)
    return ((x > 0.0) : int(8) - (x < 0.0) : int(8)) : int(8);


  /* Returns the sine of the argument `x`. */
  pragma "fn synchronization free"
  extern proc sin(x: real(64)): real(64);

  /* Returns the sine of the argument `x`. */
  inline proc sin(x: real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc sinf(x: real(32)): real(32);
    return sinf(x);
  }

  /* Returns the sine of the argument `z`. */
  inline proc sin(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    extern proc csinf(z: complex(64)): complex(64);
    return csinf(z);
  }

  /* Returns the sine of the argument `z`. */
  inline proc sin(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    extern proc csin(z: complex(128)): complex(128);
    return csin(z);
  }


  /* Returns the hyperbolic sine of the argument `x`. */
  pragma "fn synchronization free"
  extern proc sinh(x: real(64)): real(64);

  /* Returns the hyperbolic sine of the argument `x`. */
  inline proc sinh(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc sinhf(x: real(32)): real(32);
    return sinhf(x);
  }

  /* Returns the hyperbolic sine of the argument `z`. */
  inline proc sinh(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    extern proc csinhf(z: complex(64)): complex(64);
    return csinhf(z);
  }

  /* Returns the hyperbolic sine of the argument `z`. */
  inline proc sinh(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    extern proc csinh(z: complex(128)): complex(128);
    return csinh(z);
  }


  /* Returns the square root of the argument `x`.

     It is an error if the `x` is less than zero.
  */
  pragma "fn synchronization free"
  extern proc sqrt(x: real(64)): real(64);

  /* Returns the square root of the argument `x`.

     It is an error if  `x` is less than zero.
  */
  inline proc sqrt(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc sqrtf(x: real(32)): real(32);
    return sqrtf(x);
  }

  /* Returns the square root of the argument `z`. */
  inline proc sqrt(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    extern proc csqrtf(z: complex(64)): complex(64);
    return csqrtf(z);
  }

  /* Returns the square root of the argument `z`. */
  inline proc sqrt(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    extern proc csqrt(z: complex(128)): complex(128);
    return csqrt(z);
  }


  /* Returns the tangent of the argument `x`. */
  pragma "fn synchronization free"
  extern proc tan(x: real(64)): real(64);

  /* Returns the tangent of the argument `x`. */
  inline proc tan(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc tanf(x: real(32)): real(32);
    return tanf(x);
  }

  /* Returns the tangent of the argument `z`. */
  inline proc tan(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    extern proc ctanf(z: complex(64)): complex(64);
    return ctanf(z);
  }

  /* Returns the tangent of the argument `z`. */
  inline proc tan(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    extern proc ctan(z: complex(128)): complex(128);
    return ctan(z);
  }


  /* Returns the hyperbolic tangent of the argument `x`. */
  pragma "fn synchronization free"
  extern proc tanh(x: real(64)): real(64);

  /* Returns the hyperbolic tangent of the argument `x`. */
  inline proc tanh(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc tanhf(x: real(32)): real(32);
    return tanhf(x);
  }

  /* Returns the hyperbolic tangent of the argument `z`. */
  inline proc tanh(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    extern proc ctanhf(z: complex(64)): complex(64);
    return ctanhf(z);
  }

  /* Returns the hyperbolic tangent of the argument `z`. */
  inline proc tanh(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    extern proc ctanh(z: complex(128)): complex(128);
    return ctanh(z);
  }



  /* Returns the absolute value of the gamma function of the argument `x`. */
  pragma "fn synchronization free"
  extern proc tgamma(x: real(64)): real(64);

  /* Returns the absolute value of the gamma function of the argument `x`. */
  inline proc tgamma(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc tgammaf(x: real(32)): real(32);
    return tgammaf(x);
  }


  /* Returns the nearest integral value to the argument `x` that is not larger
     than `x` in absolute value. */
  pragma "fn synchronization free"
  extern proc trunc(x: real(64)): real(64);

  /* Returns the nearest integral value to the argument `x` that is not larger
     than `x` in absolute value. */
  inline proc trunc(x : real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc truncf(x: real(32)): real(32);
    return truncf(x);
  }
  
  /* Returns the greatest common divisor of the integer argument `a` and
     `b`. */
  proc gcd(in a: int,in b: int): int {
     a = abs(a);
     b = abs(b);
     var r: int;
     while(b != 0) {
       r = a % b;
       a = b;
       b = r;
     }
    return a;
  }



  /* Returns the Bessel function of the first kind of order `0` of `x`. */
  inline proc j0(x: real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc chpl_float_j0(x: real(32)): real(32);
    return chpl_float_j0(x);
  }

  /* Returns the Bessel function of the first kind of order `0` of `x`. */
  inline proc j0(x: real(64)): real(64) {
    pragma "fn synchronization free"
    extern proc j0(x: real(64)): real(64);
    return j0(x);
  }

  /* Returns the Bessel function of the first kind of order `1` of `x`. */
  inline proc j1(x: real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc chpl_float_j1(x: real(32)): real(32);
    return chpl_float_j1(x);
  }

  /* Returns the Bessel function of the first kind of order `1` of `x`. */
  inline proc j1(x: real(64)): real(64) {
    pragma "fn synchronization free"
    extern proc j1(x: real(64)): real(64);
    return j1(x);
  }

  /* Returns the Bessel function of the first kind of order `n` of `x`. */
  inline proc jn(n: int, x: real(32)): real(32) {
    pragma "fn synchronization free"
    extern proc chpl_float_jn(n: c_int, x: real(32)): real(32);
    return chpl_float_jn(n.safeCast(c_int), x);
  }

  /* Returns the Bessel function of the first kind of order `n` of `x`. */
  inline proc jn(n: int, x: real(64)): real(64) {
    pragma "fn synchronization free"
    extern proc jn(n: c_int, x: real(64)): real(64);
    return jn(n.safeCast(c_int), x);
  }

  /* Returns the Bessel function of the second kind of order `0` of `x`, where
     `x` must be greater than 0 */
  inline proc y0(x: real(32)): real(32) {
    if boundsChecking && x < 0 then
      HaltWrappers.boundsCheckHalt("Input value for y0() must be non-negative");

    pragma "fn synchronization free"
    extern proc chpl_float_y0(x: real(32)): real(32);
    return chpl_float_y0(x);
  }

  /* Returns the Bessel function of the second kind of order `0` of `x`,
     where `x` must be greater than 0 */
  inline proc y0(x: real(64)): real(64) {
    if boundsChecking && x < 0 then
      HaltWrappers.boundsCheckHalt("Input value for y0() must be non-negative");

    pragma "fn synchronization free"
    extern proc y0(x: real(64)): real(64);
    return y0(x);
  }

  /* Returns the Bessel function of the second kind of order `1` of `x`,
     where `x` must be greater than 0 */
  inline proc y1(x: real(32)): real(32) {
    if boundsChecking && x < 0 then
      HaltWrappers.boundsCheckHalt("Input value for y1() must be non-negative");

    pragma "fn synchronization free"
    extern proc chpl_float_y1(x: real(32)): real(32);
    return chpl_float_y1(x);
  }

  /* Returns the Bessel function of the second kind of order `1` of `x`,
     where `x` must be greater than 0 */
  inline proc y1(x: real(64)): real(64) {
    if boundsChecking && x < 0 then
      HaltWrappers.boundsCheckHalt("Input value for y1() must be non-negative");

    pragma "fn synchronization free"
    extern proc y1(x: real(64)): real(64);
    return y1(x);
  }

  /* Returns the Bessel function of the second kind of order `n` of `x`,
     where `x` must be greater than 0 */
  inline proc yn(n: int, x: real(32)): real(32) {
    if boundsChecking && x < 0 then
      HaltWrappers.boundsCheckHalt("Input value for yn() must be non-negative");

    pragma "fn synchronization free"
    extern proc chpl_float_yn(n: c_int, x: real(32)): real(32);
    return chpl_float_yn(n.safeCast(c_int), x);
  }

  /* Returns the Bessel function of the second kind of order `n` of `x`,
     where `x` must be greater than 0 */
  inline proc yn(n: int, x: real(64)): real(64) {
    if boundsChecking && x < 0 then
      HaltWrappers.boundsCheckHalt("Input value for yn() must be non-negative");

    pragma "fn synchronization free"
    extern proc yn(n: c_int, x: real(64)): real(64);
    return yn(n.safeCast(c_int), x);
  }

  /* Returns true if the sign of `x` is negative, else returns false. It detects
     the sign bit of zeroes, infinities, and NANs */
  inline proc signbit(x : real(32)): bool {
    pragma "fn synchronization free"
    extern proc chpl_macro_float_signbit(x: real(32)): c_int;
    return chpl_macro_float_signbit(x): bool;
  }

  /* Returns true if the sign of `x` is negative, else returns false. It detects
     the sign bit of zeroes, infinities, and NANs */
  inline proc signbit(x : real(64)): bool {
    pragma "fn synchronization free"
    extern proc chpl_macro_double_signbit(x: real(64)): c_int;
    return chpl_macro_double_signbit(x): bool;
  }

} // end of module Math

// TODO: Consolidate overloaded signatures, to simplify the documentation.
