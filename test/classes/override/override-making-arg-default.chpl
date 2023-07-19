var five = 5;
var six = 6;

class C {
  proc foo(x: int = five) { writeln("C.foo: ", x); }
}

class D : C {
  override proc foo(x: int) { writeln("D.foo: ", x); }
}

var c = new C();
c.foo();
c.foo(1);

var d = new D();
d.foo();
d.foo(2);

var d2: C = new D();
d2.foo();
d2.foo(3);
