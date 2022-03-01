module weirdEnumHiding {
  module A {
    enum foo {b, c, d};

  }

  module B {
    public use super.A;

    proc checkFooB() {
      writeln(foo.b);
    }
  }

  module C {
    private use super.A;

    proc checkFooC() {
      writeln(foo.b);
    }
  }

  module D {
    public use super.C;
    public use super.B;

    proc checkFooD() {
      writeln(foo.b);
    }
  }

  proc main() {
    B.checkFooB();
    C.checkFooC();
    D.checkFooD();
  }
}
