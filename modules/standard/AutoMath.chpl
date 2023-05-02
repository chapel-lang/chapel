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
This module provides mathematical constants and functions.

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

Rounding -- The rounding mode for floating-point addition (subtraction) is
implementation-defined.

Error Handling -- At present, Chapel does not provide control over error
handling in the AutoMath module.  The default behavior is as if the macro
``math_errhandling`` is set to 0: Given erroneous input at run-time,
all math functions will return an implementation-defined value; no
exception will be generated.

*/
pragma "module included by default"
module AutoMath {
  import HaltWrappers;
  private use CTypes;

  //////////////////////////////////////////////////////////////////////////
  // Constants (included in chpldocs)
  //

  pragma "last resort"
  @chpldoc.nodoc
  @deprecated(notes="In an upcoming release 'e' will no longer be included by default, please 'use' or 'import' the 'Math' module to access it")
  param e = 2.7182818284590452354;

  pragma "last resort"
  @chpldoc.nodoc
  @deprecated(notes="In an upcoming release 'log2_e' will no longer be included by default, please 'use' or 'import' the 'Math' module to access it")
  param log2_e = 1.4426950408889634074;

  pragma "last resort"
  @chpldoc.nodoc
  @deprecated(notes="In an upcoming release 'log10_e' will no longer be included by default, please 'use' or 'import' the 'Math' module to access it")
  param log10_e = 0.43429448190325182765;

  pragma "last resort"
  @chpldoc.nodoc
  @deprecated(notes="In an upcoming release 'ln_2' will no longer be included by default, please 'use' or 'import' the 'Math' module to access it")
  param ln_2 = 0.69314718055994530942;

  pragma "last resort"
  @chpldoc.nodoc
  @deprecated(notes="In an upcoming release 'ln_10' will no longer be included by default, please 'use' or 'import' the 'Math' module to access it")
  param ln_10 = 2.30258509299404568402;

  pragma "last resort"
  @chpldoc.nodoc
  @deprecated(notes="In an upcoming release 'pi' will no longer be included by default, please 'use' or 'import' the 'Math' module to access it")
  param pi = 3.14159265358979323846;

  pragma "last resort"
  @chpldoc.nodoc
  @deprecated(notes="In an upcoming release 'half_pi' will no longer be included by default, please 'use' or 'import' the 'Math' module to access it")
  param half_pi = 1.57079632679489661923;

  pragma "last resort"
  @chpldoc.nodoc
  @deprecated(notes="In an upcoming release 'quarter_pi' will no longer be included by default, please 'use' or 'import' the 'Math' module to access it")
  param quarter_pi = 0.78539816339744830962;

  pragma "last resort"
  @chpldoc.nodoc
  @deprecated(notes="In an upcoming release 'recipr_pi' will no longer be included by default, please 'use' or 'import' the 'Math' module to access it")
  param recipr_pi = 0.31830988618379067154;

  pragma "last resort"
  @chpldoc.nodoc
  @deprecated(notes="In an upcoming release 'twice_recipr_pi' will no longer be included by default, please 'use' or 'import' the 'Math' module to access it")
  param twice_recipr_pi = 0.63661977236758134308;

  pragma "last resort"
  @chpldoc.nodoc
  @deprecated(notes="In an upcoming release 'twice_recipr_sqrt_pi' will no longer be included by default, please 'use' or 'import' the 'Math' module to access it")
  param twice_recipr_sqrt_pi = 1.12837916709551257390;

  pragma "last resort"
  @chpldoc.nodoc
  @deprecated(notes="In an upcoming release 'sqrt_2' will no longer be included by default, please 'use' or 'import' the 'Math' module to access it")
  param sqrt_2 = 1.41421356237309504880;

  pragma "last resort"
  @chpldoc.nodoc
  @deprecated(notes="In an upcoming release 'recipr_sqrt_2' will no longer be included by default, please 'use' or 'import' the 'Math' module to access it")
  param recipr_sqrt_2 = 0.70710678118654752440;

  //////////////////////////////////////////////////////////////////////////
  // Helper constants and functions (not included in chpldocs).
  //
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  private extern proc chpl_macro_double_isinf(x: real(64)): c_int;
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  private extern proc chpl_macro_float_isinf(x: real(32)): c_int;
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  private extern proc chpl_macro_double_isfinite(x: real(64)): c_int;
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  private extern proc chpl_macro_float_isfinite(x: real(32)): c_int;
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  private extern proc chpl_macro_double_isnan(x: real(64)): c_int;
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  private extern proc chpl_macro_float_isnan(x: real(32)): c_int;

  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  private extern proc fabs(x: real(64)): real(64);


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
  inline proc abs(i : int(?w)) do return if i < 0 then -i else i;

  /* Returns the absolute value of the unsigned integer argument.

     :rtype: The type of `i`.
  */
  inline proc abs(i : uint(?w)) do return i;

  /* Returns the absolute value of the integer param argument `i`. */
  proc abs(param i : integral) param do return if i < 0 then -i else i;

  /* Returns the magnitude of the real argument `r`. */
  inline proc abs(r : real(64)):real(64) do return fabs(r);

  /* Returns the magnitude of the real argument `x`. */
  inline proc abs(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc fabsf(x: real(32)): real(32);
    return fabsf(x);
  }

  /* Returns the real magnitude of the imaginary argument `im`. */
  inline proc abs(im : imag(64)): real(64) do return fabs(_i2r(im));

  /* Returns the real magnitude of the imaginary argument `im`. */
  inline proc abs(im: imag(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc fabsf(x: real(32)): real(32);
    return fabsf(_i2r(im));
  }

  /* Returns the magnitude (often called modulus) of complex `z`.

     In concert with the related :proc:`carg`, the phase (a.k.a. argument)
     of `z`, it can be used to recompute `z`.

     :rtype: ``real(w/2)`` when `z` has a type of ``complex(w)``.
  */
  inline proc abs(z : complex(?w)): real(w/2) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc cabsf(z: complex(64)): real(32);
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc cabs(z: complex(128)): real(64);
    if w == 64 then
      return cabsf(z);
    else
      return cabs(z);
  }

  /* Returns the phase (often called `argument`) of complex `z`, an angle (in
     radians).

     In concert with the related :proc:`abs`, the magnitude (a.k.a.
     modulus) of `z`, it can be used to recompute `z`.

     :rtype: ``real(w/2)`` when `z` has a type of ``complex(w)``.
  */

