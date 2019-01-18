const D = {1..9, 1..9};
var A: [D] real;

forall (i,j) in D do
  A[i,j] = i + j/10.0;

var B = A[5..6, 7..8];

// test B's values and domain
writeln(B);
writeln(B.domain);

// ensure B doesn't alias A
B = 0.0;
writeln(B);
writeln(A);
