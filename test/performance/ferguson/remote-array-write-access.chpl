use CommUtil;


config const n = 100000;
var A: [1..n] int;

start();

on Locales[1] {
  for i in 1..n {
    A[i] = 1;
  }
}

stop();

writeln(A[1]);
writeln(A[n]);

report(maxGets=1, maxOns=1);
