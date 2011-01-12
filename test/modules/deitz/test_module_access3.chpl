def foo(r: real) { writeln(r); }

module M {
  def foo(i: int) { writeln(i); }
}

def bar() {
  M.foo(3);
}
bar();
foo(3);
