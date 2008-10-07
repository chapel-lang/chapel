// find out how much memory is leaked when a slice
// is passed in to another function

use Memory;

const N = 5;
var A: [1..N] int = 7;
var sum = -3;

def foo (X) {
  var sum = 0;
  [i in X.domain] sum += X[i]*i;
  return sum;
}

def bar () {
sum += foo(A[A.domain.low+1..A.domain.high-1]);
}

var m1 = memoryUsed();
bar();
var m2 = memoryUsed();

writeln("Amount of leaked memory after calling bar(): ", m2:int - m1:int);
writeln("sum is ", sum);
writeln(A);
