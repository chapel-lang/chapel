class C {
  proc goo() {
    writeln("C.goo");
  }
}

class D : C {
  proc foo() {
    goo();
  }
}

class E : D {
  proc goo() {
    writeln("E.goo");
  }
}

(new C()).goo();
