module M1 {
  class C {
    type t;
    var y : t;
    proc bar() do
      return foo(y);
  }
}

module M2 {
  use M1;

  record R {
    var x : int = 3;
  }

  proc foo(r : R) do
    return r.x * 2;

  proc main() {
    var c = (new owned C(R)).borrow();
    writeln(c.bar());
  }
}
