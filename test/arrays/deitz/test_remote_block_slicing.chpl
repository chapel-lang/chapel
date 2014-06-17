use BlockDist;

const D = {1..20} dmapped Block({1..20});

var A: [D] real = 1..20;
writeln(A);

A[1..5] = A[16..20];
writeln(A);

A = 1..20;
writeln(A);

A[16..20] = A[1..5];
writeln(A);

A = 1..20;
writeln(A);

A[1..5 by 2] = A[16..20 by 2];
writeln(A);

A = 1..20;
writeln(A);

A[16..20 by 2] = A[1..5 by 2];
writeln(A);
