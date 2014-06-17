proc foo(i : int = 1, j : int = 2) {
  writeln("foo of ", i, " and ", j);
}

foo(j=3);
foo(4, 5);
