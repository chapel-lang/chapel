proc foo(i : int = 1, j : int = 2, k : int = 3) {
  writeln("foo of ", i, ", ", j, ", ", k);
}

foo(i = 4, 5);
foo(j = 4, 5);
foo(k = 4, 5);
