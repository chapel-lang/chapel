proc foo(i : int) {
  writeln("fun foo called with i = ", i);
}

class bar {
  proc foo() {
    writeln("method foo called");
  }
  proc goo() {
    foo(2);
  }
}

var b = new borrowed bar();

foo(2);
b.foo();
b.goo();
