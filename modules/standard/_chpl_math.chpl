//
// Primitive math funs
//

pragma "inline" def abs(x : int) return if x < 0 then -x else x;
pragma "inline" def abs(x : int(64)) return if x < 0 then -x else x;
pragma "inline" def abs(x : uint) return if x < 0 then -x else x;
pragma "inline" def abs(x : uint(64)) return if x < 0 then -x else x;

pragma "inline" def acos(x : float) return __primitive("acos", x);
pragma "inline" def acosh(x : float) return __primitive("acosh", x);
pragma "inline" def asin(x : float) return __primitive("asin", x);
pragma "inline" def asinh(x : float) return __primitive("asinh", x);
pragma "inline" def atan(x : float) return __primitive("atan", x);
pragma "inline" def atan2(y : float, x : float) return __primitive("atan2", y, x);
pragma "inline" def atanh(x : float) return __primitive("atanh", x);
pragma "inline" def cbrt(x : float) return __primitive("cbrt", x);
pragma "inline" def ceil(x : float) return __primitive("ceil", x);
pragma "inline" def cos(x : float) return __primitive("cos", x);
pragma "inline" def cosh(x : float) return __primitive("cosh", x);
pragma "inline" def erf(x : float) return __primitive("erf", x);
pragma "inline" def erfc(x : float) return __primitive("erfc", x);
pragma "inline" def exp(x : float) return __primitive("exp", x);
pragma "inline" def exp2(x : float) return __primitive("exp2", x);
pragma "inline" def expm1(x : float) return __primitive("expm1", x);
pragma "inline" def abs(x : float) return __primitive("fabs", x);
pragma "inline" def floor(x : float) return __primitive("floor", x);
pragma "inline" def lgamma(x : float) return __primitive("lgamma", x);
pragma "inline" def log(x : float) return __primitive("log", x);
pragma "inline" def log2(x : float) return __primitive("log2", x);
pragma "inline" def log10(x : float) return __primitive("log10", x);
pragma "inline" def log1p(x : float) return __primitive("log1p", x);
pragma "inline" def nearbyint(x: float) return __primitive("nearbyint", x);
pragma "inline" def rint(x: float) return __primitive("rint", x);
pragma "inline" def round(x: float) return __primitive("round", x);
pragma "inline" def sin(x: float) return __primitive("sin", x);
pragma "inline" def sinh(x : float) return __primitive("sinh", x);
pragma "inline" def sqrt(x : float) return __primitive("sqrt", x);
pragma "inline" def tan(x: float) return __primitive("tan", x);
pragma "inline" def tanh(x : float) return __primitive("tanh", x);
pragma "inline" def tgamma(x : float) return __primitive("tgamma", x);
pragma "inline" def trunc(x : float) return __primitive("trunc", x);


def lg(in x) {
  var retval = -1;
  while (x >= 1) {
    x /= 2;
    retval += 1;
  }

  return retval;
}


