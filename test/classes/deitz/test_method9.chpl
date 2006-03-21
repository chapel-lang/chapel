class C {
  var x : int;
  fun foo() {
    writeln("foo of ", x);
  }
  fun bar() {
    writeln("bar of ", x);
    this.foo();
  }
}

var c = C();
c.foo();
c.bar();
