class C {
  var x : integer = 2;
  function foo(y : integer = 4, z : integer = 6) {
    writeln(z, " and ", y, " and ", x);
  }
}

var c = C();
writeln(c);
c.foo(3, 5);
c.foo(3);
c.foo();
