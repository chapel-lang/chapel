use CommDiagnostics;

config const n = 100000;
config param tupleLen = 10;

var A:[1..n] tupleLen*int;
resetCommDiagnostics();
startCommDiagnostics();

on Locales[1] {
  for tup in A {
    for j in 1..tupleLen {
      tup[j] = 1;
    }
  }
}

stopCommDiagnostics();

writeln(A[1]);
writeln(A[n]);

writeln(getCommDiagnostics());
