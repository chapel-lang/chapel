pragma "inline" def abs(i : int(?w)) return if i < 0 then -i else i;
pragma "inline" def abs(i : uint(?w)) return i;
pragma "inline" def abs(x : complex(?w)) return sqrt(x.re*x.re + x.im*x.im);

pragma "inline" def conjg(a: complex(?w)) return (a.re, -a.im):complex;

pragma "inline" def isnan(x: real(64)) {
  _extern def isnan(x: real(64)): bool;
  return isnan(x);
}
pragma "inline" def acos(x : real(64)) {
  _extern def acos(x: real(64)): real(64);
  return acos(x);
}
pragma "inline" def acosh(x : real(64)) {
  _extern def acosh(x: real(64)): real(64);
  return acosh(x);
}
pragma "inline" def asin(x : real(64)) {
  _extern def asin(x: real(64)): real(64);
  return asin(x);
}
pragma "inline" def asinh(x : real(64)) {
  _extern def asinh(x: real(64)): real(64);
  return asinh(x);
}
pragma "inline" def atan(x : real(64)) {
  _extern def atan(x: real(64)): real(64);
  return atan(x);
}
pragma "inline" def atan2(y: real(64), x: real(64)) {
  _extern def atan2(y: real(64), x: real(64)): real(64);
  return atan2(y, x);
}
pragma "inline" def atanh(x : real(64)) {
  _extern def atanh(x: real(64)): real(64);
  return atanh(x);
}
pragma "inline" def cbrt(x : real(64)) {
  _extern def cbrt(x: real(64)): real(64);
  return cbrt(x);
}
pragma "inline" def ceil(x : real(64)) {
  _extern def ceil(x: real(64)): real(64);
  return ceil(x);
}
pragma "inline" def cos(x : real(64)) {
  _extern def cos(x: real(64)): real(64);
  return cos(x);
}
pragma "inline" def cosh(x : real(64)) {
  _extern def cosh(x: real(64)): real(64);
  return cosh(x);
}
pragma "inline" def erf(x : real(64)) {
  _extern def erf(x: real(64)): real(64);
  return erf(x);
}
pragma "inline" def erfc(x : real(64)) {
  _extern def erfc(x: real(64)): real(64);
  return erfc(x);
}
pragma "inline" def exp(x : real(64)) {
  _extern def exp(x: real(64)): real(64);
  return exp(x);
}
pragma "inline" def exp2(x : real(64)) {
  _extern def exp2(x: real(64)): real(64);
  return exp2(x);
}
pragma "inline" def expm1(x : real(64)) {
  _extern def expm1(x: real(64)): real(64);
  return expm1(x);
}
pragma "inline" def abs(x : real(64)) {
  _extern def fabs(x: real(64)): real(64);
  return fabs(x);
}
pragma "inline" def floor(x : real(64)) {
  _extern def floor(x: real(64)): real(64);
  return floor(x);
}
pragma "inline" def lgamma(x : real(64)) {
  _extern def lgamma(x: real(64)): real(64);
  return lgamma(x);
}
pragma "inline" def log(x : real(64)) {
  _extern def log(x: real(64)): real(64);
  return log(x);
}
pragma "inline" def log2(x : real(64)) {
  _extern def log2(x: real(64)): real(64);
  return log2(x);
}
pragma "inline" def log10(x : real(64)) {
  _extern def log10(x: real(64)): real(64);
  return log10(x);
}
pragma "inline" def log1p(x : real(64)) {
  _extern def log1p(x: real(64)): real(64);
  return log1p(x);
}
pragma "inline" def nearbyint(x : real(64)) {
  _extern def nearbyint(x: real(64)): real(64);
  return nearbyint(x);
}
pragma "inline" def rint(x : real(64)) {
  _extern def rint(x: real(64)): real(64);
  return rint(x);
}
pragma "inline" def round(x : real(64)) {
  _extern def round(x: real(64)): real(64);
  return round(x);
}
pragma "inline" def sin(x: real(64)) {
  _extern def sin(x: real(64)): real(64);
  return sin(x);
}
pragma "inline" def sinh(x : real(64)) {
  _extern def sinh(x: real(64)): real(64);
  return sinh(x);
}
pragma "inline" def sqrt(x : real(64)) {
  _extern def sqrt(x: real(64)): real(64);
  return sqrt(x);
}
pragma "inline" def tan(x : real(64)) {
  _extern def tan(x: real(64)): real(64);
  return tan(x);
}
pragma "inline" def tanh(x : real(64)) {
  _extern def tanh(x: real(64)): real(64);
  return tanh(x);
}
pragma "inline" def tgamma(x : real(64)) {
  _extern def tgamma(x: real(64)): real(64);
  return tgamma(x);
}
pragma "inline" def trunc(x : real(64)) {
  _extern def trunc(x: real(64)): real(64);
  return trunc(x);
}

