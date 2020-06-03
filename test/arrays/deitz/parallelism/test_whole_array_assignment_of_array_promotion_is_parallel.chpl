use ChapelDebugPrint;

config const n: int = 20;

var A, B, C: [1..n] real;

var r = 1.0;

for a in A {
  a = r;
  r += 1.0;
}

writeln(A);

for b in B {
  b = r;
  r += 1.0;
}

writeln(B);

chpl__testParStart();
C = A + B;
chpl__testParStop();

writeln(C);
