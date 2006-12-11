use Memory;

class C {
  var c1: C;
  var c2: C;
  var x: int;
}

var u = memoryUsed();

def foo() {
  var c = C();
  c.c1 = C();
  c.c2 = C();
  c.c1.x = 2;
  c.c2.x = 3;
  c.c2 = c.c1;
}

foo();

var uu = memoryUsed();

writeln(uu-u);
