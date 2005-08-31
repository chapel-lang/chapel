function foo(i : integer) {
  writeln("function foo called with i = ", i);
}

class bar {
  function foo() {
    writeln("method foo called");
  }
  function goo() {
    foo(2);
  }
}

var b = bar();

foo(2);
b.foo();
b.goo();
