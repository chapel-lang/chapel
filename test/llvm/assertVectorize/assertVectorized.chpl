
// to ensure that when `--fast` is thrown we actually do vectorize
// use llvm.metadata to force vectorization

proc doSum(A) {
  var sum: A.eltType = 0;
  @llvm.assertVectorized()
  @llvm.metadata(("llvm.loop.vectorize.enable", true), ("llvm.loop.vectorize.width", 4), ("llvm.loop.interleave.count", 1))
  foreach i in A.domain with (ref sum) {
    const ref a = A[i];
    sum += a;
  }
  return sum;
}

proc doSum2(A) {
  var sum: A.eltType = 0;
  @llvm.assertVectorized()
  @llvm.metadata(("llvm.loop.vectorize.enable", true), ("llvm.loop.vectorize.width", 4), ("llvm.loop.interleave.count", 1))
  foreach a in A with (ref sum) {
    sum += sqrt(a);
  }
  return sum;
}

var A: [1..512] real = 1..512;
var res = doSum(A);
writeln("result ", res:int);

res = doSum2(A);
writeln("result ", res);
