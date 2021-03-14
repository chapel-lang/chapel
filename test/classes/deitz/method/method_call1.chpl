proc foo() {
  writeln("function foo called");
}

class bar {
  proc foo() {
    writeln("method foo called");
  }
  proc goo() {
    foo();
  }
}

var b = new borrowed bar();

foo();
b.foo();
b.goo();
