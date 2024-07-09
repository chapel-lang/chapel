use CommUtil;

config const n = 100000;
config param tupleLen = 10;

var A:[1..n] tupleLen*int;

start();

on Locales[1] {
  for tup in A {
    var tmp: tupleLen*int;
    for j in 0..#tupleLen {
      tmp[j] = 1;
    }
    tup = tmp;
  }
}

stop();

writeln(A[1]);
writeln(A[n]);

// LLVM backend generates 3 gets, C generates 4
report(maxGets=4, maxOns=1, maxGetsToAdjust=3);
