// FileCheck
// COMPOPTS: --llvm --fast --vectorize --llvm-print-ir loop --llvm-print-ir-stage full --mllvm -force-vector-width=4 --mllvm -force-vector-interleave=1 --mllvm -runtime-memory-check-threshold=0
// NOEXEC

//Check whether vectorization occurs when loop contains if and reduction variable
//In some cases vectorizer is able to see if loop is vectorizable
//Although it contains branch and reduction variable, we want to make sure
//that we compile loop in a way that it allows for such vectorization
proc loop (A, B) {
  var sum : int(32) = 0;
  for i in vectorizeOnly(1..511) {
    // CHECK: <4 x i32>
    if(A[i] <= B[i])
    {
      sum += A[i]+5;
    }
  }
  return sum;
}

var A : [0..511] int(32);
var B : [0..511] int(32);

var res = loop(A, B);
writeln("Result is ", res);
