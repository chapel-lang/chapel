.. default-domain:: chpl

.. module:: Math
   :synopsis: This module provides mathematical constants and functions.

Math
====
**Usage**

.. code-block:: chapel

   use Math;


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


.. data:: param e = 2.71828

   e - exp(1) or  the base of the natural logarithm 

.. data:: param log2_e = 1.4427

   log2(e) 

.. data:: param log10_e = 0.434294

   log10(e) 

.. data:: param ln_2 = 0.693147

   log(2) (natural logarithm) 

.. data:: param ln_10 = 2.30259

   log(10) (natural logarithm) 

.. data:: param pi = 3.14159

   pi - the circumference/the diameter of a circle 

.. data:: param half_pi = 1.5708

   pi/2 

.. data:: param quarter_pi = 0.785398

   pi/4 

.. data:: param recipr_pi = 0.31831

   1/pi 

.. data:: param twice_recipr_pi = 0.63662

   2/pi 

.. data:: param twice_recipr_sqrt_pi = 1.12838

   2/sqrt(pi) 

.. data:: param sqrt_2 = 1.41421

   sqrt(2) 

.. data:: param recipr_sqrt_2 = 0.707107

   1/sqrt(2) 

.. function:: proc abs(i: int(?w))

   Returns the absolute value of the integer argument.
   
   :rtype: The type of `i`.
   

.. function:: proc abs(i: uint(?w))

   Returns the absolute value of the unsigned integer argument.
   
   :rtype: The type of `i`.
   

.. function:: proc abs(param i: integral) param

   Returns the absolute value of the integer param argument `i`. 

.. function:: proc abs(r: real(64)): real(64)

   Returns the magnitude of the real argument `r`. 

.. function:: proc abs(x: real(32)): real(32)

   Returns the magnitude of the real argument `x`. 

.. function:: proc abs(im: imag(64)): real(64)

   Returns the real magnitude of the imaginary argument `im`. 

.. function:: proc abs(im: imag(32)): real(32)

   Returns the real magnitude of the imaginary argument `im`. 

.. function:: proc abs(z: complex(?w)): real(w/2)

   Returns the real magnitude of the complex argument `z`.
   
   :rtype: The type of the real component of the argument (== `w`/2).
   

.. function:: proc carg(z: complex(?w)): real(w/2)

   Returns the real phase angle of complex argument `z`. 

.. function:: proc acos(x: real(64)): real(64)

   Returns the arc cosine of the argument `x`.
   
   It is an error if `x` is less than -1 or greater than 1.
   

.. function:: proc acos(x: real(32)): real(32)

   Returns the arc cosine of the argument `x`.
   
   It is an error if `x` is less than -1 or greater than 1.
   

.. function:: proc acos(z: complex(64)): complex(64)

   Returns the arc cosine of the argument `z`. 

.. function:: proc acos(z: complex(128)): complex(128)

   Returns the arc cosine of the argument `z`. 

.. function:: proc acosh(x: real(64)): real(64)

   Returns the inverse hyperbolic cosine of the argument `x`.
   
   It is an error if `x` is less than 1.
   

.. function:: proc acosh(x: real(32)): real(32)

   Returns the inverse hyperbolic cosine of the argument `x`.
   
   It is an error if `x` is less than 1.
   

.. function:: proc acosh(z: complex(64)): complex(64)

   Returns the inverse hyperbolic cosine of the argument `z`. 

.. function:: proc acosh(z: complex(128)): complex(128)

   Returns the inverse hyperbolic cosine of the argument `z`. 

.. function:: proc asin(x: real(64)): real(64)

   Returns the arc sine of the argument `x`.
   
   It is an error if `x` is less than -1 or greater than 1.
   

.. function:: proc asin(x: real(32)): real(32)

   Returns the arc sine of the argument `x`.
   
   It is an error if `x` is less than -1 or greater than 1.
   

.. function:: proc asin(z: complex(64)): complex(64)

   Returns the arc sine of the argument `z`. 

