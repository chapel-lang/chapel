class C {
  var x : int;
  def foo() {
    writeln("foo of ", x);
  }
  def bar() {
    writeln("bar of ", x);
    foo();
  }
}

var c = new C();
c.foo();
c.bar();
