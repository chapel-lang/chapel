config const n = 10000;
proc main() {
  var A:[1..n] real;
  var B:[1..n] real;
  var C:[1..n] real;
  for i in 1..n do A[i] = 0;
  for i in 1..n do B[i] = 2;
  for i in 1..n do C[i] = i;

  kernel7(A, C);
  writeln(A[1], " ", A[n]);
}

proc kernel7(A, C) {
  var m = 0.0;
  forall c in C with (max reduce m) {
    m reduce= c;
  }
  A[1] = m;
  A[n] = 0;
}
