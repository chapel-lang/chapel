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

extern proc chpl_macro_INFINITY():real(64);
extern proc chpl_macro_NAN():real(64);

inline proc INFINITY return chpl_macro_INFINITY();
inline proc NAN return chpl_macro_NAN();

inline proc abs(i : int(?w)) return if i < 0 then -i else i;
inline proc abs(i : uint(?w)) return i;
proc abs(param i : integral) param return if i < 0 then -i else i;
inline proc abs(r : real) return fabs(r);
inline proc abs(im : imag) return fabs(_i2r(im));
inline proc abs(x : complex(?w)) return sqrt(x.re*x.re + x.im*x.im);

inline proc sgn(i : int(?w))
  return ((i > 0) : int(8) - (i < 0) : int(8)) : int(8);
inline proc sgn(i : uint(?w))
  return (i > 0) : uint(8);
proc sgn(param i : integral) param
  return if i > 0 then 1 else if i == 0 then 0 else -1;
inline proc sgn(x : real(?w))
  return ((x > 0.0) : int(8) - (x < 0.0) : int(8)) : int(8);

inline proc conjg(a: complex(?w)) return (a.re, -a.im):complex;

extern proc chpl_macro_double_isinf(x: real(64)): c_int;
extern proc chpl_macro_float_isinf(x: real(32)): c_int;
extern proc chpl_macro_double_isfinite(x: real(64)): c_int;
extern proc chpl_macro_float_isfinite(x: real(32)): c_int;
extern proc chpl_macro_double_isnan(x: real(64)): c_int;
extern proc chpl_macro_float_isnan(x: real(32)): c_int;
inline proc isinf(x: real(64)) return chpl_macro_double_isinf(x):bool;
inline proc isinf(x: real(32)) return chpl_macro_float_isinf(x):bool;
inline proc isfinite(x: real(64)) return chpl_macro_double_isfinite(x):bool;
inline proc isfinite(x: real(32)) return chpl_macro_float_isfinite(x):bool;
inline proc isnan(x: real(64)) return chpl_macro_double_isnan(x):bool;
inline proc isnan(x: real(32)) return chpl_macro_float_isnan(x):bool;

extern proc acos(x: real(64)): real(64);
extern proc acosh(x: real(64)): real(64);
extern proc asin(x: real(64)): real(64);
extern proc asinh(x: real(64)): real(64);
extern proc atan(x: real(64)): real(64);
extern proc atan2(y: real(64), x: real(64)): real(64);
extern proc atanh(x: real(64)): real(64);
extern proc cbrt(x: real(64)): real(64);
extern proc ceil(x: real(64)): real(64);
extern proc cos(x: real(64)): real(64);
extern proc cosh(x: real(64)): real(64);
extern proc erf(x: real(64)): real(64);
extern proc erfc(x: real(64)): real(64);
extern proc exp(x: real(64)): real(64);
extern proc exp2(x: real(64)): real(64);
extern proc expm1(x: real(64)): real(64);
extern proc fabs(x: real(64)): real(64);
extern proc floor(x: real(64)): real(64);
extern proc lgamma(x: real(64)): real(64);
extern proc log(x: real(64)): real(64);
extern proc log2(x: real(64)): real(64);
extern proc log10(x: real(64)): real(64);
extern proc log1p(x: real(64)): real(64);
extern proc nearbyint(x: real(64)): real(64);
extern proc rint(x: real(64)): real(64);
extern proc round(x: real(64)): real(64);
extern proc sin(x: real(64)): real(64);
extern proc sinh(x: real(64)): real(64);
extern proc sqrt(x: real(64)): real(64);
extern proc tan(x: real(64)): real(64);
extern proc tanh(x: real(64)): real(64);
extern proc tgamma(x: real(64)): real(64);
extern proc trunc(x: real(64)): real(64);

inline proc abs(im: imag(32)) {
  extern proc fabsf(x: real(32)): real(32);
  return fabsf(_i2r(im));
}

