use Memory;

var u = memoryUsed();

def foo() {
  var x: [1..4] int;
  x = 1..4;
  return x(2);
}

var y = foo() + foo() + foo();

var uu = memoryUsed();

writeln(y);

writeln(uu-u);
