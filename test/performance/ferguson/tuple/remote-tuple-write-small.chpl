use CommUtil;

config const n = 100000;
config param tupleLen = 3;

var A:[1..n] tupleLen*int;

start();

on Locales[1] {
  for tup in A {
    for j in 0..#tupleLen {
      tup[j] = 1;
    }
  }
}

stop();

writeln(A[1]);
writeln(A[n]);

// LLVM backend generates 3 gets, C generates 4
report(maxGets=4, maxOns=1, maxGetsToAdjust=3);
