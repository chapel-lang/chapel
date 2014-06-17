class C {
  proc foo() { writeln("isa C"); }
}

class D : C {
  proc foo() { writeln("isa D"); }
}

var c = new C();
c.foo();
delete c;
c = new D();
c.foo();
delete c;
