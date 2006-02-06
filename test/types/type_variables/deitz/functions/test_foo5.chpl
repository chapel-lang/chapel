function foo(type t, x : t, y : t) {
  if x == y then
    writeln("foo of ", x, " and itself");
  else
    writeln("foo of ", x, " and ", y);
}

function foo(type t, x : t, y : float) {
  writeln("foo (2) of ", x, " and ", y);
}

foo(integer, 2, 4);
foo(integer, 3, 3);
foo(integer, 2, 4.0);
foo(integer, 3, 3.0);
