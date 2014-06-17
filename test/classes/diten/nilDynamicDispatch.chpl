class C {
  proc foo() {
    writeln("C.foo()");
  }
}

class D:C {
  proc foo() {
    writeln("D.foo()");
  }
}

var c: C;
var d: D;

c.foo();
