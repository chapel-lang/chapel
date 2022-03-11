
proc loop (A, B, C, D, E, F, n) {
  foreach i in 1..n {
    A[i] = 3*B[i];
    foreach j in 1..n {
      C[i,j] = 3*D[i,j];
      foreach k in 1..n {
      // CHECK: <4 x i32>
        E[i,j,k] = 3*F[i,j,k];
      }
    }
  }
}

config const n = 1000;

 var A : [1..n] int(32);
 var B : [1..n] int(32);
 var C : [1..n,1..n] int(32);
 var D : [1..n,1..n] int(32);
 var E : [1..n,1..n,1..n] int(32);
 var F : [1..n,1..n,1..n] int(32);

loop(A, B, C, D, E, F, n);
writeln("Sum of A is ", + reduce A);
