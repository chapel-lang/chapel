use CommUtil;

config const n = 100000;
config param tupleLen = 3;

var A:[1..n] tupleLen*int;

start();

on Locales[1] {
  for tup in A {
    var tmp: tupleLen*int;
    for j in 1..tupleLen {
      tmp[j] = 1;
    }
    tup = tmp;
  }
}

stop();

writeln(A[1]);
writeln(A[n]);

report(maxGets=5, maxOns=1);
