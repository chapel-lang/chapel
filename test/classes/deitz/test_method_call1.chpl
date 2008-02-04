def foo() {
  writeln("function foo called");
}

class bar {
  def foo() {
    writeln("method foo called");
  }
  def goo() {
    foo();
  }
}

var b = new bar();

foo();
b.foo();
b.goo();
