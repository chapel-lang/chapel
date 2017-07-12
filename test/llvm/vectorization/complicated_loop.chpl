//This test checks whether vectorization occurs for more complicated loops
proc loop (A, B, C) {
  // CHECK: <4 x i32>
  for j in vectorizeOnly(0..511) {
    var i = j : int(32);
    A[i] = i*B[i]*A[i]*31+C[i]*91+C[i]%137+B[i]*C[i];
    B[i] = C[i]*(A[i] % i*113)-A[i]*17+(A[i]+1)*C[i];
  }
}

var A : [0..511] int(32);
var B : [0..511] int(32);
var C : [0..511] int(32);

loop(A, B, C);
writeln("Sum of A is ", + reduce A);
