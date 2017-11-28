class C {
  proc foo(x = 10) { writeln("C.foo:", x); }
}

class D : C {
  proc foo(x = 20) { writeln("D.foo:", x); }
}

var d = new D();
d.foo();

var c: C = new D();
c.foo();
