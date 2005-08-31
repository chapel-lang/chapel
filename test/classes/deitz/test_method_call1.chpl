function foo() {
  writeln("function foo called");
}

class bar {
  function foo() {
    writeln("method foo called");
  }
  function goo() {
    foo();
  }
}

var b = bar();

foo();
b.foo();
b.goo();
