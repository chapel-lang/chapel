def bar() {
  var x : int = 12;
  var y : int = 15;
  def foo() {
    writeln(x);
    if (x > 0) {
      goo();
    }
  }
  def goo() {
    writeln(y);
    if (y < 0) {
      foo();
    }
  }
  foo();
}
bar();
