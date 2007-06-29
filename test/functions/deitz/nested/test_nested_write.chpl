def foo() {
  var x, y: int;

  x = 1;
  bar();
  y = x;
  writeln(y);

  def bar() {
    x = 2;
  }
}

foo();
