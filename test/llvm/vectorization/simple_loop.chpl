//Check whether vectorization occurs at all
//This loop is trivial case for loop vectorizer and if
//vectorization shouldn't happen it definitely won't happen in this case
proc loop (A, B, n) {
  for i in vectorizeOnly(0..n) {
    // CHECK: <4 x i32>
    A[i] = 3*B[i];
  }
}

config const n = 1000;

var A : [0..n] int(32);
var B : [0..n] int(32);

loop(A, B, n);
writeln("Sum of A is ", + reduce A);
