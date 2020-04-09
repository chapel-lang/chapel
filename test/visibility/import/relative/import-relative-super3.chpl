module M {
  module A {
    module B {
      module C {
        module D {
          var x = 0;
          proc f() { return 1; }
        }
        public import this.D;
      }
      public import this.C;
    }
    public import this.B;
  }
  public import this.A;

  module Sibling {
    import super.A.B.C.D;
    import super.A.B.C.D.x;
    import super.A.B.C.D.f;
    proc foo() {
      writeln(x);
      writeln(f());
      writeln(D.x);
      writeln(D.f());
    }
  }
  public import this.Sibling;

  proc main() {
    Sibling.foo();
  }
}
