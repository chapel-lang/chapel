/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

Automatically included Math symbols

.. _automath-roots:

Roots
^^^^^
:proc:`~Math.cbrt`
:proc:`~Math.sqrt`

.. _automath-rounding:

Rounding
^^^^^^^^
:proc:`~Math.ceil`
:proc:`~Math.floor`
:proc:`~Math.round`
:proc:`~Math.trunc`

.. _automath-complex:

Computations Involving Complex Numbers
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
:proc:`~Math.conj`
:proc:`~Math.phase`
:proc:`~Math.riemProj`

.. _automath-inf-nan:

Infinity and NaN
^^^^^^^^^^^^^^^^
:proc:`~Math.inf`
:proc:`~Math.nan`
:proc:`~Math.isFinite`
:proc:`~Math.isInf`
:proc:`~Math.isNan`

.. _automath-comparison:

Comparison Functions
^^^^^^^^^^^^^^^^^^^^
:proc:`~Math.max`
:proc:`~Math.min`
:proc:`~Math.isClose`

.. _automath-sign:

Sign Functions
^^^^^^^^^^^^^^
:proc:`~Math.sgn`
:proc:`~Math.signbit`

.. _automath-other:

Remaining Functions
^^^^^^^^^^^^^^^^^^^
:proc:`~Math.abs`
:proc:`~Math.mod`
*/
pragma "module included by default"
@unstable("The module name 'AutoMath' is unstable.  If you want to use qualified naming on the symbols within it, please 'use' or 'import' the :mod:`Math` module")
module AutoMath {
  import HaltWrappers;
  private use CTypes;

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

     :rtype: The type of `x`.
  */
  inline proc abs(x : int(?w)) do return if x < 0 then -x else x;

  /* Returns the absolute value of the unsigned integer argument.

     :rtype: The type of `x`.
  */
  inline proc abs(x : uint(?w)) do return x;

  /* Returns the absolute value of the integer param argument `x`. */
  proc abs(param x : integral) param do return if x < 0 then -x else x;

  /* Returns the magnitude of the real argument `x`. */
  inline proc abs(x : real(64)):real(64) do return __primitive("abs", x);

  /* Return the absolute value of a param real(64) as a param */
  proc abs(param x: real(64)) param :real(64) {
    return __primitive("abs", x);
  }

  /* Returns the magnitude of the real argument `x`. */
  inline proc abs(x : real(32)): real(32) do return __primitive("abs", x);

  /* Return the absolute value of a param real(32) as a param */
  proc abs(param x: real(32)) param : real(32) {
    return __primitive("abs", x);
  }

  /* Returns the real magnitude of the imaginary argument `x`. */
  inline proc abs(x : imag(64)): real(64) do return __primitive("abs", _i2r(x));

  /* Return the real magnitude of a `param` imaginary argument `x` as a `param`
  */
  proc abs(param x: imag(64)) param :real(64) {
    return abs(x:real(64));
  }

  /* Returns the real magnitude of the imaginary argument `x`. */
  inline proc abs(x: imag(32)): real(32) do return __primitive("abs", _i2r(x));

  /* Return the real magnitude of a `param` imaginary argument `x` as a `param`
  */
  proc abs(param x: imag(32)) param :real(32) {
    return abs(x:real(32));
  }

  /* Returns the magnitude (often called modulus) of complex `x`.

     In concert with the related :proc:`phase` (a.k.a. argument)
     of `x`, it can be used to recompute `x`.
  */
  inline proc abs(x : complex(128)): real(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_cabs(x: complex(128)): real(64);
    return chpl_cabs(x);
  }

  /* Returns the magnitude of a ``param`` ``complex(128)``.
  */
  proc abs(param x : complex(128)) param :real(64) {
    return __primitive("abs", x);
  }

