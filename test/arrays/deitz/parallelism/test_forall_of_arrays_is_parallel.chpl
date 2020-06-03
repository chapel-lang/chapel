use ChapelDebugPrint;

config const n: int = 20;

var A, B, C: [1..n] real;

var r = 1.0;

for b in B {
  b = r;
  r += 1.0;
}

writeln(B);

chpl__testParStart();
forall (a,b) in zip(A,B) do
  a = b;
chpl__testParStop();

writeln(A);

chpl__testParStart();
forall (a,b,c) in zip(A,B,C) do
  c = a + b;
chpl__testParStop();

writeln(C);
