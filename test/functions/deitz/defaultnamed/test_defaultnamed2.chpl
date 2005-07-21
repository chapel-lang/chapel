function foo(i : integer = 1, j : integer = 2, k : integer = 3) {
  writeln("foo of ", i, ", ", j, ", ", k);
}

foo(i = 4, 5);
foo(j = 4, 5);
foo(k = 4, 5);
