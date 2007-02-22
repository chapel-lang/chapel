class C {
  class D {
    var x: int = 0;
    def bar() {
      writeln(d);
    }
  }
  var d: D = D();
  def foo() {
    writeln(d.x);
    d.bar();
  }
}

var c = C();
c.foo();
