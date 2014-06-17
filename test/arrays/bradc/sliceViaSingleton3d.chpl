config const n = 3;

const D = {1..n, 1..n, 1..n};

var A, B: [D] real;

[(i,j,k) in D] A(i,j,k) = i*10.0 + j + k/10.0;

writeln("A is:\n", A, "\n");

//
// slicing via degenerate ranges:
//
B[1..1, 1..n, 1..n] = A[1..1, 1..n, 1..n];
B[2..2, 2..2, 1..n] = A[2..2, 2..2, 1..n];
B[3..3, 1..n, 3..3] = A[3..3, 1..n, 3..3];

writeln("B is:\n", B, "\n");

B = 0.0;

//
// slicing via singletons:
//
B[1, 1..n, 1..n] = A[1, 1..n, 1..n];
B[2, 2, 1..n] = A[2, 2, 1..n];
B[3, 1..n, 3] = A[3, 1..n, 3];

writeln("B is:\n", B, "\n");
