class C {
  def foo() { writeln("isa C"); }
}

class D : C {
  def foo() { writeln("isa D"); }
}

var c = C();
c.foo();
c = D();
c.foo();
