def foo(r: real) { writeln(r); }

module M {
  def foo(i: int) { writeln(i); }
}

M.foo(3.0);
foo(3.0);
