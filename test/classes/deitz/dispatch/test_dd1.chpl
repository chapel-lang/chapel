class C {
  proc foo(z: int) { halt("ahh! z is ", z); }
}

class D : C {
  var x, y: int;
  override proc foo(z: int) { writeln("D.foo (z, this) = ", (z, this)); }
  proc foo(z: bool) { writeln("D.foo bool (z, this) = ", (z, this)); }
}

var c: unmanaged C = new unmanaged D(x=3,y=4);

c.foo(12);
c.foo(true);

var d: unmanaged D = c:unmanaged D;
d.foo(true);

delete c;
