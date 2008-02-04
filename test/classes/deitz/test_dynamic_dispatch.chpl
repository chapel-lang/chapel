class C {
  def foo() { writeln("isa C"); }
}

class D : C {
  def foo() { writeln("isa D"); }
}

var c = new C();
c.foo();
c = new D();
c.foo();
