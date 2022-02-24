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
    public use super.C except checkFoo;
    public use super.B except checkFoo;

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
