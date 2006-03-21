class C {
  var x : int = 2;
  function foo(y : int = 4, z : int = 6) {
    writeln(z, " and ", y, " and ", x);
  }
}

var c = C();
writeln(c);
c.foo(3, 5);
c.foo(3);
c.foo();
