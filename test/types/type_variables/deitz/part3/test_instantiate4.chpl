module M1 {
  class C {
    type t;
    var y : t;
    def bar()
      return foo(y);
  }
}

module M2 {
  use M1;

  record R {
    var x : int = 3;
  }

  def foo(r : R)
    return r.x * 2;

  def main() {
    var c = new C(R);
    writeln(c.bar());
  }
}
