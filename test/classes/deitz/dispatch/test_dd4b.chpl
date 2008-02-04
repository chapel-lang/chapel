class C {
  def goo() {
    writeln("C.goo");
  }
}

class D : C {
  def foo() {
    goo();
  }
}

class E : D {
  def goo() {
    writeln("E.goo");
  }
}

(new C()).goo();
(new D()).goo();
(new E()).goo();
