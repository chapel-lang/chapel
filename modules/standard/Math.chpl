/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

This module provides less frequently used mathematical constants and functions.

It includes wrappers for many of the constants and functions in
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

Error Handling -- At present, Chapel does not provide control over error
handling in the Math module.  The default behavior is as if the macro
``math_errhandling`` is set to 0: Given erroneous input at run-time,
all math functions will return an implementation-defined value; no
exception will be generated.

.. _math-constants:

Constants
---------
:var:`e`
:var:`log2_e`
:var:`log10_e`
:var:`ln_2`
:var:`ln_10`
:var:`pi`
:var:`half_pi`
:var:`quarter_pi`
:var:`recipr_pi`
:var:`twice_recipr_pi`
:var:`twice_recipr_sqrt_pi`
:var:`sqrt_2`
:var:`recipr_sqrt_2`

.. _math-trigonometry:

Trigonometric Functions
-----------------------
:proc:`acos`
:proc:`acosh`
:proc:`asin`
:proc:`asinh`
:proc:`atan`
:proc:`atan2`
:proc:`atanh`
:proc:`cos`
:proc:`cosh`
:proc:`sin`
:proc:`sinh`
:proc:`tan`
:proc:`tanh`

.. _math-log:

Log Functions
-------------
:proc:`log`
:proc:`log10`
:proc:`log1p`
:proc:`logBasePow2`
:proc:`log2`

.. _math-exponential:

Exponential Functions
---------------------
:proc:`exp`
:proc:`exp2`
:proc:`expm1`
:proc:`ldexp`

.. _math-rounding:

Rounding
--------
:proc:`divceil`
:proc:`divceilpos`
:proc:`divfloor`
:proc:`divfloorpos`
:proc:`nearbyint`
:proc:`rint`

.. _math-gamma:

Gamma Functions
---------------
:proc:`lgamma`
:proc:`tgamma`

.. _math-error:

Error Functions
---------------
:proc:`erf`
:proc:`erfc`

.. _math-algorithms:

Algorithms
----------
:proc:`gcd`

.. _math-bessel:

Bessel Functions
----------------
:proc:`j0`
:proc:`j1`
:proc:`jn`
:proc:`y0`
:proc:`y1`
:proc:`yn`


Constant and Function Definitions
---------------------------------

*/
module Math {
  private use CTypes;
  private use AutoMath;

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

  /* Returns the arc cosine of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1.
  */
  inline proc acos(x: real(64)): real(64) {
    return chpl_acos(x);
  }

  /* Returns the arc cosine of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1.
  */
  inline proc acos(x : real(32)): real(32) {
    return chpl_acos(x);
  }

  /* Returns the arc cosine of the argument `x`. */
  inline proc acos(x: complex(64)): complex(64) {
    return chpl_acos(x);
  }

  /* Returns the arc cosine of the argument `x`. */
  inline proc acos(x: complex(128)): complex(128) {
    return chpl_acos(x);
  }

  /* Returns the inverse hyperbolic cosine of the argument `x`.

     It is an error if `x` is less than 1.
  */
  inline proc acosh(x: real(64)): real(64) {
    return chpl_acosh(x);
  }

  /* Returns the inverse hyperbolic cosine of the argument `x`.

     It is an error if `x` is less than 1.
  */
  inline proc acosh(x : real(32)): real(32) {
    return chpl_acosh(x);
  }

  /* Returns the inverse hyperbolic cosine of the argument `x`. */
  inline proc acosh(x: complex(64)): complex(64) {
    return chpl_acosh(x);
  }

  /* Returns the inverse hyperbolic cosine of the argument `x`. */
  inline proc acosh(x: complex(128)): complex(128) {
    return chpl_acosh(x);
  }

  /* Returns the arc sine of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1.
  */
  inline proc asin(x: real(64)): real(64) {
    return chpl_asin(x);
  }

  /* Returns the arc sine of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1.
  */
  inline proc asin(x : real(32)): real(32) {
    return chpl_asin(x);
  }

  /* Returns the arc sine of the argument `x`. */
  inline proc asin(x: complex(64)): complex(64) {
    return chpl_asin(x);
  }

  /* Returns the arc sine of the argument `x`. */
  inline proc asin(x: complex(128)): complex(128) {
    return chpl_asin(x);
  }

  /* Returns the inverse hyperbolic sine of the argument `x`. */
  inline proc asinh(x: real(64)): real(64) {
    return chpl_asinh(x);
  }

