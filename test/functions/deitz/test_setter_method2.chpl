class C {
  var x: int;
  def foo() var {
    writeln(setter);
    return x;
  }
}

class D {
  var c = new C();
  def bar() {
    c.foo() = 2;
    writeln(c.foo());
  }
}

var d = new D();
d.bar();
