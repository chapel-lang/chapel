
proc loop (A, B) {
  for i in vectorizeOnly(1..511) {
    // CHECK: <4 x i32>
    if(i < 100)
    {
      A[i] = 3*B[i];
    }
    else
    {
      A[i] = 7*B[i];
    }
    A[i] = 2*A[i];
  }
}

var A : [0..511] int(32);
var B : [0..511] int(32);

loop(A, B);
writeln("Sum of A is ", + reduce A);
