module M1 {
  class C {
    type t;
    var y : t;
    function bar()
      return foo(y);
  }
}

module M2 {
  use M1;

  record R {
    var x : integer = 3;
  }

  function foo(r : R)
    return r.x * 2;

  function main() {
    var c = C(R);
    writeln(c.bar());
  }
}
