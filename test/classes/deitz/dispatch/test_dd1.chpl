class C {
  proc foo(z: int) { halt("ahh! z is ", z); }
}

class D : C {
  var x, y: int;
  proc foo(z: int) { writeln("D.foo (z, this) = ", (z, this)); }
  proc foo(z: bool) { writeln("D.foo bool (z, this) = ", (z, this)); }
}

var c: C = new D(x=3,y=4);

c.foo(12);
c.foo(true);

var d: D = c:D;
d.foo(true);
