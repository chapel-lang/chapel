var A: [1..4] real;
var B: [1..3] real;

forall (i,j) in zip(A.domain, B.domain) with (ref A) do
  A[i] = j;

writeln(A);
