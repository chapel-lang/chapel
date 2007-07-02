def foo() {
  var x, y: int;

  for i in bar() {
    x = 1;
    i = 2;
    y = x;
  }
  writeln(y);

  def bar() var {
    yield x;
  }
}

foo();
