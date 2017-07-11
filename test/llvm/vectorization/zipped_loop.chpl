// FileCheck
// COMPOPTS: --llvm --fast --vectorize --llvm-print-ir loop --llvm-print-ir-stage full --mllvm -force-vector-width=4 --mllvm -force-vector-interleave=1 --mllvm -runtime-memory-check-threshold=0
// NOEXEC

//Check if zipped vectorizeOnly loop is vectorizable
proc loop (A, B) {
  for (i,j) in vectorizeOnly(0..511, 0..511) {
    // CHECK: <4 x i32>
    A[i,j] = B[i,j]*3;
  }
}

var A : [0..511, 0..511] int(32);
var B : [0..511, 0..511] int(32);

loop(A, B);
writeln("Sum of A is ", + reduce A);
