proc foo(A: [2..4, 2..4] int) {
  for ie in zip(A.domain,A) do
    writeln(ie);
}

var D = {1..5, 1..5};
var A: [D] int;

for (i,j) in D do
  A(i,j) = 4 + 5*i + j;

writeln(A);
foo(A[2..4,2..4]);
