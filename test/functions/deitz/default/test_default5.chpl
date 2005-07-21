function foo(x : integer = 4) return x / 2.0;

function foo(x : float) {
  writeln(x);
}

foo(foo(16));

foo(foo());
