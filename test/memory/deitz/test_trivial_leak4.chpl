use Memory;

class C {
  var x: int;
}

class D {
  var c: C;
}

var u = memoryUsed();

def foo() {
  var d = D();
  d.c = C();
  d.c.x = 2;
}

foo();

var uu = memoryUsed();

writeln(uu-u);
