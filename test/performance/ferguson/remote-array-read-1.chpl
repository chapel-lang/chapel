use CommDiagnostics;

config const n = 100000;
var A: [1..n] int;

for a in A {
  a = 1;
}

resetCommDiagnostics();
startCommDiagnostics();

on Locales[1] {
  var sum = 0;
  for i in 1..n {
    sum += A[i];
  }
  writeln(sum);
}

stopCommDiagnostics();

writeln(A[1]);
writeln(A[n]);

writeln(getCommDiagnostics());
