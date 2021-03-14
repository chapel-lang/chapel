proc foo(i : int, j : int, k : int) {
  writeln("foo of ", i, ", ", j, ", ", k);
}

foo(i = 1, j = 2, 3);
foo(1, i = 2, j = 3);
foo(i = 1, 2, j = 3);

foo(i = 1, k = 2, 3);
foo(1, i = 2, k = 3);
foo(i = 1, 2, k = 3);

foo(j = 1, k = 2, 3);
foo(1, j = 2, k = 3);
foo(j = 1, 2, k = 3);

foo(j = 1, i = 2, 3);
foo(1, j = 2, i = 3);
foo(j = 1, 2, i = 3);

foo(k = 1, i = 2, 3);
foo(1, k = 2, i = 3);
foo(k = 1, 2, i = 3);

foo(k = 1, j = 2, 3);
foo(1, k = 2, j = 3);
foo(k = 1, 2, j = 3);