.. function:: proc asin(z: complex(128)): complex(128)

   Returns the arc sine of the argument `z`. 

.. function:: proc asinh(x: real(64)): real(64)

   Returns the inverse hyperbolic sine of the argument `x`. 

.. function:: proc asinh(x: real(32)): real(32)

   Returns the inverse hyperbolic sine of the argument `x`. 

.. function:: proc asinh(z: complex(64)): complex(64)

   Returns the inverse hyperbolic sine of the argument `z`. 

.. function:: proc asinh(z: complex(128)): complex(128)

   Returns the inverse hyperbolic sine of the argument `z`. 

.. function:: proc atan(x: real(64)): real(64)

   Returns the arc tangent of the argument `x`. 

.. function:: proc atan(x: real(32)): real(32)

   Returns the arc tangent of the argument `x`. 

.. function:: proc atan(z: complex(64)): complex(64)

   Returns the arc tangent of the argument `z`. 

.. function:: proc atan(z: complex(128)): complex(128)

   Returns the arc tangent of the argument `z`. 

.. function:: proc atan2(y: real(64), x: real(64)): real(64)

   Returns the arc tangent of the ratio of the two arguments.
   
   This is equivalent to
   the arc tangent of `y` / `x` except that the signs of `y`
   and `x` are used to determine the quadrant of the result. 

.. function:: proc atan2(y: real(32), x: real(32)): real(32)

   Returns the arc tangent of the two arguments.
   
   This is equivalent to
   the arc tangent of `y` / `x` except that the signs of `y`
   and `x` are used to determine the quadrant of the result. 

.. function:: proc atanh(x: real(64)): real(64)

   Returns the inverse hyperbolic tangent of the argument `x`.
   
   It is an error if `x` is less than -1 or greater than 1. 

.. function:: proc atanh(x: real(32)): real(32)

   Returns the inverse hyperbolic tangent of the argument `x`.
   
   It is an error if `x` is less than -1 or greater than 1. 

.. function:: proc atanh(z: complex(64)): complex(64)

   Returns the inverse hyperbolic tangent of the argument `z`. 

.. function:: proc atanh(z: complex(128)): complex(128)

   Returns the inverse hyperbolic tangent of the argument `z`. 

.. function:: proc cbrt(x: real(64)): real(64)

   Returns the cube root of the argument `x`. 

.. function:: proc cbrt(x: real(32)): real(32)

   Returns the cube root of the argument `x`. 

.. function:: proc ceil(x: real(64)): real(64)

   Returns the value of the argument `x` rounded up to the nearest integer. 

.. function:: proc ceil(x: real(32)): real(32)

   Returns the value of the argument `x` rounded up to the nearest integer. 

.. function:: proc conjg(z: complex(?w))

   Returns the complex conjugate of the complex argument `z`.
   
   :rtype: A complex number of the same type as `z`.
   

.. function:: proc conjg(z: imag(?w))

   Returns the complex conjugate of the imaginary argument `z`.
   
   :rtype: An imaginary number of the same type as `z`.
   

.. function:: proc conjg(z: int(?w))

   Returns the argument `z`.
   
   :rtype: A number that is not complex or imaginary of the same type as `z`.
   

.. function:: proc conjg(z: uint(?w))

.. function:: proc conjg(z: real(?w))

.. function:: proc cproj(z: complex(?w)): real(w/2)

   Returns the projection of `z` on a Riemann sphere. 

.. function:: proc cos(x: real(64)): real(64)

   Returns the cosine of the argument `x`. 

.. function:: proc cos(x: real(32)): real(32)

   Returns the cosine of the argument `x`. 

.. function:: proc cos(z: complex(64)): complex(64)

   Returns the cosine of the argument `z`. 

.. function:: proc cos(z: complex(128)): complex(128)

   Returns the cosine of the argument `z`. 

.. function:: proc cosh(x: real(64)): real(64)

   Returns the hyperbolic cosine of the argument `x`. 

