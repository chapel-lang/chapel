// Snippet taken from test/types/complex/diten/cplxMathFnTypes.chpl
proc testTypes(x: complex(?w)) {
  const res4 = cproj(z=x);
  assert(res4.type == complex(w));
}

var c64 = 1.0:real(32) + 2.0i:imag(32);
var c128 = 1.0: real(64) + 2.0i: imag(64);

testTypes(c64);
testTypes(c128);
