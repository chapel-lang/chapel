use ChapelDebugPrint;

config const n: int = 20;

var A: [1..n] real;

var r = 1.0;

for a in A {
  a = r;
  r += 1.0;
}

writeln(A);

chpl__testParStart();
writeln(+ reduce A);
chpl__testParStop();