.. function:: proc cosh(x: real(32)): real(32)

   Returns the hyperbolic cosine of the argument `x`. 

.. function:: proc cosh(z: complex(64)): complex(64)

   Returns the hyperbolic cosine of the argument `z`. 

.. function:: proc cosh(z: complex(128)): complex(128)

   Returns the hyperbolic cosine of the argument `z`. 

.. function:: proc divceil(param m: integral, param n: integral) param

   Returns :proc:`ceil`\(`m`/`n`),
   i.e., the fraction `m`/`n` rounded up to the nearest integer.
   
   If the arguments are of unsigned type, then
   fewer conditionals will be evaluated at run time.
   

.. function:: proc divceil(m: integral, n: integral)

   Returns :proc:`ceil`\(`m`/`n`),
   i.e., the fraction `m`/`n` rounded up to the nearest integer.
   
   If the arguments are of unsigned type, then
   fewer conditionals will be evaluated at run time.
   

.. function:: proc divceilpos(m: integral, n: integral)

   
   A variant of :proc:`divceil` that performs no runtime checks.
   The user must ensure that both arguments are strictly positive
   (not 0) and are of a signed integer type (not `uint`).
   

.. function:: proc divfloor(param m: integral, param n: integral) param

   Returns :proc:`floor`\(`m`/`n`),
   i.e., the fraction `m`/`n` rounded down to the nearest integer.
   
   If the arguments are of unsigned type, then
   fewer conditionals will be evaluated at run time.
   

.. function:: proc divfloor(m: integral, n: integral)

   Returns :proc:`floor`\(`m`/`n`),
   i.e., the fraction `m`/`n` rounded down to the nearest integer.
   
   If the arguments are of unsigned type, then
   fewer conditionals will be evaluated at run time.
   

.. function:: proc divfloorpos(m: integral, n: integral)

   
   A variant of :proc:`divfloor` that performs no runtime checks.
   The user must ensure that both arguments are strictly positive
   (not 0) and are of a signed integer type (not `uint`).
   

.. function:: proc erf(x: real(64)): real(64)

   Returns the error function of the argument `x`. 

.. function:: proc erf(x: real(32)): real(32)

   Returns the error function of the argument `x`. 

.. function:: proc erfc(x: real(64)): real(64)

   Returns the complementary error function of the argument.
   This is equivalent to 1.0 - :proc:`erf`\(`x`).
   

.. function:: proc erfc(x: real(32)): real(32)

   Returns the complementary error function of the argument.
   This is equivalent to 1.0 - :proc:`erf`\(`x`).
   

.. function:: proc exp(x: real(64)): real(64)

   Returns the value of the Napierian `e` raised to the power of the argument `x`. 

.. function:: proc exp(x: real(32)): real(32)

   Returns the value of the Napierian `e` raised to the power of the argument. 

.. function:: proc exp(z: complex(64)): complex(64)

   Returns the value of the Napierian `e` raised to the power of the argument. 

.. function:: proc exp(z: complex(128)): complex(128)

   Returns the value of the Napierian `e` raised to the power of the argument. 

.. function:: proc exp2(x: real(64)): real(64)

   Returns the value of `2` raised to the power of the argument `x`. 

.. function:: proc exp2(x: real(32)): real(32)

   Returns the value of `2` raised to the power of the argument `x`. 

.. function:: proc expm1(x: real(64)): real(64)

   Returns one less than the value of the Napierian `e` raised to the power
   of the argument `x`. 

.. function:: proc expm1(x: real(32)): real(32)

   Returns one less than the value of the Napierian `e` raised to the power
   of the argument `x`. 

.. function:: proc floor(x: real(64)): real(64)

   Returns the value of the argument `x` rounded down to the nearest integer. 

.. function:: proc floor(x: real(32)): real(32)

   Returns the value of the argument `x` rounded down to the nearest integer. 

.. function:: proc INFINITY param: real(64)

   Returns a value for which :proc:`isinf` will return `true`. 

