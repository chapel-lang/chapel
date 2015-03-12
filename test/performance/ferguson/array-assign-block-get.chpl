use BlockDist;
use CommDiagnostics;

config const n = 100000;

const Space = {1..n};
const D = Space dmapped Block(boundingBox=Space);
var A: [D] int;

var saveB1: int;
var saveBn: int;

for i in 1..n {
  A[i] = i;
}

resetCommDiagnostics();
startCommDiagnostics();

on Locales[1] {
  var B: [D] int;
  B = A;
  saveB1 = B[1];
  saveBn = B[n];
}

stopCommDiagnostics();

writeln(saveB1);
writeln(saveBn);

writeln(getCommDiagnostics());
