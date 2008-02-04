class C {
  class D {
    var x: int = 0;
    def bar() {
      writeln(d);
    }
  }
  var d: D = new D();
  def foo() {
    writeln(d.x);
    d.bar();
  }
}

var c = new C();
c.foo();