.. function:: proc isfinite(x: real(64)): bool

   Returns `true` if the argument `x` is a representation of a finite value;
   `false` otherwise. 

.. function:: proc isfinite(x: real(32)): bool

   Returns `true` if the argument `x` is a representation of a finite value;
   `false` otherwise. 

.. function:: proc isinf(x: real(64)): bool

   Returns `true` if the argument `x` is a representation of *infinity*;
   `false` otherwise. 

.. function:: proc isinf(x: real(32)): bool

   Returns `true` if the argument `x` is a representation of *infinity*;
   `false` otherwise. 

.. function:: proc isnan(x: real(64)): bool

   Returns `true` if the argument `x` does not represent a valid number;
   `false` otherwise. 

.. function:: proc isnan(x: real(32)): bool

   Returns `true` if the argument `x` does not represent a valid number;
   `false` otherwise. 

.. function:: proc ldexp(x: real(64), n: int(32)): real(64)

   Multiply by an integer power of 2.
   Returns x * 2**n.
   

.. function:: proc ldexp(x: real(32), n: int(32)): real(32)

.. function:: proc lgamma(x: real(64)): real(64)

   Returns the natural logarithm of the absolute value
   of the gamma function of the argument `x`.
   

.. function:: proc lgamma(x: real(32)): real(32)

   Returns the natural logarithm of the absolute value
   of the gamma function of the argument `x`.
   

.. function:: proc log(x: real(64)): real(64)

   Returns the natural logarithm of the argument `x`.
   
   It is an error if `x` is less than or equal to zero.
   

.. function:: proc log(x: real(32)): real(32)

   Returns the natural logarithm of the argument `x`.
   
   It is an error if `x` is less than or equal to zero.
   

.. function:: proc log(z: complex(64)): complex(64)

   Returns the natural logarithm of the argument `z`. 

.. function:: proc log(z: complex(128)): complex(128)

   Returns the natural logarithm of the argument `z`. 

.. function:: proc log10(x: real(64)): real(64)

   Returns the base 10 logarithm of the argument `x`.
   
   It is an error if `x` is less than or equal to zero.
   

.. function:: proc log10(x: real(32)): real(32)

   Returns the base 10 logarithm of the argument `x`.
   
   It is an error if `x` is less than or equal to zero.
   

.. function:: proc log1p(x: real(64)): real(64)

   Returns the natural logarithm of `x` + 1.
   
   It is an error if `x` is less than or equal to -1.
   

.. function:: proc log1p(x: real(32)): real(32)

   Returns the natural logarithm of `x` + 1.
   
   It is an error if `x` is less than or equal to -1.
   

.. function:: proc logBasePow2(val: int(?w), baseLog2)

   Returns the log to the base `2**baseLog2` of the given `in` value.
   If `baseLog2` is `1`, then returns the log to the base `2`;
   if `baseLog2` is `2`, then returns the log to the base `4`, etc.
   Any fractional part is discarded.
   
   :rtype: `int`
   

.. function:: proc logBasePow2(val: uint(?w), baseLog2)

   Returns the log to the base `2**baseLog2` of the given `in` value.
   If `baseLog2` is `1`, then returns the log to the base `2`;
   if `baseLog2` is `2`, then returns the log to the base `4`, etc.
   Any fractional part is discarded.
   
   :rtype: `int`
   

.. function:: proc log2(x: real(64)): real(64)

   Returns the base 2 logarithm of the argument `x`.
   
   It is an error if `x` is less than or equal to zero.
   

.. function:: proc log2(x: real(32)): real(32)

   Returns the base 2 logarithm of the argument `x`.
   
   It is an error if `x` is less than or equal to zero.
   

.. function:: proc log2(val: int(?w))

   Returns the base 2 logarithm of the argument `x`,
   rounded down.
   
   :rtype: `int`
   
   It is an error if `x` is less than or equal to zero.
   

.. function:: proc log2(val: uint(?w))

   Returns the base 2 logarithm of the argument `x`,
   rounded down.
   
   :rtype: `int`
   
   It is an error if `x` is less than or equal to zero.
   