  /* Returns the magnitude (often called modulus) of complex `x`.

     In concert with the related :proc:`phase` (a.k.a. argument)
     of `x`, it can be used to recompute `x`.
  */
  inline proc abs(x : complex(64)): real(32) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_cabsf(x: complex(64)): real(32);
    return chpl_cabsf(x);
  }

  /* Returns the magnitude of a ``param`` ``complex(64)``.
  */
  proc abs(param x : complex(64)) param :real(32) {
    return __primitive("abs", x);
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

  /* Returns the complex conjugate of the complex argument `x`.

     :rtype: A complex number of the same type as `x`.
  */
  inline proc conj(x: complex(?w)) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_conjf(x: complex(64)): complex(64);
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_conj(x: complex(128)): complex(128);
    if w == 64 then
      return chpl_conjf(x);
    else
      return chpl_conj(x);
  }

  /* Returns the complex conjugate of the imaginary argument `x`.

     :rtype: An imaginary number of the same type as `x`.
  */
  inline proc conj(x: imag(?w)) {
    return -x;
  }

  /* Returns the argument `x`.

     :rtype: A number that is not complex or imaginary of the same type as `x`.
  */
  inline proc conj(x: int(?w)) {
    return x;
  }

  inline proc conj(x: uint(?w)) {
    return x;
  }

  inline proc conj(x: real(?w)) {
    return x;
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

  /* Returns a value for which :proc:`isInf` will return `true`. */
  inline proc inf param : real(64) do return chpl_INFINITY;

  /* Returns `true` if the argument `x` is a representation of a finite value;
     `false` otherwise. */
  inline proc isFinite(x: real(64)): bool do return chpl_macro_double_isfinite(x):bool;

  /* Returns `true` if the argument `x` is a representation of a finite value;
     `false` otherwise. */
  inline proc isFinite(x: real(32)): bool do return chpl_macro_float_isfinite(x):bool;

  /* Returns `true` if the argument `x` is a representation of *infinity*;
     `false` otherwise. */
  inline proc isInf(x: real(64)): bool do return chpl_macro_double_isinf(x):bool;

  /* Returns `true` if the argument `x` is a representation of *infinity*;
     `false` otherwise. */
  inline proc isInf(x: real(32)): bool do return chpl_macro_float_isinf(x):bool;

  /* Returns `true` if the argument `x` does not represent a valid number;
     `false` otherwise. */
  inline proc isNan(x: real(64)): bool do return chpl_macro_double_isnan(x):bool;

  /* Returns `true` if the argument `x` does not represent a valid number;
     `false` otherwise. */
  inline proc isNan(x: real(32)): bool do return chpl_macro_float_isnan(x):bool;

  //
  // min and max
  //

  @chpldoc.nodoc
  inline proc max(x: int(8), y: int(8)) do return if x > y then x else y;
  @chpldoc.nodoc
  inline proc max(x: int(16), y: int(16)) do return if x > y then x else y;
  @chpldoc.nodoc
  inline proc max(x: int(32), y: int(32)) do return if x > y then x else y;
  @chpldoc.nodoc
  inline proc max(x: int(64), y: int(64)) do return if x > y then x else y;

  @chpldoc.nodoc
  inline proc max(x: uint(8), y: uint(8)) do return if x > y then x else y;
  @chpldoc.nodoc
  inline proc max(x: uint(16), y: uint(16)) do return if x > y then x else y;
  @chpldoc.nodoc
  inline proc max(x: uint(32), y: uint(32)) do return if x > y then x else y;
  @chpldoc.nodoc
  inline proc max(x: uint(64), y: uint(64)) do return if x > y then x else y;

  @chpldoc.nodoc
  inline proc max(x: real(32), y: real(32)) do return if (x > y) || isNan(x) then x else y;
  @chpldoc.nodoc
  inline proc max(x: real(64), y: real(64)) do return if (x > y) || isNan(x) then x else y;

  @chpldoc.nodoc
  inline proc max(x: int(8), y: uint(8)) do return if x > y then x : uint(8) else y;
  @chpldoc.nodoc
  inline proc max(x: int(16), y: uint(16)) do return if x > y then x : uint(16) else y;
  @chpldoc.nodoc
  inline proc max(x: int(32), y: uint(32)) do return if x > y then x : uint(32) else y;
  @chpldoc.nodoc
  inline proc max(x: int(64), y: uint(64)) do return if x > y then x : uint(64) else y;

  @chpldoc.nodoc
  inline proc max(x: uint(8), y: int(8)) do return if x > y then x else y : uint(8);
  @chpldoc.nodoc
  inline proc max(x: uint(16), y: int(16)) do return if x > y then x else y : uint(16);
  @chpldoc.nodoc
  inline proc max(x: uint(32), y: int(32)) do return if x > y then x else y : uint(32);
  @chpldoc.nodoc
  inline proc max(x: uint(64), y: int(64)) do return if x > y then x else y : uint(64);

  pragma "last resort"
  @chpldoc.nodoc
  proc max(x, y) where isAtomicType(x.type) || isAtomicType(y.type) {
    compilerError("min() and max() are not supported for atomic arguments - apply read() to those arguments first");
  }

  /* Returns the maximum value of two arguments using the ``>`` operator
     for comparison.
     If one of the arguments is :proc:`Math.nan`, the result is also nan.

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

  @chpldoc.nodoc
  inline proc min(x: int(8), y: int(8)) do return if x < y then x else y;
  @chpldoc.nodoc
  inline proc min(x: int(16), y: int(16)) do return if x < y then x else y;
  @chpldoc.nodoc
  inline proc min(x: int(32), y: int(32)) do return if x < y then x else y;
  @chpldoc.nodoc
  inline proc min(x: int(64), y: int(64)) do return if x < y then x else y;

  @chpldoc.nodoc
  inline proc min(x: uint(8), y: uint(8)) do return if x < y then x else y;
  @chpldoc.nodoc
  inline proc min(x: uint(16), y: uint(16)) do return if x < y then x else y;
  @chpldoc.nodoc
  inline proc min(x: uint(32), y: uint(32)) do return if x < y then x else y;
  @chpldoc.nodoc
  inline proc min(x: uint(64), y: uint(64)) do return if x < y then x else y;

  @chpldoc.nodoc
  inline proc min(x: real(32), y: real(32)) do return if (x < y) || isNan(x) then x else y;
  @chpldoc.nodoc
  inline proc min(x: real(64), y: real(64)) do return if (x < y) || isNan(x) then x else y;

  @chpldoc.nodoc
  inline proc min(x: int(8), y: uint(8)) do return if x < y then x else y : int(8);
  @chpldoc.nodoc
  inline proc min(x: int(16), y: uint(16)) do return if x < y then x else y : int(16);
  @chpldoc.nodoc
  inline proc min(x: int(32), y: uint(32)) do return if x < y then x else y : int(32);
  @chpldoc.nodoc
  inline proc min(x: int(64), y: uint(64)) do return if x < y then x else y : int(64);

  @chpldoc.nodoc
  inline proc min(x: uint(8), y: int(8)) do return if x < y then x : int(8) else y;
  @chpldoc.nodoc
  inline proc min(x: uint(16), y: int(16)) do return if x < y then x : int(16) else y;
  @chpldoc.nodoc
  inline proc min(x: uint(32), y: int(32)) do return if x < y then x : int(32) else y;
  @chpldoc.nodoc
  inline proc min(x: uint(64), y: int(64)) do return if x < y then x : int(64) else y;

  pragma "last resort"
  @chpldoc.nodoc
  proc min(x, y) where isAtomicType(x.type) || isAtomicType(y.type) {
    compilerError("min() and max() are not supported for atomic arguments - apply read() to those arguments first");
  }


  /* Returns the minimum value of two arguments using the ``<`` operator
     for comparison.

     If one of the arguments is :proc:`Math.nan`, the result is also nan.

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
     ``mod(x,y) = x - y * floor(x / y)``.

     The result is always >= 0 if `y` > 0.
     It is an error if `y` == 0.

     .. note::
        This does not have the same behavior as the :ref:`Modulus_Operators` (%)
        when `y` is negative.
  */
  proc mod(param x: integral, param y: integral) param {
    param temp = x % y;

    // verbatim copy from the other 'mod', to simplify maintenance
    return
      if isNonnegative(y) then
        if isUintType(x.type)
        then temp
        else ( if temp >= 0 then temp else temp + y )
      else
        // y < 0
        ( if temp <= 0 then temp else temp + y );
  }

  /* Computes the mod operator on the two arguments, defined as
     ``mod(x,y) = x - y * floor(x / y)``.

     If the arguments are of unsigned type, then
     fewer conditionals will be evaluated at run time.

     The result is always >= 0 if `y` > 0.
     It is an error if `y` == 0.

     .. note::
        This does not have the same behavior as the :ref:`Modulus_Operators` (%)
        when `y` is negative.
  */
  proc mod(x: integral, y: integral) {
    const temp = x % y;

    // eliminate some run-time tests if input(s) is(are) unsigned
    return
      if isNonnegative(y) then
        if isUintType(x.type)
        then temp
        else ( if temp >= 0 then temp else temp + y )
      else
        // y < 0
        ( if temp <= 0 then temp else temp + y );
  }

  /* Computes the mod operator on the two numbers, defined as
     ``mod(x,y) = x - y * floor(x / y)``.
  */
  proc mod(x: real(32), y: real(32)): real(32) {
    // This codes up the standard definition, according to Wikipedia.
    // Is there a more efficient implementation for reals?
    return x - y*floor(x/y);
  }

  /* Computes the mod operator on the two numbers, defined as
     ``mod(x,y) = x - y * floor(x / y)``.
  */
  proc mod(x: real(64), y: real(64)): real(64) {
    // This codes up the standard definition, according to Wikipedia.
    // Is there a more efficient implementation for reals?
    return x - y*floor(x/y);
  }

  /* Returns a value for which :proc:`isNan` will return `true`. */
  inline proc nan param : real(64) do return chpl_NAN;

  /* Returns the phase (often called `argument`) of complex `x`, an angle (in
     radians).

     In concert with the related :proc:`abs`, the magnitude (a.k.a.
     modulus) of `x`, it can be used to recompute `x`.

     :rtype: ``real(w/2)`` when `x` has a type of ``complex(w)``.
  */
  inline proc phase(x: complex(?w)): real(w/2) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_cargf(x: complex(64)): real(32);
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_carg(x: complex(128)): real(64);
    if w == 64 then
      return chpl_cargf(x);
    else
      return chpl_carg(x);
  }

  /* Returns the projection of `x` on a Riemann sphere. */
  inline proc riemProj(x: complex(?w)): complex(w) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_cprojf(x: complex(64)): complex(64);
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_cproj(x: complex(128)): complex(128);
    if w == 64 then
      return chpl_cprojf(x);
    else
      return chpl_cproj(x);
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

  /* Returns the signum function of the integer argument `x`:
     1 if positive, -1 if negative, 0 if zero.
  */
  @unstable("sgn is unstable and may change its name and return type in the future")
  inline proc sgn(x : int(?w)): int(8) do
    return ((x > 0) : int(8) - (x < 0) : int(8)) : int(8);

  /* Returns the signum function of the unsigned integer argument `x`:
     1 if positive, -1 if negative, 0 if zero.
  */
  @unstable("sgn is unstable and may change its name and return type in the future")
  inline proc sgn(x : uint(?w)): uint(8) do
    return (x > 0) : uint(8);

  /* Returns the signum function of the integer param argument `x`:
     1 if positive, -1 if negative, 0 if zero.
  */
  @unstable("sgn is unstable and may change its name and return type in the future")
  proc sgn(param x : integral) param do
    return if x > 0 then 1 else if x == 0 then 0 else -1;

  /* Returns the signum function of the real argument `x`:
     1 if positive, -1 if negative, 0 if zero.
  */
  @unstable("sgn is unstable and may change its name and return type in the future")
  inline proc sgn(x : real(?w)): int(8) do
    return ((x > 0.0) : int(8) - (x < 0.0) : int(8)) : int(8);

  /* Returns the square root of the argument `x`.

     It is an error if the `x` is less than zero.
  */
  inline proc sqrt(x: real(64)): real(64) do return __primitive("sqrt", x);

  /* Returns the square root of the argument `x`.

     It is an error if the `x` is less than zero.
  */
  proc sqrt(param x: real(64)) param :real(64) {
    if x < 0 {
      compilerError("sqrt of a negative number -- cast to complex if a complex result is desired");
    }
    return __primitive("sqrt", x);
  }

  /* Returns the square root of the argument `x`.

     It is an error if  `x` is less than zero.
  */
  inline proc sqrt(x : real(32)): real(32) do return __primitive("sqrt", x);

  /* Returns the square root of the argument `x`.

     It is an error if  `x` is less than zero.
  */
  proc sqrt(param x: real(32)) param :real(32) {
    if x < 0 {
      compilerError("sqrt of a negative number -- cast to complex if a complex result is desired");
    }
    return __primitive("sqrt", x);
  }

  /* Returns the square root of the argument `x`. */
  inline proc sqrt(x: complex(64)): complex(64) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_csqrtf(x: complex(64)): complex(64);
    return chpl_csqrtf(x);
  }

  /* Returns the square root of the argument `x`. */
  proc sqrt(param x: complex(64)) param : complex(64) {
    return __primitive("sqrt", x);
  }

  /* Returns the square root of the argument `x`. */
  inline proc sqrt(x: complex(128)): complex(128) {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_csqrt(x: complex(128)): complex(128);
    return chpl_csqrt(x);
  }

  /* Returns the square root of the argument `x`. */
  proc sqrt(param x: complex(128)) param : complex(128) {
    return __primitive("sqrt", x);
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

  /* Returns true if `x` and `y` are approximately equal, else returns false.

     `relTol` specifies the relative tolerance for differences between `x` and
     `y`, while `absTol` specifies the absolute tolerance.  Both must be
     positive when specified.

     `x` and `y` must be either `real`, `imag`, or `complex`.
   */
  inline proc isClose(x, y, relTol = 1e-5, absTol = 0.0): bool {
    if !((isRealValue(x) || isImagValue(x) || isComplexValue(x)) &&
         (isRealValue(y) || isImagValue(y) || isComplexValue(y))) {
      if (isArrayValue(x) || isArrayValue(y)) {
        compilerError("'isClose' does not support promotion, please call it with the individual values");
      } else {
        compilerError("x and y must be either 'real', 'imag', or 'complex', x was '" + x.type: string + "' and y was '" + y.type: string + "'");
      }
    }

    if boundsChecking && (relTol < 0) then
      HaltWrappers.boundsCheckHalt("Input value for relTol must be positive");
    if boundsChecking && (absTol < 0) then
      HaltWrappers.boundsCheckHalt("Input value for absTol must be positive");
    var diff: real = abs(x-y);
    return ( (diff<=abs(relTol*y)) || (diff<=abs(relTol*x)) || (diff<=absTol) );
  }

  /* Returns true if the sign of `x` is negative, else returns false. It detects
     the sign bit of zeroes, infinities, and nans */
  @unstable("signbit is unstable and may change its name in the future")
  inline proc signbit(x : real(32)): bool {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_macro_float_signbit(x: real(32)): c_int;
    return chpl_macro_float_signbit(x): bool;
  }

  /* Returns true if the sign of `x` is negative, else returns false. It detects
     the sign bit of zeroes, infinities, and nans */
  @unstable("signbit is unstable and may change its name in the future")
  inline proc signbit(x : real(64)): bool {
    pragma "fn synchronization free"
    pragma "codegen for CPU and GPU"
    extern proc chpl_macro_double_signbit(x: real(64)): c_int;
    return chpl_macro_double_signbit(x): bool;
  }

} // end of module AutoMath

// TODO: Consolidate overloaded signatures, to simplify the documentation.
