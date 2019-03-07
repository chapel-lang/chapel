module M1 {
  class C {
    var x: int;
    proc foo() { writeln("C.foo = ", this); }
  }
}

module M2 {
  use M1;
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
  proc bar(c: borrowed C) {
    c.foo();
  }
}

module M4 {
  use M1, M2, M3;

  record R {
    var z: int;
    proc goo() { writeln("R.goo"); }
  }

  proc main() {
    var c: borrowed C = new borrowed D(t=R);

    bar(c);


  }
}