.. function:: proc mod(param m: integral, param n: integral) param

   Computes the mod operator on the two arguments, defined as
   ``mod(m,n) = m - n * floor(m / n)``.
   
   The result is always >= 0 if `n` > 0.
   It is an error if `n` == 0.
   

.. function:: proc mod(m: integral, n: integral)

   Computes the mod operator on the two arguments, defined as
   ``mod(m,n) = m - n * floor(m / n)``.
   
   If the arguments are of unsigned type, then
   fewer conditionals will be evaluated at run time.
   
   The result is always >= 0 if `n` > 0.
   It is an error if `n` == 0.
   

.. function:: proc mod(x: real(?w), y: real(w)): real(w)

   Computes the mod operator on the two numbers, defined as
   ``mod(x,y) = x - y * floor(x / y)``.
   
   The return value has the same type as `x`.
   

.. function:: proc NAN param: real(64)

   Returns a value for which :proc:`isnan` will return `true`. 

.. function:: proc nearbyint(x: real(64)): real(64)

   Returns the rounded integral value of the argument `x` determined by the
   current rounding direction.  :proc:`nearbyint` will not raise the "inexact"
   floating-point exception.
   

.. function:: proc nearbyint(x: real(32)): real(32)

   Returns the rounded integral value of the argument `x` determined by the
   current rounding direction.  :proc:`nearbyint` will not raise the "inexact"
   floating-point exception.
   

.. function:: proc rint(x: real(64)): real(64)

   Returns the rounded integral value of the argument `x` determined by the
   current rounding direction.  :proc:`rint` may raise the "inexact" floating-point
   exception.
   

.. function:: proc rint(x: real(32)): real(32)

   Returns the rounded integral value of the argument `x` determined by the
   current rounding direction.  :proc:`rint` may raise the "inexact" floating-point
   exception.
   

.. function:: proc round(x: real(64)): real(64)

   Returns the rounded integral value of the argument `x`. 

.. function:: proc round(x: real(32)): real(32)

   Returns the rounded integral value of the argument `x`. 

.. function:: proc sgn(i: int(?w)): int(8)

   Returns the signum function of the integer argument `i`:
   1 if positive, -1 if negative, 0 if zero.
   

.. function:: proc sgn(i: uint(?w)): uint(8)

   Returns the signum function of the unsigned integer argument `i`:
   1 if positive, -1 if negative, 0 if zero.
   

.. function:: proc sgn(param i: integral) param

   Returns the signum function of the integer param argument `i`:
   1 if positive, -1 if negative, 0 if zero.
   

.. function:: proc sgn(x: real(?w)): int(8)

   Returns the signum function of the real argument `x`:
   1 if positive, -1 if negative, 0 if zero.
   

.. function:: proc sin(x: real(64)): real(64)

   Returns the sine of the argument `x`. 

.. function:: proc sin(x: real(32)): real(32)

   Returns the sine of the argument `x`. 

.. function:: proc sin(z: complex(64)): complex(64)

   Returns the sine of the argument `z`. 

.. function:: proc sin(z: complex(128)): complex(128)

   Returns the sine of the argument `z`. 

.. function:: proc sinh(x: real(64)): real(64)

   Returns the hyperbolic sine of the argument `x`. 

.. function:: proc sinh(x: real(32)): real(32)

   Returns the hyperbolic sine of the argument `x`. 

.. function:: proc sinh(z: complex(64)): complex(64)

   Returns the hyperbolic sine of the argument `z`. 

.. function:: proc sinh(z: complex(128)): complex(128)

   Returns the hyperbolic sine of the argument `z`. 

.. function:: proc sqrt(x: real(64)): real(64)

   Returns the square root of the argument `x`.
   
   It is an error if the `x` is less than zero.
   

.. function:: proc sqrt(x: real(32)): real(32)

   Returns the square root of the argument `x`.
   
   It is an error if  `x` is less than zero.
   

