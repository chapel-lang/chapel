proc foo(type t, param p: int) {
  var x: t;
  writeln(x, p);
}

foo(int, 1);
foo(real, 2);
foo(int, 2);
foo((int, int), 3);

