module weirdEnumHiding {
  module A {
    enum foo {b, c, d};

  }

  module B {
    public use super.A;

    proc checkFoo() {
      writeln(foo.b);
    }
  }

  module C {
    private use super.A;

    proc checkFoo() {
      writeln(foo.b);
    }
  }

  module D {
    private use super.C; // or public use super.C except checkFoo;
    private use super.B; // or public use super.B except checkFoo;

    proc checkFoo() {
      writeln(foo.b);
    }
  }

  proc main() {
    B.checkFoo();
    C.checkFoo();
    D.checkFoo();
  }
}
