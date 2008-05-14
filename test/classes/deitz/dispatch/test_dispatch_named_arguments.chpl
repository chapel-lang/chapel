class C {
  def foo(x: int) { writeln("C.foo = ", x); }
  def bar(x: int) { writeln("C.bar = ", x); }
}

class D: C {
  def foo(x: int) { writeln("D.foo = ", x); }
  def bar(y: int) { writeln("D.bar = ", y); }
}

var c: C = new C();

c.foo(1);
c.bar(1);

c = new D();

c.foo(1);
c.bar(1);
