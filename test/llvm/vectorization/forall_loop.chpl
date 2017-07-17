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
