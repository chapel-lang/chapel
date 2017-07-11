// FileCheck
// COMPOPTS: --llvm --fast --vectorize --llvm-print-ir loop --llvm-print-ir-stage full --mllvm -force-vector-width=4 --mllvm -force-vector-interleave=1 --mllvm -runtime-memory-check-threshold=0
// NOEXEC

//Check whether vectorization occurs for forall loop
proc loop (A, B) {
  // CHECK: <4 x i32>
  forall i in 0..511 {
      A[i] = B[i]*3;
  }
}

var A : [0..511] int(32);
var B : [0..511] int(32);

loop(A, B);
writeln("Sum of A is ", + reduce A);
