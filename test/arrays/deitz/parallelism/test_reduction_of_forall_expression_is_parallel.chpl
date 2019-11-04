use ChapelDebugPrint;

config const n: int = 20;

var A, B: [1..n] real;

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
writeln(+ reduce forall (a,b) in zip(A,B) do a + b);
chpl__testParStop();
