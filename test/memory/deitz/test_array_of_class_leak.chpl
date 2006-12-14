use Memory;

class C {
  var x: int;
}

var u = memoryUsed();

def foo() {
  var x: [1..4] C;
  for i in 1..4 do
    x(i) = C(i);
  return x(2).x;
}

var y = foo();

var uu = memoryUsed();

writeln(y);

writeln(uu-u);