inline proc abs(x : real(32)) {
  extern proc fabsf(x: real(32)): real(32);
  return fabsf(x);
}
inline proc acos(x : real(32)) {
  extern proc acosf(x: real(32)): real(32);
  return acosf(x);
}
inline proc acosh(x : real(32)) {
  extern proc acoshf(x: real(32)): real(32);
  return acoshf(x);
}
inline proc asin(x : real(32)) {
  extern proc asinf(x: real(32)): real(32);
  return asinf(x);
}
inline proc asinh(x : real(32)) {
  extern proc asinhf(x: real(32)): real(32);
  return asinhf(x);
}
inline proc atan(x : real(32)) {
  extern proc atanf(x: real(32)): real(32);
  return atanf(x);
}
inline proc atan2(y : real(32), x: real(32)) {
  extern proc atan2f(y: real(32), x: real(32)): real(32);
  return atan2f(y, x);
}
inline proc atanh(x : real(32)) {
  extern proc atanhf(x: real(32)): real(32);
  return atanhf(x);
}
inline proc cbrt(x : real(32)) {
  extern proc cbrtf(x: real(32)): real(32);
  return cbrtf(x);
}
inline proc ceil(x : real(32)) {
  extern proc ceilf(x: real(32)): real(32);
  return ceilf(x);
}
inline proc cos(x : real(32)) {
  extern proc cosf(x: real(32)): real(32);
  return cosf(x);
}
inline proc cosh(x : real(32)) {
  extern proc coshf(x: real(32)): real(32);
  return coshf(x);
}
inline proc erf(x : real(32)) {
  extern proc erff(x: real(32)): real(32);
  return erff(x);
}
inline proc erfc(x : real(32)) {
  extern proc erfcf(x: real(32)): real(32);
  return erfcf(x);
}
inline proc exp(x : real(32)) {
  extern proc expf(x: real(32)): real(32);
  return expf(x);
}
inline proc exp2(x : real(32)) {
  extern proc exp2f(x: real(32)): real(32);
  return exp2f(x);
}
inline proc expm1(x : real(32)) {
  extern proc expm1f(x: real(32)): real(32);
  return expm1f(x);
}
inline proc floor(x : real(32)) {
  extern proc floorf(x: real(32)): real(32);
  return floorf(x);
}
inline proc lgamma(x : real(32)) {
  extern proc lgammaf(x: real(32)): real(32);
  return lgammaf(x);
}
inline proc log(x : real(32)) {
  extern proc logf(x: real(32)): real(32);
  return logf(x);
}
inline proc log2(x : real(32)) {
  extern proc log2f(x: real(32)): real(32);
  return log2f(x);
}
inline proc log10(x : real(32)) {
  extern proc log10f(x: real(32)): real(32);
  return log10f(x);
}
inline proc log1p(x : real(32)) {
  extern proc log1pf(x: real(32)): real(32);
  return log1pf(x);
}
inline proc nearbyint(x : real(32)) {
  extern proc nearbyintf(x: real(32)): real(32);
  return nearbyintf(x);
}
inline proc rint(x : real(32)) {
  extern proc rintf(x: real(32)): real(32);
  return rintf(x);
}
inline proc round(x : real(32)) {
  extern proc roundf(x: real(32)): real(32);
  return roundf(x);
}
inline proc sin(x: real(32)) {
  extern proc sinf(x: real(32)): real(32);
  return sinf(x);
}
inline proc sinh(x : real(32)) {
  extern proc sinhf(x: real(32)): real(32);
  return sinhf(x);
}
inline proc sqrt(x : real(32)): real(32) {
  extern proc sqrtf(x: real(32)): real(32);
  return sqrtf(x);
}
inline proc tan(x : real(32)) {
  extern proc tanf(x: real(32)): real(32);
  return tanf(x);
}
inline proc tanh(x : real(32)) {
  extern proc tanhf(x: real(32)): real(32);
  return tanhf(x);
}
inline proc tgamma(x : real(32)) {
  extern proc tgammaf(x: real(32)): real(32);
  return tgammaf(x);
}
inline proc trunc(x : real(32)) {
  extern proc truncf(x: real(32)): real(32);
  return truncf(x);
}

proc _logBasePow2Help(in val, baseLog2) {
  var result = -1;
  while (val != 0) {
    val >>= baseLog2;
    result += 1;
  }
  return result;
}

proc logBasePow2(in val: int(?w), baseLog2) {
  if (val < 1) {
    halt("Can't take the log() of a non-positive integer");
  }
  return _logBasePow2Help(val, baseLog2);
}

proc logBasePow2(in val: uint(?w), baseLog2) {
  if (val < 1) {
    halt("Can't take the log() of a non-positive integer");
  }
  return _logBasePow2Help(val, baseLog2);
}

proc log2(in val: int(?w)) {
  return logBasePow2(val, 1);
}

proc log2(in val: uint(?w)) {
  return logBasePow2(val, 1);
}

// Performance note: if argument(s) is(are) of unsigned type(s),
// fewer condititionals will be evaluated at run time.
proc divceil(m: integral, n: integral) return
  if isNonnegative(m) then
    if isNonnegative(n) then (m + n - 1) / n
    else                     m / n
  else
    if isNonnegative(n) then m / n
    else                     (m + n + 1) / n;

proc divceil(param m: integral, param n: integral) param return
  if isNonnegative(m) then
    if isNonnegative(n) then (m + n - 1) / n
    else                     m / n
  else
    if isNonnegative(n) then m / n
    else                     (m + n + 1) / n;

// Performance note: if argument(s) is(are) of unsigned type(s),
// fewer condititionals will be evaluated at run time.
proc divfloor(m: integral, n: integral) return
  if isNonnegative(m) then
    if isNonnegative(n) then m / n
    else                     (m - n - 1) / n
  else
    if isNonnegative(n) then (m - n + 1) / n
    else                     m / n;

proc divfloor(param m: integral, param n: integral) param return
  if isNonnegative(m) then
    if isNonnegative(n) then m / n
    else                     (m - n - 1) / n
  else
    if isNonnegative(n) then (m - n + 1) / n
    else                     m / n;

// This codes up the standard definition, according to Wikipedia.
// Is there a more efficient implementation for reals?
proc mod(x: real(?w), y: real(w)): real(w) {
  return x - y*floor(x/y);
}

// Performance note: if argument(s) is(are) of unsigned type(s),
// fewer condititionals will be evaluated at run time.
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
