class C {
  var x : int;
  def foo() {
    writeln("foo of ", x);
  }
  def bar() {
    writeln("bar of ", x);
    this.foo();
  }
}

var c = new C();
c.foo();
c.bar();
