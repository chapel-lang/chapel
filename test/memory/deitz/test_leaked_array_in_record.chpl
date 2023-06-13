use MemDiagnostics;

config var n: int = 10;

const D = {1..n, 1..n};

record R {
  var A: [D] int;
}

proc foo() {
  var result = 0;
  var r: R;
  for (i,j) in D do
    r.A(i,j) = i+j;
  for e in r.A do
    result += e;
  return result;
}

var result = 0;

var m1 = memoryUsed();
result = foo();
var m2 = memoryUsed();

writeln(result);
writeln("leaked memory = ", m2-m1);
