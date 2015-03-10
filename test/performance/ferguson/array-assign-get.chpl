use CommDiagnostics;


config const n = 100000;
var A: [1..n] int;
var saveB1: int;
var saveBn: int;

for i in 1..n {
  A[i] = i;
}

resetCommDiagnostics();
startCommDiagnostics();

on Locales[1] {
  var B: [1..n] int;
  B = A;
  saveB1 = B[1];
  saveBn = B[n];
}

stopCommDiagnostics();

writeln(saveB1);
writeln(saveBn);

writeln(getCommDiagnostics());
