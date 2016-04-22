//
// Noakes: The style of this test implies that it has leaks
//

class C {
  proc goo() {
    writeln("C.goo");
  }
}

class D : C {
  proc foo() {
    writeln("D.foo");
    goo();
  }
}

class E : D {
  proc goo() {
    writeln("E.goo");
  }
}

(new C()).goo();
(new D()).goo();
(new E()).goo();

(new D()).foo();
(new E()).foo();
