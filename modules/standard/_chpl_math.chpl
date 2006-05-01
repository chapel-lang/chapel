//
// Primitive math funs
//


pragma "no codegen" pragma "rename acos" fun acos(x : float) : float {
  return __primitive("acos", x);
}

pragma "no codegen" pragma "rename acosh" fun acosh(x : float) : float {
  return __primitive("acosh", x);
}

pragma "no codegen" pragma "rename asin" fun asin(x : float) : float {
  return __primitive("asin", x);
}

pragma "no codegen" pragma "rename asinh" fun asinh(x : float) : float {
  return __primitive("asinh", x);
}

pragma "no codegen" pragma "rename atan" fun atan(x : float) : float {
  return __primitive("atan", x);
}

/*
pragma "no codegen" pragma "rename atan2" fun atan2(x : float) : float {
  return __primitive("atan2", x);
}
*/

pragma "no codegen" pragma "rename atanh" fun atanh(x : float) : float {
  return __primitive("atanh", x);
}

pragma "no codegen" pragma "rename ceil" fun ceil(x : float) : float {
  return __primitive("ceil", x);
}

pragma "no codegen" pragma "rename cos" fun cos(x : float) : float {
  return __primitive("cos", x);
}

pragma "no codegen" pragma "rename cosh" fun cosh(x : float) : float {
  return __primitive("cosh", x);
}

pragma "no codegen" pragma "rename exp" fun exp(x: float) : float {
  return __primitive("eexp", x);
}

pragma "no codegen" pragma "rename fabs" fun fabs(x : float) : float {
  return __primitive("fabs", x);
}

pragma "no codegen" pragma "rename floor" fun floor(x : float) : float {
  return __primitive("floor", x);
}

pragma "no codegen" pragma "rename sin" fun sin(x: float) : float {
  return __primitive("sin", x);
}

pragma "no codegen" pragma "rename sinh" fun sinh(x : float) : float {
  return __primitive("sinh", x);
}

pragma "no codegen" pragma "rename tan" fun tan(x: float) : float {
  return __primitive("tan", x);
}

pragma "no codegen" pragma "rename tanh" fun tanh(x : float) : float {
  return __primitive("tanh", x);
}
