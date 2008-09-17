class C {
  def foo() {
    writeln("C.foo()");
  }
}

class D:C {
  def foo() {
    writeln("D.foo()");
  }
}

var c: C;
var d: D;

c.foo();
