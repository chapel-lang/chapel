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
    public use super.C;
    public use super.B;

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
