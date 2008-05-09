class B {
  def bar() { writeln("right bar"); }
}

class C {
  class D: B {
    def foo() { bar(); }
  }
  def bar() { writeln("wrong bar"); }
  var d = new D();
}

var c = new C();
c.d.foo();

