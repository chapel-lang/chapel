class C {
  fun foo() { writeln("isa C"); }
}

class D : C {
  fun foo() { writeln("isa D"); }
}

var c = C();
c.foo();
c = D();
c.foo();
