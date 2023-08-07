
proc doSum(A) {
  var sum: A.eltType = 0;
  @llvm.assertVectorized()
  foreach i in A.domain {
    const ref a = A[i];
    sum += a;
  }
  return sum;
}

proc doSum2(A) {
  var sum: A.eltType = 0;
  @llvm.assertVectorized()
  foreach a in A {
    sum += sqrt(a);
  }
  return sum;
}

var A: [1..512] real = 1..512;
var res = doSum(A);
writeln("result ", res:int);

res = doSum2(A);
writeln("result ", res);
