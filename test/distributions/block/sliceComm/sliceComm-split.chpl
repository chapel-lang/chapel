use BlockDist, CommDiagnostics;

config const n = 11;

const D = {1..n, 1..n} dmapped Block({1..n, 1..n});
var A: [D] real;

resetCommDiagnostics();
startCommDiagnostics();
ref A4 = A[4..4, ..];
ref A11 = A[.., 11..11];
ref A8 = A[8, ..];
ref A2 = A[.., 2];
ref Amid = A[4..8, 4..8];
stopCommDiagnostics();

writeln(getCommDiagnostics());

resetCommDiagnostics();
startCommDiagnostics();
A4 = 1;
A11 = 2;
A8 = 3;
A2 = 4;
Amid = 5;
stopCommDiagnostics();

writeln(A);
writeln(getCommDiagnostics());
