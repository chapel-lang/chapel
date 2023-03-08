proc foo(x : int = 4) : real do return x / 2.0;

proc foo(x : real) {
  writeln(x);
}

foo(foo(16));

foo(foo());
