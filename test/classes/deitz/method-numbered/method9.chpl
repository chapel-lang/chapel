class C {
  var x : int;

  proc foo() {
    writeln("foo of ", x);
  }

  proc bar() {
    writeln("bar of ", x);
    this.foo();
  }

  proc baz() {
    writeln("baz of ", x);
    foo();
  }
}

var c = new borrowed C();

c.foo();
c.bar();
c.baz();
