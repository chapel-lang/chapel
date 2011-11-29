class C {
  var x: int;
}

class D: C {
  var y: int;
}

proc C.foo(z: int = 2) {
  writeln((this, z));
}

proc D.foo(z: int = 3) {
  writeln((this, z));
}

var c: C = new D(x=4, y=5);
var d: D = new D(x=6, y=7);

c.foo();
d.foo();
