config const n = 10000;
proc main() {
  var A:[1..n] real;
  var B:[1..n] real;
  var C:[1..n] real;
  for i in 1..n do A[i] = 0;
  for i in 1..n do B[i] = 2;
  for i in 1..n do C[i] = i;

  kernel3for(A);
  writeln(A[1], " ", A[n]);
  kernel5for(A, B, C);
  writeln(A[1], " ", A[n]);
}


proc kernel3for(A) {
  for (i,j) in vectorizeOnly(1..n, 2..) {
    A[i] = j:real;
  }
}

proc kernel5for(A, B, C) {
  for (a,b,c) in vectorizeOnly(A, B, C) {
    a = b + c;
  }
}