pragma "inline" def isnan(x: real(32)) {
  _extern def isnan(x: real(32)): bool;
  return isnan(x);
}
pragma "inline" def acos(x : real(32)) {
  _extern def acosf(x: real(32)): real(32);
  return acosf(x);
}
pragma "inline" def acosh(x : real(32)) {
  _extern def acoshf(x: real(32)): real(32);
  return acoshf(x);
}
pragma "inline" def asin(x : real(32)) {
  _extern def asinf(x: real(32)): real(32);
  return asinf(x);
}
pragma "inline" def asinh(x : real(32)) {
  _extern def asinhf(x: real(32)): real(32);
  return asinhf(x);
}
pragma "inline" def atan(x : real(32)) {
  _extern def atanf(x: real(32)): real(32);
  return atanf(x);
}
pragma "inline" def atan2(y : real(32), x: real(32)) {
  _extern def atan2f(y: real(32), x: real(32)): real(32);
  return atan2f(y, x);
}
pragma "inline" def atanh(x : real(32)) {
  _extern def atanhf(x: real(32)): real(32);
  return atanhf(x);
}
pragma "inline" def cbrt(x : real(32)) {
  _extern def cbrtf(x: real(32)): real(32);
  return cbrtf(x);
}
pragma "inline" def ceil(x : real(32)) {
  _extern def ceilf(x: real(32)): real(32);
  return ceilf(x);
}
pragma "inline" def cos(x : real(32)) {
  _extern def cosf(x: real(32)): real(32);
  return cosf(x);
}
pragma "inline" def cosh(x : real(32)) {
  _extern def coshf(x: real(32)): real(32);
  return coshf(x);
}
pragma "inline" def erf(x : real(32)) {
  _extern def erff(x: real(32)): real(32);
  return erff(x);
}
pragma "inline" def erfc(x : real(32)) {
  _extern def erfcf(x: real(32)): real(32);
  return erfcf(x);
}
pragma "inline" def exp(x : real(32)) {
  _extern def expf(x: real(32)): real(32);
  return expf(x);
}
pragma "inline" def exp2(x : real(32)) {
  _extern def exp2f(x: real(32)): real(32);
  return exp2f(x);
}
pragma "inline" def expm1(x : real(32)) {
  _extern def expm1f(x: real(32)): real(32);
  return expm1f(x);
}
pragma "inline" def abs(x : real(32)) {
  _extern def fabsf(x: real(32)): real(32);
  return fabsf(x);
}
pragma "inline" def floor(x : real(32)) {
  _extern def floorf(x: real(32)): real(32);
  return floorf(x);
}
pragma "inline" def lgamma(x : real(32)) {
  _extern def lgammaf(x: real(32)): real(32);
  return lgammaf(x);
}
pragma "inline" def log(x : real(32)) {
  _extern def logf(x: real(32)): real(32);
  return logf(x);
}
pragma "inline" def log2(x : real(32)) {
  _extern def log2f(x: real(32)): real(32);
  return log2f(x);
}
pragma "inline" def log10(x : real(32)) {
  _extern def log10f(x: real(32)): real(32);
  return log10f(x);
}
pragma "inline" def log1p(x : real(32)) {
  _extern def log1pf(x: real(32)): real(32);
  return log1pf(x);
}
pragma "inline" def nearbyint(x : real(32)) {
  _extern def nearbyintf(x: real(32)): real(32);
  return nearbyintf(x);
}
pragma "inline" def rint(x : real(32)) {
  _extern def rintf(x: real(32)): real(32);
  return rintf(x);
}
pragma "inline" def round(x : real(32)) {
  _extern def roundf(x: real(32)): real(32);
  return roundf(x);
}
pragma "inline" def sin(x: real(32)) {
  _extern def sinf(x: real(32)): real(32);
  return sinf(x);
}
pragma "inline" def sinh(x : real(32)) {
  _extern def sinhf(x: real(32)): real(32);
  return sinhf(x);
}
pragma "inline" def sqrt(x : real(32)) {
  _extern def sqrtf(x: real(32)): real(32);
  return sqrtf(x);
}
pragma "inline" def tan(x : real(32)) {
  _extern def tanf(x: real(32)): real(32);
  return tanf(x);
}
pragma "inline" def tanh(x : real(32)) {
  _extern def tanhf(x: real(32)): real(32);
  return tanhf(x);
}
pragma "inline" def tgamma(x : real(32)) {
  _extern def tgammaf(x: real(32)): real(32);
  return tgammaf(x);
}
pragma "inline" def trunc(x : real(32)) {
  _extern def truncf(x: real(32)): real(32);
  return truncf(x);
}

def _logBasePow2Help(in val, baseLog2) {
  var result = -1;
  while (val != 0) {
    val >>= baseLog2;
    result += 1;
  }
  return result;
}

def logBasePow2(in val: int(?w), baseLog2) {
  if (val < 1) {
    halt("Can't take the log() of a non-positive integer");
  }
  return _logBasePow2Help(val, baseLog2);
}

def logBasePow2(in val: uint(?w), baseLog2) {
  if (val < 1) {
    halt("Can't take the log() of a non-positive integer");
  }
  return _logBasePow2Help(val, baseLog2);
}

def log2(in val: int(?w)) {
  return logBasePow2(val, 1);
}

def log2(in val: uint(?w)) {
  return logBasePow2(val, 1);
}
