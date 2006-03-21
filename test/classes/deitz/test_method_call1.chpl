fun foo() {
  writeln("function foo called");
}

class bar {
  fun foo() {
    writeln("method foo called");
  }
  fun goo() {
    foo();
  }
}

var b = bar();

foo();
b.foo();
b.goo();
