class C {
  proc foo(x: int) { writeln("C.foo = ", x); }
  proc bar(x: int) { writeln("C.bar = ", x); }
}

class D: C {
  proc foo(x: int) { writeln("D.foo = ", x); }
  proc bar(y: int) { writeln("D.bar = ", y); }
}

var c: C = new C();

c.foo(1);
c.bar(1);
delete c;

c = new D();

c.foo(1);
c.bar(1);
delete c;
