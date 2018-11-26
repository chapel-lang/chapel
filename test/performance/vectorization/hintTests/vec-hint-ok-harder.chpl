config const n = 10000;
proc main() {
  var A:[1..n] real;
  var B:[1..n] real;
  var C:[1..n] real;
  for i in 1..n do A[i] = 0;
  for i in 1..n do B[i] = 2;
  for i in 1..n do C[i] = i;

  kernel3forall(A);
  writeln(A[1], " ", A[n]);
  kernel5forall(A, B, C);
  writeln(A[1], " ", A[n]);
  kernel6(A, C);
  writeln(A[1], " ", A[n]);
}

proc kernel3forall(A) {
  forall (i,j) in zip(1..n, 2..) {
    A[i] = j:real;
  }
}

proc kernel5forall(A, B, C) {
  forall (a,b,c) in zip(A, B, C) {
    a = b + c;
  }
}

proc kernel6(A, C) {
  var sum = 0.0;
  forall c in C with (+ reduce sum) {
    sum += c;
  }
  A[1] = sum;
  A[n] = 0;
}
