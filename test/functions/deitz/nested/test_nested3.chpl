def foo(i : int) {
  writeln("foo of ", i);
  def bar(i : int) {
    writeln("bar of ", i);
  }
  def car(i : int) {
    writeln("car of ", i);
  }
  bar(i);
  car(i);
}

foo(5);
