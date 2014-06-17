class C {
  var x : int;
  proc foo() {
    writeln("foo of ", x);
  }
  proc bar() {
    writeln("bar of ", x);
    this.foo();
  }
}

var c = new C();
c.foo();
c.bar();
delete c;
