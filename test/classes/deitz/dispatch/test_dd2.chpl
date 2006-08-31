class C {
  var x: int;
}

class D: C {
  var y: int;
}

def C.foo(z: int = 2) {
  writeln((this, z));
}

def D.foo(z: int = 3) {
  writeln((this, z));
}

var c: C = D(x=4, y=5);
var d: D = D(x=6, y=7);

c.foo();
d.foo();
