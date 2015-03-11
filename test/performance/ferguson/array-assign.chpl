use CommDiagnostics;


config const n = 100000;
var A: [1..n] int;
var B: [1..n] int;

for i in 1..n {
  A[i] = i;
}

resetCommDiagnostics();
startCommDiagnostics();

B = A;

stopCommDiagnostics();

writeln(B[1]);
writeln(B[n]);

writeln(getCommDiagnostics());
