proc foo(i : int = 1, j : int = 2, k : int = 3) {
  writeln("foo of ", i, ", ", j, ", ", k);
}

foo(5, i = 4);
foo(5, j = 4);
foo(5, k = 4);
