use CommDiagnostics;


config const n = 100000;
var A: [1..n] int;
var B: [1..n] int;

for a in A {
  a = 1;
}
for b in B {
  b = -1;
}

resetCommDiagnostics();
startCommDiagnostics();

on Locales[1] {
  var sum = 0;
  for (a,b) in zip(A,B) {
    sum += a;
    sum += b;
  }
  writeln(sum);
}

stopCommDiagnostics();

writeln(A[1], " ", B[1]);
writeln(A[n], " ", B[n]);

writeln(getCommDiagnostics());
