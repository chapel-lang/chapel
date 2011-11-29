proc foo(i : int, j : int, k : int) {
  writeln("foo1 of ", i, ", ", j, ", ", k);
}

proc foo(l : int, m : int, n : int) {
  writeln("foo2 of ", l, ", ", m, ", ", n);
}

foo(i = 1, j = 2, k = 3);
foo(i = 1, k = 2, j = 3);
foo(j = 1, i = 2, k = 3);
foo(j = 1, k = 2, i = 3);
foo(k = 1, i = 2, j = 3);
foo(k = 1, j = 2, i = 3);

foo(l = 1, m = 2, n = 3);
foo(l = 1, n = 2, m = 3);
foo(m = 1, l = 2, n = 3);
foo(m = 1, n = 2, l = 3);
foo(n = 1, l = 2, m = 3);
foo(n = 1, m = 2, l = 3);

foo(i = 1, m = 2, n = 3);
