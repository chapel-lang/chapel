proc foo(i : int, j : real, k : string) {
  writeln("foo of ", i, ", ", j, ", ", k);
}

foo(i = 1, j = 2.0, k = "three");
foo(i = 1, k = "two", j = 3.0);
foo(j = 1.0, i = 2, k = "three");
foo(j = 1.0, k = "two", i = 3);
foo(k = "one", i = 2, j = 3.0);
foo(k = "one", j = 2.0, i = 3);
