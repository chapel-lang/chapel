
proc loop (A, B) {
  // This loop shouldn't get vectorized unless passed --vectorize
  // CHECK-NOT: <4 x i32>
  for i in vectorizeOnly(0..511) {
      A[A[i]] = B[i];
      A[i] = B[i+1];
  }
}

var A : [0..511] int(32);
var B : [0..511] int(32);

loop(A, B);
writeln("Sum of A is ", + reduce A);
