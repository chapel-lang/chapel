use ChapelDebugPrint;

config const n: int = 6;

var A, B: [1..n, 1..n] real;

var r = 1.0;

for b in B {
  b = r;
  r += 1.0;
}

writeln(B);

chpl__testParStart();
A = B;
chpl__testParStop();

writeln(A);
