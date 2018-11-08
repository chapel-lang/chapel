config const n = 10;
proc main() {
  var A:[1..n] real = 0;
  var B:[1..n] real = 2;
  var C:[1..n] real = 1..n;
  kernel3for(A);
  writeln(A[1], " ", A[n]);
  kernel3forall(A);
  writeln(A[1], " ", A[n]);
  kernel5for(A, B, C);
  writeln(A[1], " ", A[n]);
  kernel5forall(A, B, C);
  writeln(A[1], " ", A[n]);
  kernel6(A, C);
  writeln(A[1], " ", A[n]);
}


proc kernel3for(A) {
  for (i,j) in vectorizeOnly(1..n, 2..) {
    A[i] = j:real;
  }
}

proc kernel3forall(A) {
  forall (i,j) in zip(1..n, 2..) {
    A[i] = j:real;
  }
}

proc kernel5for(A, B, C) {
  for (a,b,c) in vectorizeOnly(A, B, C) {
    a = b + c;
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