  inline proc carg(z: complex(?w)): real(w/2) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc cargf(z: complex(64)): real(32);
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc carg(z: complex(128)): real(64);
    if w == 64 then
      return cargf(z);
    else
      return carg(z);
  }


  // When removing this deprecated function, be sure to remove chpl_acos and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the arc cosine of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1.
  */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'acos' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc acos(x: real(64)): real(64) {
    return chpl_acos(x);
  }

  @chpldoc.nodoc
  inline proc chpl_acos(x: real(64)): real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc acos(x: real(64)): real(64);
    return acos(x);
  }

  // When removing this deprecated function, be sure to remove chpl_acos and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the arc cosine of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1.
  */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'acos' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc acos(x : real(32)): real(32) {
    return chpl_acos(x);
  }

  @chpldoc.nodoc
  inline proc chpl_acos(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc acosf(x: real(32)): real(32);
    return acosf(x);
  }

  // When removing this deprecated function, be sure to remove chpl_acos and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the arc cosine of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'acos' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc acos(z: complex(64)): complex(64) {
    return chpl_acos(z);
  }

  @chpldoc.nodoc
  inline proc chpl_acos(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc cacosf(z: complex(64)): complex(64);
    return cacosf(z);
  }

  // When removing this deprecated function, be sure to remove chpl_acos and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the arc cosine of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'acos' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc acos(z: complex(128)): complex(128) {
    return chpl_acos(z);
  }

  @chpldoc.nodoc
  inline proc chpl_acos(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc cacos(z: complex(128)): complex(128);
    return cacos(z);
  }


  // When removing this deprecated function, be sure to remove chpl_acosh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the inverse hyperbolic cosine of the argument `x`.

     It is an error if `x` is less than 1.
  */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'acosh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc acosh(x: real(64)): real(64) {
    return chpl_acosh(x);
  }

  @chpldoc.nodoc
  inline proc chpl_acosh(x: real(64)): real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc acosh(x: real(64)): real(64);
    return acosh(x);
  }

  // When removing this deprecated function, be sure to remove chpl_acosh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the inverse hyperbolic cosine of the argument `x`.

     It is an error if `x` is less than 1.
  */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'acosh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc acosh(x : real(32)): real(32) {
    return chpl_acosh(x);
  }

  @chpldoc.nodoc
  inline proc chpl_acosh(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc acoshf(x: real(32)): real(32);
    return acoshf(x);
  }

  // When removing this deprecated function, be sure to remove chpl_acosh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the inverse hyperbolic cosine of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'acosh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc acosh(z: complex(64)): complex(64) {
    return chpl_acosh(z);
  }

  @chpldoc.nodoc
  inline proc chpl_acosh(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc cacoshf(z: complex(64)): complex(64);
    return cacoshf(z);
  }

  // When removing this deprecated function, be sure to remove chpl_acosh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the inverse hyperbolic cosine of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'acosh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc acosh(z: complex(128)): complex(128) {
    return chpl_acosh(z);
  }

  @chpldoc.nodoc
  inline proc chpl_acosh(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc cacosh(z: complex(128)): complex(128);
    return cacosh(z);
  }


  // When removing this deprecated function, be sure to remove chpl_asin and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the arc sine of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1.
  */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'asin' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc asin(x: real(64)): real(64) {
    return chpl_asin(x);
  }

  @chpldoc.nodoc
  inline proc chpl_asin(x: real(64)): real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc asin(x: real(64)): real(64);
    return asin(x);
  }

  // When removing this deprecated function, be sure to remove chpl_asin and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the arc sine of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1.
  */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'asin' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc asin(x : real(32)): real(32) {
    return chpl_asin(x);
  }

  @chpldoc.nodoc
  inline proc chpl_asin(x: real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc asinf(x: real(32)): real(32);
    return asinf(x);
  }

  // When removing this deprecated function, be sure to remove chpl_asin and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the arc sine of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'asin' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc asin(z: complex(64)): complex(64) {
    return chpl_asin(z);
  }

  @chpldoc.nodoc
  inline proc chpl_asin(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc casinf(z: complex(64)): complex(64);
    return casinf(z);
  }

  // When removing this deprecated function, be sure to remove chpl_asin and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the arc sine of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'asin' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc asin(z: complex(128)): complex(128) {
    return chpl_asin(z);
  }

  @chpldoc.nodoc
  inline proc chpl_asin(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc casin(z: complex(128)): complex(128);
    return casin(z);
  }


  // When removing this deprecated function, be sure to remove chpl_asinh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the inverse hyperbolic sine of the argument `x`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'asinh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc asinh(x: real(64)): real(64) {
    return chpl_asinh(x);
  }

  @chpldoc.nodoc
  inline proc chpl_asinh(x: real(64)): real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc asinh(x: real(64)): real(64);
    return asinh(x);
  }

  // When removing this deprecated function, be sure to remove chpl_asinh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the inverse hyperbolic sine of the argument `x`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'asinh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc asinh(x : real(32)): real(32) {
    return chpl_asinh(x);
  }

  @chpldoc.nodoc
  inline proc chpl_asinh(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc asinhf(x: real(32)): real(32);
    return asinhf(x);
  }

  // When removing this deprecated function, be sure to remove chpl_asinh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the inverse hyperbolic sine of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'asinh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc asinh(z: complex(64)): complex(64) {
    return chpl_asinh(z);
  }

  @chpldoc.nodoc
  inline proc chpl_asinh(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc casinhf(z: complex(64)): complex(64);
    return casinhf(z);
  }

  // When removing this deprecated function, be sure to remove chpl_asinh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the inverse hyperbolic sine of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'asinh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc asinh(z: complex(128)): complex(128) {
    return chpl_asinh(z);
  }

  @chpldoc.nodoc
  inline proc chpl_asinh(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc casinh(z: complex(128)): complex(128);
    return casinh(z);
  }



  // When removing this deprecated function, be sure to remove chpl_atan and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the arc tangent of the argument `x`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'atan' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc atan(x: real(64)): real(64) {
    return chpl_atan(x);
  }

  @chpldoc.nodoc
  inline proc chpl_atan(x: real(64)): real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc atan(x: real(64)): real(64);
    return atan(x);
  }

  // When removing this deprecated function, be sure to remove chpl_atan and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the arc tangent of the argument `x`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'atan' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc atan(x : real(32)): real(32) {
    return chpl_atan(x);
  }

  @chpldoc.nodoc
  inline proc chpl_atan(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc atanf(x: real(32)): real(32);
    return atanf(x);
  }

  // When removing this deprecated function, be sure to remove chpl_atan and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the arc tangent of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'atan' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc atan(z: complex(64)): complex(64) {
    return chpl_atan(z);
  }

  @chpldoc.nodoc
  inline proc chpl_atan(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc catanf(z: complex(64)): complex(64);
    return catanf(z);
  }

  // When removing this deprecated function, be sure to remove chpl_atan and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the arc tangent of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'atan' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc atan(z: complex(128)): complex(128) {
    return chpl_atan(z);
  }

  @chpldoc.nodoc
  inline proc chpl_atan(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc catan(z: complex(128)): complex(128);
    return catan(z);
  }


  // When removing this deprecated function, be sure to remove chpl_atan2 and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the arc tangent of the ratio of the two arguments.

     This is equivalent to
     the arc tangent of `y` / `x` except that the signs of `y`
     and `x` are used to determine the quadrant of the result. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'atan2' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc atan2(y: real(64), x: real(64)): real(64) {
    return chpl_atan2(y, x);
  }

  @chpldoc.nodoc
  inline proc chpl_atan2(y: real(64), x: real(64)): real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc atan2(y: real(64), x: real(64)): real(64);
    return atan2(y, x);
  }

  // When removing this deprecated function, be sure to remove chpl_atan2 and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the arc tangent of the two arguments.

     This is equivalent to
     the arc tangent of `y` / `x` except that the signs of `y`
     and `x` are used to determine the quadrant of the result. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'atan2' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc atan2(y : real(32), x: real(32)): real(32) {
    return chpl_atan2(y, x);
  }

  @chpldoc.nodoc
  inline proc chpl_atan2(y : real(32), x: real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc atan2f(y: real(32), x: real(32)): real(32);
    return atan2f(y, x);
  }


  // When removing this deprecated function, be sure to remove chpl_atanh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the inverse hyperbolic tangent of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'atanh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc atanh(x: real(64)): real(64) {
    return chpl_atanh(x);
  }

  @chpldoc.nodoc
  inline proc chpl_atanh(x: real(64)): real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc atanh(x: real(64)): real(64);
    return atanh(x);
  }

  // When removing this deprecated function, be sure to remove chpl_atanh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the inverse hyperbolic tangent of the argument `x`.

     It is an error if `x` is less than -1 or greater than 1. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'atanh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc atanh(x : real(32)): real(32) {
    return chpl_atanh(x);
  }

  @chpldoc.nodoc
  inline proc chpl_atanh(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc atanhf(x: real(32)): real(32);
    return atanhf(x);
  }

  // When removing this deprecated function, be sure to remove chpl_atanh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the inverse hyperbolic tangent of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'atanh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc atanh(z: complex(64)): complex(64) {
    return chpl_atanh(z);
  }

  @chpldoc.nodoc
  inline proc chpl_atanh(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc catanhf(z: complex(64)): complex(64);
    return catanhf(z);
  }

  // When removing this deprecated function, be sure to remove chpl_atanh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the inverse hyperbolic tangent of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'atanh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc atanh(z: complex(128)): complex(128) {
    return chpl_atanh(z);
  }

  @chpldoc.nodoc
  inline proc chpl_atanh(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc catanh(z: complex(128)): complex(128);
    return catanh(z);
  }


  /* Returns the cube root of the argument `x`. */
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  extern proc cbrt(x: real(64)): real(64);

  /* Returns the cube root of the argument `x`. */
  inline proc cbrt(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc cbrtf(x: real(32)): real(32);
    return cbrtf(x);
  }


  /* Returns the value of the argument `x` rounded up to the nearest integer. */
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  extern proc ceil(x: real(64)): real(64);

  /* Returns the value of the argument `x` rounded up to the nearest integer. */
  inline proc ceil(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc ceilf(x: real(32)): real(32);
    return ceilf(x);
  }


  /* Returns the complex conjugate of the complex argument `z`.

     :rtype: A complex number of the same type as `z`.
  */
  inline proc conjg(z: complex(?w)) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc conjf(z: complex(64)): complex(64);
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
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
  inline proc cproj(z: complex(?w)): complex(w) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc cprojf(z: complex(64)): complex(64);
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc cproj(z: complex(128)): complex(128);
    if w == 64 then
      return cprojf(z);
    else
      return cproj(z);
  }


  // When removing this deprecated function, be sure to remove chpl_cos and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the cosine of the argument `x`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'cos' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc cos(x: real(64)): real(64) {
    return chpl_cos(x);
  }

  @chpldoc.nodoc
  inline proc chpl_cos(x: real(64)): real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc cos(x: real(64)): real(64);
    return cos(x);
  }

  // When removing this deprecated function, be sure to remove chpl_cos and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the cosine of the argument `x`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'cos' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc cos(x : real(32)): real(32) {
    return chpl_cos(x);
  }

  @chpldoc.nodoc
  inline proc chpl_cos(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc cosf(x: real(32)): real(32);
    return cosf(x);
  }

  // When removing this deprecated function, be sure to remove chpl_cos and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the cosine of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'cos' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc cos(z : complex(64)): complex(64) {
    return chpl_cos(z);
  }

  @chpldoc.nodoc
  inline proc chpl_cos(z : complex(64)): complex(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc ccosf(z: complex(64)): complex(64);
    return ccosf(z);
  }

  // When removing this deprecated function, be sure to remove chpl_cos and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the cosine of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'cos' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc cos(z : complex(128)): complex(128) {
    return chpl_cos(z);
  }

  @chpldoc.nodoc
  inline proc chpl_cos(z : complex(128)): complex(128) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc ccos(z: complex(128)): complex(128);
    return ccos(z);
  }


  // When removing this deprecated function, be sure to remove chpl_cosh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the hyperbolic cosine of the argument `x`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'cosh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc cosh(x: real(64)): real(64) {
    return chpl_cosh(x);
  }

  @chpldoc.nodoc
  inline proc chpl_cosh(x: real(64)): real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc cosh(x: real(64)): real(64);
    return cosh(x);
  }

  // When removing this deprecated function, be sure to remove chpl_cosh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the hyperbolic cosine of the argument `x`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'cosh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc cosh(x : real(32)): real(32) {
    return chpl_cosh(x);
  }

  @chpldoc.nodoc
  inline proc chpl_cosh(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc coshf(x: real(32)): real(32);
    return coshf(x);
  }

  // When removing this deprecated function, be sure to remove chpl_cosh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the hyperbolic cosine of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'cosh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc cosh(z: complex(64)): complex(64) {
    return chpl_cosh(z);
  }

  @chpldoc.nodoc
  inline proc chpl_cosh(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc ccoshf(z: complex(64)): complex(64);
    return ccoshf(z);
  }

  // When removing this deprecated function, be sure to remove chpl_cosh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the hyperbolic cosine of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'cosh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc cosh(z: complex(128)): complex(128) {
    return chpl_cosh(z);
  }

  @chpldoc.nodoc
  inline proc chpl_cosh(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc ccosh(z: complex(128)): complex(128);
    return ccosh(z);
  }


  /* Returns :proc:`ceil`\(`m`/`n`),
     i.e., the fraction `m`/`n` rounded up to the nearest integer.

     If the arguments are of unsigned type, then
     fewer conditionals will be evaluated at run time.
  */
  proc divceil(param m: integral, param n: integral) param do return
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
  proc divceil(m: integral, n: integral) do return
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
  proc divfloor(param m: integral, param n: integral) param do return
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
  proc divfloor(m: integral, n: integral) do return
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

  // When removing this deprecated function, be sure to remove chpl_erf and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'erf' will no longer be included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc erf(x: real(64)): real(64) {
    return chpl_erf(x);
  }

  @chpldoc.nodoc
  inline proc chpl_erf(x: real(64)): real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc erf(x: real(64)): real(64);
    return erf(x);
  }

  // When removing this deprecated function, be sure to remove chpl_erf and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'erf' will no longer be included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc erf(x : real(32)): real(32) {
    return chpl_erf(x);
  }

  @chpldoc.nodoc
  inline proc chpl_erf(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc erff(x: real(32)): real(32);
    return erff(x);
  }

  // When removing this deprecated function, be sure to remove chpl_erfc and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'erfc' will no longer be included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc erfc(x: real(64)): real(64) {
    return chpl_erfc(x);
  }

  @chpldoc.nodoc
  inline proc chpl_erfc(x: real(64)): real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc erfc(x: real(64)): real(64);
    return erfc(x);
  }

  // When removing this deprecated function, be sure to remove chpl_erfc and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'erfc' will no longer be included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc erfc(x : real(32)): real(32) {
    return chpl_erfc(x);
  }

  @chpldoc.nodoc
  inline proc chpl_erfc(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc erfcf(x: real(32)): real(32);
    return erfcf(x);
  }


  /* Returns the value of the Napierian `e` raised to the power of the argument `x`. */
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  extern proc exp(x: real(64)): real(64);

  /* Returns the value of the Napierian `e` raised to the power of the argument. */
  inline proc exp(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc expf(x: real(32)): real(32);
    return expf(x);
  }

  /* Returns the value of the Napierian `e` raised to the power of the argument. */
  inline proc exp(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc cexpf(z: complex(64)): complex(64);
    return cexpf(z);
  }

  /* Returns the value of the Napierian `e` raised to the power of the argument. */
  inline proc exp(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc cexp(z: complex(128)): complex(128);
    return cexp(z);
  }


  /* Returns the value of `2` raised to the power of the argument `x`. */
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  extern proc exp2(x: real(64)): real(64);

  /* Returns the value of `2` raised to the power of the argument `x`. */
  inline proc exp2(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc exp2f(x: real(32)): real(32);
    return exp2f(x);
  }


  /* Returns one less than the value of the Napierian `e` raised to the power
     of the argument `x`. */
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  extern proc expm1(x: real(64)): real(64);

  /* Returns one less than the value of the Napierian `e` raised to the power
     of the argument `x`. */
  inline proc expm1(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc expm1f(x: real(32)): real(32);
    return expm1f(x);
  }


  /* Returns the value of the argument `x` rounded down to the nearest integer. */
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  extern proc floor(x: real(64)): real(64);

  /* Returns the value of the argument `x` rounded down to the nearest integer. */
  inline proc floor(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc floorf(x: real(32)): real(32);
    return floorf(x);
  }


  /* Returns a value for which :proc:`isinf` will return `true`. */
  inline proc INFINITY param : real(64) do return chpl_INFINITY;


  /* Returns `true` if the argument `x` is a representation of a finite value;
     `false` otherwise. */
  inline proc isfinite(x: real(64)): bool do return chpl_macro_double_isfinite(x):bool;

  /* Returns `true` if the argument `x` is a representation of a finite value;
     `false` otherwise. */
  inline proc isfinite(x: real(32)): bool do return chpl_macro_float_isfinite(x):bool;


  /* Returns `true` if the argument `x` is a representation of *infinity*;
     `false` otherwise. */
  inline proc isinf(x: real(64)): bool do return chpl_macro_double_isinf(x):bool;

  /* Returns `true` if the argument `x` is a representation of *infinity*;
     `false` otherwise. */
  inline proc isinf(x: real(32)): bool do return chpl_macro_float_isinf(x):bool;


  /* Returns `true` if the argument `x` does not represent a valid number;
     `false` otherwise. */
  inline proc isnan(x: real(64)): bool do return chpl_macro_double_isnan(x):bool;

  /* Returns `true` if the argument `x` does not represent a valid number;
     `false` otherwise. */
  inline proc isnan(x: real(32)): bool do return chpl_macro_float_isnan(x):bool;

  // When removing this deprecated function, be sure to remove chpl_ldexp and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'ldexp' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc ldexp(x:real(64), n:int(32)):real(64) {
    return chpl_ldexp(x, n);
  }

  @chpldoc.nodoc
  inline proc chpl_ldexp(x:real(64), n:int(32)):real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc ldexp(x:real(64), n:int(32)):real(64);
    return ldexp(x, n);
  }

  // When removing this deprecated function, be sure to remove chpl_ldexp and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'ldexp' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc ldexp(x:real(32), n:int(32)):real(32) {
    return chpl_ldexp(x, n);
  }

  @chpldoc.nodoc
  inline proc chpl_ldexp(x:real(32), n:int(32)):real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc ldexpf(x:real(32), n:int(32)):real(32);
    return ldexpf(x, n);
  }

  // When removing this deprecated function, be sure to remove chpl_lgamma and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'lgamma' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc lgamma(x: real(64)): real(64) {
    return chpl_lgamma(x);
  }

  @chpldoc.nodoc
  inline proc chpl_lgamma(x: real(64)): real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc lgamma(x: real(64)): real(64);
    return lgamma(x);
  }

  // When removing this deprecated function, be sure to remove chpl_lgamma and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'lgamma' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc lgamma(x : real(32)): real(32) {
    return chpl_lgamma(x);
  }

  @chpldoc.nodoc
  inline proc chpl_lgamma(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc lgammaf(x: real(32)): real(32);
    return lgammaf(x);
  }


  /* Returns the natural logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  extern proc log(x: real(64)): real(64);

  /* Returns the natural logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc logf(x: real(32)): real(32);
    return logf(x);
  }

  /* Returns the natural logarithm of the argument `z`. */
  inline proc log(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc clogf(z: complex(64)): complex(64);
    return clogf(z);
  }

  /* Returns the natural logarithm of the argument `z`. */
  inline proc log(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc clog(z: complex(128)): complex(128);
    return clog(z);
  }


  /* Returns the base 10 logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  extern proc log10(x: real(64)): real(64);

  /* Returns the base 10 logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log10(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc log10f(x: real(32)): real(32);
    return log10f(x);
  }

  // To prevent this auto-included module from using a non-auto-included module
  // (Math)
  pragma "no doc"
  inline proc chpl_log1p(x: real(64)): real(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc log1p(x: real(64)): real(64);
    return log1p(x);
  }

  // To prevent this auto-included module from using a non-auto-included module
  // (Math)
  pragma "no doc"
  inline proc chpl_log1p(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc log1pf(x: real(32)): real(32);
    return log1pf(x);
  }

  // When removing this deprecated function, be sure to remove chpl_log1p and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module
  pragma "no doc"
  pragma "last resort"
  @deprecated(notes="log1p is no longer included by default, please 'use' or 'import' the 'Math' module to call it")
  proc log1p(x: real(64)): real(64) {
    return chpl_log1p(x);
  }

  pragma "no doc"
  pragma "last resort"
  @deprecated(notes="log1p is no longer included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc log1p(x : real(32)): real(32) {
    return chpl_log1p(x);
  }

  pragma "no doc"
  pragma "last resort"
  @deprecated(notes="logBasePow2 is no longer included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc logBasePow2(val: int(?w), baseLog2) {
    return chpl_logBasePow2(val, baseLog2);
  }

  pragma "no doc"
  pragma "last resort"
  @deprecated(notes="logBasePow2 is no longer included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc logBasePow2(val: uint(?w), baseLog2) {
    return chpl_logBasePow2(val, baseLog2);
  }

  /* Returns the base 2 logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  extern proc log2(x: real(64)): real(64);

  /* Returns the base 2 logarithm of the argument `x`.

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log2(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc log2f(x: real(32)): real(32);
    return log2f(x);
  }

  private inline proc _logBasePow2Help(in val, baseLog2) {
    // These are used here to avoid including BitOps by default.
    pragma "codegen for CPU and GPU"
    extern proc chpl_bitops_clz_32(x: c_uint) : uint(32);
    pragma "codegen for CPU and GPU"
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

  pragma "no doc"
  inline proc chpl_logBasePow2(val: int(?w), baseLog2) {
    if (val < 1) {
      halt("Can't take the log() of a non-positive integer");
    }
    return _logBasePow2Help(val, baseLog2);
  }

  pragma "no doc"
  inline proc chpl_logBasePow2(val: uint(?w), baseLog2) {
    return _logBasePow2Help(val, baseLog2);
  }

  /* Returns the base 2 logarithm of the argument `x`,
     rounded down.

     :rtype: `int`

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log2(val: int(?w)) {
    return chpl_logBasePow2(val, 1);
  }

  /* Returns the base 2 logarithm of the argument `x`,
     rounded down.

     :rtype: `int`

     It is an error if `x` is less than or equal to zero.
  */
  inline proc log2(val: uint(?w)) {
    return chpl_logBasePow2(val, 1);
  }

  //
  // min and max
  //

  pragma "no doc"
  inline proc max(x: int(?w), y: int(w)) do return if x > y then x else y;
  pragma "no doc"
  inline proc max(x: uint(?w), y: uint(w)) do return if x > y then x else y;
  pragma "no doc"
  inline proc max(x: real(?w), y: real(w)) do return if (x > y) | isnan(x) then x else y;
  pragma "no doc"
  inline proc max(x: int(?w), y: uint(w)) do return if x > y then x:uint(w) else y;
  pragma "no doc"
  inline proc max(x: uint(?w), y: int(w)) do return if x > y then x else y:uint(w);

  pragma "last resort"
  pragma "no doc"
  proc max(x, y) where isAtomicType(x.type) || isAtomicType(y.type) {
    compilerError("min() and max() are not supported for atomic arguments - apply read() to those arguments first");
  }

  /* Returns the maximum value of two arguments using the ``>`` operator
     for comparison.
     If one of the arguments is :proc:`AutoMath.NAN`, the result is also NAN.

     :rtype: The type of `x`.
   */
  inline proc max(x, y)
  where !isArray(x) && !isArray(y) &&
        !(isNumeric(_desync(x.type)) && isNumeric(_desync(y.type))) {
    return if x > y then x else y;
  }
  /* Returns the maximum value of 3 or more arguments using the above call.
   */
  inline proc max(x, y, z...?k) do return max(max(x, y), (...z));
  /* Returns the maximum of 2 param ``int``, ``uint``, ``real``, or ``imag``
     values as a param.
   */
  inline proc max(param x: numeric, param y: numeric) param
    where !(isComplex(x) || isComplex(y)) {
    return if x > y then x else y;
  }

  pragma "no doc"
  inline proc min(x: int(?w), y: int(w)) do return if x < y then x else y;
  pragma "no doc"
  inline proc min(x: uint(?w), y: uint(w)) do return if x < y then x else y;
  pragma "no doc"
  inline proc min(x: real(?w), y: real(w)) do return if (x < y) | isnan(x) then x else y;
  pragma "no doc"
  inline proc min(x: int(?w), y: uint(w)) do return if x < y then x else y:int(w);
  pragma "no doc"
  inline proc min(x: uint(?w), y: int(w)) do return if x < y then x:int(w) else y;

  pragma "last resort"
  pragma "no doc"
  proc min(x, y) where isAtomicType(x.type) || isAtomicType(y.type) {
    compilerError("min() and max() are not supported for atomic arguments - apply read() to those arguments first");
  }


  /* Returns the minimum value of two arguments using the ``<`` operator
     for comparison.

     If one of the arguments is :proc:`AutoMath.NAN`, the result is also NAN.

     :rtype: The type of `x`.
   */
  inline proc min(x, y)
  where !isArray(x) && !isArray(y) &&
        !(isNumeric(_desync(x.type)) && isNumeric(_desync(y.type))) {
    return if x < y then x else y;
  }
  /* Returns the minimum value of 3 or more arguments using the above call.
   */
  inline proc min(x, y, z...?k) do return min(min(x, y), (...z));
  /* Returns the minimum of 2 param ``int``, ``uint``, ``real``, or ``imag``
     values as a param.
   */
  inline proc min(param x: numeric, param y: numeric) param
    where !(isComplex(x) || isComplex(y)) {
    return if x < y then x else y;
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
  inline proc NAN param : real(64) do return chpl_NAN;


  /* Returns the rounded integral value of the argument `x` determined by the
     current rounding direction.  :proc:`nearbyint` will not raise the "inexact"
     floating-point exception.
  */
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  extern proc nearbyint(x: real(64)): real(64);

  /* Returns the rounded integral value of the argument `x` determined by the
     current rounding direction.  :proc:`nearbyint` will not raise the "inexact"
     floating-point exception.
  */
  inline proc nearbyint(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc nearbyintf(x: real(32)): real(32);
    return nearbyintf(x);
  }


  /* Returns the rounded integral value of the argument `x` determined by the
     current rounding direction.  :proc:`rint` may raise the "inexact" floating-point
     exception.
  */
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  extern proc rint(x: real(64)): real(64);

  /* Returns the rounded integral value of the argument `x` determined by the
     current rounding direction.  :proc:`rint` may raise the "inexact" floating-point
     exception.
  */
  inline proc rint(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc rintf(x: real(32)): real(32);
    return rintf(x);
  }


  /* Returns the nearest integral value of the argument `x`, returning that
     value which is larger than `x` in absolute value for the half-way case. */
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  extern proc round(x: real(64)): real(64);

  /* Returns the nearest integral value of the argument `x`, returning that
     value which is larger than `x` in absolute value for the half-way case. */
  inline proc round(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc roundf(x: real(32)): real(32);
    return roundf(x);
  }


  /* Returns the signum function of the integer argument `i`:
     1 if positive, -1 if negative, 0 if zero.
  */
  inline proc sgn(i : int(?w)): int(8) do
    return ((i > 0) : int(8) - (i < 0) : int(8)) : int(8);

  /* Returns the signum function of the unsigned integer argument `i`:
     1 if positive, -1 if negative, 0 if zero.
  */
  inline proc sgn(i : uint(?w)): uint(8) do
    return (i > 0) : uint(8);

  /* Returns the signum function of the integer param argument `i`:
     1 if positive, -1 if negative, 0 if zero.
  */
  proc sgn(param i : integral) param do
    return if i > 0 then 1 else if i == 0 then 0 else -1;

  /* Returns the signum function of the real argument `x`:
     1 if positive, -1 if negative, 0 if zero.
  */
  inline proc sgn(x : real(?w)): int(8) do
    return ((x > 0.0) : int(8) - (x < 0.0) : int(8)) : int(8);


  // When removing this deprecated function, be sure to remove chpl_sin and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the sine of the argument `x`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'sin' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc sin(x: real(64)): real(64) {
    return chpl_sin(x);
  }

  @chpldoc.nodoc
  inline proc chpl_sin(x: real(64)): real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc sin(x: real(64)): real(64);
    return sin(x);
  }

  // When removing this deprecated function, be sure to remove chpl_sin and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the sine of the argument `x`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'sin' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc sin(x: real(32)): real(32) {
    return chpl_sin(x);
  }

  @chpldoc.nodoc
  inline proc chpl_sin(x: real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc sinf(x: real(32)): real(32);
    return sinf(x);
  }

  // When removing this deprecated function, be sure to remove chpl_sin and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the sine of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'sin' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc sin(z: complex(64)): complex(64) {
    return chpl_sin(z);
  }

  @chpldoc.nodoc
  inline proc chpl_sin(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc csinf(z: complex(64)): complex(64);
    return csinf(z);
  }

  // When removing this deprecated function, be sure to remove chpl_sin and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the sine of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'sin' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc sin(z: complex(128)): complex(128) {
    return chpl_sin(z);
  }

  @chpldoc.nodoc
  inline proc chpl_sin(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc csin(z: complex(128)): complex(128);
    return csin(z);
  }


  // When removing this deprecated function, be sure to remove chpl_sinh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the hyperbolic sine of the argument `x`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'sinh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc sinh(x: real(64)): real(64) {
    return chpl_sinh(x);
  }

  @chpldoc.nodoc
  inline proc chpl_sinh(x: real(64)): real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc sinh(x: real(64)): real(64);
    return sinh(x);
  }

  // When removing this deprecated function, be sure to remove chpl_sinh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the hyperbolic sine of the argument `x`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'sinh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc sinh(x : real(32)): real(32) {
    return chpl_sinh(x);
  }

  @chpldoc.nodoc
  inline proc chpl_sinh(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc sinhf(x: real(32)): real(32);
    return sinhf(x);
  }

  // When removing this deprecated function, be sure to remove chpl_sinh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the hyperbolic sine of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'sinh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc sinh(z: complex(64)): complex(64) {
    return chpl_sinh(z);
  }

  @chpldoc.nodoc
  inline proc chpl_sinh(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc csinhf(z: complex(64)): complex(64);
    return csinhf(z);
  }

  // When removing this deprecated function, be sure to remove chpl_sinh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the hyperbolic sine of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'sinh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc sinh(z: complex(128)): complex(128) {
    return chpl_sinh(z);
  }

  @chpldoc.nodoc
  inline proc chpl_sinh(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc csinh(z: complex(128)): complex(128);
    return csinh(z);
  }


  /* Returns the square root of the argument `x`.

     It is an error if the `x` is less than zero.
  */
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  extern proc sqrt(x: real(64)): real(64);

  /* Returns the square root of the argument `x`.

     It is an error if  `x` is less than zero.
  */
  inline proc sqrt(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc sqrtf(x: real(32)): real(32);
    return sqrtf(x);
  }

  /* Returns the square root of the argument `z`. */
  inline proc sqrt(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc csqrtf(z: complex(64)): complex(64);
    return csqrtf(z);
  }

  /* Returns the square root of the argument `z`. */
  inline proc sqrt(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc csqrt(z: complex(128)): complex(128);
    return csqrt(z);
  }


  // When removing this deprecated function, be sure to remove chpl_tan and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the tangent of the argument `x`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'tan' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc tan(x: real(64)): real(64) {
    return chpl_tan(x);
  }

  @chpldoc.nodoc
  inline proc chpl_tan(x: real(64)): real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc tan(x: real(64)): real(64);
    return tan(x);
  }

  // When removing this deprecated function, be sure to remove chpl_tan and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the tangent of the argument `x`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'tan' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc tan(x : real(32)): real(32) {
    return chpl_tan(x);
  }

  @chpldoc.nodoc
  inline proc chpl_tan(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc tanf(x: real(32)): real(32);
    return tanf(x);
  }

  // When removing this deprecated function, be sure to remove chpl_tan and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the tangent of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'tan' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc tan(z: complex(64)): complex(64) {
    return chpl_tan(z);
  }

  @chpldoc.nodoc
  inline proc chpl_tan(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc ctanf(z: complex(64)): complex(64);
    return ctanf(z);
  }

  // When removing this deprecated function, be sure to remove chpl_tan and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the tangent of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'tan' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc tan(z: complex(128)): complex(128) {
    return chpl_tan(z);
  }

  @chpldoc.nodoc
  inline proc chpl_tan(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc ctan(z: complex(128)): complex(128);
    return ctan(z);
  }


  // When removing this deprecated function, be sure to remove chpl_tanh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the hyperbolic tangent of the argument `x`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'tanh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc tanh(x: real(64)): real(64) {
    return chpl_tanh(x);
  }

  @chpldoc.nodoc
  inline proc chpl_tanh(x: real(64)): real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc tanh(x: real(64)): real(64);
    return tanh(x);
  }

  // When removing this deprecated function, be sure to remove chpl_tanh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the hyperbolic tangent of the argument `x`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'tanh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc tanh(x : real(32)): real(32) {
    return chpl_tanh(x);
  }

  @chpldoc.nodoc
  inline proc chpl_tanh(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc tanhf(x: real(32)): real(32);
    return tanhf(x);
  }

  // When removing this deprecated function, be sure to remove chpl_tanh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the hyperbolic tangent of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'tanh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc tanh(z: complex(64)): complex(64) {
    return chpl_tanh(z);
  }

  @chpldoc.nodoc
  inline proc chpl_tanh(z: complex(64)): complex(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc ctanhf(z: complex(64)): complex(64);
    return ctanhf(z);
  }

  // When removing this deprecated function, be sure to remove chpl_tanh and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  /* Returns the hyperbolic tangent of the argument `z`. */
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'tanh' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc tanh(z: complex(128)): complex(128) {
    return chpl_tanh(z);
  }

  @chpldoc.nodoc
  inline proc chpl_tanh(z: complex(128)): complex(128) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc ctanh(z: complex(128)): complex(128);
    return ctanh(z);
  }

  // When removing this deprecated function, be sure to remove chpl_tgamma and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'tgamma' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc tgamma(x: real(64)): real(64) {
    return chpl_tgamma(x);
  }

  @chpldoc.nodoc
  inline proc chpl_tgamma(x: real(64)): real(64) {
    // Note: this extern proc was originally free standing.  It might be
    // reasonable to make it that way again when the deprecated version is
    // removed
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc tgamma(x: real(64)): real(64);
    return tgamma(x);
  }

  // When removing this deprecated function, be sure to remove chpl_tgamma and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module.
  pragma "last resort"
  @deprecated(notes="In an upcoming release 'tgamma' will no longer be included by default, please 'use' or 'import' the :mod:`Math` module to call it")
  inline proc tgamma(x : real(32)): real(32) {
    return chpl_tgamma(x);
  }

  @chpldoc.nodoc
  inline proc chpl_tgamma(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc tgammaf(x: real(32)): real(32);
    return tgammaf(x);
  }


  /* Returns the nearest integral value to the argument `x` that is not larger
     than `x` in absolute value. */
  pragma "fn synchronization free"
  pragma "codegen for CPU and GPU"
  extern proc trunc(x: real(64)): real(64);

  /* Returns the nearest integral value to the argument `x` that is not larger
     than `x` in absolute value. */
  inline proc trunc(x : real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
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

  /* Returns true if `x` and `y` are approximately equal, else returns false. */
  inline proc isclose(x, y, rtol = 1e-5, atol = 0.0): bool {
    if boundsChecking && (rtol < 0) then
      HaltWrappers.boundsCheckHalt("Input value for rtol must be positive");
    if boundsChecking && (atol < 0) then
      HaltWrappers.boundsCheckHalt("Input value for atol must be positive");
    var diff: real = abs(x-y);
    return ( (diff<=abs(rtol*y)) || (diff<=abs(rtol*x)) || (diff<=atol) );
  }

  pragma "no doc"
  inline proc chpl_j0(x: real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_float_j0(x: real(32)): real(32);
    return chpl_float_j0(x);
  }

  pragma "no doc"
  inline proc chpl_j0(x: real(64)): real(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc j0(x: real(64)): real(64);
    return j0(x);
  }

  // When removing this deprecated function, be sure to remove chpl_j0 and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module
  pragma "no doc"
  pragma "last resort"
  @deprecated(notes="j0 will soon stop being included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc j0(x: real(32)): real(32) {
    return chpl_j0(x);
  }

  // When removing this deprecated function, be sure to remove chpl_j0 and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module
  pragma "no doc"
  pragma "last resort"
  @deprecated(notes="j0 will soon stop being included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc j0(x: real(64)): real(64) {
    return chpl_j0(x);
  }

  pragma "no doc"
  inline proc chpl_j1(x: real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_float_j1(x: real(32)): real(32);
    return chpl_float_j1(x);
  }

  pragma "no doc"
  inline proc chpl_j1(x: real(64)): real(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc j1(x: real(64)): real(64);
    return j1(x);
  }

  // When removing this deprecated function, be sure to remove chpl_j1 and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module
  pragma "no doc"
  pragma "last resort"
  @deprecated(notes="j1 will soon stop being included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc j1(x: real(32)): real(32) {
    return chpl_j1(x);
  }

  // When removing this deprecated function, be sure to remove chpl_j1 and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module
  pragma "no doc"
  pragma "last resort"
  @deprecated(notes="j1 will soon stop being included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc j1(x: real(64)): real(64) {
    return chpl_j1(x);
  }

  pragma "no doc"
  inline proc chpl_jn(n: int, x: real(32)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_float_jn(n: c_int, x: real(32)): real(32);
    return chpl_float_jn(n.safeCast(c_int), x);
  }

  pragma "no doc"
  inline proc chpl_jn(n: int, x: real(64)): real(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc jn(n: c_int, x: real(64)): real(64);
    return jn(n.safeCast(c_int), x);
  }

  // When removing this deprecated function, be sure to remove chpl_jn and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module
  pragma "no doc"
  pragma "last resort"
  @deprecated(notes="jn will soon stop being included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc jn(n: int, x: real(32)): real(32) {
    return chpl_jn(n, x);
  }

  // When removing this deprecated function, be sure to remove chpl_jn and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module
  pragma "no doc"
  pragma "last resort"
  @deprecated(notes="jn will soon stop being included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc jn(n: int, x: real(64)): real(64) {
    return chpl_jn(n, x);
  }

  pragma "no doc"
  inline proc chpl_y0(x: real(32)): real(32) {
    if boundsChecking && x < 0 then
      HaltWrappers.boundsCheckHalt("Input value for y0() must be non-negative");

    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_float_y0(x: real(32)): real(32);
    return chpl_float_y0(x);
  }

  pragma "no doc"
  inline proc chpl_y0(x: real(64)): real(64) {
    if boundsChecking && x < 0 then
      HaltWrappers.boundsCheckHalt("Input value for y0() must be non-negative");

    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc y0(x: real(64)): real(64);
    return y0(x);
  }

  // When removing this deprecated function, be sure to remove chpl_y0 and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module
  pragma "no doc"
  pragma "last resort"
  @deprecated(notes="y0 will soon stop being included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc y0(x: real(32)): real(32) {
    return chpl_y0(x);
  }

  // When removing this deprecated function, be sure to remove chpl_y0 and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module
  pragma "no doc"
  pragma "last resort"
  @deprecated(notes="y0 will soon stop being included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc y0(x: real(64)): real(64) {
    return chpl_y0(x);
  }

  pragma "no doc"
  inline proc chpl_y1(x: real(32)): real(32) {
    if boundsChecking && x < 0 then
      HaltWrappers.boundsCheckHalt("Input value for y1() must be non-negative");

    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_float_y1(x: real(32)): real(32);
    return chpl_float_y1(x);
  }

  pragma "no doc"
  inline proc chpl_y1(x: real(64)): real(64) {
    if boundsChecking && x < 0 then
      HaltWrappers.boundsCheckHalt("Input value for y1() must be non-negative");

    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc y1(x: real(64)): real(64);
    return y1(x);
  }

  // When removing this deprecated function, be sure to remove chpl_y1 and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module
  pragma "no doc"
  pragma "last resort"
  @deprecated(notes="y1 will soon stop being included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc y1(x: real(32)): real(32) {
    return chpl_y1(x);
  }

  // When removing this deprecated function, be sure to remove chpl_y1 and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module
  pragma "no doc"
  pragma "last resort"
  @deprecated(notes="y1 will soon stop being included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc y1(x: real(64)): real(64) {
    return chpl_y1(x);
  }

  pragma "no doc"
  inline proc chpl_yn(n: int, x: real(32)): real(32) {
    if boundsChecking && x < 0 then
      HaltWrappers.boundsCheckHalt("Input value for yn() must be non-negative");

    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_float_yn(n: c_int, x: real(32)): real(32);
    return chpl_float_yn(n.safeCast(c_int), x);
  }

  pragma "no doc"
  inline proc chpl_yn(n: int, x: real(64)): real(64) {
    if boundsChecking && x < 0 then
      HaltWrappers.boundsCheckHalt("Input value for yn() must be non-negative");

    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc yn(n: c_int, x: real(64)): real(64);
    return yn(n.safeCast(c_int), x);
  }

  // When removing this deprecated function, be sure to remove chpl_yn and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module
  pragma "no doc"
  pragma "last resort"
  @deprecated(notes="yn will soon stop being included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc yn(n: int, x: real(32)): real(32) {
    return chpl_yn(n, x);
  }

  // When removing this deprecated function, be sure to remove chpl_yn and
  // move its contents into Math.chpl to reduce the symbols living in this
  // module
  pragma "no doc"
  pragma "last resort"
  @deprecated(notes="yn will soon stop being included by default, please 'use' or 'import' the 'Math' module to call it")
  inline proc yn(n: int, x: real(64)): real(64) {
    return chpl_yn(n, x);
  }

  /* Returns true if the sign of `x` is negative, else returns false. It detects
     the sign bit of zeroes, infinities, and NANs */
  inline proc signbit(x : real(32)): bool {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_macro_float_signbit(x: real(32)): c_int;
    return chpl_macro_float_signbit(x): bool;
  }

  /* Returns true if the sign of `x` is negative, else returns false. It detects
     the sign bit of zeroes, infinities, and NANs */
  inline proc signbit(x : real(64)): bool {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_macro_double_signbit(x: real(64)): c_int;
    return chpl_macro_double_signbit(x): bool;
  }

} // end of module AutoMath

// TODO: Consolidate overloaded signatures, to simplify the documentation.
