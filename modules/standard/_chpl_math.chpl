//
// Primitive math funs
//

pragma "inline" fun acos(x : float) return __primitive("acos", x);
pragma "inline" fun acosh(x : float) return __primitive("acosh", x);
pragma "inline" fun asin(x : float) return __primitive("asin", x);
pragma "inline" fun asinh(x : float) return __primitive("asinh", x);
pragma "inline" fun atan(x : float) return __primitive("atan", x);
pragma "inline" fun atanh(x : float) return __primitive("atanh", x);
pragma "inline" fun cbrt(x : float) return __primitive("cbrt", x);
pragma "inline" fun ceil(x : float) return __primitive("ceil", x);
pragma "inline" fun cos(x : float) return __primitive("cos", x);
pragma "inline" fun cosh(x : float) return __primitive("cosh", x);
pragma "inline" fun erf(x : float) return __primitive("erf", x);
pragma "inline" fun erfc(x : float) return __primitive("erfc", x);
pragma "inline" fun exp(x : float) return __primitive("exp", x);
pragma "inline" fun exp2(x : float) return __primitive("exp2", x);
pragma "inline" fun expm1(x : float) return __primitive("expm1", x);
pragma "inline" fun fabs(x : float) return __primitive("fabs", x);
pragma "inline" fun floor(x : float) return __primitive("floor", x);
pragma "inline" fun lgamma(x : float) return __primitive("lgamma", x);
pragma "inline" fun log(x : float) return __primitive("log", x);
pragma "inline" fun log10(x : float) return __primitive("log10", x);
pragma "inline" fun log1p(x : float) return __primitive("log1p", x);
pragma "inline" fun log2(x : float) return __primitive("log2", x);
pragma "inline" fun logb(x : float) return __primitive("logb", x);
pragma "inline" fun nearbyint(x : float) return __primitive("nearbyint", x);
pragma "inline" fun rint(x: float) return __primitive("rint", x);
pragma "inline" fun round(x: float) return __primitive("round", x);
pragma "inline" fun sin(x: float) return __primitive("sin", x);
pragma "inline" fun sinh(x : float) return __primitive("sinh", x);
pragma "inline" fun sqrt(x : float) return __primitive("sqrt", x);
pragma "inline" fun tan(x: float) return __primitive("tan", x);
pragma "inline" fun tanh(x : float) return __primitive("tanh", x);
pragma "inline" fun tgamma(x : float) return __primitive("tgamma", x);
pragma "inline" fun trunc(x : float) return __primitive("trunc", x);



// Need to update these to use inline pragma

pragma "no codegen" pragma "rename atan2" fun atan2(y : float, x : float) return __primitive("atan2", x);


/*
pragma "no codegen" pragma "rename exp" fun exp(x: float) return __primitive("exp", x);
*/