module M1 {
  class C {
    type t;
    var y : t;
    fun bar()
      return foo(y);
  }
}

module M2 {
  use M1;

  record R {
    var x : int = 3;
  }

  fun foo(r : R)
    return r.x * 2;

  fun main() {
    var c = C(R);
    writeln(c.bar());
  }
}
