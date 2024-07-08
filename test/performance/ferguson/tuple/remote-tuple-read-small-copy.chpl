use CommUtil;


config const n = 100000;
config param tupleLen = 3;

var A:[1..n] tupleLen*int;
for i in 1..n {
  for j in 0..#tupleLen {
    A[i][j] = i+j+1;
  }
}

var sumToPrint = 0;
start();

on Locales[1] {
  var sum = 0;
  for tup in A {
    var copy = tup;
    for j in 0..#tupleLen {
      sum += copy[j];
    }
  }
  sumToPrint = sum;
}

stop();

writeln(sumToPrint);
writeln(A[1]);
writeln(A[n]);

report(maxPuts=1, maxOns=1);
