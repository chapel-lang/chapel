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
