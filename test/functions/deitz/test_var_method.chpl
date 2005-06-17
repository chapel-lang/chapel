class C {
  var i : integer;
  function foo() var {
    return i;
  }
}

var c : C = C();
c.i = 2;
writeln(c);
writeln(c.foo());
c.foo() = 3;
writeln(c);
