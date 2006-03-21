fun foo(i : int) {
  writeln("fun foo called with i = ", i);
}

class bar {
  fun foo() {
    writeln("method foo called");
  }
  fun goo() {
    foo(2);
  }
}

var b = bar();

foo(2);
b.foo();
b.goo();
