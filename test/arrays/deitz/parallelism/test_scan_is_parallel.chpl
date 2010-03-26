config const n: int = 20;

var A, B: [1..n] real;

var r = 1.0;

for a in A {
  a = r;
  r += 1.0;
}

writeln(A);

chpl__testParStart();
B = + scan A;
chpl__testParStop();

writeln(B);
