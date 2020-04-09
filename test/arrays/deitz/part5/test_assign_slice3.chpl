var D = {1..5};
var InD = {2..4};

var A, B: [D] int;

B = D;

writeln(A);
writeln(B);

A[InD] = B[InD];

writeln(A);
writeln(B);
