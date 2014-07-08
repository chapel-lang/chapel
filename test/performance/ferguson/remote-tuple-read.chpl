use CommDiagnostics;

config const n = 100000;
config param tupleLen = 10;

var A:[1..n] tupleLen*int;
for i in 1..n {
  for j in 1..tupleLen {
    A[i][j] = i+j;
  }
}

resetCommDiagnostics();
startCommDiagnostics();

on Locales[1] {
  var sum = 0;
  for tup in A {
    for j in 1..tupleLen {
      sum += tup[j];
    }
  }
  writeln(sum);
}

stopCommDiagnostics();

writeln(A[1]);
writeln(A[n]);

writeln(getCommDiagnostics());
