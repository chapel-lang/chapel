class C {
  var x: int;
}

class D: C {
  var y: int;
}

proc C.foo(z: int = 2) {
  writeln((this, z));
}

override proc D.foo(z: int = 3) {
  writeln((this, z));
}

var c: unmanaged C = new unmanaged D(x=4, y=5);
var d: unmanaged D = new unmanaged D(x=6, y=7);

c.foo();
d.foo();

delete d;
delete c;
