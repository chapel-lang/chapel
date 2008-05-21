class D {
  var n: int;
  class C {
    var a: int;
    def foo(i: int) {
      n = i;
    }
  }
  def make_a_C(i: int = 0) {
    return new C(i);
  }
}

def main {
  var d = new D(1);
  var c = d.make_a_C();

  writeln(d);
  c.outer.n = 2;
  writeln(d);
  c.foo(3);
  writeln(d);
}
