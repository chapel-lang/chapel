use ChapelDebugPrint;

config const n: int = 20;

var A, B: [1..n] real;

var r = 1.0;

chpl__testParStart();
A = r;
// test for standalone with an outer variable
forall b in B do
  b = r;
forall b in B with (ref r) { // race should be OK
  r = 2.0;
  b = r;
}
chpl__testParStop();

writeln(A);
writeln(B);
