// FileCheck
// COMPOPTS: --llvm --fast --vectorize --llvm-print-ir loop --llvm-print-ir-stage full --mllvm -force-vector-width=4 --mllvm -force-vector-interleave=1 --mllvm -runtime-memory-check-threshold=0
// NOEXEC

//Check whether vectorization occurs for nested loops
proc loop (A, B) {
  for i in vectorizeOnly(0..511) {
    for j in vectorizeOnly(0..511) {
      // CHECK: <4 x i32>
      A[i,j] = B[i,j]*3;
    }
  }

}

var A : [0..511, 0..511] int(32);
var B : [0..511, 0..511] int(32);

loop(A, B);
writeln("Sum of A is ", + reduce A);
