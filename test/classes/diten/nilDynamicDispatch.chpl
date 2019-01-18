class C {
  proc foo() {
    writeln("C.foo()");
  }
}

class D:C {
  override proc foo() {
    writeln("D.foo()");
  }
}

var c: borrowed C;
var d: borrowed D;

c.foo();
