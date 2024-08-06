var A: [1..5, 1..5] int = 1;
var B: [1..5, 1..5] int = 2;

proc testAndReset() {
  writeln(A);
  writeln();
  A = 1;
}

// supported:
A[3, 1..3] = B[3, 1..3]; testAndReset();
A[1..3, 3] = B[1..3, 3]; testAndReset();

// supported:
A[3, ..] = B[3, ..]; testAndReset();
A[.., 3] = B[.., 3]; testAndReset();

// unsupported:
A[3, 1..3] = B[1..3, 3]; testAndReset();
A[1..3, 3] = B[3, 1..3]; testAndReset();
