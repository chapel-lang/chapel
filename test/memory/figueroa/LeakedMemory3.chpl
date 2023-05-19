// find out how much memory is leaked when a slice
// is passed in to another function

use MemDiagnostics;

const N = 5;
var A: [1..N] int;
serial do A = 7;
var sum = -3;

proc foo (X) {
  var sum = 0;
  for i in X.domain do sum += X[i]*i;
  return sum;
}

proc bar () {
sum += foo(A[A.domain.low+1..A.domain.high-1]);
}

var m1 = memoryUsed();
bar();
var m2 = memoryUsed();

writeln("Amount of leaked memory after calling bar(): ", m2:int - m1:int);
if m2 -m1 != 0 then
  writeln((m2, m1));
writeln("sum is ", sum);
writeln(A);
