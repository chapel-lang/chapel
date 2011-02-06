class D {
  var n: int;
  class C {
    var a: int;
    proc foo(i: int) {
      n = i;
    }
  }
  proc make_a_C(i: int = 0) {
    return new C(i);
  }
}

proc main {
  var d = new D(1);
  var c = d.make_a_C();

  writeln(d);
  c.outer.n = 2;
  writeln(d);
  c.foo(3);
  writeln(d);

  delete d;
  delete c;
}
