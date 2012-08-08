config const n = 9;

const D = {1..n, 1..n};

var A, B: [D] real;

[(i,j) in D] A(i,j) = i + j/10.0;

writeln("A is:\n", A, "\n");

//
// slicing via degenerate ranges:
//
config const doSerial = true;
serial doSerial {
  B[n/2..n/2, 1..n] = A[n/2..n/2, 1..n];
  B[1..n, n/2..n/2] = A[1..n, n/2..n/2];
  B[1..1, 1..n] = A[1..n, 1..1];
}

writeln("B is:\n", B, "\n");
