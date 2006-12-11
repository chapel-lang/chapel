use Memory;

class C {
  var c: C;
  var x: int;
}

var u = memoryUsed();

def foo() {
  var c = C();
  c.c = C();
  c.c.c = C();
  c.c.c.x = 2;
  c.c.x = 3;
  c.x = 4;
}

foo();

var uu = memoryUsed();

writeln(uu-u);
