function foo(i : integer) : float {
  return i * 3.0;
}

function bar() {
  writeln(foo(1));
  writeln(foo(2));
  writeln(foo(3));

  var foo : [1..3] float;

  foo(1) = 4.0;
  foo(2) = 2.0;
  foo(3) = 9.0;

  writeln(foo(1));
  writeln(foo(2));
  writeln(foo(3));
}

bar();
