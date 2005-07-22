class C {
  var x : integer;
  function foo() {
    writeln("foo of ", x);
  }
  function bar() {
    writeln("bar of ", x);
    foo();
  }
}

var c = C();
c.foo();
c.bar();
