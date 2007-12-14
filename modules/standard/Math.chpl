pragma "inline" def abs(i : int(?w)) return if i < 0 then -i else i;
pragma "inline" def abs(i : uint(?w)) return i;
pragma "inline" def abs(x : complex(?w)) return sqrt(x.re*x.re + x.im*x.im);

pragma "inline" def conjg(a: complex(?w)) return (a.re, -a.im):complex;

pragma "inline" def isnan(x: real) return __primitive("isnan", x);
pragma "inline" def acos(x : real) return __primitive("acos", x);
pragma "inline" def acosh(x : real) return __primitive("acosh", x);
pragma "inline" def asin(x : real) return __primitive("asin", x);
pragma "inline" def asinh(x : real) return __primitive("asinh", x);
pragma "inline" def atan(x : real) return __primitive("atan", x);
pragma "inline" def atan2(y : real, x : real) return __primitive("atan2", y, x);
pragma "inline" def atanh(x : real) return __primitive("atanh", x);
pragma "inline" def cbrt(x : real) return __primitive("cbrt", x);
pragma "inline" def ceil(x : real) return __primitive("ceil", x);
pragma "inline" def cos(x : real) return __primitive("cos", x);
pragma "inline" def cosh(x : real) return __primitive("cosh", x);
pragma "inline" def erf(x : real) return __primitive("erf", x);
pragma "inline" def erfc(x : real) return __primitive("erfc", x);
pragma "inline" def exp(x : real) return __primitive("exp", x);
pragma "inline" def exp2(x : real) return __primitive("exp2", x);
pragma "inline" def expm1(x : real) return __primitive("expm1", x);
pragma "inline" def abs(x : real) return __primitive("fabs", x);
pragma "inline" def floor(x : real) return __primitive("floor", x);
pragma "inline" def lgamma(x : real) return __primitive("lgamma", x);
pragma "inline" def log(x : real) return __primitive("log", x);
pragma "inline" def log2(x : real) return __primitive("log2", x);
pragma "inline" def log10(x : real) return __primitive("log10", x);
pragma "inline" def log1p(x : real) return __primitive("log1p", x);
pragma "inline" def nearbyint(x: real) return __primitive("nearbyint", x);
pragma "inline" def rint(x: real) return __primitive("rint", x);
pragma "inline" def round(x: real) return __primitive("round", x);
pragma "inline" def sin(x: real) return __primitive("sin", x);
pragma "inline" def sinh(x : real) return __primitive("sinh", x);
pragma "inline" def sqrt(x : real) return __primitive("sqrt", x);
pragma "inline" def tan(x: real) return __primitive("tan", x);
pragma "inline" def tanh(x : real) return __primitive("tanh", x);
pragma "inline" def tgamma(x : real) return __primitive("tgamma", x);
pragma "inline" def trunc(x : real) return __primitive("trunc", x);

pragma "inline" def isnan(x: real(32)) return __primitive("isnan", x);  // isnan() is a macro in C!
pragma "inline" def acos(x : real(32)) return __primitive("acosf", x);
pragma "inline" def acosh(x : real(32)) return __primitive("acoshf", x);
pragma "inline" def asin(x : real(32)) return __primitive("asinf", x);
pragma "inline" def asinh(x : real(32)) return __primitive("asinhf", x);
pragma "inline" def atan(x : real(32)) return __primitive("atanf", x);
pragma "inline" def atan2(y : real(32), x : real(32)) return __primitive("atan2f", y, x);
pragma "inline" def atanh(x : real(32)) return __primitive("atanhf", x);
pragma "inline" def cbrt(x : real(32)) return __primitive("cbrtf", x);
pragma "inline" def ceil(x : real(32)) return __primitive("ceilf", x);
pragma "inline" def cos(x : real(32)) return __primitive("cosf", x);
pragma "inline" def cosh(x : real(32)) return __primitive("coshf", x);
pragma "inline" def erf(x : real(32)) return __primitive("erff", x);
pragma "inline" def erfc(x : real(32)) return __primitive("erfcf", x);
pragma "inline" def exp(x : real(32)) return __primitive("expf", x);
pragma "inline" def exp2(x : real(32)) return __primitive("exp2f", x);
pragma "inline" def expm1(x : real(32)) return __primitive("expm1f", x);
pragma "inline" def abs(x : real(32)) return __primitive("fabsf", x);
pragma "inline" def floor(x : real(32)) return __primitive("floorf", x);
pragma "inline" def lgamma(x : real(32)) return __primitive("lgammaf", x);
pragma "inline" def log(x : real(32)) return __primitive("logf", x);
pragma "inline" def log2(x : real(32)) return __primitive("log2f", x);
pragma "inline" def log10(x : real(32)) return __primitive("log10f", x);
pragma "inline" def log1p(x : real(32)) return __primitive("log1pf", x);
pragma "inline" def nearbyint(x: real(32)) return __primitive("nearbyintf", x);
pragma "inline" def rint(x: real(32)) return __primitive("rintf", x);
pragma "inline" def round(x: real(32)) return __primitive("roundf", x);
pragma "inline" def sin(x: real(32)) return __primitive("sinf", x);
pragma "inline" def sinh(x : real(32)) return __primitive("sinhf", x);
pragma "inline" def sqrt(x : real(32)) return __primitive("sqrtf", x);
pragma "inline" def tan(x: real(32)) return __primitive("tanf", x);
pragma "inline" def tanh(x : real(32)) return __primitive("tanhf", x);
pragma "inline" def tgamma(x : real(32)) return __primitive("tgammaf", x);
pragma "inline" def trunc(x : real(32)) return __primitive("truncf", x);


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
