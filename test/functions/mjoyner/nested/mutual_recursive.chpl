def bar() {
  var x : int = 12;
  var y : int = 15;
  var z : int = 18;
  def foo() {
    writeln(x);
    if (x > 0) {
      goo();
    }
  }
  def goo() {
    writeln(y);
    if (y > 0) {
      fie();
    }
  }
  def fie() {
    writeln(z);
    if (z < 0) {
      foo();
    }
  } 
  foo();
}
bar();
