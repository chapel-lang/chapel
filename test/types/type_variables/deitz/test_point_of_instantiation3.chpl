module M1 {
  class C {
    var x: int;
    def foo() { writeln("C.foo = ", this); }
  }
}

module M2 {
  use M1;
  class D: C {
    type t;
    var y: t;
    def foo() {
      writeln("D.foo = ", this);
      y.goo();
    }
  }
}

module M3 {
  use M1;
  def bar(c: C) {
    c.foo();
  }
}

module M4 {
  use M1, M2, M3;
  record R {
    var z: int;
    def goo() { writeln("R.goo"); }
  }
  def main() {
    var c: C = new D(t=R);
    bar(c);
  }
}
