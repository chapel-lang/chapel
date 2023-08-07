
proc saxpy(a: ?elmType, X: [?d] a.type, Y: X.type) {
  var R: X.type = 0;

  // by setting "llvm.loop.isvectorized", the vectorizer will not run on this loop
  // it thinks the loop is already vectorized
  // this means assertVectorized will always assert true even if optimizations improve
  // this makes the test less fragile

  @llvm.assertVectorized()
  @llvm.metadata(("llvm.loop.isvectorized", true))
  foreach (r, x, y) in zip(R, X, Y) {
    r = a*x + y;
  }

  return R;
}

var X, Y: [1..16] real = 1..16;
var R = saxpy(2.0, X, Y);
writeln(R);