  /* Returns the inverse hyperbolic sine of the argument `x`. */
  inline proc asinh(x : real(32)): real(32) {
    return chpl_asinh(x);
  }

  /* Returns the inverse hyperbolic sine of the argument `x`. */
  inline proc asinh(x: complex(64)): complex(64) {
    return chpl_asinh(x);
  }

  /* Returns the inverse hyperbolic sine of the argument `x`. */
  inline proc asinh(x: complex(128)): complex(128) {
    return chpl_asinh(x);
  }

  /* Returns the arc tangent of the argument `x`. */
  inline proc atan(x: real(64)): real(64) {
    return chpl_atan(x);
  }

  /* Returns the arc tangent of the argument `x`. */
  inline proc atan(x : real(32)): real(32) {
    return chpl_atan(x);
  }

  /* Returns the arc tangent of the argument `x`. */
  inline proc atan(x: complex(64)): complex(64) {
    return chpl_atan(x);
  }

  /* Returns the arc tangent of the argument `x`. */
  inline proc atan(x: complex(128)): complex(128) {
    return chpl_atan(x);
  }

  /* Returns the arc tangent of the ratio of the two arguments.

     This is equivalent to
     the arc tangent of `y` / `x` except that the signs of `y`
     and `x` are used to determine the quadrant of the result. */
  inline proc atan2(y: real(64), x: real(64)): real(64) {
    return chpl_atan2(y, x);
  }

  /* Returns the arc tangent of the two arguments.

     This is equivalent to
     the arc tangent of `y` / `x` except that the signs of `y`
     and `x` are used to determine the quadrant of the result. */
  inline proc atan2(y : real(32), x: real(32)): real(32) {
    return chpl_atan2(y, x);
  }

  /* Returns the inverse hyperbolic tangent of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1. */
  inline proc atanh(x: real(64)): real(64) {
    return chpl_atanh(x);
  }

  /* Returns the inverse hyperbolic tangent of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1. */
  inline proc atanh(x : real(32)): real(32) {
    return chpl_atanh(x);
  }

  /* Returns the inverse hyperbolic tangent of the argument `x`. */
  inline proc atanh(x: complex(64)): complex(64) {
    return chpl_atanh(x);
  }

  /* Returns the inverse hyperbolic tangent of the argument `x`. */
  inline proc atanh(x: complex(128)): complex(128) {
    return chpl_atanh(x);
  }

  /* Returns the cosine of the argument `x`. */
  inline proc cos(x: real(64)): real(64) {
    return chpl_cos(x);
  }

  /* Returns the cosine of the argument `x`. */
  inline proc cos(x : real(32)): real(32) {
    return chpl_cos(x);
  }

  /* Returns the cosine of the argument `x`. */
  inline proc cos(x : complex(64)): complex(64) {
    return chpl_cos(x);
  }

  /* Returns the cosine of the argument `x`. */
  inline proc cos(x : complex(128)): complex(128) {
    return chpl_cos(x);
  }

  /* Returns the hyperbolic cosine of the argument `x`. */
  inline proc cosh(x: real(64)): real(64) {
    return chpl_cosh(x);
  }

  /* Returns the hyperbolic cosine of the argument `x`. */
  inline proc cosh(x : real(32)): real(32) {
    return chpl_cosh(x);
  }

  /* Returns the hyperbolic cosine of the argument `x`. */
  inline proc cosh(x: complex(64)): complex(64) {
    return chpl_cosh(x);
  }

  /* Returns the hyperbolic cosine of the argument `x`. */
  inline proc cosh(x: complex(128)): complex(128) {
    return chpl_cosh(x);
  }

  /* Returns :proc:`~AutoMath.ceil`\(`m`/`n`),
     i.e., the fraction `m`/`n` rounded up to the nearest integer.

     If the arguments are of unsigned type, then
     fewer conditionals will be evaluated at run time.
  */
  proc divceil(param m: integral, param n: integral) param do
    return chpl_divceil(m, n);

  /* Returns :proc:`~AutoMath.ceil`\(`m`/`n`),
     i.e., the fraction `m`/`n` rounded up to the nearest integer.

     If the arguments are of unsigned type, then
     fewer conditionals will be evaluated at run time.
  */
  proc divceil(m: integral, n: integral) do return chpl_divceil(m, n);

