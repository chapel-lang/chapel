def foo(r: real) { writeln(r); }

module M {
  def foo(i: int) { writeln(i); }
}

def bar() {
  use M;
  M.foo(3);
}
bar();
foo(3);
