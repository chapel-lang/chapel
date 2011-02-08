proc foo(i : int = 1, j : int = 2, k : int = 3) {
  writeln("foo of ", i, ", ", j, ", ", k);
}

foo(i = 4, j = 5);
foo(i = 4, k = 5);
foo(j = 4, i = 5);
foo(j = 4, k = 5);
foo(k = 4, i = 5);
foo(k = 4, j = 5);
