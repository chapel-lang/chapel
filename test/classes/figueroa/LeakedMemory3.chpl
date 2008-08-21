// find out how much memory is leaked when a slice
// is passed in to another function

use Memory;

const N = 5;

def foo (X) {
  var sum = 0;
  [i in X.domain] sum += X[i]*i;
  return sum;
}

var sum = -3;
var A: [1..N] int = 7;

var m1 = memoryUsed();
sum += foo(A[A.domain.low+1..A.domain.high-1]);
var m2 = memoryUsed();

writeln("Amount of leaked memory after calling foo(): ", m2-m1);
writeln("sum is ", sum);
