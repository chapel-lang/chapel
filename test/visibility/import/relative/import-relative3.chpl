module M {

  module A {
    proc foo() {
      import super.B;
      import super.B.b;

      writeln(b);
      writeln(B.b);
      writeln(B.bfunc());
    }
    record ARecord { }
    proc ARecord.bar() {
      import super.B;
      import super.B.b;

      writeln(b);
      writeln(B.b);
      writeln(B.bfunc());
    }
  }
  public import this.A;

  module B {
    var b = 2;
    proc bfunc() { return 3; }
  }
  record MRecord { }
  proc MRecord.baz() {
    import this.B;
    import this.B.b;

    writeln(b);
    writeln(B.b);
    writeln(B.bfunc());
  }
 
  proc main() {
    import this.A;
    import this.B;
    import this.B.b;

    writeln(b);
    writeln(B.b);
    writeln(B.bfunc());

    A.foo();

    var ar = new A.ARecord();
    ar.bar();

    var mr = new MRecord();
    mr.baz();
  }
}
