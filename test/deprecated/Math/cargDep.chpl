// Copied and reduced from test/types/complex/diten/cplxMathFnTypes.chpl

proc testType(x: complex(?w)) {
  const res2 = carg(x); // Should trigger the deprecation warning
  assert(res2.type == real(w/2));
}

var c64 = 1.0:real(32) + 2.0i:imag(32);
var c128 = 1.0: real(64) + 2.0i: imag(64);

// Both calls cause this deprecation message because they result in different
// instantiations
testType(c64);
testType(c128);
