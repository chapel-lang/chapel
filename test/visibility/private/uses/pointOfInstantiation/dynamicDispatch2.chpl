// Copy of test/types/type_variables/deitz/test_point_of_instantiation3.chpl
// Verifies that private uses don't affect point of instantiation and subclass
// dynamic dispatch.
module M1 {
  class C {
    var x: int;
    proc foo() { writeln("C.foo = ", this); }
  }
}

module M2 {
  private use M1;
  class D: C {
    type t;
    var y: t;
    override proc foo() {
      writeln("D.foo = ", this);
      y.goo();
    }
  }
}

module M3 {
  use M1;
  proc bar(c: C) {
    c.foo();
  }
}

module M4 {
  private use M2;
  use M1, M3;
  // Even though the use of M2 is private, we should still be able to find the
  // correct version of foo, because of dynamic dispatch.
  record R {
    var z: int;
    proc goo() { writeln("R.goo"); }
  }
  proc main() {
    var c: C = new D(t=R);
    bar(c);
  }
}