  /*
    A variant of :proc:`divceil` that performs no runtime checks.
    The user must ensure that both arguments are strictly positive
    (not 0) and are of a signed integer type (not `uint`).
  */
  @unstable("divceilpos is unstable due to questions about its utility.  If you find this function valuable, please let us know!")
  proc divceilpos(m: integral, n: integral) {
    return chpl_divceilpos(m, n);
  }

  /* Returns :proc:`~AutoMath.floor`\(`m`/`n`),
     i.e., the fraction `m`/`n` rounded down to the nearest integer.

     If the arguments are of unsigned type, then
     fewer conditionals will be evaluated at run time.
  */
  proc divfloor(param m: integral, param n: integral) param do return
    chpl_divfloor(m, n);

  /* Returns :proc:`~AutoMath.floor`\(`m`/`n`),
     i.e., the fraction `m`/`n` rounded down to the nearest integer.

     If the arguments are of unsigned type, then
     fewer conditionals will be evaluated at run time.
  */
  proc divfloor(m: integral, n: integral) do return chpl_divfloor(m, n);

  /*
    A variant of :proc:`divfloor` that performs no runtime checks.
    The user must ensure that both arguments are strictly positive
    (not 0) and are of a signed integer type (not `uint`).
  */
  @unstable("divfloorpos is unstable due to questions about its utility.  If you find this function valuable, please let us know!")
  proc divfloorpos(m: integral, n: integral) {
    return chpl_divfloorpos(m, n);
  }

  /* Returns the error function of the argument `x`. */
  inline proc erf(x: real(64)): real(64) {
    return chpl_erf(x);
  }

  /* Returns the error function of the argument `x`. */
  inline proc erf(x : real(32)): real(32) {
    return chpl_erf(x);
  }

  /* Returns the complementary error function of the argument.
     This is equivalent to 1.0 - :proc:`erf`\(`x`).
  */
  inline proc erfc(x: real(64)): real(64) {
    return chpl_erfc(x);
  }

  /* Returns the complementary error function of the argument.
     This is equivalent to 1.0 - :proc:`erf`\(`x`).
  */
  inline proc erfc(x : real(32)): real(32) {
    return chpl_erfc(x);
  }

  /* Returns the value of the Napierian `e` raised to the power of the
     argument `x`. */
  inline proc exp(x: real(64)): real(64) {
    return chpl_exp(x);
  }

  /* Returns the value of the Napierian `e` raised to the power of the
     argument. */
  inline proc exp(x : real(32)): real(32) {
    return chpl_exp(x);
  }

  /* Returns the value of the Napierian `e` raised to the power of the
     argument. */
  inline proc exp(x: complex(64)): complex(64) {
    return chpl_exp(x);
  }

  /* Returns the value of the Napierian `e` raised to the power of the
     argument. */
  inline proc exp(x: complex(128)): complex(128) {
    return chpl_exp(x);
  }

  /* Returns the value of `2` raised to the power of the argument `x`. */
  inline proc exp2(x: real(64)): real(64) {
    return chpl_exp2(x);
  }

  /* Returns the value of `2` raised to the power of the argument `x`. */
  inline proc exp2(x : real(32)): real(32) {
    return chpl_exp2(x);
  }

  /* Returns one less than the value of the Napierian `e` raised to the power
     of the argument `x`. */
  inline proc expm1(x: real(64)): real(64) {
    return chpl_expm1(x);
  }

  /* Returns one less than the value of the Napierian `e` raised to the power
     of the argument `x`. */
  inline proc expm1(x : real(32)): real(32) {
    return chpl_expm1(x);
  }

  /* Multiply by an integer power of 2.
     Returns x * 2**n.
     */
  inline proc ldexp(x:real(64), n:int(32)):real(64) {
    return chpl_ldexp(x, n);
  }

  inline proc ldexp(x:real(32), n:int(32)):real(32) {
    return chpl_ldexp(x, n);
  }

  /* Returns the natural logarithm of the absolute value
     of the gamma function of the argument `x`.
  */
  inline proc lgamma(x: real(64)): real(64) {
    return chpl_lgamma(x);
  }

  /* Returns the natural logarithm of the absolute value
     of the gamma function of the argument `x`.
  */
  inline proc lgamma(x : real(32)): real(32) {
    return chpl_lgamma(x);
  }

  /* Returns the natural logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log(x: real(64)): real(64) {
    return chpl_log(x);
  }

  /* Returns the natural logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log(x : real(32)): real(32) {
    return chpl_log(x);
  }

  /* Returns the natural logarithm of the argument `x`. */
  inline proc log(x: complex(64)): complex(64) {
    return chpl_log(x);
  }

