class C {
  def foo() { writeln("isa C"); }
}

class D : C {
  def foo() { writeln("isa D"); }
}

var c = new C();
atomic c.foo();
delete c;
c = new D();
c.foo();
delete c;
