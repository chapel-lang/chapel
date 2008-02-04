class C {
  var x;
  def foo() var {
    writeln(setter);
    return x;
  }
}

class D {
  var c = new C(1);
  def bar() {
    c.foo() = 2;
    writeln(c.foo());
  }
}

var d = new D();
d.bar();
