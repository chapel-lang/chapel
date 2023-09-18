config const n = 10000;
proc main() {
  var A:[1..n] real;
  var B:[1..n] real;
  var C:[1..n] real;
  for i in 1..n do A[i] = 0;
  for i in 1..n do B[i] = 2;
  for i in 1..n do C[i] = i;
  kernel1for(A);
  writeln(A[1], " ", A[n]);
  kernel1forall(A);
  writeln(A[1], " ", A[n]);
  kernel2for(A);
  writeln(A[1], " ", A[n]);
  kernel2forall(A);
  writeln(A[1], " ", A[n]);
  kernel4for(A, B, C);
  writeln(A[1], " ", A[n]);
  kernel4forall(A, B, C);
  writeln(A[1], " ", A[n]);
}

proc kernel1for(ref A) {
  foreach i in 1..n {
    A[i] = i;
  }
}

proc kernel1forall(ref A) {
  forall i in 1..n with (ref A) {
    A[i] = i;
  }
}

proc kernel2for(ref A) {
  foreach i in 1..n {
    A[i] = sqrt(i:real);
  }
}

proc kernel2forall(ref A) {
  forall i in 1..n with (ref A) {
    A[i] = sqrt(i:real);
  }
}

proc kernel4for(ref A, B, C) {
  foreach i in 1..n {
    A[i] = B[i] + C[i];
  }
}

proc kernel4forall(ref A, B, C) {
  forall i in 1..n with (ref A) {
    A[i] = B[i] + C[i];
  }
}
