class C {
  proc foo() { writeln("isa C"); }
}

class D : C {
  override proc foo() { writeln("isa D"); }
}

var c = new unmanaged C();
c.foo();
delete c;
c = new unmanaged D();
c.foo();
delete c;
