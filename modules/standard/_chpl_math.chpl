//
// Primitive math funs
//


pragma "no codegen" pragma "rename acos" fun acos(x : float) : float {
  return __primitive("acos", x);
}

pragma "no codegen" pragma "rename ceil" fun ceil(x : float) : float {
  return __primitive("ceil", x);
}

pragma "no codegen" pragma "rename cos" fun cos(x : float) : float {
  return __primitive("cos", x);
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

pragma "no codegen" pragma "rename tan" fun tan(x: float) : float {
  return __primitive("tan", x);
}
