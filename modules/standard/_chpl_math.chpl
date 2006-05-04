//
// Primitive math funs
//

pragma "inline" fun acos(x : float) return __primitive("acos", x);
pragma "inline" fun acosh(x : float) return __primitive("acosh", x);
pragma "inline" fun asin(x : float) return __primitive("asin", x);
pragma "inline" fun asinh(x : float) return __primitive("asinh", x);
pragma "inline" fun atan(x : float) return __primitive("atan", x);
pragma "inline" fun atanh(x : float) return __primitive("atanh", x);
pragma "inline" fun ceil(x : float) return __primitive("ceil", x);
pragma "inline" fun cos(x : float) return __primitive("cos", x);
pragma "inline" fun cosh(x : float) return __primitive("cosh", x);
pragma "inline" fun fabs(x : float) return __primitive("fabs", x);
pragma "inline" fun floor(x : float) return __primitive("floor", x);
pragma "inline" fun sin(x: float) return __primitive("sin", x);
pragma "inline" fun sinh(x : float) return __primitive("sinh", x);
pragma "inline" fun tan(x: float) return __primitive("tan", x);
pragma "inline" fun tanh(x : float) return __primitive("tanh", x);

// Need to update these to use inline pragma

pragma "no codegen" pragma "rename atan2" fun atan2(y : float, x : float) return __primitive("atan2", x);
pragma "no codegen" pragma "rename exp" fun exp(x: float) return __primitive("eexp", x);
