module A {
  enum foo {b, c, d};

}

module B {
  use A;

  proc checkFoo() {
    writeln(foo.b);
  }
}

module C {
  private use A;

  proc checkFoo() {
    writeln(foo.b);
  }
}

module D {
  use C;
  use B;

  proc checkFoo() {
    writeln(foo.b);
  }
}

proc main() {
  B.checkFoo();
  C.checkFoo();
  D.checkFoo();
}
