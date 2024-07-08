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

var ownB = new owned bar();
var b = ownB.borrow();

foo();
b.foo();
b.goo();
