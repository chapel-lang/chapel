var A: [1..10 by 2] real;
var B: [1..10 by 2 align 2] real;

forall i in A.domain do
  A[i] = i;

forall i in B.domain do
  B[i] = i;

writeln(A.last);
writeln(B.first);
