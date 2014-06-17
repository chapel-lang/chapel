proc foo(type t, x : t, y : t) {
  if x == y then
    writeln("foo of ", x, " and itself");
  else
    writeln("foo of ", x, " and ", y);
}

foo(int, 2, 4);
foo(int, 3, 3);
foo(real, 2.0, 4.0);
foo(real, 3.0, 3.0);
foo(string, "two", "four");
foo(string, "three", "three");
