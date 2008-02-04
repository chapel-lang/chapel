class C {
  class D {
    var x = 0;
    def bar() {
      writeln(d);
    }
  }
  var d = new D();
  def foo() {
    writeln(d.x);
    d.bar();
  }
}

var c = new C();
c.foo();
