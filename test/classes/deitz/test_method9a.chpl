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

var c = C();
c.foo();
c.bar();
