function foo(i : integer = 1, j : integer = 2, k : integer = 3) {
  writeln("foo of ", i, ", ", j, ", ", k);
}

foo(5, i = 4);
foo(5, j = 4);
foo(5, k = 4);
