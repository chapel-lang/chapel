var x: int = 2;

class C {
  proc foo(i: int) { x += i; }
}

class D : C {
  proc foo(i: int) { x -= i; }
}

var c = new C();
atomic c.foo(2);
delete c;
c = new D();
c.foo(2);
delete c;

writeln("x = ", x);