// find out how much memory is leaked when a function with a local array
// is called, and than array's domain is changed

use MemDiagnostics;

const N = 5;
var D = {2..N};
var B: [D] int = 1;

proc foo () {
  var sum = 0;
  var A: [D] int = 7;
  D = {1..N};
  A[A.domain.low] = 5;
  [i in A.domain with (ref sum)] sum += A[i]*i;
  return sum;
}

var sum = -3;

writeln(B);
var m1 = memoryUsed();
serial do sum += foo();
var m2 = memoryUsed();

writeln("Amount of leaked memory after calling foo(): ", m2:int - m1:int);
// This call should return 8, because the domain for B is now 8 bytes longer
// than it was previously.  That memory is not actually leaked as it will be
// handled when B and its domain are cleaned up at the end of this test.
writeln("sum is ", sum);
writeln(B);