  /* Returns the natural logarithm of the argument `x`. */
  inline proc log(x: complex(128)): complex(128) {
    return chpl_log(x);
  }

  /* Returns the base 10 logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log10(x: real(64)): real(64) {
    return chpl_log10(x);
  }

  /* Returns the base 10 logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log10(x : real(32)): real(32) {
    return chpl_log10(x);
  }

  /* Returns the natural logarithm of `x` + 1.

     It is an error if `x` is less than or equal to -1.
  */
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  extern proc log1p(x: real(64)): real(64);

  /* Returns the natural logarithm of `x` + 1.

     It is an error if `x` is less than or equal to -1.
  */
  inline proc log1p(x : real(32)): real(32) {
    return chpl_log1p(x);
  }

  /* Returns the log to the base `2**baseLog2` of the given `in` value.
     If `baseLog2` is `1`, then returns the log to the base `2`;
     if `baseLog2` is `2`, then returns the log to the base `4`, etc.
     Any fractional part is discarded.

     :rtype: `int`
  */
  inline proc logBasePow2(val: int(?w), baseLog2) {
    return chpl_logBasePow2(val, baseLog2);
  }

  /* Returns the log to the base `2**baseLog2` of the given `in` value.
     If `baseLog2` is `1`, then returns the log to the base `2`;
     if `baseLog2` is `2`, then returns the log to the base `4`, etc.
     Any fractional part is discarded.

     :rtype: `int`
  */
  inline proc logBasePow2(val: uint(?w), baseLog2) {
    return chpl_logBasePow2(val, baseLog2);
  }

  /* Returns the base 2 logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log2(x: real(64)): real(64) {
    return chpl_log2(x);
  }

  /* Returns the base 2 logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log2(x : real(32)): real(32) {
    return chpl_log2(x);
  }

  /* Returns the base 2 logarithm of the argument `x`,
     rounded down.

     :rtype: `int`

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log2(x: int(?w)) {
    return chpl_log2(x);
  }

  /* Returns the base 2 logarithm of the argument `x`,
     rounded down.

     :rtype: `int`

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log2(x: uint(?w)) {
    return chpl_log2(x);
  }

  /* Returns the rounded integral value of the argument `x` determined by the
     current rounding direction.  :proc:`nearbyint` will not raise the "inexact"
     floating-point exception.
  */
  @unstable("nearbyint is unstable while we design more thorough rounding support")
  inline proc nearbyint(x: real(64)): real(64) {
    return chpl_nearbyint(x);
  }

  /* Returns the rounded integral value of the argument `x` determined by the
     current rounding direction.  :proc:`nearbyint` will not raise the "inexact"
     floating-point exception.
  */
  @unstable("nearbyint is unstable while we design more thorough rounding support")
  inline proc nearbyint(x : real(32)): real(32) {
    return chpl_nearbyint(x);
  }

  /* Returns the rounded integral value of the argument `x` determined by the
     current rounding direction.  :proc:`rint` may raise the "inexact"
     floating-point exception.
  */
  @unstable("rint is unstable while we design more thorough rounding support")
  inline proc rint(x: real(64)): real(64) {
    return chpl_rint(x);
  }

  /* Returns the rounded integral value of the argument `x` determined by the
     current rounding direction.  :proc:`rint` may raise the "inexact" floating-point
     exception.
  */
  @unstable("rint is unstable while we design more thorough rounding support")
  inline proc rint(x : real(32)): real(32) {
    return chpl_rint(x);
  }

  /* Returns the sine of the argument `x`. */
  inline proc sin(x: real(64)): real(64) {
    return chpl_sin(x);
  }

  /* Returns the sine of the argument `x`. */
  inline proc sin(x: real(32)): real(32) {
    return chpl_sin(x);
  }

  /* Returns the sine of the argument `x`. */
  inline proc sin(x: complex(64)): complex(64) {
    return chpl_sin(x);
  }

  /* Returns the sine of the argument `x`. */
  inline proc sin(x: complex(128)): complex(128) {
    return chpl_sin(x);
  }

  /* Returns the hyperbolic sine of the argument `x`. */
  inline proc sinh(x: real(64)): real(64) {
    return chpl_sinh(x);
  }

