use Memory;

class C {
  var x: int;
}

var u = memoryUsed();

def foo() {
  var c = C();
  c.x = 2;
}

foo();

var uu = memoryUsed();

writeln(uu-u);
