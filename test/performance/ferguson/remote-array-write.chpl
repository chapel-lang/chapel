use CommDiagnostics;


config const n = 100000;
var A: [1..n] int;

resetCommDiagnostics();
startCommDiagnostics();

on Locales[1] {
  for a in A {
    a = 1;
  }
}

stopCommDiagnostics();

writeln(A[1]);
writeln(A[n]);

writeln(getCommDiagnostics());
