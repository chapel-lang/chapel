config const n = 100;
config const shift = 2;
config type myType = real;

use Random;

var randlist = new RandomStream(314159265);
var T: [1..n] real;
randlist.fillRandom(T);

proc foo(type myType) {
  var A = (T*(1<<shift):real):myType;
  var x: sync myType = 0:myType;
  forall i in 1..n do
    x += A[i];
  var sum = + reduce A;
  // This check could run into troubles with floating point types
  if x.readXX() != sum then
    writeln("ERROR: sums do not match for ", typeToString(myType),
            " (should be ", sum, ", result is ", x.readXX(), ")");
}

foo(int(32));
foo(int(64));
foo(uint(32));
foo(uint(64));
foo(real(32));
foo(real(64));
