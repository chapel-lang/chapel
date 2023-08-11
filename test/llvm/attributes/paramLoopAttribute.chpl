
config param nLanes: int = 8;

proc doSum(A) {
  var sum: A.eltType;
  // ideally, we could support arbitray param expression in the attributes that get resolved
  // the simple use case is a plain config param that allows controlling vector width with a command line option
  @llvm.metadata(("llvm.loop.vectorize.width", nLanes))
  foreach a in A {
    // something
    sum += a;
  }
  return sum;
}

var A: [0..<512] int = 0..<512;
var res = doSum(A);
writeln("result ", res);
