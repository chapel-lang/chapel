use CommUtil;


config const n = 100000;
var A: [1..n] int;

start();

on Locales[1] {
  for a in A {
    a = 1;
  }
}

stop();

writeln(A[1]);
writeln(A[n]);

report(maxGets=5, maxOns=1);
