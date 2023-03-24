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

  /* Returns the phase (often called `argument`) of complex `z`, an angle (in
     radians).

     In concert with the related :proc:`~AutoMath.abs`, the magnitude (a.k.a.
     modulus) of `z`, it can be used to recompute `z`.

     :rtype: ``real(w/2)`` when `z` has a type of ``complex(w)``.
  */
  inline proc carg(z: complex(?w)): real(w/2) {
    return chpl_carg(z);
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
