class C {
  var x : integer = 2;
  function foo(y : integer = 4) {
    writeln(y, " and ", x);
  }
}

var c = C();
writeln(c);
c.foo(3);
c.foo();
