var x10 = 10, x20 = 20, x30 = 30;

class C {
  proc foo(x = x10) { writeln("C.foo:", x); }
}

class D : C {
  override proc foo(x = x20) { writeln("D.foo:", x); }
}

class E : D {
  override proc foo(x = x30) { writeln("E.foo:", x); }
}  

var d = new borrowed D();
d.foo();

var c: borrowed C = new borrowed D();
c.foo();

var c2: borrowed C = new borrowed C();
c2.foo();

var e = new borrowed E();
e.foo();

var e2: borrowed D = new borrowed E();
e2.foo();

var e3: borrowed C = new borrowed E();
e3.foo();
