use BlockDist, CommDiagnostics;

config const n = 11;

const D = {1..n, 1..n} dmapped Block({1..n, 1..n});
var A: [D] real;

startCommDiagnostics();
A[4..4, ..] = 1;
A[.., 11..11] = 2;
A[8, ..] = 3;
A[.., 2] = 4;
A[4..8, 4..8] = 5;
stopCommDiagnostics();

writeln(A);
writeln(getCommDiagnostics());
