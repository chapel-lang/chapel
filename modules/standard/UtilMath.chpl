// UtilMath: various math-related utilities that could come handy.

// for correctness, both arguments must be strictly positive
proc divceilpos(m: integral, n: integral) {
  if !_isSignedType(m.type) || !_isSignedType(n.type) then
    compilerError("divceilpos() accepts only arguments of signed integer types");
  return (m - 1) / n + 1;
}

// for correctness, both arguments must be strictly positive
proc divfloorpos(m: integral, n: integral) {
  if !_isSignedType(m.type) || !_isSignedType(n.type) then
    compilerError("divfloorpos() accepts only arguments of signed integer types");
  return m / n;
}
