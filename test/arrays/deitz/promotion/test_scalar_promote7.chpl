class C {
  def foo(i : int, j : int)
    return i**2 + j**2;
}

var c = C();

writeln(c.foo(_seq(1, 2, 3), _seq(4, 5, 6)));
