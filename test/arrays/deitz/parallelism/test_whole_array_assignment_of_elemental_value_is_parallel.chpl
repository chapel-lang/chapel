config const n: int = 20;

var A: [1..n] real;

var r = 1.0;

chpl__testParStart();
A = r;
chpl__testParStop();

writeln(A);
