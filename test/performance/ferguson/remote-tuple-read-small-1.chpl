use CommDiagnostics;


config const n = 100000;
config param tupleLen = 3;

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
    var copy = tup;
    for j in 1..tupleLen {
      sum += copy[j];
    }
  }
  writeln(sum);
}

stopCommDiagnostics();

writeln(A[1]);
writeln(A[n]);

writeln(getCommDiagnostics());