  /* Returns the hyperbolic sine of the argument `x`. */
  inline proc sinh(x : real(32)): real(32) {
    return chpl_sinh(x);
  }

  /* Returns the hyperbolic sine of the argument `x`. */
  inline proc sinh(x: complex(64)): complex(64) {
    return chpl_sinh(x);
  }

  /* Returns the hyperbolic sine of the argument `x`. */
  inline proc sinh(x: complex(128)): complex(128) {
    return chpl_sinh(x);
  }

  /* Returns the tangent of the argument `x`. */
  inline proc tan(x: real(64)): real(64) {
    return chpl_tan(x);
  }

  /* Returns the tangent of the argument `x`. */
  inline proc tan(x : real(32)): real(32) {
    return chpl_tan(x);
  }

  /* Returns the tangent of the argument `x`. */
  inline proc tan(x: complex(64)): complex(64) {
    return chpl_tan(x);
  }

  /* Returns the tangent of the argument `x`. */
  inline proc tan(x: complex(128)): complex(128) {
    return chpl_tan(x);
  }

  /* Returns the hyperbolic tangent of the argument `x`. */
  inline proc tanh(x: real(64)): real(64) {
    return chpl_tanh(x);
  }

  /* Returns the hyperbolic tangent of the argument `x`. */
  inline proc tanh(x : real(32)): real(32) {
    return chpl_tanh(x);
  }

  /* Returns the hyperbolic tangent of the argument `x`. */
  inline proc tanh(x: complex(64)): complex(64) {
    return chpl_tanh(x);
  }

  /* Returns the hyperbolic tangent of the argument `x`. */
  inline proc tanh(x: complex(128)): complex(128) {
    return chpl_tanh(x);
  }

  /* Returns the absolute value of the gamma function of the argument `x`. */
  inline proc tgamma(x: real(64)): real(64) {
    return chpl_tgamma(x);
  }

  /* Returns the absolute value of the gamma function of the argument `x`. */
  inline proc tgamma(x : real(32)): real(32) {
    return chpl_tgamma(x);
  }

  /* Returns the greatest common divisor of the integer argument `a` and
     `b`. */
  proc gcd(in a: int,in b: int): int {
    return chpl_gcd(a, b);
  }

  /* Returns the Bessel function of the first kind of order `0` of `x`. */
  inline proc j0(x: real(32)): real(32) {
    return chpl_j0(x);
  }

  /* Returns the Bessel function of the first kind of order `0` of `x`. */
  inline proc j0(x: real(64)): real(64) {
    return chpl_j0(x);
  }

  /* Returns the Bessel function of the first kind of order `1` of `x`. */
  inline proc j1(x: real(32)): real(32) {
    return chpl_j1(x);
  }

  /* Returns the Bessel function of the first kind of order `1` of `x`. */
  inline proc j1(x: real(64)): real(64) {
    return chpl_j1(x);
  }

  /* Returns the Bessel function of the first kind of order `n` of `x`. */
  inline proc jn(n: int, x: real(32)): real(32) {
    return chpl_jn(n, x);
  }

  /* Returns the Bessel function of the first kind of order `n` of `x`. */
  inline proc jn(n: int, x: real(64)): real(64) {
    return chpl_jn(n, x);
  }

  /* Returns the Bessel function of the second kind of order `0` of `x`, where
     `x` must be greater than 0 */
  inline proc y0(x: real(32)): real(32) {
    return chpl_y0(x);
  }

  /* Returns the Bessel function of the second kind of order `0` of `x`,
     where `x` must be greater than 0 */
  inline proc y0(x: real(64)): real(64) {
    return chpl_y0(x);
  }

  /* Returns the Bessel function of the second kind of order `1` of `x`,
     where `x` must be greater than 0 */
  inline proc y1(x: real(32)): real(32) {
    return chpl_y1(x);
  }

  /* Returns the Bessel function of the second kind of order `1` of `x`,
     where `x` must be greater than 0 */
  inline proc y1(x: real(64)): real(64) {
    return chpl_y1(x);
  }

  /* Returns the Bessel function of the second kind of order `n` of `x`,
     where `x` must be greater than 0 */
  inline proc yn(n: int, x: real(32)): real(32) {
    return chpl_yn(n, x);
  }

  /* Returns the Bessel function of the second kind of order `n` of `x`,
     where `x` must be greater than 0 */
  inline proc yn(n: int, x: real(64)): real(64) {
    return chpl_yn(n, x);
  }
}