.. function:: proc sqrt(z: complex(64)): complex(64)

   Returns the square root of the argument `z`. 

.. function:: proc sqrt(z: complex(128)): complex(128)

   Returns the square root of the argument `z`. 

.. function:: proc tan(x: real(64)): real(64)

   Returns the tangent of the argument `x`. 

.. function:: proc tan(x: real(32)): real(32)

   Returns the tangent of the argument `x`. 

.. function:: proc tan(z: complex(64)): complex(64)

   Returns the tangent of the argument `z`. 

.. function:: proc tan(z: complex(128)): complex(128)

   Returns the tangent of the argument `z`. 

.. function:: proc tanh(x: real(64)): real(64)

   Returns the hyperbolic tangent of the argument `x`. 

.. function:: proc tanh(x: real(32)): real(32)

   Returns the hyperbolic tangent of the argument `x`. 

.. function:: proc tanh(z: complex(64)): complex(64)

   Returns the hyperbolic tangent of the argument `z`. 

.. function:: proc tanh(z: complex(128)): complex(128)

   Returns the hyperbolic tangent of the argument `z`. 

.. function:: proc tgamma(x: real(64)): real(64)

   Returns the absolute value of the gamma function of the argument `x`. 

.. function:: proc tgamma(x: real(32)): real(32)

   Returns the absolute value of the gamma function of the argument `x`. 

.. function:: proc trunc(x: real(64)): real(64)

   Returns the nearest integral value to the argument `x` that is not larger
   than `x` in absolute value. 

.. function:: proc trunc(x: real(32)): real(32)

   Returns the nearest integral value to the argument `x` that is not larger
   than `x` in absolute value. 

.. function:: proc gcd(in a: int, in b: int): int

   Returns the greatest common divisor of the integer argument `a` and
   `b`. 

.. function:: proc isclose(x, y, rtol = 1e-05, atol = 0.0): bool

   Returns true if `x` and `y` are approximately equal, else returns false. 

.. function:: proc j0(x: real(32)): real(32)

   Returns the Bessel function of the first kind of order `0` of `x`. 

.. function:: proc j0(x: real(64)): real(64)

   Returns the Bessel function of the first kind of order `0` of `x`. 

.. function:: proc j1(x: real(32)): real(32)

   Returns the Bessel function of the first kind of order `1` of `x`. 

.. function:: proc j1(x: real(64)): real(64)

   Returns the Bessel function of the first kind of order `1` of `x`. 

.. function:: proc jn(n: int, x: real(32)): real(32)

   Returns the Bessel function of the first kind of order `n` of `x`. 

.. function:: proc jn(n: int, x: real(64)): real(64)

   Returns the Bessel function of the first kind of order `n` of `x`. 

.. function:: proc y0(x: real(32)): real(32)

   Returns the Bessel function of the second kind of order `0` of `x`, where
   `x` must be greater than 0 

.. function:: proc y0(x: real(64)): real(64)

   Returns the Bessel function of the second kind of order `0` of `x`,
   where `x` must be greater than 0 

.. function:: proc y1(x: real(32)): real(32)

   Returns the Bessel function of the second kind of order `1` of `x`,
   where `x` must be greater than 0 

.. function:: proc y1(x: real(64)): real(64)

   Returns the Bessel function of the second kind of order `1` of `x`,
   where `x` must be greater than 0 

.. function:: proc yn(n: int, x: real(32)): real(32)

   Returns the Bessel function of the second kind of order `n` of `x`,
   where `x` must be greater than 0 

.. function:: proc yn(n: int, x: real(64)): real(64)

   Returns the Bessel function of the second kind of order `n` of `x`,
   where `x` must be greater than 0 

.. function:: proc signbit(x: real(32)): bool

   Returns true if the sign of `x` is negative, else returns false. It detects
   the sign bit of zeroes, infinities, and NANs 

.. function:: proc signbit(x: real(64)): bool

   Returns true if the sign of `x` is negative, else returns false. It detects
   the sign bit of zeroes, infinities, and NANs 

