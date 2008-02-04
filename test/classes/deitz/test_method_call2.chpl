def foo(i : int) {
  writeln("fun foo called with i = ", i);
}

class bar {
  def foo() {
    writeln("method foo called");
  }
  def goo() {
    foo(2);
  }
}

var b = new bar();

foo(2);
b.foo();
b.goo();
