var A: [1..5, 1..5] int = [(i,j) in {1..5, 1..5}] (i-1)*5 + j;

var B: [1..5] int;

writeln(A);

B = A[1..5, 1];
writeln(B);

B = A[1, 1..5];
writeln(B);

proc foo(B: [] int) {
  writeln(B);
}

foo(A[1, 1..5]);
foo(A[1..5, 1]);
