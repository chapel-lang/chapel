use Memory;

class C {
  var x: int;
}

record R {
  var c: C;
}

var u = memoryUsed();

def foo() {
  var r: R;
  r.c = C();
  r.c.x = 2;
}

foo();

var uu = memoryUsed();

writeln(uu-u);
