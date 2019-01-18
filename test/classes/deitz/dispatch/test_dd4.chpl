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
  override proc goo() {
    writeln("E.goo");
  }
}

(new borrowed C()).goo();
(new borrowed D()).goo();
(new borrowed E()).goo();

(new borrowed D()).foo();
(new borrowed E()).foo();
