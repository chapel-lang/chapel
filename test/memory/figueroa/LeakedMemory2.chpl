// find out how much memory is leaked when a function with a local array
// is called

use MemDiagnostics;

const N = 5;

proc foo () {
  var sum = 0;
  var A: [1..N] int = 7;
  for i in A.domain do sum += A[i]*i;
  return sum;
}

var sum = -3;

var m1 = memoryUsed();
serial do sum += foo();
var m2 = memoryUsed();

writeln("Amount of leaked memory after calling foo(): ", m2-m1);
writeln("sum is ", sum);
