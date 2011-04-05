_extern const INFINITY: real(64);
_extern const NAN: real(64);

pragma "inline" proc abs(i : int(?w)) return if i < 0 then -i else i;
pragma "inline" proc abs(i : uint(?w)) return i;
pragma "inline" proc abs(x : complex(?w)) return sqrt(x.re*x.re + x.im*x.im);

pragma "inline" proc conjg(a: complex(?w)) return (a.re, -a.im):complex;

_extern proc isinf(x: real(64)): bool;
_extern proc isinf(x: real(32)): bool;
_extern proc isfinite(x: real(64)): bool;
_extern proc isfinite(x: real(32)): bool;
_extern proc isnan(x: real(64)): bool;
_extern proc isnan(x: real(32)): bool;

_extern proc acos(x: real(64)): real(64);
_extern proc acosh(x: real(64)): real(64);
_extern proc asin(x: real(64)): real(64);
_extern proc asinh(x: real(64)): real(64);
_extern proc atan(x: real(64)): real(64);
_extern proc atan2(y: real(64), x: real(64)): real(64);
_extern proc atanh(x: real(64)): real(64);
_extern proc cbrt(x: real(64)): real(64);
_extern proc ceil(x: real(64)): real(64);
_extern proc cos(x: real(64)): real(64);
_extern proc cosh(x: real(64)): real(64);
_extern proc erf(x: real(64)): real(64);
_extern proc erfc(x: real(64)): real(64);
_extern proc exp(x: real(64)): real(64);
_extern proc exp2(x: real(64)): real(64);
_extern proc expm1(x: real(64)): real(64);
_extern proc fabs(x: real(64)): real(64);
_extern proc floor(x: real(64)): real(64);
_extern proc lgamma(x: real(64)): real(64);
_extern proc log(x: real(64)): real(64);
_extern proc log2(x: real(64)): real(64);
_extern proc log10(x: real(64)): real(64);
_extern proc log1p(x: real(64)): real(64);
_extern proc nearbyint(x: real(64)): real(64);
_extern proc pow(x: real(64), y: real(64)): real(64);
_extern proc rint(x: real(64)): real(64);
_extern proc round(x: real(64)): real(64);
_extern proc sin(x: real(64)): real(64);
_extern proc sinh(x: real(64)): real(64);
_extern proc sqrt(x: real(64)): real(64);
_extern proc tan(x: real(64)): real(64);
_extern proc tanh(x: real(64)): real(64);
_extern proc tgamma(x: real(64)): real(64);
_extern proc trunc(x: real(64)): real(64);

pragma "inline" proc acos(x : real(32)) {
  _extern proc acosf(x: real(32)): real(32);
  return acosf(x);
}
pragma "inline" proc acosh(x : real(32)) {
  _extern proc acoshf(x: real(32)): real(32);
  return acoshf(x);
}
pragma "inline" proc asin(x : real(32)) {
  _extern proc asinf(x: real(32)): real(32);
  return asinf(x);
}
pragma "inline" proc asinh(x : real(32)) {
  _extern proc asinhf(x: real(32)): real(32);
  return asinhf(x);
}
pragma "inline" proc atan(x : real(32)) {
  _extern proc atanf(x: real(32)): real(32);
  return atanf(x);
}
pragma "inline" proc atan2(y : real(32), x: real(32)) {
  _extern proc atan2f(y: real(32), x: real(32)): real(32);
  return atan2f(y, x);
}
pragma "inline" proc atanh(x : real(32)) {
  _extern proc atanhf(x: real(32)): real(32);
  return atanhf(x);
}
pragma "inline" proc cbrt(x : real(32)) {
  _extern proc cbrtf(x: real(32)): real(32);
  return cbrtf(x);
}
pragma "inline" proc ceil(x : real(32)) {
  _extern proc ceilf(x: real(32)): real(32);
  return ceilf(x);
}
pragma "inline" proc cos(x : real(32)) {
  _extern proc cosf(x: real(32)): real(32);
  return cosf(x);
}
pragma "inline" proc cosh(x : real(32)) {
  _extern proc coshf(x: real(32)): real(32);
  return coshf(x);
}
pragma "inline" proc erf(x : real(32)) {
  _extern proc erff(x: real(32)): real(32);
  return erff(x);
}
pragma "inline" proc erfc(x : real(32)) {
  _extern proc erfcf(x: real(32)): real(32);
  return erfcf(x);
}
pragma "inline" proc exp(x : real(32)) {
  _extern proc expf(x: real(32)): real(32);
  return expf(x);
}
pragma "inline" proc exp2(x : real(32)) {
  _extern proc exp2f(x: real(32)): real(32);
  return exp2f(x);
}
pragma "inline" proc expm1(x : real(32)) {
  _extern proc expm1f(x: real(32)): real(32);
  return expm1f(x);
}
pragma "inline" proc abs(x : real(32)) {
  _extern proc fabsf(x: real(32)): real(32);
  return fabsf(x);
}
pragma "inline" proc floor(x : real(32)) {
  _extern proc floorf(x: real(32)): real(32);
  return floorf(x);
}
pragma "inline" proc lgamma(x : real(32)) {
  _extern proc lgammaf(x: real(32)): real(32);
  return lgammaf(x);
}
pragma "inline" proc log(x : real(32)) {
  _extern proc logf(x: real(32)): real(32);
  return logf(x);
}
pragma "inline" proc log2(x : real(32)) {
  _extern proc log2f(x: real(32)): real(32);
  return log2f(x);
}
pragma "inline" proc log10(x : real(32)) {
  _extern proc log10f(x: real(32)): real(32);
  return log10f(x);
}
pragma "inline" proc log1p(x : real(32)) {
  _extern proc log1pf(x: real(32)): real(32);
  return log1pf(x);
}
pragma "inline" proc nearbyint(x : real(32)) {
  _extern proc nearbyintf(x: real(32)): real(32);
  return nearbyintf(x);
}
pragma "inline" proc rint(x : real(32)) {
  _extern proc rintf(x: real(32)): real(32);
  return rintf(x);
}
pragma "inline" proc round(x : real(32)) {
  _extern proc roundf(x: real(32)): real(32);
  return roundf(x);
}
pragma "inline" proc sin(x: real(32)) {
  _extern proc sinf(x: real(32)): real(32);
  return sinf(x);
}
pragma "inline" proc sinh(x : real(32)) {
  _extern proc sinhf(x: real(32)): real(32);
  return sinhf(x);
}
pragma "inline" proc sqrt(x : real(32)): real(32) {
  _extern proc sqrtf(x: real(32)): real(32);
  return sqrtf(x);
}
pragma "inline" proc tan(x : real(32)) {
  _extern proc tanf(x: real(32)): real(32);
  return tanf(x);
}
pragma "inline" proc tanh(x : real(32)) {
  _extern proc tanhf(x: real(32)): real(32);
  return tanhf(x);
}
pragma "inline" proc tgamma(x : real(32)) {
  _extern proc tgammaf(x: real(32)): real(32);
  return tgammaf(x);
}
pragma "inline" proc trunc(x : real(32)) {
  _extern proc truncf(x: real(32)): real(32);
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
