
config const n = 4;

var A, B: [1..n, 1..n] int;
B[n,n] = 5;
proc PLUS1(a:int, b:int) { if (a+b) != 0 then writeln(a + b); }
proc PLUS2(a:int, b:int) { if (a+b) != 0 then writeln(a + b); return 0; }

writeln("plus1");
PLUS1(A,B);
PLUS1(0,B);
writeln("plus2");
PLUS2(A,B);
PLUS2(0,B);
writeln("done");
