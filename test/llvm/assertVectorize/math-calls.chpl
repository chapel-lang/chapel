
// make sure `sqrt` and `abs` both vectorize

proc testSqrt(ref elms: [?D]) {
  @llvm.assertVectorized
  foreach i in D do
    elms[i] = sqrt(elms[i]);
}
proc testAbs(ref elms: [?D]) {
  @llvm.assertVectorized
  foreach i in D {
    elms[i] = abs(elms[i]);
  }
}

config const N = 1024;
var A32 = [i in 0..#N] i:real(32);
var A64 = [i in 0..#N] i:real(64);

testSqrt(A32);
writeln(+ reduce A32);
testSqrt(A64);
writeln(+ reduce A64);
testAbs(A32);
writeln(+ reduce A32);
testAbs(A64);
writeln(+ reduce A64);
