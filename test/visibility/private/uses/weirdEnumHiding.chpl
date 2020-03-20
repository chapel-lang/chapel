module weirdEnumHiding {
  module A {
    enum foo {b, c, d};

  }

  module B {
    public use A;

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
    public use C;
    public use B;

    proc checkFoo() {
      writeln(foo.b);
    }
  }

  proc main() {
    import weirdEnumHiding.B;
    import weirdEnumHiding.C;
    import weirdEnumHiding.D;
    B.checkFoo();
    C.checkFoo();
    D.checkFoo();
  }
}
